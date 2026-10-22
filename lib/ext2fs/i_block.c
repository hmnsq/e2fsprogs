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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, ସଂରଚନା ext2_inode *inode,
				 blk64_t ଅଙ୍କ_blocks)
{
	unsigned long long b = inode->i_blocks;

	ଯଦି (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	ଯଦି (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    ଅଙ୍କ_blocks *= fs->blocksize / 512;
	ଅଙ୍କ_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += ଅଙ୍କ_blocks;

	ଯଦି (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	ଅନ୍ୟଥା ଯଦି (b > 0xFFFFFFFF)
		ଫେରନ୍ତୁ EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, ସଂରଚନା ext2_inode *inode,
				 blk64_t ଅଙ୍କ_blocks)
{
	unsigned long long b = inode->i_blocks;

	ଯଦି (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	ଯଦି (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    ଅଙ୍କ_blocks *= fs->blocksize / 512;
	ଅଙ୍କ_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	ଯଦି (ଅଙ୍କ_blocks > b)
		ଫେରନ୍ତୁ EOVERFLOW;

	b -= ଅଙ୍କ_blocks;

	ଯଦି (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, ସଂରଚନା ext2_inode *inode, blk64_t b)
{
	ଯଦି (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	ଯଦି (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	ଅନ୍ୟଥା ଯଦି (b >> 32)
		ଫେରନ୍ତୁ EOVERFLOW;
	ଫେରନ୍ତୁ 0;
}
