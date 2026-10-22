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
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना lookup_struct  {
	ध्रुव वर्ण	*name;
	पूर्णाङ्क		len;
	ext2_ino_t	*inode;
	पूर्णाङ्क		प्राप्तम्;
};

#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
स्थिर पूर्णाङ्क lookup_proc(संरचना ext2_dir_प्रविष्टिः *dirent,
		       पूर्णाङ्क	विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
		       पूर्णाङ्क	blocksize EXT2FS_ATTR((अप्रयुक्तम्)),
		       वर्ण	*buf EXT2FS_ATTR((अप्रयुक्तम्)),
		       शून्य	*priv_दत्त)
{
	संरचना lookup_struct *ls = (संरचना lookup_struct *) priv_दत्त;

	यदि (ls->len != ext2fs_dirent_name_len(dirent))
		प्रत्यागम 0;
	यदि (वाक्ये_तुलय(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
		प्रत्यागम 0;
	*ls->inode = dirent->inode;
	ls->प्राप्तम्++;
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
	ls.प्राप्तम् = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, lookup_proc, &ls);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम (ls.प्राप्तम्) ? 0 : EXT2_ET_FILE_NOT_प्राप्तम्;
}


