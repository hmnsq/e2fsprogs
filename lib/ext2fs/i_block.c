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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <time.h>
#ضمن <string.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_iblk_جمع_کن_بلوکها(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t رقم_بلوکها)
{
	بیعلامت دراز دراز b = inode->i_بلوکها;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		b += ((دراز دراز) inode->osd2.linux2.l_i_بلوکها_hi) << 32;

	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    رقم_بلوکها *= fs->blocksize / 512;
	رقم_بلوکها *= EXT2_FS_CLUSTER_نسبت(fs);

	b += رقم_بلوکها;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلوکها_hi = b >> 32;
	وإلا إذا (b > 0xFFFFFFFF)
		أرجع EOVERFLOW;
	inode->i_بلوکها = b & 0xFFFFFFFF;
	أرجع 0;
}

errcode_t ext2fs_iblk_sub_بلوکها(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t رقم_بلوکها)
{
	بیعلامت دراز دراز b = inode->i_بلوکها;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		b += ((دراز دراز) inode->osd2.linux2.l_i_بلوکها_hi) << 32;

	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    رقم_بلوکها *= fs->blocksize / 512;
	رقم_بلوکها *= EXT2_FS_CLUSTER_نسبت(fs);

	إذا (رقم_بلوکها > b)
		أرجع EOVERFLOW;

	b -= رقم_بلوکها;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلوکها_hi = b >> 32;
	inode->i_بلوکها = b & 0xFFFFFFFF;
	أرجع 0;
}

errcode_t ext2fs_iblk_مجموعه(ext2_filsys fs, بنية ext2_inode *inode, blk64_t b)
{
	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2_FS_CLUSTER_نسبت(fs);

	inode->i_بلوکها = b & 0xFFFFFFFF;
	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلوکها_hi = b >> 32;
	وإلا إذا (b >> 32)
		أرجع EOVERFLOW;
	أرجع 0;
}
