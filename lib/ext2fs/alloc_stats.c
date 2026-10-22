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

#ضمن "config.h"
#ضمن <stdio.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

فراغ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       صحيح inuse, صحيح isdir)
{
	صحيح	group = ext2fs_group_of_ino(fs, ino);

	𐡄𐡍 (ino > fs->super->s_inodes_count) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (unsigned long) ino);
#endif
		أرجع;
	}
	𐡄𐡍 (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	وإلا
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_count_set(fs, group, ext2fs_bg_free_inodes_count(fs, group) - inuse);
	𐡄𐡍 (isdir)
		ext2fs_bg_used_dirs_count_set(fs, group, ext2fs_bg_used_dirs_count(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	𐡄𐡍 (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		𐡄𐡍 (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_count -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

فراغ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, صحيح inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

فراغ ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, صحيح inuse)
{
	صحيح	group = ext2fs_group_of_blk2(fs, blk);

	𐡄𐡍 (blk < fs->super->s_first_data_block ||
	    blk >= ext2fs_blocks_count(fs->super)) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (unsigned long) blk);
#endif
		أرجع;
	}
	𐡄𐡍 (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	وإلا
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_count_set(fs, group, ext2fs_bg_free_blocks_count(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_count_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	𐡄𐡍 (fs->block_alloc_stats)
		(fs->block_alloc_stats)(fs, (blk64_t) blk, inuse);
}

فراغ ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, صحيح inuse)
{
	ext2fs_block_alloc_stats2(fs, blk, inuse);
}

فراغ ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
					   فراغ (*func)(ext2_filsys fs,
							blk64_t blk,
							صحيح inuse),
					   فراغ (**old)(ext2_filsys fs,
							blk64_t blk,
							صحيح inuse))
{
	𐡄𐡍 (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		أرجع;
	𐡄𐡍 (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

فراغ ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t num, صحيح inuse)
{
	ثابت blk64_t orig_blk = blk;
	ثابت blk_t orig_num = num;

#ifndef OMIT_COM_ERR
	𐡄𐡍 (blk + num > ext2fs_blocks_count(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(unsigned long long) blk, num);
		أرجع;
	}
#endif
	𐡄𐡍 (inuse == 0)
		أرجع;
	𐡄𐡍 (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, num);
		inuse = 1;
	} وإلا {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, num);
		inuse = -1;
	}
	طالما (num) {
		صحيح group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = num;

		𐡄𐡍 (blk + num > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_count_set(fs, group,
			ext2fs_bg_free_blocks_count(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_count_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		num -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	𐡄𐡍 (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_num, inuse);
}

فراغ ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	فراغ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t num, صحيح inuse),
	فراغ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t num, صحيح inuse))
{
	𐡄𐡍 (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		أرجع;
	𐡄𐡍 (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
