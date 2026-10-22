/*
 * valid_blk.c --- does the inode have valid blocks?
 *
 * Copyright 1997 by Theodore Ts'o
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
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
పూర్ణాంకం ext2fs_inode_has_valid_blocks2(ext2_filsys fs, నిర్మాణం ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	ఒకవేళ (!LINUX_S_ISDIR(inode->i_బహుళకం) && !LINUX_S_ISREG(inode->i_బహుళకం) &&
	    !LINUX_S_ISLNK(inode->i_బహుళకం))
		తిరిగి 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	ఒకవేళ (LINUX_S_ISLNK (inode->i_బహుళకం)) {
		ఒకవేళ (ext2fs_file_acl_ఖండం(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			ఒకవేళ (inode->i_ఖండాలు == 0)
				తిరిగి 0;
			తిరిగి !ext2fs_is_fast_symlink(inode);
		} లేకపోతే {
			/* With an EA block, life gets more tricky */
			ఒకవేళ (inode->i_కొలత >= EXT2_N_ఖండాలు*4)
				తిరిగి 1; /* definitely using i_block[] */
			ఒకవేళ (inode->i_కొలత > 4 && inode->i_ఖండం[1] == 0)
				తిరిగి 1; /* definitely using i_block[] */
			తిరిగి 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	ఒకవేళ (inode->i_flags & EXT4_INLINE_దత్తాంశం_FL)
		తిరిగి 0;
	తిరిగి 1;
}

పూర్ణాంకం ext2fs_inode_has_valid_ఖండాలు(నిర్మాణం ext2_inode *inode)
{
	తిరిగి ext2fs_inode_has_valid_blocks2(ఖాళీ, inode);
}
