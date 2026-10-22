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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t parent_ino, எழுத்து **block)
{
	கட்டமைப்பு ext2_dir_entry 	*dir = வெற்று;
	errcode_t		retval;
	எழுத்து			*buf;
	முழு			rec_len;
	முழு			filetype = 0;
	கட்டமைப்பு ext2_dir_entry_tail	*t;
	முழு			csum_size = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	எனில் (retval)
		திரும்பு retval;
	memset(buf, 0, fs->blocksize);
	dir = (கட்டமைப்பு ext2_dir_entry *) buf;

	எனில் (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = அளவு(கட்டமைப்பு ext2_dir_entry_tail);

	retval = ext2fs_set_rec_len(fs, fs->blocksize - csum_size, dir);
	எனில் (retval) {
		ext2fs_free_mem(&buf);
		திரும்பு retval;
	}

	எனில் (dir_ino) {
		எனில் (ext2fs_has_feature_filetype(fs->super))
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
		dir = (கட்டமைப்பு ext2_dir_entry *) (buf + dir->rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, dir);
		எனில் (retval) {
			ext2fs_free_mem(&buf);
			திரும்பு retval;
		}
		dir->inode = parent_ino;
		ext2fs_dirent_set_name_len(dir, 2);
		ext2fs_dirent_set_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	எனில் (csum_size) {
		t = EXT2_DIRENT_TAIL(buf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}
	*block = buf;
	திரும்பு 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((unused)),
				     ext2_ino_t parent_ino, __u32 *iblock)
{
	கட்டமைப்பு ext2_dir_entry 	*dir = வெற்று;
	errcode_t		retval;
	முழு			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_to_le32(parent_ino);

	dir = (கட்டமைப்பு ext2_dir_entry *)((எழுத்து *)iblock +
					EXT4_INLINE_DATA_DOTDOT_SIZE);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
	retval = ext2fs_set_rec_len(fs, rec_len, dir);
	எனில் (retval)
		செல் errout;

#ifdef சொற்கள்_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (எழுத்து *)dir, rec_len, 0);
	எனில் (retval)
		செல் errout;
#endif

errout:
	திரும்பு retval;
}
