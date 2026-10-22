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
#如果 HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

static 整数 db_dir_proc(ext2_filsys fs, struct ext2_db_entry2 *db_info,
		       void *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    整数	flags,
				    char	*block_buf,
				    整数 (*func)(ext2_ino_t dir,
						整数	entry,
						struct ext2_dir_entry *dirent,
						整数	offset,
						整数	blocksize,
						char	*buf,
						void	*priv_data),
				    void *priv_data)
{
	errcode_t		retval;
	struct dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	如果 (block_buf)
		ctx.buf = block_buf;
	否则 {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		如果 (retval)
			返回 retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	如果 (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	如果 (retval)
		返回 retval;
	返回 ctx.errcode;
}

static 整数 db_dir_proc(ext2_filsys fs, struct ext2_db_entry2 *db_info,
		       void *priv_data)
{
	struct ext2_inode	inode;
	struct dir_context	*ctx;
	整数			ret;

	ctx = (struct dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	如果 (ctx->errcode)
		返回 DBLIST_ABORT;
	如果 (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	否则
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	如果 ((ret & BLOCK_ABORT) && !ctx->errcode)
		返回 DBLIST_ABORT;
	返回 0;
}
