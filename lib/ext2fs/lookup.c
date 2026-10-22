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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

ସଂରଚନା lookup_struct  {
	ଅଚଳ ଅକ୍ଷର	*name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		len;
	ext2_ino_t	*inode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_proc(ସଂରଚନା ext2_dir_entry *dirent,
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା	offset EXT2FS_ATTR((unused)),
		       ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocksize EXT2FS_ATTR((unused)),
		       ଅକ୍ଷର	*buf EXT2FS_ATTR((unused)),
		       ଶୂନ୍ୟ	*priv_data)
{
	ସଂରଚନା lookup_struct *ls = (ସଂରଚନା lookup_struct *) priv_data;

	ଯଦି (ls->len != ext2fs_dirent_name_len(dirent))
		ଫେରନ୍ତୁ 0;
	ଯଦି (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		ଫେରନ୍ତୁ 0;
	*ls->inode = dirent->inode;
	ls->found++;
	ଫେରନ୍ତୁ DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ଅଚଳ ଅକ୍ଷର *name,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା namelen, ଅକ୍ଷର *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	ସଂରଚନା lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଫେରନ୍ତୁ (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


