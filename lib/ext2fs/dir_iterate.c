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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *rec_len)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = dirent->rec_len;

	ਜੇ (fs->blocksize < 65536)
		*rec_len = len;
	ਨਹੀਂਤਾਂ ਜੇ (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	ਨਹੀਂਤਾਂ 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_ਸੈੱਟ_rec_len(ext2_filsys fs,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len,
			     ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent)
{
	ਜੇ ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ਵਾਪਸ EINVAL;
	ਜੇ (len < 65536) {
		dirent->rec_len = len;
		ਵਾਪਸ 0;
	}
	ਜੇ (len == fs->blocksize) {
		ਜੇ (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		ਨਹੀਂਤਾਂ 
			dirent->rec_len = 0;
	} ਨਹੀਂਤਾਂ
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ਵਾਪਸ 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
ਸਥਿਰ ਪੂਰਨ ext2fs_validate_ਇੰਦਰਾਜ(ext2_filsys fs, ਅੱਖਰ *buf,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ final_ਵਿੱਥ)
{
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rec_len;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRENT_MIN_ਲੰਮਾਈ 12

	ਜਦੋਂ ((ਵਿੱਥ < final_ਵਿੱਥ) &&
	       (ਵਿੱਥ <= fs->blocksize - DIRENT_MIN_ਲੰਮਾਈ)) {
		dirent = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)(buf + ਵਿੱਥ);
		ਜੇ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ਵਾਪਸ 0;
		ਵਿੱਥ += rec_len;
		ਜੇ ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ਪੂਰਨ) rec_len))
			ਵਾਪਸ 0;
	}
	ਵਾਪਸ (ਵਿੱਥ == final_ਵਿੱਥ);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ਪੂਰਨ flags,
			      ਅੱਖਰ *ਖੰਡ_buf,
			      ਪੂਰਨ (*func)(ext2_ino_t	dir,
					  ਪੂਰਨ		ਇੰਦਰਾਜ,
					  ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
					  ਪੂਰਨ	ਵਿੱਥ,
					  ਪੂਰਨ	blocksize,
					  ਅੱਖਰ	*buf,
					  ਖਾਲੀ	*priv_ਅੰਕੜੇ),
			      ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ਜੇ (ਖੰਡ_buf)
		ctx.buf = ਖੰਡ_buf;
	ਨਹੀਂਤਾਂ {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ctx.func = func;
	ctx.priv_ਅੰਕੜੇ = priv_ਅੰਕੜੇ;
	ctx.errcode = 0;
	retval = ext2fs_ਖੰਡ_iterate3(fs, dir, ਖੰਡ_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_ਖੰਡ, &ctx);
	ਜੇ (!ਖੰਡ_buf)
		ext2fs_free_mem(&ctx.buf);
	ਜੇ (retval == EXT2_ET_INLINE_ਅੰਕੜੇ_CANT_ITERATE) {
		(ਖਾਲੀ) ext2fs_inline_ਅੰਕੜੇ_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਵਾਪਸ ctx.errcode;
}

ਬਣਤਰ xlate {
	ਪੂਰਨ (*func)(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
		    ਪੂਰਨ		ਵਿੱਥ,
		    ਪੂਰਨ		blocksize,
		    ਅੱਖਰ	*buf,
		    ਖਾਲੀ	*priv_ਅੰਕੜੇ);
	ਖਾਲੀ *real_ਨਿੱਜੀ;
};

ਸਥਿਰ ਪੂਰਨ xlate_func(ext2_ino_t dir EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		      ਪੂਰਨ ਇੰਦਰਾਜ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		      ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent, ਪੂਰਨ ਵਿੱਥ,
		      ਪੂਰਨ blocksize, ਅੱਖਰ *buf, ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ xlate *xl = (ਬਣਤਰ xlate *) priv_ਅੰਕੜੇ;

	ਵਾਪਸ (*xl->func)(dirent, ਵਿੱਥ, blocksize, buf, xl->real_ਨਿੱਜੀ);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     ਪੂਰਨ flags,
			     ਅੱਖਰ *ਖੰਡ_buf,
			     ਪੂਰਨ (*func)(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
					 ਪੂਰਨ	ਵਿੱਥ,
					 ਪੂਰਨ	blocksize,
					 ਅੱਖਰ	*buf,
					 ਖਾਲੀ	*priv_ਅੰਕੜੇ),
			     ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ xlate xl;

	xl.real_ਨਿੱਜੀ = priv_ਅੰਕੜੇ;
	xl.func = func;

	ਵਾਪਸ ext2fs_dir_iterate2(fs, dir, flags, ਖੰਡ_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
ਪੂਰਨ ext2fs_process_dir_ਖੰਡ(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_ਖੰਡ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     ਪੂਰਨ	ref_ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     ਖਾਲੀ	*priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ dir_context *ctx = (ਬਣਤਰ dir_context *) priv_ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਵਿੱਥ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਅਗਲਾ_real_ਇੰਦਰਾਜ = 0;
	ਪੂਰਨ		ret = 0;
	ਪੂਰਨ		ਬਦਲਿਆ = 0;
	ਪੂਰਨ		do_abort = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	rec_len, ਮਾਪ, buflen;
	ਪੂਰਨ		ਇੰਦਰਾਜ;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent;
	ਪੂਰਨ		csum_ਮਾਪ = 0;
	ਪੂਰਨ		inline_ਅੰਕੜੇ;
	errcode_t	retval = 0;

	ਜੇ (blockcnt < 0)
		ਵਾਪਸ 0;

	ਇੰਦਰਾਜ = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_ਅੰਕੜੇ = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_ਅੰਕੜੇ);
	ਜੇ (!inline_ਅੰਕੜੇ) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ਜੇ (ctx->errcode)
			ਵਾਪਸ ਖੰਡ_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} ਨਹੀਂਤਾਂ {
		buflen = ctx->buflen;
	}

	ਜੇ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ);

	ਜੇ (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ਵਾਪਸ ਖੰਡ_ABORT;
	}
	ਜਦੋਂ (ਵਿੱਥ < buflen - 8) {
		dirent = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) (ctx->buf + ਵਿੱਥ);
		ਜੇ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ਵਾਪਸ ਖੰਡ_ABORT;
		ਜੇ (((ਵਿੱਥ + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ਪੂਰਨ) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ਵਾਪਸ ਖੰਡ_ABORT;
		}
		ਜੇ (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ਜੇ (!inline_ਅੰਕੜੇ &&
			    (ਵਿੱਥ == buflen - csum_ਮਾਪ) &&
			    (dirent->rec_len == csum_ਮਾਪ) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ਜੇ (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					ਜਾਓ ਅਗਲਾ;
				ਇੰਦਰਾਜ = DIRENT_CHECKSUM;
			} ਨਹੀਂਤਾਂ ਜੇ (!(ctx->flags & DIRENT_FLAG_INCLUDE_ਖਾਲੀ_ਥਾਵਾਂ))
				ਜਾਓ ਅਗਲਾ;
		}

		ret = (ctx->func)(ctx->dir,
				  (ਅਗਲਾ_real_ਇੰਦਰਾਜ > ਵਿੱਥ) ?
				  DIRENT_DELETED_FILE : ਇੰਦਰਾਜ,
				  dirent, ਵਿੱਥ,
				  buflen, ctx->buf,
				  ctx->priv_ਅੰਕੜੇ);
		ਜੇ (ਇੰਦਰਾਜ < DIRENT_OTHER_FILE)
			ਇੰਦਰਾਜ++;

		ਜੇ (ret & DIRENT_ਬਦਲਿਆ) {
			ਜੇ (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ਵਾਪਸ ਖੰਡ_ABORT;
			ਬਦਲਿਆ++;
		}
		ਜੇ (ret & DIRENT_ABORT) {
			do_abort++;
			ਰੋਕੋ;
		}
ਅਗਲਾ:
 		ਜੇ (ਅਗਲਾ_real_ਇੰਦਰਾਜ == ਵਿੱਥ)
			ਅਗਲਾ_real_ਇੰਦਰਾਜ += rec_len;

 		ਜੇ (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			ਮਾਪ = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ਜੇ (rec_len != ਮਾਪ)  {
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ final_ਵਿੱਥ;

				final_ਵਿੱਥ = ਵਿੱਥ + rec_len;
				ਵਿੱਥ += ਮਾਪ;
				ਜਦੋਂ (ਵਿੱਥ < final_ਵਿੱਥ &&
				       !ext2fs_validate_ਇੰਦਰਾਜ(fs, ctx->buf,
							      ਵਿੱਥ,
							      final_ਵਿੱਥ))
					ਵਿੱਥ += 4;
				ਜਾਰੀ;
			}
		}
		ਵਿੱਥ += rec_len;
	}

	ਜੇ (ਬਦਲਿਆ) {
		ਜੇ (!inline_ਅੰਕੜੇ) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ਜੇ (ctx->errcode)
				ਵਾਪਸ ਖੰਡ_ABORT;
		} ਨਹੀਂਤਾਂ {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = ਖੰਡ_INLINE_ਅੰਕੜੇ_ਬਦਲਿਆ;
		}
	}
	ਜੇ (do_abort)
		ਵਾਪਸ retval | ਖੰਡ_ABORT;
	ਵਾਪਸ retval;
}
