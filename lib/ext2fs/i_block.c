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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <time.h>
#شامل <string.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#شامل <errno.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

errcode_t ext2fs_iblk_جوڑو_بلاکیں(ext2_filsys fs, ساخت ext2_inode *inode,
				 blk64_t عدد_بلاکیں)
{
	غیرعلامتی طویل طویل b = inode->i_بلاکیں;

	اگر (ext2fs_has_feature_huge_file(fs->super))
		b += ((طویل طویل) inode->osd2.linux2.l_i_بلاکیں_hi) << 32;

	اگر (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    عدد_بلاکیں *= fs->blocksize / 512;
	عدد_بلاکیں *= EXT2_FS_CLUSTER_تناسب(fs);

	b += عدد_بلاکیں;

	اگر (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاکیں_hi = b >> 32;
	ورنہ اگر (b > 0xFFFFFFFF)
		واپس EOVERFLOW;
	inode->i_بلاکیں = b & 0xFFFFFFFF;
	واپس 0;
}

errcode_t ext2fs_iblk_sub_بلاکیں(ext2_filsys fs, ساخت ext2_inode *inode,
				 blk64_t عدد_بلاکیں)
{
	غیرعلامتی طویل طویل b = inode->i_بلاکیں;

	اگر (ext2fs_has_feature_huge_file(fs->super))
		b += ((طویل طویل) inode->osd2.linux2.l_i_بلاکیں_hi) << 32;

	اگر (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    عدد_بلاکیں *= fs->blocksize / 512;
	عدد_بلاکیں *= EXT2_FS_CLUSTER_تناسب(fs);

	اگر (عدد_بلاکیں > b)
		واپس EOVERFLOW;

	b -= عدد_بلاکیں;

	اگر (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاکیں_hi = b >> 32;
	inode->i_بلاکیں = b & 0xFFFFFFFF;
	واپس 0;
}

errcode_t ext2fs_iblk_سیٹ(ext2_filsys fs, ساخت ext2_inode *inode, blk64_t b)
{
	اگر (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2_FS_CLUSTER_تناسب(fs);

	inode->i_بلاکیں = b & 0xFFFFFFFF;
	اگر (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاکیں_hi = b >> 32;
	ورنہ اگر (b >> 32)
		واپس EOVERFLOW;
	واپس 0;
}
