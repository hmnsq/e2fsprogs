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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

கட்டமைப்பு get_pathname_struct {
	ext2_ino_t	தேடல்_ino;
	ext2_ino_t	parent;
	எழுத்து		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
நிலையான முழு get_pathname_proc(கட்டமைப்பு ext2_dir_entry *dirent,
			     முழு	offset EXT2FS_ATTR((unused)),
			     முழு	blocksize EXT2FS_ATTR((unused)),
			     எழுத்து	*buf EXT2FS_ATTR((unused)),
			     வெறுமை	*priv_data)
{
	கட்டமைப்பு get_pathname_struct	*gp;
	errcode_t			retval;
	முழு name_len = ext2fs_dirent_name_len(dirent);

	gp = (கட்டமைப்பு get_pathname_struct *) priv_data;

	எனில் ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	எனில் (dirent->inode == gp->தேடல்_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		எனில் (retval) {
			gp->errcode = retval;
			திரும்பு DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		திரும்பு DIRENT_ABORT;
	}
	திரும்பு 0;
}

நிலையான errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, முழு maxdepth,
					 எழுத்து *buf, எழுத்து **name)
{
	கட்டமைப்பு get_pathname_struct gp;
	எழுத்து	*parent_name = 0, *ret;
	errcode_t	retval;

	எனில் (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		எனில் (retval)
			திரும்பு retval;
		strcpy(*name, (dir == EXT2_வேர்_INO) ? "/" : ".");
		திரும்பு 0;
	}

	எனில் (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		எனில் (retval)
			திரும்பு retval;
		strcpy(*name, "...");
		திரும்பு 0;
	}

	gp.தேடல்_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	எனில் (retval == EXT2_ET_NO_DIRECTORY) {
		எழுத்து tmp[32];

		எனில் (ino)
			snprintf(tmp, அளவு(tmp), "<%u>/<%u>", dir, ino);
		இல்லையெனில்
			snprintf(tmp, அளவு(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		எனில் (retval)
			செல் cleanup;
		strcpy(*name, tmp);
		திரும்பு 0;
	} இல்லையெனில் எனில் (retval)
		செல் cleanup;
	எனில் (gp.errcode) {
		retval = gp.errcode;
		செல் cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	எனில் (retval)
		செல் cleanup;
	எனில் (!ino) {
		*name = parent_name;
		திரும்பு 0;
	}

	எனில் (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	இல்லையெனில்
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	எனில் (retval)
		செல் cleanup;

	ret[0] = 0;
	எனில் (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	எனில் (gp.name)
		strcat(ret, gp.name);
	இல்லையெனில்
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	திரும்பு retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      எழுத்து **name)
{
	எழுத்து	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	எனில் (retval)
		திரும்பு retval;
	எனில் (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	திரும்பு retval;

}
