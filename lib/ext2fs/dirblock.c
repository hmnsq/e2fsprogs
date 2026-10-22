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
#सचे HAVE_UNISTD_H
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
	सचे (retval)
		पटिनिवत्त retval;

	सचे (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_block_csum_verify(fs, ino,
					  (संरचना ext2_dir_entry *)buf))
		corrupt = 1;

#ifdef शब्दाः_BIGENDIAN
	retval = ext2fs_dirent_swab_in(fs, buf, flags);
#endif
	सचे (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	पटिनिवत्त retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t block,
				 शून्य *buf, पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	पटिनिवत्त ext2fs_read_dir_block4(fs, block, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t block,
				 शून्य *buf, पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	पटिनिवत्त ext2fs_read_dir_block3(fs, block, buf, flags);
}

errcode_t ext2fs_read_dir_block(ext2_filsys fs, blk_t block,
				 शून्य *buf)
{
	पटिनिवत्त ext2fs_read_dir_block3(fs, block, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t block,
				  शून्य *inbuf, पूर्णाङ्क flags EXT2FS_ATTR((unused)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	वर्ण		*buf = inbuf;

#ifdef शब्दाः_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	सचे (retval)
		पटिनिवत्त retval;
	memcpy(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_out(fs, buf, flags);
	सचे (retval)
		पटिनिवत्त retval;
#endif
	retval = ext2fs_dir_block_csum_set(fs, ino,
					   (संरचना ext2_dir_entry *)buf);
	सचे (retval)
		गच्छ out;

	retval = io_channel_write_blk64(fs->io, block, 1, buf);

out:
#ifdef शब्दाः_BIGENDIAN
	ext2fs_free_mem(&buf);
#endif
	पटिनिवत्त retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t block,
				  शून्य *inbuf, पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	पटिनिवत्त ext2fs_write_dir_block4(fs, block, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t block,
				 शून्य *inbuf, पूर्णाङ्क flags EXT2FS_ATTR((unused)))
{
	पटिनिवत्त ext2fs_write_dir_block3(fs, block, inbuf, flags);
}

errcode_t ext2fs_write_dir_block(ext2_filsys fs, blk_t block,
				 शून्य *inbuf)
{
	पटिनिवत्त ext2fs_write_dir_block3(fs, block, inbuf, 0);
}

