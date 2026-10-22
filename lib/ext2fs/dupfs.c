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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <time.h>
#समावेश <string.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(माप(काष्ठा struct_ext2_filsys), &fs);
	अगर (retval)
		वापस retval;

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
	अगर (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(म_माप(src->device_name)+1, &fs->device_name);
	अगर (retval)
		जाओ errout;
	म_नकल(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_आकार, &fs->super);
	अगर (retval)
		जाओ errout;
	स_नकल(fs->super, src->super, SUPERBLOCK_आकार);

	retval = ext2fs_get_mem(SUPERBLOCK_आकार, &fs->orig_super);
	अगर (retval)
		जाओ errout;
	स_नकल(fs->orig_super, src->orig_super, SUPERBLOCK_आकार);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	अगर (retval)
		जाओ errout;
	स_नकल(fs->group_desc, src->group_desc,
	       (माप_प्रकार) fs->desc_blocks * fs->blocksize);

	अगर (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		अगर (retval)
			जाओ errout;
	}
	अगर (src->block_map) {
		retval = ext2fs_copy_bitmap(src->block_map, &fs->block_map);
		अगर (retval)
			जाओ errout;
	}
	अगर (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		अगर (retval)
			जाओ errout;
	}
	अगर (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		अगर (retval)
			जाओ errout;
	}
	अगर (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		अगर (retval)
			जाओ errout;
		स_नकल(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	अगर (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		अगर (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			जाओ errout;
		}
	}
	अगर (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		अगर (retval)
			जाओ errout;
		स_नकल(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	वापस 0;
errout:
	ext2fs_free(fs);
	वापस retval;

}

