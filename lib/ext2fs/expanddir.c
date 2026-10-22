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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

ഘടന expand_dir_struct {
	പൂർണ്ണസംഖ്യ		done;
	പൂർണ്ണസംഖ്യ		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   പൂർണ്ണസംഖ്യ		ref_offset EXT2FS_ATTR((unused)),
			   ശൂന്യം		*priv_data)
{
	ഘടന expand_dir_struct *es = (ഘടന expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	അക്ഷരം		*block;
	errcode_t	retval;

	എങ്കിൽ (*blocknr) {
		എങ്കിൽ (blockcnt >= 0)
			es->goal = *blocknr;
		തിരികെ 0;
	}
	എങ്കിൽ (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	അല്ലെങ്കിൽ {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		എങ്കിൽ (retval) {
			es->err = retval;
			തിരികെ BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	എങ്കിൽ (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		എങ്കിൽ (retval) {
			es->err = retval;
			തിരികെ BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} അല്ലെങ്കിൽ
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, ശൂന്യ, ശൂന്യ);
	എങ്കിൽ (blockcnt >= 0)
		es->goal = new_blk;
	എങ്കിൽ (retval) {
		es->err = retval;
		തിരികെ BLOCK_ABORT;
	}
	*blocknr = new_blk;

	എങ്കിൽ (es->done)
		തിരികെ (BLOCK_CHANGED | BLOCK_ABORT);
	അല്ലെങ്കിൽ
		തിരികെ BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	ഘടന expand_dir_struct es;
	ഘടന ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (!(fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	എങ്കിൽ (!fs->block_map)
		തിരികെ EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	എങ്കിൽ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		തിരികെ ext2fs_inline_data_expand(fs, dir);

	എങ്കിൽ (es.err)
		തിരികെ es.err;
	എങ്കിൽ (!es.done)
		തിരികെ EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	എങ്കിൽ (retval)
		തിരികെ retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	തിരികെ 0;
}
