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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       শূন্য *priv_data,
			       শূন্য (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       ৱর্ণ *badstr,
					       শূন্য *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	পূর্ণাঙ্ক		গুনতি;
	ৱর্ণ		buf[128];

	যদি (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		যদি (retval)
			ঘূৰাই retval;
	}

	যেতিয়ালৈ (!feof (f)) {
		যদি (fgets(buf, পরিমাণ(buf), f) == রিক্ত)
			ভঙ্গ;
		গুনতি = sscanf(buf, "%llu", &blockno);
		যদি (গুনতি <= 0)
			অনুৱর্ত;
		/* Badblocks isn't going to be updated for 64bit */
		যদি (blockno >> 32)
			ঘূৰাই EOVERFLOW;
		যদি (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_গুনতি(fs->super)))) {
			যদি (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			অনুৱর্ত;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		যদি (retval)
			ঘূৰাই retval;
	}
	ঘূৰাই 0;
}

সংরচনা compat_struct {
	শূন্য (*invalid)(ext2_filsys, blk_t);
};

স্থির শূন্য call_compat_invalid(ext2_filsys fs, blk_t blk,
				ৱর্ণ *badstr EXT2FS_ATTR((unused)),
				শূন্য *priv_data)
{
	সংরচনা compat_struct *st;

	st = (সংরচনা compat_struct *) priv_data;
	যদি (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      শূন্য (*invalid)(ext2_filsys fs, blk_t blk))
{
	সংরচনা compat_struct st;

	st.invalid = invalid;

	ঘূৰাই ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


