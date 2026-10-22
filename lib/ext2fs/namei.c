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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

/* #define NAMEI_DEBUG */

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਸਥਿਰ errcode_t open_namei(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t ਅਧਾਰ,
			    ਅਚਲ ਅੱਖਰ *pathname, size_t pathlen, ਪੂਰਨ follow,
			    ਪੂਰਨ link_ਗਿਣਤੀ, ਅੱਖਰ *buf, ext2_ino_t *res_inode);

ਸਥਿਰ errcode_t follow_link(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t dir,
			     ext2_ino_t inode, ਪੂਰਨ link_ਗਿਣਤੀ,
			     ਅੱਖਰ *buf, ext2_ino_t *res_inode)
{
	ਅੱਖਰ *pathname;
	ਅੱਖਰ *ਬਫਰ = 0;
	errcode_t retval;
	ਬਣਤਰ ext2_inode ei;
	blk64_t blk;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NAMEI_DEBUG
	ਲਿਖੋ("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       ਜੜ੍ਹ, dir, inode, link_ਗਿਣਤੀ);

#ਜੇ_ਅੰਤ
	retval = ext2fs_read_inode (fs, inode, &ei);
	ਜੇ (retval) ਵਾਪਸ retval;
	ਜੇ (!LINUX_S_ISLNK (ei.i_ਬਹੁਲਕ)) {
		*res_inode = inode;
		ਵਾਪਸ 0;
	}
	ਜੇ (link_ਗਿਣਤੀ++ >= EXT2FS_MAX_NESTED_LINKS)
		ਵਾਪਸ EXT2_ET_SYMLINK_LOOP;

	ਜੇ (ext2fs_is_fast_symlink(&ei))
		pathname = (ਅੱਖਰ *)&(ei.i_ਖੰਡ[0]);
	ਨਹੀਂਤਾਂ ਜੇ (ei.i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL) {
		retval = ext2fs_get_memzero(ei.i_ਮਾਪ, &ਬਫਰ);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		retval = ext2fs_inline_ਅੰਕੜੇ_get(fs, inode,
						&ei, ਬਫਰ, ਨਿਰਾ);
		ਜੇ (retval) {
			ext2fs_free_mem(&ਬਫਰ);
			ਵਾਪਸ retval;
		}
		pathname = ਬਫਰ;
	} ਨਹੀਂਤਾਂ {
		retval = ext2fs_bmap2(fs, inode, &ei, ਨਿਰਾ, 0, 0, ਨਿਰਾ, &blk);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		retval = ext2fs_get_mem(fs->blocksize, &ਬਫਰ);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, ਬਫਰ);
		ਜੇ (retval) {
			ext2fs_free_mem(&ਬਫਰ);
			ਵਾਪਸ retval;
		}
		pathname = ਬਫਰ;
	}

	retval = open_namei(fs, ਜੜ੍ਹ, dir, pathname, ei.i_ਮਾਪ, 1,
			    link_ਗਿਣਤੀ, buf, res_inode);
	ਜੇ (ਬਫਰ)
		ext2fs_free_mem(&ਬਫਰ);
	ਵਾਪਸ retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
ਸਥਿਰ errcode_t dir_namei(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t dir,
			   ਅਚਲ ਅੱਖਰ *pathname, ਪੂਰਨ pathlen,
			   ਪੂਰਨ link_ਗਿਣਤੀ, ਅੱਖਰ *buf,
			   ਅਚਲ ਅੱਖਰ **name, ਪੂਰਨ *namelen,
			   ext2_ino_t *res_inode)
{
	ਅੱਖਰ c;
	ਅਚਲ ਅੱਖਰ *thisname;
	ਪੂਰਨ len;
	ext2_ino_t inode;
	errcode_t retval;

	ਜੇ ((c = *pathname) == '/') {
        	dir = ਜੜ੍ਹ;
		pathname++;
		pathlen--;
	}
	ਜਦੋਂ (1) {
        	thisname = pathname;
		ਲਈ (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			ਜੇ (c == '/')
				ਰੋਕੋ;
		}
		ਜੇ (pathlen < 0)
			ਰੋਕੋ;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		ਜੇ (retval) ਵਾਪਸ retval;
        	retval = follow_link (fs, ਜੜ੍ਹ, dir, inode,
				      link_ਗਿਣਤੀ, buf, &dir);
        	ਜੇ (retval) ਵਾਪਸ retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t open_namei(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t ਅਧਾਰ,
			    ਅਚਲ ਅੱਖਰ *pathname, size_t pathlen, ਪੂਰਨ follow,
			    ਪੂਰਨ link_ਗਿਣਤੀ, ਅੱਖਰ *buf, ext2_ino_t *res_inode)
{
	ਅਚਲ ਅੱਖਰ *ਅਧਾਰ_name;
	ਪੂਰਨ namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NAMEI_DEBUG
	ਲਿਖੋ("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       ਜੜ੍ਹ, ਅਧਾਰ, pathlen, pathname, link_ਗਿਣਤੀ);
#ਜੇ_ਅੰਤ
	retval = dir_namei(fs, ਜੜ੍ਹ, ਅਧਾਰ, pathname, pathlen,
			   link_ਗਿਣਤੀ, buf, &ਅਧਾਰ_name, &namelen, &dir);
	ਜੇ (retval) ਵਾਪਸ retval;
	ਜੇ (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		ਵਾਪਸ 0;
	}
	retval = ext2fs_lookup (fs, dir, ਅਧਾਰ_name, namelen, buf, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ (follow) {
		retval = follow_link(fs, ਜੜ੍ਹ, dir, inode, link_ਗਿਣਤੀ,
				     buf, &inode);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NAMEI_DEBUG
	ਲਿਖੋ("open_namei: (link_count=%d) returns %lu\n",
	       link_ਗਿਣਤੀ, inode);
#ਜੇ_ਅੰਤ
	*res_inode = inode;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t cwd,
		       ਅਚਲ ਅੱਖਰ *name, ext2_ino_t *inode)
{
	ਅੱਖਰ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = open_namei(fs, ਜੜ੍ਹ, cwd, name, ਲੜੀ_ਲੰਬਾਈ(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t cwd,
			      ਅਚਲ ਅੱਖਰ *name, ext2_ino_t *inode)
{
	ਅੱਖਰ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = open_namei(fs, ਜੜ੍ਹ, cwd, name, ਲੜੀ_ਲੰਬਾਈ(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ਜੜ੍ਹ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	ਅੱਖਰ *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = follow_link(fs, ਜੜ੍ਹ, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
}

