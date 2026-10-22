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
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
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
	إذا (!LINUX_S_ISDIR(inode->i_نما) && !LINUX_S_ISREG(inode->i_نما) &&
	    !LINUX_S_ISLNK(inode->i_نما))
		أرجع 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	إذا (LINUX_S_ISLNK (inode->i_نما)) {
		إذا (ext2fs_file_acl_بلوک(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			إذا (inode->i_بلوکها == 0)
				أرجع 0;
			أرجع !ext2fs_is_fast_symlink(inode);
		} وإلا {
			/* With an EA block, life gets more tricky */
			إذا (inode->i_اندازه >= EXT2_N_بلوکها*4)
				أرجع 1; /* definitely using i_block[] */
			إذا (inode->i_اندازه > 4 && inode->i_بلوک[1] == 0)
				أرجع 1; /* definitely using i_block[] */
			أرجع 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	إذا (inode->i_flags & EXT4_INLINE_داده_FL)
		أرجع 0;
	أرجع 1;
}

صحيح ext2fs_inode_has_valid_بلوکها(بنية ext2_inode *inode)
{
	أرجع ext2fs_inode_has_valid_blocks2(عدم, inode);
}
