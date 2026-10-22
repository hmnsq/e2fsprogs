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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ EXT2_FT_DIR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_DIR		2
#ਜੇ_ਅੰਤ

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_ਖੰਡ(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t ਮਾਪਾ_ino, ਅੱਖਰ **ਖੰਡ)
{
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ 	*dir = ਨਿਰਾ;
	errcode_t		retval;
	ਅੱਖਰ			*buf;
	ਪੂਰਨ			rec_len;
	ਪੂਰਨ			filetype = 0;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ	*t;
	ਪੂਰਨ			csum_ਮਾਪ = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	memset(buf, 0, fs->blocksize);
	dir = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) buf;

	ਜੇ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ);

	retval = ext2fs_ਸੈੱਟ_rec_len(fs, fs->blocksize - csum_ਮਾਪ, dir);
	ਜੇ (retval) {
		ext2fs_free_mem(&buf);
		ਵਾਪਸ retval;
	}

	ਜੇ (dir_ino) {
		ਜੇ (ext2fs_has_feature_filetype(fs->super))
			filetype = EXT2_FT_DIR;
		/*
		 * Set up entry for '.'
		 */
		dir->inode = dir_ino;
		ext2fs_dirent_ਸੈੱਟ_name_len(dir, 1);
		ext2fs_dirent_ਸੈੱਟ_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_ਮਾਪ) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) (buf + dir->rec_len);
		retval = ext2fs_ਸੈੱਟ_rec_len(fs, rec_len, dir);
		ਜੇ (retval) {
			ext2fs_free_mem(&buf);
			ਵਾਪਸ retval;
		}
		dir->inode = ਮਾਪਾ_ino;
		ext2fs_dirent_ਸੈੱਟ_name_len(dir, 2);
		ext2fs_dirent_ਸੈੱਟ_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	ਜੇ (csum_ਮਾਪ) {
		t = EXT2_DIRENT_ਪੂਛ(buf, fs->blocksize);
		ext2fs_initialize_dirent_ਪੂਛ(fs, t);
	}
	*ਖੰਡ = buf;
	ਵਾਪਸ 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_ਅੰਕੜੇ(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
				     ext2_ino_t ਮਾਪਾ_ino, __u32 *iblock)
{
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ 	*dir = ਨਿਰਾ;
	errcode_t		retval;
	ਪੂਰਨ			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਮਾਪਾ_ino);

	dir = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)((ਅੱਖਰ *)iblock +
					EXT4_INLINE_ਅੰਕੜੇ_DOTDOT_ਮਾਪ);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ - EXT4_INLINE_ਅੰਕੜੇ_DOTDOT_ਮਾਪ;
	retval = ext2fs_ਸੈੱਟ_rec_len(fs, rec_len, dir);
	ਜੇ (retval)
		ਜਾਓ errout;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (ਅੱਖਰ *)dir, rec_len, 0);
	ਜੇ (retval)
		ਜਾਓ errout;
#ਜੇ_ਅੰਤ

errout:
	ਵਾਪਸ retval;
}
