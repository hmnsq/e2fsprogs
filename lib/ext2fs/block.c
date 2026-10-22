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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

কাঠামো block_context {
	ext2_filsys	fs;
	পূর্ণসংখ্যা (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    পূর্ণসংখ্যা		ref_offset,
		    শূন্য	*priv_তথ্য);
	e2_blkcnt_t	bcount;
	পূর্ণসংখ্যা		bsize;
	পূর্ণসংখ্যা		flags;
	errcode_t	errcode;
	অক্ষর	*ind_buf;
	অক্ষর	*dind_buf;
	অক্ষর	*tind_buf;
	শূন্য	*priv_তথ্য;
};

#define check_for_ro_violation_return(ctx, ret)				\
	করো {								\
		যদি (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			ফেরত ret;					\
		}							\
	} যখন (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	করো {								\
		যদি (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			যাও label;					\
		}							\
	} যখন (0)

স্থির পূর্ণসংখ্যা block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     পূর্ণসংখ্যা ref_offset, কাঠামো block_context *ctx)
{
	পূর্ণসংখ্যা	ret = 0, changed = 0;
	পূর্ণসংখ্যা	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	যদি (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_তথ্য_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_গুনতি_IND, ref_block,
				   ref_offset, ctx->priv_তথ্য);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	যদি (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		ফেরত ret;
	}
	যদি (*ind_block >= ext2fs_blocks_গুনতি(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_তথ্য_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		ফেরত ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	যদি (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ফেরত ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	যদি (ctx->flags & BLOCK_FLAG_APPEND) {
		জন্য (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_তথ্য);
			*block_nr = blk64;
			changed	|= flags;
			যদি (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				বিরতি;
			}
			offset += আকার(blk_t);
		}
	} অন্যথা {
		জন্য (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			যদি (*block_nr == 0)
				যাও skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_তথ্য);
			*block_nr = blk64;
			changed	|= flags;
			যদি (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				বিরতি;
			}
		skip_sparse:
			offset += আকার(blk_t);
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
	    !(ctx->flags & BLOCK_FLAG_তথ্য_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_গুনতি_IND, ref_block,
				    ref_offset, ctx->priv_তথ্য);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      পূর্ণসংখ্যা ref_offset, কাঠামো block_context *ctx)
{
	পূর্ণসংখ্যা	ret = 0, changed = 0;
	পূর্ণসংখ্যা	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	যদি (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_তথ্য_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_গুনতি_DIND, ref_block,
				   ref_offset, ctx->priv_তথ্য);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	যদি (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		ফেরত ret;
	}
	যদি (*dind_block >= ext2fs_blocks_গুনতি(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_তথ্য_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		ফেরত ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	যদি (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ফেরত ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	যদি (ctx->flags & BLOCK_FLAG_APPEND) {
		জন্য (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			যদি (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				বিরতি;
			}
			offset += আকার(blk_t);
		}
	} অন্যথা {
		জন্য (i = 0; i < limit; i++, block_nr++) {
			যদি (*block_nr == 0) {
				ctx->bcount += limit;
				চালু;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			যদি (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				বিরতি;
			}
			offset += আকার(blk_t);
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
	    !(ctx->flags & BLOCK_FLAG_তথ্য_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_গুনতি_DIND, ref_block,
				    ref_offset, ctx->priv_তথ্য);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      পূর্ণসংখ্যা ref_offset, কাঠামো block_context *ctx)
{
	পূর্ণসংখ্যা	ret = 0, changed = 0;
	পূর্ণসংখ্যা	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	যদি (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_তথ্য_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_গুনতি_TIND, ref_block,
				   ref_offset, ctx->priv_তথ্য);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	যদি (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		ফেরত ret;
	}
	যদি (*tind_block >= ext2fs_blocks_গুনতি(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_তথ্য_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		ফেরত ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	যদি (ctx->errcode) {
		ret |= BLOCK_ERROR;
		ফেরত ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	যদি (ctx->flags & BLOCK_FLAG_APPEND) {
		জন্য (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			যদি (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				বিরতি;
			}
			offset += আকার(blk_t);
		}
	} অন্যথা {
		জন্য (i = 0; i < limit; i++, block_nr++) {
			যদি (*block_nr == 0) {
				ctx->bcount += limit*limit;
				চালু;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			যদি (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				বিরতি;
			}
			offset += আকার(blk_t);
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
	    !(ctx->flags & BLOCK_FLAG_তথ্য_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_গুনতি_TIND, ref_block,
				    ref_offset, ctx->priv_তথ্য);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	ফেরত ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				পূর্ণসংখ্যা	flags,
				অক্ষর *block_buf,
				পূর্ণসংখ্যা (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    পূর্ণসংখ্যা		ref_offset,
					    শূন্য	*priv_তথ্য),
				শূন্য *priv_তথ্য)
{
	পূর্ণসংখ্যা	i;
	পূর্ণসংখ্যা	r, ret = 0;
	কাঠামো ext2_inode inode;
	errcode_t	retval;
	কাঠামো block_context ctx;
	পূর্ণসংখ্যা	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	যদি (ctx.errcode)
		ফেরত ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	যদি (inode.i_flags & EXT4_INLINE_তথ্য_FL)
		ফেরত EXT2_ET_INLINE_তথ্য_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	যদি (flags & BLOCK_FLAG_NO_LARGE) {
		যদি (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_আকার_উপরের != 0))
			ফেরত EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_তথ্য = priv_তথ্য;
	ctx.flags = flags;
	ctx.bcount = 0;
	যদি (block_buf) {
		ctx.ind_buf = block_buf;
	} অন্যথা {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		যদি (retval)
			ফেরত retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	যদি ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_তথ্য_ONLY)) {
		যদি (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_গুনতি_TRANSLATOR,
					   0, 0, priv_তথ্য);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			যদি (ret & BLOCK_ABORT)
				যাও abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	যদি (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		কাঠামো ext2fs_extent	extent, পরবর্তী;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		পূর্ণসংখ্যা			op = EXT2_EXTENT_মূল;
		পূর্ণসংখ্যা			uninit;
		unsigned পূর্ণসংখ্যা		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		যদি (ctx.errcode)
			যাও abort_exit;

		যখন (1) {
			যদি (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			অন্যথা
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			যদি (ctx.errcode) {
				যদি (ctx.errcode != EXT2_ET_EXTENT_NO_পরবর্তী)
					বিরতি;
				ctx.errcode = 0;
				যদি (!(flags & BLOCK_FLAG_APPEND))
					বিরতি;
			পরবর্তী_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_তথ্য);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				যদি (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					যদি (ctx.errcode || (ret & BLOCK_ABORT))
						বিরতি;
					যদি (blk)
						যাও পরবর্তী_block_set;
				}
				বিরতি;
			}

			op = EXT2_EXTENT_পরবর্তী;
			blk = extent.e_pblk;
			যদি (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				যদি (ctx.flags & BLOCK_FLAG_তথ্য_ONLY)
					চালু;
				যদি ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_তথ্য);
					যদি (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						যদি (ctx.errcode)
							বিরতি;
					}
					যদি (ret & BLOCK_ABORT)
						বিরতি;
				}
				চালু;
			}
			uninit = 0;
			যদি (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &পরবর্তী);

#যদি 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			যদি (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				চালু;
			যদি (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			জন্য (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_তথ্য);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				যদি (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					যদি (ctx.errcode)
						যাও extent_done;
				}
				যদি (ret & BLOCK_ABORT)
					যাও extent_done;
			}
			যদি (retval == 0) {
				extent = পরবর্তী;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		যাও errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	জন্য (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		যদি (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_তথ্য);
			inode.i_block[i] = (blk_t) blk64;
			যদি (ret & BLOCK_ABORT)
				যাও abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	যদি (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		যদি (ret & BLOCK_ABORT)
			যাও abort_exit;
	} অন্যথা
		ctx.bcount += limit;
	যদি (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		যদি (ret & BLOCK_ABORT)
			যাও abort_exit;
	} অন্যথা
		ctx.bcount += limit * limit;
	যদি (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		যদি (ret & BLOCK_ABORT)
			যাও abort_exit;
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

	ফেরত (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

কাঠামো xlate64 {
	পূর্ণসংখ্যা (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    পূর্ণসংখ্যা		ref_offset,
		    শূন্য	*priv_তথ্য);
	শূন্য *real_private;
};

স্থির পূর্ণসংখ্যা xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			পূর্ণসংখ্যা ref_offset, শূন্য *priv_তথ্য)
{
	কাঠামো xlate64 *xl = (কাঠামো xlate64 *) priv_তথ্য;
	পূর্ণসংখ্যা		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	ফেরত ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				পূর্ণসংখ্যা	flags,
				অক্ষর *block_buf,
				পূর্ণসংখ্যা (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    পূর্ণসংখ্যা		ref_offset,
					    শূন্য	*priv_তথ্য),
				শূন্য *priv_তথ্য)
{
	কাঠামো xlate64 xl;

	xl.real_private = priv_তথ্য;
	xl.func = func;

	ফেরত ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


কাঠামো xlate {
	পূর্ণসংখ্যা (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    পূর্ণসংখ্যা		bcount,
		    শূন্য	*priv_তথ্য);
	শূন্য *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণসংখ্যা xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      পূর্ণসংখ্যা ref_offset EXT2FS_ATTR((unused)),
		      শূন্য *priv_তথ্য)
{
	কাঠামো xlate *xl = (কাঠামো xlate *) priv_তথ্য;

	ফেরত (*xl->func)(fs, blocknr, (পূর্ণসংখ্যা) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       পূর্ণসংখ্যা	flags,
			       অক্ষর *block_buf,
			       পূর্ণসংখ্যা (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   পূর্ণসংখ্যা	blockcnt,
					   শূন্য	*priv_তথ্য),
			       শূন্য *priv_তথ্য)
{
	কাঠামো xlate xl;

	xl.real_private = priv_তথ্য;
	xl.func = func;

	ফেরত ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

