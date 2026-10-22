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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

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
ਪੂਰਨ ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t ਸਮੂਹ,
				 ext2fs_ਖੰਡ_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	ਵਰਤਿਆ_blks, old_desc_ਖੰਡਾਂ, ਅੰਕ_ਖੰਡਾਂ;

	ext2fs_super_and_bgd_loc2(fs, ਸਮੂਹ, &super_blk,
				  &old_desc_blk, &new_desc_blk, &ਵਰਤਿਆ_blks);

	ਜੇ (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_ਖੰਡਾਂ = fs->super->s_ਪਹਿਲਾ_meta_bg;
	ਨਹੀਂਤਾਂ
		old_desc_ਖੰਡਾਂ =
			fs->desc_ਖੰਡਾਂ + fs->super->s_reserved_gdt_ਖੰਡਾਂ;

	ਜੇ (super_blk || (ਸਮੂਹ == 0))
		ext2fs_mark_ਖੰਡ_bitmap2(bmap, super_blk);
	ਜੇ ((ਸਮੂਹ == 0) && (fs->blocksize == 1024) &&
	    EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) > 1)
		ext2fs_mark_ਖੰਡ_bitmap2(bmap, 0);

	ਜੇ (old_desc_blk) {
		ਅੰਕ_ਖੰਡਾਂ = old_desc_ਖੰਡਾਂ;
		ਜੇ (old_desc_blk + ਅੰਕ_ਖੰਡਾਂ >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))
			ਅੰਕ_ਖੰਡਾਂ = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) -
				old_desc_blk;
		ext2fs_mark_ਖੰਡ_bitmap_range2(bmap, old_desc_blk, ਅੰਕ_ਖੰਡਾਂ);
	}
	ਜੇ (new_desc_blk)
		ext2fs_mark_ਖੰਡ_bitmap2(bmap, new_desc_blk);

	ਅੰਕ_ਖੰਡਾਂ = ext2fs_ਸਮੂਹ_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, ਸਮੂਹ);
	ਅੰਕ_ਖੰਡਾਂ -= 2 + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ + ਵਰਤਿਆ_blks;

	ਵਾਪਸ ਅੰਕ_ਖੰਡਾਂ  ;
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group and returns the number of blocks used by the
 * super block and group descriptors by looking up the block bitmap.
 */
errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
				        dgrp_t ਸਮੂਹ,
				        ext2fs_ਖੰਡ_bitmap bmap,
				        blk_t *desc_ਖੰਡਾਂ)
{
	blk64_t	ਅੰਕ_ਖੰਡਾਂ;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, ਸਮੂਹ, bmap);

	retval = ext2fs_ਗਿਣਤੀ_ਵਰਤਿਆ_ਖੰਡਾਂ(fs,
					ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, ਸਮੂਹ),
					ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, ਸਮੂਹ),
					&ਅੰਕ_ਖੰਡਾਂ);
	ਜੇ (!retval)
		*desc_ਖੰਡਾਂ = ਅੰਕ_ਖੰਡਾਂ;

	ਵਾਪਸ retval;
}
