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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساخت expand_dir_struct {
	صحیح		مکمل;
	صحیح		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

ساکن صحیح expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_بلاک EXT2FS_ATTR((غیر_مستعمل)),
			   صحیح		ref_انحراف EXT2FS_ATTR((غیر_مستعمل)),
			   خالی		*priv_ڈیٹا)
{
	ساخت expand_dir_struct *es = (ساخت expand_dir_struct *) priv_ڈیٹا;
	blk64_t	new_blk;
	حرف		*بلاک;
	errcode_t	retval;

	اگر (*blocknr) {
		اگر (blockcnt >= 0)
			es->goal = *blocknr;
		واپس 0;
	}
	اگر (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	ورنہ {
		es->goal &= ~EXT2_FS_CLUSTER_نقاب(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		اگر (retval) {
			es->err = retval;
			واپس بلاک_ABORT;
		}
		es->newblocks++;
		ext2fs_بلاک_alloc_stats2(fs, new_blk, +1);
	}
	اگر (blockcnt > 0) {
		retval = ext2fs_new_dir_بلاک(fs, 0, 0, &بلاک);
		اگر (retval) {
			es->err = retval;
			واپس بلاک_ABORT;
		}
		es->مکمل = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, بلاک, 0,
						 es->dir);
		ext2fs_free_mem(&بلاک);
	} ورنہ
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, تہی, تہی);
	اگر (blockcnt >= 0)
		es->goal = new_blk;
	اگر (retval) {
		es->err = retval;
		واپس بلاک_ABORT;
	}
	*blocknr = new_blk;

	اگر (es->مکمل)
		واپس (بلاک_بدلا | بلاک_ABORT);
	ورنہ
		واپس بلاک_بدلا;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	ساخت expand_dir_struct es;
	ساخت ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!(fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	اگر (!fs->بلاک_map)
		واپس EXT2_ET_NO_بلاک_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	اگر (retval)
		واپس retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	اگر (retval)
		واپس retval;

	es.مکمل = 0;
	es.err = 0;
	es.goal = ext2fs_ڈھونڈو_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_بلاک_iterate3(fs, dir, بلاک_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	اگر (retval == EXT2_ET_INLINE_ڈیٹا_CANT_ITERATE)
		واپس ext2fs_inline_ڈیٹا_expand(fs, dir);

	اگر (es.err)
		واپس es.err;
	اگر (!es.مکمل)
		واپس EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	اگر (retval)
		واپس retval;

	retval = ext2fs_inode_سائز_سیٹ(fs, &inode,
				       EXT2_I_سائز(&inode) + fs->blocksize);
	اگر (retval)
		واپس retval;
	ext2fs_iblk_جوڑو_بلاکیں(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	اگر (retval)
		واپس retval;

	واپس 0;
}
