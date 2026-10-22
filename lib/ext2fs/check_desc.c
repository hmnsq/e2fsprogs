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
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_ਖੰਡ_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t ਪਹਿਲਾ_ਖੰਡ = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
	blk64_t ਆਖਰੀ_ਖੰਡ = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)-1;
	blk64_t blk, b;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (EXT2_DESC_ਮਾਪ(fs->super) & (EXT2_DESC_ਮਾਪ(fs->super) - 1))
		ਵਾਪਸ EXT2_ET_BAD_DESC_ਮਾਪ;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਲਈ (i = 0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	ਲਈ (i = 0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		ਜੇ (!ext2fs_has_feature_flex_bg(fs->super)) {
			ਪਹਿਲਾ_ਖੰਡ = ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, i);
			ਆਖਰੀ_ਖੰਡ = ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_ਖੰਡ_bitmap_loc(fs, i);
		ਜੇ (blk < ਪਹਿਲਾ_ਖੰਡ || blk > ਆਖਰੀ_ਖੰਡ ||
		    ext2fs_test_ਖੰਡ_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_ਖੰਡ_MAP;
			ਜਾਓ errout;
		}
		ext2fs_mark_ਖੰਡ_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ਜੇ (blk < ਪਹਿਲਾ_ਖੰਡ || blk > ਆਖਰੀ_ਖੰਡ ||
		    ext2fs_test_ਖੰਡ_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			ਜਾਓ errout;
		}
		ext2fs_mark_ਖੰਡ_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_ਸਾਰਣੀ_loc(fs, i);
		ਜੇ (blk < ਪਹਿਲਾ_ਖੰਡ ||
		    ((blk + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ - 1) > ਆਖਰੀ_ਖੰਡ)) {
			retval = EXT2_ET_GDESC_BAD_INODE_ਸਾਰਣੀ;
			ਜਾਓ errout;
		}
		ਲਈ (j = 0, b = blk; j < fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
		     j++, b++) {
			ਜੇ (ext2fs_test_ਖੰਡ_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_ਸਾਰਣੀ;
				ਜਾਓ errout;
			}
			ext2fs_mark_ਖੰਡ_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_ਖੰਡ_bitmap(bmap);
	ਵਾਪਸ retval;
}
