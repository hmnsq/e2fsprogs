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
#ប្រសិនបើ HAVE_UNISTD_H
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
ចំនួនគត់ ext2fs_inode_has_valid_blocks2(ext2_filsys fs, sa_nracanaa ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	ប្រសិនបើ (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		ត្រឡប់ 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	ប្រសិនបើ (LINUX_S_ISLNK (inode->i_mode)) {
		ប្រសិនបើ (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			ប្រសិនបើ (inode->i_blocks == 0)
				ត្រឡប់ 0;
			ត្រឡប់ !ext2fs_is_fast_symlink(inode);
		} បើមិនដូច្នេះ {
			/* With an EA block, life gets more tricky */
			ប្រសិនបើ (inode->i_size >= EXT2_N_BLOCKS*4)
				ត្រឡប់ 1; /* definitely using i_block[] */
			ប្រសិនបើ (inode->i_size > 4 && inode->i_block[1] == 0)
				ត្រឡប់ 1; /* definitely using i_block[] */
			ត្រឡប់ 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	ប្រសិនបើ (inode->i_flags & EXT4_INLINE_DATA_FL)
		ត្រឡប់ 0;
	ត្រឡប់ 1;
}

ចំនួនគត់ ext2fs_inode_has_valid_blocks(sa_nracanaa ext2_inode *inode)
{
	ត្រឡប់ ext2fs_inode_has_valid_blocks2(NULL, inode);
}
