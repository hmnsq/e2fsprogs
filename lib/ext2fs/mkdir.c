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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

#अगर_अघोषित EXT2_FT_DIR
#घोषणा EXT2_FT_DIR		2
#पूर्ण_अगर

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			अचिन्हित दीर्घ flags, पूर्णांक link_flags, स्थिर अक्षर *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	काष्ठा ext2_inode	parent_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	अक्षर			*block = 0;
	पूर्णांक			inline_तथ्य = 0;
	पूर्णांक			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	अगर ((!ino || ino >= EXT2_FIRST_INO(fs->super)) &&
	    ext2fs_has_feature_inline_तथ्य(fs->super))
		inline_तथ्य = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	अगर (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFDIR | 0755,
					  0, &ino);
		अगर (retval)
			जाओ cleanup;
	}
	अगर (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	स_रखो(&inode, 0, माप(काष्ठा ext2_inode));
	अगर (!inline_तथ्य) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   शून्य, &blk);
		अगर (retval)
			जाओ cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	अगर (inline_तथ्य)
		retval = ext2fs_new_dir_inline_तथ्य(fs, ino, parent,
						    inode.i_block);
	अन्यथा
		retval = ext2fs_new_dir_block(fs, ino, parent, &block);
	अगर (retval)
		जाओ cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	अगर (parent != ino) {
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		अगर (retval)
			जाओ cleanup;
	} अन्यथा
		स_रखो(&parent_inode, 0, माप(parent_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	अगर (inline_तथ्य) {
		inode.i_flags |= EXT4_INLINE_तथ्य_FL;
		inode.i_आकार = EXT4_MIN_INLINE_तथ्य_आकार;
	} अन्यथा {
		अगर (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		अन्यथा
			inode.i_block[0] = blk;
		inode.i_आकार = fs->blocksize;
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
	अगर (retval)
		जाओ cleanup;
	अगर (inline_तथ्य) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_तथ्य_init(fs, ino);
	} अन्यथा {
		retval = ext2fs_write_dir_block4(fs, blk, block, 0, ino);
		अगर (retval)
			जाओ cleanup;

		अगर (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			अगर (retval)
				जाओ cleanup;
			retval = ext2fs_extent_set_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			अगर (retval)
				जाओ cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	अगर (!inline_तथ्य)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	अगर (name) {
		retval = ext2fs_lookup(fs, parent, name, म_माप(name), 0,
				       &scratch_ino);
		अगर (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			जाओ cleanup;
		}
		अगर (retval != EXT2_ET_FILE_NOT_FOUND)
			जाओ cleanup;
		retval = ext2fs_link(fs, parent, name, ino,
				     EXT2_FT_DIR | link_flags);
		अगर (retval)
			जाओ cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	अगर (parent != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, parent, &parent_inode);
		अगर (retval)
			जाओ cleanup;
		ext2fs_inc_nlink(fs, &parent_inode);
		retval = ext2fs_write_inode(fs, parent, &parent_inode);
		अगर (retval)
			जाओ cleanup;
	}
	drop_refcount = 0;

cleanup:
	अगर (block)
		ext2fs_free_mem(&block);
	अगर (drop_refcount) {
		अगर (!inline_तथ्य)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	वापस retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
		       स्थिर अक्षर *name)
{
	वापस ext2fs_mkdir2(fs, parent, ino, 0, 0, name, शून्य);
}
