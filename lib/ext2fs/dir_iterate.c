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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"

#define EXT4_MAX_REC_LEN		((1<<16)-1)

errcode_t ext2fs_get_rec_len(ext2_filsys fs,
			     কাঠামো ext2_dir_entry *dirent,
			     unsigned পূর্ণসংখ্যা *rec_len)
{
	unsigned পূর্ণসংখ্যা len = dirent->rec_len;

	যদি (fs->blocksize < 65536)
		*rec_len = len;
	অন্যথা যদি (len == EXT4_MAX_REC_LEN || len == 0)
		*rec_len = fs->blocksize;
	অন্যথা 
		*rec_len = (len & 65532) | ((len & 3) << 16);
	ফেরত 0;
}

errcode_t ext2fs_set_rec_len(ext2_filsys fs,
			     unsigned পূর্ণসংখ্যা len,
			     কাঠামো ext2_dir_entry *dirent)
{
	যদি ((len > fs->blocksize) || (fs->blocksize > (1 << 18)) || (len & 3))
		ফেরত EINVAL;
	যদি (len < 65536) {
		dirent->rec_len = len;
		ফেরত 0;
	}
	যদি (len == fs->blocksize) {
		যদি (fs->blocksize == 65536)
			dirent->rec_len = EXT4_MAX_REC_LEN;
		অন্যথা 
			dirent->rec_len = 0;
	} অন্যথা
		dirent->rec_len = (len & 65532) | ((len >> 16) & 3);
	ফেরত 0;
}

/*
 * This function checks to see whether or not a potential deleted
 * directory entry looks valid.  What we do is check the deleted entry
 * and each successive entry to make sure that they all look valid and
 * that the last deleted entry ends at the beginning of the next
 * undeleted entry.  Returns 1 if the deleted entry looks valid, zero
 * if not valid.
 */
স্থির পূর্ণসংখ্যা ext2fs_validate_entry(ext2_filsys fs, অক্ষর *buf,
				 unsigned পূর্ণসংখ্যা offset,
				 unsigned পূর্ণসংখ্যা final_offset)
{
	কাঠামো ext2_dir_entry *dirent;
	unsigned পূর্ণসংখ্যা rec_len;
#define DIRENT_MIN_দৈর্ঘ্য 12

	যখন ((offset < final_offset) &&
	       (offset <= fs->blocksize - DIRENT_MIN_দৈর্ঘ্য)) {
		dirent = (কাঠামো ext2_dir_entry *)(buf + offset);
		যদি (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ফেরত 0;
		offset += rec_len;
		যদি ((rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (পূর্ণসংখ্যা) rec_len))
			ফেরত 0;
	}
	ফেরত (offset == final_offset);
}

errcode_t ext2fs_dir_iterate2(ext2_filsys fs,
			      ext2_ino_t dir,
			      পূর্ণসংখ্যা flags,
			      অক্ষর *block_buf,
			      পূর্ণসংখ্যা (*func)(ext2_ino_t	dir,
					  পূর্ণসংখ্যা		entry,
					  কাঠামো ext2_dir_entry *dirent,
					  পূর্ণসংখ্যা	offset,
					  পূর্ণসংখ্যা	blocksize,
					  অক্ষর	*buf,
					  শূন্য	*priv_তথ্য),
			      শূন্য *priv_তথ্য)
{
	কাঠামো		dir_context	ctx;
	errcode_t	retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_check_directory(fs, dir);
	যদি (retval)
		ফেরত retval;

	ctx.dir = dir;
	ctx.flags = flags;
	যদি (block_buf)
		ctx.buf = block_buf;
	অন্যথা {
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		যদি (retval)
			ফেরত retval;
	}
	ctx.func = func;
	ctx.priv_তথ্য = priv_তথ্য;
	ctx.errcode = 0;
	retval = ext2fs_block_iterate3(fs, dir, BLOCK_FLAG_READ_ONLY, 0,
				       ext2fs_process_dir_block, &ctx);
	যদি (!block_buf)
		ext2fs_free_mem(&ctx.buf);
	যদি (retval == EXT2_ET_INLINE_তথ্য_CANT_ITERATE) {
		(শূন্য) ext2fs_inline_তথ্য_dir_iterate(fs, dir, &ctx);
		retval = 0;
	}
	যদি (retval)
		ফেরত retval;
	ফেরত ctx.errcode;
}

কাঠামো xlate {
	পূর্ণসংখ্যা (*func)(কাঠামো ext2_dir_entry *dirent,
		    পূর্ণসংখ্যা		offset,
		    পূর্ণসংখ্যা		blocksize,
		    অক্ষর	*buf,
		    শূন্য	*priv_তথ্য);
	শূন্য *real_private;
};

স্থির পূর্ণসংখ্যা xlate_func(ext2_ino_t dir EXT2FS_ATTR((unused)),
		      পূর্ণসংখ্যা entry EXT2FS_ATTR((unused)),
		      কাঠামো ext2_dir_entry *dirent, পূর্ণসংখ্যা offset,
		      পূর্ণসংখ্যা blocksize, অক্ষর *buf, শূন্য *priv_তথ্য)
{
	কাঠামো xlate *xl = (কাঠামো xlate *) priv_তথ্য;

	ফেরত (*xl->func)(dirent, offset, blocksize, buf, xl->real_private);
}

errcode_t ext2fs_dir_iterate(ext2_filsys fs,
			     ext2_ino_t dir,
			     পূর্ণসংখ্যা flags,
			     অক্ষর *block_buf,
			     পূর্ণসংখ্যা (*func)(কাঠামো ext2_dir_entry *dirent,
					 পূর্ণসংখ্যা	offset,
					 পূর্ণসংখ্যা	blocksize,
					 অক্ষর	*buf,
					 শূন্য	*priv_তথ্য),
			     শূন্য *priv_তথ্য)
{
	কাঠামো xlate xl;

	xl.real_private = priv_তথ্য;
	xl.func = func;

	ফেরত ext2fs_dir_iterate2(fs, dir, flags, block_buf,
				   xlate_func, &xl);
}


/*
 * Helper function which is private to this module.  Used by
 * ext2fs_dir_iterate() and ext2fs_dblist_dir_iterate()
 */
পূর্ণসংখ্যা ext2fs_process_dir_block(ext2_filsys fs,
			     blk64_t	*blocknr,
			     e2_blkcnt_t blockcnt,
			     blk64_t	ref_block EXT2FS_ATTR((unused)),
			     পূর্ণসংখ্যা	ref_offset EXT2FS_ATTR((unused)),
			     শূন্য	*priv_তথ্য)
{
	কাঠামো dir_context *ctx = (কাঠামো dir_context *) priv_তথ্য;
	unsigned পূর্ণসংখ্যা	offset = 0;
	unsigned পূর্ণসংখ্যা	পরবর্তী_real_entry = 0;
	পূর্ণসংখ্যা		ret = 0;
	পূর্ণসংখ্যা		changed = 0;
	পূর্ণসংখ্যা		do_abort = 0;
	unsigned পূর্ণসংখ্যা	rec_len, আকার_2, buflen;
	পূর্ণসংখ্যা		entry;
	কাঠামো ext2_dir_entry *dirent;
	পূর্ণসংখ্যা		csum_আকার = 0;
	পূর্ণসংখ্যা		inline_তথ্য;
	errcode_t	retval = 0;

	যদি (blockcnt < 0)
		ফেরত 0;

	entry = blockcnt ? DIRENT_OTHER_FILE : DIRENT_DOT_FILE;

	/* If a dir has inline data, we don't need to read block */
	inline_তথ্য = !!(ctx->flags & DIRENT_FLAG_INCLUDE_INLINE_তথ্য);
	যদি (!inline_তথ্য) {
		ctx->errcode = ext2fs_read_dir_block4(fs, *blocknr, ctx->buf, 0,
						      ctx->dir);
		যদি (ctx->errcode)
			ফেরত BLOCK_ABORT;
		/* If we handle a normal dir, we traverse the entire block */
		buflen = fs->blocksize;
	} অন্যথা {
		buflen = ctx->buflen;
	}

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		csum_আকার = আকার(কাঠামো ext2_dir_entry_লেজ);

	যদি (buflen < 8) {
		ctx->errcode = EXT2_ET_DIR_CORRUPTED;
		ফেরত BLOCK_ABORT;
	}
	যখন (offset < buflen - 8) {
		dirent = (কাঠামো ext2_dir_entry *) (ctx->buf + offset);
		যদি (ext2fs_get_rec_len(fs, dirent, &rec_len))
			ফেরত BLOCK_ABORT;
		যদি (((offset + rec_len) > buflen) ||
		    (rec_len < 8) ||
		    ((rec_len % 4) != 0) ||
		    ((ext2fs_dirent_name_len(dirent)+8) > (পূর্ণসংখ্যা) rec_len)) {
			ctx->errcode = EXT2_ET_DIR_CORRUPTED;
			ফেরত BLOCK_ABORT;
		}
		যদি (!dirent->inode) {
			/*
			 * We just need to check metadata_csum when this
			 * dir hasn't inline data.  That means that 'buflen'
			 * should be blocksize.
			 */
			যদি (!inline_তথ্য &&
			    (offset == buflen - csum_আকার) &&
			    (dirent->rec_len == csum_আকার) &&
			    (dirent->name_len == EXT2_DIR_NAME_LEN_CSUM)) {
				যদি (!(ctx->flags & DIRENT_FLAG_INCLUDE_CSUM))
					যাও পরবর্তী;
				entry = DIRENT_CHECKSUM;
			} অন্যথা যদি (!(ctx->flags & DIRENT_FLAG_INCLUDE_EMPTY))
				যাও পরবর্তী;
		}

		ret = (ctx->func)(ctx->dir,
				  (পরবর্তী_real_entry > offset) ?
				  DIRENT_DELETED_FILE : entry,
				  dirent, offset,
				  buflen, ctx->buf,
				  ctx->priv_তথ্য);
		যদি (entry < DIRENT_OTHER_FILE)
			entry++;

		যদি (ret & DIRENT_CHANGED) {
			যদি (ext2fs_get_rec_len(fs, dirent, &rec_len))
				ফেরত BLOCK_ABORT;
			changed++;
		}
		যদি (ret & DIRENT_ABORT) {
			do_abort++;
			বিরতি;
		}
পরবর্তী:
 		যদি (পরবর্তী_real_entry == offset)
			পরবর্তী_real_entry += rec_len;

 		যদি (ctx->flags & DIRENT_FLAG_INCLUDE_REMOVED) {
			আকার_2 = (ext2fs_dirent_name_len(dirent) + 11) & ~3;

			যদি (rec_len != আকার_2)  {
				unsigned পূর্ণসংখ্যা final_offset;

				final_offset = offset + rec_len;
				offset += আকার_2;
				যখন (offset < final_offset &&
				       !ext2fs_validate_entry(fs, ctx->buf,
							      offset,
							      final_offset))
					offset += 4;
				চালু;
			}
		}
		offset += rec_len;
	}

	যদি (changed) {
		যদি (!inline_তথ্য) {
			ctx->errcode = ext2fs_write_dir_block4(fs, *blocknr,
							       ctx->buf,
							       0, ctx->dir);
			যদি (ctx->errcode)
				ফেরত BLOCK_ABORT;
		} অন্যথা {
			/*
			 * return BLOCK_INLINE_DATA_CHANGED to notify caller
			 * that inline data has been changed.
			 */
			retval = BLOCK_INLINE_তথ্য_CHANGED;
		}
	}
	যদি (do_abort)
		ফেরত retval | BLOCK_ABORT;
	ফেরত retval;
}
