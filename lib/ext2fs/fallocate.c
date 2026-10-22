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

#ضمن "config.h"

#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#وصف_ڪريو min(a, b) ((a) < (b) ? (a) : (b))

#وصف_رد DEBUG

#جيڪڏهن_وصف_ٿيل DEBUG
# define dbg_printf(f, a...)  افعل {لکو(f, ## a); fflush(stdout); } طالما (0)
#وإلا
# define dbg_printf(f, a...)
#جيڪڏهن_پورو

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

ساكن فراغ dbg_print_extent(ثابت حرف *desc EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
		ثابت بنية ext2fs_extent *extent EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
#جيڪڏهن_وصف_ٿيل DEBUG
	إذا (desc)
		لکو("%s: ", desc);
	لکو("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_پن)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_ٻيو_گهمو)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#جيڪڏهن_پورو
}

ساكن errcode_t claim_range(ext2_filsys fs, بنية ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2_FS_CLUSTER_تناسب(fs) - 1) /
		   EXT2_FS_CLUSTER_تناسب(fs);
	ext2fs_بلاڪ_alloc_stats_range(fs, blk,
			clusters * EXT2_FS_CLUSTER_تناسب(fs), +1);
	أرجع ext2fs_iblk_جوڙيو_بلاڪون(fs, inode, clusters);
}

ساكن errcode_t ext_falloc_helper(ext2_filsys fs,
				   صحيح flags,
				   ext2_ino_t ino,
				   بنية ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   بنية ext2fs_extent *کاٻو_ext,
				   بنية ext2fs_extent *ساڄو_ext,
				   blk64_t range_شروعات, blk64_t range_len,
				   blk64_t alloc_goal)
{
	بنية ext2fs_extent	newex, ex;
	صحيح			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#جيڪڏهن_وصف_ٿيل DEBUG
	لکو("%s: ", __func__);
	إذا (کاٻو_ext)
		لکو("left_ext=%llu--%llu, ", کاٻو_ext->e_lblk,
		       کاٻو_ext->e_lblk + کاٻو_ext->e_len - 1);
	إذا (ساڄو_ext)
		لکو("right_ext=%llu--%llu, ", ساڄو_ext->e_lblk,
		       ساڄو_ext->e_lblk + ساڄو_ext->e_len - 1);
	لکو("start=%llu len=%llu, goal=%llu\n", range_شروعات, range_len,
	       alloc_goal);
	fflush(stdout);
#جيڪڏهن_پورو
	/* Can't create initialized extents past EOF? */
	إذا (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_سائيز(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2_FS_CLUSTER_نقاب(fs)) |
		     (range_شروعات & EXT2_FS_CLUSTER_نقاب(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2_FS_CLUSTER_نقاب(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2_FS_CLUSTER_نقاب(fs);

	/* We must lengthen the left extent to the end of the cluster */
	إذا (کاٻو_ext && EXT2_FS_CLUSTER_تناسب(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		إذا (کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - کاٻو_ext->e_len;
		وإلا
			fillable = max_init_len - کاٻو_ext->e_len;

		إذا (fillable > range_len)
			fillable = range_len;
		إذا (fillable == 0)
			اذهب expand_ساڄو;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2_FS_CLUSTER_تناسب(fs) -
			       (range_شروعات & EXT2_FS_CLUSTER_نقاب(fs));
		cluster_fill &= EXT2_FS_CLUSTER_نقاب(fs);
		إذا (cluster_fill == 0)
			اذهب expand_ساڄو;

		إذا (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		إذا (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = کاٻو_ext->e_lblk + کاٻو_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			إذا (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			إذا (cluster_fill == 0)
				اذهب expand_ساڄو;
		}

		err = ext2fs_extent_goto(handle, کاٻو_ext->e_lblk);
		إذا (err)
			اذهب expand_ساڄو;
		کاٻو_ext->e_len += cluster_fill;
		range_شروعات += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", کاٻو_ext);
		err = ext2fs_extent_replace(handle, 0, کاٻو_ext);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب ٻاهريون;

		/* Zero blocks */
		إذا (!(کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, کاٻو_ext->e_pblk +
						  کاٻو_ext->e_len -
						  cluster_fill, cluster_fill,
						  عدم, عدم);
			إذا (err)
				اذهب ٻاهريون;
		}
	}

expand_ساڄو:
	/* We must lengthen the right extent to the beginning of the cluster */
	إذا (ساڄو_ext && EXT2_FS_CLUSTER_تناسب(fs) > 1) {
		/* How much can we attach to right_ext? */
		إذا (ساڄو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ساڄو_ext->e_len;
		وإلا
			fillable = max_init_len - ساڄو_ext->e_len;

		إذا (fillable > range_len)
			fillable = range_len;
		إذا (fillable == 0)
			اذهب try_ملايو;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = ساڄو_ext->e_lblk & EXT2_FS_CLUSTER_نقاب(fs);
		إذا (cluster_fill == 0)
			اذهب try_ملايو;

		err = ext2fs_extent_goto(handle, ساڄو_ext->e_lblk);
		إذا (err)
			اذهب ٻاهريون;

		إذا (cluster_fill > fillable)
			cluster_fill = fillable;
		ساڄو_ext->e_lblk -= cluster_fill;
		ساڄو_ext->e_pblk -= cluster_fill;
		ساڄو_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", ساڄو_ext);
		err = ext2fs_extent_replace(handle, 0, ساڄو_ext);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب ٻاهريون;

		/* Zero blocks if necessary */
		إذا (!(ساڄو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ساڄو_ext->e_pblk,
						  cluster_fill, عدم, عدم);
			إذا (err)
				اذهب ٻاهريون;
		}
	}

try_ملايو:
	/* Merge both extents together, perhaps? */
	إذا (کاٻو_ext && ساڄو_ext) {
		/* Are the two extents mergeable? */
		إذا ((کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (ساڄو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			اذهب try_کاٻو;

		/* User requires init/uninit but extent is uninit/init. */
		إذا (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			اذهب try_کاٻو;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		إذا (!(کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_بلاڪون) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			اذهب try_کاٻو;

		/* Will it even fit? */
		x = کاٻو_ext->e_len + range_len + ساڄو_ext->e_len;
		إذا (x > (کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			اذهب try_کاٻو;

		/* Would they even be physically contiguous if merged? */
		إذا (کاٻو_ext->e_pblk + کاٻو_ext->e_len + range_len !=
				ساڄو_ext->e_pblk)
			اذهب try_کاٻو;

		err = ext2fs_extent_goto(handle, کاٻو_ext->e_lblk);
		إذا (err)
			اذهب try_کاٻو;

		/* Allocate blocks */
		y = کاٻو_ext->e_pblk + کاٻو_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_ڊيگهائي, y,
				       ساڄو_ext->e_pblk - y + 1, عدم,
				       &pblk, &plen);
		إذا (err)
			اذهب try_کاٻو;
		إذا (pblk + plen != ساڄو_ext->e_pblk)
			اذهب try_کاٻو;
		err = claim_range(fs, inode, pblk, plen);
		إذا (err)
			اذهب ٻاهريون;

		/* Modify extents */
		کاٻو_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", کاٻو_ext);
		err = ext2fs_extent_replace(handle, 0, کاٻو_ext);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_ايندڙ_پن, &newex);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_delete(handle, 0);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب ٻاهريون;
		*ساڄو_ext = *کاٻو_ext;

		/* Zero blocks */
		إذا (!(کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاڪون)) {
			err = ext2fs_zero_blocks2(fs, range_شروعات, range_len,
						  عدم, عدم);
			إذا (err)
				اذهب ٻاهريون;
		}

		أرجع 0;
	}

try_کاٻو:
	/* Extend the left extent */
	إذا (کاٻو_ext) {
		/* How many more blocks can be attached to left_ext? */
		إذا (کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - کاٻو_ext->e_len;
		وإلا إذا (flags & EXT2_FALLOCATE_ZERO_بلاڪون)
			fillable = max_init_len - کاٻو_ext->e_len;
		وإلا
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		إذا (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			اذهب try_ساڄو;

		إذا (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = کاٻو_ext->e_lblk + کاٻو_ext->e_len - 1;
		إذا (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			إذا (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		إذا (fillable == 0)
			اذهب try_ساڄو;

		/* Test if the right edge of the range is already mapped? */
		إذا (EXT2_FS_CLUSTER_تناسب(fs) > 1) {
			err = ext2fs_map_cluster_بلاڪ(fs, ino, inode,
					x + fillable, &pblk);
			إذا (err)
				اذهب ٻاهريون;
			إذا (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2_FS_CLUSTER_نقاب(fs));
			إذا (fillable == 0)
				اذهب try_ساڄو;
		}

		/* Allocate range of blocks */
		x = کاٻو_ext->e_pblk + کاٻو_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_ڊيگهائي,
				x, fillable, عدم, &pblk, &plen);
		إذا (err)
			اذهب try_ساڄو;
		err = claim_range(fs, inode, pblk, plen);
		إذا (err)
			اذهب ٻاهريون;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, کاٻو_ext->e_lblk);
		إذا (err)
			اذهب ٻاهريون;
		range_شروعات += plen;
		range_len -= plen;
		کاٻو_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", کاٻو_ext);
		err = ext2fs_extent_replace(handle, 0, کاٻو_ext);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب ٻاهريون;

		/* Zero blocks if necessary */
		إذا (!(کاٻو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاڪون)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, عدم, عدم);
			إذا (err)
				اذهب ٻاهريون;
		}
	}

try_ساڄو:
	/* Extend the right extent */
	إذا (ساڄو_ext) {
		/* How much can we attach to right_ext? */
		إذا (ساڄو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ساڄو_ext->e_len;
		وإلا إذا (flags & EXT2_FALLOCATE_ZERO_بلاڪون)
			fillable = max_init_len - ساڄو_ext->e_len;
		وإلا
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		إذا (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ساڄو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ساڄو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			اذهب try_anywhere;

		إذا (fillable > range_len)
			fillable = range_len;
		إذا (fillable == 0)
			اذهب try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		إذا (EXT2_FS_CLUSTER_تناسب(fs) > 1) {
			err = ext2fs_map_cluster_بلاڪ(fs, ino, inode,
					ساڄو_ext->e_lblk - fillable, &pblk);
			إذا (err)
				اذهب ٻاهريون;
			إذا (pblk)
				fillable -= EXT2_FS_CLUSTER_تناسب(fs) -
						((ساڄو_ext->e_lblk - fillable)
						 & EXT2_FS_CLUSTER_نقاب(fs));
			إذا (fillable == 0)
				اذهب try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_ڊيگهائي,
				ساڄو_ext->e_pblk - fillable,
				fillable, عدم, &pblk, &plen);
		إذا (err)
			اذهب try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2_FS_CLUSTER_نقاب(fs),
			      plen + (pblk & EXT2_FS_CLUSTER_نقاب(fs)));
		إذا (err)
			اذهب ٻاهريون;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, ساڄو_ext->e_lblk);
		إذا (err)
			اذهب ٻاهريون;
		range_len -= plen;
		ساڄو_ext->e_lblk -= plen;
		ساڄو_ext->e_pblk -= plen;
		ساڄو_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", ساڄو_ext);
		err = ext2fs_extent_replace(handle, 0, ساڄو_ext);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب ٻاهريون;

		/* Zero blocks if necessary */
		إذا (!(ساڄو_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاڪون)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, عدم, عدم);
			إذا (err)
				اذهب ٻاهريون;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	إذا (range_len > 0 && (range_شروعات & EXT2_FS_CLUSTER_نقاب(fs))) {
		cluster_fill = EXT2_FS_CLUSTER_تناسب(fs) -
			       (range_شروعات & EXT2_FS_CLUSTER_نقاب(fs));
		cluster_fill &= EXT2_FS_CLUSTER_نقاب(fs);
		إذا (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_شروعات;
		err = ext2fs_map_cluster_بلاڪ(fs, ino, inode, newex.e_lblk,
					       &pblk);
		إذا (err)
			اذهب ٻاهريون;
		إذا (pblk == 0)
			اذهب try_ساڄو_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		إذا (err == EXT2_ET_NO_CURRENT_ڳنڍ)
			ex.e_lblk = 0;
		وإلا إذا (err)
			اذهب ٻاهريون;

		إذا (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		وإلا
			op = EXT2_EXTENT_وجهو_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_وجهو(handle, op, &newex);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب ٻاهريون;

		إذا (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاڪون)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, عدم, عدم);
			إذا (err)
				اذهب ٻاهريون;
		}

		range_شروعات += cluster_fill;
		range_len -= cluster_fill;
	}

try_ساڄو_implied:
	y = range_شروعات + range_len;
	إذا (range_len > 0 && (y & EXT2_FS_CLUSTER_نقاب(fs))) {
		cluster_fill = y & EXT2_FS_CLUSTER_نقاب(fs);
		إذا (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2_FS_CLUSTER_نقاب(fs);
		err = ext2fs_map_cluster_بلاڪ(fs, ino, inode, newex.e_lblk,
					       &pblk);
		إذا (err)
			اذهب ٻاهريون;
		إذا (pblk == 0)
			اذهب no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		إذا (err == EXT2_ET_NO_CURRENT_ڳنڍ)
			ex.e_lblk = 0;
		وإلا إذا (err)
			اذهب ٻاهريون;

		إذا (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		وإلا
			op = EXT2_EXTENT_وجهو_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_وجهو(handle, op, &newex);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب ٻاهريون;

		إذا (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاڪون)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, عدم, عدم);
			إذا (err)
				اذهب ٻاهريون;
		}

		range_len -= cluster_fill;
	}

no_implied:
	إذا (range_len == 0)
		أرجع 0;

	newex.e_lblk = range_شروعات;
	إذا (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} وإلا {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	لكل (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2_FS_CLUSTER_نقاب(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2_FS_CLUSTER_نقاب(fs),
				       fillable,
				       عدم, &pblk, &plen);
		إذا (err)
			اذهب ٻاهريون;
		err = claim_range(fs, inode, pblk, plen);
		إذا (err)
			اذهب ٻاهريون;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		إذا (err == EXT2_ET_NO_CURRENT_ڳنڍ)
			ex.e_lblk = 0;
		وإلا إذا (err)
			اذهب ٻاهريون;

		إذا (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		وإلا
			op = EXT2_EXTENT_وجهو_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_وجهو(handle, op, &newex);
		إذا (err)
			اذهب ٻاهريون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب ٻاهريون;

		إذا (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلاڪون)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, عدم, عدم);
			إذا (err)
				اذهب ٻاهريون;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		إذا (pblk >= ext2fs_بلاڪون_ڳڻپ(fs->super))
			pblk = fs->super->s_پهريون_معلومات_بلاڪ;
	}

ٻاهريون:
	أرجع err;
}

ساكن errcode_t extent_fallocate(ext2_filsys fs, صحيح flags, ext2_ino_t ino,
				      بنية ext2_inode *inode, blk64_t goal,
				      blk64_t شروعات, blk64_t len)
{
	ext2_extent_handle_t	handle;
	بنية ext2fs_extent	کاٻو_extent, ساڄو_extent;
	بنية ext2fs_extent	*کاٻو_adjacent, *ساڄو_adjacent;
	errcode_t		err;
	blk64_t			range_شروعات, range_پڇاڙي = 0, پڇاڙي, ايندڙ;
	blk64_t			ڳڻپ, goal_مفاصلو;

	پڇاڙي = شروعات + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	إذا (err)
		أرجع err;

	/*
	 * Find the extent closest to the start of the alloc range.  We don't
	 * check the return value because _goto() sets the current node to the
	 * next-lowest extent if 'start' is in a hole; or the next-highest
	 * extent if there aren't any lower ones; or doesn't set a current node
	 * if there was a real error reading the extent tree.  In that case,
	 * _get() will error out.
	 */
شروعات_again:
	ext2fs_extent_goto(handle, شروعات);
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &کاٻو_extent);
	إذا (err == EXT2_ET_NO_CURRENT_ڳنڍ) {
		blk64_t max_بلاڪون = ext2fs_بلاڪون_ڳڻپ(fs->super);

		إذا (goal == ~0ULL)
			goal = ext2fs_ڳوليو_inode_goal(fs, ino, inode, شروعات);
		err = ext2fs_ڳوليو_پهريون_zero_بلاڪ_bitmap2(fs->بلاڪ_map,
						goal, max_بلاڪون - 1, &goal);
		goal += شروعات;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, عدم,
					عدم, شروعات, len, goal);
		اذهب errout;
	} وإلا إذا (err)
		اذهب errout;

	dbg_print_extent("ext_falloc initial", &کاٻو_extent);
	ايندڙ = کاٻو_extent.e_lblk + کاٻو_extent.e_len;
	إذا (کاٻو_extent.e_lblk > شروعات) {
		/* The nearest extent we found was beyond start??? */
		goal = کاٻو_extent.e_pblk - (کاٻو_extent.e_lblk - شروعات);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, عدم,
					&کاٻو_extent, شروعات,
					min(len, کاٻو_extent.e_lblk - شروعات),
					goal);
		إذا (err)
			اذهب errout;

		اذهب شروعات_again;
	} وإلا إذا (ايندڙ >= شروعات) {
		range_شروعات = ايندڙ;
		کاٻو_adjacent = &کاٻو_extent;
	} وإلا {
		range_شروعات = شروعات;
		کاٻو_adjacent = عدم;
	}
	goal = کاٻو_extent.e_pblk + (range_شروعات - کاٻو_extent.e_lblk);

	افعل {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_ايندڙ_پن,
					   &ساڄو_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (صحيح)err);
		dbg_print_extent("ext_falloc next", &ساڄو_extent);
		/* Stop if we've seen this extent before */
		إذا (!err && ساڄو_extent.e_lblk <= کاٻو_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_ايندڙ;

		إذا (err && err != EXT2_ET_EXTENT_NO_ايندڙ)
			اذهب errout;
		إذا (err == EXT2_ET_EXTENT_NO_ايندڙ ||
		    ساڄو_extent.e_lblk > پڇاڙي + 1) {
			range_پڇاڙي = پڇاڙي;
			ساڄو_adjacent = عدم;
		} وإلا {
			/* Handle right_extent.e_lblk <= end */
			range_پڇاڙي = ساڄو_extent.e_lblk - 1;
			ساڄو_adjacent = &ساڄو_extent;
		}
		goal_مفاصلو = range_شروعات - ايندڙ;
		إذا (err != EXT2_ET_EXTENT_NO_ايندڙ &&
		    goal_مفاصلو > (range_پڇاڙي - ساڄو_extent.e_lblk))
			goal = ساڄو_extent.e_pblk -
					(ساڄو_extent.e_lblk - range_شروعات);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_شروعات, range_پڇاڙي);
		err = 0;
		إذا (range_شروعات <= range_پڇاڙي) {
			ڳڻپ = range_پڇاڙي - range_شروعات + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						کاٻو_adjacent, ساڄو_adjacent,
						range_شروعات, ڳڻپ, goal);
			إذا (err)
				اذهب errout;
		}

		إذا (range_پڇاڙي == پڇاڙي)
			اكسر;

		err = ext2fs_extent_goto(handle, ساڄو_extent.e_lblk);
		إذا (err)
			اذهب errout;
		ايندڙ = ساڄو_extent.e_lblk + ساڄو_extent.e_len;
		کاٻو_extent = ساڄو_extent;
		کاٻو_adjacent = &کاٻو_extent;
		range_شروعات = ايندڙ;
		goal = کاٻو_extent.e_pblk + (range_شروعات - کاٻو_extent.e_lblk);
	} طالما (range_پڇاڙي < پڇاڙي);

errout:
	ext2fs_extent_free(handle);
	أرجع err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, صحيح flags, ext2_ino_t ino,
			   بنية ext2_inode *inode, blk64_t goal,
			   blk64_t شروعات, blk64_t len)
{
	بنية ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, آخري = 0;
	صحيح			zero_len = 0;
	errcode_t		err;

	إذا (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		أرجع EXT2_ET_INVALID_دليل;

	إذا (len > ext2fs_بلاڪون_ڳڻپ(fs->super))
		أرجع EXT2_ET_بلاڪ_ALLOC_FAIL;
	وإلا إذا (len == 0)
		أرجع 0;

	/* Read inode structure if necessary */
	إذا (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (err)
			أرجع err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   شروعات, len, goal);

	إذا (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, شروعات, len);
		اذهب ٻاهريون;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	لكل (blk = شروعات; blk < شروعات + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, عدم, 0, blk, 0, &x);
		إذا (err)
			أرجع err;
		إذا (x)
			استمر;

		err = ext2fs_bmap2(fs, ino, inode, عدم, BMAP_ALLOC,
				   blk, 0, &x);
		إذا (err)
			اذهب errout;
		إذا ((zero_len && (x != آخري+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  عدم, عدم);
			zero_len = 0;
			إذا (err)
				اذهب errout;
		}
		إذا (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} وإلا {
			zero_len++;
		}
		آخري = x;
	}

ٻاهريون:
	إذا (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	إذا (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, عدم, عدم);
	أرجع err;
}
