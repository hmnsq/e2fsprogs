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

#include "config.h"
#include <stdio.h>
#ᤀᤡ᤺ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <time.h>
#include <string.h>
#ᤀᤡ᤺ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᤀᤡ᤺ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <errno.h>

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, sa_nracanaa ext2_inode *inode,
				 blk64_t num_blocks)
{
	unsigned long long b = inode->i_blocks;

	ᤀᤡ᤺ (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	ᤀᤡ᤺ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    num_blocks *= fs->blocksize / 512;
	num_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += num_blocks;

	ᤀᤡ᤺ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (b > 0xFFFFFFFF)
		ᤀᤡᤴ EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	ᤀᤡᤴ 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, sa_nracanaa ext2_inode *inode,
				 blk64_t num_blocks)
{
	unsigned long long b = inode->i_blocks;

	ᤀᤡ᤺ (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	ᤀᤡ᤺ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    num_blocks *= fs->blocksize / 512;
	num_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	ᤀᤡ᤺ (num_blocks > b)
		ᤀᤡᤴ EOVERFLOW;

	b -= num_blocks;

	ᤀᤡ᤺ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	ᤀᤡᤴ 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, sa_nracanaa ext2_inode *inode, blk64_t b)
{
	ᤀᤡ᤺ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	ᤀᤡ᤺ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (b >> 32)
		ᤀᤡᤴ EOVERFLOW;
	ᤀᤡᤴ 0;
}
