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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ errcode_t open_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t base,
			    ಧ್ರುವ ವರ್ಣ *pathname, size_t pathlen, ಪೂರ್ಣಾಙ್ಕ follow,
			    ಪೂರ್ಣಾಙ್ಕ link_ಎಣಿಕೆ, ವರ್ಣ *buf, ext2_ino_t *res_inode);

ಸ್ಥಿರ errcode_t follow_link(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t dir,
			     ext2_ino_t inode, ಪೂರ್ಣಾಙ್ಕ link_ಎಣಿಕೆ,
			     ವರ್ಣ *buf, ext2_ino_t *res_inode)
{
	ವರ್ಣ *pathname;
	ವರ್ಣ *buffer = 0;
	errcode_t retval;
	ಸಂರಚನಾ ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       ಬೇರು, dir, inode, link_ಎಣಿಕೆ);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	ಯದಿ (retval) ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (link_ಎಣಿಕೆ++ >= EXT2FS_MAX_NESTED_LINKS)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_SYMLINK_LOOP;

	ಯದಿ (ext2fs_is_fast_symlink(&ei))
		pathname = (ವರ್ಣ *)&(ei.i_block[0]);
	ಅನ್ಯಥಾ ಯದಿ (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, ರಿಕ್ತ);
		ಯದಿ (retval) {
			ext2fs_free_mem(&buffer);
			ಪ್ರತ್ಯಾಗಮ retval;
		}
		pathname = buffer;
	} ಅನ್ಯಥಾ {
		retval = ext2fs_bmap2(fs, inode, &ei, ರಿಕ್ತ, 0, 0, ರಿಕ್ತ, &blk);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		ಯದಿ (retval) {
			ext2fs_free_mem(&buffer);
			ಪ್ರತ್ಯಾಗಮ retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, ಬೇರು, dir, pathname, ei.i_size, 1,
			    link_ಎಣಿಕೆ, buf, res_inode);
	ಯದಿ (buffer)
		ext2fs_free_mem(&buffer);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
ಸ್ಥಿರ errcode_t dir_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t dir,
			   ಧ್ರುವ ವರ್ಣ *pathname, ಪೂರ್ಣಾಙ್ಕ pathlen,
			   ಪೂರ್ಣಾಙ್ಕ link_ಎಣಿಕೆ, ವರ್ಣ *buf,
			   ಧ್ರುವ ವರ್ಣ **name, ಪೂರ್ಣಾಙ್ಕ *namelen,
			   ext2_ino_t *res_inode)
{
	ವರ್ಣ c;
	ಧ್ರುವ ವರ್ಣ *thisname;
	ಪೂರ್ಣಾಙ್ಕ len;
	ext2_ino_t inode;
	errcode_t retval;

	ಯದಿ ((c = *pathname) == '/') {
        	dir = ಬೇರು;
		pathname++;
		pathlen--;
	}
	ಯಾವತ್ (1) {
        	thisname = pathname;
		ಪ್ರತಿ (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			ಯದಿ (c == '/')
				ಭಙ್ಗ;
		}
		ಯದಿ (pathlen < 0)
			ಭಙ್ಗ;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		ಯದಿ (retval) ಪ್ರತ್ಯಾಗಮ retval;
        	retval = follow_link (fs, ಬೇರು, dir, inode,
				      link_ಎಣಿಕೆ, buf, &dir);
        	ಯದಿ (retval) ಪ್ರತ್ಯಾಗಮ retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t open_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t base,
			    ಧ್ರುವ ವರ್ಣ *pathname, size_t pathlen, ಪೂರ್ಣಾಙ್ಕ follow,
			    ಪೂರ್ಣಾಙ್ಕ link_ಎಣಿಕೆ, ವರ್ಣ *buf, ext2_ino_t *res_inode)
{
	ಧ್ರುವ ವರ್ಣ *base_name;
	ಪೂರ್ಣಾಙ್ಕ namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       ಬೇರು, base, pathlen, pathname, link_ಎಣಿಕೆ);
#endif
	retval = dir_namei(fs, ಬೇರು, base, pathname, pathlen,
			   link_ಎಣಿಕೆ, buf, &base_name, &namelen, &dir);
	ಯದಿ (retval) ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (follow) {
		retval = follow_link(fs, ಬೇರು, dir, inode, link_ಎಣಿಕೆ,
				     buf, &inode);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_ಎಣಿಕೆ, inode);
#endif
	*res_inode = inode;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
		       ಧ್ರುವ ವರ್ಣ *name, ext2_ino_t *inode)
{
	ವರ್ಣ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	retval = open_namei(fs, ಬೇರು, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			      ಧ್ರುವ ವರ್ಣ *name, ext2_ino_t *inode)
{
	ವರ್ಣ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	retval = open_namei(fs, ಬೇರು, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ಬೇರು, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	ವರ್ಣ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	retval = follow_link(fs, ಬೇರು, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

