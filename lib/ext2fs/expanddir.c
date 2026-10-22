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
#ᱡᱩᱫᱤ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

sa_nracanaa expand_dir_struct {
	puur_naa_nka		done;
	puur_naa_nka		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

static puur_naa_nka expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   puur_naa_nka		ref_offset EXT2FS_ATTR((unused)),
			   void		*priv_data)
{
	sa_nracanaa expand_dir_struct *es = (sa_nracanaa expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	var_na		*block;
	errcode_t	retval;

	ᱡᱩᱫᱤ (*blocknr) {
		ᱡᱩᱫᱤ (blockcnt >= 0)
			es->goal = *blocknr;
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	ᱡᱩᱫᱤ (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	ᱵᱟᱝ {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		ᱡᱩᱫᱤ (retval) {
			es->err = retval;
			ᱨᱩᱣᱟᱹᱲ BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	ᱡᱩᱫᱤ (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		ᱡᱩᱫᱤ (retval) {
			es->err = retval;
			ᱨᱩᱣᱟᱹᱲ BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} ᱵᱟᱝ
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, NULL, NULL);
	ᱡᱩᱫᱤ (blockcnt >= 0)
		es->goal = new_blk;
	ᱡᱩᱫᱤ (retval) {
		es->err = retval;
		ᱨᱩᱣᱟᱹᱲ BLOCK_ABORT;
	}
	*blocknr = new_blk;

	ᱡᱩᱫᱤ (es->done)
		ᱨᱩᱣᱟᱹᱲ (BLOCK_CHANGED | BLOCK_ABORT);
	ᱵᱟᱝ
		ᱨᱩᱣᱟᱹᱲ BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	sa_nracanaa expand_dir_struct es;
	sa_nracanaa ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ᱡᱩᱫᱤ (!(fs->flags & EXT2_FLAG_RW))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_RO_FILSYS;

	ᱡᱩᱫᱤ (!fs->block_map)
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	ᱡᱩᱫᱤ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		ᱨᱩᱣᱟᱹᱲ ext2fs_inline_data_expand(fs, dir);

	ᱡᱩᱫᱤ (es.err)
		ᱨᱩᱣᱟᱹᱲ es.err;
	ᱡᱩᱫᱤ (!es.done)
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	ᱨᱩᱣᱟᱹᱲ 0;
}
