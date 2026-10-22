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

#include "config.h"
#include <stdio.h>
#include <string.h>
#གལ་སྲིད HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#གལ་སྲིད HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#གལ་སྲིད HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			unsigned long flags, ཧྲིལ link_flags, const var_na *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	sa_nracanaa ext2_inode	parent_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	var_na			*block = 0;
	ཧྲིལ			inline_data = 0;
	ཧྲིལ			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	གལ་སྲིད ((!ino || ino >= EXT2_FIRST_INO(fs->super)) &&
	    ext2fs_has_feature_inline_data(fs->super))
		inline_data = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	གལ་སྲིད (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFDIR | 0755,
					  0, &ino);
		གལ་སྲིད (retval)
			goto cleanup;
	}
	གལ་སྲིད (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, parimaa_na(sa_nracanaa ext2_inode));
	གལ་སྲིད (!inline_data) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   NULL, &blk);
		གལ་སྲིད (retval)
			goto cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	གལ་སྲིད (inline_data)
		retval = ext2fs_new_dir_inline_data(fs, ino, parent,
						    inode.i_block);
	དེ་མིན
		retval = ext2fs_new_dir_block(fs, ino, parent, &block);
	གལ་སྲིད (retval)
		goto cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	གལ་སྲིད (parent != ino) {
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		གལ་སྲིད (retval)
			goto cleanup;
	} དེ་མིན
		memset(&parent_inode, 0, parimaa_na(parent_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	གལ་སྲིད (inline_data) {
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	} དེ་མིན {
		གལ་སྲིད (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		དེ་མིན
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
	གལ་སྲིད (retval)
		goto cleanup;
	གལ་སྲིད (inline_data) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_data_init(fs, ino);
	} དེ་མིན {
		retval = ext2fs_write_dir_block4(fs, blk, block, 0, ino);
		གལ་སྲིད (retval)
			goto cleanup;

		གལ་སྲིད (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			གལ་སྲིད (retval)
				goto cleanup;
			retval = ext2fs_extent_set_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			གལ་སྲིད (retval)
				goto cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	གལ་སྲིད (!inline_data)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	གལ་སྲིད (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		གལ་སྲིད (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			goto cleanup;
		}
		གལ་སྲིད (retval != EXT2_ET_FILE_NOT_FOUND)
			goto cleanup;
		retval = ext2fs_link(fs, parent, name, ino,
				     EXT2_FT_DIR | link_flags);
		གལ་སྲིད (retval)
			goto cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	གལ་སྲིད (parent != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		གལ་སྲིད (retval)
			goto cleanup;
		ext2fs_inc_nlink(fs, &parent_inode);
		retval = ext2fs_write_inode(fs, parent, &parent_inode);
		གལ་སྲིད (retval)
			goto cleanup;
	}
	drop_refcount = 0;

cleanup:
	གལ་སྲིད (block)
		ext2fs_free_mem(&block);
	གལ་སྲིད (drop_refcount) {
		གལ་སྲིད (!inline_data)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	ལོག retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
		       const var_na *name)
{
	ལོག ext2fs_mkdir2(fs, parent, ino, 0, 0, name, NULL);
}
