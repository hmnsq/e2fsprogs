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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_inode_has_valid_blocks2(ext2_filsys fs, ସଂରଚନା ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	ଯଦି (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		ଫେରନ୍ତୁ 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	ଯଦି (LINUX_S_ISLNK (inode->i_mode)) {
		ଯଦି (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			ଯଦି (inode->i_blocks == 0)
				ଫେରନ୍ତୁ 0;
			ଫେରନ୍ତୁ !ext2fs_is_fast_symlink(inode);
		} ଅନ୍ୟଥା {
			/* With an EA block, life gets more tricky */
			ଯଦି (inode->i_size >= EXT2_N_BLOCKS*4)
				ଫେରନ୍ତୁ 1; /* definitely using i_block[] */
			ଯଦି (inode->i_size > 4 && inode->i_block[1] == 0)
				ଫେରନ୍ତୁ 1; /* definitely using i_block[] */
			ଫେରନ୍ତୁ 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	ଯଦି (inode->i_flags & EXT4_INLINE_DATA_FL)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_inode_has_valid_blocks(ସଂରଚନା ext2_inode *inode)
{
	ଫେରନ୍ତୁ ext2fs_inode_has_valid_blocks2(ଖାଲି, inode);
}
