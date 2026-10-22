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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <limits.h> /* for PATH_MAX */

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2_ext_attr.h"

#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

ഘടന ext2_inline_data {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_size;	/* the size of inline data in ea area */
	ശൂന്യം *ea_data;
};

സ്ഥിതം errcode_t ext2fs_inline_data_ea_set(ഘടന ext2_inline_data *data)
{
	ഘടന ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = ext2fs_xattrs_read(handle);
	എങ്കിൽ (retval)
		പോകുക err;

	retval = ext2fs_xattr_set(handle, "system.data",
				  data->ea_data, data->ea_size);
err:
	(ശൂന്യം) ext2fs_xattrs_close(&handle);
	തിരികെ retval;
}

സ്ഥിതം errcode_t ext2fs_inline_data_ea_get(ഘടന ext2_inline_data *data)
{
	ഘടന ext2_xattr_handle *handle;
	errcode_t retval;

	data->ea_size = 0;
	data->ea_data = 0;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = ext2fs_xattrs_read(handle);
	എങ്കിൽ (retval)
		പോകുക err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (ശൂന്യം **)&data->ea_data, &data->ea_size);
	എങ്കിൽ (retval == EXT2_ET_EA_താക്കോൽ_NOT_FOUND) {
		data->ea_size = 0;
		data->ea_data = ശൂന്യ;
		retval = 0;
	} അല്ലെങ്കിൽ എങ്കിൽ (retval)
		പോകുക err;

err:
	(ശൂന്യം) ext2fs_xattrs_close(&handle);
	തിരികെ retval;
}

errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino)
{
	ഘടന ext2_inline_data data;
	അക്ഷരം empty[1] = { '\0' };

	data.fs = fs;
	data.ino = ino;
	data.ea_size = 0;
	data.ea_data = empty;
	തിരികെ ext2fs_inline_data_ea_set(&data);
}

errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino, size_t *size)
{
	ഘടന ext2_inode inode;
	ഘടന ext2_inline_data data;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		തിരികെ EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	എങ്കിൽ (retval)
		തിരികെ retval;

	*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	തിരികെ ext2fs_free_mem(&data.ea_data);
}

പൂർണ്ണസംഖ്യ ext2fs_inline_data_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   ശൂന്യം *priv_data)
{
	ഘടന dir_context *ctx;
	ഘടന ext2_inode inode;
	ഘടന ext2_dir_entry dirent;
	ഘടന ext2_inline_data data;
	പൂർണ്ണസംഖ്യ ret = BLOCK_ABORT;
	e2_blkcnt_t blockcnt = 0;
	അക്ഷരം *old_buf;
	unsigned പൂർണ്ണസംഖ്യ old_buflen;
	പൂർണ്ണസംഖ്യ old_flags;

	ctx = (ഘടന dir_context *)priv_data;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_DATA;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (ctx->errcode)
		പോകുക out;

	എങ്കിൽ (!(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_DATA;
		പോകുക out;
	}

	എങ്കിൽ (!LINUX_S_ISDIR(inode.i_mode)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		പോകുക out;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (അക്ഷരം *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	എങ്കിൽ (ret & BLOCK_ABORT)
		പോകുക out;

	dirent.inode = ext2fs_le32_to_cpu(inode.i_block[0]);
	dirent.name_len = 2;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (അക്ഷരം *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	എങ്കിൽ (ret & BLOCK_INLINE_DATA_CHANGED) {
		errcode_t err;

		inode.i_block[0] = ext2fs_cpu_to_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		എങ്കിൽ (err)
			പോകുക out;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	എങ്കിൽ (ret & BLOCK_ABORT)
		പോകുക out;

	ctx->buf = (അക്ഷരം *)inode.i_block + EXT4_INLINE_DATA_DOTDOT_SIZE;
	ctx->buflen = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
#ifdef വാക്കുകൾ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	എങ്കിൽ (ctx->errcode) {
		ret |= BLOCK_ABORT;
		പോകുക out;
	}
#endif
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	എങ്കിൽ (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef വാക്കുകൾ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		എങ്കിൽ (ctx->errcode) {
			ret |= BLOCK_ABORT;
			പോകുക out;
		}
#endif
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		എങ്കിൽ (ctx->errcode)
			ret |= BLOCK_ABORT;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	എങ്കിൽ (ret & BLOCK_ABORT)
		പോകുക out;

	data.fs = fs;
	data.ino = ino;
	ctx->errcode = ext2fs_inline_data_ea_get(&data);
	എങ്കിൽ (ctx->errcode) {
		ret |= BLOCK_ABORT;
		പോകുക out;
	}
	എങ്കിൽ (data.ea_size <= 0)
		പോകുക out1;

	ctx->buf = data.ea_data;
	ctx->buflen = data.ea_size;
#ifdef വാക്കുകൾ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	എങ്കിൽ (ctx->errcode) {
		ret |= BLOCK_ABORT;
		പോകുക out1;
	}
#endif

	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	എങ്കിൽ (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef വാക്കുകൾ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		എങ്കിൽ (ctx->errcode) {
			ret |= BLOCK_ABORT;
			പോകുക out1;
		}
#endif
		ctx->errcode = ext2fs_inline_data_ea_set(&data);
		എങ്കിൽ (ctx->errcode)
			ret |= BLOCK_ABORT;
	}

out1:
	ext2fs_free_mem(&data.ea_data);
out:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(BLOCK_ABORT | BLOCK_INLINE_DATA_CHANGED);
	തിരികെ ret;
}

errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	ഘടന ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = ext2fs_xattrs_read(handle);
	എങ്കിൽ (retval)
		പോകുക err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(ശൂന്യം) ext2fs_xattrs_close(&handle);
	തിരികെ retval;
}

സ്ഥിതം errcode_t ext2fs_inline_data_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						അക്ഷരം *bbuf, അക്ഷരം *ibuf, പൂർണ്ണസംഖ്യ size)
{
	ഘടന ext2_dir_entry *dir, *dir2;
	ഘടന ext2_dir_entry_tail *t;
	errcode_t retval;
	പൂർണ്ണസംഖ്യ offset;
	unsigned പൂർണ്ണസംഖ്യ rec_len;
	പൂർണ്ണസംഖ്യ csum_size = 0;
	പൂർണ്ണസംഖ്യ filetype = 0;

	എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = വലുപ്പം(ഘടന ext2_dir_entry_tail);

	/* Create '.' and '..' */
	എങ്കിൽ (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (ഘടന ext2_dir_entry *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_set_name_len(dir, 1);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_size) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (ഘടന ext2_dir_entry *) (bbuf + dir->rec_len);
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
	dir = (ഘടന ext2_dir_entry *) (bbuf + offset);
	memcpy(bbuf + offset, ibuf + EXT4_INLINE_DATA_DOTDOT_SIZE,
	       size - EXT4_INLINE_DATA_DOTDOT_SIZE);
	size += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_DATA_DOTDOT_SIZE;

	ചെയ്യുക {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		എങ്കിൽ (retval)
			പോകുക err;
		offset += rec_len;
		dir = (ഘടന ext2_dir_entry *) (bbuf + offset);
	} ആകുമ്പോൾ (offset < size);
	rec_len += fs->blocksize - csum_size - offset;
	retval = ext2fs_set_rec_len(fs, rec_len, dir2);
	എങ്കിൽ (retval)
		പോകുക err;

	എങ്കിൽ (csum_size) {
		t = EXT2_DIRENT_TAIL(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}

err:
	തിരികെ retval;
}

സ്ഥിതം errcode_t
ext2fs_inline_data_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      ഘടന ext2_inode *inode, അക്ഷരം *buf, size_t size)
{
	errcode_t retval;
	blk64_t blk;
	അക്ഷരം *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	എങ്കിൽ (retval)
		തിരികെ retval;

#ifdef വാക്കുകൾ_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_DATA_DOTDOT_SIZE,
					size, 0);
	എങ്കിൽ (retval)
		പോകുക errout;
#endif

	/* Adjust the rec_len */
	retval = ext2fs_inline_data_convert_dir(fs, ino, blk_buf, buf, size);
	എങ്കിൽ (retval)
		പോകുക errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	എങ്കിൽ (retval)
		പോകുക errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	എങ്കിൽ (retval)
		പോകുക errout;

	/* Update inode */
	എങ്കിൽ (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	retval = ext2fs_iblk_add_blocks(fs, inode, 1);
	എങ്കിൽ (retval)
		പോകുക errout;
	inode->i_size = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_SET, 0, 0, &blk);
	എങ്കിൽ (retval)
		പോകുക errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	എങ്കിൽ (retval)
		പോകുക errout;
	ext2fs_block_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	തിരികെ retval;
}

സ്ഥിതം errcode_t
ext2fs_inline_data_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       ഘടന ext2_inode *inode, അക്ഷരം *buf, size_t size)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_block, 0, വലുപ്പം(inode->i_block));
	എങ്കിൽ (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		എങ്കിൽ (retval)
			തിരികെ retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	എങ്കിൽ (retval)
		തിരികെ retval;
	retval = ext2fs_file_write(e2_file, buf, size, 0);
	ext2fs_file_close(e2_file);
	തിരികെ retval;
}

errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino)
{
	ഘടന ext2_inode inode;
	ഘടന ext2_inline_data data;
	errcode_t retval;
	size_t inline_size;
	അക്ഷരം *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		തിരികെ EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	എങ്കിൽ (retval)
		തിരികെ retval;
	inline_size = data.ea_size + EXT4_MIN_INLINE_DATA_SIZE;
	retval = ext2fs_get_mem(inline_size, &inline_buf);
	എങ്കിൽ (retval)
		പോകുക errout;

	memcpy(inline_buf, (ശൂന്യം *)inode.i_block, EXT4_MIN_INLINE_DATA_SIZE);
	എങ്കിൽ (data.ea_size > 0) {
		memcpy(inline_buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);
	}

	memset((ശൂന്യം *)inode.i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		പോകുക errout;
	retval = ext2fs_inline_data_ea_remove(fs, ino);
	എങ്കിൽ (retval)
		പോകുക errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		പോകുക errout;

	എങ്കിൽ (LINUX_S_ISDIR(inode.i_mode)) {
		retval = ext2fs_inline_data_dir_expand(fs, ino, &inode,
						inline_buf, inline_size);
	} അല്ലെങ്കിൽ {
		retval = ext2fs_inline_data_file_expand(fs, ino, &inode,
						inline_buf, inline_size);
	}

errout:
	എങ്കിൽ (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&data.ea_data);
	തിരികെ retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
				 ഘടന ext2_inode *inode,
				 ശൂന്യം *buf, size_t *size)
{
	ഘടന ext2_inode inode_buf;
	ഘടന ext2_inline_data data;
	errcode_t retval;

	എങ്കിൽ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
		inode = &inode_buf;
	}

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	എങ്കിൽ (retval)
		തിരികെ retval;

	memcpy(buf, (ശൂന്യം *)inode->i_block, EXT4_MIN_INLINE_DATA_SIZE);
	എങ്കിൽ (data.ea_size > 0)
		memcpy((അക്ഷരം *) buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);

	എങ്കിൽ (size)
		*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	ext2fs_free_mem(&data.ea_data);
	തിരികെ 0;
}

errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
				 ഘടന ext2_inode *inode,
				 ശൂന്യം *buf, size_t size)
{
	ഘടന ext2_inode inode_buf;
	ഘടന ext2_inline_data data = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_size, existing_size, free_inode_size;

	എങ്കിൽ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
		inode = &inode_buf;
	}

	എങ്കിൽ (size <= EXT4_MIN_INLINE_DATA_SIZE) {
		memcpy((ശൂന്യം *)inode->i_block, buf, size);
	} അല്ലെങ്കിൽ {
		retval = ext2fs_xattr_inode_max_size(fs, ino, &free_ea_size);
		എങ്കിൽ (retval)
			തിരികെ retval;

		retval = ext2fs_inline_data_size(fs, ino, &existing_size);
		എങ്കിൽ (retval)
			തിരികെ retval;

		എങ്കിൽ (existing_size < EXT4_MIN_INLINE_DATA_SIZE) {
			free_inode_size = EXT4_MIN_INLINE_DATA_SIZE -
					  existing_size;
		} അല്ലെങ്കിൽ {
			free_inode_size = 0;
		}

		എങ്കിൽ (size != existing_size &&
		    size > existing_size + free_ea_size + free_inode_size)
			തിരികെ EXT2_ET_INLINE_DATA_NO_SPACE;

		memcpy((ശൂന്യം *)inode->i_block, buf, EXT4_MIN_INLINE_DATA_SIZE);
		എങ്കിൽ (size > EXT4_MIN_INLINE_DATA_SIZE)
			data.ea_size = size - EXT4_MIN_INLINE_DATA_SIZE;
		data.ea_data = (അക്ഷരം *) buf + EXT4_MIN_INLINE_DATA_SIZE;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	എങ്കിൽ (retval)
		തിരികെ retval;
	തിരികെ ext2fs_inline_data_ea_set(&data);
}

#ifdef DEBUG
#അന്തര്ഭാവ "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#define BUFF_SIZE (64)

സ്ഥിതം errcode_t file_test(ext2_filsys fs)
{
	ഘടന ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t size;
	അക്ഷരം *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		തിരികെ 1;
	}

	memset(&inode, 0, വലുപ്പം(inode));
	inode.i_flags |= EXT4_INLINE_DATA_FL;
	inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	inode.i_mode = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while writing a new inode");
		തിരികെ 1;
	}

	retval = ext2fs_inline_data_init(fs, newfile);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		തിരികെ 1;
	}

	retval = ext2fs_inline_data_size(fs, newfile, &size);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while getting size");
		തിരികെ 1;
	}

	എങ്കിൽ (size != EXT4_MIN_INLINE_DATA_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		തിരികെ 1;
	}

	ext2fs_get_mem(BUFF_SIZE, &buf);
	memset(buf, 'a', BUFF_SIZE);
	retval = ext2fs_inline_data_set(fs, newfile, 0, buf, BUFF_SIZE);
	എങ്കിൽ (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		പോകുക err;
	}

	ext2fs_get_mem(BUFF_SIZE, &cmpbuf);
	retval = ext2fs_inline_data_get(fs, newfile, 0, cmpbuf, &size);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while getting inline data");
		പോകുക err;
	}

	എങ്കിൽ (size != BUFF_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			size, BUFF_SIZE);
		retval = 1;
		പോകുക err;
	}

	എങ്കിൽ (memcmp(buf, cmpbuf, BUFF_SIZE)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		പോകുക err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while truncating inode");
		പോകുക err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	എങ്കിൽ (inode.i_size != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	എങ്കിൽ (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	എങ്കിൽ (buf)
		ext2fs_free_mem(&buf);
	തിരികെ retval;
}

സ്ഥിതം errcode_t dir_test(ext2_filsys fs)
{
	സ്ഥിരം അക്ഷരം *dot_name = ".";
	സ്ഥിരം അക്ഷരം *stub_name = "stub";
	സ്ഥിരം അക്ഷരം *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	അക്ഷരം dirname[32];
	പൂർണ്ണസംഖ്യ i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	എങ്കിൽ (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		തിരികെ retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	എങ്കിൽ (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		തിരികെ retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, strlen(parent_name),
			       0, &parent);
	എങ്കിൽ (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		തിരികെ retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, strlen(dot_name),
			       0, &tmp);
	എങ്കിൽ (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		തിരികെ retval;
	}

	എങ്കിൽ (parent != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		തിരികെ 1;
	}

	വേണ്ടി (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, വലുപ്പം(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, dirname);
		എങ്കിൽ (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			തിരികെ retval;
		}

		retval = ext2fs_lookup(fs, parent, dirname, strlen(dirname),
				       0, &tmp);
		എങ്കിൽ (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			തിരികെ retval;
		}

		എങ്കിൽ (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			തിരികെ 1;
		}
	}

	snprintf(dirname, വലുപ്പം(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, dirname);
	എങ്കിൽ (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		തിരികെ retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	എങ്കിൽ (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		തിരികെ retval;
	}

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം *argv[])
{
	ext2_filsys		fs;
	ഘടന ext2_super_block param;
	errcode_t		retval;

	/* setup */
	initialize_ext2_error_table();

	memset(&param, 0, വലുപ്പം(param));
	ext2fs_blocks_എണ്ണം_set(&param, 32768);
	param.s_inodes_എണ്ണം = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_DATA;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_size = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}

	retval = ext2fs_allocate_tables(fs);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}

	/* initialize inode cache */
	എങ്കിൽ (!fs->icache) {
		ext2_ino_t first_ino = EXT2_FIRST_INO(fs->super);
		പൂർണ്ണസംഖ്യ i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		എങ്കിൽ (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			exit(1);
		}

		/* setup inode cache */
		വേണ്ടി (i = 0; i < fs->icache->cache_size; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	എങ്കിൽ (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		തിരികെ 1;
	}
	printf("tst_inline_data(FILE): OK\n");

	എങ്കിൽ (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		തിരികെ 1;
	}
	printf("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	തിരികെ 0;
}
#endif
