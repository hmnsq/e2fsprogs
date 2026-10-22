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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t base,
			    अचर वर्ण *pathname, माप_प्रकार pathlen, पूर्णाङ्क follow,
			    पूर्णाङ्क link_गन्ती, वर्ण *buf, ext2_ino_t *res_inode);

स्थिर errcode_t follow_link(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t dir,
			     ext2_ino_t inode, पूर्णाङ्क link_गन्ती,
			     वर्ण *buf, ext2_ino_t *res_inode)
{
	वर्ण *pathname;
	वर्ण *buffer = 0;
	errcode_t retval;
	संरचना ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	म_लिखो("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       जरा, dir, inode, link_गन्ती);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	यदि (retval) फर्काऊ retval;
	यदि (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		फर्काऊ 0;
	}
	यदि (link_गन्ती++ >= EXT2FS_MAX_NESTED_LINKS)
		फर्काऊ EXT2_ET_SYMLINK_LOOP;

	यदि (ext2fs_is_fast_symlink(&ei))
		pathname = (वर्ण *)&(ei.i_block[0]);
	अन्यथा यदि (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		यदि (retval)
			फर्काऊ retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, रिक्त);
		यदि (retval) {
			ext2fs_free_mem(&buffer);
			फर्काऊ retval;
		}
		pathname = buffer;
	} अन्यथा {
		retval = ext2fs_bmap2(fs, inode, &ei, रिक्त, 0, 0, रिक्त, &blk);
		यदि (retval)
			फर्काऊ retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		यदि (retval)
			फर्काऊ retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		यदि (retval) {
			ext2fs_free_mem(&buffer);
			फर्काऊ retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, जरा, dir, pathname, ei.i_size, 1,
			    link_गन्ती, buf, res_inode);
	यदि (buffer)
		ext2fs_free_mem(&buffer);
	फर्काऊ retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
स्थिर errcode_t dir_namei(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t dir,
			   अचर वर्ण *pathname, पूर्णाङ्क pathlen,
			   पूर्णाङ्क link_गन्ती, वर्ण *buf,
			   अचर वर्ण **name, पूर्णाङ्क *namelen,
			   ext2_ino_t *res_inode)
{
	वर्ण c;
	अचर वर्ण *thisname;
	पूर्णाङ्क len;
	ext2_ino_t inode;
	errcode_t retval;

	यदि ((c = *pathname) == '/') {
        	dir = जरा;
		pathname++;
		pathlen--;
	}
	जबसम्म (1) {
        	thisname = pathname;
		लागि (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			यदि (c == '/')
				रोक;
		}
		यदि (pathlen < 0)
			रोक;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		यदि (retval) फर्काऊ retval;
        	retval = follow_link (fs, जरा, dir, inode,
				      link_गन्ती, buf, &dir);
        	यदि (retval) फर्काऊ retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	फर्काऊ 0;
}

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t base,
			    अचर वर्ण *pathname, माप_प्रकार pathlen, पूर्णाङ्क follow,
			    पूर्णाङ्क link_गन्ती, वर्ण *buf, ext2_ino_t *res_inode)
{
	अचर वर्ण *base_name;
	पूर्णाङ्क namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	म_लिखो("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       जरा, base, pathlen, pathname, link_गन्ती);
#endif
	retval = dir_namei(fs, जरा, base, pathname, pathlen,
			   link_गन्ती, buf, &base_name, &namelen, &dir);
	यदि (retval) फर्काऊ retval;
	यदि (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		फर्काऊ 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	यदि (retval)
		फर्काऊ retval;
	यदि (follow) {
		retval = follow_link(fs, जरा, dir, inode, link_गन्ती,
				     buf, &inode);
		यदि (retval)
			फर्काऊ retval;
	}
#ifdef NAMEI_DEBUG
	म_लिखो("open_namei: (link_count=%d) returns %lu\n",
	       link_गन्ती, inode);
#endif
	*res_inode = inode;
	फर्काऊ 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t cwd,
		       अचर वर्ण *name, ext2_ino_t *inode)
{
	वर्ण *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		फर्काऊ retval;

	retval = open_namei(fs, जरा, cwd, name, म_माप(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	फर्काऊ retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t cwd,
			      अचर वर्ण *name, ext2_ino_t *inode)
{
	वर्ण *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		फर्काऊ retval;

	retval = open_namei(fs, जरा, cwd, name, म_माप(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	फर्काऊ retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t जरा, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	वर्ण *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		फर्काऊ retval;

	retval = follow_link(fs, जरा, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	फर्काऊ retval;
}

