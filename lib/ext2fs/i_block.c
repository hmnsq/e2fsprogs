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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <time.h>
#ضمن <string.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_iblk_جوڙيو_بلاڪون(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t انگ_بلاڪون)
{
	بنانشان ڊگهو ڊگهو b = inode->i_بلاڪون;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		b += ((ڊگهو ڊگهو) inode->osd2.linux2.l_i_بلاڪون_hi) << 32;

	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    انگ_بلاڪون *= fs->blocksize / 512;
	انگ_بلاڪون *= EXT2_FS_CLUSTER_تناسب(fs);

	b += انگ_بلاڪون;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاڪون_hi = b >> 32;
	وإلا إذا (b > 0xFFFFFFFF)
		أرجع EOVERFLOW;
	inode->i_بلاڪون = b & 0xFFFFFFFF;
	أرجع 0;
}

errcode_t ext2fs_iblk_sub_بلاڪون(ext2_filsys fs, بنية ext2_inode *inode,
				 blk64_t انگ_بلاڪون)
{
	بنانشان ڊگهو ڊگهو b = inode->i_بلاڪون;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		b += ((ڊگهو ڊگهو) inode->osd2.linux2.l_i_بلاڪون_hi) << 32;

	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
	    انگ_بلاڪون *= fs->blocksize / 512;
	انگ_بلاڪون *= EXT2_FS_CLUSTER_تناسب(fs);

	إذا (انگ_بلاڪون > b)
		أرجع EOVERFLOW;

	b -= انگ_بلاڪون;

	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاڪون_hi = b >> 32;
	inode->i_بلاڪون = b & 0xFFFFFFFF;
	أرجع 0;
}

errcode_t ext2fs_iblk_سيٽ(ext2_filsys fs, بنية ext2_inode *inode, blk64_t b)
{
	إذا (!ext2fs_has_feature_huge_file(fs->super) ||
	    !(inode->i_flags & EXT4_HUGE_FILE_FL))
		b *= fs->blocksize / 512;
	b *= EXT2_FS_CLUSTER_تناسب(fs);

	inode->i_بلاڪون = b & 0xFFFFFFFF;
	إذا (ext2fs_has_feature_huge_file(fs->super))
		inode->osd2.linux2.l_i_بلاڪون_hi = b >> 32;
	وإلا إذا (b >> 32)
		أرجع EOVERFLOW;
	أرجع 0;
}
