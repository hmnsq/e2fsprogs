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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

கட்டமைப்பு block_context {
	ext2_filsys	fs;
	முழு (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    முழு		ref_offset,
		    வெறுமை	*priv_data);
	e2_blkcnt_t	bcount;
	முழு		bsize;
	முழு		flags;
	errcode_t	errcode;
	எழுத்து	*ind_buf;
	எழுத்து	*dind_buf;
	எழுத்து	*tind_buf;
	வெறுமை	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	செய் {								\
		எனில் (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			திரும்பு ret;					\
		}							\
	} போது (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	செய் {								\
		எனில் (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			செல் label;					\
		}							\
	} போது (0)

நிலையான முழு block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     முழு ref_offset, கட்டமைப்பு block_context *ctx)
{
	முழு	ret = 0, changed = 0;
	முழு	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	எனில் (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_மொத்தம்_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	எனில் (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		திரும்பு ret;
	}
	எனில் (*ind_block >= ext2fs_blocks_மொத்தம்(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		திரும்பு ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	எனில் (ctx->errcode) {
		ret |= BLOCK_ERROR;
		திரும்பு ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	எனில் (ctx->flags & BLOCK_FLAG_APPEND) {
		க்கு (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			எனில் (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				நிறுத்து;
			}
			offset += அளவு(blk_t);
		}
	} இல்லையெனில் {
		க்கு (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			எனில் (*block_nr == 0)
				செல் skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			எனில் (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				நிறுத்து;
			}
		skip_sparse:
			offset += அளவு(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	எனில் (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		எனில் (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	எனில் ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_மொத்தம்_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	திரும்பு ret;
}

நிலையான முழு block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      முழு ref_offset, கட்டமைப்பு block_context *ctx)
{
	முழு	ret = 0, changed = 0;
	முழு	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	எனில் (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_மொத்தம்_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	எனில் (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		திரும்பு ret;
	}
	எனில் (*dind_block >= ext2fs_blocks_மொத்தம்(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		திரும்பு ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	எனில் (ctx->errcode) {
		ret |= BLOCK_ERROR;
		திரும்பு ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	எனில் (ctx->flags & BLOCK_FLAG_APPEND) {
		க்கு (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			எனில் (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				நிறுத்து;
			}
			offset += அளவு(blk_t);
		}
	} இல்லையெனில் {
		க்கு (i = 0; i < limit; i++, block_nr++) {
			எனில் (*block_nr == 0) {
				ctx->bcount += limit;
				தொடர்;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			எனில் (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				நிறுத்து;
			}
			offset += அளவு(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	எனில் (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		எனில் (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	எனில் ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_மொத்தம்_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	திரும்பு ret;
}

நிலையான முழு block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      முழு ref_offset, கட்டமைப்பு block_context *ctx)
{
	முழு	ret = 0, changed = 0;
	முழு	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	எனில் (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_மொத்தம்_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	எனில் (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		திரும்பு ret;
	}
	எனில் (*tind_block >= ext2fs_blocks_மொத்தம்(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		திரும்பு ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	எனில் (ctx->errcode) {
		ret |= BLOCK_ERROR;
		திரும்பு ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	எனில் (ctx->flags & BLOCK_FLAG_APPEND) {
		க்கு (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			எனில் (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				நிறுத்து;
			}
			offset += அளவு(blk_t);
		}
	} இல்லையெனில் {
		க்கு (i = 0; i < limit; i++, block_nr++) {
			எனில் (*block_nr == 0) {
				ctx->bcount += limit*limit;
				தொடர்;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			எனில் (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				நிறுத்து;
			}
			offset += அளவு(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	எனில் (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		எனில் (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	எனில் ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_மொத்தம்_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	திரும்பு ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				முழு	flags,
				எழுத்து *block_buf,
				முழு (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    முழு		ref_offset,
					    வெறுமை	*priv_data),
				வெறுமை *priv_data)
{
	முழு	i;
	முழு	r, ret = 0;
	கட்டமைப்பு ext2_inode inode;
	errcode_t	retval;
	கட்டமைப்பு block_context ctx;
	முழு	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	எனில் (ctx.errcode)
		திரும்பு ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	எனில் (inode.i_flags & EXT4_INLINE_DATA_FL)
		திரும்பு EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	எனில் (flags & BLOCK_FLAG_NO_LARGE) {
		எனில் (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_உயர்வு != 0))
			திரும்பு EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	எனில் (block_buf) {
		ctx.ind_buf = block_buf;
	} இல்லையெனில் {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		எனில் (retval)
			திரும்பு retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	எனில் ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		எனில் (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_மொத்தம்_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			எனில் (ret & BLOCK_ABORT)
				செல் abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	எனில் (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		கட்டமைப்பு ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		முழு			op = EXT2_EXTENT_வேர்;
		முழு			uninit;
		unsigned முழு		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		எனில் (ctx.errcode)
			செல் abort_exit;

		போது (1) {
			எனில் (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			இல்லையெனில்
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			எனில் (ctx.errcode) {
				எனில் (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					நிறுத்து;
				ctx.errcode = 0;
				எனில் (!(flags & BLOCK_FLAG_APPEND))
					நிறுத்து;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				எனில் (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					எனில் (ctx.errcode || (ret & BLOCK_ABORT))
						நிறுத்து;
					எனில் (blk)
						செல் next_block_set;
				}
				நிறுத்து;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			எனில் (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				எனில் (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					தொடர்;
				எனில் ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					எனில் (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						எனில் (ctx.errcode)
							நிறுத்து;
					}
					எனில் (ret & BLOCK_ABORT)
						நிறுத்து;
				}
				தொடர்;
			}
			uninit = 0;
			எனில் (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#எனில் 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			எனில் (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				தொடர்;
			எனில் (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			க்கு (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				எனில் (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					எனில் (ctx.errcode)
						செல் extent_done;
				}
				எனில் (ret & BLOCK_ABORT)
					செல் extent_done;
			}
			எனில் (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		செல் errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	க்கு (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		எனில் (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			எனில் (ret & BLOCK_ABORT)
				செல் abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	எனில் (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		எனில் (ret & BLOCK_ABORT)
			செல் abort_exit;
	} இல்லையெனில்
		ctx.bcount += limit;
	எனில் (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		எனில் (ret & BLOCK_ABORT)
			செல் abort_exit;
	} இல்லையெனில்
		ctx.bcount += limit * limit;
	எனில் (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		எனில் (ret & BLOCK_ABORT)
			செல் abort_exit;
	}

abort_exit:
	எனில் (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		எனில் (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	எனில் (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	திரும்பு (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

கட்டமைப்பு xlate64 {
	முழு (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    முழு		ref_offset,
		    வெறுமை	*priv_data);
	வெறுமை *real_private;
};

நிலையான முழு xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			முழு ref_offset, வெறுமை *priv_data)
{
	கட்டமைப்பு xlate64 *xl = (கட்டமைப்பு xlate64 *) priv_data;
	முழு		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	திரும்பு ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				முழு	flags,
				எழுத்து *block_buf,
				முழு (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    முழு		ref_offset,
					    வெறுமை	*priv_data),
				வெறுமை *priv_data)
{
	கட்டமைப்பு xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	திரும்பு ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


கட்டமைப்பு xlate {
	முழு (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    முழு		bcount,
		    வெறுமை	*priv_data);
	வெறுமை *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
நிலையான முழு xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      முழு ref_offset EXT2FS_ATTR((unused)),
		      வெறுமை *priv_data)
{
	கட்டமைப்பு xlate *xl = (கட்டமைப்பு xlate *) priv_data;

	திரும்பு (*xl->func)(fs, blocknr, (முழு) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       முழு	flags,
			       எழுத்து *block_buf,
			       முழு (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   முழு	blockcnt,
					   வெறுமை	*priv_data),
			       வெறுமை *priv_data)
{
	கட்டமைப்பு xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	திரும்பு ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

