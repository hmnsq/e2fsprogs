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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#تعریف_کرو EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     بنية ext2_dir_اندراج *dirent,
			     بنانشانی صحيح *rec_len)
{
	بنانشانی صحيح len = dirent->rec_len;

	إذا (fs->blocksize < 65536)
		*rec_len = len;
	وإلا إذا (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	وإلا 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	أرجع 0;
}

errcode_t ext2fs_سیٹ_rec_len(ext2_filsys fs,
			     بنانشانی صحيح len,
			     بنية ext2_dir_اندراج *dirent)
{
	إذا ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		أرجع EINVAL;
	إذا (len < 65536) {
		dirent->rec_len = len;
		أرجع 0;
	}
	إذا (len == fs->blocksize) {
		إذا (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		وإلا 
			dirent->rec_len = 0;
	} وإلا
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	أرجع 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
ساكن صحيح ext2fs_validate_اندراج(ext2_filsys fs, حرف *buf,
				 بنانشانی صحيح وتھ,
				 بنانشانی صحيح final_وتھ)
{
	بنية ext2_dir_اندراج *dirent;
	بنانشانی صحيح rec_len;
#تعریف_کرو DIRENT_MIN_لمائی 12

	طالما ((وتھ < final_وتھ) &&
	       (وتھ <= fs->blocksize - DIRENT_MIN_لمائی)) {
		dirent = (بنية ext2_dir_اندراج *)(buf + وتھ);
		إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
			أرجع 0;
		وتھ += rec_len;
		إذا ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحيح) rec_len))
			أرجع 0;
	}
	أرجع (وتھ == final_وتھ);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *بلاک_buf,
			      صحيح (*func)(ext2_ino_t	dir,
					  صحيح		اندراج,
					  بنية ext2_dir_اندراج *dirent,
					  صحيح	وتھ,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_انکڑے),
			      فراغ *priv_انکڑے)
{
	بنية		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	إذا (retval)
		أرجع retval;

	ctx.dir = dir;
	ctx.flags = flags;
	إذا (بلاک_buf)
		ctx.buf = بلاک_buf;
	وإلا {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;
	}
	ctx.func = func;
	ctx.priv_انکڑے = priv_انکڑے;
	ctx.errcode = 0;
	retval = ext2fs_بلاک_iterate3(fs, dir, بلاک_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_بلاک, &ctx);
	إذا (!بلاک_buf)
		ext2fs_free_mem(&ctx.buf);
	إذا (retval == EXT2_ET_INLINE_انکڑے_CANT_ITERATE) {
		(فراغ) ext2fs_inline_انکڑے_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	إذا (retval)
		أرجع retval;
	أرجع ctx.errcode;
}

بنية xlate {
	صحيح (*func)(بنية ext2_dir_اندراج *dirent,
		    صحيح		وتھ,
		    صحيح		blocksize,
		    حرف	*buf,
		    فراغ	*priv_انکڑے);
	فراغ *real_نجی;
};

ساكن صحيح xlate_func(ext2_ino_t dir EXT2FS_ATTR((بن_ورتیا)),
		      صحيح اندراج EXT2FS_ATTR((بن_ورتیا)),
		      بنية ext2_dir_اندراج *dirent, صحيح وتھ,
		      صحيح blocksize, حرف *buf, فراغ *priv_انکڑے)
{
	بنية xlate *xl = (بنية xlate *) priv_انکڑے;

	أرجع (*xl->func)(dirent, وتھ, blocksize, buf, xl->real_نجی);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     صحيح flags,
			     حرف *بلاک_buf,
			     صحيح (*func)(بنية ext2_dir_اندراج *dirent,
					 صحيح	وتھ,
					 صحيح	blocksize,
					 حرف	*buf,
					 فراغ	*priv_انکڑے),
			     فراغ *priv_انکڑے)
{
	بنية xlate xl;

	xl.real_نجی = priv_انکڑے;
	xl.func = func;

	أرجع ext2fs_dir_iterate2(fs, dir, flags, بلاک_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
صحيح ext2fs_process_dir_بلاک(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_بلاک EXT2FS_ATTR((بن_ورتیا)),
			     صحيح	ref_وتھ EXT2FS_ATTR((بن_ورتیا)),
			     فراغ	*priv_انکڑے)
{
	بنية dir_context *ctx = (بنية dir_context *) priv_انکڑے;
	بنانشانی صحيح	وتھ = 0;
	بنانشانی صحيح	اگلا_real_اندراج = 0;
	صحيح		ret = 0;
	صحيح		بدلیا = 0;
	صحيح		do_abort = 0;
	بنانشانی صحيح	rec_len, ناپ, buflen;
	صحيح		اندراج;
	بنية ext2_dir_اندراج *dirent;
	صحيح		csum_ناپ = 0;
	صحيح		inline_انکڑے;
	errcode_t	retval = 0;

	إذا (blockcnt < 0)
		أرجع 0;

	اندراج = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_انکڑے = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_انکڑے);
	إذا (!inline_انکڑے) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		إذا (ctx->errcode)
			أرجع بلاک_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} وإلا {
		buflen = ctx->buflen;
	}

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ناپ = حجم(بنية ext2_dir_اندراج_پوچھ);

	إذا (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		أرجع بلاک_ABORT;
	}
	طالما (وتھ < buflen - 8) {
		dirent = (بنية ext2_dir_اندراج *) (ctx->buf + وتھ);
		إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
			أرجع بلاک_ABORT;
		إذا (((وتھ + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحيح) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			أرجع بلاک_ABORT;
		}
		إذا (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			إذا (!inline_انکڑے &&
			    (وتھ == buflen - csum_ناپ) &&
			    (dirent->rec_len == csum_ناپ) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				إذا (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					اذهب اگلا;
				اندراج = DIRENT_CHECKSUM;
			} وإلا إذا (!(ctx->flags & DIRENT_FLAG_INCLUDE_خالی_تھاواں))
				اذهب اگلا;
		}

		ret = (ctx->func)(ctx->dir,
				  (اگلا_real_اندراج > وتھ) ?
				  DIRENT_DELETED_FILE : اندراج,
				  dirent, وتھ,
				  buflen, ctx->buf,
				  ctx->priv_انکڑے);
		إذا (اندراج < DIRENT_OTHER_FILE)
			اندراج++;

		إذا (ret & DIRENT_بدلیا) {
			إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
				أرجع بلاک_ABORT;
			بدلیا++;
		}
		إذا (ret & DIRENT_ABORT) {
			do_abort++;
			اكسر;
		}
اگلا:
 		إذا (اگلا_real_اندراج == وتھ)
			اگلا_real_اندراج += rec_len;

 		إذا (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			ناپ = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			إذا (rec_len != ناپ)  {
				بنانشانی صحيح final_وتھ;

				final_وتھ = وتھ + rec_len;
				وتھ += ناپ;
				طالما (وتھ < final_وتھ &&
				       !ext2fs_validate_اندراج(fs, ctx->buf,
							      وتھ,
							      final_وتھ))
					وتھ += 4;
				استمر;
			}
		}
		وتھ += rec_len;
	}

	إذا (بدلیا) {
		إذا (!inline_انکڑے) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			إذا (ctx->errcode)
				أرجع بلاک_ABORT;
		} وإلا {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = بلاک_INLINE_انکڑے_بدلیا;
		}
	}
	إذا (do_abort)
		أرجع retval | بلاک_ABORT;
	أرجع retval;
}
