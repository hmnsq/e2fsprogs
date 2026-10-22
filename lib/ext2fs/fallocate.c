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

#அந்தர்பாவ "config.h"

#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  செய் {printf(f, ## a); fflush(stdout); } போது (0)
#இல்லையெனில்
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

நிலையான வெறுமை dbg_print_extent(மாறிலி எழுத்து *desc EXT2FS_ATTR((unused)),
		மாறிலி கட்டமைப்பு ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	எனில் (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	எனில் (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	எனில் (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	எனில் (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	எனில் (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#endif
}

நிலையான errcode_t claim_range(ext2_filsys fs, கட்டமைப்பு ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	திரும்பு ext2fs_iblk_add_blocks(fs, inode, clusters);
}

நிலையான errcode_t ext_falloc_helper(ext2_filsys fs,
				   முழு flags,
				   ext2_ino_t ino,
				   கட்டமைப்பு ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   கட்டமைப்பு ext2fs_extent *இடது_ext,
				   கட்டமைப்பு ext2fs_extent *வலது_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	கட்டமைப்பு ext2fs_extent	newex, ex;
	முழு			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	printf("%s: ", __func__);
	எனில் (இடது_ext)
		printf("left_ext=%llu--%llu, ", இடது_ext->e_lblk,
		       இடது_ext->e_lblk + இடது_ext->e_len - 1);
	எனில் (வலது_ext)
		printf("right_ext=%llu--%llu, ", வலது_ext->e_lblk,
		       வலது_ext->e_lblk + வலது_ext->e_len - 1);
	printf("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#endif
	/* Can't create initialized extents past EOF? */
	எனில் (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_SIZE(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	எனில் (இடது_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		எனில் (இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - இடது_ext->e_len;
		இல்லையெனில்
			fillable = max_init_len - இடது_ext->e_len;

		எனில் (fillable > range_len)
			fillable = range_len;
		எனில் (fillable == 0)
			செல் expand_வலது;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		எனில் (cluster_fill == 0)
			செல் expand_வலது;

		எனில் (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		எனில் (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = இடது_ext->e_lblk + இடது_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			எனில் (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			எனில் (cluster_fill == 0)
				செல் expand_வலது;
		}

		err = ext2fs_extent_goto(handle, இடது_ext->e_lblk);
		எனில் (err)
			செல் expand_வலது;
		இடது_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", இடது_ext);
		err = ext2fs_extent_replace(handle, 0, இடது_ext);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_fix_parents(handle);
		எனில் (err)
			செல் out;

		/* Zero blocks */
		எனில் (!(இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, இடது_ext->e_pblk +
						  இடது_ext->e_len -
						  cluster_fill, cluster_fill,
						  வெற்று, வெற்று);
			எனில் (err)
				செல் out;
		}
	}

expand_வலது:
	/* We must lengthen the right extent to the beginning of the cluster */
	எனில் (வலது_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		எனில் (வலது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - வலது_ext->e_len;
		இல்லையெனில்
			fillable = max_init_len - வலது_ext->e_len;

		எனில் (fillable > range_len)
			fillable = range_len;
		எனில் (fillable == 0)
			செல் try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = வலது_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		எனில் (cluster_fill == 0)
			செல் try_merge;

		err = ext2fs_extent_goto(handle, வலது_ext->e_lblk);
		எனில் (err)
			செல் out;

		எனில் (cluster_fill > fillable)
			cluster_fill = fillable;
		வலது_ext->e_lblk -= cluster_fill;
		வலது_ext->e_pblk -= cluster_fill;
		வலது_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", வலது_ext);
		err = ext2fs_extent_replace(handle, 0, வலது_ext);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_fix_parents(handle);
		எனில் (err)
			செல் out;

		/* Zero blocks if necessary */
		எனில் (!(வலது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, வலது_ext->e_pblk,
						  cluster_fill, வெற்று, வெற்று);
			எனில் (err)
				செல் out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	எனில் (இடது_ext && வலது_ext) {
		/* Are the two extents mergeable? */
		எனில் ((இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (வலது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			செல் try_இடது;

		/* User requires init/uninit but extent is uninit/init. */
		எனில் (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			செல் try_இடது;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		எனில் (!(இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			செல் try_இடது;

		/* Will it even fit? */
		x = இடது_ext->e_len + range_len + வலது_ext->e_len;
		எனில் (x > (இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			செல் try_இடது;

		/* Would they even be physically contiguous if merged? */
		எனில் (இடது_ext->e_pblk + இடது_ext->e_len + range_len !=
				வலது_ext->e_pblk)
			செல் try_இடது;

		err = ext2fs_extent_goto(handle, இடது_ext->e_lblk);
		எனில் (err)
			செல் try_இடது;

		/* Allocate blocks */
		y = இடது_ext->e_pblk + இடது_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_LENGTH, y,
				       வலது_ext->e_pblk - y + 1, வெற்று,
				       &pblk, &plen);
		எனில் (err)
			செல் try_இடது;
		எனில் (pblk + plen != வலது_ext->e_pblk)
			செல் try_இடது;
		err = claim_range(fs, inode, pblk, plen);
		எனில் (err)
			செல் out;

		/* Modify extents */
		இடது_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", இடது_ext);
		err = ext2fs_extent_replace(handle, 0, இடது_ext);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_fix_parents(handle);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &newex);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_delete(handle, 0);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_fix_parents(handle);
		எனில் (err)
			செல் out;
		*வலது_ext = *இடது_ext;

		/* Zero blocks */
		எனில் (!(இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  வெற்று, வெற்று);
			எனில் (err)
				செல் out;
		}

		திரும்பு 0;
	}

try_இடது:
	/* Extend the left extent */
	எனில் (இடது_ext) {
		/* How many more blocks can be attached to left_ext? */
		எனில் (இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - இடது_ext->e_len;
		இல்லையெனில் எனில் (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - இடது_ext->e_len;
		இல்லையெனில்
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		எனில் (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			செல் try_வலது;

		எனில் (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = இடது_ext->e_lblk + இடது_ext->e_len - 1;
		எனில் (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			எனில் (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		எனில் (fillable == 0)
			செல் try_வலது;

		/* Test if the right edge of the range is already mapped? */
		எனில் (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			எனில் (err)
				செல் out;
			எனில் (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			எனில் (fillable == 0)
				செல் try_வலது;
		}

		/* Allocate range of blocks */
		x = இடது_ext->e_pblk + இடது_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				x, fillable, வெற்று, &pblk, &plen);
		எனில் (err)
			செல் try_வலது;
		err = claim_range(fs, inode, pblk, plen);
		எனில் (err)
			செல் out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, இடது_ext->e_lblk);
		எனில் (err)
			செல் out;
		range_start += plen;
		range_len -= plen;
		இடது_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", இடது_ext);
		err = ext2fs_extent_replace(handle, 0, இடது_ext);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_fix_parents(handle);
		எனில் (err)
			செல் out;

		/* Zero blocks if necessary */
		எனில் (!(இடது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, வெற்று, வெற்று);
			எனில் (err)
				செல் out;
		}
	}

try_வலது:
	/* Extend the right extent */
	எனில் (வலது_ext) {
		/* How much can we attach to right_ext? */
		எனில் (வலது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - வலது_ext->e_len;
		இல்லையெனில் எனில் (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - வலது_ext->e_len;
		இல்லையெனில்
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		எனில் (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (வலது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(வலது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			செல் try_anywhere;

		எனில் (fillable > range_len)
			fillable = range_len;
		எனில் (fillable == 0)
			செல் try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		எனில் (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					வலது_ext->e_lblk - fillable, &pblk);
			எனில் (err)
				செல் out;
			எனில் (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((வலது_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			எனில் (fillable == 0)
				செல் try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				வலது_ext->e_pblk - fillable,
				fillable, வெற்று, &pblk, &plen);
		எனில் (err)
			செல் try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		எனில் (err)
			செல் out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, வலது_ext->e_lblk);
		எனில் (err)
			செல் out;
		range_len -= plen;
		வலது_ext->e_lblk -= plen;
		வலது_ext->e_pblk -= plen;
		வலது_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", வலது_ext);
		err = ext2fs_extent_replace(handle, 0, வலது_ext);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_fix_parents(handle);
		எனில் (err)
			செல் out;

		/* Zero blocks if necessary */
		எனில் (!(வலது_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, வெற்று, வெற்று);
			எனில் (err)
				செல் out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	எனில் (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		எனில் (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		எனில் (err)
			செல் out;
		எனில் (pblk == 0)
			செல் try_வலது_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		எனில் (err == EXT2_ET_NO_CURRENT_முனை)
			ex.e_lblk = 0;
		இல்லையெனில் எனில் (err)
			செல் out;

		எனில் (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		இல்லையெனில்
			op = EXT2_EXTENT_செருகு_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_செருகு(handle, op, &newex);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_fix_parents(handle);
		எனில் (err)
			செல் out;

		எனில் (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, வெற்று, வெற்று);
			எனில் (err)
				செல் out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_வலது_implied:
	y = range_start + range_len;
	எனில் (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		எனில் (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		எனில் (err)
			செல் out;
		எனில் (pblk == 0)
			செல் no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		எனில் (err == EXT2_ET_NO_CURRENT_முனை)
			ex.e_lblk = 0;
		இல்லையெனில் எனில் (err)
			செல் out;

		எனில் (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		இல்லையெனில்
			op = EXT2_EXTENT_செருகு_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_செருகு(handle, op, &newex);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_fix_parents(handle);
		எனில் (err)
			செல் out;

		எனில் (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, வெற்று, வெற்று);
			எனில் (err)
				செல் out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	எனில் (range_len == 0)
		திரும்பு 0;

	newex.e_lblk = range_start;
	எனில் (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} இல்லையெனில் {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	க்கு (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       வெற்று, &pblk, &plen);
		எனில் (err)
			செல் out;
		err = claim_range(fs, inode, pblk, plen);
		எனில் (err)
			செல் out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		எனில் (err == EXT2_ET_NO_CURRENT_முனை)
			ex.e_lblk = 0;
		இல்லையெனில் எனில் (err)
			செல் out;

		எனில் (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		இல்லையெனில்
			op = EXT2_EXTENT_செருகு_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_செருகு(handle, op, &newex);
		எனில் (err)
			செல் out;
		err = ext2fs_extent_fix_parents(handle);
		எனில் (err)
			செல் out;

		எனில் (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, வெற்று, வெற்று);
			எனில் (err)
				செல் out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		எனில் (pblk >= ext2fs_blocks_மொத்தம்(fs->super))
			pblk = fs->super->s_first_data_block;
	}

out:
	திரும்பு err;
}

நிலையான errcode_t extent_fallocate(ext2_filsys fs, முழு flags, ext2_ino_t ino,
				      கட்டமைப்பு ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	கட்டமைப்பு ext2fs_extent	இடது_extent, வலது_extent;
	கட்டமைப்பு ext2fs_extent	*இடது_adjacent, *வலது_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, next;
	blk64_t			மொத்தம், goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	எனில் (err)
		திரும்பு err;

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
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &இடது_extent);
	எனில் (err == EXT2_ET_NO_CURRENT_முனை) {
		blk64_t max_blocks = ext2fs_blocks_மொத்தம்(fs->super);

		எனில் (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, வெற்று,
					வெற்று, start, len, goal);
		செல் errout;
	} இல்லையெனில் எனில் (err)
		செல் errout;

	dbg_print_extent("ext_falloc initial", &இடது_extent);
	next = இடது_extent.e_lblk + இடது_extent.e_len;
	எனில் (இடது_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = இடது_extent.e_pblk - (இடது_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, வெற்று,
					&இடது_extent, start,
					min(len, இடது_extent.e_lblk - start),
					goal);
		எனில் (err)
			செல் errout;

		செல் start_again;
	} இல்லையெனில் எனில் (next >= start) {
		range_start = next;
		இடது_adjacent = &இடது_extent;
	} இல்லையெனில் {
		range_start = start;
		இடது_adjacent = வெற்று;
	}
	goal = இடது_extent.e_pblk + (range_start - இடது_extent.e_lblk);

	செய் {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF,
					   &வலது_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (முழு)err);
		dbg_print_extent("ext_falloc next", &வலது_extent);
		/* Stop if we've seen this extent before */
		எனில் (!err && வலது_extent.e_lblk <= இடது_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_NEXT;

		எனில் (err && err != EXT2_ET_EXTENT_NO_NEXT)
			செல் errout;
		எனில் (err == EXT2_ET_EXTENT_NO_NEXT ||
		    வலது_extent.e_lblk > end + 1) {
			range_end = end;
			வலது_adjacent = வெற்று;
		} இல்லையெனில் {
			/* Handle right_extent.e_lblk <= end */
			range_end = வலது_extent.e_lblk - 1;
			வலது_adjacent = &வலது_extent;
		}
		goal_distance = range_start - next;
		எனில் (err != EXT2_ET_EXTENT_NO_NEXT &&
		    goal_distance > (range_end - வலது_extent.e_lblk))
			goal = வலது_extent.e_pblk -
					(வலது_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		எனில் (range_start <= range_end) {
			மொத்தம் = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						இடது_adjacent, வலது_adjacent,
						range_start, மொத்தம், goal);
			எனில் (err)
				செல் errout;
		}

		எனில் (range_end == end)
			நிறுத்து;

		err = ext2fs_extent_goto(handle, வலது_extent.e_lblk);
		எனில் (err)
			செல் errout;
		next = வலது_extent.e_lblk + வலது_extent.e_len;
		இடது_extent = வலது_extent;
		இடது_adjacent = &இடது_extent;
		range_start = next;
		goal = இடது_extent.e_pblk + (range_start - இடது_extent.e_lblk);
	} போது (range_end < end);

errout:
	ext2fs_extent_free(handle);
	திரும்பு err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, முழு flags, ext2_ino_t ino,
			   கட்டமைப்பு ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	கட்டமைப்பு ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	முழு			zero_len = 0;
	errcode_t		err;

	எனில் (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	எனில் (len > ext2fs_blocks_மொத்தம்(fs->super))
		திரும்பு EXT2_ET_BLOCK_ALLOC_FAIL;
	இல்லையெனில் எனில் (len == 0)
		திரும்பு 0;

	/* Read inode structure if necessary */
	எனில் (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		எனில் (err)
			திரும்பு err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	எனில் (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		செல் out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	க்கு (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, வெற்று, 0, blk, 0, &x);
		எனில் (err)
			திரும்பு err;
		எனில் (x)
			தொடர்;

		err = ext2fs_bmap2(fs, ino, inode, வெற்று, BMAP_ALLOC,
				   blk, 0, &x);
		எனில் (err)
			செல் errout;
		எனில் ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  வெற்று, வெற்று);
			zero_len = 0;
			எனில் (err)
				செல் errout;
		}
		எனில் (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} இல்லையெனில் {
			zero_len++;
		}
		last = x;
	}

out:
	எனில் (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	எனில் (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, வெற்று, வெற்று);
	திரும்பு err;
}
