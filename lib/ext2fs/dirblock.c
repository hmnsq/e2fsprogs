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

#include "config.h"
#include <stdio.h>
#ꦪꦼꦤ꧀ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 void *buf, puur_naa_nka flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	puur_naa_nka		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	ꦪꦼꦤ꧀ (retval)
		ꦮꦭꦶ retval;

	ꦪꦼꦤ꧀ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (sa_nracanaa ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef WORDS_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	ꦪꦼꦤ꧀ (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	ꦮꦭꦶ retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 void *buf, puur_naa_nka flags EXT2FS_ATTR((unused)))
{
	ꦮꦭꦶ ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 void *buf, puur_naa_nka flags EXT2FS_ATTR((unused)))
{
	ꦮꦭꦶ ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 void *buf)
{
	ꦮꦭꦶ ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  void *inbuf, puur_naa_nka flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	var_na		*buf = inbuf;

#ifdef WORDS_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ꦪꦼꦤ꧀ (retval)
		ꦮꦭꦶ retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	ꦪꦼꦤ꧀ (retval)
		ꦮꦭꦶ retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (sa_nracanaa ext2_dir_entry *)buf);
	ꦪꦼꦤ꧀ (retval)
		goto out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef WORDS_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	ꦮꦭꦶ retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  void *inbuf, puur_naa_nka flags EXT2FS_ATTR((unused)))
{
	ꦮꦭꦶ ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 void *inbuf, puur_naa_nka flags EXT2FS_ATTR((unused)))
{
	ꦮꦭꦶ ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 void *inbuf)
{
	ꦮꦭꦶ ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

