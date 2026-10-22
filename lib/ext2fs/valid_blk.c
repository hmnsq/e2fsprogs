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

#ضمن "config.h"
#ضمن <stdio.h>
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
صحيح ext2fs_inode_has_valid_blocks2(ext2_filsys fs, بنية ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	אם (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		החזר 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	אם (LINUX_S_ISLNK (inode->i_mode)) {
		אם (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			אם (inode->i_blocks == 0)
				החזר 0;
			החזר !ext2fs_is_fast_symlink(inode);
		} אחרת {
			/* With an EA block, life gets more tricky */
			אם (inode->i_size >= EXT2_N_BLOCKS*4)
				החזר 1; /* definitely using i_block[] */
			אם (inode->i_size > 4 && inode->i_block[1] == 0)
				החזר 1; /* definitely using i_block[] */
			החזר 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	אם (inode->i_flags & EXT4_INLINE_DATA_FL)
		החזר 0;
	החזר 1;
}

صحيح ext2fs_inode_has_valid_blocks(بنية ext2_inode *inode)
{
	החזר ext2fs_inode_has_valid_blocks2(عدم, inode);
}
