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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା db_dir_proc(ext2_filsys fs, ସଂରଚନା ext2_db_entry2 *db_info,
		       ଶୂନ୍ୟ *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା	flags,
				    ଅକ୍ଷର	*block_buf,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_ino_t dir,
						ପୂର୍ଣ୍ଣସଂଖ୍ୟା	entry,
						ସଂରଚନା ext2_dir_entry *dirent,
						ପୂର୍ଣ୍ଣସଂଖ୍ୟା	offset,
						ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocksize,
						ଅକ୍ଷର	*buf,
						ଶୂନ୍ୟ	*priv_data),
				    ଶୂନ୍ୟ *priv_data)
{
	errcode_t		retval;
	ସଂରଚନା dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	ଯଦି (block_buf)
		ctx.buf = block_buf;
	ଅନ୍ୟଥା {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	ଯଦି (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଫେରନ୍ତୁ ctx.errcode;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା db_dir_proc(ext2_filsys fs, ସଂରଚନା ext2_db_entry2 *db_info,
		       ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା ext2_inode	inode;
	ସଂରଚନା dir_context	*ctx;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			ret;

	ctx = (ସଂରଚନା dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	ଯଦି (ctx->errcode)
		ଫେରନ୍ତୁ DBLIST_ABORT;
	ଯଦି (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	ଅନ୍ୟଥା
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	ଯଦି ((ret & BLOCK_ABORT) && !ctx->errcode)
		ଫେରନ୍ତୁ DBLIST_ABORT;
	ଫେରନ୍ତୁ 0;
}
