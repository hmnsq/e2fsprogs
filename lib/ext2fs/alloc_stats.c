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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

வெறுமை ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       முழு inuse, முழு isdir)
{
	முழு	group = ext2fs_group_of_ino(fs, ino);

	எனில் (ino > fs->super->s_inodes_மொத்தம்) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (unsigned long) ino);
#endif
		திரும்பு;
	}
	எனில் (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	இல்லையெனில்
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_மொத்தம்_set(fs, group, ext2fs_bg_free_inodes_மொத்தம்(fs, group) - inuse);
	எனில் (isdir)
		ext2fs_bg_used_dirs_மொத்தம்_set(fs, group, ext2fs_bg_used_dirs_மொத்தம்(fs, group) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
	எனில் (ext2fs_has_group_desc_csum(fs)) {
		ext2_ino_t first_unused_inode =	fs->super->s_inodes_per_group -
			ext2fs_bg_itable_unused(fs, group) +
			group * fs->super->s_inodes_per_group + 1;

		எனில் (ino >= first_unused_inode)
			ext2fs_bg_itable_unused_set(fs, group, group * fs->super->s_inodes_per_group + fs->super->s_inodes_per_group - ino);
		ext2fs_group_desc_csum_set(fs, group);
	}

	fs->super->s_free_inodes_மொத்தம் -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

வெறுமை ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, முழு inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

வெறுமை ext2fs_block_alloc_stats2(ext2_filsys fs, blk64_t blk, முழு inuse)
{
	முழு	group = ext2fs_group_of_blk2(fs, blk);

	எனில் (blk < fs->super->s_first_data_block ||
	    blk >= ext2fs_blocks_மொத்தம்(fs->super)) {
#ifndef OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (unsigned long) blk);
#endif
		திரும்பு;
	}
	எனில் (inuse > 0)
		ext2fs_mark_block_bitmap2(fs->block_map, blk);
	இல்லையெனில்
		ext2fs_unmark_block_bitmap2(fs->block_map, blk);
	ext2fs_bg_free_blocks_மொத்தம்_set(fs, group, ext2fs_bg_free_blocks_மொத்தம்(fs, group) - inuse);
	ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
	ext2fs_group_desc_csum_set(fs, group);

	ext2fs_free_blocks_மொத்தம்_add(fs->super,
			     -inuse * (blk64_t) EXT2FS_CLUSTER_RATIO(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	எனில் (fs->block_alloc_stats)
		(fs->block_alloc_stats)(fs, (blk64_t) blk, inuse);
}

வெறுமை ext2fs_block_alloc_stats(ext2_filsys fs, blk_t blk, முழு inuse)
{
	ext2fs_block_alloc_stats2(fs, blk, inuse);
}

வெறுமை ext2fs_set_block_alloc_stats_callback(ext2_filsys fs,
					   வெறுமை (*func)(ext2_filsys fs,
							blk64_t blk,
							முழு inuse),
					   வெறுமை (**old)(ext2_filsys fs,
							blk64_t blk,
							முழு inuse))
{
	எனில் (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		திரும்பு;
	எனில் (old)
		*old = fs->block_alloc_stats;

	fs->block_alloc_stats = func;
}

வெறுமை ext2fs_block_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t இலக்கம், முழு inuse)
{
	மாறிலி blk64_t orig_blk = blk;
	மாறிலி blk_t orig_இலக்கம் = இலக்கம்;

#ifndef OMIT_COM_ERR
	எனில் (blk + இலக்கம் > ext2fs_blocks_மொத்தம்(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(unsigned long long) blk, இலக்கம்);
		திரும்பு;
	}
#endif
	எனில் (inuse == 0)
		திரும்பு;
	எனில் (inuse > 0) {
		ext2fs_mark_block_bitmap_range2(fs->block_map, blk, இலக்கம்);
		inuse = 1;
	} இல்லையெனில் {
		ext2fs_unmark_block_bitmap_range2(fs->block_map, blk, இலக்கம்);
		inuse = -1;
	}
	போது (இலக்கம்) {
		முழு group = ext2fs_group_of_blk2(fs, blk);
		blk64_t last_blk = ext2fs_group_last_block2(fs, group);
		blk64_t n = இலக்கம்;

		எனில் (blk + இலக்கம் > last_blk)
			n = last_blk - blk + 1;

		ext2fs_bg_free_blocks_மொத்தம்_set(fs, group,
			ext2fs_bg_free_blocks_மொத்தம்(fs, group) -
			inuse*n/EXT2FS_CLUSTER_RATIO(fs));
		ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
		ext2fs_group_desc_csum_set(fs, group);
		ext2fs_free_blocks_மொத்தம்_add(fs->super, -inuse * (blk64_t) n);
		blk += n;
		இலக்கம் -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	எனில் (fs->block_alloc_stats_range)
		(fs->block_alloc_stats_range)(fs, orig_blk, orig_இலக்கம், inuse);
}

வெறுமை ext2fs_set_block_alloc_stats_range_callback(ext2_filsys fs,
	வெறுமை (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t இலக்கம், முழு inuse),
	வெறுமை (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t இலக்கம், முழு inuse))
{
	எனில் (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		திரும்பு;
	எனில் (old)
		*old = fs->block_alloc_stats_range;

	fs->block_alloc_stats_range = func;
}
