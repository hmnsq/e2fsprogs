/*
 * get_pathname.c --- do directory/inode -> name translation
 *
 * Copyright (C) 1993, 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

/*
 *
 * 	ext2fs_get_pathname(fs, dir, ino, name)
 *
 * 	This function translates takes two inode numbers into a
 * 	string, placing the result in <name>.  <dir> is the containing
 * 	directory inode, and <ino> is the inode number itself.  If
 * 	<ino> is zero, then ext2fs_get_pathname will return pathname
 * 	of the the directory <dir>.
 *
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಸಂರಚನಾ get_pathname_struct {
	ext2_ino_t	ಹುಡುಕು_ino;
	ext2_ino_t	parent;
	ವರ್ಣ		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_pathname_proc(ಸಂರಚನಾ ext2_dir_entry *dirent,
			     ಪೂರ್ಣಾಙ್ಕ	offset EXT2FS_ATTR((unused)),
			     ಪೂರ್ಣಾಙ್ಕ	blocksize EXT2FS_ATTR((unused)),
			     ವರ್ಣ	*buf EXT2FS_ATTR((unused)),
			     ಶೂನ್ಯ	*priv_data)
{
	ಸಂರಚನಾ get_pathname_struct	*gp;
	errcode_t			retval;
	ಪೂರ್ಣಾಙ್ಕ name_len = ext2fs_dirent_name_len(dirent);

	gp = (ಸಂರಚನಾ get_pathname_struct *) priv_data;

	ಯದಿ ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	ಯದಿ (dirent->inode == gp->ಹುಡುಕು_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		ಯದಿ (retval) {
			gp->errcode = retval;
			ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ maxdepth,
					 ವರ್ಣ *buf, ವರ್ಣ **name)
{
	ಸಂರಚನಾ get_pathname_struct gp;
	ವರ್ಣ	*parent_name = 0, *ret;
	errcode_t	retval;

	ಯದಿ (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		strcpy(*name, (dir == EXT2_ಬೇರು_INO) ? "/" : ".");
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		strcpy(*name, "...");
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	gp.ಹುಡುಕು_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	ಯದಿ (retval == EXT2_ET_NO_DIRECTORY) {
		ವರ್ಣ tmp[32];

		ಯದಿ (ino)
			snprintf(tmp, ಪರಿಮಾಣ(tmp), "<%u>/<%u>", dir, ino);
		ಅನ್ಯಥಾ
			snprintf(tmp, ಪರಿಮಾಣ(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
		strcpy(*name, tmp);
		ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	ಯದಿ (gp.errcode) {
		retval = gp.errcode;
		ಗಚ್ಛ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	ಯದಿ (!ino) {
		*name = parent_name;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	ಅನ್ಯಥಾ
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	ret[0] = 0;
	ಯದಿ (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	ಯದಿ (gp.name)
		strcat(ret, gp.name);
	ಅನ್ಯಥಾ
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      ವರ್ಣ **name)
{
	ವರ್ಣ	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;

}
