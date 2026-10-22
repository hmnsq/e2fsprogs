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
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

ساخت get_pathname_struct {
	ext2_ino_t	تلاش_ino;
	ext2_ino_t	والد;
	حرف		*name;
	errcode_t	errcode;
};

#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
ساکن صحیح get_pathname_proc(ساخت ext2_dir_اندراج *dirent,
			     صحیح	انحراف EXT2FS_ATTR((غیر_مستعمل)),
			     صحیح	blocksize EXT2FS_ATTR((غیر_مستعمل)),
			     حرف	*buf EXT2FS_ATTR((غیر_مستعمل)),
			     خالی	*priv_ڈیٹا)
{
	ساخت get_pathname_struct	*gp;
	errcode_t			retval;
	صحیح name_len = ext2fs_dirent_name_len(dirent);

	gp = (ساخت get_pathname_struct *) priv_ڈیٹا;

	اگر ((name_len == 2) && !لڑی_موازنہ(dirent->name, "..", 2))
		gp->والد = dirent->inode;
	اگر (dirent->inode == gp->تلاش_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		اگر (retval) {
			gp->errcode = retval;
			واپس DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		واپس DIRENT_ABORT;
	}
	واپس 0;
}

ساکن errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, صحیح maxdepth,
					 حرف *buf, حرف **name)
{
	ساخت get_pathname_struct gp;
	حرف	*والد_name = 0, *ret;
	errcode_t	retval;

	اگر (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		اگر (retval)
			واپس retval;
		strcpy(*name, (dir == EXT2_جڑ_INO) ? "/" : ".");
		واپس 0;
	}

	اگر (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		اگر (retval)
			واپس retval;
		strcpy(*name, "...");
		واپس 0;
	}

	gp.تلاش_ino = ino;
	gp.والد = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	اگر (retval == EXT2_ET_NO_DIRECTORY) {
		حرف tmp[32];

		اگر (ino)
			snprintf(tmp, حجم(tmp), "<%u>/<%u>", dir, ino);
		ورنہ
			snprintf(tmp, حجم(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(لڑی_لمبائی(tmp)+1, name);
		اگر (retval)
			جاؤ cleanup;
		strcpy(*name, tmp);
		واپس 0;
	} ورنہ اگر (retval)
		جاؤ cleanup;
	اگر (gp.errcode) {
		retval = gp.errcode;
		جاؤ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.والد, dir, maxdepth-1,
					 buf, &والد_name);
	اگر (retval)
		جاؤ cleanup;
	اگر (!ino) {
		*name = والد_name;
		واپس 0;
	}

	اگر (gp.name)
		retval = ext2fs_get_mem(لڑی_لمبائی(والد_name)+لڑی_لمبائی(gp.name)+2,
					&ret);
	ورنہ
		retval = ext2fs_get_mem(لڑی_لمبائی(والد_name)+5, &ret);
	اگر (retval)
		جاؤ cleanup;

	ret[0] = 0;
	اگر (والد_name[1])
		strcat(ret, والد_name);
	strcat(ret, "/");
	اگر (gp.name)
		strcat(ret, gp.name);
	ورنہ
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&والد_name);
	ext2fs_free_mem(&gp.name);
	واپس retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      حرف **name)
{
	حرف	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		واپس retval;
	اگر (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	واپس retval;

}
