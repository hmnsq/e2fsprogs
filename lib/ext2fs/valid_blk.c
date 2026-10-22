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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

/*
 * This function returns 1 if the inode's block entries actually
 * contain block entries.
 */
ਪੂਰਨ ext2fs_inode_has_valid_blocks2(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode)
{
	/*
	 * Only directories, regular files, and some symbolic links
	 * have valid block entries.
	 */
	ਜੇ (!LINUX_S_ISDIR(inode->i_ਬਹੁਲਕ) && !LINUX_S_ISREG(inode->i_ਬਹੁਲਕ) &&
	    !LINUX_S_ISLNK(inode->i_ਬਹੁਲਕ))
		ਵਾਪਸ 0;

	/*
	 * If the symbolic link is a "fast symlink", then the symlink
	 * target is stored in the block entries.
	 */
	ਜੇ (LINUX_S_ISLNK (inode->i_ਬਹੁਲਕ)) {
		ਜੇ (ext2fs_file_acl_ਖੰਡ(fs, inode) == 0) {
			/* With no EA block, we can rely on i_blocks */
			ਜੇ (inode->i_ਖੰਡਾਂ == 0)
				ਵਾਪਸ 0;
			ਵਾਪਸ !ext2fs_is_fast_symlink(inode);
		} ਨਹੀਂਤਾਂ {
			/* With an EA block, life gets more tricky */
			ਜੇ (inode->i_ਮਾਪ >= EXT2_N_ਖੰਡਾਂ*4)
				ਵਾਪਸ 1; /* definitely using i_block[] */
			ਜੇ (inode->i_ਮਾਪ > 4 && inode->i_ਖੰਡ[1] == 0)
				ਵਾਪਸ 1; /* definitely using i_block[] */
			ਵਾਪਸ 0; /* Probably a fast symlink */
		}
	}

	/*
	 * If this inode has inline data, it shouldn't have valid block
	 * entries.
	 */
	ਜੇ (inode->i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL)
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

ਪੂਰਨ ext2fs_inode_has_valid_ਖੰਡਾਂ(ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ ext2fs_inode_has_valid_blocks2(ਨਿਰਾ, inode);
}
