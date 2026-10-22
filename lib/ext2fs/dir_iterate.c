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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     ರಚನೆ ext2_dir_ನಮೂದು *dirent,
			     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *rec_len)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len = dirent->rec_len;

	ಆದರೆ (fs->blocksize < 65536)
		*rec_len = len;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	ಇಲ್ಲದಿದ್ದರೆ 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_ಗಣ_rec_len(ext2_filsys fs,
			     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len,
			     ರಚನೆ ext2_dir_ನಮೂದು *dirent)
{
	ಆದರೆ ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ಹಿಂತಿರುಗಿ EINVAL;
	ಆದರೆ (len < 65536) {
		dirent->rec_len = len;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (len == fs->blocksize) {
		ಆದರೆ (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		ಇಲ್ಲದಿದ್ದರೆ 
			dirent->rec_len = 0;
	} ಇಲ್ಲದಿದ್ದರೆ
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ext2fs_validate_ನಮೂದು(ext2_filsys fs, ಅಕ್ಷರ *buf,
				 ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ,
				 ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ final_ಸ್ಥಾನಾಂತರ)
{
	ರಚನೆ ext2_dir_ನಮೂದು *dirent;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rec_len;
#ವ್ಯಾಖ್ಯಾನಿಸು DIRENT_MIN_ಉದ್ದಳತೆ 12

	ಆಗ ((ಸ್ಥಾನಾಂತರ < final_ಸ್ಥಾನಾಂತರ) &&
	       (ಸ್ಥಾನಾಂತರ <= fs->blocksize - DIRENT_MIN_ಉದ್ದಳತೆ)) {
		dirent = (ರಚನೆ ext2_dir_ನಮೂದು *)(buf + ಸ್ಥಾನಾಂತರ);
		ಆದರೆ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ಹಿಂತಿರುಗಿ 0;
		ಸ್ಥಾನಾಂತರ += rec_len;
		ಆದರೆ ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ಪೂರ್ಣಾಂಕ) rec_len))
			ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ (ಸ್ಥಾನಾಂತರ == final_ಸ್ಥಾನಾಂತರ);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ಪೂರ್ಣಾಂಕ flags,
			      ಅಕ್ಷರ *ಖಂಡ_buf,
			      ಪೂರ್ಣಾಂಕ (*func)(ext2_ino_t	dir,
					  ಪೂರ್ಣಾಂಕ		ನಮೂದು,
					  ರಚನೆ ext2_dir_ನಮೂದು *dirent,
					  ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ,
					  ಪೂರ್ಣಾಂಕ	blocksize,
					  ಅಕ್ಷರ	*buf,
					  ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
			      ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ಆದರೆ (ಖಂಡ_buf)
		ctx.buf = ಖಂಡ_buf;
	ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ctx.func = func;
	ctx.priv_ದತ್ತಾಂಶ = priv_ದತ್ತಾಂಶ;
	ctx.errcode = 0;
	retval = ext2fs_ಖಂಡ_iterate3(fs, dir, ಖಂಡ_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_ಖಂಡ, &ctx);
	ಆದರೆ (!ಖಂಡ_buf)
		ext2fs_free_mem(&ctx.buf);
	ಆದರೆ (retval == EXT2_ET_INLINE_ದತ್ತಾಂಶ_CANT_ITERATE) {
		(ಶೂನ್ಯ) ext2fs_inline_ದತ್ತಾಂಶ_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಹಿಂತಿರುಗಿ ctx.errcode;
}

ರಚನೆ xlate {
	ಪೂರ್ಣಾಂಕ (*func)(ರಚನೆ ext2_dir_ನಮೂದು *dirent,
		    ಪೂರ್ಣಾಂಕ		ಸ್ಥಾನಾಂತರ,
		    ಪೂರ್ಣಾಂಕ		blocksize,
		    ಅಕ್ಷರ	*buf,
		    ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ);
	ಶೂನ್ಯ *real_ಖಾಸಗಿ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ xlate_func(ext2_ino_t dir EXT2FS_ATTR((ಬಳಸದ)),
		      ಪೂರ್ಣಾಂಕ ನಮೂದು EXT2FS_ATTR((ಬಳಸದ)),
		      ರಚನೆ ext2_dir_ನಮೂದು *dirent, ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ,
		      ಪೂರ್ಣಾಂಕ blocksize, ಅಕ್ಷರ *buf, ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ xlate *xl = (ರಚನೆ xlate *) priv_ದತ್ತಾಂಶ;

	ಹಿಂತಿರುಗಿ (*xl->func)(dirent, ಸ್ಥಾನಾಂತರ, blocksize, buf, xl->real_ಖಾಸಗಿ);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     ಪೂರ್ಣಾಂಕ flags,
			     ಅಕ್ಷರ *ಖಂಡ_buf,
			     ಪೂರ್ಣಾಂಕ (*func)(ರಚನೆ ext2_dir_ನಮೂದು *dirent,
					 ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ,
					 ಪೂರ್ಣಾಂಕ	blocksize,
					 ಅಕ್ಷರ	*buf,
					 ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ),
			     ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ xlate xl;

	xl.real_ಖಾಸಗಿ = priv_ದತ್ತಾಂಶ;
	xl.func = func;

	ಹಿಂತಿರುಗಿ ext2fs_dir_iterate2(fs, dir, flags, ಖಂಡ_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
ಪೂರ್ಣಾಂಕ ext2fs_process_dir_ಖಂಡ(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_ಖಂಡ EXT2FS_ATTR((ಬಳಸದ)),
			     ಪೂರ್ಣಾಂಕ	ref_ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
			     ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ dir_context *ctx = (ರಚನೆ dir_context *) priv_ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ಮುಂದಿನ_real_ನಮೂದು = 0;
	ಪೂರ್ಣಾಂಕ		ret = 0;
	ಪೂರ್ಣಾಂಕ		ಬದಲಾಯಿತು = 0;
	ಪೂರ್ಣಾಂಕ		do_abort = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	rec_len, ಗಾತ್ರ_2, buflen;
	ಪೂರ್ಣಾಂಕ		ನಮೂದು;
	ರಚನೆ ext2_dir_ನಮೂದು *dirent;
	ಪೂರ್ಣಾಂಕ		csum_ಗಾತ್ರ = 0;
	ಪೂರ್ಣಾಂಕ		inline_ದತ್ತಾಂಶ;
	errcode_t	retval = 0;

	ಆದರೆ (blockcnt < 0)
		ಹಿಂತಿರುಗಿ 0;

	ನಮೂದು = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_ದತ್ತಾಂಶ = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_ದತ್ತಾಂಶ);
	ಆದರೆ (!inline_ದತ್ತಾಂಶ) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ಆದರೆ (ctx->errcode)
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} ಇಲ್ಲದಿದ್ದರೆ {
		buflen = ctx->buflen;
	}

	ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ);

	ಆದರೆ (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
	}
	ಆಗ (ಸ್ಥಾನಾಂತರ < buflen - 8) {
		dirent = (ರಚನೆ ext2_dir_ನಮೂದು *) (ctx->buf + ಸ್ಥಾನಾಂತರ);
		ಆದರೆ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		ಆದರೆ (((ಸ್ಥಾನಾಂತರ + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ಪೂರ್ಣಾಂಕ) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		}
		ಆದರೆ (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ಆದರೆ (!inline_ದತ್ತಾಂಶ &&
			    (ಸ್ಥಾನಾಂತರ == buflen - csum_ಗಾತ್ರ) &&
			    (dirent->rec_len == csum_ಗಾತ್ರ) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ಆದರೆ (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					ಹೋಗು ಮುಂದಿನ;
				ನಮೂದು = DIRENT_CHECKSUM;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(ctx->flags & DIRENT_FLAG_INCLUDE_ಖಾಲಿಗಳು))
				ಹೋಗು ಮುಂದಿನ;
		}

		ret = (ctx->func)(ctx->dir,
				  (ಮುಂದಿನ_real_ನಮೂದು > ಸ್ಥಾನಾಂತರ) ?
				  DIRENT_DELETED_FILE : ನಮೂದು,
				  dirent, ಸ್ಥಾನಾಂತರ,
				  buflen, ctx->buf,
				  ctx->priv_ದತ್ತಾಂಶ);
		ಆದರೆ (ನಮೂದು < DIRENT_OTHER_FILE)
			ನಮೂದು++;

		ಆದರೆ (ret & DIRENT_ಬದಲಾಯಿತು) {
			ಆದರೆ (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
			ಬದಲಾಯಿತು++;
		}
		ಆದರೆ (ret & DIRENT_ABORT) {
			do_abort++;
			ನಿಲ್ಲಿಸು;
		}
ಮುಂದಿನ:
 		ಆದರೆ (ಮುಂದಿನ_real_ನಮೂದು == ಸ್ಥಾನಾಂತರ)
			ಮುಂದಿನ_real_ನಮೂದು += rec_len;

 		ಆದರೆ (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			ಗಾತ್ರ_2 = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ಆದರೆ (rec_len != ಗಾತ್ರ_2)  {
				ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ final_ಸ್ಥಾನಾಂತರ;

				final_ಸ್ಥಾನಾಂತರ = ಸ್ಥಾನಾಂತರ + rec_len;
				ಸ್ಥಾನಾಂತರ += ಗಾತ್ರ_2;
				ಆಗ (ಸ್ಥಾನಾಂತರ < final_ಸ್ಥಾನಾಂತರ &&
				       !ext2fs_validate_ನಮೂದು(fs, ctx->buf,
							      ಸ್ಥಾನಾಂತರ,
							      final_ಸ್ಥಾನಾಂತರ))
					ಸ್ಥಾನಾಂತರ += 4;
				ಮುಂದುವರಿ;
			}
		}
		ಸ್ಥಾನಾಂತರ += rec_len;
	}

	ಆದರೆ (ಬದಲಾಯಿತು) {
		ಆದರೆ (!inline_ದತ್ತಾಂಶ) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ಆದರೆ (ctx->errcode)
				ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = ಖಂಡ_INLINE_ದತ್ತಾಂಶ_ಬದಲಾಯಿತು;
		}
	}
	ಆದರೆ (do_abort)
		ಹಿಂತಿರುಗಿ retval | ಖಂಡ_ABORT;
	ಹಿಂತಿರುಗಿ retval;
}
