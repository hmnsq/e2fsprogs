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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ db_dir_proc(ext2_filsys fs, ರಚನೆ ext2_db_entry2 *db_info,
		       ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);

errcode_t ext2fs_dblist_dir_iterate(ext2_dblist dblist,
				    ಪೂರ್ಣಾಂಕ	flags,
				    ಅಕ್ಷರ	*ಖಂಡ_buf,
				    ಪೂರ್ಣಾಂಕ (*func)(ext2_ino_t dir,
						ಪೂರ್ಣಾಂಕ	ನಮೂದು,
						ರಚನೆ ext2_dir_ನಮೂದು *dirent,
						ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ,
						ಪೂರ್ಣಾಂಕ	blocksize,
						ಅಕ್ಷರ	*buf,
						ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				    ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	errcode_t		retval;
	ರಚನೆ dir_context	ctx;

	EXT2_CHECK_MAGIC(dblist, EXT2_ET_MAGIC_DBLIST);

	ctx.dir = 0;
	ctx.flags = flags;
	ಆದರೆ (ಖಂಡ_buf)
		ctx.buf = ಖಂಡ_buf;
	ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_get_mem(dblist->fs->blocksize, &ctx.buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ctx.func = func;
	ctx.priv_ದತ್ತಾಂಶ = priv_ದತ್ತಾಂಶ;
	ctx.errcode = 0;

	retval = ext2fs_dblist_iterate2(dblist, db_dir_proc, &ctx);

	ಆದರೆ (!ಖಂಡ_buf)
		ext2fs_free_mem(&ctx.buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಹಿಂತಿರುಗಿ ctx.errcode;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ db_dir_proc(ext2_filsys fs, ರಚನೆ ext2_db_entry2 *db_info,
		       ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ ext2_inode	inode;
	ರಚನೆ dir_context	*ctx;
	ಪೂರ್ಣಾಂಕ			ret;

	ctx = (ರಚನೆ dir_context *) priv_ದತ್ತಾಂಶ;
	ctx->dir = db_info->ino;
	ctx->errcode = 0;

	ctx->errcode = ext2fs_read_inode(fs, ctx->dir, &inode);
	ಆದರೆ (ctx->errcode)
		ಹಿಂತಿರುಗಿ DBLIST_ABORT;
	ಆದರೆ (inode.i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL)
		ret = ext2fs_inline_ದತ್ತಾಂಶ_dir_iterate(fs, ctx->dir, ctx);
	ಇಲ್ಲದಿದ್ದರೆ
		ret = ext2fs_process_dir_ಖಂಡ(fs, &db_info->blk,
					       db_info->blockcnt, 0, 0,
					       priv_ದತ್ತಾಂಶ);
	ಆದರೆ ((ret & ಖಂಡ_ABORT) && !ctx->errcode)
		ಹಿಂತಿರುಗಿ DBLIST_ABORT;
	ಹಿಂತಿರುಗಿ 0;
}
