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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

struct get_pathname_struct {
	ext2_ino_t	search_ino;
	ext2_ino_t	parent;
	char		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static số_nguyên get_pathname_proc(struct ext2_dir_entry *dirent,
			     số_nguyên	offset EXT2FS_ATTR((unused)),
			     số_nguyên	blocksize EXT2FS_ATTR((unused)),
			     char	*buf EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	struct get_pathname_struct	*gp;
	errcode_t			retval;
	số_nguyên name_len = ext2fs_dirent_name_len(dirent);

	gp = (struct get_pathname_struct *) priv_data;

	nếu ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	nếu (dirent->inode == gp->search_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		nếu (retval) {
			gp->errcode = retval;
			trả_về DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		trả_về DIRENT_ABORT;
	}
	trả_về 0;
}

static errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, số_nguyên maxdepth,
					 char *buf, char **name)
{
	struct get_pathname_struct gp;
	char	*parent_name = 0, *ret;
	errcode_t	retval;

	nếu (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		nếu (retval)
			trả_về retval;
		strcpy(*name, (dir == EXT2_ROOT_INO) ? "/" : ".");
		trả_về 0;
	}

	nếu (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		nếu (retval)
			trả_về retval;
		strcpy(*name, "...");
		trả_về 0;
	}

	gp.search_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	nếu (retval == EXT2_ET_NO_DIRECTORY) {
		char tmp[32];

		nếu (ino)
			snprintf(tmp, sizeof(tmp), "<%u>/<%u>", dir, ino);
		không_thì
			snprintf(tmp, sizeof(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		nếu (retval)
			goto cleanup;
		strcpy(*name, tmp);
		trả_về 0;
	} không_thì nếu (retval)
		goto cleanup;
	nếu (gp.errcode) {
		retval = gp.errcode;
		goto cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	nếu (retval)
		goto cleanup;
	nếu (!ino) {
		*name = parent_name;
		trả_về 0;
	}

	nếu (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	không_thì
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	nếu (retval)
		goto cleanup;

	ret[0] = 0;
	nếu (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	nếu (gp.name)
		strcat(ret, gp.name);
	không_thì
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	trả_về retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      char **name)
{
	char	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	nếu (retval)
		trả_về retval;
	nếu (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	trả_về retval;

}
