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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#परिभाषय EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     संरचना ext2_dir_प्रविष्टिः *dirent,
			     अचिह्नित पूर्णाङ्क *rec_len)
{
	अचिह्नित पूर्णाङ्क len = dirent->rec_len;

	यदि (fs->blocksize < 65536)
		*rec_len = len;
	अन्यथा यदि (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	अन्यथा 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	प्रत्यागम 0;
}

errcode_t ext2fs_समुच्चयः_rec_len(ext2_filsys fs,
			     अचिह्नित पूर्णाङ्क len,
			     संरचना ext2_dir_प्रविष्टिः *dirent)
{
	यदि ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		प्रत्यागम EINVAL;
	यदि (len < 65536) {
		dirent->rec_len = len;
		प्रत्यागम 0;
	}
	यदि (len == fs->blocksize) {
		यदि (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		अन्यथा 
			dirent->rec_len = 0;
	} अन्यथा
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	प्रत्यागम 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
स्थिर पूर्णाङ्क ext2fs_validate_प्रविष्टिः(ext2_filsys fs, वर्ण *buf,
				 अचिह्नित पूर्णाङ्क विस्थापनम्,
				 अचिह्नित पूर्णाङ्क final_विस्थापनम्)
{
	संरचना ext2_dir_प्रविष्टिः *dirent;
	अचिह्नित पूर्णाङ्क rec_len;
#परिभाषय DIRENT_MIN_दीर्घता 12

	यावत् ((विस्थापनम् < final_विस्थापनम्) &&
	       (विस्थापनम् <= fs->blocksize - DIRENT_MIN_दीर्घता)) {
		dirent = (संरचना ext2_dir_प्रविष्टिः *)(buf + विस्थापनम्);
		यदि (ext2fs_get_rec_len(fs, dirent, &rec_len))
			प्रत्यागम 0;
		विस्थापनम् += rec_len;
		यदि ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (पूर्णाङ्क) rec_len))
			प्रत्यागम 0;
	}
	प्रत्यागम (विस्थापनम् == final_विस्थापनम्);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      पूर्णाङ्क flags,
			      वर्ण *खण्डम्_buf,
			      पूर्णाङ्क (*func)(ext2_ino_t	dir,
					  पूर्णाङ्क		प्रविष्टिः,
					  संरचना ext2_dir_प्रविष्टिः *dirent,
					  पूर्णाङ्क	विस्थापनम्,
					  पूर्णाङ्क	blocksize,
					  वर्ण	*buf,
					  शून्य	*priv_दत्त),
			      शून्य *priv_दत्त)
{
	संरचना		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	यदि (retval)
		प्रत्यागम retval;

	ctx.dir = dir;
	ctx.flags = flags;
	यदि (खण्डम्_buf)
		ctx.buf = खण्डम्_buf;
	अन्यथा {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		यदि (retval)
			प्रत्यागम retval;
	}
	ctx.func = func;
	ctx.priv_दत्त = priv_दत्त;
	ctx.errcode = 0;
	retval = ext2fs_खण्डम्_iterate3(fs, dir, खण्डम्_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_खण्डम्, &ctx);
	यदि (!खण्डम्_buf)
		ext2fs_free_mem(&ctx.buf);
	यदि (retval == EXT2_ET_INLINE_दत्त_CANT_ITERATE) {
		(शून्य) ext2fs_inline_दत्त_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	यदि (retval)
		प्रत्यागम retval;
	प्रत्यागम ctx.errcode;
}

संरचना xlate {
	पूर्णाङ्क (*func)(संरचना ext2_dir_प्रविष्टिः *dirent,
		    पूर्णाङ्क		विस्थापनम्,
		    पूर्णाङ्क		blocksize,
		    वर्ण	*buf,
		    शून्य	*priv_दत्त);
	शून्य *real_वैयक्तिकम्;
};

स्थिर पूर्णाङ्क xlate_func(ext2_ino_t dir EXT2FS_ATTR((अप्रयुक्तम्)),
		      पूर्णाङ्क प्रविष्टिः EXT2FS_ATTR((अप्रयुक्तम्)),
		      संरचना ext2_dir_प्रविष्टिः *dirent, पूर्णाङ्क विस्थापनम्,
		      पूर्णाङ्क blocksize, वर्ण *buf, शून्य *priv_दत्त)
{
	संरचना xlate *xl = (संरचना xlate *) priv_दत्त;

	प्रत्यागम (*xl->func)(dirent, विस्थापनम्, blocksize, buf, xl->real_वैयक्तिकम्);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     पूर्णाङ्क flags,
			     वर्ण *खण्डम्_buf,
			     पूर्णाङ्क (*func)(संरचना ext2_dir_प्रविष्टिः *dirent,
					 पूर्णाङ्क	विस्थापनम्,
					 पूर्णाङ्क	blocksize,
					 वर्ण	*buf,
					 शून्य	*priv_दत्त),
			     शून्य *priv_दत्त)
{
	संरचना xlate xl;

	xl.real_वैयक्तिकम् = priv_दत्त;
	xl.func = func;

	प्रत्यागम ext2fs_dir_iterate2(fs, dir, flags, खण्डम्_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
पूर्णाङ्क ext2fs_process_dir_खण्डम्(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_खण्डम् EXT2FS_ATTR((अप्रयुक्तम्)),
			     पूर्णाङ्क	ref_विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
			     शून्य	*priv_दत्त)
{
	संरचना dir_context *ctx = (संरचना dir_context *) priv_दत्त;
	अचिह्नित पूर्णाङ्क	विस्थापनम् = 0;
	अचिह्नित पूर्णाङ्क	अग्र_real_प्रविष्टिः = 0;
	पूर्णाङ्क		ret = 0;
	पूर्णाङ्क		परिवर्तितम् = 0;
	पूर्णाङ्क		do_abort = 0;
	अचिह्नित पूर्णाङ्क	rec_len, आकार, buflen;
	पूर्णाङ्क		प्रविष्टिः;
	संरचना ext2_dir_प्रविष्टिः *dirent;
	पूर्णाङ्क		csum_आकार = 0;
	पूर्णाङ्क		inline_दत्त;
	errcode_t	retval = 0;

	यदि (blockcnt < 0)
		प्रत्यागम 0;

	प्रविष्टिः = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_दत्त = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_दत्त);
	यदि (!inline_दत्त) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		यदि (ctx->errcode)
			प्रत्यागम खण्डम्_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} अन्यथा {
		buflen = ctx->buflen;
	}

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ);

	यदि (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		प्रत्यागम खण्डम्_ABORT;
	}
	यावत् (विस्थापनम् < buflen - 8) {
		dirent = (संरचना ext2_dir_प्रविष्टिः *) (ctx->buf + विस्थापनम्);
		यदि (ext2fs_get_rec_len(fs, dirent, &rec_len))
			प्रत्यागम खण्डम्_ABORT;
		यदि (((विस्थापनम् + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (पूर्णाङ्क) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			प्रत्यागम खण्डम्_ABORT;
		}
		यदि (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			यदि (!inline_दत्त &&
			    (विस्थापनम् == buflen - csum_आकार) &&
			    (dirent->rec_len == csum_आकार) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				यदि (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					गच्छ अग्र;
				प्रविष्टिः = DIRENT_CHECKSUM;
			} अन्यथा यदि (!(ctx->flags & DIRENT_FLAG_INCLUDE_रिक्तानि))
				गच्छ अग्र;
		}

		ret = (ctx->func)(ctx->dir,
				  (अग्र_real_प्रविष्टिः > विस्थापनम्) ?
				  DIRENT_DELETED_FILE : प्रविष्टिः,
				  dirent, विस्थापनम्,
				  buflen, ctx->buf,
				  ctx->priv_दत्त);
		यदि (प्रविष्टिः < DIRENT_OTHER_FILE)
			प्रविष्टिः++;

		यदि (ret & DIRENT_परिवर्तितम्) {
			यदि (ext2fs_get_rec_len(fs, dirent, &rec_len))
				प्रत्यागम खण्डम्_ABORT;
			परिवर्तितम्++;
		}
		यदि (ret & DIRENT_ABORT) {
			do_abort++;
			भङ्ग;
		}
अग्र:
 		यदि (अग्र_real_प्रविष्टिः == विस्थापनम्)
			अग्र_real_प्रविष्टिः += rec_len;

 		यदि (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			आकार = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			यदि (rec_len != आकार)  {
				अचिह्नित पूर्णाङ्क final_विस्थापनम्;

				final_विस्थापनम् = विस्थापनम् + rec_len;
				विस्थापनम् += आकार;
				यावत् (विस्थापनम् < final_विस्थापनम् &&
				       !ext2fs_validate_प्रविष्टिः(fs, ctx->buf,
							      विस्थापनम्,
							      final_विस्थापनम्))
					विस्थापनम् += 4;
				अनुवर्त;
			}
		}
		विस्थापनम् += rec_len;
	}

	यदि (परिवर्तितम्) {
		यदि (!inline_दत्त) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			यदि (ctx->errcode)
				प्रत्यागम खण्डम्_ABORT;
		} अन्यथा {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = खण्डम्_INLINE_दत्त_परिवर्तितम्;
		}
	}
	यदि (do_abort)
		प्रत्यागम retval | खण्डम्_ABORT;
	प्रत्यागम retval;
}
