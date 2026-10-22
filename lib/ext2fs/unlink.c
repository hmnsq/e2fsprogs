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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

কাঠামো link_struct  {
	ধ্রুবক অক্ষর	*name;
	পূর্ণসংখ্যা		namelen;
	ext2_ino_t	inode;
	পূর্ণসংখ্যা		flags;
	কাঠামো ext2_dir_entry *পূর্ব;
	পূর্ণসংখ্যা		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণসংখ্যা unlink_proc(কাঠামো ext2_dir_entry *dirent,
		     পূর্ণসংখ্যা	offset,
		     পূর্ণসংখ্যা	blocksize EXT2FS_ATTR((unused)),
		     অক্ষর	*buf EXT2FS_ATTR((unused)),
		     শূন্য	*priv_তথ্য)
{
	কাঠামো link_struct *ls = (কাঠামো link_struct *) priv_তথ্য;
	কাঠামো ext2_dir_entry *পূর্ব;

	পূর্ব = ls->পূর্ব;
	ls->পূর্ব = dirent;

	যদি (ls->name) {
		যদি (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ফেরত 0;
		যদি (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ফেরত 0;
	}
	যদি (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		যদি (dirent->inode != ls->inode)
			ফেরত 0;
	} অন্যথা {
		যদি (!dirent->inode)
			ফেরত 0;
	}

	যদি (offset)
		পূর্ব->rec_len += dirent->rec_len;
	অন্যথা
		dirent->inode = 0;
	ls->done++;
	ফেরত DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ধ্রুবক অক্ষর *name, ext2_ino_t ino,
			পূর্ণসংখ্যা flags)
{
	errcode_t	retval;
	কাঠামো link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!name && !ino)
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	যদি (!(fs->flags & EXT2_FLAG_RW))
		ফেরত EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.পূর্ব = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	যদি (retval)
		ফেরত retval;

	ফেরত (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

