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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <time.h>

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
পূর্ণাঙ্ক ext2fs_inode_has_valid_blocks2(ext2_filsys fs, সংরচনা ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	যদি (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		ঘূৰাই 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	যদি (LINUX_S_ISLNK (inode->i_mode)) {
		যদি (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			যদি (inode->i_blocks == 0)
				ঘূৰাই 0;
			ঘূৰাই !ext2fs_is_fast_symlink(inode);
		} অন্যথা {
			/* With an EA block, life gets more tricky */
			যদি (inode->i_size >= EXT2_N_BLOCKS*4)
				ঘূৰাই 1; /* definitely using i_block[] */
			যদি (inode->i_size > 4 && inode->i_block[1] == 0)
				ঘূৰাই 1; /* definitely using i_block[] */
			ঘূৰাই 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	যদি (inode->i_flags & EXT4_INLINE_DATA_FL)
		ঘূৰাই 0;
	ঘূৰাই 1;
}

পূর্ণাঙ্ক ext2fs_inode_has_valid_blocks(সংরচনা ext2_inode *inode)
{
	ঘূৰাই ext2fs_inode_has_valid_blocks2(রিক্ত, inode);
}
