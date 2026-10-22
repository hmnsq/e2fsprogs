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
#གལ་ཏེ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#གལ་ཏེ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#གལ་ཏེ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
static ཧྲིལ my_strnlen(const var_na * s, ཧྲིལ count)
{
	const var_na *cp = s;

	སྐབས (count-- && *cp)
		cp++;
	ཕྱིར cp - s;
}
#define strnlen(str, x) my_strnlen((str),(x))
#endif

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 const var_na *name, const var_na *target)
{
	errcode_t		retval;
	sa_nracanaa ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	ཧྲིལ			fastlink, inlinelink;
	unsigned ཧྲིལ		target_len;
	var_na			*block_buf = 0;
	ཧྲིལ			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	གལ་ཏེ (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		goto cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	གལ་ཏེ (retval)
		goto cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, parimaa_na(sa_nracanaa ext2_inode));
	fastlink = (target_len < parimaa_na(inode.i_block));
	གལ་ཏེ (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   NULL, &blk);
		གལ་ཏེ (retval)
			goto cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	གལ་ཏེ (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		གལ་ཏེ (retval)
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
	གལ་ཏེ (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((var_na *)&inode.i_block, target);
	} དེ་མིན གལ་ཏེ (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		གལ་ཏེ (retval)
			goto cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		གལ་ཏེ (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			goto need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		གལ་ཏེ (retval)
			goto cleanup;
	} དེ་མིན {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		གལ་ཏེ (ext2fs_has_feature_extents(fs->super)) {
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
	གལ་ཏེ (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	དེ་མིན
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	གལ་ཏེ (retval)
		goto cleanup;

	གལ་ཏེ (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, NULL, BMAP_SET, 0, NULL,
				      &blk);
		གལ་ཏེ (retval)
			goto cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		གལ་ཏེ (retval)
			goto cleanup;
	}

	/*
	 * Update accounting....
	 */
	གལ་ཏེ (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	གལ་ཏེ (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		གལ་ཏེ (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			goto cleanup;
		}
		གལ་ཏེ (retval != EXT2_ET_FILE_NOT_FOUND)
			goto cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		གལ་ཏེ (retval)
			goto cleanup;
	}
	drop_refcount = 0;

cleanup:
	གལ་ཏེ (block_buf)
		ext2fs_free_mem(&block_buf);
	གལ་ཏེ (drop_refcount) {
		གལ་ཏེ (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	ཕྱིར retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
ཧྲིལ ext2fs_is_fast_symlink(sa_nracanaa ext2_inode *inode)
{
	ཕྱིར LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < parimaa_na(inode->i_block);
}
