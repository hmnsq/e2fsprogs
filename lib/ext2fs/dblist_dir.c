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
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

static ကိန်းပြည့် db_dir_proc(ext2_filsys fs, sa_nracanaa ext2_db_entry2 *db_info,
		       void *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    ကိန်းပြည့်	flags,
				    var_na	*block_buf,
				    ကိန်းပြည့် (*func)(ext2_ino_t dir,
						ကိန်းပြည့်	entry,
						sa_nracanaa ext2_dir_entry *dirent,
						ကိန်းပြည့်	offset,
						ကိန်းပြည့်	blocksize,
						var_na	*buf,
						void	*priv_data),
				    void *priv_data)
{
	errcode_t		retval;
	sa_nracanaa dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	အကယ်၍ (block_buf)
		ctx.buf = block_buf;
	မဟုတ်ပါက {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		အကယ်၍ (retval)
			ပြန်ပေး retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	အကယ်၍ (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	အကယ်၍ (retval)
		ပြန်ပေး retval;
	ပြန်ပေး ctx.errcode;
}

static ကိန်းပြည့် db_dir_proc(ext2_filsys fs, sa_nracanaa ext2_db_entry2 *db_info,
		       void *priv_data)
{
	sa_nracanaa ext2_inode	inode;
	sa_nracanaa dir_context	*ctx;
	ကိန်းပြည့်			ret;

	ctx = (sa_nracanaa dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	အကယ်၍ (ctx->errcode)
		ပြန်ပေး DBLIST_ABORT;
	အကယ်၍ (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	မဟုတ်ပါက
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	အကယ်၍ ((ret & BLOCK_ABORT) && !ctx->errcode)
		ပြန်ပေး DBLIST_ABORT;
	ပြန်ပေး 0;
}
