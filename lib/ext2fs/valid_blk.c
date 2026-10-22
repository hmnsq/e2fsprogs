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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
صحیح ext2fs_inode_has_valid_blocks2(ext2_filsys fs, ساختار ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	اگر (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		بازگشت 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	اگر (LINUX_S_ISLNK (inode->i_mode)) {
		اگر (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			اگر (inode->i_blocks == 0)
				بازگشت 0;
			بازگشت !ext2fs_is_fast_symlink(inode);
		} وگرنه {
			/* With an EA block, life gets more tricky */
			اگر (inode->i_size >= EXT2_N_BLOCKS*4)
				بازگشت 1; /* definitely using i_block[] */
			اگر (inode->i_size > 4 && inode->i_block[1] == 0)
				بازگشت 1; /* definitely using i_block[] */
			بازگشت 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	اگر (inode->i_flags & EXT4_INLINE_DATA_FL)
		بازگشت 0;
	بازگشت 1;
}

صحیح ext2fs_inode_has_valid_blocks(ساختار ext2_inode *inode)
{
	بازگشت ext2fs_inode_has_valid_blocks2(پوچ, inode);
}
