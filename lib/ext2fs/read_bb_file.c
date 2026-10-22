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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       تهی *priv_data,
			       تهی (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       نویسه *badstr,
					       تهی *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	صحیح		شمار;
	نویسه		buf[128];

	اگر (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		اگر (retval)
			بازگشت retval;
	}

	وقتی (!feof (f)) {
		اگر (fgets(buf, اندازه(buf), f) == پوچ)
			بشکن;
		شمار = sscanf(buf, "%llu", &blockno);
		اگر (شمار <= 0)
			ادامه;
		/* Badblocks isn't going to be updated for 64bit */
		اگر (blockno >> 32)
			بازگشت EOVERFLOW;
		اگر (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_شمار(fs->super)))) {
			اگر (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			ادامه;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		اگر (retval)
			بازگشت retval;
	}
	بازگشت 0;
}

ساختار compat_struct {
	تهی (*invalid)(ext2_filsys, blk_t);
};

ایستا تهی call_compat_invalid(ext2_filsys fs, blk_t blk,
				نویسه *badstr EXT2FS_ATTR((unused)),
				تهی *priv_data)
{
	ساختار compat_struct *st;

	st = (ساختار compat_struct *) priv_data;
	اگر (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      تهی (*invalid)(ext2_filsys fs, blk_t blk))
{
	ساختار compat_struct st;

	st.invalid = invalid;

	بازگشت ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


