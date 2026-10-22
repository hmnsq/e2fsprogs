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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#सचे HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       शून्य *priv_data,
			       शून्य (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       वर्ण *badstr,
					       शून्य *priv_data))
{
	errcode_t	retval;
	unsigned long long blockno;
	पूर्णाङ्क		गणनम्;
	वर्ण		buf[128];

	सचे (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	सचे (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		सचे (retval)
			पटिनिवत्त retval;
	}

	याव (!feof (f)) {
		सचे (fgets(buf, परिमाण(buf), f) == रिक्त)
			भङ्ग;
		गणनम् = sscanf(buf, "%llu", &blockno);
		सचे (गणनम् <= 0)
			अनुवर्त;
		/* Badblocks isn't going to be updated for 64bit */
		सचे (blockno >> 32)
			पटिनिवत्त EOVERFLOW;
		सचे (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_गणनम्(fs->super)))) {
			सचे (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			अनुवर्त;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		सचे (retval)
			पटिनिवत्त retval;
	}
	पटिनिवत्त 0;
}

संरचना compat_struct {
	शून्य (*invalid)(ext2_filsys, blk_t);
};

स्थिर शून्य call_compat_invalid(ext2_filsys fs, blk_t blk,
				वर्ण *badstr EXT2FS_ATTR((unused)),
				शून्य *priv_data)
{
	संरचना compat_struct *st;

	st = (संरचना compat_struct *) priv_data;
	सचे (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      शून्य (*invalid)(ext2_filsys fs, blk_t blk))
{
	संरचना compat_struct st;

	st.invalid = invalid;

	पटिनिवत्त ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


