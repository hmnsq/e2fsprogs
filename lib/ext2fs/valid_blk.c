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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
പൂർണ്ണസംഖ്യ ext2fs_inode_has_valid_blocks2(ext2_filsys fs, ഘടന ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	എങ്കിൽ (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		തിരികെ 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	എങ്കിൽ (LINUX_S_ISLNK (inode->i_mode)) {
		എങ്കിൽ (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			എങ്കിൽ (inode->i_blocks == 0)
				തിരികെ 0;
			തിരികെ !ext2fs_is_fast_symlink(inode);
		} അല്ലെങ്കിൽ {
			/* With an EA block, life gets more tricky */
			എങ്കിൽ (inode->i_size >= EXT2_N_BLOCKS*4)
				തിരികെ 1; /* definitely using i_block[] */
			എങ്കിൽ (inode->i_size > 4 && inode->i_block[1] == 0)
				തിരികെ 1; /* definitely using i_block[] */
			തിരികെ 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	എങ്കിൽ (inode->i_flags & EXT4_INLINE_DATA_FL)
		തിരികെ 0;
	തിരികെ 1;
}

പൂർണ്ണസംഖ്യ ext2fs_inode_has_valid_blocks(ഘടന ext2_inode *inode)
{
	തിരികെ ext2fs_inode_has_valid_blocks2(ശൂന്യ, inode);
}
