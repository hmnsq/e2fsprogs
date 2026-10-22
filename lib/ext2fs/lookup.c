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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

সংরচনা lookup_struct  {
	ধ্রুৱ ৱর্ণ	*name;
	পূর্ণাঙ্ক		len;
	ext2_ino_t	*inode;
	পূর্ণাঙ্ক		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণাঙ্ক lookup_proc(সংরচনা ext2_dir_entry *dirent,
		       পূর্ণাঙ্ক	offset EXT2FS_ATTR((unused)),
		       পূর্ণাঙ্ক	blocksize EXT2FS_ATTR((unused)),
		       ৱর্ণ	*buf EXT2FS_ATTR((unused)),
		       শূন্য	*priv_data)
{
	সংরচনা lookup_struct *ls = (সংরচনা lookup_struct *) priv_data;

	যদি (ls->len != ext2fs_dirent_name_len(dirent))
		ঘূৰাই 0;
	যদি (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		ঘূৰাই 0;
	*ls->inode = dirent->inode;
	ls->found++;
	ঘূৰাই DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ধ্রুৱ ৱর্ণ *name,
			পূর্ণাঙ্ক namelen, ৱর্ণ *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	সংরচনা lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	যদি (retval)
		ঘূৰাই retval;

	ঘূৰাই (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


