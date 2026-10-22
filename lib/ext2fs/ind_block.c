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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

errcode_t ext2fs_read_ind_ਖੰਡ(ext2_filsys fs, blk_t blk, ਖਾਲੀ *buf)
{
	errcode_t	retval;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	blk_t		*ਖੰਡ_nr;
	ਪੂਰਨ		i;
	ਪੂਰਨ		ਹੱਦਬੰਦੀ = fs->blocksize >> 2;
#ਜੇ_ਅੰਤ

	ਜੇ ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	ਨਹੀਂਤਾਂ {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਖੰਡ_nr = (blk_t *) buf;
	ਲਈ (i = 0; i < ਹੱਦਬੰਦੀ; i++, ਖੰਡ_nr++)
		*ਖੰਡ_nr = ext2fs_swab32(*ਖੰਡ_nr);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ 0;
}

errcode_t ext2fs_write_ind_ਖੰਡ(ext2_filsys fs, blk_t blk, ਖਾਲੀ *buf)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	blk_t		*ਖੰਡ_nr;
	ਪੂਰਨ		i;
	ਪੂਰਨ		ਹੱਦਬੰਦੀ = fs->blocksize >> 2;
#ਜੇ_ਅੰਤ

	ਜੇ (fs->flags & EXT2_FLAG_IMAGE_FILE)
		ਵਾਪਸ 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਖੰਡ_nr = (blk_t *) buf;
	ਲਈ (i = 0; i < ਹੱਦਬੰਦੀ; i++, ਖੰਡ_nr++)
		*ਖੰਡ_nr = ext2fs_swab32(*ਖੰਡ_nr);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ io_channel_write_blk(fs->io, blk, 1, buf);
}


