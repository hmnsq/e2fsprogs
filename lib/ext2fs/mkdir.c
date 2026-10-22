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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			अचिन्हित दीर्घ flags, पूर्णाङ्क link_flags, ध्रुव वर्ण *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	संरचना ext2_inode	parent_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	वर्ण			*block = 0;
	पूर्णाङ्क			inline_data = 0;
	पूर्णाङ्क			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	यदि ((!ino || ino >= EXT2_FIRST_INO(fs->super)) &&
	    ext2fs_has_feature_inline_data(fs->super))
		inline_data = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	यदि (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFDIR | 0755,
					  0, &ino);
		यदि (retval)
			गच्छ cleanup;
	}
	यदि (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	स_रखो(&inode, 0, परिमाण(संरचना ext2_inode));
	यदि (!inline_data) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   रिक्त, &blk);
		यदि (retval)
			गच्छ cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	यदि (inline_data)
		retval = ext2fs_new_dir_inline_data(fs, ino, parent,
						    inode.i_block);
	अन्यथा
		retval = ext2fs_new_dir_block(fs, ino, parent, &block);
	यदि (retval)
		गच्छ cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	यदि (parent != ino) {
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा
		स_रखो(&parent_inode, 0, परिमाण(parent_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	यदि (inline_data) {
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	} अन्यथा {
		यदि (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		अन्यथा
			inode.i_block[0] = blk;
		inode.i_size = fs->blocksize;
		ext2fs_iblk_set(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_गिनती = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	यदि (retval)
		गच्छ cleanup;
	यदि (inline_data) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_data_init(fs, ino);
	} अन्यथा {
		retval = ext2fs_write_dir_block4(fs, blk, block, 0, ino);
		यदि (retval)
			गच्छ cleanup;

		यदि (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			यदि (retval)
				गच्छ cleanup;
			retval = ext2fs_extent_set_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			यदि (retval)
				गच्छ cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	यदि (!inline_data)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	यदि (name) {
		retval = ext2fs_lookup(fs, parent, name, म_माप(name), 0,
				       &scratch_ino);
		यदि (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			गच्छ cleanup;
		}
		यदि (retval != EXT2_ET_FILE_NOT_FOUND)
			गच्छ cleanup;
		retval = ext2fs_link(fs, parent, name, ino,
				     EXT2_FT_DIR | link_flags);
		यदि (retval)
			गच्छ cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	यदि (parent != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		यदि (retval)
			गच्छ cleanup;
		ext2fs_inc_nlink(fs, &parent_inode);
		retval = ext2fs_write_inode(fs, parent, &parent_inode);
		यदि (retval)
			गच्छ cleanup;
	}
	drop_refcount = 0;

cleanup:
	यदि (block)
		ext2fs_free_mem(&block);
	यदि (drop_refcount) {
		यदि (!inline_data)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	प्रत्यागम retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
		       ध्रुव वर्ण *name)
{
	प्रत्यागम ext2fs_mkdir2(fs, parent, ino, 0, 0, name, रिक्त);
}
