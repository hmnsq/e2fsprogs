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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت

/* #define NAMEI_DEBUG */

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

ساكن errcode_t open_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t ادھار,
			    ثابت حرف *pathname, size_t pathlen, صحيح follow,
			    صحيح link_گنتی, حرف *buf, ext2_ino_t *res_inode);

ساكن errcode_t follow_link(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t dir,
			     ext2_ino_t inode, صحيح link_گنتی,
			     حرف *buf, ext2_ino_t *res_inode)
{
	حرف *pathname;
	حرف *بفر = 0;
	errcode_t retval;
	بنية ext2_inode ei;
	blk64_t blk;

#جے_تعریف_شدہ NAMEI_DEBUG
	لکھو("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       جڑ, dir, inode, link_گنتی);

#جے_انت
	retval = ext2fs_read_inode (fs, inode, &ei);
	إذا (retval) أرجع retval;
	إذا (!LINUX_S_ISLNK (ei.i_منوال)) {
		*res_inode = inode;
		أرجع 0;
	}
	إذا (link_گنتی++ >= EXT2FS_MAX_NESTED_LINKS)
		أرجع EXT2_ET_SYMLINK_LOOP;

	إذا (ext2fs_is_fast_symlink(&ei))
		pathname = (حرف *)&(ei.i_بلاک[0]);
	وإلا إذا (ei.i_flags & EXT4_INLINE_انکڑے_FL) {
		retval = ext2fs_get_memzero(ei.i_ناپ, &بفر);
		إذا (retval)
			أرجع retval;

		retval = ext2fs_inline_انکڑے_get(fs, inode,
						&ei, بفر, عدم);
		إذا (retval) {
			ext2fs_free_mem(&بفر);
			أرجع retval;
		}
		pathname = بفر;
	} وإلا {
		retval = ext2fs_bmap2(fs, inode, &ei, عدم, 0, 0, عدم, &blk);
		إذا (retval)
			أرجع retval;

		retval = ext2fs_get_mem(fs->blocksize, &بفر);
		إذا (retval)
			أرجع retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, بفر);
		إذا (retval) {
			ext2fs_free_mem(&بفر);
			أرجع retval;
		}
		pathname = بفر;
	}

	retval = open_namei(fs, جڑ, dir, pathname, ei.i_ناپ, 1,
			    link_گنتی, buf, res_inode);
	إذا (بفر)
		ext2fs_free_mem(&بفر);
	أرجع retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
ساكن errcode_t dir_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t dir,
			   ثابت حرف *pathname, صحيح pathlen,
			   صحيح link_گنتی, حرف *buf,
			   ثابت حرف **name, صحيح *namelen,
			   ext2_ino_t *res_inode)
{
	حرف c;
	ثابت حرف *thisname;
	صحيح len;
	ext2_ino_t inode;
	errcode_t retval;

	إذا ((c = *pathname) == '/') {
        	dir = جڑ;
		pathname++;
		pathlen--;
	}
	طالما (1) {
        	thisname = pathname;
		لكل (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			إذا (c == '/')
				اكسر;
		}
		إذا (pathlen < 0)
			اكسر;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		إذا (retval) أرجع retval;
        	retval = follow_link (fs, جڑ, dir, inode,
				      link_گنتی, buf, &dir);
        	إذا (retval) أرجع retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	أرجع 0;
}

ساكن errcode_t open_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t ادھار,
			    ثابت حرف *pathname, size_t pathlen, صحيح follow,
			    صحيح link_گنتی, حرف *buf, ext2_ino_t *res_inode)
{
	ثابت حرف *ادھار_name;
	صحيح namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#جے_تعریف_شدہ NAMEI_DEBUG
	لکھو("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       جڑ, ادھار, pathlen, pathname, link_گنتی);
#جے_انت
	retval = dir_namei(fs, جڑ, ادھار, pathname, pathlen,
			   link_گنتی, buf, &ادھار_name, &namelen, &dir);
	إذا (retval) أرجع retval;
	إذا (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		أرجع 0;
	}
	retval = ext2fs_lookup (fs, dir, ادھار_name, namelen, buf, &inode);
	إذا (retval)
		أرجع retval;
	إذا (follow) {
		retval = follow_link(fs, جڑ, dir, inode, link_گنتی,
				     buf, &inode);
		إذا (retval)
			أرجع retval;
	}
#جے_تعریف_شدہ NAMEI_DEBUG
	لکھو("open_namei: (link_count=%d) returns %lu\n",
	       link_گنتی, inode);
#جے_انت
	*res_inode = inode;
	أرجع 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
		       ثابت حرف *name, ext2_ino_t *inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;

	retval = open_namei(fs, جڑ, cwd, name, لڑی_لمبائی(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	أرجع retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			      ثابت حرف *name, ext2_ino_t *inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;

	retval = open_namei(fs, جڑ, cwd, name, لڑی_لمبائی(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	أرجع retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t جڑ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;

	retval = follow_link(fs, جڑ, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	أرجع retval;
}

