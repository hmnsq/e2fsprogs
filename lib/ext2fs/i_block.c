/*
 * i_block.c --- Manage the i_block field for i_blocks
 *
 * Copyright (C) 2008 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

errcode_t ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
				 blk64_t ਅੰਕ_ਖੰਡਾਂ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ b = inode->i_ਖੰਡਾਂ;

	ਜੇ (ext2fs_has_feature_huge_file(fs->super))
		b += ((ਲੰਮਾ ਲੰਮਾ) inode->osd2.linux2.l_i_ਖੰਡਾਂ_hi) << 32;

	ਜੇ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    ਅੰਕ_ਖੰਡਾਂ *= fs->blocksize / 512;
	ਅੰਕ_ਖੰਡਾਂ *= EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs);

	b += ਅੰਕ_ਖੰਡਾਂ;

	ਜੇ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_ਖੰਡਾਂ_hi = b >> 32;
	ਨਹੀਂਤਾਂ ਜੇ (b > 0xFFFFFFFF)
		ਵਾਪਸ EOVERFLOW;
	inode->i_ਖੰਡਾਂ = b & 0xFFFFFFFF;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_iblk_sub_ਖੰਡਾਂ(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
				 blk64_t ਅੰਕ_ਖੰਡਾਂ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ b = inode->i_ਖੰਡਾਂ;

	ਜੇ (ext2fs_has_feature_huge_file(fs->super))
		b += ((ਲੰਮਾ ਲੰਮਾ) inode->osd2.linux2.l_i_ਖੰਡਾਂ_hi) << 32;

	ਜੇ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    ਅੰਕ_ਖੰਡਾਂ *= fs->blocksize / 512;
	ਅੰਕ_ਖੰਡਾਂ *= EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs);

	ਜੇ (ਅੰਕ_ਖੰਡਾਂ > b)
		ਵਾਪਸ EOVERFLOW;

	b -= ਅੰਕ_ਖੰਡਾਂ;

	ਜੇ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_ਖੰਡਾਂ_hi = b >> 32;
	inode->i_ਖੰਡਾਂ = b & 0xFFFFFFFF;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_iblk_ਸੈੱਟ(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode, blk64_t b)
{
	ਜੇ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs);

	inode->i_ਖੰਡਾਂ = b & 0xFFFFFFFF;
	ਜੇ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_ਖੰਡਾਂ_hi = b >> 32;
	ਨਹੀਂਤਾਂ ਜੇ (b >> 32)
		ਵਾਪਸ EOVERFLOW;
	ਵਾਪਸ 0;
}
