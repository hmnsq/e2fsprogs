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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية expand_dir_struct {
	صحيح		done;
	صحيح		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

ساكن صحيح expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   صحيح		ref_offset EXT2FS_ATTR((unused)),
			   فراغ		*priv_data)
{
	بنية expand_dir_struct *es = (بنية expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	حرف		*block;
	errcode_t	retval;

	אם (*blocknr) {
		אם (blockcnt >= 0)
			es->goal = *blocknr;
		החזר 0;
	}
	אם (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	אחרת {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		אם (retval) {
			es->err = retval;
			החזר BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	אם (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		אם (retval) {
			es->err = retval;
			החזר BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} אחרת
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, عدم, عدم);
	אם (blockcnt >= 0)
		es->goal = new_blk;
	אם (retval) {
		es->err = retval;
		החזר BLOCK_ABORT;
	}
	*blocknr = new_blk;

	אם (es->done)
		החזר (BLOCK_CHANGED | BLOCK_ABORT);
	אחרת
		החזר BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	بنية expand_dir_struct es;
	بنية ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	אם (!(fs->flags & EXT2_FLAG_RW))
		החזר EXT2_ET_RO_FILSYS;

	אם (!fs->block_map)
		החזר EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	אם (retval)
		החזר retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	אם (retval)
		החזר retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	אם (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		החזר ext2fs_inline_data_expand(fs, dir);

	אם (es.err)
		החזר es.err;
	אם (!es.done)
		החזר EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	אם (retval)
		החזר retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	אם (retval)
		החזר retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	אם (retval)
		החזר retval;

	החזר 0;
}
