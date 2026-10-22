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
#پایان_اگر

/* #define NAMEI_DEBUG */

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

ساكن errcode_t open_namei(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t پایه,
			    ثابت حرف *pathname, size_t pathlen, صحيح follow,
			    صحيح link_شمار, حرف *buf, ext2_ino_t *res_inode);

ساكن errcode_t follow_link(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t dir,
			     ext2_ino_t inode, صحيح link_شمار,
			     حرف *buf, ext2_ino_t *res_inode)
{
	حرف *pathname;
	حرف *میانگیر = 0;
	errcode_t retval;
	بنية ext2_inode ei;
	blk64_t blk;

#اگر_تعریف_شده NAMEI_DEBUG
	چاپ_کن("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       ریشه, dir, inode, link_شمار);

#پایان_اگر
	retval = ext2fs_read_inode (fs, inode, &ei);
	إذا (retval) أرجع retval;
	إذا (!LINUX_S_ISLNK (ei.i_نما)) {
		*res_inode = inode;
		أرجع 0;
	}
	إذا (link_شمار++ >= EXT2FS_MAX_NESTED_LINKS)
		أرجع EXT2_ET_SYMLINK_LOOP;

	إذا (ext2fs_is_fast_symlink(&ei))
		pathname = (حرف *)&(ei.i_بلوک[0]);
	وإلا إذا (ei.i_flags & EXT4_INLINE_داده_FL) {
		retval = ext2fs_get_memzero(ei.i_اندازه, &میانگیر);
		إذا (retval)
			أرجع retval;

		retval = ext2fs_inline_داده_get(fs, inode,
						&ei, میانگیر, عدم);
		إذا (retval) {
			ext2fs_free_mem(&میانگیر);
			أرجع retval;
		}
		pathname = میانگیر;
	} وإلا {
		retval = ext2fs_bmap2(fs, inode, &ei, عدم, 0, 0, عدم, &blk);
		إذا (retval)
			أرجع retval;

		retval = ext2fs_get_mem(fs->blocksize, &میانگیر);
		إذا (retval)
			أرجع retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, میانگیر);
		إذا (retval) {
			ext2fs_free_mem(&میانگیر);
			أرجع retval;
		}
		pathname = میانگیر;
	}

	retval = open_namei(fs, ریشه, dir, pathname, ei.i_اندازه, 1,
			    link_شمار, buf, res_inode);
	إذا (میانگیر)
		ext2fs_free_mem(&میانگیر);
	أرجع retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
ساكن errcode_t dir_namei(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t dir,
			   ثابت حرف *pathname, صحيح pathlen,
			   صحيح link_شمار, حرف *buf,
			   ثابت حرف **name, صحيح *namelen,
			   ext2_ino_t *res_inode)
{
	حرف c;
	ثابت حرف *thisname;
	صحيح len;
	ext2_ino_t inode;
	errcode_t retval;

	إذا ((c = *pathname) == '/') {
        	dir = ریشه;
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
        	retval = follow_link (fs, ریشه, dir, inode,
				      link_شمار, buf, &dir);
        	إذا (retval) أرجع retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	أرجع 0;
}

ساكن errcode_t open_namei(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t پایه,
			    ثابت حرف *pathname, size_t pathlen, صحيح follow,
			    صحيح link_شمار, حرف *buf, ext2_ino_t *res_inode)
{
	ثابت حرف *پایه_name;
	صحيح namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#اگر_تعریف_شده NAMEI_DEBUG
	چاپ_کن("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       ریشه, پایه, pathlen, pathname, link_شمار);
#پایان_اگر
	retval = dir_namei(fs, ریشه, پایه, pathname, pathlen,
			   link_شمار, buf, &پایه_name, &namelen, &dir);
	إذا (retval) أرجع retval;
	إذا (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		أرجع 0;
	}
	retval = ext2fs_lookup (fs, dir, پایه_name, namelen, buf, &inode);
	إذا (retval)
		أرجع retval;
	إذا (follow) {
		retval = follow_link(fs, ریشه, dir, inode, link_شمار,
				     buf, &inode);
		إذا (retval)
			أرجع retval;
	}
#اگر_تعریف_شده NAMEI_DEBUG
	چاپ_کن("open_namei: (link_count=%d) returns %lu\n",
	       link_شمار, inode);
#پایان_اگر
	*res_inode = inode;
	أرجع 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t cwd,
		       ثابت حرف *name, ext2_ino_t *inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;

	retval = open_namei(fs, ریشه, cwd, name, طول_رشته(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	أرجع retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t cwd,
			      ثابت حرف *name, ext2_ino_t *inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;

	retval = open_namei(fs, ریشه, cwd, name, طول_رشته(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	أرجع retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ریشه, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	إذا (retval)
		أرجع retval;

	retval = follow_link(fs, ریشه, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	أرجع retval;
}

