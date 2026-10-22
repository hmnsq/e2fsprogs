/*
 * unlink.c --- delete links in a ext2fs directory
 *
 * Copyright (C) 1993, 1994, 1997 Theodore Ts'o.
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

काष्ठा link_struct  {
	स्थिर अक्षर	*name;
	पूर्णांक		namelen;
	ext2_ino_t	inode;
	पूर्णांक		flags;
	काष्ठा ext2_dir_entry *पूर्व;
	पूर्णांक		done;
};

#अगर_घोषित __TURBOC__
 #आशय argsused
#पूर्ण_अगर
अटल पूर्णांक unlink_proc(काष्ठा ext2_dir_entry *dirent,
		     पूर्णांक	offset,
		     पूर्णांक	blocksize EXT2FS_ATTR((unused)),
		     अक्षर	*buf EXT2FS_ATTR((unused)),
		     व्योम	*priv_तथ्य)
{
	काष्ठा link_struct *ls = (काष्ठा link_struct *) priv_तथ्य;
	काष्ठा ext2_dir_entry *पूर्व;

	पूर्व = ls->पूर्व;
	ls->पूर्व = dirent;

	अगर (ls->name) {
		अगर (ext2fs_dirent_name_len(dirent) != ls->namelen)
			वापस 0;
		अगर (म_भेदन(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			वापस 0;
	}
	अगर (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		अगर (dirent->inode != ls->inode)
			वापस 0;
	} अन्यथा {
		अगर (!dirent->inode)
			वापस 0;
	}

	अगर (offset)
		पूर्व->rec_len += dirent->rec_len;
	अन्यथा
		dirent->inode = 0;
	ls->done++;
	वापस DIRENT_ABORT|DIRENT_CHANGED;
}

#अगर_घोषित __TURBOC__
 #आशय argsused
#पूर्ण_अगर
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			स्थिर अक्षर *name, ext2_ino_t ino,
			पूर्णांक flags)
{
	errcode_t	retval;
	काष्ठा link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (!name && !ino)
		वापस EXT2_ET_INVALID_ARGUMENT;

	अगर (!(fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? म_माप(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.पूर्व = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	अगर (retval)
		वापस retval;

	वापस (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

