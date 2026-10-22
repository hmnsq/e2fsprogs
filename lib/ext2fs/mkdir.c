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
#അന്തര്ഭാവ "ext2fsP.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			unsigned long flags, പൂർണ്ണസംഖ്യ link_flags, സ്ഥിരം അക്ഷരം *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	ഘടന ext2_inode	parent_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	അക്ഷരം			*block = 0;
	പൂർണ്ണസംഖ്യ			inline_data = 0;
	പൂർണ്ണസംഖ്യ			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	എങ്കിൽ ((!ino || ino >= EXT2_FIRST_INO(fs->super)) &&
	    ext2fs_has_feature_inline_data(fs->super))
		inline_data = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	എങ്കിൽ (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFDIR | 0755,
					  0, &ino);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}
	എങ്കിൽ (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, വലുപ്പം(ഘടന ext2_inode));
	എങ്കിൽ (!inline_data) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   ശൂന്യ, &blk);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	എങ്കിൽ (inline_data)
		retval = ext2fs_new_dir_inline_data(fs, ino, parent,
						    inode.i_block);
	അല്ലെങ്കിൽ
		retval = ext2fs_new_dir_block(fs, ino, parent, &block);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	എങ്കിൽ (parent != ino) {
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	} അല്ലെങ്കിൽ
		memset(&parent_inode, 0, വലുപ്പം(parent_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	എങ്കിൽ (inline_data) {
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		അല്ലെങ്കിൽ
			inode.i_block[0] = blk;
		inode.i_size = fs->blocksize;
		ext2fs_iblk_set(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_എണ്ണം = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	എങ്കിൽ (inline_data) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_data_init(fs, ino);
	} അല്ലെങ്കിൽ {
		retval = ext2fs_write_dir_block4(fs, blk, block, 0, ino);
		എങ്കിൽ (retval)
			പോകുക cleanup;

		എങ്കിൽ (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			എങ്കിൽ (retval)
				പോകുക cleanup;
			retval = ext2fs_extent_set_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			എങ്കിൽ (retval)
				പോകുക cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	എങ്കിൽ (!inline_data)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	എങ്കിൽ (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		എങ്കിൽ (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			പോകുക cleanup;
		}
		എങ്കിൽ (retval != EXT2_ET_FILE_NOT_FOUND)
			പോകുക cleanup;
		retval = ext2fs_link(fs, parent, name, ino,
				     EXT2_FT_DIR | link_flags);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	എങ്കിൽ (parent != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		എങ്കിൽ (retval)
			പോകുക cleanup;
		ext2fs_inc_nlink(fs, &parent_inode);
		retval = ext2fs_write_inode(fs, parent, &parent_inode);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}
	drop_refcount = 0;

cleanup:
	എങ്കിൽ (block)
		ext2fs_free_mem(&block);
	എങ്കിൽ (drop_refcount) {
		എങ്കിൽ (!inline_data)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	തിരികെ retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
		       സ്ഥിരം അക്ഷരം *name)
{
	തിരികെ ext2fs_mkdir2(fs, parent, ino, 0, 0, name, ശൂന്യ);
}
