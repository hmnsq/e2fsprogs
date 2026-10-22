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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <time.h>
#अन्तर्भाव <string.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t ext2fs_iblk_add_blocks(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्क_blocks)
{
	अचिन्हित दीर्घ दीर्घ b = inode->i_blocks;

	यदि (ext2fs_has_feature_huge_file(fs->super))
		b += ((दीर्घ दीर्घ) inode->osd2.linux2.l_i_blocks_hi) << 32;

	यदि (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अङ्क_blocks *= fs->blocksize / 512;
	अङ्क_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	b += अङ्क_blocks;

	यदि (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	अन्यथा यदि (b > 0xFFFFFFFF)
		फर्काऊ EOVERFLOW;
	inode->i_blocks = b & 0xFFFFFFFF;
	फर्काऊ 0;
}

errcode_t ext2fs_iblk_sub_blocks(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्क_blocks)
{
	अचिन्हित दीर्घ दीर्घ b = inode->i_blocks;

	यदि (ext2fs_has_feature_huge_file(fs->super))
		b += ((दीर्घ दीर्घ) inode->osd2.linux2.l_i_blocks_hi) << 32;

	यदि (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अङ्क_blocks *= fs->blocksize / 512;
	अङ्क_blocks *= EXT2FS_CLUSTER_RATIO(fs);

	यदि (अङ्क_blocks > b)
		फर्काऊ EOVERFLOW;

	b -= अङ्क_blocks;

	यदि (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	inode->i_blocks = b & 0xFFFFFFFF;
	फर्काऊ 0;
}

errcode_t ext2fs_iblk_set(ext2_filsys fs, संरचना ext2_inode *inode, blk64_t b)
{
	यदि (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2FS_CLUSTER_RATIO(fs);

	inode->i_blocks = b & 0xFFFFFFFF;
	यदि (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_blocks_hi = b >> 32;
	अन्यथा यदि (b >> 32)
		फर्काऊ EOVERFLOW;
	फर्काऊ 0;
}
