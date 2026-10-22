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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूळ, ext2_ino_t base,
			    स्थिरांक अक्षर *pathname, माप_प्रकार pathlen, पूर्णांक follow,
			    पूर्णांक link_मोजणी, अक्षर *buf, ext2_ino_t *res_inode);

स्थिर errcode_t follow_link(ext2_filsys fs, ext2_ino_t मूळ, ext2_ino_t dir,
			     ext2_ino_t inode, पूर्णांक link_मोजणी,
			     अक्षर *buf, ext2_ino_t *res_inode)
{
	अक्षर *pathname;
	अक्षर *buffer = 0;
	errcode_t retval;
	संरचना ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	म_लिखो("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       मूळ, dir, inode, link_मोजणी);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	जर (retval) परत retval;
	जर (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		परत 0;
	}
	जर (link_मोजणी++ >= EXT2FS_MAX_NESTED_LINKS)
		परत EXT2_ET_SYMLINK_LOOP;

	जर (ext2fs_is_fast_symlink(&ei))
		pathname = (अक्षर *)&(ei.i_block[0]);
	अन्यथा जर (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		जर (retval)
			परत retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, रिक्त);
		जर (retval) {
			ext2fs_free_mem(&buffer);
			परत retval;
		}
		pathname = buffer;
	} अन्यथा {
		retval = ext2fs_bmap2(fs, inode, &ei, रिक्त, 0, 0, रिक्त, &blk);
		जर (retval)
			परत retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		जर (retval)
			परत retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		जर (retval) {
			ext2fs_free_mem(&buffer);
			परत retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, मूळ, dir, pathname, ei.i_size, 1,
			    link_मोजणी, buf, res_inode);
	जर (buffer)
		ext2fs_free_mem(&buffer);
	परत retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
स्थिर errcode_t dir_namei(ext2_filsys fs, ext2_ino_t मूळ, ext2_ino_t dir,
			   स्थिरांक अक्षर *pathname, पूर्णांक pathlen,
			   पूर्णांक link_मोजणी, अक्षर *buf,
			   स्थिरांक अक्षर **name, पूर्णांक *namelen,
			   ext2_ino_t *res_inode)
{
	अक्षर c;
	स्थिरांक अक्षर *thisname;
	पूर्णांक len;
	ext2_ino_t inode;
	errcode_t retval;

	जर ((c = *pathname) == '/') {
        	dir = मूळ;
		pathname++;
		pathlen--;
	}
	पर्यंत (1) {
        	thisname = pathname;
		साठी (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			जर (c == '/')
				थांबा;
		}
		जर (pathlen < 0)
			थांबा;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		जर (retval) परत retval;
        	retval = follow_link (fs, मूळ, dir, inode,
				      link_मोजणी, buf, &dir);
        	जर (retval) परत retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	परत 0;
}

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूळ, ext2_ino_t base,
			    स्थिरांक अक्षर *pathname, माप_प्रकार pathlen, पूर्णांक follow,
			    पूर्णांक link_मोजणी, अक्षर *buf, ext2_ino_t *res_inode)
{
	स्थिरांक अक्षर *base_name;
	पूर्णांक namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	म_लिखो("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       मूळ, base, pathlen, pathname, link_मोजणी);
#endif
	retval = dir_namei(fs, मूळ, base, pathname, pathlen,
			   link_मोजणी, buf, &base_name, &namelen, &dir);
	जर (retval) परत retval;
	जर (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		परत 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	जर (retval)
		परत retval;
	जर (follow) {
		retval = follow_link(fs, मूळ, dir, inode, link_मोजणी,
				     buf, &inode);
		जर (retval)
			परत retval;
	}
#ifdef NAMEI_DEBUG
	म_लिखो("open_namei: (link_count=%d) returns %lu\n",
	       link_मोजणी, inode);
#endif
	*res_inode = inode;
	परत 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t मूळ, ext2_ino_t cwd,
		       स्थिरांक अक्षर *name, ext2_ino_t *inode)
{
	अक्षर *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	जर (retval)
		परत retval;

	retval = open_namei(fs, मूळ, cwd, name, म_माप(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	परत retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t मूळ, ext2_ino_t cwd,
			      स्थिरांक अक्षर *name, ext2_ino_t *inode)
{
	अक्षर *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	जर (retval)
		परत retval;

	retval = open_namei(fs, मूळ, cwd, name, म_माप(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	परत retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t मूळ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	अक्षर *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	जर (retval)
		परत retval;

	retval = follow_link(fs, मूळ, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	परत retval;
}

