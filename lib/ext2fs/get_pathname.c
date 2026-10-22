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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

ഘടന get_pathname_struct {
	ext2_ino_t	തിരയൽ_ino;
	ext2_ino_t	parent;
	അക്ഷരം		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
സ്ഥിതം പൂർണ്ണസംഖ്യ get_pathname_proc(ഘടന ext2_dir_entry *dirent,
			     പൂർണ്ണസംഖ്യ	offset EXT2FS_ATTR((unused)),
			     പൂർണ്ണസംഖ്യ	blocksize EXT2FS_ATTR((unused)),
			     അക്ഷരം	*buf EXT2FS_ATTR((unused)),
			     ശൂന്യം	*priv_data)
{
	ഘടന get_pathname_struct	*gp;
	errcode_t			retval;
	പൂർണ്ണസംഖ്യ name_len = ext2fs_dirent_name_len(dirent);

	gp = (ഘടന get_pathname_struct *) priv_data;

	എങ്കിൽ ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	എങ്കിൽ (dirent->inode == gp->തിരയൽ_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		എങ്കിൽ (retval) {
			gp->errcode = retval;
			തിരികെ DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		തിരികെ DIRENT_ABORT;
	}
	തിരികെ 0;
}

സ്ഥിതം errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, പൂർണ്ണസംഖ്യ maxdepth,
					 അക്ഷരം *buf, അക്ഷരം **name)
{
	ഘടന get_pathname_struct gp;
	അക്ഷരം	*parent_name = 0, *ret;
	errcode_t	retval;

	എങ്കിൽ (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		എങ്കിൽ (retval)
			തിരികെ retval;
		strcpy(*name, (dir == EXT2_വേര്_INO) ? "/" : ".");
		തിരികെ 0;
	}

	എങ്കിൽ (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		എങ്കിൽ (retval)
			തിരികെ retval;
		strcpy(*name, "...");
		തിരികെ 0;
	}

	gp.തിരയൽ_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	എങ്കിൽ (retval == EXT2_ET_NO_DIRECTORY) {
		അക്ഷരം tmp[32];

		എങ്കിൽ (ino)
			snprintf(tmp, വലുപ്പം(tmp), "<%u>/<%u>", dir, ino);
		അല്ലെങ്കിൽ
			snprintf(tmp, വലുപ്പം(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		എങ്കിൽ (retval)
			പോകുക cleanup;
		strcpy(*name, tmp);
		തിരികെ 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (retval)
		പോകുക cleanup;
	എങ്കിൽ (gp.errcode) {
		retval = gp.errcode;
		പോകുക cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	എങ്കിൽ (!ino) {
		*name = parent_name;
		തിരികെ 0;
	}

	എങ്കിൽ (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	അല്ലെങ്കിൽ
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	ret[0] = 0;
	എങ്കിൽ (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	എങ്കിൽ (gp.name)
		strcat(ret, gp.name);
	അല്ലെങ്കിൽ
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	തിരികെ retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      അക്ഷരം **name)
{
	അക്ഷരം	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	തിരികെ retval;

}
