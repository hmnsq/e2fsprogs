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
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#که_نه_تعریف_شوی EXT2_FT_DIR
#تعریف_کړه EXT2_FT_DIR		2
#د_که_پای

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_بلاک(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t پلار_ino, حرف **بلاک)
{
	بنية ext2_dir_ثبت 	*dir = عدم;
	errcode_t		retval;
	حرف			*buf;
	صحيح			rec_len;
	صحيح			filetype = 0;
	بنية ext2_dir_ثبت_لکۍ	*t;
	صحيح			csum_اندازه = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;
	memset(buf, 0, fs->blocksize);
	dir = (بنية ext2_dir_ثبت *) buf;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_اندازه = حجم(بنية ext2_dir_ثبت_لکۍ);

	retval = ext2fs_سېټ_rec_len(fs, fs->blocksize - csum_اندازه, dir);
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
		ext2fs_dirent_سېټ_name_len(dir, 1);
		ext2fs_dirent_سېټ_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_اندازه) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (بنية ext2_dir_ثبت *) (buf + dir->rec_len);
		retval = ext2fs_سېټ_rec_len(fs, rec_len, dir);
		إذا (retval) {
			ext2fs_free_mem(&buf);
			أرجع retval;
		}
		dir->inode = پلار_ino;
		ext2fs_dirent_سېټ_name_len(dir, 2);
		ext2fs_dirent_سېټ_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	إذا (csum_اندازه) {
		t = EXT2_DIRENT_لکۍ(buf, fs->blocksize);
		ext2fs_initialize_dirent_لکۍ(fs, t);
	}
	*بلاک = buf;
	أرجع 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_معلومات(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((ناکارول_شوی)),
				     ext2_ino_t پلار_ino, __u32 *iblock)
{
	بنية ext2_dir_ثبت 	*dir = عدم;
	errcode_t		retval;
	صحيح			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_منزل_le32(پلار_ino);

	dir = (بنية ext2_dir_ثبت *)((حرف *)iblock +
					EXT4_INLINE_معلومات_DOTDOT_اندازه);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_معلومات_اندازه - EXT4_INLINE_معلومات_DOTDOT_اندازه;
	retval = ext2fs_سېټ_rec_len(fs, rec_len, dir);
	إذا (retval)
		اذهب errout;

#که_تعریف_شوی ټکي_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (حرف *)dir, rec_len, 0);
	إذا (retval)
		اذهب errout;
#د_که_پای

errout:
	أرجع retval;
}
