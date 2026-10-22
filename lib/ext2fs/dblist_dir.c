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
#اگر_ختم
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

ساکن صحیح db_dir_proc(ext2_filsys fs, ساخت ext2_db_entry2 *db_info,
		       خالی *priv_ڈیٹا);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    صحیح	flags,
				    حرف	*بلاک_buf,
				    صحیح (*func)(ext2_ino_t dir,
						صحیح	اندراج_2,
						ساخت ext2_dir_اندراج *dirent,
						صحیح	انحراف,
						صحیح	blocksize,
						حرف	*buf,
						خالی	*priv_ڈیٹا),
				    خالی *priv_ڈیٹا)
{
	errcode_t		retval;
	ساخت dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	اگر (بلاک_buf)
		ctx.buf = بلاک_buf;
	ورنہ {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		اگر (retval)
			واپس retval;
	}
	ctx.func = func;
	ctx.priv_ڈیٹا = priv_ڈیٹا;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	اگر (!بلاک_buf)
		ext2fs_free_mem(&ctx.buf);
	اگر (retval)
		واپس retval;
	واپس ctx.errcode;
}

ساکن صحیح db_dir_proc(ext2_filsys fs, ساخت ext2_db_entry2 *db_info,
		       خالی *priv_ڈیٹا)
{
	ساخت ext2_inode	inode;
	ساخت dir_context	*ctx;
	صحیح			ret;

	ctx = (ساخت dir_context *) priv_ڈیٹا;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	اگر (ctx->errcode)
		واپس DBLIST_ABORT;
	اگر (inode.i_flags & EXT4_INLINE_ڈیٹا_FL)
		ret = ext2fs_inline_ڈیٹا_dir_iterate(fs, ctx->dir, ctx);
	ورنہ
		ret = ext2fs_process_dir_بلاک(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_ڈیٹا);
	اگر ((ret & بلاک_ABORT) && !ctx->errcode)
		واپس DBLIST_ABORT;
	واپس 0;
}
