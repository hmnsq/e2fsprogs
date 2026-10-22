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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <time.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం struct_ext2_filsys), &fs);
	ఒకవేళ (retval)
		తిరిగి retval;

	*fs = *src;
	fs->device_name = 0;
	fs->super = 0;
	fs->orig_super = 0;
	fs->సమూహం_desc = 0;
	fs->inode_map = 0;
	fs->ఖండం_map = 0;
	fs->badblocks = 0;
	fs->dblist = 0;
	fs->mmp_buf = 0;
	fs->mmp_cmp = 0;
	fs->mmp_fd = -1;

	io_channel_bumpcount(fs->io);
	ఒకవేళ (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(src->device_name)+1, &fs->device_name);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_కొలత, &fs->super);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	జ్ఞాపకం_నకలు(fs->super, src->super, SUPERBLOCK_కొలత);

	retval = ext2fs_get_mem(SUPERBLOCK_కొలత, &fs->orig_super);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	జ్ఞాపకం_నకలు(fs->orig_super, src->orig_super, SUPERBLOCK_కొలత);

	retval = ext2fs_get_array(fs->desc_ఖండాలు, fs->blocksize,
				&fs->సమూహం_desc);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	జ్ఞాపకం_నకలు(fs->సమూహం_desc, src->సమూహం_desc,
	       (size_t) fs->desc_ఖండాలు * fs->blocksize);

	ఒకవేళ (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}
	ఒకవేళ (src->ఖండం_map) {
		retval = ext2fs_copy_bitmap(src->ఖండం_map, &fs->ఖండం_map);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}
	ఒకవేళ (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}
	ఒకవేళ (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}
	ఒకవేళ (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		ఒకవేళ (retval)
			వెళ్ళు errout;
		జ్ఞాపకం_నకలు(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	ఒకవేళ (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		ఒకవేళ (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_ప్రత్యక్ష;
			వెళ్ళు errout;
		}
	}
	ఒకవేళ (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		ఒకవేళ (retval)
			వెళ్ళు errout;
		జ్ఞాపకం_నకలు(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	తిరిగి 0;
errout:
	ext2fs_free(fs);
	తిరిగి retval;

}

