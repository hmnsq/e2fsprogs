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
#اگر_ختم
#شامل <time.h>
#شامل <string.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(حجم(ساخت struct_ext2_filsys), &fs);
	اگر (retval)
		واپس retval;

	*fs = *src;
	fs->device_name = 0;
	fs->super = 0;
	fs->orig_super = 0;
	fs->گروہ_desc = 0;
	fs->inode_map = 0;
	fs->بلاک_map = 0;
	fs->badblocks = 0;
	fs->dblist = 0;
	fs->mmp_buf = 0;
	fs->mmp_cmp = 0;
	fs->mmp_fd = -1;

	io_channel_bumpcount(fs->io);
	اگر (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(لڑی_لمبائی(src->device_name)+1, &fs->device_name);
	اگر (retval)
		جاؤ errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_سائز, &fs->super);
	اگر (retval)
		جاؤ errout;
	حافظہ_نقل(fs->super, src->super, SUPERBLOCK_سائز);

	retval = ext2fs_get_mem(SUPERBLOCK_سائز, &fs->orig_super);
	اگر (retval)
		جاؤ errout;
	حافظہ_نقل(fs->orig_super, src->orig_super, SUPERBLOCK_سائز);

	retval = ext2fs_get_array(fs->desc_بلاکیں, fs->blocksize,
				&fs->گروہ_desc);
	اگر (retval)
		جاؤ errout;
	حافظہ_نقل(fs->گروہ_desc, src->گروہ_desc,
	       (size_t) fs->desc_بلاکیں * fs->blocksize);

	اگر (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		اگر (retval)
			جاؤ errout;
	}
	اگر (src->بلاک_map) {
		retval = ext2fs_copy_bitmap(src->بلاک_map, &fs->بلاک_map);
		اگر (retval)
			جاؤ errout;
	}
	اگر (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		اگر (retval)
			جاؤ errout;
	}
	اگر (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		اگر (retval)
			جاؤ errout;
	}
	اگر (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		اگر (retval)
			جاؤ errout;
		حافظہ_نقل(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	اگر (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		اگر (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_براہ_راست;
			جاؤ errout;
		}
	}
	اگر (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		اگر (retval)
			جاؤ errout;
		حافظہ_نقل(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	واپس 0;
errout:
	ext2fs_free(fs);
	واپس retval;

}

