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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <limits.h> /* for PATH_MAX */

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2_ext_attr.h"

#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

கட்டமைப்பு ext2_inline_data {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_size;	/* the size of inline data in ea area */
	வெறுமை *ea_data;
};

நிலையான errcode_t ext2fs_inline_data_ea_set(கட்டமைப்பு ext2_inline_data *data)
{
	கட்டமைப்பு ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	எனில் (retval)
		திரும்பு retval;

	retval = ext2fs_xattrs_read(handle);
	எனில் (retval)
		செல் err;

	retval = ext2fs_xattr_set(handle, "system.data",
				  data->ea_data, data->ea_size);
err:
	(வெறுமை) ext2fs_xattrs_close(&handle);
	திரும்பு retval;
}

நிலையான errcode_t ext2fs_inline_data_ea_get(கட்டமைப்பு ext2_inline_data *data)
{
	கட்டமைப்பு ext2_xattr_handle *handle;
	errcode_t retval;

	data->ea_size = 0;
	data->ea_data = 0;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	எனில் (retval)
		திரும்பு retval;

	retval = ext2fs_xattrs_read(handle);
	எனில் (retval)
		செல் err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (வெறுமை **)&data->ea_data, &data->ea_size);
	எனில் (retval == EXT2_ET_EA_சாவி_NOT_FOUND) {
		data->ea_size = 0;
		data->ea_data = வெற்று;
		retval = 0;
	} இல்லையெனில் எனில் (retval)
		செல் err;

err:
	(வெறுமை) ext2fs_xattrs_close(&handle);
	திரும்பு retval;
}

errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino)
{
	கட்டமைப்பு ext2_inline_data data;
	எழுத்து empty[1] = { '\0' };

	data.fs = fs;
	data.ino = ino;
	data.ea_size = 0;
	data.ea_data = empty;
	திரும்பு ext2fs_inline_data_ea_set(&data);
}

errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino, size_t *size)
{
	கட்டமைப்பு ext2_inode inode;
	கட்டமைப்பு ext2_inline_data data;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	எனில் (retval)
		திரும்பு retval;

	எனில் (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		திரும்பு EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	எனில் (retval)
		திரும்பு retval;

	*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	திரும்பு ext2fs_free_mem(&data.ea_data);
}

முழு ext2fs_inline_data_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   வெறுமை *priv_data)
{
	கட்டமைப்பு dir_context *ctx;
	கட்டமைப்பு ext2_inode inode;
	கட்டமைப்பு ext2_dir_entry dirent;
	கட்டமைப்பு ext2_inline_data data;
	முழு ret = BLOCK_ABORT;
	e2_blkcnt_t blockcnt = 0;
	எழுத்து *old_buf;
	unsigned முழு old_buflen;
	முழு old_flags;

	ctx = (கட்டமைப்பு dir_context *)priv_data;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_DATA;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	எனில் (ctx->errcode)
		செல் out;

	எனில் (!(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_DATA;
		செல் out;
	}

	எனில் (!LINUX_S_ISDIR(inode.i_mode)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		செல் out;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (எழுத்து *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	எனில் (ret & BLOCK_ABORT)
		செல் out;

	dirent.inode = ext2fs_le32_to_cpu(inode.i_block[0]);
	dirent.name_len = 2;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (எழுத்து *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	எனில் (ret & BLOCK_INLINE_DATA_CHANGED) {
		errcode_t err;

		inode.i_block[0] = ext2fs_cpu_to_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		எனில் (err)
			செல் out;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	எனில் (ret & BLOCK_ABORT)
		செல் out;

	ctx->buf = (எழுத்து *)inode.i_block + EXT4_INLINE_DATA_DOTDOT_SIZE;
	ctx->buflen = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
#ifdef சொற்கள்_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	எனில் (ctx->errcode) {
		ret |= BLOCK_ABORT;
		செல் out;
	}
#endif
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	எனில் (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef சொற்கள்_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		எனில் (ctx->errcode) {
			ret |= BLOCK_ABORT;
			செல் out;
		}
#endif
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		எனில் (ctx->errcode)
			ret |= BLOCK_ABORT;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	எனில் (ret & BLOCK_ABORT)
		செல் out;

	data.fs = fs;
	data.ino = ino;
	ctx->errcode = ext2fs_inline_data_ea_get(&data);
	எனில் (ctx->errcode) {
		ret |= BLOCK_ABORT;
		செல் out;
	}
	எனில் (data.ea_size <= 0)
		செல் out1;

	ctx->buf = data.ea_data;
	ctx->buflen = data.ea_size;
#ifdef சொற்கள்_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	எனில் (ctx->errcode) {
		ret |= BLOCK_ABORT;
		செல் out1;
	}
#endif

	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	எனில் (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef சொற்கள்_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		எனில் (ctx->errcode) {
			ret |= BLOCK_ABORT;
			செல் out1;
		}
#endif
		ctx->errcode = ext2fs_inline_data_ea_set(&data);
		எனில் (ctx->errcode)
			ret |= BLOCK_ABORT;
	}

out1:
	ext2fs_free_mem(&data.ea_data);
out:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(BLOCK_ABORT | BLOCK_INLINE_DATA_CHANGED);
	திரும்பு ret;
}

errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	கட்டமைப்பு ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	எனில் (retval)
		திரும்பு retval;

	retval = ext2fs_xattrs_read(handle);
	எனில் (retval)
		செல் err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(வெறுமை) ext2fs_xattrs_close(&handle);
	திரும்பு retval;
}

நிலையான errcode_t ext2fs_inline_data_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						எழுத்து *bbuf, எழுத்து *ibuf, முழு size)
{
	கட்டமைப்பு ext2_dir_entry *dir, *dir2;
	கட்டமைப்பு ext2_dir_entry_tail *t;
	errcode_t retval;
	முழு offset;
	unsigned முழு rec_len;
	முழு csum_size = 0;
	முழு filetype = 0;

	எனில் (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = அளவு(கட்டமைப்பு ext2_dir_entry_tail);

	/* Create '.' and '..' */
	எனில் (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (கட்டமைப்பு ext2_dir_entry *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_set_name_len(dir, 1);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_size) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (கட்டமைப்பு ext2_dir_entry *) (bbuf + dir->rec_len);
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
	dir = (கட்டமைப்பு ext2_dir_entry *) (bbuf + offset);
	memcpy(bbuf + offset, ibuf + EXT4_INLINE_DATA_DOTDOT_SIZE,
	       size - EXT4_INLINE_DATA_DOTDOT_SIZE);
	size += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_DATA_DOTDOT_SIZE;

	செய் {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		எனில் (retval)
			செல் err;
		offset += rec_len;
		dir = (கட்டமைப்பு ext2_dir_entry *) (bbuf + offset);
	} போது (offset < size);
	rec_len += fs->blocksize - csum_size - offset;
	retval = ext2fs_set_rec_len(fs, rec_len, dir2);
	எனில் (retval)
		செல் err;

	எனில் (csum_size) {
		t = EXT2_DIRENT_TAIL(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}

err:
	திரும்பு retval;
}

நிலையான errcode_t
ext2fs_inline_data_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      கட்டமைப்பு ext2_inode *inode, எழுத்து *buf, size_t size)
{
	errcode_t retval;
	blk64_t blk;
	எழுத்து *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	எனில் (retval)
		திரும்பு retval;

#ifdef சொற்கள்_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_DATA_DOTDOT_SIZE,
					size, 0);
	எனில் (retval)
		செல் errout;
#endif

	/* Adjust the rec_len */
	retval = ext2fs_inline_data_convert_dir(fs, ino, blk_buf, buf, size);
	எனில் (retval)
		செல் errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	எனில் (retval)
		செல் errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	எனில் (retval)
		செல் errout;

	/* Update inode */
	எனில் (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	retval = ext2fs_iblk_add_blocks(fs, inode, 1);
	எனில் (retval)
		செல் errout;
	inode->i_size = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_SET, 0, 0, &blk);
	எனில் (retval)
		செல் errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	எனில் (retval)
		செல் errout;
	ext2fs_block_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	திரும்பு retval;
}

நிலையான errcode_t
ext2fs_inline_data_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       கட்டமைப்பு ext2_inode *inode, எழுத்து *buf, size_t size)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_block, 0, அளவு(inode->i_block));
	எனில் (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		எனில் (retval)
			திரும்பு retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	எனில் (retval)
		திரும்பு retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	எனில் (retval)
		திரும்பு retval;
	retval = ext2fs_file_write(e2_file, buf, size, 0);
	ext2fs_file_close(e2_file);
	திரும்பு retval;
}

errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino)
{
	கட்டமைப்பு ext2_inode inode;
	கட்டமைப்பு ext2_inline_data data;
	errcode_t retval;
	size_t inline_size;
	எழுத்து *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	எனில் (retval)
		திரும்பு retval;

	எனில் (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		திரும்பு EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	எனில் (retval)
		திரும்பு retval;
	inline_size = data.ea_size + EXT4_MIN_INLINE_DATA_SIZE;
	retval = ext2fs_get_mem(inline_size, &inline_buf);
	எனில் (retval)
		செல் errout;

	memcpy(inline_buf, (வெறுமை *)inode.i_block, EXT4_MIN_INLINE_DATA_SIZE);
	எனில் (data.ea_size > 0) {
		memcpy(inline_buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);
	}

	memset((வெறுமை *)inode.i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	எனில் (retval)
		செல் errout;
	retval = ext2fs_inline_data_ea_remove(fs, ino);
	எனில் (retval)
		செல் errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	எனில் (retval)
		செல் errout;

	எனில் (LINUX_S_ISDIR(inode.i_mode)) {
		retval = ext2fs_inline_data_dir_expand(fs, ino, &inode,
						inline_buf, inline_size);
	} இல்லையெனில் {
		retval = ext2fs_inline_data_file_expand(fs, ino, &inode,
						inline_buf, inline_size);
	}

errout:
	எனில் (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&data.ea_data);
	திரும்பு retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
				 கட்டமைப்பு ext2_inode *inode,
				 வெறுமை *buf, size_t *size)
{
	கட்டமைப்பு ext2_inode inode_buf;
	கட்டமைப்பு ext2_inline_data data;
	errcode_t retval;

	எனில் (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		எனில் (retval)
			திரும்பு retval;
		inode = &inode_buf;
	}

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	எனில் (retval)
		திரும்பு retval;

	memcpy(buf, (வெறுமை *)inode->i_block, EXT4_MIN_INLINE_DATA_SIZE);
	எனில் (data.ea_size > 0)
		memcpy((எழுத்து *) buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);

	எனில் (size)
		*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	ext2fs_free_mem(&data.ea_data);
	திரும்பு 0;
}

errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
				 கட்டமைப்பு ext2_inode *inode,
				 வெறுமை *buf, size_t size)
{
	கட்டமைப்பு ext2_inode inode_buf;
	கட்டமைப்பு ext2_inline_data data = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_size, existing_size, free_inode_size;

	எனில் (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		எனில் (retval)
			திரும்பு retval;
		inode = &inode_buf;
	}

	எனில் (size <= EXT4_MIN_INLINE_DATA_SIZE) {
		memcpy((வெறுமை *)inode->i_block, buf, size);
	} இல்லையெனில் {
		retval = ext2fs_xattr_inode_max_size(fs, ino, &free_ea_size);
		எனில் (retval)
			திரும்பு retval;

		retval = ext2fs_inline_data_size(fs, ino, &existing_size);
		எனில் (retval)
			திரும்பு retval;

		எனில் (existing_size < EXT4_MIN_INLINE_DATA_SIZE) {
			free_inode_size = EXT4_MIN_INLINE_DATA_SIZE -
					  existing_size;
		} இல்லையெனில் {
			free_inode_size = 0;
		}

		எனில் (size != existing_size &&
		    size > existing_size + free_ea_size + free_inode_size)
			திரும்பு EXT2_ET_INLINE_DATA_NO_SPACE;

		memcpy((வெறுமை *)inode->i_block, buf, EXT4_MIN_INLINE_DATA_SIZE);
		எனில் (size > EXT4_MIN_INLINE_DATA_SIZE)
			data.ea_size = size - EXT4_MIN_INLINE_DATA_SIZE;
		data.ea_data = (எழுத்து *) buf + EXT4_MIN_INLINE_DATA_SIZE;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	எனில் (retval)
		திரும்பு retval;
	திரும்பு ext2fs_inline_data_ea_set(&data);
}

#ifdef DEBUG
#அந்தர்பாவ "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#define BUFF_SIZE (64)

நிலையான errcode_t file_test(ext2_filsys fs)
{
	கட்டமைப்பு ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t size;
	எழுத்து *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	எனில் (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		திரும்பு 1;
	}

	memset(&inode, 0, அளவு(inode));
	inode.i_flags |= EXT4_INLINE_DATA_FL;
	inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	inode.i_mode = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	எனில் (retval) {
		com_err("file_test", retval, "while writing a new inode");
		திரும்பு 1;
	}

	retval = ext2fs_inline_data_init(fs, newfile);
	எனில் (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		திரும்பு 1;
	}

	retval = ext2fs_inline_data_size(fs, newfile, &size);
	எனில் (retval) {
		com_err("file_test", retval, "while getting size");
		திரும்பு 1;
	}

	எனில் (size != EXT4_MIN_INLINE_DATA_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		திரும்பு 1;
	}

	ext2fs_get_mem(BUFF_SIZE, &buf);
	memset(buf, 'a', BUFF_SIZE);
	retval = ext2fs_inline_data_set(fs, newfile, 0, buf, BUFF_SIZE);
	எனில் (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		செல் err;
	}

	ext2fs_get_mem(BUFF_SIZE, &cmpbuf);
	retval = ext2fs_inline_data_get(fs, newfile, 0, cmpbuf, &size);
	எனில் (retval) {
		com_err("file_test", retval, "while getting inline data");
		செல் err;
	}

	எனில் (size != BUFF_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			size, BUFF_SIZE);
		retval = 1;
		செல் err;
	}

	எனில் (memcmp(buf, cmpbuf, BUFF_SIZE)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		செல் err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	எனில் (retval) {
		com_err("file_test", retval, "while truncating inode");
		செல் err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	எனில் (inode.i_size != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	எனில் (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	எனில் (buf)
		ext2fs_free_mem(&buf);
	திரும்பு retval;
}

நிலையான errcode_t dir_test(ext2_filsys fs)
{
	மாறிலி எழுத்து *dot_name = ".";
	மாறிலி எழுத்து *stub_name = "stub";
	மாறிலி எழுத்து *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	எழுத்து dirname[32];
	முழு i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	எனில் (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		திரும்பு retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	எனில் (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		திரும்பு retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, strlen(parent_name),
			       0, &parent);
	எனில் (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		திரும்பு retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, strlen(dot_name),
			       0, &tmp);
	எனில் (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		திரும்பு retval;
	}

	எனில் (parent != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		திரும்பு 1;
	}

	க்கு (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, அளவு(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, dirname);
		எனில் (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			திரும்பு retval;
		}

		retval = ext2fs_lookup(fs, parent, dirname, strlen(dirname),
				       0, &tmp);
		எனில் (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			திரும்பு retval;
		}

		எனில் (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			திரும்பு 1;
		}
	}

	snprintf(dirname, அளவு(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, dirname);
	எனில் (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		திரும்பு retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	எனில் (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		திரும்பு retval;
	}

	திரும்பு 0;
}

முழு முதன்மை(முழு argc, எழுத்து *argv[])
{
	ext2_filsys		fs;
	கட்டமைப்பு ext2_super_block param;
	errcode_t		retval;

	/* setup */
	initialize_ext2_error_table();

	memset(&param, 0, அளவு(param));
	ext2fs_blocks_மொத்தம்_set(&param, 32768);
	param.s_inodes_மொத்தம் = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_DATA;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_size = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	எனில் (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}

	retval = ext2fs_allocate_tables(fs);
	எனில் (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}

	/* initialize inode cache */
	எனில் (!fs->icache) {
		ext2_ino_t first_ino = EXT2_FIRST_INO(fs->super);
		முழு i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		எனில் (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			exit(1);
		}

		/* setup inode cache */
		க்கு (i = 0; i < fs->icache->cache_size; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	எனில் (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		திரும்பு 1;
	}
	printf("tst_inline_data(FILE): OK\n");

	எனில் (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		திரும்பு 1;
	}
	printf("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	திரும்பு 0;
}
#endif
