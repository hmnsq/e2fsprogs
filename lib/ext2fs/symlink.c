/*
 * symlink.c --- make a symlink in the filesystem, based on mkdir.c
 *
 * Copyright (c) 2012, Intel Corporation.
 * All Rights Reserved.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ my_strnlen(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * s, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp = s;

	ಆಗ (ಎಣಿಕೆ_2-- && *cp)
		cp++;
	ಹಿಂತಿರುಗಿ cp - s;
}
#ವ್ಯಾಖ್ಯಾನಿಸು strnlen(str, x) my_strnlen((str),(x))
#ಆದರೆ_ಅಂತ್ಯ

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t ಪೋಷಕ, ext2_ino_t ino,
			 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ)
{
	errcode_t		retval;
	ರಚನೆ ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	ಪೂರ್ಣಾಂಕ			fastlink, inlinelink;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ಗುರಿ_len;
	ಅಕ್ಷರ			*ಖಂಡ_buf = 0;
	ಪೂರ್ಣಾಂಕ			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	ಗುರಿ_len = strnlen(ಗುರಿ, fs->blocksize + 1);
	ಆದರೆ (ಗುರಿ_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ವಾದ;
		ಹೋಗು cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &ಖಂಡ_buf);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	memset(ಖಂಡ_buf, 0, fs->blocksize);
	strncpy(ಖಂಡ_buf, ಗುರಿ, fs->blocksize);

	memset(&inode, 0, ಗಾತ್ರ(ರಚನೆ ext2_inode));
	fastlink = (ಗುರಿ_len < ಗಾತ್ರ(inode.i_ಖಂಡ));
	ಆದರೆ (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(fs, ino,
								      &inode,
								      0),
					   ಖಾಲಿ, &blk);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	ಆದರೆ (!ino) {
		retval = ext2fs_new_inode(fs, ಪೋಷಕ, LINUX_S_IFLNK | 0755,
					  0, &ino);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_ಬಹುಲಕ = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_ಎಣಿಕೆ = 1;
	ext2fs_inode_ಗಾತ್ರ_ಗಣ(fs, &inode, ಗುರಿ_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_ದತ್ತಾಂಶ(fs->super);
	ಆದರೆ (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((ಅಕ್ಷರ *)&inode.i_ಖಂಡ, ಗುರಿ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_ದತ್ತಾಂಶ_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
		retval = ext2fs_inline_ದತ್ತಾಂಶ_ಗಣ(fs, ino, &inode, ಖಂಡ_buf,
						ಗುರಿ_len);
		ಆದರೆ (retval) {
			inode.i_flags &= ~EXT4_INLINE_ದತ್ತಾಂಶ_FL;
			inlinelink = 0;
			ಹೋಗು need_ಖಂಡ;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	} ಇಲ್ಲದಿದ್ದರೆ {
need_ಖಂಡ:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_ಗಣ(fs, &inode, 1);
		ಆದರೆ (ext2fs_has_feature_extents(fs->super)) {
			/*
			 * The extent bmap is setup after the inode and block
			 * have been written out below.
			 */
			inode.i_flags |= EXT4_EXTENTS_FL;
		}
	}

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the
	 * operations using ino must come after it.
	 */
	ಆದರೆ (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	ಇಲ್ಲದಿದ್ದರೆ
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	ಆದರೆ (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, ಖಾಲಿ, BMAP_ಗಣ, 0, ಖಾಲಿ,
				      &blk);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, ಖಂಡ_buf);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}

	/*
	 * Update accounting....
	 */
	ಆದರೆ (!fastlink && !inlinelink)
		ext2fs_ಖಂಡ_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	ಆದರೆ (name) {
		retval = ext2fs_lookup(fs, ಪೋಷಕ, name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name), 0,
				       &scratch_ino);
		ಆದರೆ (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			ಹೋಗು cleanup;
		}
		ಆದರೆ (retval != EXT2_ET_FILE_NOT_ಸಿಕ್ಕಿತು)
			ಹೋಗು cleanup;
		retval = ext2fs_link(fs, ಪೋಷಕ, name, ino, EXT2_FT_SYMLINK);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}
	drop_refcount = 0;

cleanup:
	ಆದರೆ (ಖಂಡ_buf)
		ext2fs_free_mem(&ಖಂಡ_buf);
	ಆದರೆ (drop_refcount) {
		ಆದರೆ (!fastlink && !inlinelink)
			ext2fs_ಖಂಡ_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
ಪೂರ್ಣಾಂಕ ext2fs_is_fast_symlink(ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ LINUX_S_ISLNK(inode->i_ಬಹುಲಕ) && EXT2_I_ಗಾತ್ರ(inode) &&
	       EXT2_I_ಗಾತ್ರ(inode) < ಗಾತ್ರ(inode->i_ಖಂಡ);
}
