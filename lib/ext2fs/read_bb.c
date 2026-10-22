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
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

ساختار read_bb_record {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
ایستا صحیح mark_bad_block(ext2_filsys fs, blk_t *block_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((unused)),
			  blk_t ref_block EXT2FS_ATTR((unused)),
			  صحیح ref_offset EXT2FS_ATTR((unused)),
			  تهی *priv_data)
{
	ساختار read_bb_record *rb = (ساختار read_bb_record *) priv_data;

	اگر (blockcnt < 0)
		بازگشت 0;

	اگر ((*block_nr < fs->super->s_first_data_block) ||
	    (*block_nr >= ext2fs_blocks_شمار(fs->super)))
		بازگشت 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_add(rb->bb_list, *block_nr);
	اگر (rb->err)
		بازگشت BLOCK_ABORT;
	بازگشت 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	ساختار read_bb_record rb;
	ساختار ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		اگر (retval)
			بازگشت retval;
		numblocks = inode.i_blocks;
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
			بازگشت retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO, BLOCK_FLAG_READ_ONLY,
				       0, mark_bad_block, &rb);
	اگر (retval)
		بازگشت retval;

	بازگشت rb.err;
}


