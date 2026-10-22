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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     ഘടന ext2_dir_entry *dirent,
			     unsigned പൂർണ്ണസംഖ്യ *rec_len)
{
	unsigned പൂർണ്ണസംഖ്യ len = dirent->rec_len;

	എങ്കിൽ (fs->blocksize < 65536)
		*rec_len = len;
	അല്ലെങ്കിൽ എങ്കിൽ (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	അല്ലെങ്കിൽ 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	തിരികെ 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned പൂർണ്ണസംഖ്യ len,
			     ഘടന ext2_dir_entry *dirent)
{
	എങ്കിൽ ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		തിരികെ EINVAL;
	എങ്കിൽ (len < 65536) {
		dirent->rec_len = len;
		തിരികെ 0;
	}
	എങ്കിൽ (len == fs->blocksize) {
		എങ്കിൽ (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		അല്ലെങ്കിൽ 
			dirent->rec_len = 0;
	} അല്ലെങ്കിൽ
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	തിരികെ 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ ext2fs_validate_entry(ext2_filsys fs, അക്ഷരം *buf,
				 unsigned പൂർണ്ണസംഖ്യ offset,
				 unsigned പൂർണ്ണസംഖ്യ final_offset)
{
	ഘടന ext2_dir_entry *dirent;
	unsigned പൂർണ്ണസംഖ്യ rec_len;
#define DIRENT_MIN_LENGTH 12

	ആകുമ്പോൾ ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (ഘടന ext2_dir_entry *)(buf + offset);
		എങ്കിൽ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			തിരികെ 0;
		offset += rec_len;
		എങ്കിൽ ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (പൂർണ്ണസംഖ്യ) rec_len))
			തിരികെ 0;
	}
	തിരികെ (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      പൂർണ്ണസംഖ്യ flags,
			      അക്ഷരം *block_buf,
			      പൂർണ്ണസംഖ്യ (*func)(ext2_ino_t	dir,
					  പൂർണ്ണസംഖ്യ		entry,
					  ഘടന ext2_dir_entry *dirent,
					  പൂർണ്ണസംഖ്യ	offset,
					  പൂർണ്ണസംഖ്യ	blocksize,
					  അക്ഷരം	*buf,
					  ശൂന്യം	*priv_data),
			      ശൂന്യം *priv_data)
{
	ഘടന		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	എങ്കിൽ (retval)
		തിരികെ retval;

	ctx.dir = dir;
	ctx.flags = flags;
	എങ്കിൽ (block_buf)
		ctx.buf = block_buf;
	അല്ലെങ്കിൽ {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	എങ്കിൽ (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	എങ്കിൽ (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(ശൂന്യം) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	എങ്കിൽ (retval)
		തിരികെ retval;
	തിരികെ ctx.errcode;
}

ഘടന xlate {
	പൂർണ്ണസംഖ്യ (*func)(ഘടന ext2_dir_entry *dirent,
		    പൂർണ്ണസംഖ്യ		offset,
		    പൂർണ്ണസംഖ്യ		blocksize,
		    അക്ഷരം	*buf,
		    ശൂന്യം	*priv_data);
	ശൂന്യം *real_private;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      പൂർണ്ണസംഖ്യ entry EXT2FS_ATTR((unused)),
		      ഘടന ext2_dir_entry *dirent, പൂർണ്ണസംഖ്യ offset,
		      പൂർണ്ണസംഖ്യ blocksize, അക്ഷരം *buf, ശൂന്യം *priv_data)
{
	ഘടന xlate *xl = (ഘടന xlate *) priv_data;

	തിരികെ (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     പൂർണ്ണസംഖ്യ flags,
			     അക്ഷരം *block_buf,
			     പൂർണ്ണസംഖ്യ (*func)(ഘടന ext2_dir_entry *dirent,
					 പൂർണ്ണസംഖ്യ	offset,
					 പൂർണ്ണസംഖ്യ	blocksize,
					 അക്ഷരം	*buf,
					 ശൂന്യം	*priv_data),
			     ശൂന്യം *priv_data)
{
	ഘടന xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	തിരികെ ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
പൂർണ്ണസംഖ്യ ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     പൂർണ്ണസംഖ്യ	ref_offset EXT2FS_ATTR((unused)),
			     ശൂന്യം	*priv_data)
{
	ഘടന dir_context *ctx = (ഘടന dir_context *) priv_data;
	unsigned പൂർണ്ണസംഖ്യ	offset = 0;
	unsigned പൂർണ്ണസംഖ്യ	next_real_entry = 0;
	പൂർണ്ണസംഖ്യ		ret = 0;
	പൂർണ്ണസംഖ്യ		changed = 0;
	പൂർണ്ണസംഖ്യ		do_abort = 0;
	unsigned പൂർണ്ണസംഖ്യ	rec_len, size, buflen;
	പൂർണ്ണസംഖ്യ		entry;
	ഘടന ext2_dir_entry *dirent;
	പൂർണ്ണസംഖ്യ		csum_size = 0;
	പൂർണ്ണസംഖ്യ		inline_data;
	errcode_t	retval = 0;

	എങ്കിൽ (blockcnt < 0)
		തിരികെ 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	എങ്കിൽ (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		എങ്കിൽ (ctx->errcode)
			തിരികെ BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} അല്ലെങ്കിൽ {
		buflen = ctx->buflen;
	}

	എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = വലുപ്പം(ഘടന ext2_dir_entry_tail);

	എങ്കിൽ (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		തിരികെ BLOCK_ABORT;
	}
	ആകുമ്പോൾ (offset < buflen - 8) {
		dirent = (ഘടന ext2_dir_entry *) (ctx->buf + offset);
		എങ്കിൽ (ext2fs_get_rec_len(fs, dirent, &rec_len))
			തിരികെ BLOCK_ABORT;
		എങ്കിൽ (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (പൂർണ്ണസംഖ്യ) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			തിരികെ BLOCK_ABORT;
		}
		എങ്കിൽ (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			എങ്കിൽ (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				എങ്കിൽ (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					പോകുക next;
				entry = DIRENT_CHECKSUM;
			} അല്ലെങ്കിൽ എങ്കിൽ (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				പോകുക next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		എങ്കിൽ (entry < DIRENT_OTHER_FILE)
			entry++;

		എങ്കിൽ (ret & DIRENT_CHANGED) {
			എങ്കിൽ (ext2fs_get_rec_len(fs, dirent, &rec_len))
				തിരികെ BLOCK_ABORT;
			changed++;
		}
		എങ്കിൽ (ret & DIRENT_ABORT) {
			do_abort++;
			നിർത്തുക;
		}
next:
 		എങ്കിൽ (next_real_entry == offset)
			next_real_entry += rec_len;

 		എങ്കിൽ (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			എങ്കിൽ (rec_len != size)  {
				unsigned പൂർണ്ണസംഖ്യ final_offset;

				final_offset = offset + rec_len;
				offset += size;
				ആകുമ്പോൾ (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				തുടരുക;
			}
		}
		offset += rec_len;
	}

	എങ്കിൽ (changed) {
		എങ്കിൽ (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			എങ്കിൽ (ctx->errcode)
				തിരികെ BLOCK_ABORT;
		} അല്ലെങ്കിൽ {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	എങ്കിൽ (do_abort)
		തിരികെ retval | BLOCK_ABORT;
	തിരികെ retval;
}
