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
#ܐܢ HAVE_UNISTD_H
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
	ܐܢ (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		ܦܢܐ 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	ܐܢ (LINUX_S_ISLNK (inode->i_mode)) {
		ܐܢ (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			ܐܢ (inode->i_blocks == 0)
				ܦܢܐ 0;
			ܦܢܐ !ext2fs_is_fast_symlink(inode);
		} ܐܚܪܢܐ {
			/* With an EA block, life gets more tricky */
			ܐܢ (inode->i_size >= EXT2_N_BLOCKS*4)
				ܦܢܐ 1; /* definitely using i_block[] */
			ܐܢ (inode->i_size > 4 && inode->i_block[1] == 0)
				ܦܢܐ 1; /* definitely using i_block[] */
			ܦܢܐ 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	ܐܢ (inode->i_flags & EXT4_INLINE_DATA_FL)
		ܦܢܐ 0;
	ܦܢܐ 1;
}

صحيح ext2fs_inode_has_valid_blocks(بنية ext2_inode *inode)
{
	ܦܢܐ ext2fs_inode_has_valid_blocks2(عدم, inode);
}
