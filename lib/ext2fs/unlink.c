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
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना link_struct  {
	ध्रुव वर्ण	*name;
	पूर्णाङ्क		namelen;
	ext2_ino_t	inode;
	पूर्णाङ्क		flags;
	संरचना ext2_dir_entry *prev;
	पूर्णाङ्क		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
स्थिर पूर्णाङ्क unlink_proc(संरचना ext2_dir_entry *dirent,
		     पूर्णाङ्क	offset,
		     पूर्णाङ्क	blocksize EXT2FS_ATTR((unused)),
		     वर्ण	*buf EXT2FS_ATTR((unused)),
		     शून्य	*priv_data)
{
	संरचना link_struct *ls = (संरचना link_struct *) priv_data;
	संरचना ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	सचे (ls->name) {
		सचे (ext2fs_dirent_name_len(dirent) != ls->namelen)
			पटिनिवत्त 0;
		सचे (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			पटिनिवत्त 0;
	}
	सचे (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		सचे (dirent->inode != ls->inode)
			पटिनिवत्त 0;
	} अञ्ञथा {
		सचे (!dirent->inode)
			पटिनिवत्त 0;
	}

	सचे (offset)
		prev->rec_len += dirent->rec_len;
	अञ्ञथा
		dirent->inode = 0;
	ls->done++;
	पटिनिवत्त DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ध्रुव वर्ण *name, ext2_ino_t ino,
			पूर्णाङ्क flags)
{
	errcode_t	retval;
	संरचना link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	सचे (!name && !ino)
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	सचे (!(fs->flags & EXT2_FLAG_RW))
		पटिनिवत्त EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	सचे (retval)
		पटिनिवत्त retval;

	पटिनिवत्त (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

