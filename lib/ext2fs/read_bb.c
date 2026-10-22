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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಸಂರಚನಾ read_bb_record {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ mark_bad_block(ext2_filsys fs, blk_t *block_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((unused)),
			  blk_t ref_block EXT2FS_ATTR((unused)),
			  ಪೂರ್ಣಾಙ್ಕ ref_offset EXT2FS_ATTR((unused)),
			  ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ read_bb_record *rb = (ಸಂರಚನಾ read_bb_record *) priv_data;

	ಯದಿ (blockcnt < 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ ((*block_nr < fs->super->s_first_data_block) ||
	    (*block_nr >= ext2fs_blocks_ಎಣಿಕೆ(fs->super)))
		ಪ್ರತ್ಯಾಗಮ 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_add(rb->bb_list, *block_nr);
	ಯದಿ (rb->err)
		ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	ಸಂರಚನಾ read_bb_record rb;
	ಸಂರಚನಾ ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		numblocks = inode.i_blocks;
		ಯದಿ (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		ಯದಿ (numblocks < 50)
			numblocks = 50;
		ಯದಿ (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO, BLOCK_FLAG_READ_ONLY,
				       0, mark_bad_block, &rb);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಪ್ರತ್ಯಾಗಮ rb.err;
}


