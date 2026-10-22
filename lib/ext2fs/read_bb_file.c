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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       வெறுமை *priv_data,
			       வெறுமை (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       எழுத்து *badstr,
					       வெறுமை *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	முழு		மொத்தம்;
	எழுத்து		buf[128];

	எனில் (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		எனில் (retval)
			திரும்பு retval;
	}

	போது (!feof (f)) {
		எனில் (fgets(buf, அளவு(buf), f) == வெற்று)
			நிறுத்து;
		மொத்தம் = sscanf(buf, "%llu", &blockno);
		எனில் (மொத்தம் <= 0)
			தொடர்;
		/* Badblocks isn't going to be updated for 64bit */
		எனில் (blockno >> 32)
			திரும்பு EOVERFLOW;
		எனில் (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_மொத்தம்(fs->super)))) {
			எனில் (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			தொடர்;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		எனில் (retval)
			திரும்பு retval;
	}
	திரும்பு 0;
}

கட்டமைப்பு compat_struct {
	வெறுமை (*invalid)(ext2_filsys, blk_t);
};

நிலையான வெறுமை call_compat_invalid(ext2_filsys fs, blk_t blk,
				எழுத்து *badstr EXT2FS_ATTR((unused)),
				வெறுமை *priv_data)
{
	கட்டமைப்பு compat_struct *st;

	st = (கட்டமைப்பு compat_struct *) priv_data;
	எனில் (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      வெறுமை (*invalid)(ext2_filsys fs, blk_t blk))
{
	கட்டமைப்பு compat_struct st;

	st.invalid = invalid;

	திரும்பு ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


