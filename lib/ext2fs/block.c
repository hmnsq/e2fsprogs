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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ꦪꦼꦤ꧀ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

sa_nracanaa block_context {
	ext2_filsys	fs;
	puur_naa_nka (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    puur_naa_nka		ref_offset,
		    void	*priv_data);
	e2_blkcnt_t	bcount;
	puur_naa_nka		bsize;
	puur_naa_nka		flags;
	errcode_t	errcode;
	var_na	*ind_buf;
	var_na	*dind_buf;
	var_na	*tind_buf;
	void	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	do {								\
		ꦪꦼꦤ꧀ (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			ꦮꦭꦶ ret;					\
		}							\
	} ꦱꦼꦢꦼꦭ (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	do {								\
		ꦪꦼꦤ꧀ (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			goto label;					\
		}							\
	} ꦱꦼꦢꦼꦭ (0)

static puur_naa_nka block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     puur_naa_nka ref_offset, sa_nracanaa block_context *ctx)
{
	puur_naa_nka	ret = 0, changed = 0;
	puur_naa_nka	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ꦪꦼꦤ꧀ (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_COUNT_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ꦪꦼꦤ꧀ (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		ꦮꦭꦶ ret;
	}
	ꦪꦼꦤ꧀ (*ind_block >= ext2fs_blocks_count(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		ꦮꦭꦶ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	ꦪꦼꦤ꧀ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ꦮꦭꦶ ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	ꦪꦼꦤ꧀ (ctx->flags & BLOCK_FLAG_APPEND) {
		ꦏꦁꦒꦺꦴ (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ꦪꦼꦤ꧀ (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				bha_nga;
			}
			offset += parimaa_na(blk_t);
		}
	} ꦭꦶꦪ {
		ꦏꦁꦒꦺꦴ (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			ꦪꦼꦤ꧀ (*block_nr == 0)
				goto skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ꦪꦼꦤ꧀ (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				bha_nga;
			}
		skip_sparse:
			offset += parimaa_na(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ꦪꦼꦤ꧀ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		ꦪꦼꦤ꧀ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ꦪꦼꦤ꧀ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_COUNT_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ꦮꦭꦶ ret;
}

static puur_naa_nka block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      puur_naa_nka ref_offset, sa_nracanaa block_context *ctx)
{
	puur_naa_nka	ret = 0, changed = 0;
	puur_naa_nka	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ꦪꦼꦤ꧀ (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_COUNT_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ꦪꦼꦤ꧀ (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		ꦮꦭꦶ ret;
	}
	ꦪꦼꦤ꧀ (*dind_block >= ext2fs_blocks_count(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		ꦮꦭꦶ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	ꦪꦼꦤ꧀ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ꦮꦭꦶ ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	ꦪꦼꦤ꧀ (ctx->flags & BLOCK_FLAG_APPEND) {
		ꦏꦁꦒꦺꦴ (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ꦪꦼꦤ꧀ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				bha_nga;
			}
			offset += parimaa_na(blk_t);
		}
	} ꦭꦶꦪ {
		ꦏꦁꦒꦺꦴ (i = 0; i < limit; i++, block_nr++) {
			ꦪꦼꦤ꧀ (*block_nr == 0) {
				ctx->bcount += limit;
				continue;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ꦪꦼꦤ꧀ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				bha_nga;
			}
			offset += parimaa_na(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ꦪꦼꦤ꧀ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		ꦪꦼꦤ꧀ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ꦪꦼꦤ꧀ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_COUNT_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ꦮꦭꦶ ret;
}

static puur_naa_nka block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      puur_naa_nka ref_offset, sa_nracanaa block_context *ctx)
{
	puur_naa_nka	ret = 0, changed = 0;
	puur_naa_nka	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ꦪꦼꦤ꧀ (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_COUNT_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ꦪꦼꦤ꧀ (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		ꦮꦭꦶ ret;
	}
	ꦪꦼꦤ꧀ (*tind_block >= ext2fs_blocks_count(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		ꦮꦭꦶ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	ꦪꦼꦤ꧀ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ꦮꦭꦶ ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	ꦪꦼꦤ꧀ (ctx->flags & BLOCK_FLAG_APPEND) {
		ꦏꦁꦒꦺꦴ (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ꦪꦼꦤ꧀ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				bha_nga;
			}
			offset += parimaa_na(blk_t);
		}
	} ꦭꦶꦪ {
		ꦏꦁꦒꦺꦴ (i = 0; i < limit; i++, block_nr++) {
			ꦪꦼꦤ꧀ (*block_nr == 0) {
				ctx->bcount += limit*limit;
				continue;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ꦪꦼꦤ꧀ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				bha_nga;
			}
			offset += parimaa_na(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ꦪꦼꦤ꧀ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		ꦪꦼꦤ꧀ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ꦪꦼꦤ꧀ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_COUNT_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ꦮꦭꦶ ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				puur_naa_nka	flags,
				var_na *block_buf,
				puur_naa_nka (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    puur_naa_nka		ref_offset,
					    void	*priv_data),
				void *priv_data)
{
	puur_naa_nka	i;
	puur_naa_nka	r, ret = 0;
	sa_nracanaa ext2_inode inode;
	errcode_t	retval;
	sa_nracanaa block_context ctx;
	puur_naa_nka	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	ꦪꦼꦤ꧀ (ctx.errcode)
		ꦮꦭꦶ ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	ꦪꦼꦤ꧀ (inode.i_flags & EXT4_INLINE_DATA_FL)
		ꦮꦭꦶ EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	ꦪꦼꦤ꧀ (flags & BLOCK_FLAG_NO_LARGE) {
		ꦪꦼꦤ꧀ (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_high != 0))
			ꦮꦭꦶ EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	ꦪꦼꦤ꧀ (block_buf) {
		ctx.ind_buf = block_buf;
	} ꦭꦶꦪ {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		ꦪꦼꦤ꧀ (retval)
			ꦮꦭꦶ retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	ꦪꦼꦤ꧀ ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		ꦪꦼꦤ꧀ (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_COUNT_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			ꦪꦼꦤ꧀ (ret & BLOCK_ABORT)
				goto abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	ꦪꦼꦤ꧀ (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		sa_nracanaa ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		puur_naa_nka			op = EXT2_EXTENT_ROOT;
		puur_naa_nka			uninit;
		unsigned puur_naa_nka		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		ꦪꦼꦤ꧀ (ctx.errcode)
			goto abort_exit;

		ꦱꦼꦢꦼꦭ (1) {
			ꦪꦼꦤ꧀ (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			ꦭꦶꦪ
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			ꦪꦼꦤ꧀ (ctx.errcode) {
				ꦪꦼꦤ꧀ (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					bha_nga;
				ctx.errcode = 0;
				ꦪꦼꦤ꧀ (!(flags & BLOCK_FLAG_APPEND))
					bha_nga;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ꦪꦼꦤ꧀ (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					ꦪꦼꦤ꧀ (ctx.errcode || (ret & BLOCK_ABORT))
						bha_nga;
					ꦪꦼꦤ꧀ (blk)
						goto next_block_set;
				}
				bha_nga;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			ꦪꦼꦤ꧀ (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				ꦪꦼꦤ꧀ (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					continue;
				ꦪꦼꦤ꧀ ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					ꦪꦼꦤ꧀ (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						ꦪꦼꦤ꧀ (ctx.errcode)
							bha_nga;
					}
					ꦪꦼꦤ꧀ (ret & BLOCK_ABORT)
						bha_nga;
				}
				continue;
			}
			uninit = 0;
			ꦪꦼꦤ꧀ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#ꦪꦼꦤ꧀ 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			ꦪꦼꦤ꧀ (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				continue;
			ꦪꦼꦤ꧀ (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			ꦏꦁꦒꦺꦴ (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ꦪꦼꦤ꧀ (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					ꦪꦼꦤ꧀ (ctx.errcode)
						goto extent_done;
				}
				ꦪꦼꦤ꧀ (ret & BLOCK_ABORT)
					goto extent_done;
			}
			ꦪꦼꦤ꧀ (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		goto errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	ꦏꦁꦒꦺꦴ (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		ꦪꦼꦤ꧀ (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			ꦪꦼꦤ꧀ (ret & BLOCK_ABORT)
				goto abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	ꦪꦼꦤ꧀ (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		ꦪꦼꦤ꧀ (ret & BLOCK_ABORT)
			goto abort_exit;
	} ꦭꦶꦪ
		ctx.bcount += limit;
	ꦪꦼꦤ꧀ (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		ꦪꦼꦤ꧀ (ret & BLOCK_ABORT)
			goto abort_exit;
	} ꦭꦶꦪ
		ctx.bcount += limit * limit;
	ꦪꦼꦤ꧀ (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		ꦪꦼꦤ꧀ (ret & BLOCK_ABORT)
			goto abort_exit;
	}

abort_exit:
	ꦪꦼꦤ꧀ (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		ꦪꦼꦤ꧀ (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	ꦪꦼꦤ꧀ (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	ꦮꦭꦶ (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

sa_nracanaa xlate64 {
	puur_naa_nka (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    puur_naa_nka		ref_offset,
		    void	*priv_data);
	void *real_private;
};

static puur_naa_nka xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			puur_naa_nka ref_offset, void *priv_data)
{
	sa_nracanaa xlate64 *xl = (sa_nracanaa xlate64 *) priv_data;
	puur_naa_nka		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	ꦮꦭꦶ ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				puur_naa_nka	flags,
				var_na *block_buf,
				puur_naa_nka (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    puur_naa_nka		ref_offset,
					    void	*priv_data),
				void *priv_data)
{
	sa_nracanaa xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	ꦮꦭꦶ ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


sa_nracanaa xlate {
	puur_naa_nka (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    puur_naa_nka		bcount,
		    void	*priv_data);
	void *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static puur_naa_nka xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      puur_naa_nka ref_offset EXT2FS_ATTR((unused)),
		      void *priv_data)
{
	sa_nracanaa xlate *xl = (sa_nracanaa xlate *) priv_data;

	ꦮꦭꦶ (*xl->func)(fs, blocknr, (puur_naa_nka) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       puur_naa_nka	flags,
			       var_na *block_buf,
			       puur_naa_nka (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   puur_naa_nka	blockcnt,
					   void	*priv_data),
			       void *priv_data)
{
	sa_nracanaa xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ꦮꦭꦶ ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

