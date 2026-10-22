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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

నిర్మాణం link_struct  {
	స్థిరరాశి అక్షరం	*name;
	పూర్ణాంకం		namelen;
	ext2_ino_t	inode;
	పూర్ణాంకం		flags;
	నిర్మాణం ext2_dir_నమోదు *prev;
	పూర్ణాంకం		పూర్తి;
};

#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
స్థిర పూర్ణాంకం unlink_proc(నిర్మాణం ext2_dir_నమోదు *dirent,
		     పూర్ణాంకం	స్థానభ్రంశం,
		     పూర్ణాంకం	blocksize EXT2FS_ATTR((వాడని)),
		     అక్షరం	*buf EXT2FS_ATTR((వాడని)),
		     శూన్య	*priv_దత్తాంశం)
{
	నిర్మాణం link_struct *ls = (నిర్మాణం link_struct *) priv_దత్తాంశం;
	నిర్మాణం ext2_dir_నమోదు *prev;

	prev = ls->prev;
	ls->prev = dirent;

	ఒకవేళ (ls->name) {
		ఒకవేళ (ext2fs_dirent_name_len(dirent) != ls->namelen)
			తిరిగి 0;
		ఒకవేళ (అక్షరశ్రేణి_పోలిక(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			తిరిగి 0;
	}
	ఒకవేళ (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		ఒకవేళ (dirent->inode != ls->inode)
			తిరిగి 0;
	} లేకపోతే {
		ఒకవేళ (!dirent->inode)
			తిరిగి 0;
	}

	ఒకవేళ (స్థానభ్రంశం)
		prev->rec_len += dirent->rec_len;
	లేకపోతే
		dirent->inode = 0;
	ls->పూర్తి++;
	తిరిగి DIRENT_ABORT|DIRENT_మారింది;
}

#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			స్థిరరాశి అక్షరం *name, ext2_ino_t ino,
			పూర్ణాంకం flags)
{
	errcode_t	retval;
	నిర్మాణం link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!name && !ino)
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (!(fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? అక్షరశ్రేణి_పొడవు(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.పూర్తి = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_ఖాళీలు,
				    0, unlink_proc, &ls);
	ఒకవేళ (retval)
		తిరిగి retval;

	తిరిగి (ls.పూర్తి) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

