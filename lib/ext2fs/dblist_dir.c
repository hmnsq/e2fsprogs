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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ db_dir_proc(ext2_filsys fs, ಸಂರಚನಾ ext2_db_entry2 *db_info,
		       ಶೂನ್ಯ *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    ಪೂರ್ಣಾಙ್ಕ	flags,
				    ವರ್ಣ	*block_buf,
				    ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_ino_t dir,
						ಪೂರ್ಣಾಙ್ಕ	entry,
						ಸಂರಚನಾ ext2_dir_entry *dirent,
						ಪೂರ್ಣಾಙ್ಕ	offset,
						ಪೂರ್ಣಾಙ್ಕ	blocksize,
						ವರ್ಣ	*buf,
						ಶೂನ್ಯ	*priv_data),
				    ಶೂನ್ಯ *priv_data)
{
	errcode_t		retval;
	ಸಂರಚನಾ dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	ಯದಿ (block_buf)
		ctx.buf = block_buf;
	ಅನ್ಯಥಾ {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	ಯದಿ (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಪ್ರತ್ಯಾಗಮ ctx.errcode;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ db_dir_proc(ext2_filsys fs, ಸಂರಚನಾ ext2_db_entry2 *db_info,
		       ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ ext2_inode	inode;
	ಸಂರಚನಾ dir_context	*ctx;
	ಪೂರ್ಣಾಙ್ಕ			ret;

	ctx = (ಸಂರಚನಾ dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	ಯದಿ (ctx->errcode)
		ಪ್ರತ್ಯಾಗಮ DBLIST_ABORT;
	ಯದಿ (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	ಅನ್ಯಥಾ
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	ಯದಿ ((ret & BLOCK_ABORT) && !ctx->errcode)
		ಪ್ರತ್ಯಾಗಮ DBLIST_ABORT;
	ಪ್ರತ್ಯಾಗಮ 0;
}
