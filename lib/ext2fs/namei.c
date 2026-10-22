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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସ୍ଥିର errcode_t open_namei(ext2_filsys fs, ext2_ino_t ମୂଳ, ext2_ino_t base,
			    ଅଚଳ ଅକ୍ଷର *pathname, size_t pathlen, ପୂର୍ଣ୍ଣସଂଖ୍ୟା follow,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା link_ଗଣନା, ଅକ୍ଷର *buf, ext2_ino_t *res_inode);

ସ୍ଥିର errcode_t follow_link(ext2_filsys fs, ext2_ino_t ମୂଳ, ext2_ino_t dir,
			     ext2_ino_t inode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା link_ଗଣନା,
			     ଅକ୍ଷର *buf, ext2_ino_t *res_inode)
{
	ଅକ୍ଷର *pathname;
	ଅକ୍ଷର *buffer = 0;
	errcode_t retval;
	ସଂରଚନା ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       ମୂଳ, dir, inode, link_ଗଣନା);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	ଯଦି (retval) ଫେରନ୍ତୁ retval;
	ଯଦି (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (link_ଗଣନା++ >= EXT2FS_MAX_NESTED_LINKS)
		ଫେରନ୍ତୁ EXT2_ET_SYMLINK_LOOP;

	ଯଦି (ext2fs_is_fast_symlink(&ei))
		pathname = (ଅକ୍ଷର *)&(ei.i_block[0]);
	ଅନ୍ୟଥା ଯଦି (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, ଖାଲି);
		ଯଦି (retval) {
			ext2fs_free_mem(&buffer);
			ଫେରନ୍ତୁ retval;
		}
		pathname = buffer;
	} ଅନ୍ୟଥା {
		retval = ext2fs_bmap2(fs, inode, &ei, ଖାଲି, 0, 0, ଖାଲି, &blk);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		ଯଦି (retval) {
			ext2fs_free_mem(&buffer);
			ଫେରନ୍ତୁ retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, ମୂଳ, dir, pathname, ei.i_size, 1,
			    link_ଗଣନା, buf, res_inode);
	ଯଦି (buffer)
		ext2fs_free_mem(&buffer);
	ଫେରନ୍ତୁ retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
ସ୍ଥିର errcode_t dir_namei(ext2_filsys fs, ext2_ino_t ମୂଳ, ext2_ino_t dir,
			   ଅଚଳ ଅକ୍ଷର *pathname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pathlen,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା link_ଗଣନା, ଅକ୍ଷର *buf,
			   ଅଚଳ ଅକ୍ଷର **name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *namelen,
			   ext2_ino_t *res_inode)
{
	ଅକ୍ଷର c;
	ଅଚଳ ଅକ୍ଷର *thisname;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
	ext2_ino_t inode;
	errcode_t retval;

	ଯଦି ((c = *pathname) == '/') {
        	dir = ମୂଳ;
		pathname++;
		pathlen--;
	}
	ଯେତେବେଳେ (1) {
        	thisname = pathname;
		ପାଇଁ (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			ଯଦି (c == '/')
				ଭାଙ୍ଗ;
		}
		ଯଦି (pathlen < 0)
			ଭାଙ୍ଗ;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		ଯଦି (retval) ଫେରନ୍ତୁ retval;
        	retval = follow_link (fs, ମୂଳ, dir, inode,
				      link_ଗଣନା, buf, &dir);
        	ଯଦି (retval) ଫେରନ୍ତୁ retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t open_namei(ext2_filsys fs, ext2_ino_t ମୂଳ, ext2_ino_t base,
			    ଅଚଳ ଅକ୍ଷର *pathname, size_t pathlen, ପୂର୍ଣ୍ଣସଂଖ୍ୟା follow,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା link_ଗଣନା, ଅକ୍ଷର *buf, ext2_ino_t *res_inode)
{
	ଅଚଳ ଅକ୍ଷର *base_name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       ମୂଳ, base, pathlen, pathname, link_ଗଣନା);
#endif
	retval = dir_namei(fs, ମୂଳ, base, pathname, pathlen,
			   link_ଗଣନା, buf, &base_name, &namelen, &dir);
	ଯଦି (retval) ଫେରନ୍ତୁ retval;
	ଯଦି (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		ଫେରନ୍ତୁ 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି (follow) {
		retval = follow_link(fs, ମୂଳ, dir, inode, link_ଗଣନା,
				     buf, &inode);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_ଗଣନା, inode);
#endif
	*res_inode = inode;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t ମୂଳ, ext2_ino_t cwd,
		       ଅଚଳ ଅକ୍ଷର *name, ext2_ino_t *inode)
{
	ଅକ୍ଷର *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = open_namei(fs, ମୂଳ, cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t ମୂଳ, ext2_ino_t cwd,
			      ଅଚଳ ଅକ୍ଷର *name, ext2_ino_t *inode)
{
	ଅକ୍ଷର *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = open_namei(fs, ମୂଳ, cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t ମୂଳ, ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	ଅକ୍ଷର *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = follow_link(fs, ମୂଳ, cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
}

