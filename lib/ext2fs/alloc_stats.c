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

	إذا (ino > fs->super->s_inodes_تعداد) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (unsigned long) ino);
#endif
		إرجاع;
	}
	إذا (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	وإلا
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_تعداد_set(fs, group, ext2fs_bg_free_inodes_تعداد(fs, group) - inuse);
	إذا (isdir)
		ext2fs_bg_used_dirs_تعداد_set(fs, group, ext2fs_bg_used_dirs_تعداد(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	إذا (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		إذا (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_تعداد -= inuse;
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

	إذا (blk < fs->super->s_first_data_block ||
	    blk >= ext2fs_blocks_تعداد(fs->super)) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (unsigned long) blk);
#endif
		إرجاع;
	}
	إذا (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	وإلا
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_تعداد_set(fs, group, ext2fs_bg_free_blocks_تعداد(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_تعداد_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->block_alloc_stats)
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
	إذا (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		إرجاع;
	إذا (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

فراغ ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t رقم, صحيح inuse)
{
	ثابت blk64_t orig_blk = blk;
	ثابت blk_t orig_رقم = رقم;

#ifndef OMIT_COM_ERR
	إذا (blk + رقم > ext2fs_blocks_تعداد(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(unsigned long long) blk, رقم);
		إرجاع;
	}
#endif
	إذا (inuse == 0)
		إرجاع;
	إذا (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, رقم);
		inuse = 1;
	} وإلا {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, رقم);
		inuse = -1;
	}
	بينما (رقم) {
		صحيح group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = رقم;

		إذا (blk + رقم > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_تعداد_set(fs, group,
			ext2fs_bg_free_blocks_تعداد(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_تعداد_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		رقم -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_رقم, inuse);
}

فراغ ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	فراغ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t رقم, صحيح inuse),
	فراغ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t رقم, صحيح inuse))
{
	إذا (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		إرجاع;
	إذا (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
