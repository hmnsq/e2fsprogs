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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

నిర్మాణం ఖండం_context {
	ext2_filsys	fs;
	పూర్ణాంకం (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    పూర్ణాంకం		ref_స్థానభ్రంశం,
		    శూన్య	*priv_దత్తాంశం);
	e2_blkcnt_t	bcount;
	పూర్ణాంకం		bsize;
	పూర్ణాంకం		flags;
	errcode_t	errcode;
	అక్షరం	*ind_buf;
	అక్షరం	*dind_buf;
	అక్షరం	*tind_buf;
	శూన్య	*priv_దత్తాంశం;
};

#నిర్వచించు check_for_ro_violation_return(ctx, ret)				\
	చేయి {								\
		ఒకవేళ (((ctx)->flags & ఖండం_FLAG_READ_ONLY) &&		\
		    ((ret) & ఖండం_మారింది)) {				\
			(ctx)->errcode = EXT2_ET_RO_ఖండం_ITERATE;	\
			ret |= ఖండం_ABORT | ఖండం_ERROR;		\
			తిరిగి ret;					\
		}							\
	} అయితే (0)

#నిర్వచించు check_for_ro_violation_goto(ctx, ret, label)			\
	చేయి {								\
		ఒకవేళ (((ctx)->flags & ఖండం_FLAG_READ_ONLY) &&		\
		    ((ret) & ఖండం_మారింది)) {				\
			(ctx)->errcode = EXT2_ET_RO_ఖండం_ITERATE;	\
			ret |= ఖండం_ABORT | ఖండం_ERROR;		\
			వెళ్ళు label;					\
		}							\
	} అయితే (0)

స్థిర పూర్ణాంకం ఖండం_iterate_ind(blk_t *ind_ఖండం, blk_t ref_ఖండం,
			     పూర్ణాంకం ref_స్థానభ్రంశం, నిర్మాణం ఖండం_context *ctx)
{
	పూర్ణాంకం	ret = 0, మారింది = 0;
	పూర్ణాంకం	i, flags, హద్దు, స్థానభ్రంశం;
	blk_t	*ఖండం_nr;
	blk64_t	blk64;

	హద్దు = ctx->fs->blocksize >> 2;
	ఒకవేళ (!(ctx->flags & ఖండం_FLAG_లోతు_TRAVERSE) &&
	    !(ctx->flags & ఖండం_FLAG_దత్తాంశం_ONLY)) {
		blk64 = *ind_ఖండం;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   ఖండం_లెక్క_IND, ref_ఖండం,
				   ref_స్థానభ్రంశం, ctx->priv_దత్తాంశం);
		*ind_ఖండం = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ఒకవేళ (!*ind_ఖండం || (ret & ఖండం_ABORT)) {
		ctx->bcount += హద్దు;
		తిరిగి ret;
	}
	ఒకవేళ (*ind_ఖండం >= ext2fs_ఖండాలు_లెక్క(ctx->fs->super) ||
	    *ind_ఖండం < ctx->fs->super->s_మొదటి_దత్తాంశం_ఖండం) {
		ctx->errcode = EXT2_ET_BAD_IND_ఖండం;
		ret |= ఖండం_ERROR;
		తిరిగి ret;
	}
	ctx->errcode = ext2fs_read_ind_ఖండం(ctx->fs, *ind_ఖండం,
					     ctx->ind_buf);
	ఒకవేళ (ctx->errcode) {
		ret |= ఖండం_ERROR;
		తిరిగి ret;
	}

	ఖండం_nr = (blk_t *) ctx->ind_buf;
	స్థానభ్రంశం = 0;
	ఒకవేళ (ctx->flags & ఖండం_FLAG_APPEND) {
		కొరకు (i = 0; i < హద్దు; i++, ctx->bcount++, ఖండం_nr++) {
			blk64 = *ఖండం_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_ఖండం, స్థానభ్రంశం,
					     ctx->priv_దత్తాంశం);
			*ఖండం_nr = blk64;
			మారింది	|= flags;
			ఒకవేళ (flags & ఖండం_ABORT) {
				ret |= ఖండం_ABORT;
				ఆపు;
			}
			స్థానభ్రంశం += పరిమాణం(blk_t);
		}
	} లేకపోతే {
		కొరకు (i = 0; i < హద్దు; i++, ctx->bcount++, ఖండం_nr++) {
			ఒకవేళ (*ఖండం_nr == 0)
				వెళ్ళు skip_sparse;
			blk64 = *ఖండం_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_ఖండం, స్థానభ్రంశం,
					     ctx->priv_దత్తాంశం);
			*ఖండం_nr = blk64;
			మారింది	|= flags;
			ఒకవేళ (flags & ఖండం_ABORT) {
				ret |= ఖండం_ABORT;
				ఆపు;
			}
		skip_sparse:
			స్థానభ్రంశం += పరిమాణం(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, మారింది);
	ఒకవేళ (మారింది & ఖండం_మారింది) {
		ctx->errcode = ext2fs_write_ind_ఖండం(ctx->fs, *ind_ఖండం,
						      ctx->ind_buf);
		ఒకవేళ (ctx->errcode)
			ret |= ఖండం_ERROR | ఖండం_ABORT;
	}
	ఒకవేళ ((ctx->flags & ఖండం_FLAG_లోతు_TRAVERSE) &&
	    !(ctx->flags & ఖండం_FLAG_దత్తాంశం_ONLY) &&
	    !(ret & ఖండం_ABORT)) {
		blk64 = *ind_ఖండం;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    ఖండం_లెక్క_IND, ref_ఖండం,
				    ref_స్థానభ్రంశం, ctx->priv_దత్తాంశం);
		*ind_ఖండం = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం ఖండం_iterate_dind(blk_t *dind_ఖండం, blk_t ref_ఖండం,
			      పూర్ణాంకం ref_స్థానభ్రంశం, నిర్మాణం ఖండం_context *ctx)
{
	పూర్ణాంకం	ret = 0, మారింది = 0;
	పూర్ణాంకం	i, flags, హద్దు, స్థానభ్రంశం;
	blk_t	*ఖండం_nr;
	blk64_t	blk64;

	హద్దు = ctx->fs->blocksize >> 2;
	ఒకవేళ (!(ctx->flags & (ఖండం_FLAG_లోతు_TRAVERSE |
			    ఖండం_FLAG_దత్తాంశం_ONLY))) {
		blk64 = *dind_ఖండం;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   ఖండం_లెక్క_DIND, ref_ఖండం,
				   ref_స్థానభ్రంశం, ctx->priv_దత్తాంశం);
		*dind_ఖండం = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ఒకవేళ (!*dind_ఖండం || (ret & ఖండం_ABORT)) {
		ctx->bcount += హద్దు*హద్దు;
		తిరిగి ret;
	}
	ఒకవేళ (*dind_ఖండం >= ext2fs_ఖండాలు_లెక్క(ctx->fs->super) ||
	    *dind_ఖండం < ctx->fs->super->s_మొదటి_దత్తాంశం_ఖండం) {
		ctx->errcode = EXT2_ET_BAD_DIND_ఖండం;
		ret |= ఖండం_ERROR;
		తిరిగి ret;
	}
	ctx->errcode = ext2fs_read_ind_ఖండం(ctx->fs, *dind_ఖండం,
					     ctx->dind_buf);
	ఒకవేళ (ctx->errcode) {
		ret |= ఖండం_ERROR;
		తిరిగి ret;
	}

	ఖండం_nr = (blk_t *) ctx->dind_buf;
	స్థానభ్రంశం = 0;
	ఒకవేళ (ctx->flags & ఖండం_FLAG_APPEND) {
		కొరకు (i = 0; i < హద్దు; i++, ఖండం_nr++) {
			flags = ఖండం_iterate_ind(ఖండం_nr,
						  *dind_ఖండం, స్థానభ్రంశం,
						  ctx);
			మారింది |= flags;
			ఒకవేళ (flags & (ఖండం_ABORT | ఖండం_ERROR)) {
				ret |= flags & (ఖండం_ABORT | ఖండం_ERROR);
				ఆపు;
			}
			స్థానభ్రంశం += పరిమాణం(blk_t);
		}
	} లేకపోతే {
		కొరకు (i = 0; i < హద్దు; i++, ఖండం_nr++) {
			ఒకవేళ (*ఖండం_nr == 0) {
				ctx->bcount += హద్దు;
				కొనసాగు;
			}
			flags = ఖండం_iterate_ind(ఖండం_nr,
						  *dind_ఖండం, స్థానభ్రంశం,
						  ctx);
			మారింది |= flags;
			ఒకవేళ (flags & (ఖండం_ABORT | ఖండం_ERROR)) {
				ret |= flags & (ఖండం_ABORT | ఖండం_ERROR);
				ఆపు;
			}
			స్థానభ్రంశం += పరిమాణం(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, మారింది);
	ఒకవేళ (మారింది & ఖండం_మారింది) {
		ctx->errcode = ext2fs_write_ind_ఖండం(ctx->fs, *dind_ఖండం,
						      ctx->dind_buf);
		ఒకవేళ (ctx->errcode)
			ret |= ఖండం_ERROR | ఖండం_ABORT;
	}
	ఒకవేళ ((ctx->flags & ఖండం_FLAG_లోతు_TRAVERSE) &&
	    !(ctx->flags & ఖండం_FLAG_దత్తాంశం_ONLY) &&
	    !(ret & ఖండం_ABORT)) {
		blk64 = *dind_ఖండం;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    ఖండం_లెక్క_DIND, ref_ఖండం,
				    ref_స్థానభ్రంశం, ctx->priv_దత్తాంశం);
		*dind_ఖండం = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం ఖండం_iterate_tind(blk_t *tind_ఖండం, blk_t ref_ఖండం,
			      పూర్ణాంకం ref_స్థానభ్రంశం, నిర్మాణం ఖండం_context *ctx)
{
	పూర్ణాంకం	ret = 0, మారింది = 0;
	పూర్ణాంకం	i, flags, హద్దు, స్థానభ్రంశం;
	blk_t	*ఖండం_nr;
	blk64_t	blk64;

	హద్దు = ctx->fs->blocksize >> 2;
	ఒకవేళ (!(ctx->flags & (ఖండం_FLAG_లోతు_TRAVERSE |
			    ఖండం_FLAG_దత్తాంశం_ONLY))) {
		blk64 = *tind_ఖండం;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   ఖండం_లెక్క_TIND, ref_ఖండం,
				   ref_స్థానభ్రంశం, ctx->priv_దత్తాంశం);
		*tind_ఖండం = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ఒకవేళ (!*tind_ఖండం || (ret & ఖండం_ABORT)) {
		ctx->bcount += ((గుర్తులేని దీర్ఘ దీర్ఘ) హద్దు)*హద్దు*హద్దు;
		తిరిగి ret;
	}
	ఒకవేళ (*tind_ఖండం >= ext2fs_ఖండాలు_లెక్క(ctx->fs->super) ||
	    *tind_ఖండం < ctx->fs->super->s_మొదటి_దత్తాంశం_ఖండం) {
		ctx->errcode = EXT2_ET_BAD_TIND_ఖండం;
		ret |= ఖండం_ERROR;
		తిరిగి ret;
	}
	ctx->errcode = ext2fs_read_ind_ఖండం(ctx->fs, *tind_ఖండం,
					     ctx->tind_buf);
	ఒకవేళ (ctx->errcode) {
		ret |= ఖండం_ERROR;
		తిరిగి ret;
	}

	ఖండం_nr = (blk_t *) ctx->tind_buf;
	స్థానభ్రంశం = 0;
	ఒకవేళ (ctx->flags & ఖండం_FLAG_APPEND) {
		కొరకు (i = 0; i < హద్దు; i++, ఖండం_nr++) {
			flags = ఖండం_iterate_dind(ఖండం_nr,
						   *tind_ఖండం,
						   స్థానభ్రంశం, ctx);
			మారింది |= flags;
			ఒకవేళ (flags & (ఖండం_ABORT | ఖండం_ERROR)) {
				ret |= flags & (ఖండం_ABORT | ఖండం_ERROR);
				ఆపు;
			}
			స్థానభ్రంశం += పరిమాణం(blk_t);
		}
	} లేకపోతే {
		కొరకు (i = 0; i < హద్దు; i++, ఖండం_nr++) {
			ఒకవేళ (*ఖండం_nr == 0) {
				ctx->bcount += హద్దు*హద్దు;
				కొనసాగు;
			}
			flags = ఖండం_iterate_dind(ఖండం_nr,
						   *tind_ఖండం,
						   స్థానభ్రంశం, ctx);
			మారింది |= flags;
			ఒకవేళ (flags & (ఖండం_ABORT | ఖండం_ERROR)) {
				ret |= flags & (ఖండం_ABORT | ఖండం_ERROR);
				ఆపు;
			}
			స్థానభ్రంశం += పరిమాణం(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, మారింది);
	ఒకవేళ (మారింది & ఖండం_మారింది) {
		ctx->errcode = ext2fs_write_ind_ఖండం(ctx->fs, *tind_ఖండం,
						      ctx->tind_buf);
		ఒకవేళ (ctx->errcode)
			ret |= ఖండం_ERROR | ఖండం_ABORT;
	}
	ఒకవేళ ((ctx->flags & ఖండం_FLAG_లోతు_TRAVERSE) &&
	    !(ctx->flags & ఖండం_FLAG_దత్తాంశం_ONLY) &&
	    !(ret & ఖండం_ABORT)) {
		blk64 = *tind_ఖండం;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    ఖండం_లెక్క_TIND, ref_ఖండం,
				    ref_స్థానభ్రంశం, ctx->priv_దత్తాంశం);
		*tind_ఖండం = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	తిరిగి ret;
}

errcode_t ext2fs_ఖండం_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				పూర్ణాంకం	flags,
				అక్షరం *ఖండం_buf,
				పూర్ణాంకం (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    పూర్ణాంకం		ref_స్థానభ్రంశం,
					    శూన్య	*priv_దత్తాంశం),
				శూన్య *priv_దత్తాంశం)
{
	పూర్ణాంకం	i;
	పూర్ణాంకం	r, ret = 0;
	నిర్మాణం ext2_inode inode;
	errcode_t	retval;
	నిర్మాణం ఖండం_context ctx;
	పూర్ణాంకం	హద్దు;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (ctx.errcode)
		తిరిగి ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	ఒకవేళ (inode.i_flags & EXT4_INLINE_దత్తాంశం_FL)
		తిరిగి EXT2_ET_INLINE_దత్తాంశం_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	ఒకవేళ (flags & ఖండం_FLAG_NO_LARGE) {
		ఒకవేళ (!LINUX_S_ISDIR(inode.i_బహుళకం) &&
		    (inode.i_కొలత_ఎగువ != 0))
			తిరిగి EXT2_ET_FILE_TOO_BIG;
	}

	హద్దు = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_దత్తాంశం = priv_దత్తాంశం;
	ctx.flags = flags;
	ctx.bcount = 0;
	ఒకవేళ (ఖండం_buf) {
		ctx.ind_buf = ఖండం_buf;
	} లేకపోతే {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	ఒకవేళ ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & ఖండం_FLAG_దత్తాంశం_ONLY)) {
		ఒకవేళ (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   ఖండం_లెక్క_TRANSLATOR,
					   0, 0, priv_దత్తాంశం);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			ఒకవేళ (ret & ఖండం_ABORT)
				వెళ్ళు abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	ఒకవేళ (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		నిర్మాణం ext2fs_extent	extent, తదుపరి;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		పూర్ణాంకం			op = EXT2_EXTENT_మూలం;
		పూర్ణాంకం			uninit;
		గుర్తులేని పూర్ణాంకం		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		ఒకవేళ (ctx.errcode)
			వెళ్ళు abort_exit;

		అయితే (1) {
			ఒకవేళ (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			లేకపోతే
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			ఒకవేళ (ctx.errcode) {
				ఒకవేళ (ctx.errcode != EXT2_ET_EXTENT_NO_తదుపరి)
					ఆపు;
				ctx.errcode = 0;
				ఒకవేళ (!(flags & ఖండం_FLAG_APPEND))
					ఆపు;
			తదుపరి_ఖండం_సమితి:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_దత్తాంశం);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_పూర్తి);
				ఒకవేళ (r & ఖండం_మారింది) {
					ctx.errcode =
						ext2fs_extent_సమితి_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					ఒకవేళ (ctx.errcode || (ret & ఖండం_ABORT))
						ఆపు;
					ఒకవేళ (blk)
						వెళ్ళు తదుపరి_ఖండం_సమితి;
				}
				ఆపు;
			}

			op = EXT2_EXTENT_తదుపరి;
			blk = extent.e_pblk;
			ఒకవేళ (!(extent.e_flags & EXT2_EXTENT_FLAGS_ఆకు)) {
				ఒకవేళ (ctx.flags & ఖండం_FLAG_దత్తాంశం_ONLY)
					కొనసాగు;
				ఒకవేళ ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_రెండవ_సందర్శించు) &&
				     !(ctx.flags & ఖండం_FLAG_లోతు_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_రెండవ_సందర్శించు) &&
				     (ctx.flags & ఖండం_FLAG_లోతు_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_దత్తాంశం);
					ఒకవేళ (ret & ఖండం_మారింది) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						ఒకవేళ (ctx.errcode)
							ఆపు;
					}
					ఒకవేళ (ret & ఖండం_ABORT)
						ఆపు;
				}
				కొనసాగు;
			}
			uninit = 0;
			ఒకవేళ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_సమితి_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &తదుపరి);

#ఒకవేళ 0
			రాయి("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#అయితే_ముగింపు
			ఒకవేళ (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				కొనసాగు;
			ఒకవేళ (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			కొరకు (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_దత్తాంశం);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_పూర్తి);
				ఒకవేళ (r & ఖండం_మారింది) {
					ctx.errcode =
						ext2fs_extent_సమితి_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					ఒకవేళ (ctx.errcode)
						వెళ్ళు extent_పూర్తి;
				}
				ఒకవేళ (ret & ఖండం_ABORT)
					వెళ్ళు extent_పూర్తి;
			}
			ఒకవేళ (retval == 0) {
				extent = తదుపరి;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_పూర్తి:
		ext2fs_extent_free(handle);
		ret |= ఖండం_ERROR; /* ctx.errcode is always valid here */
		వెళ్ళు errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	కొరకు (i = 0; i < EXT2_NDIR_ఖండాలు ; i++, ctx.bcount++) {
		ఒకవేళ (inode.i_ఖండం[i] || (flags & ఖండం_FLAG_APPEND)) {
			blk64 = inode.i_ఖండం[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_దత్తాంశం);
			inode.i_ఖండం[i] = (blk_t) blk64;
			ఒకవేళ (ret & ఖండం_ABORT)
				వెళ్ళు abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	ఒకవేళ (inode.i_ఖండం[EXT2_IND_ఖండం] || (flags & ఖండం_FLAG_APPEND)) {
		ret |= ఖండం_iterate_ind(&inode.i_ఖండం[EXT2_IND_ఖండం], 
					 0, EXT2_IND_ఖండం, &ctx);
		ఒకవేళ (ret & ఖండం_ABORT)
			వెళ్ళు abort_exit;
	} లేకపోతే
		ctx.bcount += హద్దు;
	ఒకవేళ (inode.i_ఖండం[EXT2_DIND_ఖండం] || (flags & ఖండం_FLAG_APPEND)) {
		ret |= ఖండం_iterate_dind(&inode.i_ఖండం[EXT2_DIND_ఖండం],
					  0, EXT2_DIND_ఖండం, &ctx);
		ఒకవేళ (ret & ఖండం_ABORT)
			వెళ్ళు abort_exit;
	} లేకపోతే
		ctx.bcount += హద్దు * హద్దు;
	ఒకవేళ (inode.i_ఖండం[EXT2_TIND_ఖండం] || (flags & ఖండం_FLAG_APPEND)) {
		ret |= ఖండం_iterate_tind(&inode.i_ఖండం[EXT2_TIND_ఖండం],
					  0, EXT2_TIND_ఖండం, &ctx);
		ఒకవేళ (ret & ఖండం_ABORT)
			వెళ్ళు abort_exit;
	}

abort_exit:
	ఒకవేళ (ret & ఖండం_మారింది) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		ఒకవేళ (retval) {
			ret |= ఖండం_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	ఒకవేళ (!ఖండం_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	తిరిగి (ret & ఖండం_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

నిర్మాణం xlate64 {
	పూర్ణాంకం (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    పూర్ణాంకం		ref_స్థానభ్రంశం,
		    శూన్య	*priv_దత్తాంశం);
	శూన్య *real_వ్యక్తిగత;
};

స్థిర పూర్ణాంకం xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			పూర్ణాంకం ref_స్థానభ్రంశం, శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం xlate64 *xl = (నిర్మాణం xlate64 *) priv_దత్తాంశం;
	పూర్ణాంకం		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_స్థానభ్రంశం,
			     xl->real_వ్యక్తిగత);
	*blocknr = block32;
	తిరిగి ret;
}

errcode_t ext2fs_ఖండం_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				పూర్ణాంకం	flags,
				అక్షరం *ఖండం_buf,
				పూర్ణాంకం (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    పూర్ణాంకం		ref_స్థానభ్రంశం,
					    శూన్య	*priv_దత్తాంశం),
				శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం xlate64 xl;

	xl.real_వ్యక్తిగత = priv_దత్తాంశం;
	xl.func = func;

	తిరిగి ext2fs_ఖండం_iterate3(fs, ino, flags, ఖండం_buf, 
				     xlate64_func, &xl);
}


నిర్మాణం xlate {
	పూర్ణాంకం (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    పూర్ణాంకం		bcount,
		    శూన్య	*priv_దత్తాంశం);
	శూన్య *real_వ్యక్తిగత;
};

#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
స్థిర పూర్ణాంకం xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_ఖండం EXT2FS_ATTR((వాడని)),
		      పూర్ణాంకం ref_స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
		      శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం xlate *xl = (నిర్మాణం xlate *) priv_దత్తాంశం;

	తిరిగి (*xl->func)(fs, blocknr, (పూర్ణాంకం) blockcnt, xl->real_వ్యక్తిగత);
}

errcode_t ext2fs_ఖండం_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       పూర్ణాంకం	flags,
			       అక్షరం *ఖండం_buf,
			       పూర్ణాంకం (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   పూర్ణాంకం	blockcnt,
					   శూన్య	*priv_దత్తాంశం),
			       శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం xlate xl;

	xl.real_వ్యక్తిగత = priv_దత్తాంశం;
	xl.func = func;

	తిరిగి ext2fs_ఖండం_iterate2(fs, ino, ఖండం_FLAG_NO_LARGE | flags,
				     ఖండం_buf, xlate_func, &xl);
}

