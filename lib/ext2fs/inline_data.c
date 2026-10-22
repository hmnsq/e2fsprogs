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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <time.h>
#অন্তর্ভাৱ <limits.h> /* for PATH_MAX */

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2_ext_attr.h"

#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

সংরচনা ext2_inline_data {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_size;	/* the size of inline data in ea area */
	শূন্য *ea_data;
};

স্থির errcode_t ext2fs_inline_data_ea_set(সংরচনা ext2_inline_data *data)
{
	সংরচনা ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	যদি (retval)
		ঘূৰাই retval;

	retval = ext2fs_xattrs_read(handle);
	যদি (retval)
		গচ্ছ err;

	retval = ext2fs_xattr_set(handle, "system.data",
				  data->ea_data, data->ea_size);
err:
	(শূন্য) ext2fs_xattrs_close(&handle);
	ঘূৰাই retval;
}

স্থির errcode_t ext2fs_inline_data_ea_get(সংরচনা ext2_inline_data *data)
{
	সংরচনা ext2_xattr_handle *handle;
	errcode_t retval;

	data->ea_size = 0;
	data->ea_data = 0;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	যদি (retval)
		ঘূৰাই retval;

	retval = ext2fs_xattrs_read(handle);
	যদি (retval)
		গচ্ছ err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (শূন্য **)&data->ea_data, &data->ea_size);
	যদি (retval == EXT2_ET_EA_চাবি_NOT_FOUND) {
		data->ea_size = 0;
		data->ea_data = রিক্ত;
		retval = 0;
	} অন্যথা যদি (retval)
		গচ্ছ err;

err:
	(শূন্য) ext2fs_xattrs_close(&handle);
	ঘূৰাই retval;
}

errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino)
{
	সংরচনা ext2_inline_data data;
	ৱর্ণ empty[1] = { '\0' };

	data.fs = fs;
	data.ino = ino;
	data.ea_size = 0;
	data.ea_data = empty;
	ঘূৰাই ext2fs_inline_data_ea_set(&data);
}

errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino, size_t *size)
{
	সংরচনা ext2_inode inode;
	সংরচনা ext2_inline_data data;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ঘূৰাই retval;

	যদি (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		ঘূৰাই EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	যদি (retval)
		ঘূৰাই retval;

	*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	ঘূৰাই ext2fs_free_mem(&data.ea_data);
}

পূর্ণাঙ্ক ext2fs_inline_data_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   শূন্য *priv_data)
{
	সংরচনা dir_context *ctx;
	সংরচনা ext2_inode inode;
	সংরচনা ext2_dir_entry dirent;
	সংরচনা ext2_inline_data data;
	পূর্ণাঙ্ক ret = BLOCK_ABORT;
	e2_blkcnt_t blockcnt = 0;
	ৱর্ণ *old_buf;
	unsigned পূর্ণাঙ্ক old_buflen;
	পূর্ণাঙ্ক old_flags;

	ctx = (সংরচনা dir_context *)priv_data;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_DATA;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	যদি (ctx->errcode)
		গচ্ছ out;

	যদি (!(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_DATA;
		গচ্ছ out;
	}

	যদি (!LINUX_S_ISDIR(inode.i_mode)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		গচ্ছ out;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (ৱর্ণ *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	যদি (ret & BLOCK_ABORT)
		গচ্ছ out;

	dirent.inode = ext2fs_le32_to_cpu(inode.i_block[0]);
	dirent.name_len = 2;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (ৱর্ণ *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	যদি (ret & BLOCK_INLINE_DATA_CHANGED) {
		errcode_t err;

		inode.i_block[0] = ext2fs_cpu_to_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		যদি (err)
			গচ্ছ out;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	যদি (ret & BLOCK_ABORT)
		গচ্ছ out;

	ctx->buf = (ৱর্ণ *)inode.i_block + EXT4_INLINE_DATA_DOTDOT_SIZE;
	ctx->buflen = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
#ifdef শব্দগুলি_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	যদি (ctx->errcode) {
		ret |= BLOCK_ABORT;
		গচ্ছ out;
	}
#endif
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	যদি (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef শব্দগুলি_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		যদি (ctx->errcode) {
			ret |= BLOCK_ABORT;
			গচ্ছ out;
		}
#endif
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		যদি (ctx->errcode)
			ret |= BLOCK_ABORT;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	যদি (ret & BLOCK_ABORT)
		গচ্ছ out;

	data.fs = fs;
	data.ino = ino;
	ctx->errcode = ext2fs_inline_data_ea_get(&data);
	যদি (ctx->errcode) {
		ret |= BLOCK_ABORT;
		গচ্ছ out;
	}
	যদি (data.ea_size <= 0)
		গচ্ছ out1;

	ctx->buf = data.ea_data;
	ctx->buflen = data.ea_size;
#ifdef শব্দগুলি_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	যদি (ctx->errcode) {
		ret |= BLOCK_ABORT;
		গচ্ছ out1;
	}
#endif

	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	যদি (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef শব্দগুলি_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		যদি (ctx->errcode) {
			ret |= BLOCK_ABORT;
			গচ্ছ out1;
		}
#endif
		ctx->errcode = ext2fs_inline_data_ea_set(&data);
		যদি (ctx->errcode)
			ret |= BLOCK_ABORT;
	}

out1:
	ext2fs_free_mem(&data.ea_data);
out:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(BLOCK_ABORT | BLOCK_INLINE_DATA_CHANGED);
	ঘূৰাই ret;
}

errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	সংরচনা ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	যদি (retval)
		ঘূৰাই retval;

	retval = ext2fs_xattrs_read(handle);
	যদি (retval)
		গচ্ছ err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(শূন্য) ext2fs_xattrs_close(&handle);
	ঘূৰাই retval;
}

স্থির errcode_t ext2fs_inline_data_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						ৱর্ণ *bbuf, ৱর্ণ *ibuf, পূর্ণাঙ্ক size)
{
	সংরচনা ext2_dir_entry *dir, *dir2;
	সংরচনা ext2_dir_entry_tail *t;
	errcode_t retval;
	পূর্ণাঙ্ক offset;
	unsigned পূর্ণাঙ্ক rec_len;
	পূর্ণাঙ্ক csum_size = 0;
	পূর্ণাঙ্ক filetype = 0;

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = পরিমাণ(সংরচনা ext2_dir_entry_tail);

	/* Create '.' and '..' */
	যদি (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (সংরচনা ext2_dir_entry *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_set_name_len(dir, 1);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_size) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (সংরচনা ext2_dir_entry *) (bbuf + dir->rec_len);
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
	dir = (সংরচনা ext2_dir_entry *) (bbuf + offset);
	memcpy(bbuf + offset, ibuf + EXT4_INLINE_DATA_DOTDOT_SIZE,
	       size - EXT4_INLINE_DATA_DOTDOT_SIZE);
	size += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_DATA_DOTDOT_SIZE;

	কৰা {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		যদি (retval)
			গচ্ছ err;
		offset += rec_len;
		dir = (সংরচনা ext2_dir_entry *) (bbuf + offset);
	} যেতিয়ালৈ (offset < size);
	rec_len += fs->blocksize - csum_size - offset;
	retval = ext2fs_set_rec_len(fs, rec_len, dir2);
	যদি (retval)
		গচ্ছ err;

	যদি (csum_size) {
		t = EXT2_DIRENT_TAIL(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}

err:
	ঘূৰাই retval;
}

স্থির errcode_t
ext2fs_inline_data_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      সংরচনা ext2_inode *inode, ৱর্ণ *buf, size_t size)
{
	errcode_t retval;
	blk64_t blk;
	ৱর্ণ *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	যদি (retval)
		ঘূৰাই retval;

#ifdef শব্দগুলি_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_DATA_DOTDOT_SIZE,
					size, 0);
	যদি (retval)
		গচ্ছ errout;
#endif

	/* Adjust the rec_len */
	retval = ext2fs_inline_data_convert_dir(fs, ino, blk_buf, buf, size);
	যদি (retval)
		গচ্ছ errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	যদি (retval)
		গচ্ছ errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	যদি (retval)
		গচ্ছ errout;

	/* Update inode */
	যদি (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	retval = ext2fs_iblk_add_blocks(fs, inode, 1);
	যদি (retval)
		গচ্ছ errout;
	inode->i_size = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_SET, 0, 0, &blk);
	যদি (retval)
		গচ্ছ errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	যদি (retval)
		গচ্ছ errout;
	ext2fs_block_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	ঘূৰাই retval;
}

স্থির errcode_t
ext2fs_inline_data_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       সংরচনা ext2_inode *inode, ৱর্ণ *buf, size_t size)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_block, 0, পরিমাণ(inode->i_block));
	যদি (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		যদি (retval)
			ঘূৰাই retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	যদি (retval)
		ঘূৰাই retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	যদি (retval)
		ঘূৰাই retval;
	retval = ext2fs_file_write(e2_file, buf, size, 0);
	ext2fs_file_close(e2_file);
	ঘূৰাই retval;
}

errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino)
{
	সংরচনা ext2_inode inode;
	সংরচনা ext2_inline_data data;
	errcode_t retval;
	size_t inline_size;
	ৱর্ণ *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ঘূৰাই retval;

	যদি (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		ঘূৰাই EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	যদি (retval)
		ঘূৰাই retval;
	inline_size = data.ea_size + EXT4_MIN_INLINE_DATA_SIZE;
	retval = ext2fs_get_mem(inline_size, &inline_buf);
	যদি (retval)
		গচ্ছ errout;

	memcpy(inline_buf, (শূন্য *)inode.i_block, EXT4_MIN_INLINE_DATA_SIZE);
	যদি (data.ea_size > 0) {
		memcpy(inline_buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);
	}

	memset((শূন্য *)inode.i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	যদি (retval)
		গচ্ছ errout;
	retval = ext2fs_inline_data_ea_remove(fs, ino);
	যদি (retval)
		গচ্ছ errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		গচ্ছ errout;

	যদি (LINUX_S_ISDIR(inode.i_mode)) {
		retval = ext2fs_inline_data_dir_expand(fs, ino, &inode,
						inline_buf, inline_size);
	} অন্যথা {
		retval = ext2fs_inline_data_file_expand(fs, ino, &inode,
						inline_buf, inline_size);
	}

errout:
	যদি (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&data.ea_data);
	ঘূৰাই retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
				 সংরচনা ext2_inode *inode,
				 শূন্য *buf, size_t *size)
{
	সংরচনা ext2_inode inode_buf;
	সংরচনা ext2_inline_data data;
	errcode_t retval;

	যদি (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		যদি (retval)
			ঘূৰাই retval;
		inode = &inode_buf;
	}

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	যদি (retval)
		ঘূৰাই retval;

	memcpy(buf, (শূন্য *)inode->i_block, EXT4_MIN_INLINE_DATA_SIZE);
	যদি (data.ea_size > 0)
		memcpy((ৱর্ণ *) buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);

	যদি (size)
		*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	ext2fs_free_mem(&data.ea_data);
	ঘূৰাই 0;
}

errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
				 সংরচনা ext2_inode *inode,
				 শূন্য *buf, size_t size)
{
	সংরচনা ext2_inode inode_buf;
	সংরচনা ext2_inline_data data = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_size, existing_size, free_inode_size;

	যদি (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		যদি (retval)
			ঘূৰাই retval;
		inode = &inode_buf;
	}

	যদি (size <= EXT4_MIN_INLINE_DATA_SIZE) {
		memcpy((শূন্য *)inode->i_block, buf, size);
	} অন্যথা {
		retval = ext2fs_xattr_inode_max_size(fs, ino, &free_ea_size);
		যদি (retval)
			ঘূৰাই retval;

		retval = ext2fs_inline_data_size(fs, ino, &existing_size);
		যদি (retval)
			ঘূৰাই retval;

		যদি (existing_size < EXT4_MIN_INLINE_DATA_SIZE) {
			free_inode_size = EXT4_MIN_INLINE_DATA_SIZE -
					  existing_size;
		} অন্যথা {
			free_inode_size = 0;
		}

		যদি (size != existing_size &&
		    size > existing_size + free_ea_size + free_inode_size)
			ঘূৰাই EXT2_ET_INLINE_DATA_NO_SPACE;

		memcpy((শূন্য *)inode->i_block, buf, EXT4_MIN_INLINE_DATA_SIZE);
		যদি (size > EXT4_MIN_INLINE_DATA_SIZE)
			data.ea_size = size - EXT4_MIN_INLINE_DATA_SIZE;
		data.ea_data = (ৱর্ণ *) buf + EXT4_MIN_INLINE_DATA_SIZE;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	যদি (retval)
		ঘূৰাই retval;
	ঘূৰাই ext2fs_inline_data_ea_set(&data);
}

#ifdef DEBUG
#অন্তর্ভাৱ "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#define BUFF_SIZE (64)

স্থির errcode_t file_test(ext2_filsys fs)
{
	সংরচনা ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t size;
	ৱর্ণ *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	যদি (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		ঘূৰাই 1;
	}

	memset(&inode, 0, পরিমাণ(inode));
	inode.i_flags |= EXT4_INLINE_DATA_FL;
	inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	inode.i_mode = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	যদি (retval) {
		com_err("file_test", retval, "while writing a new inode");
		ঘূৰাই 1;
	}

	retval = ext2fs_inline_data_init(fs, newfile);
	যদি (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		ঘূৰাই 1;
	}

	retval = ext2fs_inline_data_size(fs, newfile, &size);
	যদি (retval) {
		com_err("file_test", retval, "while getting size");
		ঘূৰাই 1;
	}

	যদি (size != EXT4_MIN_INLINE_DATA_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		ঘূৰাই 1;
	}

	ext2fs_get_mem(BUFF_SIZE, &buf);
	memset(buf, 'a', BUFF_SIZE);
	retval = ext2fs_inline_data_set(fs, newfile, 0, buf, BUFF_SIZE);
	যদি (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		গচ্ছ err;
	}

	ext2fs_get_mem(BUFF_SIZE, &cmpbuf);
	retval = ext2fs_inline_data_get(fs, newfile, 0, cmpbuf, &size);
	যদি (retval) {
		com_err("file_test", retval, "while getting inline data");
		গচ্ছ err;
	}

	যদি (size != BUFF_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			size, BUFF_SIZE);
		retval = 1;
		গচ্ছ err;
	}

	যদি (memcmp(buf, cmpbuf, BUFF_SIZE)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		গচ্ছ err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	যদি (retval) {
		com_err("file_test", retval, "while truncating inode");
		গচ্ছ err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	যদি (inode.i_size != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	যদি (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	যদি (buf)
		ext2fs_free_mem(&buf);
	ঘূৰাই retval;
}

স্থির errcode_t dir_test(ext2_filsys fs)
{
	ধ্রুৱ ৱর্ণ *dot_name = ".";
	ধ্রুৱ ৱর্ণ *stub_name = "stub";
	ধ্রুৱ ৱর্ণ *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	ৱর্ণ dirname[32];
	পূর্ণাঙ্ক i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	যদি (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		ঘূৰাই retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	যদি (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		ঘূৰাই retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, strlen(parent_name),
			       0, &parent);
	যদি (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		ঘূৰাই retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, strlen(dot_name),
			       0, &tmp);
	যদি (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		ঘূৰাই retval;
	}

	যদি (parent != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		ঘূৰাই 1;
	}

	বাবে (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, পরিমাণ(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, dirname);
		যদি (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			ঘূৰাই retval;
		}

		retval = ext2fs_lookup(fs, parent, dirname, strlen(dirname),
				       0, &tmp);
		যদি (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			ঘূৰাই retval;
		}

		যদি (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			ঘূৰাই 1;
		}
	}

	snprintf(dirname, পরিমাণ(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, dirname);
	যদি (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		ঘূৰাই retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	যদি (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		ঘূৰাই retval;
	}

	ঘূৰাই 0;
}

পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, ৱর্ণ *argv[])
{
	ext2_filsys		fs;
	সংরচনা ext2_super_block param;
	errcode_t		retval;

	/* setup */
	initialize_ext2_error_table();

	memset(&param, 0, পরিমাণ(param));
	ext2fs_blocks_গুনতি_set(&param, 32768);
	param.s_inodes_গুনতি = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_DATA;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_size = 256;

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
		পূর্ণাঙ্ক i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		যদি (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			exit(1);
		}

		/* setup inode cache */
		বাবে (i = 0; i < fs->icache->cache_size; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	যদি (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		ঘূৰাই 1;
	}
	printf("tst_inline_data(FILE): OK\n");

	যদি (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		ঘূৰাই 1;
	}
	printf("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	ঘূৰাই 0;
}
#endif
