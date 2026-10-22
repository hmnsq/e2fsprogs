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
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#تعریف_کن min(a, b) ((a) < (b) ? (a) : (b))

#لغو_تعریف DEBUG

#اگر_تعریف_شده DEBUG
# define dbg_printf(f, a...)  افعل {چاپ_کن(f, ## a); fflush(stdout); } طالما (0)
#وإلا
# define dbg_printf(f, a...)
#پایان_اگر

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

ساكن فراغ dbg_print_extent(ثابت حرف *desc EXT2FS_ATTR((استفادهنشده)),
		ثابت بنية ext2fs_extent *extent EXT2FS_ATTR((استفادهنشده)))
{
#اگر_تعریف_شده DEBUG
	إذا (desc)
		چاپ_کن("%s: ", desc);
	چاپ_کن("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_برگ)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_دومی_بازدید)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#پایان_اگر
}

ساكن errcode_t claim_range(ext2_filsys fs, بنية ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2_FS_CLUSTER_نسبت(fs) - 1) /
		   EXT2_FS_CLUSTER_نسبت(fs);
	ext2fs_بلوک_alloc_stats_range(fs, blk,
			clusters * EXT2_FS_CLUSTER_نسبت(fs), +1);
	أرجع ext2fs_iblk_جمع_کن_بلوکها(fs, inode, clusters);
}

ساكن errcode_t ext_falloc_helper(ext2_filsys fs,
				   صحيح flags,
				   ext2_ino_t ino,
				   بنية ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   بنية ext2fs_extent *چپ_ext,
				   بنية ext2fs_extent *راست_ext,
				   blk64_t range_شروع, blk64_t range_len,
				   blk64_t alloc_goal)
{
	بنية ext2fs_extent	newex, ex;
	صحيح			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#اگر_تعریف_شده DEBUG
	چاپ_کن("%s: ", __func__);
	إذا (چپ_ext)
		چاپ_کن("left_ext=%llu--%llu, ", چپ_ext->e_lblk,
		       چپ_ext->e_lblk + چپ_ext->e_len - 1);
	إذا (راست_ext)
		چاپ_کن("right_ext=%llu--%llu, ", راست_ext->e_lblk,
		       راست_ext->e_lblk + راست_ext->e_len - 1);
	چاپ_کن("start=%llu len=%llu, goal=%llu\n", range_شروع, range_len,
	       alloc_goal);
	fflush(stdout);
#پایان_اگر
	/* Can't create initialized extents past EOF? */
	إذا (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_اندازه(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2_FS_CLUSTER_ماسک(fs)) |
		     (range_شروع & EXT2_FS_CLUSTER_ماسک(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2_FS_CLUSTER_ماسک(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2_FS_CLUSTER_ماسک(fs);

	/* We must lengthen the left extent to the end of the cluster */
	إذا (چپ_ext && EXT2_FS_CLUSTER_نسبت(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		إذا (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - چپ_ext->e_len;
		وإلا
			fillable = max_init_len - چپ_ext->e_len;

		إذا (fillable > range_len)
			fillable = range_len;
		إذا (fillable == 0)
			اذهب expand_راست;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2_FS_CLUSTER_نسبت(fs) -
			       (range_شروع & EXT2_FS_CLUSTER_ماسک(fs));
		cluster_fill &= EXT2_FS_CLUSTER_ماسک(fs);
		إذا (cluster_fill == 0)
			اذهب expand_راست;

		إذا (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		إذا (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = چپ_ext->e_lblk + چپ_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			إذا (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			إذا (cluster_fill == 0)
				اذهب expand_راست;
		}

		err = ext2fs_extent_goto(handle, چپ_ext->e_lblk);
		إذا (err)
			اذهب expand_راست;
		چپ_ext->e_len += cluster_fill;
		range_شروع += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", چپ_ext);
		err = ext2fs_extent_replace(handle, 0, چپ_ext);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب بیرون;

		/* Zero blocks */
		إذا (!(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, چپ_ext->e_pblk +
						  چپ_ext->e_len -
						  cluster_fill, cluster_fill,
						  عدم, عدم);
			إذا (err)
				اذهب بیرون;
		}
	}

expand_راست:
	/* We must lengthen the right extent to the beginning of the cluster */
	إذا (راست_ext && EXT2_FS_CLUSTER_نسبت(fs) > 1) {
		/* How much can we attach to right_ext? */
		إذا (راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - راست_ext->e_len;
		وإلا
			fillable = max_init_len - راست_ext->e_len;

		إذا (fillable > range_len)
			fillable = range_len;
		إذا (fillable == 0)
			اذهب try_ادغام;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = راست_ext->e_lblk & EXT2_FS_CLUSTER_ماسک(fs);
		إذا (cluster_fill == 0)
			اذهب try_ادغام;

		err = ext2fs_extent_goto(handle, راست_ext->e_lblk);
		إذا (err)
			اذهب بیرون;

		إذا (cluster_fill > fillable)
			cluster_fill = fillable;
		راست_ext->e_lblk -= cluster_fill;
		راست_ext->e_pblk -= cluster_fill;
		راست_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", راست_ext);
		err = ext2fs_extent_replace(handle, 0, راست_ext);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب بیرون;

		/* Zero blocks if necessary */
		إذا (!(راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, راست_ext->e_pblk,
						  cluster_fill, عدم, عدم);
			إذا (err)
				اذهب بیرون;
		}
	}

try_ادغام:
	/* Merge both extents together, perhaps? */
	إذا (چپ_ext && راست_ext) {
		/* Are the two extents mergeable? */
		إذا ((چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			اذهب try_چپ;

		/* User requires init/uninit but extent is uninit/init. */
		إذا (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			اذهب try_چپ;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		إذا (!(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_بلوکها) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			اذهب try_چپ;

		/* Will it even fit? */
		x = چپ_ext->e_len + range_len + راست_ext->e_len;
		إذا (x > (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			اذهب try_چپ;

		/* Would they even be physically contiguous if merged? */
		إذا (چپ_ext->e_pblk + چپ_ext->e_len + range_len !=
				راست_ext->e_pblk)
			اذهب try_چپ;

		err = ext2fs_extent_goto(handle, چپ_ext->e_lblk);
		إذا (err)
			اذهب try_چپ;

		/* Allocate blocks */
		y = چپ_ext->e_pblk + چپ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_درازی, y,
				       راست_ext->e_pblk - y + 1, عدم,
				       &pblk, &plen);
		إذا (err)
			اذهب try_چپ;
		إذا (pblk + plen != راست_ext->e_pblk)
			اذهب try_چپ;
		err = claim_range(fs, inode, pblk, plen);
		إذا (err)
			اذهب بیرون;

		/* Modify extents */
		چپ_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", چپ_ext);
		err = ext2fs_extent_replace(handle, 0, چپ_ext);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_بعدی_برگ, &newex);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_delete(handle, 0);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب بیرون;
		*راست_ext = *چپ_ext;

		/* Zero blocks */
		إذا (!(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلوکها)) {
			err = ext2fs_zero_blocks2(fs, range_شروع, range_len,
						  عدم, عدم);
			إذا (err)
				اذهب بیرون;
		}

		أرجع 0;
	}

try_چپ:
	/* Extend the left extent */
	إذا (چپ_ext) {
		/* How many more blocks can be attached to left_ext? */
		إذا (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - چپ_ext->e_len;
		وإلا إذا (flags & EXT2_FALLOCATE_ZERO_بلوکها)
			fillable = max_init_len - چپ_ext->e_len;
		وإلا
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		إذا (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			اذهب try_راست;

		إذا (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = چپ_ext->e_lblk + چپ_ext->e_len - 1;
		إذا (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			إذا (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		إذا (fillable == 0)
			اذهب try_راست;

		/* Test if the right edge of the range is already mapped? */
		إذا (EXT2_FS_CLUSTER_نسبت(fs) > 1) {
			err = ext2fs_map_cluster_بلوک(fs, ino, inode,
					x + fillable, &pblk);
			إذا (err)
				اذهب بیرون;
			إذا (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2_FS_CLUSTER_ماسک(fs));
			إذا (fillable == 0)
				اذهب try_راست;
		}

		/* Allocate range of blocks */
		x = چپ_ext->e_pblk + چپ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_درازی,
				x, fillable, عدم, &pblk, &plen);
		إذا (err)
			اذهب try_راست;
		err = claim_range(fs, inode, pblk, plen);
		إذا (err)
			اذهب بیرون;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, چپ_ext->e_lblk);
		إذا (err)
			اذهب بیرون;
		range_شروع += plen;
		range_len -= plen;
		چپ_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", چپ_ext);
		err = ext2fs_extent_replace(handle, 0, چپ_ext);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب بیرون;

		/* Zero blocks if necessary */
		إذا (!(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلوکها)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, عدم, عدم);
			إذا (err)
				اذهب بیرون;
		}
	}

try_راست:
	/* Extend the right extent */
	إذا (راست_ext) {
		/* How much can we attach to right_ext? */
		إذا (راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - راست_ext->e_len;
		وإلا إذا (flags & EXT2_FALLOCATE_ZERO_بلوکها)
			fillable = max_init_len - راست_ext->e_len;
		وإلا
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		إذا (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			اذهب try_anywhere;

		إذا (fillable > range_len)
			fillable = range_len;
		إذا (fillable == 0)
			اذهب try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		إذا (EXT2_FS_CLUSTER_نسبت(fs) > 1) {
			err = ext2fs_map_cluster_بلوک(fs, ino, inode,
					راست_ext->e_lblk - fillable, &pblk);
			إذا (err)
				اذهب بیرون;
			إذا (pblk)
				fillable -= EXT2_FS_CLUSTER_نسبت(fs) -
						((راست_ext->e_lblk - fillable)
						 & EXT2_FS_CLUSTER_ماسک(fs));
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
				EXT2_NEWRANGE_MIN_درازی,
				راست_ext->e_pblk - fillable,
				fillable, عدم, &pblk, &plen);
		إذا (err)
			اذهب try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2_FS_CLUSTER_ماسک(fs),
			      plen + (pblk & EXT2_FS_CLUSTER_ماسک(fs)));
		إذا (err)
			اذهب بیرون;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, راست_ext->e_lblk);
		إذا (err)
			اذهب بیرون;
		range_len -= plen;
		راست_ext->e_lblk -= plen;
		راست_ext->e_pblk -= plen;
		راست_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", راست_ext);
		err = ext2fs_extent_replace(handle, 0, راست_ext);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب بیرون;

		/* Zero blocks if necessary */
		إذا (!(راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلوکها)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, عدم, عدم);
			إذا (err)
				اذهب بیرون;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	إذا (range_len > 0 && (range_شروع & EXT2_FS_CLUSTER_ماسک(fs))) {
		cluster_fill = EXT2_FS_CLUSTER_نسبت(fs) -
			       (range_شروع & EXT2_FS_CLUSTER_ماسک(fs));
		cluster_fill &= EXT2_FS_CLUSTER_ماسک(fs);
		إذا (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_شروع;
		err = ext2fs_map_cluster_بلوک(fs, ino, inode, newex.e_lblk,
					       &pblk);
		إذا (err)
			اذهب بیرون;
		إذا (pblk == 0)
			اذهب try_راست_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		إذا (err == EXT2_ET_NO_CURRENT_گره)
			ex.e_lblk = 0;
		وإلا إذا (err)
			اذهب بیرون;

		إذا (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		وإلا
			op = EXT2_EXTENT_درج_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_درج(handle, op, &newex);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب بیرون;

		إذا (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلوکها)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, عدم, عدم);
			إذا (err)
				اذهب بیرون;
		}

		range_شروع += cluster_fill;
		range_len -= cluster_fill;
	}

try_راست_implied:
	y = range_شروع + range_len;
	إذا (range_len > 0 && (y & EXT2_FS_CLUSTER_ماسک(fs))) {
		cluster_fill = y & EXT2_FS_CLUSTER_ماسک(fs);
		إذا (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2_FS_CLUSTER_ماسک(fs);
		err = ext2fs_map_cluster_بلوک(fs, ino, inode, newex.e_lblk,
					       &pblk);
		إذا (err)
			اذهب بیرون;
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
		إذا (err == EXT2_ET_NO_CURRENT_گره)
			ex.e_lblk = 0;
		وإلا إذا (err)
			اذهب بیرون;

		إذا (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		وإلا
			op = EXT2_EXTENT_درج_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_درج(handle, op, &newex);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب بیرون;

		إذا (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلوکها)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, عدم, عدم);
			إذا (err)
				اذهب بیرون;
		}

		range_len -= cluster_fill;
	}

no_implied:
	إذا (range_len == 0)
		أرجع 0;

	newex.e_lblk = range_شروع;
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
		cluster_fill = newex.e_lblk & EXT2_FS_CLUSTER_ماسک(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2_FS_CLUSTER_ماسک(fs),
				       fillable,
				       عدم, &pblk, &plen);
		إذا (err)
			اذهب بیرون;
		err = claim_range(fs, inode, pblk, plen);
		إذا (err)
			اذهب بیرون;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		إذا (err == EXT2_ET_NO_CURRENT_گره)
			ex.e_lblk = 0;
		وإلا إذا (err)
			اذهب بیرون;

		إذا (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		وإلا
			op = EXT2_EXTENT_درج_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_درج(handle, op, &newex);
		إذا (err)
			اذهب بیرون;
		err = ext2fs_extent_fix_parents(handle);
		إذا (err)
			اذهب بیرون;

		إذا (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_بلوکها)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, عدم, عدم);
			إذا (err)
				اذهب بیرون;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		إذا (pblk >= ext2fs_بلوکها_شمار(fs->super))
			pblk = fs->super->s_اولی_داده_بلوک;
	}

بیرون:
	أرجع err;
}

ساكن errcode_t extent_fallocate(ext2_filsys fs, صحيح flags, ext2_ino_t ino,
				      بنية ext2_inode *inode, blk64_t goal,
				      blk64_t شروع, blk64_t len)
{
	ext2_extent_handle_t	handle;
	بنية ext2fs_extent	چپ_extent, راست_extent;
	بنية ext2fs_extent	*چپ_adjacent, *راست_adjacent;
	errcode_t		err;
	blk64_t			range_شروع, range_انتها = 0, انتها, بعدی;
	blk64_t			شمار, goal_فاصله;

	انتها = شروع + len - 1;
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
شروع_again:
	ext2fs_extent_goto(handle, شروع);
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &چپ_extent);
	إذا (err == EXT2_ET_NO_CURRENT_گره) {
		blk64_t max_بلوکها = ext2fs_بلوکها_شمار(fs->super);

		إذا (goal == ~0ULL)
			goal = ext2fs_پیدا_کن_inode_goal(fs, ino, inode, شروع);
		err = ext2fs_پیدا_کن_اولی_zero_بلوک_bitmap2(fs->بلوک_map,
						goal, max_بلوکها - 1, &goal);
		goal += شروع;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, عدم,
					عدم, شروع, len, goal);
		اذهب errout;
	} وإلا إذا (err)
		اذهب errout;

	dbg_print_extent("ext_falloc initial", &چپ_extent);
	بعدی = چپ_extent.e_lblk + چپ_extent.e_len;
	إذا (چپ_extent.e_lblk > شروع) {
		/* The nearest extent we found was beyond start??? */
		goal = چپ_extent.e_pblk - (چپ_extent.e_lblk - شروع);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, عدم,
					&چپ_extent, شروع,
					min(len, چپ_extent.e_lblk - شروع),
					goal);
		إذا (err)
			اذهب errout;

		اذهب شروع_again;
	} وإلا إذا (بعدی >= شروع) {
		range_شروع = بعدی;
		چپ_adjacent = &چپ_extent;
	} وإلا {
		range_شروع = شروع;
		چپ_adjacent = عدم;
	}
	goal = چپ_extent.e_pblk + (range_شروع - چپ_extent.e_lblk);

	افعل {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_بعدی_برگ,
					   &راست_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (صحيح)err);
		dbg_print_extent("ext_falloc next", &راست_extent);
		/* Stop if we've seen this extent before */
		إذا (!err && راست_extent.e_lblk <= چپ_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_بعدی;

		إذا (err && err != EXT2_ET_EXTENT_NO_بعدی)
			اذهب errout;
		إذا (err == EXT2_ET_EXTENT_NO_بعدی ||
		    راست_extent.e_lblk > انتها + 1) {
			range_انتها = انتها;
			راست_adjacent = عدم;
		} وإلا {
			/* Handle right_extent.e_lblk <= end */
			range_انتها = راست_extent.e_lblk - 1;
			راست_adjacent = &راست_extent;
		}
		goal_فاصله = range_شروع - بعدی;
		إذا (err != EXT2_ET_EXTENT_NO_بعدی &&
		    goal_فاصله > (range_انتها - راست_extent.e_lblk))
			goal = راست_extent.e_pblk -
					(راست_extent.e_lblk - range_شروع);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_شروع, range_انتها);
		err = 0;
		إذا (range_شروع <= range_انتها) {
			شمار = range_انتها - range_شروع + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						چپ_adjacent, راست_adjacent,
						range_شروع, شمار, goal);
			إذا (err)
				اذهب errout;
		}

		إذا (range_انتها == انتها)
			اكسر;

		err = ext2fs_extent_goto(handle, راست_extent.e_lblk);
		إذا (err)
			اذهب errout;
		بعدی = راست_extent.e_lblk + راست_extent.e_len;
		چپ_extent = راست_extent;
		چپ_adjacent = &چپ_extent;
		range_شروع = بعدی;
		goal = چپ_extent.e_pblk + (range_شروع - چپ_extent.e_lblk);
	} طالما (range_انتها < انتها);

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
			   blk64_t شروع, blk64_t len)
{
	بنية ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, آخرین = 0;
	صحيح			zero_len = 0;
	errcode_t		err;

	إذا (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		أرجع EXT2_ET_INVALID_آرگومان;

	إذا (len > ext2fs_بلوکها_شمار(fs->super))
		أرجع EXT2_ET_بلوک_ALLOC_FAIL;
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
		   شروع, len, goal);

	إذا (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, شروع, len);
		اذهب بیرون;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	لكل (blk = شروع; blk < شروع + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, عدم, 0, blk, 0, &x);
		إذا (err)
			أرجع err;
		إذا (x)
			استمر;

		err = ext2fs_bmap2(fs, ino, inode, عدم, BMAP_ALLOC,
				   blk, 0, &x);
		إذا (err)
			اذهب errout;
		إذا ((zero_len && (x != آخرین+1)) ||
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
		آخرین = x;
	}

بیرون:
	إذا (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	إذا (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, عدم, عدم);
	أرجع err;
}
