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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ struct_ext2_filsys), &fs);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	*fs = *src;
	fs->device_name = 0;
	fs->super = 0;
	fs->orig_super = 0;
	fs->ਸਮੂਹ_desc = 0;
	fs->inode_map = 0;
	fs->ਖੰਡ_map = 0;
	fs->badblocks = 0;
	fs->dblist = 0;
	fs->mmp_buf = 0;
	fs->mmp_cmp = 0;
	fs->mmp_fd = -1;

	io_channel_bumpcount(fs->io);
	ਜੇ (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(src->device_name)+1, &fs->device_name);
	ਜੇ (retval)
		ਜਾਓ errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_ਮਾਪ, &fs->super);
	ਜੇ (retval)
		ਜਾਓ errout;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->super, src->super, SUPERBLOCK_ਮਾਪ);

	retval = ext2fs_get_mem(SUPERBLOCK_ਮਾਪ, &fs->orig_super);
	ਜੇ (retval)
		ਜਾਓ errout;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->orig_super, src->orig_super, SUPERBLOCK_ਮਾਪ);

	retval = ext2fs_get_array(fs->desc_ਖੰਡਾਂ, fs->blocksize,
				&fs->ਸਮੂਹ_desc);
	ਜੇ (retval)
		ਜਾਓ errout;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->ਸਮੂਹ_desc, src->ਸਮੂਹ_desc,
	       (size_t) fs->desc_ਖੰਡਾਂ * fs->blocksize);

	ਜੇ (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		ਜੇ (retval)
			ਜਾਓ errout;
	}
	ਜੇ (src->ਖੰਡ_map) {
		retval = ext2fs_copy_bitmap(src->ਖੰਡ_map, &fs->ਖੰਡ_map);
		ਜੇ (retval)
			ਜਾਓ errout;
	}
	ਜੇ (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		ਜੇ (retval)
			ਜਾਓ errout;
	}
	ਜੇ (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		ਜੇ (retval)
			ਜਾਓ errout;
	}
	ਜੇ (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		ਜੇ (retval)
			ਜਾਓ errout;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	ਜੇ (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		ਜੇ (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_ਸਿੱਧਾ;
			ਜਾਓ errout;
		}
	}
	ਜੇ (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		ਜੇ (retval)
			ਜਾਓ errout;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	ਵਾਪਸ 0;
errout:
	ext2fs_free(fs);
	ਵਾਪਸ retval;

}

