/*
 * inline_data.c --- data in inode
 *
 * Copyright (C) 2012 Zheng Liu <wenqing.lz@taobao.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU library
 * General Public License, version 2.
 * %End-Header%
 */

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <limits.h> /* for PATH_MAX */

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2_ext_attr.h"

#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

কাঠামো ext2_inline_তথ্য {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_আকার;	/* the size of inline data in ea area */
	শূন্য *ea_তথ্য;
};

স্থির errcode_t ext2fs_inline_তথ্য_ea_set(কাঠামো ext2_inline_তথ্য *তথ্য)
{
	কাঠামো ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(তথ্য->fs, তথ্য->ino, &handle);
	যদি (retval)
		ফেরত retval;

	retval = ext2fs_xattrs_read(handle);
	যদি (retval)
		যাও err;

	retval = ext2fs_xattr_set(handle, "system.data",
				  তথ্য->ea_তথ্য, তথ্য->ea_আকার);
err:
	(শূন্য) ext2fs_xattrs_close(&handle);
	ফেরত retval;
}

স্থির errcode_t ext2fs_inline_তথ্য_ea_get(কাঠামো ext2_inline_তথ্য *তথ্য)
{
	কাঠামো ext2_xattr_handle *handle;
	errcode_t retval;

	তথ্য->ea_আকার = 0;
	তথ্য->ea_তথ্য = 0;

	retval = ext2fs_xattrs_open(তথ্য->fs, তথ্য->ino, &handle);
	যদি (retval)
		ফেরত retval;

	retval = ext2fs_xattrs_read(handle);
	যদি (retval)
		যাও err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (শূন্য **)&তথ্য->ea_তথ্য, &তথ্য->ea_আকার);
	যদি (retval == EXT2_ET_EA_চাবি_NOT_FOUND) {
		তথ্য->ea_আকার = 0;
		তথ্য->ea_তথ্য = ফাঁকা;
		retval = 0;
	} অন্যথা যদি (retval)
		যাও err;

err:
	(শূন্য) ext2fs_xattrs_close(&handle);
	ফেরত retval;
}

errcode_t ext2fs_inline_তথ্য_init(ext2_filsys fs, ext2_ino_t ino)
{
	কাঠামো ext2_inline_তথ্য তথ্য;
	অক্ষর empty[1] = { '\0' };

	তথ্য.fs = fs;
	তথ্য.ino = ino;
	তথ্য.ea_আকার = 0;
	তথ্য.ea_তথ্য = empty;
	ফেরত ext2fs_inline_তথ্য_ea_set(&তথ্য);
}

errcode_t ext2fs_inline_তথ্য_আকার(ext2_filsys fs, ext2_ino_t ino, size_t *আকার_2)
{
	কাঠামো ext2_inode inode;
	কাঠামো ext2_inline_তথ্য তথ্য;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ফেরত retval;

	যদি (!(inode.i_flags & EXT4_INLINE_তথ্য_FL))
		ফেরত EXT2_ET_NO_INLINE_তথ্য;

	তথ্য.fs = fs;
	তথ্য.ino = ino;
	retval = ext2fs_inline_তথ্য_ea_get(&তথ্য);
	যদি (retval)
		ফেরত retval;

	*আকার_2 = EXT4_MIN_INLINE_তথ্য_আকার + তথ্য.ea_আকার;
	ফেরত ext2fs_free_mem(&তথ্য.ea_তথ্য);
}

পূর্ণসংখ্যা ext2fs_inline_তথ্য_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   শূন্য *priv_তথ্য)
{
	কাঠামো dir_context *ctx;
	কাঠামো ext2_inode inode;
	কাঠামো ext2_dir_entry dirent;
	কাঠামো ext2_inline_তথ্য তথ্য;
	পূর্ণসংখ্যা ret = BLOCK_ABORT;
	e2_blkcnt_t blockcnt = 0;
	অক্ষর *old_buf;
	unsigned পূর্ণসংখ্যা old_buflen;
	পূর্ণসংখ্যা old_flags;

	ctx = (কাঠামো dir_context *)priv_তথ্য;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_তথ্য;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	যদি (ctx->errcode)
		যাও out;

	যদি (!(inode.i_flags & EXT4_INLINE_তথ্য_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_তথ্য;
		যাও out;
	}

	যদি (!LINUX_S_ISDIR(inode.i_mode)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		যাও out;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (অক্ষর *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_তথ্য);
	যদি (ret & BLOCK_ABORT)
		যাও out;

	dirent.inode = ext2fs_le32_to_cpu(inode.i_block[0]);
	dirent.name_len = 2;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (অক্ষর *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_তথ্য);
	যদি (ret & BLOCK_INLINE_তথ্য_CHANGED) {
		errcode_t err;

		inode.i_block[0] = ext2fs_cpu_to_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		যদি (err)
			যাও out;
		ret &= ~BLOCK_INLINE_তথ্য_CHANGED;
	}
	যদি (ret & BLOCK_ABORT)
		যাও out;

	ctx->buf = (অক্ষর *)inode.i_block + EXT4_INLINE_তথ্য_DOTDOT_আকার;
	ctx->buflen = EXT4_MIN_INLINE_তথ্য_আকার - EXT4_INLINE_তথ্য_DOTDOT_আকার;
#ifdef শব্দগুলি_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	যদি (ctx->errcode) {
		ret |= BLOCK_ABORT;
		যাও out;
	}
#endif
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_তথ্য);
	যদি (ret & BLOCK_INLINE_তথ্য_CHANGED) {
#ifdef শব্দগুলি_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		যদি (ctx->errcode) {
			ret |= BLOCK_ABORT;
			যাও out;
		}
#endif
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		যদি (ctx->errcode)
			ret |= BLOCK_ABORT;
		ret &= ~BLOCK_INLINE_তথ্য_CHANGED;
	}
	যদি (ret & BLOCK_ABORT)
		যাও out;

	তথ্য.fs = fs;
	তথ্য.ino = ino;
	ctx->errcode = ext2fs_inline_তথ্য_ea_get(&তথ্য);
	যদি (ctx->errcode) {
		ret |= BLOCK_ABORT;
		যাও out;
	}
	যদি (তথ্য.ea_আকার <= 0)
		যাও out1;

	ctx->buf = তথ্য.ea_তথ্য;
	ctx->buflen = তথ্য.ea_আকার;
#ifdef শব্দগুলি_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	যদি (ctx->errcode) {
		ret |= BLOCK_ABORT;
		যাও out1;
	}
#endif

	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_তথ্য);
	যদি (ret & BLOCK_INLINE_তথ্য_CHANGED) {
#ifdef শব্দগুলি_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		যদি (ctx->errcode) {
			ret |= BLOCK_ABORT;
			যাও out1;
		}
#endif
		ctx->errcode = ext2fs_inline_তথ্য_ea_set(&তথ্য);
		যদি (ctx->errcode)
			ret |= BLOCK_ABORT;
	}

out1:
	ext2fs_free_mem(&তথ্য.ea_তথ্য);
out:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(BLOCK_ABORT | BLOCK_INLINE_তথ্য_CHANGED);
	ফেরত ret;
}

errcode_t ext2fs_inline_তথ্য_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	কাঠামো ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	যদি (retval)
		ফেরত retval;

	retval = ext2fs_xattrs_read(handle);
	যদি (retval)
		যাও err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(শূন্য) ext2fs_xattrs_close(&handle);
	ফেরত retval;
}

স্থির errcode_t ext2fs_inline_তথ্য_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						অক্ষর *bbuf, অক্ষর *ibuf, পূর্ণসংখ্যা আকার_2)
{
	কাঠামো ext2_dir_entry *dir, *dir2;
	কাঠামো ext2_dir_entry_লেজ *t;
	errcode_t retval;
	পূর্ণসংখ্যা offset;
	unsigned পূর্ণসংখ্যা rec_len;
	পূর্ণসংখ্যা csum_আকার = 0;
	পূর্ণসংখ্যা filetype = 0;

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		csum_আকার = আকার(কাঠামো ext2_dir_entry_লেজ);

	/* Create '.' and '..' */
	যদি (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (কাঠামো ext2_dir_entry *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_set_name_len(dir, 1);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_আকার) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (কাঠামো ext2_dir_entry *) (bbuf + dir->rec_len);
	dir->rec_len = EXT2_DIR_REC_LEN(2);
	dir->inode = ext2fs_le32_to_cpu(((__u32 *)ibuf)[0]);
	ext2fs_dirent_set_name_len(dir, 2);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	dir->name[1] = '.';

	/*
	 * Adjust the last rec_len
	 */
	offset = EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2);
	dir = (কাঠামো ext2_dir_entry *) (bbuf + offset);
	memcpy(bbuf + offset, ibuf + EXT4_INLINE_তথ্য_DOTDOT_আকার,
	       আকার_2 - EXT4_INLINE_তথ্য_DOTDOT_আকার);
	আকার_2 += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_তথ্য_DOTDOT_আকার;

	করো {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		যদি (retval)
			যাও err;
		offset += rec_len;
		dir = (কাঠামো ext2_dir_entry *) (bbuf + offset);
	} যখন (offset < আকার_2);
	rec_len += fs->blocksize - csum_আকার - offset;
	retval = ext2fs_set_rec_len(fs, rec_len, dir2);
	যদি (retval)
		যাও err;

	যদি (csum_আকার) {
		t = EXT2_DIRENT_লেজ(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_লেজ(fs, t);
	}

err:
	ফেরত retval;
}

স্থির errcode_t
ext2fs_inline_তথ্য_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      কাঠামো ext2_inode *inode, অক্ষর *buf, size_t আকার_2)
{
	errcode_t retval;
	blk64_t blk;
	অক্ষর *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	যদি (retval)
		ফেরত retval;

#ifdef শব্দগুলি_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_তথ্য_DOTDOT_আকার,
					আকার_2, 0);
	যদি (retval)
		যাও errout;
#endif

	/* Adjust the rec_len */
	retval = ext2fs_inline_তথ্য_convert_dir(fs, ino, blk_buf, buf, আকার_2);
	যদি (retval)
		যাও errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	যদি (retval)
		যাও errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	যদি (retval)
		যাও errout;

	/* Update inode */
	যদি (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_তথ্য_FL;
	retval = ext2fs_iblk_add_blocks(fs, inode, 1);
	যদি (retval)
		যাও errout;
	inode->i_আকার = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_SET, 0, 0, &blk);
	যদি (retval)
		যাও errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	যদি (retval)
		যাও errout;
	ext2fs_block_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	ফেরত retval;
}

স্থির errcode_t
ext2fs_inline_তথ্য_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       কাঠামো ext2_inode *inode, অক্ষর *buf, size_t আকার_2)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_block, 0, আকার(inode->i_block));
	যদি (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		যদি (retval)
			ফেরত retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_তথ্য_FL;
	inode->i_আকার = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	যদি (retval)
		ফেরত retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	যদি (retval)
		ফেরত retval;
	retval = ext2fs_file_write(e2_file, buf, আকার_2, 0);
	ext2fs_file_close(e2_file);
	ফেরত retval;
}

errcode_t ext2fs_inline_তথ্য_expand(ext2_filsys fs, ext2_ino_t ino)
{
	কাঠামো ext2_inode inode;
	কাঠামো ext2_inline_তথ্য তথ্য;
	errcode_t retval;
	size_t inline_আকার;
	অক্ষর *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ফেরত retval;

	যদি (!(inode.i_flags & EXT4_INLINE_তথ্য_FL))
		ফেরত EXT2_ET_NO_INLINE_তথ্য;

	তথ্য.fs = fs;
	তথ্য.ino = ino;
	retval = ext2fs_inline_তথ্য_ea_get(&তথ্য);
	যদি (retval)
		ফেরত retval;
	inline_আকার = তথ্য.ea_আকার + EXT4_MIN_INLINE_তথ্য_আকার;
	retval = ext2fs_get_mem(inline_আকার, &inline_buf);
	যদি (retval)
		যাও errout;

	memcpy(inline_buf, (শূন্য *)inode.i_block, EXT4_MIN_INLINE_তথ্য_আকার);
	যদি (তথ্য.ea_আকার > 0) {
		memcpy(inline_buf + EXT4_MIN_INLINE_তথ্য_আকার,
		       তথ্য.ea_তথ্য, তথ্য.ea_আকার);
	}

	memset((শূন্য *)inode.i_block, 0, EXT4_MIN_INLINE_তথ্য_আকার);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	যদি (retval)
		যাও errout;
	retval = ext2fs_inline_তথ্য_ea_remove(fs, ino);
	যদি (retval)
		যাও errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		যাও errout;

	যদি (LINUX_S_ISDIR(inode.i_mode)) {
		retval = ext2fs_inline_তথ্য_dir_expand(fs, ino, &inode,
						inline_buf, inline_আকার);
	} অন্যথা {
		retval = ext2fs_inline_তথ্য_file_expand(fs, ino, &inode,
						inline_buf, inline_আকার);
	}

errout:
	যদি (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&তথ্য.ea_তথ্য);
	ফেরত retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_তথ্য_get(ext2_filsys fs, ext2_ino_t ino,
				 কাঠামো ext2_inode *inode,
				 শূন্য *buf, size_t *আকার_2)
{
	কাঠামো ext2_inode inode_buf;
	কাঠামো ext2_inline_তথ্য তথ্য;
	errcode_t retval;

	যদি (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		যদি (retval)
			ফেরত retval;
		inode = &inode_buf;
	}

	তথ্য.fs = fs;
	তথ্য.ino = ino;
	retval = ext2fs_inline_তথ্য_ea_get(&তথ্য);
	যদি (retval)
		ফেরত retval;

	memcpy(buf, (শূন্য *)inode->i_block, EXT4_MIN_INLINE_তথ্য_আকার);
	যদি (তথ্য.ea_আকার > 0)
		memcpy((অক্ষর *) buf + EXT4_MIN_INLINE_তথ্য_আকার,
		       তথ্য.ea_তথ্য, তথ্য.ea_আকার);

	যদি (আকার_2)
		*আকার_2 = EXT4_MIN_INLINE_তথ্য_আকার + তথ্য.ea_আকার;
	ext2fs_free_mem(&তথ্য.ea_তথ্য);
	ফেরত 0;
}

errcode_t ext2fs_inline_তথ্য_set(ext2_filsys fs, ext2_ino_t ino,
				 কাঠামো ext2_inode *inode,
				 শূন্য *buf, size_t আকার_2)
{
	কাঠামো ext2_inode inode_buf;
	কাঠামো ext2_inline_তথ্য তথ্য = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_আকার, existing_আকার, free_inode_আকার;

	যদি (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		যদি (retval)
			ফেরত retval;
		inode = &inode_buf;
	}

	যদি (আকার_2 <= EXT4_MIN_INLINE_তথ্য_আকার) {
		memcpy((শূন্য *)inode->i_block, buf, আকার_2);
	} অন্যথা {
		retval = ext2fs_xattr_inode_max_আকার(fs, ino, &free_ea_আকার);
		যদি (retval)
			ফেরত retval;

		retval = ext2fs_inline_তথ্য_আকার(fs, ino, &existing_আকার);
		যদি (retval)
			ফেরত retval;

		যদি (existing_আকার < EXT4_MIN_INLINE_তথ্য_আকার) {
			free_inode_আকার = EXT4_MIN_INLINE_তথ্য_আকার -
					  existing_আকার;
		} অন্যথা {
			free_inode_আকার = 0;
		}

		যদি (আকার_2 != existing_আকার &&
		    আকার_2 > existing_আকার + free_ea_আকার + free_inode_আকার)
			ফেরত EXT2_ET_INLINE_তথ্য_NO_SPACE;

		memcpy((শূন্য *)inode->i_block, buf, EXT4_MIN_INLINE_তথ্য_আকার);
		যদি (আকার_2 > EXT4_MIN_INLINE_তথ্য_আকার)
			তথ্য.ea_আকার = আকার_2 - EXT4_MIN_INLINE_তথ্য_আকার;
		তথ্য.ea_তথ্য = (অক্ষর *) buf + EXT4_MIN_INLINE_তথ্য_আকার;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	যদি (retval)
		ফেরত retval;
	ফেরত ext2fs_inline_তথ্য_ea_set(&তথ্য);
}

#ifdef DEBUG
#অন্তর্ভাব "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#define BUFF_আকার (64)

স্থির errcode_t file_test(ext2_filsys fs)
{
	কাঠামো ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t আকার_2;
	অক্ষর *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	যদি (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		ফেরত 1;
	}

	memset(&inode, 0, আকার(inode));
	inode.i_flags |= EXT4_INLINE_তথ্য_FL;
	inode.i_আকার = EXT4_MIN_INLINE_তথ্য_আকার;
	inode.i_mode = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	যদি (retval) {
		com_err("file_test", retval, "while writing a new inode");
		ফেরত 1;
	}

	retval = ext2fs_inline_তথ্য_init(fs, newfile);
	যদি (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		ফেরত 1;
	}

	retval = ext2fs_inline_তথ্য_আকার(fs, newfile, &আকার_2);
	যদি (retval) {
		com_err("file_test", retval, "while getting size");
		ফেরত 1;
	}

	যদি (আকার_2 != EXT4_MIN_INLINE_তথ্য_আকার) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		ফেরত 1;
	}

	ext2fs_get_mem(BUFF_আকার, &buf);
	memset(buf, 'a', BUFF_আকার);
	retval = ext2fs_inline_তথ্য_set(fs, newfile, 0, buf, BUFF_আকার);
	যদি (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		যাও err;
	}

	ext2fs_get_mem(BUFF_আকার, &cmpbuf);
	retval = ext2fs_inline_তথ্য_get(fs, newfile, 0, cmpbuf, &আকার_2);
	যদি (retval) {
		com_err("file_test", retval, "while getting inline data");
		যাও err;
	}

	যদি (আকার_2 != BUFF_আকার) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			আকার_2, BUFF_আকার);
		retval = 1;
		যাও err;
	}

	যদি (memcmp(buf, cmpbuf, BUFF_আকার)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		যাও err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	যদি (retval) {
		com_err("file_test", retval, "while truncating inode");
		যাও err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	যদি (inode.i_আকার != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	যদি (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	যদি (buf)
		ext2fs_free_mem(&buf);
	ফেরত retval;
}

স্থির errcode_t dir_test(ext2_filsys fs)
{
	ধ্রুবক অক্ষর *dot_name = ".";
	ধ্রুবক অক্ষর *stub_name = "stub";
	ধ্রুবক অক্ষর *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	অক্ষর dirname[32];
	পূর্ণসংখ্যা i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	যদি (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		ফেরত retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	যদি (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		ফেরত retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, strlen(parent_name),
			       0, &parent);
	যদি (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		ফেরত retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, strlen(dot_name),
			       0, &tmp);
	যদি (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		ফেরত retval;
	}

	যদি (parent != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		ফেরত 1;
	}

	জন্য (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, আকার(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, dirname);
		যদি (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			ফেরত retval;
		}

		retval = ext2fs_lookup(fs, parent, dirname, strlen(dirname),
				       0, &tmp);
		যদি (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			ফেরত retval;
		}

		যদি (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			ফেরত 1;
		}
	}

	snprintf(dirname, আকার(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, dirname);
	যদি (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		ফেরত retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	যদি (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		ফেরত retval;
	}

	ফেরত 0;
}

পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর *argv[])
{
	ext2_filsys		fs;
	কাঠামো ext2_super_block param;
	errcode_t		retval;

	/* setup */
	initialize_ext2_error_table();

	memset(&param, 0, আকার(param));
	ext2fs_blocks_গুনতি_set(&param, 32768);
	param.s_inodes_গুনতি = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_তথ্য;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_আকার = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	যদি (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}

	retval = ext2fs_allocate_tables(fs);
	যদি (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}

	/* initialize inode cache */
	যদি (!fs->icache) {
		ext2_ino_t first_ino = EXT2_FIRST_INO(fs->super);
		পূর্ণসংখ্যা i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		যদি (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			exit(1);
		}

		/* setup inode cache */
		জন্য (i = 0; i < fs->icache->cache_আকার; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	যদি (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		ফেরত 1;
	}
	printf("tst_inline_data(FILE): OK\n");

	যদি (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		ফেরত 1;
	}
	printf("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	ফেরত 0;
}
#endif
