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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#અન્તર્ભાવ <errno.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, સંરચના ext2_inode *inode,
				 blk64_t અંક_blocks)
{
	unsigned long long b = inode->i_blocks;

	જો (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	જો (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    અંક_blocks *= fs->blocksize / 512;
	અંક_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += અંક_blocks;

	જો (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	નહીંતર જો (b > 0xFFFFFFFF)
		પાછા EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	પાછા 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, સંરચના ext2_inode *inode,
				 blk64_t અંક_blocks)
{
	unsigned long long b = inode->i_blocks;

	જો (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	જો (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    અંક_blocks *= fs->blocksize / 512;
	અંક_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	જો (અંક_blocks > b)
		પાછા EOVERFLOW;

	b -= અંક_blocks;

	જો (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	પાછા 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, સંરચના ext2_inode *inode, blk64_t b)
{
	જો (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	જો (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	નહીંતર જો (b >> 32)
		પાછા EOVERFLOW;
	પાછા 0;
}
