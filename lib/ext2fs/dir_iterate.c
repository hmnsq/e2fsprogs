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
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#وصف_ڪريو EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     بنية ext2_dir_داخلا *dirent,
			     بنانشان صحيح *rec_len)
{
	بنانشان صحيح len = dirent->rec_len;

	إذا (fs->blocksize < 65536)
		*rec_len = len;
	وإلا إذا (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	وإلا 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	أرجع 0;
}

errcode_t ext2fs_سيٽ_rec_len(ext2_filsys fs,
			     بنانشان صحيح len,
			     بنية ext2_dir_داخلا *dirent)
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
ساكن صحيح ext2fs_validate_داخلا(ext2_filsys fs, حرف *buf,
				 بنانشان صحيح وٿي,
				 بنانشان صحيح final_وٿي)
{
	بنية ext2_dir_داخلا *dirent;
	بنانشان صحيح rec_len;
#وصف_ڪريو DIRENT_MIN_ڊيگهائي 12

	طالما ((وٿي < final_وٿي) &&
	       (وٿي <= fs->blocksize - DIRENT_MIN_ڊيگهائي)) {
		dirent = (بنية ext2_dir_داخلا *)(buf + وٿي);
		إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
			أرجع 0;
		وٿي += rec_len;
		إذا ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحيح) rec_len))
			أرجع 0;
	}
	أرجع (وٿي == final_وٿي);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *بلاڪ_buf,
			      صحيح (*func)(ext2_ino_t	dir,
					  صحيح		داخلا,
					  بنية ext2_dir_داخلا *dirent,
					  صحيح	وٿي,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_معلومات),
			      فراغ *priv_معلومات)
{
	بنية		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	إذا (retval)
		أرجع retval;

	ctx.dir = dir;
	ctx.flags = flags;
	إذا (بلاڪ_buf)
		ctx.buf = بلاڪ_buf;
	وإلا {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;
	}
	ctx.func = func;
	ctx.priv_معلومات = priv_معلومات;
	ctx.errcode = 0;
	retval = ext2fs_بلاڪ_iterate3(fs, dir, بلاڪ_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_بلاڪ, &ctx);
	إذا (!بلاڪ_buf)
		ext2fs_free_mem(&ctx.buf);
	إذا (retval == EXT2_ET_INLINE_معلومات_CANT_ITERATE) {
		(فراغ) ext2fs_inline_معلومات_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	إذا (retval)
		أرجع retval;
	أرجع ctx.errcode;
}

بنية xlate {
	صحيح (*func)(بنية ext2_dir_داخلا *dirent,
		    صحيح		وٿي,
		    صحيح		blocksize,
		    حرف	*buf,
		    فراغ	*priv_معلومات);
	فراغ *real_ذاتي;
};

ساكن صحيح xlate_func(ext2_ino_t dir EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
		      صحيح داخلا EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
		      بنية ext2_dir_داخلا *dirent, صحيح وٿي,
		      صحيح blocksize, حرف *buf, فراغ *priv_معلومات)
{
	بنية xlate *xl = (بنية xlate *) priv_معلومات;

	أرجع (*xl->func)(dirent, وٿي, blocksize, buf, xl->real_ذاتي);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     صحيح flags,
			     حرف *بلاڪ_buf,
			     صحيح (*func)(بنية ext2_dir_داخلا *dirent,
					 صحيح	وٿي,
					 صحيح	blocksize,
					 حرف	*buf,
					 فراغ	*priv_معلومات),
			     فراغ *priv_معلومات)
{
	بنية xlate xl;

	xl.real_ذاتي = priv_معلومات;
	xl.func = func;

	أرجع ext2fs_dir_iterate2(fs, dir, flags, بلاڪ_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
صحيح ext2fs_process_dir_بلاڪ(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_بلاڪ EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			     صحيح	ref_وٿي EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			     فراغ	*priv_معلومات)
{
	بنية dir_context *ctx = (بنية dir_context *) priv_معلومات;
	بنانشان صحيح	وٿي = 0;
	بنانشان صحيح	ايندڙ_real_داخلا = 0;
	صحيح		ret = 0;
	صحيح		بدليو = 0;
	صحيح		do_abort = 0;
	بنانشان صحيح	rec_len, سائيز, buflen;
	صحيح		داخلا;
	بنية ext2_dir_داخلا *dirent;
	صحيح		csum_سائيز = 0;
	صحيح		inline_معلومات;
	errcode_t	retval = 0;

	إذا (blockcnt < 0)
		أرجع 0;

	داخلا = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_معلومات = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_معلومات);
	إذا (!inline_معلومات) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		إذا (ctx->errcode)
			أرجع بلاڪ_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} وإلا {
		buflen = ctx->buflen;
	}

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائيز = حجم(بنية ext2_dir_داخلا_پڇ);

	إذا (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		أرجع بلاڪ_ABORT;
	}
	طالما (وٿي < buflen - 8) {
		dirent = (بنية ext2_dir_داخلا *) (ctx->buf + وٿي);
		إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
			أرجع بلاڪ_ABORT;
		إذا (((وٿي + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحيح) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			أرجع بلاڪ_ABORT;
		}
		إذا (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			إذا (!inline_معلومات &&
			    (وٿي == buflen - csum_سائيز) &&
			    (dirent->rec_len == csum_سائيز) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				إذا (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					اذهب ايندڙ;
				داخلا = DIRENT_CHECKSUM;
			} وإلا إذا (!(ctx->flags & DIRENT_FLAG_INCLUDE_خالي_جايون))
				اذهب ايندڙ;
		}

		ret = (ctx->func)(ctx->dir,
				  (ايندڙ_real_داخلا > وٿي) ?
				  DIRENT_DELETED_FILE : داخلا,
				  dirent, وٿي,
				  buflen, ctx->buf,
				  ctx->priv_معلومات);
		إذا (داخلا < DIRENT_OTHER_FILE)
			داخلا++;

		إذا (ret & DIRENT_بدليو) {
			إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
				أرجع بلاڪ_ABORT;
			بدليو++;
		}
		إذا (ret & DIRENT_ABORT) {
			do_abort++;
			اكسر;
		}
ايندڙ:
 		إذا (ايندڙ_real_داخلا == وٿي)
			ايندڙ_real_داخلا += rec_len;

 		إذا (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			سائيز = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			إذا (rec_len != سائيز)  {
				بنانشان صحيح final_وٿي;

				final_وٿي = وٿي + rec_len;
				وٿي += سائيز;
				طالما (وٿي < final_وٿي &&
				       !ext2fs_validate_داخلا(fs, ctx->buf,
							      وٿي,
							      final_وٿي))
					وٿي += 4;
				استمر;
			}
		}
		وٿي += rec_len;
	}

	إذا (بدليو) {
		إذا (!inline_معلومات) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			إذا (ctx->errcode)
				أرجع بلاڪ_ABORT;
		} وإلا {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = بلاڪ_INLINE_معلومات_بدليو;
		}
	}
	إذا (do_abort)
		أرجع retval | بلاڪ_ABORT;
	أرجع retval;
}
