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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t ਖੰਡ,
				 ਖਾਲੀ *buf, ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	ਪੂਰਨ		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, ਖੰਡ, 1, buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_ਖੰਡ_csum_verify(fs, ino,
					  (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)buf))
		corrupt = 1;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	retval = ext2fs_dirent_swab_ਵਿੱਚ(fs, buf, flags);
#ਜੇ_ਅੰਤ
	ਜੇ (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	ਵਾਪਸ retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t ਖੰਡ,
				 ਖਾਲੀ *buf, ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ ext2fs_read_dir_block4(fs, ਖੰਡ, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t ਖੰਡ,
				 ਖਾਲੀ *buf, ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ ext2fs_read_dir_block3(fs, ਖੰਡ, buf, flags);
}

errcode_t ext2fs_read_dir_ਖੰਡ(ext2_filsys fs, blk_t ਖੰਡ,
				 ਖਾਲੀ *buf)
{
	ਵਾਪਸ ext2fs_read_dir_block3(fs, ਖੰਡ, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t ਖੰਡ,
				  ਖਾਲੀ *inbuf, ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	ਅੱਖਰ		*buf = inbuf;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_ਬਾਹਰਲਾ(fs, buf, flags);
	ਜੇ (retval)
		ਵਾਪਸ retval;
#ਜੇ_ਅੰਤ
	retval = ext2fs_dir_ਖੰਡ_csum_ਸੈੱਟ(fs, ino,
					   (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)buf);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;

	retval = io_channel_write_blk64(fs->io, ਖੰਡ, 1, buf);

ਬਾਹਰਲਾ:
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_free_mem(&buf);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t ਖੰਡ,
				  ਖਾਲੀ *inbuf, ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ ext2fs_write_dir_block4(fs, ਖੰਡ, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t ਖੰਡ,
				 ਖਾਲੀ *inbuf, ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ ext2fs_write_dir_block3(fs, ਖੰਡ, inbuf, flags);
}

errcode_t ext2fs_write_dir_ਖੰਡ(ext2_filsys fs, blk_t ਖੰਡ,
				 ਖਾਲੀ *inbuf)
{
	ਵਾਪਸ ext2fs_write_dir_block3(fs, ਖੰਡ, inbuf, 0);
}

