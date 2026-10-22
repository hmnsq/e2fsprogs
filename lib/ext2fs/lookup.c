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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

கட்டமைப்பு lookup_struct  {
	மாறிலி எழுத்து	*name;
	முழு		len;
	ext2_ino_t	*inode;
	முழு		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
நிலையான முழு lookup_proc(கட்டமைப்பு ext2_dir_entry *dirent,
		       முழு	offset EXT2FS_ATTR((unused)),
		       முழு	blocksize EXT2FS_ATTR((unused)),
		       எழுத்து	*buf EXT2FS_ATTR((unused)),
		       வெறுமை	*priv_data)
{
	கட்டமைப்பு lookup_struct *ls = (கட்டமைப்பு lookup_struct *) priv_data;

	எனில் (ls->len != ext2fs_dirent_name_len(dirent))
		திரும்பு 0;
	எனில் (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		திரும்பு 0;
	*ls->inode = dirent->inode;
	ls->found++;
	திரும்பு DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, மாறிலி எழுத்து *name,
			முழு namelen, எழுத்து *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	கட்டமைப்பு lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	எனில் (retval)
		திரும்பு retval;

	திரும்பு (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


