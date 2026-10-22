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
#यदि HAVE_UNISTD_H
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

	यदि (ls->name) {
		यदि (ext2fs_dirent_name_len(dirent) != ls->namelen)
			प्रत्यागम 0;
		यदि (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			प्रत्यागम 0;
	}
	यदि (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		यदि (dirent->inode != ls->inode)
			प्रत्यागम 0;
	} अन्यथा {
		यदि (!dirent->inode)
			प्रत्यागम 0;
	}

	यदि (offset)
		prev->rec_len += dirent->rec_len;
	अन्यथा
		dirent->inode = 0;
	ls->done++;
	प्रत्यागम DIRENT_ABORT|DIRENT_CHANGED;
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

	यदि (!name && !ino)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	यदि (!(fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

