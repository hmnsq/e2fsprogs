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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

काष्ठा block_context {
	ext2_filsys	fs;
	पूर्णांक (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    पूर्णांक		ref_offset,
		    व्योम	*priv_तथ्य);
	e2_blkcnt_t	bcount;
	पूर्णांक		bsize;
	पूर्णांक		flags;
	errcode_t	errcode;
	अक्षर	*ind_buf;
	अक्षर	*dind_buf;
	अक्षर	*tind_buf;
	व्योम	*priv_तथ्य;
};

#घोषणा check_for_ro_violation_return(ctx, ret)				\
	करो {								\
		अगर (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			वापस ret;					\
		}							\
	} जबतक (0)

#घोषणा check_for_ro_violation_goto(ctx, ret, label)			\
	करो {								\
		अगर (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			जाओ label;					\
		}							\
	} जबतक (0)

अटल पूर्णांक block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     पूर्णांक ref_offset, काष्ठा block_context *ctx)
{
	पूर्णांक	ret = 0, changed = 0;
	पूर्णांक	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	अगर (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_तथ्य_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_गिनती_IND, ref_block,
				   ref_offset, ctx->priv_तथ्य);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	अगर (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		वापस ret;
	}
	अगर (*ind_block >= ext2fs_blocks_गिनती(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_तथ्य_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		वापस ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	अगर (ctx->errcode) {
		ret |= BLOCK_ERROR;
		वापस ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	अगर (ctx->flags & BLOCK_FLAG_APPEND) {
		क्रम (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_तथ्य);
			*block_nr = blk64;
			changed	|= flags;
			अगर (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				अवरोध;
			}
			offset += माप(blk_t);
		}
	} अन्यथा {
		क्रम (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			अगर (*block_nr == 0)
				जाओ skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_तथ्य);
			*block_nr = blk64;
			changed	|= flags;
			अगर (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				अवरोध;
			}
		skip_sparse:
			offset += माप(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	अगर (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		अगर (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	अगर ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_तथ्य_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_गिनती_IND, ref_block,
				    ref_offset, ctx->priv_तथ्य);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	वापस ret;
}

अटल पूर्णांक block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      पूर्णांक ref_offset, काष्ठा block_context *ctx)
{
	पूर्णांक	ret = 0, changed = 0;
	पूर्णांक	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	अगर (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_तथ्य_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_गिनती_DIND, ref_block,
				   ref_offset, ctx->priv_तथ्य);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	अगर (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		वापस ret;
	}
	अगर (*dind_block >= ext2fs_blocks_गिनती(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_तथ्य_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		वापस ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	अगर (ctx->errcode) {
		ret |= BLOCK_ERROR;
		वापस ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	अगर (ctx->flags & BLOCK_FLAG_APPEND) {
		क्रम (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			अगर (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				अवरोध;
			}
			offset += माप(blk_t);
		}
	} अन्यथा {
		क्रम (i = 0; i < limit; i++, block_nr++) {
			अगर (*block_nr == 0) {
				ctx->bcount += limit;
				जारी;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			अगर (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				अवरोध;
			}
			offset += माप(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	अगर (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		अगर (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	अगर ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_तथ्य_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_गिनती_DIND, ref_block,
				    ref_offset, ctx->priv_तथ्य);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	वापस ret;
}

अटल पूर्णांक block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      पूर्णांक ref_offset, काष्ठा block_context *ctx)
{
	पूर्णांक	ret = 0, changed = 0;
	पूर्णांक	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	अगर (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_तथ्य_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_गिनती_TIND, ref_block,
				   ref_offset, ctx->priv_तथ्य);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	अगर (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((अचिन्हित दीर्घ दीर्घ) limit)*limit*limit;
		वापस ret;
	}
	अगर (*tind_block >= ext2fs_blocks_गिनती(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_तथ्य_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		वापस ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	अगर (ctx->errcode) {
		ret |= BLOCK_ERROR;
		वापस ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	अगर (ctx->flags & BLOCK_FLAG_APPEND) {
		क्रम (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			अगर (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				अवरोध;
			}
			offset += माप(blk_t);
		}
	} अन्यथा {
		क्रम (i = 0; i < limit; i++, block_nr++) {
			अगर (*block_nr == 0) {
				ctx->bcount += limit*limit;
				जारी;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			अगर (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				अवरोध;
			}
			offset += माप(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	अगर (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		अगर (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	अगर ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_तथ्य_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_गिनती_TIND, ref_block,
				    ref_offset, ctx->priv_तथ्य);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	वापस ret;
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
					    व्योम	*priv_तथ्य),
				व्योम *priv_तथ्य)
{
	पूर्णांक	i;
	पूर्णांक	r, ret = 0;
	काष्ठा ext2_inode inode;
	errcode_t	retval;
	काष्ठा block_context ctx;
	पूर्णांक	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	अगर (ctx.errcode)
		वापस ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	अगर (inode.i_flags & EXT4_INLINE_तथ्य_FL)
		वापस EXT2_ET_INLINE_तथ्य_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	अगर (flags & BLOCK_FLAG_NO_LARGE) {
		अगर (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_आकार_ऊपरी != 0))
			वापस EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_तथ्य = priv_तथ्य;
	ctx.flags = flags;
	ctx.bcount = 0;
	अगर (block_buf) {
		ctx.ind_buf = block_buf;
	} अन्यथा {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		अगर (retval)
			वापस retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	अगर ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_तथ्य_ONLY)) {
		अगर (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_गिनती_TRANSLATOR,
					   0, 0, priv_तथ्य);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			अगर (ret & BLOCK_ABORT)
				जाओ abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	अगर (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		काष्ठा ext2fs_extent	extent, अग्र;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		पूर्णांक			op = EXT2_EXTENT_मूल;
		पूर्णांक			uninit;
		अचिन्हित पूर्णांक		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		अगर (ctx.errcode)
			जाओ abort_exit;

		जबतक (1) {
			अगर (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			अन्यथा
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			अगर (ctx.errcode) {
				अगर (ctx.errcode != EXT2_ET_EXTENT_NO_अग्र)
					अवरोध;
				ctx.errcode = 0;
				अगर (!(flags & BLOCK_FLAG_APPEND))
					अवरोध;
			अग्र_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_तथ्य);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				अगर (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					अगर (ctx.errcode || (ret & BLOCK_ABORT))
						अवरोध;
					अगर (blk)
						जाओ अग्र_block_set;
				}
				अवरोध;
			}

			op = EXT2_EXTENT_अग्र;
			blk = extent.e_pblk;
			अगर (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				अगर (ctx.flags & BLOCK_FLAG_तथ्य_ONLY)
					जारी;
				अगर ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_तथ्य);
					अगर (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						अगर (ctx.errcode)
							अवरोध;
					}
					अगर (ret & BLOCK_ABORT)
						अवरोध;
				}
				जारी;
			}
			uninit = 0;
			अगर (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &अग्र);

#अगर 0
			म_लिखो("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#पूर्ण_अगर
			अगर (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				जारी;
			अगर (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			क्रम (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_तथ्य);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				अगर (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					अगर (ctx.errcode)
						जाओ extent_done;
				}
				अगर (ret & BLOCK_ABORT)
					जाओ extent_done;
			}
			अगर (retval == 0) {
				extent = अग्र;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		जाओ errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	क्रम (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		अगर (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_तथ्य);
			inode.i_block[i] = (blk_t) blk64;
			अगर (ret & BLOCK_ABORT)
				जाओ abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	अगर (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		अगर (ret & BLOCK_ABORT)
			जाओ abort_exit;
	} अन्यथा
		ctx.bcount += limit;
	अगर (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		अगर (ret & BLOCK_ABORT)
			जाओ abort_exit;
	} अन्यथा
		ctx.bcount += limit * limit;
	अगर (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		अगर (ret & BLOCK_ABORT)
			जाओ abort_exit;
	}

abort_exit:
	अगर (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		अगर (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	अगर (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	वापस (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

काष्ठा xlate64 {
	पूर्णांक (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    पूर्णांक		ref_offset,
		    व्योम	*priv_तथ्य);
	व्योम *real_private;
};

अटल पूर्णांक xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			पूर्णांक ref_offset, व्योम *priv_तथ्य)
{
	काष्ठा xlate64 *xl = (काष्ठा xlate64 *) priv_तथ्य;
	पूर्णांक		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	वापस ret;
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
					    व्योम	*priv_तथ्य),
				व्योम *priv_तथ्य)
{
	काष्ठा xlate64 xl;

	xl.real_private = priv_तथ्य;
	xl.func = func;

	वापस ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


काष्ठा xlate {
	पूर्णांक (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    पूर्णांक		bcount,
		    व्योम	*priv_तथ्य);
	व्योम *real_private;
};

#अगर_घोषित __TURBOC__
 #आशय argsused
#पूर्ण_अगर
अटल पूर्णांक xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      पूर्णांक ref_offset EXT2FS_ATTR((unused)),
		      व्योम *priv_तथ्य)
{
	काष्ठा xlate *xl = (काष्ठा xlate *) priv_तथ्य;

	वापस (*xl->func)(fs, blocknr, (पूर्णांक) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       पूर्णांक	flags,
			       अक्षर *block_buf,
			       पूर्णांक (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   पूर्णांक	blockcnt,
					   व्योम	*priv_तथ्य),
			       व्योम *priv_तथ्य)
{
	काष्ठा xlate xl;

	xl.real_private = priv_तथ्य;
	xl.func = func;

	वापस ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

