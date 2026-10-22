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

#include "config.h"
#include <stdio.h>
#include <string.h>
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, void *buf)
{
	errcode_t	retval;
#ifdef WORDS_BIGENDIAN
	blk_t		*block_nr;
	ကိန်းပြည့်		i;
	ကိန်းပြည့်		limit = fs->blocksize >> 2;
#endif

	အကယ်၍ ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	မဟုတ်ပါက {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		အကယ်၍ (retval)
			ပြန်ပေး retval;
	}
#ifdef WORDS_BIGENDIAN
	block_nr = (blk_t *) buf;
	အတွက် (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	ပြန်ပေး 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, void *buf)
{
#ifdef WORDS_BIGENDIAN
	blk_t		*block_nr;
	ကိန်းပြည့်		i;
	ကိန်းပြည့်		limit = fs->blocksize >> 2;
#endif

	အကယ်၍ (fs->flags & EXT2_FLAG_IMAGE_FILE)
		ပြန်ပေး 0;

#ifdef WORDS_BIGENDIAN
	block_nr = (blk_t *) buf;
	အတွက် (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	ပြန်ပေး io_channel_write_blk(fs->io, blk, 1, buf);
}


