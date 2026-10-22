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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 શૂન્ય *buf, પૂર્ણાંક flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	પૂર્ણાંક		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	જો (retval)
		પાછા retval;

	જો (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (સંરચના ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef શબ્દો_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	જો (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	પાછા retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 શૂન્ય *buf, પૂર્ણાંક flags EXT2FS_ATTR((unused)))
{
	પાછા ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 શૂન્ય *buf, પૂર્ણાંક flags EXT2FS_ATTR((unused)))
{
	પાછા ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 શૂન્ય *buf)
{
	પાછા ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  શૂન્ય *inbuf, પૂર્ણાંક flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	અક્ષર		*buf = inbuf;

#ifdef શબ્દો_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	જો (retval)
		પાછા retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	જો (retval)
		પાછા retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (સંરચના ext2_dir_entry *)buf);
	જો (retval)
		જાઓ out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef શબ્દો_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	પાછા retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  શૂન્ય *inbuf, પૂર્ણાંક flags EXT2FS_ATTR((unused)))
{
	પાછા ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 શૂન્ય *inbuf, પૂર્ણાંક flags EXT2FS_ATTR((unused)))
{
	પાછા ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 શૂન્ય *inbuf)
{
	પાછા ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

