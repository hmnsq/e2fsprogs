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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

সংরচনা read_bb_record {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণাঙ্ক mark_bad_block(ext2_filsys fs, blk_t *block_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((unused)),
			  blk_t ref_block EXT2FS_ATTR((unused)),
			  পূর্ণাঙ্ক ref_offset EXT2FS_ATTR((unused)),
			  শূন্য *priv_data)
{
	সংরচনা read_bb_record *rb = (সংরচনা read_bb_record *) priv_data;

	যদি (blockcnt < 0)
		প্রত্যাগম 0;

	যদি ((*block_nr < fs->super->s_first_data_block) ||
	    (*block_nr >= ext2fs_blocks_গুনতি(fs->super)))
		প্রত্যাগম 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_add(rb->bb_list, *block_nr);
	যদি (rb->err)
		প্রত্যাগম BLOCK_ABORT;
	প্রত্যাগম 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	সংরচনা read_bb_record rb;
	সংরচনা ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		যদি (retval)
			প্রত্যাগম retval;
		numblocks = inode.i_blocks;
		যদি (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		যদি (numblocks < 50)
			numblocks = 50;
		যদি (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		যদি (retval)
			প্রত্যাগম retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO, BLOCK_FLAG_READ_ONLY,
				       0, mark_bad_block, &rb);
	যদি (retval)
		প্রত্যাগম retval;

	প্রত্যাগম rb.err;
}


