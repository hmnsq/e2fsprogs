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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, खाता *f,
			       ext2_badblocks_list *bb_list,
			       व्योम *priv_तथ्य,
			       व्योम (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       अक्षर *badstr,
					       व्योम *priv_तथ्य))
{
	errcode_t	retval;
	अचिन्हित दीर्घ दीर्घ blockno;
	पूर्णांक		गिनती;
	अक्षर		buf[128];

	अगर (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		अगर (retval)
			वापस retval;
	}

	जबतक (!ख_पूर्ण (f)) {
		अगर (ख_माला_लो(buf, माप(buf), f) == शून्य)
			अवरोध;
		गिनती = माला_पूछो(buf, "%llu", &blockno);
		अगर (गिनती <= 0)
			जारी;
		/* Badblocks isn't going to be updated for 64bit */
		अगर (blockno >> 32)
			वापस EOVERFLOW;
		अगर (fs &&
		    ((blockno < fs->super->s_first_तथ्य_block) ||
		     (blockno >= ext2fs_blocks_गिनती(fs->super)))) {
			अगर (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_तथ्य);
			जारी;
		}
		retval = ext2fs_badblocks_list_add(*bb_list, (blk64_t) blockno);
		अगर (retval)
			वापस retval;
	}
	वापस 0;
}

काष्ठा compat_struct {
	व्योम (*invalid)(ext2_filsys, blk_t);
};

अटल व्योम call_compat_invalid(ext2_filsys fs, blk_t blk,
				अक्षर *badstr EXT2FS_ATTR((unused)),
				व्योम *priv_तथ्य)
{
	काष्ठा compat_struct *st;

	st = (काष्ठा compat_struct *) priv_तथ्य;
	अगर (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, खाता *f,
			      ext2_badblocks_list *bb_list,
			      व्योम (*invalid)(ext2_filsys fs, blk_t blk))
{
	काष्ठा compat_struct st;

	st.invalid = invalid;

	वापस ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


