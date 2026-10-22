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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସଂରଚନା expand_dir_struct {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		done;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ref_offset EXT2FS_ATTR((unused)),
			   ଶୂନ୍ୟ		*priv_data)
{
	ସଂରଚନା expand_dir_struct *es = (ସଂରଚନା expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	ଅକ୍ଷର		*block;
	errcode_t	retval;

	ଯଦି (*blocknr) {
		ଯଦି (blockcnt >= 0)
			es->goal = *blocknr;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	ଅନ୍ୟଥା {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		ଯଦି (retval) {
			es->err = retval;
			ଫେରନ୍ତୁ BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	ଯଦି (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		ଯଦି (retval) {
			es->err = retval;
			ଫେରନ୍ତୁ BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} ଅନ୍ୟଥା
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, ଖାଲି, ଖାଲି);
	ଯଦି (blockcnt >= 0)
		es->goal = new_blk;
	ଯଦି (retval) {
		es->err = retval;
		ଫେରନ୍ତୁ BLOCK_ABORT;
	}
	*blocknr = new_blk;

	ଯଦି (es->done)
		ଫେରନ୍ତୁ (BLOCK_CHANGED | BLOCK_ABORT);
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	ସଂରଚନା expand_dir_struct es;
	ସଂରଚନା ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (!(fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	ଯଦି (!fs->block_map)
		ଫେରନ୍ତୁ EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	ଯଦି (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		ଫେରନ୍ତୁ ext2fs_inline_data_expand(fs, dir);

	ଯଦି (es.err)
		ଫେରନ୍ତୁ es.err;
	ଯଦି (!es.done)
		ଫେରନ୍ତୁ EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଫେରନ୍ତୁ 0;
}
