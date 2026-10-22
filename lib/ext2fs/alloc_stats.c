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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

શૂન્ય ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       પૂર્ણાંક inuse, પૂર્ણાંક isdir)
{
	પૂર્ણાંક	group = ext2fs_group_of_ino(fs, ino);

	જો (ino > fs->super->s_inodes_ગણતરી) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (unsigned long) ino);
#endif
		પાછા;
	}
	જો (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	નહીંતર
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_ગણતરી_set(fs, group, ext2fs_bg_free_inodes_ગણતરી(fs, group) - inuse);
	જો (isdir)
		ext2fs_bg_used_dirs_ગણતરી_set(fs, group, ext2fs_bg_used_dirs_ગણતરી(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	જો (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		જો (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_ગણતરી -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

શૂન્ય ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, પૂર્ણાંક inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

શૂન્ય ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, પૂર્ણાંક inuse)
{
	પૂર્ણાંક	group = ext2fs_group_of_blk2(fs, blk);

	જો (blk < fs->super->s_first_data_block ||
	    blk >= ext2fs_blocks_ગણતરી(fs->super)) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (unsigned long) blk);
#endif
		પાછા;
	}
	જો (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	નહીંતર
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_ગણતરી_set(fs, group, ext2fs_bg_free_blocks_ગણતરી(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_ગણતરી_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	જો (fs->block_alloc_stats)
		(fs->block_alloc_stats)(fs, (blk64_t) blk, inuse);
}

શૂન્ય ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, પૂર્ણાંક inuse)
{
	ext2fs_block_alloc_stats2(fs, blk, inuse);
}

શૂન્ય ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
					   શૂન્ય (*func)(ext2_filsys fs,
							blk64_t blk,
							પૂર્ણાંક inuse),
					   શૂન્ય (**old)(ext2_filsys fs,
							blk64_t blk,
							પૂર્ણાંક inuse))
{
	જો (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		પાછા;
	જો (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

શૂન્ય ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t અંક, પૂર્ણાંક inuse)
{
	અચલ blk64_t orig_blk = blk;
	અચલ blk_t orig_અંક = અંક;

#ifndef OMIT_COM_ERR
	જો (blk + અંક > ext2fs_blocks_ગણતરી(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(unsigned long long) blk, અંક);
		પાછા;
	}
#endif
	જો (inuse == 0)
		પાછા;
	જો (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, અંક);
		inuse = 1;
	} નહીંતર {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, અંક);
		inuse = -1;
	}
	જ્યારે (અંક) {
		પૂર્ણાંક group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = અંક;

		જો (blk + અંક > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_ગણતરી_set(fs, group,
			ext2fs_bg_free_blocks_ગણતરી(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_ગણતરી_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		અંક -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	જો (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_અંક, inuse);
}

શૂન્ય ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	શૂન્ય (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t અંક, પૂર્ણાંક inuse),
	શૂન્ય (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t અંક, પૂર્ણાંક inuse))
{
	જો (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		પાછા;
	જો (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
