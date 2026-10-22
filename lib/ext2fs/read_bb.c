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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना read_bb_लेखः {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
स्थिर पूर्णाङ्क mark_bad_खण्डम्(ext2_filsys fs, blk_t *खण्डम्_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((अप्रयुक्तम्)),
			  blk_t ref_खण्डम् EXT2FS_ATTR((अप्रयुक्तम्)),
			  पूर्णाङ्क ref_विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
			  शून्य *priv_दत्त)
{
	संरचना read_bb_लेखः *rb = (संरचना read_bb_लेखः *) priv_दत्त;

	यदि (blockcnt < 0)
		प्रत्यागम 0;

	यदि ((*खण्डम्_nr < fs->super->s_first_दत्त_खण्डम्) ||
	    (*खण्डम्_nr >= ext2fs_खण्डानि_गणनम्(fs->super)))
		प्रत्यागम 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_सङ्कलय(rb->bb_list, *खण्डम्_nr);
	यदि (rb->err)
		प्रत्यागम खण्डम्_ABORT;
	प्रत्यागम 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	संरचना read_bb_लेखः rb;
	संरचना ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		यदि (retval)
			प्रत्यागम retval;
		numblocks = inode.i_खण्डानि;
		यदि (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		यदि (numblocks < 50)
			numblocks = 50;
		यदि (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		यदि (retval)
			प्रत्यागम retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_खण्डम्_iterate2(fs, EXT2_BAD_INO, खण्डम्_FLAG_READ_ONLY,
				       0, mark_bad_खण्डम्, &rb);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम rb.err;
}


