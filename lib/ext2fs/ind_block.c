/*
 * ind_block.c --- indirect block I/O routines
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
 * 	2001, 2002, 2003, 2004, 2005 by  Theodore Ts'o.
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

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

errcode_t ext2fs_read_ind_ఖండం(ext2_filsys fs, blk_t blk, శూన్య *buf)
{
	errcode_t	retval;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	blk_t		*ఖండం_nr;
	పూర్ణాంకం		i;
	పూర్ణాంకం		హద్దు = fs->blocksize >> 2;
#అయితే_ముగింపు

	ఒకవేళ ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	లేకపోతే {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
#నిర్వచితమైతే పదాలు_BIGENDIAN
	ఖండం_nr = (blk_t *) buf;
	కొరకు (i = 0; i < హద్దు; i++, ఖండం_nr++)
		*ఖండం_nr = ext2fs_swab32(*ఖండం_nr);
#అయితే_ముగింపు
	తిరిగి 0;
}

errcode_t ext2fs_write_ind_ఖండం(ext2_filsys fs, blk_t blk, శూన్య *buf)
{
#నిర్వచితమైతే పదాలు_BIGENDIAN
	blk_t		*ఖండం_nr;
	పూర్ణాంకం		i;
	పూర్ణాంకం		హద్దు = fs->blocksize >> 2;
#అయితే_ముగింపు

	ఒకవేళ (fs->flags & EXT2_FLAG_IMAGE_FILE)
		తిరిగి 0;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	ఖండం_nr = (blk_t *) buf;
	కొరకు (i = 0; i < హద్దు; i++, ఖండం_nr++)
		*ఖండం_nr = ext2fs_swab32(*ఖండం_nr);
#అయితే_ముగింపు
	తిరిగి io_channel_write_blk(fs->io, blk, 1, buf);
}


