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
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_بلوک_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t اولی_بلوک = fs->super->s_اولی_داده_بلوک;
	blk64_t آخرین_بلوک = ext2fs_بلوکها_شمار(fs->super)-1;
	blk64_t blk, b;
	بیعلامت صحيح j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (EXT2_DESC_اندازه(fs->super) & (EXT2_DESC_اندازه(fs->super) - 1))
		أرجع EXT2_ET_BAD_DESC_اندازه;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	إذا (retval)
		أرجع retval;

	لكل (i = 0; i < fs->گروه_desc_شمار; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	لكل (i = 0; i < fs->گروه_desc_شمار; i++) {
		إذا (!ext2fs_has_feature_flex_bg(fs->super)) {
			اولی_بلوک = ext2fs_گروه_اولی_block2(fs, i);
			آخرین_بلوک = ext2fs_گروه_آخرین_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_بلوک_bitmap_loc(fs, i);
		إذا (blk < اولی_بلوک || blk > آخرین_بلوک ||
		    ext2fs_test_بلوک_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_بلوک_MAP;
			اذهب errout;
		}
		ext2fs_mark_بلوک_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk < اولی_بلوک || blk > آخرین_بلوک ||
		    ext2fs_test_بلوک_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			اذهب errout;
		}
		ext2fs_mark_بلوک_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		إذا (blk < اولی_بلوک ||
		    ((blk + fs->inode_بلوکها_در_هر_گروه - 1) > آخرین_بلوک)) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			اذهب errout;
		}
		لكل (j = 0, b = blk; j < fs->inode_بلوکها_در_هر_گروه;
		     j++, b++) {
			إذا (ext2fs_test_بلوک_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_جدول;
				اذهب errout;
			}
			ext2fs_mark_بلوک_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_بلوک_bitmap(bmap);
	أرجع retval;
}
