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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

கட்டமைப்பு link_struct  {
	மாறிலி எழுத்து	*name;
	முழு		namelen;
	ext2_ino_t	inode;
	முழு		flags;
	கட்டமைப்பு ext2_dir_entry *prev;
	முழு		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
நிலையான முழு unlink_proc(கட்டமைப்பு ext2_dir_entry *dirent,
		     முழு	offset,
		     முழு	blocksize EXT2FS_ATTR((unused)),
		     எழுத்து	*buf EXT2FS_ATTR((unused)),
		     வெறுமை	*priv_data)
{
	கட்டமைப்பு link_struct *ls = (கட்டமைப்பு link_struct *) priv_data;
	கட்டமைப்பு ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	எனில் (ls->name) {
		எனில் (ext2fs_dirent_name_len(dirent) != ls->namelen)
			திரும்பு 0;
		எனில் (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			திரும்பு 0;
	}
	எனில் (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		எனில் (dirent->inode != ls->inode)
			திரும்பு 0;
	} இல்லையெனில் {
		எனில் (!dirent->inode)
			திரும்பு 0;
	}

	எனில் (offset)
		prev->rec_len += dirent->rec_len;
	இல்லையெனில்
		dirent->inode = 0;
	ls->done++;
	திரும்பு DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			மாறிலி எழுத்து *name, ext2_ino_t ino,
			முழு flags)
{
	errcode_t	retval;
	கட்டமைப்பு link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (!name && !ino)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	எனில் (!(fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	எனில் (retval)
		திரும்பு retval;

	திரும்பு (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

