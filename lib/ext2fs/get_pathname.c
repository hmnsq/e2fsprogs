/*
 * get_pathname.c --- do directory/inode -> name translation
 *
 * Copyright (C) 1993, 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

/*
 *
 * 	ext2fs_get_pathname(fs, dir, ino, name)
 *
 * 	This function translates takes two inode numbers into a
 * 	string, placing the result in <name>.  <dir> is the containing
 * 	directory inode, and <ino> is the inode number itself.  If
 * 	<ino> is zero, then ext2fs_get_pathname will return pathname
 * 	of the the directory <dir>.
 *
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

నిర్మాణం get_pathname_struct {
	ext2_ino_t	శోధన_ino;
	ext2_ino_t	మాతృ;
	అక్షరం		*name;
	errcode_t	errcode;
};

#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
స్థిర పూర్ణాంకం get_pathname_proc(నిర్మాణం ext2_dir_నమోదు *dirent,
			     పూర్ణాంకం	స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
			     పూర్ణాంకం	blocksize EXT2FS_ATTR((వాడని)),
			     అక్షరం	*buf EXT2FS_ATTR((వాడని)),
			     శూన్య	*priv_దత్తాంశం)
{
	నిర్మాణం get_pathname_struct	*gp;
	errcode_t			retval;
	పూర్ణాంకం name_len = ext2fs_dirent_name_len(dirent);

	gp = (నిర్మాణం get_pathname_struct *) priv_దత్తాంశం;

	ఒకవేళ ((name_len == 2) && !అక్షరశ్రేణి_పోలిక(dirent->name, "..", 2))
		gp->మాతృ = dirent->inode;
	ఒకవేళ (dirent->inode == gp->శోధన_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		ఒకవేళ (retval) {
			gp->errcode = retval;
			తిరిగి DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		తిరిగి DIRENT_ABORT;
	}
	తిరిగి 0;
}

స్థిర errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, పూర్ణాంకం maxdepth,
					 అక్షరం *buf, అక్షరం **name)
{
	నిర్మాణం get_pathname_struct gp;
	అక్షరం	*మాతృ_name = 0, *ret;
	errcode_t	retval;

	ఒకవేళ (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		ఒకవేళ (retval)
			తిరిగి retval;
		strcpy(*name, (dir == EXT2_మూలం_INO) ? "/" : ".");
		తిరిగి 0;
	}

	ఒకవేళ (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		ఒకవేళ (retval)
			తిరిగి retval;
		strcpy(*name, "...");
		తిరిగి 0;
	}

	gp.శోధన_ino = ino;
	gp.మాతృ = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	ఒకవేళ (retval == EXT2_ET_NO_DIRECTORY) {
		అక్షరం tmp[32];

		ఒకవేళ (ino)
			snprintf(tmp, పరిమాణం(tmp), "<%u>/<%u>", dir, ino);
		లేకపోతే
			snprintf(tmp, పరిమాణం(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(tmp)+1, name);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
		strcpy(*name, tmp);
		తిరిగి 0;
	} లేకపోతే ఒకవేళ (retval)
		వెళ్ళు cleanup;
	ఒకవేళ (gp.errcode) {
		retval = gp.errcode;
		వెళ్ళు cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.మాతృ, dir, maxdepth-1,
					 buf, &మాతృ_name);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	ఒకవేళ (!ino) {
		*name = మాతృ_name;
		తిరిగి 0;
	}

	ఒకవేళ (gp.name)
		retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(మాతృ_name)+అక్షరశ్రేణి_పొడవు(gp.name)+2,
					&ret);
	లేకపోతే
		retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(మాతృ_name)+5, &ret);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	ret[0] = 0;
	ఒకవేళ (మాతృ_name[1])
		strcat(ret, మాతృ_name);
	strcat(ret, "/");
	ఒకవేళ (gp.name)
		strcat(ret, gp.name);
	లేకపోతే
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&మాతృ_name);
	ext2fs_free_mem(&gp.name);
	తిరిగి retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      అక్షరం **name)
{
	అక్షరం	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	తిరిగి retval;

}
