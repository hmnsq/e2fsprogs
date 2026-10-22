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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

ସଂରଚନା link_struct  {
	ଅଚଳ ଅକ୍ଷର	*name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		namelen;
	ext2_ino_t	inode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		flags;
	ସଂରଚନା ext2_dir_entry *prev;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା unlink_proc(ସଂରଚନା ext2_dir_entry *dirent,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା	offset,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocksize EXT2FS_ATTR((unused)),
		     ଅକ୍ଷର	*buf EXT2FS_ATTR((unused)),
		     ଶୂନ୍ୟ	*priv_data)
{
	ସଂରଚନା link_struct *ls = (ସଂରଚନା link_struct *) priv_data;
	ସଂରଚନା ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	ଯଦି (ls->name) {
		ଯଦି (ext2fs_dirent_name_len(dirent) != ls->namelen)
			ଫେରନ୍ତୁ 0;
		ଯଦି (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			ଫେରନ୍ତୁ 0;
	}
	ଯଦି (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		ଯଦି (dirent->inode != ls->inode)
			ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା {
		ଯଦି (!dirent->inode)
			ଫେରନ୍ତୁ 0;
	}

	ଯଦି (offset)
		prev->rec_len += dirent->rec_len;
	ଅନ୍ୟଥା
		dirent->inode = 0;
	ls->done++;
	ଫେରନ୍ତୁ DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ଅଚଳ ଅକ୍ଷର *name, ext2_ino_t ino,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	errcode_t	retval;
	ସଂରଚନା link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (!name && !ino)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ଯଦି (!(fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଫେରନ୍ତୁ (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

