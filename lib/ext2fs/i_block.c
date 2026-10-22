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
#यदि_अन्तः
#अन्तर्भाव <time.h>
#अन्तर्भाव <string.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t ext2fs_iblk_सङ्कलय_खण्डानि(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्कः_खण्डानि)
{
	अचिह्नित दीर्घ दीर्घ b = inode->i_खण्डानि;

	यदि (ext2fs_has_feature_huge_file(fs->super))
		b += ((दीर्घ दीर्घ) inode->osd2.linux2.l_i_खण्डानि_hi) << 32;

	यदि (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अङ्कः_खण्डानि *= fs->blocksize / 512;
	अङ्कः_खण्डानि *= EXT2_FS_CLUSTER_अनुपातः(fs);

	b += अङ्कः_खण्डानि;

	यदि (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_खण्डानि_hi = b >> 32;
	अन्यथा यदि (b > 0xFFFFFFFF)
		प्रत्यागम EOVERFLOW;
	inode->i_खण्डानि = b & 0xFFFFFFFF;
	प्रत्यागम 0;
}

errcode_t ext2fs_iblk_sub_खण्डानि(ext2_filsys fs, संरचना ext2_inode *inode,
				 blk64_t अङ्कः_खण्डानि)
{
	अचिह्नित दीर्घ दीर्घ b = inode->i_खण्डानि;

	यदि (ext2fs_has_feature_huge_file(fs->super))
		b += ((दीर्घ दीर्घ) inode->osd2.linux2.l_i_खण्डानि_hi) << 32;

	यदि (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    अङ्कः_खण्डानि *= fs->blocksize / 512;
	अङ्कः_खण्डानि *= EXT2_FS_CLUSTER_अनुपातः(fs);

	यदि (अङ्कः_खण्डानि > b)
		प्रत्यागम EOVERFLOW;

	b -= अङ्कः_खण्डानि;

	यदि (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_खण्डानि_hi = b >> 32;
	inode->i_खण्डानि = b & 0xFFFFFFFF;
	प्रत्यागम 0;
}

errcode_t ext2fs_iblk_समुच्चयः(ext2_filsys fs, संरचना ext2_inode *inode, blk64_t b)
{
	यदि (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2_FS_CLUSTER_अनुपातः(fs);

	inode->i_खण्डानि = b & 0xFFFFFFFF;
	यदि (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_खण्डानि_hi = b >> 32;
	अन्यथा यदि (b >> 32)
		प्रत्यागम EOVERFLOW;
	प्रत्यागम 0;
}
