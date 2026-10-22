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
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية بلاک_context {
	ext2_filsys	fs;
	صحيح (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    صحيح		ref_دننه_واټن,
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

#تعریف_کړه check_for_ro_violation_return(ctx, ret)				\
	افعل {								\
		إذا (((ctx)->flags & بلاک_FLAG_READ_ONLY) &&		\
		    ((ret) & بلاک_بدل_شوی)) {				\
			(ctx)->errcode = EXT2_ET_RO_بلاک_ITERATE;	\
			ret |= بلاک_ABORT | بلاک_ERROR;		\
			أرجع ret;					\
		}							\
	} طالما (0)

#تعریف_کړه check_for_ro_violation_goto(ctx, ret, label)			\
	افعل {								\
		إذا (((ctx)->flags & بلاک_FLAG_READ_ONLY) &&		\
		    ((ret) & بلاک_بدل_شوی)) {				\
			(ctx)->errcode = EXT2_ET_RO_بلاک_ITERATE;	\
			ret |= بلاک_ABORT | بلاک_ERROR;		\
			اذهب label;					\
		}							\
	} طالما (0)

ساكن صحيح بلاک_iterate_ind(blk_t *ind_بلاک, blk_t ref_بلاک,
			     صحيح ref_دننه_واټن, بنية بلاک_context *ctx)
{
	صحيح	ret = 0, بدل_شوی = 0;
	صحيح	i, flags, حد, دننه_واټن;
	blk_t	*بلاک_nr;
	blk64_t	blk64;

	حد = ctx->fs->blocksize >> 2;
	إذا (!(ctx->flags & بلاک_FLAG_ژوروالی_TRAVERSE) &&
	    !(ctx->flags & بلاک_FLAG_معلومات_ONLY)) {
		blk64 = *ind_بلاک;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلاک_شمېر_IND, ref_بلاک,
				   ref_دننه_واټن, ctx->priv_معلومات);
		*ind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	إذا (!*ind_بلاک || (ret & بلاک_ABORT)) {
		ctx->bcount += حد;
		أرجع ret;
	}
	إذا (*ind_بلاک >= ext2fs_بلاکونه_شمېر(ctx->fs->super) ||
	    *ind_بلاک < ctx->fs->super->s_لومړی_معلومات_بلاک) {
		ctx->errcode = EXT2_ET_BAD_IND_بلاک;
		ret |= بلاک_ERROR;
		أرجع ret;
	}
	ctx->errcode = ext2fs_read_ind_بلاک(ctx->fs, *ind_بلاک,
					     ctx->ind_buf);
	إذا (ctx->errcode) {
		ret |= بلاک_ERROR;
		أرجع ret;
	}

	بلاک_nr = (blk_t *) ctx->ind_buf;
	دننه_واټن = 0;
	إذا (ctx->flags & بلاک_FLAG_APPEND) {
		لكل (i = 0; i < حد; i++, ctx->bcount++, بلاک_nr++) {
			blk64 = *بلاک_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_بلاک, دننه_واټن,
					     ctx->priv_معلومات);
			*بلاک_nr = blk64;
			بدل_شوی	|= flags;
			إذا (flags & بلاک_ABORT) {
				ret |= بلاک_ABORT;
				اكسر;
			}
			دننه_واټن += حجم(blk_t);
		}
	} وإلا {
		لكل (i = 0; i < حد; i++, ctx->bcount++, بلاک_nr++) {
			إذا (*بلاک_nr == 0)
				اذهب skip_sparse;
			blk64 = *بلاک_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_بلاک, دننه_واټن,
					     ctx->priv_معلومات);
			*بلاک_nr = blk64;
			بدل_شوی	|= flags;
			إذا (flags & بلاک_ABORT) {
				ret |= بلاک_ABORT;
				اكسر;
			}
		skip_sparse:
			دننه_واټن += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, بدل_شوی);
	إذا (بدل_شوی & بلاک_بدل_شوی) {
		ctx->errcode = ext2fs_write_ind_بلاک(ctx->fs, *ind_بلاک,
						      ctx->ind_buf);
		إذا (ctx->errcode)
			ret |= بلاک_ERROR | بلاک_ABORT;
	}
	إذا ((ctx->flags & بلاک_FLAG_ژوروالی_TRAVERSE) &&
	    !(ctx->flags & بلاک_FLAG_معلومات_ONLY) &&
	    !(ret & بلاک_ABORT)) {
		blk64 = *ind_بلاک;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلاک_شمېر_IND, ref_بلاک,
				    ref_دننه_واټن, ctx->priv_معلومات);
		*ind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	أرجع ret;
}

ساكن صحيح بلاک_iterate_dind(blk_t *dind_بلاک, blk_t ref_بلاک,
			      صحيح ref_دننه_واټن, بنية بلاک_context *ctx)
{
	صحيح	ret = 0, بدل_شوی = 0;
	صحيح	i, flags, حد, دننه_واټن;
	blk_t	*بلاک_nr;
	blk64_t	blk64;

	حد = ctx->fs->blocksize >> 2;
	إذا (!(ctx->flags & (بلاک_FLAG_ژوروالی_TRAVERSE |
			    بلاک_FLAG_معلومات_ONLY))) {
		blk64 = *dind_بلاک;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلاک_شمېر_DIND, ref_بلاک,
				   ref_دننه_واټن, ctx->priv_معلومات);
		*dind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	إذا (!*dind_بلاک || (ret & بلاک_ABORT)) {
		ctx->bcount += حد*حد;
		أرجع ret;
	}
	إذا (*dind_بلاک >= ext2fs_بلاکونه_شمېر(ctx->fs->super) ||
	    *dind_بلاک < ctx->fs->super->s_لومړی_معلومات_بلاک) {
		ctx->errcode = EXT2_ET_BAD_DIND_بلاک;
		ret |= بلاک_ERROR;
		أرجع ret;
	}
	ctx->errcode = ext2fs_read_ind_بلاک(ctx->fs, *dind_بلاک,
					     ctx->dind_buf);
	إذا (ctx->errcode) {
		ret |= بلاک_ERROR;
		أرجع ret;
	}

	بلاک_nr = (blk_t *) ctx->dind_buf;
	دننه_واټن = 0;
	إذا (ctx->flags & بلاک_FLAG_APPEND) {
		لكل (i = 0; i < حد; i++, بلاک_nr++) {
			flags = بلاک_iterate_ind(بلاک_nr,
						  *dind_بلاک, دننه_واټن,
						  ctx);
			بدل_شوی |= flags;
			إذا (flags & (بلاک_ABORT | بلاک_ERROR)) {
				ret |= flags & (بلاک_ABORT | بلاک_ERROR);
				اكسر;
			}
			دننه_واټن += حجم(blk_t);
		}
	} وإلا {
		لكل (i = 0; i < حد; i++, بلاک_nr++) {
			إذا (*بلاک_nr == 0) {
				ctx->bcount += حد;
				استمر;
			}
			flags = بلاک_iterate_ind(بلاک_nr,
						  *dind_بلاک, دننه_واټن,
						  ctx);
			بدل_شوی |= flags;
			إذا (flags & (بلاک_ABORT | بلاک_ERROR)) {
				ret |= flags & (بلاک_ABORT | بلاک_ERROR);
				اكسر;
			}
			دننه_واټن += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, بدل_شوی);
	إذا (بدل_شوی & بلاک_بدل_شوی) {
		ctx->errcode = ext2fs_write_ind_بلاک(ctx->fs, *dind_بلاک,
						      ctx->dind_buf);
		إذا (ctx->errcode)
			ret |= بلاک_ERROR | بلاک_ABORT;
	}
	إذا ((ctx->flags & بلاک_FLAG_ژوروالی_TRAVERSE) &&
	    !(ctx->flags & بلاک_FLAG_معلومات_ONLY) &&
	    !(ret & بلاک_ABORT)) {
		blk64 = *dind_بلاک;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلاک_شمېر_DIND, ref_بلاک,
				    ref_دننه_واټن, ctx->priv_معلومات);
		*dind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	أرجع ret;
}

ساكن صحيح بلاک_iterate_tind(blk_t *tind_بلاک, blk_t ref_بلاک,
			      صحيح ref_دننه_واټن, بنية بلاک_context *ctx)
{
	صحيح	ret = 0, بدل_شوی = 0;
	صحيح	i, flags, حد, دننه_واټن;
	blk_t	*بلاک_nr;
	blk64_t	blk64;

	حد = ctx->fs->blocksize >> 2;
	إذا (!(ctx->flags & (بلاک_FLAG_ژوروالی_TRAVERSE |
			    بلاک_FLAG_معلومات_ONLY))) {
		blk64 = *tind_بلاک;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلاک_شمېر_TIND, ref_بلاک,
				   ref_دننه_واټن, ctx->priv_معلومات);
		*tind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	إذا (!*tind_بلاک || (ret & بلاک_ABORT)) {
		ctx->bcount += ((بینښه اوږد اوږد) حد)*حد*حد;
		أرجع ret;
	}
	إذا (*tind_بلاک >= ext2fs_بلاکونه_شمېر(ctx->fs->super) ||
	    *tind_بلاک < ctx->fs->super->s_لومړی_معلومات_بلاک) {
		ctx->errcode = EXT2_ET_BAD_TIND_بلاک;
		ret |= بلاک_ERROR;
		أرجع ret;
	}
	ctx->errcode = ext2fs_read_ind_بلاک(ctx->fs, *tind_بلاک,
					     ctx->tind_buf);
	إذا (ctx->errcode) {
		ret |= بلاک_ERROR;
		أرجع ret;
	}

	بلاک_nr = (blk_t *) ctx->tind_buf;
	دننه_واټن = 0;
	إذا (ctx->flags & بلاک_FLAG_APPEND) {
		لكل (i = 0; i < حد; i++, بلاک_nr++) {
			flags = بلاک_iterate_dind(بلاک_nr,
						   *tind_بلاک,
						   دننه_واټن, ctx);
			بدل_شوی |= flags;
			إذا (flags & (بلاک_ABORT | بلاک_ERROR)) {
				ret |= flags & (بلاک_ABORT | بلاک_ERROR);
				اكسر;
			}
			دننه_واټن += حجم(blk_t);
		}
	} وإلا {
		لكل (i = 0; i < حد; i++, بلاک_nr++) {
			إذا (*بلاک_nr == 0) {
				ctx->bcount += حد*حد;
				استمر;
			}
			flags = بلاک_iterate_dind(بلاک_nr,
						   *tind_بلاک,
						   دننه_واټن, ctx);
			بدل_شوی |= flags;
			إذا (flags & (بلاک_ABORT | بلاک_ERROR)) {
				ret |= flags & (بلاک_ABORT | بلاک_ERROR);
				اكسر;
			}
			دننه_واټن += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, بدل_شوی);
	إذا (بدل_شوی & بلاک_بدل_شوی) {
		ctx->errcode = ext2fs_write_ind_بلاک(ctx->fs, *tind_بلاک,
						      ctx->tind_buf);
		إذا (ctx->errcode)
			ret |= بلاک_ERROR | بلاک_ABORT;
	}
	إذا ((ctx->flags & بلاک_FLAG_ژوروالی_TRAVERSE) &&
	    !(ctx->flags & بلاک_FLAG_معلومات_ONLY) &&
	    !(ret & بلاک_ABORT)) {
		blk64 = *tind_بلاک;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلاک_شمېر_TIND, ref_بلاک,
				    ref_دننه_واټن, ctx->priv_معلومات);
		*tind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	أرجع ret;
}

errcode_t ext2fs_بلاک_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *بلاک_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحيح		ref_دننه_واټن,
					    فراغ	*priv_معلومات),
				فراغ *priv_معلومات)
{
	صحيح	i;
	صحيح	r, ret = 0;
	بنية ext2_inode inode;
	errcode_t	retval;
	بنية بلاک_context ctx;
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
	إذا (inode.i_flags & EXT4_INLINE_معلومات_FL)
		أرجع EXT2_ET_INLINE_معلومات_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	إذا (flags & بلاک_FLAG_NO_LARGE) {
		إذا (!LINUX_S_ISDIR(inode.i_ډېرپېښ) &&
		    (inode.i_اندازه_لوړ != 0))
			أرجع EXT2_ET_FILE_TOO_BIG;
	}

	حد = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_معلومات = priv_معلومات;
	ctx.flags = flags;
	ctx.bcount = 0;
	إذا (بلاک_buf) {
		ctx.ind_buf = بلاک_buf;
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
	    !(flags & بلاک_FLAG_معلومات_ONLY)) {
		إذا (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   بلاک_شمېر_TRANSLATOR,
					   0, 0, priv_معلومات);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			إذا (ret & بلاک_ABORT)
				اذهب abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	إذا (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		بنية ext2fs_extent	extent, بل;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		صحيح			op = EXT2_EXTENT_ریښه;
		صحيح			uninit;
		بینښه صحيح		j;

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
				إذا (ctx.errcode != EXT2_ET_EXTENT_NO_بل)
					اكسر;
				ctx.errcode = 0;
				إذا (!(flags & بلاک_FLAG_APPEND))
					اكسر;
			بل_بلاک_سېټ:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_معلومات);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_بشپړ);
				إذا (r & بلاک_بدل_شوی) {
					ctx.errcode =
						ext2fs_extent_سېټ_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					إذا (ctx.errcode || (ret & بلاک_ABORT))
						اكسر;
					إذا (blk)
						اذهب بل_بلاک_سېټ;
				}
				اكسر;
			}

			op = EXT2_EXTENT_بل;
			blk = extent.e_pblk;
			إذا (!(extent.e_flags & EXT2_EXTENT_FLAGS_ونې_پاڼه)) {
				إذا (ctx.flags & بلاک_FLAG_معلومات_ONLY)
					استمر;
				إذا ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_دوهم_لیدنه) &&
				     !(ctx.flags & بلاک_FLAG_ژوروالی_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_دوهم_لیدنه) &&
				     (ctx.flags & بلاک_FLAG_ژوروالی_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_معلومات);
					إذا (ret & بلاک_بدل_شوی) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						إذا (ctx.errcode)
							اكسر;
					}
					إذا (ret & بلاک_ABORT)
						اكسر;
				}
				استمر;
			}
			uninit = 0;
			إذا (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_سېټ_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &بل);

#إذا 0
			چاپ_کړه("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#د_که_پای
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
							    extent_بشپړ);
				إذا (r & بلاک_بدل_شوی) {
					ctx.errcode =
						ext2fs_extent_سېټ_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					إذا (ctx.errcode)
						اذهب extent_بشپړ;
				}
				إذا (ret & بلاک_ABORT)
					اذهب extent_بشپړ;
			}
			إذا (retval == 0) {
				extent = بل;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_بشپړ:
		ext2fs_extent_free(handle);
		ret |= بلاک_ERROR; /* ctx.errcode is always valid here */
		اذهب errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	لكل (i = 0; i < EXT2_NDIR_بلاکونه ; i++, ctx.bcount++) {
		إذا (inode.i_بلاک[i] || (flags & بلاک_FLAG_APPEND)) {
			blk64 = inode.i_بلاک[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_معلومات);
			inode.i_بلاک[i] = (blk_t) blk64;
			إذا (ret & بلاک_ABORT)
				اذهب abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	إذا (inode.i_بلاک[EXT2_IND_بلاک] || (flags & بلاک_FLAG_APPEND)) {
		ret |= بلاک_iterate_ind(&inode.i_بلاک[EXT2_IND_بلاک], 
					 0, EXT2_IND_بلاک, &ctx);
		إذا (ret & بلاک_ABORT)
			اذهب abort_exit;
	} وإلا
		ctx.bcount += حد;
	إذا (inode.i_بلاک[EXT2_DIND_بلاک] || (flags & بلاک_FLAG_APPEND)) {
		ret |= بلاک_iterate_dind(&inode.i_بلاک[EXT2_DIND_بلاک],
					  0, EXT2_DIND_بلاک, &ctx);
		إذا (ret & بلاک_ABORT)
			اذهب abort_exit;
	} وإلا
		ctx.bcount += حد * حد;
	إذا (inode.i_بلاک[EXT2_TIND_بلاک] || (flags & بلاک_FLAG_APPEND)) {
		ret |= بلاک_iterate_tind(&inode.i_بلاک[EXT2_TIND_بلاک],
					  0, EXT2_TIND_بلاک, &ctx);
		إذا (ret & بلاک_ABORT)
			اذهب abort_exit;
	}

abort_exit:
	إذا (ret & بلاک_بدل_شوی) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		إذا (retval) {
			ret |= بلاک_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	إذا (!بلاک_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	أرجع (ret & بلاک_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

بنية xlate64 {
	صحيح (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    صحيح		ref_دننه_واټن,
		    فراغ	*priv_معلومات);
	فراغ *real_شخصي;
};

ساكن صحيح xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			صحيح ref_دننه_واټن, فراغ *priv_معلومات)
{
	بنية xlate64 *xl = (بنية xlate64 *) priv_معلومات;
	صحيح		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_دننه_واټن,
			     xl->real_شخصي);
	*blocknr = block32;
	أرجع ret;
}

errcode_t ext2fs_بلاک_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *بلاک_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحيح		ref_دننه_واټن,
					    فراغ	*priv_معلومات),
				فراغ *priv_معلومات)
{
	بنية xlate64 xl;

	xl.real_شخصي = priv_معلومات;
	xl.func = func;

	أرجع ext2fs_بلاک_iterate3(fs, ino, flags, بلاک_buf, 
				     xlate64_func, &xl);
}


بنية xlate {
	صحيح (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    صحيح		bcount,
		    فراغ	*priv_معلومات);
	فراغ *real_شخصي;
};

#که_تعریف_شوی __TURBOC__
 #لارښوونه argsused
#د_که_پای
ساكن صحيح xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_بلاک EXT2FS_ATTR((ناکارول_شوی)),
		      صحيح ref_دننه_واټن EXT2FS_ATTR((ناکارول_شوی)),
		      فراغ *priv_معلومات)
{
	بنية xlate *xl = (بنية xlate *) priv_معلومات;

	أرجع (*xl->func)(fs, blocknr, (صحيح) blockcnt, xl->real_شخصي);
}

errcode_t ext2fs_بلاک_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       صحيح	flags,
			       حرف *بلاک_buf,
			       صحيح (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   صحيح	blockcnt,
					   فراغ	*priv_معلومات),
			       فراغ *priv_معلومات)
{
	بنية xlate xl;

	xl.real_شخصي = priv_معلومات;
	xl.func = func;

	أرجع ext2fs_بلاک_iterate2(fs, ino, بلاک_FLAG_NO_LARGE | flags,
				     بلاک_buf, xlate_func, &xl);
}

