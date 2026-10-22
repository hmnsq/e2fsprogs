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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

સંરચના get_pathname_struct {
	ext2_ino_t	શોધ_ino;
	ext2_ino_t	parent;
	અક્ષર		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
સ્થિર પૂર્ણાંક get_pathname_proc(સંરચના ext2_dir_entry *dirent,
			     પૂર્ણાંક	offset EXT2FS_ATTR((unused)),
			     પૂર્ણાંક	blocksize EXT2FS_ATTR((unused)),
			     અક્ષર	*buf EXT2FS_ATTR((unused)),
			     શૂન્ય	*priv_data)
{
	સંરચના get_pathname_struct	*gp;
	errcode_t			retval;
	પૂર્ણાંક name_len = ext2fs_dirent_name_len(dirent);

	gp = (સંરચના get_pathname_struct *) priv_data;

	જો ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	જો (dirent->inode == gp->શોધ_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		જો (retval) {
			gp->errcode = retval;
			પાછા DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		પાછા DIRENT_ABORT;
	}
	પાછા 0;
}

સ્થિર errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, પૂર્ણાંક maxdepth,
					 અક્ષર *buf, અક્ષર **name)
{
	સંરચના get_pathname_struct gp;
	અક્ષર	*parent_name = 0, *ret;
	errcode_t	retval;

	જો (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		જો (retval)
			પાછા retval;
		strcpy(*name, (dir == EXT2_મૂળ_INO) ? "/" : ".");
		પાછા 0;
	}

	જો (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		જો (retval)
			પાછા retval;
		strcpy(*name, "...");
		પાછા 0;
	}

	gp.શોધ_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	જો (retval == EXT2_ET_NO_DIRECTORY) {
		અક્ષર tmp[32];

		જો (ino)
			snprintf(tmp, કદ(tmp), "<%u>/<%u>", dir, ino);
		નહીંતર
			snprintf(tmp, કદ(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		જો (retval)
			જાઓ cleanup;
		strcpy(*name, tmp);
		પાછા 0;
	} નહીંતર જો (retval)
		જાઓ cleanup;
	જો (gp.errcode) {
		retval = gp.errcode;
		જાઓ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	જો (retval)
		જાઓ cleanup;
	જો (!ino) {
		*name = parent_name;
		પાછા 0;
	}

	જો (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	નહીંતર
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	જો (retval)
		જાઓ cleanup;

	ret[0] = 0;
	જો (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	જો (gp.name)
		strcat(ret, gp.name);
	નહીંતર
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	પાછા retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      અક્ષર **name)
{
	અક્ષર	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	જો (retval)
		પાછા retval;
	જો (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	પાછા retval;

}
