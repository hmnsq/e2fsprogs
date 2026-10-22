/*
 * read_bb --- read the bad blocks inode
 *
 * Copyright (C) 1994 Theodore Ts'o.
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
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

ساخت read_bb_ریکارڈ {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
ساکن صحیح mark_bad_بلاک(ext2_filsys fs, blk_t *بلاک_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((غیر_مستعمل)),
			  blk_t ref_بلاک EXT2FS_ATTR((غیر_مستعمل)),
			  صحیح ref_انحراف EXT2FS_ATTR((غیر_مستعمل)),
			  خالی *priv_ڈیٹا)
{
	ساخت read_bb_ریکارڈ *rb = (ساخت read_bb_ریکارڈ *) priv_ڈیٹا;

	اگر (blockcnt < 0)
		واپس 0;

	اگر ((*بلاک_nr < fs->super->s_پہلا_ڈیٹا_بلاک) ||
	    (*بلاک_nr >= ext2fs_بلاکیں_گنتی(fs->super)))
		واپس 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_جوڑو(rb->bb_list, *بلاک_nr);
	اگر (rb->err)
		واپس بلاک_ABORT;
	واپس 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	ساخت read_bb_ریکارڈ rb;
	ساخت ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		اگر (retval)
			واپس retval;
		numblocks = inode.i_بلاکیں;
		اگر (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		اگر (numblocks < 50)
			numblocks = 50;
		اگر (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		اگر (retval)
			واپس retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_بلاک_iterate2(fs, EXT2_BAD_INO, بلاک_FLAG_READ_ONLY,
				       0, mark_bad_بلاک, &rb);
	اگر (retval)
		واپس retval;

	واپس rb.err;
}


