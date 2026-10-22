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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना link_struct  {
	स्थिरांक अक्षर	*name;
	पूर्णांक		namelen;
	ext2_ino_t	inode;
	पूर्णांक		flags;
	संरचना ext2_dir_entry *prev;
	पूर्णांक		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
स्थिर पूर्णांक unlink_proc(संरचना ext2_dir_entry *dirent,
		     पूर्णांक	offset,
		     पूर्णांक	blocksize EXT2FS_ATTR((unused)),
		     अक्षर	*buf EXT2FS_ATTR((unused)),
		     शून्य	*priv_data)
{
	संरचना link_struct *ls = (संरचना link_struct *) priv_data;
	संरचना ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	जर (ls->name) {
		जर (ext2fs_dirent_name_len(dirent) != ls->namelen)
			परत 0;
		जर (म_भेदन(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			परत 0;
	}
	जर (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		जर (dirent->inode != ls->inode)
			परत 0;
	} अन्यथा {
		जर (!dirent->inode)
			परत 0;
	}

	जर (offset)
		prev->rec_len += dirent->rec_len;
	अन्यथा
		dirent->inode = 0;
	ls->done++;
	परत DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			स्थिरांक अक्षर *name, ext2_ino_t ino,
			पूर्णांक flags)
{
	errcode_t	retval;
	संरचना link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर (!name && !ino)
		परत EXT2_ET_INVALID_ARGUMENT;

	जर (!(fs->flags & EXT2_FLAG_RW))
		परत EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? म_माप(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	जर (retval)
		परत retval;

	परत (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

