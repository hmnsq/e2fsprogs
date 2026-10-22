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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#جے_بنا_تعریف EXT2_FT_DIR
#تعریف_کرو EXT2_FT_DIR		2
#جے_انت

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_بلاک(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t ماپا_ino, حرف **بلاک)
{
	بنية ext2_dir_اندراج 	*dir = عدم;
	errcode_t		retval;
	حرف			*buf;
	صحيح			rec_len;
	صحيح			filetype = 0;
	بنية ext2_dir_اندراج_پوچھ	*t;
	صحيح			csum_ناپ = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;
	memset(buf, 0, fs->blocksize);
	dir = (بنية ext2_dir_اندراج *) buf;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ناپ = حجم(بنية ext2_dir_اندراج_پوچھ);

	retval = ext2fs_سیٹ_rec_len(fs, fs->blocksize - csum_ناپ, dir);
	إذا (retval) {
		ext2fs_free_mem(&buf);
		أرجع retval;
	}

	إذا (dir_ino) {
		إذا (ext2fs_has_feature_filetype(fs->super))
			filetype = EXT2_FT_DIR;
		/*
		 * Set up entry for '.'
		 */
		dir->inode = dir_ino;
		ext2fs_dirent_سیٹ_name_len(dir, 1);
		ext2fs_dirent_سیٹ_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_ناپ) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (بنية ext2_dir_اندراج *) (buf + dir->rec_len);
		retval = ext2fs_سیٹ_rec_len(fs, rec_len, dir);
		إذا (retval) {
			ext2fs_free_mem(&buf);
			أرجع retval;
		}
		dir->inode = ماپا_ino;
		ext2fs_dirent_سیٹ_name_len(dir, 2);
		ext2fs_dirent_سیٹ_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	إذا (csum_ناپ) {
		t = EXT2_DIRENT_پوچھ(buf, fs->blocksize);
		ext2fs_initialize_dirent_پوچھ(fs, t);
	}
	*بلاک = buf;
	أرجع 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_انکڑے(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((بن_ورتیا)),
				     ext2_ino_t ماپا_ino, __u32 *iblock)
{
	بنية ext2_dir_اندراج 	*dir = عدم;
	errcode_t		retval;
	صحيح			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_منزل_le32(ماپا_ino);

	dir = (بنية ext2_dir_اندراج *)((حرف *)iblock +
					EXT4_INLINE_انکڑے_DOTDOT_ناپ);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_انکڑے_ناپ - EXT4_INLINE_انکڑے_DOTDOT_ناپ;
	retval = ext2fs_سیٹ_rec_len(fs, rec_len, dir);
	إذا (retval)
		اذهب errout;

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (حرف *)dir, rec_len, 0);
	إذا (retval)
		اذهب errout;
#جے_انت

errout:
	أرجع retval;
}
