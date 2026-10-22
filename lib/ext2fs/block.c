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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

ସଂରଚନା block_context {
	ext2_filsys	fs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ref_offset,
		    ଶୂନ୍ୟ	*priv_data);
	e2_blkcnt_t	bcount;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		bsize;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		flags;
	errcode_t	errcode;
	ଅକ୍ଷର	*ind_buf;
	ଅକ୍ଷର	*dind_buf;
	ଅକ୍ଷର	*tind_buf;
	ଶୂନ୍ୟ	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	କର {								\
		ଯଦି (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			ଫେରନ୍ତୁ ret;					\
		}							\
	} ଯେତେବେଳେ (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	କର {								\
		ଯଦି (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			ଯାଅ label;					\
		}							\
	} ଯେତେବେଳେ (0)

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset, ସଂରଚନା block_context *ctx)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ret = 0, changed = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ଯଦି (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_ଗଣନା_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ଯଦି (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		ଫେରନ୍ତୁ ret;
	}
	ଯଦି (*ind_block >= ext2fs_blocks_ଗଣନା(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		ଫେରନ୍ତୁ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	ଯଦି (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ଫେରନ୍ତୁ ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	ଯଦି (ctx->flags & BLOCK_FLAG_APPEND) {
		ପାଇଁ (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ଯଦି (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				ଭାଙ୍ଗ;
			}
			offset += ଆକାର(blk_t);
		}
	} ଅନ୍ୟଥା {
		ପାଇଁ (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			ଯଦି (*block_nr == 0)
				ଯାଅ skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ଯଦି (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				ଭାଙ୍ଗ;
			}
		skip_sparse:
			offset += ଆକାର(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ଯଦି (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		ଯଦି (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ଯଦି ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_ଗଣନା_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset, ସଂରଚନା block_context *ctx)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ret = 0, changed = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ଯଦି (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_ଗଣନା_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ଯଦି (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		ଫେରନ୍ତୁ ret;
	}
	ଯଦି (*dind_block >= ext2fs_blocks_ଗଣନା(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		ଫେରନ୍ତୁ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	ଯଦି (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ଫେରନ୍ତୁ ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	ଯଦି (ctx->flags & BLOCK_FLAG_APPEND) {
		ପାଇଁ (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ଯଦି (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ଭାଙ୍ଗ;
			}
			offset += ଆକାର(blk_t);
		}
	} ଅନ୍ୟଥା {
		ପାଇଁ (i = 0; i < limit; i++, block_nr++) {
			ଯଦି (*block_nr == 0) {
				ctx->bcount += limit;
				ଜାରି;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ଯଦି (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ଭାଙ୍ଗ;
			}
			offset += ଆକାର(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ଯଦି (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		ଯଦି (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ଯଦି ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_ଗଣନା_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset, ସଂରଚନା block_context *ctx)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ret = 0, changed = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ଯଦି (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_ଗଣନା_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ଯଦି (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		ଫେରନ୍ତୁ ret;
	}
	ଯଦି (*tind_block >= ext2fs_blocks_ଗଣନା(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		ଫେରନ୍ତୁ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	ଯଦି (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ଫେରନ୍ତୁ ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	ଯଦି (ctx->flags & BLOCK_FLAG_APPEND) {
		ପାଇଁ (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ଯଦି (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ଭାଙ୍ଗ;
			}
			offset += ଆକାର(blk_t);
		}
	} ଅନ୍ୟଥା {
		ପାଇଁ (i = 0; i < limit; i++, block_nr++) {
			ଯଦି (*block_nr == 0) {
				ctx->bcount += limit*limit;
				ଜାରି;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ଯଦି (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ଭାଙ୍ଗ;
			}
			offset += ଆକାର(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ଯଦି (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		ଯଦି (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ଯଦି ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_ଗଣନା_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ଫେରନ୍ତୁ ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା	flags,
				ଅକ୍ଷର *block_buf,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ref_offset,
					    ଶୂନ୍ୟ	*priv_data),
				ଶୂନ୍ୟ *priv_data)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	r, ret = 0;
	ସଂରଚନା ext2_inode inode;
	errcode_t	retval;
	ସଂରଚନା block_context ctx;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (ctx.errcode)
		ଫେରନ୍ତୁ ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	ଯଦି (inode.i_flags & EXT4_INLINE_DATA_FL)
		ଫେରନ୍ତୁ EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	ଯଦି (flags & BLOCK_FLAG_NO_LARGE) {
		ଯଦି (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_ଉପର != 0))
			ଫେରନ୍ତୁ EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	ଯଦି (block_buf) {
		ctx.ind_buf = block_buf;
	} ଅନ୍ୟଥା {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	ଯଦି ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		ଯଦି (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_ଗଣନା_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			ଯଦି (ret & BLOCK_ABORT)
				ଯାଅ abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	ଯଦି (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		ସଂରଚନା ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା			op = EXT2_EXTENT_ମୂଳ;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା			uninit;
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		ଯଦି (ctx.errcode)
			ଯାଅ abort_exit;

		ଯେତେବେଳେ (1) {
			ଯଦି (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			ଅନ୍ୟଥା
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			ଯଦି (ctx.errcode) {
				ଯଦି (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					ଭାଙ୍ଗ;
				ctx.errcode = 0;
				ଯଦି (!(flags & BLOCK_FLAG_APPEND))
					ଭାଙ୍ଗ;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ଯଦି (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					ଯଦି (ctx.errcode || (ret & BLOCK_ABORT))
						ଭାଙ୍ଗ;
					ଯଦି (blk)
						ଯାଅ next_block_set;
				}
				ଭାଙ୍ଗ;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			ଯଦି (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				ଯଦି (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					ଜାରି;
				ଯଦି ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					ଯଦି (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						ଯଦି (ctx.errcode)
							ଭାଙ୍ଗ;
					}
					ଯଦି (ret & BLOCK_ABORT)
						ଭାଙ୍ଗ;
				}
				ଜାରି;
			}
			uninit = 0;
			ଯଦି (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#ଯଦି 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			ଯଦି (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				ଜାରି;
			ଯଦି (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			ପାଇଁ (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ଯଦି (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					ଯଦି (ctx.errcode)
						ଯାଅ extent_done;
				}
				ଯଦି (ret & BLOCK_ABORT)
					ଯାଅ extent_done;
			}
			ଯଦି (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		ଯାଅ errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	ପାଇଁ (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		ଯଦି (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			ଯଦି (ret & BLOCK_ABORT)
				ଯାଅ abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	ଯଦି (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		ଯଦି (ret & BLOCK_ABORT)
			ଯାଅ abort_exit;
	} ଅନ୍ୟଥା
		ctx.bcount += limit;
	ଯଦି (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		ଯଦି (ret & BLOCK_ABORT)
			ଯାଅ abort_exit;
	} ଅନ୍ୟଥା
		ctx.bcount += limit * limit;
	ଯଦି (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		ଯଦି (ret & BLOCK_ABORT)
			ଯାଅ abort_exit;
	}

abort_exit:
	ଯଦି (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		ଯଦି (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	ଯଦି (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	ଫେରନ୍ତୁ (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

ସଂରଚନା xlate64 {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ref_offset,
		    ଶୂନ୍ୟ	*priv_data);
	ଶୂନ୍ୟ *real_private;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset, ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା xlate64 *xl = (ସଂରଚନା xlate64 *) priv_data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	ଫେରନ୍ତୁ ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା	flags,
				ଅକ୍ଷର *block_buf,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ref_offset,
					    ଶୂନ୍ୟ	*priv_data),
				ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	ଫେରନ୍ତୁ ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


ସଂରଚନା xlate {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା		bcount,
		    ଶୂନ୍ୟ	*priv_data);
	ଶୂନ୍ୟ *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset EXT2FS_ATTR((unused)),
		      ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା xlate *xl = (ସଂରଚନା xlate *) priv_data;

	ଫେରନ୍ତୁ (*xl->func)(fs, blocknr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା	flags,
			       ଅକ୍ଷର *block_buf,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blockcnt,
					   ଶୂନ୍ୟ	*priv_data),
			       ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ଫେରନ୍ତୁ ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

