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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

/* #define NAMEI_DEBUG */

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

நிலையான errcode_t open_namei(ext2_filsys fs, ext2_ino_t வேர், ext2_ino_t base,
			    மாறிலி எழுத்து *pathname, size_t pathlen, முழு follow,
			    முழு link_மொத்தம், எழுத்து *buf, ext2_ino_t *res_inode);

நிலையான errcode_t follow_link(ext2_filsys fs, ext2_ino_t வேர், ext2_ino_t dir,
			     ext2_ino_t inode, முழு link_மொத்தம்,
			     எழுத்து *buf, ext2_ino_t *res_inode)
{
	எழுத்து *pathname;
	எழுத்து *buffer = 0;
	errcode_t retval;
	கட்டமைப்பு ext2_inode ei;
	blk64_t blk;

#ifdef NAMEI_DEBUG
	printf("follow_link: root=%lu, dir=%lu, inode=%lu, lc=%d\n",
	       வேர், dir, inode, link_மொத்தம்);

#endif
	retval = ext2fs_read_inode (fs, inode, &ei);
	எனில் (retval) திரும்பு retval;
	எனில் (!LINUX_S_ISLNK (ei.i_mode)) {
		*res_inode = inode;
		திரும்பு 0;
	}
	எனில் (link_மொத்தம்++ >= EXT2FS_MAX_NESTED_LINKS)
		திரும்பு EXT2_ET_SYMLINK_LOOP;

	எனில் (ext2fs_is_fast_symlink(&ei))
		pathname = (எழுத்து *)&(ei.i_block[0]);
	இல்லையெனில் எனில் (ei.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_get_memzero(ei.i_size, &buffer);
		எனில் (retval)
			திரும்பு retval;

		retval = ext2fs_inline_data_get(fs, inode,
						&ei, buffer, வெற்று);
		எனில் (retval) {
			ext2fs_free_mem(&buffer);
			திரும்பு retval;
		}
		pathname = buffer;
	} இல்லையெனில் {
		retval = ext2fs_bmap2(fs, inode, &ei, வெற்று, 0, 0, வெற்று, &blk);
		எனில் (retval)
			திரும்பு retval;

		retval = ext2fs_get_mem(fs->blocksize, &buffer);
		எனில் (retval)
			திரும்பு retval;

		retval = io_channel_read_blk64(fs->io, blk, 1, buffer);
		எனில் (retval) {
			ext2fs_free_mem(&buffer);
			திரும்பு retval;
		}
		pathname = buffer;
	}

	retval = open_namei(fs, வேர், dir, pathname, ei.i_size, 1,
			    link_மொத்தம், buf, res_inode);
	எனில் (buffer)
		ext2fs_free_mem(&buffer);
	திரும்பு retval;
}

/*
 * This routine interprets a pathname in the context of the current
 * directory and the root directory, and returns the inode of the
 * containing directory, and a pointer to the filename of the file
 * (pointing into the pathname) and the length of the filename.
 */
நிலையான errcode_t dir_namei(ext2_filsys fs, ext2_ino_t வேர், ext2_ino_t dir,
			   மாறிலி எழுத்து *pathname, முழு pathlen,
			   முழு link_மொத்தம், எழுத்து *buf,
			   மாறிலி எழுத்து **name, முழு *namelen,
			   ext2_ino_t *res_inode)
{
	எழுத்து c;
	மாறிலி எழுத்து *thisname;
	முழு len;
	ext2_ino_t inode;
	errcode_t retval;

	எனில் ((c = *pathname) == '/') {
        	dir = வேர்;
		pathname++;
		pathlen--;
	}
	போது (1) {
        	thisname = pathname;
		க்கு (len=0; --pathlen >= 0;len++) {
			c = *(pathname++);
			எனில் (c == '/')
				நிறுத்து;
		}
		எனில் (pathlen < 0)
			நிறுத்து;
		retval = ext2fs_lookup (fs, dir, thisname, len, buf, &inode);
		எனில் (retval) திரும்பு retval;
        	retval = follow_link (fs, வேர், dir, inode,
				      link_மொத்தம், buf, &dir);
        	எனில் (retval) திரும்பு retval;
    	}
	*name = thisname;
	*namelen = len;
	*res_inode = dir;
	திரும்பு 0;
}

நிலையான errcode_t open_namei(ext2_filsys fs, ext2_ino_t வேர், ext2_ino_t base,
			    மாறிலி எழுத்து *pathname, size_t pathlen, முழு follow,
			    முழு link_மொத்தம், எழுத்து *buf, ext2_ino_t *res_inode)
{
	மாறிலி எழுத்து *base_name;
	முழு namelen;
	ext2_ino_t dir, inode;
	errcode_t retval;

#ifdef NAMEI_DEBUG
	printf("open_namei: root=%lu, dir=%lu, path=%.*s, lc=%d\n",
	       வேர், base, pathlen, pathname, link_மொத்தம்);
#endif
	retval = dir_namei(fs, வேர், base, pathname, pathlen,
			   link_மொத்தம், buf, &base_name, &namelen, &dir);
	எனில் (retval) திரும்பு retval;
	எனில் (!namelen) {                     /* special case: '/usr/' etc */
		*res_inode=dir;
		திரும்பு 0;
	}
	retval = ext2fs_lookup (fs, dir, base_name, namelen, buf, &inode);
	எனில் (retval)
		திரும்பு retval;
	எனில் (follow) {
		retval = follow_link(fs, வேர், dir, inode, link_மொத்தம்,
				     buf, &inode);
		எனில் (retval)
			திரும்பு retval;
	}
#ifdef NAMEI_DEBUG
	printf("open_namei: (link_count=%d) returns %lu\n",
	       link_மொத்தம், inode);
#endif
	*res_inode = inode;
	திரும்பு 0;
}

errcode_t ext2fs_namei(ext2_filsys fs, ext2_ino_t வேர், ext2_ino_t cwd,
		       மாறிலி எழுத்து *name, ext2_ino_t *inode)
{
	எழுத்து *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	எனில் (retval)
		திரும்பு retval;

	retval = open_namei(fs, வேர், cwd, name, strlen(name), 0, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	திரும்பு retval;
}

errcode_t ext2fs_namei_follow(ext2_filsys fs, ext2_ino_t வேர், ext2_ino_t cwd,
			      மாறிலி எழுத்து *name, ext2_ino_t *inode)
{
	எழுத்து *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	எனில் (retval)
		திரும்பு retval;

	retval = open_namei(fs, வேர், cwd, name, strlen(name), 1, 0,
			    buf, inode);

	ext2fs_free_mem(&buf);
	திரும்பு retval;
}

errcode_t ext2fs_follow_link(ext2_filsys fs, ext2_ino_t வேர், ext2_ino_t cwd,
			ext2_ino_t inode, ext2_ino_t *res_inode)
{
	எழுத்து *buf;
	errcode_t retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	எனில் (retval)
		திரும்பு retval;

	retval = follow_link(fs, வேர், cwd, inode, 0, buf, res_inode);

	ext2fs_free_mem(&buf);
	திரும்பு retval;
}

