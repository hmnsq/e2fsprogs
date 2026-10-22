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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <time.h>
#ضمن <string.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
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
 * Clear the uninit block bitmap flag if necessary
 */
فراغ ext2fs_clear_بلوک_uninit(ext2_filsys fs, dgrp_t گروه)
{
	إذا (گروه >= fs->گروه_desc_شمار ||
	    !ext2fs_has_گروه_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, گروه, EXT2_BG_بلوک_UNINIT)))
		أرجع;

	/* uninit block bitmaps are now initialized in read_bitmaps() */

	ext2fs_bg_flags_clear(fs, گروه, EXT2_BG_بلوک_UNINIT);
	ext2fs_گروه_desc_csum_مجموعه(fs, گروه);
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
}

/*
 * Check for uninit inode bitmaps and deal with them appropriately
 */
ساكن فراغ check_inode_uninit(ext2_filsys fs, ext2fs_inode_bitmap map,
			  dgrp_t گروه)
{
	ext2_ino_t	i, ino;

	إذا (گروه >= fs->گروه_desc_شمار ||
	    !ext2fs_has_گروه_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, گروه, EXT2_BG_INODE_UNINIT)))
		أرجع;

	ino = (گروه * fs->super->s_inodes_در_هر_گروه) + 1;
	لكل (i=0; i < fs->super->s_inodes_در_هر_گروه; i++, ino++)
		ext2fs_fast_unmark_inode_bitmap2(map, ino);

	ext2fs_bg_flags_clear(fs, گروه, EXT2_BG_INODE_UNINIT);
	/* Mimics what the kernel does */
	ext2fs_bg_flags_clear(fs, گروه, EXT2_BG_بلوک_UNINIT);
	ext2fs_گروه_desc_csum_مجموعه(fs, گروه);
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
			   صحيح نما EXT2FS_ATTR((استفادهنشده)),
			   ext2fs_inode_bitmap map, ext2_ino_t *ret)
{
	ext2_ino_t	شروع_inode = 0;
	ext2_ino_t	i, ino_درون_گروه, upto, اولی_zero;
	errcode_t	retval;
	dgrp_t		گروه;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!map)
		map = fs->inode_map;
	إذا (!map)
		أرجع EXT2_ET_NO_INODE_BITMAP;

	إذا (dir > 0) {
		گروه = (dir - 1) / EXT2_INODES_در_هر_گروه(fs->super);
		شروع_inode = (گروه * EXT2_INODES_در_هر_گروه(fs->super)) + 1;
	}
	إذا (شروع_inode < EXT2_اولی_INODE(fs->super))
		شروع_inode = EXT2_اولی_INODE(fs->super);
	إذا (شروع_inode > fs->super->s_inodes_شمار)
		أرجع EXT2_ET_INODE_ALLOC_FAIL;
	i = شروع_inode;
	افعل {
		ino_درون_گروه = (i - 1) % EXT2_INODES_در_هر_گروه(fs->super);
		گروه = (i - 1) / EXT2_INODES_در_هر_گروه(fs->super);

		check_inode_uninit(fs, map, گروه);
		upto = i + (EXT2_INODES_در_هر_گروه(fs->super) - ino_درون_گروه);
		إذا (i < شروع_inode && upto >= شروع_inode)
			upto = شروع_inode - 1;
		إذا (upto > fs->super->s_inodes_شمار)
			upto = fs->super->s_inodes_شمار;

		retval = ext2fs_پیدا_کن_اولی_zero_inode_bitmap2(map, i, upto,
							      &اولی_zero);
		إذا (retval == 0) {
			i = اولی_zero;
			اكسر;
		}
		إذا (retval != ENOENT)
			أرجع EXT2_ET_INODE_ALLOC_FAIL;
		i = upto + 1;
		إذا (i > fs->super->s_inodes_شمار)
			i = EXT2_اولی_INODE(fs->super);
	} طالما (i != شروع_inode);

	إذا (ext2fs_test_inode_bitmap2(map, i))
		أرجع EXT2_ET_INODE_ALLOC_FAIL;
	*ret = i;
	أرجع 0;
}

/*
 * Stupid algorithm --- we now just search forward starting from the
 * goal.  Should put in a smarter one someday....
 */
errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
			    ext2fs_بلوک_bitmap map, blk64_t *ret,
			    بنية blk_alloc_ctx *ctx)
{
	errcode_t retval;
	blk64_t	b = 0;
	errcode_t (*gab)(ext2_filsys fs, blk64_t goal, blk64_t *ret);
	errcode_t (*gab2)(ext2_filsys, blk64_t, blk64_t *,
			  بنية blk_alloc_ctx *);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!map) {
		/*
		 * In case there are clients out there whose get_alloc_block
		 * handlers call ext2fs_new_block2 with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		إذا (fs->get_alloc_block2) {
			gab2 = fs->get_alloc_block2;
			fs->get_alloc_block2 = عدم;
			retval = gab2(fs, goal, &b, ctx);
			fs->get_alloc_block2 = gab2;
			اذهب allocated;
		} وإلا إذا (fs->get_alloc_بلوک) {
			gab = fs->get_alloc_بلوک;
			fs->get_alloc_بلوک = عدم;
			retval = gab(fs, goal, &b);
			fs->get_alloc_بلوک = gab;
			اذهب allocated;
		}
	}
	إذا (!map)
		map = fs->بلوک_map;
	إذا (!map)
		أرجع EXT2_ET_NO_بلوک_BITMAP;
	إذا (!goal || (goal >= ext2fs_بلوکها_شمار(fs->super)))
		goal = fs->super->s_اولی_داده_بلوک;
	goal &= ~EXT2_FS_CLUSTER_ماسک(fs);

	retval = ext2fs_پیدا_کن_اولی_zero_بلوک_bitmap2(map,
			goal, ext2fs_بلوکها_شمار(fs->super) - 1, &b);
	إذا ((retval == ENOENT) && (goal != fs->super->s_اولی_داده_بلوک))
		retval = ext2fs_پیدا_کن_اولی_zero_بلوک_bitmap2(map,
			fs->super->s_اولی_داده_بلوک, goal - 1, &b);
allocated:
	إذا (retval == ENOENT)
		أرجع EXT2_ET_بلوک_ALLOC_FAIL;
	إذا (retval)
		أرجع retval;

	ext2fs_clear_بلوک_uninit(fs, ext2fs_گروه_از_blk2(fs, b));
	*ret = b;
	أرجع 0;
}

errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
			   ext2fs_بلوک_bitmap map, blk64_t *ret)
{
	أرجع ext2fs_new_block3(fs, goal, map, ret, عدم);
}

errcode_t ext2fs_new_بلوک(ext2_filsys fs, blk_t goal,
			   ext2fs_بلوک_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_new_block2(fs, goal, map, &val);
	إذا (!retval)
		*ret = (blk_t) val;
	أرجع retval;
}

/*
 * This function zeros out the allocated block, and updates all of the
 * appropriate filesystem records.
 */
errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal, حرف *بلوک_buf,
			      blk64_t *ret, بنية blk_alloc_ctx *ctx)
{
	errcode_t	retval;
	blk64_t		بلوک;

	إذا (fs->get_alloc_block2) {
		retval = (fs->get_alloc_block2)(fs, goal, &بلوک, ctx);
		إذا (retval)
			اذهب fail;
	} وإلا إذا (fs->get_alloc_بلوک) {
		retval = (fs->get_alloc_بلوک)(fs, goal, &بلوک);
		إذا (retval)
			اذهب fail;
	} وإلا {
		إذا (!fs->بلوک_map) {
			retval = ext2fs_read_بلوک_bitmap(fs);
			إذا (retval)
				اذهب fail;
		}

		retval = ext2fs_new_block3(fs, goal, 0, &بلوک, ctx);
		إذا (retval)
			اذهب fail;
	}

	إذا (بلوک_buf) {
		memset(بلوک_buf, 0, fs->blocksize);
		retval = io_channel_write_blk64(fs->io, بلوک, 1, بلوک_buf);
	} وإلا
		retval = ext2fs_zero_blocks2(fs, بلوک, 1, عدم, عدم);
	إذا (retval)
		اذهب fail;

	ext2fs_بلوک_alloc_stats2(fs, بلوک, +1);
	*ret = بلوک;

fail:
	أرجع retval;
}

errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
			     حرف *بلوک_buf, blk64_t *ret)
{
	أرجع ext2fs_alloc_block3(fs, goal, بلوک_buf, ret, عدم);
}

errcode_t ext2fs_alloc_بلوک(ext2_filsys fs, blk_t goal,
			     حرف *بلوک_buf, blk_t *ret)
{
	errcode_t retval;
	blk64_t ret64, goal64 = goal;
	retval = ext2fs_alloc_block3(fs, goal64, بلوک_buf, &ret64, عدم);
	إذا (!retval)
		*ret = (blk_t)ret64;
        أرجع retval;
}

errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t شروع, blk64_t پایان,
				 صحيح رقم, ext2fs_بلوک_bitmap map, blk64_t *ret)
{
	blk64_t	b = شروع;
	صحيح	c_نسبت;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!map)
		map = fs->بلوک_map;
	إذا (!map)
		أرجع EXT2_ET_NO_بلوک_BITMAP;
	إذا (!b)
		b = fs->super->s_اولی_داده_بلوک;
	إذا (!پایان)
		پایان = شروع;
	إذا (!رقم)
		رقم = 1;
	c_نسبت = 1 << ext2fs_get_bitmap_granularity(map);
	b &= ~(c_نسبت - 1);
	پایان &= ~(c_نسبت -1);
	افعل {
		إذا (b + رقم - 1 >= ext2fs_بلوکها_شمار(fs->super)) {
			إذا (پایان > شروع)
				أرجع EXT2_ET_بلوک_ALLOC_FAIL;
			b = fs->super->s_اولی_داده_بلوک;
		}
		إذا (ext2fs_fast_test_بلوک_bitmap_range2(map, b, رقم)) {
			*ret = b;
			أرجع 0;
		}
		b += c_نسبت;
	} طالما (b != پایان);
	أرجع EXT2_ET_بلوک_ALLOC_FAIL;
}

errcode_t ext2fs_get_free_بلوکها(ext2_filsys fs, blk_t شروع, blk_t پایان,
				 صحيح رقم, ext2fs_بلوک_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_get_free_blocks2(fs, شروع, پایان, رقم, map, &val);
	إذا(!retval)
		*ret = (blk_t) val;
	أرجع retval;
}

فراغ ext2fs_مجموعه_alloc_بلوک_callback(ext2_filsys fs,
				     errcode_t (*func)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret),
				     errcode_t (**old)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret))
{
	إذا (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		أرجع;

	إذا (old)
		*old = fs->get_alloc_بلوک;

	fs->get_alloc_بلوک = func;
}

blk64_t ext2fs_پیدا_کن_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode *inode, blk64_t lblk)
{
	dgrp_t			گروه;
	__u8			log_flex;
	بنية ext2fs_extent	extent;
	ext2_extent_handle_t	handle = عدم;
	errcode_t		err;

	/* Make sure data stored in inode->i_block is neither fast symlink nor
	 * inline data.
	 */
	إذا (inode == عدم || ext2fs_is_fast_symlink(inode) ||
	    inode->i_flags & EXT4_INLINE_داده_FL)
		اذهب no_بلوکها;

	إذا (inode->i_flags & EXT4_EXTENTS_FL) {
		err = ext2fs_extent_open2(fs, ino, inode, &handle);
		إذا (err)
			اذهب no_بلوکها;
		err = ext2fs_extent_goto2(handle, 0, lblk);
		إذا (err)
			اذهب no_بلوکها;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
		إذا (err)
			اذهب no_بلوکها;
		ext2fs_extent_free(handle);
		أرجع extent.e_pblk + (lblk - extent.e_lblk);
	}

	/* block mapped file; see if block zero is mapped? */
	إذا (inode->i_بلوک[0])
		أرجع inode->i_بلوک[0];

no_بلوکها:
	ext2fs_extent_free(handle);
	log_flex = fs->super->s_log_groups_در_هر_flex;
	گروه = ext2fs_گروه_از_ino(fs, ino);
	إذا (log_flex)
		گروه = گروه & ~((1 << (log_flex)) - 1);
	أرجع ext2fs_گروه_اولی_block2(fs, گروه);
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
errcode_t ext2fs_new_range(ext2_filsys fs, صحيح flags, blk64_t goal,
			   blk64_t len, ext2fs_بلوک_bitmap map, blk64_t *pblk,
			   blk64_t *plen)
{
	errcode_t retval;
	blk64_t شروع, انتها, b;
	صحيح looped = 0;
	blk64_t max_بلوکها = ext2fs_بلوکها_شمار(fs->super);
	errcode_t (*nrf)(ext2_filsys fs, صحيح flags, blk64_t goal,
			 blk64_t len, blk64_t *pblk, blk64_t *plen);

	dbg_printf("%s: flags=0x%x goal=%llu len=%llu\n", __func__, flags,
		   goal, len);
	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	إذا (len == 0 || (flags & ~EXT2_NEWRANGE_ALL_FLAGS))
		أرجع EXT2_ET_INVALID_آرگومان;

	إذا (!map && fs->new_range) {
		/*
		 * In case there are clients out there whose new_range
		 * handlers call ext2fs_new_range with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		nrf = fs->new_range;
		fs->new_range = عدم;
		retval = nrf(fs, flags, goal, len, pblk, plen);
		fs->new_range = nrf;
		إذا (retval)
			أرجع retval;
		شروع = *pblk;
		انتها = *pblk + *plen;
		اذهب allocated;
	}
	إذا (!map)
		map = fs->بلوک_map;
	إذا (!map)
		أرجع EXT2_ET_NO_بلوک_BITMAP;
	إذا (!goal || goal >= ext2fs_بلوکها_شمار(fs->super))
		goal = fs->super->s_اولی_داده_بلوک;

	شروع = goal;
	طالما (!looped || شروع <= goal) {
		retval = ext2fs_پیدا_کن_اولی_zero_بلوک_bitmap2(map, شروع,
							      max_بلوکها - 1,
							      &شروع);
		إذا (retval == ENOENT) {
			/*
			 * If there are no free blocks beyond the starting
			 * point, try scanning the whole filesystem, unless the
			 * user told us only to allocate from _goal_, or if
			 * we're already scanning the whole filesystem.
			 */
			إذا (flags & EXT2_NEWRANGE_FIXED_GOAL ||
			    شروع == fs->super->s_اولی_داده_بلوک)
				اذهب fail;
			شروع = fs->super->s_اولی_داده_بلوک;
			استمر;
		} وإلا إذا (retval)
			اذهب errout;

		إذا (flags & EXT2_NEWRANGE_FIXED_GOAL && شروع != goal)
			اذهب fail;

		b = min(شروع + len - 1, max_بلوکها - 1);
		retval =  ext2fs_پیدا_کن_اولی_مجموعه_بلوک_bitmap2(map, شروع, b,
							      &انتها);
		إذا (retval == ENOENT)
			انتها = b + 1;
		وإلا إذا (retval)
			اذهب errout;

		إذا (!(flags & EXT2_NEWRANGE_MIN_درازی) ||
		    (انتها - شروع) >= len) {
			/* Success! */
			*pblk = شروع;
			*plen = انتها - شروع;
			dbg_printf("%s: new_range goal=%llu--%llu "
				   "blk=%llu--%llu %llu\n",
				   __func__, goal, goal + len - 1,
				   *pblk, *pblk + *plen - 1, *plen);
allocated:
			لكل (b = شروع; b < انتها;
			     b += fs->super->s_بلوکها_در_هر_گروه)
				ext2fs_clear_بلوک_uninit(fs,
						ext2fs_گروه_از_blk2(fs, b));
			أرجع 0;
		}

		إذا (flags & EXT2_NEWRANGE_FIXED_GOAL)
			اذهب fail;
		شروع = انتها;
		إذا (شروع >= max_بلوکها) {
			إذا (looped)
				اذهب fail;
			looped = 1;
			شروع = fs->super->s_اولی_داده_بلوک;
		}
	}

fail:
	retval = EXT2_ET_بلوک_ALLOC_FAIL;
errout:
	أرجع retval;
}

فراغ ext2fs_مجموعه_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, صحيح flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen))
{
	إذا (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		أرجع;

	إذا (old)
		*old = fs->new_range;

	fs->new_range = func;
}

errcode_t ext2fs_alloc_range(ext2_filsys fs, صحيح flags, blk64_t goal,
			     blk_t len, blk64_t *ret)
{
	صحيح newr_flags = EXT2_NEWRANGE_MIN_درازی;
	errcode_t retval;
	blk64_t plen;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	إذا (len == 0 || (flags & ~EXT2_ALLOCRANGE_ALL_FLAGS))
		أرجع EXT2_ET_INVALID_آرگومان;

	إذا (flags & EXT2_ALLOCRANGE_FIXED_GOAL)
		newr_flags |= EXT2_NEWRANGE_FIXED_GOAL;

	retval = ext2fs_new_range(fs, newr_flags, goal, len, عدم, ret, &plen);
	إذا (retval)
		أرجع retval;

	إذا (plen < len)
		أرجع EXT2_ET_بلوک_ALLOC_FAIL;

	إذا (flags & EXT2_ALLOCRANGE_ZERO_بلوکها) {
		retval = ext2fs_zero_blocks2(fs, *ret, len, عدم, عدم);
		إذا (retval)
			أرجع retval;
	}

	ext2fs_بلوک_alloc_stats_range(fs, *ret, len, +1);
	أرجع retval;
}
