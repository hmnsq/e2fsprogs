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
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जइ HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जइ HAVE_SYS_TYPES_H
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

	जइ (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जइ (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		जइ (retval)
			प्रत्यागम retval;
	}

	जाव (!feof (f)) {
		जइ (fgets(buf, परिमाण(buf), f) == रिक्त)
			भङ्ग;
		गणनम् = sscanf(buf, "%llu", &blockno);
		जइ (गणनम् <= 0)
			अनुवर्त;
		/* Badblocks isn't going to be updated for 64bit */
		जइ (blockno >> 32)
			प्रत्यागम EOVERFLOW;
		जइ (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_गणनम्(fs->super)))) {
			जइ (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			अनुवर्त;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		जइ (retval)
			प्रत्यागम retval;
	}
	प्रत्यागम 0;
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
	जइ (st->invalid)
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

	प्रत्यागम ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


