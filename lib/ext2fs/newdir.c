/*
 * newdir.c --- create a new directory block
 *
 * Copyright (C) 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

#నిర్వచితం_కాకపోతే EXT2_FT_DIR
#నిర్వచించు EXT2_FT_DIR		2
#అయితే_ముగింపు

/*
 * Create new directory block
 */
errcode_t ext2fs_new_dir_ఖండం(ext2_filsys fs, ext2_ino_t dir_ino,
			       ext2_ino_t మాతృ_ino, అక్షరం **ఖండం)
{
	నిర్మాణం ext2_dir_నమోదు 	*dir = ఖాళీ;
	errcode_t		retval;
	అక్షరం			*buf;
	పూర్ణాంకం			rec_len;
	పూర్ణాంకం			filetype = 0;
	నిర్మాణం ext2_dir_నమోదు_తోక	*t;
	పూర్ణాంకం			csum_కొలత = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ఒకవేళ (retval)
		తిరిగి retval;
	memset(buf, 0, fs->blocksize);
	dir = (నిర్మాణం ext2_dir_నమోదు *) buf;

	ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_కొలత = పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక);

	retval = ext2fs_సమితి_rec_len(fs, fs->blocksize - csum_కొలత, dir);
	ఒకవేళ (retval) {
		ext2fs_free_mem(&buf);
		తిరిగి retval;
	}

	ఒకవేళ (dir_ino) {
		ఒకవేళ (ext2fs_has_feature_filetype(fs->super))
			filetype = EXT2_FT_DIR;
		/*
		 * Set up entry for '.'
		 */
		dir->inode = dir_ino;
		ext2fs_dirent_సమితి_name_len(dir, 1);
		ext2fs_dirent_సమితి_file_type(dir, filetype);
		dir->name[0] = '.';
		rec_len = (fs->blocksize - csum_కొలత) - EXT2_DIR_REC_LEN(1);
		dir->rec_len = EXT2_DIR_REC_LEN(1);

		/*
		 * Set up entry for '..'
		 */
		dir = (నిర్మాణం ext2_dir_నమోదు *) (buf + dir->rec_len);
		retval = ext2fs_సమితి_rec_len(fs, rec_len, dir);
		ఒకవేళ (retval) {
			ext2fs_free_mem(&buf);
			తిరిగి retval;
		}
		dir->inode = మాతృ_ino;
		ext2fs_dirent_సమితి_name_len(dir, 2);
		ext2fs_dirent_సమితి_file_type(dir, filetype);
		dir->name[0] = '.';
		dir->name[1] = '.';

	}

	ఒకవేళ (csum_కొలత) {
		t = EXT2_DIRENT_తోక(buf, fs->blocksize);
		ext2fs_initialize_dirent_తోక(fs, t);
	}
	*ఖండం = buf;
	తిరిగి 0;
}

/*
 * Create new directory on inline data
 */
errcode_t ext2fs_new_dir_inline_దత్తాంశం(ext2_filsys fs,
				     ext2_ino_t dir_ino EXT2FS_ATTR((వాడని)),
				     ext2_ino_t మాతృ_ino, __u32 *iblock)
{
	నిర్మాణం ext2_dir_నమోదు 	*dir = ఖాళీ;
	errcode_t		retval;
	పూర్ణాంకం			rec_len;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	iblock[0] = ext2fs_cpu_గమ్యం_le32(మాతృ_ino);

	dir = (నిర్మాణం ext2_dir_నమోదు *)((అక్షరం *)iblock +
					EXT4_INLINE_దత్తాంశం_DOTDOT_కొలత);
	dir->inode = 0;
	rec_len = EXT4_MIN_INLINE_దత్తాంశం_కొలత - EXT4_INLINE_దత్తాంశం_DOTDOT_కొలత;
	retval = ext2fs_సమితి_rec_len(fs, rec_len, dir);
	ఒకవేళ (retval)
		వెళ్ళు errout;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	retval = ext2fs_dirent_swab_out2(fs, (అక్షరం *)dir, rec_len, 0);
	ఒకవేళ (retval)
		వెళ్ళు errout;
#అయితే_ముగింపు

errout:
	తిరిగి retval;
}
