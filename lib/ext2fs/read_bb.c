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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

ഘടന read_bb_record {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
സ്ഥിതം പൂർണ്ണസംഖ്യ mark_bad_block(ext2_filsys fs, blk_t *block_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((unused)),
			  blk_t ref_block EXT2FS_ATTR((unused)),
			  പൂർണ്ണസംഖ്യ ref_offset EXT2FS_ATTR((unused)),
			  ശൂന്യം *priv_data)
{
	ഘടന read_bb_record *rb = (ഘടന read_bb_record *) priv_data;

	എങ്കിൽ (blockcnt < 0)
		തിരികെ 0;

	എങ്കിൽ ((*block_nr < fs->super->s_first_data_block) ||
	    (*block_nr >= ext2fs_blocks_എണ്ണം(fs->super)))
		തിരികെ 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_add(rb->bb_list, *block_nr);
	എങ്കിൽ (rb->err)
		തിരികെ BLOCK_ABORT;
	തിരികെ 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	ഘടന read_bb_record rb;
	ഘടന ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		എങ്കിൽ (retval)
			തിരികെ retval;
		numblocks = inode.i_blocks;
		എങ്കിൽ (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		എങ്കിൽ (numblocks < 50)
			numblocks = 50;
		എങ്കിൽ (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO, BLOCK_FLAG_READ_ONLY,
				       0, mark_bad_block, &rb);
	എങ്കിൽ (retval)
		തിരികെ retval;

	തിരികെ rb.err;
}


