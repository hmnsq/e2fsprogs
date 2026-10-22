/*
 * read_bb_file.c --- read a list of bad blocks from a FILE *
 *
 * Copyright (C) 1994, 1995, 2000 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ,
			       ಶೂನ್ಯ (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       ಅಕ್ಷರ *badstr,
					       ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ))
{
	errcode_t	retval;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ blockno;
	ಪೂರ್ಣಾಂಕ		ಎಣಿಕೆ_2;
	ಅಕ್ಷರ		buf[128];

	ಆದರೆ (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	ಆಗ (!feof (f)) {
		ಆದರೆ (fgets(buf, ಗಾತ್ರ(buf), f) == ಖಾಲಿ)
			ನಿಲ್ಲಿಸು;
		ಎಣಿಕೆ_2 = ಅಕ್ಷರಸರಣಿ_ಓದು(buf, "%llu", &blockno);
		ಆದರೆ (ಎಣಿಕೆ_2 <= 0)
			ಮುಂದುವರಿ;
		/* Badblocks isn't going to be updated for 64bit */
		ಆದರೆ (blockno >> 32)
			ಹಿಂತಿರುಗಿ EOVERFLOW;
		ಆದರೆ (fs &&
		    ((blockno < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
		     (blockno >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)))) {
			ಆದರೆ (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_ದತ್ತಾಂಶ);
			ಮುಂದುವರಿ;
		}
		retval = ext2fs_badblocks_list_ಕೂಡಿಸು(*bb_list, (blk64_t) blockno);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ compat_struct {
	ಶೂನ್ಯ (*invalid)(ext2_filsys, blk_t);
};

ಸ್ಥಿರ ಶೂನ್ಯ call_compat_invalid(ext2_filsys fs, blk_t blk,
				ಅಕ್ಷರ *badstr EXT2FS_ATTR((ಬಳಸದ)),
				ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ compat_struct *st;

	st = (ರಚನೆ compat_struct *) priv_ದತ್ತಾಂಶ;
	ಆದರೆ (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      ಶೂನ್ಯ (*invalid)(ext2_filsys fs, blk_t blk))
{
	ರಚನೆ compat_struct st;

	st.invalid = invalid;

	ಹಿಂತಿರುಗಿ ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


