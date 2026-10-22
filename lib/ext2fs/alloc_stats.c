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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

ଶୂନ୍ୟ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା inuse, ପୂର୍ଣ୍ଣସଂଖ୍ୟା isdir)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	group = ext2fs_group_of_ino(fs, ino);

	ଯଦି (ino > fs->super->s_inodes_ଗଣନା) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (unsigned long) ino);
#endif
		ଫେରନ୍ତୁ;
	}
	ଯଦି (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	ଅନ୍ୟଥା
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_ଗଣନା_set(fs, group, ext2fs_bg_free_inodes_ଗଣନା(fs, group) - inuse);
	ଯଦି (isdir)
		ext2fs_bg_used_dirs_ଗଣନା_set(fs, group, ext2fs_bg_used_dirs_ଗଣନା(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	ଯଦି (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		ଯଦି (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_ଗଣନା -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

ଶୂନ୍ୟ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

ଶୂନ୍ୟ ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inuse)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	group = ext2fs_group_of_blk2(fs, blk);

	ଯଦି (blk < fs->super->s_first_data_block ||
	    blk >= ext2fs_blocks_ଗଣନା(fs->super)) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (unsigned long) blk);
#endif
		ଫେରନ୍ତୁ;
	}
	ଯଦି (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	ଅନ୍ୟଥା
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_ଗଣନା_set(fs, group, ext2fs_bg_free_blocks_ଗଣନା(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_ଗଣନା_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ଯଦି (fs->block_alloc_stats)
		(fs->block_alloc_stats)(fs, (blk64_t) blk, inuse);
}

ଶୂନ୍ୟ ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inuse)
{
	ext2fs_block_alloc_stats2(fs, blk, inuse);
}

ଶୂନ୍ୟ ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
					   ଶୂନ୍ୟ (*func)(ext2_filsys fs,
							blk64_t blk,
							ପୂର୍ଣ୍ଣସଂଖ୍ୟା inuse),
					   ଶୂନ୍ୟ (**old)(ext2_filsys fs,
							blk64_t blk,
							ପୂର୍ଣ୍ଣସଂଖ୍ୟା inuse))
{
	ଯଦି (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ଫେରନ୍ତୁ;
	ଯଦି (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

ଶୂନ୍ୟ ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t ଅଙ୍କ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inuse)
{
	ଅଚଳ blk64_t orig_blk = blk;
	ଅଚଳ blk_t orig_ଅଙ୍କ = ଅଙ୍କ;

#ifndef OMIT_COM_ERR
	ଯଦି (blk + ଅଙ୍କ > ext2fs_blocks_ଗଣନା(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(unsigned long long) blk, ଅଙ୍କ);
		ଫେରନ୍ତୁ;
	}
#endif
	ଯଦି (inuse == 0)
		ଫେରନ୍ତୁ;
	ଯଦି (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, ଅଙ୍କ);
		inuse = 1;
	} ଅନ୍ୟଥା {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, ଅଙ୍କ);
		inuse = -1;
	}
	ଯେତେବେଳେ (ଅଙ୍କ) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = ଅଙ୍କ;

		ଯଦି (blk + ଅଙ୍କ > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_ଗଣନା_set(fs, group,
			ext2fs_bg_free_blocks_ଗଣନା(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_ଗଣନା_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		ଅଙ୍କ -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ଯଦି (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_ଅଙ୍କ, inuse);
}

ଶୂନ୍ୟ ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	ଶୂନ୍ୟ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t ଅଙ୍କ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inuse),
	ଶୂନ୍ୟ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t ଅଙ୍କ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inuse))
{
	ଯଦି (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ଫେରନ୍ତୁ;
	ଯଦି (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
