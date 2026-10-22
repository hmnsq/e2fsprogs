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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

शून्य ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       पूर्णाङ्क inuse, पूर्णाङ्क isdir)
{
	पूर्णाङ्क	group = ext2fs_group_of_ino(fs, ino);

	यदि (ino > fs->super->s_inodes_मोजणी) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (unsigned long) ino);
#endif
		प्रत्यागम;
	}
	यदि (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	अन्यथा
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_मोजणी_set(fs, group, ext2fs_bg_free_inodes_मोजणी(fs, group) - inuse);
	यदि (isdir)
		ext2fs_bg_used_dirs_मोजणी_set(fs, group, ext2fs_bg_used_dirs_मोजणी(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	यदि (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		यदि (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_मोजणी -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

शून्य ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, पूर्णाङ्क inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

शून्य ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, पूर्णाङ्क inuse)
{
	पूर्णाङ्क	group = ext2fs_group_of_blk2(fs, blk);

	यदि (blk < fs->super->s_first_data_block ||
	    blk >= ext2fs_blocks_मोजणी(fs->super)) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (unsigned long) blk);
#endif
		प्रत्यागम;
	}
	यदि (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	अन्यथा
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_मोजणी_set(fs, group, ext2fs_bg_free_blocks_मोजणी(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_मोजणी_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	यदि (fs->block_alloc_stats)
		(fs->block_alloc_stats)(fs, (blk64_t) blk, inuse);
}

शून्य ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, पूर्णाङ्क inuse)
{
	ext2fs_block_alloc_stats2(fs, blk, inuse);
}

शून्य ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
					   शून्य (*func)(ext2_filsys fs,
							blk64_t blk,
							पूर्णाङ्क inuse),
					   शून्य (**old)(ext2_filsys fs,
							blk64_t blk,
							पूर्णाङ्क inuse))
{
	यदि (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		प्रत्यागम;
	यदि (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

शून्य ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t अंक, पूर्णाङ्क inuse)
{
	ध्रुव blk64_t orig_blk = blk;
	ध्रुव blk_t orig_अंक = अंक;

#ifndef OMIT_COM_ERR
	यदि (blk + अंक > ext2fs_blocks_मोजणी(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(unsigned long long) blk, अंक);
		प्रत्यागम;
	}
#endif
	यदि (inuse == 0)
		प्रत्यागम;
	यदि (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, अंक);
		inuse = 1;
	} अन्यथा {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, अंक);
		inuse = -1;
	}
	यावत् (अंक) {
		पूर्णाङ्क group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = अंक;

		यदि (blk + अंक > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_मोजणी_set(fs, group,
			ext2fs_bg_free_blocks_मोजणी(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_मोजणी_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		अंक -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	यदि (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_अंक, inuse);
}

शून्य ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	शून्य (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t अंक, पूर्णाङ्क inuse),
	शून्य (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t अंक, पूर्णाङ्क inuse))
{
	यदि (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		प्रत्यागम;
	यदि (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
