/*
 * unlink.c --- delete links in a ext2fs directory
 *
 * Copyright (C) 1993, 1994, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

ഘടന link_struct  {
	സ്ഥിരം അക്ഷരം	*name;
	പൂർണ്ണസംഖ്യ		namelen;
	ext2_ino_t	inode;
	പൂർണ്ണസംഖ്യ		flags;
	ഘടന ext2_dir_entry *prev;
	പൂർണ്ണസംഖ്യ		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
സ്ഥിതം പൂർണ്ണസംഖ്യ unlink_proc(ഘടന ext2_dir_entry *dirent,
		     പൂർണ്ണസംഖ്യ	offset,
		     പൂർണ്ണസംഖ്യ	blocksize EXT2FS_ATTR((unused)),
		     അക്ഷരം	*buf EXT2FS_ATTR((unused)),
		     ശൂന്യം	*priv_data)
{
	ഘടന link_struct *ls = (ഘടന link_struct *) priv_data;
	ഘടന ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	എങ്കിൽ (ls->name) {
		എങ്കിൽ (ext2fs_dirent_name_len(dirent) != ls->namelen)
			തിരികെ 0;
		എങ്കിൽ (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			തിരികെ 0;
	}
	എങ്കിൽ (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		എങ്കിൽ (dirent->inode != ls->inode)
			തിരികെ 0;
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (!dirent->inode)
			തിരികെ 0;
	}

	എങ്കിൽ (offset)
		prev->rec_len += dirent->rec_len;
	അല്ലെങ്കിൽ
		dirent->inode = 0;
	ls->done++;
	തിരികെ DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			സ്ഥിരം അക്ഷരം *name, ext2_ino_t ino,
			പൂർണ്ണസംഖ്യ flags)
{
	errcode_t	retval;
	ഘടന link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (!name && !ino)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	എങ്കിൽ (!(fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	എങ്കിൽ (retval)
		തിരികെ retval;

	തിരികെ (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

