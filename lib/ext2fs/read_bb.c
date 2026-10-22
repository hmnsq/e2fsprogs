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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

నిర్మాణం read_bb_రికార్డు {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
స్థిర పూర్ణాంకం mark_bad_ఖండం(ext2_filsys fs, blk_t *ఖండం_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((వాడని)),
			  blk_t ref_ఖండం EXT2FS_ATTR((వాడని)),
			  పూర్ణాంకం ref_స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
			  శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం read_bb_రికార్డు *rb = (నిర్మాణం read_bb_రికార్డు *) priv_దత్తాంశం;

	ఒకవేళ (blockcnt < 0)
		తిరిగి 0;

	ఒకవేళ ((*ఖండం_nr < fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
	    (*ఖండం_nr >= ext2fs_ఖండాలు_లెక్క(fs->super)))
		తిరిగి 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_కూడు(rb->bb_list, *ఖండం_nr);
	ఒకవేళ (rb->err)
		తిరిగి ఖండం_ABORT;
	తిరిగి 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	నిర్మాణం read_bb_రికార్డు rb;
	నిర్మాణం ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		ఒకవేళ (retval)
			తిరిగి retval;
		numblocks = inode.i_ఖండాలు;
		ఒకవేళ (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		ఒకవేళ (numblocks < 50)
			numblocks = 50;
		ఒకవేళ (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_ఖండం_iterate2(fs, EXT2_BAD_INO, ఖండం_FLAG_READ_ONLY,
				       0, mark_bad_ఖండం, &rb);
	ఒకవేళ (retval)
		తిరిగి retval;

	తిరిగి rb.err;
}


