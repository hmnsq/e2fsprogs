/*
 * read_bb_file.c --- read a list of bad blocks from a FILE *
 *
 * Copyright (C) 1994, 1995, 2000 Theodore Ts'o.
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

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       శూన్య *priv_దత్తాంశం,
			       శూన్య (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       అక్షరం *badstr,
					       శూన్య *priv_దత్తాంశం))
{
	errcode_t	retval;
	గుర్తులేని దీర్ఘ దీర్ఘ blockno;
	పూర్ణాంకం		లెక్క;
	అక్షరం		buf[128];

	ఒకవేళ (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	అయితే (!feof (f)) {
		ఒకవేళ (fgets(buf, పరిమాణం(buf), f) == ఖాళీ)
			ఆపు;
		లెక్క = అక్షరశ్రేణి_చదువు(buf, "%llu", &blockno);
		ఒకవేళ (లెక్క <= 0)
			కొనసాగు;
		/* Badblocks isn't going to be updated for 64bit */
		ఒకవేళ (blockno >> 32)
			తిరిగి EOVERFLOW;
		ఒకవేళ (fs &&
		    ((blockno < fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
		     (blockno >= ext2fs_ఖండాలు_లెక్క(fs->super)))) {
			ఒకవేళ (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_దత్తాంశం);
			కొనసాగు;
		}
		retval = ext2fs_badblocks_list_కూడు(*bb_list, (blk64_t) blockno);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	తిరిగి 0;
}

నిర్మాణం compat_struct {
	శూన్య (*invalid)(ext2_filsys, blk_t);
};

స్థిర శూన్య call_compat_invalid(ext2_filsys fs, blk_t blk,
				అక్షరం *badstr EXT2FS_ATTR((వాడని)),
				శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం compat_struct *st;

	st = (నిర్మాణం compat_struct *) priv_దత్తాంశం;
	ఒకవేళ (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      శూన్య (*invalid)(ext2_filsys fs, blk_t blk))
{
	నిర్మాణం compat_struct st;

	st.invalid = invalid;

	తిరిగి ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


