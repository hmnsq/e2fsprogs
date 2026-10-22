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
#پایان_اگر
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t بلوک,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((استفادهنشده)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	صحيح		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, بلوک, 1, buf);
	إذا (retval)
		أرجع retval;

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_بلوک_csum_verify(fs, ino,
					  (بنية ext2_dir_مدخل *)buf))
		corrupt = 1;

#اگر_تعریف_شده واژگان_BIGENDIAN
	retval = ext2fs_dirent_swab_درون(fs, buf, flags);
#پایان_اگر
	إذا (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	أرجع retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t بلوک,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((استفادهنشده)))
{
	أرجع ext2fs_read_dir_block4(fs, بلوک, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t بلوک,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((استفادهنشده)))
{
	أرجع ext2fs_read_dir_block3(fs, بلوک, buf, flags);
}

errcode_t ext2fs_read_dir_بلوک(ext2_filsys fs, blk_t بلوک,
				 فراغ *buf)
{
	أرجع ext2fs_read_dir_block3(fs, بلوک, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t بلوک,
				  فراغ *inbuf, صحيح flags EXT2FS_ATTR((استفادهنشده)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	حرف		*buf = inbuf;

#اگر_تعریف_شده واژگان_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;
	کپی_حافظه(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_بیرون(fs, buf, flags);
	إذا (retval)
		أرجع retval;
#پایان_اگر
	retval = ext2fs_dir_بلوک_csum_مجموعه(fs, ino,
					   (بنية ext2_dir_مدخل *)buf);
	إذا (retval)
		اذهب بیرون;

	retval = io_channel_write_blk64(fs->io, بلوک, 1, buf);

بیرون:
#اگر_تعریف_شده واژگان_BIGENDIAN
	ext2fs_free_mem(&buf);
#پایان_اگر
	أرجع retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t بلوک,
				  فراغ *inbuf, صحيح flags EXT2FS_ATTR((استفادهنشده)))
{
	أرجع ext2fs_write_dir_block4(fs, بلوک, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t بلوک,
				 فراغ *inbuf, صحيح flags EXT2FS_ATTR((استفادهنشده)))
{
	أرجع ext2fs_write_dir_block3(fs, بلوک, inbuf, flags);
}

errcode_t ext2fs_write_dir_بلوک(ext2_filsys fs, blk_t بلوک,
				 فراغ *inbuf)
{
	أرجع ext2fs_write_dir_block3(fs, بلوک, inbuf, 0);
}

