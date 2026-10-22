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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ایستا errcode_t open_namei(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t base,
			    ثابت نویسه *pathname, size_t pathlen, صحیح follow,
			    صحیح link_شمار, نویسه *buf, ext2_ino_t *res_inode);

ایستا errcode_t follow_link(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t dir,
			     ext2_ino_t inode, صحیح link_شمار,
			     نویسه *buf, ext2_ino_t *res_inode)
{
	نویسه *pathname;
	نویسه *buffer = 0;
	errcode_t retval;
	ساختار ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       ریشه, dir, inode, link_شمار);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	اگر (retval) بازگشت retval;
	اگر (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		بازگشت 0;
	}
	اگر (link_شمار++ >= EXT2FS_MAX_NESTED_LINKS)
		بازگشت EXT2_ET_SYMLINK_LOOP;

	اگر (ext2fs_is_fast_symlink(&ei))
		pathname = (نویسه *)&(ei.i_block[0]);
	وگرنه اگر (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		اگر (retval)
			بازگشت retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, پوچ);
		اگر (retval) {
			ext2fs_free_mem(&buffer);
			بازگشت retval;
		}
		pathname = buffer;
	} وگرنه {
		retval = ext2fs_bmap2(fs, inode, &ei, پوچ, 0, 0, پوچ, &blk);
		اگر (retval)
			بازگشت retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		اگر (retval)
			بازگشت retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		اگر (retval) {
			ext2fs_free_mem(&buffer);
			بازگشت retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, ریشه, dir, pathname, ei.i_size, 1,
			    link_شمار, buf, res_inode);
	اگر (buffer)
		ext2fs_free_mem(&buffer);
	بازگشت retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
ایستا errcode_t dir_namei(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t dir,
			   ثابت نویسه *pathname, صحیح pathlen,
			   صحیح link_شمار, نویسه *buf,
			   ثابت نویسه **name, صحیح *namelen,
			   ext2_ino_t *res_inode)
{
	نویسه c;
	ثابت نویسه *thisname;
	صحیح len;
	ext2_ino_t inode;
	errcode_t retval;

	اگر ((c = *pathname) == '/') {
        	dir = ریشه;
		pathname++;
		pathlen--;
	}
	وقتی (1) {
        	thisname = pathname;
		برای (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			اگر (c == '/')
				بشکن;
		}
		اگر (pathlen < 0)
			بشکن;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		اگر (retval) بازگشت retval;
        	retval = follow_link (fs, ریشه, dir, inode,
				      link_شمار, buf, &dir);
        	اگر (retval) بازگشت retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	بازگشت 0;
}

ایستا errcode_t open_namei(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t base,
			    ثابت نویسه *pathname, size_t pathlen, صحیح follow,
			    صحیح link_شمار, نویسه *buf, ext2_ino_t *res_inode)
{
	ثابت نویسه *base_name;
	صحیح namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       ریشه, base, pathlen, pathname, link_شمار);
#endif
	retval = dir_namei(fs, ریشه, base, pathname, pathlen,
			   link_شمار, buf, &base_name, &namelen, &dir);
	اگر (retval) بازگشت retval;
	اگر (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		بازگشت 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	اگر (retval)
		بازگشت retval;
	اگر (follow) {
		retval = follow_link(fs, ریشه, dir, inode, link_شمار,
				     buf, &inode);
		اگر (retval)
			بازگشت retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_شمار, inode);
#endif
	*res_inode = inode;
	بازگشت 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t cwd,
		       ثابت نویسه *name, ext2_ino_t *inode)
{
	نویسه *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		بازگشت retval;

	retval = open_namei(fs, ریشه, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	بازگشت retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t cwd,
			      ثابت نویسه *name, ext2_ino_t *inode)
{
	نویسه *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		بازگشت retval;

	retval = open_namei(fs, ریشه, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	بازگشت retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	نویسه *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		بازگشت retval;

	retval = follow_link(fs, ریشه, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	بازگشت retval;
}

