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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

સંરચના block_context {
	ext2_filsys	fs;
	પૂર્ણાંક (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    પૂર્ણાંક		ref_offset,
		    શૂન્ય	*priv_data);
	e2_blkcnt_t	bcount;
	પૂર્ણાંક		bsize;
	પૂર્ણાંક		flags;
	errcode_t	errcode;
	અક્ષર	*ind_buf;
	અક્ષર	*dind_buf;
	અક્ષર	*tind_buf;
	શૂન્ય	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	કરો {								\
		જો (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			પાછા ret;					\
		}							\
	} જ્યારે (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	કરો {								\
		જો (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			જાઓ label;					\
		}							\
	} જ્યારે (0)

સ્થિર પૂર્ણાંક block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     પૂર્ણાંક ref_offset, સંરચના block_context *ctx)
{
	પૂર્ણાંક	ret = 0, changed = 0;
	પૂર્ણાંક	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	જો (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_ગણતરી_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	જો (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		પાછા ret;
	}
	જો (*ind_block >= ext2fs_blocks_ગણતરી(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		પાછા ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	જો (ctx->errcode) {
		ret |= BLOCK_ERROR;
		પાછા ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	જો (ctx->flags & BLOCK_FLAG_APPEND) {
		માટે (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			જો (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				તોડો;
			}
			offset += કદ(blk_t);
		}
	} નહીંતર {
		માટે (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			જો (*block_nr == 0)
				જાઓ skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			જો (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				તોડો;
			}
		skip_sparse:
			offset += કદ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	જો (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		જો (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	જો ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_ગણતરી_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	પાછા ret;
}

સ્થિર પૂર્ણાંક block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      પૂર્ણાંક ref_offset, સંરચના block_context *ctx)
{
	પૂર્ણાંક	ret = 0, changed = 0;
	પૂર્ણાંક	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	જો (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_ગણતરી_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	જો (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		પાછા ret;
	}
	જો (*dind_block >= ext2fs_blocks_ગણતરી(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		પાછા ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	જો (ctx->errcode) {
		ret |= BLOCK_ERROR;
		પાછા ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	જો (ctx->flags & BLOCK_FLAG_APPEND) {
		માટે (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			જો (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				તોડો;
			}
			offset += કદ(blk_t);
		}
	} નહીંતર {
		માટે (i = 0; i < limit; i++, block_nr++) {
			જો (*block_nr == 0) {
				ctx->bcount += limit;
				ચાલુ;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			જો (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				તોડો;
			}
			offset += કદ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	જો (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		જો (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	જો ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_ગણતરી_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	પાછા ret;
}

સ્થિર પૂર્ણાંક block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      પૂર્ણાંક ref_offset, સંરચના block_context *ctx)
{
	પૂર્ણાંક	ret = 0, changed = 0;
	પૂર્ણાંક	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	જો (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_ગણતરી_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	જો (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		પાછા ret;
	}
	જો (*tind_block >= ext2fs_blocks_ગણતરી(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		પાછા ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	જો (ctx->errcode) {
		ret |= BLOCK_ERROR;
		પાછા ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	જો (ctx->flags & BLOCK_FLAG_APPEND) {
		માટે (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			જો (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				તોડો;
			}
			offset += કદ(blk_t);
		}
	} નહીંતર {
		માટે (i = 0; i < limit; i++, block_nr++) {
			જો (*block_nr == 0) {
				ctx->bcount += limit*limit;
				ચાલુ;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			જો (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				તોડો;
			}
			offset += કદ(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	જો (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		જો (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	જો ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_ગણતરી_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	પાછા ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				પૂર્ણાંક	flags,
				અક્ષર *block_buf,
				પૂર્ણાંક (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    પૂર્ણાંક		ref_offset,
					    શૂન્ય	*priv_data),
				શૂન્ય *priv_data)
{
	પૂર્ણાંક	i;
	પૂર્ણાંક	r, ret = 0;
	સંરચના ext2_inode inode;
	errcode_t	retval;
	સંરચના block_context ctx;
	પૂર્ણાંક	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	જો (ctx.errcode)
		પાછા ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	જો (inode.i_flags & EXT4_INLINE_DATA_FL)
		પાછા EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	જો (flags & BLOCK_FLAG_NO_LARGE) {
		જો (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_ઉપલું != 0))
			પાછા EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	જો (block_buf) {
		ctx.ind_buf = block_buf;
	} નહીંતર {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		જો (retval)
			પાછા retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	જો ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		જો (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_ગણતરી_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			જો (ret & BLOCK_ABORT)
				જાઓ abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	જો (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		સંરચના ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		પૂર્ણાંક			op = EXT2_EXTENT_મૂળ;
		પૂર્ણાંક			uninit;
		unsigned પૂર્ણાંક		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		જો (ctx.errcode)
			જાઓ abort_exit;

		જ્યારે (1) {
			જો (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			નહીંતર
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			જો (ctx.errcode) {
				જો (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					તોડો;
				ctx.errcode = 0;
				જો (!(flags & BLOCK_FLAG_APPEND))
					તોડો;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				જો (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					જો (ctx.errcode || (ret & BLOCK_ABORT))
						તોડો;
					જો (blk)
						જાઓ next_block_set;
				}
				તોડો;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			જો (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				જો (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					ચાલુ;
				જો ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					જો (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						જો (ctx.errcode)
							તોડો;
					}
					જો (ret & BLOCK_ABORT)
						તોડો;
				}
				ચાલુ;
			}
			uninit = 0;
			જો (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#જો 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			જો (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				ચાલુ;
			જો (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			માટે (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				જો (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					જો (ctx.errcode)
						જાઓ extent_done;
				}
				જો (ret & BLOCK_ABORT)
					જાઓ extent_done;
			}
			જો (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		જાઓ errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	માટે (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		જો (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			જો (ret & BLOCK_ABORT)
				જાઓ abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	જો (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		જો (ret & BLOCK_ABORT)
			જાઓ abort_exit;
	} નહીંતર
		ctx.bcount += limit;
	જો (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		જો (ret & BLOCK_ABORT)
			જાઓ abort_exit;
	} નહીંતર
		ctx.bcount += limit * limit;
	જો (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		જો (ret & BLOCK_ABORT)
			જાઓ abort_exit;
	}

abort_exit:
	જો (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		જો (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	જો (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	પાછા (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

સંરચના xlate64 {
	પૂર્ણાંક (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    પૂર્ણાંક		ref_offset,
		    શૂન્ય	*priv_data);
	શૂન્ય *real_private;
};

સ્થિર પૂર્ણાંક xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			પૂર્ણાંક ref_offset, શૂન્ય *priv_data)
{
	સંરચના xlate64 *xl = (સંરચના xlate64 *) priv_data;
	પૂર્ણાંક		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	પાછા ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				પૂર્ણાંક	flags,
				અક્ષર *block_buf,
				પૂર્ણાંક (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    પૂર્ણાંક		ref_offset,
					    શૂન્ય	*priv_data),
				શૂન્ય *priv_data)
{
	સંરચના xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	પાછા ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


સંરચના xlate {
	પૂર્ણાંક (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    પૂર્ણાંક		bcount,
		    શૂન્ય	*priv_data);
	શૂન્ય *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
સ્થિર પૂર્ણાંક xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      પૂર્ણાંક ref_offset EXT2FS_ATTR((unused)),
		      શૂન્ય *priv_data)
{
	સંરચના xlate *xl = (સંરચના xlate *) priv_data;

	પાછા (*xl->func)(fs, blocknr, (પૂર્ણાંક) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       પૂર્ણાંક	flags,
			       અક્ષર *block_buf,
			       પૂર્ણાંક (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   પૂર્ણાંક	blockcnt,
					   શૂન્ય	*priv_data),
			       શૂન્ય *priv_data)
{
	સંરચના xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	પાછા ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

