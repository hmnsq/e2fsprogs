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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, खाता *f,
			       ext2_badblocks_list *bb_list,
			       शून्य *priv_data,
			       शून्य (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       वर्ण *badstr,
					       शून्य *priv_data))
{
	errcode_t	retval;
	अचिन्हित दीर्घ दीर्घ blockno;
	पूर्णाङ्क		गन्ती;
	वर्ण		buf[128];

	यदि (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		यदि (retval)
			फर्काऊ retval;
	}

	जबसम्म (!ख_पूर्ण (f)) {
		यदि (ख_माला_लो(buf, परिमाण(buf), f) == रिक्त)
			रोक;
		गन्ती = माला_पूछो(buf, "%llu", &blockno);
		यदि (गन्ती <= 0)
			जारी;
		/* Badblocks isn't going to be updated for 64bit */
		यदि (blockno >> 32)
			फर्काऊ EOVERFLOW;
		यदि (fs &&
		    ((blockno < fs->super->s_first_data_block) ||
		     (blockno >= ext2fs_blocks_गन्ती(fs->super)))) {
			यदि (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_data);
			जारी;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		यदि (retval)
			फर्काऊ retval;
	}
	फर्काऊ 0;
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
	यदि (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, खाता *f,
			      ext2_badblocks_list *bb_list,
			      शून्य (*invalid)(ext2_filsys fs, blk_t blk))
{
	संरचना compat_struct st;

	st.invalid = invalid;

	फर्काऊ ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


