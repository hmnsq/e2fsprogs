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
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية expand_dir_struct {
	صحيح		پورو;
	صحيح		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

ساكن صحيح expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_بلاڪ EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			   صحيح		ref_وٿي EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			   فراغ		*priv_معلومات)
{
	بنية expand_dir_struct *es = (بنية expand_dir_struct *) priv_معلومات;
	blk64_t	new_blk;
	حرف		*بلاڪ;
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
		es->goal &= ~EXT2_FS_CLUSTER_نقاب(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		إذا (retval) {
			es->err = retval;
			أرجع بلاڪ_ABORT;
		}
		es->newblocks++;
		ext2fs_بلاڪ_alloc_stats2(fs, new_blk, +1);
	}
	إذا (blockcnt > 0) {
		retval = ext2fs_new_dir_بلاڪ(fs, 0, 0, &بلاڪ);
		إذا (retval) {
			es->err = retval;
			أرجع بلاڪ_ABORT;
		}
		es->پورو = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, بلاڪ, 0,
						 es->dir);
		ext2fs_free_mem(&بلاڪ);
	} وإلا
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, عدم, عدم);
	إذا (blockcnt >= 0)
		es->goal = new_blk;
	إذا (retval) {
		es->err = retval;
		أرجع بلاڪ_ABORT;
	}
	*blocknr = new_blk;

	إذا (es->پورو)
		أرجع (بلاڪ_بدليو | بلاڪ_ABORT);
	وإلا
		أرجع بلاڪ_بدليو;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	بنية expand_dir_struct es;
	بنية ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	إذا (!fs->بلاڪ_map)
		أرجع EXT2_ET_NO_بلاڪ_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	es.پورو = 0;
	es.err = 0;
	es.goal = ext2fs_ڳوليو_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_بلاڪ_iterate3(fs, dir, بلاڪ_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	إذا (retval == EXT2_ET_INLINE_معلومات_CANT_ITERATE)
		أرجع ext2fs_inline_معلومات_expand(fs, dir);

	إذا (es.err)
		أرجع es.err;
	إذا (!es.پورو)
		أرجع EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_inode_سائيز_سيٽ(fs, &inode,
				       EXT2_I_سائيز(&inode) + fs->blocksize);
	إذا (retval)
		أرجع retval;
	ext2fs_iblk_جوڙيو_بلاڪون(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	أرجع 0;
}
