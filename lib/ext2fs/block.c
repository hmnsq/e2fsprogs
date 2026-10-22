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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

সংরচনা block_context {
	ext2_filsys	fs;
	পূর্ণাঙ্ক (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    পূর্ণাঙ্ক		ref_offset,
		    শূন্য	*priv_data);
	e2_blkcnt_t	bcount;
	পূর্ণাঙ্ক		bsize;
	পূর্ণাঙ্ক		flags;
	errcode_t	errcode;
	ৱর্ণ	*ind_buf;
	ৱর্ণ	*dind_buf;
	ৱর্ণ	*tind_buf;
	শূন্য	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	কৰা {								\
		যদি (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			ঘূৰাই ret;					\
		}							\
	} যেতিয়ালৈ (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	কৰা {								\
		যদি (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			গচ্ছ label;					\
		}							\
	} যেতিয়ালৈ (0)

স্থির পূর্ণাঙ্ক block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     পূর্ণাঙ্ক ref_offset, সংরচনা block_context *ctx)
{
	পূর্ণাঙ্ক	ret = 0, changed = 0;
	পূর্ণাঙ্ক	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	যদি (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_গুনতি_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	যদি (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		ঘূৰাই ret;
	}
	যদি (*ind_block >= ext2fs_blocks_গুনতি(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		ঘূৰাই ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	যদি (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ঘূৰাই ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	যদি (ctx->flags & BLOCK_FLAG_APPEND) {
		বাবে (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			যদি (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				ভঙ্গ;
			}
			offset += পরিমাণ(blk_t);
		}
	} অন্যথা {
		বাবে (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			যদি (*block_nr == 0)
				গচ্ছ skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			যদি (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				ভঙ্গ;
			}
		skip_sparse:
			offset += পরিমাণ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	যদি (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		যদি (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	যদি ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_গুনতি_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      পূর্ণাঙ্ক ref_offset, সংরচনা block_context *ctx)
{
	পূর্ণাঙ্ক	ret = 0, changed = 0;
	পূর্ণাঙ্ক	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	যদি (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_গুনতি_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	যদি (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		ঘূৰাই ret;
	}
	যদি (*dind_block >= ext2fs_blocks_গুনতি(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		ঘূৰাই ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	যদি (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ঘূৰাই ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	যদি (ctx->flags & BLOCK_FLAG_APPEND) {
		বাবে (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			যদি (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ভঙ্গ;
			}
			offset += পরিমাণ(blk_t);
		}
	} অন্যথা {
		বাবে (i = 0; i < limit; i++, block_nr++) {
			যদি (*block_nr == 0) {
				ctx->bcount += limit;
				অনুৱর্ত;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			যদি (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ভঙ্গ;
			}
			offset += পরিমাণ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	যদি (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		যদি (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	যদি ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_গুনতি_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      পূর্ণাঙ্ক ref_offset, সংরচনা block_context *ctx)
{
	পূর্ণাঙ্ক	ret = 0, changed = 0;
	পূর্ণাঙ্ক	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	যদি (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_গুনতি_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	যদি (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		ঘূৰাই ret;
	}
	যদি (*tind_block >= ext2fs_blocks_গুনতি(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		ঘূৰাই ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	যদি (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ঘূৰাই ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	যদি (ctx->flags & BLOCK_FLAG_APPEND) {
		বাবে (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			যদি (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ভঙ্গ;
			}
			offset += পরিমাণ(blk_t);
		}
	} অন্যথা {
		বাবে (i = 0; i < limit; i++, block_nr++) {
			যদি (*block_nr == 0) {
				ctx->bcount += limit*limit;
				অনুৱর্ত;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			যদি (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ভঙ্গ;
			}
			offset += পরিমাণ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	যদি (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		যদি (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	যদি ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_গুনতি_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ঘূৰাই ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				পূর্ণাঙ্ক	flags,
				ৱর্ণ *block_buf,
				পূর্ণাঙ্ক (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    পূর্ণাঙ্ক		ref_offset,
					    শূন্য	*priv_data),
				শূন্য *priv_data)
{
	পূর্ণাঙ্ক	i;
	পূর্ণাঙ্ক	r, ret = 0;
	সংরচনা ext2_inode inode;
	errcode_t	retval;
	সংরচনা block_context ctx;
	পূর্ণাঙ্ক	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	যদি (ctx.errcode)
		ঘূৰাই ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	যদি (inode.i_flags & EXT4_INLINE_DATA_FL)
		ঘূৰাই EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	যদি (flags & BLOCK_FLAG_NO_LARGE) {
		যদি (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_উপৰেৰ != 0))
			ঘূৰাই EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	যদি (block_buf) {
		ctx.ind_buf = block_buf;
	} অন্যথা {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		যদি (retval)
			ঘূৰাই retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	যদি ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		যদি (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_গুনতি_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			যদি (ret & BLOCK_ABORT)
				গচ্ছ abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	যদি (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		সংরচনা ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		পূর্ণাঙ্ক			op = EXT2_EXTENT_মূল;
		পূর্ণাঙ্ক			uninit;
		unsigned পূর্ণাঙ্ক		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		যদি (ctx.errcode)
			গচ্ছ abort_exit;

		যেতিয়ালৈ (1) {
			যদি (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			অন্যথা
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			যদি (ctx.errcode) {
				যদি (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					ভঙ্গ;
				ctx.errcode = 0;
				যদি (!(flags & BLOCK_FLAG_APPEND))
					ভঙ্গ;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				যদি (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					যদি (ctx.errcode || (ret & BLOCK_ABORT))
						ভঙ্গ;
					যদি (blk)
						গচ্ছ next_block_set;
				}
				ভঙ্গ;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			যদি (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				যদি (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					অনুৱর্ত;
				যদি ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					যদি (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						যদি (ctx.errcode)
							ভঙ্গ;
					}
					যদি (ret & BLOCK_ABORT)
						ভঙ্গ;
				}
				অনুৱর্ত;
			}
			uninit = 0;
			যদি (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#যদি 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			যদি (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				অনুৱর্ত;
			যদি (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			বাবে (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				যদি (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					যদি (ctx.errcode)
						গচ্ছ extent_done;
				}
				যদি (ret & BLOCK_ABORT)
					গচ্ছ extent_done;
			}
			যদি (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		গচ্ছ errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	বাবে (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		যদি (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			যদি (ret & BLOCK_ABORT)
				গচ্ছ abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	যদি (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		যদি (ret & BLOCK_ABORT)
			গচ্ছ abort_exit;
	} অন্যথা
		ctx.bcount += limit;
	যদি (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		যদি (ret & BLOCK_ABORT)
			গচ্ছ abort_exit;
	} অন্যথা
		ctx.bcount += limit * limit;
	যদি (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		যদি (ret & BLOCK_ABORT)
			গচ্ছ abort_exit;
	}

abort_exit:
	যদি (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		যদি (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	যদি (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	ঘূৰাই (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

সংরচনা xlate64 {
	পূর্ণাঙ্ক (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    পূর্ণাঙ্ক		ref_offset,
		    শূন্য	*priv_data);
	শূন্য *real_private;
};

স্থির পূর্ণাঙ্ক xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			পূর্ণাঙ্ক ref_offset, শূন্য *priv_data)
{
	সংরচনা xlate64 *xl = (সংরচনা xlate64 *) priv_data;
	পূর্ণাঙ্ক		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	ঘূৰাই ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				পূর্ণাঙ্ক	flags,
				ৱর্ণ *block_buf,
				পূর্ণাঙ্ক (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    পূর্ণাঙ্ক		ref_offset,
					    শূন্য	*priv_data),
				শূন্য *priv_data)
{
	সংরচনা xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	ঘূৰাই ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


সংরচনা xlate {
	পূর্ণাঙ্ক (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    পূর্ণাঙ্ক		bcount,
		    শূন্য	*priv_data);
	শূন্য *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণাঙ্ক xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      পূর্ণাঙ্ক ref_offset EXT2FS_ATTR((unused)),
		      শূন্য *priv_data)
{
	সংরচনা xlate *xl = (সংরচনা xlate *) priv_data;

	ঘূৰাই (*xl->func)(fs, blocknr, (পূর্ণাঙ্ক) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       পূর্ণাঙ্ক	flags,
			       ৱর্ণ *block_buf,
			       পূর্ণাঙ্ক (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   পূর্ণাঙ্ক	blockcnt,
					   শূন্য	*priv_data),
			       শূন্য *priv_data)
{
	সংরচনা xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ঘূৰাই ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

