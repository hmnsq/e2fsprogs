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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

ശൂന്യം ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       പൂർണ്ണസംഖ്യ inuse, പൂർണ്ണസംഖ്യ isdir)
{
	പൂർണ്ണസംഖ്യ	group = ext2fs_group_of_ino(fs, ino);

	എങ്കിൽ (ino > fs->super->s_inodes_എണ്ണം) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (unsigned long) ino);
#endif
		തിരികെ;
	}
	എങ്കിൽ (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	അല്ലെങ്കിൽ
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_എണ്ണം_set(fs, group, ext2fs_bg_free_inodes_എണ്ണം(fs, group) - inuse);
	എങ്കിൽ (isdir)
		ext2fs_bg_used_dirs_എണ്ണം_set(fs, group, ext2fs_bg_used_dirs_എണ്ണം(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	എങ്കിൽ (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		എങ്കിൽ (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_എണ്ണം -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

ശൂന്യം ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, പൂർണ്ണസംഖ്യ inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

ശൂന്യം ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, പൂർണ്ണസംഖ്യ inuse)
{
	പൂർണ്ണസംഖ്യ	group = ext2fs_group_of_blk2(fs, blk);

	എങ്കിൽ (blk < fs->super->s_first_data_block ||
	    blk >= ext2fs_blocks_എണ്ണം(fs->super)) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (unsigned long) blk);
#endif
		തിരികെ;
	}
	എങ്കിൽ (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	അല്ലെങ്കിൽ
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_എണ്ണം_set(fs, group, ext2fs_bg_free_blocks_എണ്ണം(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_എണ്ണം_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	എങ്കിൽ (fs->block_alloc_stats)
		(fs->block_alloc_stats)(fs, (blk64_t) blk, inuse);
}

ശൂന്യം ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, പൂർണ്ണസംഖ്യ inuse)
{
	ext2fs_block_alloc_stats2(fs, blk, inuse);
}

ശൂന്യം ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
					   ശൂന്യം (*func)(ext2_filsys fs,
							blk64_t blk,
							പൂർണ്ണസംഖ്യ inuse),
					   ശൂന്യം (**old)(ext2_filsys fs,
							blk64_t blk,
							പൂർണ്ണസംഖ്യ inuse))
{
	എങ്കിൽ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		തിരികെ;
	എങ്കിൽ (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

ശൂന്യം ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t അക്കം, പൂർണ്ണസംഖ്യ inuse)
{
	സ്ഥിരം blk64_t orig_blk = blk;
	സ്ഥിരം blk_t orig_അക്കം = അക്കം;

#ifndef OMIT_COM_ERR
	എങ്കിൽ (blk + അക്കം > ext2fs_blocks_എണ്ണം(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(unsigned long long) blk, അക്കം);
		തിരികെ;
	}
#endif
	എങ്കിൽ (inuse == 0)
		തിരികെ;
	എങ്കിൽ (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, അക്കം);
		inuse = 1;
	} അല്ലെങ്കിൽ {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, അക്കം);
		inuse = -1;
	}
	ആകുമ്പോൾ (അക്കം) {
		പൂർണ്ണസംഖ്യ group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = അക്കം;

		എങ്കിൽ (blk + അക്കം > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_എണ്ണം_set(fs, group,
			ext2fs_bg_free_blocks_എണ്ണം(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_എണ്ണം_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		അക്കം -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	എങ്കിൽ (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_അക്കം, inuse);
}

ശൂന്യം ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	ശൂന്യം (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t അക്കം, പൂർണ്ണസംഖ്യ inuse),
	ശൂന്യം (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t അക്കം, പൂർണ്ണസംഖ്യ inuse))
{
	എങ്കിൽ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		തിരികെ;
	എങ്കിൽ (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
