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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

నిర్మాణం lookup_struct  {
	స్థిరరాశి అక్షరం	*name;
	పూర్ణాంకం		len;
	ext2_ino_t	*inode;
	పూర్ణాంకం		దొరికింది;
};

#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
స్థిర పూర్ణాంకం lookup_proc(నిర్మాణం ext2_dir_నమోదు *dirent,
		       పూర్ణాంకం	స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
		       పూర్ణాంకం	blocksize EXT2FS_ATTR((వాడని)),
		       అక్షరం	*buf EXT2FS_ATTR((వాడని)),
		       శూన్య	*priv_దత్తాంశం)
{
	నిర్మాణం lookup_struct *ls = (నిర్మాణం lookup_struct *) priv_దత్తాంశం;

	ఒకవేళ (ls->len != ext2fs_dirent_name_len(dirent))
		తిరిగి 0;
	ఒకవేళ (అక్షరశ్రేణి_పోలిక(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		తిరిగి 0;
	*ls->inode = dirent->inode;
	ls->దొరికింది++;
	తిరిగి DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, స్థిరరాశి అక్షరం *name,
			పూర్ణాంకం namelen, అక్షరం *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	నిర్మాణం lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.దొరికింది = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	ఒకవేళ (retval)
		తిరిగి retval;

	తిరిగి (ls.దొరికింది) ? 0 : EXT2_ET_FILE_NOT_దొరికింది;
}


