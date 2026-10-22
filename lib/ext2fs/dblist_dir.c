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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर पूर्णाङ्क db_dir_proc(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		       शून्य *priv_data);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    पूर्णाङ्क	flags,
				    वर्ण	*block_buf,
				    पूर्णाङ्क (*func)(ext2_ino_t dir,
						पूर्णाङ्क	entry,
						संरचना ext2_dir_entry *dirent,
						पूर्णाङ्क	offset,
						पूर्णाङ्क	blocksize,
						वर्ण	*buf,
						शून्य	*priv_data),
				    शून्य *priv_data)
{
	errcode_t		retval;
	संरचना dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	यदि (block_buf)
		ctx.buf = block_buf;
	अन्यथा {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		यदि (retval)
			फर्काऊ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	यदि (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	यदि (retval)
		फर्काऊ retval;
	फर्काऊ ctx.errcode;
}

स्थिर पूर्णाङ्क db_dir_proc(ext2_filsys fs, संरचना ext2_db_entry2 *db_info,
		       शून्य *priv_data)
{
	संरचना ext2_inode	inode;
	संरचना dir_context	*ctx;
	पूर्णाङ्क			ret;

	ctx = (संरचना dir_context *) priv_data;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	यदि (ctx->errcode)
		फर्काऊ DBLIST_ABORT;
	यदि (inode.i_flags & EXT4_INLINE_DATA_FL)
		ret = ext2fs_inline_data_dir_iterate(fs, ctx->dir, ctx);
	अन्यथा
		ret = ext2fs_process_dir_block(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_data);
	यदि ((ret & BLOCK_ABORT) && !ctx->errcode)
		फर्काऊ DBLIST_ABORT;
	फर्काऊ 0;
}
