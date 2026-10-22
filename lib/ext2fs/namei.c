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
#اگر_ختم

/* #define NAMEI_DEBUG */

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساکن errcode_t open_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t بنیاد,
			    ثابت حرف *pathname, size_t pathlen, صحیح follow,
			    صحیح link_گنتی, حرف *buf, ext2_ino_t *res_inode);

ساکن errcode_t follow_link(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t dir,
			     ext2_ino_t inode, صحیح link_گنتی,
			     حرف *buf, ext2_ino_t *res_inode)
{
	حرف *pathname;
	حرف *بفر = 0;
	errcode_t retval;
	ساخت ext2_inode ei;
	blk64_t blk;

#اگر_تعریف_شدہ NAMEI_DEBUG
	لکھو("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       جڑ, dir, inode, link_گنتی);

#اگر_ختم
	retval = ext2fs_read_inode (fs, inode, &ei);
	اگر (retval) واپس retval;
	اگر (!LINUX_S_ISLNK (ei.i_منوال)) {
		*res_inode = inode;
		واپس 0;
	}
	اگر (link_گنتی++ >= EXT2FS_MAX_NESTED_LINKS)
		واپس EXT2_ET_SYMLINK_LOOP;

	اگر (ext2fs_is_fast_symlink(&ei))
		pathname = (حرف *)&(ei.i_بلاک[0]);
	ورنہ اگر (ei.i_flags & EXT4_INLINE_ڈیٹا_FL) {
		retval = ext2fs_get_memzero(ei.i_سائز, &بفر);
		اگر (retval)
			واپس retval;

		retval = ext2fs_inline_ڈیٹا_get(fs, inode,
						&ei, بفر, تہی);
		اگر (retval) {
			ext2fs_free_mem(&بفر);
			واپس retval;
		}
		pathname = بفر;
	} ورنہ {
		retval = ext2fs_bmap2(fs, inode, &ei, تہی, 0, 0, تہی, &blk);
		اگر (retval)
			واپس retval;

		retval = ext2fs_get_mem(fs->blocksize, &بفر);
		اگر (retval)
			واپس retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, بفر);
		اگر (retval) {
			ext2fs_free_mem(&بفر);
			واپس retval;
		}
		pathname = بفر;
	}

	retval = open_namei(fs, جڑ, dir, pathname, ei.i_سائز, 1,
			    link_گنتی, buf, res_inode);
	اگر (بفر)
		ext2fs_free_mem(&بفر);
	واپس retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
ساکن errcode_t dir_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t dir,
			   ثابت حرف *pathname, صحیح pathlen,
			   صحیح link_گنتی, حرف *buf,
			   ثابت حرف **name, صحیح *namelen,
			   ext2_ino_t *res_inode)
{
	حرف c;
	ثابت حرف *thisname;
	صحیح len;
	ext2_ino_t inode;
	errcode_t retval;

	اگر ((c = *pathname) == '/') {
        	dir = جڑ;
		pathname++;
		pathlen--;
	}
	جب (1) {
        	thisname = pathname;
		برائے (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			اگر (c == '/')
				توڑو;
		}
		اگر (pathlen < 0)
			توڑو;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		اگر (retval) واپس retval;
        	retval = follow_link (fs, جڑ, dir, inode,
				      link_گنتی, buf, &dir);
        	اگر (retval) واپس retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	واپس 0;
}

ساکن errcode_t open_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t بنیاد,
			    ثابت حرف *pathname, size_t pathlen, صحیح follow,
			    صحیح link_گنتی, حرف *buf, ext2_ino_t *res_inode)
{
	ثابت حرف *بنیاد_name;
	صحیح namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#اگر_تعریف_شدہ NAMEI_DEBUG
	لکھو("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       جڑ, بنیاد, pathlen, pathname, link_گنتی);
#اگر_ختم
	retval = dir_namei(fs, جڑ, بنیاد, pathname, pathlen,
			   link_گنتی, buf, &بنیاد_name, &namelen, &dir);
	اگر (retval) واپس retval;
	اگر (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		واپس 0;
	}
	retval = ext2fs_lookup (fs, dir, بنیاد_name, namelen, buf, &inode);
	اگر (retval)
		واپس retval;
	اگر (follow) {
		retval = follow_link(fs, جڑ, dir, inode, link_گنتی,
				     buf, &inode);
		اگر (retval)
			واپس retval;
	}
#اگر_تعریف_شدہ NAMEI_DEBUG
	لکھو("open_namei: (link_count=%d) returns %lu\n",
	       link_گنتی, inode);
#اگر_ختم
	*res_inode = inode;
	واپس 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
		       ثابت حرف *name, ext2_ino_t *inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		واپس retval;

	retval = open_namei(fs, جڑ, cwd, name, لڑی_لمبائی(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	واپس retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			      ثابت حرف *name, ext2_ino_t *inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		واپس retval;

	retval = open_namei(fs, جڑ, cwd, name, لڑی_لمبائی(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	واپس retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	اگر (retval)
		واپس retval;

	retval = follow_link(fs, جڑ, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	واپس retval;
}

