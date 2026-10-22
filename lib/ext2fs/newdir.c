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
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#جيڪڏهن_وصف_نه_ٿيل EXT2_FT_DIR
#وصف_ڪريو EXT2_FT_DIR		2
#جيڪڏهن_پورو

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_بلاڪ(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t والد_ino, حرف **بلاڪ)
{
	بنية ext2_dir_داخلا 	*dir = عدم;
	errcode_t		retval;
	حرف			*buf;
	صحيح			rec_len;
	صحيح			filetype = 0;
	بنية ext2_dir_داخلا_پڇ	*t;
	صحيح			csum_سائيز = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;
	memset(buf, 0, fs->blocksize);
	dir = (بنية ext2_dir_داخلا *) buf;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائيز = حجم(بنية ext2_dir_داخلا_پڇ);

	retval = ext2fs_سيٽ_rec_len(fs, fs->blocksize - csum_سائيز, dir);
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
		ext2fs_dirent_سيٽ_name_len(dir, 1);
		ext2fs_dirent_سيٽ_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_سائيز) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (بنية ext2_dir_داخلا *) (buf + dir->rec_len);
		retval = ext2fs_سيٽ_rec_len(fs, rec_len, dir);
		إذا (retval) {
			ext2fs_free_mem(&buf);
			أرجع retval;
		}
		dir->inode = والد_ino;
		ext2fs_dirent_سيٽ_name_len(dir, 2);
		ext2fs_dirent_سيٽ_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	إذا (csum_سائيز) {
		t = EXT2_DIRENT_پڇ(buf, fs->blocksize);
		ext2fs_initialize_dirent_پڇ(fs, t);
	}
	*بلاڪ = buf;
	أرجع 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_معلومات(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
				     ext2_ino_t والد_ino, __u32 *iblock)
{
	بنية ext2_dir_داخلا 	*dir = عدم;
	errcode_t		retval;
	صحيح			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_منزل_le32(والد_ino);

	dir = (بنية ext2_dir_داخلا *)((حرف *)iblock +
					EXT4_INLINE_معلومات_DOTDOT_سائيز);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_معلومات_سائيز - EXT4_INLINE_معلومات_DOTDOT_سائيز;
	retval = ext2fs_سيٽ_rec_len(fs, rec_len, dir);
	إذا (retval)
		اذهب errout;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (حرف *)dir, rec_len, 0);
	إذا (retval)
		اذهب errout;
#جيڪڏهن_پورو

errout:
	أرجع retval;
}
