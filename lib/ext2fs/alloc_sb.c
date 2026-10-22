/*
 * alloc_sb.c --- Allocate the superblock and block group descriptors for a
 * newly initialized filesystem.  Used by mke2fs when initializing a filesystem
 *
 * Copyright (C) 1994, 1995, 1996, 2003 Theodore Ts'o.
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
 * This function reserves the superblock and block group descriptors
 * for a given block group.  It currently returns the number of free
 * blocks assuming that inode table and allocation bitmaps will be in
 * the group.  This is not necessarily the case when the flex_bg
 * feature is enabled, so callers should take care!  It was only
 * really intended for use by mke2fs, and even there it's not that
 * useful.  In the future, when we redo this function for 64-bit block
 * numbers, we should probably return the number of blocks used by the
 * super block and group descriptors instead.
 *
 * See also the comment for ext2fs_super_and_bgd_loc()
 */
صحيح ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t گروه,
				 ext2fs_بلوک_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	استفاده_شده_blks, old_desc_بلوکها, رقم_بلوکها;

	ext2fs_super_and_bgd_loc2(fs, گروه, &super_blk,
				  &old_desc_blk, &new_desc_blk, &استفاده_شده_blks);

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_بلوکها = fs->super->s_اولی_meta_bg;
	وإلا
		old_desc_بلوکها =
			fs->desc_بلوکها + fs->super->s_reserved_gdt_بلوکها;

	إذا (super_blk || (گروه == 0))
		ext2fs_mark_بلوک_bitmap2(bmap, super_blk);
	إذا ((گروه == 0) && (fs->blocksize == 1024) &&
	    EXT2_FS_CLUSTER_نسبت(fs) > 1)
		ext2fs_mark_بلوک_bitmap2(bmap, 0);

	إذا (old_desc_blk) {
		رقم_بلوکها = old_desc_بلوکها;
		إذا (old_desc_blk + رقم_بلوکها >= ext2fs_بلوکها_شمار(fs->super))
			رقم_بلوکها = ext2fs_بلوکها_شمار(fs->super) -
				old_desc_blk;
		ext2fs_mark_بلوک_bitmap_range2(bmap, old_desc_blk, رقم_بلوکها);
	}
	إذا (new_desc_blk)
		ext2fs_mark_بلوک_bitmap2(bmap, new_desc_blk);

	رقم_بلوکها = ext2fs_گروه_بلوکها_شمار(fs, گروه);
	رقم_بلوکها -= 2 + fs->inode_بلوکها_در_هر_گروه + استفاده_شده_blks;

	أرجع رقم_بلوکها  ;
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group and returns the number of blocks used by the
 * super block and group descriptors by looking up the block bitmap.
 */
errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
				        dgrp_t گروه,
				        ext2fs_بلوک_bitmap bmap,
				        blk_t *desc_بلوکها)
{
	blk64_t	رقم_بلوکها;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, گروه, bmap);

	retval = ext2fs_شمار_استفاده_شده_بلوکها(fs,
					ext2fs_گروه_اولی_block2(fs, گروه),
					ext2fs_گروه_آخرین_block2(fs, گروه),
					&رقم_بلوکها);
	إذا (!retval)
		*desc_بلوکها = رقم_بلوکها;

	أرجع retval;
}
