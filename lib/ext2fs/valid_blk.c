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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
પૂર્ણાંક ext2fs_inode_has_valid_blocks2(ext2_filsys fs, સંરચના ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	જો (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		પાછા 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	જો (LINUX_S_ISLNK (inode->i_mode)) {
		જો (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			જો (inode->i_blocks == 0)
				પાછા 0;
			પાછા !ext2fs_is_fast_symlink(inode);
		} નહીંતર {
			/* With an EA block, life gets more tricky */
			જો (inode->i_size >= EXT2_N_BLOCKS*4)
				પાછા 1; /* definitely using i_block[] */
			જો (inode->i_size > 4 && inode->i_block[1] == 0)
				પાછા 1; /* definitely using i_block[] */
			પાછા 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	જો (inode->i_flags & EXT4_INLINE_DATA_FL)
		પાછા 0;
	પાછા 1;
}

પૂર્ણાંક ext2fs_inode_has_valid_blocks(સંરચના ext2_inode *inode)
{
	પાછા ext2fs_inode_has_valid_blocks2(ખાલી, inode);
}
