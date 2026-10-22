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
صحیح ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t گروہ,
				 ext2fs_بلاک_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	مستعمل_blks, old_desc_بلاکیں, عدد_بلاکیں;

	ext2fs_super_and_bgd_loc2(fs, گروہ, &super_blk,
				  &old_desc_blk, &new_desc_blk, &مستعمل_blks);

	اگر (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_بلاکیں = fs->super->s_پہلا_meta_bg;
	ورنہ
		old_desc_بلاکیں =
			fs->desc_بلاکیں + fs->super->s_reserved_gdt_بلاکیں;

	اگر (super_blk || (گروہ == 0))
		ext2fs_mark_بلاک_bitmap2(bmap, super_blk);
	اگر ((گروہ == 0) && (fs->blocksize == 1024) &&
	    EXT2_FS_CLUSTER_تناسب(fs) > 1)
		ext2fs_mark_بلاک_bitmap2(bmap, 0);

	اگر (old_desc_blk) {
		عدد_بلاکیں = old_desc_بلاکیں;
		اگر (old_desc_blk + عدد_بلاکیں >= ext2fs_بلاکیں_گنتی(fs->super))
			عدد_بلاکیں = ext2fs_بلاکیں_گنتی(fs->super) -
				old_desc_blk;
		ext2fs_mark_بلاک_bitmap_range2(bmap, old_desc_blk, عدد_بلاکیں);
	}
	اگر (new_desc_blk)
		ext2fs_mark_بلاک_bitmap2(bmap, new_desc_blk);

	عدد_بلاکیں = ext2fs_گروہ_بلاکیں_گنتی(fs, گروہ);
	عدد_بلاکیں -= 2 + fs->inode_بلاکیں_فی_گروہ + مستعمل_blks;

	واپس عدد_بلاکیں  ;
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group and returns the number of blocks used by the
 * super block and group descriptors by looking up the block bitmap.
 */
errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
				        dgrp_t گروہ,
				        ext2fs_بلاک_bitmap bmap,
				        blk_t *desc_بلاکیں)
{
	blk64_t	عدد_بلاکیں;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, گروہ, bmap);

	retval = ext2fs_گنتی_مستعمل_بلاکیں(fs,
					ext2fs_گروہ_پہلا_block2(fs, گروہ),
					ext2fs_گروہ_آخری_block2(fs, گروہ),
					&عدد_بلاکیں);
	اگر (!retval)
		*desc_بلاکیں = عدد_بلاکیں;

	واپس retval;
}
