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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     ساختار ext2_dir_entry *dirent,
			     unsigned صحیح *rec_len)
{
	unsigned صحیح len = dirent->rec_len;

	اگر (fs->blocksize < 65536)
		*rec_len = len;
	وگرنه اگر (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	وگرنه 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	بازگشت 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned صحیح len,
			     ساختار ext2_dir_entry *dirent)
{
	اگر ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		بازگشت EINVAL;
	اگر (len < 65536) {
		dirent->rec_len = len;
		بازگشت 0;
	}
	اگر (len == fs->blocksize) {
		اگر (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		وگرنه 
			dirent->rec_len = 0;
	} وگرنه
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	بازگشت 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
ایستا صحیح ext2fs_validate_entry(ext2_filsys fs, نویسه *buf,
				 unsigned صحیح offset,
				 unsigned صحیح final_offset)
{
	ساختار ext2_dir_entry *dirent;
	unsigned صحیح rec_len;
#define DIRENT_MIN_LENGTH 12

	وقتی ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (ساختار ext2_dir_entry *)(buf + offset);
		اگر (ext2fs_get_rec_len(fs, dirent, &rec_len))
			بازگشت 0;
		offset += rec_len;
		اگر ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحیح) rec_len))
			بازگشت 0;
	}
	بازگشت (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      صحیح flags,
			      نویسه *block_buf,
			      صحیح (*func)(ext2_ino_t	dir,
					  صحیح		entry,
					  ساختار ext2_dir_entry *dirent,
					  صحیح	offset,
					  صحیح	blocksize,
					  نویسه	*buf,
					  تهی	*priv_data),
			      تهی *priv_data)
{
	ساختار		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	اگر (retval)
		بازگشت retval;

	ctx.dir = dir;
	ctx.flags = flags;
	اگر (block_buf)
		ctx.buf = block_buf;
	وگرنه {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		اگر (retval)
			بازگشت retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	اگر (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	اگر (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(تهی) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	اگر (retval)
		بازگشت retval;
	بازگشت ctx.errcode;
}

ساختار xlate {
	صحیح (*func)(ساختار ext2_dir_entry *dirent,
		    صحیح		offset,
		    صحیح		blocksize,
		    نویسه	*buf,
		    تهی	*priv_data);
	تهی *real_private;
};

ایستا صحیح xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      صحیح entry EXT2FS_ATTR((unused)),
		      ساختار ext2_dir_entry *dirent, صحیح offset,
		      صحیح blocksize, نویسه *buf, تهی *priv_data)
{
	ساختار xlate *xl = (ساختار xlate *) priv_data;

	بازگشت (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     صحیح flags,
			     نویسه *block_buf,
			     صحیح (*func)(ساختار ext2_dir_entry *dirent,
					 صحیح	offset,
					 صحیح	blocksize,
					 نویسه	*buf,
					 تهی	*priv_data),
			     تهی *priv_data)
{
	ساختار xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	بازگشت ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
صحیح ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     صحیح	ref_offset EXT2FS_ATTR((unused)),
			     تهی	*priv_data)
{
	ساختار dir_context *ctx = (ساختار dir_context *) priv_data;
	unsigned صحیح	offset = 0;
	unsigned صحیح	next_real_entry = 0;
	صحیح		ret = 0;
	صحیح		changed = 0;
	صحیح		do_abort = 0;
	unsigned صحیح	rec_len, size, buflen;
	صحیح		entry;
	ساختار ext2_dir_entry *dirent;
	صحیح		csum_size = 0;
	صحیح		inline_data;
	errcode_t	retval = 0;

	اگر (blockcnt < 0)
		بازگشت 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	اگر (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		اگر (ctx->errcode)
			بازگشت BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} وگرنه {
		buflen = ctx->buflen;
	}

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = اندازه(ساختار ext2_dir_entry_tail);

	اگر (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		بازگشت BLOCK_ABORT;
	}
	وقتی (offset < buflen - 8) {
		dirent = (ساختار ext2_dir_entry *) (ctx->buf + offset);
		اگر (ext2fs_get_rec_len(fs, dirent, &rec_len))
			بازگشت BLOCK_ABORT;
		اگر (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (صحیح) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			بازگشت BLOCK_ABORT;
		}
		اگر (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			اگر (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				اگر (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					برو next;
				entry = DIRENT_CHECKSUM;
			} وگرنه اگر (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				برو next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		اگر (entry < DIRENT_OTHER_FILE)
			entry++;

		اگر (ret & DIRENT_CHANGED) {
			اگر (ext2fs_get_rec_len(fs, dirent, &rec_len))
				بازگشت BLOCK_ABORT;
			changed++;
		}
		اگر (ret & DIRENT_ABORT) {
			do_abort++;
			بشکن;
		}
next:
 		اگر (next_real_entry == offset)
			next_real_entry += rec_len;

 		اگر (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			اگر (rec_len != size)  {
				unsigned صحیح final_offset;

				final_offset = offset + rec_len;
				offset += size;
				وقتی (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				ادامه;
			}
		}
		offset += rec_len;
	}

	اگر (changed) {
		اگر (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			اگر (ctx->errcode)
				بازگشت BLOCK_ABORT;
		} وگرنه {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	اگر (do_abort)
		بازگشت retval | BLOCK_ABORT;
	بازگشت retval;
}
