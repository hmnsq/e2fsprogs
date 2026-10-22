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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#nếu HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     struct ext2_dir_entry *dirent,
			     unsigned số_nguyên *rec_len)
{
	unsigned số_nguyên len = dirent->rec_len;

	nếu (fs->blocksize < 65536)
		*rec_len = len;
	không_thì nếu (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	không_thì 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	trả_về 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned số_nguyên len,
			     struct ext2_dir_entry *dirent)
{
	nếu ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		trả_về EINVAL;
	nếu (len < 65536) {
		dirent->rec_len = len;
		trả_về 0;
	}
	nếu (len == fs->blocksize) {
		nếu (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		không_thì 
			dirent->rec_len = 0;
	} không_thì
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	trả_về 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
static số_nguyên ext2fs_validate_entry(ext2_filsys fs, char *buf,
				 unsigned số_nguyên offset,
				 unsigned số_nguyên final_offset)
{
	struct ext2_dir_entry *dirent;
	unsigned số_nguyên rec_len;
#define DIRENT_MIN_LENGTH 12

	trong_khi ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (struct ext2_dir_entry *)(buf + offset);
		nếu (ext2fs_get_rec_len(fs, dirent, &rec_len))
			trả_về 0;
		offset += rec_len;
		nếu ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (số_nguyên) rec_len))
			trả_về 0;
	}
	trả_về (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      số_nguyên flags,
			      char *block_buf,
			      số_nguyên (*func)(ext2_ino_t	dir,
					  số_nguyên		entry,
					  struct ext2_dir_entry *dirent,
					  số_nguyên	offset,
					  số_nguyên	blocksize,
					  char	*buf,
					  void	*priv_data),
			      void *priv_data)
{
	struct		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	nếu (retval)
		trả_về retval;

	ctx.dir = dir;
	ctx.flags = flags;
	nếu (block_buf)
		ctx.buf = block_buf;
	không_thì {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		nếu (retval)
			trả_về retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	nếu (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	nếu (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(void) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	nếu (retval)
		trả_về retval;
	trả_về ctx.errcode;
}

struct xlate {
	số_nguyên (*func)(struct ext2_dir_entry *dirent,
		    số_nguyên		offset,
		    số_nguyên		blocksize,
		    char	*buf,
		    void	*priv_data);
	void *real_private;
};

static số_nguyên xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      số_nguyên entry EXT2FS_ATTR((unused)),
		      struct ext2_dir_entry *dirent, số_nguyên offset,
		      số_nguyên blocksize, char *buf, void *priv_data)
{
	struct xlate *xl = (struct xlate *) priv_data;

	trả_về (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     số_nguyên flags,
			     char *block_buf,
			     số_nguyên (*func)(struct ext2_dir_entry *dirent,
					 số_nguyên	offset,
					 số_nguyên	blocksize,
					 char	*buf,
					 void	*priv_data),
			     void *priv_data)
{
	struct xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	trả_về ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
số_nguyên ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     số_nguyên	ref_offset EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	struct dir_context *ctx = (struct dir_context *) priv_data;
	unsigned số_nguyên	offset = 0;
	unsigned số_nguyên	next_real_entry = 0;
	số_nguyên		ret = 0;
	số_nguyên		changed = 0;
	số_nguyên		do_abort = 0;
	unsigned số_nguyên	rec_len, size, buflen;
	số_nguyên		entry;
	struct ext2_dir_entry *dirent;
	số_nguyên		csum_size = 0;
	số_nguyên		inline_data;
	errcode_t	retval = 0;

	nếu (blockcnt < 0)
		trả_về 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	nếu (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		nếu (ctx->errcode)
			trả_về BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} không_thì {
		buflen = ctx->buflen;
	}

	nếu (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = sizeof(struct ext2_dir_entry_tail);

	nếu (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		trả_về BLOCK_ABORT;
	}
	trong_khi (offset < buflen - 8) {
		dirent = (struct ext2_dir_entry *) (ctx->buf + offset);
		nếu (ext2fs_get_rec_len(fs, dirent, &rec_len))
			trả_về BLOCK_ABORT;
		nếu (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (số_nguyên) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			trả_về BLOCK_ABORT;
		}
		nếu (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			nếu (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				nếu (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					goto next;
				entry = DIRENT_CHECKSUM;
			} không_thì nếu (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				goto next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		nếu (entry < DIRENT_OTHER_FILE)
			entry++;

		nếu (ret & DIRENT_CHANGED) {
			nếu (ext2fs_get_rec_len(fs, dirent, &rec_len))
				trả_về BLOCK_ABORT;
			changed++;
		}
		nếu (ret & DIRENT_ABORT) {
			do_abort++;
			break;
		}
next:
 		nếu (next_real_entry == offset)
			next_real_entry += rec_len;

 		nếu (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			nếu (rec_len != size)  {
				unsigned số_nguyên final_offset;

				final_offset = offset + rec_len;
				offset += size;
				trong_khi (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				continue;
			}
		}
		offset += rec_len;
	}

	nếu (changed) {
		nếu (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			nếu (ctx->errcode)
				trả_về BLOCK_ABORT;
		} không_thì {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	nếu (do_abort)
		trả_về retval | BLOCK_ABORT;
	trả_về retval;
}
