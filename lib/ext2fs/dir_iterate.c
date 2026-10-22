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
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif
#အကယ်၍ HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     sa_nracanaa ext2_dir_entry *dirent,
			     unsigned ကိန်းပြည့် *rec_len)
{
	unsigned ကိန်းပြည့် len = dirent->rec_len;

	အကယ်၍ (fs->blocksize < 65536)
		*rec_len = len;
	မဟုတ်ပါက အကယ်၍ (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	မဟုတ်ပါက 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ပြန်ပေး 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned ကိန်းပြည့် len,
			     sa_nracanaa ext2_dir_entry *dirent)
{
	အကယ်၍ ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ပြန်ပေး EINVAL;
	အကယ်၍ (len < 65536) {
		dirent->rec_len = len;
		ပြန်ပေး 0;
	}
	အကယ်၍ (len == fs->blocksize) {
		အကယ်၍ (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		မဟုတ်ပါက 
			dirent->rec_len = 0;
	} မဟုတ်ပါက
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ပြန်ပေး 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
static ကိန်းပြည့် ext2fs_validate_entry(ext2_filsys fs, var_na *buf,
				 unsigned ကိန်းပြည့် offset,
				 unsigned ကိန်းပြည့် final_offset)
{
	sa_nracanaa ext2_dir_entry *dirent;
	unsigned ကိန်းပြည့် rec_len;
#define DIRENT_MIN_LENGTH 12

	နေစဉ် ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (sa_nracanaa ext2_dir_entry *)(buf + offset);
		အကယ်၍ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ပြန်ပေး 0;
		offset += rec_len;
		အကယ်၍ ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ကိန်းပြည့်) rec_len))
			ပြန်ပေး 0;
	}
	ပြန်ပေး (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ကိန်းပြည့် flags,
			      var_na *block_buf,
			      ကိန်းပြည့် (*func)(ext2_ino_t	dir,
					  ကိန်းပြည့်		entry,
					  sa_nracanaa ext2_dir_entry *dirent,
					  ကိန်းပြည့်	offset,
					  ကိန်းပြည့်	blocksize,
					  var_na	*buf,
					  void	*priv_data),
			      void *priv_data)
{
	sa_nracanaa		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	အကယ်၍ (retval)
		ပြန်ပေး retval;

	ctx.dir = dir;
	ctx.flags = flags;
	အကယ်၍ (block_buf)
		ctx.buf = block_buf;
	မဟုတ်ပါက {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		အကယ်၍ (retval)
			ပြန်ပေး retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	အကယ်၍ (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	အကယ်၍ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(void) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	အကယ်၍ (retval)
		ပြန်ပေး retval;
	ပြန်ပေး ctx.errcode;
}

sa_nracanaa xlate {
	ကိန်းပြည့် (*func)(sa_nracanaa ext2_dir_entry *dirent,
		    ကိန်းပြည့်		offset,
		    ကိန်းပြည့်		blocksize,
		    var_na	*buf,
		    void	*priv_data);
	void *real_private;
};

static ကိန်းပြည့် xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      ကိန်းပြည့် entry EXT2FS_ATTR((unused)),
		      sa_nracanaa ext2_dir_entry *dirent, ကိန်းပြည့် offset,
		      ကိန်းပြည့် blocksize, var_na *buf, void *priv_data)
{
	sa_nracanaa xlate *xl = (sa_nracanaa xlate *) priv_data;

	ပြန်ပေး (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     ကိန်းပြည့် flags,
			     var_na *block_buf,
			     ကိန်းပြည့် (*func)(sa_nracanaa ext2_dir_entry *dirent,
					 ကိန်းပြည့်	offset,
					 ကိန်းပြည့်	blocksize,
					 var_na	*buf,
					 void	*priv_data),
			     void *priv_data)
{
	sa_nracanaa xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ပြန်ပေး ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
ကိန်းပြည့် ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     ကိန်းပြည့်	ref_offset EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	sa_nracanaa dir_context *ctx = (sa_nracanaa dir_context *) priv_data;
	unsigned ကိန်းပြည့်	offset = 0;
	unsigned ကိန်းပြည့်	next_real_entry = 0;
	ကိန်းပြည့်		ret = 0;
	ကိန်းပြည့်		changed = 0;
	ကိန်းပြည့်		do_abort = 0;
	unsigned ကိန်းပြည့်	rec_len, size, buflen;
	ကိန်းပြည့်		entry;
	sa_nracanaa ext2_dir_entry *dirent;
	ကိန်းပြည့်		csum_size = 0;
	ကိန်းပြည့်		inline_data;
	errcode_t	retval = 0;

	အကယ်၍ (blockcnt < 0)
		ပြန်ပေး 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	အကယ်၍ (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		အကယ်၍ (ctx->errcode)
			ပြန်ပေး BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} မဟုတ်ပါက {
		buflen = ctx->buflen;
	}

	အကယ်၍ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = parimaa_na(sa_nracanaa ext2_dir_entry_tail);

	အကယ်၍ (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ပြန်ပေး BLOCK_ABORT;
	}
	နေစဉ် (offset < buflen - 8) {
		dirent = (sa_nracanaa ext2_dir_entry *) (ctx->buf + offset);
		အကယ်၍ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ပြန်ပေး BLOCK_ABORT;
		အကယ်၍ (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ကိန်းပြည့်) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ပြန်ပေး BLOCK_ABORT;
		}
		အကယ်၍ (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			အကယ်၍ (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				အကယ်၍ (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					goto next;
				entry = DIRENT_CHECKSUM;
			} မဟုတ်ပါက အကယ်၍ (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				goto next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		အကယ်၍ (entry < DIRENT_OTHER_FILE)
			entry++;

		အကယ်၍ (ret & DIRENT_CHANGED) {
			အကယ်၍ (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ပြန်ပေး BLOCK_ABORT;
			changed++;
		}
		အကယ်၍ (ret & DIRENT_ABORT) {
			do_abort++;
			bha_nga;
		}
next:
 		အကယ်၍ (next_real_entry == offset)
			next_real_entry += rec_len;

 		အကယ်၍ (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			အကယ်၍ (rec_len != size)  {
				unsigned ကိန်းပြည့် final_offset;

				final_offset = offset + rec_len;
				offset += size;
				နေစဉ် (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				continue;
			}
		}
		offset += rec_len;
	}

	အကယ်၍ (changed) {
		အကယ်၍ (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			အကယ်၍ (ctx->errcode)
				ပြန်ပေး BLOCK_ABORT;
		} မဟုတ်ပါက {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	အကယ်၍ (do_abort)
		ပြန်ပေး retval | BLOCK_ABORT;
	ပြန်ပေး retval;
}
