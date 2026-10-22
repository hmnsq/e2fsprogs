/*
 * lookup.c --- ext2fs directory lookup operations
 *
 * Copyright (C) 1993, 1994, 1994, 1995 Theodore Ts'o.
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

ഘടന lookup_struct  {
	സ്ഥിരം അക്ഷരം	*name;
	പൂർണ്ണസംഖ്യ		len;
	ext2_ino_t	*inode;
	പൂർണ്ണസംഖ്യ		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
സ്ഥിതം പൂർണ്ണസംഖ്യ lookup_proc(ഘടന ext2_dir_entry *dirent,
		       പൂർണ്ണസംഖ്യ	offset EXT2FS_ATTR((unused)),
		       പൂർണ്ണസംഖ്യ	blocksize EXT2FS_ATTR((unused)),
		       അക്ഷരം	*buf EXT2FS_ATTR((unused)),
		       ശൂന്യം	*priv_data)
{
	ഘടന lookup_struct *ls = (ഘടന lookup_struct *) priv_data;

	എങ്കിൽ (ls->len != ext2fs_dirent_name_len(dirent))
		തിരികെ 0;
	എങ്കിൽ (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		തിരികെ 0;
	*ls->inode = dirent->inode;
	ls->found++;
	തിരികെ DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, സ്ഥിരം അക്ഷരം *name,
			പൂർണ്ണസംഖ്യ namelen, അക്ഷരം *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	ഘടന lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	എങ്കിൽ (retval)
		തിരികെ retval;

	തിരികെ (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


