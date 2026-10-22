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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t parent_ino, অক্ষর **block)
{
	কাঠামো ext2_dir_entry 	*dir = ফাঁকা;
	errcode_t		retval;
	অক্ষর			*buf;
	পূর্ণসংখ্যা			rec_len;
	পূর্ণসংখ্যা			filetype = 0;
	কাঠামো ext2_dir_entry_লেজ	*t;
	পূর্ণসংখ্যা			csum_আকার = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ফেরত retval;
	memset(buf, 0, fs->blocksize);
	dir = (কাঠামো ext2_dir_entry *) buf;

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		csum_আকার = আকার(কাঠামো ext2_dir_entry_লেজ);

	retval = ext2fs_set_rec_len(fs, fs->blocksize - csum_আকার, dir);
	যদি (retval) {
		ext2fs_free_mem(&buf);
		ফেরত retval;
	}

	যদি (dir_ino) {
		যদি (ext2fs_has_feature_filetype(fs->super))
			filetype = EXT2_FT_DIR;
		/*
		 * Set up entry for '.'
		 */
		dir->inode = dir_ino;
		ext2fs_dirent_set_name_len(dir, 1);
		ext2fs_dirent_set_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_আকার) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (কাঠামো ext2_dir_entry *) (buf + dir->rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, dir);
		যদি (retval) {
			ext2fs_free_mem(&buf);
			ফেরত retval;
		}
		dir->inode = parent_ino;
		ext2fs_dirent_set_name_len(dir, 2);
		ext2fs_dirent_set_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	যদি (csum_আকার) {
		t = EXT2_DIRENT_লেজ(buf, fs->blocksize);
		ext2fs_initialize_dirent_লেজ(fs, t);
	}
	*block = buf;
	ফেরত 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_তথ্য(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((unused)),
				     ext2_ino_t parent_ino, __u32 *iblock)
{
	কাঠামো ext2_dir_entry 	*dir = ফাঁকা;
	errcode_t		retval;
	পূর্ণসংখ্যা			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_to_le32(parent_ino);

	dir = (কাঠামো ext2_dir_entry *)((অক্ষর *)iblock +
					EXT4_INLINE_তথ্য_DOTDOT_আকার);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_তথ্য_আকার - EXT4_INLINE_তথ্য_DOTDOT_আকার;
	retval = ext2fs_set_rec_len(fs, rec_len, dir);
	যদি (retval)
		যাও errout;

#ifdef শব্দগুলি_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (অক্ষর *)dir, rec_len, 0);
	যদি (retval)
		যাও errout;
#endif

errout:
	ফেরত retval;
}
