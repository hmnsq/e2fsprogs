/*
 * dupfs.c --- duplicate a ext2 filesystem handle
 *
 * Copyright (C) 1997, 1998, 2001, 2003, 2005 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#include "config.h"
#include <stdio.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <time.h>
#include <string.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(sizeof(struct struct_ext2_filsys), &fs);
	nếu (retval)
		trả_về retval;

	*fs = *src;
	fs->device_name = 0;
	fs->super = 0;
	fs->orig_super = 0;
	fs->group_desc = 0;
	fs->inode_map = 0;
	fs->block_map = 0;
	fs->badblocks = 0;
	fs->dblist = 0;
	fs->mmp_buf = 0;
	fs->mmp_cmp = 0;
	fs->mmp_fd = -1;

	io_channel_bumpcount(fs->io);
	nếu (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(strlen(src->device_name)+1, &fs->device_name);
	nếu (retval)
		goto errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->super);
	nếu (retval)
		goto errout;
	memcpy(fs->super, src->super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
	nếu (retval)
		goto errout;
	memcpy(fs->orig_super, src->orig_super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	nếu (retval)
		goto errout;
	memcpy(fs->group_desc, src->group_desc,
	       (size_t) fs->desc_blocks * fs->blocksize);

	nếu (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		nếu (retval)
			goto errout;
	}
	nếu (src->block_map) {
		retval = ext2fs_copy_bitmap(src->block_map, &fs->block_map);
		nếu (retval)
			goto errout;
	}
	nếu (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		nếu (retval)
			goto errout;
	}
	nếu (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		nếu (retval)
			goto errout;
	}
	nếu (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		nếu (retval)
			goto errout;
		memcpy(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	nếu (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		nếu (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			goto errout;
		}
	}
	nếu (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		nếu (retval)
			goto errout;
		memcpy(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	trả_về 0;
errout:
	ext2fs_free(fs);
	trả_về retval;

}

