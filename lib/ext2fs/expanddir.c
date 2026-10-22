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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸಂರಚನಾ expand_dir_struct {
	ಪೂರ್ಣಾಙ್ಕ		done;
	ಪೂರ್ಣಾಙ್ಕ		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   ಪೂರ್ಣಾಙ್ಕ		ref_offset EXT2FS_ATTR((unused)),
			   ಶೂನ್ಯ		*priv_data)
{
	ಸಂರಚನಾ expand_dir_struct *es = (ಸಂರಚನಾ expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	ವರ್ಣ		*block;
	errcode_t	retval;

	ಯದಿ (*blocknr) {
		ಯದಿ (blockcnt >= 0)
			es->goal = *blocknr;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	ಅನ್ಯಥಾ {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		ಯದಿ (retval) {
			es->err = retval;
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	ಯದಿ (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		ಯದಿ (retval) {
			es->err = retval;
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} ಅನ್ಯಥಾ
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, ರಿಕ್ತ, ರಿಕ್ತ);
	ಯದಿ (blockcnt >= 0)
		es->goal = new_blk;
	ಯದಿ (retval) {
		es->err = retval;
		ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
	}
	*blocknr = new_blk;

	ಯದಿ (es->done)
		ಪ್ರತ್ಯಾಗಮ (BLOCK_CHANGED | BLOCK_ABORT);
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	ಸಂರಚನಾ expand_dir_struct es;
	ಸಂರಚನಾ ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (!(fs->flags & EXT2_FLAG_RW))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_RO_FILSYS;

	ಯದಿ (!fs->block_map)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	ಯದಿ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		ಪ್ರತ್ಯಾಗಮ ext2fs_inline_data_expand(fs, dir);

	ಯದಿ (es.err)
		ಪ್ರತ್ಯಾಗಮ es.err;
	ಯದಿ (!es.done)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಪ್ರತ್ಯಾಗಮ 0;
}
