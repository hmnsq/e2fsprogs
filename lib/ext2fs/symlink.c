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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
ایستا صحیح my_strnlen(ثابت نویسه * s, صحیح شمار)
{
	ثابت نویسه *cp = s;

	وقتی (شمار-- && *cp)
		cp++;
	بازگشت cp - s;
}
#define strnlen(str, x) my_strnlen((str),(x))
#endif

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 ثابت نویسه *name, ثابت نویسه *target)
{
	errcode_t		retval;
	ساختار ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	صحیح			fastlink, inlinelink;
	unsigned صحیح		target_len;
	نویسه			*block_buf = 0;
	صحیح			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	اگر (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		برو cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	اگر (retval)
		برو cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, اندازه(ساختار ext2_inode));
	fastlink = (target_len < اندازه(inode.i_block));
	اگر (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   پوچ, &blk);
		اگر (retval)
			برو cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	اگر (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		اگر (retval)
			برو cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_شمار = 1;
	ext2fs_inode_size_set(fs, &inode, target_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_data(fs->super);
	اگر (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((نویسه *)&inode.i_block, target);
	} وگرنه اگر (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		اگر (retval)
			برو cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		اگر (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			برو need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		اگر (retval)
			برو cleanup;
	} وگرنه {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		اگر (ext2fs_has_feature_extents(fs->super)) {
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
	اگر (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	وگرنه
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	اگر (retval)
		برو cleanup;

	اگر (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, پوچ, BMAP_SET, 0, پوچ,
				      &blk);
		اگر (retval)
			برو cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		اگر (retval)
			برو cleanup;
	}

	/*
	 * Update accounting....
	 */
	اگر (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	اگر (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		اگر (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			برو cleanup;
		}
		اگر (retval != EXT2_ET_FILE_NOT_FOUND)
			برو cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		اگر (retval)
			برو cleanup;
	}
	drop_refcount = 0;

cleanup:
	اگر (block_buf)
		ext2fs_free_mem(&block_buf);
	اگر (drop_refcount) {
		اگر (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	بازگشت retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
صحیح ext2fs_is_fast_symlink(ساختار ext2_inode *inode)
{
	بازگشت LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < اندازه(inode->i_block);
}
