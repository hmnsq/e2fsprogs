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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना lookup_struct  {
	ध्रुव वर्ण	*name;
	पूर्णाङ्क		len;
	ext2_ino_t	*inode;
	पूर्णाङ्क		found;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
स्थिर पूर्णाङ्क lookup_proc(संरचना ext2_dir_entry *dirent,
		       पूर्णाङ्क	offset EXT2FS_ATTR((unused)),
		       पूर्णाङ्क	blocksize EXT2FS_ATTR((unused)),
		       वर्ण	*buf EXT2FS_ATTR((unused)),
		       शून्य	*priv_data)
{
	संरचना lookup_struct *ls = (संरचना lookup_struct *) priv_data;

	यदि (ls->len != ext2fs_dirent_name_len(dirent))
		प्रत्यागम 0;
	यदि (म_भेदन(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		प्रत्यागम 0;
	*ls->inode = dirent->inode;
	ls->found++;
	प्रत्यागम DIRENT_ABORT;
}


errcode_t ext2fs_lookup(ext2_filsys fs, ext2_ino_t dir, ध्रुव वर्ण *name,
			पूर्णाङ्क namelen, वर्ण *buf, ext2_ino_t *inode)
{
	errcode_t	retval;
	संरचना lookup_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ls.name = name;
	ls.len = namelen;
	ls.inode = inode;
	ls.found = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम (ls.found) ? 0 : EXT2_ET_FILE_NOT_FOUND;
}


