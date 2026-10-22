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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સ્થિર પૂર્ણાંક db_dir_proc(ext2_filsys fs, સંરચના ext2_db_entry2 *db_info,
		       શૂન્ય *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    પૂર્ણાંક	flags,
				    અક્ષર	*block_buf,
				    પૂર્ણાંક (*func)(ext2_ino_t dir,
						પૂર્ણાંક	entry,
						સંરચના ext2_dir_entry *dirent,
						પૂર્ણાંક	offset,
						પૂર્ણાંક	blocksize,
						અક્ષર	*buf,
						શૂન્ય	*priv_data),
				    શૂન્ય *priv_data)
{
	errcode_t		retval;
	સંરચના dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	જો (block_buf)
		ctx.buf = block_buf;
	નહીંતર {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		જો (retval)
			પાછા retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	જો (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	જો (retval)
		પાછા retval;
	પાછા ctx.errcode;
}

સ્થિર પૂર્ણાંક db_dir_proc(ext2_filsys fs, સંરચના ext2_db_entry2 *db_info,
		       શૂન્ય *priv_data)
{
	સંરચના ext2_inode	inode;
	સંરચના dir_context	*ctx;
	પૂર્ણાંક			ret;

	ctx = (સંરચના dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	જો (ctx->errcode)
		પાછા DBLIST_ABORT;
	જો (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	નહીંતર
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	જો ((ret & BLOCK_ABORT) && !ctx->errcode)
		પાછા DBLIST_ABORT;
	પાછા 0;
}
