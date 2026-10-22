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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <time.h>

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

স্থির পূর্ণাঙ্ক db_dir_proc(ext2_filsys fs, সংরচনা ext2_db_entry2 *db_info,
		       শূন্য *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    পূর্ণাঙ্ক	flags,
				    ৱর্ণ	*block_buf,
				    পূর্ণাঙ্ক (*func)(ext2_ino_t dir,
						পূর্ণাঙ্ক	entry,
						সংরচনা ext2_dir_entry *dirent,
						পূর্ণাঙ্ক	offset,
						পূর্ণাঙ্ক	blocksize,
						ৱর্ণ	*buf,
						শূন্য	*priv_data),
				    শূন্য *priv_data)
{
	errcode_t		retval;
	সংরচনা dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	যদি (block_buf)
		ctx.buf = block_buf;
	অন্যথা {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		যদি (retval)
			ঘূৰাই retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	যদি (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	যদি (retval)
		ঘূৰাই retval;
	ঘূৰাই ctx.errcode;
}

স্থির পূর্ণাঙ্ক db_dir_proc(ext2_filsys fs, সংরচনা ext2_db_entry2 *db_info,
		       শূন্য *priv_data)
{
	সংরচনা ext2_inode	inode;
	সংরচনা dir_context	*ctx;
	পূর্ণাঙ্ক			ret;

	ctx = (সংরচনা dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	যদি (ctx->errcode)
		ঘূৰাই DBLIST_ABORT;
	যদি (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	অন্যথা
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	যদি ((ret & BLOCK_ABORT) && !ctx->errcode)
		ঘূৰাই DBLIST_ABORT;
	ঘূৰাই 0;
}
