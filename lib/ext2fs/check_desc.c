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
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_بلاک_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t پہلا_بلاک = fs->super->s_پہلا_ڈیٹا_بلاک;
	blk64_t آخری_بلاک = ext2fs_بلاکیں_گنتی(fs->super)-1;
	blk64_t blk, b;
	غیرعلامتی صحیح j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (EXT2_DESC_سائز(fs->super) & (EXT2_DESC_سائز(fs->super) - 1))
		واپس EXT2_ET_BAD_DESC_سائز;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	اگر (retval)
		واپس retval;

	برائے (i = 0; i < fs->گروہ_desc_گنتی; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	برائے (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		اگر (!ext2fs_has_feature_flex_bg(fs->super)) {
			پہلا_بلاک = ext2fs_گروہ_پہلا_block2(fs, i);
			آخری_بلاک = ext2fs_گروہ_آخری_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_بلاک_bitmap_loc(fs, i);
		اگر (blk < پہلا_بلاک || blk > آخری_بلاک ||
		    ext2fs_test_بلاک_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_بلاک_MAP;
			جاؤ errout;
		}
		ext2fs_mark_بلاک_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		اگر (blk < پہلا_بلاک || blk > آخری_بلاک ||
		    ext2fs_test_بلاک_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			جاؤ errout;
		}
		ext2fs_mark_بلاک_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		اگر (blk < پہلا_بلاک ||
		    ((blk + fs->inode_بلاکیں_فی_گروہ - 1) > آخری_بلاک)) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			جاؤ errout;
		}
		برائے (j = 0, b = blk; j < fs->inode_بلاکیں_فی_گروہ;
		     j++, b++) {
			اگر (ext2fs_test_بلاک_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_جدول;
				جاؤ errout;
			}
			ext2fs_mark_بلاک_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_بلاک_bitmap(bmap);
	واپس retval;
}
