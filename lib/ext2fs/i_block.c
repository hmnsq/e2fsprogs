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
#د_که_پای
#ضمن <time.h>
#ضمن <string.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_iblk_جمع_کړه_بلاکونه(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t عدد_بلاکونه)
{
	بینښه اوږد اوږد b = inode->i_بلاکونه;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		b += ((اوږد اوږد) inode->osd2.linux2.l_i_بلاکونه_hi) << 32;

	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    عدد_بلاکونه *= fs->blocksize / 512;
	عدد_بلاکونه *= EXT2_FS_CLUSTER_نسبت(fs);

	b += عدد_بلاکونه;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاکونه_hi = b >> 32;
	وإلا إذا (b > 0xFFFFFFFF)
		أرجع EOVERFLOW;
	inode->i_بلاکونه = b & 0xFFFFFFFF;
	أرجع 0;
}

errcode_t ext2fs_iblk_sub_بلاکونه(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t عدد_بلاکونه)
{
	بینښه اوږد اوږد b = inode->i_بلاکونه;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		b += ((اوږد اوږد) inode->osd2.linux2.l_i_بلاکونه_hi) << 32;

	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    عدد_بلاکونه *= fs->blocksize / 512;
	عدد_بلاکونه *= EXT2_FS_CLUSTER_نسبت(fs);

	إذا (عدد_بلاکونه > b)
		أرجع EOVERFLOW;

	b -= عدد_بلاکونه;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاکونه_hi = b >> 32;
	inode->i_بلاکونه = b & 0xFFFFFFFF;
	أرجع 0;
}

errcode_t ext2fs_iblk_سېټ(ext2_filsys fs, بنية ext2_inode *inode, blk64_t b)
{
	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2_FS_CLUSTER_نسبت(fs);

	inode->i_بلاکونه = b & 0xFFFFFFFF;
	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاکونه_hi = b >> 32;
	وإلا إذا (b >> 32)
		أرجع EOVERFLOW;
	أرجع 0;
}
