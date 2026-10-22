/*
 * newdir.c --- create a new directory block
 *
 * Copyright (C) 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#اگر_غیر_تعریف_شدہ EXT2_FT_DIR
#تعریف_کرو EXT2_FT_DIR		2
#اگر_ختم

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_بلاک(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t والد_ino, حرف **بلاک)
{
	ساخت ext2_dir_اندراج 	*dir = تہی;
	errcode_t		retval;
	حرف			*buf;
	صحیح			rec_len;
	صحیح			filetype = 0;
	ساخت ext2_dir_اندراج_دم	*t;
	صحیح			csum_سائز = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		واپس retval;
	memset(buf, 0, fs->blocksize);
	dir = (ساخت ext2_dir_اندراج *) buf;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائز = حجم(ساخت ext2_dir_اندراج_دم);

	retval = ext2fs_سیٹ_rec_len(fs, fs->blocksize - csum_سائز, dir);
	اگر (retval) {
		ext2fs_free_mem(&buf);
		واپس retval;
	}

	اگر (dir_ino) {
		اگر (ext2fs_has_feature_filetype(fs->super))
			filetype = EXT2_FT_DIR;
		/*
		 * Set up entry for '.'
		 */
		dir->inode = dir_ino;
		ext2fs_dirent_سیٹ_name_len(dir, 1);
		ext2fs_dirent_سیٹ_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_سائز) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (ساخت ext2_dir_اندراج *) (buf + dir->rec_len);
		retval = ext2fs_سیٹ_rec_len(fs, rec_len, dir);
		اگر (retval) {
			ext2fs_free_mem(&buf);
			واپس retval;
		}
		dir->inode = والد_ino;
		ext2fs_dirent_سیٹ_name_len(dir, 2);
		ext2fs_dirent_سیٹ_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	اگر (csum_سائز) {
		t = EXT2_DIRENT_دم(buf, fs->blocksize);
		ext2fs_initialize_dirent_دم(fs, t);
	}
	*بلاک = buf;
	واپس 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_ڈیٹا(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((غیر_مستعمل)),
				     ext2_ino_t والد_ino, __u32 *iblock)
{
	ساخت ext2_dir_اندراج 	*dir = تہی;
	errcode_t		retval;
	صحیح			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_مقصد_le32(والد_ino);

	dir = (ساخت ext2_dir_اندراج *)((حرف *)iblock +
					EXT4_INLINE_ڈیٹا_DOTDOT_سائز);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_ڈیٹا_سائز - EXT4_INLINE_ڈیٹا_DOTDOT_سائز;
	retval = ext2fs_سیٹ_rec_len(fs, rec_len, dir);
	اگر (retval)
		جاؤ errout;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (حرف *)dir, rec_len, 0);
	اگر (retval)
		جاؤ errout;
#اگر_ختم

errout:
	واپس retval;
}
