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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       ശൂന്യം *priv_data,
			       ശൂന്യം (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       അക്ഷരം *badstr,
					       ശൂന്യം *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	പൂർണ്ണസംഖ്യ		എണ്ണം;
	അക്ഷരം		buf[128];

	എങ്കിൽ (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}

	ആകുമ്പോൾ (!feof (f)) {
		എങ്കിൽ (fgets(buf, വലുപ്പം(buf), f) == ശൂന്യ)
			നിർത്തുക;
		എണ്ണം = sscanf(buf, "%llu", &blockno);
		എങ്കിൽ (എണ്ണം <= 0)
			തുടരുക;
		/* Badblocks isn't going to be updated for 64bit */
		എങ്കിൽ (blockno >> 32)
			തിരികെ EOVERFLOW;
		എങ്കിൽ (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_എണ്ണം(fs->super)))) {
			എങ്കിൽ (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			തുടരുക;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	തിരികെ 0;
}

ഘടന compat_struct {
	ശൂന്യം (*invalid)(ext2_filsys, blk_t);
};

സ്ഥിതം ശൂന്യം call_compat_invalid(ext2_filsys fs, blk_t blk,
				അക്ഷരം *badstr EXT2FS_ATTR((unused)),
				ശൂന്യം *priv_data)
{
	ഘടന compat_struct *st;

	st = (ഘടന compat_struct *) priv_data;
	എങ്കിൽ (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      ശൂന്യം (*invalid)(ext2_filsys fs, blk_t blk))
{
	ഘടന compat_struct st;

	st.invalid = invalid;

	തിരികെ ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


