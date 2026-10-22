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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, ഘടന ext2_inode *inode,
				 blk64_t അക്കം_blocks)
{
	unsigned long long b = inode->i_blocks;

	എങ്കിൽ (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	എങ്കിൽ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    അക്കം_blocks *= fs->blocksize / 512;
	അക്കം_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += അക്കം_blocks;

	എങ്കിൽ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	അല്ലെങ്കിൽ എങ്കിൽ (b > 0xFFFFFFFF)
		തിരികെ EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	തിരികെ 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, ഘടന ext2_inode *inode,
				 blk64_t അക്കം_blocks)
{
	unsigned long long b = inode->i_blocks;

	എങ്കിൽ (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	എങ്കിൽ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    അക്കം_blocks *= fs->blocksize / 512;
	അക്കം_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	എങ്കിൽ (അക്കം_blocks > b)
		തിരികെ EOVERFLOW;

	b -= അക്കം_blocks;

	എങ്കിൽ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	തിരികെ 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, ഘടന ext2_inode *inode, blk64_t b)
{
	എങ്കിൽ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	എങ്കിൽ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	അല്ലെങ്കിൽ എങ്കിൽ (b >> 32)
		തിരികെ EOVERFLOW;
	തിരികെ 0;
}
