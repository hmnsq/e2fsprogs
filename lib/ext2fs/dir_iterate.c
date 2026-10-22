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
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ຖ້າ HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     sa_nracanaa ext2_dir_entry *dirent,
			     unsigned ຈຳນວນເຕັມ *rec_len)
{
	unsigned ຈຳນວນເຕັມ len = dirent->rec_len;

	ຖ້າ (fs->blocksize < 65536)
		*rec_len = len;
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	ບໍ່ດັ່ງນັ້ນ 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned ຈຳນວນເຕັມ len,
			     sa_nracanaa ext2_dir_entry *dirent)
{
	ຖ້າ ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ສົ່ງຄືນ EINVAL;
	ຖ້າ (len < 65536) {
		dirent->rec_len = len;
		ສົ່ງຄືນ 0;
	}
	ຖ້າ (len == fs->blocksize) {
		ຖ້າ (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		ບໍ່ດັ່ງນັ້ນ 
			dirent->rec_len = 0;
	} ບໍ່ດັ່ງນັ້ນ
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ສົ່ງຄືນ 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
static ຈຳນວນເຕັມ ext2fs_validate_entry(ext2_filsys fs, var_na *buf,
				 unsigned ຈຳນວນເຕັມ offset,
				 unsigned ຈຳນວນເຕັມ final_offset)
{
	sa_nracanaa ext2_dir_entry *dirent;
	unsigned ຈຳນວນເຕັມ rec_len;
#define DIRENT_MIN_LENGTH 12

	ໃນຂະນະທີ່ ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (sa_nracanaa ext2_dir_entry *)(buf + offset);
		ຖ້າ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ສົ່ງຄືນ 0;
		offset += rec_len;
		ຖ້າ ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ຈຳນວນເຕັມ) rec_len))
			ສົ່ງຄືນ 0;
	}
	ສົ່ງຄືນ (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ຈຳນວນເຕັມ flags,
			      var_na *block_buf,
			      ຈຳນວນເຕັມ (*func)(ext2_ino_t	dir,
					  ຈຳນວນເຕັມ		entry,
					  sa_nracanaa ext2_dir_entry *dirent,
					  ຈຳນວນເຕັມ	offset,
					  ຈຳນວນເຕັມ	blocksize,
					  var_na	*buf,
					  void	*priv_data),
			      void *priv_data)
{
	sa_nracanaa		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ຖ້າ (block_buf)
		ctx.buf = block_buf;
	ບໍ່ດັ່ງນັ້ນ {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	ຖ້າ (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	ຖ້າ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(void) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;
	ສົ່ງຄືນ ctx.errcode;
}

sa_nracanaa xlate {
	ຈຳນວນເຕັມ (*func)(sa_nracanaa ext2_dir_entry *dirent,
		    ຈຳນວນເຕັມ		offset,
		    ຈຳນວນເຕັມ		blocksize,
		    var_na	*buf,
		    void	*priv_data);
	void *real_private;
};

static ຈຳນວນເຕັມ xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      ຈຳນວນເຕັມ entry EXT2FS_ATTR((unused)),
		      sa_nracanaa ext2_dir_entry *dirent, ຈຳນວນເຕັມ offset,
		      ຈຳນວນເຕັມ blocksize, var_na *buf, void *priv_data)
{
	sa_nracanaa xlate *xl = (sa_nracanaa xlate *) priv_data;

	ສົ່ງຄືນ (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     ຈຳນວນເຕັມ flags,
			     var_na *block_buf,
			     ຈຳນວນເຕັມ (*func)(sa_nracanaa ext2_dir_entry *dirent,
					 ຈຳນວນເຕັມ	offset,
					 ຈຳນວນເຕັມ	blocksize,
					 var_na	*buf,
					 void	*priv_data),
			     void *priv_data)
{
	sa_nracanaa xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ສົ່ງຄືນ ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
ຈຳນວນເຕັມ ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     ຈຳນວນເຕັມ	ref_offset EXT2FS_ATTR((unused)),
			     void	*priv_data)
{
	sa_nracanaa dir_context *ctx = (sa_nracanaa dir_context *) priv_data;
	unsigned ຈຳນວນເຕັມ	offset = 0;
	unsigned ຈຳນວນເຕັມ	next_real_entry = 0;
	ຈຳນວນເຕັມ		ret = 0;
	ຈຳນວນເຕັມ		changed = 0;
	ຈຳນວນເຕັມ		do_abort = 0;
	unsigned ຈຳນວນເຕັມ	rec_len, size, buflen;
	ຈຳນວນເຕັມ		entry;
	sa_nracanaa ext2_dir_entry *dirent;
	ຈຳນວນເຕັມ		csum_size = 0;
	ຈຳນວນເຕັມ		inline_data;
	errcode_t	retval = 0;

	ຖ້າ (blockcnt < 0)
		ສົ່ງຄືນ 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	ຖ້າ (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ຖ້າ (ctx->errcode)
			ສົ່ງຄືນ BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} ບໍ່ດັ່ງນັ້ນ {
		buflen = ctx->buflen;
	}

	ຖ້າ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = parimaa_na(sa_nracanaa ext2_dir_entry_tail);

	ຖ້າ (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ສົ່ງຄືນ BLOCK_ABORT;
	}
	ໃນຂະນະທີ່ (offset < buflen - 8) {
		dirent = (sa_nracanaa ext2_dir_entry *) (ctx->buf + offset);
		ຖ້າ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ສົ່ງຄືນ BLOCK_ABORT;
		ຖ້າ (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ຈຳນວນເຕັມ) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ສົ່ງຄືນ BLOCK_ABORT;
		}
		ຖ້າ (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ຖ້າ (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ຖ້າ (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					goto next;
				entry = DIRENT_CHECKSUM;
			} ບໍ່ດັ່ງນັ້ນ ຖ້າ (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				goto next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		ຖ້າ (entry < DIRENT_OTHER_FILE)
			entry++;

		ຖ້າ (ret & DIRENT_CHANGED) {
			ຖ້າ (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ສົ່ງຄືນ BLOCK_ABORT;
			changed++;
		}
		ຖ້າ (ret & DIRENT_ABORT) {
			do_abort++;
			bha_nga;
		}
next:
 		ຖ້າ (next_real_entry == offset)
			next_real_entry += rec_len;

 		ຖ້າ (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ຖ້າ (rec_len != size)  {
				unsigned ຈຳນວນເຕັມ final_offset;

				final_offset = offset + rec_len;
				offset += size;
				ໃນຂະນະທີ່ (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				continue;
			}
		}
		offset += rec_len;
	}

	ຖ້າ (changed) {
		ຖ້າ (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ຖ້າ (ctx->errcode)
				ສົ່ງຄືນ BLOCK_ABORT;
		} ບໍ່ດັ່ງນັ້ນ {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	ຖ້າ (do_abort)
		ສົ່ງຄືນ retval | BLOCK_ABORT;
	ສົ່ງຄືນ retval;
}
