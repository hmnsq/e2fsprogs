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
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية expand_dir_struct {
	صحيح		بشپړ;
	صحيح		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

ساكن صحيح expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_بلاک EXT2FS_ATTR((ناکارول_شوی)),
			   صحيح		ref_دننه_واټن EXT2FS_ATTR((ناکارول_شوی)),
			   فراغ		*priv_معلومات)
{
	بنية expand_dir_struct *es = (بنية expand_dir_struct *) priv_معلومات;
	blk64_t	new_blk;
	حرف		*بلاک;
	errcode_t	retval;

	إذا (*blocknr) {
		إذا (blockcnt >= 0)
			es->goal = *blocknr;
		أرجع 0;
	}
	إذا (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	وإلا {
		es->goal &= ~EXT2_FS_CLUSTER_ماسک(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		إذا (retval) {
			es->err = retval;
			أرجع بلاک_ABORT;
		}
		es->newblocks++;
		ext2fs_بلاک_alloc_stats2(fs, new_blk, +1);
	}
	إذا (blockcnt > 0) {
		retval = ext2fs_new_dir_بلاک(fs, 0, 0, &بلاک);
		إذا (retval) {
			es->err = retval;
			أرجع بلاک_ABORT;
		}
		es->بشپړ = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, بلاک, 0,
						 es->dir);
		ext2fs_free_mem(&بلاک);
	} وإلا
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, عدم, عدم);
	إذا (blockcnt >= 0)
		es->goal = new_blk;
	إذا (retval) {
		es->err = retval;
		أرجع بلاک_ABORT;
	}
	*blocknr = new_blk;

	إذا (es->بشپړ)
		أرجع (بلاک_بدل_شوی | بلاک_ABORT);
	وإلا
		أرجع بلاک_بدل_شوی;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	بنية expand_dir_struct es;
	بنية ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!fs->بلاک_map)
		أرجع EXT2_ET_NO_بلاک_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	es.بشپړ = 0;
	es.err = 0;
	es.goal = ext2fs_پیدا_کړه_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_بلاک_iterate3(fs, dir, بلاک_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	إذا (retval == EXT2_ET_INLINE_معلومات_CANT_ITERATE)
		أرجع ext2fs_inline_معلومات_expand(fs, dir);

	إذا (es.err)
		أرجع es.err;
	إذا (!es.بشپړ)
		أرجع EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_inode_اندازه_سېټ(fs, &inode,
				       EXT2_I_اندازه(&inode) + fs->blocksize);
	إذا (retval)
		أرجع retval;
	ext2fs_iblk_جمع_کړه_بلاکونه(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	أرجع 0;
}
