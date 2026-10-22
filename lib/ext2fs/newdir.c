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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#यदि_अपरिभाषितम् EXT2_FT_DIR
#परिभाषय EXT2_FT_DIR		2
#यदि_अन्तः

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_खण्डम्(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t parent_ino, वर्ण **खण्डम्)
{
	संरचना ext2_dir_प्रविष्टिः 	*dir = रिक्त;
	errcode_t		retval;
	वर्ण			*buf;
	पूर्णाङ्क			rec_len;
	पूर्णाङ्क			filetype = 0;
	संरचना ext2_dir_प्रविष्टिः_पुच्छ	*t;
	पूर्णाङ्क			csum_आकार = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;
	स्मृतिं_पूरय(buf, 0, fs->blocksize);
	dir = (संरचना ext2_dir_प्रविष्टिः *) buf;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ);

	retval = ext2fs_समुच्चयः_rec_len(fs, fs->blocksize - csum_आकार, dir);
	यदि (retval) {
		ext2fs_free_mem(&buf);
		प्रत्यागम retval;
	}

	यदि (dir_ino) {
		यदि (ext2fs_has_feature_filetype(fs->super))
			filetype = EXT2_FT_DIR;
		/*
		 * Set up entry for '.'
		 */
		dir->inode = dir_ino;
		ext2fs_dirent_समुच्चयः_name_len(dir, 1);
		ext2fs_dirent_समुच्चयः_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_आकार) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (संरचना ext2_dir_प्रविष्टिः *) (buf + dir->rec_len);
		retval = ext2fs_समुच्चयः_rec_len(fs, rec_len, dir);
		यदि (retval) {
			ext2fs_free_mem(&buf);
			प्रत्यागम retval;
		}
		dir->inode = parent_ino;
		ext2fs_dirent_समुच्चयः_name_len(dir, 2);
		ext2fs_dirent_समुच्चयः_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	यदि (csum_आकार) {
		t = EXT2_DIRENT_पुच्छ(buf, fs->blocksize);
		ext2fs_initialize_dirent_पुच्छ(fs, t);
	}
	*खण्डम् = buf;
	प्रत्यागम 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_दत्त(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((अप्रयुक्तम्)),
				     ext2_ino_t parent_ino, __u32 *iblock)
{
	संरचना ext2_dir_प्रविष्टिः 	*dir = रिक्त;
	errcode_t		retval;
	पूर्णाङ्क			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_गन्तव्यम्_le32(parent_ino);

	dir = (संरचना ext2_dir_प्रविष्टिः *)((वर्ण *)iblock +
					EXT4_INLINE_दत्त_DOTDOT_आकार);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_दत्त_आकार - EXT4_INLINE_दत्त_DOTDOT_आकार;
	retval = ext2fs_समुच्चयः_rec_len(fs, rec_len, dir);
	यदि (retval)
		गच्छ errout;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (वर्ण *)dir, rec_len, 0);
	यदि (retval)
		गच्छ errout;
#यदि_अन्तः

errout:
	प्रत्यागम retval;
}
