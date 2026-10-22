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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 ଶୂନ୍ୟ *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (ସଂରଚନା ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	ଯଦି (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 ଶୂନ୍ୟ *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)))
{
	ଫେରନ୍ତୁ ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 ଶୂନ୍ୟ *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)))
{
	ଫେରନ୍ତୁ ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  ଶୂନ୍ୟ *inbuf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	ଅକ୍ଷର		*buf = inbuf;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (ସଂରଚନା ext2_dir_entry *)buf);
	ଯଦି (retval)
		ଯାଅ out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  ଶୂନ୍ୟ *inbuf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)))
{
	ଫେରନ୍ତୁ ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 ଶୂନ୍ୟ *inbuf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)))
{
	ଫେରନ୍ତୁ ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 ଶୂନ୍ୟ *inbuf)
{
	ଫେରନ୍ତୁ ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

