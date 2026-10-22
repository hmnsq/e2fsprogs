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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			unsigned long flags, صحيح link_flags, ثابت حرف *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	بنية ext2_inode	parent_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	حرف			*block = 0;
	صحيح			inline_data = 0;
	صحيح			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	ܐܢ ((!ino || ino >= EXT2_FIRST_INO(fs->super)) &&
	    ext2fs_has_feature_inline_data(fs->super))
		inline_data = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	ܐܢ (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFDIR | 0755,
					  0, &ino);
		ܐܢ (retval)
			اذهب cleanup;
	}
	ܐܢ (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, حجم(بنية ext2_inode));
	ܐܢ (!inline_data) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   عدم, &blk);
		ܐܢ (retval)
			اذهب cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	ܐܢ (inline_data)
		retval = ext2fs_new_dir_inline_data(fs, ino, parent,
						    inode.i_block);
	ܐܚܪܢܐ
		retval = ext2fs_new_dir_block(fs, ino, parent, &block);
	ܐܢ (retval)
		اذهب cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	ܐܢ (parent != ino) {
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		ܐܢ (retval)
			اذهب cleanup;
	} ܐܚܪܢܐ
		memset(&parent_inode, 0, حجم(parent_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	ܐܢ (inline_data) {
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	} ܐܚܪܢܐ {
		ܐܢ (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		ܐܚܪܢܐ
			inode.i_block[0] = blk;
		inode.i_size = fs->blocksize;
		ext2fs_iblk_set(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_count = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	ܐܢ (retval)
		اذهب cleanup;
	ܐܢ (inline_data) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_data_init(fs, ino);
	} ܐܚܪܢܐ {
		retval = ext2fs_write_dir_block4(fs, blk, block, 0, ino);
		ܐܢ (retval)
			اذهب cleanup;

		ܐܢ (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			ܐܢ (retval)
				اذهب cleanup;
			retval = ext2fs_extent_set_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			ܐܢ (retval)
				اذهب cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	ܐܢ (!inline_data)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	ܐܢ (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		ܐܢ (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			اذهب cleanup;
		}
		ܐܢ (retval != EXT2_ET_FILE_NOT_FOUND)
			اذهب cleanup;
		retval = ext2fs_link(fs, parent, name, ino,
				     EXT2_FT_DIR | link_flags);
		ܐܢ (retval)
			اذهب cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	ܐܢ (parent != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		ܐܢ (retval)
			اذهب cleanup;
		ext2fs_inc_nlink(fs, &parent_inode);
		retval = ext2fs_write_inode(fs, parent, &parent_inode);
		ܐܢ (retval)
			اذهب cleanup;
	}
	drop_refcount = 0;

cleanup:
	ܐܢ (block)
		ext2fs_free_mem(&block);
	ܐܢ (drop_refcount) {
		ܐܢ (!inline_data)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	ܦܢܐ retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
		       ثابت حرف *name)
{
	ܦܢܐ ext2fs_mkdir2(fs, parent, ino, 0, 0, name, عدم);
}
