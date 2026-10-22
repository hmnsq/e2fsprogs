/*
 * fallocate.c -- Allocate large chunks of file.
 *
 * Copyright (C) 2014 Oracle.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"

#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  करोहि {printf(f, ## a); fflush(stdout); } याव (0)
#अञ्ञथा
# define dbg_printf(f, a...)
#endif

/*
 * Extent-based fallocate code.
 *
 * Find runs of unmapped logical blocks by starting at start and walking the
 * extents until we reach the end of the range we want.
 *
 * For each run of unmapped blocks, try to find the extents on either side of
 * the range.  If there's a left extent that can grow by at least a cluster and
 * there are lblocks between start and the next lcluster after start, see if
 * there's an implied cluster allocation; if so, zero the blocks (if the left
 * extent is initialized) and adjust the extent.  Ditto for the blocks between
 * the end of the last full lcluster and end, if there's a right extent.
 *
 * Try to attach as much as we can to the left extent, then try to attach as
 * much as we can to the right extent.  For the remainder, try to allocate the
 * whole range; map in whatever we get; and repeat until we're done.
 *
 * To attach to a left extent, figure out the maximum amount we can add to the
 * extent and try to allocate that much, and append if successful.  To attach
 * to a right extent, figure out the max we can add to the extent, try to
 * allocate that much, and prepend if successful.
 *
 * We need an alloc_range function that tells us how much we can allocate given
 * a maximum length and one of a suggested start, a fixed start, or a fixed end
 * point.
 *
 * Every time we modify the extent tree we also need to update the block stats.
 *
 * At the end, update i_blocks and i_size appropriately.
 */

स्थिर शून्य dbg_print_extent(ध्रुव वर्ण *desc EXT2FS_ATTR((unused)),
		ध्रुव संरचना ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	सचे (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	सचे (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	सचे (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	सचे (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	सचे (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#endif
}

स्थिर errcode_t claim_range(ext2_filsys fs, संरचना ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	पटिनिवत्त ext2fs_iblk_add_blocks(fs, inode, clusters);
}

स्थिर errcode_t ext_falloc_helper(ext2_filsys fs,
				   पूर्णाङ्क flags,
				   ext2_ino_t ino,
				   संरचना ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   संरचना ext2fs_extent *वामः_ext,
				   संरचना ext2fs_extent *दक्षिणः_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	संरचना ext2fs_extent	newex, ex;
	पूर्णाङ्क			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	printf("%s: ", __func__);
	सचे (वामः_ext)
		printf("left_ext=%llu--%llu, ", वामः_ext->e_lblk,
		       वामः_ext->e_lblk + वामः_ext->e_len - 1);
	सचे (दक्षिणः_ext)
		printf("right_ext=%llu--%llu, ", दक्षिणः_ext->e_lblk,
		       दक्षिणः_ext->e_lblk + दक्षिणः_ext->e_len - 1);
	printf("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#endif
	/* Can't create initialized extents past EOF? */
	सचे (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_SIZE(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	सचे (वामः_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		सचे (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - वामः_ext->e_len;
		अञ्ञथा
			fillable = max_init_len - वामः_ext->e_len;

		सचे (fillable > range_len)
			fillable = range_len;
		सचे (fillable == 0)
			गच्छ expand_दक्षिणः;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		सचे (cluster_fill == 0)
			गच्छ expand_दक्षिणः;

		सचे (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		सचे (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = वामः_ext->e_lblk + वामः_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			सचे (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			सचे (cluster_fill == 0)
				गच्छ expand_दक्षिणः;
		}

		err = ext2fs_extent_goto(handle, वामः_ext->e_lblk);
		सचे (err)
			गच्छ expand_दक्षिणः;
		वामः_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", वामः_ext);
		err = ext2fs_extent_replace(handle, 0, वामः_ext);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_fix_parents(handle);
		सचे (err)
			गच्छ out;

		/* Zero blocks */
		सचे (!(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, वामः_ext->e_pblk +
						  वामः_ext->e_len -
						  cluster_fill, cluster_fill,
						  रिक्त, रिक्त);
			सचे (err)
				गच्छ out;
		}
	}

expand_दक्षिणः:
	/* We must lengthen the right extent to the beginning of the cluster */
	सचे (दक्षिणः_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		सचे (दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - दक्षिणः_ext->e_len;
		अञ्ञथा
			fillable = max_init_len - दक्षिणः_ext->e_len;

		सचे (fillable > range_len)
			fillable = range_len;
		सचे (fillable == 0)
			गच्छ try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = दक्षिणः_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		सचे (cluster_fill == 0)
			गच्छ try_merge;

		err = ext2fs_extent_goto(handle, दक्षिणः_ext->e_lblk);
		सचे (err)
			गच्छ out;

		सचे (cluster_fill > fillable)
			cluster_fill = fillable;
		दक्षिणः_ext->e_lblk -= cluster_fill;
		दक्षिणः_ext->e_pblk -= cluster_fill;
		दक्षिणः_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", दक्षिणः_ext);
		err = ext2fs_extent_replace(handle, 0, दक्षिणः_ext);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_fix_parents(handle);
		सचे (err)
			गच्छ out;

		/* Zero blocks if necessary */
		सचे (!(दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, दक्षिणः_ext->e_pblk,
						  cluster_fill, रिक्त, रिक्त);
			सचे (err)
				गच्छ out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	सचे (वामः_ext && दक्षिणः_ext) {
		/* Are the two extents mergeable? */
		सचे ((वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			गच्छ try_वामः;

		/* User requires init/uninit but extent is uninit/init. */
		सचे (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			गच्छ try_वामः;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		सचे (!(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			गच्छ try_वामः;

		/* Will it even fit? */
		x = वामः_ext->e_len + range_len + दक्षिणः_ext->e_len;
		सचे (x > (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			गच्छ try_वामः;

		/* Would they even be physically contiguous if merged? */
		सचे (वामः_ext->e_pblk + वामः_ext->e_len + range_len !=
				दक्षिणः_ext->e_pblk)
			गच्छ try_वामः;

		err = ext2fs_extent_goto(handle, वामः_ext->e_lblk);
		सचे (err)
			गच्छ try_वामः;

		/* Allocate blocks */
		y = वामः_ext->e_pblk + वामः_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_LENGTH, y,
				       दक्षिणः_ext->e_pblk - y + 1, रिक्त,
				       &pblk, &plen);
		सचे (err)
			गच्छ try_वामः;
		सचे (pblk + plen != दक्षिणः_ext->e_pblk)
			गच्छ try_वामः;
		err = claim_range(fs, inode, pblk, plen);
		सचे (err)
			गच्छ out;

		/* Modify extents */
		वामः_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", वामः_ext);
		err = ext2fs_extent_replace(handle, 0, वामः_ext);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_fix_parents(handle);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &newex);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_delete(handle, 0);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_fix_parents(handle);
		सचे (err)
			गच्छ out;
		*दक्षिणः_ext = *वामः_ext;

		/* Zero blocks */
		सचे (!(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  रिक्त, रिक्त);
			सचे (err)
				गच्छ out;
		}

		पटिनिवत्त 0;
	}

try_वामः:
	/* Extend the left extent */
	सचे (वामः_ext) {
		/* How many more blocks can be attached to left_ext? */
		सचे (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - वामः_ext->e_len;
		अञ्ञथा सचे (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - वामः_ext->e_len;
		अञ्ञथा
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		सचे (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			गच्छ try_दक्षिणः;

		सचे (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = वामः_ext->e_lblk + वामः_ext->e_len - 1;
		सचे (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			सचे (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		सचे (fillable == 0)
			गच्छ try_दक्षिणः;

		/* Test if the right edge of the range is already mapped? */
		सचे (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			सचे (err)
				गच्छ out;
			सचे (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			सचे (fillable == 0)
				गच्छ try_दक्षिणः;
		}

		/* Allocate range of blocks */
		x = वामः_ext->e_pblk + वामः_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				x, fillable, रिक्त, &pblk, &plen);
		सचे (err)
			गच्छ try_दक्षिणः;
		err = claim_range(fs, inode, pblk, plen);
		सचे (err)
			गच्छ out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, वामः_ext->e_lblk);
		सचे (err)
			गच्छ out;
		range_start += plen;
		range_len -= plen;
		वामः_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", वामः_ext);
		err = ext2fs_extent_replace(handle, 0, वामः_ext);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_fix_parents(handle);
		सचे (err)
			गच्छ out;

		/* Zero blocks if necessary */
		सचे (!(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, रिक्त, रिक्त);
			सचे (err)
				गच्छ out;
		}
	}

try_दक्षिणः:
	/* Extend the right extent */
	सचे (दक्षिणः_ext) {
		/* How much can we attach to right_ext? */
		सचे (दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - दक्षिणः_ext->e_len;
		अञ्ञथा सचे (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - दक्षिणः_ext->e_len;
		अञ्ञथा
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		सचे (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			गच्छ try_anywhere;

		सचे (fillable > range_len)
			fillable = range_len;
		सचे (fillable == 0)
			गच्छ try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		सचे (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					दक्षिणः_ext->e_lblk - fillable, &pblk);
			सचे (err)
				गच्छ out;
			सचे (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((दक्षिणः_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			सचे (fillable == 0)
				गच्छ try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				दक्षिणः_ext->e_pblk - fillable,
				fillable, रिक्त, &pblk, &plen);
		सचे (err)
			गच्छ try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		सचे (err)
			गच्छ out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, दक्षिणः_ext->e_lblk);
		सचे (err)
			गच्छ out;
		range_len -= plen;
		दक्षिणः_ext->e_lblk -= plen;
		दक्षिणः_ext->e_pblk -= plen;
		दक्षिणः_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", दक्षिणः_ext);
		err = ext2fs_extent_replace(handle, 0, दक्षिणः_ext);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_fix_parents(handle);
		सचे (err)
			गच्छ out;

		/* Zero blocks if necessary */
		सचे (!(दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, रिक्त, रिक्त);
			सचे (err)
				गच्छ out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	सचे (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		सचे (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		सचे (err)
			गच्छ out;
		सचे (pblk == 0)
			गच्छ try_दक्षिणः_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		सचे (err == EXT2_ET_NO_CURRENT_ग्रन्थिः)
			ex.e_lblk = 0;
		अञ्ञथा सचे (err)
			गच्छ out;

		सचे (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अञ्ञथा
			op = EXT2_EXTENT_निवेशय_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_निवेशय(handle, op, &newex);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_fix_parents(handle);
		सचे (err)
			गच्छ out;

		सचे (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, रिक्त, रिक्त);
			सचे (err)
				गच्छ out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_दक्षिणः_implied:
	y = range_start + range_len;
	सचे (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		सचे (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		सचे (err)
			गच्छ out;
		सचे (pblk == 0)
			गच्छ no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		सचे (err == EXT2_ET_NO_CURRENT_ग्रन्थिः)
			ex.e_lblk = 0;
		अञ्ञथा सचे (err)
			गच्छ out;

		सचे (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अञ्ञथा
			op = EXT2_EXTENT_निवेशय_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_निवेशय(handle, op, &newex);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_fix_parents(handle);
		सचे (err)
			गच्छ out;

		सचे (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, रिक्त, रिक्त);
			सचे (err)
				गच्छ out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	सचे (range_len == 0)
		पटिनिवत्त 0;

	newex.e_lblk = range_start;
	सचे (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} अञ्ञथा {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	अत्थाय (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       रिक्त, &pblk, &plen);
		सचे (err)
			गच्छ out;
		err = claim_range(fs, inode, pblk, plen);
		सचे (err)
			गच्छ out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		सचे (err == EXT2_ET_NO_CURRENT_ग्रन्थिः)
			ex.e_lblk = 0;
		अञ्ञथा सचे (err)
			गच्छ out;

		सचे (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अञ्ञथा
			op = EXT2_EXTENT_निवेशय_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_निवेशय(handle, op, &newex);
		सचे (err)
			गच्छ out;
		err = ext2fs_extent_fix_parents(handle);
		सचे (err)
			गच्छ out;

		सचे (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, रिक्त, रिक्त);
			सचे (err)
				गच्छ out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		सचे (pblk >= ext2fs_blocks_गणनम्(fs->super))
			pblk = fs->super->s_first_data_block;
	}

out:
	पटिनिवत्त err;
}

स्थिर errcode_t extent_fallocate(ext2_filsys fs, पूर्णाङ्क flags, ext2_ino_t ino,
				      संरचना ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	संरचना ext2fs_extent	वामः_extent, दक्षिणः_extent;
	संरचना ext2fs_extent	*वामः_adjacent, *दक्षिणः_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, next;
	blk64_t			गणनम्, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	सचे (err)
		पटिनिवत्त err;

	/*
	 * Find the extent closest to the start of the alloc range.  We don't
	 * check the return value because _goto() sets the current node to the
	 * next-lowest extent if 'start' is in a hole; or the next-highest
	 * extent if there aren't any lower ones; or doesn't set a current node
	 * if there was a real error reading the extent tree.  In that case,
	 * _get() will error out.
	 */
start_again:
	ext2fs_extent_goto(handle, start);
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &वामः_extent);
	सचे (err == EXT2_ET_NO_CURRENT_ग्रन्थिः) {
		blk64_t max_blocks = ext2fs_blocks_गणनम्(fs->super);

		सचे (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, रिक्त,
					रिक्त, start, len, goal);
		गच्छ errout;
	} अञ्ञथा सचे (err)
		गच्छ errout;

	dbg_print_extent("ext_falloc initial", &वामः_extent);
	next = वामः_extent.e_lblk + वामः_extent.e_len;
	सचे (वामः_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = वामः_extent.e_pblk - (वामः_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, रिक्त,
					&वामः_extent, start,
					min(len, वामः_extent.e_lblk - start),
					goal);
		सचे (err)
			गच्छ errout;

		गच्छ start_again;
	} अञ्ञथा सचे (next >= start) {
		range_start = next;
		वामः_adjacent = &वामः_extent;
	} अञ्ञथा {
		range_start = start;
		वामः_adjacent = रिक्त;
	}
	goal = वामः_extent.e_pblk + (range_start - वामः_extent.e_lblk);

	करोहि {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF,
					   &दक्षिणः_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (पूर्णाङ्क)err);
		dbg_print_extent("ext_falloc next", &दक्षिणः_extent);
		/* Stop if we've seen this extent before */
		सचे (!err && दक्षिणः_extent.e_lblk <= वामः_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_NEXT;

		सचे (err && err != EXT2_ET_EXTENT_NO_NEXT)
			गच्छ errout;
		सचे (err == EXT2_ET_EXTENT_NO_NEXT ||
		    दक्षिणः_extent.e_lblk > end + 1) {
			range_end = end;
			दक्षिणः_adjacent = रिक्त;
		} अञ्ञथा {
			/* Handle right_extent.e_lblk <= end */
			range_end = दक्षिणः_extent.e_lblk - 1;
			दक्षिणः_adjacent = &दक्षिणः_extent;
		}
		goal_distance = range_start - next;
		सचे (err != EXT2_ET_EXTENT_NO_NEXT &&
		    goal_distance > (range_end - दक्षिणः_extent.e_lblk))
			goal = दक्षिणः_extent.e_pblk -
					(दक्षिणः_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		सचे (range_start <= range_end) {
			गणनम् = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						वामः_adjacent, दक्षिणः_adjacent,
						range_start, गणनम्, goal);
			सचे (err)
				गच्छ errout;
		}

		सचे (range_end == end)
			भङ्ग;

		err = ext2fs_extent_goto(handle, दक्षिणः_extent.e_lblk);
		सचे (err)
			गच्छ errout;
		next = दक्षिणः_extent.e_lblk + दक्षिणः_extent.e_len;
		वामः_extent = दक्षिणः_extent;
		वामः_adjacent = &वामः_extent;
		range_start = next;
		goal = वामः_extent.e_pblk + (range_start - वामः_extent.e_lblk);
	} याव (range_end < end);

errout:
	ext2fs_extent_free(handle);
	पटिनिवत्त err;
}

/*
 * Map physical blocks to a range of logical blocks within a file.  The range
 * of logical blocks are (start, start + len).  If there are already extents,
 * the mappings will try to extend the mappings; otherwise, it will try to map
 * start as if logical block 0 points to goal.  If goal is ~0ULL, then the goal
 * is calculated based on the inode group.
 *
 * Flags:
 * - EXT2_FALLOCATE_ZERO_BLOCKS: Zero the blocks that are allocated.
 * - EXT2_FALLOCATE_FORCE_INIT: Create only initialized extents.
 * - EXT2_FALLOCATE_FORCE_UNINIT: Create only uninitialized extents.
 * - EXT2_FALLOCATE_INIT_BEYOND_EOF: Create extents beyond EOF.
 *
 * If neither FORCE_INIT nor FORCE_UNINIT are specified, this function will
 * try to expand any extents it finds, zeroing blocks as necessary.
 */
errcode_t ext2fs_fallocate(ext2_filsys fs, पूर्णाङ्क flags, ext2_ino_t ino,
			   संरचना ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	संरचना ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	पूर्णाङ्क			zero_len = 0;
	errcode_t		err;

	सचे (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	सचे (len > ext2fs_blocks_गणनम्(fs->super))
		पटिनिवत्त EXT2_ET_BLOCK_ALLOC_FAIL;
	अञ्ञथा सचे (len == 0)
		पटिनिवत्त 0;

	/* Read inode structure if necessary */
	सचे (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		सचे (err)
			पटिनिवत्त err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	सचे (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		गच्छ out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	अत्थाय (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, रिक्त, 0, blk, 0, &x);
		सचे (err)
			पटिनिवत्त err;
		सचे (x)
			अनुवर्त;

		err = ext2fs_bmap2(fs, ino, inode, रिक्त, BMAP_ALLOC,
				   blk, 0, &x);
		सचे (err)
			गच्छ errout;
		सचे ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  रिक्त, रिक्त);
			zero_len = 0;
			सचे (err)
				गच्छ errout;
		}
		सचे (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} अञ्ञथा {
			zero_len++;
		}
		last = x;
	}

out:
	सचे (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	सचे (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, रिक्त, रिक्त);
	पटिनिवत्त err;
}
