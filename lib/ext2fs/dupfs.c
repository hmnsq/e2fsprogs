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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

errcode_t ext2fs_dup_handle(ext2_filsys src, ext2_filsys *dest)
{
	ext2_filsys	fs;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(src, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ struct_ext2_filsys), &fs);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	*fs = *src;
	fs->device_name = 0;
	fs->super = 0;
	fs->orig_super = 0;
	fs->ಗುಂಪು_desc = 0;
	fs->inode_map = 0;
	fs->ಖಂಡ_map = 0;
	fs->badblocks = 0;
	fs->dblist = 0;
	fs->mmp_buf = 0;
	fs->mmp_cmp = 0;
	fs->mmp_fd = -1;

	io_channel_bumpcount(fs->io);
	ಆದರೆ (fs->icache)
		fs->icache->refcount++;

	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(src->device_name)+1, &fs->device_name);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	strcpy(fs->device_name, src->device_name);

	retval = ext2fs_get_mem(SUPERBLOCK_ಗಾತ್ರ, &fs->super);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	ಸ್ಮರಣೆ_ನಕಲು(fs->super, src->super, SUPERBLOCK_ಗಾತ್ರ);

	retval = ext2fs_get_mem(SUPERBLOCK_ಗಾತ್ರ, &fs->orig_super);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	ಸ್ಮರಣೆ_ನಕಲು(fs->orig_super, src->orig_super, SUPERBLOCK_ಗಾತ್ರ);

	retval = ext2fs_get_array(fs->desc_ಖಂಡಗಳು, fs->blocksize,
				&fs->ಗುಂಪು_desc);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	ಸ್ಮರಣೆ_ನಕಲು(fs->ಗುಂಪು_desc, src->ಗುಂಪು_desc,
	       (size_t) fs->desc_ಖಂಡಗಳು * fs->blocksize);

	ಆದರೆ (src->inode_map) {
		retval = ext2fs_copy_bitmap(src->inode_map, &fs->inode_map);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}
	ಆದರೆ (src->ಖಂಡ_map) {
		retval = ext2fs_copy_bitmap(src->ಖಂಡ_map, &fs->ಖಂಡ_map);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}
	ಆದರೆ (src->badblocks) {
		retval = ext2fs_badblocks_copy(src->badblocks, &fs->badblocks);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}
	ಆದರೆ (src->dblist) {
		retval = ext2fs_copy_dblist(src->dblist, &fs->dblist);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}
	ಆದರೆ (src->mmp_buf) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_buf);
		ಆದರೆ (retval)
			ಹೋಗು errout;
		ಸ್ಮರಣೆ_ನಕಲು(fs->mmp_buf, src->mmp_buf, src->blocksize);
	}
	ಆದರೆ (src->mmp_fd >= 0) {
		fs->mmp_fd = dup(src->mmp_fd);
		ಆದರೆ (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_ನೇರ;
			ಹೋಗು errout;
		}
	}
	ಆದರೆ (src->mmp_cmp) {
		retval = ext2fs_mmp_get_mem(src, &fs->mmp_cmp);
		ಆದರೆ (retval)
			ಹೋಗು errout;
		ಸ್ಮರಣೆ_ನಕಲು(fs->mmp_cmp, src->mmp_cmp, src->blocksize);
	}
	*dest = fs;
	ಹಿಂತಿರುಗಿ 0;
errout:
	ext2fs_free(fs);
	ಹಿಂತಿರುಗಿ retval;

}

