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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode,
				 blk64_t ಅಂಕೆ_blocks)
{
	unsigned long long b = inode->i_blocks;

	ಯದಿ (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	ಯದಿ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    ಅಂಕೆ_blocks *= fs->blocksize / 512;
	ಅಂಕೆ_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += ಅಂಕೆ_blocks;

	ಯದಿ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	ಅನ್ಯಥಾ ಯದಿ (b > 0xFFFFFFFF)
		ಪ್ರತ್ಯಾಗಮ EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode,
				 blk64_t ಅಂಕೆ_blocks)
{
	unsigned long long b = inode->i_blocks;

	ಯದಿ (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	ಯದಿ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    ಅಂಕೆ_blocks *= fs->blocksize / 512;
	ಅಂಕೆ_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	ಯದಿ (ಅಂಕೆ_blocks > b)
		ಪ್ರತ್ಯಾಗಮ EOVERFLOW;

	b -= ಅಂಕೆ_blocks;

	ಯದಿ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode, blk64_t b)
{
	ಯದಿ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	ಯದಿ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	ಅನ್ಯಥಾ ಯದಿ (b >> 32)
		ಪ್ರತ್ಯಾಗಮ EOVERFLOW;
	ಪ್ರತ್ಯಾಗಮ 0;
}
