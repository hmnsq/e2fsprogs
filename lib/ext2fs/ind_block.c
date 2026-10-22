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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_read_ind_بلاک(ext2_filsys fs, blk_t blk, فراغ *buf)
{
	errcode_t	retval;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	blk_t		*بلاک_nr;
	صحيح		i;
	صحيح		حد_بندی = fs->blocksize >> 2;
#جے_انت

	إذا ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	وإلا {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		إذا (retval)
			أرجع retval;
	}
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	بلاک_nr = (blk_t *) buf;
	لكل (i = 0; i < حد_بندی; i++, بلاک_nr++)
		*بلاک_nr = ext2fs_swab32(*بلاک_nr);
#جے_انت
	أرجع 0;
}

errcode_t ext2fs_write_ind_بلاک(ext2_filsys fs, blk_t blk, فراغ *buf)
{
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	blk_t		*بلاک_nr;
	صحيح		i;
	صحيح		حد_بندی = fs->blocksize >> 2;
#جے_انت

	إذا (fs->flags & EXT2_FLAG_IMAGE_FILE)
		أرجع 0;

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	بلاک_nr = (blk_t *) buf;
	لكل (i = 0; i < حد_بندی; i++, بلاک_nr++)
		*بلاک_nr = ext2fs_swab32(*بلاک_nr);
#جے_انت
	أرجع io_channel_write_blk(fs->io, blk, 1, buf);
}


