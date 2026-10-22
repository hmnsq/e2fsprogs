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
#якщо HAVE_UNISTD_H
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
	якщо (retval)
		повернути retval;

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
	якщо (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(strlen(src->device_name)+1, &fs->device_name);
	якщо (retval)
		goto errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->super);
	якщо (retval)
		goto errout;
	memcpy(fs->super, src->super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
	якщо (retval)
		goto errout;
	memcpy(fs->orig_super, src->orig_super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	якщо (retval)
		goto errout;
	memcpy(fs->group_desc, src->group_desc,
	       (size_t) fs->desc_blocks * fs->blocksize);

	якщо (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		якщо (retval)
			goto errout;
	}
	якщо (src->block_map) {
		retval = ext2fs_copy_bitmap(src->block_map, &fs->block_map);
		якщо (retval)
			goto errout;
	}
	якщо (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		якщо (retval)
			goto errout;
	}
	якщо (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		якщо (retval)
			goto errout;
	}
	якщо (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		якщо (retval)
			goto errout;
		memcpy(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	якщо (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		якщо (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			goto errout;
		}
	}
	якщо (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		якщо (retval)
			goto errout;
		memcpy(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	повернути 0;
errout:
	ext2fs_free(fs);
	повернути retval;

}

