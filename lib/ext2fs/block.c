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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

ساخت بلاک_context {
	ext2_filsys	fs;
	صحیح (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    صحیح		ref_انحراف,
		    خالی	*priv_ڈیٹا);
	e2_blkcnt_t	bcount;
	صحیح		bsize;
	صحیح		flags;
	errcode_t	errcode;
	حرف	*ind_buf;
	حرف	*dind_buf;
	حرف	*tind_buf;
	خالی	*priv_ڈیٹا;
};

#تعریف_کرو check_for_ro_violation_return(ctx, ret)				\
	کرو {								\
		اگر (((ctx)->flags & بلاک_FLAG_READ_ONLY) &&		\
		    ((ret) & بلاک_بدلا)) {				\
			(ctx)->errcode = EXT2_ET_RO_بلاک_ITERATE;	\
			ret |= بلاک_ABORT | بلاک_ERROR;		\
			واپس ret;					\
		}							\
	} جب (0)

#تعریف_کرو check_for_ro_violation_goto(ctx, ret, label)			\
	کرو {								\
		اگر (((ctx)->flags & بلاک_FLAG_READ_ONLY) &&		\
		    ((ret) & بلاک_بدلا)) {				\
			(ctx)->errcode = EXT2_ET_RO_بلاک_ITERATE;	\
			ret |= بلاک_ABORT | بلاک_ERROR;		\
			جاؤ label;					\
		}							\
	} جب (0)

ساکن صحیح بلاک_iterate_ind(blk_t *ind_بلاک, blk_t ref_بلاک,
			     صحیح ref_انحراف, ساخت بلاک_context *ctx)
{
	صحیح	ret = 0, بدلا = 0;
	صحیح	i, flags, حد_بندی, انحراف;
	blk_t	*بلاک_nr;
	blk64_t	blk64;

	حد_بندی = ctx->fs->blocksize >> 2;
	اگر (!(ctx->flags & بلاک_FLAG_گہرائی_TRAVERSE) &&
	    !(ctx->flags & بلاک_FLAG_ڈیٹا_ONLY)) {
		blk64 = *ind_بلاک;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلاک_گنتی_IND, ref_بلاک,
				   ref_انحراف, ctx->priv_ڈیٹا);
		*ind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	اگر (!*ind_بلاک || (ret & بلاک_ABORT)) {
		ctx->bcount += حد_بندی;
		واپس ret;
	}
	اگر (*ind_بلاک >= ext2fs_بلاکیں_گنتی(ctx->fs->super) ||
	    *ind_بلاک < ctx->fs->super->s_پہلا_ڈیٹا_بلاک) {
		ctx->errcode = EXT2_ET_BAD_IND_بلاک;
		ret |= بلاک_ERROR;
		واپس ret;
	}
	ctx->errcode = ext2fs_read_ind_بلاک(ctx->fs, *ind_بلاک,
					     ctx->ind_buf);
	اگر (ctx->errcode) {
		ret |= بلاک_ERROR;
		واپس ret;
	}

	بلاک_nr = (blk_t *) ctx->ind_buf;
	انحراف = 0;
	اگر (ctx->flags & بلاک_FLAG_APPEND) {
		برائے (i = 0; i < حد_بندی; i++, ctx->bcount++, بلاک_nr++) {
			blk64 = *بلاک_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_بلاک, انحراف,
					     ctx->priv_ڈیٹا);
			*بلاک_nr = blk64;
			بدلا	|= flags;
			اگر (flags & بلاک_ABORT) {
				ret |= بلاک_ABORT;
				توڑو;
			}
			انحراف += حجم(blk_t);
		}
	} ورنہ {
		برائے (i = 0; i < حد_بندی; i++, ctx->bcount++, بلاک_nr++) {
			اگر (*بلاک_nr == 0)
				جاؤ skip_sparse;
			blk64 = *بلاک_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_بلاک, انحراف,
					     ctx->priv_ڈیٹا);
			*بلاک_nr = blk64;
			بدلا	|= flags;
			اگر (flags & بلاک_ABORT) {
				ret |= بلاک_ABORT;
				توڑو;
			}
		skip_sparse:
			انحراف += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, بدلا);
	اگر (بدلا & بلاک_بدلا) {
		ctx->errcode = ext2fs_write_ind_بلاک(ctx->fs, *ind_بلاک,
						      ctx->ind_buf);
		اگر (ctx->errcode)
			ret |= بلاک_ERROR | بلاک_ABORT;
	}
	اگر ((ctx->flags & بلاک_FLAG_گہرائی_TRAVERSE) &&
	    !(ctx->flags & بلاک_FLAG_ڈیٹا_ONLY) &&
	    !(ret & بلاک_ABORT)) {
		blk64 = *ind_بلاک;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلاک_گنتی_IND, ref_بلاک,
				    ref_انحراف, ctx->priv_ڈیٹا);
		*ind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	واپس ret;
}

ساکن صحیح بلاک_iterate_dind(blk_t *dind_بلاک, blk_t ref_بلاک,
			      صحیح ref_انحراف, ساخت بلاک_context *ctx)
{
	صحیح	ret = 0, بدلا = 0;
	صحیح	i, flags, حد_بندی, انحراف;
	blk_t	*بلاک_nr;
	blk64_t	blk64;

	حد_بندی = ctx->fs->blocksize >> 2;
	اگر (!(ctx->flags & (بلاک_FLAG_گہرائی_TRAVERSE |
			    بلاک_FLAG_ڈیٹا_ONLY))) {
		blk64 = *dind_بلاک;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلاک_گنتی_DIND, ref_بلاک,
				   ref_انحراف, ctx->priv_ڈیٹا);
		*dind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	اگر (!*dind_بلاک || (ret & بلاک_ABORT)) {
		ctx->bcount += حد_بندی*حد_بندی;
		واپس ret;
	}
	اگر (*dind_بلاک >= ext2fs_بلاکیں_گنتی(ctx->fs->super) ||
	    *dind_بلاک < ctx->fs->super->s_پہلا_ڈیٹا_بلاک) {
		ctx->errcode = EXT2_ET_BAD_DIND_بلاک;
		ret |= بلاک_ERROR;
		واپس ret;
	}
	ctx->errcode = ext2fs_read_ind_بلاک(ctx->fs, *dind_بلاک,
					     ctx->dind_buf);
	اگر (ctx->errcode) {
		ret |= بلاک_ERROR;
		واپس ret;
	}

	بلاک_nr = (blk_t *) ctx->dind_buf;
	انحراف = 0;
	اگر (ctx->flags & بلاک_FLAG_APPEND) {
		برائے (i = 0; i < حد_بندی; i++, بلاک_nr++) {
			flags = بلاک_iterate_ind(بلاک_nr,
						  *dind_بلاک, انحراف,
						  ctx);
			بدلا |= flags;
			اگر (flags & (بلاک_ABORT | بلاک_ERROR)) {
				ret |= flags & (بلاک_ABORT | بلاک_ERROR);
				توڑو;
			}
			انحراف += حجم(blk_t);
		}
	} ورنہ {
		برائے (i = 0; i < حد_بندی; i++, بلاک_nr++) {
			اگر (*بلاک_nr == 0) {
				ctx->bcount += حد_بندی;
				جاری;
			}
			flags = بلاک_iterate_ind(بلاک_nr,
						  *dind_بلاک, انحراف,
						  ctx);
			بدلا |= flags;
			اگر (flags & (بلاک_ABORT | بلاک_ERROR)) {
				ret |= flags & (بلاک_ABORT | بلاک_ERROR);
				توڑو;
			}
			انحراف += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, بدلا);
	اگر (بدلا & بلاک_بدلا) {
		ctx->errcode = ext2fs_write_ind_بلاک(ctx->fs, *dind_بلاک,
						      ctx->dind_buf);
		اگر (ctx->errcode)
			ret |= بلاک_ERROR | بلاک_ABORT;
	}
	اگر ((ctx->flags & بلاک_FLAG_گہرائی_TRAVERSE) &&
	    !(ctx->flags & بلاک_FLAG_ڈیٹا_ONLY) &&
	    !(ret & بلاک_ABORT)) {
		blk64 = *dind_بلاک;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلاک_گنتی_DIND, ref_بلاک,
				    ref_انحراف, ctx->priv_ڈیٹا);
		*dind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	واپس ret;
}

ساکن صحیح بلاک_iterate_tind(blk_t *tind_بلاک, blk_t ref_بلاک,
			      صحیح ref_انحراف, ساخت بلاک_context *ctx)
{
	صحیح	ret = 0, بدلا = 0;
	صحیح	i, flags, حد_بندی, انحراف;
	blk_t	*بلاک_nr;
	blk64_t	blk64;

	حد_بندی = ctx->fs->blocksize >> 2;
	اگر (!(ctx->flags & (بلاک_FLAG_گہرائی_TRAVERSE |
			    بلاک_FLAG_ڈیٹا_ONLY))) {
		blk64 = *tind_بلاک;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   بلاک_گنتی_TIND, ref_بلاک,
				   ref_انحراف, ctx->priv_ڈیٹا);
		*tind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	اگر (!*tind_بلاک || (ret & بلاک_ABORT)) {
		ctx->bcount += ((غیرعلامتی طویل طویل) حد_بندی)*حد_بندی*حد_بندی;
		واپس ret;
	}
	اگر (*tind_بلاک >= ext2fs_بلاکیں_گنتی(ctx->fs->super) ||
	    *tind_بلاک < ctx->fs->super->s_پہلا_ڈیٹا_بلاک) {
		ctx->errcode = EXT2_ET_BAD_TIND_بلاک;
		ret |= بلاک_ERROR;
		واپس ret;
	}
	ctx->errcode = ext2fs_read_ind_بلاک(ctx->fs, *tind_بلاک,
					     ctx->tind_buf);
	اگر (ctx->errcode) {
		ret |= بلاک_ERROR;
		واپس ret;
	}

	بلاک_nr = (blk_t *) ctx->tind_buf;
	انحراف = 0;
	اگر (ctx->flags & بلاک_FLAG_APPEND) {
		برائے (i = 0; i < حد_بندی; i++, بلاک_nr++) {
			flags = بلاک_iterate_dind(بلاک_nr,
						   *tind_بلاک,
						   انحراف, ctx);
			بدلا |= flags;
			اگر (flags & (بلاک_ABORT | بلاک_ERROR)) {
				ret |= flags & (بلاک_ABORT | بلاک_ERROR);
				توڑو;
			}
			انحراف += حجم(blk_t);
		}
	} ورنہ {
		برائے (i = 0; i < حد_بندی; i++, بلاک_nr++) {
			اگر (*بلاک_nr == 0) {
				ctx->bcount += حد_بندی*حد_بندی;
				جاری;
			}
			flags = بلاک_iterate_dind(بلاک_nr,
						   *tind_بلاک,
						   انحراف, ctx);
			بدلا |= flags;
			اگر (flags & (بلاک_ABORT | بلاک_ERROR)) {
				ret |= flags & (بلاک_ABORT | بلاک_ERROR);
				توڑو;
			}
			انحراف += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, بدلا);
	اگر (بدلا & بلاک_بدلا) {
		ctx->errcode = ext2fs_write_ind_بلاک(ctx->fs, *tind_بلاک,
						      ctx->tind_buf);
		اگر (ctx->errcode)
			ret |= بلاک_ERROR | بلاک_ABORT;
	}
	اگر ((ctx->flags & بلاک_FLAG_گہرائی_TRAVERSE) &&
	    !(ctx->flags & بلاک_FLAG_ڈیٹا_ONLY) &&
	    !(ret & بلاک_ABORT)) {
		blk64 = *tind_بلاک;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    بلاک_گنتی_TIND, ref_بلاک,
				    ref_انحراف, ctx->priv_ڈیٹا);
		*tind_بلاک = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	واپس ret;
}

errcode_t ext2fs_بلاک_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحیح	flags,
				حرف *بلاک_buf,
				صحیح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحیح		ref_انحراف,
					    خالی	*priv_ڈیٹا),
				خالی *priv_ڈیٹا)
{
	صحیح	i;
	صحیح	r, ret = 0;
	ساخت ext2_inode inode;
	errcode_t	retval;
	ساخت بلاک_context ctx;
	صحیح	حد_بندی;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	اگر (ctx.errcode)
		واپس ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	اگر (inode.i_flags & EXT4_INLINE_ڈیٹا_FL)
		واپس EXT2_ET_INLINE_ڈیٹا_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	اگر (flags & بلاک_FLAG_NO_LARGE) {
		اگر (!LINUX_S_ISDIR(inode.i_منوال) &&
		    (inode.i_سائز_بلند != 0))
			واپس EXT2_ET_FILE_TOO_BIG;
	}

	حد_بندی = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_ڈیٹا = priv_ڈیٹا;
	ctx.flags = flags;
	ctx.bcount = 0;
	اگر (بلاک_buf) {
		ctx.ind_buf = بلاک_buf;
	} ورنہ {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		اگر (retval)
			واپس retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	اگر ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & بلاک_FLAG_ڈیٹا_ONLY)) {
		اگر (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   بلاک_گنتی_TRANSLATOR,
					   0, 0, priv_ڈیٹا);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			اگر (ret & بلاک_ABORT)
				جاؤ abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	اگر (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		ساخت ext2fs_extent	extent, اگلا;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		صحیح			op = EXT2_EXTENT_جڑ;
		صحیح			uninit;
		غیرعلامتی صحیح		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		اگر (ctx.errcode)
			جاؤ abort_exit;

		جب (1) {
			اگر (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			ورنہ
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			اگر (ctx.errcode) {
				اگر (ctx.errcode != EXT2_ET_EXTENT_NO_اگلا)
					توڑو;
				ctx.errcode = 0;
				اگر (!(flags & بلاک_FLAG_APPEND))
					توڑو;
			اگلا_بلاک_سیٹ:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_ڈیٹا);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_مکمل);
				اگر (r & بلاک_بدلا) {
					ctx.errcode =
						ext2fs_extent_سیٹ_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					اگر (ctx.errcode || (ret & بلاک_ABORT))
						توڑو;
					اگر (blk)
						جاؤ اگلا_بلاک_سیٹ;
				}
				توڑو;
			}

			op = EXT2_EXTENT_اگلا;
			blk = extent.e_pblk;
			اگر (!(extent.e_flags & EXT2_EXTENT_FLAGS_برگ)) {
				اگر (ctx.flags & بلاک_FLAG_ڈیٹا_ONLY)
					جاری;
				اگر ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_دوسرا_دورہ) &&
				     !(ctx.flags & بلاک_FLAG_گہرائی_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_دوسرا_دورہ) &&
				     (ctx.flags & بلاک_FLAG_گہرائی_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_ڈیٹا);
					اگر (ret & بلاک_بدلا) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						اگر (ctx.errcode)
							توڑو;
					}
					اگر (ret & بلاک_ABORT)
						توڑو;
				}
				جاری;
			}
			uninit = 0;
			اگر (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_سیٹ_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &اگلا);

#اگر 0
			لکھو("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#اگر_ختم
			اگر (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				جاری;
			اگر (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			برائے (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_ڈیٹا);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_مکمل);
				اگر (r & بلاک_بدلا) {
					ctx.errcode =
						ext2fs_extent_سیٹ_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					اگر (ctx.errcode)
						جاؤ extent_مکمل;
				}
				اگر (ret & بلاک_ABORT)
					جاؤ extent_مکمل;
			}
			اگر (retval == 0) {
				extent = اگلا;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_مکمل:
		ext2fs_extent_free(handle);
		ret |= بلاک_ERROR; /* ctx.errcode is always valid here */
		جاؤ errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	برائے (i = 0; i < EXT2_NDIR_بلاکیں ; i++, ctx.bcount++) {
		اگر (inode.i_بلاک[i] || (flags & بلاک_FLAG_APPEND)) {
			blk64 = inode.i_بلاک[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_ڈیٹا);
			inode.i_بلاک[i] = (blk_t) blk64;
			اگر (ret & بلاک_ABORT)
				جاؤ abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	اگر (inode.i_بلاک[EXT2_IND_بلاک] || (flags & بلاک_FLAG_APPEND)) {
		ret |= بلاک_iterate_ind(&inode.i_بلاک[EXT2_IND_بلاک], 
					 0, EXT2_IND_بلاک, &ctx);
		اگر (ret & بلاک_ABORT)
			جاؤ abort_exit;
	} ورنہ
		ctx.bcount += حد_بندی;
	اگر (inode.i_بلاک[EXT2_DIND_بلاک] || (flags & بلاک_FLAG_APPEND)) {
		ret |= بلاک_iterate_dind(&inode.i_بلاک[EXT2_DIND_بلاک],
					  0, EXT2_DIND_بلاک, &ctx);
		اگر (ret & بلاک_ABORT)
			جاؤ abort_exit;
	} ورنہ
		ctx.bcount += حد_بندی * حد_بندی;
	اگر (inode.i_بلاک[EXT2_TIND_بلاک] || (flags & بلاک_FLAG_APPEND)) {
		ret |= بلاک_iterate_tind(&inode.i_بلاک[EXT2_TIND_بلاک],
					  0, EXT2_TIND_بلاک, &ctx);
		اگر (ret & بلاک_ABORT)
			جاؤ abort_exit;
	}

abort_exit:
	اگر (ret & بلاک_بدلا) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		اگر (retval) {
			ret |= بلاک_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	اگر (!بلاک_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	واپس (ret & بلاک_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

ساخت xlate64 {
	صحیح (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    صحیح		ref_انحراف,
		    خالی	*priv_ڈیٹا);
	خالی *real_نجی;
};

ساکن صحیح xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			صحیح ref_انحراف, خالی *priv_ڈیٹا)
{
	ساخت xlate64 *xl = (ساخت xlate64 *) priv_ڈیٹا;
	صحیح		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_انحراف,
			     xl->real_نجی);
	*blocknr = block32;
	واپس ret;
}

errcode_t ext2fs_بلاک_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				صحیح	flags,
				حرف *بلاک_buf,
				صحیح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحیح		ref_انحراف,
					    خالی	*priv_ڈیٹا),
				خالی *priv_ڈیٹا)
{
	ساخت xlate64 xl;

	xl.real_نجی = priv_ڈیٹا;
	xl.func = func;

	واپس ext2fs_بلاک_iterate3(fs, ino, flags, بلاک_buf, 
				     xlate64_func, &xl);
}


ساخت xlate {
	صحیح (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    صحیح		bcount,
		    خالی	*priv_ڈیٹا);
	خالی *real_نجی;
};

#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
ساکن صحیح xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_بلاک EXT2FS_ATTR((غیر_مستعمل)),
		      صحیح ref_انحراف EXT2FS_ATTR((غیر_مستعمل)),
		      خالی *priv_ڈیٹا)
{
	ساخت xlate *xl = (ساخت xlate *) priv_ڈیٹا;

	واپس (*xl->func)(fs, blocknr, (صحیح) blockcnt, xl->real_نجی);
}

errcode_t ext2fs_بلاک_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       صحیح	flags,
			       حرف *بلاک_buf,
			       صحیح (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   صحیح	blockcnt,
					   خالی	*priv_ڈیٹا),
			       خالی *priv_ڈیٹا)
{
	ساخت xlate xl;

	xl.real_نجی = priv_ڈیٹا;
	xl.func = func;

	واپس ext2fs_بلاک_iterate2(fs, ino, بلاک_FLAG_NO_LARGE | flags,
				     بلاک_buf, xlate_func, &xl);
}

