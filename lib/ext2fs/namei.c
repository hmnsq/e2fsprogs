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
#यदि_अन्तः

/* #define NAMEI_DEBUG */

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t आधारः,
			    ध्रुव वर्ण *pathname, size_t pathlen, पूर्णाङ्क follow,
			    पूर्णाङ्क link_गणनम्, वर्ण *buf, ext2_ino_t *res_inode);

स्थिर errcode_t follow_link(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t dir,
			     ext2_ino_t inode, पूर्णाङ्क link_गणनम्,
			     वर्ण *buf, ext2_ino_t *res_inode)
{
	वर्ण *pathname;
	वर्ण *बफर = 0;
	errcode_t retval;
	संरचना ext2_inode ei;
	blk64_t blk;

#यदि_परिभाषितम् NAMEI_DEBUG
	प्रारूप_लिख("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       मूलम्, dir, inode, link_गणनम्);

#यदि_अन्तः
	retval = ext2fs_read_inode (fs, inode, &ei);
	यदि (retval) प्रत्यागम retval;
	यदि (!LINUX_S_ISLNK (ei.i_बहुलकम्)) {
		*res_inode = inode;
		प्रत्यागम 0;
	}
	यदि (link_गणनम्++ >= EXT2FS_MAX_NESTED_LINKS)
		प्रत्यागम EXT2_ET_SYMLINK_LOOP;

	यदि (ext2fs_is_fast_symlink(&ei))
		pathname = (वर्ण *)&(ei.i_खण्डम्[0]);
	अन्यथा यदि (ei.i_flags & EXT4_INLINE_दत्त_FL) {
		retval = ext2fs_get_memzero(ei.i_आकार, &बफर);
		यदि (retval)
			प्रत्यागम retval;

		retval = ext2fs_inline_दत्त_get(fs, inode,
						&ei, बफर, रिक्त);
		यदि (retval) {
			ext2fs_free_mem(&बफर);
			प्रत्यागम retval;
		}
		pathname = बफर;
	} अन्यथा {
		retval = ext2fs_bmap2(fs, inode, &ei, रिक्त, 0, 0, रिक्त, &blk);
		यदि (retval)
			प्रत्यागम retval;

		retval = ext2fs_get_mem(fs->blocksize, &बफर);
		यदि (retval)
			प्रत्यागम retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, बफर);
		यदि (retval) {
			ext2fs_free_mem(&बफर);
			प्रत्यागम retval;
		}
		pathname = बफर;
	}

	retval = open_namei(fs, मूलम्, dir, pathname, ei.i_आकार, 1,
			    link_गणनम्, buf, res_inode);
	यदि (बफर)
		ext2fs_free_mem(&बफर);
	प्रत्यागम retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
स्थिर errcode_t dir_namei(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t dir,
			   ध्रुव वर्ण *pathname, पूर्णाङ्क pathlen,
			   पूर्णाङ्क link_गणनम्, वर्ण *buf,
			   ध्रुव वर्ण **name, पूर्णाङ्क *namelen,
			   ext2_ino_t *res_inode)
{
	वर्ण c;
	ध्रुव वर्ण *thisname;
	पूर्णाङ्क len;
	ext2_ino_t inode;
	errcode_t retval;

	यदि ((c = *pathname) == '/') {
        	dir = मूलम्;
		pathname++;
		pathlen--;
	}
	यावत् (1) {
        	thisname = pathname;
		प्रति (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			यदि (c == '/')
				भङ्ग;
		}
		यदि (pathlen < 0)
			भङ्ग;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		यदि (retval) प्रत्यागम retval;
        	retval = follow_link (fs, मूलम्, dir, inode,
				      link_गणनम्, buf, &dir);
        	यदि (retval) प्रत्यागम retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	प्रत्यागम 0;
}

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t आधारः,
			    ध्रुव वर्ण *pathname, size_t pathlen, पूर्णाङ्क follow,
			    पूर्णाङ्क link_गणनम्, वर्ण *buf, ext2_ino_t *res_inode)
{
	ध्रुव वर्ण *आधारः_name;
	पूर्णाङ्क namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#यदि_परिभाषितम् NAMEI_DEBUG
	प्रारूप_लिख("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       मूलम्, आधारः, pathlen, pathname, link_गणनम्);
#यदि_अन्तः
	retval = dir_namei(fs, मूलम्, आधारः, pathname, pathlen,
			   link_गणनम्, buf, &आधारः_name, &namelen, &dir);
	यदि (retval) प्रत्यागम retval;
	यदि (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		प्रत्यागम 0;
	}
	retval = ext2fs_lookup (fs, dir, आधारः_name, namelen, buf, &inode);
	यदि (retval)
		प्रत्यागम retval;
	यदि (follow) {
		retval = follow_link(fs, मूलम्, dir, inode, link_गणनम्,
				     buf, &inode);
		यदि (retval)
			प्रत्यागम retval;
	}
#यदि_परिभाषितम् NAMEI_DEBUG
	प्रारूप_लिख("open_namei: (link_count=%d) returns %lu\n",
	       link_गणनम्, inode);
#यदि_अन्तः
	*res_inode = inode;
	प्रत्यागम 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t cwd,
		       ध्रुव वर्ण *name, ext2_ino_t *inode)
{
	वर्ण *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;

	retval = open_namei(fs, मूलम्, cwd, name, वाक्यदैर्घ्यम्(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t cwd,
			      ध्रुव वर्ण *name, ext2_ino_t *inode)
{
	वर्ण *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;

	retval = open_namei(fs, मूलम्, cwd, name, वाक्यदैर्घ्यम्(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	वर्ण *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;

	retval = follow_link(fs, मूलम्, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

