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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना block_context {
	ext2_filsys	fs;
	पूर्णाङ्क (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    पूर्णाङ्क		ref_offset,
		    शून्य	*priv_data);
	e2_blkcnt_t	bcount;
	पूर्णाङ्क		bsize;
	पूर्णाङ्क		flags;
	errcode_t	errcode;
	वर्ण	*ind_buf;
	वर्ण	*dind_buf;
	वर्ण	*tind_buf;
	शून्य	*priv_data;
};

#define check_for_ro_violation_return(ctx, ret)				\
	कुरु {								\
		यदि (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			प्रत्यागम ret;					\
		}							\
	} यावत् (0)

#define check_for_ro_violation_goto(ctx, ret, label)			\
	कुरु {								\
		यदि (((ctx)->flags & BLOCK_FLAG_READ_ONLY) &&		\
		    ((ret) & BLOCK_CHANGED)) {				\
			(ctx)->errcode = EXT2_ET_RO_BLOCK_ITERATE;	\
			ret |= BLOCK_ABORT | BLOCK_ERROR;		\
			गच्छ label;					\
		}							\
	} यावत् (0)

स्थिर पूर्णाङ्क block_iterate_ind(blk_t *ind_block, blk_t ref_block,
			     पूर्णाङ्क ref_offset, संरचना block_context *ctx)
{
	पूर्णाङ्क	ret = 0, changed = 0;
	पूर्णाङ्क	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	यदि (!(ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY)) {
		blk64 = *ind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_गिनती_IND, ref_block,
				   ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	यदि (!*ind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit;
		प्रत्यागम ret;
	}
	यदि (*ind_block >= ext2fs_blocks_गिनती(ctx->fs->super) ||
	    *ind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_IND_BLOCK;
		ret |= BLOCK_ERROR;
		प्रत्यागम ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *ind_block,
					     ctx->ind_buf);
	यदि (ctx->errcode) {
		ret |= BLOCK_ERROR;
		प्रत्यागम ret;
	}

	block_nr = (blk_t *) ctx->ind_buf;
	offset = 0;
	यदि (ctx->flags & BLOCK_FLAG_APPEND) {
		प्रति (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			यदि (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				भङ्ग;
			}
			offset += परिमाण(blk_t);
		}
	} अन्यथा {
		प्रति (i = 0; i < limit; i++, ctx->bcount++, block_nr++) {
			यदि (*block_nr == 0)
				गच्छ skip_sparse;
			blk64 = *block_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_block, offset,
					     ctx->priv_data);
			*block_nr = blk64;
			changed	|= flags;
			यदि (flags & BLOCK_ABORT) {
				ret |= BLOCK_ABORT;
				भङ्ग;
			}
		skip_sparse:
			offset += परिमाण(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	यदि (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *ind_block,
						      ctx->ind_buf);
		यदि (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	यदि ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *ind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_गिनती_IND, ref_block,
				    ref_offset, ctx->priv_data);
		*ind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क block_iterate_dind(blk_t *dind_block, blk_t ref_block,
			      पूर्णाङ्क ref_offset, संरचना block_context *ctx)
{
	पूर्णाङ्क	ret = 0, changed = 0;
	पूर्णाङ्क	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	यदि (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *dind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_गिनती_DIND, ref_block,
				   ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	यदि (!*dind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += limit*limit;
		प्रत्यागम ret;
	}
	यदि (*dind_block >= ext2fs_blocks_गिनती(ctx->fs->super) ||
	    *dind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_DIND_BLOCK;
		ret |= BLOCK_ERROR;
		प्रत्यागम ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *dind_block,
					     ctx->dind_buf);
	यदि (ctx->errcode) {
		ret |= BLOCK_ERROR;
		प्रत्यागम ret;
	}

	block_nr = (blk_t *) ctx->dind_buf;
	offset = 0;
	यदि (ctx->flags & BLOCK_FLAG_APPEND) {
		प्रति (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			यदि (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				भङ्ग;
			}
			offset += परिमाण(blk_t);
		}
	} अन्यथा {
		प्रति (i = 0; i < limit; i++, block_nr++) {
			यदि (*block_nr == 0) {
				ctx->bcount += limit;
				अनुवर्त;
			}
			flags = block_iterate_ind(block_nr,
						  *dind_block, offset,
						  ctx);
			changed |= flags;
			यदि (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				भङ्ग;
			}
			offset += परिमाण(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	यदि (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *dind_block,
						      ctx->dind_buf);
		यदि (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	यदि ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *dind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_गिनती_DIND, ref_block,
				    ref_offset, ctx->priv_data);
		*dind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क block_iterate_tind(blk_t *tind_block, blk_t ref_block,
			      पूर्णाङ्क ref_offset, संरचना block_context *ctx)
{
	पूर्णाङ्क	ret = 0, changed = 0;
	पूर्णाङ्क	i, flags, limit, offset;
	blk_t	*block_nr;
	blk64_t	blk64;

	limit = ctx->fs->blocksize >> 2;
	यदि (!(ctx->flags & (BLOCK_FLAG_DEPTH_TRAVERSE |
			    BLOCK_FLAG_DATA_ONLY))) {
		blk64 = *tind_block;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   BLOCK_गिनती_TIND, ref_block,
				   ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	यदि (!*tind_block || (ret & BLOCK_ABORT)) {
		ctx->bcount += ((unsigned long long) limit)*limit*limit;
		प्रत्यागम ret;
	}
	यदि (*tind_block >= ext2fs_blocks_गिनती(ctx->fs->super) ||
	    *tind_block < ctx->fs->super->s_first_data_block) {
		ctx->errcode = EXT2_ET_BAD_TIND_BLOCK;
		ret |= BLOCK_ERROR;
		प्रत्यागम ret;
	}
	ctx->errcode = ext2fs_read_ind_block(ctx->fs, *tind_block,
					     ctx->tind_buf);
	यदि (ctx->errcode) {
		ret |= BLOCK_ERROR;
		प्रत्यागम ret;
	}

	block_nr = (blk_t *) ctx->tind_buf;
	offset = 0;
	यदि (ctx->flags & BLOCK_FLAG_APPEND) {
		प्रति (i = 0; i < limit; i++, block_nr++) {
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			यदि (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				भङ्ग;
			}
			offset += परिमाण(blk_t);
		}
	} अन्यथा {
		प्रति (i = 0; i < limit; i++, block_nr++) {
			यदि (*block_nr == 0) {
				ctx->bcount += limit*limit;
				अनुवर्त;
			}
			flags = block_iterate_dind(block_nr,
						   *tind_block,
						   offset, ctx);
			changed |= flags;
			यदि (flags & (BLOCK_ABORT | BLOCK_ERROR)) {
				ret |= flags & (BLOCK_ABORT | BLOCK_ERROR);
				भङ्ग;
			}
			offset += परिमाण(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, changed);
	यदि (changed & BLOCK_CHANGED) {
		ctx->errcode = ext2fs_write_ind_block(ctx->fs, *tind_block,
						      ctx->tind_buf);
		यदि (ctx->errcode)
			ret |= BLOCK_ERROR | BLOCK_ABORT;
	}
	यदि ((ctx->flags & BLOCK_FLAG_DEPTH_TRAVERSE) &&
	    !(ctx->flags & BLOCK_FLAG_DATA_ONLY) &&
	    !(ret & BLOCK_ABORT)) {
		blk64 = *tind_block;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    BLOCK_गिनती_TIND, ref_block,
				    ref_offset, ctx->priv_data);
		*tind_block = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	प्रत्यागम ret;
}

errcode_t ext2fs_block_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				पूर्णाङ्क	flags,
				वर्ण *block_buf,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    पूर्णाङ्क		ref_offset,
					    शून्य	*priv_data),
				शून्य *priv_data)
{
	पूर्णाङ्क	i;
	पूर्णाङ्क	r, ret = 0;
	संरचना ext2_inode inode;
	errcode_t	retval;
	संरचना block_context ctx;
	पूर्णाङ्क	limit;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	यदि (ctx.errcode)
		प्रत्यागम ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	यदि (inode.i_flags & EXT4_INLINE_DATA_FL)
		प्रत्यागम EXT2_ET_INLINE_DATA_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	यदि (flags & BLOCK_FLAG_NO_LARGE) {
		यदि (!LINUX_S_ISDIR(inode.i_mode) &&
		    (inode.i_size_ऊपरी != 0))
			प्रत्यागम EXT2_ET_FILE_TOO_BIG;
	}

	limit = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.flags = flags;
	ctx.bcount = 0;
	यदि (block_buf) {
		ctx.ind_buf = block_buf;
	} अन्यथा {
		retval = ext2fs_get_array(3, fs->blocksize, &ctx.ind_buf);
		यदि (retval)
			प्रत्यागम retval;
	}
	ctx.dind_buf = ctx.ind_buf + fs->blocksize;
	ctx.tind_buf = ctx.dind_buf + fs->blocksize;

	/*
	 * Iterate over the HURD translator block (if present)
	 */
	यदि ((fs->super->s_creator_os == EXT2_OS_HURD) &&
	    !(flags & BLOCK_FLAG_DATA_ONLY)) {
		यदि (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   BLOCK_गिनती_TRANSLATOR,
					   0, 0, priv_data);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			यदि (ret & BLOCK_ABORT)
				गच्छ abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	यदि (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		संरचना ext2fs_extent	extent, next;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		पूर्णाङ्क			op = EXT2_EXTENT_मूल;
		पूर्णाङ्क			uninit;
		unsigned पूर्णाङ्क		j;

		ctx.errcode = ext2fs_extent_open2(fs, ino, &inode, &handle);
		यदि (ctx.errcode)
			गच्छ abort_exit;

		यावत् (1) {
			यदि (op == EXT2_EXTENT_CURRENT)
				ctx.errcode = 0;
			अन्यथा
				ctx.errcode = ext2fs_extent_get(handle, op,
								&extent);
			यदि (ctx.errcode) {
				यदि (ctx.errcode != EXT2_ET_EXTENT_NO_NEXT)
					भङ्ग;
				ctx.errcode = 0;
				यदि (!(flags & BLOCK_FLAG_APPEND))
					भङ्ग;
			next_block_set:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				यदि (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					यदि (ctx.errcode || (ret & BLOCK_ABORT))
						भङ्ग;
					यदि (blk)
						गच्छ next_block_set;
				}
				भङ्ग;
			}

			op = EXT2_EXTENT_NEXT;
			blk = extent.e_pblk;
			यदि (!(extent.e_flags & EXT2_EXTENT_FLAGS_LEAF)) {
				यदि (ctx.flags & BLOCK_FLAG_DATA_ONLY)
					अनुवर्त;
				यदि ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     !(ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_VISIT) &&
				     (ctx.flags & BLOCK_FLAG_DEPTH_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_data);
					यदि (ret & BLOCK_CHANGED) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						यदि (ctx.errcode)
							भङ्ग;
					}
					यदि (ret & BLOCK_ABORT)
						भङ्ग;
				}
				अनुवर्त;
			}
			uninit = 0;
			यदि (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_SET_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &next);

#यदि 0
			printf("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#endif
			यदि (extent.e_lblk + extent.e_len <= (blk64_t) blockcnt)
				अनुवर्त;
			यदि (extent.e_lblk > (blk64_t) blockcnt)
				blockcnt = extent.e_lblk;
			j = blockcnt - extent.e_lblk;
			blk += j;
			प्रति (blockcnt = extent.e_lblk, j = 0;
			     j < extent.e_len;
			     blk++, blockcnt++, j++) {
				new_blk = blk;
				r = (*ctx.func)(fs, &new_blk, blockcnt,
						0, 0, priv_data);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_done);
				यदि (r & BLOCK_CHANGED) {
					ctx.errcode =
						ext2fs_extent_set_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					यदि (ctx.errcode)
						गच्छ extent_done;
				}
				यदि (ret & BLOCK_ABORT)
					गच्छ extent_done;
			}
			यदि (retval == 0) {
				extent = next;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_done:
		ext2fs_extent_free(handle);
		ret |= BLOCK_ERROR; /* ctx.errcode is always valid here */
		गच्छ errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	प्रति (i = 0; i < EXT2_NDIR_BLOCKS ; i++, ctx.bcount++) {
		यदि (inode.i_block[i] || (flags & BLOCK_FLAG_APPEND)) {
			blk64 = inode.i_block[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_data);
			inode.i_block[i] = (blk_t) blk64;
			यदि (ret & BLOCK_ABORT)
				गच्छ abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	यदि (inode.i_block[EXT2_IND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_ind(&inode.i_block[EXT2_IND_BLOCK], 
					 0, EXT2_IND_BLOCK, &ctx);
		यदि (ret & BLOCK_ABORT)
			गच्छ abort_exit;
	} अन्यथा
		ctx.bcount += limit;
	यदि (inode.i_block[EXT2_DIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_dind(&inode.i_block[EXT2_DIND_BLOCK],
					  0, EXT2_DIND_BLOCK, &ctx);
		यदि (ret & BLOCK_ABORT)
			गच्छ abort_exit;
	} अन्यथा
		ctx.bcount += limit * limit;
	यदि (inode.i_block[EXT2_TIND_BLOCK] || (flags & BLOCK_FLAG_APPEND)) {
		ret |= block_iterate_tind(&inode.i_block[EXT2_TIND_BLOCK],
					  0, EXT2_TIND_BLOCK, &ctx);
		यदि (ret & BLOCK_ABORT)
			गच्छ abort_exit;
	}

abort_exit:
	यदि (ret & BLOCK_CHANGED) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		यदि (retval) {
			ret |= BLOCK_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	यदि (!block_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	प्रत्यागम (ret & BLOCK_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

संरचना xlate64 {
	पूर्णाङ्क (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    पूर्णाङ्क		ref_offset,
		    शून्य	*priv_data);
	शून्य *real_private;
};

स्थिर पूर्णाङ्क xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			पूर्णाङ्क ref_offset, शून्य *priv_data)
{
	संरचना xlate64 *xl = (संरचना xlate64 *) priv_data;
	पूर्णाङ्क		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_offset,
			     xl->real_private);
	*blocknr = block32;
	प्रत्यागम ret;
}

errcode_t ext2fs_block_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				पूर्णाङ्क	flags,
				वर्ण *block_buf,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    पूर्णाङ्क		ref_offset,
					    शून्य	*priv_data),
				शून्य *priv_data)
{
	संरचना xlate64 xl;

	xl.real_private = priv_data;
	xl.func = func;

	प्रत्यागम ext2fs_block_iterate3(fs, ino, flags, block_buf, 
				     xlate64_func, &xl);
}


संरचना xlate {
	पूर्णाङ्क (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    पूर्णाङ्क		bcount,
		    शून्य	*priv_data);
	शून्य *real_private;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
स्थिर पूर्णाङ्क xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_block EXT2FS_ATTR((unused)),
		      पूर्णाङ्क ref_offset EXT2FS_ATTR((unused)),
		      शून्य *priv_data)
{
	संरचना xlate *xl = (संरचना xlate *) priv_data;

	प्रत्यागम (*xl->func)(fs, blocknr, (पूर्णाङ्क) blockcnt, xl->real_private);
}

errcode_t ext2fs_block_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       पूर्णाङ्क	flags,
			       वर्ण *block_buf,
			       पूर्णाङ्क (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   पूर्णाङ्क	blockcnt,
					   शून्य	*priv_data),
			       शून्य *priv_data)
{
	संरचना xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	प्रत्यागम ext2fs_block_iterate2(fs, ino, BLOCK_FLAG_NO_LARGE | flags,
				     block_buf, xlate_func, &xl);
}

