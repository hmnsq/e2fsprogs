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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية get_pathname_struct {
	ext2_ino_t	search_ino;
	ext2_ino_t	parent;
	حرف		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ساكن صحيح get_pathname_proc(بنية ext2_dir_entry *dirent,
			     صحيح	offset EXT2FS_ATTR((unused)),
			     صحيح	blocksize EXT2FS_ATTR((unused)),
			     حرف	*buf EXT2FS_ATTR((unused)),
			     فراغ	*priv_data)
{
	بنية get_pathname_struct	*gp;
	errcode_t			retval;
	صحيح name_len = ext2fs_dirent_name_len(dirent);

	gp = (بنية get_pathname_struct *) priv_data;

	אם ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	אם (dirent->inode == gp->search_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		אם (retval) {
			gp->errcode = retval;
			החזר DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		החזר DIRENT_ABORT;
	}
	החזר 0;
}

ساكن errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, صحيح maxdepth,
					 حرف *buf, حرف **name)
{
	بنية get_pathname_struct gp;
	حرف	*parent_name = 0, *ret;
	errcode_t	retval;

	אם (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		אם (retval)
			החזר retval;
		strcpy(*name, (dir == EXT2_ROOT_INO) ? "/" : ".");
		החזר 0;
	}

	אם (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		אם (retval)
			החזר retval;
		strcpy(*name, "...");
		החזר 0;
	}

	gp.search_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	אם (retval == EXT2_ET_NO_DIRECTORY) {
		حرف tmp[32];

		אם (ino)
			snprintf(tmp, حجم(tmp), "<%u>/<%u>", dir, ino);
		אחרת
			snprintf(tmp, حجم(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		אם (retval)
			اذهب cleanup;
		strcpy(*name, tmp);
		החזר 0;
	} אחרת אם (retval)
		اذهب cleanup;
	אם (gp.errcode) {
		retval = gp.errcode;
		اذهب cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	אם (retval)
		اذهب cleanup;
	אם (!ino) {
		*name = parent_name;
		החזר 0;
	}

	אם (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	אחרת
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	אם (retval)
		اذهب cleanup;

	ret[0] = 0;
	אם (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	אם (gp.name)
		strcat(ret, gp.name);
	אחרת
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	החזר retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      حرف **name)
{
	حرف	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	אם (retval)
		החזר retval;
	אם (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	החזר retval;

}
