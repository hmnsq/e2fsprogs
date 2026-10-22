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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

ସଂରଚନା read_bb_record {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_bad_block(ext2_filsys fs, blk_t *block_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((unused)),
			  blk_t ref_block EXT2FS_ATTR((unused)),
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset EXT2FS_ATTR((unused)),
			  ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା read_bb_record *rb = (ସଂରଚନା read_bb_record *) priv_data;

	ଯଦି (blockcnt < 0)
		ଫେରନ୍ତୁ 0;

	ଯଦି ((*block_nr < fs->super->s_first_data_block) ||
	    (*block_nr >= ext2fs_blocks_ଗଣନା(fs->super)))
		ଫେରନ୍ତୁ 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_add(rb->bb_list, *block_nr);
	ଯଦି (rb->err)
		ଫେରନ୍ତୁ BLOCK_ABORT;
	ଫେରନ୍ତୁ 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	ସଂରଚନା read_bb_record rb;
	ସଂରଚନା ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		numblocks = inode.i_blocks;
		ଯଦି (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		ଯଦି (numblocks < 50)
			numblocks = 50;
		ଯଦି (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO, BLOCK_FLAG_READ_ONLY,
				       0, mark_bad_block, &rb);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଫେରନ୍ତୁ rb.err;
}


