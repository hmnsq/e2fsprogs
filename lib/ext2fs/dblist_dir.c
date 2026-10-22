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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

நிலையான முழு db_dir_proc(ext2_filsys fs, கட்டமைப்பு ext2_db_entry2 *db_info,
		       வெறுமை *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    முழு	flags,
				    எழுத்து	*block_buf,
				    முழு (*func)(ext2_ino_t dir,
						முழு	entry,
						கட்டமைப்பு ext2_dir_entry *dirent,
						முழு	offset,
						முழு	blocksize,
						எழுத்து	*buf,
						வெறுமை	*priv_data),
				    வெறுமை *priv_data)
{
	errcode_t		retval;
	கட்டமைப்பு dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	எனில் (block_buf)
		ctx.buf = block_buf;
	இல்லையெனில் {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		எனில் (retval)
			திரும்பு retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	எனில் (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	எனில் (retval)
		திரும்பு retval;
	திரும்பு ctx.errcode;
}

நிலையான முழு db_dir_proc(ext2_filsys fs, கட்டமைப்பு ext2_db_entry2 *db_info,
		       வெறுமை *priv_data)
{
	கட்டமைப்பு ext2_inode	inode;
	கட்டமைப்பு dir_context	*ctx;
	முழு			ret;

	ctx = (கட்டமைப்பு dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	எனில் (ctx->errcode)
		திரும்பு DBLIST_ABORT;
	எனில் (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	இல்லையெனில்
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	எனில் ((ret & BLOCK_ABORT) && !ctx->errcode)
		திரும்பு DBLIST_ABORT;
	திரும்பு 0;
}
