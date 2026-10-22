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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t بلاک,
				 خالی *buf, صحیح flags EXT2FS_ATTR((غیر_مستعمل)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	صحیح		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, بلاک, 1, buf);
	اگر (retval)
		واپس retval;

	اگر (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_بلاک_csum_verify(fs, ino,
					  (ساخت ext2_dir_اندراج *)buf))
		corrupt = 1;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	retval = ext2fs_dirent_swab_اندر(fs, buf, flags);
#اگر_ختم
	اگر (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	واپس retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t بلاک,
				 خالی *buf, صحیح flags EXT2FS_ATTR((غیر_مستعمل)))
{
	واپس ext2fs_read_dir_block4(fs, بلاک, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t بلاک,
				 خالی *buf, صحیح flags EXT2FS_ATTR((غیر_مستعمل)))
{
	واپس ext2fs_read_dir_block3(fs, بلاک, buf, flags);
}

errcode_t ext2fs_read_dir_بلاک(ext2_filsys fs, blk_t بلاک,
				 خالی *buf)
{
	واپس ext2fs_read_dir_block3(fs, بلاک, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t بلاک,
				  خالی *inbuf, صحیح flags EXT2FS_ATTR((غیر_مستعمل)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	حرف		*buf = inbuf;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		واپس retval;
	حافظہ_نقل(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_برآمد(fs, buf, flags);
	اگر (retval)
		واپس retval;
#اگر_ختم
	retval = ext2fs_dir_بلاک_csum_سیٹ(fs, ino,
					   (ساخت ext2_dir_اندراج *)buf);
	اگر (retval)
		جاؤ برآمد;

	retval = io_channel_write_blk64(fs->io, بلاک, 1, buf);

برآمد:
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_free_mem(&buf);
#اگر_ختم
	واپس retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t بلاک,
				  خالی *inbuf, صحیح flags EXT2FS_ATTR((غیر_مستعمل)))
{
	واپس ext2fs_write_dir_block4(fs, بلاک, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t بلاک,
				 خالی *inbuf, صحیح flags EXT2FS_ATTR((غیر_مستعمل)))
{
	واپس ext2fs_write_dir_block3(fs, بلاک, inbuf, flags);
}

errcode_t ext2fs_write_dir_بلاک(ext2_filsys fs, blk_t بلاک,
				 خالی *inbuf)
{
	واپس ext2fs_write_dir_block3(fs, بلاک, inbuf, 0);
}

