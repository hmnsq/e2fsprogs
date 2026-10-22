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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 ശൂന്യം *buf, പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	പൂർണ്ണസംഖ്യ		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (ഘടന ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef വാക്കുകൾ_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	എങ്കിൽ (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	തിരികെ retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 ശൂന്യം *buf, പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)))
{
	തിരികെ ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 ശൂന്യം *buf, പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)))
{
	തിരികെ ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 ശൂന്യം *buf)
{
	തിരികെ ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  ശൂന്യം *inbuf, പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	അക്ഷരം		*buf = inbuf;

#ifdef വാക്കുകൾ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	എങ്കിൽ (retval)
		തിരികെ retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	എങ്കിൽ (retval)
		തിരികെ retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (ഘടന ext2_dir_entry *)buf);
	എങ്കിൽ (retval)
		പോകുക out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef വാക്കുകൾ_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	തിരികെ retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  ശൂന്യം *inbuf, പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)))
{
	തിരികെ ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 ശൂന്യം *inbuf, പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)))
{
	തിരികെ ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 ശൂന്യം *inbuf)
{
	തിരികെ ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

