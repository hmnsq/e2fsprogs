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
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, تهی *buf)
{
	errcode_t	retval;
#ifdef واژگان_BIGENDIAN
	blk_t		*block_nr;
	صحیح		i;
	صحیح		limit = fs->blocksize >> 2;
#endif

	اگر ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	وگرنه {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		اگر (retval)
			بازگشت retval;
	}
#ifdef واژگان_BIGENDIAN
	block_nr = (blk_t *) buf;
	برای (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	بازگشت 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, تهی *buf)
{
#ifdef واژگان_BIGENDIAN
	blk_t		*block_nr;
	صحیح		i;
	صحیح		limit = fs->blocksize >> 2;
#endif

	اگر (fs->flags & EXT2_FLAG_IMAGE_FILE)
		بازگشت 0;

#ifdef واژگان_BIGENDIAN
	block_nr = (blk_t *) buf;
	برای (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	بازگشت io_channel_write_blk(fs->io, blk, 1, buf);
}


