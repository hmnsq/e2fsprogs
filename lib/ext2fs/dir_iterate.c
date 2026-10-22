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
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ប្រសិនបើ HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     sa_nracanaa ext2_dir_entry *dirent,
			     unsigned ចំនួនគត់ *rec_len)
{
	unsigned ចំនួនគត់ len = dirent->rec_len;

	ប្រសិនបើ (fs->blocksize < 65536)
		*rec_len = len;
	បើមិនដូច្នេះ ប្រសិនបើ (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	បើមិនដូច្នេះ 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ត្រឡប់ 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned ចំនួនគត់ len,
			     sa_nracanaa ext2_dir_entry *dirent)
{
	ប្រសិនបើ ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ត្រឡប់ EINVAL;
	ប្រសិនបើ (len < 65536) {
		dirent->rec_len = len;
		ត្រឡប់ 0;
	}
	ប្រសិនបើ (len == fs->blocksize) {
		ប្រសិនបើ (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		បើមិនដូច្នេះ 
			dirent->rec_len = 0;
	} បើមិនដូច្នេះ
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ត្រឡប់ 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
static ចំនួនគត់ ext2fs_validate_entry(ext2_filsys fs, var_na *buf,
				 unsigned ចំនួនគត់ offset,
				 unsigned ចំនួនគត់ final_offset)
{
	sa_nracanaa ext2_dir_entry *dirent;
	unsigned ចំនួនគត់ rec_len;
#define DIRENT_MIN_LENGTH 12

	ខណៈពេល ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (sa_nracanaa ext2_dir_entry *)(buf + offset);
		ប្រសិនបើ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ត្រឡប់ 0;
		offset += rec_len;
		ប្រសិនបើ ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ចំនួនគត់) rec_len))
			ត្រឡប់ 0;
	}
	ត្រឡប់ (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ចំនួនគត់ flags,
			      var_na *block_buf,
			      ចំនួនគត់ (*func)(ext2_ino_t	dir,
					  ចំនួនគត់		entry,
					  sa_nracanaa ext2_dir_entry *dirent,
					  ចំនួនគត់	offset,
					  ចំនួនគត់	blocksize,
					  var_na	*buf,
					  void	*priv_data),
			      void *priv_data)
{
	sa_nracanaa		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ប្រសិនបើ (block_buf)
		ctx.buf = block_buf;
	បើមិនដូច្នេះ {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ប្រសិនបើ (retval)
			ត្រឡប់ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	ប្រសិនបើ (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	ប្រសិនបើ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(void) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;
	ត្រឡប់ ctx.errcode;
}

sa_nracanaa xlate {
	ចំនួនគត់ (*func)(sa_nracanaa ext2_dir_entry *dirent,
		    ចំនួនគត់		offset,
		    ចំនួនគត់		blocksize,
		    var_na	*buf,
		    void	*priv_data);
	void *real_private;
};

static ចំនួនគត់ xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      ចំនួនគត់ entry EXT2FS_ATTR((unused)),
		      sa_nracanaa ext2_dir_entry *dirent, ចំនួនគត់ offset,
		      ចំនួនគត់ blocksize, var_na *buf, void *priv_data)
{
	sa_nracanaa xlate *xl = (sa_nracanaa xlate *) priv_data;

	ត្រឡប់ (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     ចំនួនគត់ flags,
			     var_na *block_buf,
			     ចំនួនគត់ (*func)(sa_nracanaa ext2_dir_entry *dirent,
					 ចំនួនគត់	offset,
					 ចំនួនគត់	blocksize,
					 var_na	*buf,
					 void	*priv_data),
			     void *priv_data)
{
	sa_nracanaa xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ត្រឡប់ ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
ចំនួនគត់ ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     ចំនួនគត់	ref_offset EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	sa_nracanaa dir_context *ctx = (sa_nracanaa dir_context *) priv_data;
	unsigned ចំនួនគត់	offset = 0;
	unsigned ចំនួនគត់	next_real_entry = 0;
	ចំនួនគត់		ret = 0;
	ចំនួនគត់		changed = 0;
	ចំនួនគត់		do_abort = 0;
	unsigned ចំនួនគត់	rec_len, size, buflen;
	ចំនួនគត់		entry;
	sa_nracanaa ext2_dir_entry *dirent;
	ចំនួនគត់		csum_size = 0;
	ចំនួនគត់		inline_data;
	errcode_t	retval = 0;

	ប្រសិនបើ (blockcnt < 0)
		ត្រឡប់ 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	ប្រសិនបើ (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ប្រសិនបើ (ctx->errcode)
			ត្រឡប់ BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} បើមិនដូច្នេះ {
		buflen = ctx->buflen;
	}

	ប្រសិនបើ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = parimaa_na(sa_nracanaa ext2_dir_entry_tail);

	ប្រសិនបើ (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ត្រឡប់ BLOCK_ABORT;
	}
	ខណៈពេល (offset < buflen - 8) {
		dirent = (sa_nracanaa ext2_dir_entry *) (ctx->buf + offset);
		ប្រសិនបើ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ត្រឡប់ BLOCK_ABORT;
		ប្រសិនបើ (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ចំនួនគត់) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ត្រឡប់ BLOCK_ABORT;
		}
		ប្រសិនបើ (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ប្រសិនបើ (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ប្រសិនបើ (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					goto next;
				entry = DIRENT_CHECKSUM;
			} បើមិនដូច្នេះ ប្រសិនបើ (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				goto next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		ប្រសិនបើ (entry < DIRENT_OTHER_FILE)
			entry++;

		ប្រសិនបើ (ret & DIRENT_CHANGED) {
			ប្រសិនបើ (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ត្រឡប់ BLOCK_ABORT;
			changed++;
		}
		ប្រសិនបើ (ret & DIRENT_ABORT) {
			do_abort++;
			bha_nga;
		}
next:
 		ប្រសិនបើ (next_real_entry == offset)
			next_real_entry += rec_len;

 		ប្រសិនបើ (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ប្រសិនបើ (rec_len != size)  {
				unsigned ចំនួនគត់ final_offset;

				final_offset = offset + rec_len;
				offset += size;
				ខណៈពេល (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				continue;
			}
		}
		offset += rec_len;
	}

	ប្រសិនបើ (changed) {
		ប្រសិនបើ (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ប្រសិនបើ (ctx->errcode)
				ត្រឡប់ BLOCK_ABORT;
		} បើមិនដូច្នេះ {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	ប្រសិនបើ (do_abort)
		ត្រឡប់ retval | BLOCK_ABORT;
	ត្រឡប់ retval;
}
