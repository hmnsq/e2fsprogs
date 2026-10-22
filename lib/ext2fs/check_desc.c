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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_بلاک_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t پہلا_بلاک = fs->super->s_پہلا_انکڑے_بلاک;
	blk64_t آخری_بلاک = ext2fs_بلاکاں_گنتی(fs->super)-1;
	blk64_t blk, b;
	بنانشانی صحيح j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (EXT2_DESC_ناپ(fs->super) & (EXT2_DESC_ناپ(fs->super) - 1))
		أرجع EXT2_ET_BAD_DESC_ناپ;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	إذا (retval)
		أرجع retval;

	لكل (i = 0; i < fs->گروہ_desc_گنتی; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	لكل (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		إذا (!ext2fs_has_feature_flex_bg(fs->super)) {
			پہلا_بلاک = ext2fs_گروہ_پہلا_block2(fs, i);
			آخری_بلاک = ext2fs_گروہ_آخری_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_بلاک_bitmap_loc(fs, i);
		إذا (blk < پہلا_بلاک || blk > آخری_بلاک ||
		    ext2fs_test_بلاک_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_بلاک_MAP;
			اذهب errout;
		}
		ext2fs_mark_بلاک_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk < پہلا_بلاک || blk > آخری_بلاک ||
		    ext2fs_test_بلاک_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			اذهب errout;
		}
		ext2fs_mark_بلاک_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		إذا (blk < پہلا_بلاک ||
		    ((blk + fs->inode_بلاکاں_فی_گروہ - 1) > آخری_بلاک)) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			اذهب errout;
		}
		لكل (j = 0, b = blk; j < fs->inode_بلاکاں_فی_گروہ;
		     j++, b++) {
			إذا (ext2fs_test_بلاک_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_جدول;
				اذهب errout;
			}
			ext2fs_mark_بلاک_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_بلاک_bitmap(bmap);
	أرجع retval;
}
