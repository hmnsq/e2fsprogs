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
#ако HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

struct block_context {
	ext2_filsys	fs;
	цяло (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    цяло		ref_offset,
		    void	*priv_data);
	e2_blkcnt_t	bcount;
	цяло		bsize;
	цяло		flags;
	errcode_t	errcode;
	char	*ind_buf;
	char	*dind_buf;
	char	*tind_buf;
	void	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	do {								\
		ако (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			върни ret;					\
		}							\
	} докато (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	do {								\
		ако (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			goto label;					\
		}							\
	} докато (0)

static цяло block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     цяло ref_offset, struct block_context *ctx)
{
	цяло	ret = 0, changed = 0;
	цяло	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ако (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_COUNT_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ако (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		върни ret;
	}
	ако (*ind_block >= ext2fs_blocks_count(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		върни ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	ако (ctx->errcode) {
		ret |= BLOCK_ERROR;
		върни ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	ако (ctx->flags & BLOCK_FLAG_APPEND) {
		за (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ако (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				break;
			}
			offset += sizeof(blk_t);
		}
	} иначе {
		за (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			ако (*block_nr == 0)
				goto skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			ако (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				break;
			}
		skip_sparse:
			offset += sizeof(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ако (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		ако (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ако ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_COUNT_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	върни ret;
}

static цяло block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      цяло ref_offset, struct block_context *ctx)
{
	цяло	ret = 0, changed = 0;
	цяло	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ако (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_COUNT_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ако (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		върни ret;
	}
	ако (*dind_block >= ext2fs_blocks_count(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		върни ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	ако (ctx->errcode) {
		ret |= BLOCK_ERROR;
		върни ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	ако (ctx->flags & BLOCK_FLAG_APPEND) {
		за (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ако (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				break;
			}
			offset += sizeof(blk_t);
		}
	} иначе {
		за (i = 0; i < limit; i++, block_nr++) {
			ако (*block_nr == 0) {
				ctx->bcount += limit;
				continue;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			ако (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				break;
			}
			offset += sizeof(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ако (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		ако (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ако ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_COUNT_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	върни ret;
}

static цяло block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      цяло ref_offset, struct block_context *ctx)
{
	цяло	ret = 0, changed = 0;
	цяло	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	ако (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_COUNT_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ако (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		върни ret;
	}
	ако (*tind_block >= ext2fs_blocks_count(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		върни ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	ако (ctx->errcode) {
		ret |= BLOCK_ERROR;
		върни ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	ако (ctx->flags & BLOCK_FLAG_APPEND) {
		за (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ако (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				break;
			}
			offset += sizeof(blk_t);
		}
	} иначе {
		за (i = 0; i < limit; i++, block_nr++) {
			ако (*block_nr == 0) {
				ctx->bcount += limit*limit;
				continue;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			ако (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				break;
			}
			offset += sizeof(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	ако (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		ако (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	ако ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_COUNT_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	върни ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				цяло	flags,
				char *block_buf,
				цяло (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    цяло		ref_offset,
					    void	*priv_data),
				void *priv_data)
{
	цяло	i;
	цяло	r, ret = 0;
	struct ext2_inode inode;
	errcode_t	retval;
	struct block_context ctx;
	цяло	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	ако (ctx.errcode)
		върни ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	ако (inode.i_flags & EXT4_INLINE_DATA_FL)
		върни EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	ако (flags & BLOCK_FLAG_NO_LARGE) {
		ако (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_high != 0))
			върни EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	ако (block_buf) {
		ctx.ind_buf = block_buf;
	} иначе {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		ако (retval)
			върни retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	ако ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		ако (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_COUNT_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			ако (ret & BLOCK_ABORT)
				goto abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	ако (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		struct ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		цяло			op = EXT2_EXTENT_ROOT;
		цяло			uninit;
		unsigned цяло		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		ако (ctx.errcode)
			goto abort_exit;

		докато (1) {
			ако (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			иначе
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			ако (ctx.errcode) {
				ако (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					break;
				ctx.errcode = 0;
				ако (!(flags & BLOCK_FLAG_APPEND))
					break;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ако (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					ако (ctx.errcode || (ret & BLOCK_ABORT))
						break;
					ако (blk)
						goto next_block_set;
				}
				break;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			ако (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				ако (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					continue;
				ако ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					ако (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						ако (ctx.errcode)
							break;
					}
					ако (ret & BLOCK_ABORT)
						break;
				}
				continue;
			}
			uninit = 0;
			ако (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#ако 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			ако (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				continue;
			ако (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			за (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				ако (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					ако (ctx.errcode)
						goto extent_done;
				}
				ако (ret & BLOCK_ABORT)
					goto extent_done;
			}
			ако (retval == 0) {
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
	за (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		ако (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			ако (ret & BLOCK_ABORT)
				goto abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	ако (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		ако (ret & BLOCK_ABORT)
			goto abort_exit;
	} иначе
		ctx.bcount += limit;
	ако (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		ако (ret & BLOCK_ABORT)
			goto abort_exit;
	} иначе
		ctx.bcount += limit * limit;
	ако (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		ако (ret & BLOCK_ABORT)
			goto abort_exit;
	}

abort_exit:
	ако (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		ако (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	ако (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	върни (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

struct xlate64 {
	цяло (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    цяло		ref_offset,
		    void	*priv_data);
	void *real_private;
};

static цяло xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			цяло ref_offset, void *priv_data)
{
	struct xlate64 *xl = (struct xlate64 *) priv_data;
	цяло		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	върни ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				цяло	flags,
				char *block_buf,
				цяло (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    цяло		ref_offset,
					    void	*priv_data),
				void *priv_data)
{
	struct xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	върни ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


struct xlate {
	цяло (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    цяло		bcount,
		    void	*priv_data);
	void *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
static цяло xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      цяло ref_offset EXT2FS_ATTR((unused)),
		      void *priv_data)
{
	struct xlate *xl = (struct xlate *) priv_data;

	върни (*xl->func)(fs, blocknr, (цяло) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       цяло	flags,
			       char *block_buf,
			       цяло (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   цяло	blockcnt,
					   void	*priv_data),
			       void *priv_data)
{
	struct xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	върни ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

