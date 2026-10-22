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
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना link_struct  {
	ध्रुव वर्ण	*name;
	पूर्णाङ्क		namelen;
	ext2_ino_t	inode;
	पूर्णाङ्क		flags;
	संरचना ext2_dir_प्रविष्टिः *पूर्व;
	पूर्णाङ्क		समाप्तम्;
};

#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
स्थिर पूर्णाङ्क unlink_proc(संरचना ext2_dir_प्रविष्टिः *dirent,
		     पूर्णाङ्क	विस्थापनम्,
		     पूर्णाङ्क	blocksize EXT2FS_ATTR((अप्रयुक्तम्)),
		     वर्ण	*buf EXT2FS_ATTR((अप्रयुक्तम्)),
		     शून्य	*priv_दत्त)
{
	संरचना link_struct *ls = (संरचना link_struct *) priv_दत्त;
	संरचना ext2_dir_प्रविष्टिः *पूर्व;

	पूर्व = ls->पूर्व;
	ls->पूर्व = dirent;

	यदि (ls->name) {
		यदि (ext2fs_dirent_name_len(dirent) != ls->namelen)
			प्रत्यागम 0;
		यदि (वाक्ये_तुलय(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			प्रत्यागम 0;
	}
	यदि (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		यदि (dirent->inode != ls->inode)
			प्रत्यागम 0;
	} अन्यथा {
		यदि (!dirent->inode)
			प्रत्यागम 0;
	}

	यदि (विस्थापनम्)
		पूर्व->rec_len += dirent->rec_len;
	अन्यथा
		dirent->inode = 0;
	ls->समाप्तम्++;
	प्रत्यागम DIRENT_ABORT|DIRENT_परिवर्तितम्;
}

#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			ध्रुव वर्ण *name, ext2_ino_t ino,
			पूर्णाङ्क flags)
{
	errcode_t	retval;
	संरचना link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!name && !ino)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	यदि (!(fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? वाक्यदैर्घ्यम्(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.समाप्तम् = 0;
	ls.पूर्व = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_रिक्तानि,
				    0, unlink_proc, &ls);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम (ls.समाप्तम्) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

