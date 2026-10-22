/*
 * dblist_dir.c --- iterate by directory entry
 *
 * Copyright 1997 by Theodore Ts'o
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर पूर्णाङ्क db_dir_proc(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		       शून्य *priv_दत्त);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    पूर्णाङ्क	flags,
				    वर्ण	*खण्डम्_buf,
				    पूर्णाङ्क (*func)(ext2_ino_t dir,
						पूर्णाङ्क	प्रविष्टिः,
						संरचना ext2_dir_प्रविष्टिः *dirent,
						पूर्णाङ्क	विस्थापनम्,
						पूर्णाङ्क	blocksize,
						वर्ण	*buf,
						शून्य	*priv_दत्त),
				    शून्य *priv_दत्त)
{
	errcode_t		retval;
	संरचना dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	यदि (खण्डम्_buf)
		ctx.buf = खण्डम्_buf;
	अन्यथा {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		यदि (retval)
			प्रत्यागम retval;
	}
	ctx.func = func;
	ctx.priv_दत्त = priv_दत्त;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	यदि (!खण्डम्_buf)
		ext2fs_free_mem(&ctx.buf);
	यदि (retval)
		प्रत्यागम retval;
	प्रत्यागम ctx.errcode;
}

स्थिर पूर्णाङ्क db_dir_proc(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		       शून्य *priv_दत्त)
{
	संरचना ext2_inode	inode;
	संरचना dir_context	*ctx;
	पूर्णाङ्क			ret;

	ctx = (संरचना dir_context *) priv_दत्त;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	यदि (ctx->errcode)
		प्रत्यागम DBLIST_ABORT;
	यदि (inode.i_flags & EXT4_INLINE_दत्त_FL)
		ret = ext2fs_inline_दत्त_dir_iterate(fs, ctx->dir, ctx);
	अन्यथा
		ret = ext2fs_process_dir_खण्डम्(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_दत्त);
	यदि ((ret & खण्डम्_ABORT) && !ctx->errcode)
		प्रत्यागम DBLIST_ABORT;
	प्रत्यागम 0;
}
