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

#شامل "config.h"

#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#تعریف_کرو min(a, b) ((a) < (b) ? (a) : (b))

#تعریف_منسوخ DEBUG

#اگر_تعریف_شدہ DEBUG
# define dbg_printf(f, a...)  کرو {لکھو(f, ## a); fflush(stdout); } جب (0)
#ورنہ
# define dbg_printf(f, a...)
#اگر_ختم

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

ساکن خالی dbg_print_extent(ثابت حرف *desc EXT2FS_ATTR((غیر_مستعمل)),
		ثابت ساخت ext2fs_extent *extent EXT2FS_ATTR((غیر_مستعمل)))
{
#اگر_تعریف_شدہ DEBUG
	اگر (desc)
		لکھو("%s: ", desc);
	لکھو("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_برگ)
		fputs("LEAF ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_دوسرا_دورہ)
		fputs("2ND_VISIT ", stdout);
	اگر (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#اگر_ختم
}

ساکن errcode_t claim_range(ext2_filsys fs, ساخت ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2_FS_CLUSTER_تناسب(fs) - 1) /
		   EXT2_FS_CLUSTER_تناسب(fs);
	ext2fs_بلاک_alloc_stats_range(fs, blk,
			clusters * EXT2_FS_CLUSTER_تناسب(fs), +1);
	واپس ext2fs_iblk_جوڑو_بلاکیں(fs, inode, clusters);
}

ساکن errcode_t ext_falloc_helper(ext2_filsys fs,
				   صحیح flags,
				   ext2_ino_t ino,
				   ساخت ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   ساخت ext2fs_extent *بایاں_ext,
				   ساخت ext2fs_extent *دایاں_ext,
				   blk64_t range_آغاز, blk64_t range_len,
				   blk64_t alloc_goal)
{
	ساخت ext2fs_extent	newex, ex;
	صحیح			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#اگر_تعریف_شدہ DEBUG
	لکھو("%s: ", __func__);
	اگر (بایاں_ext)
		لکھو("left_ext=%llu--%llu, ", بایاں_ext->e_lblk,
		       بایاں_ext->e_lblk + بایاں_ext->e_len - 1);
	اگر (دایاں_ext)
		لکھو("right_ext=%llu--%llu, ", دایاں_ext->e_lblk,
		       دایاں_ext->e_lblk + دایاں_ext->e_len - 1);
	لکھو("start=%llu len=%llu, goal=%llu\n", range_آغاز, range_len,
	       alloc_goal);
	fflush(stdout);
#اگر_ختم
	/* Can't create initialized extents past EOF? */
	اگر (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_سائز(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2_FS_CLUSTER_نقاب(fs)) |
		     (range_آغاز & EXT2_FS_CLUSTER_نقاب(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2_FS_CLUSTER_نقاب(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2_FS_CLUSTER_نقاب(fs);

	/* We must lengthen the left extent to the end of the cluster */
	اگر (بایاں_ext && EXT2_FS_CLUSTER_تناسب(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		اگر (بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - بایاں_ext->e_len;
		ورنہ
			fillable = max_init_len - بایاں_ext->e_len;

		اگر (fillable > range_len)
			fillable = range_len;
		اگر (fillable == 0)
			جاؤ expand_دایاں;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2_FS_CLUSTER_تناسب(fs) -
			       (range_آغاز & EXT2_FS_CLUSTER_نقاب(fs));
		cluster_fill &= EXT2_FS_CLUSTER_نقاب(fs);
		اگر (cluster_fill == 0)
			جاؤ expand_دایاں;

		اگر (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		اگر (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = بایاں_ext->e_lblk + بایاں_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			اگر (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			اگر (cluster_fill == 0)
				جاؤ expand_دایاں;
		}

		err = ext2fs_extent_goto(handle, بایاں_ext->e_lblk);
		اگر (err)
			جاؤ expand_دایاں;
		بایاں_ext->e_len += cluster_fill;
		range_آغاز += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", بایاں_ext);
		err = ext2fs_extent_replace(handle, 0, بایاں_ext);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			جاؤ برآمد;

		/* Zero blocks */
		اگر (!(بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, بایاں_ext->e_pblk +
						  بایاں_ext->e_len -
						  cluster_fill, cluster_fill,
						  تہی, تہی);
			اگر (err)
				جاؤ برآمد;
		}
	}

expand_دایاں:
	/* We must lengthen the right extent to the beginning of the cluster */
	اگر (دایاں_ext && EXT2_FS_CLUSTER_تناسب(fs) > 1) {
		/* How much can we attach to right_ext? */
		اگر (دایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - دایاں_ext->e_len;
		ورنہ
			fillable = max_init_len - دایاں_ext->e_len;

		اگر (fillable > range_len)
			fillable = range_len;
		اگر (fillable == 0)
			جاؤ try_ملاؤ;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = دایاں_ext->e_lblk & EXT2_FS_CLUSTER_نقاب(fs);
		اگر (cluster_fill == 0)
			جاؤ try_ملاؤ;

		err = ext2fs_extent_goto(handle, دایاں_ext->e_lblk);
		اگر (err)
			جاؤ برآمد;

		اگر (cluster_fill > fillable)
			cluster_fill = fillable;
		دایاں_ext->e_lblk -= cluster_fill;
		دایاں_ext->e_pblk -= cluster_fill;
		دایاں_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", دایاں_ext);
		err = ext2fs_extent_replace(handle, 0, دایاں_ext);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			جاؤ برآمد;

		/* Zero blocks if necessary */
		اگر (!(دایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, دایاں_ext->e_pblk,
						  cluster_fill, تہی, تہی);
			اگر (err)
				جاؤ برآمد;
		}
	}

try_ملاؤ:
	/* Merge both extents together, perhaps? */
	اگر (بایاں_ext && دایاں_ext) {
		/* Are the two extents mergeable? */
		اگر ((بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (دایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			جاؤ try_بایاں;

		/* User requires init/uninit but extent is uninit/init. */
		اگر (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			جاؤ try_بایاں;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		اگر (!(بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_بلاکیں) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			جاؤ try_بایاں;

		/* Will it even fit? */
		x = بایاں_ext->e_len + range_len + دایاں_ext->e_len;
		اگر (x > (بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			جاؤ try_بایاں;

		/* Would they even be physically contiguous if merged? */
		اگر (بایاں_ext->e_pblk + بایاں_ext->e_len + range_len !=
				دایاں_ext->e_pblk)
			جاؤ try_بایاں;

		err = ext2fs_extent_goto(handle, بایاں_ext->e_lblk);
		اگر (err)
			جاؤ try_بایاں;

		/* Allocate blocks */
		y = بایاں_ext->e_pblk + بایاں_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_لمبائی, y,
				       دایاں_ext->e_pblk - y + 1, تہی,
				       &pblk, &plen);
		اگر (err)
			جاؤ try_بایاں;
		اگر (pblk + plen != دایاں_ext->e_pblk)
			جاؤ try_بایاں;
		err = claim_range(fs, inode, pblk, plen);
		اگر (err)
			جاؤ برآمد;

		/* Modify extents */
		بایاں_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", بایاں_ext);
		err = ext2fs_extent_replace(handle, 0, بایاں_ext);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا_برگ, &newex);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_delete(handle, 0);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			جاؤ برآمد;
		*دایاں_ext = *بایاں_ext;

		/* Zero blocks */
		اگر (!(بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاکیں)) {
			err = ext2fs_zero_blocks2(fs, range_آغاز, range_len,
						  تہی, تہی);
			اگر (err)
				جاؤ برآمد;
		}

		واپس 0;
	}

try_بایاں:
	/* Extend the left extent */
	اگر (بایاں_ext) {
		/* How many more blocks can be attached to left_ext? */
		اگر (بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - بایاں_ext->e_len;
		ورنہ اگر (flags & EXT2_FALLOCATE_ZERO_بلاکیں)
			fillable = max_init_len - بایاں_ext->e_len;
		ورنہ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		اگر (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			جاؤ try_دایاں;

		اگر (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = بایاں_ext->e_lblk + بایاں_ext->e_len - 1;
		اگر (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			اگر (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		اگر (fillable == 0)
			جاؤ try_دایاں;

		/* Test if the right edge of the range is already mapped? */
		اگر (EXT2_FS_CLUSTER_تناسب(fs) > 1) {
			err = ext2fs_map_cluster_بلاک(fs, ino, inode,
					x + fillable, &pblk);
			اگر (err)
				جاؤ برآمد;
			اگر (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2_FS_CLUSTER_نقاب(fs));
			اگر (fillable == 0)
				جاؤ try_دایاں;
		}

		/* Allocate range of blocks */
		x = بایاں_ext->e_pblk + بایاں_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_لمبائی,
				x, fillable, تہی, &pblk, &plen);
		اگر (err)
			جاؤ try_دایاں;
		err = claim_range(fs, inode, pblk, plen);
		اگر (err)
			جاؤ برآمد;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, بایاں_ext->e_lblk);
		اگر (err)
			جاؤ برآمد;
		range_آغاز += plen;
		range_len -= plen;
		بایاں_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", بایاں_ext);
		err = ext2fs_extent_replace(handle, 0, بایاں_ext);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			جاؤ برآمد;

		/* Zero blocks if necessary */
		اگر (!(بایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاکیں)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, تہی, تہی);
			اگر (err)
				جاؤ برآمد;
		}
	}

try_دایاں:
	/* Extend the right extent */
	اگر (دایاں_ext) {
		/* How much can we attach to right_ext? */
		اگر (دایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - دایاں_ext->e_len;
		ورنہ اگر (flags & EXT2_FALLOCATE_ZERO_بلاکیں)
			fillable = max_init_len - دایاں_ext->e_len;
		ورنہ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		اگر (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (دایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(دایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			جاؤ try_anywhere;

		اگر (fillable > range_len)
			fillable = range_len;
		اگر (fillable == 0)
			جاؤ try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		اگر (EXT2_FS_CLUSTER_تناسب(fs) > 1) {
			err = ext2fs_map_cluster_بلاک(fs, ino, inode,
					دایاں_ext->e_lblk - fillable, &pblk);
			اگر (err)
				جاؤ برآمد;
			اگر (pblk)
				fillable -= EXT2_FS_CLUSTER_تناسب(fs) -
						((دایاں_ext->e_lblk - fillable)
						 & EXT2_FS_CLUSTER_نقاب(fs));
			اگر (fillable == 0)
				جاؤ try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_لمبائی,
				دایاں_ext->e_pblk - fillable,
				fillable, تہی, &pblk, &plen);
		اگر (err)
			جاؤ try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2_FS_CLUSTER_نقاب(fs),
			      plen + (pblk & EXT2_FS_CLUSTER_نقاب(fs)));
		اگر (err)
			جاؤ برآمد;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, دایاں_ext->e_lblk);
		اگر (err)
			جاؤ برآمد;
		range_len -= plen;
		دایاں_ext->e_lblk -= plen;
		دایاں_ext->e_pblk -= plen;
		دایاں_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", دایاں_ext);
		err = ext2fs_extent_replace(handle, 0, دایاں_ext);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			جاؤ برآمد;

		/* Zero blocks if necessary */
		اگر (!(دایاں_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاکیں)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, تہی, تہی);
			اگر (err)
				جاؤ برآمد;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	اگر (range_len > 0 && (range_آغاز & EXT2_FS_CLUSTER_نقاب(fs))) {
		cluster_fill = EXT2_FS_CLUSTER_تناسب(fs) -
			       (range_آغاز & EXT2_FS_CLUSTER_نقاب(fs));
		cluster_fill &= EXT2_FS_CLUSTER_نقاب(fs);
		اگر (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_آغاز;
		err = ext2fs_map_cluster_بلاک(fs, ino, inode, newex.e_lblk,
					       &pblk);
		اگر (err)
			جاؤ برآمد;
		اگر (pblk == 0)
			جاؤ try_دایاں_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		اگر (err == EXT2_ET_NO_CURRENT_گرہ)
			ex.e_lblk = 0;
		ورنہ اگر (err)
			جاؤ برآمد;

		اگر (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ورنہ
			op = EXT2_EXTENT_داخل_کرو_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_داخل_کرو(handle, op, &newex);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			جاؤ برآمد;

		اگر (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاکیں)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, تہی, تہی);
			اگر (err)
				جاؤ برآمد;
		}

		range_آغاز += cluster_fill;
		range_len -= cluster_fill;
	}

try_دایاں_implied:
	y = range_آغاز + range_len;
	اگر (range_len > 0 && (y & EXT2_FS_CLUSTER_نقاب(fs))) {
		cluster_fill = y & EXT2_FS_CLUSTER_نقاب(fs);
		اگر (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2_FS_CLUSTER_نقاب(fs);
		err = ext2fs_map_cluster_بلاک(fs, ino, inode, newex.e_lblk,
					       &pblk);
		اگر (err)
			جاؤ برآمد;
		اگر (pblk == 0)
			جاؤ no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		اگر (err == EXT2_ET_NO_CURRENT_گرہ)
			ex.e_lblk = 0;
		ورنہ اگر (err)
			جاؤ برآمد;

		اگر (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ورنہ
			op = EXT2_EXTENT_داخل_کرو_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_داخل_کرو(handle, op, &newex);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			جاؤ برآمد;

		اگر (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاکیں)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, تہی, تہی);
			اگر (err)
				جاؤ برآمد;
		}

		range_len -= cluster_fill;
	}

no_implied:
	اگر (range_len == 0)
		واپس 0;

	newex.e_lblk = range_آغاز;
	اگر (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} ورنہ {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	برائے (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2_FS_CLUSTER_نقاب(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2_FS_CLUSTER_نقاب(fs),
				       fillable,
				       تہی, &pblk, &plen);
		اگر (err)
			جاؤ برآمد;
		err = claim_range(fs, inode, pblk, plen);
		اگر (err)
			جاؤ برآمد;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		اگر (err == EXT2_ET_NO_CURRENT_گرہ)
			ex.e_lblk = 0;
		ورنہ اگر (err)
			جاؤ برآمد;

		اگر (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ورنہ
			op = EXT2_EXTENT_داخل_کرو_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_داخل_کرو(handle, op, &newex);
		اگر (err)
			جاؤ برآمد;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			جاؤ برآمد;

		اگر (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاکیں)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, تہی, تہی);
			اگر (err)
				جاؤ برآمد;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		اگر (pblk >= ext2fs_بلاکیں_گنتی(fs->super))
			pblk = fs->super->s_پہلا_ڈیٹا_بلاک;
	}

برآمد:
	واپس err;
}

ساکن errcode_t extent_fallocate(ext2_filsys fs, صحیح flags, ext2_ino_t ino,
				      ساخت ext2_inode *inode, blk64_t goal,
				      blk64_t آغاز, blk64_t len)
{
	ext2_extent_handle_t	handle;
	ساخت ext2fs_extent	بایاں_extent, دایاں_extent;
	ساخت ext2fs_extent	*بایاں_adjacent, *دایاں_adjacent;
	errcode_t		err;
	blk64_t			range_آغاز, range_انتہا = 0, انتہا, اگلا;
	blk64_t			گنتی, goal_فاصلہ;

	انتہا = آغاز + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	اگر (err)
		واپس err;

	/*
	 * Find the extent closest to the start of the alloc range.  We don't
	 * check the return value because _goto() sets the current node to the
	 * next-lowest extent if 'start' is in a hole; or the next-highest
	 * extent if there aren't any lower ones; or doesn't set a current node
	 * if there was a real error reading the extent tree.  In that case,
	 * _get() will error out.
	 */
آغاز_again:
	ext2fs_extent_goto(handle, آغاز);
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &بایاں_extent);
	اگر (err == EXT2_ET_NO_CURRENT_گرہ) {
		blk64_t max_بلاکیں = ext2fs_بلاکیں_گنتی(fs->super);

		اگر (goal == ~0ULL)
			goal = ext2fs_ڈھونڈو_inode_goal(fs, ino, inode, آغاز);
		err = ext2fs_ڈھونڈو_پہلا_zero_بلاک_bitmap2(fs->بلاک_map,
						goal, max_بلاکیں - 1, &goal);
		goal += آغاز;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, تہی,
					تہی, آغاز, len, goal);
		جاؤ errout;
	} ورنہ اگر (err)
		جاؤ errout;

	dbg_print_extent("ext_falloc initial", &بایاں_extent);
	اگلا = بایاں_extent.e_lblk + بایاں_extent.e_len;
	اگر (بایاں_extent.e_lblk > آغاز) {
		/* The nearest extent we found was beyond start??? */
		goal = بایاں_extent.e_pblk - (بایاں_extent.e_lblk - آغاز);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, تہی,
					&بایاں_extent, آغاز,
					min(len, بایاں_extent.e_lblk - آغاز),
					goal);
		اگر (err)
			جاؤ errout;

		جاؤ آغاز_again;
	} ورنہ اگر (اگلا >= آغاز) {
		range_آغاز = اگلا;
		بایاں_adjacent = &بایاں_extent;
	} ورنہ {
		range_آغاز = آغاز;
		بایاں_adjacent = تہی;
	}
	goal = بایاں_extent.e_pblk + (range_آغاز - بایاں_extent.e_lblk);

	کرو {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_اگلا_برگ,
					   &دایاں_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (صحیح)err);
		dbg_print_extent("ext_falloc next", &دایاں_extent);
		/* Stop if we've seen this extent before */
		اگر (!err && دایاں_extent.e_lblk <= بایاں_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_اگلا;

		اگر (err && err != EXT2_ET_EXTENT_NO_اگلا)
			جاؤ errout;
		اگر (err == EXT2_ET_EXTENT_NO_اگلا ||
		    دایاں_extent.e_lblk > انتہا + 1) {
			range_انتہا = انتہا;
			دایاں_adjacent = تہی;
		} ورنہ {
			/* Handle right_extent.e_lblk <= end */
			range_انتہا = دایاں_extent.e_lblk - 1;
			دایاں_adjacent = &دایاں_extent;
		}
		goal_فاصلہ = range_آغاز - اگلا;
		اگر (err != EXT2_ET_EXTENT_NO_اگلا &&
		    goal_فاصلہ > (range_انتہا - دایاں_extent.e_lblk))
			goal = دایاں_extent.e_pblk -
					(دایاں_extent.e_lblk - range_آغاز);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_آغاز, range_انتہا);
		err = 0;
		اگر (range_آغاز <= range_انتہا) {
			گنتی = range_انتہا - range_آغاز + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						بایاں_adjacent, دایاں_adjacent,
						range_آغاز, گنتی, goal);
			اگر (err)
				جاؤ errout;
		}

		اگر (range_انتہا == انتہا)
			توڑو;

		err = ext2fs_extent_goto(handle, دایاں_extent.e_lblk);
		اگر (err)
			جاؤ errout;
		اگلا = دایاں_extent.e_lblk + دایاں_extent.e_len;
		بایاں_extent = دایاں_extent;
		بایاں_adjacent = &بایاں_extent;
		range_آغاز = اگلا;
		goal = بایاں_extent.e_pblk + (range_آغاز - بایاں_extent.e_lblk);
	} جب (range_انتہا < انتہا);

errout:
	ext2fs_extent_free(handle);
	واپس err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, صحیح flags, ext2_ino_t ino,
			   ساخت ext2_inode *inode, blk64_t goal,
			   blk64_t آغاز, blk64_t len)
{
	ساخت ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, آخری = 0;
	صحیح			zero_len = 0;
	errcode_t		err;

	اگر (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		واپس EXT2_ET_INVALID_دلیل;

	اگر (len > ext2fs_بلاکیں_گنتی(fs->super))
		واپس EXT2_ET_بلاک_ALLOC_FAIL;
	ورنہ اگر (len == 0)
		واپس 0;

	/* Read inode structure if necessary */
	اگر (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		اگر (err)
			واپس err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   آغاز, len, goal);

	اگر (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, آغاز, len);
		جاؤ برآمد;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	برائے (blk = آغاز; blk < آغاز + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, تہی, 0, blk, 0, &x);
		اگر (err)
			واپس err;
		اگر (x)
			جاری;

		err = ext2fs_bmap2(fs, ino, inode, تہی, BMAP_ALLOC,
				   blk, 0, &x);
		اگر (err)
			جاؤ errout;
		اگر ((zero_len && (x != آخری+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  تہی, تہی);
			zero_len = 0;
			اگر (err)
				جاؤ errout;
		}
		اگر (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} ورنہ {
			zero_len++;
		}
		آخری = x;
	}

برآمد:
	اگر (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	اگر (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, تہی, تہی);
	واپس err;
}
