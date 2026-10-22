/*
 * check_desc.c --- Check the group descriptors of an ext2 filesystem
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

/*
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_block_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t first_block = fs->super->s_first_data_block;
	blk64_t last_block = ext2fs_blocks_ଗଣନା(fs->super)-1;
	blk64_t blk, b;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (EXT2_DESC_SIZE(fs->super) & (EXT2_DESC_SIZE(fs->super) - 1))
		ଫେରନ୍ତୁ EXT2_ET_BAD_DESC_SIZE;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ପାଇଁ (i = 0; i < fs->group_desc_ଗଣନା; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	ପାଇଁ (i = 0; i < fs->group_desc_ଗଣନା; i++) {
		ଯଦି (!ext2fs_has_feature_flex_bg(fs->super)) {
			first_block = ext2fs_group_first_block2(fs, i);
			last_block = ext2fs_group_last_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		ଯଦି (blk < first_block || blk > last_block ||
		    ext2fs_test_block_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_BLOCK_MAP;
			ଯାଅ errout;
		}
		ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ଯଦି (blk < first_block || blk > last_block ||
		    ext2fs_test_block_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			ଯାଅ errout;
		}
		ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		ଯଦି (blk < first_block ||
		    ((blk + fs->inode_blocks_per_group - 1) > last_block)) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			ଯାଅ errout;
		}
		ପାଇଁ (j = 0, b = blk; j < fs->inode_blocks_per_group;
		     j++, b++) {
			ଯଦି (ext2fs_test_block_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
				ଯାଅ errout;
			}
			ext2fs_mark_block_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_block_bitmap(bmap);
	ଫେରନ୍ତୁ retval;
}
