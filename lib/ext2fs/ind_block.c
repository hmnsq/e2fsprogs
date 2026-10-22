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
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_read_ind_بلاڪ(ext2_filsys fs, blk_t blk, فراغ *buf)
{
	errcode_t	retval;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	blk_t		*بلاڪ_nr;
	صحيح		i;
	صحيح		حد_بندي = fs->blocksize >> 2;
#جيڪڏهن_پورو

	إذا ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	وإلا {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		إذا (retval)
			أرجع retval;
	}
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	بلاڪ_nr = (blk_t *) buf;
	لكل (i = 0; i < حد_بندي; i++, بلاڪ_nr++)
		*بلاڪ_nr = ext2fs_swab32(*بلاڪ_nr);
#جيڪڏهن_پورو
	أرجع 0;
}

errcode_t ext2fs_write_ind_بلاڪ(ext2_filsys fs, blk_t blk, فراغ *buf)
{
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	blk_t		*بلاڪ_nr;
	صحيح		i;
	صحيح		حد_بندي = fs->blocksize >> 2;
#جيڪڏهن_پورو

	إذا (fs->flags & EXT2_FLAG_IMAGE_FILE)
		أرجع 0;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	بلاڪ_nr = (blk_t *) buf;
	لكل (i = 0; i < حد_بندي; i++, بلاڪ_nr++)
		*بلاڪ_nr = ext2fs_swab32(*بلاڪ_nr);
#جيڪڏهن_پورو
	أرجع io_channel_write_blk(fs->io, blk, 1, buf);
}


