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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਬਣਤਰ get_pathname_struct {
	ext2_ino_t	ਖੋਜ_ino;
	ext2_ino_t	ਮਾਪਾ;
	ਅੱਖਰ		*name;
	errcode_t	errcode;
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਪੂਰਨ get_pathname_proc(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
			     ਪੂਰਨ	ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     ਪੂਰਨ	blocksize EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     ਅੱਖਰ	*buf EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     ਖਾਲੀ	*priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ get_pathname_struct	*gp;
	errcode_t			retval;
	ਪੂਰਨ name_len = ext2fs_dirent_name_len(dirent);

	gp = (ਬਣਤਰ get_pathname_struct *) priv_ਅੰਕੜੇ;

	ਜੇ ((name_len == 2) && !ਲੜੀ_ਤੁਲਨਾ(dirent->name, "..", 2))
		gp->ਮਾਪਾ = dirent->inode;
	ਜੇ (dirent->inode == gp->ਖੋਜ_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		ਜੇ (retval) {
			gp->errcode = retval;
			ਵਾਪਸ DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		ਵਾਪਸ DIRENT_ABORT;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, ਪੂਰਨ maxdepth,
					 ਅੱਖਰ *buf, ਅੱਖਰ **name)
{
	ਬਣਤਰ get_pathname_struct gp;
	ਅੱਖਰ	*ਮਾਪਾ_name = 0, *ret;
	errcode_t	retval;

	ਜੇ (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		strcpy(*name, (dir == EXT2_ਜੜ੍ਹ_INO) ? "/" : ".");
		ਵਾਪਸ 0;
	}

	ਜੇ (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		strcpy(*name, "...");
		ਵਾਪਸ 0;
	}

	gp.ਖੋਜ_ino = ino;
	gp.ਮਾਪਾ = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	ਜੇ (retval == EXT2_ET_NO_DIRECTORY) {
		ਅੱਖਰ tmp[32];

		ਜੇ (ino)
			snprintf(tmp, ਆਕਾਰ(tmp), "<%u>/<%u>", dir, ino);
		ਨਹੀਂਤਾਂ
			snprintf(tmp, ਆਕਾਰ(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(tmp)+1, name);
		ਜੇ (retval)
			ਜਾਓ cleanup;
		strcpy(*name, tmp);
		ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ ਜੇ (retval)
		ਜਾਓ cleanup;
	ਜੇ (gp.errcode) {
		retval = gp.errcode;
		ਜਾਓ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.ਮਾਪਾ, dir, maxdepth-1,
					 buf, &ਮਾਪਾ_name);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	ਜੇ (!ino) {
		*name = ਮਾਪਾ_name;
		ਵਾਪਸ 0;
	}

	ਜੇ (gp.name)
		retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(ਮਾਪਾ_name)+ਲੜੀ_ਲੰਬਾਈ(gp.name)+2,
					&ret);
	ਨਹੀਂਤਾਂ
		retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(ਮਾਪਾ_name)+5, &ret);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	ret[0] = 0;
	ਜੇ (ਮਾਪਾ_name[1])
		strcat(ret, ਮਾਪਾ_name);
	strcat(ret, "/");
	ਜੇ (gp.name)
		strcat(ret, gp.name);
	ਨਹੀਂਤਾਂ
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&ਮਾਪਾ_name);
	ext2fs_free_mem(&gp.name);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      ਅੱਖਰ **name)
{
	ਅੱਖਰ	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;

}
