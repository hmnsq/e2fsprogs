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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ EXT2_FT_DIR
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_DIR		2
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_ಖಂಡ(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t ಪೋಷಕ_ino, ಅಕ್ಷರ **ಖಂಡ)
{
	ರಚನೆ ext2_dir_ನಮೂದು 	*dir = ಖಾಲಿ;
	errcode_t		retval;
	ಅಕ್ಷರ			*buf;
	ಪೂರ್ಣಾಂಕ			rec_len;
	ಪೂರ್ಣಾಂಕ			filetype = 0;
	ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ	*t;
	ಪೂರ್ಣಾಂಕ			csum_ಗಾತ್ರ = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	memset(buf, 0, fs->blocksize);
	dir = (ರಚನೆ ext2_dir_ನಮೂದು *) buf;

	ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ);

	retval = ext2fs_ಗಣ_rec_len(fs, fs->blocksize - csum_ಗಾತ್ರ, dir);
	ಆದರೆ (retval) {
		ext2fs_free_mem(&buf);
		ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ (dir_ino) {
		ಆದರೆ (ext2fs_has_feature_filetype(fs->super))
			filetype = EXT2_FT_DIR;
		/*
		 * Set up entry for '.'
		 */
		dir->inode = dir_ino;
		ext2fs_dirent_ಗಣ_name_len(dir, 1);
		ext2fs_dirent_ಗಣ_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_ಗಾತ್ರ) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (ರಚನೆ ext2_dir_ನಮೂದು *) (buf + dir->rec_len);
		retval = ext2fs_ಗಣ_rec_len(fs, rec_len, dir);
		ಆದರೆ (retval) {
			ext2fs_free_mem(&buf);
			ಹಿಂತಿರುಗಿ retval;
		}
		dir->inode = ಪೋಷಕ_ino;
		ext2fs_dirent_ಗಣ_name_len(dir, 2);
		ext2fs_dirent_ಗಣ_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	ಆದರೆ (csum_ಗಾತ್ರ) {
		t = EXT2_DIRENT_ಬಾಲ(buf, fs->blocksize);
		ext2fs_initialize_dirent_ಬಾಲ(fs, t);
	}
	*ಖಂಡ = buf;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_ದತ್ತಾಂಶ(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((ಬಳಸದ)),
				     ext2_ino_t ಪೋಷಕ_ino, __u32 *iblock)
{
	ರಚನೆ ext2_dir_ನಮೂದು 	*dir = ಖಾಲಿ;
	errcode_t		retval;
	ಪೂರ್ಣಾಂಕ			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_ಗಮ್ಯ_le32(ಪೋಷಕ_ino);

	dir = (ರಚನೆ ext2_dir_ನಮೂದು *)((ಅಕ್ಷರ *)iblock +
					EXT4_INLINE_ದತ್ತಾಂಶ_DOTDOT_ಗಾತ್ರ);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ - EXT4_INLINE_ದತ್ತಾಂಶ_DOTDOT_ಗಾತ್ರ;
	retval = ext2fs_ಗಣ_rec_len(fs, rec_len, dir);
	ಆದರೆ (retval)
		ಹೋಗು errout;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (ಅಕ್ಷರ *)dir, rec_len, 0);
	ಆದರೆ (retval)
		ಹೋಗು errout;
#ಆದರೆ_ಅಂತ್ಯ

errout:
	ಹಿಂತಿರುಗಿ retval;
}
