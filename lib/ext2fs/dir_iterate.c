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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     সংরচনা ext2_dir_entry *dirent,
			     unsigned পূর্ণাঙ্ক *rec_len)
{
	unsigned পূর্ণাঙ্ক len = dirent->rec_len;

	যদি (fs->blocksize < 65536)
		*rec_len = len;
	অন্যথা যদি (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	অন্যথা 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ঘূৰাই 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned পূর্ণাঙ্ক len,
			     সংরচনা ext2_dir_entry *dirent)
{
	যদি ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ঘূৰাই EINVAL;
	যদি (len < 65536) {
		dirent->rec_len = len;
		ঘূৰাই 0;
	}
	যদি (len == fs->blocksize) {
		যদি (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		অন্যথা 
			dirent->rec_len = 0;
	} অন্যথা
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ঘূৰাই 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
স্থির পূর্ণাঙ্ক ext2fs_validate_entry(ext2_filsys fs, ৱর্ণ *buf,
				 unsigned পূর্ণাঙ্ক offset,
				 unsigned পূর্ণাঙ্ক final_offset)
{
	সংরচনা ext2_dir_entry *dirent;
	unsigned পূর্ণাঙ্ক rec_len;
#define DIRENT_MIN_LENGTH 12

	যেতিয়ালৈ ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (সংরচনা ext2_dir_entry *)(buf + offset);
		যদি (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ঘূৰাই 0;
		offset += rec_len;
		যদি ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (পূর্ণাঙ্ক) rec_len))
			ঘূৰাই 0;
	}
	ঘূৰাই (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      পূর্ণাঙ্ক flags,
			      ৱর্ণ *block_buf,
			      পূর্ণাঙ্ক (*func)(ext2_ino_t	dir,
					  পূর্ণাঙ্ক		entry,
					  সংরচনা ext2_dir_entry *dirent,
					  পূর্ণাঙ্ক	offset,
					  পূর্ণাঙ্ক	blocksize,
					  ৱর্ণ	*buf,
					  শূন্য	*priv_data),
			      শূন্য *priv_data)
{
	সংরচনা		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	যদি (retval)
		ঘূৰাই retval;

	ctx.dir = dir;
	ctx.flags = flags;
	যদি (block_buf)
		ctx.buf = block_buf;
	অন্যথা {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		যদি (retval)
			ঘূৰাই retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	যদি (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	যদি (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(শূন্য) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	যদি (retval)
		ঘূৰাই retval;
	ঘূৰাই ctx.errcode;
}

সংরচনা xlate {
	পূর্ণাঙ্ক (*func)(সংরচনা ext2_dir_entry *dirent,
		    পূর্ণাঙ্ক		offset,
		    পূর্ণাঙ্ক		blocksize,
		    ৱর্ণ	*buf,
		    শূন্য	*priv_data);
	শূন্য *real_private;
};

স্থির পূর্ণাঙ্ক xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      পূর্ণাঙ্ক entry EXT2FS_ATTR((unused)),
		      সংরচনা ext2_dir_entry *dirent, পূর্ণাঙ্ক offset,
		      পূর্ণাঙ্ক blocksize, ৱর্ণ *buf, শূন্য *priv_data)
{
	সংরচনা xlate *xl = (সংরচনা xlate *) priv_data;

	ঘূৰাই (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     পূর্ণাঙ্ক flags,
			     ৱর্ণ *block_buf,
			     পূর্ণাঙ্ক (*func)(সংরচনা ext2_dir_entry *dirent,
					 পূর্ণাঙ্ক	offset,
					 পূর্ণাঙ্ক	blocksize,
					 ৱর্ণ	*buf,
					 শূন্য	*priv_data),
			     শূন্য *priv_data)
{
	সংরচনা xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ঘূৰাই ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
পূর্ণাঙ্ক ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     পূর্ণাঙ্ক	ref_offset EXT2FS_ATTR((unused)),
			     শূন্য	*priv_data)
{
	সংরচনা dir_context *ctx = (সংরচনা dir_context *) priv_data;
	unsigned পূর্ণাঙ্ক	offset = 0;
	unsigned পূর্ণাঙ্ক	next_real_entry = 0;
	পূর্ণাঙ্ক		ret = 0;
	পূর্ণাঙ্ক		changed = 0;
	পূর্ণাঙ্ক		do_abort = 0;
	unsigned পূর্ণাঙ্ক	rec_len, size, buflen;
	পূর্ণাঙ্ক		entry;
	সংরচনা ext2_dir_entry *dirent;
	পূর্ণাঙ্ক		csum_size = 0;
	পূর্ণাঙ্ক		inline_data;
	errcode_t	retval = 0;

	যদি (blockcnt < 0)
		ঘূৰাই 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	যদি (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		যদি (ctx->errcode)
			ঘূৰাই BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} অন্যথা {
		buflen = ctx->buflen;
	}

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = পরিমাণ(সংরচনা ext2_dir_entry_tail);

	যদি (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ঘূৰাই BLOCK_ABORT;
	}
	যেতিয়ালৈ (offset < buflen - 8) {
		dirent = (সংরচনা ext2_dir_entry *) (ctx->buf + offset);
		যদি (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ঘূৰাই BLOCK_ABORT;
		যদি (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (পূর্ণাঙ্ক) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ঘূৰাই BLOCK_ABORT;
		}
		যদি (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			যদি (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				যদি (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					গচ্ছ next;
				entry = DIRENT_CHECKSUM;
			} অন্যথা যদি (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				গচ্ছ next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		যদি (entry < DIRENT_OTHER_FILE)
			entry++;

		যদি (ret & DIRENT_CHANGED) {
			যদি (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ঘূৰাই BLOCK_ABORT;
			changed++;
		}
		যদি (ret & DIRENT_ABORT) {
			do_abort++;
			ভঙ্গ;
		}
next:
 		যদি (next_real_entry == offset)
			next_real_entry += rec_len;

 		যদি (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			যদি (rec_len != size)  {
				unsigned পূর্ণাঙ্ক final_offset;

				final_offset = offset + rec_len;
				offset += size;
				যেতিয়ালৈ (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				অনুৱর্ত;
			}
		}
		offset += rec_len;
	}

	যদি (changed) {
		যদি (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			যদি (ctx->errcode)
				ঘূৰাই BLOCK_ABORT;
		} অন্যথা {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	যদি (do_abort)
		ঘূৰাই retval | BLOCK_ABORT;
	ঘূৰাই retval;
}
