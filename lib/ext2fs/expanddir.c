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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

నిర్మాణం expand_dir_struct {
	పూర్ణాంకం		పూర్తి;
	పూర్ణాంకం		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

స్థిర పూర్ణాంకం expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_ఖండం EXT2FS_ATTR((వాడని)),
			   పూర్ణాంకం		ref_స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
			   శూన్య		*priv_దత్తాంశం)
{
	నిర్మాణం expand_dir_struct *es = (నిర్మాణం expand_dir_struct *) priv_దత్తాంశం;
	blk64_t	new_blk;
	అక్షరం		*ఖండం;
	errcode_t	retval;

	ఒకవేళ (*blocknr) {
		ఒకవేళ (blockcnt >= 0)
			es->goal = *blocknr;
		తిరిగి 0;
	}
	ఒకవేళ (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	లేకపోతే {
		es->goal &= ~EXT2_FS_CLUSTER_ముసుగు(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		ఒకవేళ (retval) {
			es->err = retval;
			తిరిగి ఖండం_ABORT;
		}
		es->newblocks++;
		ext2fs_ఖండం_alloc_stats2(fs, new_blk, +1);
	}
	ఒకవేళ (blockcnt > 0) {
		retval = ext2fs_new_dir_ఖండం(fs, 0, 0, &ఖండం);
		ఒకవేళ (retval) {
			es->err = retval;
			తిరిగి ఖండం_ABORT;
		}
		es->పూర్తి = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, ఖండం, 0,
						 es->dir);
		ext2fs_free_mem(&ఖండం);
	} లేకపోతే
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, ఖాళీ, ఖాళీ);
	ఒకవేళ (blockcnt >= 0)
		es->goal = new_blk;
	ఒకవేళ (retval) {
		es->err = retval;
		తిరిగి ఖండం_ABORT;
	}
	*blocknr = new_blk;

	ఒకవేళ (es->పూర్తి)
		తిరిగి (ఖండం_మారింది | ఖండం_ABORT);
	లేకపోతే
		తిరిగి ఖండం_మారింది;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	నిర్మాణం expand_dir_struct es;
	నిర్మాణం ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!(fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	ఒకవేళ (!fs->ఖండం_map)
		తిరిగి EXT2_ET_NO_ఖండం_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	es.పూర్తి = 0;
	es.err = 0;
	es.goal = ext2fs_కనుగొను_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_ఖండం_iterate3(fs, dir, ఖండం_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	ఒకవేళ (retval == EXT2_ET_INLINE_దత్తాంశం_CANT_ITERATE)
		తిరిగి ext2fs_inline_దత్తాంశం_expand(fs, dir);

	ఒకవేళ (es.err)
		తిరిగి es.err;
	ఒకవేళ (!es.పూర్తి)
		తిరిగి EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = ext2fs_inode_కొలత_సమితి(fs, &inode,
				       EXT2_I_కొలత(&inode) + fs->blocksize);
	ఒకవేళ (retval)
		తిరిగి retval;
	ext2fs_iblk_కూడు_ఖండాలు(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	తిరిగి 0;
}
