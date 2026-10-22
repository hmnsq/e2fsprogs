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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

errcode_t ext2fs_read_ind_block(ext2_filsys fs, blk_t blk, व्योम *buf)
{
	errcode_t	retval;
#अगर_घोषित शब्दगण_BIGENDIAN
	blk_t		*block_nr;
	पूर्णांक		i;
	पूर्णांक		limit = fs->blocksize >> 2;
#पूर्ण_अगर

	अगर ((fs->flags & EXT2_FLAG_IMAGE_FILE) &&
	    (fs->io != fs->image_io))
		स_रखो(buf, 0, fs->blocksize);
	अन्यथा {
		retval = io_channel_read_blk(fs->io, blk, 1, buf);
		अगर (retval)
			वापस retval;
	}
#अगर_घोषित शब्दगण_BIGENDIAN
	block_nr = (blk_t *) buf;
	क्रम (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#पूर्ण_अगर
	वापस 0;
}

errcode_t ext2fs_write_ind_block(ext2_filsys fs, blk_t blk, व्योम *buf)
{
#अगर_घोषित शब्दगण_BIGENDIAN
	blk_t		*block_nr;
	पूर्णांक		i;
	पूर्णांक		limit = fs->blocksize >> 2;
#पूर्ण_अगर

	अगर (fs->flags & EXT2_FLAG_IMAGE_FILE)
		वापस 0;

#अगर_घोषित शब्दगण_BIGENDIAN
	block_nr = (blk_t *) buf;
	क्रम (i = 0; i < limit; i++, block_nr++)
		*block_nr = ext2fs_swab32(*block_nr);
#पूर्ण_अगर
	वापस io_channel_write_blk(fs->io, blk, 1, buf);
}


