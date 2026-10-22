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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 শূন্য *buf, পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	পূর্ণসংখ্যা		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	যদি (retval)
		ফেরত retval;

	যদি (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (কাঠামো ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef শব্দগুলি_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	যদি (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	ফেরত retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 শূন্য *buf, পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)))
{
	ফেরত ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 শূন্য *buf, পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)))
{
	ফেরত ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 শূন্য *buf)
{
	ফেরত ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  শূন্য *inbuf, পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	অক্ষর		*buf = inbuf;

#ifdef শব্দগুলি_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	যদি (retval)
		ফেরত retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	যদি (retval)
		ফেরত retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (কাঠামো ext2_dir_entry *)buf);
	যদি (retval)
		যাও out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	ফেরত retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  শূন্য *inbuf, পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)))
{
	ফেরত ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 শূন্য *inbuf, পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)))
{
	ফেরত ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 শূন্য *inbuf)
{
	ফেরত ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

