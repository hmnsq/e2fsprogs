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
#nếu HAVE_UNISTD_H
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
số_nguyên ext2fs_inode_has_valid_blocks2(ext2_filsys fs, struct ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	nếu (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		trả_về 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	nếu (LINUX_S_ISLNK (inode->i_mode)) {
		nếu (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			nếu (inode->i_blocks == 0)
				trả_về 0;
			trả_về !ext2fs_is_fast_symlink(inode);
		} không_thì {
			/* With an EA block, life gets more tricky */
			nếu (inode->i_size >= EXT2_N_BLOCKS*4)
				trả_về 1; /* definitely using i_block[] */
			nếu (inode->i_size > 4 && inode->i_block[1] == 0)
				trả_về 1; /* definitely using i_block[] */
			trả_về 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	nếu (inode->i_flags & EXT4_INLINE_DATA_FL)
		trả_về 0;
	trả_về 1;
}

số_nguyên ext2fs_inode_has_valid_blocks(struct ext2_inode *inode)
{
	trả_về ext2fs_inode_has_valid_blocks2(NULL, inode);
}
