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
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

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
	blk64_t لومړی_بلاک = fs->super->s_لومړی_معلومات_بلاک;
	blk64_t وروستی_بلاک = ext2fs_بلاکونه_شمېر(fs->super)-1;
	blk64_t blk, b;
	بینښه صحيح j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (EXT2_DESC_اندازه(fs->super) & (EXT2_DESC_اندازه(fs->super) - 1))
		أرجع EXT2_ET_BAD_DESC_اندازه;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	إذا (retval)
		أرجع retval;

	لكل (i = 0; i < fs->ډله_desc_شمېر; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	لكل (i = 0; i < fs->ډله_desc_شمېر; i++) {
		إذا (!ext2fs_has_feature_flex_bg(fs->super)) {
			لومړی_بلاک = ext2fs_ډله_لومړی_block2(fs, i);
			وروستی_بلاک = ext2fs_ډله_وروستی_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_بلاک_bitmap_loc(fs, i);
		إذا (blk < لومړی_بلاک || blk > وروستی_بلاک ||
		    ext2fs_test_بلاک_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_بلاک_MAP;
			اذهب errout;
		}
		ext2fs_mark_بلاک_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk < لومړی_بلاک || blk > وروستی_بلاک ||
		    ext2fs_test_بلاک_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			اذهب errout;
		}
		ext2fs_mark_بلاک_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		إذا (blk < لومړی_بلاک ||
		    ((blk + fs->inode_بلاکونه_فی_ډله - 1) > وروستی_بلاک)) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			اذهب errout;
		}
		لكل (j = 0, b = blk; j < fs->inode_بلاکونه_فی_ډله;
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
