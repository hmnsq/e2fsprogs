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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

/* #define NAMEI_DEBUG */

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

స్థిర errcode_t open_namei(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t పునాది,
			    స్థిరరాశి అక్షరం *pathname, size_t pathlen, పూర్ణాంకం follow,
			    పూర్ణాంకం link_లెక్క, అక్షరం *buf, ext2_ino_t *res_inode);

స్థిర errcode_t follow_link(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t dir,
			     ext2_ino_t inode, పూర్ణాంకం link_లెక్క,
			     అక్షరం *buf, ext2_ino_t *res_inode)
{
	అక్షరం *pathname;
	అక్షరం *బఫరు = 0;
	errcode_t retval;
	నిర్మాణం ext2_inode ei;
	blk64_t blk;

#నిర్వచితమైతే NAMEI_DEBUG
	రాయి("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       మూలం, dir, inode, link_లెక్క);

#అయితే_ముగింపు
	retval = ext2fs_read_inode (fs, inode, &ei);
	ఒకవేళ (retval) తిరిగి retval;
	ఒకవేళ (!LINUX_S_ISLNK (ei.i_బహుళకం)) {
		*res_inode = inode;
		తిరిగి 0;
	}
	ఒకవేళ (link_లెక్క++ >= EXT2FS_MAX_NESTED_LINKS)
		తిరిగి EXT2_ET_SYMLINK_LOOP;

	ఒకవేళ (ext2fs_is_fast_symlink(&ei))
		pathname = (అక్షరం *)&(ei.i_ఖండం[0]);
	లేకపోతే ఒకవేళ (ei.i_flags & EXT4_INLINE_దత్తాంశం_FL) {
		retval = ext2fs_get_memzero(ei.i_కొలత, &బఫరు);
		ఒకవేళ (retval)
			తిరిగి retval;

		retval = ext2fs_inline_దత్తాంశం_get(fs, inode,
						&ei, బఫరు, ఖాళీ);
		ఒకవేళ (retval) {
			ext2fs_free_mem(&బఫరు);
			తిరిగి retval;
		}
		pathname = బఫరు;
	} లేకపోతే {
		retval = ext2fs_bmap2(fs, inode, &ei, ఖాళీ, 0, 0, ఖాళీ, &blk);
		ఒకవేళ (retval)
			తిరిగి retval;

		retval = ext2fs_get_mem(fs->blocksize, &బఫరు);
		ఒకవేళ (retval)
			తిరిగి retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, బఫరు);
		ఒకవేళ (retval) {
			ext2fs_free_mem(&బఫరు);
			తిరిగి retval;
		}
		pathname = బఫరు;
	}

	retval = open_namei(fs, మూలం, dir, pathname, ei.i_కొలత, 1,
			    link_లెక్క, buf, res_inode);
	ఒకవేళ (బఫరు)
		ext2fs_free_mem(&బఫరు);
	తిరిగి retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
స్థిర errcode_t dir_namei(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t dir,
			   స్థిరరాశి అక్షరం *pathname, పూర్ణాంకం pathlen,
			   పూర్ణాంకం link_లెక్క, అక్షరం *buf,
			   స్థిరరాశి అక్షరం **name, పూర్ణాంకం *namelen,
			   ext2_ino_t *res_inode)
{
	అక్షరం c;
	స్థిరరాశి అక్షరం *thisname;
	పూర్ణాంకం len;
	ext2_ino_t inode;
	errcode_t retval;

	ఒకవేళ ((c = *pathname) == '/') {
        	dir = మూలం;
		pathname++;
		pathlen--;
	}
	అయితే (1) {
        	thisname = pathname;
		కొరకు (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			ఒకవేళ (c == '/')
				ఆపు;
		}
		ఒకవేళ (pathlen < 0)
			ఆపు;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		ఒకవేళ (retval) తిరిగి retval;
        	retval = follow_link (fs, మూలం, dir, inode,
				      link_లెక్క, buf, &dir);
        	ఒకవేళ (retval) తిరిగి retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	తిరిగి 0;
}

స్థిర errcode_t open_namei(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t పునాది,
			    స్థిరరాశి అక్షరం *pathname, size_t pathlen, పూర్ణాంకం follow,
			    పూర్ణాంకం link_లెక్క, అక్షరం *buf, ext2_ino_t *res_inode)
{
	స్థిరరాశి అక్షరం *పునాది_name;
	పూర్ణాంకం namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#నిర్వచితమైతే NAMEI_DEBUG
	రాయి("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       మూలం, పునాది, pathlen, pathname, link_లెక్క);
#అయితే_ముగింపు
	retval = dir_namei(fs, మూలం, పునాది, pathname, pathlen,
			   link_లెక్క, buf, &పునాది_name, &namelen, &dir);
	ఒకవేళ (retval) తిరిగి retval;
	ఒకవేళ (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		తిరిగి 0;
	}
	retval = ext2fs_lookup (fs, dir, పునాది_name, namelen, buf, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ (follow) {
		retval = follow_link(fs, మూలం, dir, inode, link_లెక్క,
				     buf, &inode);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
#నిర్వచితమైతే NAMEI_DEBUG
	రాయి("open_namei: (link_count=%d) returns %lu\n",
	       link_లెక్క, inode);
#అయితే_ముగింపు
	*res_inode = inode;
	తిరిగి 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t cwd,
		       స్థిరరాశి అక్షరం *name, ext2_ino_t *inode)
{
	అక్షరం *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = open_namei(fs, మూలం, cwd, name, అక్షరశ్రేణి_పొడవు(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	తిరిగి retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t cwd,
			      స్థిరరాశి అక్షరం *name, ext2_ino_t *inode)
{
	అక్షరం *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = open_namei(fs, మూలం, cwd, name, అక్షరశ్రేణి_పొడవు(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	తిరిగి retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t మూలం, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	అక్షరం *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = follow_link(fs, మూలం, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	తిరిగి retval;
}

