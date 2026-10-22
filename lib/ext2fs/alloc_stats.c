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

#समावेश "config.h"
#समावेश <stdio.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

व्योम ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       पूर्णांक inuse, पूर्णांक isdir)
{
	पूर्णांक	group = ext2fs_group_of_ino(fs, ino);

	अगर (ino > fs->super->s_inodes_गिनती) {
#अगर_अघोषित OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (अचिन्हित दीर्घ) ino);
#पूर्ण_अगर
		वापस;
	}
	अगर (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	अन्यथा
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_गिनती_set(fs, group, ext2fs_bg_free_inodes_गिनती(fs, group) - inuse);
	अगर (isdir)
		ext2fs_bg_used_dirs_गिनती_set(fs, group, ext2fs_bg_used_dirs_गिनती(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	अगर (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		अगर (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_गिनती -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

व्योम ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, पूर्णांक inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

व्योम ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, पूर्णांक inuse)
{
	पूर्णांक	group = ext2fs_group_of_blk2(fs, blk);

	अगर (blk < fs->super->s_first_तथ्य_block ||
	    blk >= ext2fs_blocks_गिनती(fs->super)) {
#अगर_अघोषित OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (अचिन्हित दीर्घ) blk);
#पूर्ण_अगर
		वापस;
	}
	अगर (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	अन्यथा
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_गिनती_set(fs, group, ext2fs_bg_free_blocks_गिनती(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_गिनती_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	अगर (fs->block_alloc_stats)
		(fs->block_alloc_stats)(fs, (blk64_t) blk, inuse);
}

व्योम ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, पूर्णांक inuse)
{
	ext2fs_block_alloc_stats2(fs, blk, inuse);
}

व्योम ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
					   व्योम (*func)(ext2_filsys fs,
							blk64_t blk,
							पूर्णांक inuse),
					   व्योम (**old)(ext2_filsys fs,
							blk64_t blk,
							पूर्णांक inuse))
{
	अगर (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		वापस;
	अगर (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

व्योम ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t अंक, पूर्णांक inuse)
{
	स्थिर blk64_t orig_blk = blk;
	स्थिर blk_t orig_अंक = अंक;

#अगर_अघोषित OMIT_COM_ERR
	अगर (blk + अंक > ext2fs_blocks_गिनती(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(अचिन्हित दीर्घ दीर्घ) blk, अंक);
		वापस;
	}
#पूर्ण_अगर
	अगर (inuse == 0)
		वापस;
	अगर (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, अंक);
		inuse = 1;
	} अन्यथा {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, अंक);
		inuse = -1;
	}
	जबतक (अंक) {
		पूर्णांक group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = अंक;

		अगर (blk + अंक > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_गिनती_set(fs, group,
			ext2fs_bg_free_blocks_गिनती(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_गिनती_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		अंक -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	अगर (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_अंक, inuse);
}

व्योम ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	व्योम (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t अंक, पूर्णांक inuse),
	व्योम (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t अंक, पूर्णांक inuse))
{
	अगर (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		वापस;
	अगर (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
