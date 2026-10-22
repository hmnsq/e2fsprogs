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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t खण्डम्,
				 शून्य *buf, पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	पूर्णाङ्क		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, खण्डम्, 1, buf);
	यदि (retval)
		प्रत्यागम retval;

	यदि (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_खण्डम्_csum_verify(fs, ino,
					  (संरचना ext2_dir_प्रविष्टिः *)buf))
		corrupt = 1;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	retval = ext2fs_dirent_swab_अन्तर्(fs, buf, flags);
#यदि_अन्तः
	यदि (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	प्रत्यागम retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t खण्डम्,
				 शून्य *buf, पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)))
{
	प्रत्यागम ext2fs_read_dir_block4(fs, खण्डम्, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t खण्डम्,
				 शून्य *buf, पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)))
{
	प्रत्यागम ext2fs_read_dir_block3(fs, खण्डम्, buf, flags);
}

errcode_t ext2fs_read_dir_खण्डम्(ext2_filsys fs, blk_t खण्डम्,
				 शून्य *buf)
{
	प्रत्यागम ext2fs_read_dir_block3(fs, खण्डम्, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t खण्डम्,
				  शून्य *inbuf, पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	वर्ण		*buf = inbuf;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;
	स्मृतिं_प्रतिलिख(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_बहिर्गमनम्(fs, buf, flags);
	यदि (retval)
		प्रत्यागम retval;
#यदि_अन्तः
	retval = ext2fs_dir_खण्डम्_csum_समुच्चयः(fs, ino,
					   (संरचना ext2_dir_प्रविष्टिः *)buf);
	यदि (retval)
		गच्छ बहिर्गमनम्;

	retval = io_channel_write_blk64(fs->io, खण्डम्, 1, buf);

बहिर्गमनम्:
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	ext2fs_free_mem(&buf);
#यदि_अन्तः
	प्रत्यागम retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t खण्डम्,
				  शून्य *inbuf, पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)))
{
	प्रत्यागम ext2fs_write_dir_block4(fs, खण्डम्, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t खण्डम्,
				 शून्य *inbuf, पूर्णाङ्क flags EXT2FS_ATTR((अप्रयुक्तम्)))
{
	प्रत्यागम ext2fs_write_dir_block3(fs, खण्डम्, inbuf, flags);
}

errcode_t ext2fs_write_dir_खण्डम्(ext2_filsys fs, blk_t खण्डम्,
				 शून्य *inbuf)
{
	प्रत्यागम ext2fs_write_dir_block3(fs, खण्डम्, inbuf, 0);
}

