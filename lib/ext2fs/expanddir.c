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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

sa_nracanaa expand_dir_struct {
	ចំនួនគត់		done;
	ចំនួនគត់		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

static ចំនួនគត់ expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   ចំនួនគត់		ref_offset EXT2FS_ATTR((unused)),
			   void		*priv_data)
{
	sa_nracanaa expand_dir_struct *es = (sa_nracanaa expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	var_na		*block;
	errcode_t	retval;

	ប្រសិនបើ (*blocknr) {
		ប្រសិនបើ (blockcnt >= 0)
			es->goal = *blocknr;
		ត្រឡប់ 0;
	}
	ប្រសិនបើ (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	បើមិនដូច្នេះ {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		ប្រសិនបើ (retval) {
			es->err = retval;
			ត្រឡប់ BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	ប្រសិនបើ (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		ប្រសិនបើ (retval) {
			es->err = retval;
			ត្រឡប់ BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} បើមិនដូច្នេះ
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, NULL, NULL);
	ប្រសិនបើ (blockcnt >= 0)
		es->goal = new_blk;
	ប្រសិនបើ (retval) {
		es->err = retval;
		ត្រឡប់ BLOCK_ABORT;
	}
	*blocknr = new_blk;

	ប្រសិនបើ (es->done)
		ត្រឡប់ (BLOCK_CHANGED | BLOCK_ABORT);
	បើមិនដូច្នេះ
		ត្រឡប់ BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	sa_nracanaa expand_dir_struct es;
	sa_nracanaa ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ប្រសិនបើ (!(fs->flags & EXT2_FLAG_RW))
		ត្រឡប់ EXT2_ET_RO_FILSYS;

	ប្រសិនបើ (!fs->block_map)
		ត្រឡប់ EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	ប្រសិនបើ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		ត្រឡប់ ext2fs_inline_data_expand(fs, dir);

	ប្រសិនបើ (es.err)
		ត្រឡប់ es.err;
	ប្រសិនបើ (!es.done)
		ត្រឡប់ EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	ត្រឡប់ 0;
}
