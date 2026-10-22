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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ my_strnlen(സ്ഥിരം അക്ഷരം * s, പൂർണ്ണസംഖ്യ എണ്ണം)
{
	സ്ഥിരം അക്ഷരം *cp = s;

	ആകുമ്പോൾ (എണ്ണം-- && *cp)
		cp++;
	തിരികെ cp - s;
}
#define strnlen(str, x) my_strnlen((str),(x))
#endif

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *target)
{
	errcode_t		retval;
	ഘടന ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	പൂർണ്ണസംഖ്യ			fastlink, inlinelink;
	unsigned പൂർണ്ണസംഖ്യ		target_len;
	അക്ഷരം			*block_buf = 0;
	പൂർണ്ണസംഖ്യ			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	എങ്കിൽ (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		പോകുക cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, വലുപ്പം(ഘടന ext2_inode));
	fastlink = (target_len < വലുപ്പം(inode.i_block));
	എങ്കിൽ (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   ശൂന്യ, &blk);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	എങ്കിൽ (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_എണ്ണം = 1;
	ext2fs_inode_size_set(fs, &inode, target_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_data(fs->super);
	എങ്കിൽ (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((അക്ഷരം *)&inode.i_block, target);
	} അല്ലെങ്കിൽ എങ്കിൽ (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		എങ്കിൽ (retval)
			പോകുക cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		എങ്കിൽ (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			പോകുക need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	} അല്ലെങ്കിൽ {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		എങ്കിൽ (ext2fs_has_feature_extents(fs->super)) {
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
	എങ്കിൽ (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	അല്ലെങ്കിൽ
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	എങ്കിൽ (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, ശൂന്യ, BMAP_SET, 0, ശൂന്യ,
				      &blk);
		എങ്കിൽ (retval)
			പോകുക cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}

	/*
	 * Update accounting....
	 */
	എങ്കിൽ (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	എങ്കിൽ (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		എങ്കിൽ (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			പോകുക cleanup;
		}
		എങ്കിൽ (retval != EXT2_ET_FILE_NOT_FOUND)
			പോകുക cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}
	drop_refcount = 0;

cleanup:
	എങ്കിൽ (block_buf)
		ext2fs_free_mem(&block_buf);
	എങ്കിൽ (drop_refcount) {
		എങ്കിൽ (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	തിരികെ retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
പൂർണ്ണസംഖ്യ ext2fs_is_fast_symlink(ഘടന ext2_inode *inode)
{
	തിരികെ LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < വലുപ്പം(inode->i_block);
}
