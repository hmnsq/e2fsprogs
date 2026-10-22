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
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, शून्य *buf)
{
	errcode_t	retval;
#ifdef शब्दगण_BIGENDIAN
	blk_t		*block_nr;
	पूर्णाङ्क		i;
	पूर्णाङ्क		limit = fs->blocksize >> 2;
#endif

	यदि ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		memset(buf, 0, fs->blocksize);
	अन्यथा {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		यदि (retval)
			प्रत्यागम retval;
	}
#ifdef शब्दगण_BIGENDIAN
	block_nr = (blk_t *) buf;
	प्रति (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	प्रत्यागम 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, शून्य *buf)
{
#ifdef शब्दगण_BIGENDIAN
	blk_t		*block_nr;
	पूर्णाङ्क		i;
	पूर्णाङ्क		limit = fs->blocksize >> 2;
#endif

	यदि (fs->flags & EXT2_FLAG_IMAGE_FILE)
		प्रत्यागम 0;

#ifdef शब्दगण_BIGENDIAN
	block_nr = (blk_t *) buf;
	प्रति (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#endif
	प्रत्यागम io_channel_write_blk(fs->io, blk, 1, buf);
}


