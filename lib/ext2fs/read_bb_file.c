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
#ပေႃး HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ပေႃး HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ပေႃး HAVE_SYS_TYPES_H
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
					       var_na *badstr,
					       void *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	puur_naa_nka		count;
	var_na		buf[128];

	ပေႃး (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ပေႃး (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		ပေႃး (retval)
			ပွၵ်ႈ retval;
	}

	ၶၢဝ်း (!feof (f)) {
		ပေႃး (fgets(buf, parimaa_na(buf), f) == NULL)
			bha_nga;
		count = sscanf(buf, "%llu", &blockno);
		ပေႃး (count <= 0)
			continue;
		/* Badblocks isn't going to be updated for 64bit */
		ပေႃး (blockno >> 32)
			ပွၵ်ႈ EOVERFLOW;
		ပေႃး (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_count(fs->super)))) {
			ပေႃး (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			continue;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		ပေႃး (retval)
			ပွၵ်ႈ retval;
	}
	ပွၵ်ႈ 0;
}

sa_nracanaa compat_struct {
	void (*invalid)(ext2_filsys, blk_t);
};

static void call_compat_invalid(ext2_filsys fs, blk_t blk,
				var_na *badstr EXT2FS_ATTR((unused)),
				void *priv_data)
{
	sa_nracanaa compat_struct *st;

	st = (sa_nracanaa compat_struct *) priv_data;
	ပေႃး (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      void (*invalid)(ext2_filsys fs, blk_t blk))
{
	sa_nracanaa compat_struct st;

	st.invalid = invalid;

	ပွၵ်ႈ ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


