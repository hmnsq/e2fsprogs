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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساختار expand_dir_struct {
	صحیح		done;
	صحیح		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

ایستا صحیح expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   صحیح		ref_offset EXT2FS_ATTR((unused)),
			   تهی		*priv_data)
{
	ساختار expand_dir_struct *es = (ساختار expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	نویسه		*block;
	errcode_t	retval;

	اگر (*blocknr) {
		اگر (blockcnt >= 0)
			es->goal = *blocknr;
		بازگشت 0;
	}
	اگر (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	وگرنه {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		اگر (retval) {
			es->err = retval;
			بازگشت BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	اگر (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		اگر (retval) {
			es->err = retval;
			بازگشت BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} وگرنه
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, پوچ, پوچ);
	اگر (blockcnt >= 0)
		es->goal = new_blk;
	اگر (retval) {
		es->err = retval;
		بازگشت BLOCK_ABORT;
	}
	*blocknr = new_blk;

	اگر (es->done)
		بازگشت (BLOCK_CHANGED | BLOCK_ABORT);
	وگرنه
		بازگشت BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	ساختار expand_dir_struct es;
	ساختار ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!(fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	اگر (!fs->block_map)
		بازگشت EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	اگر (retval)
		بازگشت retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	اگر (retval)
		بازگشت retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	اگر (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		بازگشت ext2fs_inline_data_expand(fs, dir);

	اگر (es.err)
		بازگشت es.err;
	اگر (!es.done)
		بازگشت EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	اگر (retval)
		بازگشت retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	اگر (retval)
		بازگشت retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	اگر (retval)
		بازگشت retval;

	بازگشت 0;
}
