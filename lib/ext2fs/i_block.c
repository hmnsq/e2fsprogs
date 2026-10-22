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
#endif
#شامل <time.h>
#شامل <string.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#شامل <errno.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, ساختار ext2_inode *inode,
				 blk64_t رقم_blocks)
{
	unsigned long long b = inode->i_blocks;

	اگر (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	اگر (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    رقم_blocks *= fs->blocksize / 512;
	رقم_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += رقم_blocks;

	اگر (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	وگرنه اگر (b > 0xFFFFFFFF)
		بازگشت EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	بازگشت 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, ساختار ext2_inode *inode,
				 blk64_t رقم_blocks)
{
	unsigned long long b = inode->i_blocks;

	اگر (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	اگر (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    رقم_blocks *= fs->blocksize / 512;
	رقم_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	اگر (رقم_blocks > b)
		بازگشت EOVERFLOW;

	b -= رقم_blocks;

	اگر (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	بازگشت 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, ساختار ext2_inode *inode, blk64_t b)
{
	اگر (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	اگر (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	وگرنه اگر (b >> 32)
		بازگشت EOVERFLOW;
	بازگشت 0;
}
