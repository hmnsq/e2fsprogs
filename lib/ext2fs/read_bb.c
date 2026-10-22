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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᱡᱩᱫᱤ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᱡᱩᱫᱤ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᱡᱩᱫᱤ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

sa_nracanaa read_bb_record {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
static puur_naa_nka mark_bad_block(ext2_filsys fs, blk_t *block_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((unused)),
			  blk_t ref_block EXT2FS_ATTR((unused)),
			  puur_naa_nka ref_offset EXT2FS_ATTR((unused)),
			  void *priv_data)
{
	sa_nracanaa read_bb_record *rb = (sa_nracanaa read_bb_record *) priv_data;

	ᱡᱩᱫᱤ (blockcnt < 0)
		ᱨᱩᱣᱟᱹᱲ 0;

	ᱡᱩᱫᱤ ((*block_nr < fs->super->s_first_data_block) ||
	    (*block_nr >= ext2fs_blocks_count(fs->super)))
		ᱨᱩᱣᱟᱹᱲ 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_add(rb->bb_list, *block_nr);
	ᱡᱩᱫᱤ (rb->err)
		ᱨᱩᱣᱟᱹᱲ BLOCK_ABORT;
	ᱨᱩᱣᱟᱹᱲ 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	sa_nracanaa read_bb_record rb;
	sa_nracanaa ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ᱡᱩᱫᱤ (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
		numblocks = inode.i_blocks;
		ᱡᱩᱫᱤ (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		ᱡᱩᱫᱤ (numblocks < 50)
			numblocks = 50;
		ᱡᱩᱫᱤ (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO, BLOCK_FLAG_READ_ONLY,
				       0, mark_bad_block, &rb);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	ᱨᱩᱣᱟᱹᱲ rb.err;
}


