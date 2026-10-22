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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ರಚನೆ get_pathname_struct {
	ext2_ino_t	ಹುಡುಕು_ino;
	ext2_ino_t	ಪೋಷಕ;
	ಅಕ್ಷರ		*name;
	errcode_t	errcode;
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_pathname_proc(ರಚನೆ ext2_dir_ನಮೂದು *dirent,
			     ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
			     ಪೂರ್ಣಾಂಕ	blocksize EXT2FS_ATTR((ಬಳಸದ)),
			     ಅಕ್ಷರ	*buf EXT2FS_ATTR((ಬಳಸದ)),
			     ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ get_pathname_struct	*gp;
	errcode_t			retval;
	ಪೂರ್ಣಾಂಕ name_len = ext2fs_dirent_name_len(dirent);

	gp = (ರಚನೆ get_pathname_struct *) priv_ದತ್ತಾಂಶ;

	ಆದರೆ ((name_len == 2) && !ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(dirent->name, "..", 2))
		gp->ಪೋಷಕ = dirent->inode;
	ಆದರೆ (dirent->inode == gp->ಹುಡುಕು_ino) {
		retval = ext2fs_get_mem(name_len + 1, &gp->name);
		ಆದರೆ (retval) {
			gp->errcode = retval;
			ಹಿಂತಿರುಗಿ DIRENT_ABORT;
		}
		strncpy(gp->name, dirent->name, name_len);
		gp->name[name_len] = '\0';
		ಹಿಂತಿರುಗಿ DIRENT_ABORT;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ext2fs_get_pathname_int(ext2_filsys fs, ext2_ino_t dir,
					 ext2_ino_t ino, ಪೂರ್ಣಾಂಕ maxdepth,
					 ಅಕ್ಷರ *buf, ಅಕ್ಷರ **name)
{
	ರಚನೆ get_pathname_struct gp;
	ಅಕ್ಷರ	*ಪೋಷಕ_name = 0, *ret;
	errcode_t	retval;

	ಆದರೆ (dir == ino) {
		retval = ext2fs_get_mem(2, name);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		strcpy(*name, (dir == EXT2_ಬೇರು_INO) ? "/" : ".");
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!dir || (maxdepth < 0)) {
		retval = ext2fs_get_mem(4, name);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		strcpy(*name, "...");
		ಹಿಂತಿರುಗಿ 0;
	}

	gp.ಹುಡುಕು_ino = ino;
	gp.ಪೋಷಕ = 0;
	gp.name = 0;
	gp.errcode = 0;

	retval = ext2fs_dir_iterate(fs, dir, 0, buf, get_pathname_proc, &gp);
	ಆದರೆ (retval == EXT2_ET_NO_DIRECTORY) {
		ಅಕ್ಷರ tmp[32];

		ಆದರೆ (ino)
			snprintf(tmp, ಗಾತ್ರ(tmp), "<%u>/<%u>", dir, ino);
		ಇಲ್ಲದಿದ್ದರೆ
			snprintf(tmp, ಗಾತ್ರ(tmp), "<%u>", dir);
		retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(tmp)+1, name);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
		strcpy(*name, tmp);
		ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (retval)
		ಹೋಗು cleanup;
	ಆದರೆ (gp.errcode) {
		retval = gp.errcode;
		ಹೋಗು cleanup;
	}

	retval = ext2fs_get_pathname_int(fs, gp.ಪೋಷಕ, dir, maxdepth-1,
					 buf, &ಪೋಷಕ_name);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	ಆದರೆ (!ino) {
		*name = ಪೋಷಕ_name;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (gp.name)
		retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೋಷಕ_name)+ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(gp.name)+2,
					&ret);
	ಇಲ್ಲದಿದ್ದರೆ
		retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೋಷಕ_name)+5, &ret);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	ret[0] = 0;
	ಆದರೆ (ಪೋಷಕ_name[1])
		strcat(ret, ಪೋಷಕ_name);
	strcat(ret, "/");
	ಆದರೆ (gp.name)
		strcat(ret, gp.name);
	ಇಲ್ಲದಿದ್ದರೆ
		strcat(ret, "???");
	*name = ret;
	retval = 0;

cleanup:
	ext2fs_free_mem(&ಪೋಷಕ_name);
	ext2fs_free_mem(&gp.name);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_get_pathname(ext2_filsys fs, ext2_ino_t dir, ext2_ino_t ino,
			      ಅಕ್ಷರ **name)
{
	ಅಕ್ಷರ	*buf;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (dir == ino)
		ino = 0;
	retval = ext2fs_get_pathname_int(fs, dir, ino, 32, buf, name);
	ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;

}
