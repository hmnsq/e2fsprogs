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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સંરચના expand_dir_struct {
	પૂર્ણાંક		done;
	પૂર્ણાંક		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

સ્થિર પૂર્ણાંક expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   પૂર્ણાંક		ref_offset EXT2FS_ATTR((unused)),
			   શૂન્ય		*priv_data)
{
	સંરચના expand_dir_struct *es = (સંરચના expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	અક્ષર		*block;
	errcode_t	retval;

	જો (*blocknr) {
		જો (blockcnt >= 0)
			es->goal = *blocknr;
		પાછા 0;
	}
	જો (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	નહીંતર {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		જો (retval) {
			es->err = retval;
			પાછા BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	જો (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		જો (retval) {
			es->err = retval;
			પાછા BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} નહીંતર
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, ખાલી, ખાલી);
	જો (blockcnt >= 0)
		es->goal = new_blk;
	જો (retval) {
		es->err = retval;
		પાછા BLOCK_ABORT;
	}
	*blocknr = new_blk;

	જો (es->done)
		પાછા (BLOCK_CHANGED | BLOCK_ABORT);
	નહીંતર
		પાછા BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	સંરચના expand_dir_struct es;
	સંરચના ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (!(fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	જો (!fs->block_map)
		પાછા EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	જો (retval)
		પાછા retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	જો (retval)
		પાછા retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	જો (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		પાછા ext2fs_inline_data_expand(fs, dir);

	જો (es.err)
		પાછા es.err;
	જો (!es.done)
		પાછા EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	જો (retval)
		પાછા retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	જો (retval)
		પાછા retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	જો (retval)
		પાછા retval;

	પાછા 0;
}
