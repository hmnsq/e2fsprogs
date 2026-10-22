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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#सचे HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     संरचना ext2_dir_entry *dirent,
			     unsigned पूर्णाङ्क *rec_len)
{
	unsigned पूर्णाङ्क len = dirent->rec_len;

	सचे (fs->blocksize < 65536)
		*rec_len = len;
	अञ्ञथा सचे (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	अञ्ञथा 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	पटिनिवत्त 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned पूर्णाङ्क len,
			     संरचना ext2_dir_entry *dirent)
{
	सचे ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		पटिनिवत्त EINVAL;
	सचे (len < 65536) {
		dirent->rec_len = len;
		पटिनिवत्त 0;
	}
	सचे (len == fs->blocksize) {
		सचे (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		अञ्ञथा 
			dirent->rec_len = 0;
	} अञ्ञथा
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	पटिनिवत्त 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
स्थिर पूर्णाङ्क ext2fs_validate_entry(ext2_filsys fs, वर्ण *buf,
				 unsigned पूर्णाङ्क offset,
				 unsigned पूर्णाङ्क final_offset)
{
	संरचना ext2_dir_entry *dirent;
	unsigned पूर्णाङ्क rec_len;
#define DIRENT_MIN_LENGTH 12

	याव ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (संरचना ext2_dir_entry *)(buf + offset);
		सचे (ext2fs_get_rec_len(fs, dirent, &rec_len))
			पटिनिवत्त 0;
		offset += rec_len;
		सचे ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (पूर्णाङ्क) rec_len))
			पटिनिवत्त 0;
	}
	पटिनिवत्त (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णाङ्क flags,
			      वर्ण *block_buf,
			      पूर्णाङ्क (*func)(ext2_ino_t	dir,
					  पूर्णाङ्क		entry,
					  संरचना ext2_dir_entry *dirent,
					  पूर्णाङ्क	offset,
					  पूर्णाङ्क	blocksize,
					  वर्ण	*buf,
					  शून्य	*priv_data),
			      शून्य *priv_data)
{
	संरचना		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	सचे (retval)
		पटिनिवत्त retval;

	ctx.dir = dir;
	ctx.flags = flags;
	सचे (block_buf)
		ctx.buf = block_buf;
	अञ्ञथा {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		सचे (retval)
			पटिनिवत्त retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	सचे (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	सचे (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(शून्य) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	सचे (retval)
		पटिनिवत्त retval;
	पटिनिवत्त ctx.errcode;
}

संरचना xlate {
	पूर्णाङ्क (*func)(संरचना ext2_dir_entry *dirent,
		    पूर्णाङ्क		offset,
		    पूर्णाङ्क		blocksize,
		    वर्ण	*buf,
		    शून्य	*priv_data);
	शून्य *real_private;
};

स्थिर पूर्णाङ्क xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      पूर्णाङ्क entry EXT2FS_ATTR((unused)),
		      संरचना ext2_dir_entry *dirent, पूर्णाङ्क offset,
		      पूर्णाङ्क blocksize, वर्ण *buf, शून्य *priv_data)
{
	संरचना xlate *xl = (संरचना xlate *) priv_data;

	पटिनिवत्त (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     पूर्णाङ्क flags,
			     वर्ण *block_buf,
			     पूर्णाङ्क (*func)(संरचना ext2_dir_entry *dirent,
					 पूर्णाङ्क	offset,
					 पूर्णाङ्क	blocksize,
					 वर्ण	*buf,
					 शून्य	*priv_data),
			     शून्य *priv_data)
{
	संरचना xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	पटिनिवत्त ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
पूर्णाङ्क ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     पूर्णाङ्क	ref_offset EXT2FS_ATTR((unused)),
			     शून्य	*priv_data)
{
	संरचना dir_context *ctx = (संरचना dir_context *) priv_data;
	unsigned पूर्णाङ्क	offset = 0;
	unsigned पूर्णाङ्क	next_real_entry = 0;
	पूर्णाङ्क		ret = 0;
	पूर्णाङ्क		changed = 0;
	पूर्णाङ्क		do_abort = 0;
	unsigned पूर्णाङ्क	rec_len, size, buflen;
	पूर्णाङ्क		entry;
	संरचना ext2_dir_entry *dirent;
	पूर्णाङ्क		csum_size = 0;
	पूर्णाङ्क		inline_data;
	errcode_t	retval = 0;

	सचे (blockcnt < 0)
		पटिनिवत्त 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	सचे (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		सचे (ctx->errcode)
			पटिनिवत्त BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} अञ्ञथा {
		buflen = ctx->buflen;
	}

	सचे (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = परिमाण(संरचना ext2_dir_entry_tail);

	सचे (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		पटिनिवत्त BLOCK_ABORT;
	}
	याव (offset < buflen - 8) {
		dirent = (संरचना ext2_dir_entry *) (ctx->buf + offset);
		सचे (ext2fs_get_rec_len(fs, dirent, &rec_len))
			पटिनिवत्त BLOCK_ABORT;
		सचे (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (पूर्णाङ्क) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			पटिनिवत्त BLOCK_ABORT;
		}
		सचे (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			सचे (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				सचे (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					गच्छ next;
				entry = DIRENT_CHECKSUM;
			} अञ्ञथा सचे (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				गच्छ next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		सचे (entry < DIRENT_OTHER_FILE)
			entry++;

		सचे (ret & DIRENT_CHANGED) {
			सचे (ext2fs_get_rec_len(fs, dirent, &rec_len))
				पटिनिवत्त BLOCK_ABORT;
			changed++;
		}
		सचे (ret & DIRENT_ABORT) {
			do_abort++;
			भङ्ग;
		}
next:
 		सचे (next_real_entry == offset)
			next_real_entry += rec_len;

 		सचे (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			सचे (rec_len != size)  {
				unsigned पूर्णाङ्क final_offset;

				final_offset = offset + rec_len;
				offset += size;
				याव (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				अनुवर्त;
			}
		}
		offset += rec_len;
	}

	सचे (changed) {
		सचे (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			सचे (ctx->errcode)
				पटिनिवत्त BLOCK_ABORT;
		} अञ्ञथा {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	सचे (do_abort)
		पटिनिवत्त retval | BLOCK_ABORT;
	पटिनिवत्त retval;
}
