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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
पूर्णाङ्क ext2fs_inode_has_valid_blocks2(ext2_filsys fs, संरचना ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	यदि (!LINUX_S_ISDIR(inode->i_mode) && !LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode))
		फर्काऊ 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	यदि (LINUX_S_ISLNK (inode->i_mode)) {
		यदि (ext2fs_file_acl_block(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			यदि (inode->i_blocks == 0)
				फर्काऊ 0;
			फर्काऊ !ext2fs_is_fast_symlink(inode);
		} अन्यथा {
			/* With an EA block, life gets more tricky */
			यदि (inode->i_size >= EXT2_N_BLOCKS*4)
				फर्काऊ 1; /* definitely using i_block[] */
			यदि (inode->i_size > 4 && inode->i_block[1] == 0)
				फर्काऊ 1; /* definitely using i_block[] */
			फर्काऊ 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	यदि (inode->i_flags & EXT4_INLINE_DATA_FL)
		फर्काऊ 0;
	फर्काऊ 1;
}

पूर्णाङ्क ext2fs_inode_has_valid_blocks(संरचना ext2_inode *inode)
{
	फर्काऊ ext2fs_inode_has_valid_blocks2(रिक्त, inode);
}
