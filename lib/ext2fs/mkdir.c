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
#شامل "ext2fsP.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			unsigned long flags, صحیح link_flags, ثابت نویسه *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	ساختار ext2_inode	parent_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	نویسه			*block = 0;
	صحیح			inline_data = 0;
	صحیح			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	اگر ((!ino || ino >= EXT2_FIRST_INO(fs->super)) &&
	    ext2fs_has_feature_inline_data(fs->super))
		inline_data = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	اگر (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFDIR | 0755,
					  0, &ino);
		اگر (retval)
			برو cleanup;
	}
	اگر (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, اندازه(ساختار ext2_inode));
	اگر (!inline_data) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   پوچ, &blk);
		اگر (retval)
			برو cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	اگر (inline_data)
		retval = ext2fs_new_dir_inline_data(fs, ino, parent,
						    inode.i_block);
	وگرنه
		retval = ext2fs_new_dir_block(fs, ino, parent, &block);
	اگر (retval)
		برو cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	اگر (parent != ino) {
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		اگر (retval)
			برو cleanup;
	} وگرنه
		memset(&parent_inode, 0, اندازه(parent_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	اگر (inline_data) {
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	} وگرنه {
		اگر (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		وگرنه
			inode.i_block[0] = blk;
		inode.i_size = fs->blocksize;
		ext2fs_iblk_set(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_شمار = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	اگر (retval)
		برو cleanup;
	اگر (inline_data) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_data_init(fs, ino);
	} وگرنه {
		retval = ext2fs_write_dir_block4(fs, blk, block, 0, ino);
		اگر (retval)
			برو cleanup;

		اگر (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			اگر (retval)
				برو cleanup;
			retval = ext2fs_extent_set_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			اگر (retval)
				برو cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	اگر (!inline_data)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	اگر (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		اگر (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			برو cleanup;
		}
		اگر (retval != EXT2_ET_FILE_NOT_FOUND)
			برو cleanup;
		retval = ext2fs_link(fs, parent, name, ino,
				     EXT2_FT_DIR | link_flags);
		اگر (retval)
			برو cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	اگر (parent != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		اگر (retval)
			برو cleanup;
		ext2fs_inc_nlink(fs, &parent_inode);
		retval = ext2fs_write_inode(fs, parent, &parent_inode);
		اگر (retval)
			برو cleanup;
	}
	drop_refcount = 0;

cleanup:
	اگر (block)
		ext2fs_free_mem(&block);
	اگر (drop_refcount) {
		اگر (!inline_data)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	بازگشت retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
		       ثابت نویسه *name)
{
	بازگشت ext2fs_mkdir2(fs, parent, ino, 0, 0, name, پوچ);
}
