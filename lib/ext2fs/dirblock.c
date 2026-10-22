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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t block,
				 शून्य *buf, पूर्णाङ्क flags EXT2FS_ATTR((unused)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	पूर्णाङ्क		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	जइ (retval)
		प्रत्यागम retval;

	जइ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (संरचना ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef शब्दाः_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	जइ (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	प्रत्यागम retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 शून्य *buf, पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	प्रत्यागम ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 शून्य *buf, पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	प्रत्यागम ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 शून्य *buf)
{
	प्रत्यागम ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  शून्य *inbuf, पूर्णाङ्क flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	वर्ण		*buf = inbuf;

#ifdef शब्दाः_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	जइ (retval)
		प्रत्यागम retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	जइ (retval)
		प्रत्यागम retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (संरचना ext2_dir_entry *)buf);
	जइ (retval)
		गच्छ out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef शब्दाः_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	प्रत्यागम retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  शून्य *inbuf, पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	प्रत्यागम ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 शून्य *inbuf, पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	प्रत्यागम ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 शून्य *inbuf)
{
	प्रत्यागम ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

