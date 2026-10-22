/*
 * block.c --- iterate over all blocks in an inode
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਬਣਤਰ ਖੰਡ_context {
	ext2_filsys	fs;
	ਪੂਰਨ (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    ਪੂਰਨ		ref_ਵਿੱਥ,
		    ਖਾਲੀ	*priv_ਅੰਕੜੇ);
	e2_blkcnt_t	bcount;
	ਪੂਰਨ		bsize;
	ਪੂਰਨ		flags;
	errcode_t	errcode;
	ਅੱਖਰ	*ind_buf;
	ਅੱਖਰ	*dind_buf;
	ਅੱਖਰ	*tind_buf;
	ਖਾਲੀ	*priv_ਅੰਕੜੇ;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ check_for_ro_violation_return(ctx, ret)				\
	ਕਰੋ {								\
		ਜੇ (((ctx)->flags & ਖੰਡ_FLAG_READ_ONLY) &&		\
		    ((ret) & ਖੰਡ_ਬਦਲਿਆ)) {				\
			(ctx)->errcode = EXT2_ET_RO_ਖੰਡ_ITERATE;	\
			ret |= ਖੰਡ_ABORT | ਖੰਡ_ERROR;		\
			ਵਾਪਸ ret;					\
		}							\
	} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ check_for_ro_violation_goto(ctx, ret, label)			\
	ਕਰੋ {								\
		ਜੇ (((ctx)->flags & ਖੰਡ_FLAG_READ_ONLY) &&		\
		    ((ret) & ਖੰਡ_ਬਦਲਿਆ)) {				\
			(ctx)->errcode = EXT2_ET_RO_ਖੰਡ_ITERATE;	\
			ret |= ਖੰਡ_ABORT | ਖੰਡ_ERROR;		\
			ਜਾਓ label;					\
		}							\
	} ਜਦੋਂ (0)

ਸਥਿਰ ਪੂਰਨ ਖੰਡ_iterate_ind(blk_t *ind_ਖੰਡ, blk_t ref_ਖੰਡ,
			     ਪੂਰਨ ref_ਵਿੱਥ, ਬਣਤਰ ਖੰਡ_context *ctx)
{
	ਪੂਰਨ	ret = 0, ਬਦਲਿਆ = 0;
	ਪੂਰਨ	i, flags, ਹੱਦਬੰਦੀ, ਵਿੱਥ;
	blk_t	*ਖੰਡ_nr;
	blk64_t	blk64;

	ਹੱਦਬੰਦੀ = ctx->fs->blocksize >> 2;
	ਜੇ (!(ctx->flags & ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE) &&
	    !(ctx->flags & ਖੰਡ_FLAG_ਅੰਕੜੇ_ONLY)) {
		blk64 = *ind_ਖੰਡ;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   ਖੰਡ_ਗਿਣਤੀ_IND, ref_ਖੰਡ,
				   ref_ਵਿੱਥ, ctx->priv_ਅੰਕੜੇ);
		*ind_ਖੰਡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ਜੇ (!*ind_ਖੰਡ || (ret & ਖੰਡ_ABORT)) {
		ctx->bcount += ਹੱਦਬੰਦੀ;
		ਵਾਪਸ ret;
	}
	ਜੇ (*ind_ਖੰਡ >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(ctx->fs->super) ||
	    *ind_ਖੰਡ < ctx->fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) {
		ctx->errcode = EXT2_ET_BAD_IND_ਖੰਡ;
		ret |= ਖੰਡ_ERROR;
		ਵਾਪਸ ret;
	}
	ctx->errcode = ext2fs_read_ind_ਖੰਡ(ctx->fs, *ind_ਖੰਡ,
					     ctx->ind_buf);
	ਜੇ (ctx->errcode) {
		ret |= ਖੰਡ_ERROR;
		ਵਾਪਸ ret;
	}

	ਖੰਡ_nr = (blk_t *) ctx->ind_buf;
	ਵਿੱਥ = 0;
	ਜੇ (ctx->flags & ਖੰਡ_FLAG_APPEND) {
		ਲਈ (i = 0; i < ਹੱਦਬੰਦੀ; i++, ctx->bcount++, ਖੰਡ_nr++) {
			blk64 = *ਖੰਡ_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_ਖੰਡ, ਵਿੱਥ,
					     ctx->priv_ਅੰਕੜੇ);
			*ਖੰਡ_nr = blk64;
			ਬਦਲਿਆ	|= flags;
			ਜੇ (flags & ਖੰਡ_ABORT) {
				ret |= ਖੰਡ_ABORT;
				ਰੋਕੋ;
			}
			ਵਿੱਥ += ਆਕਾਰ(blk_t);
		}
	} ਨਹੀਂਤਾਂ {
		ਲਈ (i = 0; i < ਹੱਦਬੰਦੀ; i++, ctx->bcount++, ਖੰਡ_nr++) {
			ਜੇ (*ਖੰਡ_nr == 0)
				ਜਾਓ skip_sparse;
			blk64 = *ਖੰਡ_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_ਖੰਡ, ਵਿੱਥ,
					     ctx->priv_ਅੰਕੜੇ);
			*ਖੰਡ_nr = blk64;
			ਬਦਲਿਆ	|= flags;
			ਜੇ (flags & ਖੰਡ_ABORT) {
				ret |= ਖੰਡ_ABORT;
				ਰੋਕੋ;
			}
		skip_sparse:
			ਵਿੱਥ += ਆਕਾਰ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, ਬਦਲਿਆ);
	ਜੇ (ਬਦਲਿਆ & ਖੰਡ_ਬਦਲਿਆ) {
		ctx->errcode = ext2fs_write_ind_ਖੰਡ(ctx->fs, *ind_ਖੰਡ,
						      ctx->ind_buf);
		ਜੇ (ctx->errcode)
			ret |= ਖੰਡ_ERROR | ਖੰਡ_ABORT;
	}
	ਜੇ ((ctx->flags & ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE) &&
	    !(ctx->flags & ਖੰਡ_FLAG_ਅੰਕੜੇ_ONLY) &&
	    !(ret & ਖੰਡ_ABORT)) {
		blk64 = *ind_ਖੰਡ;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    ਖੰਡ_ਗਿਣਤੀ_IND, ref_ਖੰਡ,
				    ref_ਵਿੱਥ, ctx->priv_ਅੰਕੜੇ);
		*ind_ਖੰਡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਖੰਡ_iterate_dind(blk_t *dind_ਖੰਡ, blk_t ref_ਖੰਡ,
			      ਪੂਰਨ ref_ਵਿੱਥ, ਬਣਤਰ ਖੰਡ_context *ctx)
{
	ਪੂਰਨ	ret = 0, ਬਦਲਿਆ = 0;
	ਪੂਰਨ	i, flags, ਹੱਦਬੰਦੀ, ਵਿੱਥ;
	blk_t	*ਖੰਡ_nr;
	blk64_t	blk64;

	ਹੱਦਬੰਦੀ = ctx->fs->blocksize >> 2;
	ਜੇ (!(ctx->flags & (ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE |
			    ਖੰਡ_FLAG_ਅੰਕੜੇ_ONLY))) {
		blk64 = *dind_ਖੰਡ;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   ਖੰਡ_ਗਿਣਤੀ_DIND, ref_ਖੰਡ,
				   ref_ਵਿੱਥ, ctx->priv_ਅੰਕੜੇ);
		*dind_ਖੰਡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ਜੇ (!*dind_ਖੰਡ || (ret & ਖੰਡ_ABORT)) {
		ctx->bcount += ਹੱਦਬੰਦੀ*ਹੱਦਬੰਦੀ;
		ਵਾਪਸ ret;
	}
	ਜੇ (*dind_ਖੰਡ >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(ctx->fs->super) ||
	    *dind_ਖੰਡ < ctx->fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) {
		ctx->errcode = EXT2_ET_BAD_DIND_ਖੰਡ;
		ret |= ਖੰਡ_ERROR;
		ਵਾਪਸ ret;
	}
	ctx->errcode = ext2fs_read_ind_ਖੰਡ(ctx->fs, *dind_ਖੰਡ,
					     ctx->dind_buf);
	ਜੇ (ctx->errcode) {
		ret |= ਖੰਡ_ERROR;
		ਵਾਪਸ ret;
	}

	ਖੰਡ_nr = (blk_t *) ctx->dind_buf;
	ਵਿੱਥ = 0;
	ਜੇ (ctx->flags & ਖੰਡ_FLAG_APPEND) {
		ਲਈ (i = 0; i < ਹੱਦਬੰਦੀ; i++, ਖੰਡ_nr++) {
			flags = ਖੰਡ_iterate_ind(ਖੰਡ_nr,
						  *dind_ਖੰਡ, ਵਿੱਥ,
						  ctx);
			ਬਦਲਿਆ |= flags;
			ਜੇ (flags & (ਖੰਡ_ABORT | ਖੰਡ_ERROR)) {
				ret |= flags & (ਖੰਡ_ABORT | ਖੰਡ_ERROR);
				ਰੋਕੋ;
			}
			ਵਿੱਥ += ਆਕਾਰ(blk_t);
		}
	} ਨਹੀਂਤਾਂ {
		ਲਈ (i = 0; i < ਹੱਦਬੰਦੀ; i++, ਖੰਡ_nr++) {
			ਜੇ (*ਖੰਡ_nr == 0) {
				ctx->bcount += ਹੱਦਬੰਦੀ;
				ਜਾਰੀ;
			}
			flags = ਖੰਡ_iterate_ind(ਖੰਡ_nr,
						  *dind_ਖੰਡ, ਵਿੱਥ,
						  ctx);
			ਬਦਲਿਆ |= flags;
			ਜੇ (flags & (ਖੰਡ_ABORT | ਖੰਡ_ERROR)) {
				ret |= flags & (ਖੰਡ_ABORT | ਖੰਡ_ERROR);
				ਰੋਕੋ;
			}
			ਵਿੱਥ += ਆਕਾਰ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, ਬਦਲਿਆ);
	ਜੇ (ਬਦਲਿਆ & ਖੰਡ_ਬਦਲਿਆ) {
		ctx->errcode = ext2fs_write_ind_ਖੰਡ(ctx->fs, *dind_ਖੰਡ,
						      ctx->dind_buf);
		ਜੇ (ctx->errcode)
			ret |= ਖੰਡ_ERROR | ਖੰਡ_ABORT;
	}
	ਜੇ ((ctx->flags & ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE) &&
	    !(ctx->flags & ਖੰਡ_FLAG_ਅੰਕੜੇ_ONLY) &&
	    !(ret & ਖੰਡ_ABORT)) {
		blk64 = *dind_ਖੰਡ;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    ਖੰਡ_ਗਿਣਤੀ_DIND, ref_ਖੰਡ,
				    ref_ਵਿੱਥ, ctx->priv_ਅੰਕੜੇ);
		*dind_ਖੰਡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਖੰਡ_iterate_tind(blk_t *tind_ਖੰਡ, blk_t ref_ਖੰਡ,
			      ਪੂਰਨ ref_ਵਿੱਥ, ਬਣਤਰ ਖੰਡ_context *ctx)
{
	ਪੂਰਨ	ret = 0, ਬਦਲਿਆ = 0;
	ਪੂਰਨ	i, flags, ਹੱਦਬੰਦੀ, ਵਿੱਥ;
	blk_t	*ਖੰਡ_nr;
	blk64_t	blk64;

	ਹੱਦਬੰਦੀ = ctx->fs->blocksize >> 2;
	ਜੇ (!(ctx->flags & (ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE |
			    ਖੰਡ_FLAG_ਅੰਕੜੇ_ONLY))) {
		blk64 = *tind_ਖੰਡ;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   ਖੰਡ_ਗਿਣਤੀ_TIND, ref_ਖੰਡ,
				   ref_ਵਿੱਥ, ctx->priv_ਅੰਕੜੇ);
		*tind_ਖੰਡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ਜੇ (!*tind_ਖੰਡ || (ret & ਖੰਡ_ABORT)) {
		ctx->bcount += ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਹੱਦਬੰਦੀ)*ਹੱਦਬੰਦੀ*ਹੱਦਬੰਦੀ;
		ਵਾਪਸ ret;
	}
	ਜੇ (*tind_ਖੰਡ >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(ctx->fs->super) ||
	    *tind_ਖੰਡ < ctx->fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) {
		ctx->errcode = EXT2_ET_BAD_TIND_ਖੰਡ;
		ret |= ਖੰਡ_ERROR;
		ਵਾਪਸ ret;
	}
	ctx->errcode = ext2fs_read_ind_ਖੰਡ(ctx->fs, *tind_ਖੰਡ,
					     ctx->tind_buf);
	ਜੇ (ctx->errcode) {
		ret |= ਖੰਡ_ERROR;
		ਵਾਪਸ ret;
	}

	ਖੰਡ_nr = (blk_t *) ctx->tind_buf;
	ਵਿੱਥ = 0;
	ਜੇ (ctx->flags & ਖੰਡ_FLAG_APPEND) {
		ਲਈ (i = 0; i < ਹੱਦਬੰਦੀ; i++, ਖੰਡ_nr++) {
			flags = ਖੰਡ_iterate_dind(ਖੰਡ_nr,
						   *tind_ਖੰਡ,
						   ਵਿੱਥ, ctx);
			ਬਦਲਿਆ |= flags;
			ਜੇ (flags & (ਖੰਡ_ABORT | ਖੰਡ_ERROR)) {
				ret |= flags & (ਖੰਡ_ABORT | ਖੰਡ_ERROR);
				ਰੋਕੋ;
			}
			ਵਿੱਥ += ਆਕਾਰ(blk_t);
		}
	} ਨਹੀਂਤਾਂ {
		ਲਈ (i = 0; i < ਹੱਦਬੰਦੀ; i++, ਖੰਡ_nr++) {
			ਜੇ (*ਖੰਡ_nr == 0) {
				ctx->bcount += ਹੱਦਬੰਦੀ*ਹੱਦਬੰਦੀ;
				ਜਾਰੀ;
			}
			flags = ਖੰਡ_iterate_dind(ਖੰਡ_nr,
						   *tind_ਖੰਡ,
						   ਵਿੱਥ, ctx);
			ਬਦਲਿਆ |= flags;
			ਜੇ (flags & (ਖੰਡ_ABORT | ਖੰਡ_ERROR)) {
				ret |= flags & (ਖੰਡ_ABORT | ਖੰਡ_ERROR);
				ਰੋਕੋ;
			}
			ਵਿੱਥ += ਆਕਾਰ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, ਬਦਲਿਆ);
	ਜੇ (ਬਦਲਿਆ & ਖੰਡ_ਬਦਲਿਆ) {
		ctx->errcode = ext2fs_write_ind_ਖੰਡ(ctx->fs, *tind_ਖੰਡ,
						      ctx->tind_buf);
		ਜੇ (ctx->errcode)
			ret |= ਖੰਡ_ERROR | ਖੰਡ_ABORT;
	}
	ਜੇ ((ctx->flags & ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE) &&
	    !(ctx->flags & ਖੰਡ_FLAG_ਅੰਕੜੇ_ONLY) &&
	    !(ret & ਖੰਡ_ABORT)) {
		blk64 = *tind_ਖੰਡ;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    ਖੰਡ_ਗਿਣਤੀ_TIND, ref_ਖੰਡ,
				    ref_ਵਿੱਥ, ctx->priv_ਅੰਕੜੇ);
		*tind_ਖੰਡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ਵਾਪਸ ret;
}

errcode_t ext2fs_ਖੰਡ_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				ਪੂਰਨ	flags,
				ਅੱਖਰ *ਖੰਡ_buf,
				ਪੂਰਨ (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    ਪੂਰਨ		ref_ਵਿੱਥ,
					    ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਪੂਰਨ	i;
	ਪੂਰਨ	r, ret = 0;
	ਬਣਤਰ ext2_inode inode;
	errcode_t	retval;
	ਬਣਤਰ ਖੰਡ_context ctx;
	ਪੂਰਨ	ਹੱਦਬੰਦੀ;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (ctx.errcode)
		ਵਾਪਸ ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	ਜੇ (inode.i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL)
		ਵਾਪਸ EXT2_ET_INLINE_ਅੰਕੜੇ_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	ਜੇ (flags & ਖੰਡ_FLAG_NO_LARGE) {
		ਜੇ (!LINUX_S_ISDIR(inode.i_ਬਹੁਲਕ) &&
		    (inode.i_ਮਾਪ_ਉੱਚਾ != 0))
			ਵਾਪਸ EXT2_ET_FILE_TOO_BIG;
	}

	ਹੱਦਬੰਦੀ = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_ਅੰਕੜੇ = priv_ਅੰਕੜੇ;
	ctx.flags = flags;
	ctx.bcount = 0;
	ਜੇ (ਖੰਡ_buf) {
		ctx.ind_buf = ਖੰਡ_buf;
	} ਨਹੀਂਤਾਂ {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	ਜੇ ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & ਖੰਡ_FLAG_ਅੰਕੜੇ_ONLY)) {
		ਜੇ (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   ਖੰਡ_ਗਿਣਤੀ_TRANSLATOR,
					   0, 0, priv_ਅੰਕੜੇ);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			ਜੇ (ret & ਖੰਡ_ABORT)
				ਜਾਓ abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	ਜੇ (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		ਬਣਤਰ ext2fs_extent	extent, ਅਗਲਾ;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		ਪੂਰਨ			op = EXT2_EXTENT_ਜੜ੍ਹ;
		ਪੂਰਨ			uninit;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		ਜੇ (ctx.errcode)
			ਜਾਓ abort_exit;

		ਜਦੋਂ (1) {
			ਜੇ (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			ਨਹੀਂਤਾਂ
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			ਜੇ (ctx.errcode) {
				ਜੇ (ctx.errcode != EXT2_ET_EXTENT_NO_ਅਗਲਾ)
					ਰੋਕੋ;
				ctx.errcode = 0;
				ਜੇ (!(flags & ਖੰਡ_FLAG_APPEND))
					ਰੋਕੋ;
			ਅਗਲਾ_ਖੰਡ_ਸੈੱਟ:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_ਅੰਕੜੇ);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_ਪੂਰਾ);
				ਜੇ (r & ਖੰਡ_ਬਦਲਿਆ) {
					ctx.errcode =
						ext2fs_extent_ਸੈੱਟ_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					ਜੇ (ctx.errcode || (ret & ਖੰਡ_ABORT))
						ਰੋਕੋ;
					ਜੇ (blk)
						ਜਾਓ ਅਗਲਾ_ਖੰਡ_ਸੈੱਟ;
				}
				ਰੋਕੋ;
			}

			op = EXT2_EXTENT_ਅਗਲਾ;
			blk = extent.e_pblk;
			ਜੇ (!(extent.e_flags & EXT2_EXTENT_FLAGS_ਪੱਤੀ)) {
				ਜੇ (ctx.flags & ਖੰਡ_FLAG_ਅੰਕੜੇ_ONLY)
					ਜਾਰੀ;
				ਜੇ ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_ਦੂਜਾ_ਦੇਖੋ) &&
				     !(ctx.flags & ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_ਦੂਜਾ_ਦੇਖੋ) &&
				     (ctx.flags & ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_ਅੰਕੜੇ);
					ਜੇ (ret & ਖੰਡ_ਬਦਲਿਆ) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						ਜੇ (ctx.errcode)
							ਰੋਕੋ;
					}
					ਜੇ (ret & ਖੰਡ_ABORT)
						ਰੋਕੋ;
				}
				ਜਾਰੀ;
			}
			uninit = 0;
			ਜੇ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_ਸੈੱਟ_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &ਅਗਲਾ);

#ਜੇ 0
			ਲਿਖੋ("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#ਜੇ_ਅੰਤ
			ਜੇ (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				ਜਾਰੀ;
			ਜੇ (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			ਲਈ (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_ਅੰਕੜੇ);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_ਪੂਰਾ);
				ਜੇ (r & ਖੰਡ_ਬਦਲਿਆ) {
					ctx.errcode =
						ext2fs_extent_ਸੈੱਟ_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					ਜੇ (ctx.errcode)
						ਜਾਓ extent_ਪੂਰਾ;
				}
				ਜੇ (ret & ਖੰਡ_ABORT)
					ਜਾਓ extent_ਪੂਰਾ;
			}
			ਜੇ (retval == 0) {
				extent = ਅਗਲਾ;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_ਪੂਰਾ:
		ext2fs_extent_free(handle);
		ret |= ਖੰਡ_ERROR; /* ctx.errcode is always valid here */
		ਜਾਓ errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	ਲਈ (i = 0; i < EXT2_NDIR_ਖੰਡਾਂ ; i++, ctx.bcount++) {
		ਜੇ (inode.i_ਖੰਡ[i] || (flags & ਖੰਡ_FLAG_APPEND)) {
			blk64 = inode.i_ਖੰਡ[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_ਅੰਕੜੇ);
			inode.i_ਖੰਡ[i] = (blk_t) blk64;
			ਜੇ (ret & ਖੰਡ_ABORT)
				ਜਾਓ abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	ਜੇ (inode.i_ਖੰਡ[EXT2_IND_ਖੰਡ] || (flags & ਖੰਡ_FLAG_APPEND)) {
		ret |= ਖੰਡ_iterate_ind(&inode.i_ਖੰਡ[EXT2_IND_ਖੰਡ], 
					 0, EXT2_IND_ਖੰਡ, &ctx);
		ਜੇ (ret & ਖੰਡ_ABORT)
			ਜਾਓ abort_exit;
	} ਨਹੀਂਤਾਂ
		ctx.bcount += ਹੱਦਬੰਦੀ;
	ਜੇ (inode.i_ਖੰਡ[EXT2_DIND_ਖੰਡ] || (flags & ਖੰਡ_FLAG_APPEND)) {
		ret |= ਖੰਡ_iterate_dind(&inode.i_ਖੰਡ[EXT2_DIND_ਖੰਡ],
					  0, EXT2_DIND_ਖੰਡ, &ctx);
		ਜੇ (ret & ਖੰਡ_ABORT)
			ਜਾਓ abort_exit;
	} ਨਹੀਂਤਾਂ
		ctx.bcount += ਹੱਦਬੰਦੀ * ਹੱਦਬੰਦੀ;
	ਜੇ (inode.i_ਖੰਡ[EXT2_TIND_ਖੰਡ] || (flags & ਖੰਡ_FLAG_APPEND)) {
		ret |= ਖੰਡ_iterate_tind(&inode.i_ਖੰਡ[EXT2_TIND_ਖੰਡ],
					  0, EXT2_TIND_ਖੰਡ, &ctx);
		ਜੇ (ret & ਖੰਡ_ABORT)
			ਜਾਓ abort_exit;
	}

abort_exit:
	ਜੇ (ret & ਖੰਡ_ਬਦਲਿਆ) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		ਜੇ (retval) {
			ret |= ਖੰਡ_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	ਜੇ (!ਖੰਡ_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	ਵਾਪਸ (ret & ਖੰਡ_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

ਬਣਤਰ xlate64 {
	ਪੂਰਨ (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    ਪੂਰਨ		ref_ਵਿੱਥ,
		    ਖਾਲੀ	*priv_ਅੰਕੜੇ);
	ਖਾਲੀ *real_ਨਿੱਜੀ;
};

ਸਥਿਰ ਪੂਰਨ xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			ਪੂਰਨ ref_ਵਿੱਥ, ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ xlate64 *xl = (ਬਣਤਰ xlate64 *) priv_ਅੰਕੜੇ;
	ਪੂਰਨ		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_ਵਿੱਥ,
			     xl->real_ਨਿੱਜੀ);
	*blocknr = block32;
	ਵਾਪਸ ret;
}

errcode_t ext2fs_ਖੰਡ_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				ਪੂਰਨ	flags,
				ਅੱਖਰ *ਖੰਡ_buf,
				ਪੂਰਨ (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    ਪੂਰਨ		ref_ਵਿੱਥ,
					    ਖਾਲੀ	*priv_ਅੰਕੜੇ),
				ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ xlate64 xl;

	xl.real_ਨਿੱਜੀ = priv_ਅੰਕੜੇ;
	xl.func = func;

	ਵਾਪਸ ext2fs_ਖੰਡ_iterate3(fs, ino, flags, ਖੰਡ_buf, 
				     xlate64_func, &xl);
}


ਬਣਤਰ xlate {
	ਪੂਰਨ (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    ਪੂਰਨ		bcount,
		    ਖਾਲੀ	*priv_ਅੰਕੜੇ);
	ਖਾਲੀ *real_ਨਿੱਜੀ;
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਪੂਰਨ xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_ਖੰਡ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		      ਪੂਰਨ ref_ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		      ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ xlate *xl = (ਬਣਤਰ xlate *) priv_ਅੰਕੜੇ;

	ਵਾਪਸ (*xl->func)(fs, blocknr, (ਪੂਰਨ) blockcnt, xl->real_ਨਿੱਜੀ);
}

errcode_t ext2fs_ਖੰਡ_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       ਪੂਰਨ	flags,
			       ਅੱਖਰ *ਖੰਡ_buf,
			       ਪੂਰਨ (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   ਪੂਰਨ	blockcnt,
					   ਖਾਲੀ	*priv_ਅੰਕੜੇ),
			       ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ xlate xl;

	xl.real_ਨਿੱਜੀ = priv_ਅੰਕੜੇ;
	xl.func = func;

	ਵਾਪਸ ext2fs_ਖੰਡ_iterate2(fs, ino, ਖੰਡ_FLAG_NO_LARGE | flags,
				     ਖੰਡ_buf, xlate_func, &xl);
}

