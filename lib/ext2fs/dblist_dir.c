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

#include "config.h"
#include <stdio.h>
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

static ціле db_dir_proc(ext2_filsys fs, struct ext2_db_entry2 *db_info,
		       void *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    ціле	flags,
				    char	*block_buf,
				    ціле (*func)(ext2_ino_t dir,
						ціле	entry,
						struct ext2_dir_entry *dirent,
						ціле	offset,
						ціле	blocksize,
						char	*buf,
						void	*priv_data),
				    void *priv_data)
{
	errcode_t		retval;
	struct dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	якщо (block_buf)
		ctx.buf = block_buf;
	інакше {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		якщо (retval)
			повернути retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	якщо (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	якщо (retval)
		повернути retval;
	повернути ctx.errcode;
}

static ціле db_dir_proc(ext2_filsys fs, struct ext2_db_entry2 *db_info,
		       void *priv_data)
{
	struct ext2_inode	inode;
	struct dir_context	*ctx;
	ціле			ret;

	ctx = (struct dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	якщо (ctx->errcode)
		повернути DBLIST_ABORT;
	якщо (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	інакше
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	якщо ((ret & BLOCK_ABORT) && !ctx->errcode)
		повернути DBLIST_ABORT;
	повернути 0;
}
