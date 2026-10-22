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

#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ವ್ಯಾಖ್ಯಾನಿಸು min(a, b) ((a) < (b) ? (a) : (b))

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEBUG

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
# define dbg_printf(f, a...)  ಮಾಡು {ಬರೆ(f, ## a); fflush(stdout); } ಆಗ (0)
#ಇಲ್ಲದಿದ್ದರೆ
# define dbg_printf(f, a...)
#ಆದರೆ_ಅಂತ್ಯ

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

ಸ್ಥಿರ ಶೂನ್ಯ dbg_print_extent(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *desc EXT2FS_ATTR((ಬಳಸದ)),
		ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_extent *extent EXT2FS_ATTR((ಬಳಸದ)))
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	ಆದರೆ (desc)
		ಬರೆ("%s: ", desc);
	ಬರೆ("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_ಎಲೆ)
		fputs("LEAF ", stdout);
	ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_ಎರಡನೇ_ಭೇಟಿ)
		fputs("2ND_VISIT ", stdout);
	ಆದರೆ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ errcode_t claim_range(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) - 1) /
		   EXT2_FS_CLUSTER_ಅನುಪಾತ(fs);
	ext2fs_ಖಂಡ_alloc_stats_range(fs, blk,
			clusters * EXT2_FS_CLUSTER_ಅನುಪಾತ(fs), +1);
	ಹಿಂತಿರುಗಿ ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(fs, inode, clusters);
}

ಸ್ಥಿರ errcode_t ext_falloc_helper(ext2_filsys fs,
				   ಪೂರ್ಣಾಂಕ flags,
				   ext2_ino_t ino,
				   ರಚನೆ ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   ರಚನೆ ext2fs_extent *ಎಡ_ext,
				   ರಚನೆ ext2fs_extent *ಬಲ_ext,
				   blk64_t range_ಆರಂಭ, blk64_t range_len,
				   blk64_t alloc_goal)
{
	ರಚನೆ ext2fs_extent	newex, ex;
	ಪೂರ್ಣಾಂಕ			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
	ಬರೆ("%s: ", __func__);
	ಆದರೆ (ಎಡ_ext)
		ಬರೆ("left_ext=%llu--%llu, ", ಎಡ_ext->e_lblk,
		       ಎಡ_ext->e_lblk + ಎಡ_ext->e_len - 1);
	ಆದರೆ (ಬಲ_ext)
		ಬರೆ("right_ext=%llu--%llu, ", ಬಲ_ext->e_lblk,
		       ಬಲ_ext->e_lblk + ಬಲ_ext->e_len - 1);
	ಬರೆ("start=%llu len=%llu, goal=%llu\n", range_ಆರಂಭ, range_len,
	       alloc_goal);
	fflush(stdout);
#ಆದರೆ_ಅಂತ್ಯ
	/* Can't create initialized extents past EOF? */
	ಆದರೆ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_ಗಾತ್ರ(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs)) |
		     (range_ಆರಂಭ & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);

	/* We must lengthen the left extent to the end of the cluster */
	ಆದರೆ (ಎಡ_ext && EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		ಆದರೆ (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ಎಡ_ext->e_len;
		ಇಲ್ಲದಿದ್ದರೆ
			fillable = max_init_len - ಎಡ_ext->e_len;

		ಆದರೆ (fillable > range_len)
			fillable = range_len;
		ಆದರೆ (fillable == 0)
			ಹೋಗು expand_ಬಲ;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) -
			       (range_ಆರಂಭ & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs));
		cluster_fill &= EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
		ಆದರೆ (cluster_fill == 0)
			ಹೋಗು expand_ಬಲ;

		ಆದರೆ (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		ಆದರೆ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = ಎಡ_ext->e_lblk + ಎಡ_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			ಆದರೆ (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			ಆದರೆ (cluster_fill == 0)
				ಹೋಗು expand_ಬಲ;
		}

		err = ext2fs_extent_goto(handle, ಎಡ_ext->e_lblk);
		ಆದರೆ (err)
			ಹೋಗು expand_ಬಲ;
		ಎಡ_ext->e_len += cluster_fill;
		range_ಆರಂಭ += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", ಎಡ_ext);
		err = ext2fs_extent_replace(handle, 0, ಎಡ_ext);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		/* Zero blocks */
		ಆದರೆ (!(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ಎಡ_ext->e_pblk +
						  ಎಡ_ext->e_len -
						  cluster_fill, cluster_fill,
						  ಖಾಲಿ, ಖಾಲಿ);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
		}
	}

expand_ಬಲ:
	/* We must lengthen the right extent to the beginning of the cluster */
	ಆದರೆ (ಬಲ_ext && EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) > 1) {
		/* How much can we attach to right_ext? */
		ಆದರೆ (ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ಬಲ_ext->e_len;
		ಇಲ್ಲದಿದ್ದರೆ
			fillable = max_init_len - ಬಲ_ext->e_len;

		ಆದರೆ (fillable > range_len)
			fillable = range_len;
		ಆದರೆ (fillable == 0)
			ಹೋಗು try_ವಿಲೀನ;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = ಬಲ_ext->e_lblk & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
		ಆದರೆ (cluster_fill == 0)
			ಹೋಗು try_ವಿಲೀನ;

		err = ext2fs_extent_goto(handle, ಬಲ_ext->e_lblk);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (cluster_fill > fillable)
			cluster_fill = fillable;
		ಬಲ_ext->e_lblk -= cluster_fill;
		ಬಲ_ext->e_pblk -= cluster_fill;
		ಬಲ_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", ಬಲ_ext);
		err = ext2fs_extent_replace(handle, 0, ಬಲ_ext);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		/* Zero blocks if necessary */
		ಆದರೆ (!(ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ಬಲ_ext->e_pblk,
						  cluster_fill, ಖಾಲಿ, ಖಾಲಿ);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
		}
	}

try_ವಿಲೀನ:
	/* Merge both extents together, perhaps? */
	ಆದರೆ (ಎಡ_ext && ಬಲ_ext) {
		/* Are the two extents mergeable? */
		ಆದರೆ ((ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			ಹೋಗು try_ಎಡ;

		/* User requires init/uninit but extent is uninit/init. */
		ಆದರೆ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ಹೋಗು try_ಎಡ;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		ಆದರೆ (!(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			ಹೋಗು try_ಎಡ;

		/* Will it even fit? */
		x = ಎಡ_ext->e_len + range_len + ಬಲ_ext->e_len;
		ಆದರೆ (x > (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			ಹೋಗು try_ಎಡ;

		/* Would they even be physically contiguous if merged? */
		ಆದರೆ (ಎಡ_ext->e_pblk + ಎಡ_ext->e_len + range_len !=
				ಬಲ_ext->e_pblk)
			ಹೋಗು try_ಎಡ;

		err = ext2fs_extent_goto(handle, ಎಡ_ext->e_lblk);
		ಆದರೆ (err)
			ಹೋಗು try_ಎಡ;

		/* Allocate blocks */
		y = ಎಡ_ext->e_pblk + ಎಡ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_ಉದ್ದಳತೆ, y,
				       ಬಲ_ext->e_pblk - y + 1, ಖಾಲಿ,
				       &pblk, &plen);
		ಆದರೆ (err)
			ಹೋಗು try_ಎಡ;
		ಆದರೆ (pblk + plen != ಬಲ_ext->e_pblk)
			ಹೋಗು try_ಎಡ;
		err = claim_range(fs, inode, pblk, plen);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		/* Modify extents */
		ಎಡ_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", ಎಡ_ext);
		err = ext2fs_extent_replace(handle, 0, ಎಡ_ext);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ, &newex);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_delete(handle, 0);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		*ಬಲ_ext = *ಎಡ_ext;

		/* Zero blocks */
		ಆದರೆ (!(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು)) {
			err = ext2fs_zero_blocks2(fs, range_ಆರಂಭ, range_len,
						  ಖಾಲಿ, ಖಾಲಿ);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
		}

		ಹಿಂತಿರುಗಿ 0;
	}

try_ಎಡ:
	/* Extend the left extent */
	ಆದರೆ (ಎಡ_ext) {
		/* How many more blocks can be attached to left_ext? */
		ಆದರೆ (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ಎಡ_ext->e_len;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು)
			fillable = max_init_len - ಎಡ_ext->e_len;
		ಇಲ್ಲದಿದ್ದರೆ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ಆದರೆ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ಹೋಗು try_ಬಲ;

		ಆದರೆ (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = ಎಡ_ext->e_lblk + ಎಡ_ext->e_len - 1;
		ಆದರೆ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			ಆದರೆ (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		ಆದರೆ (fillable == 0)
			ಹೋಗು try_ಬಲ;

		/* Test if the right edge of the range is already mapped? */
		ಆದರೆ (EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) > 1) {
			err = ext2fs_map_cluster_ಖಂಡ(fs, ino, inode,
					x + fillable, &pblk);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
			ಆದರೆ (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs));
			ಆದರೆ (fillable == 0)
				ಹೋಗು try_ಬಲ;
		}

		/* Allocate range of blocks */
		x = ಎಡ_ext->e_pblk + ಎಡ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_ಉದ್ದಳತೆ,
				x, fillable, ಖಾಲಿ, &pblk, &plen);
		ಆದರೆ (err)
			ಹೋಗು try_ಬಲ;
		err = claim_range(fs, inode, pblk, plen);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, ಎಡ_ext->e_lblk);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		range_ಆರಂಭ += plen;
		range_len -= plen;
		ಎಡ_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", ಎಡ_ext);
		err = ext2fs_extent_replace(handle, 0, ಎಡ_ext);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		/* Zero blocks if necessary */
		ಆದರೆ (!(ಎಡ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ಖಾಲಿ, ಖಾಲಿ);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
		}
	}

try_ಬಲ:
	/* Extend the right extent */
	ಆದರೆ (ಬಲ_ext) {
		/* How much can we attach to right_ext? */
		ಆದರೆ (ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ಬಲ_ext->e_len;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು)
			fillable = max_init_len - ಬಲ_ext->e_len;
		ಇಲ್ಲದಿದ್ದರೆ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ಆದರೆ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ಹೋಗು try_anywhere;

		ಆದರೆ (fillable > range_len)
			fillable = range_len;
		ಆದರೆ (fillable == 0)
			ಹೋಗು try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		ಆದರೆ (EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) > 1) {
			err = ext2fs_map_cluster_ಖಂಡ(fs, ino, inode,
					ಬಲ_ext->e_lblk - fillable, &pblk);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
			ಆದರೆ (pblk)
				fillable -= EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) -
						((ಬಲ_ext->e_lblk - fillable)
						 & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs));
			ಆದರೆ (fillable == 0)
				ಹೋಗು try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_ಉದ್ದಳತೆ,
				ಬಲ_ext->e_pblk - fillable,
				fillable, ಖಾಲಿ, &pblk, &plen);
		ಆದರೆ (err)
			ಹೋಗು try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs),
			      plen + (pblk & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs)));
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, ಬಲ_ext->e_lblk);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		range_len -= plen;
		ಬಲ_ext->e_lblk -= plen;
		ಬಲ_ext->e_pblk -= plen;
		ಬಲ_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", ಬಲ_ext);
		err = ext2fs_extent_replace(handle, 0, ಬಲ_ext);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		/* Zero blocks if necessary */
		ಆದರೆ (!(ಬಲ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, ಖಾಲಿ, ಖಾಲಿ);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	ಆದರೆ (range_len > 0 && (range_ಆರಂಭ & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs))) {
		cluster_fill = EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) -
			       (range_ಆರಂಭ & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs));
		cluster_fill &= EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
		ಆದರೆ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_ಆರಂಭ;
		err = ext2fs_map_cluster_ಖಂಡ(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		ಆದರೆ (pblk == 0)
			ಹೋಗು try_ಬಲ_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ಆದರೆ (err == EXT2_ET_NO_CURRENT_ಗಂಟು)
			ex.e_lblk = 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ಇಲ್ಲದಿದ್ದರೆ
			op = EXT2_EXTENT_ಸೇರಿಸು_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ಸೇರಿಸು(handle, op, &newex);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ಖಾಲಿ, ಖಾಲಿ);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
		}

		range_ಆರಂಭ += cluster_fill;
		range_len -= cluster_fill;
	}

try_ಬಲ_implied:
	y = range_ಆರಂಭ + range_len;
	ಆದರೆ (range_len > 0 && (y & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs))) {
		cluster_fill = y & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
		ಆದರೆ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
		err = ext2fs_map_cluster_ಖಂಡ(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		ಆದರೆ (pblk == 0)
			ಹೋಗು no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ಆದರೆ (err == EXT2_ET_NO_CURRENT_ಗಂಟು)
			ex.e_lblk = 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ಇಲ್ಲದಿದ್ದರೆ
			op = EXT2_EXTENT_ಸೇರಿಸು_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ಸೇರಿಸು(handle, op, &newex);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ಖಾಲಿ, ಖಾಲಿ);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
		}

		range_len -= cluster_fill;
	}

no_implied:
	ಆದರೆ (range_len == 0)
		ಹಿಂತಿರುಗಿ 0;

	newex.e_lblk = range_ಆರಂಭ;
	ಆದರೆ (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	ಗಾಗಿ (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs),
				       fillable,
				       ಖಾಲಿ, &pblk, &plen);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = claim_range(fs, inode, pblk, plen);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ಆದರೆ (err == EXT2_ET_NO_CURRENT_ಗಂಟು)
			ex.e_lblk = 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ಇಲ್ಲದಿದ್ದರೆ
			op = EXT2_EXTENT_ಸೇರಿಸು_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ಸೇರಿಸು(handle, op, &newex);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		err = ext2fs_extent_fix_parents(handle);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		ಆದರೆ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ಖಾಲಿ, ಖಾಲಿ);
			ಆದರೆ (err)
				ಹೋಗು ಹೊರಹೋಗು;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		ಆದರೆ (pblk >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))
			pblk = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
	}

ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ errcode_t extent_fallocate(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, ext2_ino_t ino,
				      ರಚನೆ ext2_inode *inode, blk64_t goal,
				      blk64_t ಆರಂಭ, blk64_t len)
{
	ext2_extent_handle_t	handle;
	ರಚನೆ ext2fs_extent	ಎಡ_extent, ಬಲ_extent;
	ರಚನೆ ext2fs_extent	*ಎಡ_adjacent, *ಬಲ_adjacent;
	errcode_t		err;
	blk64_t			range_ಆರಂಭ, range_ಕೊನೆ = 0, ಕೊನೆ, ಮುಂದಿನ;
	blk64_t			ಎಣಿಕೆ_2, goal_ದೂರ;

	ಕೊನೆ = ಆರಂಭ + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	/*
	 * Find the extent closest to the start of the alloc range.  We don't
	 * check the return value because _goto() sets the current node to the
	 * next-lowest extent if 'start' is in a hole; or the next-highest
	 * extent if there aren't any lower ones; or doesn't set a current node
	 * if there was a real error reading the extent tree.  In that case,
	 * _get() will error out.
	 */
ಆರಂಭ_again:
	ext2fs_extent_goto(handle, ಆರಂಭ);
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &ಎಡ_extent);
	ಆದರೆ (err == EXT2_ET_NO_CURRENT_ಗಂಟು) {
		blk64_t max_ಖಂಡಗಳು = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super);

		ಆದರೆ (goal == ~0ULL)
			goal = ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(fs, ino, inode, ಆರಂಭ);
		err = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_ಖಂಡ_bitmap2(fs->ಖಂಡ_map,
						goal, max_ಖಂಡಗಳು - 1, &goal);
		goal += ಆರಂಭ;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ಖಾಲಿ,
					ಖಾಲಿ, ಆರಂಭ, len, goal);
		ಹೋಗು errout;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (err)
		ಹೋಗು errout;

	dbg_print_extent("ext_falloc initial", &ಎಡ_extent);
	ಮುಂದಿನ = ಎಡ_extent.e_lblk + ಎಡ_extent.e_len;
	ಆದರೆ (ಎಡ_extent.e_lblk > ಆರಂಭ) {
		/* The nearest extent we found was beyond start??? */
		goal = ಎಡ_extent.e_pblk - (ಎಡ_extent.e_lblk - ಆರಂಭ);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ಖಾಲಿ,
					&ಎಡ_extent, ಆರಂಭ,
					min(len, ಎಡ_extent.e_lblk - ಆರಂಭ),
					goal);
		ಆದರೆ (err)
			ಹೋಗು errout;

		ಹೋಗು ಆರಂಭ_again;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮುಂದಿನ >= ಆರಂಭ) {
		range_ಆರಂಭ = ಮುಂದಿನ;
		ಎಡ_adjacent = &ಎಡ_extent;
	} ಇಲ್ಲದಿದ್ದರೆ {
		range_ಆರಂಭ = ಆರಂಭ;
		ಎಡ_adjacent = ಖಾಲಿ;
	}
	goal = ಎಡ_extent.e_pblk + (range_ಆರಂಭ - ಎಡ_extent.e_lblk);

	ಮಾಡು {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ,
					   &ಬಲ_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (ಪೂರ್ಣಾಂಕ)err);
		dbg_print_extent("ext_falloc next", &ಬಲ_extent);
		/* Stop if we've seen this extent before */
		ಆದರೆ (!err && ಬಲ_extent.e_lblk <= ಎಡ_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_ಮುಂದಿನ;

		ಆದರೆ (err && err != EXT2_ET_EXTENT_NO_ಮುಂದಿನ)
			ಹೋಗು errout;
		ಆದರೆ (err == EXT2_ET_EXTENT_NO_ಮುಂದಿನ ||
		    ಬಲ_extent.e_lblk > ಕೊನೆ + 1) {
			range_ಕೊನೆ = ಕೊನೆ;
			ಬಲ_adjacent = ಖಾಲಿ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Handle right_extent.e_lblk <= end */
			range_ಕೊನೆ = ಬಲ_extent.e_lblk - 1;
			ಬಲ_adjacent = &ಬಲ_extent;
		}
		goal_ದೂರ = range_ಆರಂಭ - ಮುಂದಿನ;
		ಆದರೆ (err != EXT2_ET_EXTENT_NO_ಮುಂದಿನ &&
		    goal_ದೂರ > (range_ಕೊನೆ - ಬಲ_extent.e_lblk))
			goal = ಬಲ_extent.e_pblk -
					(ಬಲ_extent.e_lblk - range_ಆರಂಭ);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_ಆರಂಭ, range_ಕೊನೆ);
		err = 0;
		ಆದರೆ (range_ಆರಂಭ <= range_ಕೊನೆ) {
			ಎಣಿಕೆ_2 = range_ಕೊನೆ - range_ಆರಂಭ + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						ಎಡ_adjacent, ಬಲ_adjacent,
						range_ಆರಂಭ, ಎಣಿಕೆ_2, goal);
			ಆದರೆ (err)
				ಹೋಗು errout;
		}

		ಆದರೆ (range_ಕೊನೆ == ಕೊನೆ)
			ನಿಲ್ಲಿಸು;

		err = ext2fs_extent_goto(handle, ಬಲ_extent.e_lblk);
		ಆದರೆ (err)
			ಹೋಗು errout;
		ಮುಂದಿನ = ಬಲ_extent.e_lblk + ಬಲ_extent.e_len;
		ಎಡ_extent = ಬಲ_extent;
		ಎಡ_adjacent = &ಎಡ_extent;
		range_ಆರಂಭ = ಮುಂದಿನ;
		goal = ಎಡ_extent.e_pblk + (range_ಆರಂಭ - ಎಡ_extent.e_lblk);
	} ಆಗ (range_ಕೊನೆ < ಕೊನೆ);

errout:
	ext2fs_extent_free(handle);
	ಹಿಂತಿರುಗಿ err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, ext2_ino_t ino,
			   ರಚನೆ ext2_inode *inode, blk64_t goal,
			   blk64_t ಆರಂಭ, blk64_t len)
{
	ರಚನೆ ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, ಕೊನೆಯ = 0;
	ಪೂರ್ಣಾಂಕ			zero_len = 0;
	errcode_t		err;

	ಆದರೆ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (len > ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_ಖಂಡ_ALLOC_FAIL;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len == 0)
		ಹಿಂತಿರುಗಿ 0;

	/* Read inode structure if necessary */
	ಆದರೆ (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   ಆರಂಭ, len, goal);

	ಆದರೆ (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, ಆರಂಭ, len);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	ಗಾಗಿ (blk = ಆರಂಭ; blk < ಆರಂಭ + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, ಖಾಲಿ, 0, blk, 0, &x);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ err;
		ಆದರೆ (x)
			ಮುಂದುವರಿ;

		err = ext2fs_bmap2(fs, ino, inode, ಖಾಲಿ, BMAP_ALLOC,
				   blk, 0, &x);
		ಆದರೆ (err)
			ಹೋಗು errout;
		ಆದರೆ ((zero_len && (x != ಕೊನೆಯ+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  ಖಾಲಿ, ಖಾಲಿ);
			zero_len = 0;
			ಆದರೆ (err)
				ಹೋಗು errout;
		}
		ಆದರೆ (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} ಇಲ್ಲದಿದ್ದರೆ {
			zero_len++;
		}
		ಕೊನೆಯ = x;
	}

ಹೊರಹೋಗು:
	ಆದರೆ (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	ಆದರೆ (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, ಖಾಲಿ, ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ err;
}
