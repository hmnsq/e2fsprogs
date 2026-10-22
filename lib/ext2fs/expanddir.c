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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

கட்டமைப்பு expand_dir_struct {
	முழு		done;
	முழு		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

நிலையான முழு expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   முழு		ref_offset EXT2FS_ATTR((unused)),
			   வெறுமை		*priv_data)
{
	கட்டமைப்பு expand_dir_struct *es = (கட்டமைப்பு expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	எழுத்து		*block;
	errcode_t	retval;

	எனில் (*blocknr) {
		எனில் (blockcnt >= 0)
			es->goal = *blocknr;
		திரும்பு 0;
	}
	எனில் (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	இல்லையெனில் {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		எனில் (retval) {
			es->err = retval;
			திரும்பு BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	எனில் (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		எனில் (retval) {
			es->err = retval;
			திரும்பு BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} இல்லையெனில்
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, வெற்று, வெற்று);
	எனில் (blockcnt >= 0)
		es->goal = new_blk;
	எனில் (retval) {
		es->err = retval;
		திரும்பு BLOCK_ABORT;
	}
	*blocknr = new_blk;

	எனில் (es->done)
		திரும்பு (BLOCK_CHANGED | BLOCK_ABORT);
	இல்லையெனில்
		திரும்பு BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	கட்டமைப்பு expand_dir_struct es;
	கட்டமைப்பு ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (!(fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	எனில் (!fs->block_map)
		திரும்பு EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	எனில் (retval)
		திரும்பு retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	எனில் (retval)
		திரும்பு retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	எனில் (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		திரும்பு ext2fs_inline_data_expand(fs, dir);

	எனில் (es.err)
		திரும்பு es.err;
	எனில் (!es.done)
		திரும்பு EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	எனில் (retval)
		திரும்பு retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	எனில் (retval)
		திரும்பு retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	எனில் (retval)
		திரும்பு retval;

	திரும்பு 0;
}
