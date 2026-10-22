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
#جے_انت
#ضمن <time.h>
#ضمن <string.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_iblk_جوڑو_بلاکاں(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t عدد_بلاکاں)
{
	بنانشانی لما لما b = inode->i_بلاکاں;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		b += ((لما لما) inode->osd2.linux2.l_i_بلاکاں_hi) << 32;

	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    عدد_بلاکاں *= fs->blocksize / 512;
	عدد_بلاکاں *= EXT2_FS_CLUSTER_تناسب(fs);

	b += عدد_بلاکاں;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاکاں_hi = b >> 32;
	وإلا إذا (b > 0xFFFFFFFF)
		أرجع EOVERFLOW;
	inode->i_بلاکاں = b & 0xFFFFFFFF;
	أرجع 0;
}

errcode_t ext2fs_iblk_sub_بلاکاں(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t عدد_بلاکاں)
{
	بنانشانی لما لما b = inode->i_بلاکاں;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		b += ((لما لما) inode->osd2.linux2.l_i_بلاکاں_hi) << 32;

	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    عدد_بلاکاں *= fs->blocksize / 512;
	عدد_بلاکاں *= EXT2_FS_CLUSTER_تناسب(fs);

	إذا (عدد_بلاکاں > b)
		أرجع EOVERFLOW;

	b -= عدد_بلاکاں;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاکاں_hi = b >> 32;
	inode->i_بلاکاں = b & 0xFFFFFFFF;
	أرجع 0;
}

errcode_t ext2fs_iblk_سیٹ(ext2_filsys fs, بنية ext2_inode *inode, blk64_t b)
{
	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2_FS_CLUSTER_تناسب(fs);

	inode->i_بلاکاں = b & 0xFFFFFFFF;
	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاکاں_hi = b >> 32;
	وإلا إذا (b >> 32)
		أرجع EOVERFLOW;
	أرجع 0;
}
