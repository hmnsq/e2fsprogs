/*
 * expand.c --- expand an ext2fs directory
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997, 1998, 1999  Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना expand_dir_struct {
	पूर्णाङ्क		done;
	पूर्णाङ्क		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

स्थिर पूर्णाङ्क expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_block EXT2FS_ATTR((unused)),
			   पूर्णाङ्क		ref_offset EXT2FS_ATTR((unused)),
			   शून्य		*priv_data)
{
	संरचना expand_dir_struct *es = (संरचना expand_dir_struct *) priv_data;
	blk64_t	new_blk;
	वर्ण		*block;
	errcode_t	retval;

	सचे (*blocknr) {
		सचे (blockcnt >= 0)
			es->goal = *blocknr;
		पटिनिवत्त 0;
	}
	सचे (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	अञ्ञथा {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		सचे (retval) {
			es->err = retval;
			पटिनिवत्त BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	सचे (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		सचे (retval) {
			es->err = retval;
			पटिनिवत्त BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} अञ्ञथा
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, रिक्त, रिक्त);
	सचे (blockcnt >= 0)
		es->goal = new_blk;
	सचे (retval) {
		es->err = retval;
		पटिनिवत्त BLOCK_ABORT;
	}
	*blocknr = new_blk;

	सचे (es->done)
		पटिनिवत्त (BLOCK_CHANGED | BLOCK_ABORT);
	अञ्ञथा
		पटिनिवत्त BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	संरचना expand_dir_struct es;
	संरचना ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	सचे (!(fs->flags & EXT2_FLAG_RW))
		पटिनिवत्त EXT2_ET_RO_FILSYS;

	सचे (!fs->block_map)
		पटिनिवत्त EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	सचे (retval)
		पटिनिवत्त retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	सचे (retval)
		पटिनिवत्त retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	सचे (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		पटिनिवत्त ext2fs_inline_data_expand(fs, dir);

	सचे (es.err)
		पटिनिवत्त es.err;
	सचे (!es.done)
		पटिनिवत्त EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	सचे (retval)
		पटिनिवत्त retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	सचे (retval)
		पटिनिवत्त retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	सचे (retval)
		पटिनिवत्त retval;

	पटिनिवत्त 0;
}
