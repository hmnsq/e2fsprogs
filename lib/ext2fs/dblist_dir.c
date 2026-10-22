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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

ایستا صحیح db_dir_proc(ext2_filsys fs, ساختار ext2_db_entry2 *db_info,
		       تهی *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    صحیح	flags,
				    نویسه	*block_buf,
				    صحیح (*func)(ext2_ino_t dir,
						صحیح	entry,
						ساختار ext2_dir_entry *dirent,
						صحیح	offset,
						صحیح	blocksize,
						نویسه	*buf,
						تهی	*priv_data),
				    تهی *priv_data)
{
	errcode_t		retval;
	ساختار dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	اگر (block_buf)
		ctx.buf = block_buf;
	وگرنه {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		اگر (retval)
			بازگشت retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	اگر (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	اگر (retval)
		بازگشت retval;
	بازگشت ctx.errcode;
}

ایستا صحیح db_dir_proc(ext2_filsys fs, ساختار ext2_db_entry2 *db_info,
		       تهی *priv_data)
{
	ساختار ext2_inode	inode;
	ساختار dir_context	*ctx;
	صحیح			ret;

	ctx = (ساختار dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	اگر (ctx->errcode)
		بازگشت DBLIST_ABORT;
	اگر (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	وگرنه
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	اگر ((ret & BLOCK_ABORT) && !ctx->errcode)
		بازگشت DBLIST_ABORT;
	بازگشت 0;
}
