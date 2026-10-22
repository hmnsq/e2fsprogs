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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <time.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
पूर्णांक ext2fs_inode_has_valid_blocks2(ext2_filsys fs, काष्ठा ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	अगर (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		वापस 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	अगर (LINUX_S_ISLNK (inode->i_mode)) {
		अगर (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			अगर (inode->i_blocks == 0)
				वापस 0;
			वापस !ext2fs_is_fast_symlink(inode);
		} अन्यथा {
			/* With an EA block, life gets more tricky */
			अगर (inode->i_आकार >= EXT2_N_BLOCKS*4)
				वापस 1; /* definitely using i_block[] */
			अगर (inode->i_आकार > 4 && inode->i_block[1] == 0)
				वापस 1; /* definitely using i_block[] */
			वापस 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	अगर (inode->i_flags & EXT4_INLINE_तथ्य_FL)
		वापस 0;
	वापस 1;
}

पूर्णांक ext2fs_inode_has_valid_blocks(काष्ठा ext2_inode *inode)
{
	वापस ext2fs_inode_has_valid_blocks2(शून्य, inode);
}
