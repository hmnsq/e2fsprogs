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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

#ifndef EXT2_FT_DIR
#define EXT2_FT_DIR		2
#endif

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t parent_ino, અક્ષર **block)
{
	સંરચના ext2_dir_entry 	*dir = ખાલી;
	errcode_t		retval;
	અક્ષર			*buf;
	પૂર્ણાંક			rec_len;
	પૂર્ણાંક			filetype = 0;
	સંરચના ext2_dir_entry_tail	*t;
	પૂર્ણાંક			csum_size = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	જો (retval)
		પાછા retval;
	memset(buf, 0, fs->blocksize);
	dir = (સંરચના ext2_dir_entry *) buf;

	જો (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = કદ(સંરચના ext2_dir_entry_tail);

	retval = ext2fs_set_rec_len(fs, fs->blocksize - csum_size, dir);
	જો (retval) {
		ext2fs_free_mem(&buf);
		પાછા retval;
	}

	જો (dir_ino) {
		જો (ext2fs_has_feature_filetype(fs->super))
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
		dir = (સંરચના ext2_dir_entry *) (buf + dir->rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, dir);
		જો (retval) {
			ext2fs_free_mem(&buf);
			પાછા retval;
		}
		dir->inode = parent_ino;
		ext2fs_dirent_set_name_len(dir, 2);
		ext2fs_dirent_set_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	જો (csum_size) {
		t = EXT2_DIRENT_TAIL(buf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}
	*block = buf;
	પાછા 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_data(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((unused)),
				     ext2_ino_t parent_ino, __u32 *iblock)
{
	સંરચના ext2_dir_entry 	*dir = ખાલી;
	errcode_t		retval;
	પૂર્ણાંક			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_to_le32(parent_ino);

	dir = (સંરચના ext2_dir_entry *)((અક્ષર *)iblock +
					EXT4_INLINE_DATA_DOTDOT_SIZE);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
	retval = ext2fs_set_rec_len(fs, rec_len, dir);
	જો (retval)
		જાઓ errout;

#ifdef શબ્દો_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (અક્ષર *)dir, rec_len, 0);
	જો (retval)
		જાઓ errout;
#endif

errout:
	પાછા retval;
}
