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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर पूर्णांक db_dir_proc(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		       शून्य *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    पूर्णांक	flags,
				    अक्षर	*block_buf,
				    पूर्णांक (*func)(ext2_ino_t dir,
						पूर्णांक	entry,
						संरचना ext2_dir_entry *dirent,
						पूर्णांक	offset,
						पूर्णांक	blocksize,
						अक्षर	*buf,
						शून्य	*priv_data),
				    शून्य *priv_data)
{
	errcode_t		retval;
	संरचना dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	जर (block_buf)
		ctx.buf = block_buf;
	अन्यथा {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		जर (retval)
			परत retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	जर (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	जर (retval)
		परत retval;
	परत ctx.errcode;
}

स्थिर पूर्णांक db_dir_proc(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		       शून्य *priv_data)
{
	संरचना ext2_inode	inode;
	संरचना dir_context	*ctx;
	पूर्णांक			ret;

	ctx = (संरचना dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	जर (ctx->errcode)
		परत DBLIST_ABORT;
	जर (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	अन्यथा
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	जर ((ret & BLOCK_ABORT) && !ctx->errcode)
		परत DBLIST_ABORT;
	परत 0;
}
