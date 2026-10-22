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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ถ้า HAVE_UNISTD_H
#include <unistd.h>
#endif
#ถ้า HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     sa_nracanaa ext2_dir_entry *dirent,
			     unsigned จำนวนเต็ม *rec_len)
{
	unsigned จำนวนเต็ม len = dirent->rec_len;

	ถ้า (fs->blocksize < 65536)
		*rec_len = len;
	มิฉะนั้น ถ้า (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	มิฉะนั้น 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	คืน 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned จำนวนเต็ม len,
			     sa_nracanaa ext2_dir_entry *dirent)
{
	ถ้า ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		คืน EINVAL;
	ถ้า (len < 65536) {
		dirent->rec_len = len;
		คืน 0;
	}
	ถ้า (len == fs->blocksize) {
		ถ้า (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		มิฉะนั้น 
			dirent->rec_len = 0;
	} มิฉะนั้น
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	คืน 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
static จำนวนเต็ม ext2fs_validate_entry(ext2_filsys fs, var_na *buf,
				 unsigned จำนวนเต็ม offset,
				 unsigned จำนวนเต็ม final_offset)
{
	sa_nracanaa ext2_dir_entry *dirent;
	unsigned จำนวนเต็ม rec_len;
#define DIRENT_MIN_LENGTH 12

	ขณะ ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (sa_nracanaa ext2_dir_entry *)(buf + offset);
		ถ้า (ext2fs_get_rec_len(fs, dirent, &rec_len))
			คืน 0;
		offset += rec_len;
		ถ้า ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (จำนวนเต็ม) rec_len))
			คืน 0;
	}
	คืน (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      จำนวนเต็ม flags,
			      var_na *block_buf,
			      จำนวนเต็ม (*func)(ext2_ino_t	dir,
					  จำนวนเต็ม		entry,
					  sa_nracanaa ext2_dir_entry *dirent,
					  จำนวนเต็ม	offset,
					  จำนวนเต็ม	blocksize,
					  var_na	*buf,
					  void	*priv_data),
			      void *priv_data)
{
	sa_nracanaa		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ถ้า (retval)
		คืน retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ถ้า (block_buf)
		ctx.buf = block_buf;
	มิฉะนั้น {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ถ้า (retval)
			คืน retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	ถ้า (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	ถ้า (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(void) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ถ้า (retval)
		คืน retval;
	คืน ctx.errcode;
}

sa_nracanaa xlate {
	จำนวนเต็ม (*func)(sa_nracanaa ext2_dir_entry *dirent,
		    จำนวนเต็ม		offset,
		    จำนวนเต็ม		blocksize,
		    var_na	*buf,
		    void	*priv_data);
	void *real_private;
};

static จำนวนเต็ม xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      จำนวนเต็ม entry EXT2FS_ATTR((unused)),
		      sa_nracanaa ext2_dir_entry *dirent, จำนวนเต็ม offset,
		      จำนวนเต็ม blocksize, var_na *buf, void *priv_data)
{
	sa_nracanaa xlate *xl = (sa_nracanaa xlate *) priv_data;

	คืน (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     จำนวนเต็ม flags,
			     var_na *block_buf,
			     จำนวนเต็ม (*func)(sa_nracanaa ext2_dir_entry *dirent,
					 จำนวนเต็ม	offset,
					 จำนวนเต็ม	blocksize,
					 var_na	*buf,
					 void	*priv_data),
			     void *priv_data)
{
	sa_nracanaa xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	คืน ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
จำนวนเต็ม ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     จำนวนเต็ม	ref_offset EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	sa_nracanaa dir_context *ctx = (sa_nracanaa dir_context *) priv_data;
	unsigned จำนวนเต็ม	offset = 0;
	unsigned จำนวนเต็ม	next_real_entry = 0;
	จำนวนเต็ม		ret = 0;
	จำนวนเต็ม		changed = 0;
	จำนวนเต็ม		do_abort = 0;
	unsigned จำนวนเต็ม	rec_len, size, buflen;
	จำนวนเต็ม		entry;
	sa_nracanaa ext2_dir_entry *dirent;
	จำนวนเต็ม		csum_size = 0;
	จำนวนเต็ม		inline_data;
	errcode_t	retval = 0;

	ถ้า (blockcnt < 0)
		คืน 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	ถ้า (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ถ้า (ctx->errcode)
			คืน BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} มิฉะนั้น {
		buflen = ctx->buflen;
	}

	ถ้า (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = parimaa_na(sa_nracanaa ext2_dir_entry_tail);

	ถ้า (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		คืน BLOCK_ABORT;
	}
	ขณะ (offset < buflen - 8) {
		dirent = (sa_nracanaa ext2_dir_entry *) (ctx->buf + offset);
		ถ้า (ext2fs_get_rec_len(fs, dirent, &rec_len))
			คืน BLOCK_ABORT;
		ถ้า (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (จำนวนเต็ม) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			คืน BLOCK_ABORT;
		}
		ถ้า (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ถ้า (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ถ้า (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					goto next;
				entry = DIRENT_CHECKSUM;
			} มิฉะนั้น ถ้า (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				goto next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		ถ้า (entry < DIRENT_OTHER_FILE)
			entry++;

		ถ้า (ret & DIRENT_CHANGED) {
			ถ้า (ext2fs_get_rec_len(fs, dirent, &rec_len))
				คืน BLOCK_ABORT;
			changed++;
		}
		ถ้า (ret & DIRENT_ABORT) {
			do_abort++;
			bha_nga;
		}
next:
 		ถ้า (next_real_entry == offset)
			next_real_entry += rec_len;

 		ถ้า (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ถ้า (rec_len != size)  {
				unsigned จำนวนเต็ม final_offset;

				final_offset = offset + rec_len;
				offset += size;
				ขณะ (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				continue;
			}
		}
		offset += rec_len;
	}

	ถ้า (changed) {
		ถ้า (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ถ้า (ctx->errcode)
				คืน BLOCK_ABORT;
		} มิฉะนั้น {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	ถ้า (do_abort)
		คืน retval | BLOCK_ABORT;
	คืน retval;
}
