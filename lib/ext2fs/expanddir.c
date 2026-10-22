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
#ܐܢ HAVE_UNISTD_H
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

	ܐܢ (*blocknr) {
		ܐܢ (blockcnt >= 0)
			es->goal = *blocknr;
		ܦܢܐ 0;
	}
	ܐܢ (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	ܐܚܪܢܐ {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		ܐܢ (retval) {
			es->err = retval;
			ܦܢܐ BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	ܐܢ (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		ܐܢ (retval) {
			es->err = retval;
			ܦܢܐ BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} ܐܚܪܢܐ
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, عدم, عدم);
	ܐܢ (blockcnt >= 0)
		es->goal = new_blk;
	ܐܢ (retval) {
		es->err = retval;
		ܦܢܐ BLOCK_ABORT;
	}
	*blocknr = new_blk;

	ܐܢ (es->done)
		ܦܢܐ (BLOCK_CHANGED | BLOCK_ABORT);
	ܐܚܪܢܐ
		ܦܢܐ BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	بنية expand_dir_struct es;
	بنية ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ܐܢ (!(fs->flags & EXT2_FLAG_RW))
		ܦܢܐ EXT2_ET_RO_FILSYS;

	ܐܢ (!fs->block_map)
		ܦܢܐ EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	ܐܢ (retval)
		ܦܢܐ retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ܐܢ (retval)
		ܦܢܐ retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	ܐܢ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		ܦܢܐ ext2fs_inline_data_expand(fs, dir);

	ܐܢ (es.err)
		ܦܢܐ es.err;
	ܐܢ (!es.done)
		ܦܢܐ EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	ܐܢ (retval)
		ܦܢܐ retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	ܐܢ (retval)
		ܦܢܐ retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	ܐܢ (retval)
		ܦܢܐ retval;

	ܦܢܐ 0;
}
