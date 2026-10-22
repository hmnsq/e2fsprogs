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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା struct_ext2_filsys), &fs);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

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
	ଯଦି (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(strlen(src->device_name)+1, &fs->device_name);
	ଯଦି (retval)
		ଯାଅ errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->super);
	ଯଦି (retval)
		ଯାଅ errout;
	memcpy(fs->super, src->super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
	ଯଦି (retval)
		ଯାଅ errout;
	memcpy(fs->orig_super, src->orig_super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	ଯଦି (retval)
		ଯାଅ errout;
	memcpy(fs->group_desc, src->group_desc,
	       (size_t) fs->desc_blocks * fs->blocksize);

	ଯଦି (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		ଯଦି (retval)
			ଯାଅ errout;
	}
	ଯଦି (src->block_map) {
		retval = ext2fs_copy_bitmap(src->block_map, &fs->block_map);
		ଯଦି (retval)
			ଯାଅ errout;
	}
	ଯଦି (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		ଯଦି (retval)
			ଯାଅ errout;
	}
	ଯଦି (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		ଯଦି (retval)
			ଯାଅ errout;
	}
	ଯଦି (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		ଯଦି (retval)
			ଯାଅ errout;
		memcpy(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	ଯଦି (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		ଯଦି (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			ଯାଅ errout;
		}
	}
	ଯଦି (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		ଯଦି (retval)
			ଯାଅ errout;
		memcpy(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	ଫେରନ୍ତୁ 0;
errout:
	ext2fs_free(fs);
	ଫେରନ୍ତୁ retval;

}

