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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

/*
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_ఖండం_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t మొదటి_ఖండం = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
	blk64_t చివరి_ఖండం = ext2fs_ఖండాలు_లెక్క(fs->super)-1;
	blk64_t blk, b;
	గుర్తులేని పూర్ణాంకం j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (EXT2_DESC_కొలత(fs->super) & (EXT2_DESC_కొలత(fs->super) - 1))
		తిరిగి EXT2_ET_BAD_DESC_కొలత;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	ఒకవేళ (retval)
		తిరిగి retval;

	కొరకు (i = 0; i < fs->సమూహం_desc_లెక్క; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	కొరకు (i = 0; i < fs->సమూహం_desc_లెక్క; i++) {
		ఒకవేళ (!ext2fs_has_feature_flex_bg(fs->super)) {
			మొదటి_ఖండం = ext2fs_సమూహం_మొదటి_block2(fs, i);
			చివరి_ఖండం = ext2fs_సమూహం_చివరి_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_ఖండం_bitmap_loc(fs, i);
		ఒకవేళ (blk < మొదటి_ఖండం || blk > చివరి_ఖండం ||
		    ext2fs_test_ఖండం_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_ఖండం_MAP;
			వెళ్ళు errout;
		}
		ext2fs_mark_ఖండం_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ఒకవేళ (blk < మొదటి_ఖండం || blk > చివరి_ఖండం ||
		    ext2fs_test_ఖండం_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			వెళ్ళు errout;
		}
		ext2fs_mark_ఖండం_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_పట్టిక_loc(fs, i);
		ఒకవేళ (blk < మొదటి_ఖండం ||
		    ((blk + fs->inode_ఖండాలు_చొప్పున_సమూహం - 1) > చివరి_ఖండం)) {
			retval = EXT2_ET_GDESC_BAD_INODE_పట్టిక;
			వెళ్ళు errout;
		}
		కొరకు (j = 0, b = blk; j < fs->inode_ఖండాలు_చొప్పున_సమూహం;
		     j++, b++) {
			ఒకవేళ (ext2fs_test_ఖండం_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_పట్టిక;
				వెళ్ళు errout;
			}
			ext2fs_mark_ఖండం_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_ఖండం_bitmap(bmap);
	తిరిగి retval;
}
