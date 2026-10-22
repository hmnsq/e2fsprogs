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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना block_context {
	ext2_filsys	fs;
	पूर्णांक (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    पूर्णांक		ref_offset,
		    शून्य	*priv_data);
	e2_blkcnt_t	bcount;
	पूर्णांक		bsize;
	पूर्णांक		flags;
	errcode_t	errcode;
	अक्षर	*ind_buf;
	अक्षर	*dind_buf;
	अक्षर	*tind_buf;
	शून्य	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	करा {								\
		जर (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			परत ret;					\
		}							\
	} पर्यंत (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	करा {								\
		जर (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			जा label;					\
		}							\
	} पर्यंत (0)

स्थिर पूर्णांक block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     पूर्णांक ref_offset, संरचना block_context *ctx)
{
	पूर्णांक	ret = 0, changed = 0;
	पूर्णांक	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	जर (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_मोजणी_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	जर (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		परत ret;
	}
	जर (*ind_block >= ext2fs_blocks_मोजणी(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		परत ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	जर (ctx->errcode) {
		ret |= BLOCK_ERROR;
		परत ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	जर (ctx->flags & BLOCK_FLAG_APPEND) {
		साठी (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			जर (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				थांबा;
			}
			offset += आकार(blk_t);
		}
	} अन्यथा {
		साठी (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			जर (*block_nr == 0)
				जा skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			जर (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				थांबा;
			}
		skip_sparse:
			offset += आकार(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	जर (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		जर (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	जर ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_मोजणी_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	परत ret;
}

स्थिर पूर्णांक block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      पूर्णांक ref_offset, संरचना block_context *ctx)
{
	पूर्णांक	ret = 0, changed = 0;
	पूर्णांक	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	जर (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_मोजणी_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	जर (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		परत ret;
	}
	जर (*dind_block >= ext2fs_blocks_मोजणी(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		परत ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	जर (ctx->errcode) {
		ret |= BLOCK_ERROR;
		परत ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	जर (ctx->flags & BLOCK_FLAG_APPEND) {
		साठी (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			जर (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				थांबा;
			}
			offset += आकार(blk_t);
		}
	} अन्यथा {
		साठी (i = 0; i < limit; i++, block_nr++) {
			जर (*block_nr == 0) {
				ctx->bcount += limit;
				सुरूठेवा;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			जर (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				थांबा;
			}
			offset += आकार(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	जर (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		जर (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	जर ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_मोजणी_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	परत ret;
}

स्थिर पूर्णांक block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      पूर्णांक ref_offset, संरचना block_context *ctx)
{
	पूर्णांक	ret = 0, changed = 0;
	पूर्णांक	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	जर (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_मोजणी_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	जर (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((अचिन्हित दीर्घ दीर्घ) limit)*limit*limit;
		परत ret;
	}
	जर (*tind_block >= ext2fs_blocks_मोजणी(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		परत ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	जर (ctx->errcode) {
		ret |= BLOCK_ERROR;
		परत ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	जर (ctx->flags & BLOCK_FLAG_APPEND) {
		साठी (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			जर (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				थांबा;
			}
			offset += आकार(blk_t);
		}
	} अन्यथा {
		साठी (i = 0; i < limit; i++, block_nr++) {
			जर (*block_nr == 0) {
				ctx->bcount += limit*limit;
				सुरूठेवा;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			जर (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				थांबा;
			}
			offset += आकार(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	जर (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		जर (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	जर ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_मोजणी_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	परत ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				पूर्णांक	flags,
				अक्षर *block_buf,
				पूर्णांक (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    पूर्णांक		ref_offset,
					    शून्य	*priv_data),
				शून्य *priv_data)
{
	पूर्णांक	i;
	पूर्णांक	r, ret = 0;
	संरचना ext2_inode inode;
	errcode_t	retval;
	संरचना block_context ctx;
	पूर्णांक	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	जर (ctx.errcode)
		परत ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	जर (inode.i_flags & EXT4_INLINE_DATA_FL)
		परत EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	जर (flags & BLOCK_FLAG_NO_LARGE) {
		जर (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_वरचा != 0))
			परत EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	जर (block_buf) {
		ctx.ind_buf = block_buf;
	} अन्यथा {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		जर (retval)
			परत retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	जर ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		जर (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_मोजणी_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			जर (ret & BLOCK_ABORT)
				जा abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	जर (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		संरचना ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		पूर्णांक			op = EXT2_EXTENT_मूळ;
		पूर्णांक			uninit;
		अचिन्हित पूर्णांक		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		जर (ctx.errcode)
			जा abort_exit;

		पर्यंत (1) {
			जर (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			अन्यथा
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			जर (ctx.errcode) {
				जर (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					थांबा;
				ctx.errcode = 0;
				जर (!(flags & BLOCK_FLAG_APPEND))
					थांबा;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				जर (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					जर (ctx.errcode || (ret & BLOCK_ABORT))
						थांबा;
					जर (blk)
						जा next_block_set;
				}
				थांबा;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			जर (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				जर (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					सुरूठेवा;
				जर ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					जर (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						जर (ctx.errcode)
							थांबा;
					}
					जर (ret & BLOCK_ABORT)
						थांबा;
				}
				सुरूठेवा;
			}
			uninit = 0;
			जर (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#जर 0
			म_लिखो("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			जर (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				सुरूठेवा;
			जर (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			साठी (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				जर (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					जर (ctx.errcode)
						जा extent_done;
				}
				जर (ret & BLOCK_ABORT)
					जा extent_done;
			}
			जर (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		जा errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	साठी (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		जर (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			जर (ret & BLOCK_ABORT)
				जा abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	जर (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		जर (ret & BLOCK_ABORT)
			जा abort_exit;
	} अन्यथा
		ctx.bcount += limit;
	जर (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		जर (ret & BLOCK_ABORT)
			जा abort_exit;
	} अन्यथा
		ctx.bcount += limit * limit;
	जर (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		जर (ret & BLOCK_ABORT)
			जा abort_exit;
	}

abort_exit:
	जर (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		जर (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	जर (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	परत (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

संरचना xlate64 {
	पूर्णांक (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    पूर्णांक		ref_offset,
		    शून्य	*priv_data);
	शून्य *real_private;
};

स्थिर पूर्णांक xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			पूर्णांक ref_offset, शून्य *priv_data)
{
	संरचना xlate64 *xl = (संरचना xlate64 *) priv_data;
	पूर्णांक		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	परत ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				पूर्णांक	flags,
				अक्षर *block_buf,
				पूर्णांक (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    पूर्णांक		ref_offset,
					    शून्य	*priv_data),
				शून्य *priv_data)
{
	संरचना xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	परत ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


संरचना xlate {
	पूर्णांक (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    पूर्णांक		bcount,
		    शून्य	*priv_data);
	शून्य *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
स्थिर पूर्णांक xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      पूर्णांक ref_offset EXT2FS_ATTR((unused)),
		      शून्य *priv_data)
{
	संरचना xlate *xl = (संरचना xlate *) priv_data;

	परत (*xl->func)(fs, blocknr, (पूर्णांक) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       पूर्णांक	flags,
			       अक्षर *block_buf,
			       पूर्णांक (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   पूर्णांक	blockcnt,
					   शून्य	*priv_data),
			       शून्य *priv_data)
{
	संरचना xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	परत ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

