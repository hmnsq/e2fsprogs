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
#د_که_پای
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#تعریف_کړه EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     بنية ext2_dir_ثبت *dirent,
			     بینښه صحيح *rec_len)
{
	بینښه صحيح len = dirent->rec_len;

	إذا (fs->blocksize < 65536)
		*rec_len = len;
	وإلا إذا (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	وإلا 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	أرجع 0;
}

errcode_t ext2fs_سېټ_rec_len(ext2_filsys fs,
			     بینښه صحيح len,
			     بنية ext2_dir_ثبت *dirent)
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
ساكن صحيح ext2fs_validate_ثبت(ext2_filsys fs, حرف *buf,
				 بینښه صحيح دننه_واټن,
				 بینښه صحيح final_دننه_واټن)
{
	بنية ext2_dir_ثبت *dirent;
	بینښه صحيح rec_len;
#تعریف_کړه DIRENT_MIN_اوږدتیا 12

	طالما ((دننه_واټن < final_دننه_واټن) &&
	       (دننه_واټن <= fs->blocksize - DIRENT_MIN_اوږدتیا)) {
		dirent = (بنية ext2_dir_ثبت *)(buf + دننه_واټن);
		إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
			أرجع 0;
		دننه_واټن += rec_len;
		إذا ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحيح) rec_len))
			أرجع 0;
	}
	أرجع (دننه_واټن == final_دننه_واټن);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *بلاک_buf,
			      صحيح (*func)(ext2_ino_t	dir,
					  صحيح		ثبت,
					  بنية ext2_dir_ثبت *dirent,
					  صحيح	دننه_واټن,
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
	إذا (بلاک_buf)
		ctx.buf = بلاک_buf;
	وإلا {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;
	}
	ctx.func = func;
	ctx.priv_معلومات = priv_معلومات;
	ctx.errcode = 0;
	retval = ext2fs_بلاک_iterate3(fs, dir, بلاک_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_بلاک, &ctx);
	إذا (!بلاک_buf)
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
	صحيح (*func)(بنية ext2_dir_ثبت *dirent,
		    صحيح		دننه_واټن,
		    صحيح		blocksize,
		    حرف	*buf,
		    فراغ	*priv_معلومات);
	فراغ *real_شخصي;
};

ساكن صحيح xlate_func(ext2_ino_t dir EXT2FS_ATTR((ناکارول_شوی)),
		      صحيح ثبت EXT2FS_ATTR((ناکارول_شوی)),
		      بنية ext2_dir_ثبت *dirent, صحيح دننه_واټن,
		      صحيح blocksize, حرف *buf, فراغ *priv_معلومات)
{
	بنية xlate *xl = (بنية xlate *) priv_معلومات;

	أرجع (*xl->func)(dirent, دننه_واټن, blocksize, buf, xl->real_شخصي);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     صحيح flags,
			     حرف *بلاک_buf,
			     صحيح (*func)(بنية ext2_dir_ثبت *dirent,
					 صحيح	دننه_واټن,
					 صحيح	blocksize,
					 حرف	*buf,
					 فراغ	*priv_معلومات),
			     فراغ *priv_معلومات)
{
	بنية xlate xl;

	xl.real_شخصي = priv_معلومات;
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
			     blk64_t	ref_بلاک EXT2FS_ATTR((ناکارول_شوی)),
			     صحيح	ref_دننه_واټن EXT2FS_ATTR((ناکارول_شوی)),
			     فراغ	*priv_معلومات)
{
	بنية dir_context *ctx = (بنية dir_context *) priv_معلومات;
	بینښه صحيح	دننه_واټن = 0;
	بینښه صحيح	بل_real_ثبت = 0;
	صحيح		ret = 0;
	صحيح		بدل_شوی = 0;
	صحيح		do_abort = 0;
	بینښه صحيح	rec_len, اندازه, buflen;
	صحيح		ثبت;
	بنية ext2_dir_ثبت *dirent;
	صحيح		csum_اندازه = 0;
	صحيح		inline_معلومات;
	errcode_t	retval = 0;

	إذا (blockcnt < 0)
		أرجع 0;

	ثبت = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_معلومات = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_معلومات);
	إذا (!inline_معلومات) {
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
		csum_اندازه = حجم(بنية ext2_dir_ثبت_لکۍ);

	إذا (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		أرجع بلاک_ABORT;
	}
	طالما (دننه_واټن < buflen - 8) {
		dirent = (بنية ext2_dir_ثبت *) (ctx->buf + دننه_واټن);
		إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
			أرجع بلاک_ABORT;
		إذا (((دننه_واټن + rec_len) > buflen) ||
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
			إذا (!inline_معلومات &&
			    (دننه_واټن == buflen - csum_اندازه) &&
			    (dirent->rec_len == csum_اندازه) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				إذا (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					اذهب بل;
				ثبت = DIRENT_CHECKSUM;
			} وإلا إذا (!(ctx->flags & DIRENT_FLAG_INCLUDE_تشه))
				اذهب بل;
		}

		ret = (ctx->func)(ctx->dir,
				  (بل_real_ثبت > دننه_واټن) ?
				  DIRENT_DELETED_FILE : ثبت,
				  dirent, دننه_واټن,
				  buflen, ctx->buf,
				  ctx->priv_معلومات);
		إذا (ثبت < DIRENT_OTHER_FILE)
			ثبت++;

		إذا (ret & DIRENT_بدل_شوی) {
			إذا (ext2fs_get_rec_len(fs, dirent, &rec_len))
				أرجع بلاک_ABORT;
			بدل_شوی++;
		}
		إذا (ret & DIRENT_ABORT) {
			do_abort++;
			اكسر;
		}
بل:
 		إذا (بل_real_ثبت == دننه_واټن)
			بل_real_ثبت += rec_len;

 		إذا (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			اندازه = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			إذا (rec_len != اندازه)  {
				بینښه صحيح final_دننه_واټن;

				final_دننه_واټن = دننه_واټن + rec_len;
				دننه_واټن += اندازه;
				طالما (دننه_واټن < final_دننه_واټن &&
				       !ext2fs_validate_ثبت(fs, ctx->buf,
							      دننه_واټن,
							      final_دننه_واټن))
					دننه_واټن += 4;
				استمر;
			}
		}
		دننه_واټن += rec_len;
	}

	إذا (بدل_شوی) {
		إذا (!inline_معلومات) {
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
			retval = بلاک_INLINE_معلومات_بدل_شوی;
		}
	}
	إذا (do_abort)
		أرجع retval | بلاک_ABORT;
	أرجع retval;
}
