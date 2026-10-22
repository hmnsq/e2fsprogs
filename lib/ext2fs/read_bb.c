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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

काष्ठा read_bb_record {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#अगर_घोषित __TURBOC__
 #आशय argsused
#पूर्ण_अगर
अटल पूर्णांक mark_bad_block(ext2_filsys fs, blk_t *block_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((unused)),
			  blk_t ref_block EXT2FS_ATTR((unused)),
			  पूर्णांक ref_offset EXT2FS_ATTR((unused)),
			  व्योम *priv_तथ्य)
{
	काष्ठा read_bb_record *rb = (काष्ठा read_bb_record *) priv_तथ्य;

	अगर (blockcnt < 0)
		वापस 0;

	अगर ((*block_nr < fs->super->s_first_तथ्य_block) ||
	    (*block_nr >= ext2fs_blocks_गिनती(fs->super)))
		वापस 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_add(rb->bb_list, *block_nr);
	अगर (rb->err)
		वापस BLOCK_ABORT;
	वापस 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	काष्ठा read_bb_record rb;
	काष्ठा ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		अगर (retval)
			वापस retval;
		numblocks = inode.i_blocks;
		अगर (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		अगर (numblocks < 50)
			numblocks = 50;
		अगर (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		अगर (retval)
			वापस retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO, BLOCK_FLAG_READ_ONLY,
				       0, mark_bad_block, &rb);
	अगर (retval)
		वापस retval;

	वापस rb.err;
}


