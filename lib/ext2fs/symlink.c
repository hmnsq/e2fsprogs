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

#include "config.h"
#include <stdio.h>
#include <string.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
static số_nguyên my_strnlen(const char * s, số_nguyên count)
{
	const char *cp = s;

	trong_khi (count-- && *cp)
		cp++;
	trả_về cp - s;
}
#define strnlen(str, x) my_strnlen((str),(x))
#endif

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 const char *name, const char *target)
{
	errcode_t		retval;
	struct ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	số_nguyên			fastlink, inlinelink;
	unsigned số_nguyên		target_len;
	char			*block_buf = 0;
	số_nguyên			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	nếu (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		goto cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	nếu (retval)
		goto cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, sizeof(struct ext2_inode));
	fastlink = (target_len < sizeof(inode.i_block));
	nếu (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   NULL, &blk);
		nếu (retval)
			goto cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	nếu (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		nếu (retval)
			goto cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_count = 1;
	ext2fs_inode_size_set(fs, &inode, target_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_data(fs->super);
	nếu (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((char *)&inode.i_block, target);
	} không_thì nếu (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		nếu (retval)
			goto cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		nếu (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			goto need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		nếu (retval)
			goto cleanup;
	} không_thì {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		nếu (ext2fs_has_feature_extents(fs->super)) {
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
	nếu (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	không_thì
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	nếu (retval)
		goto cleanup;

	nếu (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, NULL, BMAP_SET, 0, NULL,
				      &blk);
		nếu (retval)
			goto cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		nếu (retval)
			goto cleanup;
	}

	/*
	 * Update accounting....
	 */
	nếu (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	nếu (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		nếu (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			goto cleanup;
		}
		nếu (retval != EXT2_ET_FILE_NOT_FOUND)
			goto cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		nếu (retval)
			goto cleanup;
	}
	drop_refcount = 0;

cleanup:
	nếu (block_buf)
		ext2fs_free_mem(&block_buf);
	nếu (drop_refcount) {
		nếu (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	trả_về retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
số_nguyên ext2fs_is_fast_symlink(struct ext2_inode *inode)
{
	trả_về LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < sizeof(inode->i_block);
}
