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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
স্থির পূর্ণাঙ্ক my_strnlen(ধ্রুব বর্ণ * s, পূর্ণাঙ্ক গুনতি)
{
	ধ্রুব বর্ণ *cp = s;

	যাবত্ (গুনতি-- && *cp)
		cp++;
	প্রত্যাগম cp - s;
}
#define strnlen(str, x) my_strnlen((str),(x))
#endif

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *target)
{
	errcode_t		retval;
	সংরচনা ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	পূর্ণাঙ্ক			fastlink, inlinelink;
	unsigned পূর্ণাঙ্ক		target_len;
	বর্ণ			*block_buf = 0;
	পূর্ণাঙ্ক			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	যদি (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		গচ্ছ cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	যদি (retval)
		গচ্ছ cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, পরিমাণ(সংরচনা ext2_inode));
	fastlink = (target_len < পরিমাণ(inode.i_block));
	যদি (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   রিক্ত, &blk);
		যদি (retval)
			গচ্ছ cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	যদি (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		যদি (retval)
			গচ্ছ cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_গুনতি = 1;
	ext2fs_inode_size_set(fs, &inode, target_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_data(fs->super);
	যদি (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((বর্ণ *)&inode.i_block, target);
	} অন্যথা যদি (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		যদি (retval)
			গচ্ছ cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		যদি (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			গচ্ছ need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		যদি (retval)
			গচ্ছ cleanup;
	} অন্যথা {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		যদি (ext2fs_has_feature_extents(fs->super)) {
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
	যদি (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	অন্যথা
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	যদি (retval)
		গচ্ছ cleanup;

	যদি (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, রিক্ত, BMAP_SET, 0, রিক্ত,
				      &blk);
		যদি (retval)
			গচ্ছ cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		যদি (retval)
			গচ্ছ cleanup;
	}

	/*
	 * Update accounting....
	 */
	যদি (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	যদি (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		যদি (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			গচ্ছ cleanup;
		}
		যদি (retval != EXT2_ET_FILE_NOT_FOUND)
			গচ্ছ cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		যদি (retval)
			গচ্ছ cleanup;
	}
	drop_refcount = 0;

cleanup:
	যদি (block_buf)
		ext2fs_free_mem(&block_buf);
	যদি (drop_refcount) {
		যদি (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	প্রত্যাগম retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
পূর্ণাঙ্ক ext2fs_is_fast_symlink(সংরচনা ext2_inode *inode)
{
	প্রত্যাগম LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < পরিমাণ(inode->i_block);
}
