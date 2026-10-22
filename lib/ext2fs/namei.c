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
#𐡄𐡍 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

ساكن errcode_t open_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t base,
			    ثابت حرف *pathname, size_t pathlen, صحيح follow,
			    صحيح link_count, حرف *buf, ext2_ino_t *res_inode);

ساكن errcode_t follow_link(ext2_filsys fs, ext2_ino_t root, ext2_ino_t dir,
			     ext2_ino_t inode, صحيح link_count,
			     حرف *buf, ext2_ino_t *res_inode)
{
	حرف *pathname;
	حرف *buffer = 0;
	errcode_t retval;
	بنية ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       root, dir, inode, link_count);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	𐡄𐡍 (retval) أرجع retval;
	𐡄𐡍 (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		أرجع 0;
	}
	𐡄𐡍 (link_count++ >= EXT2FS_MAX_NESTED_LINKS)
		أرجع EXT2_ET_SYMLINK_LOOP;

	𐡄𐡍 (ext2fs_is_fast_symlink(&ei))
		pathname = (حرف *)&(ei.i_block[0]);
	وإلا 𐡄𐡍 (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		𐡄𐡍 (retval)
			أرجع retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, عدم);
		𐡄𐡍 (retval) {
			ext2fs_free_mem(&buffer);
			أرجع retval;
		}
		pathname = buffer;
	} وإلا {
		retval = ext2fs_bmap2(fs, inode, &ei, عدم, 0, 0, عدم, &blk);
		𐡄𐡍 (retval)
			أرجع retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		𐡄𐡍 (retval)
			أرجع retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		𐡄𐡍 (retval) {
			ext2fs_free_mem(&buffer);
			أرجع retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, root, dir, pathname, ei.i_size, 1,
			    link_count, buf, res_inode);
	𐡄𐡍 (buffer)
		ext2fs_free_mem(&buffer);
	أرجع retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
ساكن errcode_t dir_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t dir,
			   ثابت حرف *pathname, صحيح pathlen,
			   صحيح link_count, حرف *buf,
			   ثابت حرف **name, صحيح *namelen,
			   ext2_ino_t *res_inode)
{
	حرف c;
	ثابت حرف *thisname;
	صحيح len;
	ext2_ino_t inode;
	errcode_t retval;

	𐡄𐡍 ((c = *pathname) == '/') {
        	dir = root;
		pathname++;
		pathlen--;
	}
	طالما (1) {
        	thisname = pathname;
		لكل (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			𐡄𐡍 (c == '/')
				اكسر;
		}
		𐡄𐡍 (pathlen < 0)
			اكسر;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		𐡄𐡍 (retval) أرجع retval;
        	retval = follow_link (fs, root, dir, inode,
				      link_count, buf, &dir);
        	𐡄𐡍 (retval) أرجع retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	أرجع 0;
}

ساكن errcode_t open_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t base,
			    ثابت حرف *pathname, size_t pathlen, صحيح follow,
			    صحيح link_count, حرف *buf, ext2_ino_t *res_inode)
{
	ثابت حرف *base_name;
	صحيح namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       root, base, pathlen, pathname, link_count);
#endif
	retval = dir_namei(fs, root, base, pathname, pathlen,
			   link_count, buf, &base_name, &namelen, &dir);
	𐡄𐡍 (retval) أرجع retval;
	𐡄𐡍 (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		أرجع 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	𐡄𐡍 (retval)
		أرجع retval;
	𐡄𐡍 (follow) {
		retval = follow_link(fs, root, dir, inode, link_count,
				     buf, &inode);
		𐡄𐡍 (retval)
			أرجع retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_count, inode);
#endif
	*res_inode = inode;
	أرجع 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t root, ext2_ino_t cwd,
		       ثابت حرف *name, ext2_ino_t *inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	𐡄𐡍 (retval)
		أرجع retval;

	retval = open_namei(fs, root, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	أرجع retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t root, ext2_ino_t cwd,
			      ثابت حرف *name, ext2_ino_t *inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	𐡄𐡍 (retval)
		أرجع retval;

	retval = open_namei(fs, root, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	أرجع retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t root, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	حرف *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	𐡄𐡍 (retval)
		أرجع retval;

	retval = follow_link(fs, root, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	أرجع retval;
}

