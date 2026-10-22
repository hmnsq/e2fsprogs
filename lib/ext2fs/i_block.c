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
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <time.h>
#ضمن <string.h>
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t num_blocks)
{
	unsigned long long b = inode->i_blocks;

	ܐܢ (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	ܐܢ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    num_blocks *= fs->blocksize / 512;
	num_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += num_blocks;

	ܐܢ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	ܐܚܪܢܐ ܐܢ (b > 0xFFFFFFFF)
		ܦܢܐ EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	ܦܢܐ 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t num_blocks)
{
	unsigned long long b = inode->i_blocks;

	ܐܢ (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	ܐܢ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    num_blocks *= fs->blocksize / 512;
	num_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	ܐܢ (num_blocks > b)
		ܦܢܐ EOVERFLOW;

	b -= num_blocks;

	ܐܢ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	ܦܢܐ 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, بنية ext2_inode *inode, blk64_t b)
{
	ܐܢ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	ܐܢ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	ܐܚܪܢܐ ܐܢ (b >> 32)
		ܦܢܐ EOVERFLOW;
	ܦܢܐ 0;
}
