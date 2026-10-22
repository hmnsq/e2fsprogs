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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ min(a, b) ((a) < (b) ? (a) : (b))

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DEBUG

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
# define dbg_printf(f, a...)  ਕਰੋ {ਲਿਖੋ(f, ## a); fflush(stdout); } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
# define dbg_printf(f, a...)
#ਜੇ_ਅੰਤ

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

ਸਥਿਰ ਖਾਲੀ dbg_print_extent(ਅਚਲ ਅੱਖਰ *desc EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		ਅਚਲ ਬਣਤਰ ext2fs_extent *extent EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	ਜੇ (desc)
		ਲਿਖੋ("%s: ", desc);
	ਲਿਖੋ("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_ਪੱਤੀ)
		fputs("LEAF ", stdout);
	ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_ਦੂਜਾ_ਦੇਖੋ)
		fputs("2ND_VISIT ", stdout);
	ਜੇ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ errcode_t claim_range(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) - 1) /
		   EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs);
	ext2fs_ਖੰਡ_alloc_stats_range(fs, blk,
			clusters * EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs), +1);
	ਵਾਪਸ ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(fs, inode, clusters);
}

ਸਥਿਰ errcode_t ext_falloc_helper(ext2_filsys fs,
				   ਪੂਰਨ flags,
				   ext2_ino_t ino,
				   ਬਣਤਰ ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   ਬਣਤਰ ext2fs_extent *ਖੱਬਾ_ext,
				   ਬਣਤਰ ext2fs_extent *ਸੱਜਾ_ext,
				   blk64_t range_ਸ਼ੁਰੂ, blk64_t range_len,
				   blk64_t alloc_goal)
{
	ਬਣਤਰ ext2fs_extent	newex, ex;
	ਪੂਰਨ			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
	ਲਿਖੋ("%s: ", __func__);
	ਜੇ (ਖੱਬਾ_ext)
		ਲਿਖੋ("left_ext=%llu--%llu, ", ਖੱਬਾ_ext->e_lblk,
		       ਖੱਬਾ_ext->e_lblk + ਖੱਬਾ_ext->e_len - 1);
	ਜੇ (ਸੱਜਾ_ext)
		ਲਿਖੋ("right_ext=%llu--%llu, ", ਸੱਜਾ_ext->e_lblk,
		       ਸੱਜਾ_ext->e_lblk + ਸੱਜਾ_ext->e_len - 1);
	ਲਿਖੋ("start=%llu len=%llu, goal=%llu\n", range_ਸ਼ੁਰੂ, range_len,
	       alloc_goal);
	fflush(stdout);
#ਜੇ_ਅੰਤ
	/* Can't create initialized extents past EOF? */
	ਜੇ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_ਮਾਪ(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs)) |
		     (range_ਸ਼ੁਰੂ & EXT2_FS_CLUSTER_ਨਕਾਬ(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs);

	/* We must lengthen the left extent to the end of the cluster */
	ਜੇ (ਖੱਬਾ_ext && EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		ਜੇ (ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ਖੱਬਾ_ext->e_len;
		ਨਹੀਂਤਾਂ
			fillable = max_init_len - ਖੱਬਾ_ext->e_len;

		ਜੇ (fillable > range_len)
			fillable = range_len;
		ਜੇ (fillable == 0)
			ਜਾਓ expand_ਸੱਜਾ;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) -
			       (range_ਸ਼ੁਰੂ & EXT2_FS_CLUSTER_ਨਕਾਬ(fs));
		cluster_fill &= EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
		ਜੇ (cluster_fill == 0)
			ਜਾਓ expand_ਸੱਜਾ;

		ਜੇ (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		ਜੇ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = ਖੱਬਾ_ext->e_lblk + ਖੱਬਾ_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			ਜੇ (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			ਜੇ (cluster_fill == 0)
				ਜਾਓ expand_ਸੱਜਾ;
		}

		err = ext2fs_extent_goto(handle, ਖੱਬਾ_ext->e_lblk);
		ਜੇ (err)
			ਜਾਓ expand_ਸੱਜਾ;
		ਖੱਬਾ_ext->e_len += cluster_fill;
		range_ਸ਼ੁਰੂ += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", ਖੱਬਾ_ext);
		err = ext2fs_extent_replace(handle, 0, ਖੱਬਾ_ext);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_fix_parents(handle);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		/* Zero blocks */
		ਜੇ (!(ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ਖੱਬਾ_ext->e_pblk +
						  ਖੱਬਾ_ext->e_len -
						  cluster_fill, cluster_fill,
						  ਨਿਰਾ, ਨਿਰਾ);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

expand_ਸੱਜਾ:
	/* We must lengthen the right extent to the beginning of the cluster */
	ਜੇ (ਸੱਜਾ_ext && EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) > 1) {
		/* How much can we attach to right_ext? */
		ਜੇ (ਸੱਜਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ਸੱਜਾ_ext->e_len;
		ਨਹੀਂਤਾਂ
			fillable = max_init_len - ਸੱਜਾ_ext->e_len;

		ਜੇ (fillable > range_len)
			fillable = range_len;
		ਜੇ (fillable == 0)
			ਜਾਓ try_ਮਿਲਾਓ;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = ਸੱਜਾ_ext->e_lblk & EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
		ਜੇ (cluster_fill == 0)
			ਜਾਓ try_ਮਿਲਾਓ;

		err = ext2fs_extent_goto(handle, ਸੱਜਾ_ext->e_lblk);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (cluster_fill > fillable)
			cluster_fill = fillable;
		ਸੱਜਾ_ext->e_lblk -= cluster_fill;
		ਸੱਜਾ_ext->e_pblk -= cluster_fill;
		ਸੱਜਾ_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", ਸੱਜਾ_ext);
		err = ext2fs_extent_replace(handle, 0, ਸੱਜਾ_ext);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_fix_parents(handle);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		/* Zero blocks if necessary */
		ਜੇ (!(ਸੱਜਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ਸੱਜਾ_ext->e_pblk,
						  cluster_fill, ਨਿਰਾ, ਨਿਰਾ);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

try_ਮਿਲਾਓ:
	/* Merge both extents together, perhaps? */
	ਜੇ (ਖੱਬਾ_ext && ਸੱਜਾ_ext) {
		/* Are the two extents mergeable? */
		ਜੇ ((ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (ਸੱਜਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			ਜਾਓ try_ਖੱਬਾ;

		/* User requires init/uninit but extent is uninit/init. */
		ਜੇ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ਜਾਓ try_ਖੱਬਾ;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		ਜੇ (!(ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			ਜਾਓ try_ਖੱਬਾ;

		/* Will it even fit? */
		x = ਖੱਬਾ_ext->e_len + range_len + ਸੱਜਾ_ext->e_len;
		ਜੇ (x > (ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			ਜਾਓ try_ਖੱਬਾ;

		/* Would they even be physically contiguous if merged? */
		ਜੇ (ਖੱਬਾ_ext->e_pblk + ਖੱਬਾ_ext->e_len + range_len !=
				ਸੱਜਾ_ext->e_pblk)
			ਜਾਓ try_ਖੱਬਾ;

		err = ext2fs_extent_goto(handle, ਖੱਬਾ_ext->e_lblk);
		ਜੇ (err)
			ਜਾਓ try_ਖੱਬਾ;

		/* Allocate blocks */
		y = ਖੱਬਾ_ext->e_pblk + ਖੱਬਾ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_ਲੰਮਾਈ, y,
				       ਸੱਜਾ_ext->e_pblk - y + 1, ਨਿਰਾ,
				       &pblk, &plen);
		ਜੇ (err)
			ਜਾਓ try_ਖੱਬਾ;
		ਜੇ (pblk + plen != ਸੱਜਾ_ext->e_pblk)
			ਜਾਓ try_ਖੱਬਾ;
		err = claim_range(fs, inode, pblk, plen);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		/* Modify extents */
		ਖੱਬਾ_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", ਖੱਬਾ_ext);
		err = ext2fs_extent_replace(handle, 0, ਖੱਬਾ_ext);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_fix_parents(handle);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ, &newex);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_delete(handle, 0);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_fix_parents(handle);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		*ਸੱਜਾ_ext = *ਖੱਬਾ_ext;

		/* Zero blocks */
		ਜੇ (!(ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ)) {
			err = ext2fs_zero_blocks2(fs, range_ਸ਼ੁਰੂ, range_len,
						  ਨਿਰਾ, ਨਿਰਾ);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਵਾਪਸ 0;
	}

try_ਖੱਬਾ:
	/* Extend the left extent */
	ਜੇ (ਖੱਬਾ_ext) {
		/* How many more blocks can be attached to left_ext? */
		ਜੇ (ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ਖੱਬਾ_ext->e_len;
		ਨਹੀਂਤਾਂ ਜੇ (flags & EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ)
			fillable = max_init_len - ਖੱਬਾ_ext->e_len;
		ਨਹੀਂਤਾਂ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ਜੇ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ਜਾਓ try_ਸੱਜਾ;

		ਜੇ (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = ਖੱਬਾ_ext->e_lblk + ਖੱਬਾ_ext->e_len - 1;
		ਜੇ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			ਜੇ (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		ਜੇ (fillable == 0)
			ਜਾਓ try_ਸੱਜਾ;

		/* Test if the right edge of the range is already mapped? */
		ਜੇ (EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) > 1) {
			err = ext2fs_map_cluster_ਖੰਡ(fs, ino, inode,
					x + fillable, &pblk);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
			ਜੇ (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2_FS_CLUSTER_ਨਕਾਬ(fs));
			ਜੇ (fillable == 0)
				ਜਾਓ try_ਸੱਜਾ;
		}

		/* Allocate range of blocks */
		x = ਖੱਬਾ_ext->e_pblk + ਖੱਬਾ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_ਲੰਮਾਈ,
				x, fillable, ਨਿਰਾ, &pblk, &plen);
		ਜੇ (err)
			ਜਾਓ try_ਸੱਜਾ;
		err = claim_range(fs, inode, pblk, plen);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, ਖੱਬਾ_ext->e_lblk);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		range_ਸ਼ੁਰੂ += plen;
		range_len -= plen;
		ਖੱਬਾ_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", ਖੱਬਾ_ext);
		err = ext2fs_extent_replace(handle, 0, ਖੱਬਾ_ext);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_fix_parents(handle);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		/* Zero blocks if necessary */
		ਜੇ (!(ਖੱਬਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ਨਿਰਾ, ਨਿਰਾ);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

try_ਸੱਜਾ:
	/* Extend the right extent */
	ਜੇ (ਸੱਜਾ_ext) {
		/* How much can we attach to right_ext? */
		ਜੇ (ਸੱਜਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ਸੱਜਾ_ext->e_len;
		ਨਹੀਂਤਾਂ ਜੇ (flags & EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ)
			fillable = max_init_len - ਸੱਜਾ_ext->e_len;
		ਨਹੀਂਤਾਂ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ਜੇ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ਸੱਜਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ਸੱਜਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ਜਾਓ try_anywhere;

		ਜੇ (fillable > range_len)
			fillable = range_len;
		ਜੇ (fillable == 0)
			ਜਾਓ try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		ਜੇ (EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) > 1) {
			err = ext2fs_map_cluster_ਖੰਡ(fs, ino, inode,
					ਸੱਜਾ_ext->e_lblk - fillable, &pblk);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
			ਜੇ (pblk)
				fillable -= EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) -
						((ਸੱਜਾ_ext->e_lblk - fillable)
						 & EXT2_FS_CLUSTER_ਨਕਾਬ(fs));
			ਜੇ (fillable == 0)
				ਜਾਓ try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_ਲੰਮਾਈ,
				ਸੱਜਾ_ext->e_pblk - fillable,
				fillable, ਨਿਰਾ, &pblk, &plen);
		ਜੇ (err)
			ਜਾਓ try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs),
			      plen + (pblk & EXT2_FS_CLUSTER_ਨਕਾਬ(fs)));
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, ਸੱਜਾ_ext->e_lblk);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		range_len -= plen;
		ਸੱਜਾ_ext->e_lblk -= plen;
		ਸੱਜਾ_ext->e_pblk -= plen;
		ਸੱਜਾ_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", ਸੱਜਾ_ext);
		err = ext2fs_extent_replace(handle, 0, ਸੱਜਾ_ext);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_fix_parents(handle);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		/* Zero blocks if necessary */
		ਜੇ (!(ਸੱਜਾ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, ਨਿਰਾ, ਨਿਰਾ);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	ਜੇ (range_len > 0 && (range_ਸ਼ੁਰੂ & EXT2_FS_CLUSTER_ਨਕਾਬ(fs))) {
		cluster_fill = EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) -
			       (range_ਸ਼ੁਰੂ & EXT2_FS_CLUSTER_ਨਕਾਬ(fs));
		cluster_fill &= EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
		ਜੇ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_ਸ਼ੁਰੂ;
		err = ext2fs_map_cluster_ਖੰਡ(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		ਜੇ (pblk == 0)
			ਜਾਓ try_ਸੱਜਾ_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ਜੇ (err == EXT2_ET_NO_CURRENT_ਗੰਢ)
			ex.e_lblk = 0;
		ਨਹੀਂਤਾਂ ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ਨਹੀਂਤਾਂ
			op = EXT2_EXTENT_ਪਾਓ_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ਪਾਓ(handle, op, &newex);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_fix_parents(handle);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ਨਿਰਾ, ਨਿਰਾ);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
		}

		range_ਸ਼ੁਰੂ += cluster_fill;
		range_len -= cluster_fill;
	}

try_ਸੱਜਾ_implied:
	y = range_ਸ਼ੁਰੂ + range_len;
	ਜੇ (range_len > 0 && (y & EXT2_FS_CLUSTER_ਨਕਾਬ(fs))) {
		cluster_fill = y & EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
		ਜੇ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
		err = ext2fs_map_cluster_ਖੰਡ(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		ਜੇ (pblk == 0)
			ਜਾਓ no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ਜੇ (err == EXT2_ET_NO_CURRENT_ਗੰਢ)
			ex.e_lblk = 0;
		ਨਹੀਂਤਾਂ ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ਨਹੀਂਤਾਂ
			op = EXT2_EXTENT_ਪਾਓ_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ਪਾਓ(handle, op, &newex);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_fix_parents(handle);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ਨਿਰਾ, ਨਿਰਾ);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
		}

		range_len -= cluster_fill;
	}

no_implied:
	ਜੇ (range_len == 0)
		ਵਾਪਸ 0;

	newex.e_lblk = range_ਸ਼ੁਰੂ;
	ਜੇ (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} ਨਹੀਂਤਾਂ {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	ਲਈ (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs),
				       fillable,
				       ਨਿਰਾ, &pblk, &plen);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = claim_range(fs, inode, pblk, plen);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ਜੇ (err == EXT2_ET_NO_CURRENT_ਗੰਢ)
			ex.e_lblk = 0;
		ਨਹੀਂਤਾਂ ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ਨਹੀਂਤਾਂ
			op = EXT2_EXTENT_ਪਾਓ_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ਪਾਓ(handle, op, &newex);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		err = ext2fs_extent_fix_parents(handle);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		ਜੇ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ਨਿਰਾ, ਨਿਰਾ);
			ਜੇ (err)
				ਜਾਓ ਬਾਹਰਲਾ;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		ਜੇ (pblk >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))
			pblk = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
	}

ਬਾਹਰਲਾ:
	ਵਾਪਸ err;
}

ਸਥਿਰ errcode_t extent_fallocate(ext2_filsys fs, ਪੂਰਨ flags, ext2_ino_t ino,
				      ਬਣਤਰ ext2_inode *inode, blk64_t goal,
				      blk64_t ਸ਼ੁਰੂ, blk64_t len)
{
	ext2_extent_handle_t	handle;
	ਬਣਤਰ ext2fs_extent	ਖੱਬਾ_extent, ਸੱਜਾ_extent;
	ਬਣਤਰ ext2fs_extent	*ਖੱਬਾ_adjacent, *ਸੱਜਾ_adjacent;
	errcode_t		err;
	blk64_t			range_ਸ਼ੁਰੂ, range_ਸਿਰਾ = 0, ਸਿਰਾ, ਅਗਲਾ;
	blk64_t			ਗਿਣਤੀ_2, goal_ਦੂਰੀ;

	ਸਿਰਾ = ਸ਼ੁਰੂ + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	ਜੇ (err)
		ਵਾਪਸ err;

	/*
	 * Find the extent closest to the start of the alloc range.  We don't
	 * check the return value because _goto() sets the current node to the
	 * next-lowest extent if 'start' is in a hole; or the next-highest
	 * extent if there aren't any lower ones; or doesn't set a current node
	 * if there was a real error reading the extent tree.  In that case,
	 * _get() will error out.
	 */
ਸ਼ੁਰੂ_again:
	ext2fs_extent_goto(handle, ਸ਼ੁਰੂ);
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &ਖੱਬਾ_extent);
	ਜੇ (err == EXT2_ET_NO_CURRENT_ਗੰਢ) {
		blk64_t max_ਖੰਡਾਂ = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super);

		ਜੇ (goal == ~0ULL)
			goal = ext2fs_ਲੱਭੋ_inode_goal(fs, ino, inode, ਸ਼ੁਰੂ);
		err = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_ਖੰਡ_bitmap2(fs->ਖੰਡ_map,
						goal, max_ਖੰਡਾਂ - 1, &goal);
		goal += ਸ਼ੁਰੂ;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ਨਿਰਾ,
					ਨਿਰਾ, ਸ਼ੁਰੂ, len, goal);
		ਜਾਓ errout;
	} ਨਹੀਂਤਾਂ ਜੇ (err)
		ਜਾਓ errout;

	dbg_print_extent("ext_falloc initial", &ਖੱਬਾ_extent);
	ਅਗਲਾ = ਖੱਬਾ_extent.e_lblk + ਖੱਬਾ_extent.e_len;
	ਜੇ (ਖੱਬਾ_extent.e_lblk > ਸ਼ੁਰੂ) {
		/* The nearest extent we found was beyond start??? */
		goal = ਖੱਬਾ_extent.e_pblk - (ਖੱਬਾ_extent.e_lblk - ਸ਼ੁਰੂ);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ਨਿਰਾ,
					&ਖੱਬਾ_extent, ਸ਼ੁਰੂ,
					min(len, ਖੱਬਾ_extent.e_lblk - ਸ਼ੁਰੂ),
					goal);
		ਜੇ (err)
			ਜਾਓ errout;

		ਜਾਓ ਸ਼ੁਰੂ_again;
	} ਨਹੀਂਤਾਂ ਜੇ (ਅਗਲਾ >= ਸ਼ੁਰੂ) {
		range_ਸ਼ੁਰੂ = ਅਗਲਾ;
		ਖੱਬਾ_adjacent = &ਖੱਬਾ_extent;
	} ਨਹੀਂਤਾਂ {
		range_ਸ਼ੁਰੂ = ਸ਼ੁਰੂ;
		ਖੱਬਾ_adjacent = ਨਿਰਾ;
	}
	goal = ਖੱਬਾ_extent.e_pblk + (range_ਸ਼ੁਰੂ - ਖੱਬਾ_extent.e_lblk);

	ਕਰੋ {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ,
					   &ਸੱਜਾ_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (ਪੂਰਨ)err);
		dbg_print_extent("ext_falloc next", &ਸੱਜਾ_extent);
		/* Stop if we've seen this extent before */
		ਜੇ (!err && ਸੱਜਾ_extent.e_lblk <= ਖੱਬਾ_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_ਅਗਲਾ;

		ਜੇ (err && err != EXT2_ET_EXTENT_NO_ਅਗਲਾ)
			ਜਾਓ errout;
		ਜੇ (err == EXT2_ET_EXTENT_NO_ਅਗਲਾ ||
		    ਸੱਜਾ_extent.e_lblk > ਸਿਰਾ + 1) {
			range_ਸਿਰਾ = ਸਿਰਾ;
			ਸੱਜਾ_adjacent = ਨਿਰਾ;
		} ਨਹੀਂਤਾਂ {
			/* Handle right_extent.e_lblk <= end */
			range_ਸਿਰਾ = ਸੱਜਾ_extent.e_lblk - 1;
			ਸੱਜਾ_adjacent = &ਸੱਜਾ_extent;
		}
		goal_ਦੂਰੀ = range_ਸ਼ੁਰੂ - ਅਗਲਾ;
		ਜੇ (err != EXT2_ET_EXTENT_NO_ਅਗਲਾ &&
		    goal_ਦੂਰੀ > (range_ਸਿਰਾ - ਸੱਜਾ_extent.e_lblk))
			goal = ਸੱਜਾ_extent.e_pblk -
					(ਸੱਜਾ_extent.e_lblk - range_ਸ਼ੁਰੂ);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_ਸ਼ੁਰੂ, range_ਸਿਰਾ);
		err = 0;
		ਜੇ (range_ਸ਼ੁਰੂ <= range_ਸਿਰਾ) {
			ਗਿਣਤੀ_2 = range_ਸਿਰਾ - range_ਸ਼ੁਰੂ + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						ਖੱਬਾ_adjacent, ਸੱਜਾ_adjacent,
						range_ਸ਼ੁਰੂ, ਗਿਣਤੀ_2, goal);
			ਜੇ (err)
				ਜਾਓ errout;
		}

		ਜੇ (range_ਸਿਰਾ == ਸਿਰਾ)
			ਰੋਕੋ;

		err = ext2fs_extent_goto(handle, ਸੱਜਾ_extent.e_lblk);
		ਜੇ (err)
			ਜਾਓ errout;
		ਅਗਲਾ = ਸੱਜਾ_extent.e_lblk + ਸੱਜਾ_extent.e_len;
		ਖੱਬਾ_extent = ਸੱਜਾ_extent;
		ਖੱਬਾ_adjacent = &ਖੱਬਾ_extent;
		range_ਸ਼ੁਰੂ = ਅਗਲਾ;
		goal = ਖੱਬਾ_extent.e_pblk + (range_ਸ਼ੁਰੂ - ਖੱਬਾ_extent.e_lblk);
	} ਜਦੋਂ (range_ਸਿਰਾ < ਸਿਰਾ);

errout:
	ext2fs_extent_free(handle);
	ਵਾਪਸ err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, ਪੂਰਨ flags, ext2_ino_t ino,
			   ਬਣਤਰ ext2_inode *inode, blk64_t goal,
			   blk64_t ਸ਼ੁਰੂ, blk64_t len)
{
	ਬਣਤਰ ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, ਆਖਰੀ = 0;
	ਪੂਰਨ			zero_len = 0;
	errcode_t		err;

	ਜੇ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (len > ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))
		ਵਾਪਸ EXT2_ET_ਖੰਡ_ALLOC_FAIL;
	ਨਹੀਂਤਾਂ ਜੇ (len == 0)
		ਵਾਪਸ 0;

	/* Read inode structure if necessary */
	ਜੇ (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		ਜੇ (err)
			ਵਾਪਸ err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   ਸ਼ੁਰੂ, len, goal);

	ਜੇ (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, ਸ਼ੁਰੂ, len);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	ਲਈ (blk = ਸ਼ੁਰੂ; blk < ਸ਼ੁਰੂ + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, ਨਿਰਾ, 0, blk, 0, &x);
		ਜੇ (err)
			ਵਾਪਸ err;
		ਜੇ (x)
			ਜਾਰੀ;

		err = ext2fs_bmap2(fs, ino, inode, ਨਿਰਾ, BMAP_ALLOC,
				   blk, 0, &x);
		ਜੇ (err)
			ਜਾਓ errout;
		ਜੇ ((zero_len && (x != ਆਖਰੀ+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  ਨਿਰਾ, ਨਿਰਾ);
			zero_len = 0;
			ਜੇ (err)
				ਜਾਓ errout;
		}
		ਜੇ (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} ਨਹੀਂਤਾਂ {
			zero_len++;
		}
		ਆਖਰੀ = x;
	}

ਬਾਹਰਲਾ:
	ਜੇ (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	ਜੇ (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, ਨਿਰਾ, ਨਿਰਾ);
	ਵਾਪਸ err;
}
