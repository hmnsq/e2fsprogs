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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       ಶೂನ್ಯ *priv_data,
			       ಶೂನ್ಯ (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       ವರ್ಣ *badstr,
					       ಶೂನ್ಯ *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	ಪೂರ್ಣಾಙ್ಕ		ಎಣಿಕೆ;
	ವರ್ಣ		buf[128];

	ಯದಿ (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	ಯಾವತ್ (!feof (f)) {
		ಯದಿ (fgets(buf, ಪರಿಮಾಣ(buf), f) == ರಿಕ್ತ)
			ಭಙ್ಗ;
		ಎಣಿಕೆ = sscanf(buf, "%llu", &blockno);
		ಯದಿ (ಎಣಿಕೆ <= 0)
			ಅನುವರ್ತ;
		/* Badblocks isn't going to be updated for 64bit */
		ಯದಿ (blockno >> 32)
			ಪ್ರತ್ಯಾಗಮ EOVERFLOW;
		ಯದಿ (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_ಎಣಿಕೆ(fs->super)))) {
			ಯದಿ (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			ಅನುವರ್ತ;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ compat_struct {
	ಶೂನ್ಯ (*invalid)(ext2_filsys, blk_t);
};

ಸ್ಥಿರ ಶೂನ್ಯ call_compat_invalid(ext2_filsys fs, blk_t blk,
				ವರ್ಣ *badstr EXT2FS_ATTR((unused)),
				ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ compat_struct *st;

	st = (ಸಂರಚನಾ compat_struct *) priv_data;
	ಯದಿ (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      ಶೂನ್ಯ (*invalid)(ext2_filsys fs, blk_t blk))
{
	ಸಂರಚನಾ compat_struct st;

	st.invalid = invalid;

	ಪ್ರತ್ಯಾಗಮ ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


