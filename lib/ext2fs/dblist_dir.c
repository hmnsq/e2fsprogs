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
#གལ་སྲིད HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

static ཧྲིལ db_dir_proc(ext2_filsys fs, sa_nracanaa ext2_db_entry2 *db_info,
		       void *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    ཧྲིལ	flags,
				    var_na	*block_buf,
				    ཧྲིལ (*func)(ext2_ino_t dir,
						ཧྲིལ	entry,
						sa_nracanaa ext2_dir_entry *dirent,
						ཧྲིལ	offset,
						ཧྲིལ	blocksize,
						var_na	*buf,
						void	*priv_data),
				    void *priv_data)
{
	errcode_t		retval;
	sa_nracanaa dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	གལ་སྲིད (block_buf)
		ctx.buf = block_buf;
	དེ་མིན {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		གལ་སྲིད (retval)
			ལོག retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	གལ་སྲིད (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	གལ་སྲིད (retval)
		ལོག retval;
	ལོག ctx.errcode;
}

static ཧྲིལ db_dir_proc(ext2_filsys fs, sa_nracanaa ext2_db_entry2 *db_info,
		       void *priv_data)
{
	sa_nracanaa ext2_inode	inode;
	sa_nracanaa dir_context	*ctx;
	ཧྲིལ			ret;

	ctx = (sa_nracanaa dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	གལ་སྲིད (ctx->errcode)
		ལོག DBLIST_ABORT;
	གལ་སྲིད (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	དེ་མིན
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	གལ་སྲིད ((ret & BLOCK_ABORT) && !ctx->errcode)
		ལོག DBLIST_ABORT;
	ལོག 0;
}
