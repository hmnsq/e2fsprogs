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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	ಪೂರ್ಣಾಙ್ಕ		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (ಸಂರಚನಾ ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef ಪದಗಳು_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	ಯದಿ (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)))
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)))
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  ಶೂನ್ಯ *inbuf, ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	ವರ್ಣ		*buf = inbuf;

#ifdef ಪದಗಳು_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (ಸಂರಚನಾ ext2_dir_entry *)buf);
	ಯದಿ (retval)
		ಗಚ್ಛ out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef ಪದಗಳು_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  ಶೂನ್ಯ *inbuf, ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)))
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 ಶೂನ್ಯ *inbuf, ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)))
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 ಶೂನ್ಯ *inbuf)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

