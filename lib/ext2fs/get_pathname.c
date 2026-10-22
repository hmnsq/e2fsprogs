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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना get_pathname_struct {
	ext2_ino_t	शोध_ino;
	ext2_ino_t	parent;
	अक्षर		*name;
	errcode_t	errcode;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
स्थिर पूर्णांक get_pathname_proc(संरचना ext2_dir_entry *dirent,
			     पूर्णांक	offset EXT2FS_ATTR((unused)),
			     पूर्णांक	blocksize EXT2FS_ATTR((unused)),
			     अक्षर	*buf EXT2FS_ATTR((unused)),
			     शून्य	*priv_data)
{
	संरचना get_pathname_struct	*gp;
	errcode_t			retval;
	पूर्णांक name_len = ext2fs_dirent_name_len(dirent);

	gp = (संरचना get_pathname_struct *) priv_data;

	जर ((name_len == 2) && !म_भेदन(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	जर (dirent->inode == gp->शोध_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		जर (retval) {
			gp->errcode = retval;
			परत DIRENT_ABORT;
		}
		म_नकलन(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		परत DIRENT_ABORT;
	}
	परत 0;
}

स्थिर errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, पूर्णांक maxdepth,
					 अक्षर *buf, अक्षर **name)
{
	संरचना get_pathname_struct gp;
	अक्षर	*parent_name = 0, *ret;
	errcode_t	retval;

	जर (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		जर (retval)
			परत retval;
		म_नकल(*name, (dir == EXT2_मूळ_INO) ? "/" : ".");
		परत 0;
	}

	जर (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		जर (retval)
			परत retval;
		म_नकल(*name, "...");
		परत 0;
	}

	gp.शोध_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	जर (retval == EXT2_ET_NO_DIRECTORY) {
		अक्षर tmp[32];

		जर (ino)
			snprintf(tmp, आकार(tmp), "<%u>/<%u>", dir, ino);
		अन्यथा
			snprintf(tmp, आकार(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(म_माप(tmp)+1, name);
		जर (retval)
			जा cleanup;
		म_नकल(*name, tmp);
		परत 0;
	} अन्यथा जर (retval)
		जा cleanup;
	जर (gp.errcode) {
		retval = gp.errcode;
		जा cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	जर (retval)
		जा cleanup;
	जर (!ino) {
		*name = parent_name;
		परत 0;
	}

	जर (gp.name)
		retval = ext2fs_get_mem(म_माप(parent_name)+म_माप(gp.name)+2,
					&ret);
	अन्यथा
		retval = ext2fs_get_mem(म_माप(parent_name)+5, &ret);
	जर (retval)
		जा cleanup;

	ret[0] = 0;
	जर (parent_name[1])
		म_जोड़ो(ret, parent_name);
	म_जोड़ो(ret, "/");
	जर (gp.name)
		म_जोड़ो(ret, gp.name);
	अन्यथा
		म_जोड़ो(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	परत retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      अक्षर **name)
{
	अक्षर	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	जर (retval)
		परत retval;
	जर (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	परत retval;

}
