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
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#якщо HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#якщо HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
static ціле my_strnlen(const char * s, ціле count)
{
	const char *cp = s;

	поки (count-- && *cp)
		cp++;
	повернути cp - s;
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
	ціле			fastlink, inlinelink;
	unsigned ціле		target_len;
	char			*block_buf = 0;
	ціле			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	якщо (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		goto cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	якщо (retval)
		goto cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, sizeof(struct ext2_inode));
	fastlink = (target_len < sizeof(inode.i_block));
	якщо (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   NULL, &blk);
		якщо (retval)
			goto cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	якщо (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		якщо (retval)
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
	якщо (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((char *)&inode.i_block, target);
	} інакше якщо (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		якщо (retval)
			goto cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		якщо (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			goto need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		якщо (retval)
			goto cleanup;
	} інакше {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		якщо (ext2fs_has_feature_extents(fs->super)) {
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
	якщо (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	інакше
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	якщо (retval)
		goto cleanup;

	якщо (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, NULL, BMAP_SET, 0, NULL,
				      &blk);
		якщо (retval)
			goto cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		якщо (retval)
			goto cleanup;
	}

	/*
	 * Update accounting....
	 */
	якщо (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	якщо (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		якщо (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			goto cleanup;
		}
		якщо (retval != EXT2_ET_FILE_NOT_FOUND)
			goto cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		якщо (retval)
			goto cleanup;
	}
	drop_refcount = 0;

cleanup:
	якщо (block_buf)
		ext2fs_free_mem(&block_buf);
	якщо (drop_refcount) {
		якщо (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	повернути retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
ціле ext2fs_is_fast_symlink(struct ext2_inode *inode)
{
	повернути LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < sizeof(inode->i_block);
}
