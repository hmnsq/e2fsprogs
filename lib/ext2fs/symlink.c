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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
நிலையான முழு my_strnlen(மாறிலி எழுத்து * s, முழு மொத்தம்)
{
	மாறிலி எழுத்து *cp = s;

	போது (மொத்தம்-- && *cp)
		cp++;
	திரும்பு cp - s;
}
#define strnlen(str, x) my_strnlen((str),(x))
#endif

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 மாறிலி எழுத்து *name, மாறிலி எழுத்து *target)
{
	errcode_t		retval;
	கட்டமைப்பு ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	முழு			fastlink, inlinelink;
	unsigned முழு		target_len;
	எழுத்து			*block_buf = 0;
	முழு			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	எனில் (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		செல் cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	எனில் (retval)
		செல் cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, அளவு(கட்டமைப்பு ext2_inode));
	fastlink = (target_len < அளவு(inode.i_block));
	எனில் (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   வெற்று, &blk);
		எனில் (retval)
			செல் cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	எனில் (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		எனில் (retval)
			செல் cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_மொத்தம் = 1;
	ext2fs_inode_size_set(fs, &inode, target_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_data(fs->super);
	எனில் (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((எழுத்து *)&inode.i_block, target);
	} இல்லையெனில் எனில் (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		எனில் (retval)
			செல் cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		எனில் (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			செல் need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		எனில் (retval)
			செல் cleanup;
	} இல்லையெனில் {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		எனில் (ext2fs_has_feature_extents(fs->super)) {
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
	எனில் (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	இல்லையெனில்
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	எனில் (retval)
		செல் cleanup;

	எனில் (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, வெற்று, BMAP_SET, 0, வெற்று,
				      &blk);
		எனில் (retval)
			செல் cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		எனில் (retval)
			செல் cleanup;
	}

	/*
	 * Update accounting....
	 */
	எனில் (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	எனில் (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		எனில் (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			செல் cleanup;
		}
		எனில் (retval != EXT2_ET_FILE_NOT_FOUND)
			செல் cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		எனில் (retval)
			செல் cleanup;
	}
	drop_refcount = 0;

cleanup:
	எனில் (block_buf)
		ext2fs_free_mem(&block_buf);
	எனில் (drop_refcount) {
		எனில் (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	திரும்பு retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
முழு ext2fs_is_fast_symlink(கட்டமைப்பு ext2_inode *inode)
{
	திரும்பு LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < அளவு(inode->i_block);
}
