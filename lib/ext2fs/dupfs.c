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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന struct_ext2_filsys), &fs);
	എങ്കിൽ (retval)
		തിരികെ retval;

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
	എങ്കിൽ (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(strlen(src->device_name)+1, &fs->device_name);
	എങ്കിൽ (retval)
		പോകുക errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->super);
	എങ്കിൽ (retval)
		പോകുക errout;
	memcpy(fs->super, src->super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
	എങ്കിൽ (retval)
		പോകുക errout;
	memcpy(fs->orig_super, src->orig_super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	എങ്കിൽ (retval)
		പോകുക errout;
	memcpy(fs->group_desc, src->group_desc,
	       (size_t) fs->desc_blocks * fs->blocksize);

	എങ്കിൽ (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		എങ്കിൽ (retval)
			പോകുക errout;
	}
	എങ്കിൽ (src->block_map) {
		retval = ext2fs_copy_bitmap(src->block_map, &fs->block_map);
		എങ്കിൽ (retval)
			പോകുക errout;
	}
	എങ്കിൽ (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		എങ്കിൽ (retval)
			പോകുക errout;
	}
	എങ്കിൽ (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		എങ്കിൽ (retval)
			പോകുക errout;
	}
	എങ്കിൽ (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		എങ്കിൽ (retval)
			പോകുക errout;
		memcpy(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	എങ്കിൽ (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		എങ്കിൽ (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			പോകുക errout;
		}
	}
	എങ്കിൽ (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		എങ്കിൽ (retval)
			പോകുക errout;
		memcpy(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	തിരികെ 0;
errout:
	ext2fs_free(fs);
	തിരികെ retval;

}

