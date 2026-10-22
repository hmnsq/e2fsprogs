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
				 dgrp_t ډله,
				 ext2fs_بلاک_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	کارول_شوی_blks, old_desc_بلاکونه, عدد_بلاکونه;

	ext2fs_super_and_bgd_loc2(fs, ډله, &super_blk,
				  &old_desc_blk, &new_desc_blk, &کارول_شوی_blks);

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_بلاکونه = fs->super->s_لومړی_meta_bg;
	وإلا
		old_desc_بلاکونه =
			fs->desc_بلاکونه + fs->super->s_reserved_gdt_بلاکونه;

	إذا (super_blk || (ډله == 0))
		ext2fs_mark_بلاک_bitmap2(bmap, super_blk);
	إذا ((ډله == 0) && (fs->blocksize == 1024) &&
	    EXT2_FS_CLUSTER_نسبت(fs) > 1)
		ext2fs_mark_بلاک_bitmap2(bmap, 0);

	إذا (old_desc_blk) {
		عدد_بلاکونه = old_desc_بلاکونه;
		إذا (old_desc_blk + عدد_بلاکونه >= ext2fs_بلاکونه_شمېر(fs->super))
			عدد_بلاکونه = ext2fs_بلاکونه_شمېر(fs->super) -
				old_desc_blk;
		ext2fs_mark_بلاک_bitmap_range2(bmap, old_desc_blk, عدد_بلاکونه);
	}
	إذا (new_desc_blk)
		ext2fs_mark_بلاک_bitmap2(bmap, new_desc_blk);

	عدد_بلاکونه = ext2fs_ډله_بلاکونه_شمېر(fs, ډله);
	عدد_بلاکونه -= 2 + fs->inode_بلاکونه_فی_ډله + کارول_شوی_blks;

	أرجع عدد_بلاکونه  ;
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group and returns the number of blocks used by the
 * super block and group descriptors by looking up the block bitmap.
 */
errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
				        dgrp_t ډله,
				        ext2fs_بلاک_bitmap bmap,
				        blk_t *desc_بلاکونه)
{
	blk64_t	عدد_بلاکونه;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, ډله, bmap);

	retval = ext2fs_شمېر_کارول_شوی_بلاکونه(fs,
					ext2fs_ډله_لومړی_block2(fs, ډله),
					ext2fs_ډله_وروستی_block2(fs, ډله),
					&عدد_بلاکونه);
	إذا (!retval)
		*desc_بلاکونه = عدد_بلاکونه;

	أرجع retval;
}
