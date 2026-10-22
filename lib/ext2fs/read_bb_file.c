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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       فراغ *priv_data,
			       فراغ (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       حرف *badstr,
					       فراغ *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	صحيح		count;
	حرف		buf[128];

	ܐܢ (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ܐܢ (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		ܐܢ (retval)
			ܦܢܐ retval;
	}

	ܟܡܐ (!feof (f)) {
		ܐܢ (fgets(buf, حجم(buf), f) == عدم)
			اكسر;
		count = sscanf(buf, "%llu", &blockno);
		ܐܢ (count <= 0)
			استمر;
		/* Badblocks isn't going to be updated for 64bit */
		ܐܢ (blockno >> 32)
			ܦܢܐ EOVERFLOW;
		ܐܢ (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_count(fs->super)))) {
			ܐܢ (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			استمر;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		ܐܢ (retval)
			ܦܢܐ retval;
	}
	ܦܢܐ 0;
}

بنية compat_struct {
	فراغ (*invalid)(ext2_filsys, blk_t);
};

ساكن فراغ call_compat_invalid(ext2_filsys fs, blk_t blk,
				حرف *badstr EXT2FS_ATTR((unused)),
				فراغ *priv_data)
{
	بنية compat_struct *st;

	st = (بنية compat_struct *) priv_data;
	ܐܢ (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      فراغ (*invalid)(ext2_filsys fs, blk_t blk))
{
	بنية compat_struct st;

	st.invalid = invalid;

	ܦܢܐ ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


