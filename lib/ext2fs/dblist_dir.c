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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <time.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

अटल पूर्णांक db_dir_proc(ext2_filsys fs, काष्ठा ext2_db_entry2 *db_info,
		       व्योम *priv_तथ्य);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    पूर्णांक	flags,
				    अक्षर	*block_buf,
				    पूर्णांक (*func)(ext2_ino_t dir,
						पूर्णांक	entry,
						काष्ठा ext2_dir_entry *dirent,
						पूर्णांक	offset,
						पूर्णांक	blocksize,
						अक्षर	*buf,
						व्योम	*priv_तथ्य),
				    व्योम *priv_तथ्य)
{
	errcode_t		retval;
	काष्ठा dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	अगर (block_buf)
		ctx.buf = block_buf;
	अन्यथा {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		अगर (retval)
			वापस retval;
	}
	ctx.func = func;
	ctx.priv_तथ्य = priv_तथ्य;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	अगर (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	अगर (retval)
		वापस retval;
	वापस ctx.errcode;
}

अटल पूर्णांक db_dir_proc(ext2_filsys fs, काष्ठा ext2_db_entry2 *db_info,
		       व्योम *priv_तथ्य)
{
	काष्ठा ext2_inode	inode;
	काष्ठा dir_context	*ctx;
	पूर्णांक			ret;

	ctx = (काष्ठा dir_context *) priv_तथ्य;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	अगर (ctx->errcode)
		वापस DBLIST_ABORT;
	अगर (inode.i_flags & EXT4_INLINE_तथ्य_FL)
		ret = ext2fs_inline_तथ्य_dir_iterate(fs, ctx->dir, ctx);
	अन्यथा
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_तथ्य);
	अगर ((ret & BLOCK_ABORT) && !ctx->errcode)
		वापस DBLIST_ABORT;
	वापस 0;
}
