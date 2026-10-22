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

#ifndef HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
स्थिर पूर्णाङ्क my_strnlen(ध्रुव वर्ण * s, पूर्णाङ्क गिनती)
{
	ध्रुव वर्ण *cp = s;

	यावत् (गिनती-- && *cp)
		cp++;
	प्रत्यागम cp - s;
}
#define strnlen(str, x) my_strnlen((str),(x))
#endif

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t parent, ext2_ino_t ino,
			 ध्रुव वर्ण *name, ध्रुव वर्ण *target)
{
	errcode_t		retval;
	संरचना ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	पूर्णाङ्क			fastlink, inlinelink;
	unsigned पूर्णाङ्क		target_len;
	वर्ण			*block_buf = 0;
	पूर्णाङ्क			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	target_len = strnlen(target, fs->blocksize + 1);
	यदि (target_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		गच्छ cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &block_buf);
	यदि (retval)
		गच्छ cleanup;
	memset(block_buf, 0, fs->blocksize);
	strncpy(block_buf, target, fs->blocksize);

	memset(&inode, 0, परिमाण(संरचना ext2_inode));
	fastlink = (target_len < परिमाण(inode.i_block));
	यदि (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_find_inode_goal(fs, ino,
								      &inode,
								      0),
					   रिक्त, &blk);
		यदि (retval)
			गच्छ cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	यदि (!ino) {
		retval = ext2fs_new_inode(fs, parent, LINUX_S_IFLNK | 0755,
					  0, &ino);
		यदि (retval)
			गच्छ cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_mode = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_गिनती = 1;
	ext2fs_inode_size_set(fs, &inode, target_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_data(fs->super);
	यदि (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((वर्ण *)&inode.i_block, target);
	} अन्यथा यदि (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_DATA_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		यदि (retval)
			गच्छ cleanup;
		retval = ext2fs_inline_data_set(fs, ino, &inode, block_buf,
						target_len);
		यदि (retval) {
			inode.i_flags &= ~EXT4_INLINE_DATA_FL;
			inlinelink = 0;
			गच्छ need_block;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा {
need_block:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_set(fs, &inode, 1);
		यदि (ext2fs_has_feature_extents(fs->super)) {
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
	यदि (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	अन्यथा
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	यदि (retval)
		गच्छ cleanup;

	यदि (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, रिक्त, BMAP_SET, 0, रिक्त,
				      &blk);
		यदि (retval)
			गच्छ cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, block_buf);
		यदि (retval)
			गच्छ cleanup;
	}

	/*
	 * Update accounting....
	 */
	यदि (!fastlink && !inlinelink)
		ext2fs_block_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	यदि (name) {
		retval = ext2fs_lookup(fs, parent, name, strlen(name), 0,
				       &scratch_ino);
		यदि (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			गच्छ cleanup;
		}
		यदि (retval != EXT2_ET_FILE_NOT_FOUND)
			गच्छ cleanup;
		retval = ext2fs_link(fs, parent, name, ino, EXT2_FT_SYMLINK);
		यदि (retval)
			गच्छ cleanup;
	}
	drop_refcount = 0;

cleanup:
	यदि (block_buf)
		ext2fs_free_mem(&block_buf);
	यदि (drop_refcount) {
		यदि (!fastlink && !inlinelink)
			ext2fs_block_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	प्रत्यागम retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
पूर्णाङ्क ext2fs_is_fast_symlink(संरचना ext2_inode *inode)
{
	प्रत्यागम LINUX_S_ISLNK(inode->i_mode) && EXT2_I_SIZE(inode) &&
	       EXT2_I_SIZE(inode) < परिमाण(inode->i_block);
}
