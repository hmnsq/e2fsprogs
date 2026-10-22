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

#include "config.h"
#include <stdio.h>
#include <string.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       void *priv_data,
			       void (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       char *badstr,
					       void *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	số_nguyên		count;
	char		buf[128];

	nếu (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	nếu (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		nếu (retval)
			trả_về retval;
	}

	trong_khi (!feof (f)) {
		nếu (fgets(buf, sizeof(buf), f) == NULL)
			break;
		count = sscanf(buf, "%llu", &blockno);
		nếu (count <= 0)
			continue;
		/* Badblocks isn't going to be updated for 64bit */
		nếu (blockno >> 32)
			trả_về EOVERFLOW;
		nếu (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_count(fs->super)))) {
			nếu (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			continue;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		nếu (retval)
			trả_về retval;
	}
	trả_về 0;
}

struct compat_struct {
	void (*invalid)(ext2_filsys, blk_t);
};

static void call_compat_invalid(ext2_filsys fs, blk_t blk,
				char *badstr EXT2FS_ATTR((unused)),
				void *priv_data)
{
	struct compat_struct *st;

	st = (struct compat_struct *) priv_data;
	nếu (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      void (*invalid)(ext2_filsys fs, blk_t blk))
{
	struct compat_struct st;

	st.invalid = invalid;

	trả_về ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


