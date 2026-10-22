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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

errcode_t ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
				 blk64_t ಅಂಕೆ_ಖಂಡಗಳು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ b = inode->i_ಖಂಡಗಳು;

	ಆದರೆ (ext2fs_has_feature_huge_file(fs->super))
		b += ((ದೀರ್ಘ ದೀರ್ಘ) inode->osd2.linux2.l_i_ಖಂಡಗಳು_hi) << 32;

	ಆದರೆ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    ಅಂಕೆ_ಖಂಡಗಳು *= fs->blocksize / 512;
	ಅಂಕೆ_ಖಂಡಗಳು *= EXT2_FS_CLUSTER_ಅನುಪಾತ(fs);

	b += ಅಂಕೆ_ಖಂಡಗಳು;

	ಆದರೆ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_ಖಂಡಗಳು_hi = b >> 32;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (b > 0xFFFFFFFF)
		ಹಿಂತಿರುಗಿ EOVERFLOW;
	inode->i_ಖಂಡಗಳು = b & 0xFFFFFFFF;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_iblk_sub_ಖಂಡಗಳು(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
				 blk64_t ಅಂಕೆ_ಖಂಡಗಳು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ b = inode->i_ಖಂಡಗಳು;

	ಆದರೆ (ext2fs_has_feature_huge_file(fs->super))
		b += ((ದೀರ್ಘ ದೀರ್ಘ) inode->osd2.linux2.l_i_ಖಂಡಗಳು_hi) << 32;

	ಆದರೆ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    ಅಂಕೆ_ಖಂಡಗಳು *= fs->blocksize / 512;
	ಅಂಕೆ_ಖಂಡಗಳು *= EXT2_FS_CLUSTER_ಅನುಪಾತ(fs);

	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು > b)
		ಹಿಂತಿರುಗಿ EOVERFLOW;

	b -= ಅಂಕೆ_ಖಂಡಗಳು;

	ಆದರೆ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_ಖಂಡಗಳು_hi = b >> 32;
	inode->i_ಖಂಡಗಳು = b & 0xFFFFFFFF;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_iblk_ಗಣ(ext2_filsys fs, ರಚನೆ ext2_inode *inode, blk64_t b)
{
	ಆದರೆ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2_FS_CLUSTER_ಅನುಪಾತ(fs);

	inode->i_ಖಂಡಗಳು = b & 0xFFFFFFFF;
	ಆದರೆ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_ಖಂಡಗಳು_hi = b >> 32;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (b >> 32)
		ಹಿಂತಿರುಗಿ EOVERFLOW;
	ಹಿಂತಿರುಗಿ 0;
}
