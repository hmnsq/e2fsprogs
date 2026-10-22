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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

#घोषणा EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     काष्ठा ext2_dir_entry *dirent,
			     अचिन्हित पूर्णांक *rec_len)
{
	अचिन्हित पूर्णांक len = dirent->rec_len;

	अगर (fs->blocksize < 65536)
		*rec_len = len;
	अन्यथा अगर (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	अन्यथा 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	वापस 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     अचिन्हित पूर्णांक len,
			     काष्ठा ext2_dir_entry *dirent)
{
	अगर ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		वापस EINVAL;
	अगर (len < 65536) {
		dirent->rec_len = len;
		वापस 0;
	}
	अगर (len == fs->blocksize) {
		अगर (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		अन्यथा 
			dirent->rec_len = 0;
	} अन्यथा
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	वापस 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
अटल पूर्णांक ext2fs_validate_entry(ext2_filsys fs, अक्षर *buf,
				 अचिन्हित पूर्णांक offset,
				 अचिन्हित पूर्णांक final_offset)
{
	काष्ठा ext2_dir_entry *dirent;
	अचिन्हित पूर्णांक rec_len;
#घोषणा DIRENT_MIN_लंबाई 12

	जबतक ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_लंबाई)) {
		dirent = (काष्ठा ext2_dir_entry *)(buf + offset);
		अगर (ext2fs_get_rec_len(fs, dirent, &rec_len))
			वापस 0;
		offset += rec_len;
		अगर ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (पूर्णांक) rec_len))
			वापस 0;
	}
	वापस (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णांक flags,
			      अक्षर *block_buf,
			      पूर्णांक (*func)(ext2_ino_t	dir,
					  पूर्णांक		entry,
					  काष्ठा ext2_dir_entry *dirent,
					  पूर्णांक	offset,
					  पूर्णांक	blocksize,
					  अक्षर	*buf,
					  व्योम	*priv_तथ्य),
			      व्योम *priv_तथ्य)
{
	काष्ठा		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	अगर (retval)
		वापस retval;

	ctx.dir = dir;
	ctx.flags = flags;
	अगर (block_buf)
		ctx.buf = block_buf;
	अन्यथा {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		अगर (retval)
			वापस retval;
	}
	ctx.func = func;
	ctx.priv_तथ्य = priv_तथ्य;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	अगर (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	अगर (retval == EXT2_ET_INLINE_तथ्य_CANT_ITERATE) {
		(व्योम) ext2fs_inline_तथ्य_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	अगर (retval)
		वापस retval;
	वापस ctx.errcode;
}

काष्ठा xlate {
	पूर्णांक (*func)(काष्ठा ext2_dir_entry *dirent,
		    पूर्णांक		offset,
		    पूर्णांक		blocksize,
		    अक्षर	*buf,
		    व्योम	*priv_तथ्य);
	व्योम *real_private;
};

अटल पूर्णांक xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      पूर्णांक entry EXT2FS_ATTR((unused)),
		      काष्ठा ext2_dir_entry *dirent, पूर्णांक offset,
		      पूर्णांक blocksize, अक्षर *buf, व्योम *priv_तथ्य)
{
	काष्ठा xlate *xl = (काष्ठा xlate *) priv_तथ्य;

	वापस (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     पूर्णांक flags,
			     अक्षर *block_buf,
			     पूर्णांक (*func)(काष्ठा ext2_dir_entry *dirent,
					 पूर्णांक	offset,
					 पूर्णांक	blocksize,
					 अक्षर	*buf,
					 व्योम	*priv_तथ्य),
			     व्योम *priv_तथ्य)
{
	काष्ठा xlate xl;

	xl.real_private = priv_तथ्य;
	xl.func = func;

	वापस ext2fs_dir_iterate2(fs, dir, flags, block_buf,
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
			     व्योम	*priv_तथ्य)
{
	काष्ठा dir_context *ctx = (काष्ठा dir_context *) priv_तथ्य;
	अचिन्हित पूर्णांक	offset = 0;
	अचिन्हित पूर्णांक	अग्र_real_entry = 0;
	पूर्णांक		ret = 0;
	पूर्णांक		changed = 0;
	पूर्णांक		do_abort = 0;
	अचिन्हित पूर्णांक	rec_len, आकार, buflen;
	पूर्णांक		entry;
	काष्ठा ext2_dir_entry *dirent;
	पूर्णांक		csum_आकार = 0;
	पूर्णांक		inline_तथ्य;
	errcode_t	retval = 0;

	अगर (blockcnt < 0)
		वापस 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_तथ्य = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_तथ्य);
	अगर (!inline_तथ्य) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		अगर (ctx->errcode)
			वापस BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} अन्यथा {
		buflen = ctx->buflen;
	}

	अगर (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = माप(काष्ठा ext2_dir_entry_पुच्छ);

	अगर (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		वापस BLOCK_ABORT;
	}
	जबतक (offset < buflen - 8) {
		dirent = (काष्ठा ext2_dir_entry *) (ctx->buf + offset);
		अगर (ext2fs_get_rec_len(fs, dirent, &rec_len))
			वापस BLOCK_ABORT;
		अगर (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (पूर्णांक) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			वापस BLOCK_ABORT;
		}
		अगर (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			अगर (!inline_तथ्य &&
			    (offset == buflen - csum_आकार) &&
			    (dirent->rec_len == csum_आकार) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				अगर (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					जाओ अग्र;
				entry = DIRENT_CHECKSUM;
			} अन्यथा अगर (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				जाओ अग्र;
		}

		ret = (ctx->func)(ctx->dir,
				  (अग्र_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_तथ्य);
		अगर (entry < DIRENT_OTHER_FILE)
			entry++;

		अगर (ret & DIRENT_CHANGED) {
			अगर (ext2fs_get_rec_len(fs, dirent, &rec_len))
				वापस BLOCK_ABORT;
			changed++;
		}
		अगर (ret & DIRENT_ABORT) {
			do_abort++;
			अवरोध;
		}
अग्र:
 		अगर (अग्र_real_entry == offset)
			अग्र_real_entry += rec_len;

 		अगर (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			आकार = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			अगर (rec_len != आकार)  {
				अचिन्हित पूर्णांक final_offset;

				final_offset = offset + rec_len;
				offset += आकार;
				जबतक (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				जारी;
			}
		}
		offset += rec_len;
	}

	अगर (changed) {
		अगर (!inline_तथ्य) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			अगर (ctx->errcode)
				वापस BLOCK_ABORT;
		} अन्यथा {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_तथ्य_CHANGED;
		}
	}
	अगर (do_abort)
		वापस retval | BLOCK_ABORT;
	वापस retval;
}
