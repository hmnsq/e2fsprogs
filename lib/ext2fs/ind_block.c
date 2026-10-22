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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, ശൂന്യം *buf)
{
	errcode_t	retval;
#ifdef വാക്കുകൾ_BIGENDIAN
	blk_t		*block_nr;
	പൂർണ്ണസംഖ്യ		i;
	പൂർണ്ണസംഖ്യ		limit = fs->blocksize >> 2;
#endif

	എങ്കിൽ ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	അല്ലെങ്കിൽ {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
#ifdef വാക്കുകൾ_BIGENDIAN
	block_nr = (blk_t *) buf;
	വേണ്ടി (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	തിരികെ 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, ശൂന്യം *buf)
{
#ifdef വാക്കുകൾ_BIGENDIAN
	blk_t		*block_nr;
	പൂർണ്ണസംഖ്യ		i;
	പൂർണ്ണസംഖ്യ		limit = fs->blocksize >> 2;
#endif

	എങ്കിൽ (fs->flags & EXT2_FLAG_IMAGE_FILE)
		തിരികെ 0;

#ifdef വാക്കുകൾ_BIGENDIAN
	block_nr = (blk_t *) buf;
	വേണ്ടി (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	തിരികെ io_channel_write_blk(fs->io, blk, 1, buf);
}


