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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

কাঠামো expand_dir_struct {
	পূর্ণসংখ্যা		done;
	পূর্ণসংখ্যা		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

স্থির পূর্ণসংখ্যা expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   পূর্ণসংখ্যা		ref_offset EXT2FS_ATTR((unused)),
			   শূন্য		*priv_তথ্য)
{
	কাঠামো expand_dir_struct *es = (কাঠামো expand_dir_struct *) priv_তথ্য;
	blk64_t	new_blk;
	অক্ষর		*block;
	errcode_t	retval;

	যদি (*blocknr) {
		যদি (blockcnt >= 0)
			es->goal = *blocknr;
		ফেরত 0;
	}
	যদি (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	অন্যথা {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		যদি (retval) {
			es->err = retval;
			ফেরত BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	যদি (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		যদি (retval) {
			es->err = retval;
			ফেরত BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} অন্যথা
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, ফাঁকা, ফাঁকা);
	যদি (blockcnt >= 0)
		es->goal = new_blk;
	যদি (retval) {
		es->err = retval;
		ফেরত BLOCK_ABORT;
	}
	*blocknr = new_blk;

	যদি (es->done)
		ফেরত (BLOCK_CHANGED | BLOCK_ABORT);
	অন্যথা
		ফেরত BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	কাঠামো expand_dir_struct es;
	কাঠামো ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!(fs->flags & EXT2_FLAG_RW))
		ফেরত EXT2_ET_RO_FILSYS;

	যদি (!fs->block_map)
		ফেরত EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	যদি (retval)
		ফেরত retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	যদি (retval)
		ফেরত retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	যদি (retval == EXT2_ET_INLINE_তথ্য_CANT_ITERATE)
		ফেরত ext2fs_inline_তথ্য_expand(fs, dir);

	যদি (es.err)
		ফেরত es.err;
	যদি (!es.done)
		ফেরত EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	যদি (retval)
		ফেরত retval;

	retval = ext2fs_inode_আকার_set(fs, &inode,
				       EXT2_I_আকার(&inode) + fs->blocksize);
	যদি (retval)
		ফেরত retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	যদি (retval)
		ফেরত retval;

	ফেরত 0;
}
