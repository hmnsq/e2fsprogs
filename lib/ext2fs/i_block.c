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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <time.h>
#अन्तर्भाव <string.h>
#सचे HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्कः_blocks)
{
	unsigned long long b = inode->i_blocks;

	सचे (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	सचे (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अङ्कः_blocks *= fs->blocksize / 512;
	अङ्कः_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += अङ्कः_blocks;

	सचे (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	अञ्ञथा सचे (b > 0xFFFFFFFF)
		पटिनिवत्त EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	पटिनिवत्त 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्कः_blocks)
{
	unsigned long long b = inode->i_blocks;

	सचे (ext2fs_has_feature_huge_file(fs->super))
		b += ((long long) inode->osd2.linux2.l_i_blocks_hi) << 32;

	सचे (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अङ्कः_blocks *= fs->blocksize / 512;
	अङ्कः_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	सचे (अङ्कः_blocks > b)
		पटिनिवत्त EOVERFLOW;

	b -= अङ्कः_blocks;

	सचे (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	पटिनिवत्त 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, संरचना ext2_inode *inode, blk64_t b)
{
	सचे (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	सचे (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	अञ्ञथा सचे (b >> 32)
		पटिनिवत्त EOVERFLOW;
	पटिनिवत्त 0;
}
