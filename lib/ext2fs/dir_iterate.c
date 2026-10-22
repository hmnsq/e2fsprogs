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
#もし HAVE_UNISTD_H
#include <unistd.h>
#endif
#もし HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     struct ext2_dir_entry *dirent,
			     unsigned 整数 *rec_len)
{
	unsigned 整数 len = dirent->rec_len;

	もし (fs->blocksize < 65536)
		*rec_len = len;
	否则 もし (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	否则 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	戻る 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned 整数 len,
			     struct ext2_dir_entry *dirent)
{
	もし ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		戻る EINVAL;
	もし (len < 65536) {
		dirent->rec_len = len;
		戻る 0;
	}
	もし (len == fs->blocksize) {
		もし (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		否则 
			dirent->rec_len = 0;
	} 否则
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	戻る 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
static 整数 ext2fs_validate_entry(ext2_filsys fs, char *buf,
				 unsigned 整数 offset,
				 unsigned 整数 final_offset)
{
	struct ext2_dir_entry *dirent;
	unsigned 整数 rec_len;
#define DIRENT_MIN_LENGTH 12

	ながら ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (struct ext2_dir_entry *)(buf + offset);
		もし (ext2fs_get_rec_len(fs, dirent, &rec_len))
			戻る 0;
		offset += rec_len;
		もし ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (整数) rec_len))
			戻る 0;
	}
	戻る (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      整数 flags,
			      char *block_buf,
			      整数 (*func)(ext2_ino_t	dir,
					  整数		entry,
					  struct ext2_dir_entry *dirent,
					  整数	offset,
					  整数	blocksize,
					  char	*buf,
					  void	*priv_data),
			      void *priv_data)
{
	struct		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	もし (retval)
		戻る retval;

	ctx.dir = dir;
	ctx.flags = flags;
	もし (block_buf)
		ctx.buf = block_buf;
	否则 {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		もし (retval)
			戻る retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	もし (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	もし (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(void) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	もし (retval)
		戻る retval;
	戻る ctx.errcode;
}

struct xlate {
	整数 (*func)(struct ext2_dir_entry *dirent,
		    整数		offset,
		    整数		blocksize,
		    char	*buf,
		    void	*priv_data);
	void *real_private;
};

static 整数 xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      整数 entry EXT2FS_ATTR((unused)),
		      struct ext2_dir_entry *dirent, 整数 offset,
		      整数 blocksize, char *buf, void *priv_data)
{
	struct xlate *xl = (struct xlate *) priv_data;

	戻る (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     整数 flags,
			     char *block_buf,
			     整数 (*func)(struct ext2_dir_entry *dirent,
					 整数	offset,
					 整数	blocksize,
					 char	*buf,
					 void	*priv_data),
			     void *priv_data)
{
	struct xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	戻る ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
整数 ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     整数	ref_offset EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	struct dir_context *ctx = (struct dir_context *) priv_data;
	unsigned 整数	offset = 0;
	unsigned 整数	next_real_entry = 0;
	整数		ret = 0;
	整数		changed = 0;
	整数		do_abort = 0;
	unsigned 整数	rec_len, size, buflen;
	整数		entry;
	struct ext2_dir_entry *dirent;
	整数		csum_size = 0;
	整数		inline_data;
	errcode_t	retval = 0;

	もし (blockcnt < 0)
		戻る 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	もし (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		もし (ctx->errcode)
			戻る BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} 否则 {
		buflen = ctx->buflen;
	}

	もし (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = sizeof(struct ext2_dir_entry_tail);

	もし (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		戻る BLOCK_ABORT;
	}
	ながら (offset < buflen - 8) {
		dirent = (struct ext2_dir_entry *) (ctx->buf + offset);
		もし (ext2fs_get_rec_len(fs, dirent, &rec_len))
			戻る BLOCK_ABORT;
		もし (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (整数) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			戻る BLOCK_ABORT;
		}
		もし (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			もし (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				もし (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					goto next;
				entry = DIRENT_CHECKSUM;
			} 否则 もし (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				goto next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		もし (entry < DIRENT_OTHER_FILE)
			entry++;

		もし (ret & DIRENT_CHANGED) {
			もし (ext2fs_get_rec_len(fs, dirent, &rec_len))
				戻る BLOCK_ABORT;
			changed++;
		}
		もし (ret & DIRENT_ABORT) {
			do_abort++;
			break;
		}
next:
 		もし (next_real_entry == offset)
			next_real_entry += rec_len;

 		もし (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			もし (rec_len != size)  {
				unsigned 整数 final_offset;

				final_offset = offset + rec_len;
				offset += size;
				ながら (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				continue;
			}
		}
		offset += rec_len;
	}

	もし (changed) {
		もし (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			もし (ctx->errcode)
				戻る BLOCK_ABORT;
		} 否则 {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	もし (do_abort)
		戻る retval | BLOCK_ABORT;
	戻る retval;
}
