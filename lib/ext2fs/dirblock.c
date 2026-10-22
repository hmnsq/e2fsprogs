/*
 * dirblock.c --- directory block routines.
 *
 * Copyright (C) 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <time.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 व्योम *buf, पूर्णांक flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	पूर्णांक		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	अगर (retval)
		वापस retval;

	अगर (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (काष्ठा ext2_dir_entry *)buf))
		corrupt = 1;

#अगर_घोषित शब्दगण_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#पूर्ण_अगर
	अगर (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	वापस retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 व्योम *buf, पूर्णांक flags EXT2FS_ATTR((unused)))
{
	वापस ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 व्योम *buf, पूर्णांक flags EXT2FS_ATTR((unused)))
{
	वापस ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 व्योम *buf)
{
	वापस ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  व्योम *inbuf, पूर्णांक flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	अक्षर		*buf = inbuf;

#अगर_घोषित शब्दगण_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	अगर (retval)
		वापस retval;
	स_नकल(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	अगर (retval)
		वापस retval;
#पूर्ण_अगर
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (काष्ठा ext2_dir_entry *)buf);
	अगर (retval)
		जाओ out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#अगर_घोषित शब्दगण_BIGENDIAN
	ext2fs_free_mem(&buf);
#पूर्ण_अगर
	वापस retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  व्योम *inbuf, पूर्णांक flags EXT2FS_ATTR((unused)))
{
	वापस ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 व्योम *inbuf, पूर्णांक flags EXT2FS_ATTR((unused)))
{
	वापस ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 व्योम *inbuf)
{
	वापस ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

