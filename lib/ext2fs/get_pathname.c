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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

काष्ठा get_pathname_struct {
	ext2_ino_t	खोज_ino;
	ext2_ino_t	parent;
	अक्षर		*name;
	errcode_t	errcode;
};

#अगर_घोषित __TURBOC__
 #आशय argsused
#पूर्ण_अगर
अटल पूर्णांक get_pathname_proc(काष्ठा ext2_dir_entry *dirent,
			     पूर्णांक	offset EXT2FS_ATTR((unused)),
			     पूर्णांक	blocksize EXT2FS_ATTR((unused)),
			     अक्षर	*buf EXT2FS_ATTR((unused)),
			     व्योम	*priv_तथ्य)
{
	काष्ठा get_pathname_struct	*gp;
	errcode_t			retval;
	पूर्णांक name_len = ext2fs_dirent_name_len(dirent);

	gp = (काष्ठा get_pathname_struct *) priv_तथ्य;

	अगर ((name_len == 2) && !म_भेदन(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	अगर (dirent->inode == gp->खोज_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		अगर (retval) {
			gp->errcode = retval;
			वापस DIRENT_ABORT;
		}
		म_नकलन(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		वापस DIRENT_ABORT;
	}
	वापस 0;
}

अटल errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, पूर्णांक maxdepth,
					 अक्षर *buf, अक्षर **name)
{
	काष्ठा get_pathname_struct gp;
	अक्षर	*parent_name = 0, *ret;
	errcode_t	retval;

	अगर (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		अगर (retval)
			वापस retval;
		म_नकल(*name, (dir == EXT2_मूल_INO) ? "/" : ".");
		वापस 0;
	}

	अगर (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		अगर (retval)
			वापस retval;
		म_नकल(*name, "...");
		वापस 0;
	}

	gp.खोज_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	अगर (retval == EXT2_ET_NO_DIRECTORY) {
		अक्षर tmp[32];

		अगर (ino)
			snprintf(tmp, माप(tmp), "<%u>/<%u>", dir, ino);
		अन्यथा
			snprintf(tmp, माप(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(म_माप(tmp)+1, name);
		अगर (retval)
			जाओ cleanup;
		म_नकल(*name, tmp);
		वापस 0;
	} अन्यथा अगर (retval)
		जाओ cleanup;
	अगर (gp.errcode) {
		retval = gp.errcode;
		जाओ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	अगर (retval)
		जाओ cleanup;
	अगर (!ino) {
		*name = parent_name;
		वापस 0;
	}

	अगर (gp.name)
		retval = ext2fs_get_mem(म_माप(parent_name)+म_माप(gp.name)+2,
					&ret);
	अन्यथा
		retval = ext2fs_get_mem(म_माप(parent_name)+5, &ret);
	अगर (retval)
		जाओ cleanup;

	ret[0] = 0;
	अगर (parent_name[1])
		म_जोड़ो(ret, parent_name);
	म_जोड़ो(ret, "/");
	अगर (gp.name)
		म_जोड़ो(ret, gp.name);
	अन्यथा
		म_जोड़ो(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	वापस retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      अक्षर **name)
{
	अक्षर	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	अगर (retval)
		वापस retval;
	अगर (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	वापस retval;

}
