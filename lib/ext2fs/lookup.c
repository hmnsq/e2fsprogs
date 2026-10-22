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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

સંરચના lookup_struct  {
	અચલ અક્ષર	*name;
	પૂર્ણાંક		len;
	ext2_ino_t	*inode;
	પૂર્ણાંક		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
સ્થિર પૂર્ણાંક lookup_proc(સંરચના ext2_dir_entry *dirent,
		       પૂર્ણાંક	offset EXT2FS_ATTR((unused)),
		       પૂર્ણાંક	blocksize EXT2FS_ATTR((unused)),
		       અક્ષર	*buf EXT2FS_ATTR((unused)),
		       શૂન્ય	*priv_data)
{
	સંરચના lookup_struct *ls = (સંરચના lookup_struct *) priv_data;

	જો (ls->len != ext2fs_dirent_name_len(dirent))
		પાછા 0;
	જો (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		પાછા 0;
	*ls->inode = dirent->inode;
	ls->found++;
	પાછા DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, અચલ અક્ષર *name,
			પૂર્ણાંક namelen, અક્ષર *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	સંરચના lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	જો (retval)
		પાછા retval;

	પાછા (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


