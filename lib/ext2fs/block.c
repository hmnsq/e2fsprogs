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
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية بلوک_context {
	ext2_filsys	fs;
	صحيح (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    صحيح		ref_جابه_جایی,
		    فراغ	*priv_داده);
	e2_blkcnt_t	bcount;
	صحيح		bsize;
	صحيح		flags;
	errcode_t	errcode;
	حرف	*ind_buf;
	حرف	*dind_buf;
	حرف	*tind_buf;
	فراغ	*priv_داده;
};

#تعریف_کن check_for_ro_violation_return(ctx, ret)				\
	افعل {								\
		إذا (((ctx)->flags & بلوک_FLAG_READ_ONLY) &&		\
		    ((ret) & بلوک_تغییریافته)) {				\
			(ctx)->errcode = EXT2_ET_RO_بلوک_ITERATE;	\
			ret |= بلوک_ABORT | بلوک_ERROR;		\
			أرجع ret;					\
		}							\
	} طالما (0)

#تعریف_کن check_for_ro_violation_goto(ctx, ret, label)			\
	افعل {								\
		إذا (((ctx)->flags & بلوک_FLAG_READ_ONLY) &&		\
		    ((ret) & بلوک_تغییریافته)) {				\
			(ctx)->errcode = EXT2_ET_RO_بلوک_ITERATE;	\
			ret |= بلوک_ABORT | بلوک_ERROR;		\
			اذهب label;					\
		}							\
	} طالما (0)

ساكن صحيح بلوک_iterate_ind(blk_t *ind_بلوک, blk_t ref_بلوک,
			     صحيح ref_جابه_جایی, بنية بلوک_context *ctx)
{
	صحيح	ret = 0, تغییریافته = 0;
	صحيح	i, flags, حد, جابه_جایی;
	blk_t	*بلوک_nr;
	blk64_t	blk64;

	حد = ctx->fs->blocksize >> 2;
	إذا (!(ctx->flags & بلوک_FLAG_عمق_TRAVERSE) &&
	    !(ctx->flags & بلوک_FLAG_داده_ONLY)) {
		blk64 = *ind_بلوک;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلوک_شمار_IND, ref_بلوک,
				   ref_جابه_جایی, ctx->priv_داده);
		*ind_بلوک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	إذا (!*ind_بلوک || (ret & بلوک_ABORT)) {
		ctx->bcount += حد;
		أرجع ret;
	}
	إذا (*ind_بلوک >= ext2fs_بلوکها_شمار(ctx->fs->super) ||
	    *ind_بلوک < ctx->fs->super->s_اولی_داده_بلوک) {
		ctx->errcode = EXT2_ET_BAD_IND_بلوک;
		ret |= بلوک_ERROR;
		أرجع ret;
	}
	ctx->errcode = ext2fs_read_ind_بلوک(ctx->fs, *ind_بلوک,
					     ctx->ind_buf);
	إذا (ctx->errcode) {
		ret |= بلوک_ERROR;
		أرجع ret;
	}

	بلوک_nr = (blk_t *) ctx->ind_buf;
	جابه_جایی = 0;
	إذا (ctx->flags & بلوک_FLAG_APPEND) {
		لكل (i = 0; i < حد; i++, ctx->bcount++, بلوک_nr++) {
			blk64 = *بلوک_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_بلوک, جابه_جایی,
					     ctx->priv_داده);
			*بلوک_nr = blk64;
			تغییریافته	|= flags;
			إذا (flags & بلوک_ABORT) {
				ret |= بلوک_ABORT;
				اكسر;
			}
			جابه_جایی += حجم(blk_t);
		}
	} وإلا {
		لكل (i = 0; i < حد; i++, ctx->bcount++, بلوک_nr++) {
			إذا (*بلوک_nr == 0)
				اذهب skip_sparse;
			blk64 = *بلوک_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_بلوک, جابه_جایی,
					     ctx->priv_داده);
			*بلوک_nr = blk64;
			تغییریافته	|= flags;
			إذا (flags & بلوک_ABORT) {
				ret |= بلوک_ABORT;
				اكسر;
			}
		skip_sparse:
			جابه_جایی += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, تغییریافته);
	إذا (تغییریافته & بلوک_تغییریافته) {
		ctx->errcode = ext2fs_write_ind_بلوک(ctx->fs, *ind_بلوک,
						      ctx->ind_buf);
		إذا (ctx->errcode)
			ret |= بلوک_ERROR | بلوک_ABORT;
	}
	إذا ((ctx->flags & بلوک_FLAG_عمق_TRAVERSE) &&
	    !(ctx->flags & بلوک_FLAG_داده_ONLY) &&
	    !(ret & بلوک_ABORT)) {
		blk64 = *ind_بلوک;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلوک_شمار_IND, ref_بلوک,
				    ref_جابه_جایی, ctx->priv_داده);
		*ind_بلوک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	أرجع ret;
}

ساكن صحيح بلوک_iterate_dind(blk_t *dind_بلوک, blk_t ref_بلوک,
			      صحيح ref_جابه_جایی, بنية بلوک_context *ctx)
{
	صحيح	ret = 0, تغییریافته = 0;
	صحيح	i, flags, حد, جابه_جایی;
	blk_t	*بلوک_nr;
	blk64_t	blk64;

	حد = ctx->fs->blocksize >> 2;
	إذا (!(ctx->flags & (بلوک_FLAG_عمق_TRAVERSE |
			    بلوک_FLAG_داده_ONLY))) {
		blk64 = *dind_بلوک;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلوک_شمار_DIND, ref_بلوک,
				   ref_جابه_جایی, ctx->priv_داده);
		*dind_بلوک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	إذا (!*dind_بلوک || (ret & بلوک_ABORT)) {
		ctx->bcount += حد*حد;
		أرجع ret;
	}
	إذا (*dind_بلوک >= ext2fs_بلوکها_شمار(ctx->fs->super) ||
	    *dind_بلوک < ctx->fs->super->s_اولی_داده_بلوک) {
		ctx->errcode = EXT2_ET_BAD_DIND_بلوک;
		ret |= بلوک_ERROR;
		أرجع ret;
	}
	ctx->errcode = ext2fs_read_ind_بلوک(ctx->fs, *dind_بلوک,
					     ctx->dind_buf);
	إذا (ctx->errcode) {
		ret |= بلوک_ERROR;
		أرجع ret;
	}

	بلوک_nr = (blk_t *) ctx->dind_buf;
	جابه_جایی = 0;
	إذا (ctx->flags & بلوک_FLAG_APPEND) {
		لكل (i = 0; i < حد; i++, بلوک_nr++) {
			flags = بلوک_iterate_ind(بلوک_nr,
						  *dind_بلوک, جابه_جایی,
						  ctx);
			تغییریافته |= flags;
			إذا (flags & (بلوک_ABORT | بلوک_ERROR)) {
				ret |= flags & (بلوک_ABORT | بلوک_ERROR);
				اكسر;
			}
			جابه_جایی += حجم(blk_t);
		}
	} وإلا {
		لكل (i = 0; i < حد; i++, بلوک_nr++) {
			إذا (*بلوک_nr == 0) {
				ctx->bcount += حد;
				استمر;
			}
			flags = بلوک_iterate_ind(بلوک_nr,
						  *dind_بلوک, جابه_جایی,
						  ctx);
			تغییریافته |= flags;
			إذا (flags & (بلوک_ABORT | بلوک_ERROR)) {
				ret |= flags & (بلوک_ABORT | بلوک_ERROR);
				اكسر;
			}
			جابه_جایی += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, تغییریافته);
	إذا (تغییریافته & بلوک_تغییریافته) {
		ctx->errcode = ext2fs_write_ind_بلوک(ctx->fs, *dind_بلوک,
						      ctx->dind_buf);
		إذا (ctx->errcode)
			ret |= بلوک_ERROR | بلوک_ABORT;
	}
	إذا ((ctx->flags & بلوک_FLAG_عمق_TRAVERSE) &&
	    !(ctx->flags & بلوک_FLAG_داده_ONLY) &&
	    !(ret & بلوک_ABORT)) {
		blk64 = *dind_بلوک;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلوک_شمار_DIND, ref_بلوک,
				    ref_جابه_جایی, ctx->priv_داده);
		*dind_بلوک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	أرجع ret;
}

ساكن صحيح بلوک_iterate_tind(blk_t *tind_بلوک, blk_t ref_بلوک,
			      صحيح ref_جابه_جایی, بنية بلوک_context *ctx)
{
	صحيح	ret = 0, تغییریافته = 0;
	صحيح	i, flags, حد, جابه_جایی;
	blk_t	*بلوک_nr;
	blk64_t	blk64;

	حد = ctx->fs->blocksize >> 2;
	إذا (!(ctx->flags & (بلوک_FLAG_عمق_TRAVERSE |
			    بلوک_FLAG_داده_ONLY))) {
		blk64 = *tind_بلوک;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلوک_شمار_TIND, ref_بلوک,
				   ref_جابه_جایی, ctx->priv_داده);
		*tind_بلوک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	إذا (!*tind_بلوک || (ret & بلوک_ABORT)) {
		ctx->bcount += ((بیعلامت دراز دراز) حد)*حد*حد;
		أرجع ret;
	}
	إذا (*tind_بلوک >= ext2fs_بلوکها_شمار(ctx->fs->super) ||
	    *tind_بلوک < ctx->fs->super->s_اولی_داده_بلوک) {
		ctx->errcode = EXT2_ET_BAD_TIND_بلوک;
		ret |= بلوک_ERROR;
		أرجع ret;
	}
	ctx->errcode = ext2fs_read_ind_بلوک(ctx->fs, *tind_بلوک,
					     ctx->tind_buf);
	إذا (ctx->errcode) {
		ret |= بلوک_ERROR;
		أرجع ret;
	}

	بلوک_nr = (blk_t *) ctx->tind_buf;
	جابه_جایی = 0;
	إذا (ctx->flags & بلوک_FLAG_APPEND) {
		لكل (i = 0; i < حد; i++, بلوک_nr++) {
			flags = بلوک_iterate_dind(بلوک_nr,
						   *tind_بلوک,
						   جابه_جایی, ctx);
			تغییریافته |= flags;
			إذا (flags & (بلوک_ABORT | بلوک_ERROR)) {
				ret |= flags & (بلوک_ABORT | بلوک_ERROR);
				اكسر;
			}
			جابه_جایی += حجم(blk_t);
		}
	} وإلا {
		لكل (i = 0; i < حد; i++, بلوک_nr++) {
			إذا (*بلوک_nr == 0) {
				ctx->bcount += حد*حد;
				استمر;
			}
			flags = بلوک_iterate_dind(بلوک_nr,
						   *tind_بلوک,
						   جابه_جایی, ctx);
			تغییریافته |= flags;
			إذا (flags & (بلوک_ABORT | بلوک_ERROR)) {
				ret |= flags & (بلوک_ABORT | بلوک_ERROR);
				اكسر;
			}
			جابه_جایی += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, تغییریافته);
	إذا (تغییریافته & بلوک_تغییریافته) {
		ctx->errcode = ext2fs_write_ind_بلوک(ctx->fs, *tind_بلوک,
						      ctx->tind_buf);
		إذا (ctx->errcode)
			ret |= بلوک_ERROR | بلوک_ABORT;
	}
	إذا ((ctx->flags & بلوک_FLAG_عمق_TRAVERSE) &&
	    !(ctx->flags & بلوک_FLAG_داده_ONLY) &&
	    !(ret & بلوک_ABORT)) {
		blk64 = *tind_بلوک;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلوک_شمار_TIND, ref_بلوک,
				    ref_جابه_جایی, ctx->priv_داده);
		*tind_بلوک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	أرجع ret;
}

errcode_t ext2fs_بلوک_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *بلوک_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحيح		ref_جابه_جایی,
					    فراغ	*priv_داده),
				فراغ *priv_داده)
{
	صحيح	i;
	صحيح	r, ret = 0;
	بنية ext2_inode inode;
	errcode_t	retval;
	بنية بلوک_context ctx;
	صحيح	حد;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	إذا (ctx.errcode)
		أرجع ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	إذا (inode.i_flags & EXT4_INLINE_داده_FL)
		أرجع EXT2_ET_INLINE_داده_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	إذا (flags & بلوک_FLAG_NO_LARGE) {
		إذا (!LINUX_S_ISDIR(inode.i_نما) &&
		    (inode.i_اندازه_بالا != 0))
			أرجع EXT2_ET_FILE_TOO_BIG;
	}

	حد = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_داده = priv_داده;
	ctx.flags = flags;
	ctx.bcount = 0;
	إذا (بلوک_buf) {
		ctx.ind_buf = بلوک_buf;
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
	    !(flags & بلوک_FLAG_داده_ONLY)) {
		إذا (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   بلوک_شمار_TRANSLATOR,
					   0, 0, priv_داده);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			إذا (ret & بلوک_ABORT)
				اذهب abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	إذا (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		بنية ext2fs_extent	extent, بعدی;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		صحيح			op = EXT2_EXTENT_ریشه;
		صحيح			uninit;
		بیعلامت صحيح		j;

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
				إذا (ctx.errcode != EXT2_ET_EXTENT_NO_بعدی)
					اكسر;
				ctx.errcode = 0;
				إذا (!(flags & بلوک_FLAG_APPEND))
					اكسر;
			بعدی_بلوک_مجموعه:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_داده);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_انجامشده);
				إذا (r & بلوک_تغییریافته) {
					ctx.errcode =
						ext2fs_extent_مجموعه_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					إذا (ctx.errcode || (ret & بلوک_ABORT))
						اكسر;
					إذا (blk)
						اذهب بعدی_بلوک_مجموعه;
				}
				اكسر;
			}

			op = EXT2_EXTENT_بعدی;
			blk = extent.e_pblk;
			إذا (!(extent.e_flags & EXT2_EXTENT_FLAGS_برگ)) {
				إذا (ctx.flags & بلوک_FLAG_داده_ONLY)
					استمر;
				إذا ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_دومی_بازدید) &&
				     !(ctx.flags & بلوک_FLAG_عمق_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_دومی_بازدید) &&
				     (ctx.flags & بلوک_FLAG_عمق_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_داده);
					إذا (ret & بلوک_تغییریافته) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						إذا (ctx.errcode)
							اكسر;
					}
					إذا (ret & بلوک_ABORT)
						اكسر;
				}
				استمر;
			}
			uninit = 0;
			إذا (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_مجموعه_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &بعدی);

#إذا 0
			چاپ_کن("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#پایان_اگر
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
						0, 0, priv_داده);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_انجامشده);
				إذا (r & بلوک_تغییریافته) {
					ctx.errcode =
						ext2fs_extent_مجموعه_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					إذا (ctx.errcode)
						اذهب extent_انجامشده;
				}
				إذا (ret & بلوک_ABORT)
					اذهب extent_انجامشده;
			}
			إذا (retval == 0) {
				extent = بعدی;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_انجامشده:
		ext2fs_extent_free(handle);
		ret |= بلوک_ERROR; /* ctx.errcode is always valid here */
		اذهب errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	لكل (i = 0; i < EXT2_NDIR_بلوکها ; i++, ctx.bcount++) {
		إذا (inode.i_بلوک[i] || (flags & بلوک_FLAG_APPEND)) {
			blk64 = inode.i_بلوک[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_داده);
			inode.i_بلوک[i] = (blk_t) blk64;
			إذا (ret & بلوک_ABORT)
				اذهب abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	إذا (inode.i_بلوک[EXT2_IND_بلوک] || (flags & بلوک_FLAG_APPEND)) {
		ret |= بلوک_iterate_ind(&inode.i_بلوک[EXT2_IND_بلوک], 
					 0, EXT2_IND_بلوک, &ctx);
		إذا (ret & بلوک_ABORT)
			اذهب abort_exit;
	} وإلا
		ctx.bcount += حد;
	إذا (inode.i_بلوک[EXT2_DIND_بلوک] || (flags & بلوک_FLAG_APPEND)) {
		ret |= بلوک_iterate_dind(&inode.i_بلوک[EXT2_DIND_بلوک],
					  0, EXT2_DIND_بلوک, &ctx);
		إذا (ret & بلوک_ABORT)
			اذهب abort_exit;
	} وإلا
		ctx.bcount += حد * حد;
	إذا (inode.i_بلوک[EXT2_TIND_بلوک] || (flags & بلوک_FLAG_APPEND)) {
		ret |= بلوک_iterate_tind(&inode.i_بلوک[EXT2_TIND_بلوک],
					  0, EXT2_TIND_بلوک, &ctx);
		إذا (ret & بلوک_ABORT)
			اذهب abort_exit;
	}

abort_exit:
	إذا (ret & بلوک_تغییریافته) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		إذا (retval) {
			ret |= بلوک_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	إذا (!بلوک_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	أرجع (ret & بلوک_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

بنية xlate64 {
	صحيح (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    صحيح		ref_جابه_جایی,
		    فراغ	*priv_داده);
	فراغ *real_خصوصی;
};

ساكن صحيح xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			صحيح ref_جابه_جایی, فراغ *priv_داده)
{
	بنية xlate64 *xl = (بنية xlate64 *) priv_داده;
	صحيح		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_جابه_جایی,
			     xl->real_خصوصی);
	*blocknr = block32;
	أرجع ret;
}

errcode_t ext2fs_بلوک_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *بلوک_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحيح		ref_جابه_جایی,
					    فراغ	*priv_داده),
				فراغ *priv_داده)
{
	بنية xlate64 xl;

	xl.real_خصوصی = priv_داده;
	xl.func = func;

	أرجع ext2fs_بلوک_iterate3(fs, ino, flags, بلوک_buf, 
				     xlate64_func, &xl);
}


بنية xlate {
	صحيح (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    صحيح		bcount,
		    فراغ	*priv_داده);
	فراغ *real_خصوصی;
};

#اگر_تعریف_شده __TURBOC__
 #رهنمود argsused
#پایان_اگر
ساكن صحيح xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_بلوک EXT2FS_ATTR((استفادهنشده)),
		      صحيح ref_جابه_جایی EXT2FS_ATTR((استفادهنشده)),
		      فراغ *priv_داده)
{
	بنية xlate *xl = (بنية xlate *) priv_داده;

	أرجع (*xl->func)(fs, blocknr, (صحيح) blockcnt, xl->real_خصوصی);
}

errcode_t ext2fs_بلوک_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       صحيح	flags,
			       حرف *بلوک_buf,
			       صحيح (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   صحيح	blockcnt,
					   فراغ	*priv_داده),
			       فراغ *priv_داده)
{
	بنية xlate xl;

	xl.real_خصوصی = priv_داده;
	xl.func = func;

	أرجع ext2fs_بلوک_iterate2(fs, ino, بلوک_FLAG_NO_LARGE | flags,
				     بلوک_buf, xlate_func, &xl);
}

