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
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#تعریف_کن EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     بنية ext2_dir_مدخل *dirent,
			     بیعلامت صحيح *rec_len)
{
	بیعلامت صحيح len = dirent->rec_len;

	إذا (fs->blocksize < 65536)
		*rec_len = len;
	وإلا إذا (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	وإلا 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	أرجع 0;
}

errcode_t ext2fs_مجموعه_rec_len(ext2_filsys fs,
			     بیعلامت صحيح len,
			     بنية ext2_dir_مدخل *dirent)
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
ساكن صحيح ext2fs_validate_مدخل(ext2_filsys fs, حرف *buf,
				 بیعلامت صحيح جابه_جایی,
				 بیعلامت صحيح final_جابه_جایی)
{
	بنية ext2_dir_مدخل *dirent;
	بیعلامت صحيح rec_len;
#تعریف_کن DIRENT_MIN_درازی 12

	طالما ((جابه_جایی < final_جابه_جایی) &&
	       (جابه_جایی <= fs->blocksize - DIRENT_MIN_درازی)) {
		dirent = (بنية ext2_dir_مدخل *)(buf + جابه_جایی);
		إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
			أرجع 0;
		جابه_جایی += rec_len;
		إذا ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحيح) rec_len))
			أرجع 0;
	}
	أرجع (جابه_جایی == final_جابه_جایی);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *بلوک_buf,
			      صحيح (*func)(ext2_ino_t	dir,
					  صحيح		مدخل,
					  بنية ext2_dir_مدخل *dirent,
					  صحيح	جابه_جایی,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_داده),
			      فراغ *priv_داده)
{
	بنية		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	إذا (retval)
		أرجع retval;

	ctx.dir = dir;
	ctx.flags = flags;
	إذا (بلوک_buf)
		ctx.buf = بلوک_buf;
	وإلا {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;
	}
	ctx.func = func;
	ctx.priv_داده = priv_داده;
	ctx.errcode = 0;
	retval = ext2fs_بلوک_iterate3(fs, dir, بلوک_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_بلوک, &ctx);
	إذا (!بلوک_buf)
		ext2fs_free_mem(&ctx.buf);
	إذا (retval == EXT2_ET_INLINE_داده_CANT_ITERATE) {
		(فراغ) ext2fs_inline_داده_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	إذا (retval)
		أرجع retval;
	أرجع ctx.errcode;
}

بنية xlate {
	صحيح (*func)(بنية ext2_dir_مدخل *dirent,
		    صحيح		جابه_جایی,
		    صحيح		blocksize,
		    حرف	*buf,
		    فراغ	*priv_داده);
	فراغ *real_خصوصی;
};

ساكن صحيح xlate_func(ext2_ino_t dir EXT2FS_ATTR((استفادهنشده)),
		      صحيح مدخل EXT2FS_ATTR((استفادهنشده)),
		      بنية ext2_dir_مدخل *dirent, صحيح جابه_جایی,
		      صحيح blocksize, حرف *buf, فراغ *priv_داده)
{
	بنية xlate *xl = (بنية xlate *) priv_داده;

	أرجع (*xl->func)(dirent, جابه_جایی, blocksize, buf, xl->real_خصوصی);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     صحيح flags,
			     حرف *بلوک_buf,
			     صحيح (*func)(بنية ext2_dir_مدخل *dirent,
					 صحيح	جابه_جایی,
					 صحيح	blocksize,
					 حرف	*buf,
					 فراغ	*priv_داده),
			     فراغ *priv_داده)
{
	بنية xlate xl;

	xl.real_خصوصی = priv_داده;
	xl.func = func;

	أرجع ext2fs_dir_iterate2(fs, dir, flags, بلوک_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
صحيح ext2fs_process_dir_بلوک(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_بلوک EXT2FS_ATTR((استفادهنشده)),
			     صحيح	ref_جابه_جایی EXT2FS_ATTR((استفادهنشده)),
			     فراغ	*priv_داده)
{
	بنية dir_context *ctx = (بنية dir_context *) priv_داده;
	بیعلامت صحيح	جابه_جایی = 0;
	بیعلامت صحيح	بعدی_real_مدخل = 0;
	صحيح		ret = 0;
	صحيح		تغییریافته = 0;
	صحيح		do_abort = 0;
	بیعلامت صحيح	rec_len, اندازه, buflen;
	صحيح		مدخل;
	بنية ext2_dir_مدخل *dirent;
	صحيح		csum_اندازه = 0;
	صحيح		inline_داده;
	errcode_t	retval = 0;

	إذا (blockcnt < 0)
		أرجع 0;

	مدخل = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_داده = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_داده);
	إذا (!inline_داده) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		إذا (ctx->errcode)
			أرجع بلوک_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} وإلا {
		buflen = ctx->buflen;
	}

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_اندازه = حجم(بنية ext2_dir_مدخل_دم);

	إذا (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		أرجع بلوک_ABORT;
	}
	طالما (جابه_جایی < buflen - 8) {
		dirent = (بنية ext2_dir_مدخل *) (ctx->buf + جابه_جایی);
		إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
			أرجع بلوک_ABORT;
		إذا (((جابه_جایی + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحيح) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			أرجع بلوک_ABORT;
		}
		إذا (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			إذا (!inline_داده &&
			    (جابه_جایی == buflen - csum_اندازه) &&
			    (dirent->rec_len == csum_اندازه) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				إذا (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					اذهب بعدی;
				مدخل = DIRENT_CHECKSUM;
			} وإلا إذا (!(ctx->flags & DIRENT_FLAG_INCLUDE_خالی))
				اذهب بعدی;
		}

		ret = (ctx->func)(ctx->dir,
				  (بعدی_real_مدخل > جابه_جایی) ?
				  DIRENT_DELETED_FILE : مدخل,
				  dirent, جابه_جایی,
				  buflen, ctx->buf,
				  ctx->priv_داده);
		إذا (مدخل < DIRENT_OTHER_FILE)
			مدخل++;

		إذا (ret & DIRENT_تغییریافته) {
			إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
				أرجع بلوک_ABORT;
			تغییریافته++;
		}
		إذا (ret & DIRENT_ABORT) {
			do_abort++;
			اكسر;
		}
بعدی:
 		إذا (بعدی_real_مدخل == جابه_جایی)
			بعدی_real_مدخل += rec_len;

 		إذا (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			اندازه = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			إذا (rec_len != اندازه)  {
				بیعلامت صحيح final_جابه_جایی;

				final_جابه_جایی = جابه_جایی + rec_len;
				جابه_جایی += اندازه;
				طالما (جابه_جایی < final_جابه_جایی &&
				       !ext2fs_validate_مدخل(fs, ctx->buf,
							      جابه_جایی,
							      final_جابه_جایی))
					جابه_جایی += 4;
				استمر;
			}
		}
		جابه_جایی += rec_len;
	}

	إذا (تغییریافته) {
		إذا (!inline_داده) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			إذا (ctx->errcode)
				أرجع بلوک_ABORT;
		} وإلا {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = بلوک_INLINE_داده_تغییریافته;
		}
	}
	إذا (do_abort)
		أرجع retval | بلوک_ABORT;
	أرجع retval;
}
