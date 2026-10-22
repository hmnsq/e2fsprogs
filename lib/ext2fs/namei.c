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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर

/* #define NAMEI_DEBUG */

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

अटल errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t base,
			    स्थिर अक्षर *pathname, माप_प्रकार pathlen, पूर्णांक follow,
			    पूर्णांक link_गिनती, अक्षर *buf, ext2_ino_t *res_inode);

अटल errcode_t follow_link(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t dir,
			     ext2_ino_t inode, पूर्णांक link_गिनती,
			     अक्षर *buf, ext2_ino_t *res_inode)
{
	अक्षर *pathname;
	अक्षर *बफर = 0;
	errcode_t retval;
	काष्ठा ext2_inode ei;
	blk64_t blk;

#अगर_घोषित NAMEI_DEBUG
	म_लिखो("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       मूल, dir, inode, link_गिनती);

#पूर्ण_अगर
	retval = ext2fs_read_inode (fs, inode, &ei);
	अगर (retval) वापस retval;
	अगर (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		वापस 0;
	}
	अगर (link_गिनती++ >= EXT2FS_MAX_NESTED_LINKS)
		वापस EXT2_ET_SYMLINK_LOOP;

	अगर (ext2fs_is_fast_symlink(&ei))
		pathname = (अक्षर *)&(ei.i_block[0]);
	अन्यथा अगर (ei.i_flags & EXT4_INLINE_तथ्य_FL) {
		retval = ext2fs_get_memzero(ei.i_आकार, &बफर);
		अगर (retval)
			वापस retval;

		retval = ext2fs_inline_तथ्य_get(fs, inode,
						&ei, बफर, शून्य);
		अगर (retval) {
			ext2fs_free_mem(&बफर);
			वापस retval;
		}
		pathname = बफर;
	} अन्यथा {
		retval = ext2fs_bmap2(fs, inode, &ei, शून्य, 0, 0, शून्य, &blk);
		अगर (retval)
			वापस retval;

		retval = ext2fs_get_mem(fs->blocksize, &बफर);
		अगर (retval)
			वापस retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, बफर);
		अगर (retval) {
			ext2fs_free_mem(&बफर);
			वापस retval;
		}
		pathname = बफर;
	}

	retval = open_namei(fs, मूल, dir, pathname, ei.i_आकार, 1,
			    link_गिनती, buf, res_inode);
	अगर (बफर)
		ext2fs_free_mem(&बफर);
	वापस retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
अटल errcode_t dir_namei(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t dir,
			   स्थिर अक्षर *pathname, पूर्णांक pathlen,
			   पूर्णांक link_गिनती, अक्षर *buf,
			   स्थिर अक्षर **name, पूर्णांक *namelen,
			   ext2_ino_t *res_inode)
{
	अक्षर c;
	स्थिर अक्षर *thisname;
	पूर्णांक len;
	ext2_ino_t inode;
	errcode_t retval;

	अगर ((c = *pathname) == '/') {
        	dir = मूल;
		pathname++;
		pathlen--;
	}
	जबतक (1) {
        	thisname = pathname;
		क्रम (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			अगर (c == '/')
				अवरोध;
		}
		अगर (pathlen < 0)
			अवरोध;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		अगर (retval) वापस retval;
        	retval = follow_link (fs, मूल, dir, inode,
				      link_गिनती, buf, &dir);
        	अगर (retval) वापस retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	वापस 0;
}

अटल errcode_t open_namei(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t base,
			    स्थिर अक्षर *pathname, माप_प्रकार pathlen, पूर्णांक follow,
			    पूर्णांक link_गिनती, अक्षर *buf, ext2_ino_t *res_inode)
{
	स्थिर अक्षर *base_name;
	पूर्णांक namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#अगर_घोषित NAMEI_DEBUG
	म_लिखो("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       मूल, base, pathlen, pathname, link_गिनती);
#पूर्ण_अगर
	retval = dir_namei(fs, मूल, base, pathname, pathlen,
			   link_गिनती, buf, &base_name, &namelen, &dir);
	अगर (retval) वापस retval;
	अगर (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		वापस 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	अगर (retval)
		वापस retval;
	अगर (follow) {
		retval = follow_link(fs, मूल, dir, inode, link_गिनती,
				     buf, &inode);
		अगर (retval)
			वापस retval;
	}
#अगर_घोषित NAMEI_DEBUG
	म_लिखो("open_namei: (link_count=%d) returns %lu\n",
	       link_गिनती, inode);
#पूर्ण_अगर
	*res_inode = inode;
	वापस 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t cwd,
		       स्थिर अक्षर *name, ext2_ino_t *inode)
{
	अक्षर *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	अगर (retval)
		वापस retval;

	retval = open_namei(fs, मूल, cwd, name, म_माप(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	वापस retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t cwd,
			      स्थिर अक्षर *name, ext2_ino_t *inode)
{
	अक्षर *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	अगर (retval)
		वापस retval;

	retval = open_namei(fs, मूल, cwd, name, म_माप(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	वापस retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t मूल, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	अक्षर *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	अगर (retval)
		वापस retval;

	retval = follow_link(fs, मूल, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	वापस retval;
}

