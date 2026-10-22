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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     بنية ext2_dir_entry *dirent,
			     unsigned صحيح *rec_len)
{
	unsigned صحيح len = dirent->rec_len;

	ܐܢ (fs->blocksize < 65536)
		*rec_len = len;
	ܐܚܪܢܐ ܐܢ (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	ܐܚܪܢܐ 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ܦܢܐ 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned صحيح len,
			     بنية ext2_dir_entry *dirent)
{
	ܐܢ ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ܦܢܐ EINVAL;
	ܐܢ (len < 65536) {
		dirent->rec_len = len;
		ܦܢܐ 0;
	}
	ܐܢ (len == fs->blocksize) {
		ܐܢ (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		ܐܚܪܢܐ 
			dirent->rec_len = 0;
	} ܐܚܪܢܐ
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ܦܢܐ 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
ساكن صحيح ext2fs_validate_entry(ext2_filsys fs, حرف *buf,
				 unsigned صحيح offset,
				 unsigned صحيح final_offset)
{
	بنية ext2_dir_entry *dirent;
	unsigned صحيح rec_len;
#define DIRENT_MIN_LENGTH 12

	ܟܡܐ ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (بنية ext2_dir_entry *)(buf + offset);
		ܐܢ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ܦܢܐ 0;
		offset += rec_len;
		ܐܢ ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحيح) rec_len))
			ܦܢܐ 0;
	}
	ܦܢܐ (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحيح flags,
			      حرف *block_buf,
			      صحيح (*func)(ext2_ino_t	dir,
					  صحيح		entry,
					  بنية ext2_dir_entry *dirent,
					  صحيح	offset,
					  صحيح	blocksize,
					  حرف	*buf,
					  فراغ	*priv_data),
			      فراغ *priv_data)
{
	بنية		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ܐܢ (retval)
		ܦܢܐ retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ܐܢ (block_buf)
		ctx.buf = block_buf;
	ܐܚܪܢܐ {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ܐܢ (retval)
			ܦܢܐ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	ܐܢ (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	ܐܢ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(فراغ) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ܐܢ (retval)
		ܦܢܐ retval;
	ܦܢܐ ctx.errcode;
}

بنية xlate {
	صحيح (*func)(بنية ext2_dir_entry *dirent,
		    صحيح		offset,
		    صحيح		blocksize,
		    حرف	*buf,
		    فراغ	*priv_data);
	فراغ *real_private;
};

ساكن صحيح xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      صحيح entry EXT2FS_ATTR((unused)),
		      بنية ext2_dir_entry *dirent, صحيح offset,
		      صحيح blocksize, حرف *buf, فراغ *priv_data)
{
	بنية xlate *xl = (بنية xlate *) priv_data;

	ܦܢܐ (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     صحيح flags,
			     حرف *block_buf,
			     صحيح (*func)(بنية ext2_dir_entry *dirent,
					 صحيح	offset,
					 صحيح	blocksize,
					 حرف	*buf,
					 فراغ	*priv_data),
			     فراغ *priv_data)
{
	بنية xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ܦܢܐ ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
صحيح ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     صحيح	ref_offset EXT2FS_ATTR((unused)),
			     فراغ	*priv_data)
{
	بنية dir_context *ctx = (بنية dir_context *) priv_data;
	unsigned صحيح	offset = 0;
	unsigned صحيح	next_real_entry = 0;
	صحيح		ret = 0;
	صحيح		changed = 0;
	صحيح		do_abort = 0;
	unsigned صحيح	rec_len, size, buflen;
	صحيح		entry;
	بنية ext2_dir_entry *dirent;
	صحيح		csum_size = 0;
	صحيح		inline_data;
	errcode_t	retval = 0;

	ܐܢ (blockcnt < 0)
		ܦܢܐ 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	ܐܢ (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ܐܢ (ctx->errcode)
			ܦܢܐ BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} ܐܚܪܢܐ {
		buflen = ctx->buflen;
	}

	ܐܢ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = حجم(بنية ext2_dir_entry_tail);

	ܐܢ (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ܦܢܐ BLOCK_ABORT;
	}
	ܟܡܐ (offset < buflen - 8) {
		dirent = (بنية ext2_dir_entry *) (ctx->buf + offset);
		ܐܢ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ܦܢܐ BLOCK_ABORT;
		ܐܢ (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحيح) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ܦܢܐ BLOCK_ABORT;
		}
		ܐܢ (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ܐܢ (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ܐܢ (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					اذهب next;
				entry = DIRENT_CHECKSUM;
			} ܐܚܪܢܐ ܐܢ (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				اذهب next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		ܐܢ (entry < DIRENT_OTHER_FILE)
			entry++;

		ܐܢ (ret & DIRENT_CHANGED) {
			ܐܢ (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ܦܢܐ BLOCK_ABORT;
			changed++;
		}
		ܐܢ (ret & DIRENT_ABORT) {
			do_abort++;
			اكسر;
		}
next:
 		ܐܢ (next_real_entry == offset)
			next_real_entry += rec_len;

 		ܐܢ (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ܐܢ (rec_len != size)  {
				unsigned صحيح final_offset;

				final_offset = offset + rec_len;
				offset += size;
				ܟܡܐ (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				استمر;
			}
		}
		offset += rec_len;
	}

	ܐܢ (changed) {
		ܐܢ (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ܐܢ (ctx->errcode)
				ܦܢܐ BLOCK_ABORT;
		} ܐܚܪܢܐ {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	ܐܢ (do_abort)
		ܦܢܐ retval | BLOCK_ABORT;
	ܦܢܐ retval;
}
