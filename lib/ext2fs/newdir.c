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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

#अगर_अघोषित EXT2_FT_DIR
#घोषणा EXT2_FT_DIR		2
#पूर्ण_अगर

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_block(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t parent_ino, अक्षर **block)
{
	काष्ठा ext2_dir_entry 	*dir = शून्य;
	errcode_t		retval;
	अक्षर			*buf;
	पूर्णांक			rec_len;
	पूर्णांक			filetype = 0;
	काष्ठा ext2_dir_entry_पुच्छ	*t;
	पूर्णांक			csum_आकार = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	अगर (retval)
		वापस retval;
	स_रखो(buf, 0, fs->blocksize);
	dir = (काष्ठा ext2_dir_entry *) buf;

	अगर (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = माप(काष्ठा ext2_dir_entry_पुच्छ);

	retval = ext2fs_set_rec_len(fs, fs->blocksize - csum_आकार, dir);
	अगर (retval) {
		ext2fs_free_mem(&buf);
		वापस retval;
	}

	अगर (dir_ino) {
		अगर (ext2fs_has_feature_filetype(fs->super))
			filetype = EXT2_FT_DIR;
		/*
		 * Set up entry for '.'
		 */
		dir->inode = dir_ino;
		ext2fs_dirent_set_name_len(dir, 1);
		ext2fs_dirent_set_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_आकार) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (काष्ठा ext2_dir_entry *) (buf + dir->rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, dir);
		अगर (retval) {
			ext2fs_free_mem(&buf);
			वापस retval;
		}
		dir->inode = parent_ino;
		ext2fs_dirent_set_name_len(dir, 2);
		ext2fs_dirent_set_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	अगर (csum_आकार) {
		t = EXT2_DIRENT_पुच्छ(buf, fs->blocksize);
		ext2fs_initialize_dirent_पुच्छ(fs, t);
	}
	*block = buf;
	वापस 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_तथ्य(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((unused)),
				     ext2_ino_t parent_ino, __u32 *iblock)
{
	काष्ठा ext2_dir_entry 	*dir = शून्य;
	errcode_t		retval;
	पूर्णांक			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_to_le32(parent_ino);

	dir = (काष्ठा ext2_dir_entry *)((अक्षर *)iblock +
					EXT4_INLINE_तथ्य_DOTDOT_आकार);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_तथ्य_आकार - EXT4_INLINE_तथ्य_DOTDOT_आकार;
	retval = ext2fs_set_rec_len(fs, rec_len, dir);
	अगर (retval)
		जाओ errout;

#अगर_घोषित शब्दगण_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (अक्षर *)dir, rec_len, 0);
	अगर (retval)
		जाओ errout;
#पूर्ण_अगर

errout:
	वापस retval;
}
