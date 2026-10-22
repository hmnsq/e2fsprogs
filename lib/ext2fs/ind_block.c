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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, ಶೂನ್ಯ *buf)
{
	errcode_t	retval;
#ifdef ಪದಗಳು_BIGENDIAN
	blk_t		*block_nr;
	ಪೂರ್ಣಾಙ್ಕ		i;
	ಪೂರ್ಣಾಙ್ಕ		limit = fs->blocksize >> 2;
#endif

	ಯದಿ ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	ಅನ್ಯಥಾ {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
#ifdef ಪದಗಳು_BIGENDIAN
	block_nr = (blk_t *) buf;
	ಪ್ರತಿ (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, ಶೂನ್ಯ *buf)
{
#ifdef ಪದಗಳು_BIGENDIAN
	blk_t		*block_nr;
	ಪೂರ್ಣಾಙ್ಕ		i;
	ಪೂರ್ಣಾಙ್ಕ		limit = fs->blocksize >> 2;
#endif

	ಯದಿ (fs->flags & EXT2_FLAG_IMAGE_FILE)
		ಪ್ರತ್ಯಾಗಮ 0;

#ifdef ಪದಗಳು_BIGENDIAN
	block_nr = (blk_t *) buf;
	ಪ್ರತಿ (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	ಪ್ರತ್ಯಾಗಮ io_channel_write_blk(fs->io, blk, 1, buf);
}


