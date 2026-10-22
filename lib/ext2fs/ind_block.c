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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

errcode_t ext2fs_read_ind_ಖಂಡ(ext2_filsys fs, blk_t blk, ಶೂನ್ಯ *buf)
{
	errcode_t	retval;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	blk_t		*ಖಂಡ_nr;
	ಪೂರ್ಣಾಂಕ		i;
	ಪೂರ್ಣಾಂಕ		ಎಲ್ಲೆ = fs->blocksize >> 2;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	ಇಲ್ಲದಿದ್ದರೆ {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ಖಂಡ_nr = (blk_t *) buf;
	ಗಾಗಿ (i = 0; i < ಎಲ್ಲೆ; i++, ಖಂಡ_nr++)
		*ಖಂಡ_nr = ext2fs_swab32(*ಖಂಡ_nr);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_write_ind_ಖಂಡ(ext2_filsys fs, blk_t blk, ಶೂನ್ಯ *buf)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	blk_t		*ಖಂಡ_nr;
	ಪೂರ್ಣಾಂಕ		i;
	ಪೂರ್ಣಾಂಕ		ಎಲ್ಲೆ = fs->blocksize >> 2;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (fs->flags & EXT2_FLAG_IMAGE_FILE)
		ಹಿಂತಿರುಗಿ 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ಖಂಡ_nr = (blk_t *) buf;
	ಗಾಗಿ (i = 0; i < ಎಲ್ಲೆ; i++, ಖಂಡ_nr++)
		*ಖಂಡ_nr = ext2fs_swab32(*ಖಂಡ_nr);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ io_channel_write_blk(fs->io, blk, 1, buf);
}


