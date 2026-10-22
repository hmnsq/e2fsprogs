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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ db_dir_proc(ext2_filsys fs, ഘടന ext2_db_entry2 *db_info,
		       ശൂന്യം *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    പൂർണ്ണസംഖ്യ	flags,
				    അക്ഷരം	*block_buf,
				    പൂർണ്ണസംഖ്യ (*func)(ext2_ino_t dir,
						പൂർണ്ണസംഖ്യ	entry,
						ഘടന ext2_dir_entry *dirent,
						പൂർണ്ണസംഖ്യ	offset,
						പൂർണ്ണസംഖ്യ	blocksize,
						അക്ഷരം	*buf,
						ശൂന്യം	*priv_data),
				    ശൂന്യം *priv_data)
{
	errcode_t		retval;
	ഘടന dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	എങ്കിൽ (block_buf)
		ctx.buf = block_buf;
	അല്ലെങ്കിൽ {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	എങ്കിൽ (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	എങ്കിൽ (retval)
		തിരികെ retval;
	തിരികെ ctx.errcode;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ db_dir_proc(ext2_filsys fs, ഘടന ext2_db_entry2 *db_info,
		       ശൂന്യം *priv_data)
{
	ഘടന ext2_inode	inode;
	ഘടന dir_context	*ctx;
	പൂർണ്ണസംഖ്യ			ret;

	ctx = (ഘടന dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	എങ്കിൽ (ctx->errcode)
		തിരികെ DBLIST_ABORT;
	എങ്കിൽ (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	അല്ലെങ്കിൽ
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	എങ്കിൽ ((ret & BLOCK_ABORT) && !ctx->errcode)
		തിരികെ DBLIST_ABORT;
	തിരികെ 0;
}
