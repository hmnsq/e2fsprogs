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
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

ساختار block_context {
	ext2_filsys	fs;
	صحیح (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    صحیح		ref_offset,
		    تهی	*priv_data);
	e2_blkcnt_t	bcount;
	صحیح		bsize;
	صحیح		flags;
	errcode_t	errcode;
	نویسه	*ind_buf;
	نویسه	*dind_buf;
	نویسه	*tind_buf;
	تهی	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	انجامبده {								\
		اگر (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			بازگشت ret;					\
		}							\
	} وقتی (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	انجامبده {								\
		اگر (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			برو label;					\
		}							\
	} وقتی (0)

ایستا صحیح block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     صحیح ref_offset, ساختار block_context *ctx)
{
	صحیح	ret = 0, changed = 0;
	صحیح	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	اگر (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_شمار_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	اگر (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		بازگشت ret;
	}
	اگر (*ind_block >= ext2fs_blocks_شمار(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		بازگشت ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	اگر (ctx->errcode) {
		ret |= BLOCK_ERROR;
		بازگشت ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	اگر (ctx->flags & BLOCK_FLAG_APPEND) {
		برای (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			اگر (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				بشکن;
			}
			offset += اندازه(blk_t);
		}
	} وگرنه {
		برای (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			اگر (*block_nr == 0)
				برو skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			اگر (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				بشکن;
			}
		skip_sparse:
			offset += اندازه(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	اگر (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		اگر (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	اگر ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_شمار_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	بازگشت ret;
}

ایستا صحیح block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      صحیح ref_offset, ساختار block_context *ctx)
{
	صحیح	ret = 0, changed = 0;
	صحیح	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	اگر (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_شمار_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	اگر (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		بازگشت ret;
	}
	اگر (*dind_block >= ext2fs_blocks_شمار(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		بازگشت ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	اگر (ctx->errcode) {
		ret |= BLOCK_ERROR;
		بازگشت ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	اگر (ctx->flags & BLOCK_FLAG_APPEND) {
		برای (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			اگر (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				بشکن;
			}
			offset += اندازه(blk_t);
		}
	} وگرنه {
		برای (i = 0; i < limit; i++, block_nr++) {
			اگر (*block_nr == 0) {
				ctx->bcount += limit;
				ادامه;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			اگر (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				بشکن;
			}
			offset += اندازه(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	اگر (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		اگر (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	اگر ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_شمار_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	بازگشت ret;
}

ایستا صحیح block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      صحیح ref_offset, ساختار block_context *ctx)
{
	صحیح	ret = 0, changed = 0;
	صحیح	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	اگر (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_شمار_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	اگر (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		بازگشت ret;
	}
	اگر (*tind_block >= ext2fs_blocks_شمار(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		بازگشت ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	اگر (ctx->errcode) {
		ret |= BLOCK_ERROR;
		بازگشت ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	اگر (ctx->flags & BLOCK_FLAG_APPEND) {
		برای (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			اگر (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				بشکن;
			}
			offset += اندازه(blk_t);
		}
	} وگرنه {
		برای (i = 0; i < limit; i++, block_nr++) {
			اگر (*block_nr == 0) {
				ctx->bcount += limit*limit;
				ادامه;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			اگر (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				بشکن;
			}
			offset += اندازه(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	اگر (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		اگر (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	اگر ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_شمار_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	بازگشت ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحیح	flags,
				نویسه *block_buf,
				صحیح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحیح		ref_offset,
					    تهی	*priv_data),
				تهی *priv_data)
{
	صحیح	i;
	صحیح	r, ret = 0;
	ساختار ext2_inode inode;
	errcode_t	retval;
	ساختار block_context ctx;
	صحیح	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	اگر (ctx.errcode)
		بازگشت ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	اگر (inode.i_flags & EXT4_INLINE_DATA_FL)
		بازگشت EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	اگر (flags & BLOCK_FLAG_NO_LARGE) {
		اگر (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_بالا != 0))
			بازگشت EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	اگر (block_buf) {
		ctx.ind_buf = block_buf;
	} وگرنه {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		اگر (retval)
			بازگشت retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	اگر ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		اگر (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_شمار_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			اگر (ret & BLOCK_ABORT)
				برو abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	اگر (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		ساختار ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		صحیح			op = EXT2_EXTENT_ریشه;
		صحیح			uninit;
		unsigned صحیح		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		اگر (ctx.errcode)
			برو abort_exit;

		وقتی (1) {
			اگر (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			وگرنه
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			اگر (ctx.errcode) {
				اگر (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					بشکن;
				ctx.errcode = 0;
				اگر (!(flags & BLOCK_FLAG_APPEND))
					بشکن;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				اگر (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					اگر (ctx.errcode || (ret & BLOCK_ABORT))
						بشکن;
					اگر (blk)
						برو next_block_set;
				}
				بشکن;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			اگر (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				اگر (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					ادامه;
				اگر ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					اگر (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						اگر (ctx.errcode)
							بشکن;
					}
					اگر (ret & BLOCK_ABORT)
						بشکن;
				}
				ادامه;
			}
			uninit = 0;
			اگر (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#اگر 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			اگر (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				ادامه;
			اگر (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			برای (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				اگر (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					اگر (ctx.errcode)
						برو extent_done;
				}
				اگر (ret & BLOCK_ABORT)
					برو extent_done;
			}
			اگر (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		برو errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	برای (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		اگر (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			اگر (ret & BLOCK_ABORT)
				برو abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	اگر (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		اگر (ret & BLOCK_ABORT)
			برو abort_exit;
	} وگرنه
		ctx.bcount += limit;
	اگر (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		اگر (ret & BLOCK_ABORT)
			برو abort_exit;
	} وگرنه
		ctx.bcount += limit * limit;
	اگر (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		اگر (ret & BLOCK_ABORT)
			برو abort_exit;
	}

abort_exit:
	اگر (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		اگر (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	اگر (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	بازگشت (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

ساختار xlate64 {
	صحیح (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    صحیح		ref_offset,
		    تهی	*priv_data);
	تهی *real_private;
};

ایستا صحیح xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			صحیح ref_offset, تهی *priv_data)
{
	ساختار xlate64 *xl = (ساختار xlate64 *) priv_data;
	صحیح		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	بازگشت ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				صحیح	flags,
				نویسه *block_buf,
				صحیح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحیح		ref_offset,
					    تهی	*priv_data),
				تهی *priv_data)
{
	ساختار xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	بازگشت ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


ساختار xlate {
	صحیح (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    صحیح		bcount,
		    تهی	*priv_data);
	تهی *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ایستا صحیح xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      صحیح ref_offset EXT2FS_ATTR((unused)),
		      تهی *priv_data)
{
	ساختار xlate *xl = (ساختار xlate *) priv_data;

	بازگشت (*xl->func)(fs, blocknr, (صحیح) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       صحیح	flags,
			       نویسه *block_buf,
			       صحیح (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   صحیح	blockcnt,
					   تهی	*priv_data),
			       تهی *priv_data)
{
	ساختار xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	بازگشت ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

