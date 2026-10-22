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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಸಂರಚನಾ block_context {
	ext2_filsys	fs;
	ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    ಪೂರ್ಣಾಙ್ಕ		ref_offset,
		    ಶೂನ್ಯ	*priv_data);
	e2_blkcnt_t	bcount;
	ಪೂರ್ಣಾಙ್ಕ		bsize;
	ಪೂರ್ಣಾಙ್ಕ		flags;
	errcode_t	errcode;
	ವರ್ಣ	*ind_buf;
	ವರ್ಣ	*dind_buf;
	ವರ್ಣ	*tind_buf;
	ಶೂನ್ಯ	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	ಕುರು {								\
		ಯದಿ (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			ಪ್ರತ್ಯಾಗಮ ret;					\
		}							\
	} ಯಾವತ್ (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	ಕುರು {								\
		ಯದಿ (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			ಗಚ್ಛ label;					\
		}							\
	} ಯಾವತ್ (0)

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     ಪೂರ್ಣಾಙ್ಕ ref_offset, ಸಂರಚನಾ block_context *ctx)
{
	ಪೂರ್ಣಾಙ್ಕ	ret = 0, changed = 0;
	ಪೂರ್ಣಾಙ್ಕ	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ಯದಿ (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_ಎಣಿಕೆ_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಯದಿ (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		ಪ್ರತ್ಯಾಗಮ ret;
	}
	ಯದಿ (*ind_block >= ext2fs_blocks_ಎಣಿಕೆ(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		ಪ್ರತ್ಯಾಗಮ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	ಯದಿ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ಪ್ರತ್ಯಾಗಮ ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	ಯದಿ (ctx->flags & BLOCK_FLAG_APPEND) {
		ಪ್ರತಿ (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ಯದಿ (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				ಭಙ್ಗ;
			}
			offset += ಪರಿಮಾಣ(blk_t);
		}
	} ಅನ್ಯಥಾ {
		ಪ್ರತಿ (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			ಯದಿ (*block_nr == 0)
				ಗಚ್ಛ skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ಯದಿ (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				ಭಙ್ಗ;
			}
		skip_sparse:
			offset += ಪರಿಮಾಣ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ಯದಿ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		ಯದಿ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ಯದಿ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_ಎಣಿಕೆ_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      ಪೂರ್ಣಾಙ್ಕ ref_offset, ಸಂರಚನಾ block_context *ctx)
{
	ಪೂರ್ಣಾಙ್ಕ	ret = 0, changed = 0;
	ಪೂರ್ಣಾಙ್ಕ	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ಯದಿ (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_ಎಣಿಕೆ_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಯದಿ (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		ಪ್ರತ್ಯಾಗಮ ret;
	}
	ಯದಿ (*dind_block >= ext2fs_blocks_ಎಣಿಕೆ(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		ಪ್ರತ್ಯಾಗಮ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	ಯದಿ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ಪ್ರತ್ಯಾಗಮ ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	ಯದಿ (ctx->flags & BLOCK_FLAG_APPEND) {
		ಪ್ರತಿ (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ಯದಿ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ಭಙ್ಗ;
			}
			offset += ಪರಿಮಾಣ(blk_t);
		}
	} ಅನ್ಯಥಾ {
		ಪ್ರತಿ (i = 0; i < limit; i++, block_nr++) {
			ಯದಿ (*block_nr == 0) {
				ctx->bcount += limit;
				ಅನುವರ್ತ;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ಯದಿ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ಭಙ್ಗ;
			}
			offset += ಪರಿಮಾಣ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ಯದಿ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		ಯದಿ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ಯದಿ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_ಎಣಿಕೆ_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      ಪೂರ್ಣಾಙ್ಕ ref_offset, ಸಂರಚನಾ block_context *ctx)
{
	ಪೂರ್ಣಾಙ್ಕ	ret = 0, changed = 0;
	ಪೂರ್ಣಾಙ್ಕ	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ಯದಿ (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_ಎಣಿಕೆ_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಯದಿ (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		ಪ್ರತ್ಯಾಗಮ ret;
	}
	ಯದಿ (*tind_block >= ext2fs_blocks_ಎಣಿಕೆ(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		ಪ್ರತ್ಯಾಗಮ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	ಯದಿ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ಪ್ರತ್ಯಾಗಮ ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	ಯದಿ (ctx->flags & BLOCK_FLAG_APPEND) {
		ಪ್ರತಿ (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ಯದಿ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ಭಙ್ಗ;
			}
			offset += ಪರಿಮಾಣ(blk_t);
		}
	} ಅನ್ಯಥಾ {
		ಪ್ರತಿ (i = 0; i < limit; i++, block_nr++) {
			ಯದಿ (*block_nr == 0) {
				ctx->bcount += limit*limit;
				ಅನುವರ್ತ;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ಯದಿ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				ಭಙ್ಗ;
			}
			offset += ಪರಿಮಾಣ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ಯದಿ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		ಯದಿ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ಯದಿ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_ಎಣಿಕೆ_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ಪ್ರತ್ಯಾಗಮ ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				ಪೂರ್ಣಾಙ್ಕ	flags,
				ವರ್ಣ *block_buf,
				ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    ಪೂರ್ಣಾಙ್ಕ		ref_offset,
					    ಶೂನ್ಯ	*priv_data),
				ಶೂನ್ಯ *priv_data)
{
	ಪೂರ್ಣಾಙ್ಕ	i;
	ಪೂರ್ಣಾಙ್ಕ	r, ret = 0;
	ಸಂರಚನಾ ext2_inode inode;
	errcode_t	retval;
	ಸಂರಚನಾ block_context ctx;
	ಪೂರ್ಣಾಙ್ಕ	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	ಯದಿ (ctx.errcode)
		ಪ್ರತ್ಯಾಗಮ ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	ಯದಿ (inode.i_flags & EXT4_INLINE_DATA_FL)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	ಯದಿ (flags & BLOCK_FLAG_NO_LARGE) {
		ಯದಿ (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_ಮೇಲಿನ != 0))
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	ಯದಿ (block_buf) {
		ctx.ind_buf = block_buf;
	} ಅನ್ಯಥಾ {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	ಯದಿ ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		ಯದಿ (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_ಎಣಿಕೆ_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			ಯದಿ (ret & BLOCK_ABORT)
				ಗಚ್ಛ abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	ಯದಿ (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		ಸಂರಚನಾ ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		ಪೂರ್ಣಾಙ್ಕ			op = EXT2_EXTENT_ಬೇರು;
		ಪೂರ್ಣಾಙ್ಕ			uninit;
		unsigned ಪೂರ್ಣಾಙ್ಕ		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		ಯದಿ (ctx.errcode)
			ಗಚ್ಛ abort_exit;

		ಯಾವತ್ (1) {
			ಯದಿ (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			ಅನ್ಯಥಾ
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			ಯದಿ (ctx.errcode) {
				ಯದಿ (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					ಭಙ್ಗ;
				ctx.errcode = 0;
				ಯದಿ (!(flags & BLOCK_FLAG_APPEND))
					ಭಙ್ಗ;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ಯದಿ (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					ಯದಿ (ctx.errcode || (ret & BLOCK_ABORT))
						ಭಙ್ಗ;
					ಯದಿ (blk)
						ಗಚ್ಛ next_block_set;
				}
				ಭಙ್ಗ;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			ಯದಿ (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				ಯದಿ (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					ಅನುವರ್ತ;
				ಯದಿ ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					ಯದಿ (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						ಯದಿ (ctx.errcode)
							ಭಙ್ಗ;
					}
					ಯದಿ (ret & BLOCK_ABORT)
						ಭಙ್ಗ;
				}
				ಅನುವರ್ತ;
			}
			uninit = 0;
			ಯದಿ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#ಯದಿ 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			ಯದಿ (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				ಅನುವರ್ತ;
			ಯದಿ (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			ಪ್ರತಿ (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ಯದಿ (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					ಯದಿ (ctx.errcode)
						ಗಚ್ಛ extent_done;
				}
				ಯದಿ (ret & BLOCK_ABORT)
					ಗಚ್ಛ extent_done;
			}
			ಯದಿ (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		ಗಚ್ಛ errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	ಪ್ರತಿ (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		ಯದಿ (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			ಯದಿ (ret & BLOCK_ABORT)
				ಗಚ್ಛ abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	ಯದಿ (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		ಯದಿ (ret & BLOCK_ABORT)
			ಗಚ್ಛ abort_exit;
	} ಅನ್ಯಥಾ
		ctx.bcount += limit;
	ಯದಿ (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		ಯದಿ (ret & BLOCK_ABORT)
			ಗಚ್ಛ abort_exit;
	} ಅನ್ಯಥಾ
		ctx.bcount += limit * limit;
	ಯದಿ (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		ಯದಿ (ret & BLOCK_ABORT)
			ಗಚ್ಛ abort_exit;
	}

abort_exit:
	ಯದಿ (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		ಯದಿ (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	ಯದಿ (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	ಪ್ರತ್ಯಾಗಮ (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

ಸಂರಚನಾ xlate64 {
	ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    ಪೂರ್ಣಾಙ್ಕ		ref_offset,
		    ಶೂನ್ಯ	*priv_data);
	ಶೂನ್ಯ *real_private;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			ಪೂರ್ಣಾಙ್ಕ ref_offset, ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ xlate64 *xl = (ಸಂರಚನಾ xlate64 *) priv_data;
	ಪೂರ್ಣಾಙ್ಕ		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	ಪ್ರತ್ಯಾಗಮ ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				ಪೂರ್ಣಾಙ್ಕ	flags,
				ವರ್ಣ *block_buf,
				ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    ಪೂರ್ಣಾಙ್ಕ		ref_offset,
					    ಶೂನ್ಯ	*priv_data),
				ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	ಪ್ರತ್ಯಾಗಮ ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


ಸಂರಚನಾ xlate {
	ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    ಪೂರ್ಣಾಙ್ಕ		bcount,
		    ಶೂನ್ಯ	*priv_data);
	ಶೂನ್ಯ *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      ಪೂರ್ಣಾಙ್ಕ ref_offset EXT2FS_ATTR((unused)),
		      ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ xlate *xl = (ಸಂರಚನಾ xlate *) priv_data;

	ಪ್ರತ್ಯಾಗಮ (*xl->func)(fs, blocknr, (ಪೂರ್ಣಾಙ್ಕ) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       ಪೂರ್ಣಾಙ್ಕ	flags,
			       ವರ್ಣ *block_buf,
			       ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   ಪೂರ್ಣಾಙ್ಕ	blockcnt,
					   ಶೂನ್ಯ	*priv_data),
			       ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ಪ್ರತ್ಯಾಗಮ ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

