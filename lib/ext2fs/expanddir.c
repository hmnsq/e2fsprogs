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
#जइ HAVE_UNISTD_H
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

	जइ (*blocknr) {
		जइ (blockcnt >= 0)
			es->goal = *blocknr;
		प्रत्यागम 0;
	}
	जइ (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	अण्णहा {
		es->goal &= ~EXT2FS_CLUSTER_MASK(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		जइ (retval) {
			es->err = retval;
			प्रत्यागम BLOCK_ABORT;
		}
		es->newblocks++;
		ext2fs_block_alloc_stats2(fs, new_blk, +1);
	}
	जइ (blockcnt > 0) {
		retval = ext2fs_new_dir_block(fs, 0, 0, &block);
		जइ (retval) {
			es->err = retval;
			प्रत्यागम BLOCK_ABORT;
		}
		es->done = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, block, 0,
						 es->dir);
		ext2fs_free_mem(&block);
	} अण्णहा
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, रिक्त, रिक्त);
	जइ (blockcnt >= 0)
		es->goal = new_blk;
	जइ (retval) {
		es->err = retval;
		प्रत्यागम BLOCK_ABORT;
	}
	*blocknr = new_blk;

	जइ (es->done)
		प्रत्यागम (BLOCK_CHANGED | BLOCK_ABORT);
	अण्णहा
		प्रत्यागम BLOCK_CHANGED;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	संरचना expand_dir_struct es;
	संरचना ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जइ (!(fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	जइ (!fs->block_map)
		प्रत्यागम EXT2_ET_NO_BLOCK_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	जइ (retval)
		प्रत्यागम retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	जइ (retval)
		प्रत्यागम retval;

	es.done = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	जइ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE)
		प्रत्यागम ext2fs_inline_data_expand(fs, dir);

	जइ (es.err)
		प्रत्यागम es.err;
	जइ (!es.done)
		प्रत्यागम EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	जइ (retval)
		प्रत्यागम retval;

	retval = ext2fs_inode_size_set(fs, &inode,
				       EXT2_I_SIZE(&inode) + fs->blocksize);
	जइ (retval)
		प्रत्यागम retval;
	ext2fs_iblk_add_blocks(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	जइ (retval)
		प्रत्यागम retval;

	प्रत्यागम 0;
}
