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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 வெறுமை *buf, முழு flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	முழு		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	எனில் (retval)
		திரும்பு retval;

	எனில் (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (கட்டமைப்பு ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef சொற்கள்_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	எனில் (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	திரும்பு retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 வெறுமை *buf, முழு flags EXT2FS_ATTR((unused)))
{
	திரும்பு ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 வெறுமை *buf, முழு flags EXT2FS_ATTR((unused)))
{
	திரும்பு ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 வெறுமை *buf)
{
	திரும்பு ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  வெறுமை *inbuf, முழு flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	எழுத்து		*buf = inbuf;

#ifdef சொற்கள்_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	எனில் (retval)
		திரும்பு retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	எனில் (retval)
		திரும்பு retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (கட்டமைப்பு ext2_dir_entry *)buf);
	எனில் (retval)
		செல் out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef சொற்கள்_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	திரும்பு retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  வெறுமை *inbuf, முழு flags EXT2FS_ATTR((unused)))
{
	திரும்பு ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 வெறுமை *inbuf, முழு flags EXT2FS_ATTR((unused)))
{
	திரும்பு ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 வெறுமை *inbuf)
{
	திரும்பு ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

