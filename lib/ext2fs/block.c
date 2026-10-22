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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية بلاڪ_context {
	ext2_filsys	fs;
	صحيح (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    صحيح		ref_وٿي,
		    فراغ	*priv_معلومات);
	e2_blkcnt_t	bcount;
	صحيح		bsize;
	صحيح		flags;
	errcode_t	errcode;
	حرف	*ind_buf;
	حرف	*dind_buf;
	حرف	*tind_buf;
	فراغ	*priv_معلومات;
};

#وصف_ڪريو check_for_ro_violation_return(ctx, ret)				\
	افعل {								\
		إذا (((ctx)->flags & بلاڪ_FLAG_READ_ONLY) &&		\
		    ((ret) & بلاڪ_بدليو)) {				\
			(ctx)->errcode = EXT2_ET_RO_بلاڪ_ITERATE;	\
			ret |= بلاڪ_ABORT | بلاڪ_ERROR;		\
			أرجع ret;					\
		}							\
	} طالما (0)

#وصف_ڪريو check_for_ro_violation_goto(ctx, ret, label)			\
	افعل {								\
		إذا (((ctx)->flags & بلاڪ_FLAG_READ_ONLY) &&		\
		    ((ret) & بلاڪ_بدليو)) {				\
			(ctx)->errcode = EXT2_ET_RO_بلاڪ_ITERATE;	\
			ret |= بلاڪ_ABORT | بلاڪ_ERROR;		\
			اذهب label;					\
		}							\
	} طالما (0)

ساكن صحيح بلاڪ_iterate_ind(blk_t *ind_بلاڪ, blk_t ref_بلاڪ,
			     صحيح ref_وٿي, بنية بلاڪ_context *ctx)
{
	صحيح	ret = 0, بدليو = 0;
	صحيح	i, flags, حد_بندي, وٿي;
	blk_t	*بلاڪ_nr;
	blk64_t	blk64;

	حد_بندي = ctx->fs->blocksize >> 2;
	إذا (!(ctx->flags & بلاڪ_FLAG_اونهائي_TRAVERSE) &&
	    !(ctx->flags & بلاڪ_FLAG_معلومات_ONLY)) {
		blk64 = *ind_بلاڪ;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلاڪ_ڳڻپ_IND, ref_بلاڪ,
				   ref_وٿي, ctx->priv_معلومات);
		*ind_بلاڪ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	إذا (!*ind_بلاڪ || (ret & بلاڪ_ABORT)) {
		ctx->bcount += حد_بندي;
		أرجع ret;
	}
	إذا (*ind_بلاڪ >= ext2fs_بلاڪون_ڳڻپ(ctx->fs->super) ||
	    *ind_بلاڪ < ctx->fs->super->s_پهريون_معلومات_بلاڪ) {
		ctx->errcode = EXT2_ET_BAD_IND_بلاڪ;
		ret |= بلاڪ_ERROR;
		أرجع ret;
	}
	ctx->errcode = ext2fs_read_ind_بلاڪ(ctx->fs, *ind_بلاڪ,
					     ctx->ind_buf);
	إذا (ctx->errcode) {
		ret |= بلاڪ_ERROR;
		أرجع ret;
	}

	بلاڪ_nr = (blk_t *) ctx->ind_buf;
	وٿي = 0;
	إذا (ctx->flags & بلاڪ_FLAG_APPEND) {
		لكل (i = 0; i < حد_بندي; i++, ctx->bcount++, بلاڪ_nr++) {
			blk64 = *بلاڪ_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_بلاڪ, وٿي,
					     ctx->priv_معلومات);
			*بلاڪ_nr = blk64;
			بدليو	|= flags;
			إذا (flags & بلاڪ_ABORT) {
				ret |= بلاڪ_ABORT;
				اكسر;
			}
			وٿي += حجم(blk_t);
		}
	} وإلا {
		لكل (i = 0; i < حد_بندي; i++, ctx->bcount++, بلاڪ_nr++) {
			إذا (*بلاڪ_nr == 0)
				اذهب skip_sparse;
			blk64 = *بلاڪ_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_بلاڪ, وٿي,
					     ctx->priv_معلومات);
			*بلاڪ_nr = blk64;
			بدليو	|= flags;
			إذا (flags & بلاڪ_ABORT) {
				ret |= بلاڪ_ABORT;
				اكسر;
			}
		skip_sparse:
			وٿي += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, بدليو);
	إذا (بدليو & بلاڪ_بدليو) {
		ctx->errcode = ext2fs_write_ind_بلاڪ(ctx->fs, *ind_بلاڪ,
						      ctx->ind_buf);
		إذا (ctx->errcode)
			ret |= بلاڪ_ERROR | بلاڪ_ABORT;
	}
	إذا ((ctx->flags & بلاڪ_FLAG_اونهائي_TRAVERSE) &&
	    !(ctx->flags & بلاڪ_FLAG_معلومات_ONLY) &&
	    !(ret & بلاڪ_ABORT)) {
		blk64 = *ind_بلاڪ;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلاڪ_ڳڻپ_IND, ref_بلاڪ,
				    ref_وٿي, ctx->priv_معلومات);
		*ind_بلاڪ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	أرجع ret;
}

ساكن صحيح بلاڪ_iterate_dind(blk_t *dind_بلاڪ, blk_t ref_بلاڪ,
			      صحيح ref_وٿي, بنية بلاڪ_context *ctx)
{
	صحيح	ret = 0, بدليو = 0;
	صحيح	i, flags, حد_بندي, وٿي;
	blk_t	*بلاڪ_nr;
	blk64_t	blk64;

	حد_بندي = ctx->fs->blocksize >> 2;
	إذا (!(ctx->flags & (بلاڪ_FLAG_اونهائي_TRAVERSE |
			    بلاڪ_FLAG_معلومات_ONLY))) {
		blk64 = *dind_بلاڪ;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلاڪ_ڳڻپ_DIND, ref_بلاڪ,
				   ref_وٿي, ctx->priv_معلومات);
		*dind_بلاڪ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	إذا (!*dind_بلاڪ || (ret & بلاڪ_ABORT)) {
		ctx->bcount += حد_بندي*حد_بندي;
		أرجع ret;
	}
	إذا (*dind_بلاڪ >= ext2fs_بلاڪون_ڳڻپ(ctx->fs->super) ||
	    *dind_بلاڪ < ctx->fs->super->s_پهريون_معلومات_بلاڪ) {
		ctx->errcode = EXT2_ET_BAD_DIND_بلاڪ;
		ret |= بلاڪ_ERROR;
		أرجع ret;
	}
	ctx->errcode = ext2fs_read_ind_بلاڪ(ctx->fs, *dind_بلاڪ,
					     ctx->dind_buf);
	إذا (ctx->errcode) {
		ret |= بلاڪ_ERROR;
		أرجع ret;
	}

	بلاڪ_nr = (blk_t *) ctx->dind_buf;
	وٿي = 0;
	إذا (ctx->flags & بلاڪ_FLAG_APPEND) {
		لكل (i = 0; i < حد_بندي; i++, بلاڪ_nr++) {
			flags = بلاڪ_iterate_ind(بلاڪ_nr,
						  *dind_بلاڪ, وٿي,
						  ctx);
			بدليو |= flags;
			إذا (flags & (بلاڪ_ABORT | بلاڪ_ERROR)) {
				ret |= flags & (بلاڪ_ABORT | بلاڪ_ERROR);
				اكسر;
			}
			وٿي += حجم(blk_t);
		}
	} وإلا {
		لكل (i = 0; i < حد_بندي; i++, بلاڪ_nr++) {
			إذا (*بلاڪ_nr == 0) {
				ctx->bcount += حد_بندي;
				استمر;
			}
			flags = بلاڪ_iterate_ind(بلاڪ_nr,
						  *dind_بلاڪ, وٿي,
						  ctx);
			بدليو |= flags;
			إذا (flags & (بلاڪ_ABORT | بلاڪ_ERROR)) {
				ret |= flags & (بلاڪ_ABORT | بلاڪ_ERROR);
				اكسر;
			}
			وٿي += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, بدليو);
	إذا (بدليو & بلاڪ_بدليو) {
		ctx->errcode = ext2fs_write_ind_بلاڪ(ctx->fs, *dind_بلاڪ,
						      ctx->dind_buf);
		إذا (ctx->errcode)
			ret |= بلاڪ_ERROR | بلاڪ_ABORT;
	}
	إذا ((ctx->flags & بلاڪ_FLAG_اونهائي_TRAVERSE) &&
	    !(ctx->flags & بلاڪ_FLAG_معلومات_ONLY) &&
	    !(ret & بلاڪ_ABORT)) {
		blk64 = *dind_بلاڪ;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلاڪ_ڳڻپ_DIND, ref_بلاڪ,
				    ref_وٿي, ctx->priv_معلومات);
		*dind_بلاڪ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	أرجع ret;
}

ساكن صحيح بلاڪ_iterate_tind(blk_t *tind_بلاڪ, blk_t ref_بلاڪ,
			      صحيح ref_وٿي, بنية بلاڪ_context *ctx)
{
	صحيح	ret = 0, بدليو = 0;
	صحيح	i, flags, حد_بندي, وٿي;
	blk_t	*بلاڪ_nr;
	blk64_t	blk64;

	حد_بندي = ctx->fs->blocksize >> 2;
	إذا (!(ctx->flags & (بلاڪ_FLAG_اونهائي_TRAVERSE |
			    بلاڪ_FLAG_معلومات_ONLY))) {
		blk64 = *tind_بلاڪ;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلاڪ_ڳڻپ_TIND, ref_بلاڪ,
				   ref_وٿي, ctx->priv_معلومات);
		*tind_بلاڪ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	إذا (!*tind_بلاڪ || (ret & بلاڪ_ABORT)) {
		ctx->bcount += ((بنانشان ڊگهو ڊگهو) حد_بندي)*حد_بندي*حد_بندي;
		أرجع ret;
	}
	إذا (*tind_بلاڪ >= ext2fs_بلاڪون_ڳڻپ(ctx->fs->super) ||
	    *tind_بلاڪ < ctx->fs->super->s_پهريون_معلومات_بلاڪ) {
		ctx->errcode = EXT2_ET_BAD_TIND_بلاڪ;
		ret |= بلاڪ_ERROR;
		أرجع ret;
	}
	ctx->errcode = ext2fs_read_ind_بلاڪ(ctx->fs, *tind_بلاڪ,
					     ctx->tind_buf);
	إذا (ctx->errcode) {
		ret |= بلاڪ_ERROR;
		أرجع ret;
	}

	بلاڪ_nr = (blk_t *) ctx->tind_buf;
	وٿي = 0;
	إذا (ctx->flags & بلاڪ_FLAG_APPEND) {
		لكل (i = 0; i < حد_بندي; i++, بلاڪ_nr++) {
			flags = بلاڪ_iterate_dind(بلاڪ_nr,
						   *tind_بلاڪ,
						   وٿي, ctx);
			بدليو |= flags;
			إذا (flags & (بلاڪ_ABORT | بلاڪ_ERROR)) {
				ret |= flags & (بلاڪ_ABORT | بلاڪ_ERROR);
				اكسر;
			}
			وٿي += حجم(blk_t);
		}
	} وإلا {
		لكل (i = 0; i < حد_بندي; i++, بلاڪ_nr++) {
			إذا (*بلاڪ_nr == 0) {
				ctx->bcount += حد_بندي*حد_بندي;
				استمر;
			}
			flags = بلاڪ_iterate_dind(بلاڪ_nr,
						   *tind_بلاڪ,
						   وٿي, ctx);
			بدليو |= flags;
			إذا (flags & (بلاڪ_ABORT | بلاڪ_ERROR)) {
				ret |= flags & (بلاڪ_ABORT | بلاڪ_ERROR);
				اكسر;
			}
			وٿي += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, بدليو);
	إذا (بدليو & بلاڪ_بدليو) {
		ctx->errcode = ext2fs_write_ind_بلاڪ(ctx->fs, *tind_بلاڪ,
						      ctx->tind_buf);
		إذا (ctx->errcode)
			ret |= بلاڪ_ERROR | بلاڪ_ABORT;
	}
	إذا ((ctx->flags & بلاڪ_FLAG_اونهائي_TRAVERSE) &&
	    !(ctx->flags & بلاڪ_FLAG_معلومات_ONLY) &&
	    !(ret & بلاڪ_ABORT)) {
		blk64 = *tind_بلاڪ;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلاڪ_ڳڻپ_TIND, ref_بلاڪ,
				    ref_وٿي, ctx->priv_معلومات);
		*tind_بلاڪ = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	أرجع ret;
}

errcode_t ext2fs_بلاڪ_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *بلاڪ_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحيح		ref_وٿي,
					    فراغ	*priv_معلومات),
				فراغ *priv_معلومات)
{
	صحيح	i;
	صحيح	r, ret = 0;
	بنية ext2_inode inode;
	errcode_t	retval;
	بنية بلاڪ_context ctx;
	صحيح	حد_بندي;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	إذا (ctx.errcode)
		أرجع ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	إذا (inode.i_flags & EXT4_INLINE_معلومات_FL)
		أرجع EXT2_ET_INLINE_معلومات_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	إذا (flags & بلاڪ_FLAG_NO_LARGE) {
		إذا (!LINUX_S_ISDIR(inode.i_گهڻائي) &&
		    (inode.i_سائيز_مٿاهون != 0))
			أرجع EXT2_ET_FILE_TOO_BIG;
	}

	حد_بندي = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_معلومات = priv_معلومات;
	ctx.flags = flags;
	ctx.bcount = 0;
	إذا (بلاڪ_buf) {
		ctx.ind_buf = بلاڪ_buf;
	} وإلا {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		إذا (retval)
			أرجع retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	إذا ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & بلاڪ_FLAG_معلومات_ONLY)) {
		إذا (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   بلاڪ_ڳڻپ_TRANSLATOR,
					   0, 0, priv_معلومات);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			إذا (ret & بلاڪ_ABORT)
				اذهب abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	إذا (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		بنية ext2fs_extent	extent, ايندڙ;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		صحيح			op = EXT2_EXTENT_پاڙ;
		صحيح			uninit;
		بنانشان صحيح		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		إذا (ctx.errcode)
			اذهب abort_exit;

		طالما (1) {
			إذا (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			وإلا
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			إذا (ctx.errcode) {
				إذا (ctx.errcode != EXT2_ET_EXTENT_NO_ايندڙ)
					اكسر;
				ctx.errcode = 0;
				إذا (!(flags & بلاڪ_FLAG_APPEND))
					اكسر;
			ايندڙ_بلاڪ_سيٽ:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_معلومات);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_پورو);
				إذا (r & بلاڪ_بدليو) {
					ctx.errcode =
						ext2fs_extent_سيٽ_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					إذا (ctx.errcode || (ret & بلاڪ_ABORT))
						اكسر;
					إذا (blk)
						اذهب ايندڙ_بلاڪ_سيٽ;
				}
				اكسر;
			}

			op = EXT2_EXTENT_ايندڙ;
			blk = extent.e_pblk;
			إذا (!(extent.e_flags & EXT2_EXTENT_FLAGS_پن)) {
				إذا (ctx.flags & بلاڪ_FLAG_معلومات_ONLY)
					استمر;
				إذا ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_ٻيو_گهمو) &&
				     !(ctx.flags & بلاڪ_FLAG_اونهائي_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_ٻيو_گهمو) &&
				     (ctx.flags & بلاڪ_FLAG_اونهائي_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_معلومات);
					إذا (ret & بلاڪ_بدليو) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						إذا (ctx.errcode)
							اكسر;
					}
					إذا (ret & بلاڪ_ABORT)
						اكسر;
				}
				استمر;
			}
			uninit = 0;
			إذا (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_سيٽ_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &ايندڙ);

#إذا 0
			لکو("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#جيڪڏهن_پورو
			إذا (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				استمر;
			إذا (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			لكل (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_معلومات);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_پورو);
				إذا (r & بلاڪ_بدليو) {
					ctx.errcode =
						ext2fs_extent_سيٽ_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					إذا (ctx.errcode)
						اذهب extent_پورو;
				}
				إذا (ret & بلاڪ_ABORT)
					اذهب extent_پورو;
			}
			إذا (retval == 0) {
				extent = ايندڙ;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_پورو:
		ext2fs_extent_free(handle);
		ret |= بلاڪ_ERROR; /* ctx.errcode is always valid here */
		اذهب errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	لكل (i = 0; i < EXT2_NDIR_بلاڪون ; i++, ctx.bcount++) {
		إذا (inode.i_بلاڪ[i] || (flags & بلاڪ_FLAG_APPEND)) {
			blk64 = inode.i_بلاڪ[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_معلومات);
			inode.i_بلاڪ[i] = (blk_t) blk64;
			إذا (ret & بلاڪ_ABORT)
				اذهب abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	إذا (inode.i_بلاڪ[EXT2_IND_بلاڪ] || (flags & بلاڪ_FLAG_APPEND)) {
		ret |= بلاڪ_iterate_ind(&inode.i_بلاڪ[EXT2_IND_بلاڪ], 
					 0, EXT2_IND_بلاڪ, &ctx);
		إذا (ret & بلاڪ_ABORT)
			اذهب abort_exit;
	} وإلا
		ctx.bcount += حد_بندي;
	إذا (inode.i_بلاڪ[EXT2_DIND_بلاڪ] || (flags & بلاڪ_FLAG_APPEND)) {
		ret |= بلاڪ_iterate_dind(&inode.i_بلاڪ[EXT2_DIND_بلاڪ],
					  0, EXT2_DIND_بلاڪ, &ctx);
		إذا (ret & بلاڪ_ABORT)
			اذهب abort_exit;
	} وإلا
		ctx.bcount += حد_بندي * حد_بندي;
	إذا (inode.i_بلاڪ[EXT2_TIND_بلاڪ] || (flags & بلاڪ_FLAG_APPEND)) {
		ret |= بلاڪ_iterate_tind(&inode.i_بلاڪ[EXT2_TIND_بلاڪ],
					  0, EXT2_TIND_بلاڪ, &ctx);
		إذا (ret & بلاڪ_ABORT)
			اذهب abort_exit;
	}

abort_exit:
	إذا (ret & بلاڪ_بدليو) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		إذا (retval) {
			ret |= بلاڪ_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	إذا (!بلاڪ_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	أرجع (ret & بلاڪ_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

بنية xlate64 {
	صحيح (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    صحيح		ref_وٿي,
		    فراغ	*priv_معلومات);
	فراغ *real_ذاتي;
};

ساكن صحيح xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			صحيح ref_وٿي, فراغ *priv_معلومات)
{
	بنية xlate64 *xl = (بنية xlate64 *) priv_معلومات;
	صحيح		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_وٿي,
			     xl->real_ذاتي);
	*blocknr = block32;
	أرجع ret;
}

errcode_t ext2fs_بلاڪ_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *بلاڪ_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحيح		ref_وٿي,
					    فراغ	*priv_معلومات),
				فراغ *priv_معلومات)
{
	بنية xlate64 xl;

	xl.real_ذاتي = priv_معلومات;
	xl.func = func;

	أرجع ext2fs_بلاڪ_iterate3(fs, ino, flags, بلاڪ_buf, 
				     xlate64_func, &xl);
}


بنية xlate {
	صحيح (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    صحيح		bcount,
		    فراغ	*priv_معلومات);
	فراغ *real_ذاتي;
};

#جيڪڏهن_وصف_ٿيل __TURBOC__
 #هدايت argsused
#جيڪڏهن_پورو
ساكن صحيح xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_بلاڪ EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
		      صحيح ref_وٿي EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
		      فراغ *priv_معلومات)
{
	بنية xlate *xl = (بنية xlate *) priv_معلومات;

	أرجع (*xl->func)(fs, blocknr, (صحيح) blockcnt, xl->real_ذاتي);
}

errcode_t ext2fs_بلاڪ_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       صحيح	flags,
			       حرف *بلاڪ_buf,
			       صحيح (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   صحيح	blockcnt,
					   فراغ	*priv_معلومات),
			       فراغ *priv_معلومات)
{
	بنية xlate xl;

	xl.real_ذاتي = priv_معلومات;
	xl.func = func;

	أرجع ext2fs_بلاڪ_iterate2(fs, ino, بلاڪ_FLAG_NO_LARGE | flags,
				     بلاڪ_buf, xlate_func, &xl);
}

