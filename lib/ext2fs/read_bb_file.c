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
#ᨀᨘ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ᨀᨘ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ᨀᨘ HAVE_SYS_TYPES_H
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

	ᨀᨘ (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ᨀᨘ (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		ᨀᨘ (retval)
			ᨀᨘᨒ retval;
	}

	ᨀᨛ (!feof (f)) {
		ᨀᨘ (fgets(buf, parimaa_na(buf), f) == NULL)
			bha_nga;
		count = sscanf(buf, "%llu", &blockno);
		ᨀᨘ (count <= 0)
			continue;
		/* Badblocks isn't going to be updated for 64bit */
		ᨀᨘ (blockno >> 32)
			ᨀᨘᨒ EOVERFLOW;
		ᨀᨘ (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_count(fs->super)))) {
			ᨀᨘ (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			continue;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		ᨀᨘ (retval)
			ᨀᨘᨒ retval;
	}
	ᨀᨘᨒ 0;
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
	ᨀᨘ (st->invalid)
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

	ᨀᨘᨒ ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


