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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
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
 * Clear the uninit block bitmap flag if necessary
 */
ਖਾਲੀ ext2fs_clear_ਖੰਡ_uninit(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਜੇ (ਸਮੂਹ >= fs->ਸਮੂਹ_desc_ਗਿਣਤੀ ||
	    !ext2fs_has_ਸਮੂਹ_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, ਸਮੂਹ, EXT2_BG_ਖੰਡ_UNINIT)))
		ਵਾਪਸ;

	/* uninit block bitmaps are now initialized in read_bitmaps() */

	ext2fs_bg_flags_clear(fs, ਸਮੂਹ, EXT2_BG_ਖੰਡ_UNINIT);
	ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, ਸਮੂਹ);
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
}

/*
 * Check for uninit inode bitmaps and deal with them appropriately
 */
ਸਥਿਰ ਖਾਲੀ check_inode_uninit(ext2_filsys fs, ext2fs_inode_bitmap map,
			  dgrp_t ਸਮੂਹ)
{
	ext2_ino_t	i, ino;

	ਜੇ (ਸਮੂਹ >= fs->ਸਮੂਹ_desc_ਗਿਣਤੀ ||
	    !ext2fs_has_ਸਮੂਹ_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, ਸਮੂਹ, EXT2_BG_INODE_UNINIT)))
		ਵਾਪਸ;

	ino = (ਸਮੂਹ * fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ) + 1;
	ਲਈ (i=0; i < fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ; i++, ino++)
		ext2fs_fast_unmark_inode_bitmap2(map, ino);

	ext2fs_bg_flags_clear(fs, ਸਮੂਹ, EXT2_BG_INODE_UNINIT);
	/* Mimics what the kernel does */
	ext2fs_bg_flags_clear(fs, ਸਮੂਹ, EXT2_BG_ਖੰਡ_UNINIT);
	ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, ਸਮੂਹ);
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
			   ਪੂਰਨ ਬਹੁਲਕ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			   ext2fs_inode_bitmap map, ext2_ino_t *ret)
{
	ext2_ino_t	ਸ਼ੁਰੂ_inode = 0;
	ext2_ino_t	i, ino_ਵਿੱਚ_ਸਮੂਹ, upto, ਪਹਿਲਾ_zero;
	errcode_t	retval;
	dgrp_t		ਸਮੂਹ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!map)
		map = fs->inode_map;
	ਜੇ (!map)
		ਵਾਪਸ EXT2_ET_NO_INODE_BITMAP;

	ਜੇ (dir > 0) {
		ਸਮੂਹ = (dir - 1) / EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super);
		ਸ਼ੁਰੂ_inode = (ਸਮੂਹ * EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super)) + 1;
	}
	ਜੇ (ਸ਼ੁਰੂ_inode < EXT2_ਪਹਿਲਾ_INODE(fs->super))
		ਸ਼ੁਰੂ_inode = EXT2_ਪਹਿਲਾ_INODE(fs->super);
	ਜੇ (ਸ਼ੁਰੂ_inode > fs->super->s_inodes_ਗਿਣਤੀ)
		ਵਾਪਸ EXT2_ET_INODE_ALLOC_FAIL;
	i = ਸ਼ੁਰੂ_inode;
	ਕਰੋ {
		ino_ਵਿੱਚ_ਸਮੂਹ = (i - 1) % EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super);
		ਸਮੂਹ = (i - 1) / EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super);

		check_inode_uninit(fs, map, ਸਮੂਹ);
		upto = i + (EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) - ino_ਵਿੱਚ_ਸਮੂਹ);
		ਜੇ (i < ਸ਼ੁਰੂ_inode && upto >= ਸ਼ੁਰੂ_inode)
			upto = ਸ਼ੁਰੂ_inode - 1;
		ਜੇ (upto > fs->super->s_inodes_ਗਿਣਤੀ)
			upto = fs->super->s_inodes_ਗਿਣਤੀ;

		retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_inode_bitmap2(map, i, upto,
							      &ਪਹਿਲਾ_zero);
		ਜੇ (retval == 0) {
			i = ਪਹਿਲਾ_zero;
			ਰੋਕੋ;
		}
		ਜੇ (retval != ENOENT)
			ਵਾਪਸ EXT2_ET_INODE_ALLOC_FAIL;
		i = upto + 1;
		ਜੇ (i > fs->super->s_inodes_ਗਿਣਤੀ)
			i = EXT2_ਪਹਿਲਾ_INODE(fs->super);
	} ਜਦੋਂ (i != ਸ਼ੁਰੂ_inode);

	ਜੇ (ext2fs_test_inode_bitmap2(map, i))
		ਵਾਪਸ EXT2_ET_INODE_ALLOC_FAIL;
	*ret = i;
	ਵਾਪਸ 0;
}

/*
 * Stupid algorithm --- we now just search forward starting from the
 * goal.  Should put in a smarter one someday....
 */
errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
			    ext2fs_ਖੰਡ_bitmap map, blk64_t *ret,
			    ਬਣਤਰ blk_alloc_ctx *ctx)
{
	errcode_t retval;
	blk64_t	b = 0;
	errcode_t (*gab)(ext2_filsys fs, blk64_t goal, blk64_t *ret);
	errcode_t (*gab2)(ext2_filsys, blk64_t, blk64_t *,
			  ਬਣਤਰ blk_alloc_ctx *);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!map) {
		/*
		 * In case there are clients out there whose get_alloc_block
		 * handlers call ext2fs_new_block2 with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		ਜੇ (fs->get_alloc_block2) {
			gab2 = fs->get_alloc_block2;
			fs->get_alloc_block2 = ਨਿਰਾ;
			retval = gab2(fs, goal, &b, ctx);
			fs->get_alloc_block2 = gab2;
			ਜਾਓ allocated;
		} ਨਹੀਂਤਾਂ ਜੇ (fs->get_alloc_ਖੰਡ) {
			gab = fs->get_alloc_ਖੰਡ;
			fs->get_alloc_ਖੰਡ = ਨਿਰਾ;
			retval = gab(fs, goal, &b);
			fs->get_alloc_ਖੰਡ = gab;
			ਜਾਓ allocated;
		}
	}
	ਜੇ (!map)
		map = fs->ਖੰਡ_map;
	ਜੇ (!map)
		ਵਾਪਸ EXT2_ET_NO_ਖੰਡ_BITMAP;
	ਜੇ (!goal || (goal >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)))
		goal = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
	goal &= ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs);

	retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_ਖੰਡ_bitmap2(map,
			goal, ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) - 1, &b);
	ਜੇ ((retval == ENOENT) && (goal != fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ))
		retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_ਖੰਡ_bitmap2(map,
			fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ, goal - 1, &b);
allocated:
	ਜੇ (retval == ENOENT)
		ਵਾਪਸ EXT2_ET_ਖੰਡ_ALLOC_FAIL;
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ext2fs_clear_ਖੰਡ_uninit(fs, ext2fs_ਸਮੂਹ_ਦਾ_blk2(fs, b));
	*ret = b;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
			   ext2fs_ਖੰਡ_bitmap map, blk64_t *ret)
{
	ਵਾਪਸ ext2fs_new_block3(fs, goal, map, ret, ਨਿਰਾ);
}

errcode_t ext2fs_new_ਖੰਡ(ext2_filsys fs, blk_t goal,
			   ext2fs_ਖੰਡ_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_new_block2(fs, goal, map, &val);
	ਜੇ (!retval)
		*ret = (blk_t) val;
	ਵਾਪਸ retval;
}

/*
 * This function zeros out the allocated block, and updates all of the
 * appropriate filesystem records.
 */
errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal, ਅੱਖਰ *ਖੰਡ_buf,
			      blk64_t *ret, ਬਣਤਰ blk_alloc_ctx *ctx)
{
	errcode_t	retval;
	blk64_t		ਖੰਡ;

	ਜੇ (fs->get_alloc_block2) {
		retval = (fs->get_alloc_block2)(fs, goal, &ਖੰਡ, ctx);
		ਜੇ (retval)
			ਜਾਓ fail;
	} ਨਹੀਂਤਾਂ ਜੇ (fs->get_alloc_ਖੰਡ) {
		retval = (fs->get_alloc_ਖੰਡ)(fs, goal, &ਖੰਡ);
		ਜੇ (retval)
			ਜਾਓ fail;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (!fs->ਖੰਡ_map) {
			retval = ext2fs_read_ਖੰਡ_bitmap(fs);
			ਜੇ (retval)
				ਜਾਓ fail;
		}

		retval = ext2fs_new_block3(fs, goal, 0, &ਖੰਡ, ctx);
		ਜੇ (retval)
			ਜਾਓ fail;
	}

	ਜੇ (ਖੰਡ_buf) {
		memset(ਖੰਡ_buf, 0, fs->blocksize);
		retval = io_channel_write_blk64(fs->io, ਖੰਡ, 1, ਖੰਡ_buf);
	} ਨਹੀਂਤਾਂ
		retval = ext2fs_zero_blocks2(fs, ਖੰਡ, 1, ਨਿਰਾ, ਨਿਰਾ);
	ਜੇ (retval)
		ਜਾਓ fail;

	ext2fs_ਖੰਡ_alloc_stats2(fs, ਖੰਡ, +1);
	*ret = ਖੰਡ;

fail:
	ਵਾਪਸ retval;
}

errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
			     ਅੱਖਰ *ਖੰਡ_buf, blk64_t *ret)
{
	ਵਾਪਸ ext2fs_alloc_block3(fs, goal, ਖੰਡ_buf, ret, ਨਿਰਾ);
}

errcode_t ext2fs_alloc_ਖੰਡ(ext2_filsys fs, blk_t goal,
			     ਅੱਖਰ *ਖੰਡ_buf, blk_t *ret)
{
	errcode_t retval;
	blk64_t ret64, goal64 = goal;
	retval = ext2fs_alloc_block3(fs, goal64, ਖੰਡ_buf, &ret64, ਨਿਰਾ);
	ਜੇ (!retval)
		*ret = (blk_t)ret64;
        ਵਾਪਸ retval;
}

errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t ਸ਼ੁਰੂ, blk64_t ਅੰਤ,
				 ਪੂਰਨ ਅੰਕ, ext2fs_ਖੰਡ_bitmap map, blk64_t *ret)
{
	blk64_t	b = ਸ਼ੁਰੂ;
	ਪੂਰਨ	c_ਅਨੁਪਾਤ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!map)
		map = fs->ਖੰਡ_map;
	ਜੇ (!map)
		ਵਾਪਸ EXT2_ET_NO_ਖੰਡ_BITMAP;
	ਜੇ (!b)
		b = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
	ਜੇ (!ਅੰਤ)
		ਅੰਤ = ਸ਼ੁਰੂ;
	ਜੇ (!ਅੰਕ)
		ਅੰਕ = 1;
	c_ਅਨੁਪਾਤ = 1 << ext2fs_get_bitmap_granularity(map);
	b &= ~(c_ਅਨੁਪਾਤ - 1);
	ਅੰਤ &= ~(c_ਅਨੁਪਾਤ -1);
	ਕਰੋ {
		ਜੇ (b + ਅੰਕ - 1 >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)) {
			ਜੇ (ਅੰਤ > ਸ਼ੁਰੂ)
				ਵਾਪਸ EXT2_ET_ਖੰਡ_ALLOC_FAIL;
			b = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
		}
		ਜੇ (ext2fs_fast_test_ਖੰਡ_bitmap_range2(map, b, ਅੰਕ)) {
			*ret = b;
			ਵਾਪਸ 0;
		}
		b += c_ਅਨੁਪਾਤ;
	} ਜਦੋਂ (b != ਅੰਤ);
	ਵਾਪਸ EXT2_ET_ਖੰਡ_ALLOC_FAIL;
}

errcode_t ext2fs_get_free_ਖੰਡਾਂ(ext2_filsys fs, blk_t ਸ਼ੁਰੂ, blk_t ਅੰਤ,
				 ਪੂਰਨ ਅੰਕ, ext2fs_ਖੰਡ_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_get_free_blocks2(fs, ਸ਼ੁਰੂ, ਅੰਤ, ਅੰਕ, map, &val);
	ਜੇ(!retval)
		*ret = (blk_t) val;
	ਵਾਪਸ retval;
}

ਖਾਲੀ ext2fs_ਸੈੱਟ_alloc_ਖੰਡ_callback(ext2_filsys fs,
				     errcode_t (*func)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret),
				     errcode_t (**old)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret))
{
	ਜੇ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ਵਾਪਸ;

	ਜੇ (old)
		*old = fs->get_alloc_ਖੰਡ;

	fs->get_alloc_ਖੰਡ = func;
}

blk64_t ext2fs_ਲੱਭੋ_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       ਬਣਤਰ ext2_inode *inode, blk64_t lblk)
{
	dgrp_t			ਸਮੂਹ;
	__u8			log_flex;
	ਬਣਤਰ ext2fs_extent	extent;
	ext2_extent_handle_t	handle = ਨਿਰਾ;
	errcode_t		err;

	/* Make sure data stored in inode->i_block is neither fast symlink nor
	 * inline data.
	 */
	ਜੇ (inode == ਨਿਰਾ || ext2fs_is_fast_symlink(inode) ||
	    inode->i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL)
		ਜਾਓ no_ਖੰਡਾਂ;

	ਜੇ (inode->i_flags & EXT4_EXTENTS_FL) {
		err = ext2fs_extent_open2(fs, ino, inode, &handle);
		ਜੇ (err)
			ਜਾਓ no_ਖੰਡਾਂ;
		err = ext2fs_extent_goto2(handle, 0, lblk);
		ਜੇ (err)
			ਜਾਓ no_ਖੰਡਾਂ;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
		ਜੇ (err)
			ਜਾਓ no_ਖੰਡਾਂ;
		ext2fs_extent_free(handle);
		ਵਾਪਸ extent.e_pblk + (lblk - extent.e_lblk);
	}

	/* block mapped file; see if block zero is mapped? */
	ਜੇ (inode->i_ਖੰਡ[0])
		ਵਾਪਸ inode->i_ਖੰਡ[0];

no_ਖੰਡਾਂ:
	ext2fs_extent_free(handle);
	log_flex = fs->super->s_log_groups_ਪ੍ਰਤੀ_flex;
	ਸਮੂਹ = ext2fs_ਸਮੂਹ_ਦਾ_ino(fs, ino);
	ਜੇ (log_flex)
		ਸਮੂਹ = ਸਮੂਹ & ~((1 << (log_flex)) - 1);
	ਵਾਪਸ ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, ਸਮੂਹ);
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
errcode_t ext2fs_new_range(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			   blk64_t len, ext2fs_ਖੰਡ_bitmap map, blk64_t *pblk,
			   blk64_t *plen)
{
	errcode_t retval;
	blk64_t ਸ਼ੁਰੂ, ਸਿਰਾ, b;
	ਪੂਰਨ looped = 0;
	blk64_t max_ਖੰਡਾਂ = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super);
	errcode_t (*nrf)(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			 blk64_t len, blk64_t *pblk, blk64_t *plen);

	dbg_printf("%s: flags=0x%x goal=%llu len=%llu\n", __func__, flags,
		   goal, len);
	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	ਜੇ (len == 0 || (flags & ~EXT2_NEWRANGE_ALL_FLAGS))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (!map && fs->new_range) {
		/*
		 * In case there are clients out there whose new_range
		 * handlers call ext2fs_new_range with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		nrf = fs->new_range;
		fs->new_range = ਨਿਰਾ;
		retval = nrf(fs, flags, goal, len, pblk, plen);
		fs->new_range = nrf;
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਸ਼ੁਰੂ = *pblk;
		ਸਿਰਾ = *pblk + *plen;
		ਜਾਓ allocated;
	}
	ਜੇ (!map)
		map = fs->ਖੰਡ_map;
	ਜੇ (!map)
		ਵਾਪਸ EXT2_ET_NO_ਖੰਡ_BITMAP;
	ਜੇ (!goal || goal >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))
		goal = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;

	ਸ਼ੁਰੂ = goal;
	ਜਦੋਂ (!looped || ਸ਼ੁਰੂ <= goal) {
		retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_ਖੰਡ_bitmap2(map, ਸ਼ੁਰੂ,
							      max_ਖੰਡਾਂ - 1,
							      &ਸ਼ੁਰੂ);
		ਜੇ (retval == ENOENT) {
			/*
			 * If there are no free blocks beyond the starting
			 * point, try scanning the whole filesystem, unless the
			 * user told us only to allocate from _goal_, or if
			 * we're already scanning the whole filesystem.
			 */
			ਜੇ (flags & EXT2_NEWRANGE_FIXED_GOAL ||
			    ਸ਼ੁਰੂ == fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ)
				ਜਾਓ fail;
			ਸ਼ੁਰੂ = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ ਜੇ (retval)
			ਜਾਓ errout;

		ਜੇ (flags & EXT2_NEWRANGE_FIXED_GOAL && ਸ਼ੁਰੂ != goal)
			ਜਾਓ fail;

		b = min(ਸ਼ੁਰੂ + len - 1, max_ਖੰਡਾਂ - 1);
		retval =  ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_ਖੰਡ_bitmap2(map, ਸ਼ੁਰੂ, b,
							      &ਸਿਰਾ);
		ਜੇ (retval == ENOENT)
			ਸਿਰਾ = b + 1;
		ਨਹੀਂਤਾਂ ਜੇ (retval)
			ਜਾਓ errout;

		ਜੇ (!(flags & EXT2_NEWRANGE_MIN_ਲੰਮਾਈ) ||
		    (ਸਿਰਾ - ਸ਼ੁਰੂ) >= len) {
			/* Success! */
			*pblk = ਸ਼ੁਰੂ;
			*plen = ਸਿਰਾ - ਸ਼ੁਰੂ;
			dbg_printf("%s: new_range goal=%llu--%llu "
				   "blk=%llu--%llu %llu\n",
				   __func__, goal, goal + len - 1,
				   *pblk, *pblk + *plen - 1, *plen);
allocated:
			ਲਈ (b = ਸ਼ੁਰੂ; b < ਸਿਰਾ;
			     b += fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ)
				ext2fs_clear_ਖੰਡ_uninit(fs,
						ext2fs_ਸਮੂਹ_ਦਾ_blk2(fs, b));
			ਵਾਪਸ 0;
		}

		ਜੇ (flags & EXT2_NEWRANGE_FIXED_GOAL)
			ਜਾਓ fail;
		ਸ਼ੁਰੂ = ਸਿਰਾ;
		ਜੇ (ਸ਼ੁਰੂ >= max_ਖੰਡਾਂ) {
			ਜੇ (looped)
				ਜਾਓ fail;
			looped = 1;
			ਸ਼ੁਰੂ = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
		}
	}

fail:
	retval = EXT2_ET_ਖੰਡ_ALLOC_FAIL;
errout:
	ਵਾਪਸ retval;
}

ਖਾਲੀ ext2fs_ਸੈੱਟ_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen))
{
	ਜੇ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ਵਾਪਸ;

	ਜੇ (old)
		*old = fs->new_range;

	fs->new_range = func;
}

errcode_t ext2fs_alloc_range(ext2_filsys fs, ਪੂਰਨ flags, blk64_t goal,
			     blk_t len, blk64_t *ret)
{
	ਪੂਰਨ newr_flags = EXT2_NEWRANGE_MIN_ਲੰਮਾਈ;
	errcode_t retval;
	blk64_t plen;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	ਜੇ (len == 0 || (flags & ~EXT2_ALLOCRANGE_ALL_FLAGS))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (flags & EXT2_ALLOCRANGE_FIXED_GOAL)
		newr_flags |= EXT2_NEWRANGE_FIXED_GOAL;

	retval = ext2fs_new_range(fs, newr_flags, goal, len, ਨਿਰਾ, ret, &plen);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (plen < len)
		ਵਾਪਸ EXT2_ET_ਖੰਡ_ALLOC_FAIL;

	ਜੇ (flags & EXT2_ALLOCRANGE_ZERO_ਖੰਡਾਂ) {
		retval = ext2fs_zero_blocks2(fs, *ret, len, ਨਿਰਾ, ਨਿਰਾ);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	ext2fs_ਖੰਡ_alloc_stats_range(fs, *ret, len, +1);
	ਵਾਪਸ retval;
}
