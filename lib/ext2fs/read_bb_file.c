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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       શૂન્ય *priv_data,
			       શૂન્ય (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       અક્ષર *badstr,
					       શૂન્ય *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	પૂર્ણાંક		ગણતરી;
	અક્ષર		buf[128];

	જો (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		જો (retval)
			પાછા retval;
	}

	જ્યારે (!feof (f)) {
		જો (fgets(buf, કદ(buf), f) == ખાલી)
			તોડો;
		ગણતરી = sscanf(buf, "%llu", &blockno);
		જો (ગણતરી <= 0)
			ચાલુ;
		/* Badblocks isn't going to be updated for 64bit */
		જો (blockno >> 32)
			પાછા EOVERFLOW;
		જો (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_ગણતરી(fs->super)))) {
			જો (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			ચાલુ;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		જો (retval)
			પાછા retval;
	}
	પાછા 0;
}

સંરચના compat_struct {
	શૂન્ય (*invalid)(ext2_filsys, blk_t);
};

સ્થિર શૂન્ય call_compat_invalid(ext2_filsys fs, blk_t blk,
				અક્ષર *badstr EXT2FS_ATTR((unused)),
				શૂન્ય *priv_data)
{
	સંરચના compat_struct *st;

	st = (સંરચના compat_struct *) priv_data;
	જો (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      શૂન્ય (*invalid)(ext2_filsys fs, blk_t blk))
{
	સંરચના compat_struct st;

	st.invalid = invalid;

	પાછા ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


