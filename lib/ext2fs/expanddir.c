/*
 * expand.c --- expand an ext2fs directory
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997, 1998, 1999  Theodore Ts'o.
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

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

काष्ठा expand_dir_struct {
	पूर्णांक		done;
	पूर्णांक		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

अटल पूर्णांक expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   पूर्णांक		ref_offset EXT2FS_ATTR((unused)),
			   व्योम		*priv_तथ्य)
{
	काष्ठा expand_dir_struct *es = (काष्ठा expand_dir_struct *) priv_तथ्य;
	blk64_t	new_blk;
	अक्षर		*block;
	errcode_t	retval;

	अगर (*blocknr) {
		अगर (blockcnt >= 0)
			es->goal = *blocknr;
		वापस 0;
	}
	अगर (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	अन्यथा {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		अगर (retval) {
			es->err = retval;
			वापस BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	अगर (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		अगर (retval) {
			es->err = retval;
			वापस BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} अन्यथा
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, शून्य, शून्य);
	अगर (blockcnt >= 0)
		es->goal = new_blk;
	अगर (retval) {
		es->err = retval;
		वापस BLOCK_ABORT;
	}
	*blocknr = new_blk;

	अगर (es->done)
		वापस (BLOCK_CHANGED | BLOCK_ABORT);
	अन्यथा
		वापस BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	काष्ठा expand_dir_struct es;
	काष्ठा ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (!(fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	अगर (!fs->block_map)
		वापस EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	अगर (retval)
		वापस retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	अगर (retval)
		वापस retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	अगर (retval == EXT2_ET_INLINE_तथ्य_CANT_ITERATE)
		वापस ext2fs_inline_तथ्य_expand(fs, dir);

	अगर (es.err)
		वापस es.err;
	अगर (!es.done)
		वापस EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	अगर (retval)
		वापस retval;

	retval = ext2fs_inode_आकार_set(fs, &inode,
				       EXT2_I_आकार(&inode) + fs->blocksize);
	अगर (retval)
		वापस retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	अगर (retval)
		वापस retval;

	वापस 0;
}
