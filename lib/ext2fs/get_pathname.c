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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

ساختار get_pathname_struct {
	ext2_ino_t	جستجو_ino;
	ext2_ino_t	parent;
	نویسه		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ایستا صحیح get_pathname_proc(ساختار ext2_dir_entry *dirent,
			     صحیح	offset EXT2FS_ATTR((unused)),
			     صحیح	blocksize EXT2FS_ATTR((unused)),
			     نویسه	*buf EXT2FS_ATTR((unused)),
			     تهی	*priv_data)
{
	ساختار get_pathname_struct	*gp;
	errcode_t			retval;
	صحیح name_len = ext2fs_dirent_name_len(dirent);

	gp = (ساختار get_pathname_struct *) priv_data;

	اگر ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	اگر (dirent->inode == gp->جستجو_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		اگر (retval) {
			gp->errcode = retval;
			بازگشت DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		بازگشت DIRENT_ABORT;
	}
	بازگشت 0;
}

ایستا errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, صحیح maxdepth,
					 نویسه *buf, نویسه **name)
{
	ساختار get_pathname_struct gp;
	نویسه	*parent_name = 0, *ret;
	errcode_t	retval;

	اگر (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		اگر (retval)
			بازگشت retval;
		strcpy(*name, (dir == EXT2_ریشه_INO) ? "/" : ".");
		بازگشت 0;
	}

	اگر (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		اگر (retval)
			بازگشت retval;
		strcpy(*name, "...");
		بازگشت 0;
	}

	gp.جستجو_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	اگر (retval == EXT2_ET_NO_DIRECTORY) {
		نویسه tmp[32];

		اگر (ino)
			snprintf(tmp, اندازه(tmp), "<%u>/<%u>", dir, ino);
		وگرنه
			snprintf(tmp, اندازه(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		اگر (retval)
			برو cleanup;
		strcpy(*name, tmp);
		بازگشت 0;
	} وگرنه اگر (retval)
		برو cleanup;
	اگر (gp.errcode) {
		retval = gp.errcode;
		برو cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	اگر (retval)
		برو cleanup;
	اگر (!ino) {
		*name = parent_name;
		بازگشت 0;
	}

	اگر (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	وگرنه
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	اگر (retval)
		برو cleanup;

	ret[0] = 0;
	اگر (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	اگر (gp.name)
		strcat(ret, gp.name);
	وگرنه
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	بازگشت retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      نویسه **name)
{
	نویسه	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		بازگشت retval;
	اگر (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	بازگشت retval;

}
