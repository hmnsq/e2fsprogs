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

#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#परिभाषय min(a, b) ((a) < (b) ? (a) : (b))

#अपरिभाषय DEBUG

#यदि_परिभाषितम् DEBUG
# define dbg_printf(f, a...)  कुरु {प्रारूप_लिख(f, ## a); fflush(stdout); } यावत् (0)
#अन्यथा
# define dbg_printf(f, a...)
#यदि_अन्तः

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

स्थिर शून्य dbg_print_extent(ध्रुव वर्ण *desc EXT2FS_ATTR((अप्रयुक्तम्)),
		ध्रुव संरचना ext2fs_extent *extent EXT2FS_ATTR((अप्रयुक्तम्)))
{
#यदि_परिभाषितम् DEBUG
	यदि (desc)
		प्रारूप_लिख("%s: ", desc);
	प्रारूप_लिख("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_पर्णम्)
		fputs("LEAF ", stdout);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_दर्शय)
		fputs("2ND_VISIT ", stdout);
	यदि (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#यदि_अन्तः
}

स्थिर errcode_t claim_range(ext2_filsys fs, संरचना ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2_FS_CLUSTER_अनुपातः(fs) - 1) /
		   EXT2_FS_CLUSTER_अनुपातः(fs);
	ext2fs_खण्डम्_alloc_stats_range(fs, blk,
			clusters * EXT2_FS_CLUSTER_अनुपातः(fs), +1);
	प्रत्यागम ext2fs_iblk_सङ्कलय_खण्डानि(fs, inode, clusters);
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

#यदि_परिभाषितम् DEBUG
	प्रारूप_लिख("%s: ", __func__);
	यदि (वामः_ext)
		प्रारूप_लिख("left_ext=%llu--%llu, ", वामः_ext->e_lblk,
		       वामः_ext->e_lblk + वामः_ext->e_len - 1);
	यदि (दक्षिणः_ext)
		प्रारूप_लिख("right_ext=%llu--%llu, ", दक्षिणः_ext->e_lblk,
		       दक्षिणः_ext->e_lblk + दक्षिणः_ext->e_len - 1);
	प्रारूप_लिख("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#यदि_अन्तः
	/* Can't create initialized extents past EOF? */
	यदि (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_आकार(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2_FS_CLUSTER_आवरणम्(fs)) |
		     (range_start & EXT2_FS_CLUSTER_आवरणम्(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2_FS_CLUSTER_आवरणम्(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2_FS_CLUSTER_आवरणम्(fs);

	/* We must lengthen the left extent to the end of the cluster */
	यदि (वामः_ext && EXT2_FS_CLUSTER_अनुपातः(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		यदि (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - वामः_ext->e_len;
		अन्यथा
			fillable = max_init_len - वामः_ext->e_len;

		यदि (fillable > range_len)
			fillable = range_len;
		यदि (fillable == 0)
			गच्छ expand_दक्षिणः;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2_FS_CLUSTER_अनुपातः(fs) -
			       (range_start & EXT2_FS_CLUSTER_आवरणम्(fs));
		cluster_fill &= EXT2_FS_CLUSTER_आवरणम्(fs);
		यदि (cluster_fill == 0)
			गच्छ expand_दक्षिणः;

		यदि (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		यदि (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = वामः_ext->e_lblk + वामः_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			यदि (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			यदि (cluster_fill == 0)
				गच्छ expand_दक्षिणः;
		}

		err = ext2fs_extent_goto(handle, वामः_ext->e_lblk);
		यदि (err)
			गच्छ expand_दक्षिणः;
		वामः_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", वामः_ext);
		err = ext2fs_extent_replace(handle, 0, वामः_ext);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_fix_parents(handle);
		यदि (err)
			गच्छ बहिर्गमनम्;

		/* Zero blocks */
		यदि (!(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, वामः_ext->e_pblk +
						  वामः_ext->e_len -
						  cluster_fill, cluster_fill,
						  रिक्त, रिक्त);
			यदि (err)
				गच्छ बहिर्गमनम्;
		}
	}

expand_दक्षिणः:
	/* We must lengthen the right extent to the beginning of the cluster */
	यदि (दक्षिणः_ext && EXT2_FS_CLUSTER_अनुपातः(fs) > 1) {
		/* How much can we attach to right_ext? */
		यदि (दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - दक्षिणः_ext->e_len;
		अन्यथा
			fillable = max_init_len - दक्षिणः_ext->e_len;

		यदि (fillable > range_len)
			fillable = range_len;
		यदि (fillable == 0)
			गच्छ try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = दक्षिणः_ext->e_lblk & EXT2_FS_CLUSTER_आवरणम्(fs);
		यदि (cluster_fill == 0)
			गच्छ try_merge;

		err = ext2fs_extent_goto(handle, दक्षिणः_ext->e_lblk);
		यदि (err)
			गच्छ बहिर्गमनम्;

		यदि (cluster_fill > fillable)
			cluster_fill = fillable;
		दक्षिणः_ext->e_lblk -= cluster_fill;
		दक्षिणः_ext->e_pblk -= cluster_fill;
		दक्षिणः_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", दक्षिणः_ext);
		err = ext2fs_extent_replace(handle, 0, दक्षिणः_ext);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_fix_parents(handle);
		यदि (err)
			गच्छ बहिर्गमनम्;

		/* Zero blocks if necessary */
		यदि (!(दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, दक्षिणः_ext->e_pblk,
						  cluster_fill, रिक्त, रिक्त);
			यदि (err)
				गच्छ बहिर्गमनम्;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	यदि (वामः_ext && दक्षिणः_ext) {
		/* Are the two extents mergeable? */
		यदि ((वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			गच्छ try_वामः;

		/* User requires init/uninit but extent is uninit/init. */
		यदि (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			गच्छ try_वामः;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		यदि (!(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_खण्डानि) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			गच्छ try_वामः;

		/* Will it even fit? */
		x = वामः_ext->e_len + range_len + दक्षिणः_ext->e_len;
		यदि (x > (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			गच्छ try_वामः;

		/* Would they even be physically contiguous if merged? */
		यदि (वामः_ext->e_pblk + वामः_ext->e_len + range_len !=
				दक्षिणः_ext->e_pblk)
			गच्छ try_वामः;

		err = ext2fs_extent_goto(handle, वामः_ext->e_lblk);
		यदि (err)
			गच्छ try_वामः;

		/* Allocate blocks */
		y = वामः_ext->e_pblk + वामः_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_दीर्घता, y,
				       दक्षिणः_ext->e_pblk - y + 1, रिक्त,
				       &pblk, &plen);
		यदि (err)
			गच्छ try_वामः;
		यदि (pblk + plen != दक्षिणः_ext->e_pblk)
			गच्छ try_वामः;
		err = claim_range(fs, inode, pblk, plen);
		यदि (err)
			गच्छ बहिर्गमनम्;

		/* Modify extents */
		वामः_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", वामः_ext);
		err = ext2fs_extent_replace(handle, 0, वामः_ext);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_fix_parents(handle);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र_पर्णम्, &newex);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_delete(handle, 0);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_fix_parents(handle);
		यदि (err)
			गच्छ बहिर्गमनम्;
		*दक्षिणः_ext = *वामः_ext;

		/* Zero blocks */
		यदि (!(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_खण्डानि)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  रिक्त, रिक्त);
			यदि (err)
				गच्छ बहिर्गमनम्;
		}

		प्रत्यागम 0;
	}

try_वामः:
	/* Extend the left extent */
	यदि (वामः_ext) {
		/* How many more blocks can be attached to left_ext? */
		यदि (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - वामः_ext->e_len;
		अन्यथा यदि (flags & EXT2_FALLOCATE_ZERO_खण्डानि)
			fillable = max_init_len - वामः_ext->e_len;
		अन्यथा
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		यदि (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			गच्छ try_दक्षिणः;

		यदि (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = वामः_ext->e_lblk + वामः_ext->e_len - 1;
		यदि (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			यदि (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		यदि (fillable == 0)
			गच्छ try_दक्षिणः;

		/* Test if the right edge of the range is already mapped? */
		यदि (EXT2_FS_CLUSTER_अनुपातः(fs) > 1) {
			err = ext2fs_map_cluster_खण्डम्(fs, ino, inode,
					x + fillable, &pblk);
			यदि (err)
				गच्छ बहिर्गमनम्;
			यदि (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2_FS_CLUSTER_आवरणम्(fs));
			यदि (fillable == 0)
				गच्छ try_दक्षिणः;
		}

		/* Allocate range of blocks */
		x = वामः_ext->e_pblk + वामः_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_दीर्घता,
				x, fillable, रिक्त, &pblk, &plen);
		यदि (err)
			गच्छ try_दक्षिणः;
		err = claim_range(fs, inode, pblk, plen);
		यदि (err)
			गच्छ बहिर्गमनम्;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, वामः_ext->e_lblk);
		यदि (err)
			गच्छ बहिर्गमनम्;
		range_start += plen;
		range_len -= plen;
		वामः_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", वामः_ext);
		err = ext2fs_extent_replace(handle, 0, वामः_ext);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_fix_parents(handle);
		यदि (err)
			गच्छ बहिर्गमनम्;

		/* Zero blocks if necessary */
		यदि (!(वामः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_खण्डानि)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, रिक्त, रिक्त);
			यदि (err)
				गच्छ बहिर्गमनम्;
		}
	}

try_दक्षिणः:
	/* Extend the right extent */
	यदि (दक्षिणः_ext) {
		/* How much can we attach to right_ext? */
		यदि (दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - दक्षिणः_ext->e_len;
		अन्यथा यदि (flags & EXT2_FALLOCATE_ZERO_खण्डानि)
			fillable = max_init_len - दक्षिणः_ext->e_len;
		अन्यथा
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		यदि (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			गच्छ try_anywhere;

		यदि (fillable > range_len)
			fillable = range_len;
		यदि (fillable == 0)
			गच्छ try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		यदि (EXT2_FS_CLUSTER_अनुपातः(fs) > 1) {
			err = ext2fs_map_cluster_खण्डम्(fs, ino, inode,
					दक्षिणः_ext->e_lblk - fillable, &pblk);
			यदि (err)
				गच्छ बहिर्गमनम्;
			यदि (pblk)
				fillable -= EXT2_FS_CLUSTER_अनुपातः(fs) -
						((दक्षिणः_ext->e_lblk - fillable)
						 & EXT2_FS_CLUSTER_आवरणम्(fs));
			यदि (fillable == 0)
				गच्छ try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_दीर्घता,
				दक्षिणः_ext->e_pblk - fillable,
				fillable, रिक्त, &pblk, &plen);
		यदि (err)
			गच्छ try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2_FS_CLUSTER_आवरणम्(fs),
			      plen + (pblk & EXT2_FS_CLUSTER_आवरणम्(fs)));
		यदि (err)
			गच्छ बहिर्गमनम्;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, दक्षिणः_ext->e_lblk);
		यदि (err)
			गच्छ बहिर्गमनम्;
		range_len -= plen;
		दक्षिणः_ext->e_lblk -= plen;
		दक्षिणः_ext->e_pblk -= plen;
		दक्षिणः_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", दक्षिणः_ext);
		err = ext2fs_extent_replace(handle, 0, दक्षिणः_ext);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_fix_parents(handle);
		यदि (err)
			गच्छ बहिर्गमनम्;

		/* Zero blocks if necessary */
		यदि (!(दक्षिणः_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_खण्डानि)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, रिक्त, रिक्त);
			यदि (err)
				गच्छ बहिर्गमनम्;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	यदि (range_len > 0 && (range_start & EXT2_FS_CLUSTER_आवरणम्(fs))) {
		cluster_fill = EXT2_FS_CLUSTER_अनुपातः(fs) -
			       (range_start & EXT2_FS_CLUSTER_आवरणम्(fs));
		cluster_fill &= EXT2_FS_CLUSTER_आवरणम्(fs);
		यदि (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_खण्डम्(fs, ino, inode, newex.e_lblk,
					       &pblk);
		यदि (err)
			गच्छ बहिर्गमनम्;
		यदि (pblk == 0)
			गच्छ try_दक्षिणः_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		यदि (err == EXT2_ET_NO_CURRENT_ग्रन्थिः)
			ex.e_lblk = 0;
		अन्यथा यदि (err)
			गच्छ बहिर्गमनम्;

		यदि (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अन्यथा
			op = EXT2_EXTENT_निवेशय_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_निवेशय(handle, op, &newex);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_fix_parents(handle);
		यदि (err)
			गच्छ बहिर्गमनम्;

		यदि (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_खण्डानि)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, रिक्त, रिक्त);
			यदि (err)
				गच्छ बहिर्गमनम्;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_दक्षिणः_implied:
	y = range_start + range_len;
	यदि (range_len > 0 && (y & EXT2_FS_CLUSTER_आवरणम्(fs))) {
		cluster_fill = y & EXT2_FS_CLUSTER_आवरणम्(fs);
		यदि (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2_FS_CLUSTER_आवरणम्(fs);
		err = ext2fs_map_cluster_खण्डम्(fs, ino, inode, newex.e_lblk,
					       &pblk);
		यदि (err)
			गच्छ बहिर्गमनम्;
		यदि (pblk == 0)
			गच्छ no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		यदि (err == EXT2_ET_NO_CURRENT_ग्रन्थिः)
			ex.e_lblk = 0;
		अन्यथा यदि (err)
			गच्छ बहिर्गमनम्;

		यदि (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अन्यथा
			op = EXT2_EXTENT_निवेशय_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_निवेशय(handle, op, &newex);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_fix_parents(handle);
		यदि (err)
			गच्छ बहिर्गमनम्;

		यदि (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_खण्डानि)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, रिक्त, रिक्त);
			यदि (err)
				गच्छ बहिर्गमनम्;
		}

		range_len -= cluster_fill;
	}

no_implied:
	यदि (range_len == 0)
		प्रत्यागम 0;

	newex.e_lblk = range_start;
	यदि (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} अन्यथा {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	प्रति (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2_FS_CLUSTER_आवरणम्(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2_FS_CLUSTER_आवरणम्(fs),
				       fillable,
				       रिक्त, &pblk, &plen);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = claim_range(fs, inode, pblk, plen);
		यदि (err)
			गच्छ बहिर्गमनम्;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		यदि (err == EXT2_ET_NO_CURRENT_ग्रन्थिः)
			ex.e_lblk = 0;
		अन्यथा यदि (err)
			गच्छ बहिर्गमनम्;

		यदि (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अन्यथा
			op = EXT2_EXTENT_निवेशय_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_निवेशय(handle, op, &newex);
		यदि (err)
			गच्छ बहिर्गमनम्;
		err = ext2fs_extent_fix_parents(handle);
		यदि (err)
			गच्छ बहिर्गमनम्;

		यदि (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_खण्डानि)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, रिक्त, रिक्त);
			यदि (err)
				गच्छ बहिर्गमनम्;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		यदि (pblk >= ext2fs_खण्डानि_गणनम्(fs->super))
			pblk = fs->super->s_first_दत्त_खण्डम्;
	}

बहिर्गमनम्:
	प्रत्यागम err;
}

स्थिर errcode_t extent_fallocate(ext2_filsys fs, पूर्णाङ्क flags, ext2_ino_t ino,
				      संरचना ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	संरचना ext2fs_extent	वामः_extent, दक्षिणः_extent;
	संरचना ext2fs_extent	*वामः_adjacent, *दक्षिणः_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_अन्तः = 0, अन्तः, अग्र;
	blk64_t			गणनम्, goal_distance;

	अन्तः = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	यदि (err)
		प्रत्यागम err;

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
	यदि (err == EXT2_ET_NO_CURRENT_ग्रन्थिः) {
		blk64_t max_खण्डानि = ext2fs_खण्डानि_गणनम्(fs->super);

		यदि (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_खण्डम्_bitmap2(fs->खण्डम्_map,
						goal, max_खण्डानि - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, रिक्त,
					रिक्त, start, len, goal);
		गच्छ errout;
	} अन्यथा यदि (err)
		गच्छ errout;

	dbg_print_extent("ext_falloc initial", &वामः_extent);
	अग्र = वामः_extent.e_lblk + वामः_extent.e_len;
	यदि (वामः_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = वामः_extent.e_pblk - (वामः_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, रिक्त,
					&वामः_extent, start,
					min(len, वामः_extent.e_lblk - start),
					goal);
		यदि (err)
			गच्छ errout;

		गच्छ start_again;
	} अन्यथा यदि (अग्र >= start) {
		range_start = अग्र;
		वामः_adjacent = &वामः_extent;
	} अन्यथा {
		range_start = start;
		वामः_adjacent = रिक्त;
	}
	goal = वामः_extent.e_pblk + (range_start - वामः_extent.e_lblk);

	कुरु {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र_पर्णम्,
					   &दक्षिणः_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (पूर्णाङ्क)err);
		dbg_print_extent("ext_falloc next", &दक्षिणः_extent);
		/* Stop if we've seen this extent before */
		यदि (!err && दक्षिणः_extent.e_lblk <= वामः_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_अग्र;

		यदि (err && err != EXT2_ET_EXTENT_NO_अग्र)
			गच्छ errout;
		यदि (err == EXT2_ET_EXTENT_NO_अग्र ||
		    दक्षिणः_extent.e_lblk > अन्तः + 1) {
			range_अन्तः = अन्तः;
			दक्षिणः_adjacent = रिक्त;
		} अन्यथा {
			/* Handle right_extent.e_lblk <= end */
			range_अन्तः = दक्षिणः_extent.e_lblk - 1;
			दक्षिणः_adjacent = &दक्षिणः_extent;
		}
		goal_distance = range_start - अग्र;
		यदि (err != EXT2_ET_EXTENT_NO_अग्र &&
		    goal_distance > (range_अन्तः - दक्षिणः_extent.e_lblk))
			goal = दक्षिणः_extent.e_pblk -
					(दक्षिणः_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_अन्तः);
		err = 0;
		यदि (range_start <= range_अन्तः) {
			गणनम् = range_अन्तः - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						वामः_adjacent, दक्षिणः_adjacent,
						range_start, गणनम्, goal);
			यदि (err)
				गच्छ errout;
		}

		यदि (range_अन्तः == अन्तः)
			भङ्ग;

		err = ext2fs_extent_goto(handle, दक्षिणः_extent.e_lblk);
		यदि (err)
			गच्छ errout;
		अग्र = दक्षिणः_extent.e_lblk + दक्षिणः_extent.e_len;
		वामः_extent = दक्षिणः_extent;
		वामः_adjacent = &वामः_extent;
		range_start = अग्र;
		goal = वामः_extent.e_pblk + (range_start - वामः_extent.e_lblk);
	} यावत् (range_अन्तः < अन्तः);

errout:
	ext2fs_extent_free(handle);
	प्रत्यागम err;
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
	blk64_t			blk, x, zero_blk, चरमः = 0;
	पूर्णाङ्क			zero_len = 0;
	errcode_t		err;

	यदि (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	यदि (len > ext2fs_खण्डानि_गणनम्(fs->super))
		प्रत्यागम EXT2_ET_खण्डम्_ALLOC_FAIL;
	अन्यथा यदि (len == 0)
		प्रत्यागम 0;

	/* Read inode structure if necessary */
	यदि (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		यदि (err)
			प्रत्यागम err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	यदि (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		गच्छ बहिर्गमनम्;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	प्रति (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, रिक्त, 0, blk, 0, &x);
		यदि (err)
			प्रत्यागम err;
		यदि (x)
			अनुवर्त;

		err = ext2fs_bmap2(fs, ino, inode, रिक्त, BMAP_ALLOC,
				   blk, 0, &x);
		यदि (err)
			गच्छ errout;
		यदि ((zero_len && (x != चरमः+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  रिक्त, रिक्त);
			zero_len = 0;
			यदि (err)
				गच्छ errout;
		}
		यदि (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} अन्यथा {
			zero_len++;
		}
		चरमः = x;
	}

बहिर्गमनम्:
	यदि (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	यदि (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, रिक्त, रिक्त);
	प्रत्यागम err;
}
