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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना expand_dir_struct {
	पूर्णाङ्क		समाप्तम्;
	पूर्णाङ्क		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

स्थिर पूर्णाङ्क expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_खण्डम् EXT2FS_ATTR((अप्रयुक्तम्)),
			   पूर्णाङ्क		ref_विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
			   शून्य		*priv_दत्त)
{
	संरचना expand_dir_struct *es = (संरचना expand_dir_struct *) priv_दत्त;
	blk64_t	new_blk;
	वर्ण		*खण्डम्;
	errcode_t	retval;

	यदि (*blocknr) {
		यदि (blockcnt >= 0)
			es->goal = *blocknr;
		प्रत्यागम 0;
	}
	यदि (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	अन्यथा {
		es->goal &= ~EXT2_FS_CLUSTER_आवरणम्(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		यदि (retval) {
			es->err = retval;
			प्रत्यागम खण्डम्_ABORT;
		}
		es->newblocks++;
		ext2fs_खण्डम्_alloc_stats2(fs, new_blk, +1);
	}
	यदि (blockcnt > 0) {
		retval = ext2fs_new_dir_खण्डम्(fs, 0, 0, &खण्डम्);
		यदि (retval) {
			es->err = retval;
			प्रत्यागम खण्डम्_ABORT;
		}
		es->समाप्तम् = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, खण्डम्, 0,
						 es->dir);
		ext2fs_free_mem(&खण्डम्);
	} अन्यथा
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, रिक्त, रिक्त);
	यदि (blockcnt >= 0)
		es->goal = new_blk;
	यदि (retval) {
		es->err = retval;
		प्रत्यागम खण्डम्_ABORT;
	}
	*blocknr = new_blk;

	यदि (es->समाप्तम्)
		प्रत्यागम (खण्डम्_परिवर्तितम् | खण्डम्_ABORT);
	अन्यथा
		प्रत्यागम खण्डम्_परिवर्तितम्;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	संरचना expand_dir_struct es;
	संरचना ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!(fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	यदि (!fs->खण्डम्_map)
		प्रत्यागम EXT2_ET_NO_खण्डम्_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	यदि (retval)
		प्रत्यागम retval;

	es.समाप्तम् = 0;
	es.err = 0;
	es.goal = ext2fs_find_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_खण्डम्_iterate3(fs, dir, खण्डम्_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	यदि (retval == EXT2_ET_INLINE_दत्त_CANT_ITERATE)
		प्रत्यागम ext2fs_inline_दत्त_expand(fs, dir);

	यदि (es.err)
		प्रत्यागम es.err;
	यदि (!es.समाप्तम्)
		प्रत्यागम EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_inode_आकार_समुच्चयः(fs, &inode,
				       EXT2_I_आकार(&inode) + fs->blocksize);
	यदि (retval)
		प्रत्यागम retval;
	ext2fs_iblk_सङ्कलय_खण्डानि(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम 0;
}
