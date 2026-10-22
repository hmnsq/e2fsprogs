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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
સ્થિર પૂર્ણાંક my_strnlen(અચલ અક્ષર * s, પૂર્ણાંક ગણતરી)
{
	અચલ અક્ષર *cp = s;

	જ્યારે (ગણતરી-- && *cp)
		cp++;
	પાછા cp - s;
}
#define strnlen(str, x) my_strnlen((str),(x))
#endif

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 અચલ અક્ષર *name, અચલ અક્ષર *target)
{
	errcode_t		retval;
	સંરચના ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	પૂર્ણાંક			fastlink, inlinelink;
	unsigned પૂર્ણાંક		target_len;
	અક્ષર			*block_buf = 0;
	પૂર્ણાંક			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	જો (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		જાઓ cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	જો (retval)
		જાઓ cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, કદ(સંરચના ext2_inode));
	fastlink = (target_len < કદ(inode.i_block));
	જો (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   ખાલી, &blk);
		જો (retval)
			જાઓ cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	જો (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		જો (retval)
			જાઓ cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_ગણતરી = 1;
	ext2fs_inode_size_set(fs, &inode, target_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_data(fs->super);
	જો (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((અક્ષર *)&inode.i_block, target);
	} નહીંતર જો (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		જો (retval)
			જાઓ cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		જો (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			જાઓ need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		જો (retval)
			જાઓ cleanup;
	} નહીંતર {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		જો (ext2fs_has_feature_extents(fs->super)) {
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
	જો (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	નહીંતર
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	જો (retval)
		જાઓ cleanup;

	જો (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, ખાલી, BMAP_SET, 0, ખાલી,
				      &blk);
		જો (retval)
			જાઓ cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		જો (retval)
			જાઓ cleanup;
	}

	/*
	 * Update accounting....
	 */
	જો (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	જો (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		જો (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			જાઓ cleanup;
		}
		જો (retval != EXT2_ET_FILE_NOT_FOUND)
			જાઓ cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		જો (retval)
			જાઓ cleanup;
	}
	drop_refcount = 0;

cleanup:
	જો (block_buf)
		ext2fs_free_mem(&block_buf);
	જો (drop_refcount) {
		જો (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	પાછા retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
પૂર્ણાંક ext2fs_is_fast_symlink(સંરચના ext2_inode *inode)
{
	પાછા LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < કદ(inode->i_block);
}
