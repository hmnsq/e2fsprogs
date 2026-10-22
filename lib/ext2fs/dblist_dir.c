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
#جيڪڏهن_پورو
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

ساكن صحيح db_dir_proc(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		       فراغ *priv_معلومات);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    صحيح	flags,
				    حرف	*بلاڪ_buf,
				    صحيح (*func)(ext2_ino_t dir,
						صحيح	داخلا,
						بنية ext2_dir_داخلا *dirent,
						صحيح	وٿي,
						صحيح	blocksize,
						حرف	*buf,
						فراغ	*priv_معلومات),
				    فراغ *priv_معلومات)
{
	errcode_t		retval;
	بنية dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	إذا (بلاڪ_buf)
		ctx.buf = بلاڪ_buf;
	وإلا {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;
	}
	ctx.func = func;
	ctx.priv_معلومات = priv_معلومات;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	إذا (!بلاڪ_buf)
		ext2fs_free_mem(&ctx.buf);
	إذا (retval)
		أرجع retval;
	أرجع ctx.errcode;
}

ساكن صحيح db_dir_proc(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		       فراغ *priv_معلومات)
{
	بنية ext2_inode	inode;
	بنية dir_context	*ctx;
	صحيح			ret;

	ctx = (بنية dir_context *) priv_معلومات;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	إذا (ctx->errcode)
		أرجع DBLIST_ABORT;
	إذا (inode.i_flags & EXT4_INLINE_معلومات_FL)
		ret = ext2fs_inline_معلومات_dir_iterate(fs, ctx->dir, ctx);
	وإلا
		ret = ext2fs_process_dir_بلاڪ(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_معلومات);
	إذا ((ret & بلاڪ_ABORT) && !ctx->errcode)
		أرجع DBLIST_ABORT;
	أرجع 0;
}
