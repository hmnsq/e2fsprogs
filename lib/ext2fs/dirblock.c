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
#جيڪڏهن_پورو
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t بلاڪ,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	صحيح		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, بلاڪ, 1, buf);
	إذا (retval)
		أرجع retval;

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_بلاڪ_csum_verify(fs, ino,
					  (بنية ext2_dir_داخلا *)buf))
		corrupt = 1;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	retval = ext2fs_dirent_swab_اندريون(fs, buf, flags);
#جيڪڏهن_پورو
	إذا (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	أرجع retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t بلاڪ,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	أرجع ext2fs_read_dir_block4(fs, بلاڪ, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t بلاڪ,
				 فراغ *buf, صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	أرجع ext2fs_read_dir_block3(fs, بلاڪ, buf, flags);
}

errcode_t ext2fs_read_dir_بلاڪ(ext2_filsys fs, blk_t بلاڪ,
				 فراغ *buf)
{
	أرجع ext2fs_read_dir_block3(fs, بلاڪ, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t بلاڪ,
				  فراغ *inbuf, صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	حرف		*buf = inbuf;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;
	يادگيري_نقل(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_ٻاهريون(fs, buf, flags);
	إذا (retval)
		أرجع retval;
#جيڪڏهن_پورو
	retval = ext2fs_dir_بلاڪ_csum_سيٽ(fs, ino,
					   (بنية ext2_dir_داخلا *)buf);
	إذا (retval)
		اذهب ٻاهريون;

	retval = io_channel_write_blk64(fs->io, بلاڪ, 1, buf);

ٻاهريون:
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	ext2fs_free_mem(&buf);
#جيڪڏهن_پورو
	أرجع retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t بلاڪ,
				  فراغ *inbuf, صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	أرجع ext2fs_write_dir_block4(fs, بلاڪ, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t بلاڪ,
				 فراغ *inbuf, صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	أرجع ext2fs_write_dir_block3(fs, بلاڪ, inbuf, flags);
}

errcode_t ext2fs_write_dir_بلاڪ(ext2_filsys fs, blk_t بلاڪ,
				 فراغ *inbuf)
{
	أرجع ext2fs_write_dir_block3(fs, بلاڪ, inbuf, 0);
}

