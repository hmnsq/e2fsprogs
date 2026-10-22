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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना get_pathname_struct {
	ext2_ino_t	अन्वेषणम्_ino;
	ext2_ino_t	parent;
	वर्ण		*name;
	errcode_t	errcode;
};

#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
स्थिर पूर्णाङ्क get_pathname_proc(संरचना ext2_dir_प्रविष्टिः *dirent,
			     पूर्णाङ्क	विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
			     पूर्णाङ्क	blocksize EXT2FS_ATTR((अप्रयुक्तम्)),
			     वर्ण	*buf EXT2FS_ATTR((अप्रयुक्तम्)),
			     शून्य	*priv_दत्त)
{
	संरचना get_pathname_struct	*gp;
	errcode_t			retval;
	पूर्णाङ्क name_len = ext2fs_dirent_name_len(dirent);

	gp = (संरचना get_pathname_struct *) priv_दत्त;

	यदि ((name_len == 2) && !वाक्ये_तुलय(dirent->name, "..", 2))
		gp->parent = dirent->inode;
	यदि (dirent->inode == gp->अन्वेषणम्_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		यदि (retval) {
			gp->errcode = retval;
			प्रत्यागम DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		प्रत्यागम DIRENT_ABORT;
	}
	प्रत्यागम 0;
}

स्थिर errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, पूर्णाङ्क maxdepth,
					 वर्ण *buf, वर्ण **name)
{
	संरचना get_pathname_struct gp;
	वर्ण	*parent_name = 0, *ret;
	errcode_t	retval;

	यदि (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		यदि (retval)
			प्रत्यागम retval;
		वाक्यं_प्रतिलिख(*name, (dir == EXT2_मूलम्_INO) ? "/" : ".");
		प्रत्यागम 0;
	}

	यदि (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		यदि (retval)
			प्रत्यागम retval;
		वाक्यं_प्रतिलिख(*name, "...");
		प्रत्यागम 0;
	}

	gp.अन्वेषणम्_ino = ino;
	gp.parent = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	यदि (retval == EXT2_ET_NO_DIRECTORY) {
		वर्ण tmp[32];

		यदि (ino)
			snprintf(tmp, परिमाण(tmp), "<%u>/<%u>", dir, ino);
		अन्यथा
			snprintf(tmp, परिमाण(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(tmp)+1, name);
		यदि (retval)
			गच्छ cleanup;
		वाक्यं_प्रतिलिख(*name, tmp);
		प्रत्यागम 0;
	} अन्यथा यदि (retval)
		गच्छ cleanup;
	यदि (gp.errcode) {
		retval = gp.errcode;
		गच्छ cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.parent, dir, maxdepth-1,
					 buf, &parent_name);
	यदि (retval)
		गच्छ cleanup;
	यदि (!ino) {
		*name = parent_name;
		प्रत्यागम 0;
	}

	यदि (gp.name)
		retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(parent_name)+वाक्यदैर्घ्यम्(gp.name)+2,
					&ret);
	अन्यथा
		retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(parent_name)+5, &ret);
	यदि (retval)
		गच्छ cleanup;

	ret[0] = 0;
	यदि (parent_name[1])
		वाक्यं_योजय(ret, parent_name);
	वाक्यं_योजय(ret, "/");
	यदि (gp.name)
		वाक्यं_योजय(ret, gp.name);
	अन्यथा
		वाक्यं_योजय(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&parent_name);
	ext2fs_free_mem(&gp.name);
	प्रत्यागम retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      वर्ण **name)
{
	वर्ण	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;
	यदि (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	प्रत्यागम retval;

}
