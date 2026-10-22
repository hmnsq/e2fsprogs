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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"

স্থির পূর্ণসংখ্যা db_dir_proc(ext2_filsys fs, কাঠামো ext2_db_entry2 *db_info,
		       শূন্য *priv_তথ্য);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    পূর্ণসংখ্যা	flags,
				    অক্ষর	*block_buf,
				    পূর্ণসংখ্যা (*func)(ext2_ino_t dir,
						পূর্ণসংখ্যা	entry,
						কাঠামো ext2_dir_entry *dirent,
						পূর্ণসংখ্যা	offset,
						পূর্ণসংখ্যা	blocksize,
						অক্ষর	*buf,
						শূন্য	*priv_তথ্য),
				    শূন্য *priv_তথ্য)
{
	errcode_t		retval;
	কাঠামো dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	যদি (block_buf)
		ctx.buf = block_buf;
	অন্যথা {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		যদি (retval)
			ফেরত retval;
	}
	ctx.func = func;
	ctx.priv_তথ্য = priv_তথ্য;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	যদি (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	যদি (retval)
		ফেরত retval;
	ফেরত ctx.errcode;
}

স্থির পূর্ণসংখ্যা db_dir_proc(ext2_filsys fs, কাঠামো ext2_db_entry2 *db_info,
		       শূন্য *priv_তথ্য)
{
	কাঠামো ext2_inode	inode;
	কাঠামো dir_context	*ctx;
	পূর্ণসংখ্যা			ret;

	ctx = (কাঠামো dir_context *) priv_তথ্য;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	যদি (ctx->errcode)
		ফেরত DBLIST_ABORT;
	যদি (inode.i_flags & EXT4_INLINE_তথ্য_FL)
		ret = ext2fs_inline_তথ্য_dir_iterate(fs, ctx->dir, ctx);
	অন্যথা
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_তথ্য);
	যদি ((ret & BLOCK_ABORT) && !ctx->errcode)
		ফেরত DBLIST_ABORT;
	ফেরত 0;
}
