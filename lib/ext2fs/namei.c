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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

static errcode_t open_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t base,
			    const var_na *pathname, size_t pathlen, ចំនួនគត់ follow,
			    ចំនួនគត់ link_count, var_na *buf, ext2_ino_t *res_inode);

static errcode_t follow_link(ext2_filsys fs, ext2_ino_t root, ext2_ino_t dir,
			     ext2_ino_t inode, ចំនួនគត់ link_count,
			     var_na *buf, ext2_ino_t *res_inode)
{
	var_na *pathname;
	var_na *buffer = 0;
	errcode_t retval;
	sa_nracanaa ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       root, dir, inode, link_count);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	ប្រសិនបើ (retval) ត្រឡប់ retval;
	ប្រសិនបើ (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		ត្រឡប់ 0;
	}
	ប្រសិនបើ (link_count++ >= EXT2FS_MAX_NESTED_LINKS)
		ត្រឡប់ EXT2_ET_SYMLINK_LOOP;

	ប្រសិនបើ (ext2fs_is_fast_symlink(&ei))
		pathname = (var_na *)&(ei.i_block[0]);
	បើមិនដូច្នេះ ប្រសិនបើ (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		ប្រសិនបើ (retval)
			ត្រឡប់ retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, NULL);
		ប្រសិនបើ (retval) {
			ext2fs_free_mem(&buffer);
			ត្រឡប់ retval;
		}
		pathname = buffer;
	} បើមិនដូច្នេះ {
		retval = ext2fs_bmap2(fs, inode, &ei, NULL, 0, 0, NULL, &blk);
		ប្រសិនបើ (retval)
			ត្រឡប់ retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		ប្រសិនបើ (retval)
			ត្រឡប់ retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		ប្រសិនបើ (retval) {
			ext2fs_free_mem(&buffer);
			ត្រឡប់ retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, root, dir, pathname, ei.i_size, 1,
			    link_count, buf, res_inode);
	ប្រសិនបើ (buffer)
		ext2fs_free_mem(&buffer);
	ត្រឡប់ retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
static errcode_t dir_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t dir,
			   const var_na *pathname, ចំនួនគត់ pathlen,
			   ចំនួនគត់ link_count, var_na *buf,
			   const var_na **name, ចំនួនគត់ *namelen,
			   ext2_ino_t *res_inode)
{
	var_na c;
	const var_na *thisname;
	ចំនួនគត់ len;
	ext2_ino_t inode;
	errcode_t retval;

	ប្រសិនបើ ((c = *pathname) == '/') {
        	dir = root;
		pathname++;
		pathlen--;
	}
	ខណៈពេល (1) {
        	thisname = pathname;
		សម្រាប់ (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			ប្រសិនបើ (c == '/')
				bha_nga;
		}
		ប្រសិនបើ (pathlen < 0)
			bha_nga;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		ប្រសិនបើ (retval) ត្រឡប់ retval;
        	retval = follow_link (fs, root, dir, inode,
				      link_count, buf, &dir);
        	ប្រសិនបើ (retval) ត្រឡប់ retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	ត្រឡប់ 0;
}

static errcode_t open_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t base,
			    const var_na *pathname, size_t pathlen, ចំនួនគត់ follow,
			    ចំនួនគត់ link_count, var_na *buf, ext2_ino_t *res_inode)
{
	const var_na *base_name;
	ចំនួនគត់ namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       root, base, pathlen, pathname, link_count);
#endif
	retval = dir_namei(fs, root, base, pathname, pathlen,
			   link_count, buf, &base_name, &namelen, &dir);
	ប្រសិនបើ (retval) ត្រឡប់ retval;
	ប្រសិនបើ (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		ត្រឡប់ 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;
	ប្រសិនបើ (follow) {
		retval = follow_link(fs, root, dir, inode, link_count,
				     buf, &inode);
		ប្រសិនបើ (retval)
			ត្រឡប់ retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_count, inode);
#endif
	*res_inode = inode;
	ត្រឡប់ 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t cwd,
		       const var_na *name, ext2_ino_t *inode)
{
	var_na *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	retval = open_namei(fs, root, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ត្រឡប់ retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t root, ext2_ino_t cwd,
			      const var_na *name, ext2_ino_t *inode)
{
	var_na *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	retval = open_namei(fs, root, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ត្រឡប់ retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t root, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	var_na *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	retval = follow_link(fs, root, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	ត្រឡប់ retval;
}

