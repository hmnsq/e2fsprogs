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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

#تعریف_کرو EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     ساخت ext2_dir_اندراج *dirent,
			     غیرعلامتی صحیح *rec_len)
{
	غیرعلامتی صحیح len = dirent->rec_len;

	اگر (fs->blocksize < 65536)
		*rec_len = len;
	ورنہ اگر (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	ورنہ 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	واپس 0;
}

errcode_t ext2fs_سیٹ_rec_len(ext2_filsys fs,
			     غیرعلامتی صحیح len,
			     ساخت ext2_dir_اندراج *dirent)
{
	اگر ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		واپس EINVAL;
	اگر (len < 65536) {
		dirent->rec_len = len;
		واپس 0;
	}
	اگر (len == fs->blocksize) {
		اگر (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		ورنہ 
			dirent->rec_len = 0;
	} ورنہ
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	واپس 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
ساکن صحیح ext2fs_validate_اندراج(ext2_filsys fs, حرف *buf,
				 غیرعلامتی صحیح انحراف,
				 غیرعلامتی صحیح final_انحراف)
{
	ساخت ext2_dir_اندراج *dirent;
	غیرعلامتی صحیح rec_len;
#تعریف_کرو DIRENT_MIN_لمبائی 12

	جب ((انحراف < final_انحراف) &&
	       (انحراف <= fs->blocksize - DIRENT_MIN_لمبائی)) {
		dirent = (ساخت ext2_dir_اندراج *)(buf + انحراف);
		اگر (ext2fs_get_rec_len(fs, dirent, &rec_len))
			واپس 0;
		انحراف += rec_len;
		اگر ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحیح) rec_len))
			واپس 0;
	}
	واپس (انحراف == final_انحراف);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحیح flags,
			      حرف *بلاک_buf,
			      صحیح (*func)(ext2_ino_t	dir,
					  صحیح		اندراج_2,
					  ساخت ext2_dir_اندراج *dirent,
					  صحیح	انحراف,
					  صحیح	blocksize,
					  حرف	*buf,
					  خالی	*priv_ڈیٹا),
			      خالی *priv_ڈیٹا)
{
	ساخت		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	اگر (retval)
		واپس retval;

	ctx.dir = dir;
	ctx.flags = flags;
	اگر (بلاک_buf)
		ctx.buf = بلاک_buf;
	ورنہ {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		اگر (retval)
			واپس retval;
	}
	ctx.func = func;
	ctx.priv_ڈیٹا = priv_ڈیٹا;
	ctx.errcode = 0;
	retval = ext2fs_بلاک_iterate3(fs, dir, بلاک_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_بلاک, &ctx);
	اگر (!بلاک_buf)
		ext2fs_free_mem(&ctx.buf);
	اگر (retval == EXT2_ET_INLINE_ڈیٹا_CANT_ITERATE) {
		(خالی) ext2fs_inline_ڈیٹا_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	اگر (retval)
		واپس retval;
	واپس ctx.errcode;
}

ساخت xlate {
	صحیح (*func)(ساخت ext2_dir_اندراج *dirent,
		    صحیح		انحراف,
		    صحیح		blocksize,
		    حرف	*buf,
		    خالی	*priv_ڈیٹا);
	خالی *real_نجی;
};

ساکن صحیح xlate_func(ext2_ino_t dir EXT2FS_ATTR((غیر_مستعمل)),
		      صحیح اندراج_2 EXT2FS_ATTR((غیر_مستعمل)),
		      ساخت ext2_dir_اندراج *dirent, صحیح انحراف,
		      صحیح blocksize, حرف *buf, خالی *priv_ڈیٹا)
{
	ساخت xlate *xl = (ساخت xlate *) priv_ڈیٹا;

	واپس (*xl->func)(dirent, انحراف, blocksize, buf, xl->real_نجی);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     صحیح flags,
			     حرف *بلاک_buf,
			     صحیح (*func)(ساخت ext2_dir_اندراج *dirent,
					 صحیح	انحراف,
					 صحیح	blocksize,
					 حرف	*buf,
					 خالی	*priv_ڈیٹا),
			     خالی *priv_ڈیٹا)
{
	ساخت xlate xl;

	xl.real_نجی = priv_ڈیٹا;
	xl.func = func;

	واپس ext2fs_dir_iterate2(fs, dir, flags, بلاک_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
صحیح ext2fs_process_dir_بلاک(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_بلاک EXT2FS_ATTR((غیر_مستعمل)),
			     صحیح	ref_انحراف EXT2FS_ATTR((غیر_مستعمل)),
			     خالی	*priv_ڈیٹا)
{
	ساخت dir_context *ctx = (ساخت dir_context *) priv_ڈیٹا;
	غیرعلامتی صحیح	انحراف = 0;
	غیرعلامتی صحیح	اگلا_real_اندراج = 0;
	صحیح		ret = 0;
	صحیح		بدلا = 0;
	صحیح		do_abort = 0;
	غیرعلامتی صحیح	rec_len, سائز, buflen;
	صحیح		اندراج_2;
	ساخت ext2_dir_اندراج *dirent;
	صحیح		csum_سائز = 0;
	صحیح		inline_ڈیٹا;
	errcode_t	retval = 0;

	اگر (blockcnt < 0)
		واپس 0;

	اندراج_2 = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_ڈیٹا = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_ڈیٹا);
	اگر (!inline_ڈیٹا) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		اگر (ctx->errcode)
			واپس بلاک_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} ورنہ {
		buflen = ctx->buflen;
	}

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائز = حجم(ساخت ext2_dir_اندراج_دم);

	اگر (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		واپس بلاک_ABORT;
	}
	جب (انحراف < buflen - 8) {
		dirent = (ساخت ext2_dir_اندراج *) (ctx->buf + انحراف);
		اگر (ext2fs_get_rec_len(fs, dirent, &rec_len))
			واپس بلاک_ABORT;
		اگر (((انحراف + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحیح) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			واپس بلاک_ABORT;
		}
		اگر (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			اگر (!inline_ڈیٹا &&
			    (انحراف == buflen - csum_سائز) &&
			    (dirent->rec_len == csum_سائز) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				اگر (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					جاؤ اگلا;
				اندراج_2 = DIRENT_CHECKSUM;
			} ورنہ اگر (!(ctx->flags & DIRENT_FLAG_INCLUDE_خالی_خانے))
				جاؤ اگلا;
		}

		ret = (ctx->func)(ctx->dir,
				  (اگلا_real_اندراج > انحراف) ?
				  DIRENT_DELETED_FILE : اندراج_2,
				  dirent, انحراف,
				  buflen, ctx->buf,
				  ctx->priv_ڈیٹا);
		اگر (اندراج_2 < DIRENT_OTHER_FILE)
			اندراج_2++;

		اگر (ret & DIRENT_بدلا) {
			اگر (ext2fs_get_rec_len(fs, dirent, &rec_len))
				واپس بلاک_ABORT;
			بدلا++;
		}
		اگر (ret & DIRENT_ABORT) {
			do_abort++;
			توڑو;
		}
اگلا:
 		اگر (اگلا_real_اندراج == انحراف)
			اگلا_real_اندراج += rec_len;

 		اگر (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			سائز = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			اگر (rec_len != سائز)  {
				غیرعلامتی صحیح final_انحراف;

				final_انحراف = انحراف + rec_len;
				انحراف += سائز;
				جب (انحراف < final_انحراف &&
				       !ext2fs_validate_اندراج(fs, ctx->buf,
							      انحراف,
							      final_انحراف))
					انحراف += 4;
				جاری;
			}
		}
		انحراف += rec_len;
	}

	اگر (بدلا) {
		اگر (!inline_ڈیٹا) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			اگر (ctx->errcode)
				واپس بلاک_ABORT;
		} ورنہ {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = بلاک_INLINE_ڈیٹا_بدلا;
		}
	}
	اگر (do_abort)
		واپس retval | بلاک_ABORT;
	واپس retval;
}
