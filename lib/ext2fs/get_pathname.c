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
#सचे HAVE_UNISTD_H
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

	सचे ((name_len == 2) && !strncmp(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	सचे (dirent->inode == gp->अन्वेषणम्_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		सचे (retval) {
			gp->errcode = retval;
			पटिनिवत्त DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		पटिनिवत्त DIRENT_ABORT;
	}
	पटिनिवत्त 0;
}

स्थिर errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, पूर्णाङ्क maxdepth,
					 वर्ण *buf, वर्ण **name)
{
	संरचना get_pathname_struct gp;
	वर्ण	*parent_name = 0, *ret;
	errcode_t	retval;

	सचे (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		सचे (retval)
			पटिनिवत्त retval;
		strcpy(*name, (dir == EXT2_मूलम्_INO) ? "/" : ".");
		पटिनिवत्त 0;
	}

	सचे (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		सचे (retval)
			पटिनिवत्त retval;
		strcpy(*name, "...");
		पटिनिवत्त 0;
	}

	gp.अन्वेषणम्_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	सचे (retval == EXT2_ET_NO_DIRECTORY) {
		वर्ण tmp[32];

		सचे (ino)
			snprintf(tmp, परिमाण(tmp), "<%u>/<%u>", dir, ino);
		अञ्ञथा
			snprintf(tmp, परिमाण(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(strlen(tmp)+1, name);
		सचे (retval)
			गच्छ cleanup;
		strcpy(*name, tmp);
		पटिनिवत्त 0;
	} अञ्ञथा सचे (retval)
		गच्छ cleanup;
	सचे (gp.errcode) {
		retval = gp.errcode;
		गच्छ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	सचे (retval)
		गच्छ cleanup;
	सचे (!ino) {
		*name = parent_name;
		पटिनिवत्त 0;
	}

	सचे (gp.name)
		retval = ext2fs_get_mem(strlen(parent_name)+strlen(gp.name)+2,
					&ret);
	अञ्ञथा
		retval = ext2fs_get_mem(strlen(parent_name)+5, &ret);
	सचे (retval)
		गच्छ cleanup;

	ret[0] = 0;
	सचे (parent_name[1])
		strcat(ret, parent_name);
	strcat(ret, "/");
	सचे (gp.name)
		strcat(ret, gp.name);
	अञ्ञथा
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	पटिनिवत्त retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      वर्ण **name)
{
	वर्ण	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	सचे (retval)
		पटिनिवत्त retval;
	सचे (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	पटिनिवत्त retval;

}
