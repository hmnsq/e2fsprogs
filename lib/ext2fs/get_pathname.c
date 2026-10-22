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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

ସଂରଚନା get_pathname_struct {
	ext2_ino_t	ସନ୍ଧାନ_ino;
	ext2_ino_t	parent;
	ଅକ୍ଷର		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_pathname_proc(ସଂରଚନା ext2_dir_entry *dirent,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା	offset EXT2FS_ATTR((unused)),
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocksize EXT2FS_ATTR((unused)),
			     ଅକ୍ଷର	*buf EXT2FS_ATTR((unused)),
			     ଶୂନ୍ୟ	*priv_data)
{
	ସଂରଚନା get_pathname_struct	*gp;
	errcode_t			retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_len = ext2fs_dirent_name_len(dirent);

	gp = (ସଂରଚନା get_pathname_struct *) priv_data;

	ଯଦି ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	ଯଦି (dirent->inode == gp->ସନ୍ଧାନ_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		ଯଦି (retval) {
			gp->errcode = retval;
			ଫେରନ୍ତୁ DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		ଫେରନ୍ତୁ DIRENT_ABORT;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxdepth,
					 ଅକ୍ଷର *buf, ଅକ୍ଷର **name)
{
	ସଂରଚନା get_pathname_struct gp;
	ଅକ୍ଷର	*parent_name = 0, *ret;
	errcode_t	retval;

	ଯଦି (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		strcpy(*name, (dir == EXT2_ମୂଳ_INO) ? "/" : ".");
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		strcpy(*name, "...");
		ଫେରନ୍ତୁ 0;
	}

	gp.ସନ୍ଧାନ_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	ଯଦି (retval == EXT2_ET_NO_DIRECTORY) {
		ଅକ୍ଷର tmp[32];

		ଯଦି (ino)
			snprintf(tmp, ଆକାର(tmp), "<%u>/<%u>", dir, ino);
		ଅନ୍ୟଥା
			snprintf(tmp, ଆକାର(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		ଯଦି (retval)
			ଯାଅ cleanup;
		strcpy(*name, tmp);
		ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା ଯଦି (retval)
		ଯାଅ cleanup;
	ଯଦି (gp.errcode) {
		retval = gp.errcode;
		ଯାଅ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	ଯଦି (retval)
		ଯାଅ cleanup;
	ଯଦି (!ino) {
		*name = parent_name;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	ଅନ୍ୟଥା
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	ଯଦି (retval)
		ଯାଅ cleanup;

	ret[0] = 0;
	ଯଦି (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	ଯଦି (gp.name)
		strcat(ret, gp.name);
	ଅନ୍ୟଥା
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      ଅକ୍ଷର **name)
{
	ଅକ୍ଷର	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;

}
