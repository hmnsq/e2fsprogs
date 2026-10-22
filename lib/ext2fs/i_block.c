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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <time.h>
#अन्तर्भाव <string.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अंक_blocks)
{
	अचिन्हित दीर्घ दीर्घ b = inode->i_blocks;

	जर (ext2fs_has_feature_huge_file(fs->super))
		b += ((दीर्घ दीर्घ) inode->osd2.linux2.l_i_blocks_hi) << 32;

	जर (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अंक_blocks *= fs->blocksize / 512;
	अंक_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += अंक_blocks;

	जर (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	अन्यथा जर (b > 0xFFFFFFFF)
		परत EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	परत 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अंक_blocks)
{
	अचिन्हित दीर्घ दीर्घ b = inode->i_blocks;

	जर (ext2fs_has_feature_huge_file(fs->super))
		b += ((दीर्घ दीर्घ) inode->osd2.linux2.l_i_blocks_hi) << 32;

	जर (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अंक_blocks *= fs->blocksize / 512;
	अंक_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	जर (अंक_blocks > b)
		परत EOVERFLOW;

	b -= अंक_blocks;

	जर (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	परत 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, संरचना ext2_inode *inode, blk64_t b)
{
	जर (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	जर (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	अन्यथा जर (b >> 32)
		परत EOVERFLOW;
	परत 0;
}
