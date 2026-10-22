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

#include "config.h"
#include <stdio.h>
#ᬬᬾᬦ᭄ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
puur_naa_nka ext2fs_inode_has_valid_blocks2(ext2_filsys fs, sa_nracanaa ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	ᬬᬾᬦ᭄ (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		ᬯᬮᬶ 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	ᬬᬾᬦ᭄ (LINUX_S_ISLNK (inode->i_mode)) {
		ᬬᬾᬦ᭄ (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			ᬬᬾᬦ᭄ (inode->i_blocks == 0)
				ᬯᬮᬶ 0;
			ᬯᬮᬶ !ext2fs_is_fast_symlink(inode);
		} ᬮᬶᬬ {
			/* With an EA block, life gets more tricky */
			ᬬᬾᬦ᭄ (inode->i_size >= EXT2_N_BLOCKS*4)
				ᬯᬮᬶ 1; /* definitely using i_block[] */
			ᬬᬾᬦ᭄ (inode->i_size > 4 && inode->i_block[1] == 0)
				ᬯᬮᬶ 1; /* definitely using i_block[] */
			ᬯᬮᬶ 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	ᬬᬾᬦ᭄ (inode->i_flags & EXT4_INLINE_DATA_FL)
		ᬯᬮᬶ 0;
	ᬯᬮᬶ 1;
}

puur_naa_nka ext2fs_inode_has_valid_blocks(sa_nracanaa ext2_inode *inode)
{
	ᬯᬮᬶ ext2fs_inode_has_valid_blocks2(NULL, inode);
}
