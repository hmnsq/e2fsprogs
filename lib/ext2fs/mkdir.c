/*
 * mkdir.c --- make a directory in the filesystem
 *
 * Copyright (C) 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			unsigned long flags, পূর্ণাঙ্ক link_flags, ধ্রুৱ ৱর্ণ *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	সংরচনা ext2_inode	parent_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	ৱর্ণ			*block = 0;
	পূর্ণাঙ্ক			inline_data = 0;
	পূর্ণাঙ্ক			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	যদি ((!ino || ino >= EXT2_FIRST_INO(fs->super)) &&
	    ext2fs_has_feature_inline_data(fs->super))
		inline_data = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	যদি (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFDIR | 0755,
					  0, &ino);
		যদি (retval)
			গচ্ছ cleanup;
	}
	যদি (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, পরিমাণ(সংরচনা ext2_inode));
	যদি (!inline_data) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   রিক্ত, &blk);
		যদি (retval)
			গচ্ছ cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	যদি (inline_data)
		retval = ext2fs_new_dir_inline_data(fs, ino, parent,
						    inode.i_block);
	অন্যথা
		retval = ext2fs_new_dir_block(fs, ino, parent, &block);
	যদি (retval)
		গচ্ছ cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	যদি (parent != ino) {
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		যদি (retval)
			গচ্ছ cleanup;
	} অন্যথা
		memset(&parent_inode, 0, পরিমাণ(parent_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	যদি (inline_data) {
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	} অন্যথা {
		যদি (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		অন্যথা
			inode.i_block[0] = blk;
		inode.i_size = fs->blocksize;
		ext2fs_iblk_set(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_গুনতি = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	যদি (retval)
		গচ্ছ cleanup;
	যদি (inline_data) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_data_init(fs, ino);
	} অন্যথা {
		retval = ext2fs_write_dir_block4(fs, blk, block, 0, ino);
		যদি (retval)
			গচ্ছ cleanup;

		যদি (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			যদি (retval)
				গচ্ছ cleanup;
			retval = ext2fs_extent_set_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			যদি (retval)
				গচ্ছ cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	যদি (!inline_data)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	যদি (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		যদি (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			গচ্ছ cleanup;
		}
		যদি (retval != EXT2_ET_FILE_NOT_FOUND)
			গচ্ছ cleanup;
		retval = ext2fs_link(fs, parent, name, ino,
				     EXT2_FT_DIR | link_flags);
		যদি (retval)
			গচ্ছ cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	যদি (parent != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		যদি (retval)
			গচ্ছ cleanup;
		ext2fs_inc_nlink(fs, &parent_inode);
		retval = ext2fs_write_inode(fs, parent, &parent_inode);
		যদি (retval)
			গচ্ছ cleanup;
	}
	drop_refcount = 0;

cleanup:
	যদি (block)
		ext2fs_free_mem(&block);
	যদি (drop_refcount) {
		যদি (!inline_data)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	ঘূৰাই retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
		       ধ্রুৱ ৱর্ণ *name)
{
	ঘূৰাই ext2fs_mkdir2(fs, parent, ino, 0, 0, name, রিক্ত);
}
