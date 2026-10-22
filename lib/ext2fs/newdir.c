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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t parent_ino, ৱর্ণ **block)
{
	সংরচনা ext2_dir_entry 	*dir = রিক্ত;
	errcode_t		retval;
	ৱর্ণ			*buf;
	পূর্ণাঙ্ক			rec_len;
	পূর্ণাঙ্ক			filetype = 0;
	সংরচনা ext2_dir_entry_tail	*t;
	পূর্ণাঙ্ক			csum_size = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ঘূৰাই retval;
	memset(buf, 0, fs->blocksize);
	dir = (সংরচনা ext2_dir_entry *) buf;

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = পরিমাণ(সংরচনা ext2_dir_entry_tail);

	retval = ext2fs_set_rec_len(fs, fs->blocksize - csum_size, dir);
	যদি (retval) {
		ext2fs_free_mem(&buf);
		ঘূৰাই retval;
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
		rec_len = (fs->blocksize - csum_size) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (সংরচনা ext2_dir_entry *) (buf + dir->rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, dir);
		যদি (retval) {
			ext2fs_free_mem(&buf);
			ঘূৰাই retval;
		}
		dir->inode = parent_ino;
		ext2fs_dirent_set_name_len(dir, 2);
		ext2fs_dirent_set_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	যদি (csum_size) {
		t = EXT2_DIRENT_TAIL(buf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}
	*block = buf;
	ঘূৰাই 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((unused)),
				     ext2_ino_t parent_ino, __u32 *iblock)
{
	সংরচনা ext2_dir_entry 	*dir = রিক্ত;
	errcode_t		retval;
	পূর্ণাঙ্ক			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_to_le32(parent_ino);

	dir = (সংরচনা ext2_dir_entry *)((ৱর্ণ *)iblock +
					EXT4_INLINE_DATA_DOTDOT_SIZE);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
	retval = ext2fs_set_rec_len(fs, rec_len, dir);
	যদি (retval)
		গচ্ছ errout;

#ifdef শব্দগুলি_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (ৱর্ণ *)dir, rec_len, 0);
	যদি (retval)
		গচ্ছ errout;
#endif

errout:
	ঘূৰাই retval;
}
