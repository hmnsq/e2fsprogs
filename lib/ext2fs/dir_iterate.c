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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     கட்டமைப்பு ext2_dir_entry *dirent,
			     unsigned முழு *rec_len)
{
	unsigned முழு len = dirent->rec_len;

	எனில் (fs->blocksize < 65536)
		*rec_len = len;
	இல்லையெனில் எனில் (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	இல்லையெனில் 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	திரும்பு 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned முழு len,
			     கட்டமைப்பு ext2_dir_entry *dirent)
{
	எனில் ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		திரும்பு EINVAL;
	எனில் (len < 65536) {
		dirent->rec_len = len;
		திரும்பு 0;
	}
	எனில் (len == fs->blocksize) {
		எனில் (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		இல்லையெனில் 
			dirent->rec_len = 0;
	} இல்லையெனில்
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	திரும்பு 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
நிலையான முழு ext2fs_validate_entry(ext2_filsys fs, எழுத்து *buf,
				 unsigned முழு offset,
				 unsigned முழு final_offset)
{
	கட்டமைப்பு ext2_dir_entry *dirent;
	unsigned முழு rec_len;
#define DIRENT_MIN_LENGTH 12

	போது ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (கட்டமைப்பு ext2_dir_entry *)(buf + offset);
		எனில் (ext2fs_get_rec_len(fs, dirent, &rec_len))
			திரும்பு 0;
		offset += rec_len;
		எனில் ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (முழு) rec_len))
			திரும்பு 0;
	}
	திரும்பு (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      முழு flags,
			      எழுத்து *block_buf,
			      முழு (*func)(ext2_ino_t	dir,
					  முழு		entry,
					  கட்டமைப்பு ext2_dir_entry *dirent,
					  முழு	offset,
					  முழு	blocksize,
					  எழுத்து	*buf,
					  வெறுமை	*priv_data),
			      வெறுமை *priv_data)
{
	கட்டமைப்பு		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	எனில் (retval)
		திரும்பு retval;

	ctx.dir = dir;
	ctx.flags = flags;
	எனில் (block_buf)
		ctx.buf = block_buf;
	இல்லையெனில் {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		எனில் (retval)
			திரும்பு retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	எனில் (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	எனில் (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(வெறுமை) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	எனில் (retval)
		திரும்பு retval;
	திரும்பு ctx.errcode;
}

கட்டமைப்பு xlate {
	முழு (*func)(கட்டமைப்பு ext2_dir_entry *dirent,
		    முழு		offset,
		    முழு		blocksize,
		    எழுத்து	*buf,
		    வெறுமை	*priv_data);
	வெறுமை *real_private;
};

நிலையான முழு xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      முழு entry EXT2FS_ATTR((unused)),
		      கட்டமைப்பு ext2_dir_entry *dirent, முழு offset,
		      முழு blocksize, எழுத்து *buf, வெறுமை *priv_data)
{
	கட்டமைப்பு xlate *xl = (கட்டமைப்பு xlate *) priv_data;

	திரும்பு (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     முழு flags,
			     எழுத்து *block_buf,
			     முழு (*func)(கட்டமைப்பு ext2_dir_entry *dirent,
					 முழு	offset,
					 முழு	blocksize,
					 எழுத்து	*buf,
					 வெறுமை	*priv_data),
			     வெறுமை *priv_data)
{
	கட்டமைப்பு xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	திரும்பு ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
முழு ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     முழு	ref_offset EXT2FS_ATTR((unused)),
			     வெறுமை	*priv_data)
{
	கட்டமைப்பு dir_context *ctx = (கட்டமைப்பு dir_context *) priv_data;
	unsigned முழு	offset = 0;
	unsigned முழு	next_real_entry = 0;
	முழு		ret = 0;
	முழு		changed = 0;
	முழு		do_abort = 0;
	unsigned முழு	rec_len, size, buflen;
	முழு		entry;
	கட்டமைப்பு ext2_dir_entry *dirent;
	முழு		csum_size = 0;
	முழு		inline_data;
	errcode_t	retval = 0;

	எனில் (blockcnt < 0)
		திரும்பு 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	எனில் (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		எனில் (ctx->errcode)
			திரும்பு BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} இல்லையெனில் {
		buflen = ctx->buflen;
	}

	எனில் (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = அளவு(கட்டமைப்பு ext2_dir_entry_tail);

	எனில் (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		திரும்பு BLOCK_ABORT;
	}
	போது (offset < buflen - 8) {
		dirent = (கட்டமைப்பு ext2_dir_entry *) (ctx->buf + offset);
		எனில் (ext2fs_get_rec_len(fs, dirent, &rec_len))
			திரும்பு BLOCK_ABORT;
		எனில் (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (முழு) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			திரும்பு BLOCK_ABORT;
		}
		எனில் (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			எனில் (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				எனில் (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					செல் next;
				entry = DIRENT_CHECKSUM;
			} இல்லையெனில் எனில் (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				செல் next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		எனில் (entry < DIRENT_OTHER_FILE)
			entry++;

		எனில் (ret & DIRENT_CHANGED) {
			எனில் (ext2fs_get_rec_len(fs, dirent, &rec_len))
				திரும்பு BLOCK_ABORT;
			changed++;
		}
		எனில் (ret & DIRENT_ABORT) {
			do_abort++;
			நிறுத்து;
		}
next:
 		எனில் (next_real_entry == offset)
			next_real_entry += rec_len;

 		எனில் (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			எனில் (rec_len != size)  {
				unsigned முழு final_offset;

				final_offset = offset + rec_len;
				offset += size;
				போது (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				தொடர்;
			}
		}
		offset += rec_len;
	}

	எனில் (changed) {
		எனில் (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			எனில் (ctx->errcode)
				திரும்பு BLOCK_ABORT;
		} இல்லையெனில் {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	எனில் (do_abort)
		திரும்பு retval | BLOCK_ABORT;
	திரும்பு retval;
}
