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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಶೂನ್ಯ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       ಪೂರ್ಣಾಙ್ಕ inuse, ಪೂರ್ಣಾಙ್ಕ isdir)
{
	ಪೂರ್ಣಾಙ್ಕ	group = ext2fs_group_of_ino(fs, ino);

	ಯದಿ (ino > fs->super->s_inodes_ಎಣಿಕೆ) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (unsigned long) ino);
#endif
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	ಅನ್ಯಥಾ
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_ಎಣಿಕೆ_set(fs, group, ext2fs_bg_free_inodes_ಎಣಿಕೆ(fs, group) - inuse);
	ಯದಿ (isdir)
		ext2fs_bg_used_dirs_ಎಣಿಕೆ_set(fs, group, ext2fs_bg_used_dirs_ಎಣಿಕೆ(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	ಯದಿ (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		ಯದಿ (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_ಎಣಿಕೆ -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

ಶೂನ್ಯ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

ಶೂನ್ಯ ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಙ್ಕ inuse)
{
	ಪೂರ್ಣಾಙ್ಕ	group = ext2fs_group_of_blk2(fs, blk);

	ಯದಿ (blk < fs->super->s_first_data_block ||
	    blk >= ext2fs_blocks_ಎಣಿಕೆ(fs->super)) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (unsigned long) blk);
#endif
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	ಅನ್ಯಥಾ
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_ಎಣಿಕೆ_set(fs, group, ext2fs_bg_free_blocks_ಎಣಿಕೆ(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_ಎಣಿಕೆ_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ಯದಿ (fs->block_alloc_stats)
		(fs->block_alloc_stats)(fs, (blk64_t) blk, inuse);
}

ಶೂನ್ಯ ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, ಪೂರ್ಣಾಙ್ಕ inuse)
{
	ext2fs_block_alloc_stats2(fs, blk, inuse);
}

ಶೂನ್ಯ ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
					   ಶೂನ್ಯ (*func)(ext2_filsys fs,
							blk64_t blk,
							ಪೂರ್ಣಾಙ್ಕ inuse),
					   ಶೂನ್ಯ (**old)(ext2_filsys fs,
							blk64_t blk,
							ಪೂರ್ಣಾಙ್ಕ inuse))
{
	ಯದಿ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

ಶೂನ್ಯ ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಙ್ಕ inuse)
{
	ಧ್ರುವ blk64_t orig_blk = blk;
	ಧ್ರುವ blk_t orig_ಅಂಕೆ = ಅಂಕೆ;

#ifndef OMIT_COM_ERR
	ಯದಿ (blk + ಅಂಕೆ > ext2fs_blocks_ಎಣಿಕೆ(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(unsigned long long) blk, ಅಂಕೆ);
		ಪ್ರತ್ಯಾಗಮ;
	}
#endif
	ಯದಿ (inuse == 0)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, ಅಂಕೆ);
		inuse = 1;
	} ಅನ್ಯಥಾ {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, ಅಂಕೆ);
		inuse = -1;
	}
	ಯಾವತ್ (ಅಂಕೆ) {
		ಪೂರ್ಣಾಙ್ಕ group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = ಅಂಕೆ;

		ಯದಿ (blk + ಅಂಕೆ > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_ಎಣಿಕೆ_set(fs, group,
			ext2fs_bg_free_blocks_ಎಣಿಕೆ(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_ಎಣಿಕೆ_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		ಅಂಕೆ -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ಯದಿ (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_ಅಂಕೆ, inuse);
}

ಶೂನ್ಯ ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	ಶೂನ್ಯ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಙ್ಕ inuse),
	ಶೂನ್ಯ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಙ್ಕ inuse))
{
	ಯದಿ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
