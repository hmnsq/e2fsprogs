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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

கட்டமைப்பு read_bb_record {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
நிலையான முழு mark_bad_block(ext2_filsys fs, blk_t *block_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((unused)),
			  blk_t ref_block EXT2FS_ATTR((unused)),
			  முழு ref_offset EXT2FS_ATTR((unused)),
			  வெறுமை *priv_data)
{
	கட்டமைப்பு read_bb_record *rb = (கட்டமைப்பு read_bb_record *) priv_data;

	எனில் (blockcnt < 0)
		திரும்பு 0;

	எனில் ((*block_nr < fs->super->s_first_data_block) ||
	    (*block_nr >= ext2fs_blocks_மொத்தம்(fs->super)))
		திரும்பு 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_add(rb->bb_list, *block_nr);
	எனில் (rb->err)
		திரும்பு BLOCK_ABORT;
	திரும்பு 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	கட்டமைப்பு read_bb_record rb;
	கட்டமைப்பு ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		எனில் (retval)
			திரும்பு retval;
		numblocks = inode.i_blocks;
		எனில் (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		எனில் (numblocks < 50)
			numblocks = 50;
		எனில் (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		எனில் (retval)
			திரும்பு retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO, BLOCK_FLAG_READ_ONLY,
				       0, mark_bad_block, &rb);
	எனில் (retval)
		திரும்பு retval;

	திரும்பு rb.err;
}


