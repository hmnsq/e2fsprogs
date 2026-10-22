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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना get_pathname_struct {
	ext2_ino_t	अन्वेषणम्_ino;
	ext2_ino_t	parent;
	वर्ण		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
स्थिर पूर्णाङ्क get_pathname_proc(संरचना ext2_dir_entry *dirent,
			     पूर्णाङ्क	offset EXT2FS_ATTR((unused)),
			     पूर्णाङ्क	blocksize EXT2FS_ATTR((unused)),
			     वर्ण	*buf EXT2FS_ATTR((unused)),
			     शून्य	*priv_data)
{
	संरचना get_pathname_struct	*gp;
	errcode_t			retval;
	पूर्णाङ्क name_len = ext2fs_dirent_name_len(dirent);

	gp = (संरचना get_pathname_struct *) priv_data;

	जइ ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	जइ (dirent->inode == gp->अन्वेषणम्_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		जइ (retval) {
			gp->errcode = retval;
			प्रत्यागम DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		प्रत्यागम DIRENT_ABORT;
	}
	प्रत्यागम 0;
}

स्थिर errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, पूर्णाङ्क maxdepth,
					 वर्ण *buf, वर्ण **name)
{
	संरचना get_pathname_struct gp;
	वर्ण	*parent_name = 0, *ret;
	errcode_t	retval;

	जइ (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		जइ (retval)
			प्रत्यागम retval;
		strcpy(*name, (dir == EXT2_मूलम्_INO) ? "/" : ".");
		प्रत्यागम 0;
	}

	जइ (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		जइ (retval)
			प्रत्यागम retval;
		strcpy(*name, "...");
		प्रत्यागम 0;
	}

	gp.अन्वेषणम्_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	जइ (retval == EXT2_ET_NO_DIRECTORY) {
		वर्ण tmp[32];

		जइ (ino)
			snprintf(tmp, परिमाण(tmp), "<%u>/<%u>", dir, ino);
		अण्णहा
			snprintf(tmp, परिमाण(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		जइ (retval)
			गच्छ cleanup;
		strcpy(*name, tmp);
		प्रत्यागम 0;
	} अण्णहा जइ (retval)
		गच्छ cleanup;
	जइ (gp.errcode) {
		retval = gp.errcode;
		गच्छ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	जइ (retval)
		गच्छ cleanup;
	जइ (!ino) {
		*name = parent_name;
		प्रत्यागम 0;
	}

	जइ (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	अण्णहा
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	जइ (retval)
		गच्छ cleanup;

	ret[0] = 0;
	जइ (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	जइ (gp.name)
		strcat(ret, gp.name);
	अण्णहा
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	प्रत्यागम retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      वर्ण **name)
{
	वर्ण	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	जइ (retval)
		प्रत्यागम retval;
	जइ (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	प्रत्यागम retval;

}
