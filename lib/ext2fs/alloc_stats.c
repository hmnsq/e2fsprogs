/*
 * alloc_stats.c --- Update allocation statistics for ext2fs
 *
 * Copyright (C) 2001 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

শূন্য ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       পূর্ণাঙ্ক inuse, পূর্ণাঙ্ক isdir)
{
	পূর্ণাঙ্ক	group = ext2fs_group_of_ino(fs, ino);

	যদি (ino > fs->super->s_inodes_গুনতি) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (unsigned long) ino);
#endif
		ঘূৰাই;
	}
	যদি (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	অন্যথা
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_গুনতি_set(fs, group, ext2fs_bg_free_inodes_গুনতি(fs, group) - inuse);
	যদি (isdir)
		ext2fs_bg_used_dirs_গুনতি_set(fs, group, ext2fs_bg_used_dirs_গুনতি(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	যদি (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		যদি (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_গুনতি -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

শূন্য ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, পূর্ণাঙ্ক inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

শূন্য ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, পূর্ণাঙ্ক inuse)
{
	পূর্ণাঙ্ক	group = ext2fs_group_of_blk2(fs, blk);

	যদি (blk < fs->super->s_first_data_block ||
	    blk >= ext2fs_blocks_গুনতি(fs->super)) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (unsigned long) blk);
#endif
		ঘূৰাই;
	}
	যদি (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	অন্যথা
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_গুনতি_set(fs, group, ext2fs_bg_free_blocks_গুনতি(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_গুনতি_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	যদি (fs->block_alloc_stats)
		(fs->block_alloc_stats)(fs, (blk64_t) blk, inuse);
}

শূন্য ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, পূর্ণাঙ্ক inuse)
{
	ext2fs_block_alloc_stats2(fs, blk, inuse);
}

শূন্য ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
					   শূন্য (*func)(ext2_filsys fs,
							blk64_t blk,
							পূর্ণাঙ্ক inuse),
					   শূন্য (**old)(ext2_filsys fs,
							blk64_t blk,
							পূর্ণাঙ্ক inuse))
{
	যদি (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ঘূৰাই;
	যদি (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

শূন্য ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t অঙ্ক, পূর্ণাঙ্ক inuse)
{
	ধ্রুৱ blk64_t orig_blk = blk;
	ধ্রুৱ blk_t orig_অঙ্ক = অঙ্ক;

#ifndef OMIT_COM_ERR
	যদি (blk + অঙ্ক > ext2fs_blocks_গুনতি(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(unsigned long long) blk, অঙ্ক);
		ঘূৰাই;
	}
#endif
	যদি (inuse == 0)
		ঘূৰাই;
	যদি (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, অঙ্ক);
		inuse = 1;
	} অন্যথা {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, অঙ্ক);
		inuse = -1;
	}
	যেতিয়ালৈ (অঙ্ক) {
		পূর্ণাঙ্ক group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = অঙ্ক;

		যদি (blk + অঙ্ক > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_গুনতি_set(fs, group,
			ext2fs_bg_free_blocks_গুনতি(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_গুনতি_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		অঙ্ক -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	যদি (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_অঙ্ক, inuse);
}

শূন্য ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	শূন্য (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t অঙ্ক, পূর্ণাঙ্ক inuse),
	শূন্য (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t অঙ্ক, পূর্ণাঙ্ক inuse))
{
	যদি (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ঘূৰাই;
	যদি (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
