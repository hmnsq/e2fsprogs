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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

errcode_t ext2fs_read_dir_block4(ext2_filsys fs, blk64_t ಖಂಡ,
				 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)),
				 ext2_ino_t ino)
{
	errcode_t	retval;
	ಪೂರ್ಣಾಂಕ		corrupt = 0;

	retval = io_channel_read_blk64(fs->io, ಖಂಡ, 1, buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_dir_ಖಂಡ_csum_verify(fs, ino,
					  (ರಚನೆ ext2_dir_ನಮೂದು *)buf))
		corrupt = 1;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	retval = ext2fs_dirent_swab_ಒಳ(fs, buf, flags);
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (!retval && corrupt)
		retval = EXT2_ET_DIR_CSUM_INVALID;
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_read_dir_block3(ext2_filsys fs, blk64_t ಖಂಡ,
				 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ ext2fs_read_dir_block4(fs, ಖಂಡ, buf, flags, 0);
}

errcode_t ext2fs_read_dir_block2(ext2_filsys fs, blk_t ಖಂಡ,
				 ಶೂನ್ಯ *buf, ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ ext2fs_read_dir_block3(fs, ಖಂಡ, buf, flags);
}

errcode_t ext2fs_read_dir_ಖಂಡ(ext2_filsys fs, blk_t ಖಂಡ,
				 ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ ext2fs_read_dir_block3(fs, ಖಂಡ, buf, 0);
}


errcode_t ext2fs_write_dir_block4(ext2_filsys fs, blk64_t ಖಂಡ,
				  ಶೂನ್ಯ *inbuf, ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)),
				  ext2_ino_t ino)
{
	errcode_t	retval;
	ಅಕ್ಷರ		*buf = inbuf;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಸ್ಮರಣೆ_ನಕಲು(buf, inbuf, fs->blocksize);
	retval = ext2fs_dirent_swab_ಹೊರಹೋಗು(fs, buf, flags);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
#ಆದರೆ_ಅಂತ್ಯ
	retval = ext2fs_dir_ಖಂಡ_csum_ಗಣ(fs, ino,
					   (ರಚನೆ ext2_dir_ನಮೂದು *)buf);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;

	retval = io_channel_write_blk64(fs->io, ಖಂಡ, 1, buf);

ಹೊರಹೋಗು:
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_free_mem(&buf);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_write_dir_block3(ext2_filsys fs, blk64_t ಖಂಡ,
				  ಶೂನ್ಯ *inbuf, ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ ext2fs_write_dir_block4(fs, ಖಂಡ, inbuf, flags, 0);
}

errcode_t ext2fs_write_dir_block2(ext2_filsys fs, blk_t ಖಂಡ,
				 ಶೂನ್ಯ *inbuf, ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ ext2fs_write_dir_block3(fs, ಖಂಡ, inbuf, flags);
}

errcode_t ext2fs_write_dir_ಖಂಡ(ext2_filsys fs, blk_t ಖಂಡ,
				 ಶೂನ್ಯ *inbuf)
{
	ಹಿಂತಿರುಗಿ ext2fs_write_dir_block3(fs, ಖಂಡ, inbuf, 0);
}

