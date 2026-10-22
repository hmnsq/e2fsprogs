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
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t base,
			    ध्रुव वर्ण *pathname, size_t pathlen, पूर्णाङ्क follow,
			    पूर्णाङ्क link_गणनम्, वर्ण *buf, ext2_ino_t *res_inode);

स्थिर errcode_t follow_link(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t dir,
			     ext2_ino_t inode, पूर्णाङ्क link_गणनम्,
			     वर्ण *buf, ext2_ino_t *res_inode)
{
	वर्ण *pathname;
	वर्ण *buffer = 0;
	errcode_t retval;
	संरचना ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       मूलम्, dir, inode, link_गणनम्);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	जइ (retval) प्रत्यागम retval;
	जइ (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		प्रत्यागम 0;
	}
	जइ (link_गणनम्++ >= EXT2FS_MAX_NESTED_LINKS)
		प्रत्यागम EXT2_ET_SYMLINK_LOOP;

	जइ (ext2fs_is_fast_symlink(&ei))
		pathname = (वर्ण *)&(ei.i_block[0]);
	अण्णहा जइ (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		जइ (retval)
			प्रत्यागम retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, रिक्त);
		जइ (retval) {
			ext2fs_free_mem(&buffer);
			प्रत्यागम retval;
		}
		pathname = buffer;
	} अण्णहा {
		retval = ext2fs_bmap2(fs, inode, &ei, रिक्त, 0, 0, रिक्त, &blk);
		जइ (retval)
			प्रत्यागम retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		जइ (retval)
			प्रत्यागम retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		जइ (retval) {
			ext2fs_free_mem(&buffer);
			प्रत्यागम retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, मूलम्, dir, pathname, ei.i_size, 1,
			    link_गणनम्, buf, res_inode);
	जइ (buffer)
		ext2fs_free_mem(&buffer);
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

	जइ ((c = *pathname) == '/') {
        	dir = मूलम्;
		pathname++;
		pathlen--;
	}
	जाव (1) {
        	thisname = pathname;
		प्रति (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			जइ (c == '/')
				भङ्ग;
		}
		जइ (pathlen < 0)
			भङ्ग;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		जइ (retval) प्रत्यागम retval;
        	retval = follow_link (fs, मूलम्, dir, inode,
				      link_गणनम्, buf, &dir);
        	जइ (retval) प्रत्यागम retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	प्रत्यागम 0;
}

स्थिर errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूलम्, ext2_ino_t base,
			    ध्रुव वर्ण *pathname, size_t pathlen, पूर्णाङ्क follow,
			    पूर्णाङ्क link_गणनम्, वर्ण *buf, ext2_ino_t *res_inode)
{
	ध्रुव वर्ण *base_name;
	पूर्णाङ्क namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       मूलम्, base, pathlen, pathname, link_गणनम्);
#endif
	retval = dir_namei(fs, मूलम्, base, pathname, pathlen,
			   link_गणनम्, buf, &base_name, &namelen, &dir);
	जइ (retval) प्रत्यागम retval;
	जइ (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		प्रत्यागम 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	जइ (retval)
		प्रत्यागम retval;
	जइ (follow) {
		retval = follow_link(fs, मूलम्, dir, inode, link_गणनम्,
				     buf, &inode);
		जइ (retval)
			प्रत्यागम retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_गणनम्, inode);
#endif
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
	जइ (retval)
		प्रत्यागम retval;

	retval = open_namei(fs, मूलम्, cwd, name, strlen(name), 0, 0,
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
	जइ (retval)
		प्रत्यागम retval;

	retval = open_namei(fs, मूलम्, cwd, name, strlen(name), 1, 0,
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
	जइ (retval)
		प्रत्यागम retval;

	retval = follow_link(fs, मूलम्, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

