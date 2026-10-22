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
#만약 HAVE_UNISTD_H
#include <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

static errcode_t open_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t base,
			    const char *pathname, size_t pathlen, 정수 follow,
			    정수 link_count, char *buf, ext2_ino_t *res_inode);

static errcode_t follow_link(ext2_filsys fs, ext2_ino_t root, ext2_ino_t dir,
			     ext2_ino_t inode, 정수 link_count,
			     char *buf, ext2_ino_t *res_inode)
{
	char *pathname;
	char *buffer = 0;
	errcode_t retval;
	struct ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       root, dir, inode, link_count);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	만약 (retval) 반환 retval;
	만약 (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		반환 0;
	}
	만약 (link_count++ >= EXT2FS_MAX_NESTED_LINKS)
		반환 EXT2_ET_SYMLINK_LOOP;

	만약 (ext2fs_is_fast_symlink(&ei))
		pathname = (char *)&(ei.i_block[0]);
	아니면 만약 (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		만약 (retval)
			반환 retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, NULL);
		만약 (retval) {
			ext2fs_free_mem(&buffer);
			반환 retval;
		}
		pathname = buffer;
	} 아니면 {
		retval = ext2fs_bmap2(fs, inode, &ei, NULL, 0, 0, NULL, &blk);
		만약 (retval)
			반환 retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		만약 (retval)
			반환 retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		만약 (retval) {
			ext2fs_free_mem(&buffer);
			반환 retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, root, dir, pathname, ei.i_size, 1,
			    link_count, buf, res_inode);
	만약 (buffer)
		ext2fs_free_mem(&buffer);
	반환 retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
static errcode_t dir_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t dir,
			   const char *pathname, 정수 pathlen,
			   정수 link_count, char *buf,
			   const char **name, 정수 *namelen,
			   ext2_ino_t *res_inode)
{
	char c;
	const char *thisname;
	정수 len;
	ext2_ino_t inode;
	errcode_t retval;

	만약 ((c = *pathname) == '/') {
        	dir = root;
		pathname++;
		pathlen--;
	}
	동안 (1) {
        	thisname = pathname;
		위해 (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			만약 (c == '/')
				break;
		}
		만약 (pathlen < 0)
			break;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		만약 (retval) 반환 retval;
        	retval = follow_link (fs, root, dir, inode,
				      link_count, buf, &dir);
        	만약 (retval) 반환 retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	반환 0;
}

static errcode_t open_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t base,
			    const char *pathname, size_t pathlen, 정수 follow,
			    정수 link_count, char *buf, ext2_ino_t *res_inode)
{
	const char *base_name;
	정수 namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       root, base, pathlen, pathname, link_count);
#endif
	retval = dir_namei(fs, root, base, pathname, pathlen,
			   link_count, buf, &base_name, &namelen, &dir);
	만약 (retval) 반환 retval;
	만약 (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		반환 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	만약 (retval)
		반환 retval;
	만약 (follow) {
		retval = follow_link(fs, root, dir, inode, link_count,
				     buf, &inode);
		만약 (retval)
			반환 retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_count, inode);
#endif
	*res_inode = inode;
	반환 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t cwd,
		       const char *name, ext2_ino_t *inode)
{
	char *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	만약 (retval)
		반환 retval;

	retval = open_namei(fs, root, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	반환 retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t root, ext2_ino_t cwd,
			      const char *name, ext2_ino_t *inode)
{
	char *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	만약 (retval)
		반환 retval;

	retval = open_namei(fs, root, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	반환 retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t root, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	char *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	만약 (retval)
		반환 retval;

	retval = follow_link(fs, root, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	반환 retval;
}

