/*
 * alloc.c --- allocate new inodes, blocks for ext2fs
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <time.h>
#شامل <string.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
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
 * Clear the uninit block bitmap flag if necessary
 */
خالی ext2fs_clear_بلاک_uninit(ext2_filsys fs, dgrp_t گروہ)
{
	اگر (گروہ >= fs->گروہ_desc_گنتی ||
	    !ext2fs_has_گروہ_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, گروہ, EXT2_BG_بلاک_UNINIT)))
		واپس;

	/* uninit block bitmaps are now initialized in read_bitmaps() */

	ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_بلاک_UNINIT);
	ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
}

/*
 * Check for uninit inode bitmaps and deal with them appropriately
 */
ساکن خالی check_inode_uninit(ext2_filsys fs, ext2fs_inode_bitmap map,
			  dgrp_t گروہ)
{
	ext2_ino_t	i, ino;

	اگر (گروہ >= fs->گروہ_desc_گنتی ||
	    !ext2fs_has_گروہ_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, گروہ, EXT2_BG_INODE_UNINIT)))
		واپس;

	ino = (گروہ * fs->super->s_inodes_فی_گروہ) + 1;
	برائے (i=0; i < fs->super->s_inodes_فی_گروہ; i++, ino++)
		ext2fs_fast_unmark_inode_bitmap2(map, ino);

	ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_INODE_UNINIT);
	/* Mimics what the kernel does */
	ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_بلاک_UNINIT);
	ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);
	ext2fs_mark_ib_dirty(fs);
	ext2fs_mark_super_dirty(fs);
}

/*
 * Right now, just search forward from the parent directory's block
 * group to find the next free inode.
 *
 * Should have a special policy for directories.
 */
errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir,
			   صحیح منوال EXT2FS_ATTR((غیر_مستعمل)),
			   ext2fs_inode_bitmap map, ext2_ino_t *ret)
{
	ext2_ino_t	آغاز_inode = 0;
	ext2_ino_t	i, ino_اندر_گروہ, upto, پہلا_zero;
	errcode_t	retval;
	dgrp_t		گروہ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!map)
		map = fs->inode_map;
	اگر (!map)
		واپس EXT2_ET_NO_INODE_BITMAP;

	اگر (dir > 0) {
		گروہ = (dir - 1) / EXT2_INODES_فی_گروہ(fs->super);
		آغاز_inode = (گروہ * EXT2_INODES_فی_گروہ(fs->super)) + 1;
	}
	اگر (آغاز_inode < EXT2_پہلا_INODE(fs->super))
		آغاز_inode = EXT2_پہلا_INODE(fs->super);
	اگر (آغاز_inode > fs->super->s_inodes_گنتی)
		واپس EXT2_ET_INODE_ALLOC_FAIL;
	i = آغاز_inode;
	کرو {
		ino_اندر_گروہ = (i - 1) % EXT2_INODES_فی_گروہ(fs->super);
		گروہ = (i - 1) / EXT2_INODES_فی_گروہ(fs->super);

		check_inode_uninit(fs, map, گروہ);
		upto = i + (EXT2_INODES_فی_گروہ(fs->super) - ino_اندر_گروہ);
		اگر (i < آغاز_inode && upto >= آغاز_inode)
			upto = آغاز_inode - 1;
		اگر (upto > fs->super->s_inodes_گنتی)
			upto = fs->super->s_inodes_گنتی;

		retval = ext2fs_ڈھونڈو_پہلا_zero_inode_bitmap2(map, i, upto,
							      &پہلا_zero);
		اگر (retval == 0) {
			i = پہلا_zero;
			توڑو;
		}
		اگر (retval != ENOENT)
			واپس EXT2_ET_INODE_ALLOC_FAIL;
		i = upto + 1;
		اگر (i > fs->super->s_inodes_گنتی)
			i = EXT2_پہلا_INODE(fs->super);
	} جب (i != آغاز_inode);

	اگر (ext2fs_test_inode_bitmap2(map, i))
		واپس EXT2_ET_INODE_ALLOC_FAIL;
	*ret = i;
	واپس 0;
}

/*
 * Stupid algorithm --- we now just search forward starting from the
 * goal.  Should put in a smarter one someday....
 */
errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
			    ext2fs_بلاک_bitmap map, blk64_t *ret,
			    ساخت blk_alloc_ctx *ctx)
{
	errcode_t retval;
	blk64_t	b = 0;
	errcode_t (*gab)(ext2_filsys fs, blk64_t goal, blk64_t *ret);
	errcode_t (*gab2)(ext2_filsys, blk64_t, blk64_t *,
			  ساخت blk_alloc_ctx *);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!map) {
		/*
		 * In case there are clients out there whose get_alloc_block
		 * handlers call ext2fs_new_block2 with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		اگر (fs->get_alloc_block2) {
			gab2 = fs->get_alloc_block2;
			fs->get_alloc_block2 = تہی;
			retval = gab2(fs, goal, &b, ctx);
			fs->get_alloc_block2 = gab2;
			جاؤ allocated;
		} ورنہ اگر (fs->get_alloc_بلاک) {
			gab = fs->get_alloc_بلاک;
			fs->get_alloc_بلاک = تہی;
			retval = gab(fs, goal, &b);
			fs->get_alloc_بلاک = gab;
			جاؤ allocated;
		}
	}
	اگر (!map)
		map = fs->بلاک_map;
	اگر (!map)
		واپس EXT2_ET_NO_بلاک_BITMAP;
	اگر (!goal || (goal >= ext2fs_بلاکیں_گنتی(fs->super)))
		goal = fs->super->s_پہلا_ڈیٹا_بلاک;
	goal &= ~EXT2_FS_CLUSTER_نقاب(fs);

	retval = ext2fs_ڈھونڈو_پہلا_zero_بلاک_bitmap2(map,
			goal, ext2fs_بلاکیں_گنتی(fs->super) - 1, &b);
	اگر ((retval == ENOENT) && (goal != fs->super->s_پہلا_ڈیٹا_بلاک))
		retval = ext2fs_ڈھونڈو_پہلا_zero_بلاک_bitmap2(map,
			fs->super->s_پہلا_ڈیٹا_بلاک, goal - 1, &b);
allocated:
	اگر (retval == ENOENT)
		واپس EXT2_ET_بلاک_ALLOC_FAIL;
	اگر (retval)
		واپس retval;

	ext2fs_clear_بلاک_uninit(fs, ext2fs_گروہ_کا_blk2(fs, b));
	*ret = b;
	واپس 0;
}

errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
			   ext2fs_بلاک_bitmap map, blk64_t *ret)
{
	واپس ext2fs_new_block3(fs, goal, map, ret, تہی);
}

errcode_t ext2fs_new_بلاک(ext2_filsys fs, blk_t goal,
			   ext2fs_بلاک_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_new_block2(fs, goal, map, &val);
	اگر (!retval)
		*ret = (blk_t) val;
	واپس retval;
}

/*
 * This function zeros out the allocated block, and updates all of the
 * appropriate filesystem records.
 */
errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal, حرف *بلاک_buf,
			      blk64_t *ret, ساخت blk_alloc_ctx *ctx)
{
	errcode_t	retval;
	blk64_t		بلاک;

	اگر (fs->get_alloc_block2) {
		retval = (fs->get_alloc_block2)(fs, goal, &بلاک, ctx);
		اگر (retval)
			جاؤ fail;
	} ورنہ اگر (fs->get_alloc_بلاک) {
		retval = (fs->get_alloc_بلاک)(fs, goal, &بلاک);
		اگر (retval)
			جاؤ fail;
	} ورنہ {
		اگر (!fs->بلاک_map) {
			retval = ext2fs_read_بلاک_bitmap(fs);
			اگر (retval)
				جاؤ fail;
		}

		retval = ext2fs_new_block3(fs, goal, 0, &بلاک, ctx);
		اگر (retval)
			جاؤ fail;
	}

	اگر (بلاک_buf) {
		memset(بلاک_buf, 0, fs->blocksize);
		retval = io_channel_write_blk64(fs->io, بلاک, 1, بلاک_buf);
	} ورنہ
		retval = ext2fs_zero_blocks2(fs, بلاک, 1, تہی, تہی);
	اگر (retval)
		جاؤ fail;

	ext2fs_بلاک_alloc_stats2(fs, بلاک, +1);
	*ret = بلاک;

fail:
	واپس retval;
}

errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
			     حرف *بلاک_buf, blk64_t *ret)
{
	واپس ext2fs_alloc_block3(fs, goal, بلاک_buf, ret, تہی);
}

errcode_t ext2fs_alloc_بلاک(ext2_filsys fs, blk_t goal,
			     حرف *بلاک_buf, blk_t *ret)
{
	errcode_t retval;
	blk64_t ret64, goal64 = goal;
	retval = ext2fs_alloc_block3(fs, goal64, بلاک_buf, &ret64, تہی);
	اگر (!retval)
		*ret = (blk_t)ret64;
        واپس retval;
}

errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t آغاز, blk64_t اختتام,
				 صحیح عدد, ext2fs_بلاک_bitmap map, blk64_t *ret)
{
	blk64_t	b = آغاز;
	صحیح	c_تناسب;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!map)
		map = fs->بلاک_map;
	اگر (!map)
		واپس EXT2_ET_NO_بلاک_BITMAP;
	اگر (!b)
		b = fs->super->s_پہلا_ڈیٹا_بلاک;
	اگر (!اختتام)
		اختتام = آغاز;
	اگر (!عدد)
		عدد = 1;
	c_تناسب = 1 << ext2fs_get_bitmap_granularity(map);
	b &= ~(c_تناسب - 1);
	اختتام &= ~(c_تناسب -1);
	کرو {
		اگر (b + عدد - 1 >= ext2fs_بلاکیں_گنتی(fs->super)) {
			اگر (اختتام > آغاز)
				واپس EXT2_ET_بلاک_ALLOC_FAIL;
			b = fs->super->s_پہلا_ڈیٹا_بلاک;
		}
		اگر (ext2fs_fast_test_بلاک_bitmap_range2(map, b, عدد)) {
			*ret = b;
			واپس 0;
		}
		b += c_تناسب;
	} جب (b != اختتام);
	واپس EXT2_ET_بلاک_ALLOC_FAIL;
}

errcode_t ext2fs_get_free_بلاکیں(ext2_filsys fs, blk_t آغاز, blk_t اختتام,
				 صحیح عدد, ext2fs_بلاک_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_get_free_blocks2(fs, آغاز, اختتام, عدد, map, &val);
	اگر(!retval)
		*ret = (blk_t) val;
	واپس retval;
}

خالی ext2fs_سیٹ_alloc_بلاک_callback(ext2_filsys fs,
				     errcode_t (*func)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret),
				     errcode_t (**old)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret))
{
	اگر (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		واپس;

	اگر (old)
		*old = fs->get_alloc_بلاک;

	fs->get_alloc_بلاک = func;
}

blk64_t ext2fs_ڈھونڈو_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       ساخت ext2_inode *inode, blk64_t lblk)
{
	dgrp_t			گروہ;
	__u8			log_flex;
	ساخت ext2fs_extent	extent;
	ext2_extent_handle_t	handle = تہی;
	errcode_t		err;

	/* Make sure data stored in inode->i_block is neither fast symlink nor
	 * inline data.
	 */
	اگر (inode == تہی || ext2fs_is_fast_symlink(inode) ||
	    inode->i_flags & EXT4_INLINE_ڈیٹا_FL)
		جاؤ no_بلاکیں;

	اگر (inode->i_flags & EXT4_EXTENTS_FL) {
		err = ext2fs_extent_open2(fs, ino, inode, &handle);
		اگر (err)
			جاؤ no_بلاکیں;
		err = ext2fs_extent_goto2(handle, 0, lblk);
		اگر (err)
			جاؤ no_بلاکیں;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
		اگر (err)
			جاؤ no_بلاکیں;
		ext2fs_extent_free(handle);
		واپس extent.e_pblk + (lblk - extent.e_lblk);
	}

	/* block mapped file; see if block zero is mapped? */
	اگر (inode->i_بلاک[0])
		واپس inode->i_بلاک[0];

no_بلاکیں:
	ext2fs_extent_free(handle);
	log_flex = fs->super->s_log_groups_فی_flex;
	گروہ = ext2fs_گروہ_کا_ino(fs, ino);
	اگر (log_flex)
		گروہ = گروہ & ~((1 << (log_flex)) - 1);
	واپس ext2fs_گروہ_پہلا_block2(fs, گروہ);
}

/*
 * Starting at _goal_, scan around the filesystem to find a run of free blocks
 * that's at least _len_ blocks long.  Possible flags:
 * - EXT2_NEWRANGE_EXACT_GOAL: The range of blocks must start at _goal_.
 * - EXT2_NEWRANGE_MIN_LENGTH: do not return a allocation shorter than _len_.
 * - EXT2_NEWRANGE_ZERO_BLOCKS: Zero blocks pblk to pblk+plen before returning.
 *
 * The starting block is returned in _pblk_ and the length is returned via
 * _plen_.  The blocks are not marked in the bitmap; the caller must mark
 * however much of the returned run they actually use, hopefully via
 * ext2fs_block_alloc_stats_range().
 *
 * This function can return a range that is longer than what was requested.
 */
errcode_t ext2fs_new_range(ext2_filsys fs, صحیح flags, blk64_t goal,
			   blk64_t len, ext2fs_بلاک_bitmap map, blk64_t *pblk,
			   blk64_t *plen)
{
	errcode_t retval;
	blk64_t آغاز, انتہا, b;
	صحیح looped = 0;
	blk64_t max_بلاکیں = ext2fs_بلاکیں_گنتی(fs->super);
	errcode_t (*nrf)(ext2_filsys fs, صحیح flags, blk64_t goal,
			 blk64_t len, blk64_t *pblk, blk64_t *plen);

	dbg_printf("%s: flags=0x%x goal=%llu len=%llu\n", __func__, flags,
		   goal, len);
	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	اگر (len == 0 || (flags & ~EXT2_NEWRANGE_ALL_FLAGS))
		واپس EXT2_ET_INVALID_دلیل;

	اگر (!map && fs->new_range) {
		/*
		 * In case there are clients out there whose new_range
		 * handlers call ext2fs_new_range with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		nrf = fs->new_range;
		fs->new_range = تہی;
		retval = nrf(fs, flags, goal, len, pblk, plen);
		fs->new_range = nrf;
		اگر (retval)
			واپس retval;
		آغاز = *pblk;
		انتہا = *pblk + *plen;
		جاؤ allocated;
	}
	اگر (!map)
		map = fs->بلاک_map;
	اگر (!map)
		واپس EXT2_ET_NO_بلاک_BITMAP;
	اگر (!goal || goal >= ext2fs_بلاکیں_گنتی(fs->super))
		goal = fs->super->s_پہلا_ڈیٹا_بلاک;

	آغاز = goal;
	جب (!looped || آغاز <= goal) {
		retval = ext2fs_ڈھونڈو_پہلا_zero_بلاک_bitmap2(map, آغاز,
							      max_بلاکیں - 1,
							      &آغاز);
		اگر (retval == ENOENT) {
			/*
			 * If there are no free blocks beyond the starting
			 * point, try scanning the whole filesystem, unless the
			 * user told us only to allocate from _goal_, or if
			 * we're already scanning the whole filesystem.
			 */
			اگر (flags & EXT2_NEWRANGE_FIXED_GOAL ||
			    آغاز == fs->super->s_پہلا_ڈیٹا_بلاک)
				جاؤ fail;
			آغاز = fs->super->s_پہلا_ڈیٹا_بلاک;
			جاری;
		} ورنہ اگر (retval)
			جاؤ errout;

		اگر (flags & EXT2_NEWRANGE_FIXED_GOAL && آغاز != goal)
			جاؤ fail;

		b = min(آغاز + len - 1, max_بلاکیں - 1);
		retval =  ext2fs_ڈھونڈو_پہلا_سیٹ_بلاک_bitmap2(map, آغاز, b,
							      &انتہا);
		اگر (retval == ENOENT)
			انتہا = b + 1;
		ورنہ اگر (retval)
			جاؤ errout;

		اگر (!(flags & EXT2_NEWRANGE_MIN_لمبائی) ||
		    (انتہا - آغاز) >= len) {
			/* Success! */
			*pblk = آغاز;
			*plen = انتہا - آغاز;
			dbg_printf("%s: new_range goal=%llu--%llu "
				   "blk=%llu--%llu %llu\n",
				   __func__, goal, goal + len - 1,
				   *pblk, *pblk + *plen - 1, *plen);
allocated:
			برائے (b = آغاز; b < انتہا;
			     b += fs->super->s_بلاکیں_فی_گروہ)
				ext2fs_clear_بلاک_uninit(fs,
						ext2fs_گروہ_کا_blk2(fs, b));
			واپس 0;
		}

		اگر (flags & EXT2_NEWRANGE_FIXED_GOAL)
			جاؤ fail;
		آغاز = انتہا;
		اگر (آغاز >= max_بلاکیں) {
			اگر (looped)
				جاؤ fail;
			looped = 1;
			آغاز = fs->super->s_پہلا_ڈیٹا_بلاک;
		}
	}

fail:
	retval = EXT2_ET_بلاک_ALLOC_FAIL;
errout:
	واپس retval;
}

خالی ext2fs_سیٹ_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, صحیح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, صحیح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen))
{
	اگر (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		واپس;

	اگر (old)
		*old = fs->new_range;

	fs->new_range = func;
}

errcode_t ext2fs_alloc_range(ext2_filsys fs, صحیح flags, blk64_t goal,
			     blk_t len, blk64_t *ret)
{
	صحیح newr_flags = EXT2_NEWRANGE_MIN_لمبائی;
	errcode_t retval;
	blk64_t plen;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	اگر (len == 0 || (flags & ~EXT2_ALLOCRANGE_ALL_FLAGS))
		واپس EXT2_ET_INVALID_دلیل;

	اگر (flags & EXT2_ALLOCRANGE_FIXED_GOAL)
		newr_flags |= EXT2_NEWRANGE_FIXED_GOAL;

	retval = ext2fs_new_range(fs, newr_flags, goal, len, تہی, ret, &plen);
	اگر (retval)
		واپس retval;

	اگر (plen < len)
		واپس EXT2_ET_بلاک_ALLOC_FAIL;

	اگر (flags & EXT2_ALLOCRANGE_ZERO_بلاکیں) {
		retval = ext2fs_zero_blocks2(fs, *ret, len, تہی, تہی);
		اگر (retval)
			واپس retval;
	}

	ext2fs_بلاک_alloc_stats_range(fs, *ret, len, +1);
	واپس retval;
}
