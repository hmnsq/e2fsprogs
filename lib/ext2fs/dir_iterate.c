/*
 * dir_iterate.c --- ext2fs directory iteration operations
 *
 * Copyright (C) 1993, 1994, 1994, 1995, 1996, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

#నిర్వచించు EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     నిర్మాణం ext2_dir_నమోదు *dirent,
			     గుర్తులేని పూర్ణాంకం *rec_len)
{
	గుర్తులేని పూర్ణాంకం len = dirent->rec_len;

	ఒకవేళ (fs->blocksize < 65536)
		*rec_len = len;
	లేకపోతే ఒకవేళ (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	లేకపోతే 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	తిరిగి 0;
}

errcode_t ext2fs_సమితి_rec_len(ext2_filsys fs,
			     గుర్తులేని పూర్ణాంకం len,
			     నిర్మాణం ext2_dir_నమోదు *dirent)
{
	ఒకవేళ ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		తిరిగి EINVAL;
	ఒకవేళ (len < 65536) {
		dirent->rec_len = len;
		తిరిగి 0;
	}
	ఒకవేళ (len == fs->blocksize) {
		ఒకవేళ (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		లేకపోతే 
			dirent->rec_len = 0;
	} లేకపోతే
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	తిరిగి 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
స్థిర పూర్ణాంకం ext2fs_validate_నమోదు(ext2_filsys fs, అక్షరం *buf,
				 గుర్తులేని పూర్ణాంకం స్థానభ్రంశం,
				 గుర్తులేని పూర్ణాంకం final_స్థానభ్రంశం)
{
	నిర్మాణం ext2_dir_నమోదు *dirent;
	గుర్తులేని పూర్ణాంకం rec_len;
#నిర్వచించు DIRENT_MIN_నిడివి 12

	అయితే ((స్థానభ్రంశం < final_స్థానభ్రంశం) &&
	       (స్థానభ్రంశం <= fs->blocksize - DIRENT_MIN_నిడివి)) {
		dirent = (నిర్మాణం ext2_dir_నమోదు *)(buf + స్థానభ్రంశం);
		ఒకవేళ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			తిరిగి 0;
		స్థానభ్రంశం += rec_len;
		ఒకవేళ ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (పూర్ణాంకం) rec_len))
			తిరిగి 0;
	}
	తిరిగి (స్థానభ్రంశం == final_స్థానభ్రంశం);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      పూర్ణాంకం flags,
			      అక్షరం *ఖండం_buf,
			      పూర్ణాంకం (*func)(ext2_ino_t	dir,
					  పూర్ణాంకం		నమోదు_2,
					  నిర్మాణం ext2_dir_నమోదు *dirent,
					  పూర్ణాంకం	స్థానభ్రంశం,
					  పూర్ణాంకం	blocksize,
					  అక్షరం	*buf,
					  శూన్య	*priv_దత్తాంశం),
			      శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ఒకవేళ (retval)
		తిరిగి retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ఒకవేళ (ఖండం_buf)
		ctx.buf = ఖండం_buf;
	లేకపోతే {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	ctx.func = func;
	ctx.priv_దత్తాంశం = priv_దత్తాంశం;
	ctx.errcode = 0;
	retval = ext2fs_ఖండం_iterate3(fs, dir, ఖండం_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_ఖండం, &ctx);
	ఒకవేళ (!ఖండం_buf)
		ext2fs_free_mem(&ctx.buf);
	ఒకవేళ (retval == EXT2_ET_INLINE_దత్తాంశం_CANT_ITERATE) {
		(శూన్య) ext2fs_inline_దత్తాంశం_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ఒకవేళ (retval)
		తిరిగి retval;
	తిరిగి ctx.errcode;
}

నిర్మాణం xlate {
	పూర్ణాంకం (*func)(నిర్మాణం ext2_dir_నమోదు *dirent,
		    పూర్ణాంకం		స్థానభ్రంశం,
		    పూర్ణాంకం		blocksize,
		    అక్షరం	*buf,
		    శూన్య	*priv_దత్తాంశం);
	శూన్య *real_వ్యక్తిగత;
};

స్థిర పూర్ణాంకం xlate_func(ext2_ino_t dir EXT2FS_ATTR((వాడని)),
		      పూర్ణాంకం నమోదు_2 EXT2FS_ATTR((వాడని)),
		      నిర్మాణం ext2_dir_నమోదు *dirent, పూర్ణాంకం స్థానభ్రంశం,
		      పూర్ణాంకం blocksize, అక్షరం *buf, శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం xlate *xl = (నిర్మాణం xlate *) priv_దత్తాంశం;

	తిరిగి (*xl->func)(dirent, స్థానభ్రంశం, blocksize, buf, xl->real_వ్యక్తిగత);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     పూర్ణాంకం flags,
			     అక్షరం *ఖండం_buf,
			     పూర్ణాంకం (*func)(నిర్మాణం ext2_dir_నమోదు *dirent,
					 పూర్ణాంకం	స్థానభ్రంశం,
					 పూర్ణాంకం	blocksize,
					 అక్షరం	*buf,
					 శూన్య	*priv_దత్తాంశం),
			     శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం xlate xl;

	xl.real_వ్యక్తిగత = priv_దత్తాంశం;
	xl.func = func;

	తిరిగి ext2fs_dir_iterate2(fs, dir, flags, ఖండం_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
పూర్ణాంకం ext2fs_process_dir_ఖండం(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_ఖండం EXT2FS_ATTR((వాడని)),
			     పూర్ణాంకం	ref_స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
			     శూన్య	*priv_దత్తాంశం)
{
	నిర్మాణం dir_context *ctx = (నిర్మాణం dir_context *) priv_దత్తాంశం;
	గుర్తులేని పూర్ణాంకం	స్థానభ్రంశం = 0;
	గుర్తులేని పూర్ణాంకం	తదుపరి_real_నమోదు = 0;
	పూర్ణాంకం		ret = 0;
	పూర్ణాంకం		మారింది = 0;
	పూర్ణాంకం		do_abort = 0;
	గుర్తులేని పూర్ణాంకం	rec_len, కొలత, buflen;
	పూర్ణాంకం		నమోదు_2;
	నిర్మాణం ext2_dir_నమోదు *dirent;
	పూర్ణాంకం		csum_కొలత = 0;
	పూర్ణాంకం		inline_దత్తాంశం;
	errcode_t	retval = 0;

	ఒకవేళ (blockcnt < 0)
		తిరిగి 0;

	నమోదు_2 = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_దత్తాంశం = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_దత్తాంశం);
	ఒకవేళ (!inline_దత్తాంశం) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ఒకవేళ (ctx->errcode)
			తిరిగి ఖండం_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} లేకపోతే {
		buflen = ctx->buflen;
	}

	ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_కొలత = పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక);

	ఒకవేళ (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		తిరిగి ఖండం_ABORT;
	}
	అయితే (స్థానభ్రంశం < buflen - 8) {
		dirent = (నిర్మాణం ext2_dir_నమోదు *) (ctx->buf + స్థానభ్రంశం);
		ఒకవేళ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			తిరిగి ఖండం_ABORT;
		ఒకవేళ (((స్థానభ్రంశం + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (పూర్ణాంకం) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			తిరిగి ఖండం_ABORT;
		}
		ఒకవేళ (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ఒకవేళ (!inline_దత్తాంశం &&
			    (స్థానభ్రంశం == buflen - csum_కొలత) &&
			    (dirent->rec_len == csum_కొలత) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ఒకవేళ (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					వెళ్ళు తదుపరి;
				నమోదు_2 = DIRENT_CHECKSUM;
			} లేకపోతే ఒకవేళ (!(ctx->flags & DIRENT_FLAG_INCLUDE_ఖాళీలు))
				వెళ్ళు తదుపరి;
		}

		ret = (ctx->func)(ctx->dir,
				  (తదుపరి_real_నమోదు > స్థానభ్రంశం) ?
				  DIRENT_DELETED_FILE : నమోదు_2,
				  dirent, స్థానభ్రంశం,
				  buflen, ctx->buf,
				  ctx->priv_దత్తాంశం);
		ఒకవేళ (నమోదు_2 < DIRENT_OTHER_FILE)
			నమోదు_2++;

		ఒకవేళ (ret & DIRENT_మారింది) {
			ఒకవేళ (ext2fs_get_rec_len(fs, dirent, &rec_len))
				తిరిగి ఖండం_ABORT;
			మారింది++;
		}
		ఒకవేళ (ret & DIRENT_ABORT) {
			do_abort++;
			ఆపు;
		}
తదుపరి:
 		ఒకవేళ (తదుపరి_real_నమోదు == స్థానభ్రంశం)
			తదుపరి_real_నమోదు += rec_len;

 		ఒకవేళ (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			కొలత = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ఒకవేళ (rec_len != కొలత)  {
				గుర్తులేని పూర్ణాంకం final_స్థానభ్రంశం;

				final_స్థానభ్రంశం = స్థానభ్రంశం + rec_len;
				స్థానభ్రంశం += కొలత;
				అయితే (స్థానభ్రంశం < final_స్థానభ్రంశం &&
				       !ext2fs_validate_నమోదు(fs, ctx->buf,
							      స్థానభ్రంశం,
							      final_స్థానభ్రంశం))
					స్థానభ్రంశం += 4;
				కొనసాగు;
			}
		}
		స్థానభ్రంశం += rec_len;
	}

	ఒకవేళ (మారింది) {
		ఒకవేళ (!inline_దత్తాంశం) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ఒకవేళ (ctx->errcode)
				తిరిగి ఖండం_ABORT;
		} లేకపోతే {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = ఖండం_INLINE_దత్తాంశం_మారింది;
		}
	}
	ఒకవేళ (do_abort)
		తిరిగి retval | ఖండం_ABORT;
	తిరిగి retval;
}
