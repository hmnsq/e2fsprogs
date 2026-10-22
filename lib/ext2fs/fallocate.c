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

#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  करा {म_लिखो(f, ## a); ख_साफ(मानक_निकास); } पर्यंत (0)
#अन्यथा
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

स्थिर शून्य dbg_print_extent(स्थिरांक अक्षर *desc EXT2FS_ATTR((unused)),
		स्थिरांक संरचना ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	जर (desc)
		म_लिखो("%s: ", desc);
	म_लिखो("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	जर (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		ख_माला_दो("LEAF ", मानक_निकास);
	जर (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		ख_माला_दो("UNINIT ", मानक_निकास);
	जर (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		ख_माला_दो("2ND_VISIT ", मानक_निकास);
	जर (!extent->e_flags)
		ख_माला_दो("(none)", मानक_निकास);
	ख_अक्षर_दो('\n', मानक_निकास);
	ख_साफ(मानक_निकास);
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
	परत ext2fs_iblk_add_blocks(fs, inode, clusters);
}

स्थिर errcode_t ext_falloc_helper(ext2_filsys fs,
				   पूर्णांक flags,
				   ext2_ino_t ino,
				   संरचना ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   संरचना ext2fs_extent *डावा_ext,
				   संरचना ext2fs_extent *उजवा_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	संरचना ext2fs_extent	newex, ex;
	पूर्णांक			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	म_लिखो("%s: ", __func__);
	जर (डावा_ext)
		म_लिखो("left_ext=%llu--%llu, ", डावा_ext->e_lblk,
		       डावा_ext->e_lblk + डावा_ext->e_len - 1);
	जर (उजवा_ext)
		म_लिखो("right_ext=%llu--%llu, ", उजवा_ext->e_lblk,
		       उजवा_ext->e_lblk + उजवा_ext->e_len - 1);
	म_लिखो("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	ख_साफ(मानक_निकास);
#endif
	/* Can't create initialized extents past EOF? */
	जर (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_SIZE(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	जर (डावा_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		जर (डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - डावा_ext->e_len;
		अन्यथा
			fillable = max_init_len - डावा_ext->e_len;

		जर (fillable > range_len)
			fillable = range_len;
		जर (fillable == 0)
			जा expand_उजवा;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		जर (cluster_fill == 0)
			जा expand_उजवा;

		जर (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		जर (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = डावा_ext->e_lblk + डावा_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			जर (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			जर (cluster_fill == 0)
				जा expand_उजवा;
		}

		err = ext2fs_extent_goto(handle, डावा_ext->e_lblk);
		जर (err)
			जा expand_उजवा;
		डावा_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", डावा_ext);
		err = ext2fs_extent_replace(handle, 0, डावा_ext);
		जर (err)
			जा out;
		err = ext2fs_extent_fix_parents(handle);
		जर (err)
			जा out;

		/* Zero blocks */
		जर (!(डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, डावा_ext->e_pblk +
						  डावा_ext->e_len -
						  cluster_fill, cluster_fill,
						  रिक्त, रिक्त);
			जर (err)
				जा out;
		}
	}

expand_उजवा:
	/* We must lengthen the right extent to the beginning of the cluster */
	जर (उजवा_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		जर (उजवा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - उजवा_ext->e_len;
		अन्यथा
			fillable = max_init_len - उजवा_ext->e_len;

		जर (fillable > range_len)
			fillable = range_len;
		जर (fillable == 0)
			जा try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = उजवा_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		जर (cluster_fill == 0)
			जा try_merge;

		err = ext2fs_extent_goto(handle, उजवा_ext->e_lblk);
		जर (err)
			जा out;

		जर (cluster_fill > fillable)
			cluster_fill = fillable;
		उजवा_ext->e_lblk -= cluster_fill;
		उजवा_ext->e_pblk -= cluster_fill;
		उजवा_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", उजवा_ext);
		err = ext2fs_extent_replace(handle, 0, उजवा_ext);
		जर (err)
			जा out;
		err = ext2fs_extent_fix_parents(handle);
		जर (err)
			जा out;

		/* Zero blocks if necessary */
		जर (!(उजवा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, उजवा_ext->e_pblk,
						  cluster_fill, रिक्त, रिक्त);
			जर (err)
				जा out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	जर (डावा_ext && उजवा_ext) {
		/* Are the two extents mergeable? */
		जर ((डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (उजवा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			जा try_डावा;

		/* User requires init/uninit but extent is uninit/init. */
		जर (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			जा try_डावा;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		जर (!(डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			जा try_डावा;

		/* Will it even fit? */
		x = डावा_ext->e_len + range_len + उजवा_ext->e_len;
		जर (x > (डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			जा try_डावा;

		/* Would they even be physically contiguous if merged? */
		जर (डावा_ext->e_pblk + डावा_ext->e_len + range_len !=
				उजवा_ext->e_pblk)
			जा try_डावा;

		err = ext2fs_extent_goto(handle, डावा_ext->e_lblk);
		जर (err)
			जा try_डावा;

		/* Allocate blocks */
		y = डावा_ext->e_pblk + डावा_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_LENGTH, y,
				       उजवा_ext->e_pblk - y + 1, रिक्त,
				       &pblk, &plen);
		जर (err)
			जा try_डावा;
		जर (pblk + plen != उजवा_ext->e_pblk)
			जा try_डावा;
		err = claim_range(fs, inode, pblk, plen);
		जर (err)
			जा out;

		/* Modify extents */
		डावा_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", डावा_ext);
		err = ext2fs_extent_replace(handle, 0, डावा_ext);
		जर (err)
			जा out;
		err = ext2fs_extent_fix_parents(handle);
		जर (err)
			जा out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &newex);
		जर (err)
			जा out;
		err = ext2fs_extent_delete(handle, 0);
		जर (err)
			जा out;
		err = ext2fs_extent_fix_parents(handle);
		जर (err)
			जा out;
		*उजवा_ext = *डावा_ext;

		/* Zero blocks */
		जर (!(डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  रिक्त, रिक्त);
			जर (err)
				जा out;
		}

		परत 0;
	}

try_डावा:
	/* Extend the left extent */
	जर (डावा_ext) {
		/* How many more blocks can be attached to left_ext? */
		जर (डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - डावा_ext->e_len;
		अन्यथा जर (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - डावा_ext->e_len;
		अन्यथा
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		जर (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			जा try_उजवा;

		जर (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = डावा_ext->e_lblk + डावा_ext->e_len - 1;
		जर (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			जर (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		जर (fillable == 0)
			जा try_उजवा;

		/* Test if the right edge of the range is already mapped? */
		जर (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			जर (err)
				जा out;
			जर (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			जर (fillable == 0)
				जा try_उजवा;
		}

		/* Allocate range of blocks */
		x = डावा_ext->e_pblk + डावा_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				x, fillable, रिक्त, &pblk, &plen);
		जर (err)
			जा try_उजवा;
		err = claim_range(fs, inode, pblk, plen);
		जर (err)
			जा out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, डावा_ext->e_lblk);
		जर (err)
			जा out;
		range_start += plen;
		range_len -= plen;
		डावा_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", डावा_ext);
		err = ext2fs_extent_replace(handle, 0, डावा_ext);
		जर (err)
			जा out;
		err = ext2fs_extent_fix_parents(handle);
		जर (err)
			जा out;

		/* Zero blocks if necessary */
		जर (!(डावा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, रिक्त, रिक्त);
			जर (err)
				जा out;
		}
	}

try_उजवा:
	/* Extend the right extent */
	जर (उजवा_ext) {
		/* How much can we attach to right_ext? */
		जर (उजवा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - उजवा_ext->e_len;
		अन्यथा जर (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - उजवा_ext->e_len;
		अन्यथा
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		जर (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (उजवा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(उजवा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			जा try_anywhere;

		जर (fillable > range_len)
			fillable = range_len;
		जर (fillable == 0)
			जा try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		जर (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					उजवा_ext->e_lblk - fillable, &pblk);
			जर (err)
				जा out;
			जर (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((उजवा_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			जर (fillable == 0)
				जा try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				उजवा_ext->e_pblk - fillable,
				fillable, रिक्त, &pblk, &plen);
		जर (err)
			जा try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		जर (err)
			जा out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, उजवा_ext->e_lblk);
		जर (err)
			जा out;
		range_len -= plen;
		उजवा_ext->e_lblk -= plen;
		उजवा_ext->e_pblk -= plen;
		उजवा_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", उजवा_ext);
		err = ext2fs_extent_replace(handle, 0, उजवा_ext);
		जर (err)
			जा out;
		err = ext2fs_extent_fix_parents(handle);
		जर (err)
			जा out;

		/* Zero blocks if necessary */
		जर (!(उजवा_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, रिक्त, रिक्त);
			जर (err)
				जा out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	जर (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		जर (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		जर (err)
			जा out;
		जर (pblk == 0)
			जा try_उजवा_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		जर (err == EXT2_ET_NO_CURRENT_गाठ)
			ex.e_lblk = 0;
		अन्यथा जर (err)
			जा out;

		जर (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अन्यथा
			op = EXT2_EXTENT_घाला_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_घाला(handle, op, &newex);
		जर (err)
			जा out;
		err = ext2fs_extent_fix_parents(handle);
		जर (err)
			जा out;

		जर (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, रिक्त, रिक्त);
			जर (err)
				जा out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_उजवा_implied:
	y = range_start + range_len;
	जर (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		जर (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		जर (err)
			जा out;
		जर (pblk == 0)
			जा no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		जर (err == EXT2_ET_NO_CURRENT_गाठ)
			ex.e_lblk = 0;
		अन्यथा जर (err)
			जा out;

		जर (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अन्यथा
			op = EXT2_EXTENT_घाला_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_घाला(handle, op, &newex);
		जर (err)
			जा out;
		err = ext2fs_extent_fix_parents(handle);
		जर (err)
			जा out;

		जर (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, रिक्त, रिक्त);
			जर (err)
				जा out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	जर (range_len == 0)
		परत 0;

	newex.e_lblk = range_start;
	जर (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} अन्यथा {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	साठी (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       रिक्त, &pblk, &plen);
		जर (err)
			जा out;
		err = claim_range(fs, inode, pblk, plen);
		जर (err)
			जा out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		जर (err == EXT2_ET_NO_CURRENT_गाठ)
			ex.e_lblk = 0;
		अन्यथा जर (err)
			जा out;

		जर (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अन्यथा
			op = EXT2_EXTENT_घाला_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_घाला(handle, op, &newex);
		जर (err)
			जा out;
		err = ext2fs_extent_fix_parents(handle);
		जर (err)
			जा out;

		जर (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, रिक्त, रिक्त);
			जर (err)
				जा out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		जर (pblk >= ext2fs_blocks_मोजणी(fs->super))
			pblk = fs->super->s_first_data_block;
	}

out:
	परत err;
}

स्थिर errcode_t extent_fallocate(ext2_filsys fs, पूर्णांक flags, ext2_ino_t ino,
				      संरचना ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	संरचना ext2fs_extent	डावा_extent, उजवा_extent;
	संरचना ext2fs_extent	*डावा_adjacent, *उजवा_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, next;
	blk64_t			मोजणी, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	जर (err)
		परत err;

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
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &डावा_extent);
	जर (err == EXT2_ET_NO_CURRENT_गाठ) {
		blk64_t max_blocks = ext2fs_blocks_मोजणी(fs->super);

		जर (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, रिक्त,
					रिक्त, start, len, goal);
		जा errout;
	} अन्यथा जर (err)
		जा errout;

	dbg_print_extent("ext_falloc initial", &डावा_extent);
	next = डावा_extent.e_lblk + डावा_extent.e_len;
	जर (डावा_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = डावा_extent.e_pblk - (डावा_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, रिक्त,
					&डावा_extent, start,
					min(len, डावा_extent.e_lblk - start),
					goal);
		जर (err)
			जा errout;

		जा start_again;
	} अन्यथा जर (next >= start) {
		range_start = next;
		डावा_adjacent = &डावा_extent;
	} अन्यथा {
		range_start = start;
		डावा_adjacent = रिक्त;
	}
	goal = डावा_extent.e_pblk + (range_start - डावा_extent.e_lblk);

	करा {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF,
					   &उजवा_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (पूर्णांक)err);
		dbg_print_extent("ext_falloc next", &उजवा_extent);
		/* Stop if we've seen this extent before */
		जर (!err && उजवा_extent.e_lblk <= डावा_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_NEXT;

		जर (err && err != EXT2_ET_EXTENT_NO_NEXT)
			जा errout;
		जर (err == EXT2_ET_EXTENT_NO_NEXT ||
		    उजवा_extent.e_lblk > end + 1) {
			range_end = end;
			उजवा_adjacent = रिक्त;
		} अन्यथा {
			/* Handle right_extent.e_lblk <= end */
			range_end = उजवा_extent.e_lblk - 1;
			उजवा_adjacent = &उजवा_extent;
		}
		goal_distance = range_start - next;
		जर (err != EXT2_ET_EXTENT_NO_NEXT &&
		    goal_distance > (range_end - उजवा_extent.e_lblk))
			goal = उजवा_extent.e_pblk -
					(उजवा_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		जर (range_start <= range_end) {
			मोजणी = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						डावा_adjacent, उजवा_adjacent,
						range_start, मोजणी, goal);
			जर (err)
				जा errout;
		}

		जर (range_end == end)
			थांबा;

		err = ext2fs_extent_goto(handle, उजवा_extent.e_lblk);
		जर (err)
			जा errout;
		next = उजवा_extent.e_lblk + उजवा_extent.e_len;
		डावा_extent = उजवा_extent;
		डावा_adjacent = &डावा_extent;
		range_start = next;
		goal = डावा_extent.e_pblk + (range_start - डावा_extent.e_lblk);
	} पर्यंत (range_end < end);

errout:
	ext2fs_extent_free(handle);
	परत err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, पूर्णांक flags, ext2_ino_t ino,
			   संरचना ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	संरचना ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	पूर्णांक			zero_len = 0;
	errcode_t		err;

	जर (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		परत EXT2_ET_INVALID_ARGUMENT;

	जर (len > ext2fs_blocks_मोजणी(fs->super))
		परत EXT2_ET_BLOCK_ALLOC_FAIL;
	अन्यथा जर (len == 0)
		परत 0;

	/* Read inode structure if necessary */
	जर (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		जर (err)
			परत err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	जर (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		जा out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	साठी (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, रिक्त, 0, blk, 0, &x);
		जर (err)
			परत err;
		जर (x)
			सुरूठेवा;

		err = ext2fs_bmap2(fs, ino, inode, रिक्त, BMAP_ALLOC,
				   blk, 0, &x);
		जर (err)
			जा errout;
		जर ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  रिक्त, रिक्त);
			zero_len = 0;
			जर (err)
				जा errout;
		}
		जर (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} अन्यथा {
			zero_len++;
		}
		last = x;
	}

out:
	जर (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	जर (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, रिक्त, रिक्त);
	परत err;
}
