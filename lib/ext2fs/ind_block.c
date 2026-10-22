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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, શૂન્ય *buf)
{
	errcode_t	retval;
#ifdef શબ્દો_BIGENDIAN
	blk_t		*block_nr;
	પૂર્ણાંક		i;
	પૂર્ણાંક		limit = fs->blocksize >> 2;
#endif

	જો ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	નહીંતર {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		જો (retval)
			પાછા retval;
	}
#ifdef શબ્દો_BIGENDIAN
	block_nr = (blk_t *) buf;
	માટે (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	પાછા 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, શૂન્ય *buf)
{
#ifdef શબ્દો_BIGENDIAN
	blk_t		*block_nr;
	પૂર્ણાંક		i;
	પૂર્ણાંક		limit = fs->blocksize >> 2;
#endif

	જો (fs->flags & EXT2_FLAG_IMAGE_FILE)
		પાછા 0;

#ifdef શબ્દો_BIGENDIAN
	block_nr = (blk_t *) buf;
	માટે (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	પાછા io_channel_write_blk(fs->io, blk, 1, buf);
}


