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
#د_که_پای
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t بلاک,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((ناکارول_شوی)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	صحيح		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, بلاک, 1, buf);
	إذا (retval)
		أرجع retval;

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_بلاک_csum_verify(fs, ino,
					  (بنية ext2_dir_ثبت *)buf))
		corrupt = 1;

#که_تعریف_شوی ټکي_BIGENDIAN
	retval = ext2fs_dirent_swab_دننه(fs, buf, flags);
#د_که_پای
	إذا (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	أرجع retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t بلاک,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((ناکارول_شوی)))
{
	أرجع ext2fs_read_dir_block4(fs, بلاک, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t بلاک,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((ناکارول_شوی)))
{
	أرجع ext2fs_read_dir_block3(fs, بلاک, buf, flags);
}

errcode_t ext2fs_read_dir_بلاک(ext2_filsys fs, blk_t بلاک,
				 فراغ *buf)
{
	أرجع ext2fs_read_dir_block3(fs, بلاک, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t بلاک,
				  فراغ *inbuf, صحيح flags EXT2FS_ATTR((ناکارول_شوی)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	حرف		*buf = inbuf;

#که_تعریف_شوی ټکي_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;
	حافظه_نقل(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_بهر(fs, buf, flags);
	إذا (retval)
		أرجع retval;
#د_که_پای
	retval = ext2fs_dir_بلاک_csum_سېټ(fs, ino,
					   (بنية ext2_dir_ثبت *)buf);
	إذا (retval)
		اذهب بهر;

	retval = io_channel_write_blk64(fs->io, بلاک, 1, buf);

بهر:
#که_تعریف_شوی ټکي_BIGENDIAN
	ext2fs_free_mem(&buf);
#د_که_پای
	أرجع retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t بلاک,
				  فراغ *inbuf, صحيح flags EXT2FS_ATTR((ناکارول_شوی)))
{
	أرجع ext2fs_write_dir_block4(fs, بلاک, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t بلاک,
				 فراغ *inbuf, صحيح flags EXT2FS_ATTR((ناکارول_شوی)))
{
	أرجع ext2fs_write_dir_block3(fs, بلاک, inbuf, flags);
}

errcode_t ext2fs_write_dir_بلاک(ext2_filsys fs, blk_t بلاک,
				 فراغ *inbuf)
{
	أرجع ext2fs_write_dir_block3(fs, بلاک, inbuf, 0);
}

