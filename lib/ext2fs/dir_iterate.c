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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     સંરચના ext2_dir_entry *dirent,
			     unsigned પૂર્ણાંક *rec_len)
{
	unsigned પૂર્ણાંક len = dirent->rec_len;

	જો (fs->blocksize < 65536)
		*rec_len = len;
	નહીંતર જો (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	નહીંતર 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	પાછા 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned પૂર્ણાંક len,
			     સંરચના ext2_dir_entry *dirent)
{
	જો ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		પાછા EINVAL;
	જો (len < 65536) {
		dirent->rec_len = len;
		પાછા 0;
	}
	જો (len == fs->blocksize) {
		જો (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		નહીંતર 
			dirent->rec_len = 0;
	} નહીંતર
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	પાછા 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
સ્થિર પૂર્ણાંક ext2fs_validate_entry(ext2_filsys fs, અક્ષર *buf,
				 unsigned પૂર્ણાંક offset,
				 unsigned પૂર્ણાંક final_offset)
{
	સંરચના ext2_dir_entry *dirent;
	unsigned પૂર્ણાંક rec_len;
#define DIRENT_MIN_LENGTH 12

	જ્યારે ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (સંરચના ext2_dir_entry *)(buf + offset);
		જો (ext2fs_get_rec_len(fs, dirent, &rec_len))
			પાછા 0;
		offset += rec_len;
		જો ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (પૂર્ણાંક) rec_len))
			પાછા 0;
	}
	પાછા (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      પૂર્ણાંક flags,
			      અક્ષર *block_buf,
			      પૂર્ણાંક (*func)(ext2_ino_t	dir,
					  પૂર્ણાંક		entry,
					  સંરચના ext2_dir_entry *dirent,
					  પૂર્ણાંક	offset,
					  પૂર્ણાંક	blocksize,
					  અક્ષર	*buf,
					  શૂન્ય	*priv_data),
			      શૂન્ય *priv_data)
{
	સંરચના		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	જો (retval)
		પાછા retval;

	ctx.dir = dir;
	ctx.flags = flags;
	જો (block_buf)
		ctx.buf = block_buf;
	નહીંતર {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		જો (retval)
			પાછા retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	જો (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	જો (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(શૂન્ય) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	જો (retval)
		પાછા retval;
	પાછા ctx.errcode;
}

સંરચના xlate {
	પૂર્ણાંક (*func)(સંરચના ext2_dir_entry *dirent,
		    પૂર્ણાંક		offset,
		    પૂર્ણાંક		blocksize,
		    અક્ષર	*buf,
		    શૂન્ય	*priv_data);
	શૂન્ય *real_private;
};

સ્થિર પૂર્ણાંક xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      પૂર્ણાંક entry EXT2FS_ATTR((unused)),
		      સંરચના ext2_dir_entry *dirent, પૂર્ણાંક offset,
		      પૂર્ણાંક blocksize, અક્ષર *buf, શૂન્ય *priv_data)
{
	સંરચના xlate *xl = (સંરચના xlate *) priv_data;

	પાછા (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     પૂર્ણાંક flags,
			     અક્ષર *block_buf,
			     પૂર્ણાંક (*func)(સંરચના ext2_dir_entry *dirent,
					 પૂર્ણાંક	offset,
					 પૂર્ણાંક	blocksize,
					 અક્ષર	*buf,
					 શૂન્ય	*priv_data),
			     શૂન્ય *priv_data)
{
	સંરચના xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	પાછા ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
પૂર્ણાંક ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     પૂર્ણાંક	ref_offset EXT2FS_ATTR((unused)),
			     શૂન્ય	*priv_data)
{
	સંરચના dir_context *ctx = (સંરચના dir_context *) priv_data;
	unsigned પૂર્ણાંક	offset = 0;
	unsigned પૂર્ણાંક	next_real_entry = 0;
	પૂર્ણાંક		ret = 0;
	પૂર્ણાંક		changed = 0;
	પૂર્ણાંક		do_abort = 0;
	unsigned પૂર્ણાંક	rec_len, size, buflen;
	પૂર્ણાંક		entry;
	સંરચના ext2_dir_entry *dirent;
	પૂર્ણાંક		csum_size = 0;
	પૂર્ણાંક		inline_data;
	errcode_t	retval = 0;

	જો (blockcnt < 0)
		પાછા 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	જો (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		જો (ctx->errcode)
			પાછા BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} નહીંતર {
		buflen = ctx->buflen;
	}

	જો (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = કદ(સંરચના ext2_dir_entry_tail);

	જો (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		પાછા BLOCK_ABORT;
	}
	જ્યારે (offset < buflen - 8) {
		dirent = (સંરચના ext2_dir_entry *) (ctx->buf + offset);
		જો (ext2fs_get_rec_len(fs, dirent, &rec_len))
			પાછા BLOCK_ABORT;
		જો (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (પૂર્ણાંક) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			પાછા BLOCK_ABORT;
		}
		જો (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			જો (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				જો (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					જાઓ next;
				entry = DIRENT_CHECKSUM;
			} નહીંતર જો (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				જાઓ next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		જો (entry < DIRENT_OTHER_FILE)
			entry++;

		જો (ret & DIRENT_CHANGED) {
			જો (ext2fs_get_rec_len(fs, dirent, &rec_len))
				પાછા BLOCK_ABORT;
			changed++;
		}
		જો (ret & DIRENT_ABORT) {
			do_abort++;
			તોડો;
		}
next:
 		જો (next_real_entry == offset)
			next_real_entry += rec_len;

 		જો (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			જો (rec_len != size)  {
				unsigned પૂર્ણાંક final_offset;

				final_offset = offset + rec_len;
				offset += size;
				જ્યારે (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				ચાલુ;
			}
		}
		offset += rec_len;
	}

	જો (changed) {
		જો (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			જો (ctx->errcode)
				પાછા BLOCK_ABORT;
		} નહીંતર {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	જો (do_abort)
		પાછા retval | BLOCK_ABORT;
	પાછા retval;
}
