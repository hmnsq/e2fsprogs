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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ my_strnlen(ಧ್ರುವ ವರ್ಣ * s, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)
{
	ಧ್ರುವ ವರ್ಣ *cp = s;

	ಯಾವತ್ (ಎಣಿಕೆ-- && *cp)
		cp++;
	ಪ್ರತ್ಯಾಗಮ cp - s;
}
#define strnlen(str, x) my_strnlen((str),(x))
#endif

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *target)
{
	errcode_t		retval;
	ಸಂರಚನಾ ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	ಪೂರ್ಣಾಙ್ಕ			fastlink, inlinelink;
	unsigned ಪೂರ್ಣಾಙ್ಕ		target_len;
	ವರ್ಣ			*block_buf = 0;
	ಪೂರ್ಣಾಙ್ಕ			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	ಯದಿ (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		ಗಚ್ಛ cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode));
	fastlink = (target_len < ಪರಿಮಾಣ(inode.i_block));
	ಯದಿ (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   ರಿಕ್ತ, &blk);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	ಯದಿ (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_ಎಣಿಕೆ = 1;
	ext2fs_inode_size_set(fs, &inode, target_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_data(fs->super);
	ಯದಿ (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((ವರ್ಣ *)&inode.i_block, target);
	} ಅನ್ಯಥಾ ಯದಿ (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		ಯದಿ (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			ಗಚ್ಛ need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	} ಅನ್ಯಥಾ {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		ಯದಿ (ext2fs_has_feature_extents(fs->super)) {
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
	ಯದಿ (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	ಅನ್ಯಥಾ
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	ಯದಿ (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, ರಿಕ್ತ, BMAP_SET, 0, ರಿಕ್ತ,
				      &blk);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	}

	/*
	 * Update accounting....
	 */
	ಯದಿ (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	ಯದಿ (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		ಯದಿ (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			ಗಚ್ಛ cleanup;
		}
		ಯದಿ (retval != EXT2_ET_FILE_NOT_FOUND)
			ಗಚ್ಛ cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	}
	drop_refcount = 0;

cleanup:
	ಯದಿ (block_buf)
		ext2fs_free_mem(&block_buf);
	ಯದಿ (drop_refcount) {
		ಯದಿ (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
ಪೂರ್ಣಾಙ್ಕ ext2fs_is_fast_symlink(ಸಂರಚನಾ ext2_inode *inode)
{
	ಪ್ರತ್ಯಾಗಮ LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < ಪರಿಮಾಣ(inode->i_block);
}
