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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᬬᬾᬦ᭄ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

sa_nracanaa get_pathname_struct {
	ext2_ino_t	search_ino;
	ext2_ino_t	parent;
	var_na		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static puur_naa_nka get_pathname_proc(sa_nracanaa ext2_dir_entry *dirent,
			     puur_naa_nka	offset EXT2FS_ATTR((unused)),
			     puur_naa_nka	blocksize EXT2FS_ATTR((unused)),
			     var_na	*buf EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	sa_nracanaa get_pathname_struct	*gp;
	errcode_t			retval;
	puur_naa_nka name_len = ext2fs_dirent_name_len(dirent);

	gp = (sa_nracanaa get_pathname_struct *) priv_data;

	ᬬᬾᬦ᭄ ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	ᬬᬾᬦ᭄ (dirent->inode == gp->search_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		ᬬᬾᬦ᭄ (retval) {
			gp->errcode = retval;
			ᬯᬮᬶ DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		ᬯᬮᬶ DIRENT_ABORT;
	}
	ᬯᬮᬶ 0;
}

static errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, puur_naa_nka maxdepth,
					 var_na *buf, var_na **name)
{
	sa_nracanaa get_pathname_struct gp;
	var_na	*parent_name = 0, *ret;
	errcode_t	retval;

	ᬬᬾᬦ᭄ (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		ᬬᬾᬦ᭄ (retval)
			ᬯᬮᬶ retval;
		strcpy(*name, (dir == EXT2_ROOT_INO) ? "/" : ".");
		ᬯᬮᬶ 0;
	}

	ᬬᬾᬦ᭄ (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		ᬬᬾᬦ᭄ (retval)
			ᬯᬮᬶ retval;
		strcpy(*name, "...");
		ᬯᬮᬶ 0;
	}

	gp.search_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	ᬬᬾᬦ᭄ (retval == EXT2_ET_NO_DIRECTORY) {
		var_na tmp[32];

		ᬬᬾᬦ᭄ (ino)
			snprintf(tmp, parimaa_na(tmp), "<%u>/<%u>", dir, ino);
		ᬮᬶᬬ
			snprintf(tmp, parimaa_na(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		ᬬᬾᬦ᭄ (retval)
			goto cleanup;
		strcpy(*name, tmp);
		ᬯᬮᬶ 0;
	} ᬮᬶᬬ ᬬᬾᬦ᭄ (retval)
		goto cleanup;
	ᬬᬾᬦ᭄ (gp.errcode) {
		retval = gp.errcode;
		goto cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	ᬬᬾᬦ᭄ (retval)
		goto cleanup;
	ᬬᬾᬦ᭄ (!ino) {
		*name = parent_name;
		ᬯᬮᬶ 0;
	}

	ᬬᬾᬦ᭄ (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	ᬮᬶᬬ
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	ᬬᬾᬦ᭄ (retval)
		goto cleanup;

	ret[0] = 0;
	ᬬᬾᬦ᭄ (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	ᬬᬾᬦ᭄ (gp.name)
		strcat(ret, gp.name);
	ᬮᬶᬬ
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	ᬯᬮᬶ retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      var_na **name)
{
	var_na	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ᬬᬾᬦ᭄ (retval)
		ᬯᬮᬶ retval;
	ᬬᬾᬦ᭄ (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	ᬯᬮᬶ retval;

}
