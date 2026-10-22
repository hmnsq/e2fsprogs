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
#ако HAVE_UNISTD_H
#include <unistd.h>
#endif
#ако HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     struct ext2_dir_entry *dirent,
			     unsigned цео *rec_len)
{
	unsigned цео len = dirent->rec_len;

	ако (fs->blocksize < 65536)
		*rec_len = len;
	инако ако (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	инако 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	врати 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned цео len,
			     struct ext2_dir_entry *dirent)
{
	ако ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		врати EINVAL;
	ако (len < 65536) {
		dirent->rec_len = len;
		врати 0;
	}
	ако (len == fs->blocksize) {
		ако (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		инако 
			dirent->rec_len = 0;
	} инако
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	врати 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
static цео ext2fs_validate_entry(ext2_filsys fs, char *buf,
				 unsigned цео offset,
				 unsigned цео final_offset)
{
	struct ext2_dir_entry *dirent;
	unsigned цео rec_len;
#define DIRENT_MIN_LENGTH 12

	док ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (struct ext2_dir_entry *)(buf + offset);
		ако (ext2fs_get_rec_len(fs, dirent, &rec_len))
			врати 0;
		offset += rec_len;
		ако ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (цео) rec_len))
			врати 0;
	}
	врати (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      цео flags,
			      char *block_buf,
			      цео (*func)(ext2_ino_t	dir,
					  цео		entry,
					  struct ext2_dir_entry *dirent,
					  цео	offset,
					  цео	blocksize,
					  char	*buf,
					  void	*priv_data),
			      void *priv_data)
{
	struct		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ако (retval)
		врати retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ако (block_buf)
		ctx.buf = block_buf;
	инако {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ако (retval)
			врати retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	ако (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	ако (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(void) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ако (retval)
		врати retval;
	врати ctx.errcode;
}

struct xlate {
	цео (*func)(struct ext2_dir_entry *dirent,
		    цео		offset,
		    цео		blocksize,
		    char	*buf,
		    void	*priv_data);
	void *real_private;
};

static цео xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      цео entry EXT2FS_ATTR((unused)),
		      struct ext2_dir_entry *dirent, цео offset,
		      цео blocksize, char *buf, void *priv_data)
{
	struct xlate *xl = (struct xlate *) priv_data;

	врати (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     цео flags,
			     char *block_buf,
			     цео (*func)(struct ext2_dir_entry *dirent,
					 цео	offset,
					 цео	blocksize,
					 char	*buf,
					 void	*priv_data),
			     void *priv_data)
{
	struct xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	врати ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
цео ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     цео	ref_offset EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	struct dir_context *ctx = (struct dir_context *) priv_data;
	unsigned цео	offset = 0;
	unsigned цео	next_real_entry = 0;
	цео		ret = 0;
	цео		changed = 0;
	цео		do_abort = 0;
	unsigned цео	rec_len, size, buflen;
	цео		entry;
	struct ext2_dir_entry *dirent;
	цео		csum_size = 0;
	цео		inline_data;
	errcode_t	retval = 0;

	ако (blockcnt < 0)
		врати 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	ако (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ако (ctx->errcode)
			врати BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} инако {
		buflen = ctx->buflen;
	}

	ако (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = sizeof(struct ext2_dir_entry_tail);

	ако (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		врати BLOCK_ABORT;
	}
	док (offset < buflen - 8) {
		dirent = (struct ext2_dir_entry *) (ctx->buf + offset);
		ако (ext2fs_get_rec_len(fs, dirent, &rec_len))
			врати BLOCK_ABORT;
		ако (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (цео) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			врати BLOCK_ABORT;
		}
		ако (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ако (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ако (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					goto next;
				entry = DIRENT_CHECKSUM;
			} инако ако (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				goto next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		ако (entry < DIRENT_OTHER_FILE)
			entry++;

		ако (ret & DIRENT_CHANGED) {
			ако (ext2fs_get_rec_len(fs, dirent, &rec_len))
				врати BLOCK_ABORT;
			changed++;
		}
		ако (ret & DIRENT_ABORT) {
			do_abort++;
			break;
		}
next:
 		ако (next_real_entry == offset)
			next_real_entry += rec_len;

 		ако (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ако (rec_len != size)  {
				unsigned цео final_offset;

				final_offset = offset + rec_len;
				offset += size;
				док (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				continue;
			}
		}
		offset += rec_len;
	}

	ако (changed) {
		ако (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ако (ctx->errcode)
				врати BLOCK_ABORT;
		} инако {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	ако (do_abort)
		врати retval | BLOCK_ABORT;
	врати retval;
}
