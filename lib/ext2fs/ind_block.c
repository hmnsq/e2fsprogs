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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, ଶୂନ୍ୟ *buf)
{
	errcode_t	retval;
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	blk_t		*block_nr;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		limit = fs->blocksize >> 2;
#endif

	ଯଦି ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	ଅନ୍ୟଥା {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	block_nr = (blk_t *) buf;
	ପାଇଁ (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, ଶୂନ୍ୟ *buf)
{
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	blk_t		*block_nr;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		limit = fs->blocksize >> 2;
#endif

	ଯଦି (fs->flags & EXT2_FLAG_IMAGE_FILE)
		ଫେରନ୍ତୁ 0;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	block_nr = (blk_t *) buf;
	ପାଇଁ (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	ଫେରନ୍ତୁ io_channel_write_blk(fs->io, blk, 1, buf);
}


