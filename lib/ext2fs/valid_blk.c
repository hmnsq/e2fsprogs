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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_has_valid_blocks2(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	ಯದಿ (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		ಪ್ರತ್ಯಾಗಮ 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	ಯದಿ (LINUX_S_ISLNK (inode->i_mode)) {
		ಯದಿ (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			ಯದಿ (inode->i_blocks == 0)
				ಪ್ರತ್ಯಾಗಮ 0;
			ಪ್ರತ್ಯಾಗಮ !ext2fs_is_fast_symlink(inode);
		} ಅನ್ಯಥಾ {
			/* With an EA block, life gets more tricky */
			ಯದಿ (inode->i_size >= EXT2_N_BLOCKS*4)
				ಪ್ರತ್ಯಾಗಮ 1; /* definitely using i_block[] */
			ಯದಿ (inode->i_size > 4 && inode->i_block[1] == 0)
				ಪ್ರತ್ಯಾಗಮ 1; /* definitely using i_block[] */
			ಪ್ರತ್ಯಾಗಮ 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	ಯದಿ (inode->i_flags & EXT4_INLINE_DATA_FL)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_has_valid_blocks(ಸಂರಚನಾ ext2_inode *inode)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_inode_has_valid_blocks2(ರಿಕ್ತ, inode);
}
