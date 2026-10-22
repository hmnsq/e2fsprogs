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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     ಸಂರಚನಾ ext2_dir_entry *dirent,
			     unsigned ಪೂರ್ಣಾಙ್ಕ *rec_len)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ len = dirent->rec_len;

	ಯದಿ (fs->blocksize < 65536)
		*rec_len = len;
	ಅನ್ಯಥಾ ಯದಿ (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	ಅನ್ಯಥಾ 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned ಪೂರ್ಣಾಙ್ಕ len,
			     ಸಂರಚನಾ ext2_dir_entry *dirent)
{
	ಯದಿ ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ಪ್ರತ್ಯಾಗಮ EINVAL;
	ಯದಿ (len < 65536) {
		dirent->rec_len = len;
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಯದಿ (len == fs->blocksize) {
		ಯದಿ (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		ಅನ್ಯಥಾ 
			dirent->rec_len = 0;
	} ಅನ್ಯಥಾ
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ext2fs_validate_entry(ext2_filsys fs, ವರ್ಣ *buf,
				 unsigned ಪೂರ್ಣಾಙ್ಕ offset,
				 unsigned ಪೂರ್ಣಾಙ್ಕ final_offset)
{
	ಸಂರಚನಾ ext2_dir_entry *dirent;
	unsigned ಪೂರ್ಣಾಙ್ಕ rec_len;
#define DIRENT_MIN_LENGTH 12

	ಯಾವತ್ ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (ಸಂರಚನಾ ext2_dir_entry *)(buf + offset);
		ಯದಿ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ಪ್ರತ್ಯಾಗಮ 0;
		offset += rec_len;
		ಯದಿ ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ಪೂರ್ಣಾಙ್ಕ) rec_len))
			ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ಪೂರ್ಣಾಙ್ಕ flags,
			      ವರ್ಣ *block_buf,
			      ಪೂರ್ಣಾಙ್ಕ (*func)(ext2_ino_t	dir,
					  ಪೂರ್ಣಾಙ್ಕ		entry,
					  ಸಂರಚನಾ ext2_dir_entry *dirent,
					  ಪೂರ್ಣಾಙ್ಕ	offset,
					  ಪೂರ್ಣಾಙ್ಕ	blocksize,
					  ವರ್ಣ	*buf,
					  ಶೂನ್ಯ	*priv_data),
			      ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ಯದಿ (block_buf)
		ctx.buf = block_buf;
	ಅನ್ಯಥಾ {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	ಯದಿ (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	ಯದಿ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(ಶೂನ್ಯ) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಪ್ರತ್ಯಾಗಮ ctx.errcode;
}

ಸಂರಚನಾ xlate {
	ಪೂರ್ಣಾಙ್ಕ (*func)(ಸಂರಚನಾ ext2_dir_entry *dirent,
		    ಪೂರ್ಣಾಙ್ಕ		offset,
		    ಪೂರ್ಣಾಙ್ಕ		blocksize,
		    ವರ್ಣ	*buf,
		    ಶೂನ್ಯ	*priv_data);
	ಶೂನ್ಯ *real_private;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      ಪೂರ್ಣಾಙ್ಕ entry EXT2FS_ATTR((unused)),
		      ಸಂರಚನಾ ext2_dir_entry *dirent, ಪೂರ್ಣಾಙ್ಕ offset,
		      ಪೂರ್ಣಾಙ್ಕ blocksize, ವರ್ಣ *buf, ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ xlate *xl = (ಸಂರಚನಾ xlate *) priv_data;

	ಪ್ರತ್ಯಾಗಮ (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     ಪೂರ್ಣಾಙ್ಕ flags,
			     ವರ್ಣ *block_buf,
			     ಪೂರ್ಣಾಙ್ಕ (*func)(ಸಂರಚನಾ ext2_dir_entry *dirent,
					 ಪೂರ್ಣಾಙ್ಕ	offset,
					 ಪೂರ್ಣಾಙ್ಕ	blocksize,
					 ವರ್ಣ	*buf,
					 ಶೂನ್ಯ	*priv_data),
			     ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ಪ್ರತ್ಯಾಗಮ ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
ಪೂರ್ಣಾಙ್ಕ ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     ಪೂರ್ಣಾಙ್ಕ	ref_offset EXT2FS_ATTR((unused)),
			     ಶೂನ್ಯ	*priv_data)
{
	ಸಂರಚನಾ dir_context *ctx = (ಸಂರಚನಾ dir_context *) priv_data;
	unsigned ಪೂರ್ಣಾಙ್ಕ	offset = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ	next_real_entry = 0;
	ಪೂರ್ಣಾಙ್ಕ		ret = 0;
	ಪೂರ್ಣಾಙ್ಕ		changed = 0;
	ಪೂರ್ಣಾಙ್ಕ		do_abort = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ	rec_len, size, buflen;
	ಪೂರ್ಣಾಙ್ಕ		entry;
	ಸಂರಚನಾ ext2_dir_entry *dirent;
	ಪೂರ್ಣಾಙ್ಕ		csum_size = 0;
	ಪೂರ್ಣಾಙ್ಕ		inline_data;
	errcode_t	retval = 0;

	ಯದಿ (blockcnt < 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	ಯದಿ (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ಯದಿ (ctx->errcode)
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} ಅನ್ಯಥಾ {
		buflen = ctx->buflen;
	}

	ಯದಿ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dir_entry_tail);

	ಯದಿ (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
	}
	ಯಾವತ್ (offset < buflen - 8) {
		dirent = (ಸಂರಚನಾ ext2_dir_entry *) (ctx->buf + offset);
		ಯದಿ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		ಯದಿ (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ಪೂರ್ಣಾಙ್ಕ) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		}
		ಯದಿ (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ಯದಿ (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ಯದಿ (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					ಗಚ್ಛ next;
				entry = DIRENT_CHECKSUM;
			} ಅನ್ಯಥಾ ಯದಿ (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				ಗಚ್ಛ next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		ಯದಿ (entry < DIRENT_OTHER_FILE)
			entry++;

		ಯದಿ (ret & DIRENT_CHANGED) {
			ಯದಿ (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
			changed++;
		}
		ಯದಿ (ret & DIRENT_ABORT) {
			do_abort++;
			ಭಙ್ಗ;
		}
next:
 		ಯದಿ (next_real_entry == offset)
			next_real_entry += rec_len;

 		ಯದಿ (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ಯದಿ (rec_len != size)  {
				unsigned ಪೂರ್ಣಾಙ್ಕ final_offset;

				final_offset = offset + rec_len;
				offset += size;
				ಯಾವತ್ (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				ಅನುವರ್ತ;
			}
		}
		offset += rec_len;
	}

	ಯದಿ (changed) {
		ಯದಿ (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ಯದಿ (ctx->errcode)
				ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		} ಅನ್ಯಥಾ {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	ಯದಿ (do_abort)
		ಪ್ರತ್ಯಾಗಮ retval | BLOCK_ABORT;
	ಪ್ರತ್ಯಾಗಮ retval;
}
