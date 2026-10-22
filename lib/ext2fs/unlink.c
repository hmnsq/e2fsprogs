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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

ساختار link_struct  {
	ثابت نویسه	*name;
	صحیح		namelen;
	ext2_ino_t	inode;
	صحیح		flags;
	ساختار ext2_dir_entry *prev;
	صحیح		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ایستا صحیح unlink_proc(ساختار ext2_dir_entry *dirent,
		     صحیح	offset,
		     صحیح	blocksize EXT2FS_ATTR((unused)),
		     نویسه	*buf EXT2FS_ATTR((unused)),
		     تهی	*priv_data)
{
	ساختار link_struct *ls = (ساختار link_struct *) priv_data;
	ساختار ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	اگر (ls->name) {
		اگر (ext2fs_dirent_name_len(dirent) != ls->namelen)
			بازگشت 0;
		اگر (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			بازگشت 0;
	}
	اگر (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		اگر (dirent->inode != ls->inode)
			بازگشت 0;
	} وگرنه {
		اگر (!dirent->inode)
			بازگشت 0;
	}

	اگر (offset)
		prev->rec_len += dirent->rec_len;
	وگرنه
		dirent->inode = 0;
	ls->done++;
	بازگشت DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ثابت نویسه *name, ext2_ino_t ino,
			صحیح flags)
{
	errcode_t	retval;
	ساختار link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!name && !ino)
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	اگر (!(fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	اگر (retval)
		بازگشت retval;

	بازگشت (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

