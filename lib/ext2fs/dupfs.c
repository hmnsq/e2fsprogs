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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <time.h>
#شامل <string.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(اندازه(ساختار struct_ext2_filsys), &fs);
	اگر (retval)
		بازگشت retval;

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
	اگر (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(strlen(src->device_name)+1, &fs->device_name);
	اگر (retval)
		برو errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->super);
	اگر (retval)
		برو errout;
	memcpy(fs->super, src->super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
	اگر (retval)
		برو errout;
	memcpy(fs->orig_super, src->orig_super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	اگر (retval)
		برو errout;
	memcpy(fs->group_desc, src->group_desc,
	       (size_t) fs->desc_blocks * fs->blocksize);

	اگر (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		اگر (retval)
			برو errout;
	}
	اگر (src->block_map) {
		retval = ext2fs_copy_bitmap(src->block_map, &fs->block_map);
		اگر (retval)
			برو errout;
	}
	اگر (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		اگر (retval)
			برو errout;
	}
	اگر (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		اگر (retval)
			برو errout;
	}
	اگر (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		اگر (retval)
			برو errout;
		memcpy(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	اگر (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		اگر (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			برو errout;
		}
	}
	اگر (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		اگر (retval)
			برو errout;
		memcpy(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	بازگشت 0;
errout:
	ext2fs_free(fs);
	بازگشت retval;

}

