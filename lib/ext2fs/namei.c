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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

স্থির errcode_t open_namei(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t base,
			    ধ্রুবক অক্ষর *pathname, size_t pathlen, পূর্ণসংখ্যা follow,
			    পূর্ণসংখ্যা link_গুনতি, অক্ষর *buf, ext2_ino_t *res_inode);

স্থির errcode_t follow_link(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t dir,
			     ext2_ino_t inode, পূর্ণসংখ্যা link_গুনতি,
			     অক্ষর *buf, ext2_ino_t *res_inode)
{
	অক্ষর *pathname;
	অক্ষর *বাফার = 0;
	errcode_t retval;
	কাঠামো ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       মূল, dir, inode, link_গুনতি);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	যদি (retval) ফেরত retval;
	যদি (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		ফেরত 0;
	}
	যদি (link_গুনতি++ >= EXT2FS_MAX_NESTED_LINKS)
		ফেরত EXT2_ET_SYMLINK_LOOP;

	যদি (ext2fs_is_fast_symlink(&ei))
		pathname = (অক্ষর *)&(ei.i_block[0]);
	অন্যথা যদি (ei.i_flags & EXT4_INLINE_তথ্য_FL) {
		retval = ext2fs_get_memzero(ei.i_আকার, &বাফার);
		যদি (retval)
			ফেরত retval;

		retval = ext2fs_inline_তথ্য_get(fs, inode,
						&ei, বাফার, ফাঁকা);
		যদি (retval) {
			ext2fs_free_mem(&বাফার);
			ফেরত retval;
		}
		pathname = বাফার;
	} অন্যথা {
		retval = ext2fs_bmap2(fs, inode, &ei, ফাঁকা, 0, 0, ফাঁকা, &blk);
		যদি (retval)
			ফেরত retval;

		retval = ext2fs_get_mem(fs->blocksize, &বাফার);
		যদি (retval)
			ফেরত retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, বাফার);
		যদি (retval) {
			ext2fs_free_mem(&বাফার);
			ফেরত retval;
		}
		pathname = বাফার;
	}

	retval = open_namei(fs, মূল, dir, pathname, ei.i_আকার, 1,
			    link_গুনতি, buf, res_inode);
	যদি (বাফার)
		ext2fs_free_mem(&বাফার);
	ফেরত retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
স্থির errcode_t dir_namei(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t dir,
			   ধ্রুবক অক্ষর *pathname, পূর্ণসংখ্যা pathlen,
			   পূর্ণসংখ্যা link_গুনতি, অক্ষর *buf,
			   ধ্রুবক অক্ষর **name, পূর্ণসংখ্যা *namelen,
			   ext2_ino_t *res_inode)
{
	অক্ষর c;
	ধ্রুবক অক্ষর *thisname;
	পূর্ণসংখ্যা len;
	ext2_ino_t inode;
	errcode_t retval;

	যদি ((c = *pathname) == '/') {
        	dir = মূল;
		pathname++;
		pathlen--;
	}
	যখন (1) {
        	thisname = pathname;
		জন্য (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			যদি (c == '/')
				বিরতি;
		}
		যদি (pathlen < 0)
			বিরতি;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		যদি (retval) ফেরত retval;
        	retval = follow_link (fs, মূল, dir, inode,
				      link_গুনতি, buf, &dir);
        	যদি (retval) ফেরত retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	ফেরত 0;
}

স্থির errcode_t open_namei(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t base,
			    ধ্রুবক অক্ষর *pathname, size_t pathlen, পূর্ণসংখ্যা follow,
			    পূর্ণসংখ্যা link_গুনতি, অক্ষর *buf, ext2_ino_t *res_inode)
{
	ধ্রুবক অক্ষর *base_name;
	পূর্ণসংখ্যা namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       মূল, base, pathlen, pathname, link_গুনতি);
#endif
	retval = dir_namei(fs, মূল, base, pathname, pathlen,
			   link_গুনতি, buf, &base_name, &namelen, &dir);
	যদি (retval) ফেরত retval;
	যদি (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		ফেরত 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	যদি (retval)
		ফেরত retval;
	যদি (follow) {
		retval = follow_link(fs, মূল, dir, inode, link_গুনতি,
				     buf, &inode);
		যদি (retval)
			ফেরত retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_গুনতি, inode);
#endif
	*res_inode = inode;
	ফেরত 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t cwd,
		       ধ্রুবক অক্ষর *name, ext2_ino_t *inode)
{
	অক্ষর *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ফেরত retval;

	retval = open_namei(fs, মূল, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ফেরত retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t cwd,
			      ধ্রুবক অক্ষর *name, ext2_ino_t *inode)
{
	অক্ষর *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ফেরত retval;

	retval = open_namei(fs, মূল, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ফেরত retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t মূল, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	অক্ষর *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ফেরত retval;

	retval = follow_link(fs, মূল, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	ফেরত retval;
}

