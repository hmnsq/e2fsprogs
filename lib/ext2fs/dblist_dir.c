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
#𐤀𐤌 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

ساكن صحيح db_dir_proc(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		       فراغ *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    صحيح	flags,
				    حرف	*block_buf,
				    صحيح (*func)(ext2_ino_t dir,
						صحيح	entry,
						بنية ext2_dir_entry *dirent,
						صحيح	offset,
						صحيح	blocksize,
						حرف	*buf,
						فراغ	*priv_data),
				    فراغ *priv_data)
{
	errcode_t		retval;
	بنية dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	𐤀𐤌 (block_buf)
		ctx.buf = block_buf;
	وإلا {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		𐤀𐤌 (retval)
			أرجع retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	𐤀𐤌 (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	𐤀𐤌 (retval)
		أرجع retval;
	أرجع ctx.errcode;
}

ساكن صحيح db_dir_proc(ext2_filsys fs, بنية ext2_db_entry2 *db_info,
		       فراغ *priv_data)
{
	بنية ext2_inode	inode;
	بنية dir_context	*ctx;
	صحيح			ret;

	ctx = (بنية dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	𐤀𐤌 (ctx->errcode)
		أرجع DBLIST_ABORT;
	𐤀𐤌 (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	وإلا
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	𐤀𐤌 ((ret & BLOCK_ABORT) && !ctx->errcode)
		أرجع DBLIST_ABORT;
	أرجع 0;
}
