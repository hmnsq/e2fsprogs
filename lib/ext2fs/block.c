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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

ഘടന block_context {
	ext2_filsys	fs;
	പൂർണ്ണസംഖ്യ (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    പൂർണ്ണസംഖ്യ		ref_offset,
		    ശൂന്യം	*priv_data);
	e2_blkcnt_t	bcount;
	പൂർണ്ണസംഖ്യ		bsize;
	പൂർണ്ണസംഖ്യ		flags;
	errcode_t	errcode;
	അക്ഷരം	*ind_buf;
	അക്ഷരം	*dind_buf;
	അക്ഷരം	*tind_buf;
	ശൂന്യം	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	ചെയ്യുക {								\
		എങ്കിൽ (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			തിരികെ ret;					\
		}							\
	} ആകുമ്പോൾ (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	ചെയ്യുക {								\
		എങ്കിൽ (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			പോകുക label;					\
		}							\
	} ആകുമ്പോൾ (0)

സ്ഥിതം പൂർണ്ണസംഖ്യ block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     പൂർണ്ണസംഖ്യ ref_offset, ഘടന block_context *ctx)
{
	പൂർണ്ണസംഖ്യ	ret = 0, changed = 0;
	പൂർണ്ണസംഖ്യ	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	എങ്കിൽ (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_എണ്ണം_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	എങ്കിൽ (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		തിരികെ ret;
	}
	എങ്കിൽ (*ind_block >= ext2fs_blocks_എണ്ണം(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		തിരികെ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	എങ്കിൽ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		തിരികെ ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	എങ്കിൽ (ctx->flags & BLOCK_FLAG_APPEND) {
		വേണ്ടി (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			എങ്കിൽ (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				നിർത്തുക;
			}
			offset += വലുപ്പം(blk_t);
		}
	} അല്ലെങ്കിൽ {
		വേണ്ടി (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			എങ്കിൽ (*block_nr == 0)
				പോകുക skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			എങ്കിൽ (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				നിർത്തുക;
			}
		skip_sparse:
			offset += വലുപ്പം(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	എങ്കിൽ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		എങ്കിൽ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	എങ്കിൽ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_എണ്ണം_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      പൂർണ്ണസംഖ്യ ref_offset, ഘടന block_context *ctx)
{
	പൂർണ്ണസംഖ്യ	ret = 0, changed = 0;
	പൂർണ്ണസംഖ്യ	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	എങ്കിൽ (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_എണ്ണം_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	എങ്കിൽ (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		തിരികെ ret;
	}
	എങ്കിൽ (*dind_block >= ext2fs_blocks_എണ്ണം(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		തിരികെ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	എങ്കിൽ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		തിരികെ ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	എങ്കിൽ (ctx->flags & BLOCK_FLAG_APPEND) {
		വേണ്ടി (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			എങ്കിൽ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				നിർത്തുക;
			}
			offset += വലുപ്പം(blk_t);
		}
	} അല്ലെങ്കിൽ {
		വേണ്ടി (i = 0; i < limit; i++, block_nr++) {
			എങ്കിൽ (*block_nr == 0) {
				ctx->bcount += limit;
				തുടരുക;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			എങ്കിൽ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				നിർത്തുക;
			}
			offset += വലുപ്പം(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	എങ്കിൽ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		എങ്കിൽ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	എങ്കിൽ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_എണ്ണം_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      പൂർണ്ണസംഖ്യ ref_offset, ഘടന block_context *ctx)
{
	പൂർണ്ണസംഖ്യ	ret = 0, changed = 0;
	പൂർണ്ണസംഖ്യ	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	എങ്കിൽ (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_എണ്ണം_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	എങ്കിൽ (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		തിരികെ ret;
	}
	എങ്കിൽ (*tind_block >= ext2fs_blocks_എണ്ണം(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		തിരികെ ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	എങ്കിൽ (ctx->errcode) {
		ret |= BLOCK_ERROR;
		തിരികെ ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	എങ്കിൽ (ctx->flags & BLOCK_FLAG_APPEND) {
		വേണ്ടി (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			എങ്കിൽ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				നിർത്തുക;
			}
			offset += വലുപ്പം(blk_t);
		}
	} അല്ലെങ്കിൽ {
		വേണ്ടി (i = 0; i < limit; i++, block_nr++) {
			എങ്കിൽ (*block_nr == 0) {
				ctx->bcount += limit*limit;
				തുടരുക;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			എങ്കിൽ (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				നിർത്തുക;
			}
			offset += വലുപ്പം(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	എങ്കിൽ (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		എങ്കിൽ (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	എങ്കിൽ ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_എണ്ണം_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	തിരികെ ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				പൂർണ്ണസംഖ്യ	flags,
				അക്ഷരം *block_buf,
				പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    പൂർണ്ണസംഖ്യ		ref_offset,
					    ശൂന്യം	*priv_data),
				ശൂന്യം *priv_data)
{
	പൂർണ്ണസംഖ്യ	i;
	പൂർണ്ണസംഖ്യ	r, ret = 0;
	ഘടന ext2_inode inode;
	errcode_t	retval;
	ഘടന block_context ctx;
	പൂർണ്ണസംഖ്യ	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (ctx.errcode)
		തിരികെ ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	എങ്കിൽ (inode.i_flags & EXT4_INLINE_DATA_FL)
		തിരികെ EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	എങ്കിൽ (flags & BLOCK_FLAG_NO_LARGE) {
		എങ്കിൽ (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_മുകളിലെ != 0))
			തിരികെ EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	എങ്കിൽ (block_buf) {
		ctx.ind_buf = block_buf;
	} അല്ലെങ്കിൽ {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	എങ്കിൽ ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		എങ്കിൽ (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_എണ്ണം_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			എങ്കിൽ (ret & BLOCK_ABORT)
				പോകുക abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	എങ്കിൽ (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		ഘടന ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		പൂർണ്ണസംഖ്യ			op = EXT2_EXTENT_വേര്;
		പൂർണ്ണസംഖ്യ			uninit;
		unsigned പൂർണ്ണസംഖ്യ		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		എങ്കിൽ (ctx.errcode)
			പോകുക abort_exit;

		ആകുമ്പോൾ (1) {
			എങ്കിൽ (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			അല്ലെങ്കിൽ
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			എങ്കിൽ (ctx.errcode) {
				എങ്കിൽ (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					നിർത്തുക;
				ctx.errcode = 0;
				എങ്കിൽ (!(flags & BLOCK_FLAG_APPEND))
					നിർത്തുക;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				എങ്കിൽ (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					എങ്കിൽ (ctx.errcode || (ret & BLOCK_ABORT))
						നിർത്തുക;
					എങ്കിൽ (blk)
						പോകുക next_block_set;
				}
				നിർത്തുക;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			എങ്കിൽ (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				എങ്കിൽ (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					തുടരുക;
				എങ്കിൽ ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					എങ്കിൽ (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						എങ്കിൽ (ctx.errcode)
							നിർത്തുക;
					}
					എങ്കിൽ (ret & BLOCK_ABORT)
						നിർത്തുക;
				}
				തുടരുക;
			}
			uninit = 0;
			എങ്കിൽ (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#എങ്കിൽ 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			എങ്കിൽ (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				തുടരുക;
			എങ്കിൽ (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			വേണ്ടി (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				എങ്കിൽ (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					എങ്കിൽ (ctx.errcode)
						പോകുക extent_done;
				}
				എങ്കിൽ (ret & BLOCK_ABORT)
					പോകുക extent_done;
			}
			എങ്കിൽ (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		പോകുക errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	വേണ്ടി (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		എങ്കിൽ (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			എങ്കിൽ (ret & BLOCK_ABORT)
				പോകുക abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	എങ്കിൽ (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		എങ്കിൽ (ret & BLOCK_ABORT)
			പോകുക abort_exit;
	} അല്ലെങ്കിൽ
		ctx.bcount += limit;
	എങ്കിൽ (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		എങ്കിൽ (ret & BLOCK_ABORT)
			പോകുക abort_exit;
	} അല്ലെങ്കിൽ
		ctx.bcount += limit * limit;
	എങ്കിൽ (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		എങ്കിൽ (ret & BLOCK_ABORT)
			പോകുക abort_exit;
	}

abort_exit:
	എങ്കിൽ (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		എങ്കിൽ (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	എങ്കിൽ (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	തിരികെ (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

ഘടന xlate64 {
	പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    പൂർണ്ണസംഖ്യ		ref_offset,
		    ശൂന്യം	*priv_data);
	ശൂന്യം *real_private;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			പൂർണ്ണസംഖ്യ ref_offset, ശൂന്യം *priv_data)
{
	ഘടന xlate64 *xl = (ഘടന xlate64 *) priv_data;
	പൂർണ്ണസംഖ്യ		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	തിരികെ ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				പൂർണ്ണസംഖ്യ	flags,
				അക്ഷരം *block_buf,
				പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    പൂർണ്ണസംഖ്യ		ref_offset,
					    ശൂന്യം	*priv_data),
				ശൂന്യം *priv_data)
{
	ഘടന xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	തിരികെ ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


ഘടന xlate {
	പൂർണ്ണസംഖ്യ (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    പൂർണ്ണസംഖ്യ		bcount,
		    ശൂന്യം	*priv_data);
	ശൂന്യം *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
സ്ഥിതം പൂർണ്ണസംഖ്യ xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      പൂർണ്ണസംഖ്യ ref_offset EXT2FS_ATTR((unused)),
		      ശൂന്യം *priv_data)
{
	ഘടന xlate *xl = (ഘടന xlate *) priv_data;

	തിരികെ (*xl->func)(fs, blocknr, (പൂർണ്ണസംഖ്യ) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       പൂർണ്ണസംഖ്യ	flags,
			       അക്ഷരം *block_buf,
			       പൂർണ്ണസംഖ്യ (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   പൂർണ്ണസംഖ്യ	blockcnt,
					   ശൂന്യം	*priv_data),
			       ശൂന്യം *priv_data)
{
	ഘടന xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	തിരികെ ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

