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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ರಚನೆ ಖಂಡ_context {
	ext2_filsys	fs;
	ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    ಪೂರ್ಣಾಂಕ		ref_ಸ್ಥಾನಾಂತರ,
		    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ);
	e2_blkcnt_t	bcount;
	ಪೂರ್ಣಾಂಕ		bsize;
	ಪೂರ್ಣಾಂಕ		flags;
	errcode_t	errcode;
	ಅಕ್ಷರ	*ind_buf;
	ಅಕ್ಷರ	*dind_buf;
	ಅಕ್ಷರ	*tind_buf;
	ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ;
};

#ವ್ಯಾಖ್ಯಾನಿಸು check_for_ro_violation_return(ctx, ret)				\
	ಮಾಡು {								\
		ಆದರೆ (((ctx)->flags & ಖಂಡ_FLAG_READ_ONLY) &&		\
		    ((ret) & ಖಂಡ_ಬದಲಾಯಿತು)) {				\
			(ctx)->errcode = EXT2_ET_RO_ಖಂಡ_ITERATE;	\
			ret |= ಖಂಡ_ABORT | ಖಂಡ_ERROR;		\
			ಹಿಂತಿರುಗಿ ret;					\
		}							\
	} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು check_for_ro_violation_goto(ctx, ret, label)			\
	ಮಾಡು {								\
		ಆದರೆ (((ctx)->flags & ಖಂಡ_FLAG_READ_ONLY) &&		\
		    ((ret) & ಖಂಡ_ಬದಲಾಯಿತು)) {				\
			(ctx)->errcode = EXT2_ET_RO_ಖಂಡ_ITERATE;	\
			ret |= ಖಂಡ_ABORT | ಖಂಡ_ERROR;		\
			ಹೋಗು label;					\
		}							\
	} ಆಗ (0)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಖಂಡ_iterate_ind(blk_t *ind_ಖಂಡ, blk_t ref_ಖಂಡ,
			     ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ, ರಚನೆ ಖಂಡ_context *ctx)
{
	ಪೂರ್ಣಾಂಕ	ret = 0, ಬದಲಾಯಿತು = 0;
	ಪೂರ್ಣಾಂಕ	i, flags, ಎಲ್ಲೆ, ಸ್ಥಾನಾಂತರ;
	blk_t	*ಖಂಡ_nr;
	blk64_t	blk64;

	ಎಲ್ಲೆ = ctx->fs->blocksize >> 2;
	ಆದರೆ (!(ctx->flags & ಖಂಡ_FLAG_ಆಳ_TRAVERSE) &&
	    !(ctx->flags & ಖಂಡ_FLAG_ದತ್ತಾಂಶ_ONLY)) {
		blk64 = *ind_ಖಂಡ;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   ಖಂಡ_ಎಣಿಕೆ_IND, ref_ಖಂಡ,
				   ref_ಸ್ಥಾನಾಂತರ, ctx->priv_ದತ್ತಾಂಶ);
		*ind_ಖಂಡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಆದರೆ (!*ind_ಖಂಡ || (ret & ಖಂಡ_ABORT)) {
		ctx->bcount += ಎಲ್ಲೆ;
		ಹಿಂತಿರುಗಿ ret;
	}
	ಆದರೆ (*ind_ಖಂಡ >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(ctx->fs->super) ||
	    *ind_ಖಂಡ < ctx->fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) {
		ctx->errcode = EXT2_ET_BAD_IND_ಖಂಡ;
		ret |= ಖಂಡ_ERROR;
		ಹಿಂತಿರುಗಿ ret;
	}
	ctx->errcode = ext2fs_read_ind_ಖಂಡ(ctx->fs, *ind_ಖಂಡ,
					     ctx->ind_buf);
	ಆದರೆ (ctx->errcode) {
		ret |= ಖಂಡ_ERROR;
		ಹಿಂತಿರುಗಿ ret;
	}

	ಖಂಡ_nr = (blk_t *) ctx->ind_buf;
	ಸ್ಥಾನಾಂತರ = 0;
	ಆದರೆ (ctx->flags & ಖಂಡ_FLAG_APPEND) {
		ಗಾಗಿ (i = 0; i < ಎಲ್ಲೆ; i++, ctx->bcount++, ಖಂಡ_nr++) {
			blk64 = *ಖಂಡ_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_ಖಂಡ, ಸ್ಥಾನಾಂತರ,
					     ctx->priv_ದತ್ತಾಂಶ);
			*ಖಂಡ_nr = blk64;
			ಬದಲಾಯಿತು	|= flags;
			ಆದರೆ (flags & ಖಂಡ_ABORT) {
				ret |= ಖಂಡ_ABORT;
				ನಿಲ್ಲಿಸು;
			}
			ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ(blk_t);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (i = 0; i < ಎಲ್ಲೆ; i++, ctx->bcount++, ಖಂಡ_nr++) {
			ಆದರೆ (*ಖಂಡ_nr == 0)
				ಹೋಗು skip_sparse;
			blk64 = *ಖಂಡ_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_ಖಂಡ, ಸ್ಥಾನಾಂತರ,
					     ctx->priv_ದತ್ತಾಂಶ);
			*ಖಂಡ_nr = blk64;
			ಬದಲಾಯಿತು	|= flags;
			ಆದರೆ (flags & ಖಂಡ_ABORT) {
				ret |= ಖಂಡ_ABORT;
				ನಿಲ್ಲಿಸು;
			}
		skip_sparse:
			ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, ಬದಲಾಯಿತು);
	ಆದರೆ (ಬದಲಾಯಿತು & ಖಂಡ_ಬದಲಾಯಿತು) {
		ctx->errcode = ext2fs_write_ind_ಖಂಡ(ctx->fs, *ind_ಖಂಡ,
						      ctx->ind_buf);
		ಆದರೆ (ctx->errcode)
			ret |= ಖಂಡ_ERROR | ಖಂಡ_ABORT;
	}
	ಆದರೆ ((ctx->flags & ಖಂಡ_FLAG_ಆಳ_TRAVERSE) &&
	    !(ctx->flags & ಖಂಡ_FLAG_ದತ್ತಾಂಶ_ONLY) &&
	    !(ret & ಖಂಡ_ABORT)) {
		blk64 = *ind_ಖಂಡ;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    ಖಂಡ_ಎಣಿಕೆ_IND, ref_ಖಂಡ,
				    ref_ಸ್ಥಾನಾಂತರ, ctx->priv_ದತ್ತಾಂಶ);
		*ind_ಖಂಡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಖಂಡ_iterate_dind(blk_t *dind_ಖಂಡ, blk_t ref_ಖಂಡ,
			      ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ, ರಚನೆ ಖಂಡ_context *ctx)
{
	ಪೂರ್ಣಾಂಕ	ret = 0, ಬದಲಾಯಿತು = 0;
	ಪೂರ್ಣಾಂಕ	i, flags, ಎಲ್ಲೆ, ಸ್ಥಾನಾಂತರ;
	blk_t	*ಖಂಡ_nr;
	blk64_t	blk64;

	ಎಲ್ಲೆ = ctx->fs->blocksize >> 2;
	ಆದರೆ (!(ctx->flags & (ಖಂಡ_FLAG_ಆಳ_TRAVERSE |
			    ಖಂಡ_FLAG_ದತ್ತಾಂಶ_ONLY))) {
		blk64 = *dind_ಖಂಡ;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   ಖಂಡ_ಎಣಿಕೆ_DIND, ref_ಖಂಡ,
				   ref_ಸ್ಥಾನಾಂತರ, ctx->priv_ದತ್ತಾಂಶ);
		*dind_ಖಂಡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಆದರೆ (!*dind_ಖಂಡ || (ret & ಖಂಡ_ABORT)) {
		ctx->bcount += ಎಲ್ಲೆ*ಎಲ್ಲೆ;
		ಹಿಂತಿರುಗಿ ret;
	}
	ಆದರೆ (*dind_ಖಂಡ >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(ctx->fs->super) ||
	    *dind_ಖಂಡ < ctx->fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) {
		ctx->errcode = EXT2_ET_BAD_DIND_ಖಂಡ;
		ret |= ಖಂಡ_ERROR;
		ಹಿಂತಿರುಗಿ ret;
	}
	ctx->errcode = ext2fs_read_ind_ಖಂಡ(ctx->fs, *dind_ಖಂಡ,
					     ctx->dind_buf);
	ಆದರೆ (ctx->errcode) {
		ret |= ಖಂಡ_ERROR;
		ಹಿಂತಿರುಗಿ ret;
	}

	ಖಂಡ_nr = (blk_t *) ctx->dind_buf;
	ಸ್ಥಾನಾಂತರ = 0;
	ಆದರೆ (ctx->flags & ಖಂಡ_FLAG_APPEND) {
		ಗಾಗಿ (i = 0; i < ಎಲ್ಲೆ; i++, ಖಂಡ_nr++) {
			flags = ಖಂಡ_iterate_ind(ಖಂಡ_nr,
						  *dind_ಖಂಡ, ಸ್ಥಾನಾಂತರ,
						  ctx);
			ಬದಲಾಯಿತು |= flags;
			ಆದರೆ (flags & (ಖಂಡ_ABORT | ಖಂಡ_ERROR)) {
				ret |= flags & (ಖಂಡ_ABORT | ಖಂಡ_ERROR);
				ನಿಲ್ಲಿಸು;
			}
			ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ(blk_t);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (i = 0; i < ಎಲ್ಲೆ; i++, ಖಂಡ_nr++) {
			ಆದರೆ (*ಖಂಡ_nr == 0) {
				ctx->bcount += ಎಲ್ಲೆ;
				ಮುಂದುವರಿ;
			}
			flags = ಖಂಡ_iterate_ind(ಖಂಡ_nr,
						  *dind_ಖಂಡ, ಸ್ಥಾನಾಂತರ,
						  ctx);
			ಬದಲಾಯಿತು |= flags;
			ಆದರೆ (flags & (ಖಂಡ_ABORT | ಖಂಡ_ERROR)) {
				ret |= flags & (ಖಂಡ_ABORT | ಖಂಡ_ERROR);
				ನಿಲ್ಲಿಸು;
			}
			ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, ಬದಲಾಯಿತು);
	ಆದರೆ (ಬದಲಾಯಿತು & ಖಂಡ_ಬದಲಾಯಿತು) {
		ctx->errcode = ext2fs_write_ind_ಖಂಡ(ctx->fs, *dind_ಖಂಡ,
						      ctx->dind_buf);
		ಆದರೆ (ctx->errcode)
			ret |= ಖಂಡ_ERROR | ಖಂಡ_ABORT;
	}
	ಆದರೆ ((ctx->flags & ಖಂಡ_FLAG_ಆಳ_TRAVERSE) &&
	    !(ctx->flags & ಖಂಡ_FLAG_ದತ್ತಾಂಶ_ONLY) &&
	    !(ret & ಖಂಡ_ABORT)) {
		blk64 = *dind_ಖಂಡ;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    ಖಂಡ_ಎಣಿಕೆ_DIND, ref_ಖಂಡ,
				    ref_ಸ್ಥಾನಾಂತರ, ctx->priv_ದತ್ತಾಂಶ);
		*dind_ಖಂಡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಖಂಡ_iterate_tind(blk_t *tind_ಖಂಡ, blk_t ref_ಖಂಡ,
			      ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ, ರಚನೆ ಖಂಡ_context *ctx)
{
	ಪೂರ್ಣಾಂಕ	ret = 0, ಬದಲಾಯಿತು = 0;
	ಪೂರ್ಣಾಂಕ	i, flags, ಎಲ್ಲೆ, ಸ್ಥಾನಾಂತರ;
	blk_t	*ಖಂಡ_nr;
	blk64_t	blk64;

	ಎಲ್ಲೆ = ctx->fs->blocksize >> 2;
	ಆದರೆ (!(ctx->flags & (ಖಂಡ_FLAG_ಆಳ_TRAVERSE |
			    ಖಂಡ_FLAG_ದತ್ತಾಂಶ_ONLY))) {
		blk64 = *tind_ಖಂಡ;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   ಖಂಡ_ಎಣಿಕೆ_TIND, ref_ಖಂಡ,
				   ref_ಸ್ಥಾನಾಂತರ, ctx->priv_ದತ್ತಾಂಶ);
		*tind_ಖಂಡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಆದರೆ (!*tind_ಖಂಡ || (ret & ಖಂಡ_ABORT)) {
		ctx->bcount += ((ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಎಲ್ಲೆ)*ಎಲ್ಲೆ*ಎಲ್ಲೆ;
		ಹಿಂತಿರುಗಿ ret;
	}
	ಆದರೆ (*tind_ಖಂಡ >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(ctx->fs->super) ||
	    *tind_ಖಂಡ < ctx->fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) {
		ctx->errcode = EXT2_ET_BAD_TIND_ಖಂಡ;
		ret |= ಖಂಡ_ERROR;
		ಹಿಂತಿರುಗಿ ret;
	}
	ctx->errcode = ext2fs_read_ind_ಖಂಡ(ctx->fs, *tind_ಖಂಡ,
					     ctx->tind_buf);
	ಆದರೆ (ctx->errcode) {
		ret |= ಖಂಡ_ERROR;
		ಹಿಂತಿರುಗಿ ret;
	}

	ಖಂಡ_nr = (blk_t *) ctx->tind_buf;
	ಸ್ಥಾನಾಂತರ = 0;
	ಆದರೆ (ctx->flags & ಖಂಡ_FLAG_APPEND) {
		ಗಾಗಿ (i = 0; i < ಎಲ್ಲೆ; i++, ಖಂಡ_nr++) {
			flags = ಖಂಡ_iterate_dind(ಖಂಡ_nr,
						   *tind_ಖಂಡ,
						   ಸ್ಥಾನಾಂತರ, ctx);
			ಬದಲಾಯಿತು |= flags;
			ಆದರೆ (flags & (ಖಂಡ_ABORT | ಖಂಡ_ERROR)) {
				ret |= flags & (ಖಂಡ_ABORT | ಖಂಡ_ERROR);
				ನಿಲ್ಲಿಸು;
			}
			ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ(blk_t);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಾಗಿ (i = 0; i < ಎಲ್ಲೆ; i++, ಖಂಡ_nr++) {
			ಆದರೆ (*ಖಂಡ_nr == 0) {
				ctx->bcount += ಎಲ್ಲೆ*ಎಲ್ಲೆ;
				ಮುಂದುವರಿ;
			}
			flags = ಖಂಡ_iterate_dind(ಖಂಡ_nr,
						   *tind_ಖಂಡ,
						   ಸ್ಥಾನಾಂತರ, ctx);
			ಬದಲಾಯಿತು |= flags;
			ಆದರೆ (flags & (ಖಂಡ_ABORT | ಖಂಡ_ERROR)) {
				ret |= flags & (ಖಂಡ_ABORT | ಖಂಡ_ERROR);
				ನಿಲ್ಲಿಸು;
			}
			ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, ಬದಲಾಯಿತು);
	ಆದರೆ (ಬದಲಾಯಿತು & ಖಂಡ_ಬದಲಾಯಿತು) {
		ctx->errcode = ext2fs_write_ind_ಖಂಡ(ctx->fs, *tind_ಖಂಡ,
						      ctx->tind_buf);
		ಆದರೆ (ctx->errcode)
			ret |= ಖಂಡ_ERROR | ಖಂಡ_ABORT;
	}
	ಆದರೆ ((ctx->flags & ಖಂಡ_FLAG_ಆಳ_TRAVERSE) &&
	    !(ctx->flags & ಖಂಡ_FLAG_ದತ್ತಾಂಶ_ONLY) &&
	    !(ret & ಖಂಡ_ABORT)) {
		blk64 = *tind_ಖಂಡ;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    ಖಂಡ_ಎಣಿಕೆ_TIND, ref_ಖಂಡ,
				    ref_ಸ್ಥಾನಾಂತರ, ctx->priv_ದತ್ತಾಂಶ);
		*tind_ಖಂಡ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಹಿಂತಿರುಗಿ ret;
}

errcode_t ext2fs_ಖಂಡ_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				ಪೂರ್ಣಾಂಕ	flags,
				ಅಕ್ಷರ *ಖಂಡ_buf,
				ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    ಪೂರ್ಣಾಂಕ		ref_ಸ್ಥಾನಾಂತರ,
					    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ	i;
	ಪೂರ್ಣಾಂಕ	r, ret = 0;
	ರಚನೆ ext2_inode inode;
	errcode_t	retval;
	ರಚನೆ ಖಂಡ_context ctx;
	ಪೂರ್ಣಾಂಕ	ಎಲ್ಲೆ;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (ctx.errcode)
		ಹಿಂತಿರುಗಿ ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	ಆದರೆ (inode.i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL)
		ಹಿಂತಿರುಗಿ EXT2_ET_INLINE_ದತ್ತಾಂಶ_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	ಆದರೆ (flags & ಖಂಡ_FLAG_NO_LARGE) {
		ಆದರೆ (!LINUX_S_ISDIR(inode.i_ಬಹುಲಕ) &&
		    (inode.i_ಗಾತ್ರ_ಮೇಲಿನ != 0))
			ಹಿಂತಿರುಗಿ EXT2_ET_FILE_TOO_BIG;
	}

	ಎಲ್ಲೆ = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_ದತ್ತಾಂಶ = priv_ದತ್ತಾಂಶ;
	ctx.flags = flags;
	ctx.bcount = 0;
	ಆದರೆ (ಖಂಡ_buf) {
		ctx.ind_buf = ಖಂಡ_buf;
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	ಆದರೆ ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & ಖಂಡ_FLAG_ದತ್ತಾಂಶ_ONLY)) {
		ಆದರೆ (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   ಖಂಡ_ಎಣಿಕೆ_TRANSLATOR,
					   0, 0, priv_ದತ್ತಾಂಶ);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			ಆದರೆ (ret & ಖಂಡ_ABORT)
				ಹೋಗು abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	ಆದರೆ (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		ರಚನೆ ext2fs_extent	extent, ಮುಂದಿನ;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		ಪೂರ್ಣಾಂಕ			op = EXT2_EXTENT_ಬೇರು;
		ಪೂರ್ಣಾಂಕ			uninit;
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		ಆದರೆ (ctx.errcode)
			ಹೋಗು abort_exit;

		ಆಗ (1) {
			ಆದರೆ (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			ಇಲ್ಲದಿದ್ದರೆ
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			ಆದರೆ (ctx.errcode) {
				ಆದರೆ (ctx.errcode != EXT2_ET_EXTENT_NO_ಮುಂದಿನ)
					ನಿಲ್ಲಿಸು;
				ctx.errcode = 0;
				ಆದರೆ (!(flags & ಖಂಡ_FLAG_APPEND))
					ನಿಲ್ಲಿಸು;
			ಮುಂದಿನ_ಖಂಡ_ಗಣ:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_ದತ್ತಾಂಶ);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_ಮುಗಿದ);
				ಆದರೆ (r & ಖಂಡ_ಬದಲಾಯಿತು) {
					ctx.errcode =
						ext2fs_extent_ಗಣ_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					ಆದರೆ (ctx.errcode || (ret & ಖಂಡ_ABORT))
						ನಿಲ್ಲಿಸು;
					ಆದರೆ (blk)
						ಹೋಗು ಮುಂದಿನ_ಖಂಡ_ಗಣ;
				}
				ನಿಲ್ಲಿಸು;
			}

			op = EXT2_EXTENT_ಮುಂದಿನ;
			blk = extent.e_pblk;
			ಆದರೆ (!(extent.e_flags & EXT2_EXTENT_FLAGS_ಎಲೆ)) {
				ಆದರೆ (ctx.flags & ಖಂಡ_FLAG_ದತ್ತಾಂಶ_ONLY)
					ಮುಂದುವರಿ;
				ಆದರೆ ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_ಎರಡನೇ_ಭೇಟಿ) &&
				     !(ctx.flags & ಖಂಡ_FLAG_ಆಳ_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_ಎರಡನೇ_ಭೇಟಿ) &&
				     (ctx.flags & ಖಂಡ_FLAG_ಆಳ_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_ದತ್ತಾಂಶ);
					ಆದರೆ (ret & ಖಂಡ_ಬದಲಾಯಿತು) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						ಆದರೆ (ctx.errcode)
							ನಿಲ್ಲಿಸು;
					}
					ಆದರೆ (ret & ಖಂಡ_ABORT)
						ನಿಲ್ಲಿಸು;
				}
				ಮುಂದುವರಿ;
			}
			uninit = 0;
			ಆದರೆ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_ಗಣ_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &ಮುಂದಿನ);

#ಆದರೆ 0
			ಬರೆ("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#ಆದರೆ_ಅಂತ್ಯ
			ಆದರೆ (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				ಮುಂದುವರಿ;
			ಆದರೆ (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			ಗಾಗಿ (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_ದತ್ತಾಂಶ);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_ಮುಗಿದ);
				ಆದರೆ (r & ಖಂಡ_ಬದಲಾಯಿತು) {
					ctx.errcode =
						ext2fs_extent_ಗಣ_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					ಆದರೆ (ctx.errcode)
						ಹೋಗು extent_ಮುಗಿದ;
				}
				ಆದರೆ (ret & ಖಂಡ_ABORT)
					ಹೋಗು extent_ಮುಗಿದ;
			}
			ಆದರೆ (retval == 0) {
				extent = ಮುಂದಿನ;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_ಮುಗಿದ:
		ext2fs_extent_free(handle);
		ret |= ಖಂಡ_ERROR; /* ctx.errcode is always valid here */
		ಹೋಗು errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	ಗಾಗಿ (i = 0; i < EXT2_NDIR_ಖಂಡಗಳು ; i++, ctx.bcount++) {
		ಆದರೆ (inode.i_ಖಂಡ[i] || (flags & ಖಂಡ_FLAG_APPEND)) {
			blk64 = inode.i_ಖಂಡ[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_ದತ್ತಾಂಶ);
			inode.i_ಖಂಡ[i] = (blk_t) blk64;
			ಆದರೆ (ret & ಖಂಡ_ABORT)
				ಹೋಗು abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	ಆದರೆ (inode.i_ಖಂಡ[EXT2_IND_ಖಂಡ] || (flags & ಖಂಡ_FLAG_APPEND)) {
		ret |= ಖಂಡ_iterate_ind(&inode.i_ಖಂಡ[EXT2_IND_ಖಂಡ], 
					 0, EXT2_IND_ಖಂಡ, &ctx);
		ಆದರೆ (ret & ಖಂಡ_ABORT)
			ಹೋಗು abort_exit;
	} ಇಲ್ಲದಿದ್ದರೆ
		ctx.bcount += ಎಲ್ಲೆ;
	ಆದರೆ (inode.i_ಖಂಡ[EXT2_DIND_ಖಂಡ] || (flags & ಖಂಡ_FLAG_APPEND)) {
		ret |= ಖಂಡ_iterate_dind(&inode.i_ಖಂಡ[EXT2_DIND_ಖಂಡ],
					  0, EXT2_DIND_ಖಂಡ, &ctx);
		ಆದರೆ (ret & ಖಂಡ_ABORT)
			ಹೋಗು abort_exit;
	} ಇಲ್ಲದಿದ್ದರೆ
		ctx.bcount += ಎಲ್ಲೆ * ಎಲ್ಲೆ;
	ಆದರೆ (inode.i_ಖಂಡ[EXT2_TIND_ಖಂಡ] || (flags & ಖಂಡ_FLAG_APPEND)) {
		ret |= ಖಂಡ_iterate_tind(&inode.i_ಖಂಡ[EXT2_TIND_ಖಂಡ],
					  0, EXT2_TIND_ಖಂಡ, &ctx);
		ಆದರೆ (ret & ಖಂಡ_ABORT)
			ಹೋಗು abort_exit;
	}

abort_exit:
	ಆದರೆ (ret & ಖಂಡ_ಬದಲಾಯಿತು) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		ಆದರೆ (retval) {
			ret |= ಖಂಡ_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	ಆದರೆ (!ಖಂಡ_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	ಹಿಂತಿರುಗಿ (ret & ಖಂಡ_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

ರಚನೆ xlate64 {
	ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    ಪೂರ್ಣಾಂಕ		ref_ಸ್ಥಾನಾಂತರ,
		    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ);
	ಶೂನ್ಯ *real_ಖಾಸಗಿ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ, ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ xlate64 *xl = (ರಚನೆ xlate64 *) priv_ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_ಸ್ಥಾನಾಂತರ,
			     xl->real_ಖಾಸಗಿ);
	*blocknr = block32;
	ಹಿಂತಿರುಗಿ ret;
}

errcode_t ext2fs_ಖಂಡ_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				ಪೂರ್ಣಾಂಕ	flags,
				ಅಕ್ಷರ *ಖಂಡ_buf,
				ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    ಪೂರ್ಣಾಂಕ		ref_ಸ್ಥಾನಾಂತರ,
					    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
				ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ xlate64 xl;

	xl.real_ಖಾಸಗಿ = priv_ದತ್ತಾಂಶ;
	xl.func = func;

	ಹಿಂತಿರುಗಿ ext2fs_ಖಂಡ_iterate3(fs, ino, flags, ಖಂಡ_buf, 
				     xlate64_func, &xl);
}


ರಚನೆ xlate {
	ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    ಪೂರ್ಣಾಂಕ		bcount,
		    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ);
	ಶೂನ್ಯ *real_ಖಾಸಗಿ;
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_ಖಂಡ EXT2FS_ATTR((ಬಳಸದ)),
		      ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
		      ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ xlate *xl = (ರಚನೆ xlate *) priv_ದತ್ತಾಂಶ;

	ಹಿಂತಿರುಗಿ (*xl->func)(fs, blocknr, (ಪೂರ್ಣಾಂಕ) blockcnt, xl->real_ಖಾಸಗಿ);
}

errcode_t ext2fs_ಖಂಡ_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       ಪೂರ್ಣಾಂಕ	flags,
			       ಅಕ್ಷರ *ಖಂಡ_buf,
			       ಪೂರ್ಣಾಂಕ (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   ಪೂರ್ಣಾಂಕ	blockcnt,
					   ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
			       ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ xlate xl;

	xl.real_ಖಾಸಗಿ = priv_ದತ್ತಾಂಶ;
	xl.func = func;

	ಹಿಂತಿರುಗಿ ext2fs_ಖಂಡ_iterate2(fs, ino, ಖಂಡ_FLAG_NO_LARGE | flags,
				     ಖಂಡ_buf, xlate_func, &xl);
}

