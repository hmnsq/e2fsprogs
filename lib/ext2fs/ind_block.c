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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, வெறுமை *buf)
{
	errcode_t	retval;
#ifdef சொற்கள்_BIGENDIAN
	blk_t		*block_nr;
	முழு		i;
	முழு		limit = fs->blocksize >> 2;
#endif

	எனில் ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	இல்லையெனில் {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		எனில் (retval)
			திரும்பு retval;
	}
#ifdef சொற்கள்_BIGENDIAN
	block_nr = (blk_t *) buf;
	க்கு (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	திரும்பு 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, வெறுமை *buf)
{
#ifdef சொற்கள்_BIGENDIAN
	blk_t		*block_nr;
	முழு		i;
	முழு		limit = fs->blocksize >> 2;
#endif

	எனில் (fs->flags & EXT2_FLAG_IMAGE_FILE)
		திரும்பு 0;

#ifdef சொற்கள்_BIGENDIAN
	block_nr = (blk_t *) buf;
	க்கு (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	திரும்பு io_channel_write_blk(fs->io, blk, 1, buf);
}


