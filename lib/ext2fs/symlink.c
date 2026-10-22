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

#अगर_अघोषित HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
अटल पूर्णांक my_strnlen(स्थिर अक्षर * s, पूर्णांक गिनती)
{
	स्थिर अक्षर *cp = s;

	जबतक (गिनती-- && *cp)
		cp++;
	वापस cp - s;
}
#घोषणा strnlen(str, x) my_strnlen((str),(x))
#पूर्ण_अगर

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 स्थिर अक्षर *name, स्थिर अक्षर *target)
{
	errcode_t		retval;
	काष्ठा ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	पूर्णांक			fastlink, inlinelink;
	अचिन्हित पूर्णांक		target_len;
	अक्षर			*block_buf = 0;
	पूर्णांक			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	अगर (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		जाओ cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	अगर (retval)
		जाओ cleanup;
	स_रखो(block_buf, 0, fs->blocksize);
	म_नकलन(block_buf, target, fs->blocksize);

	स_रखो(&inode, 0, माप(काष्ठा ext2_inode));
	fastlink = (target_len < माप(inode.i_block));
	अगर (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   शून्य, &blk);
		अगर (retval)
			जाओ cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	अगर (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		अगर (retval)
			जाओ cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_गिनती = 1;
	ext2fs_inode_आकार_set(fs, &inode, target_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_तथ्य(fs->super);
	अगर (fastlink) {
		/* Fast symlinks, target stored in inode */
		म_नकल((अक्षर *)&inode.i_block, target);
	} अन्यथा अगर (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_तथ्य_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		अगर (retval)
			जाओ cleanup;
		retval = ext2fs_inline_तथ्य_set(fs, ino, &inode, block_buf,
						target_len);
		अगर (retval) {
			inode.i_flags &= ~EXT4_INLINE_तथ्य_FL;
			inlinelink = 0;
			जाओ need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		अगर (retval)
			जाओ cleanup;
	} अन्यथा {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		अगर (ext2fs_has_feature_extents(fs->super)) {
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
	अगर (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	अन्यथा
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	अगर (retval)
		जाओ cleanup;

	अगर (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, शून्य, BMAP_SET, 0, शून्य,
				      &blk);
		अगर (retval)
			जाओ cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		अगर (retval)
			जाओ cleanup;
	}

	/*
	 * Update accounting....
	 */
	अगर (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	अगर (name) {
		retval = ext2fs_lookup(fs, parent, name, म_माप(name), 0,
				       &scratch_ino);
		अगर (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			जाओ cleanup;
		}
		अगर (retval != EXT2_ET_FILE_NOT_FOUND)
			जाओ cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		अगर (retval)
			जाओ cleanup;
	}
	drop_refcount = 0;

cleanup:
	अगर (block_buf)
		ext2fs_free_mem(&block_buf);
	अगर (drop_refcount) {
		अगर (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	वापस retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
पूर्णांक ext2fs_is_fast_symlink(काष्ठा ext2_inode *inode)
{
	वापस LINUX_S_ISLNK(inode->i_mode) && EXT2_I_आकार(inode) &&
	       EXT2_I_आकार(inode) < माप(inode->i_block);
}
