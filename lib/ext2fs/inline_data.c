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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <limits.h> /* for PATH_MAX */

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2_ext_attr.h"

#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସଂରଚନା ext2_inline_data {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_size;	/* the size of inline data in ea area */
	ଶୂନ୍ୟ *ea_data;
};

ସ୍ଥିର errcode_t ext2fs_inline_data_ea_set(ସଂରଚନା ext2_inline_data *data)
{
	ସଂରଚନା ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = ext2fs_xattrs_read(handle);
	ଯଦି (retval)
		ଯାଅ err;

	retval = ext2fs_xattr_set(handle, "system.data",
				  data->ea_data, data->ea_size);
err:
	(ଶୂନ୍ୟ) ext2fs_xattrs_close(&handle);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t ext2fs_inline_data_ea_get(ସଂରଚନା ext2_inline_data *data)
{
	ସଂରଚନା ext2_xattr_handle *handle;
	errcode_t retval;

	data->ea_size = 0;
	data->ea_data = 0;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = ext2fs_xattrs_read(handle);
	ଯଦି (retval)
		ଯାଅ err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (ଶୂନ୍ୟ **)&data->ea_data, &data->ea_size);
	ଯଦି (retval == EXT2_ET_EA_ଚାବି_NOT_FOUND) {
		data->ea_size = 0;
		data->ea_data = ଖାଲି;
		retval = 0;
	} ଅନ୍ୟଥା ଯଦି (retval)
		ଯାଅ err;

err:
	(ଶୂନ୍ୟ) ext2fs_xattrs_close(&handle);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino)
{
	ସଂରଚନା ext2_inline_data data;
	ଅକ୍ଷର empty[1] = { '\0' };

	data.fs = fs;
	data.ino = ino;
	data.ea_size = 0;
	data.ea_data = empty;
	ଫେରନ୍ତୁ ext2fs_inline_data_ea_set(&data);
}

errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino, size_t *size)
{
	ସଂରଚନା ext2_inode inode;
	ସଂରଚନା ext2_inline_data data;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		ଫେରନ୍ତୁ EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	ଫେରନ୍ତୁ ext2fs_free_mem(&data.ea_data);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_inline_data_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା dir_context *ctx;
	ସଂରଚନା ext2_inode inode;
	ସଂରଚନା ext2_dir_entry dirent;
	ସଂରଚନା ext2_inline_data data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = BLOCK_ABORT;
	e2_blkcnt_t blockcnt = 0;
	ଅକ୍ଷର *old_buf;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_buflen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_flags;

	ctx = (ସଂରଚନା dir_context *)priv_data;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_DATA;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (ctx->errcode)
		ଯାଅ out;

	ଯଦି (!(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_DATA;
		ଯାଅ out;
	}

	ଯଦି (!LINUX_S_ISDIR(inode.i_mode)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		ଯାଅ out;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (ଅକ୍ଷର *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	ଯଦି (ret & BLOCK_ABORT)
		ଯାଅ out;

	dirent.inode = ext2fs_le32_to_cpu(inode.i_block[0]);
	dirent.name_len = 2;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (ଅକ୍ଷର *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	ଯଦି (ret & BLOCK_INLINE_DATA_CHANGED) {
		errcode_t err;

		inode.i_block[0] = ext2fs_cpu_to_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		ଯଦି (err)
			ଯାଅ out;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	ଯଦି (ret & BLOCK_ABORT)
		ଯାଅ out;

	ctx->buf = (ଅକ୍ଷର *)inode.i_block + EXT4_INLINE_DATA_DOTDOT_SIZE;
	ctx->buflen = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	ଯଦି (ctx->errcode) {
		ret |= BLOCK_ABORT;
		ଯାଅ out;
	}
#endif
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	ଯଦି (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		ଯଦି (ctx->errcode) {
			ret |= BLOCK_ABORT;
			ଯାଅ out;
		}
#endif
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		ଯଦି (ctx->errcode)
			ret |= BLOCK_ABORT;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	ଯଦି (ret & BLOCK_ABORT)
		ଯାଅ out;

	data.fs = fs;
	data.ino = ino;
	ctx->errcode = ext2fs_inline_data_ea_get(&data);
	ଯଦି (ctx->errcode) {
		ret |= BLOCK_ABORT;
		ଯାଅ out;
	}
	ଯଦି (data.ea_size <= 0)
		ଯାଅ out1;

	ctx->buf = data.ea_data;
	ctx->buflen = data.ea_size;
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	ଯଦି (ctx->errcode) {
		ret |= BLOCK_ABORT;
		ଯାଅ out1;
	}
#endif

	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	ଯଦି (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		ଯଦି (ctx->errcode) {
			ret |= BLOCK_ABORT;
			ଯାଅ out1;
		}
#endif
		ctx->errcode = ext2fs_inline_data_ea_set(&data);
		ଯଦି (ctx->errcode)
			ret |= BLOCK_ABORT;
	}

out1:
	ext2fs_free_mem(&data.ea_data);
out:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(BLOCK_ABORT | BLOCK_INLINE_DATA_CHANGED);
	ଫେରନ୍ତୁ ret;
}

errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	ସଂରଚନା ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = ext2fs_xattrs_read(handle);
	ଯଦି (retval)
		ଯାଅ err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(ଶୂନ୍ୟ) ext2fs_xattrs_close(&handle);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t ext2fs_inline_data_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						ଅକ୍ଷର *bbuf, ଅକ୍ଷର *ibuf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
	ସଂରଚନା ext2_dir_entry *dir, *dir2;
	ସଂରଚନା ext2_dir_entry_tail *t;
	errcode_t retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rec_len;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା csum_size = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା filetype = 0;

	ଯଦି (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ଆକାର(ସଂରଚନା ext2_dir_entry_tail);

	/* Create '.' and '..' */
	ଯଦି (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (ସଂରଚନା ext2_dir_entry *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_set_name_len(dir, 1);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_size) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (ସଂରଚନା ext2_dir_entry *) (bbuf + dir->rec_len);
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
	dir = (ସଂରଚନା ext2_dir_entry *) (bbuf + offset);
	memcpy(bbuf + offset, ibuf + EXT4_INLINE_DATA_DOTDOT_SIZE,
	       size - EXT4_INLINE_DATA_DOTDOT_SIZE);
	size += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_DATA_DOTDOT_SIZE;

	କର {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		ଯଦି (retval)
			ଯାଅ err;
		offset += rec_len;
		dir = (ସଂରଚନା ext2_dir_entry *) (bbuf + offset);
	} ଯେତେବେଳେ (offset < size);
	rec_len += fs->blocksize - csum_size - offset;
	retval = ext2fs_set_rec_len(fs, rec_len, dir2);
	ଯଦି (retval)
		ଯାଅ err;

	ଯଦି (csum_size) {
		t = EXT2_DIRENT_TAIL(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}

err:
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t
ext2fs_inline_data_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      ସଂରଚନା ext2_inode *inode, ଅକ୍ଷର *buf, size_t size)
{
	errcode_t retval;
	blk64_t blk;
	ଅକ୍ଷର *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_DATA_DOTDOT_SIZE,
					size, 0);
	ଯଦି (retval)
		ଯାଅ errout;
#endif

	/* Adjust the rec_len */
	retval = ext2fs_inline_data_convert_dir(fs, ino, blk_buf, buf, size);
	ଯଦି (retval)
		ଯାଅ errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	ଯଦି (retval)
		ଯାଅ errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	ଯଦି (retval)
		ଯାଅ errout;

	/* Update inode */
	ଯଦି (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	retval = ext2fs_iblk_add_blocks(fs, inode, 1);
	ଯଦି (retval)
		ଯାଅ errout;
	inode->i_size = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_SET, 0, 0, &blk);
	ଯଦି (retval)
		ଯାଅ errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	ଯଦି (retval)
		ଯାଅ errout;
	ext2fs_block_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t
ext2fs_inline_data_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       ସଂରଚନା ext2_inode *inode, ଅକ୍ଷର *buf, size_t size)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_block, 0, ଆକାର(inode->i_block));
	ଯଦି (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	retval = ext2fs_file_write(e2_file, buf, size, 0);
	ext2fs_file_close(e2_file);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino)
{
	ସଂରଚନା ext2_inode inode;
	ସଂରଚନା ext2_inline_data data;
	errcode_t retval;
	size_t inline_size;
	ଅକ୍ଷର *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		ଫେରନ୍ତୁ EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	inline_size = data.ea_size + EXT4_MIN_INLINE_DATA_SIZE;
	retval = ext2fs_get_mem(inline_size, &inline_buf);
	ଯଦି (retval)
		ଯାଅ errout;

	memcpy(inline_buf, (ଶୂନ୍ୟ *)inode.i_block, EXT4_MIN_INLINE_DATA_SIZE);
	ଯଦି (data.ea_size > 0) {
		memcpy(inline_buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);
	}

	memset((ଶୂନ୍ୟ *)inode.i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	ଯଦି (retval)
		ଯାଅ errout;
	retval = ext2fs_inline_data_ea_remove(fs, ino);
	ଯଦି (retval)
		ଯାଅ errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (retval)
		ଯାଅ errout;

	ଯଦି (LINUX_S_ISDIR(inode.i_mode)) {
		retval = ext2fs_inline_data_dir_expand(fs, ino, &inode,
						inline_buf, inline_size);
	} ଅନ୍ୟଥା {
		retval = ext2fs_inline_data_file_expand(fs, ino, &inode,
						inline_buf, inline_size);
	}

errout:
	ଯଦି (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&data.ea_data);
	ଫେରନ୍ତୁ retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
				 ସଂରଚନା ext2_inode *inode,
				 ଶୂନ୍ୟ *buf, size_t *size)
{
	ସଂରଚନା ext2_inode inode_buf;
	ସଂରଚନା ext2_inline_data data;
	errcode_t retval;

	ଯଦି (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		inode = &inode_buf;
	}

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	memcpy(buf, (ଶୂନ୍ୟ *)inode->i_block, EXT4_MIN_INLINE_DATA_SIZE);
	ଯଦି (data.ea_size > 0)
		memcpy((ଅକ୍ଷର *) buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);

	ଯଦି (size)
		*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	ext2fs_free_mem(&data.ea_data);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
				 ସଂରଚନା ext2_inode *inode,
				 ଶୂନ୍ୟ *buf, size_t size)
{
	ସଂରଚନା ext2_inode inode_buf;
	ସଂରଚନା ext2_inline_data data = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_size, existing_size, free_inode_size;

	ଯଦି (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		inode = &inode_buf;
	}

	ଯଦି (size <= EXT4_MIN_INLINE_DATA_SIZE) {
		memcpy((ଶୂନ୍ୟ *)inode->i_block, buf, size);
	} ଅନ୍ୟଥା {
		retval = ext2fs_xattr_inode_max_size(fs, ino, &free_ea_size);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		retval = ext2fs_inline_data_size(fs, ino, &existing_size);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		ଯଦି (existing_size < EXT4_MIN_INLINE_DATA_SIZE) {
			free_inode_size = EXT4_MIN_INLINE_DATA_SIZE -
					  existing_size;
		} ଅନ୍ୟଥା {
			free_inode_size = 0;
		}

		ଯଦି (size != existing_size &&
		    size > existing_size + free_ea_size + free_inode_size)
			ଫେରନ୍ତୁ EXT2_ET_INLINE_DATA_NO_SPACE;

		memcpy((ଶୂନ୍ୟ *)inode->i_block, buf, EXT4_MIN_INLINE_DATA_SIZE);
		ଯଦି (size > EXT4_MIN_INLINE_DATA_SIZE)
			data.ea_size = size - EXT4_MIN_INLINE_DATA_SIZE;
		data.ea_data = (ଅକ୍ଷର *) buf + EXT4_MIN_INLINE_DATA_SIZE;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଫେରନ୍ତୁ ext2fs_inline_data_ea_set(&data);
}

#ifdef DEBUG
#ଅନ୍ତର୍ଭାଵ "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#define BUFF_SIZE (64)

ସ୍ଥିର errcode_t file_test(ext2_filsys fs)
{
	ସଂରଚନା ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t size;
	ଅକ୍ଷର *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	ଯଦି (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		ଫେରନ୍ତୁ 1;
	}

	memset(&inode, 0, ଆକାର(inode));
	inode.i_flags |= EXT4_INLINE_DATA_FL;
	inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	inode.i_mode = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	ଯଦି (retval) {
		com_err("file_test", retval, "while writing a new inode");
		ଫେରନ୍ତୁ 1;
	}

	retval = ext2fs_inline_data_init(fs, newfile);
	ଯଦି (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		ଫେରନ୍ତୁ 1;
	}

	retval = ext2fs_inline_data_size(fs, newfile, &size);
	ଯଦି (retval) {
		com_err("file_test", retval, "while getting size");
		ଫେରନ୍ତୁ 1;
	}

	ଯଦି (size != EXT4_MIN_INLINE_DATA_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		ଫେରନ୍ତୁ 1;
	}

	ext2fs_get_mem(BUFF_SIZE, &buf);
	memset(buf, 'a', BUFF_SIZE);
	retval = ext2fs_inline_data_set(fs, newfile, 0, buf, BUFF_SIZE);
	ଯଦି (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		ଯାଅ err;
	}

	ext2fs_get_mem(BUFF_SIZE, &cmpbuf);
	retval = ext2fs_inline_data_get(fs, newfile, 0, cmpbuf, &size);
	ଯଦି (retval) {
		com_err("file_test", retval, "while getting inline data");
		ଯାଅ err;
	}

	ଯଦି (size != BUFF_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			size, BUFF_SIZE);
		retval = 1;
		ଯାଅ err;
	}

	ଯଦି (memcmp(buf, cmpbuf, BUFF_SIZE)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		ଯାଅ err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	ଯଦି (retval) {
		com_err("file_test", retval, "while truncating inode");
		ଯାଅ err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	ଯଦି (inode.i_size != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	ଯଦି (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	ଯଦି (buf)
		ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t dir_test(ext2_filsys fs)
{
	ଅଚଳ ଅକ୍ଷର *dot_name = ".";
	ଅଚଳ ଅକ୍ଷର *stub_name = "stub";
	ଅଚଳ ଅକ୍ଷର *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	ଅକ୍ଷର dirname[32];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	ଯଦି (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		ଫେରନ୍ତୁ retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	ଯଦି (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		ଫେରନ୍ତୁ retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, strlen(parent_name),
			       0, &parent);
	ଯଦି (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		ଫେରନ୍ତୁ retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, strlen(dot_name),
			       0, &tmp);
	ଯଦି (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		ଫେରନ୍ତୁ retval;
	}

	ଯଦି (parent != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		ଫେରନ୍ତୁ 1;
	}

	ପାଇଁ (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, ଆକାର(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, dirname);
		ଯଦି (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			ଫେରନ୍ତୁ retval;
		}

		retval = ext2fs_lookup(fs, parent, dirname, strlen(dirname),
				       0, &tmp);
		ଯଦି (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			ଫେରନ୍ତୁ retval;
		}

		ଯଦି (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			ଫେରନ୍ତୁ 1;
		}
	}

	snprintf(dirname, ଆକାର(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, dirname);
	ଯଦି (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		ଫେରନ୍ତୁ retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	ଯଦି (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		ଫେରନ୍ତୁ retval;
	}

	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *argv[])
{
	ext2_filsys		fs;
	ସଂରଚନା ext2_super_block param;
	errcode_t		retval;

	/* setup */
	initialize_ext2_error_table();

	memset(&param, 0, ଆକାର(param));
	ext2fs_blocks_ଗଣନା_set(&param, 32768);
	param.s_inodes_ଗଣନା = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_DATA;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_size = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	ଯଦି (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}

	retval = ext2fs_allocate_tables(fs);
	ଯଦି (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}

	/* initialize inode cache */
	ଯଦି (!fs->icache) {
		ext2_ino_t first_ino = EXT2_FIRST_INO(fs->super);
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		ଯଦି (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			exit(1);
		}

		/* setup inode cache */
		ପାଇଁ (i = 0; i < fs->icache->cache_size; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	ଯଦି (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		ଫେରନ୍ତୁ 1;
	}
	printf("tst_inline_data(FILE): OK\n");

	ଯଦି (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		ଫେରନ୍ତୁ 1;
	}
	printf("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	ଫେରନ୍ତୁ 0;
}
#endif
