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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     संरचना ext2_dir_entry *dirent,
			     अचिन्हित पूर्णांक *rec_len)
{
	अचिन्हित पूर्णांक len = dirent->rec_len;

	जर (fs->blocksize < 65536)
		*rec_len = len;
	अन्यथा जर (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	अन्यथा 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	परत 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     अचिन्हित पूर्णांक len,
			     संरचना ext2_dir_entry *dirent)
{
	जर ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		परत EINVAL;
	जर (len < 65536) {
		dirent->rec_len = len;
		परत 0;
	}
	जर (len == fs->blocksize) {
		जर (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		अन्यथा 
			dirent->rec_len = 0;
	} अन्यथा
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	परत 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
स्थिर पूर्णांक ext2fs_validate_entry(ext2_filsys fs, अक्षर *buf,
				 अचिन्हित पूर्णांक offset,
				 अचिन्हित पूर्णांक final_offset)
{
	संरचना ext2_dir_entry *dirent;
	अचिन्हित पूर्णांक rec_len;
#define DIRENT_MIN_LENGTH 12

	पर्यंत ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_LENGTH)) {
		dirent = (संरचना ext2_dir_entry *)(buf + offset);
		जर (ext2fs_get_rec_len(fs, dirent, &rec_len))
			परत 0;
		offset += rec_len;
		जर ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (पूर्णांक) rec_len))
			परत 0;
	}
	परत (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णांक flags,
			      अक्षर *block_buf,
			      पूर्णांक (*func)(ext2_ino_t	dir,
					  पूर्णांक		entry,
					  संरचना ext2_dir_entry *dirent,
					  पूर्णांक	offset,
					  पूर्णांक	blocksize,
					  अक्षर	*buf,
					  शून्य	*priv_data),
			      शून्य *priv_data)
{
	संरचना		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	जर (retval)
		परत retval;

	ctx.dir = dir;
	ctx.flags = flags;
	जर (block_buf)
		ctx.buf = block_buf;
	अन्यथा {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		जर (retval)
			परत retval;
	}
	ctx.func = func;
	ctx.priv_data = priv_data;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	जर (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	जर (retval == EXT2_ET_INLINE_DATA_CANT_ITERATE) {
		(शून्य) ext2fs_inline_data_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	जर (retval)
		परत retval;
	परत ctx.errcode;
}

संरचना xlate {
	पूर्णांक (*func)(संरचना ext2_dir_entry *dirent,
		    पूर्णांक		offset,
		    पूर्णांक		blocksize,
		    अक्षर	*buf,
		    शून्य	*priv_data);
	शून्य *real_private;
};

स्थिर पूर्णांक xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      पूर्णांक entry EXT2FS_ATTR((unused)),
		      संरचना ext2_dir_entry *dirent, पूर्णांक offset,
		      पूर्णांक blocksize, अक्षर *buf, शून्य *priv_data)
{
	संरचना xlate *xl = (संरचना xlate *) priv_data;

	परत (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     पूर्णांक flags,
			     अक्षर *block_buf,
			     पूर्णांक (*func)(संरचना ext2_dir_entry *dirent,
					 पूर्णांक	offset,
					 पूर्णांक	blocksize,
					 अक्षर	*buf,
					 शून्य	*priv_data),
			     शून्य *priv_data)
{
	संरचना xlate xl;

	xl.real_private = priv_data;
	xl.func = func;

	परत ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
पूर्णांक ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     पूर्णांक	ref_offset EXT2FS_ATTR((unused)),
			     शून्य	*priv_data)
{
	संरचना dir_context *ctx = (संरचना dir_context *) priv_data;
	अचिन्हित पूर्णांक	offset = 0;
	अचिन्हित पूर्णांक	next_real_entry = 0;
	पूर्णांक		ret = 0;
	पूर्णांक		changed = 0;
	पूर्णांक		do_abort = 0;
	अचिन्हित पूर्णांक	rec_len, size, buflen;
	पूर्णांक		entry;
	संरचना ext2_dir_entry *dirent;
	पूर्णांक		csum_size = 0;
	पूर्णांक		inline_data;
	errcode_t	retval = 0;

	जर (blockcnt < 0)
		परत 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_data = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_DATA);
	जर (!inline_data) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		जर (ctx->errcode)
			परत BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} अन्यथा {
		buflen = ctx->buflen;
	}

	जर (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = आकार(संरचना ext2_dir_entry_tail);

	जर (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		परत BLOCK_ABORT;
	}
	पर्यंत (offset < buflen - 8) {
		dirent = (संरचना ext2_dir_entry *) (ctx->buf + offset);
		जर (ext2fs_get_rec_len(fs, dirent, &rec_len))
			परत BLOCK_ABORT;
		जर (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (पूर्णांक) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			परत BLOCK_ABORT;
		}
		जर (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			जर (!inline_data &&
			    (offset == buflen - csum_size) &&
			    (dirent->rec_len == csum_size) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				जर (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					जा next;
				entry = DIRENT_CHECKSUM;
			} अन्यथा जर (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				जा next;
		}

		ret = (ctx->func)(ctx->dir,
				  (next_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_data);
		जर (entry < DIRENT_OTHER_FILE)
			entry++;

		जर (ret & DIRENT_CHANGED) {
			जर (ext2fs_get_rec_len(fs, dirent, &rec_len))
				परत BLOCK_ABORT;
			changed++;
		}
		जर (ret & DIRENT_ABORT) {
			do_abort++;
			थांबा;
		}
next:
 		जर (next_real_entry == offset)
			next_real_entry += rec_len;

 		जर (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			size = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			जर (rec_len != size)  {
				अचिन्हित पूर्णांक final_offset;

				final_offset = offset + rec_len;
				offset += size;
				पर्यंत (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				सुरूठेवा;
			}
		}
		offset += rec_len;
	}

	जर (changed) {
		जर (!inline_data) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			जर (ctx->errcode)
				परत BLOCK_ABORT;
		} अन्यथा {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_DATA_CHANGED;
		}
	}
	जर (do_abort)
		परत retval | BLOCK_ABORT;
	परत retval;
}
