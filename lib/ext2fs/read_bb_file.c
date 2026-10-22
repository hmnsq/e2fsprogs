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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       ଶୂନ୍ୟ *priv_data,
			       ଶୂନ୍ୟ (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       ଅକ୍ଷର *badstr,
					       ଶୂନ୍ୟ *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ଗଣନା_2;
	ଅକ୍ଷର		buf[128];

	ଯଦି (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}

	ଯେତେବେଳେ (!feof (f)) {
		ଯଦି (fgets(buf, ଆକାର(buf), f) == ଖାଲି)
			ଭାଙ୍ଗ;
		ଗଣନା_2 = sscanf(buf, "%llu", &blockno);
		ଯଦି (ଗଣନା_2 <= 0)
			ଜାରି;
		/* Badblocks isn't going to be updated for 64bit */
		ଯଦି (blockno >> 32)
			ଫେରନ୍ତୁ EOVERFLOW;
		ଯଦି (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_ଗଣନା(fs->super)))) {
			ଯଦି (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			ଜାରି;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା compat_struct {
	ଶୂନ୍ୟ (*invalid)(ext2_filsys, blk_t);
};

ସ୍ଥିର ଶୂନ୍ୟ call_compat_invalid(ext2_filsys fs, blk_t blk,
				ଅକ୍ଷର *badstr EXT2FS_ATTR((unused)),
				ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା compat_struct *st;

	st = (ସଂରଚନା compat_struct *) priv_data;
	ଯଦି (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      ଶୂନ୍ୟ (*invalid)(ext2_filsys fs, blk_t blk))
{
	ସଂରଚନା compat_struct st;

	st.invalid = invalid;

	ଫେରନ୍ତୁ ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


