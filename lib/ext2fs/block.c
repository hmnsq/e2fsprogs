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
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية block_context {
	ext2_filsys	fs;
	صحيح (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    صحيح		ref_offset,
		    فراغ	*priv_data);
	e2_blkcnt_t	bcount;
	صحيح		bsize;
	صحيح		flags;
	errcode_t	errcode;
	حرف	*ind_buf;
	حرف	*dind_buf;
	حرف	*tind_buf;
	فراغ	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	ܥܒܕ {								\
		ܐܢ (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			ܦܢܐ ret;					\
		}							\
	} ܟܡܐ (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	ܥܒܕ {								\
		ܐܢ (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			اذهب label;					\
		}							\
	} ܟܡܐ (0)

ساكن صحيح block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     صحيح ref_offset, بنية block_context *ctx)
{
	صحيح	ret = 0, changed = 0;
	صحيح	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ܐܢ (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_COUNT_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ܐܢ (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		ܦܢܐ ret;
	}
	ܐܢ (*ind_block >= ext2fs_blocks_count(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		ܦܢܐ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	ܐܢ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ܦܢܐ ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	ܐܢ (ctx->flags & BLOCK_FLAG_APPEND) {
		لكل (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ܐܢ (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				اكسر;
			}
			offset += حجم(blk_t);
		}
	} ܐܚܪܢܐ {
		لكل (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			ܐܢ (*block_nr == 0)
				اذهب skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ܐܢ (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				اكسر;
			}
		skip_sparse:
			offset += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ܐܢ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		ܐܢ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ܐܢ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_COUNT_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ܦܢܐ ret;
}

ساكن صحيح block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      صحيح ref_offset, بنية block_context *ctx)
{
	صحيح	ret = 0, changed = 0;
	صحيح	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ܐܢ (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_COUNT_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ܐܢ (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		ܦܢܐ ret;
	}
	ܐܢ (*dind_block >= ext2fs_blocks_count(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		ܦܢܐ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	ܐܢ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ܦܢܐ ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	ܐܢ (ctx->flags & BLOCK_FLAG_APPEND) {
		لكل (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ܐܢ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				اكسر;
			}
			offset += حجم(blk_t);
		}
	} ܐܚܪܢܐ {
		لكل (i = 0; i < limit; i++, block_nr++) {
			ܐܢ (*block_nr == 0) {
				ctx->bcount += limit;
				استمر;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ܐܢ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				اكسر;
			}
			offset += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ܐܢ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		ܐܢ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ܐܢ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_COUNT_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ܦܢܐ ret;
}

ساكن صحيح block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      صحيح ref_offset, بنية block_context *ctx)
{
	صحيح	ret = 0, changed = 0;
	صحيح	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ܐܢ (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_COUNT_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ܐܢ (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		ܦܢܐ ret;
	}
	ܐܢ (*tind_block >= ext2fs_blocks_count(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		ܦܢܐ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	ܐܢ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ܦܢܐ ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	ܐܢ (ctx->flags & BLOCK_FLAG_APPEND) {
		لكل (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ܐܢ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				اكسر;
			}
			offset += حجم(blk_t);
		}
	} ܐܚܪܢܐ {
		لكل (i = 0; i < limit; i++, block_nr++) {
			ܐܢ (*block_nr == 0) {
				ctx->bcount += limit*limit;
				استمر;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ܐܢ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				اكسر;
			}
			offset += حجم(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ܐܢ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		ܐܢ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ܐܢ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_COUNT_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ܦܢܐ ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *block_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    صحيح		ref_offset,
					    فراغ	*priv_data),
				فراغ *priv_data)
{
	صحيح	i;
	صحيح	r, ret = 0;
	بنية ext2_inode inode;
	errcode_t	retval;
	بنية block_context ctx;
	صحيح	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	ܐܢ (ctx.errcode)
		ܦܢܐ ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	ܐܢ (inode.i_flags & EXT4_INLINE_DATA_FL)
		ܦܢܐ EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	ܐܢ (flags & BLOCK_FLAG_NO_LARGE) {
		ܐܢ (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_high != 0))
			ܦܢܐ EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	ܐܢ (block_buf) {
		ctx.ind_buf = block_buf;
	} ܐܚܪܢܐ {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		ܐܢ (retval)
			ܦܢܐ retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	ܐܢ ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		ܐܢ (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_COUNT_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			ܐܢ (ret & BLOCK_ABORT)
				اذهب abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	ܐܢ (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		بنية ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		صحيح			op = EXT2_EXTENT_ROOT;
		صحيح			uninit;
		unsigned صحيح		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		ܐܢ (ctx.errcode)
			اذهب abort_exit;

		ܟܡܐ (1) {
			ܐܢ (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			ܐܚܪܢܐ
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			ܐܢ (ctx.errcode) {
				ܐܢ (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					اكسر;
				ctx.errcode = 0;
				ܐܢ (!(flags & BLOCK_FLAG_APPEND))
					اكسر;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ܐܢ (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					ܐܢ (ctx.errcode || (ret & BLOCK_ABORT))
						اكسر;
					ܐܢ (blk)
						اذهب next_block_set;
				}
				اكسر;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			ܐܢ (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				ܐܢ (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					استمر;
				ܐܢ ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					ܐܢ (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						ܐܢ (ctx.errcode)
							اكسر;
					}
					ܐܢ (ret & BLOCK_ABORT)
						اكسر;
				}
				استمر;
			}
			uninit = 0;
			ܐܢ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#ܐܢ 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			ܐܢ (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				استمر;
			ܐܢ (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			لكل (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ܐܢ (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					ܐܢ (ctx.errcode)
						اذهب extent_done;
				}
				ܐܢ (ret & BLOCK_ABORT)
					اذهب extent_done;
			}
			ܐܢ (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		اذهب errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	لكل (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		ܐܢ (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			ܐܢ (ret & BLOCK_ABORT)
				اذهب abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	ܐܢ (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		ܐܢ (ret & BLOCK_ABORT)
			اذهب abort_exit;
	} ܐܚܪܢܐ
		ctx.bcount += limit;
	ܐܢ (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		ܐܢ (ret & BLOCK_ABORT)
			اذهب abort_exit;
	} ܐܚܪܢܐ
		ctx.bcount += limit * limit;
	ܐܢ (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		ܐܢ (ret & BLOCK_ABORT)
			اذهب abort_exit;
	}

abort_exit:
	ܐܢ (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		ܐܢ (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	ܐܢ (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	ܦܢܐ (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

بنية xlate64 {
	صحيح (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    صحيح		ref_offset,
		    فراغ	*priv_data);
	فراغ *real_private;
};

ساكن صحيح xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			صحيح ref_offset, فراغ *priv_data)
{
	بنية xlate64 *xl = (بنية xlate64 *) priv_data;
	صحيح		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	ܦܢܐ ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				صحيح	flags,
				حرف *block_buf,
				صحيح (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    صحيح		ref_offset,
					    فراغ	*priv_data),
				فراغ *priv_data)
{
	بنية xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	ܦܢܐ ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


بنية xlate {
	صحيح (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    صحيح		bcount,
		    فراغ	*priv_data);
	فراغ *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ساكن صحيح xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      صحيح ref_offset EXT2FS_ATTR((unused)),
		      فراغ *priv_data)
{
	بنية xlate *xl = (بنية xlate *) priv_data;

	ܦܢܐ (*xl->func)(fs, blocknr, (صحيح) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       صحيح	flags,
			       حرف *block_buf,
			       صحيح (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   صحيح	blockcnt,
					   فراغ	*priv_data),
			       فراغ *priv_data)
{
	بنية xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ܦܢܐ ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

