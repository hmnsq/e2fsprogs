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
#𐤀𐤌 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#𐤀𐤌 HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#𐤀𐤌 HAVE_SYS_TYPES_H
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
	𐤀𐤌 ((!ino || ino >= EXT2_FIRST_INO(fs->super)) &&
	    ext2fs_has_feature_inline_data(fs->super))
		inline_data = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	𐤀𐤌 (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFDIR | 0755,
					  0, &ino);
		𐤀𐤌 (retval)
			اذهب cleanup;
	}
	𐤀𐤌 (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, حجم(بنية ext2_inode));
	𐤀𐤌 (!inline_data) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   عدم, &blk);
		𐤀𐤌 (retval)
			اذهب cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	𐤀𐤌 (inline_data)
		retval = ext2fs_new_dir_inline_data(fs, ino, parent,
						    inode.i_block);
	وإلا
		retval = ext2fs_new_dir_block(fs, ino, parent, &block);
	𐤀𐤌 (retval)
		اذهب cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	𐤀𐤌 (parent != ino) {
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		𐤀𐤌 (retval)
			اذهب cleanup;
	} وإلا
		memset(&parent_inode, 0, حجم(parent_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	𐤀𐤌 (inline_data) {
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	} وإلا {
		𐤀𐤌 (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		وإلا
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
	𐤀𐤌 (retval)
		اذهب cleanup;
	𐤀𐤌 (inline_data) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_data_init(fs, ino);
	} وإلا {
		retval = ext2fs_write_dir_block4(fs, blk, block, 0, ino);
		𐤀𐤌 (retval)
			اذهب cleanup;

		𐤀𐤌 (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			𐤀𐤌 (retval)
				اذهب cleanup;
			retval = ext2fs_extent_set_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			𐤀𐤌 (retval)
				اذهب cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	𐤀𐤌 (!inline_data)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	𐤀𐤌 (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		𐤀𐤌 (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			اذهب cleanup;
		}
		𐤀𐤌 (retval != EXT2_ET_FILE_NOT_FOUND)
			اذهب cleanup;
		retval = ext2fs_link(fs, parent, name, ino,
				     EXT2_FT_DIR | link_flags);
		𐤀𐤌 (retval)
			اذهب cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	𐤀𐤌 (parent != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		𐤀𐤌 (retval)
			اذهب cleanup;
		ext2fs_inc_nlink(fs, &parent_inode);
		retval = ext2fs_write_inode(fs, parent, &parent_inode);
		𐤀𐤌 (retval)
			اذهب cleanup;
	}
	drop_refcount = 0;

cleanup:
	𐤀𐤌 (block)
		ext2fs_free_mem(&block);
	𐤀𐤌 (drop_refcount) {
		𐤀𐤌 (!inline_data)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	أرجع retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
		       ثابت حرف *name)
{
	أرجع ext2fs_mkdir2(fs, parent, ino, 0, 0, name, عدم);
}
