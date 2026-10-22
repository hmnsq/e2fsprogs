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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	صحيح		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	إذا (retval)
		أرجع retval;

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (بنية ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef كلموم_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	إذا (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	أرجع retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((unused)))
{
	أرجع ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((unused)))
{
	أرجع ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 فراغ *buf)
{
	أرجع ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  فراغ *inbuf, صحيح flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	حرف		*buf = inbuf;

#ifdef كلموم_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	إذا (retval)
		أرجع retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (بنية ext2_dir_entry *)buf);
	إذا (retval)
		اذهب out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef كلموم_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	أرجع retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  فراغ *inbuf, صحيح flags EXT2FS_ATTR((unused)))
{
	أرجع ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 فراغ *inbuf, صحيح flags EXT2FS_ATTR((unused)))
{
	أرجع ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 فراغ *inbuf)
{
	أرجع ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

