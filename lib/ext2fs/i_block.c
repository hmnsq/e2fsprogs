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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, கட்டமைப்பு ext2_inode *inode,
				 blk64_t இலக்கம்_blocks)
{
	unsigned long long b = inode->i_blocks;

	எனில் (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	எனில் (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    இலக்கம்_blocks *= fs->blocksize / 512;
	இலக்கம்_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += இலக்கம்_blocks;

	எனில் (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	இல்லையெனில் எனில் (b > 0xFFFFFFFF)
		திரும்பு EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	திரும்பு 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, கட்டமைப்பு ext2_inode *inode,
				 blk64_t இலக்கம்_blocks)
{
	unsigned long long b = inode->i_blocks;

	எனில் (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	எனில் (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    இலக்கம்_blocks *= fs->blocksize / 512;
	இலக்கம்_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	எனில் (இலக்கம்_blocks > b)
		திரும்பு EOVERFLOW;

	b -= இலக்கம்_blocks;

	எனில் (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	திரும்பு 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, கட்டமைப்பு ext2_inode *inode, blk64_t b)
{
	எனில் (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	எனில் (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	இல்லையெனில் எனில் (b >> 32)
		திரும்பு EOVERFLOW;
	திரும்பு 0;
}
