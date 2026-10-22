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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <time.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(आकार(संरचना struct_ext2_filsys), &fs);
	जर (retval)
		परत retval;

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
	जर (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(म_माप(src->device_name)+1, &fs->device_name);
	जर (retval)
		जा errout;
	म_नकल(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->super);
	जर (retval)
		जा errout;
	स_नकल(fs->super, src->super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
	जर (retval)
		जा errout;
	स_नकल(fs->orig_super, src->orig_super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	जर (retval)
		जा errout;
	स_नकल(fs->group_desc, src->group_desc,
	       (माप_प्रकार) fs->desc_blocks * fs->blocksize);

	जर (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		जर (retval)
			जा errout;
	}
	जर (src->block_map) {
		retval = ext2fs_copy_bitmap(src->block_map, &fs->block_map);
		जर (retval)
			जा errout;
	}
	जर (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		जर (retval)
			जा errout;
	}
	जर (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		जर (retval)
			जा errout;
	}
	जर (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		जर (retval)
			जा errout;
		स_नकल(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	जर (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		जर (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			जा errout;
		}
	}
	जर (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		जर (retval)
			जा errout;
		स_नकल(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	परत 0;
errout:
	ext2fs_free(fs);
	परत retval;

}

