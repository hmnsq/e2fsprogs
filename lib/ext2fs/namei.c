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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

স্থির errcode_t open_namei(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t base,
			    ধ্রুৱ ৱর্ণ *pathname, size_t pathlen, পূর্ণাঙ্ক follow,
			    পূর্ণাঙ্ক link_গুনতি, ৱর্ণ *buf, ext2_ino_t *res_inode);

স্থির errcode_t follow_link(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t dir,
			     ext2_ino_t inode, পূর্ণাঙ্ক link_গুনতি,
			     ৱর্ণ *buf, ext2_ino_t *res_inode)
{
	ৱর্ণ *pathname;
	ৱর্ণ *buffer = 0;
	errcode_t retval;
	সংরচনা ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       মূল, dir, inode, link_গুনতি);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	যদি (retval) ঘূৰাই retval;
	যদি (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		ঘূৰাই 0;
	}
	যদি (link_গুনতি++ >= EXT2FS_MAX_NESTED_LINKS)
		ঘূৰাই EXT2_ET_SYMLINK_LOOP;

	যদি (ext2fs_is_fast_symlink(&ei))
		pathname = (ৱর্ণ *)&(ei.i_block[0]);
	অন্যথা যদি (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		যদি (retval)
			ঘূৰাই retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, রিক্ত);
		যদি (retval) {
			ext2fs_free_mem(&buffer);
			ঘূৰাই retval;
		}
		pathname = buffer;
	} অন্যথা {
		retval = ext2fs_bmap2(fs, inode, &ei, রিক্ত, 0, 0, রিক্ত, &blk);
		যদি (retval)
			ঘূৰাই retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		যদি (retval)
			ঘূৰাই retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		যদি (retval) {
			ext2fs_free_mem(&buffer);
			ঘূৰাই retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, মূল, dir, pathname, ei.i_size, 1,
			    link_গুনতি, buf, res_inode);
	যদি (buffer)
		ext2fs_free_mem(&buffer);
	ঘূৰাই retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
স্থির errcode_t dir_namei(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t dir,
			   ধ্রুৱ ৱর্ণ *pathname, পূর্ণাঙ্ক pathlen,
			   পূর্ণাঙ্ক link_গুনতি, ৱর্ণ *buf,
			   ধ্রুৱ ৱর্ণ **name, পূর্ণাঙ্ক *namelen,
			   ext2_ino_t *res_inode)
{
	ৱর্ণ c;
	ধ্রুৱ ৱর্ণ *thisname;
	পূর্ণাঙ্ক len;
	ext2_ino_t inode;
	errcode_t retval;

	যদি ((c = *pathname) == '/') {
        	dir = মূল;
		pathname++;
		pathlen--;
	}
	যেতিয়ালৈ (1) {
        	thisname = pathname;
		বাবে (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			যদি (c == '/')
				ভঙ্গ;
		}
		যদি (pathlen < 0)
			ভঙ্গ;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		যদি (retval) ঘূৰাই retval;
        	retval = follow_link (fs, মূল, dir, inode,
				      link_গুনতি, buf, &dir);
        	যদি (retval) ঘূৰাই retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	ঘূৰাই 0;
}

স্থির errcode_t open_namei(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t base,
			    ধ্রুৱ ৱর্ণ *pathname, size_t pathlen, পূর্ণাঙ্ক follow,
			    পূর্ণাঙ্ক link_গুনতি, ৱর্ণ *buf, ext2_ino_t *res_inode)
{
	ধ্রুৱ ৱর্ণ *base_name;
	পূর্ণাঙ্ক namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       মূল, base, pathlen, pathname, link_গুনতি);
#endif
	retval = dir_namei(fs, মূল, base, pathname, pathlen,
			   link_গুনতি, buf, &base_name, &namelen, &dir);
	যদি (retval) ঘূৰাই retval;
	যদি (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		ঘূৰাই 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	যদি (retval)
		ঘূৰাই retval;
	যদি (follow) {
		retval = follow_link(fs, মূল, dir, inode, link_গুনতি,
				     buf, &inode);
		যদি (retval)
			ঘূৰাই retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_গুনতি, inode);
#endif
	*res_inode = inode;
	ঘূৰাই 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t cwd,
		       ধ্রুৱ ৱর্ণ *name, ext2_ino_t *inode)
{
	ৱর্ণ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ঘূৰাই retval;

	retval = open_namei(fs, মূল, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ঘূৰাই retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t cwd,
			      ধ্রুৱ ৱর্ণ *name, ext2_ino_t *inode)
{
	ৱর্ণ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ঘূৰাই retval;

	retval = open_namei(fs, মূল, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ঘূৰাই retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	ৱর্ণ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ঘূৰাই retval;

	retval = follow_link(fs, মূল, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	ঘূৰাই retval;
}

