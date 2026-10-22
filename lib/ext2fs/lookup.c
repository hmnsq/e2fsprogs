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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

काष्ठा lookup_struct  {
	स्थिर अक्षर	*name;
	पूर्णांक		len;
	ext2_ino_t	*inode;
	पूर्णांक		found;
};

#अगर_घोषित __TURBOC__
 #आशय argsused
#पूर्ण_अगर
अटल पूर्णांक lookup_proc(काष्ठा ext2_dir_entry *dirent,
		       पूर्णांक	offset EXT2FS_ATTR((unused)),
		       पूर्णांक	blocksize EXT2FS_ATTR((unused)),
		       अक्षर	*buf EXT2FS_ATTR((unused)),
		       व्योम	*priv_तथ्य)
{
	काष्ठा lookup_struct *ls = (काष्ठा lookup_struct *) priv_तथ्य;

	अगर (ls->len != ext2fs_dirent_name_len(dirent))
		वापस 0;
	अगर (म_भेदन(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		वापस 0;
	*ls->inode = dirent->inode;
	ls->found++;
	वापस DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, स्थिर अक्षर *name,
			पूर्णांक namelen, अक्षर *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	काष्ठा lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	अगर (retval)
		वापस retval;

	वापस (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


