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
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية get_pathname_struct {
	ext2_ino_t	بحث_ino;
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

	إذا ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	إذا (dirent->inode == gp->بحث_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		إذا (retval) {
			gp->errcode = retval;
			إرجاع DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		إرجاع DIRENT_ABORT;
	}
	إرجاع 0;
}

ساكن errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, صحيح maxdepth,
					 حرف *buf, حرف **name)
{
	بنية get_pathname_struct gp;
	حرف	*parent_name = 0, *ret;
	errcode_t	retval;

	إذا (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		إذا (retval)
			إرجاع retval;
		strcpy(*name, (dir == EXT2_جذر_INO) ? "/" : ".");
		إرجاع 0;
	}

	إذا (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		إذا (retval)
			إرجاع retval;
		strcpy(*name, "...");
		إرجاع 0;
	}

	gp.بحث_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	إذا (retval == EXT2_ET_NO_DIRECTORY) {
		حرف tmp[32];

		إذا (ino)
			snprintf(tmp, حجم(tmp), "<%u>/<%u>", dir, ino);
		وإلا
			snprintf(tmp, حجم(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		إذا (retval)
			اذهب cleanup;
		strcpy(*name, tmp);
		إرجاع 0;
	} وإلا إذا (retval)
		اذهب cleanup;
	إذا (gp.errcode) {
		retval = gp.errcode;
		اذهب cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	إذا (retval)
		اذهب cleanup;
	إذا (!ino) {
		*name = parent_name;
		إرجاع 0;
	}

	إذا (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	وإلا
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	إذا (retval)
		اذهب cleanup;

	ret[0] = 0;
	إذا (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	إذا (gp.name)
		strcat(ret, gp.name);
	وإلا
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	إرجاع retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      حرف **name)
{
	حرف	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		إرجاع retval;
	إذا (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	إرجاع retval;

}
