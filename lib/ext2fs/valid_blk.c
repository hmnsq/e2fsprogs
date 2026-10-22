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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
ಪೂರ್ಣಾಂಕ ext2fs_inode_has_valid_blocks2(ext2_filsys fs, ರಚನೆ ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	ಆದರೆ (!LINUX_S_ISDIR(inode->i_ಬಹುಲಕ) && !LINUX_S_ISREG(inode->i_ಬಹುಲಕ) &&
	    !LINUX_S_ISLNK(inode->i_ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	ಆದರೆ (LINUX_S_ISLNK (inode->i_ಬಹುಲಕ)) {
		ಆದರೆ (ext2fs_file_acl_ಖಂಡ(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			ಆದರೆ (inode->i_ಖಂಡಗಳು == 0)
				ಹಿಂತಿರುಗಿ 0;
			ಹಿಂತಿರುಗಿ !ext2fs_is_fast_symlink(inode);
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* With an EA block, life gets more tricky */
			ಆದರೆ (inode->i_ಗಾತ್ರ >= EXT2_N_ಖಂಡಗಳು*4)
				ಹಿಂತಿರುಗಿ 1; /* definitely using i_block[] */
			ಆದರೆ (inode->i_ಗಾತ್ರ > 4 && inode->i_ಖಂಡ[1] == 0)
				ಹಿಂತಿರುಗಿ 1; /* definitely using i_block[] */
			ಹಿಂತಿರುಗಿ 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	ಆದರೆ (inode->i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ ext2fs_inode_has_valid_ಖಂಡಗಳು(ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_inode_has_valid_blocks2(ಖಾಲಿ, inode);
}
