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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

কাঠামো lookup_struct  {
	ধ্রুবক অক্ষর	*name;
	পূর্ণসংখ্যা		len;
	ext2_ino_t	*inode;
	পূর্ণসংখ্যা		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণসংখ্যা lookup_proc(কাঠামো ext2_dir_entry *dirent,
		       পূর্ণসংখ্যা	offset EXT2FS_ATTR((unused)),
		       পূর্ণসংখ্যা	blocksize EXT2FS_ATTR((unused)),
		       অক্ষর	*buf EXT2FS_ATTR((unused)),
		       শূন্য	*priv_তথ্য)
{
	কাঠামো lookup_struct *ls = (কাঠামো lookup_struct *) priv_তথ্য;

	যদি (ls->len != ext2fs_dirent_name_len(dirent))
		ফেরত 0;
	যদি (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		ফেরত 0;
	*ls->inode = dirent->inode;
	ls->found++;
	ফেরত DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ধ্রুবক অক্ষর *name,
			পূর্ণসংখ্যা namelen, অক্ষর *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	কাঠামো lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	যদি (retval)
		ফেরত retval;

	ফেরত (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


