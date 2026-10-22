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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਸਥਿਰ ਪੂਰਨ db_dir_proc(ext2_filsys fs, ਬਣਤਰ ext2_db_entry2 *db_info,
		       ਖਾਲੀ *priv_ਅੰਕੜੇ);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    ਪੂਰਨ	flags,
				    ਅੱਖਰ	*ਖੰਡ_buf,
				    ਪੂਰਨ (*func)(ext2_ino_t dir,
						ਪੂਰਨ	ਇੰਦਰਾਜ,
						ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
						ਪੂਰਨ	ਵਿੱਥ,
						ਪੂਰਨ	blocksize,
						ਅੱਖਰ	*buf,
						ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				    ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	errcode_t		retval;
	ਬਣਤਰ dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	ਜੇ (ਖੰਡ_buf)
		ctx.buf = ਖੰਡ_buf;
	ਨਹੀਂਤਾਂ {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ctx.func = func;
	ctx.priv_ਅੰਕੜੇ = priv_ਅੰਕੜੇ;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	ਜੇ (!ਖੰਡ_buf)
		ext2fs_free_mem(&ctx.buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਵਾਪਸ ctx.errcode;
}

ਸਥਿਰ ਪੂਰਨ db_dir_proc(ext2_filsys fs, ਬਣਤਰ ext2_db_entry2 *db_info,
		       ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ ext2_inode	inode;
	ਬਣਤਰ dir_context	*ctx;
	ਪੂਰਨ			ret;

	ctx = (ਬਣਤਰ dir_context *) priv_ਅੰਕੜੇ;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	ਜੇ (ctx->errcode)
		ਵਾਪਸ DBLIST_ABORT;
	ਜੇ (inode.i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL)
		ret = ext2fs_inline_ਅੰਕੜੇ_dir_iterate(fs, ctx->dir, ctx);
	ਨਹੀਂਤਾਂ
		ret = ext2fs_process_dir_ਖੰਡ(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_ਅੰਕੜੇ);
	ਜੇ ((ret & ਖੰਡ_ABORT) && !ctx->errcode)
		ਵਾਪਸ DBLIST_ABORT;
	ਵਾਪਸ 0;
}
