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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  করো {printf(f, ## a); fflush(stdout); } যখন (0)
#অন্যথা
# define dbg_printf(f, a...)
#endif

/*
 * Clear the uninit block bitmap flag if necessary
 */
শূন্য ext2fs_clear_block_uninit(ext2_filsys fs, dgrp_t group)
{
	যদি (group >= fs->group_desc_গুনতি ||
	    !ext2fs_has_group_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, group, EXT2_BG_BLOCK_UNINIT)))
		ফেরত;

	/* uninit block bitmaps are now initialized in read_bitmaps() */

	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
}

/*
 * Check for uninit inode bitmaps and deal with them appropriately
 */
স্থির শূন্য check_inode_uninit(ext2_filsys fs, ext2fs_inode_bitmap map,
			  dgrp_t group)
{
	ext2_ino_t	i, ino;

	যদি (group >= fs->group_desc_গুনতি ||
	    !ext2fs_has_group_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, group, EXT2_BG_INODE_UNINIT)))
		ফেরত;

	ino = (group * fs->super->s_inodes_per_group) + 1;
	জন্য (i=0; i < fs->super->s_inodes_per_group; i++, ino++)
		ext2fs_fast_unmark_inode_bitmap2(map, ino);

	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	/* Mimics what the kernel does */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);
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
			   পূর্ণসংখ্যা mode EXT2FS_ATTR((unused)),
			   ext2fs_inode_bitmap map, ext2_ino_t *ret)
{
	ext2_ino_t	start_inode = 0;
	ext2_ino_t	i, ino_in_group, upto, first_zero;
	errcode_t	retval;
	dgrp_t		group;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!map)
		map = fs->inode_map;
	যদি (!map)
		ফেরত EXT2_ET_NO_INODE_BITMAP;

	যদি (dir > 0) {
		group = (dir - 1) / EXT2_INODES_PER_GROUP(fs->super);
		start_inode = (group * EXT2_INODES_PER_GROUP(fs->super)) + 1;
	}
	যদি (start_inode < EXT2_FIRST_INODE(fs->super))
		start_inode = EXT2_FIRST_INODE(fs->super);
	যদি (start_inode > fs->super->s_inodes_গুনতি)
		ফেরত EXT2_ET_INODE_ALLOC_FAIL;
	i = start_inode;
	করো {
		ino_in_group = (i - 1) % EXT2_INODES_PER_GROUP(fs->super);
		group = (i - 1) / EXT2_INODES_PER_GROUP(fs->super);

		check_inode_uninit(fs, map, group);
		upto = i + (EXT2_INODES_PER_GROUP(fs->super) - ino_in_group);
		যদি (i < start_inode && upto >= start_inode)
			upto = start_inode - 1;
		যদি (upto > fs->super->s_inodes_গুনতি)
			upto = fs->super->s_inodes_গুনতি;

		retval = ext2fs_find_first_zero_inode_bitmap2(map, i, upto,
							      &first_zero);
		যদি (retval == 0) {
			i = first_zero;
			বিরতি;
		}
		যদি (retval != ENOENT)
			ফেরত EXT2_ET_INODE_ALLOC_FAIL;
		i = upto + 1;
		যদি (i > fs->super->s_inodes_গুনতি)
			i = EXT2_FIRST_INODE(fs->super);
	} যখন (i != start_inode);

	যদি (ext2fs_test_inode_bitmap2(map, i))
		ফেরত EXT2_ET_INODE_ALLOC_FAIL;
	*ret = i;
	ফেরত 0;
}

/*
 * Stupid algorithm --- we now just search forward starting from the
 * goal.  Should put in a smarter one someday....
 */
errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
			    ext2fs_block_bitmap map, blk64_t *ret,
			    কাঠামো blk_alloc_ctx *ctx)
{
	errcode_t retval;
	blk64_t	b = 0;
	errcode_t (*gab)(ext2_filsys fs, blk64_t goal, blk64_t *ret);
	errcode_t (*gab2)(ext2_filsys, blk64_t, blk64_t *,
			  কাঠামো blk_alloc_ctx *);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!map) {
		/*
		 * In case there are clients out there whose get_alloc_block
		 * handlers call ext2fs_new_block2 with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		যদি (fs->get_alloc_block2) {
			gab2 = fs->get_alloc_block2;
			fs->get_alloc_block2 = ফাঁকা;
			retval = gab2(fs, goal, &b, ctx);
			fs->get_alloc_block2 = gab2;
			যাও allocated;
		} অন্যথা যদি (fs->get_alloc_block) {
			gab = fs->get_alloc_block;
			fs->get_alloc_block = ফাঁকা;
			retval = gab(fs, goal, &b);
			fs->get_alloc_block = gab;
			যাও allocated;
		}
	}
	যদি (!map)
		map = fs->block_map;
	যদি (!map)
		ফেরত EXT2_ET_NO_BLOCK_BITMAP;
	যদি (!goal || (goal >= ext2fs_blocks_গুনতি(fs->super)))
		goal = fs->super->s_first_তথ্য_block;
	goal &= ~EXT2FS_CLUSTER_MASK(fs);

	retval = ext2fs_find_first_zero_block_bitmap2(map,
			goal, ext2fs_blocks_গুনতি(fs->super) - 1, &b);
	যদি ((retval == ENOENT) && (goal != fs->super->s_first_তথ্য_block))
		retval = ext2fs_find_first_zero_block_bitmap2(map,
			fs->super->s_first_তথ্য_block, goal - 1, &b);
allocated:
	যদি (retval == ENOENT)
		ফেরত EXT2_ET_BLOCK_ALLOC_FAIL;
	যদি (retval)
		ফেরত retval;

	ext2fs_clear_block_uninit(fs, ext2fs_group_of_blk2(fs, b));
	*ret = b;
	ফেরত 0;
}

errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
			   ext2fs_block_bitmap map, blk64_t *ret)
{
	ফেরত ext2fs_new_block3(fs, goal, map, ret, ফাঁকা);
}

errcode_t ext2fs_new_block(ext2_filsys fs, blk_t goal,
			   ext2fs_block_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_new_block2(fs, goal, map, &val);
	যদি (!retval)
		*ret = (blk_t) val;
	ফেরত retval;
}

/*
 * This function zeros out the allocated block, and updates all of the
 * appropriate filesystem records.
 */
errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal, অক্ষর *block_buf,
			      blk64_t *ret, কাঠামো blk_alloc_ctx *ctx)
{
	errcode_t	retval;
	blk64_t		block;

	যদি (fs->get_alloc_block2) {
		retval = (fs->get_alloc_block2)(fs, goal, &block, ctx);
		যদি (retval)
			যাও fail;
	} অন্যথা যদি (fs->get_alloc_block) {
		retval = (fs->get_alloc_block)(fs, goal, &block);
		যদি (retval)
			যাও fail;
	} অন্যথা {
		যদি (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			যদি (retval)
				যাও fail;
		}

		retval = ext2fs_new_block3(fs, goal, 0, &block, ctx);
		যদি (retval)
			যাও fail;
	}

	যদি (block_buf) {
		memset(block_buf, 0, fs->blocksize);
		retval = io_channel_write_blk64(fs->io, block, 1, block_buf);
	} অন্যথা
		retval = ext2fs_zero_blocks2(fs, block, 1, ফাঁকা, ফাঁকা);
	যদি (retval)
		যাও fail;

	ext2fs_block_alloc_stats2(fs, block, +1);
	*ret = block;

fail:
	ফেরত retval;
}

errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
			     অক্ষর *block_buf, blk64_t *ret)
{
	ফেরত ext2fs_alloc_block3(fs, goal, block_buf, ret, ফাঁকা);
}

errcode_t ext2fs_alloc_block(ext2_filsys fs, blk_t goal,
			     অক্ষর *block_buf, blk_t *ret)
{
	errcode_t retval;
	blk64_t ret64, goal64 = goal;
	retval = ext2fs_alloc_block3(fs, goal64, block_buf, &ret64, ফাঁকা);
	যদি (!retval)
		*ret = (blk_t)ret64;
        ফেরত retval;
}

errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start, blk64_t finish,
				 পূর্ণসংখ্যা অঙ্ক, ext2fs_block_bitmap map, blk64_t *ret)
{
	blk64_t	b = start;
	পূর্ণসংখ্যা	c_ratio;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!map)
		map = fs->block_map;
	যদি (!map)
		ফেরত EXT2_ET_NO_BLOCK_BITMAP;
	যদি (!b)
		b = fs->super->s_first_তথ্য_block;
	যদি (!finish)
		finish = start;
	যদি (!অঙ্ক)
		অঙ্ক = 1;
	c_ratio = 1 << ext2fs_get_bitmap_granularity(map);
	b &= ~(c_ratio - 1);
	finish &= ~(c_ratio -1);
	করো {
		যদি (b + অঙ্ক - 1 >= ext2fs_blocks_গুনতি(fs->super)) {
			যদি (finish > start)
				ফেরত EXT2_ET_BLOCK_ALLOC_FAIL;
			b = fs->super->s_first_তথ্য_block;
		}
		যদি (ext2fs_fast_test_block_bitmap_range2(map, b, অঙ্ক)) {
			*ret = b;
			ফেরত 0;
		}
		b += c_ratio;
	} যখন (b != finish);
	ফেরত EXT2_ET_BLOCK_ALLOC_FAIL;
}

errcode_t ext2fs_get_free_blocks(ext2_filsys fs, blk_t start, blk_t finish,
				 পূর্ণসংখ্যা অঙ্ক, ext2fs_block_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_get_free_blocks2(fs, start, finish, অঙ্ক, map, &val);
	যদি(!retval)
		*ret = (blk_t) val;
	ফেরত retval;
}

শূন্য ext2fs_set_alloc_block_callback(ext2_filsys fs,
				     errcode_t (*func)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret),
				     errcode_t (**old)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret))
{
	যদি (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ফেরত;

	যদি (old)
		*old = fs->get_alloc_block;

	fs->get_alloc_block = func;
}

blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       কাঠামো ext2_inode *inode, blk64_t lblk)
{
	dgrp_t			group;
	__u8			log_flex;
	কাঠামো ext2fs_extent	extent;
	ext2_extent_handle_t	handle = ফাঁকা;
	errcode_t		err;

	/* Make sure data stored in inode->i_block is neither fast symlink nor
	 * inline data.
	 */
	যদি (inode == ফাঁকা || ext2fs_is_fast_symlink(inode) ||
	    inode->i_flags & EXT4_INLINE_তথ্য_FL)
		যাও no_blocks;

	যদি (inode->i_flags & EXT4_EXTENTS_FL) {
		err = ext2fs_extent_open2(fs, ino, inode, &handle);
		যদি (err)
			যাও no_blocks;
		err = ext2fs_extent_goto2(handle, 0, lblk);
		যদি (err)
			যাও no_blocks;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
		যদি (err)
			যাও no_blocks;
		ext2fs_extent_free(handle);
		ফেরত extent.e_pblk + (lblk - extent.e_lblk);
	}

	/* block mapped file; see if block zero is mapped? */
	যদি (inode->i_block[0])
		ফেরত inode->i_block[0];

no_blocks:
	ext2fs_extent_free(handle);
	log_flex = fs->super->s_log_groups_per_flex;
	group = ext2fs_group_of_ino(fs, ino);
	যদি (log_flex)
		group = group & ~((1 << (log_flex)) - 1);
	ফেরত ext2fs_group_first_block2(fs, group);
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
errcode_t ext2fs_new_range(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			   blk64_t len, ext2fs_block_bitmap map, blk64_t *pblk,
			   blk64_t *plen)
{
	errcode_t retval;
	blk64_t start, end, b;
	পূর্ণসংখ্যা looped = 0;
	blk64_t max_blocks = ext2fs_blocks_গুনতি(fs->super);
	errcode_t (*nrf)(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			 blk64_t len, blk64_t *pblk, blk64_t *plen);

	dbg_printf("%s: flags=0x%x goal=%llu len=%llu\n", __func__, flags,
		   goal, len);
	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	যদি (len == 0 || (flags & ~EXT2_NEWRANGE_ALL_FLAGS))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	যদি (!map && fs->new_range) {
		/*
		 * In case there are clients out there whose new_range
		 * handlers call ext2fs_new_range with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		nrf = fs->new_range;
		fs->new_range = ফাঁকা;
		retval = nrf(fs, flags, goal, len, pblk, plen);
		fs->new_range = nrf;
		যদি (retval)
			ফেরত retval;
		start = *pblk;
		end = *pblk + *plen;
		যাও allocated;
	}
	যদি (!map)
		map = fs->block_map;
	যদি (!map)
		ফেরত EXT2_ET_NO_BLOCK_BITMAP;
	যদি (!goal || goal >= ext2fs_blocks_গুনতি(fs->super))
		goal = fs->super->s_first_তথ্য_block;

	start = goal;
	যখন (!looped || start <= goal) {
		retval = ext2fs_find_first_zero_block_bitmap2(map, start,
							      max_blocks - 1,
							      &start);
		যদি (retval == ENOENT) {
			/*
			 * If there are no free blocks beyond the starting
			 * point, try scanning the whole filesystem, unless the
			 * user told us only to allocate from _goal_, or if
			 * we're already scanning the whole filesystem.
			 */
			যদি (flags & EXT2_NEWRANGE_FIXED_GOAL ||
			    start == fs->super->s_first_তথ্য_block)
				যাও fail;
			start = fs->super->s_first_তথ্য_block;
			চালু;
		} অন্যথা যদি (retval)
			যাও errout;

		যদি (flags & EXT2_NEWRANGE_FIXED_GOAL && start != goal)
			যাও fail;

		b = min(start + len - 1, max_blocks - 1);
		retval =  ext2fs_find_first_set_block_bitmap2(map, start, b,
							      &end);
		যদি (retval == ENOENT)
			end = b + 1;
		অন্যথা যদি (retval)
			যাও errout;

		যদি (!(flags & EXT2_NEWRANGE_MIN_দৈর্ঘ্য) ||
		    (end - start) >= len) {
			/* Success! */
			*pblk = start;
			*plen = end - start;
			dbg_printf("%s: new_range goal=%llu--%llu "
				   "blk=%llu--%llu %llu\n",
				   __func__, goal, goal + len - 1,
				   *pblk, *pblk + *plen - 1, *plen);
allocated:
			জন্য (b = start; b < end;
			     b += fs->super->s_blocks_per_group)
				ext2fs_clear_block_uninit(fs,
						ext2fs_group_of_blk2(fs, b));
			ফেরত 0;
		}

		যদি (flags & EXT2_NEWRANGE_FIXED_GOAL)
			যাও fail;
		start = end;
		যদি (start >= max_blocks) {
			যদি (looped)
				যাও fail;
			looped = 1;
			start = fs->super->s_first_তথ্য_block;
		}
	}

fail:
	retval = EXT2_ET_BLOCK_ALLOC_FAIL;
errout:
	ফেরত retval;
}

শূন্য ext2fs_set_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen))
{
	যদি (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ফেরত;

	যদি (old)
		*old = fs->new_range;

	fs->new_range = func;
}

errcode_t ext2fs_alloc_range(ext2_filsys fs, পূর্ণসংখ্যা flags, blk64_t goal,
			     blk_t len, blk64_t *ret)
{
	পূর্ণসংখ্যা newr_flags = EXT2_NEWRANGE_MIN_দৈর্ঘ্য;
	errcode_t retval;
	blk64_t plen;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	যদি (len == 0 || (flags & ~EXT2_ALLOCRANGE_ALL_FLAGS))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	যদি (flags & EXT2_ALLOCRANGE_FIXED_GOAL)
		newr_flags |= EXT2_NEWRANGE_FIXED_GOAL;

	retval = ext2fs_new_range(fs, newr_flags, goal, len, ফাঁকা, ret, &plen);
	যদি (retval)
		ফেরত retval;

	যদি (plen < len)
		ফেরত EXT2_ET_BLOCK_ALLOC_FAIL;

	যদি (flags & EXT2_ALLOCRANGE_ZERO_BLOCKS) {
		retval = ext2fs_zero_blocks2(fs, *ret, len, ফাঁকা, ফাঁকা);
		যদি (retval)
			ফেরত retval;
	}

	ext2fs_block_alloc_stats_range(fs, *ret, len, +1);
	ফেরত retval;
}
