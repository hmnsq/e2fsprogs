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
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

संरचना खण्डम्_context {
	ext2_filsys	fs;
	पूर्णाङ्क (*func)(ext2_filsys	fs,
		    blk64_t	*blocknr,
		    e2_blkcnt_t	bcount,
		    blk64_t	ref_blk,
		    पूर्णाङ्क		ref_विस्थापनम्,
		    शून्य	*priv_दत्त);
	e2_blkcnt_t	bcount;
	पूर्णाङ्क		bsize;
	पूर्णाङ्क		flags;
	errcode_t	errcode;
	वर्ण	*ind_buf;
	वर्ण	*dind_buf;
	वर्ण	*tind_buf;
	शून्य	*priv_दत्त;
};

#परिभाषय check_for_ro_violation_return(ctx, ret)				\
	कुरु {								\
		यदि (((ctx)->flags & खण्डम्_FLAG_READ_ONLY) &&		\
		    ((ret) & खण्डम्_परिवर्तितम्)) {				\
			(ctx)->errcode = EXT2_ET_RO_खण्डम्_ITERATE;	\
			ret |= खण्डम्_ABORT | खण्डम्_ERROR;		\
			प्रत्यागम ret;					\
		}							\
	} यावत् (0)

#परिभाषय check_for_ro_violation_goto(ctx, ret, label)			\
	कुरु {								\
		यदि (((ctx)->flags & खण्डम्_FLAG_READ_ONLY) &&		\
		    ((ret) & खण्डम्_परिवर्तितम्)) {				\
			(ctx)->errcode = EXT2_ET_RO_खण्डम्_ITERATE;	\
			ret |= खण्डम्_ABORT | खण्डम्_ERROR;		\
			गच्छ label;					\
		}							\
	} यावत् (0)

स्थिर पूर्णाङ्क खण्डम्_iterate_ind(blk_t *ind_खण्डम्, blk_t ref_खण्डम्,
			     पूर्णाङ्क ref_विस्थापनम्, संरचना खण्डम्_context *ctx)
{
	पूर्णाङ्क	ret = 0, परिवर्तितम् = 0;
	पूर्णाङ्क	i, flags, सीमा, विस्थापनम्;
	blk_t	*खण्डम्_nr;
	blk64_t	blk64;

	सीमा = ctx->fs->blocksize >> 2;
	यदि (!(ctx->flags & खण्डम्_FLAG_गभीरता_TRAVERSE) &&
	    !(ctx->flags & खण्डम्_FLAG_दत्त_ONLY)) {
		blk64 = *ind_खण्डम्;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   खण्डम्_गणनम्_IND, ref_खण्डम्,
				   ref_विस्थापनम्, ctx->priv_दत्त);
		*ind_खण्डम् = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	यदि (!*ind_खण्डम् || (ret & खण्डम्_ABORT)) {
		ctx->bcount += सीमा;
		प्रत्यागम ret;
	}
	यदि (*ind_खण्डम् >= ext2fs_खण्डानि_गणनम्(ctx->fs->super) ||
	    *ind_खण्डम् < ctx->fs->super->s_first_दत्त_खण्डम्) {
		ctx->errcode = EXT2_ET_BAD_IND_खण्डम्;
		ret |= खण्डम्_ERROR;
		प्रत्यागम ret;
	}
	ctx->errcode = ext2fs_read_ind_खण्डम्(ctx->fs, *ind_खण्डम्,
					     ctx->ind_buf);
	यदि (ctx->errcode) {
		ret |= खण्डम्_ERROR;
		प्रत्यागम ret;
	}

	खण्डम्_nr = (blk_t *) ctx->ind_buf;
	विस्थापनम् = 0;
	यदि (ctx->flags & खण्डम्_FLAG_APPEND) {
		प्रति (i = 0; i < सीमा; i++, ctx->bcount++, खण्डम्_nr++) {
			blk64 = *खण्डम्_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_खण्डम्, विस्थापनम्,
					     ctx->priv_दत्त);
			*खण्डम्_nr = blk64;
			परिवर्तितम्	|= flags;
			यदि (flags & खण्डम्_ABORT) {
				ret |= खण्डम्_ABORT;
				भङ्ग;
			}
			विस्थापनम् += परिमाण(blk_t);
		}
	} अन्यथा {
		प्रति (i = 0; i < सीमा; i++, ctx->bcount++, खण्डम्_nr++) {
			यदि (*खण्डम्_nr == 0)
				गच्छ skip_sparse;
			blk64 = *खण्डम्_nr;
			flags = (*ctx->func)(ctx->fs, &blk64, ctx->bcount,
					     *ind_खण्डम्, विस्थापनम्,
					     ctx->priv_दत्त);
			*खण्डम्_nr = blk64;
			परिवर्तितम्	|= flags;
			यदि (flags & खण्डम्_ABORT) {
				ret |= खण्डम्_ABORT;
				भङ्ग;
			}
		skip_sparse:
			विस्थापनम् += परिमाण(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, परिवर्तितम्);
	यदि (परिवर्तितम् & खण्डम्_परिवर्तितम्) {
		ctx->errcode = ext2fs_write_ind_खण्डम्(ctx->fs, *ind_खण्डम्,
						      ctx->ind_buf);
		यदि (ctx->errcode)
			ret |= खण्डम्_ERROR | खण्डम्_ABORT;
	}
	यदि ((ctx->flags & खण्डम्_FLAG_गभीरता_TRAVERSE) &&
	    !(ctx->flags & खण्डम्_FLAG_दत्त_ONLY) &&
	    !(ret & खण्डम्_ABORT)) {
		blk64 = *ind_खण्डम्;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    खण्डम्_गणनम्_IND, ref_खण्डम्,
				    ref_विस्थापनम्, ctx->priv_दत्त);
		*ind_खण्डम् = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क खण्डम्_iterate_dind(blk_t *dind_खण्डम्, blk_t ref_खण्डम्,
			      पूर्णाङ्क ref_विस्थापनम्, संरचना खण्डम्_context *ctx)
{
	पूर्णाङ्क	ret = 0, परिवर्तितम् = 0;
	पूर्णाङ्क	i, flags, सीमा, विस्थापनम्;
	blk_t	*खण्डम्_nr;
	blk64_t	blk64;

	सीमा = ctx->fs->blocksize >> 2;
	यदि (!(ctx->flags & (खण्डम्_FLAG_गभीरता_TRAVERSE |
			    खण्डम्_FLAG_दत्त_ONLY))) {
		blk64 = *dind_खण्डम्;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   खण्डम्_गणनम्_DIND, ref_खण्डम्,
				   ref_विस्थापनम्, ctx->priv_दत्त);
		*dind_खण्डम् = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	यदि (!*dind_खण्डम् || (ret & खण्डम्_ABORT)) {
		ctx->bcount += सीमा*सीमा;
		प्रत्यागम ret;
	}
	यदि (*dind_खण्डम् >= ext2fs_खण्डानि_गणनम्(ctx->fs->super) ||
	    *dind_खण्डम् < ctx->fs->super->s_first_दत्त_खण्डम्) {
		ctx->errcode = EXT2_ET_BAD_DIND_खण्डम्;
		ret |= खण्डम्_ERROR;
		प्रत्यागम ret;
	}
	ctx->errcode = ext2fs_read_ind_खण्डम्(ctx->fs, *dind_खण्डम्,
					     ctx->dind_buf);
	यदि (ctx->errcode) {
		ret |= खण्डम्_ERROR;
		प्रत्यागम ret;
	}

	खण्डम्_nr = (blk_t *) ctx->dind_buf;
	विस्थापनम् = 0;
	यदि (ctx->flags & खण्डम्_FLAG_APPEND) {
		प्रति (i = 0; i < सीमा; i++, खण्डम्_nr++) {
			flags = खण्डम्_iterate_ind(खण्डम्_nr,
						  *dind_खण्डम्, विस्थापनम्,
						  ctx);
			परिवर्तितम् |= flags;
			यदि (flags & (खण्डम्_ABORT | खण्डम्_ERROR)) {
				ret |= flags & (खण्डम्_ABORT | खण्डम्_ERROR);
				भङ्ग;
			}
			विस्थापनम् += परिमाण(blk_t);
		}
	} अन्यथा {
		प्रति (i = 0; i < सीमा; i++, खण्डम्_nr++) {
			यदि (*खण्डम्_nr == 0) {
				ctx->bcount += सीमा;
				अनुवर्त;
			}
			flags = खण्डम्_iterate_ind(खण्डम्_nr,
						  *dind_खण्डम्, विस्थापनम्,
						  ctx);
			परिवर्तितम् |= flags;
			यदि (flags & (खण्डम्_ABORT | खण्डम्_ERROR)) {
				ret |= flags & (खण्डम्_ABORT | खण्डम्_ERROR);
				भङ्ग;
			}
			विस्थापनम् += परिमाण(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, परिवर्तितम्);
	यदि (परिवर्तितम् & खण्डम्_परिवर्तितम्) {
		ctx->errcode = ext2fs_write_ind_खण्डम्(ctx->fs, *dind_खण्डम्,
						      ctx->dind_buf);
		यदि (ctx->errcode)
			ret |= खण्डम्_ERROR | खण्डम्_ABORT;
	}
	यदि ((ctx->flags & खण्डम्_FLAG_गभीरता_TRAVERSE) &&
	    !(ctx->flags & खण्डम्_FLAG_दत्त_ONLY) &&
	    !(ret & खण्डम्_ABORT)) {
		blk64 = *dind_खण्डम्;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    खण्डम्_गणनम्_DIND, ref_खण्डम्,
				    ref_विस्थापनम्, ctx->priv_दत्त);
		*dind_खण्डम् = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क खण्डम्_iterate_tind(blk_t *tind_खण्डम्, blk_t ref_खण्डम्,
			      पूर्णाङ्क ref_विस्थापनम्, संरचना खण्डम्_context *ctx)
{
	पूर्णाङ्क	ret = 0, परिवर्तितम् = 0;
	पूर्णाङ्क	i, flags, सीमा, विस्थापनम्;
	blk_t	*खण्डम्_nr;
	blk64_t	blk64;

	सीमा = ctx->fs->blocksize >> 2;
	यदि (!(ctx->flags & (खण्डम्_FLAG_गभीरता_TRAVERSE |
			    खण्डम्_FLAG_दत्त_ONLY))) {
		blk64 = *tind_खण्डम्;
		ret = (*ctx->func)(ctx->fs, &blk64,
				   खण्डम्_गणनम्_TIND, ref_खण्डम्,
				   ref_विस्थापनम्, ctx->priv_दत्त);
		*tind_खण्डम् = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	यदि (!*tind_खण्डम् || (ret & खण्डम्_ABORT)) {
		ctx->bcount += ((अचिह्नित दीर्घ दीर्घ) सीमा)*सीमा*सीमा;
		प्रत्यागम ret;
	}
	यदि (*tind_खण्डम् >= ext2fs_खण्डानि_गणनम्(ctx->fs->super) ||
	    *tind_खण्डम् < ctx->fs->super->s_first_दत्त_खण्डम्) {
		ctx->errcode = EXT2_ET_BAD_TIND_खण्डम्;
		ret |= खण्डम्_ERROR;
		प्रत्यागम ret;
	}
	ctx->errcode = ext2fs_read_ind_खण्डम्(ctx->fs, *tind_खण्डम्,
					     ctx->tind_buf);
	यदि (ctx->errcode) {
		ret |= खण्डम्_ERROR;
		प्रत्यागम ret;
	}

	खण्डम्_nr = (blk_t *) ctx->tind_buf;
	विस्थापनम् = 0;
	यदि (ctx->flags & खण्डम्_FLAG_APPEND) {
		प्रति (i = 0; i < सीमा; i++, खण्डम्_nr++) {
			flags = खण्डम्_iterate_dind(खण्डम्_nr,
						   *tind_खण्डम्,
						   विस्थापनम्, ctx);
			परिवर्तितम् |= flags;
			यदि (flags & (खण्डम्_ABORT | खण्डम्_ERROR)) {
				ret |= flags & (खण्डम्_ABORT | खण्डम्_ERROR);
				भङ्ग;
			}
			विस्थापनम् += परिमाण(blk_t);
		}
	} अन्यथा {
		प्रति (i = 0; i < सीमा; i++, खण्डम्_nr++) {
			यदि (*खण्डम्_nr == 0) {
				ctx->bcount += सीमा*सीमा;
				अनुवर्त;
			}
			flags = खण्डम्_iterate_dind(खण्डम्_nr,
						   *tind_खण्डम्,
						   विस्थापनम्, ctx);
			परिवर्तितम् |= flags;
			यदि (flags & (खण्डम्_ABORT | खण्डम्_ERROR)) {
				ret |= flags & (खण्डम्_ABORT | खण्डम्_ERROR);
				भङ्ग;
			}
			विस्थापनम् += परिमाण(blk_t);
		}
	}
	check_for_ro_violation_return(ctx, परिवर्तितम्);
	यदि (परिवर्तितम् & खण्डम्_परिवर्तितम्) {
		ctx->errcode = ext2fs_write_ind_खण्डम्(ctx->fs, *tind_खण्डम्,
						      ctx->tind_buf);
		यदि (ctx->errcode)
			ret |= खण्डम्_ERROR | खण्डम्_ABORT;
	}
	यदि ((ctx->flags & खण्डम्_FLAG_गभीरता_TRAVERSE) &&
	    !(ctx->flags & खण्डम्_FLAG_दत्त_ONLY) &&
	    !(ret & खण्डम्_ABORT)) {
		blk64 = *tind_खण्डम्;
		ret |= (*ctx->func)(ctx->fs, &blk64,
				    खण्डम्_गणनम्_TIND, ref_खण्डम्,
				    ref_विस्थापनम्, ctx->priv_दत्त);
		*tind_खण्डम् = blk64;
	}
	check_for_ro_violation_return(ctx, ret);
	प्रत्यागम ret;
}

errcode_t ext2fs_खण्डम्_iterate3(ext2_filsys fs,
				ext2_ino_t ino,
				पूर्णाङ्क	flags,
				वर्ण *खण्डम्_buf,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    blk64_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk64_t	ref_blk,
					    पूर्णाङ्क		ref_विस्थापनम्,
					    शून्य	*priv_दत्त),
				शून्य *priv_दत्त)
{
	पूर्णाङ्क	i;
	पूर्णाङ्क	r, ret = 0;
	संरचना ext2_inode inode;
	errcode_t	retval;
	संरचना खण्डम्_context ctx;
	पूर्णाङ्क	सीमा;
	blk64_t	blk64;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ctx.errcode = ext2fs_read_inode(fs, ino, &inode);
	यदि (ctx.errcode)
		प्रत्यागम ctx.errcode;

	/*
	 * An inode with inline data has no blocks over which to
	 * iterate, so return an error code indicating this fact.
	 */
	यदि (inode.i_flags & EXT4_INLINE_दत्त_FL)
		प्रत्यागम EXT2_ET_INLINE_दत्त_CANT_ITERATE;

	/*
	 * Check to see if we need to limit large files
	 */
	यदि (flags & खण्डम्_FLAG_NO_LARGE) {
		यदि (!LINUX_S_ISDIR(inode.i_बहुलकम्) &&
		    (inode.i_आकार_उच्चः != 0))
			प्रत्यागम EXT2_ET_FILE_TOO_BIG;
	}

	सीमा = fs->blocksize >> 2;

	ctx.fs = fs;
	ctx.func = func;
	ctx.priv_दत्त = priv_दत्त;
	ctx.flags = flags;
	ctx.bcount = 0;
	यदि (खण्डम्_buf) {
		ctx.ind_buf = खण्डम्_buf;
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
	    !(flags & खण्डम्_FLAG_दत्त_ONLY)) {
		यदि (inode.osd1.hurd1.h_i_translator) {
			blk64 = inode.osd1.hurd1.h_i_translator;
			ret |= (*ctx.func)(fs, &blk64,
					   खण्डम्_गणनम्_TRANSLATOR,
					   0, 0, priv_दत्त);
			inode.osd1.hurd1.h_i_translator = (blk_t) blk64;
			यदि (ret & खण्डम्_ABORT)
				गच्छ abort_exit;
			check_for_ro_violation_goto(&ctx, ret, abort_exit);
		}
	}

	यदि (inode.i_flags & EXT4_EXTENTS_FL) {
		ext2_extent_handle_t	handle;
		संरचना ext2fs_extent	extent, अग्र;
		e2_blkcnt_t		blockcnt = 0;
		blk64_t			blk, new_blk;
		पूर्णाङ्क			op = EXT2_EXTENT_मूलम्;
		पूर्णाङ्क			uninit;
		अचिह्नित पूर्णाङ्क		j;

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
				यदि (ctx.errcode != EXT2_ET_EXTENT_NO_अग्र)
					भङ्ग;
				ctx.errcode = 0;
				यदि (!(flags & खण्डम्_FLAG_APPEND))
					भङ्ग;
			अग्र_खण्डम्_समुच्चयः:
				blk = 0;
				r = (*ctx.func)(fs, &blk, blockcnt,
						0, 0, priv_दत्त);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_समाप्तम्);
				यदि (r & खण्डम्_परिवर्तितम्) {
					ctx.errcode =
						ext2fs_extent_समुच्चयः_bmap(handle,
						       (blk64_t) blockcnt++,
						       (blk64_t) blk, 0);
					यदि (ctx.errcode || (ret & खण्डम्_ABORT))
						भङ्ग;
					यदि (blk)
						गच्छ अग्र_खण्डम्_समुच्चयः;
				}
				भङ्ग;
			}

			op = EXT2_EXTENT_अग्र;
			blk = extent.e_pblk;
			यदि (!(extent.e_flags & EXT2_EXTENT_FLAGS_पर्णम्)) {
				यदि (ctx.flags & खण्डम्_FLAG_दत्त_ONLY)
					अनुवर्त;
				यदि ((!(extent.e_flags &
				       EXT2_EXTENT_FLAGS_SECOND_दर्शय) &&
				     !(ctx.flags & खण्डम्_FLAG_गभीरता_TRAVERSE)) ||
				    ((extent.e_flags &
				      EXT2_EXTENT_FLAGS_SECOND_दर्शय) &&
				     (ctx.flags & खण्डम्_FLAG_गभीरता_TRAVERSE))) {
					ret |= (*ctx.func)(fs, &blk,
							   -1, 0, 0, priv_दत्त);
					यदि (ret & खण्डम्_परिवर्तितम्) {
						extent.e_pblk = blk;
						ctx.errcode =
				ext2fs_extent_replace(handle, 0, &extent);
						यदि (ctx.errcode)
							भङ्ग;
					}
					यदि (ret & खण्डम्_ABORT)
						भङ्ग;
				}
				अनुवर्त;
			}
			uninit = 0;
			यदि (extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
				uninit = EXT2_EXTENT_समुच्चयः_BMAP_UNINIT;

			/* 
			 * Get the next extent before we start messing
			 * with the current extent
			 */
			retval = ext2fs_extent_get(handle, op, &अग्र);

#यदि 0
			प्रारूप_लिख("lblk %llu pblk %llu len %d blockcnt %llu\n",
			       extent.e_lblk, extent.e_pblk,
			       extent.e_len, blockcnt);
#यदि_अन्तः
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
						0, 0, priv_दत्त);
				ret |= r;
				check_for_ro_violation_goto(&ctx, ret,
							    extent_समाप्तम्);
				यदि (r & खण्डम्_परिवर्तितम्) {
					ctx.errcode =
						ext2fs_extent_समुच्चयः_bmap(handle,
						       (blk64_t) blockcnt,
						       new_blk, uninit);
					यदि (ctx.errcode)
						गच्छ extent_समाप्तम्;
				}
				यदि (ret & खण्डम्_ABORT)
					गच्छ extent_समाप्तम्;
			}
			यदि (retval == 0) {
				extent = अग्र;
				op = EXT2_EXTENT_CURRENT;
			}
		}

	extent_समाप्तम्:
		ext2fs_extent_free(handle);
		ret |= खण्डम्_ERROR; /* ctx.errcode is always valid here */
		गच्छ errout;
	}

	/*
	 * Iterate over normal data blocks
	 */
	प्रति (i = 0; i < EXT2_NDIR_खण्डानि ; i++, ctx.bcount++) {
		यदि (inode.i_खण्डम्[i] || (flags & खण्डम्_FLAG_APPEND)) {
			blk64 = inode.i_खण्डम्[i];
			ret |= (*ctx.func)(fs, &blk64, ctx.bcount, 0, i, 
					   priv_दत्त);
			inode.i_खण्डम्[i] = (blk_t) blk64;
			यदि (ret & खण्डम्_ABORT)
				गच्छ abort_exit;
		}
	}
	check_for_ro_violation_goto(&ctx, ret, abort_exit);
	यदि (inode.i_खण्डम्[EXT2_IND_खण्डम्] || (flags & खण्डम्_FLAG_APPEND)) {
		ret |= खण्डम्_iterate_ind(&inode.i_खण्डम्[EXT2_IND_खण्डम्], 
					 0, EXT2_IND_खण्डम्, &ctx);
		यदि (ret & खण्डम्_ABORT)
			गच्छ abort_exit;
	} अन्यथा
		ctx.bcount += सीमा;
	यदि (inode.i_खण्डम्[EXT2_DIND_खण्डम्] || (flags & खण्डम्_FLAG_APPEND)) {
		ret |= खण्डम्_iterate_dind(&inode.i_खण्डम्[EXT2_DIND_खण्डम्],
					  0, EXT2_DIND_खण्डम्, &ctx);
		यदि (ret & खण्डम्_ABORT)
			गच्छ abort_exit;
	} अन्यथा
		ctx.bcount += सीमा * सीमा;
	यदि (inode.i_खण्डम्[EXT2_TIND_खण्डम्] || (flags & खण्डम्_FLAG_APPEND)) {
		ret |= खण्डम्_iterate_tind(&inode.i_खण्डम्[EXT2_TIND_खण्डम्],
					  0, EXT2_TIND_खण्डम्, &ctx);
		यदि (ret & खण्डम्_ABORT)
			गच्छ abort_exit;
	}

abort_exit:
	यदि (ret & खण्डम्_परिवर्तितम्) {
		retval = ext2fs_write_inode(fs, ino, &inode);
		यदि (retval) {
			ret |= खण्डम्_ERROR;
			ctx.errcode = retval;
		}
	}
errout:
	यदि (!खण्डम्_buf)
		ext2fs_free_mem(&ctx.ind_buf);

	प्रत्यागम (ret & खण्डम्_ERROR) ? ctx.errcode : 0;
}

/*
 * Emulate the old ext2fs_block_iterate function!
 */

संरचना xlate64 {
	पूर्णाङ्क (*func)(ext2_filsys fs,
		    blk_t	*blocknr,
		    e2_blkcnt_t	blockcnt,
		    blk_t	ref_blk,
		    पूर्णाङ्क		ref_विस्थापनम्,
		    शून्य	*priv_दत्त);
	शून्य *real_वैयक्तिकम्;
};

स्थिर पूर्णाङ्क xlate64_func(ext2_filsys fs, blk64_t	*blocknr,
			e2_blkcnt_t blockcnt, blk64_t ref_blk,
			पूर्णाङ्क ref_विस्थापनम्, शून्य *priv_दत्त)
{
	संरचना xlate64 *xl = (संरचना xlate64 *) priv_दत्त;
	पूर्णाङ्क		ret;
	blk_t		block32 = *blocknr;
	
	ret = (*xl->func)(fs, &block32, blockcnt, (blk_t) ref_blk, ref_विस्थापनम्,
			     xl->real_वैयक्तिकम्);
	*blocknr = block32;
	प्रत्यागम ret;
}

errcode_t ext2fs_खण्डम्_iterate2(ext2_filsys fs,
				ext2_ino_t ino,
				पूर्णाङ्क	flags,
				वर्ण *खण्डम्_buf,
				पूर्णाङ्क (*func)(ext2_filsys fs,
					    blk_t	*blocknr,
					    e2_blkcnt_t	blockcnt,
					    blk_t	ref_blk,
					    पूर्णाङ्क		ref_विस्थापनम्,
					    शून्य	*priv_दत्त),
				शून्य *priv_दत्त)
{
	संरचना xlate64 xl;

	xl.real_वैयक्तिकम् = priv_दत्त;
	xl.func = func;

	प्रत्यागम ext2fs_खण्डम्_iterate3(fs, ino, flags, खण्डम्_buf, 
				     xlate64_func, &xl);
}


संरचना xlate {
	पूर्णाङ्क (*func)(ext2_filsys	fs,
		    blk_t	*blocknr,
		    पूर्णाङ्क		bcount,
		    शून्य	*priv_दत्त);
	शून्य *real_वैयक्तिकम्;
};

#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
स्थिर पूर्णाङ्क xlate_func(ext2_filsys fs, blk_t *blocknr, e2_blkcnt_t blockcnt,
		      blk_t ref_खण्डम् EXT2FS_ATTR((अप्रयुक्तम्)),
		      पूर्णाङ्क ref_विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
		      शून्य *priv_दत्त)
{
	संरचना xlate *xl = (संरचना xlate *) priv_दत्त;

	प्रत्यागम (*xl->func)(fs, blocknr, (पूर्णाङ्क) blockcnt, xl->real_वैयक्तिकम्);
}

errcode_t ext2fs_खण्डम्_iterate(ext2_filsys fs,
			       ext2_ino_t ino,
			       पूर्णाङ्क	flags,
			       वर्ण *खण्डम्_buf,
			       पूर्णाङ्क (*func)(ext2_filsys fs,
					   blk_t	*blocknr,
					   पूर्णाङ्क	blockcnt,
					   शून्य	*priv_दत्त),
			       शून्य *priv_दत्त)
{
	संरचना xlate xl;

	xl.real_वैयक्तिकम् = priv_दत्त;
	xl.func = func;

	प्रत्यागम ext2fs_खण्डम्_iterate2(fs, ino, खण्डम्_FLAG_NO_LARGE | flags,
				     खण्डम्_buf, xlate_func, &xl);
}

