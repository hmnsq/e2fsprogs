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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, শূন্য *buf)
{
	errcode_t	retval;
#ifdef শব্দগুলি_BIGENDIAN
	blk_t		*block_nr;
	পূর্ণাঙ্ক		i;
	পূর্ণাঙ্ক		limit = fs->blocksize >> 2;
#endif

	যদি ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	অন্যথা {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		যদি (retval)
			ঘূৰাই retval;
	}
#ifdef শব্দগুলি_BIGENDIAN
	block_nr = (blk_t *) buf;
	বাবে (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	ঘূৰাই 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, শূন্য *buf)
{
#ifdef শব্দগুলি_BIGENDIAN
	blk_t		*block_nr;
	পূর্ণাঙ্ক		i;
	পূর্ণাঙ্ক		limit = fs->blocksize >> 2;
#endif

	যদি (fs->flags & EXT2_FLAG_IMAGE_FILE)
		ঘূৰাই 0;

#ifdef শব্দগুলি_BIGENDIAN
	block_nr = (blk_t *) buf;
	বাবে (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	ঘূৰাই io_channel_write_blk(fs->io, blk, 1, buf);
}


