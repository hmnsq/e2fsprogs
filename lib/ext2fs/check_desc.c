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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_block_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t first_block = fs->super->s_first_data_block;
	blk64_t last_block = ext2fs_blocks_شمار(fs->super)-1;
	blk64_t blk, b;
	unsigned صحیح j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (EXT2_DESC_SIZE(fs->super) & (EXT2_DESC_SIZE(fs->super) - 1))
		بازگشت EXT2_ET_BAD_DESC_SIZE;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	اگر (retval)
		بازگشت retval;

	برای (i = 0; i < fs->group_desc_شمار; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	برای (i = 0; i < fs->group_desc_شمار; i++) {
		اگر (!ext2fs_has_feature_flex_bg(fs->super)) {
			first_block = ext2fs_group_first_block2(fs, i);
			last_block = ext2fs_group_last_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		اگر (blk < first_block || blk > last_block ||
		    ext2fs_test_block_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_BLOCK_MAP;
			برو errout;
		}
		ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		اگر (blk < first_block || blk > last_block ||
		    ext2fs_test_block_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			برو errout;
		}
		ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		اگر (blk < first_block ||
		    ((blk + fs->inode_blocks_per_group - 1) > last_block)) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			برو errout;
		}
		برای (j = 0, b = blk; j < fs->inode_blocks_per_group;
		     j++, b++) {
			اگر (ext2fs_test_block_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
				برو errout;
			}
			ext2fs_mark_block_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_block_bitmap(bmap);
	بازگشت retval;
}
