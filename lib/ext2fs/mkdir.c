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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			unsigned long flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା link_flags, ଅଚଳ ଅକ୍ଷର *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	ସଂରଚନା ext2_inode	parent_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	ଅକ୍ଷର			*block = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			inline_data = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	ଯଦି ((!ino || ino >= EXT2_FIRST_INO(fs->super)) &&
	    ext2fs_has_feature_inline_data(fs->super))
		inline_data = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	ଯଦି (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFDIR | 0755,
					  0, &ino);
		ଯଦି (retval)
			ଯାଅ cleanup;
	}
	ଯଦି (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, ଆକାର(ସଂରଚନା ext2_inode));
	ଯଦି (!inline_data) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   ଖାଲି, &blk);
		ଯଦି (retval)
			ଯାଅ cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	ଯଦି (inline_data)
		retval = ext2fs_new_dir_inline_data(fs, ino, parent,
						    inode.i_block);
	ଅନ୍ୟଥା
		retval = ext2fs_new_dir_block(fs, ino, parent, &block);
	ଯଦି (retval)
		ଯାଅ cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	ଯଦି (parent != ino) {
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		ଯଦି (retval)
			ଯାଅ cleanup;
	} ଅନ୍ୟଥା
		memset(&parent_inode, 0, ଆକାର(parent_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	ଯଦି (inline_data) {
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	} ଅନ୍ୟଥା {
		ଯଦି (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		ଅନ୍ୟଥା
			inode.i_block[0] = blk;
		inode.i_size = fs->blocksize;
		ext2fs_iblk_set(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_ଗଣନା = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	ଯଦି (retval)
		ଯାଅ cleanup;
	ଯଦି (inline_data) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_data_init(fs, ino);
	} ଅନ୍ୟଥା {
		retval = ext2fs_write_dir_block4(fs, blk, block, 0, ino);
		ଯଦି (retval)
			ଯାଅ cleanup;

		ଯଦି (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			ଯଦି (retval)
				ଯାଅ cleanup;
			retval = ext2fs_extent_set_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			ଯଦି (retval)
				ଯାଅ cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	ଯଦି (!inline_data)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	ଯଦି (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		ଯଦି (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			ଯାଅ cleanup;
		}
		ଯଦି (retval != EXT2_ET_FILE_NOT_FOUND)
			ଯାଅ cleanup;
		retval = ext2fs_link(fs, parent, name, ino,
				     EXT2_FT_DIR | link_flags);
		ଯଦି (retval)
			ଯାଅ cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	ଯଦି (parent != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		ଯଦି (retval)
			ଯାଅ cleanup;
		ext2fs_inc_nlink(fs, &parent_inode);
		retval = ext2fs_write_inode(fs, parent, &parent_inode);
		ଯଦି (retval)
			ଯାଅ cleanup;
	}
	drop_refcount = 0;

cleanup:
	ଯଦି (block)
		ext2fs_free_mem(&block);
	ଯଦି (drop_refcount) {
		ଯଦି (!inline_data)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	ଫେରନ୍ତୁ retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
		       ଅଚଳ ଅକ୍ଷର *name)
{
	ଫେରନ୍ତୁ ext2fs_mkdir2(fs, parent, ino, 0, 0, name, ଖାଲି);
}
