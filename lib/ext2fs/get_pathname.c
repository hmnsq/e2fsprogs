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
#အကယ်၍ HAVE_UNISTD_H
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
static ကိန်းပြည့် get_pathname_proc(sa_nracanaa ext2_dir_entry *dirent,
			     ကိန်းပြည့်	offset EXT2FS_ATTR((unused)),
			     ကိန်းပြည့်	blocksize EXT2FS_ATTR((unused)),
			     var_na	*buf EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	sa_nracanaa get_pathname_struct	*gp;
	errcode_t			retval;
	ကိန်းပြည့် name_len = ext2fs_dirent_name_len(dirent);

	gp = (sa_nracanaa get_pathname_struct *) priv_data;

	အကယ်၍ ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	အကယ်၍ (dirent->inode == gp->search_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		အကယ်၍ (retval) {
			gp->errcode = retval;
			ပြန်ပေး DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		ပြန်ပေး DIRENT_ABORT;
	}
	ပြန်ပေး 0;
}

static errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, ကိန်းပြည့် maxdepth,
					 var_na *buf, var_na **name)
{
	sa_nracanaa get_pathname_struct gp;
	var_na	*parent_name = 0, *ret;
	errcode_t	retval;

	အကယ်၍ (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		အကယ်၍ (retval)
			ပြန်ပေး retval;
		strcpy(*name, (dir == EXT2_ROOT_INO) ? "/" : ".");
		ပြန်ပေး 0;
	}

	အကယ်၍ (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		အကယ်၍ (retval)
			ပြန်ပေး retval;
		strcpy(*name, "...");
		ပြန်ပေး 0;
	}

	gp.search_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	အကယ်၍ (retval == EXT2_ET_NO_DIRECTORY) {
		var_na tmp[32];

		အကယ်၍ (ino)
			snprintf(tmp, parimaa_na(tmp), "<%u>/<%u>", dir, ino);
		မဟုတ်ပါက
			snprintf(tmp, parimaa_na(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		အကယ်၍ (retval)
			goto cleanup;
		strcpy(*name, tmp);
		ပြန်ပေး 0;
	} မဟုတ်ပါက အကယ်၍ (retval)
		goto cleanup;
	အကယ်၍ (gp.errcode) {
		retval = gp.errcode;
		goto cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	အကယ်၍ (retval)
		goto cleanup;
	အကယ်၍ (!ino) {
		*name = parent_name;
		ပြန်ပေး 0;
	}

	အကယ်၍ (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	မဟုတ်ပါက
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	အကယ်၍ (retval)
		goto cleanup;

	ret[0] = 0;
	အကယ်၍ (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	အကယ်၍ (gp.name)
		strcat(ret, gp.name);
	မဟုတ်ပါက
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	ပြန်ပေး retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      var_na **name)
{
	var_na	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	အကယ်၍ (retval)
		ပြန်ပေး retval;
	အကယ်၍ (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	ပြန်ပေး retval;

}
