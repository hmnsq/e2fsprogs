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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સ્થિર errcode_t open_namei(ext2_filsys fs, ext2_ino_t મૂળ, ext2_ino_t base,
			    અચલ અક્ષર *pathname, size_t pathlen, પૂર્ણાંક follow,
			    પૂર્ણાંક link_ગણતરી, અક્ષર *buf, ext2_ino_t *res_inode);

સ્થિર errcode_t follow_link(ext2_filsys fs, ext2_ino_t મૂળ, ext2_ino_t dir,
			     ext2_ino_t inode, પૂર્ણાંક link_ગણતરી,
			     અક્ષર *buf, ext2_ino_t *res_inode)
{
	અક્ષર *pathname;
	અક્ષર *buffer = 0;
	errcode_t retval;
	સંરચના ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       મૂળ, dir, inode, link_ગણતરી);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	જો (retval) પાછા retval;
	જો (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		પાછા 0;
	}
	જો (link_ગણતરી++ >= EXT2FS_MAX_NESTED_LINKS)
		પાછા EXT2_ET_SYMLINK_LOOP;

	જો (ext2fs_is_fast_symlink(&ei))
		pathname = (અક્ષર *)&(ei.i_block[0]);
	નહીંતર જો (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		જો (retval)
			પાછા retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, ખાલી);
		જો (retval) {
			ext2fs_free_mem(&buffer);
			પાછા retval;
		}
		pathname = buffer;
	} નહીંતર {
		retval = ext2fs_bmap2(fs, inode, &ei, ખાલી, 0, 0, ખાલી, &blk);
		જો (retval)
			પાછા retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		જો (retval)
			પાછા retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		જો (retval) {
			ext2fs_free_mem(&buffer);
			પાછા retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, મૂળ, dir, pathname, ei.i_size, 1,
			    link_ગણતરી, buf, res_inode);
	જો (buffer)
		ext2fs_free_mem(&buffer);
	પાછા retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
સ્થિર errcode_t dir_namei(ext2_filsys fs, ext2_ino_t મૂળ, ext2_ino_t dir,
			   અચલ અક્ષર *pathname, પૂર્ણાંક pathlen,
			   પૂર્ણાંક link_ગણતરી, અક્ષર *buf,
			   અચલ અક્ષર **name, પૂર્ણાંક *namelen,
			   ext2_ino_t *res_inode)
{
	અક્ષર c;
	અચલ અક્ષર *thisname;
	પૂર્ણાંક len;
	ext2_ino_t inode;
	errcode_t retval;

	જો ((c = *pathname) == '/') {
        	dir = મૂળ;
		pathname++;
		pathlen--;
	}
	જ્યારે (1) {
        	thisname = pathname;
		માટે (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			જો (c == '/')
				તોડો;
		}
		જો (pathlen < 0)
			તોડો;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		જો (retval) પાછા retval;
        	retval = follow_link (fs, મૂળ, dir, inode,
				      link_ગણતરી, buf, &dir);
        	જો (retval) પાછા retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	પાછા 0;
}

સ્થિર errcode_t open_namei(ext2_filsys fs, ext2_ino_t મૂળ, ext2_ino_t base,
			    અચલ અક્ષર *pathname, size_t pathlen, પૂર્ણાંક follow,
			    પૂર્ણાંક link_ગણતરી, અક્ષર *buf, ext2_ino_t *res_inode)
{
	અચલ અક્ષર *base_name;
	પૂર્ણાંક namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       મૂળ, base, pathlen, pathname, link_ગણતરી);
#endif
	retval = dir_namei(fs, મૂળ, base, pathname, pathlen,
			   link_ગણતરી, buf, &base_name, &namelen, &dir);
	જો (retval) પાછા retval;
	જો (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		પાછા 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	જો (retval)
		પાછા retval;
	જો (follow) {
		retval = follow_link(fs, મૂળ, dir, inode, link_ગણતરી,
				     buf, &inode);
		જો (retval)
			પાછા retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_ગણતરી, inode);
#endif
	*res_inode = inode;
	પાછા 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t મૂળ, ext2_ino_t cwd,
		       અચલ અક્ષર *name, ext2_ino_t *inode)
{
	અક્ષર *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	જો (retval)
		પાછા retval;

	retval = open_namei(fs, મૂળ, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	પાછા retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t મૂળ, ext2_ino_t cwd,
			      અચલ અક્ષર *name, ext2_ino_t *inode)
{
	અક્ષર *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	જો (retval)
		પાછા retval;

	retval = open_namei(fs, મૂળ, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	પાછા retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t મૂળ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	અક્ષર *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	જો (retval)
		પાછા retval;

	retval = follow_link(fs, મૂળ, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	પાછા retval;
}

