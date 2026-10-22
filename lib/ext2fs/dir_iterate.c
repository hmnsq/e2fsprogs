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
#නම් HAVE_UNISTD_H
#include <unistd.h>
#endif
#නම් HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     sa_nracanaa ext2_dir_entry *dirent,
			     unsigned පූර්ණ_සංඛ්‍යා *rec_len)
{
	unsigned පූර්ණ_සංඛ්‍යා len = dirent->rec_len;

	නම් (fs->blocksize < 65536)
		*rec_len = len;
	නැතහොත් නම් (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	නැතහොත් 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ආපසු 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned පූර්ණ_සංඛ්‍යා len,
			     sa_nracanaa ext2_dir_entry *dirent)
{
	නම් ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ආපසු EINVAL;
	නම් (len < 65536) {
		dirent->rec_len = len;
		ආපසු 0;
	}
	නම් (len == fs->blocksize) {
		නම් (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		නැතහොත් 
			dirent->rec_len = 0;
	} නැතහොත්
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ආපසු 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
static පූර්ණ_සංඛ්‍යා ext2fs_validate_entry(ext2_filsys fs, var_na *buf,
				 unsigned පූර්ණ_සංඛ්‍යා offset,
				 unsigned පූර්ණ_සංඛ්‍යා final_offset)
{
	sa_nracanaa ext2_dir_entry *dirent;
	unsigned පූර්ණ_සංඛ්‍යා rec_len;
#define DIRENT_MIN_LENGTH 12

	අතරතුර ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (sa_nracanaa ext2_dir_entry *)(buf + offset);
		නම් (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ආපසු 0;
		offset += rec_len;
		නම් ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (පූර්ණ_සංඛ්‍යා) rec_len))
			ආපසු 0;
	}
	ආපසු (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      පූර්ණ_සංඛ්‍යා flags,
			      var_na *block_buf,
			      පූර්ණ_සංඛ්‍යා (*func)(ext2_ino_t	dir,
					  පූර්ණ_සංඛ්‍යා		entry,
					  sa_nracanaa ext2_dir_entry *dirent,
					  පූර්ණ_සංඛ්‍යා	offset,
					  පූර්ණ_සංඛ්‍යා	blocksize,
					  var_na	*buf,
					  void	*priv_data),
			      void *priv_data)
{
	sa_nracanaa		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	නම් (retval)
		ආපසු retval;

	ctx.dir = dir;
	ctx.flags = flags;
	නම් (block_buf)
		ctx.buf = block_buf;
	නැතහොත් {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		නම් (retval)
			ආපසු retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	නම් (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	නම් (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(void) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	නම් (retval)
		ආපසු retval;
	ආපසු ctx.errcode;
}

sa_nracanaa xlate {
	පූර්ණ_සංඛ්‍යා (*func)(sa_nracanaa ext2_dir_entry *dirent,
		    පූර්ණ_සංඛ්‍යා		offset,
		    පූර්ණ_සංඛ්‍යා		blocksize,
		    var_na	*buf,
		    void	*priv_data);
	void *real_private;
};

static පූර්ණ_සංඛ්‍යා xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      පූර්ණ_සංඛ්‍යා entry EXT2FS_ATTR((unused)),
		      sa_nracanaa ext2_dir_entry *dirent, පූර්ණ_සංඛ්‍යා offset,
		      පූර්ණ_සංඛ්‍යා blocksize, var_na *buf, void *priv_data)
{
	sa_nracanaa xlate *xl = (sa_nracanaa xlate *) priv_data;

	ආපසු (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     පූර්ණ_සංඛ්‍යා flags,
			     var_na *block_buf,
			     පූර්ණ_සංඛ්‍යා (*func)(sa_nracanaa ext2_dir_entry *dirent,
					 පූර්ණ_සංඛ්‍යා	offset,
					 පූර්ණ_සංඛ්‍යා	blocksize,
					 var_na	*buf,
					 void	*priv_data),
			     void *priv_data)
{
	sa_nracanaa xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ආපසු ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
පූර්ණ_සංඛ්‍යා ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     පූර්ණ_සංඛ්‍යා	ref_offset EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	sa_nracanaa dir_context *ctx = (sa_nracanaa dir_context *) priv_data;
	unsigned පූර්ණ_සංඛ්‍යා	offset = 0;
	unsigned පූර්ණ_සංඛ්‍යා	next_real_entry = 0;
	පූර්ණ_සංඛ්‍යා		ret = 0;
	පූර්ණ_සංඛ්‍යා		changed = 0;
	පූර්ණ_සංඛ්‍යා		do_abort = 0;
	unsigned පූර්ණ_සංඛ්‍යා	rec_len, size, buflen;
	පූර්ණ_සංඛ්‍යා		entry;
	sa_nracanaa ext2_dir_entry *dirent;
	පූර්ණ_සංඛ්‍යා		csum_size = 0;
	පූර්ණ_සංඛ්‍යා		inline_data;
	errcode_t	retval = 0;

	නම් (blockcnt < 0)
		ආපසු 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	නම් (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		නම් (ctx->errcode)
			ආපසු BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} නැතහොත් {
		buflen = ctx->buflen;
	}

	නම් (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = parimaa_na(sa_nracanaa ext2_dir_entry_tail);

	නම් (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ආපසු BLOCK_ABORT;
	}
	අතරතුර (offset < buflen - 8) {
		dirent = (sa_nracanaa ext2_dir_entry *) (ctx->buf + offset);
		නම් (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ආපසු BLOCK_ABORT;
		නම් (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (පූර්ණ_සංඛ්‍යා) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ආපසු BLOCK_ABORT;
		}
		නම් (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			නම් (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				නම් (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					goto next;
				entry = DIRENT_CHECKSUM;
			} නැතහොත් නම් (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				goto next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		නම් (entry < DIRENT_OTHER_FILE)
			entry++;

		නම් (ret & DIRENT_CHANGED) {
			නම් (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ආපසු BLOCK_ABORT;
			changed++;
		}
		නම් (ret & DIRENT_ABORT) {
			do_abort++;
			bha_nga;
		}
next:
 		නම් (next_real_entry == offset)
			next_real_entry += rec_len;

 		නම් (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			නම් (rec_len != size)  {
				unsigned පූර්ණ_සංඛ්‍යා final_offset;

				final_offset = offset + rec_len;
				offset += size;
				අතරතුර (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				continue;
			}
		}
		offset += rec_len;
	}

	නම් (changed) {
		නම් (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			නම් (ctx->errcode)
				ආපසු BLOCK_ABORT;
		} නැතහොත් {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	නම් (do_abort)
		ආපසු retval | BLOCK_ABORT;
	ආපසු retval;
}
