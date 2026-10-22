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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

errcode_t ext2fs_read_ind_بلاک(ext2_filsys fs, blk_t blk, خالی *buf)
{
	errcode_t	retval;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	blk_t		*بلاک_nr;
	صحیح		i;
	صحیح		حد_بندی = fs->blocksize >> 2;
#اگر_ختم

	اگر ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	ورنہ {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		اگر (retval)
			واپس retval;
	}
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	بلاک_nr = (blk_t *) buf;
	برائے (i = 0; i < حد_بندی; i++, بلاک_nr++)
		*بلاک_nr = ext2fs_swab32(*بلاک_nr);
#اگر_ختم
	واپس 0;
}

errcode_t ext2fs_write_ind_بلاک(ext2_filsys fs, blk_t blk, خالی *buf)
{
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	blk_t		*بلاک_nr;
	صحیح		i;
	صحیح		حد_بندی = fs->blocksize >> 2;
#اگر_ختم

	اگر (fs->flags & EXT2_FLAG_IMAGE_FILE)
		واپس 0;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	بلاک_nr = (blk_t *) buf;
	برائے (i = 0; i < حد_بندی; i++, بلاک_nr++)
		*بلاک_nr = ext2fs_swab32(*بلاک_nr);
#اگر_ختم
	واپس io_channel_write_blk(fs->io, blk, 1, buf);
}


