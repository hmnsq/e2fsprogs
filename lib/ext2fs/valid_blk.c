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
#यदि_अन्तः
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
	यदि (!LINUX_S_ISDIR(inode->i_बहुलकम्) && !LINUX_S_ISREG(inode->i_बहुलकम्) &&
	    !LINUX_S_ISLNK(inode->i_बहुलकम्))
		प्रत्यागम 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	यदि (LINUX_S_ISLNK (inode->i_बहुलकम्)) {
		यदि (ext2fs_file_acl_खण्डम्(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			यदि (inode->i_खण्डानि == 0)
				प्रत्यागम 0;
			प्रत्यागम !ext2fs_is_fast_symlink(inode);
		} अन्यथा {
			/* With an EA block, life gets more tricky */
			यदि (inode->i_आकार >= EXT2_N_खण्डानि*4)
				प्रत्यागम 1; /* definitely using i_block[] */
			यदि (inode->i_आकार > 4 && inode->i_खण्डम्[1] == 0)
				प्रत्यागम 1; /* definitely using i_block[] */
			प्रत्यागम 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	यदि (inode->i_flags & EXT4_INLINE_दत्त_FL)
		प्रत्यागम 0;
	प्रत्यागम 1;
}

पूर्णाङ्क ext2fs_inode_has_valid_खण्डानि(संरचना ext2_inode *inode)
{
	प्रत्यागम ext2fs_inode_has_valid_blocks2(रिक्त, inode);
}
