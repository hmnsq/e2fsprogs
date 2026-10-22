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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_بلاڪ_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t پهريون_بلاڪ = fs->super->s_پهريون_معلومات_بلاڪ;
	blk64_t آخري_بلاڪ = ext2fs_بلاڪون_ڳڻپ(fs->super)-1;
	blk64_t blk, b;
	بنانشان صحيح j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (EXT2_DESC_سائيز(fs->super) & (EXT2_DESC_سائيز(fs->super) - 1))
		أرجع EXT2_ET_BAD_DESC_سائيز;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	إذا (retval)
		أرجع retval;

	لكل (i = 0; i < fs->گروهه_desc_ڳڻپ; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	لكل (i = 0; i < fs->گروهه_desc_ڳڻپ; i++) {
		إذا (!ext2fs_has_feature_flex_bg(fs->super)) {
			پهريون_بلاڪ = ext2fs_گروهه_پهريون_block2(fs, i);
			آخري_بلاڪ = ext2fs_گروهه_آخري_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_بلاڪ_bitmap_loc(fs, i);
		إذا (blk < پهريون_بلاڪ || blk > آخري_بلاڪ ||
		    ext2fs_test_بلاڪ_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_بلاڪ_MAP;
			اذهب errout;
		}
		ext2fs_mark_بلاڪ_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk < پهريون_بلاڪ || blk > آخري_بلاڪ ||
		    ext2fs_test_بلاڪ_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			اذهب errout;
		}
		ext2fs_mark_بلاڪ_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		إذا (blk < پهريون_بلاڪ ||
		    ((blk + fs->inode_بلاڪون_في_گروهه - 1) > آخري_بلاڪ)) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			اذهب errout;
		}
		لكل (j = 0, b = blk; j < fs->inode_بلاڪون_في_گروهه;
		     j++, b++) {
			إذا (ext2fs_test_بلاڪ_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_جدول;
				اذهب errout;
			}
			ext2fs_mark_بلاڪ_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_بلاڪ_bitmap(bmap);
	أرجع retval;
}
