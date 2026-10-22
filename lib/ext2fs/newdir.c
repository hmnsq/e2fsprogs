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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t parent_ino, ವರ್ಣ **block)
{
	ಸಂರಚನಾ ext2_dir_entry 	*dir = ರಿಕ್ತ;
	errcode_t		retval;
	ವರ್ಣ			*buf;
	ಪೂರ್ಣಾಙ್ಕ			rec_len;
	ಪೂರ್ಣಾಙ್ಕ			filetype = 0;
	ಸಂರಚನಾ ext2_dir_entry_tail	*t;
	ಪೂರ್ಣಾಙ್ಕ			csum_size = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	memset(buf, 0, fs->blocksize);
	dir = (ಸಂರಚನಾ ext2_dir_entry *) buf;

	ಯದಿ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dir_entry_tail);

	retval = ext2fs_set_rec_len(fs, fs->blocksize - csum_size, dir);
	ಯದಿ (retval) {
		ext2fs_free_mem(&buf);
		ಪ್ರತ್ಯಾಗಮ retval;
	}

	ಯದಿ (dir_ino) {
		ಯದಿ (ext2fs_has_feature_filetype(fs->super))
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
		dir = (ಸಂರಚನಾ ext2_dir_entry *) (buf + dir->rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, dir);
		ಯದಿ (retval) {
			ext2fs_free_mem(&buf);
			ಪ್ರತ್ಯಾಗಮ retval;
		}
		dir->inode = parent_ino;
		ext2fs_dirent_set_name_len(dir, 2);
		ext2fs_dirent_set_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	ಯದಿ (csum_size) {
		t = EXT2_DIRENT_TAIL(buf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}
	*block = buf;
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((unused)),
				     ext2_ino_t parent_ino, __u32 *iblock)
{
	ಸಂರಚನಾ ext2_dir_entry 	*dir = ರಿಕ್ತ;
	errcode_t		retval;
	ಪೂರ್ಣಾಙ್ಕ			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_to_le32(parent_ino);

	dir = (ಸಂರಚನಾ ext2_dir_entry *)((ವರ್ಣ *)iblock +
					EXT4_INLINE_DATA_DOTDOT_SIZE);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
	retval = ext2fs_set_rec_len(fs, rec_len, dir);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;

#ifdef ಪದಗಳು_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (ವರ್ಣ *)dir, rec_len, 0);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;
#endif

errout:
	ಪ್ರತ್ಯಾಗಮ retval;
}
