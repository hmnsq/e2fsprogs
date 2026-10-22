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

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t base,
			    ध्रुव वर्ण *pathname, माप_प्रकार pathlen, पूर्णाङ्क follow,
			    पूर्णाङ्क link_गिनती, वर्ण *buf, ext2_ino_t *res_inode);

स्थिर errcode_t follow_link(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t dir,
			     ext2_ino_t inode, पूर्णाङ्क link_गिनती,
			     वर्ण *buf, ext2_ino_t *res_inode)
{
	वर्ण *pathname;
	वर्ण *buffer = 0;
	errcode_t retval;
	संरचना ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	म_लिखो("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       मूल, dir, inode, link_गिनती);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	यदि (retval) प्रत्यागम retval;
	यदि (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		प्रत्यागम 0;
	}
	यदि (link_गिनती++ >= EXT2FS_MAX_NESTED_LINKS)
		प्रत्यागम EXT2_ET_SYMLINK_LOOP;

	यदि (ext2fs_is_fast_symlink(&ei))
		pathname = (वर्ण *)&(ei.i_block[0]);
	अन्यथा यदि (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		यदि (retval)
			प्रत्यागम retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, रिक्त);
		यदि (retval) {
			ext2fs_free_mem(&buffer);
			प्रत्यागम retval;
		}
		pathname = buffer;
	} अन्यथा {
		retval = ext2fs_bmap2(fs, inode, &ei, रिक्त, 0, 0, रिक्त, &blk);
		यदि (retval)
			प्रत्यागम retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		यदि (retval)
			प्रत्यागम retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		यदि (retval) {
			ext2fs_free_mem(&buffer);
			प्रत्यागम retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, मूल, dir, pathname, ei.i_size, 1,
			    link_गिनती, buf, res_inode);
	यदि (buffer)
		ext2fs_free_mem(&buffer);
	प्रत्यागम retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
स्थिर errcode_t dir_namei(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t dir,
			   ध्रुव वर्ण *pathname, पूर्णाङ्क pathlen,
			   पूर्णाङ्क link_गिनती, वर्ण *buf,
			   ध्रुव वर्ण **name, पूर्णाङ्क *namelen,
			   ext2_ino_t *res_inode)
{
	वर्ण c;
	ध्रुव वर्ण *thisname;
	पूर्णाङ्क len;
	ext2_ino_t inode;
	errcode_t retval;

	यदि ((c = *pathname) == '/') {
        	dir = मूल;
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
        	retval = follow_link (fs, मूल, dir, inode,
				      link_गिनती, buf, &dir);
        	यदि (retval) प्रत्यागम retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	प्रत्यागम 0;
}

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t base,
			    ध्रुव वर्ण *pathname, माप_प्रकार pathlen, पूर्णाङ्क follow,
			    पूर्णाङ्क link_गिनती, वर्ण *buf, ext2_ino_t *res_inode)
{
	ध्रुव वर्ण *base_name;
	पूर्णाङ्क namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	म_लिखो("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       मूल, base, pathlen, pathname, link_गिनती);
#endif
	retval = dir_namei(fs, मूल, base, pathname, pathlen,
			   link_गिनती, buf, &base_name, &namelen, &dir);
	यदि (retval) प्रत्यागम retval;
	यदि (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		प्रत्यागम 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	यदि (retval)
		प्रत्यागम retval;
	यदि (follow) {
		retval = follow_link(fs, मूल, dir, inode, link_गिनती,
				     buf, &inode);
		यदि (retval)
			प्रत्यागम retval;
	}
#ifdef NAMEI_DEBUG
	म_लिखो("open_namei: (link_count=%d) returns %lu\n",
	       link_गिनती, inode);
#endif
	*res_inode = inode;
	प्रत्यागम 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t cwd,
		       ध्रुव वर्ण *name, ext2_ino_t *inode)
{
	वर्ण *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;

	retval = open_namei(fs, मूल, cwd, name, म_माप(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t cwd,
			      ध्रुव वर्ण *name, ext2_ino_t *inode)
{
	वर्ण *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;

	retval = open_namei(fs, मूल, cwd, name, म_माप(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	वर्ण *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	यदि (retval)
		प्रत्यागम retval;

	retval = follow_link(fs, मूल, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

