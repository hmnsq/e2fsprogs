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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  ಕುರು {printf(f, ## a); fflush(stdout); } ಯಾವತ್ (0)
#ಅನ್ಯಥಾ
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

ಸ್ಥಿರ ಶೂನ್ಯ dbg_print_extent(ಧ್ರುವ ವರ್ಣ *desc EXT2FS_ATTR((unused)),
		ಧ್ರುವ ಸಂರಚನಾ ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	ಯದಿ (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ಯದಿ (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	ಯದಿ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ಯದಿ (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	ಯದಿ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#endif
}

ಸ್ಥಿರ errcode_t claim_range(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	ಪ್ರತ್ಯಾಗಮ ext2fs_iblk_add_blocks(fs, inode, clusters);
}

ಸ್ಥಿರ errcode_t ext_falloc_helper(ext2_filsys fs,
				   ಪೂರ್ಣಾಙ್ಕ flags,
				   ext2_ino_t ino,
				   ಸಂರಚನಾ ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   ಸಂರಚನಾ ext2fs_extent *ಎಡ_ext,
				   ಸಂರಚನಾ ext2fs_extent *ಬಲ_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	ಸಂರಚನಾ ext2fs_extent	newex, ex;
	ಪೂರ್ಣಾಙ್ಕ			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	printf("%s: ", __func__);
	ಯದಿ (ಎಡ_ext)
		printf("left_ext=%llu--%llu, ", ಎಡ_ext->e_lblk,
		       ಎಡ_ext->e_lblk + ಎಡ_ext->e_len - 1);
	ಯದಿ (ಬಲ_ext)
		printf("right_ext=%llu--%llu, ", ಬಲ_ext->e_lblk,
		       ಬಲ_ext->e_lblk + ಬಲ_ext->e_len - 1);
	printf("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#endif
	/* Can't create initialized extents past EOF? */
	ಯದಿ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_SIZE(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	ಯದಿ (ಎಡ_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		ಯದಿ (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ಎಡ_ext->e_len;
		ಅನ್ಯಥಾ
			fillable = max_init_len - ಎಡ_ext->e_len;

		ಯದಿ (fillable > range_len)
			fillable = range_len;
		ಯದಿ (fillable == 0)
			ಗಚ್ಛ expand_ಬಲ;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		ಯದಿ (cluster_fill == 0)
			ಗಚ್ಛ expand_ಬಲ;

		ಯದಿ (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		ಯದಿ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = ಎಡ_ext->e_lblk + ಎಡ_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			ಯದಿ (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			ಯದಿ (cluster_fill == 0)
				ಗಚ್ಛ expand_ಬಲ;
		}

		err = ext2fs_extent_goto(handle, ಎಡ_ext->e_lblk);
		ಯದಿ (err)
			ಗಚ್ಛ expand_ಬಲ;
		ಎಡ_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", ಎಡ_ext);
		err = ext2fs_extent_replace(handle, 0, ಎಡ_ext);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_fix_parents(handle);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		/* Zero blocks */
		ಯದಿ (!(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ಎಡ_ext->e_pblk +
						  ಎಡ_ext->e_len -
						  cluster_fill, cluster_fill,
						  ರಿಕ್ತ, ರಿಕ್ತ);
			ಯದಿ (err)
				ಗಚ್ಛ out;
		}
	}

expand_ಬಲ:
	/* We must lengthen the right extent to the beginning of the cluster */
	ಯದಿ (ಬಲ_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		ಯದಿ (ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ಬಲ_ext->e_len;
		ಅನ್ಯಥಾ
			fillable = max_init_len - ಬಲ_ext->e_len;

		ಯದಿ (fillable > range_len)
			fillable = range_len;
		ಯದಿ (fillable == 0)
			ಗಚ್ಛ try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = ಬಲ_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		ಯದಿ (cluster_fill == 0)
			ಗಚ್ಛ try_merge;

		err = ext2fs_extent_goto(handle, ಬಲ_ext->e_lblk);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		ಯದಿ (cluster_fill > fillable)
			cluster_fill = fillable;
		ಬಲ_ext->e_lblk -= cluster_fill;
		ಬಲ_ext->e_pblk -= cluster_fill;
		ಬಲ_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", ಬಲ_ext);
		err = ext2fs_extent_replace(handle, 0, ಬಲ_ext);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_fix_parents(handle);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		/* Zero blocks if necessary */
		ಯದಿ (!(ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ಬಲ_ext->e_pblk,
						  cluster_fill, ರಿಕ್ತ, ರಿಕ್ತ);
			ಯದಿ (err)
				ಗಚ್ಛ out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	ಯದಿ (ಎಡ_ext && ಬಲ_ext) {
		/* Are the two extents mergeable? */
		ಯದಿ ((ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			ಗಚ್ಛ try_ಎಡ;

		/* User requires init/uninit but extent is uninit/init. */
		ಯದಿ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ಗಚ್ಛ try_ಎಡ;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		ಯದಿ (!(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			ಗಚ್ಛ try_ಎಡ;

		/* Will it even fit? */
		x = ಎಡ_ext->e_len + range_len + ಬಲ_ext->e_len;
		ಯದಿ (x > (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			ಗಚ್ಛ try_ಎಡ;

		/* Would they even be physically contiguous if merged? */
		ಯದಿ (ಎಡ_ext->e_pblk + ಎಡ_ext->e_len + range_len !=
				ಬಲ_ext->e_pblk)
			ಗಚ್ಛ try_ಎಡ;

		err = ext2fs_extent_goto(handle, ಎಡ_ext->e_lblk);
		ಯದಿ (err)
			ಗಚ್ಛ try_ಎಡ;

		/* Allocate blocks */
		y = ಎಡ_ext->e_pblk + ಎಡ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_LENGTH, y,
				       ಬಲ_ext->e_pblk - y + 1, ರಿಕ್ತ,
				       &pblk, &plen);
		ಯದಿ (err)
			ಗಚ್ಛ try_ಎಡ;
		ಯದಿ (pblk + plen != ಬಲ_ext->e_pblk)
			ಗಚ್ಛ try_ಎಡ;
		err = claim_range(fs, inode, pblk, plen);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		/* Modify extents */
		ಎಡ_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", ಎಡ_ext);
		err = ext2fs_extent_replace(handle, 0, ಎಡ_ext);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_fix_parents(handle);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &newex);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_delete(handle, 0);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_fix_parents(handle);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		*ಬಲ_ext = *ಎಡ_ext;

		/* Zero blocks */
		ಯದಿ (!(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  ರಿಕ್ತ, ರಿಕ್ತ);
			ಯದಿ (err)
				ಗಚ್ಛ out;
		}

		ಪ್ರತ್ಯಾಗಮ 0;
	}

try_ಎಡ:
	/* Extend the left extent */
	ಯದಿ (ಎಡ_ext) {
		/* How many more blocks can be attached to left_ext? */
		ಯದಿ (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ಎಡ_ext->e_len;
		ಅನ್ಯಥಾ ಯದಿ (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - ಎಡ_ext->e_len;
		ಅನ್ಯಥಾ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ಯದಿ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ಗಚ್ಛ try_ಬಲ;

		ಯದಿ (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = ಎಡ_ext->e_lblk + ಎಡ_ext->e_len - 1;
		ಯದಿ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			ಯದಿ (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		ಯದಿ (fillable == 0)
			ಗಚ್ಛ try_ಬಲ;

		/* Test if the right edge of the range is already mapped? */
		ಯದಿ (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			ಯದಿ (err)
				ಗಚ್ಛ out;
			ಯದಿ (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			ಯದಿ (fillable == 0)
				ಗಚ್ಛ try_ಬಲ;
		}

		/* Allocate range of blocks */
		x = ಎಡ_ext->e_pblk + ಎಡ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				x, fillable, ರಿಕ್ತ, &pblk, &plen);
		ಯದಿ (err)
			ಗಚ್ಛ try_ಬಲ;
		err = claim_range(fs, inode, pblk, plen);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, ಎಡ_ext->e_lblk);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		range_start += plen;
		range_len -= plen;
		ಎಡ_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", ಎಡ_ext);
		err = ext2fs_extent_replace(handle, 0, ಎಡ_ext);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_fix_parents(handle);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		/* Zero blocks if necessary */
		ಯದಿ (!(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ರಿಕ್ತ, ರಿಕ್ತ);
			ಯದಿ (err)
				ಗಚ್ಛ out;
		}
	}

try_ಬಲ:
	/* Extend the right extent */
	ಯದಿ (ಬಲ_ext) {
		/* How much can we attach to right_ext? */
		ಯದಿ (ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ಬಲ_ext->e_len;
		ಅನ್ಯಥಾ ಯದಿ (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - ಬಲ_ext->e_len;
		ಅನ್ಯಥಾ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ಯದಿ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ಗಚ್ಛ try_anywhere;

		ಯದಿ (fillable > range_len)
			fillable = range_len;
		ಯದಿ (fillable == 0)
			ಗಚ್ಛ try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		ಯದಿ (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					ಬಲ_ext->e_lblk - fillable, &pblk);
			ಯದಿ (err)
				ಗಚ್ಛ out;
			ಯದಿ (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((ಬಲ_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			ಯದಿ (fillable == 0)
				ಗಚ್ಛ try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				ಬಲ_ext->e_pblk - fillable,
				fillable, ರಿಕ್ತ, &pblk, &plen);
		ಯದಿ (err)
			ಗಚ್ಛ try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		ಯದಿ (err)
			ಗಚ್ಛ out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, ಬಲ_ext->e_lblk);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		range_len -= plen;
		ಬಲ_ext->e_lblk -= plen;
		ಬಲ_ext->e_pblk -= plen;
		ಬಲ_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", ಬಲ_ext);
		err = ext2fs_extent_replace(handle, 0, ಬಲ_ext);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_fix_parents(handle);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		/* Zero blocks if necessary */
		ಯದಿ (!(ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, ರಿಕ್ತ, ರಿಕ್ತ);
			ಯದಿ (err)
				ಗಚ್ಛ out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	ಯದಿ (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		ಯದಿ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		ಯದಿ (pblk == 0)
			ಗಚ್ಛ try_ಬಲ_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ಯದಿ (err == EXT2_ET_NO_CURRENT_ಗಂಟು)
			ex.e_lblk = 0;
		ಅನ್ಯಥಾ ಯದಿ (err)
			ಗಚ್ಛ out;

		ಯದಿ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ಅನ್ಯಥಾ
			op = EXT2_EXTENT_ಸೇರಿಸು_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ಸೇರಿಸು(handle, op, &newex);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_fix_parents(handle);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		ಯದಿ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ರಿಕ್ತ, ರಿಕ್ತ);
			ಯದಿ (err)
				ಗಚ್ಛ out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_ಬಲ_implied:
	y = range_start + range_len;
	ಯದಿ (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		ಯದಿ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		ಯದಿ (pblk == 0)
			ಗಚ್ಛ no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ಯದಿ (err == EXT2_ET_NO_CURRENT_ಗಂಟು)
			ex.e_lblk = 0;
		ಅನ್ಯಥಾ ಯದಿ (err)
			ಗಚ್ಛ out;

		ಯದಿ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ಅನ್ಯಥಾ
			op = EXT2_EXTENT_ಸೇರಿಸು_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ಸೇರಿಸು(handle, op, &newex);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_fix_parents(handle);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		ಯದಿ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ರಿಕ್ತ, ರಿಕ್ತ);
			ಯದಿ (err)
				ಗಚ್ಛ out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	ಯದಿ (range_len == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	newex.e_lblk = range_start;
	ಯದಿ (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} ಅನ್ಯಥಾ {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	ಪ್ರತಿ (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       ರಿಕ್ತ, &pblk, &plen);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = claim_range(fs, inode, pblk, plen);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ಯದಿ (err == EXT2_ET_NO_CURRENT_ಗಂಟು)
			ex.e_lblk = 0;
		ಅನ್ಯಥಾ ಯದಿ (err)
			ಗಚ್ಛ out;

		ಯದಿ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ಅನ್ಯಥಾ
			op = EXT2_EXTENT_ಸೇರಿಸು_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ಸೇರಿಸು(handle, op, &newex);
		ಯದಿ (err)
			ಗಚ್ಛ out;
		err = ext2fs_extent_fix_parents(handle);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		ಯದಿ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ರಿಕ್ತ, ರಿಕ್ತ);
			ಯದಿ (err)
				ಗಚ್ಛ out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		ಯದಿ (pblk >= ext2fs_blocks_ಎಣಿಕೆ(fs->super))
			pblk = fs->super->s_first_data_block;
	}

out:
	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ errcode_t extent_fallocate(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, ext2_ino_t ino,
				      ಸಂರಚನಾ ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	ಸಂರಚನಾ ext2fs_extent	ಎಡ_extent, ಬಲ_extent;
	ಸಂರಚನಾ ext2fs_extent	*ಎಡ_adjacent, *ಬಲ_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, next;
	blk64_t			ಎಣಿಕೆ, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

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
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &ಎಡ_extent);
	ಯದಿ (err == EXT2_ET_NO_CURRENT_ಗಂಟು) {
		blk64_t max_blocks = ext2fs_blocks_ಎಣಿಕೆ(fs->super);

		ಯದಿ (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ರಿಕ್ತ,
					ರಿಕ್ತ, start, len, goal);
		ಗಚ್ಛ errout;
	} ಅನ್ಯಥಾ ಯದಿ (err)
		ಗಚ್ಛ errout;

	dbg_print_extent("ext_falloc initial", &ಎಡ_extent);
	next = ಎಡ_extent.e_lblk + ಎಡ_extent.e_len;
	ಯದಿ (ಎಡ_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = ಎಡ_extent.e_pblk - (ಎಡ_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ರಿಕ್ತ,
					&ಎಡ_extent, start,
					min(len, ಎಡ_extent.e_lblk - start),
					goal);
		ಯದಿ (err)
			ಗಚ್ಛ errout;

		ಗಚ್ಛ start_again;
	} ಅನ್ಯಥಾ ಯದಿ (next >= start) {
		range_start = next;
		ಎಡ_adjacent = &ಎಡ_extent;
	} ಅನ್ಯಥಾ {
		range_start = start;
		ಎಡ_adjacent = ರಿಕ್ತ;
	}
	goal = ಎಡ_extent.e_pblk + (range_start - ಎಡ_extent.e_lblk);

	ಕುರು {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF,
					   &ಬಲ_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (ಪೂರ್ಣಾಙ್ಕ)err);
		dbg_print_extent("ext_falloc next", &ಬಲ_extent);
		/* Stop if we've seen this extent before */
		ಯದಿ (!err && ಬಲ_extent.e_lblk <= ಎಡ_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_NEXT;

		ಯದಿ (err && err != EXT2_ET_EXTENT_NO_NEXT)
			ಗಚ್ಛ errout;
		ಯದಿ (err == EXT2_ET_EXTENT_NO_NEXT ||
		    ಬಲ_extent.e_lblk > end + 1) {
			range_end = end;
			ಬಲ_adjacent = ರಿಕ್ತ;
		} ಅನ್ಯಥಾ {
			/* Handle right_extent.e_lblk <= end */
			range_end = ಬಲ_extent.e_lblk - 1;
			ಬಲ_adjacent = &ಬಲ_extent;
		}
		goal_distance = range_start - next;
		ಯದಿ (err != EXT2_ET_EXTENT_NO_NEXT &&
		    goal_distance > (range_end - ಬಲ_extent.e_lblk))
			goal = ಬಲ_extent.e_pblk -
					(ಬಲ_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		ಯದಿ (range_start <= range_end) {
			ಎಣಿಕೆ = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						ಎಡ_adjacent, ಬಲ_adjacent,
						range_start, ಎಣಿಕೆ, goal);
			ಯದಿ (err)
				ಗಚ್ಛ errout;
		}

		ಯದಿ (range_end == end)
			ಭಙ್ಗ;

		err = ext2fs_extent_goto(handle, ಬಲ_extent.e_lblk);
		ಯದಿ (err)
			ಗಚ್ಛ errout;
		next = ಬಲ_extent.e_lblk + ಬಲ_extent.e_len;
		ಎಡ_extent = ಬಲ_extent;
		ಎಡ_adjacent = &ಎಡ_extent;
		range_start = next;
		goal = ಎಡ_extent.e_pblk + (range_start - ಎಡ_extent.e_lblk);
	} ಯಾವತ್ (range_end < end);

errout:
	ext2fs_extent_free(handle);
	ಪ್ರತ್ಯಾಗಮ err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, ext2_ino_t ino,
			   ಸಂರಚನಾ ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	ಸಂರಚನಾ ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	ಪೂರ್ಣಾಙ್ಕ			zero_len = 0;
	errcode_t		err;

	ಯದಿ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	ಯದಿ (len > ext2fs_blocks_ಎಣಿಕೆ(fs->super))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BLOCK_ALLOC_FAIL;
	ಅನ್ಯಥಾ ಯದಿ (len == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Read inode structure if necessary */
	ಯದಿ (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	ಯದಿ (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		ಗಚ್ಛ out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	ಪ್ರತಿ (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, ರಿಕ್ತ, 0, blk, 0, &x);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ err;
		ಯದಿ (x)
			ಅನುವರ್ತ;

		err = ext2fs_bmap2(fs, ino, inode, ರಿಕ್ತ, BMAP_ALLOC,
				   blk, 0, &x);
		ಯದಿ (err)
			ಗಚ್ಛ errout;
		ಯದಿ ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  ರಿಕ್ತ, ರಿಕ್ತ);
			zero_len = 0;
			ಯದಿ (err)
				ಗಚ್ಛ errout;
		}
		ಯದಿ (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} ಅನ್ಯಥಾ {
			zero_len++;
		}
		last = x;
	}

out:
	ಯದಿ (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	ಯದಿ (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, ರಿಕ್ತ, ರಿಕ್ತ);
	ಪ್ರತ್ಯಾಗಮ err;
}
