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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     ସଂରଚନା ext2_dir_entry *dirent,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rec_len)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = dirent->rec_len;

	ଯଦି (fs->blocksize < 65536)
		*rec_len = len;
	ଅନ୍ୟଥା ଯଦି (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	ଅନ୍ୟଥା 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len,
			     ସଂରଚନା ext2_dir_entry *dirent)
{
	ଯଦି ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ଫେରନ୍ତୁ EINVAL;
	ଯଦି (len < 65536) {
		dirent->rec_len = len;
		ଫେରନ୍ତୁ 0;
	}
	ଯଦି (len == fs->blocksize) {
		ଯଦି (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		ଅନ୍ୟଥା 
			dirent->rec_len = 0;
	} ଅନ୍ୟଥା
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ଫେରନ୍ତୁ 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_validate_entry(ext2_filsys fs, ଅକ୍ଷର *buf,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା final_offset)
{
	ସଂରଚନା ext2_dir_entry *dirent;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rec_len;
#define DIRENT_MIN_LENGTH 12

	ଯେତେବେଳେ ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (ସଂରଚନା ext2_dir_entry *)(buf + offset);
		ଯଦି (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ଫେରନ୍ତୁ 0;
		offset += rec_len;
		ଯଦି ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) rec_len))
			ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
			      ଅକ୍ଷର *block_buf,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ext2_ino_t	dir,
					  ପୂର୍ଣ୍ଣସଂଖ୍ୟା		entry,
					  ସଂରଚନା ext2_dir_entry *dirent,
					  ପୂର୍ଣ୍ଣସଂଖ୍ୟା	offset,
					  ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocksize,
					  ଅକ୍ଷର	*buf,
					  ଶୂନ୍ୟ	*priv_data),
			      ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ctx.dir = dir;
	ctx.flags = flags;
	ଯଦି (block_buf)
		ctx.buf = block_buf;
	ଅନ୍ୟଥା {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	ଯଦି (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	ଯଦି (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(ଶୂନ୍ୟ) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଫେରନ୍ତୁ ctx.errcode;
}

ସଂରଚନା xlate {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ସଂରଚନା ext2_dir_entry *dirent,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା		offset,
		    ପୂର୍ଣ୍ଣସଂଖ୍ୟା		blocksize,
		    ଅକ୍ଷର	*buf,
		    ଶୂନ୍ୟ	*priv_data);
	ଶୂନ୍ୟ *real_private;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry EXT2FS_ATTR((unused)),
		      ସଂରଚନା ext2_dir_entry *dirent, ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset,
		      ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize, ଅକ୍ଷର *buf, ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା xlate *xl = (ସଂରଚନା xlate *) priv_data;

	ଫେରନ୍ତୁ (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
			     ଅକ୍ଷର *block_buf,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ସଂରଚନା ext2_dir_entry *dirent,
					 ପୂର୍ଣ୍ଣସଂଖ୍ୟା	offset,
					 ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocksize,
					 ଅକ୍ଷର	*buf,
					 ଶୂନ୍ୟ	*priv_data),
			     ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	ଫେରନ୍ତୁ ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ref_offset EXT2FS_ATTR((unused)),
			     ଶୂନ୍ୟ	*priv_data)
{
	ସଂରଚନା dir_context *ctx = (ସଂରଚନା dir_context *) priv_data;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	offset = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	next_real_entry = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		changed = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		do_abort = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	rec_len, size, buflen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		entry;
	ସଂରଚନା ext2_dir_entry *dirent;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		csum_size = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		inline_data;
	errcode_t	retval = 0;

	ଯଦି (blockcnt < 0)
		ଫେରନ୍ତୁ 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	ଯଦି (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		ଯଦି (ctx->errcode)
			ଫେରନ୍ତୁ BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} ଅନ୍ୟଥା {
		buflen = ctx->buflen;
	}

	ଯଦି (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ଆକାର(ସଂରଚନା ext2_dir_entry_tail);

	ଯଦି (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ଫେରନ୍ତୁ BLOCK_ABORT;
	}
	ଯେତେବେଳେ (offset < buflen - 8) {
		dirent = (ସଂରଚନା ext2_dir_entry *) (ctx->buf + offset);
		ଯଦି (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ଫେରନ୍ତୁ BLOCK_ABORT;
		ଯଦି (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ଫେରନ୍ତୁ BLOCK_ABORT;
		}
		ଯଦି (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			ଯଦି (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				ଯଦି (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					ଯାଅ next;
				entry = DIRENT_CHECKSUM;
			} ଅନ୍ୟଥା ଯଦି (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				ଯାଅ next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		ଯଦି (entry < DIRENT_OTHER_FILE)
			entry++;

		ଯଦି (ret & DIRENT_CHANGED) {
			ଯଦି (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ଫେରନ୍ତୁ BLOCK_ABORT;
			changed++;
		}
		ଯଦି (ret & DIRENT_ABORT) {
			do_abort++;
			ଭାଙ୍ଗ;
		}
next:
 		ଯଦି (next_real_entry == offset)
			next_real_entry += rec_len;

 		ଯଦି (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			ଯଦି (rec_len != size)  {
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା final_offset;

				final_offset = offset + rec_len;
				offset += size;
				ଯେତେବେଳେ (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				ଜାରି;
			}
		}
		offset += rec_len;
	}

	ଯଦି (changed) {
		ଯଦି (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			ଯଦି (ctx->errcode)
				ଫେରନ୍ତୁ BLOCK_ABORT;
		} ଅନ୍ୟଥା {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	ଯଦି (do_abort)
		ଫେରନ୍ତୁ retval | BLOCK_ABORT;
	ଫେରନ୍ତୁ retval;
}
