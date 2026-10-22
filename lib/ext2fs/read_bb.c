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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية read_bb_record {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
ساكن صحيح mark_bad_block(ext2_filsys fs, blk_t *block_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((unused)),
			  blk_t ref_block EXT2FS_ATTR((unused)),
			  صحيح ref_offset EXT2FS_ATTR((unused)),
			  فراغ *priv_data)
{
	بنية read_bb_record *rb = (بنية read_bb_record *) priv_data;

	إذا (blockcnt < 0)
		أرجع 0;

	إذا ((*block_nr < fs->super->s_first_data_block) ||
	    (*block_nr >= ext2fs_blocks_کيراءن(fs->super)))
		أرجع 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_add(rb->bb_list, *block_nr);
	إذا (rb->err)
		أرجع BLOCK_ABORT;
	أرجع 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	بنية read_bb_record rb;
	بنية ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		إذا (retval)
			أرجع retval;
		numblocks = inode.i_blocks;
		إذا (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		إذا (numblocks < 50)
			numblocks = 50;
		إذا (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		إذا (retval)
			أرجع retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO, BLOCK_FLAG_READ_ONLY,
				       0, mark_bad_block, &rb);
	إذا (retval)
		أرجع retval;

	أرجع rb.err;
}


