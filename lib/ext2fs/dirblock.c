/*
 * dirblock.c --- directory block routines.
 *
 * Copyright (C) 1995, 1996 Theodore Ts'o.
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
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t ఖండం,
				 శూన్య *buf, పూర్ణాంకం flags EXT2FS_ATTR((వాడని)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	పూర్ణాంకం		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, ఖండం, 1, buf);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_ఖండం_csum_verify(fs, ino,
					  (నిర్మాణం ext2_dir_నమోదు *)buf))
		corrupt = 1;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	retval = ext2fs_dirent_swab_లోపల(fs, buf, flags);
#అయితే_ముగింపు
	ఒకవేళ (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	తిరిగి retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t ఖండం,
				 శూన్య *buf, పూర్ణాంకం flags EXT2FS_ATTR((వాడని)))
{
	తిరిగి ext2fs_read_dir_block4(fs, ఖండం, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t ఖండం,
				 శూన్య *buf, పూర్ణాంకం flags EXT2FS_ATTR((వాడని)))
{
	తిరిగి ext2fs_read_dir_block3(fs, ఖండం, buf, flags);
}

errcode_t ext2fs_read_dir_ఖండం(ext2_filsys fs, blk_t ఖండం,
				 శూన్య *buf)
{
	తిరిగి ext2fs_read_dir_block3(fs, ఖండం, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t ఖండం,
				  శూన్య *inbuf, పూర్ణాంకం flags EXT2FS_ATTR((వాడని)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	అక్షరం		*buf = inbuf;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ఒకవేళ (retval)
		తిరిగి retval;
	జ్ఞాపకం_నకలు(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_బహిర్గతం(fs, buf, flags);
	ఒకవేళ (retval)
		తిరిగి retval;
#అయితే_ముగింపు
	retval = ext2fs_dir_ఖండం_csum_సమితి(fs, ino,
					   (నిర్మాణం ext2_dir_నమోదు *)buf);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;

	retval = io_channel_write_blk64(fs->io, ఖండం, 1, buf);

బహిర్గతం:
#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_free_mem(&buf);
#అయితే_ముగింపు
	తిరిగి retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t ఖండం,
				  శూన్య *inbuf, పూర్ణాంకం flags EXT2FS_ATTR((వాడని)))
{
	తిరిగి ext2fs_write_dir_block4(fs, ఖండం, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t ఖండం,
				 శూన్య *inbuf, పూర్ణాంకం flags EXT2FS_ATTR((వాడని)))
{
	తిరిగి ext2fs_write_dir_block3(fs, ఖండం, inbuf, flags);
}

errcode_t ext2fs_write_dir_ఖండం(ext2_filsys fs, blk_t ఖండం,
				 శూన్య *inbuf)
{
	తిరిగి ext2fs_write_dir_block3(fs, ఖండం, inbuf, 0);
}

