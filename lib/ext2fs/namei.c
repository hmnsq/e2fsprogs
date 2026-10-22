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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

സ്ഥിതം errcode_t open_namei(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t base,
			    സ്ഥിരം അക്ഷരം *pathname, size_t pathlen, പൂർണ്ണസംഖ്യ follow,
			    പൂർണ്ണസംഖ്യ link_എണ്ണം, അക്ഷരം *buf, ext2_ino_t *res_inode);

സ്ഥിതം errcode_t follow_link(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t dir,
			     ext2_ino_t inode, പൂർണ്ണസംഖ്യ link_എണ്ണം,
			     അക്ഷരം *buf, ext2_ino_t *res_inode)
{
	അക്ഷരം *pathname;
	അക്ഷരം *buffer = 0;
	errcode_t retval;
	ഘടന ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       വേര്, dir, inode, link_എണ്ണം);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	എങ്കിൽ (retval) തിരികെ retval;
	എങ്കിൽ (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		തിരികെ 0;
	}
	എങ്കിൽ (link_എണ്ണം++ >= EXT2FS_MAX_NESTED_LINKS)
		തിരികെ EXT2_ET_SYMLINK_LOOP;

	എങ്കിൽ (ext2fs_is_fast_symlink(&ei))
		pathname = (അക്ഷരം *)&(ei.i_block[0]);
	അല്ലെങ്കിൽ എങ്കിൽ (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		എങ്കിൽ (retval)
			തിരികെ retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, ശൂന്യ);
		എങ്കിൽ (retval) {
			ext2fs_free_mem(&buffer);
			തിരികെ retval;
		}
		pathname = buffer;
	} അല്ലെങ്കിൽ {
		retval = ext2fs_bmap2(fs, inode, &ei, ശൂന്യ, 0, 0, ശൂന്യ, &blk);
		എങ്കിൽ (retval)
			തിരികെ retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		എങ്കിൽ (retval)
			തിരികെ retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		എങ്കിൽ (retval) {
			ext2fs_free_mem(&buffer);
			തിരികെ retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, വേര്, dir, pathname, ei.i_size, 1,
			    link_എണ്ണം, buf, res_inode);
	എങ്കിൽ (buffer)
		ext2fs_free_mem(&buffer);
	തിരികെ retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
സ്ഥിതം errcode_t dir_namei(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t dir,
			   സ്ഥിരം അക്ഷരം *pathname, പൂർണ്ണസംഖ്യ pathlen,
			   പൂർണ്ണസംഖ്യ link_എണ്ണം, അക്ഷരം *buf,
			   സ്ഥിരം അക്ഷരം **name, പൂർണ്ണസംഖ്യ *namelen,
			   ext2_ino_t *res_inode)
{
	അക്ഷരം c;
	സ്ഥിരം അക്ഷരം *thisname;
	പൂർണ്ണസംഖ്യ len;
	ext2_ino_t inode;
	errcode_t retval;

	എങ്കിൽ ((c = *pathname) == '/') {
        	dir = വേര്;
		pathname++;
		pathlen--;
	}
	ആകുമ്പോൾ (1) {
        	thisname = pathname;
		വേണ്ടി (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			എങ്കിൽ (c == '/')
				നിർത്തുക;
		}
		എങ്കിൽ (pathlen < 0)
			നിർത്തുക;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		എങ്കിൽ (retval) തിരികെ retval;
        	retval = follow_link (fs, വേര്, dir, inode,
				      link_എണ്ണം, buf, &dir);
        	എങ്കിൽ (retval) തിരികെ retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	തിരികെ 0;
}

സ്ഥിതം errcode_t open_namei(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t base,
			    സ്ഥിരം അക്ഷരം *pathname, size_t pathlen, പൂർണ്ണസംഖ്യ follow,
			    പൂർണ്ണസംഖ്യ link_എണ്ണം, അക്ഷരം *buf, ext2_ino_t *res_inode)
{
	സ്ഥിരം അക്ഷരം *base_name;
	പൂർണ്ണസംഖ്യ namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       വേര്, base, pathlen, pathname, link_എണ്ണം);
#endif
	retval = dir_namei(fs, വേര്, base, pathname, pathlen,
			   link_എണ്ണം, buf, &base_name, &namelen, &dir);
	എങ്കിൽ (retval) തിരികെ retval;
	എങ്കിൽ (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		തിരികെ 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ (follow) {
		retval = follow_link(fs, വേര്, dir, inode, link_എണ്ണം,
				     buf, &inode);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_എണ്ണം, inode);
#endif
	*res_inode = inode;
	തിരികെ 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t cwd,
		       സ്ഥിരം അക്ഷരം *name, ext2_ino_t *inode)
{
	അക്ഷരം *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = open_namei(fs, വേര്, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	തിരികെ retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t cwd,
			      സ്ഥിരം അക്ഷരം *name, ext2_ino_t *inode)
{
	അക്ഷരം *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = open_namei(fs, വേര്, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	തിരികെ retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t വേര്, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	അക്ഷരം *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = follow_link(fs, വേര്, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	തിരികെ retval;
}

