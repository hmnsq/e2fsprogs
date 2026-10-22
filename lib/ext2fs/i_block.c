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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <time.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#అన్తర్భావ <errno.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

errcode_t ext2fs_iblk_కూడు_ఖండాలు(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
				 blk64_t అంకె_ఖండాలు)
{
	గుర్తులేని దీర్ఘ దీర్ఘ b = inode->i_ఖండాలు;

	ఒకవేళ (ext2fs_has_feature_huge_file(fs->super))
		b += ((దీర్ఘ దీర్ఘ) inode->osd2.linux2.l_i_ఖండాలు_hi) << 32;

	ఒకవేళ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    అంకె_ఖండాలు *= fs->blocksize / 512;
	అంకె_ఖండాలు *= EXT2_FS_CLUSTER_నిష్పత్తి(fs);

	b += అంకె_ఖండాలు;

	ఒకవేళ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_ఖండాలు_hi = b >> 32;
	లేకపోతే ఒకవేళ (b > 0xFFFFFFFF)
		తిరిగి EOVERFLOW;
	inode->i_ఖండాలు = b & 0xFFFFFFFF;
	తిరిగి 0;
}

errcode_t ext2fs_iblk_sub_ఖండాలు(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
				 blk64_t అంకె_ఖండాలు)
{
	గుర్తులేని దీర్ఘ దీర్ఘ b = inode->i_ఖండాలు;

	ఒకవేళ (ext2fs_has_feature_huge_file(fs->super))
		b += ((దీర్ఘ దీర్ఘ) inode->osd2.linux2.l_i_ఖండాలు_hi) << 32;

	ఒకవేళ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    అంకె_ఖండాలు *= fs->blocksize / 512;
	అంకె_ఖండాలు *= EXT2_FS_CLUSTER_నిష్పత్తి(fs);

	ఒకవేళ (అంకె_ఖండాలు > b)
		తిరిగి EOVERFLOW;

	b -= అంకె_ఖండాలు;

	ఒకవేళ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_ఖండాలు_hi = b >> 32;
	inode->i_ఖండాలు = b & 0xFFFFFFFF;
	తిరిగి 0;
}

errcode_t ext2fs_iblk_సమితి(ext2_filsys fs, నిర్మాణం ext2_inode *inode, blk64_t b)
{
	ఒకవేళ (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2_FS_CLUSTER_నిష్పత్తి(fs);

	inode->i_ఖండాలు = b & 0xFFFFFFFF;
	ఒకవేళ (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_ఖండాలు_hi = b >> 32;
	లేకపోతే ఒకవేళ (b >> 32)
		తిరిగి EOVERFLOW;
	తిరిగి 0;
}
