/*
 * namei.c --- ext2fs directory lookup operations
 *
 * Copyright (C) 1993, 1994, 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

/* #define NAMEI_DEBUG */

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ errcode_t open_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t ತಳ,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, size_t pathlen, ಪೂರ್ಣಾಂಕ follow,
			    ಪೂರ್ಣಾಂಕ link_ಎಣಿಕೆ, ಅಕ್ಷರ *buf, ext2_ino_t *res_inode);

ಸ್ಥಿರ errcode_t follow_link(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t dir,
			     ext2_ino_t inode, ಪೂರ್ಣಾಂಕ link_ಎಣಿಕೆ,
			     ಅಕ್ಷರ *buf, ext2_ino_t *res_inode)
{
	ಅಕ್ಷರ *pathname;
	ಅಕ್ಷರ *ಬಫರ್ = 0;
	errcode_t retval;
	ರಚನೆ ext2_inode ei;
	blk64_t blk;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NAMEI_DEBUG
	ಬರೆ("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       ಬೇರು, dir, inode, link_ಎಣಿಕೆ);

#ಆದರೆ_ಅಂತ್ಯ
	retval = ext2fs_read_inode (fs, inode, &ei);
	ಆದರೆ (retval) ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (!LINUX_S_ISLNK (ei.i_ಬಹುಲಕ)) {
		*res_inode = inode;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (link_ಎಣಿಕೆ++ >= EXT2FS_MAX_NESTED_LINKS)
		ಹಿಂತಿರುಗಿ EXT2_ET_SYMLINK_LOOP;

	ಆದರೆ (ext2fs_is_fast_symlink(&ei))
		pathname = (ಅಕ್ಷರ *)&(ei.i_ಖಂಡ[0]);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ei.i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL) {
		retval = ext2fs_get_memzero(ei.i_ಗಾತ್ರ, &ಬಫರ್);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		retval = ext2fs_inline_ದತ್ತಾಂಶ_get(fs, inode,
						&ei, ಬಫರ್, ಖಾಲಿ);
		ಆದರೆ (retval) {
			ext2fs_free_mem(&ಬಫರ್);
			ಹಿಂತಿರುಗಿ retval;
		}
		pathname = ಬಫರ್;
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_bmap2(fs, inode, &ei, ಖಾಲಿ, 0, 0, ಖಾಲಿ, &blk);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		retval = ext2fs_get_mem(fs->blocksize, &ಬಫರ್);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, ಬಫರ್);
		ಆದರೆ (retval) {
			ext2fs_free_mem(&ಬಫರ್);
			ಹಿಂತಿರುಗಿ retval;
		}
		pathname = ಬಫರ್;
	}

	retval = open_namei(fs, ಬೇರು, dir, pathname, ei.i_ಗಾತ್ರ, 1,
			    link_ಎಣಿಕೆ, buf, res_inode);
	ಆದರೆ (ಬಫರ್)
		ext2fs_free_mem(&ಬಫರ್);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
ಸ್ಥಿರ errcode_t dir_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t dir,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಪೂರ್ಣಾಂಕ pathlen,
			   ಪೂರ್ಣಾಂಕ link_ಎಣಿಕೆ, ಅಕ್ಷರ *buf,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **name, ಪೂರ್ಣಾಂಕ *namelen,
			   ext2_ino_t *res_inode)
{
	ಅಕ್ಷರ c;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *thisname;
	ಪೂರ್ಣಾಂಕ len;
	ext2_ino_t inode;
	errcode_t retval;

	ಆದರೆ ((c = *pathname) == '/') {
        	dir = ಬೇರು;
		pathname++;
		pathlen--;
	}
	ಆಗ (1) {
        	thisname = pathname;
		ಗಾಗಿ (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			ಆದರೆ (c == '/')
				ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (pathlen < 0)
			ನಿಲ್ಲಿಸು;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		ಆದರೆ (retval) ಹಿಂತಿರುಗಿ retval;
        	retval = follow_link (fs, ಬೇರು, dir, inode,
				      link_ಎಣಿಕೆ, buf, &dir);
        	ಆದರೆ (retval) ಹಿಂತಿರುಗಿ retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t open_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t ತಳ,
			    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, size_t pathlen, ಪೂರ್ಣಾಂಕ follow,
			    ಪೂರ್ಣಾಂಕ link_ಎಣಿಕೆ, ಅಕ್ಷರ *buf, ext2_ino_t *res_inode)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತಳ_name;
	ಪೂರ್ಣಾಂಕ namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NAMEI_DEBUG
	ಬರೆ("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       ಬೇರು, ತಳ, pathlen, pathname, link_ಎಣಿಕೆ);
#ಆದರೆ_ಅಂತ್ಯ
	retval = dir_namei(fs, ಬೇರು, ತಳ, pathname, pathlen,
			   link_ಎಣಿಕೆ, buf, &ತಳ_name, &namelen, &dir);
	ಆದರೆ (retval) ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		ಹಿಂತಿರುಗಿ 0;
	}
	retval = ext2fs_lookup (fs, dir, ತಳ_name, namelen, buf, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (follow) {
		retval = follow_link(fs, ಬೇರು, dir, inode, link_ಎಣಿಕೆ,
				     buf, &inode);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NAMEI_DEBUG
	ಬರೆ("open_namei: (link_count=%d) returns %lu\n",
	       link_ಎಣಿಕೆ, inode);
#ಆದರೆ_ಅಂತ್ಯ
	*res_inode = inode;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ext2_ino_t *inode)
{
	ಅಕ್ಷರ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = open_namei(fs, ಬೇರು, cwd, name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ext2_ino_t *inode)
{
	ಅಕ್ಷರ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = open_namei(fs, ಬೇರು, cwd, name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	ಅಕ್ಷರ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = follow_link(fs, ಬೇರು, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
}

