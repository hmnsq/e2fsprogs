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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t parent_ino, അക്ഷരം **block)
{
	ഘടന ext2_dir_entry 	*dir = ശൂന്യ;
	errcode_t		retval;
	അക്ഷരം			*buf;
	പൂർണ്ണസംഖ്യ			rec_len;
	പൂർണ്ണസംഖ്യ			filetype = 0;
	ഘടന ext2_dir_entry_tail	*t;
	പൂർണ്ണസംഖ്യ			csum_size = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	എങ്കിൽ (retval)
		തിരികെ retval;
	memset(buf, 0, fs->blocksize);
	dir = (ഘടന ext2_dir_entry *) buf;

	എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = വലുപ്പം(ഘടന ext2_dir_entry_tail);

	retval = ext2fs_set_rec_len(fs, fs->blocksize - csum_size, dir);
	എങ്കിൽ (retval) {
		ext2fs_free_mem(&buf);
		തിരികെ retval;
	}

	എങ്കിൽ (dir_ino) {
		എങ്കിൽ (ext2fs_has_feature_filetype(fs->super))
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
		dir = (ഘടന ext2_dir_entry *) (buf + dir->rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, dir);
		എങ്കിൽ (retval) {
			ext2fs_free_mem(&buf);
			തിരികെ retval;
		}
		dir->inode = parent_ino;
		ext2fs_dirent_set_name_len(dir, 2);
		ext2fs_dirent_set_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	എങ്കിൽ (csum_size) {
		t = EXT2_DIRENT_TAIL(buf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}
	*block = buf;
	തിരികെ 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((unused)),
				     ext2_ino_t parent_ino, __u32 *iblock)
{
	ഘടന ext2_dir_entry 	*dir = ശൂന്യ;
	errcode_t		retval;
	പൂർണ്ണസംഖ്യ			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_to_le32(parent_ino);

	dir = (ഘടന ext2_dir_entry *)((അക്ഷരം *)iblock +
					EXT4_INLINE_DATA_DOTDOT_SIZE);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
	retval = ext2fs_set_rec_len(fs, rec_len, dir);
	എങ്കിൽ (retval)
		പോകുക errout;

#ifdef വാക്കുകൾ_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (അക്ഷരം *)dir, rec_len, 0);
	എങ്കിൽ (retval)
		പോകുക errout;
#endif

errout:
	തിരികെ retval;
}
