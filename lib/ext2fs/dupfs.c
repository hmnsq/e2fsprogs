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
#यदि HAVE_UNISTD_H
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

	retval = ext2fs_get_mem(परिमाण(संरचना struct_ext2_filsys), &fs);
	यदि (retval)
		प्रत्यागम retval;

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
	यदि (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(म_माप(src->device_name)+1, &fs->device_name);
	यदि (retval)
		गच्छ errout;
	म_नकल(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->super);
	यदि (retval)
		गच्छ errout;
	स_नकल(fs->super, src->super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
	यदि (retval)
		गच्छ errout;
	स_नकल(fs->orig_super, src->orig_super, SUPERBLOCK_SIZE);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	यदि (retval)
		गच्छ errout;
	स_नकल(fs->group_desc, src->group_desc,
	       (माप_प्रकार) fs->desc_blocks * fs->blocksize);

	यदि (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		यदि (retval)
			गच्छ errout;
	}
	यदि (src->block_map) {
		retval = ext2fs_copy_bitmap(src->block_map, &fs->block_map);
		यदि (retval)
			गच्छ errout;
	}
	यदि (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		यदि (retval)
			गच्छ errout;
	}
	यदि (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		यदि (retval)
			गच्छ errout;
	}
	यदि (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		यदि (retval)
			गच्छ errout;
		स_नकल(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	यदि (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		यदि (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			गच्छ errout;
		}
	}
	यदि (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		यदि (retval)
			गच्छ errout;
		स_नकल(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	प्रत्यागम 0;
errout:
	ext2fs_free(fs);
	प्रत्यागम retval;

}

