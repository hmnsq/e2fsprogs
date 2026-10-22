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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

ساكن صحيح db_dir_proc(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		       فراغ *priv_داده);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    صحيح	flags,
				    حرف	*بلوک_buf,
				    صحيح (*func)(ext2_ino_t dir,
						صحيح	مدخل,
						بنية ext2_dir_مدخل *dirent,
						صحيح	جابه_جایی,
						صحيح	blocksize,
						حرف	*buf,
						فراغ	*priv_داده),
				    فراغ *priv_داده)
{
	errcode_t		retval;
	بنية dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	إذا (بلوک_buf)
		ctx.buf = بلوک_buf;
	وإلا {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;
	}
	ctx.func = func;
	ctx.priv_داده = priv_داده;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	إذا (!بلوک_buf)
		ext2fs_free_mem(&ctx.buf);
	إذا (retval)
		أرجع retval;
	أرجع ctx.errcode;
}

ساكن صحيح db_dir_proc(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		       فراغ *priv_داده)
{
	بنية ext2_inode	inode;
	بنية dir_context	*ctx;
	صحيح			ret;

	ctx = (بنية dir_context *) priv_داده;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	إذا (ctx->errcode)
		أرجع DBLIST_ABORT;
	إذا (inode.i_flags & EXT4_INLINE_داده_FL)
		ret = ext2fs_inline_داده_dir_iterate(fs, ctx->dir, ctx);
	وإلا
		ret = ext2fs_process_dir_بلوک(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_داده);
	إذا ((ret & بلوک_ABORT) && !ctx->errcode)
		أرجع DBLIST_ABORT;
	أرجع 0;
}
