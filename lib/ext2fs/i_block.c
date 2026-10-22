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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <time.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif
#অন্তর্ভাৱ <errno.h>

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, সংরচনা ext2_inode *inode,
				 blk64_t অঙ্ক_blocks)
{
	unsigned long long b = inode->i_blocks;

	যদি (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	যদি (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    অঙ্ক_blocks *= fs->blocksize / 512;
	অঙ্ক_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += অঙ্ক_blocks;

	যদি (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	অন্যথা যদি (b > 0xFFFFFFFF)
		ঘূৰাই EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	ঘূৰাই 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, সংরচনা ext2_inode *inode,
				 blk64_t অঙ্ক_blocks)
{
	unsigned long long b = inode->i_blocks;

	যদি (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	যদি (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    অঙ্ক_blocks *= fs->blocksize / 512;
	অঙ্ক_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	যদি (অঙ্ক_blocks > b)
		ঘূৰাই EOVERFLOW;

	b -= অঙ্ক_blocks;

	যদি (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	ঘূৰাই 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, সংরচনা ext2_inode *inode, blk64_t b)
{
	যদি (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	যদি (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	অন্যথা যদি (b >> 32)
		ঘূৰাই EOVERFLOW;
	ঘূৰাই 0;
}
