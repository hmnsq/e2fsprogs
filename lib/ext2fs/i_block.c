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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <time.h>
#समावेश <string.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#समावेश <errno.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, काष्ठा ext2_inode *inode,
				 blk64_t अंक_blocks)
{
	अचिन्हित दीर्घ दीर्घ b = inode->i_blocks;

	अगर (ext2fs_has_feature_huge_file(fs->super))
		b += ((दीर्घ दीर्घ) inode->osd2.linux2.l_i_blocks_hi) << 32;

	अगर (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अंक_blocks *= fs->blocksize / 512;
	अंक_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += अंक_blocks;

	अगर (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	अन्यथा अगर (b > 0xFFFFFFFF)
		वापस EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	वापस 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, काष्ठा ext2_inode *inode,
				 blk64_t अंक_blocks)
{
	अचिन्हित दीर्घ दीर्घ b = inode->i_blocks;

	अगर (ext2fs_has_feature_huge_file(fs->super))
		b += ((दीर्घ दीर्घ) inode->osd2.linux2.l_i_blocks_hi) << 32;

	अगर (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अंक_blocks *= fs->blocksize / 512;
	अंक_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	अगर (अंक_blocks > b)
		वापस EOVERFLOW;

	b -= अंक_blocks;

	अगर (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	वापस 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, काष्ठा ext2_inode *inode, blk64_t b)
{
	अगर (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	अगर (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	अन्यथा अगर (b >> 32)
		वापस EOVERFLOW;
	वापस 0;
}
