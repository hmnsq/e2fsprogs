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
				 dgrp_t گروهه,
				 ext2fs_بلاڪ_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	استعمال_ٿيل_blks, old_desc_بلاڪون, انگ_بلاڪون;

	ext2fs_super_and_bgd_loc2(fs, گروهه, &super_blk,
				  &old_desc_blk, &new_desc_blk, &استعمال_ٿيل_blks);

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_بلاڪون = fs->super->s_پهريون_meta_bg;
	وإلا
		old_desc_بلاڪون =
			fs->desc_بلاڪون + fs->super->s_reserved_gdt_بلاڪون;

	إذا (super_blk || (گروهه == 0))
		ext2fs_mark_بلاڪ_bitmap2(bmap, super_blk);
	إذا ((گروهه == 0) && (fs->blocksize == 1024) &&
	    EXT2_FS_CLUSTER_تناسب(fs) > 1)
		ext2fs_mark_بلاڪ_bitmap2(bmap, 0);

	إذا (old_desc_blk) {
		انگ_بلاڪون = old_desc_بلاڪون;
		إذا (old_desc_blk + انگ_بلاڪون >= ext2fs_بلاڪون_ڳڻپ(fs->super))
			انگ_بلاڪون = ext2fs_بلاڪون_ڳڻپ(fs->super) -
				old_desc_blk;
		ext2fs_mark_بلاڪ_bitmap_range2(bmap, old_desc_blk, انگ_بلاڪون);
	}
	إذا (new_desc_blk)
		ext2fs_mark_بلاڪ_bitmap2(bmap, new_desc_blk);

	انگ_بلاڪون = ext2fs_گروهه_بلاڪون_ڳڻپ(fs, گروهه);
	انگ_بلاڪون -= 2 + fs->inode_بلاڪون_في_گروهه + استعمال_ٿيل_blks;

	أرجع انگ_بلاڪون  ;
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group and returns the number of blocks used by the
 * super block and group descriptors by looking up the block bitmap.
 */
errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
				        dgrp_t گروهه,
				        ext2fs_بلاڪ_bitmap bmap,
				        blk_t *desc_بلاڪون)
{
	blk64_t	انگ_بلاڪون;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, گروهه, bmap);

	retval = ext2fs_ڳڻپ_استعمال_ٿيل_بلاڪون(fs,
					ext2fs_گروهه_پهريون_block2(fs, گروهه),
					ext2fs_گروهه_آخري_block2(fs, گروهه),
					&انگ_بلاڪون);
	إذا (!retval)
		*desc_بلاڪون = انگ_بلاڪون;

	أرجع retval;
}
