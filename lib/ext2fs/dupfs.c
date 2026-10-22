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

#ضمن "config.h"
#ضمن <stdio.h>
#𐡄𐡍 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <time.h>
#ضمن <string.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(حجم(بنية struct_ext2_filsys), &fs);
	𐡄𐡍 (retval)
		أرجع retval;

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
	𐡄𐡍 (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(strlen(src->device_name)+1, &fs->device_name);
	𐡄𐡍 (retval)
		اذهب errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->super);
	𐡄𐡍 (retval)
		اذهب errout;
	memcpy(fs->super, src->super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
	𐡄𐡍 (retval)
		اذهب errout;
	memcpy(fs->orig_super, src->orig_super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	𐡄𐡍 (retval)
		اذهب errout;
	memcpy(fs->group_desc, src->group_desc,
	       (size_t) fs->desc_blocks * fs->blocksize);

	𐡄𐡍 (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		𐡄𐡍 (retval)
			اذهب errout;
	}
	𐡄𐡍 (src->block_map) {
		retval = ext2fs_copy_bitmap(src->block_map, &fs->block_map);
		𐡄𐡍 (retval)
			اذهب errout;
	}
	𐡄𐡍 (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		𐡄𐡍 (retval)
			اذهب errout;
	}
	𐡄𐡍 (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		𐡄𐡍 (retval)
			اذهب errout;
	}
	𐡄𐡍 (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		𐡄𐡍 (retval)
			اذهب errout;
		memcpy(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	𐡄𐡍 (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		𐡄𐡍 (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			اذهب errout;
		}
	}
	𐡄𐡍 (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		𐡄𐡍 (retval)
			اذهب errout;
		memcpy(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	أرجع 0;
errout:
	ext2fs_free(fs);
	أرجع retval;

}

