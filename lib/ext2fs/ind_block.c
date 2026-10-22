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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t ext2fs_read_ind_खण्डम्(ext2_filsys fs, blk_t blk, शून्य *buf)
{
	errcode_t	retval;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	blk_t		*खण्डम्_nr;
	पूर्णाङ्क		i;
	पूर्णाङ्क		सीमा = fs->blocksize >> 2;
#यदि_अन्तः

	यदि ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		स्मृतिं_पूरय(buf, 0, fs->blocksize);
	अन्यथा {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		यदि (retval)
			प्रत्यागम retval;
	}
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	खण्डम्_nr = (blk_t *) buf;
	प्रति (i = 0; i < सीमा; i++, खण्डम्_nr++)
		*खण्डम्_nr = ext2fs_swab32(*खण्डम्_nr);
#यदि_अन्तः
	प्रत्यागम 0;
}

errcode_t ext2fs_write_ind_खण्डम्(ext2_filsys fs, blk_t blk, शून्य *buf)
{
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	blk_t		*खण्डम्_nr;
	पूर्णाङ्क		i;
	पूर्णाङ्क		सीमा = fs->blocksize >> 2;
#यदि_अन्तः

	यदि (fs->flags & EXT2_FLAG_IMAGE_FILE)
		प्रत्यागम 0;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	खण्डम्_nr = (blk_t *) buf;
	प्रति (i = 0; i < सीमा; i++, खण्डम्_nr++)
		*खण्डम्_nr = ext2fs_swab32(*खण्डम्_nr);
#यदि_अन्तः
	प्रत्यागम io_channel_write_blk(fs->io, blk, 1, buf);
}


