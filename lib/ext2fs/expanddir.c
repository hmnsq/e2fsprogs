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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਬਣਤਰ expand_dir_struct {
	ਪੂਰਨ		ਪੂਰਾ;
	ਪੂਰਨ		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

ਸਥਿਰ ਪੂਰਨ expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_ਖੰਡ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			   ਪੂਰਨ		ref_ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			   ਖਾਲੀ		*priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ expand_dir_struct *es = (ਬਣਤਰ expand_dir_struct *) priv_ਅੰਕੜੇ;
	blk64_t	new_blk;
	ਅੱਖਰ		*ਖੰਡ;
	errcode_t	retval;

	ਜੇ (*blocknr) {
		ਜੇ (blockcnt >= 0)
			es->goal = *blocknr;
		ਵਾਪਸ 0;
	}
	ਜੇ (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	ਨਹੀਂਤਾਂ {
		es->goal &= ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		ਜੇ (retval) {
			es->err = retval;
			ਵਾਪਸ ਖੰਡ_ABORT;
		}
		es->newblocks++;
		ext2fs_ਖੰਡ_alloc_stats2(fs, new_blk, +1);
	}
	ਜੇ (blockcnt > 0) {
		retval = ext2fs_new_dir_ਖੰਡ(fs, 0, 0, &ਖੰਡ);
		ਜੇ (retval) {
			es->err = retval;
			ਵਾਪਸ ਖੰਡ_ABORT;
		}
		es->ਪੂਰਾ = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, ਖੰਡ, 0,
						 es->dir);
		ext2fs_free_mem(&ਖੰਡ);
	} ਨਹੀਂਤਾਂ
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, ਨਿਰਾ, ਨਿਰਾ);
	ਜੇ (blockcnt >= 0)
		es->goal = new_blk;
	ਜੇ (retval) {
		es->err = retval;
		ਵਾਪਸ ਖੰਡ_ABORT;
	}
	*blocknr = new_blk;

	ਜੇ (es->ਪੂਰਾ)
		ਵਾਪਸ (ਖੰਡ_ਬਦਲਿਆ | ਖੰਡ_ABORT);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ ਖੰਡ_ਬਦਲਿਆ;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	ਬਣਤਰ expand_dir_struct es;
	ਬਣਤਰ ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!(fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	ਜੇ (!fs->ਖੰਡ_map)
		ਵਾਪਸ EXT2_ET_NO_ਖੰਡ_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	es.ਪੂਰਾ = 0;
	es.err = 0;
	es.goal = ext2fs_ਲੱਭੋ_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_ਖੰਡ_iterate3(fs, dir, ਖੰਡ_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	ਜੇ (retval == EXT2_ET_INLINE_ਅੰਕੜੇ_CANT_ITERATE)
		ਵਾਪਸ ext2fs_inline_ਅੰਕੜੇ_expand(fs, dir);

	ਜੇ (es.err)
		ਵਾਪਸ es.err;
	ਜੇ (!es.ਪੂਰਾ)
		ਵਾਪਸ EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = ext2fs_inode_ਮਾਪ_ਸੈੱਟ(fs, &inode,
				       EXT2_I_ਮਾਪ(&inode) + fs->blocksize);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਵਾਪਸ 0;
}
