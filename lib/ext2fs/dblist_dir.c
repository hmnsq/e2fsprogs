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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

స్థిర పూర్ణాంకం db_dir_proc(ext2_filsys fs, నిర్మాణం ext2_db_entry2 *db_info,
		       శూన్య *priv_దత్తాంశం);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    పూర్ణాంకం	flags,
				    అక్షరం	*ఖండం_buf,
				    పూర్ణాంకం (*func)(ext2_ino_t dir,
						పూర్ణాంకం	నమోదు_2,
						నిర్మాణం ext2_dir_నమోదు *dirent,
						పూర్ణాంకం	స్థానభ్రంశం,
						పూర్ణాంకం	blocksize,
						అక్షరం	*buf,
						శూన్య	*priv_దత్తాంశం),
				    శూన్య *priv_దత్తాంశం)
{
	errcode_t		retval;
	నిర్మాణం dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	ఒకవేళ (ఖండం_buf)
		ctx.buf = ఖండం_buf;
	లేకపోతే {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	ctx.func = func;
	ctx.priv_దత్తాంశం = priv_దత్తాంశం;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	ఒకవేళ (!ఖండం_buf)
		ext2fs_free_mem(&ctx.buf);
	ఒకవేళ (retval)
		తిరిగి retval;
	తిరిగి ctx.errcode;
}

స్థిర పూర్ణాంకం db_dir_proc(ext2_filsys fs, నిర్మాణం ext2_db_entry2 *db_info,
		       శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం ext2_inode	inode;
	నిర్మాణం dir_context	*ctx;
	పూర్ణాంకం			ret;

	ctx = (నిర్మాణం dir_context *) priv_దత్తాంశం;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	ఒకవేళ (ctx->errcode)
		తిరిగి DBLIST_ABORT;
	ఒకవేళ (inode.i_flags & EXT4_INLINE_దత్తాంశం_FL)
		ret = ext2fs_inline_దత్తాంశం_dir_iterate(fs, ctx->dir, ctx);
	లేకపోతే
		ret = ext2fs_process_dir_ఖండం(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_దత్తాంశం);
	ఒకవేళ ((ret & ఖండం_ABORT) && !ctx->errcode)
		తిరిగి DBLIST_ABORT;
	తిరిగి 0;
}
