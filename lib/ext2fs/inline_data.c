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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <limits.h> /* for PATH_MAX */

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2_ext_attr.h"

#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સંરચના ext2_inline_data {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_size;	/* the size of inline data in ea area */
	શૂન્ય *ea_data;
};

સ્થિર errcode_t ext2fs_inline_data_ea_set(સંરચના ext2_inline_data *data)
{
	સંરચના ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	જો (retval)
		પાછા retval;

	retval = ext2fs_xattrs_read(handle);
	જો (retval)
		જાઓ err;

	retval = ext2fs_xattr_set(handle, "system.data",
				  data->ea_data, data->ea_size);
err:
	(શૂન્ય) ext2fs_xattrs_close(&handle);
	પાછા retval;
}

સ્થિર errcode_t ext2fs_inline_data_ea_get(સંરચના ext2_inline_data *data)
{
	સંરચના ext2_xattr_handle *handle;
	errcode_t retval;

	data->ea_size = 0;
	data->ea_data = 0;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	જો (retval)
		પાછા retval;

	retval = ext2fs_xattrs_read(handle);
	જો (retval)
		જાઓ err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (શૂન્ય **)&data->ea_data, &data->ea_size);
	જો (retval == EXT2_ET_EA_ચાવી_NOT_FOUND) {
		data->ea_size = 0;
		data->ea_data = ખાલી;
		retval = 0;
	} નહીંતર જો (retval)
		જાઓ err;

err:
	(શૂન્ય) ext2fs_xattrs_close(&handle);
	પાછા retval;
}

errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino)
{
	સંરચના ext2_inline_data data;
	અક્ષર empty[1] = { '\0' };

	data.fs = fs;
	data.ino = ino;
	data.ea_size = 0;
	data.ea_data = empty;
	પાછા ext2fs_inline_data_ea_set(&data);
}

errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino, size_t *size)
{
	સંરચના ext2_inode inode;
	સંરચના ext2_inline_data data;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	જો (retval)
		પાછા retval;

	જો (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		પાછા EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	જો (retval)
		પાછા retval;

	*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	પાછા ext2fs_free_mem(&data.ea_data);
}

પૂર્ણાંક ext2fs_inline_data_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   શૂન્ય *priv_data)
{
	સંરચના dir_context *ctx;
	સંરચના ext2_inode inode;
	સંરચના ext2_dir_entry dirent;
	સંરચના ext2_inline_data data;
	પૂર્ણાંક ret = BLOCK_ABORT;
	e2_blkcnt_t blockcnt = 0;
	અક્ષર *old_buf;
	unsigned પૂર્ણાંક old_buflen;
	પૂર્ણાંક old_flags;

	ctx = (સંરચના dir_context *)priv_data;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_DATA;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	જો (ctx->errcode)
		જાઓ out;

	જો (!(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_DATA;
		જાઓ out;
	}

	જો (!LINUX_S_ISDIR(inode.i_mode)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		જાઓ out;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (અક્ષર *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	જો (ret & BLOCK_ABORT)
		જાઓ out;

	dirent.inode = ext2fs_le32_to_cpu(inode.i_block[0]);
	dirent.name_len = 2;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (અક્ષર *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	જો (ret & BLOCK_INLINE_DATA_CHANGED) {
		errcode_t err;

		inode.i_block[0] = ext2fs_cpu_to_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		જો (err)
			જાઓ out;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	જો (ret & BLOCK_ABORT)
		જાઓ out;

	ctx->buf = (અક્ષર *)inode.i_block + EXT4_INLINE_DATA_DOTDOT_SIZE;
	ctx->buflen = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
#ifdef શબ્દો_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	જો (ctx->errcode) {
		ret |= BLOCK_ABORT;
		જાઓ out;
	}
#endif
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	જો (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef શબ્દો_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		જો (ctx->errcode) {
			ret |= BLOCK_ABORT;
			જાઓ out;
		}
#endif
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		જો (ctx->errcode)
			ret |= BLOCK_ABORT;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	જો (ret & BLOCK_ABORT)
		જાઓ out;

	data.fs = fs;
	data.ino = ino;
	ctx->errcode = ext2fs_inline_data_ea_get(&data);
	જો (ctx->errcode) {
		ret |= BLOCK_ABORT;
		જાઓ out;
	}
	જો (data.ea_size <= 0)
		જાઓ out1;

	ctx->buf = data.ea_data;
	ctx->buflen = data.ea_size;
#ifdef શબ્દો_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	જો (ctx->errcode) {
		ret |= BLOCK_ABORT;
		જાઓ out1;
	}
#endif

	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	જો (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef શબ્દો_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		જો (ctx->errcode) {
			ret |= BLOCK_ABORT;
			જાઓ out1;
		}
#endif
		ctx->errcode = ext2fs_inline_data_ea_set(&data);
		જો (ctx->errcode)
			ret |= BLOCK_ABORT;
	}

out1:
	ext2fs_free_mem(&data.ea_data);
out:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(BLOCK_ABORT | BLOCK_INLINE_DATA_CHANGED);
	પાછા ret;
}

errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	સંરચના ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	જો (retval)
		પાછા retval;

	retval = ext2fs_xattrs_read(handle);
	જો (retval)
		જાઓ err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(શૂન્ય) ext2fs_xattrs_close(&handle);
	પાછા retval;
}

સ્થિર errcode_t ext2fs_inline_data_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						અક્ષર *bbuf, અક્ષર *ibuf, પૂર્ણાંક size)
{
	સંરચના ext2_dir_entry *dir, *dir2;
	સંરચના ext2_dir_entry_tail *t;
	errcode_t retval;
	પૂર્ણાંક offset;
	unsigned પૂર્ણાંક rec_len;
	પૂર્ણાંક csum_size = 0;
	પૂર્ણાંક filetype = 0;

	જો (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = કદ(સંરચના ext2_dir_entry_tail);

	/* Create '.' and '..' */
	જો (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (સંરચના ext2_dir_entry *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_set_name_len(dir, 1);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_size) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (સંરચના ext2_dir_entry *) (bbuf + dir->rec_len);
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
	dir = (સંરચના ext2_dir_entry *) (bbuf + offset);
	memcpy(bbuf + offset, ibuf + EXT4_INLINE_DATA_DOTDOT_SIZE,
	       size - EXT4_INLINE_DATA_DOTDOT_SIZE);
	size += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_DATA_DOTDOT_SIZE;

	કરો {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		જો (retval)
			જાઓ err;
		offset += rec_len;
		dir = (સંરચના ext2_dir_entry *) (bbuf + offset);
	} જ્યારે (offset < size);
	rec_len += fs->blocksize - csum_size - offset;
	retval = ext2fs_set_rec_len(fs, rec_len, dir2);
	જો (retval)
		જાઓ err;

	જો (csum_size) {
		t = EXT2_DIRENT_TAIL(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}

err:
	પાછા retval;
}

સ્થિર errcode_t
ext2fs_inline_data_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      સંરચના ext2_inode *inode, અક્ષર *buf, size_t size)
{
	errcode_t retval;
	blk64_t blk;
	અક્ષર *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	જો (retval)
		પાછા retval;

#ifdef શબ્દો_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_DATA_DOTDOT_SIZE,
					size, 0);
	જો (retval)
		જાઓ errout;
#endif

	/* Adjust the rec_len */
	retval = ext2fs_inline_data_convert_dir(fs, ino, blk_buf, buf, size);
	જો (retval)
		જાઓ errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	જો (retval)
		જાઓ errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	જો (retval)
		જાઓ errout;

	/* Update inode */
	જો (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	retval = ext2fs_iblk_add_blocks(fs, inode, 1);
	જો (retval)
		જાઓ errout;
	inode->i_size = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_SET, 0, 0, &blk);
	જો (retval)
		જાઓ errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	જો (retval)
		જાઓ errout;
	ext2fs_block_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	પાછા retval;
}

સ્થિર errcode_t
ext2fs_inline_data_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       સંરચના ext2_inode *inode, અક્ષર *buf, size_t size)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_block, 0, કદ(inode->i_block));
	જો (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		જો (retval)
			પાછા retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	જો (retval)
		પાછા retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	જો (retval)
		પાછા retval;
	retval = ext2fs_file_write(e2_file, buf, size, 0);
	ext2fs_file_close(e2_file);
	પાછા retval;
}

errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino)
{
	સંરચના ext2_inode inode;
	સંરચના ext2_inline_data data;
	errcode_t retval;
	size_t inline_size;
	અક્ષર *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	જો (retval)
		પાછા retval;

	જો (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		પાછા EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	જો (retval)
		પાછા retval;
	inline_size = data.ea_size + EXT4_MIN_INLINE_DATA_SIZE;
	retval = ext2fs_get_mem(inline_size, &inline_buf);
	જો (retval)
		જાઓ errout;

	memcpy(inline_buf, (શૂન્ય *)inode.i_block, EXT4_MIN_INLINE_DATA_SIZE);
	જો (data.ea_size > 0) {
		memcpy(inline_buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);
	}

	memset((શૂન્ય *)inode.i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	જો (retval)
		જાઓ errout;
	retval = ext2fs_inline_data_ea_remove(fs, ino);
	જો (retval)
		જાઓ errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	જો (retval)
		જાઓ errout;

	જો (LINUX_S_ISDIR(inode.i_mode)) {
		retval = ext2fs_inline_data_dir_expand(fs, ino, &inode,
						inline_buf, inline_size);
	} નહીંતર {
		retval = ext2fs_inline_data_file_expand(fs, ino, &inode,
						inline_buf, inline_size);
	}

errout:
	જો (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&data.ea_data);
	પાછા retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
				 સંરચના ext2_inode *inode,
				 શૂન્ય *buf, size_t *size)
{
	સંરચના ext2_inode inode_buf;
	સંરચના ext2_inline_data data;
	errcode_t retval;

	જો (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		જો (retval)
			પાછા retval;
		inode = &inode_buf;
	}

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	જો (retval)
		પાછા retval;

	memcpy(buf, (શૂન્ય *)inode->i_block, EXT4_MIN_INLINE_DATA_SIZE);
	જો (data.ea_size > 0)
		memcpy((અક્ષર *) buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);

	જો (size)
		*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	ext2fs_free_mem(&data.ea_data);
	પાછા 0;
}

errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
				 સંરચના ext2_inode *inode,
				 શૂન્ય *buf, size_t size)
{
	સંરચના ext2_inode inode_buf;
	સંરચના ext2_inline_data data = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_size, existing_size, free_inode_size;

	જો (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		જો (retval)
			પાછા retval;
		inode = &inode_buf;
	}

	જો (size <= EXT4_MIN_INLINE_DATA_SIZE) {
		memcpy((શૂન્ય *)inode->i_block, buf, size);
	} નહીંતર {
		retval = ext2fs_xattr_inode_max_size(fs, ino, &free_ea_size);
		જો (retval)
			પાછા retval;

		retval = ext2fs_inline_data_size(fs, ino, &existing_size);
		જો (retval)
			પાછા retval;

		જો (existing_size < EXT4_MIN_INLINE_DATA_SIZE) {
			free_inode_size = EXT4_MIN_INLINE_DATA_SIZE -
					  existing_size;
		} નહીંતર {
			free_inode_size = 0;
		}

		જો (size != existing_size &&
		    size > existing_size + free_ea_size + free_inode_size)
			પાછા EXT2_ET_INLINE_DATA_NO_SPACE;

		memcpy((શૂન્ય *)inode->i_block, buf, EXT4_MIN_INLINE_DATA_SIZE);
		જો (size > EXT4_MIN_INLINE_DATA_SIZE)
			data.ea_size = size - EXT4_MIN_INLINE_DATA_SIZE;
		data.ea_data = (અક્ષર *) buf + EXT4_MIN_INLINE_DATA_SIZE;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	જો (retval)
		પાછા retval;
	પાછા ext2fs_inline_data_ea_set(&data);
}

#ifdef DEBUG
#અન્તર્ભાવ "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#define BUFF_SIZE (64)

સ્થિર errcode_t file_test(ext2_filsys fs)
{
	સંરચના ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t size;
	અક્ષર *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	જો (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		પાછા 1;
	}

	memset(&inode, 0, કદ(inode));
	inode.i_flags |= EXT4_INLINE_DATA_FL;
	inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	inode.i_mode = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	જો (retval) {
		com_err("file_test", retval, "while writing a new inode");
		પાછા 1;
	}

	retval = ext2fs_inline_data_init(fs, newfile);
	જો (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		પાછા 1;
	}

	retval = ext2fs_inline_data_size(fs, newfile, &size);
	જો (retval) {
		com_err("file_test", retval, "while getting size");
		પાછા 1;
	}

	જો (size != EXT4_MIN_INLINE_DATA_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		પાછા 1;
	}

	ext2fs_get_mem(BUFF_SIZE, &buf);
	memset(buf, 'a', BUFF_SIZE);
	retval = ext2fs_inline_data_set(fs, newfile, 0, buf, BUFF_SIZE);
	જો (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		જાઓ err;
	}

	ext2fs_get_mem(BUFF_SIZE, &cmpbuf);
	retval = ext2fs_inline_data_get(fs, newfile, 0, cmpbuf, &size);
	જો (retval) {
		com_err("file_test", retval, "while getting inline data");
		જાઓ err;
	}

	જો (size != BUFF_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			size, BUFF_SIZE);
		retval = 1;
		જાઓ err;
	}

	જો (memcmp(buf, cmpbuf, BUFF_SIZE)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		જાઓ err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	જો (retval) {
		com_err("file_test", retval, "while truncating inode");
		જાઓ err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	જો (inode.i_size != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	જો (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	જો (buf)
		ext2fs_free_mem(&buf);
	પાછા retval;
}

સ્થિર errcode_t dir_test(ext2_filsys fs)
{
	અચલ અક્ષર *dot_name = ".";
	અચલ અક્ષર *stub_name = "stub";
	અચલ અક્ષર *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	અક્ષર dirname[32];
	પૂર્ણાંક i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	જો (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		પાછા retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	જો (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		પાછા retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, strlen(parent_name),
			       0, &parent);
	જો (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		પાછા retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, strlen(dot_name),
			       0, &tmp);
	જો (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		પાછા retval;
	}

	જો (parent != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		પાછા 1;
	}

	માટે (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, કદ(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, dirname);
		જો (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			પાછા retval;
		}

		retval = ext2fs_lookup(fs, parent, dirname, strlen(dirname),
				       0, &tmp);
		જો (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			પાછા retval;
		}

		જો (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			પાછા 1;
		}
	}

	snprintf(dirname, કદ(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, dirname);
	જો (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		પાછા retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	જો (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		પાછા retval;
	}

	પાછા 0;
}

પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અક્ષર *argv[])
{
	ext2_filsys		fs;
	સંરચના ext2_super_block param;
	errcode_t		retval;

	/* setup */
	initialize_ext2_error_table();

	memset(&param, 0, કદ(param));
	ext2fs_blocks_ગણતરી_set(&param, 32768);
	param.s_inodes_ગણતરી = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_DATA;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_size = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	જો (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}

	retval = ext2fs_allocate_tables(fs);
	જો (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}

	/* initialize inode cache */
	જો (!fs->icache) {
		ext2_ino_t first_ino = EXT2_FIRST_INO(fs->super);
		પૂર્ણાંક i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		જો (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			exit(1);
		}

		/* setup inode cache */
		માટે (i = 0; i < fs->icache->cache_size; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	જો (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		પાછા 1;
	}
	printf("tst_inline_data(FILE): OK\n");

	જો (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		પાછા 1;
	}
	printf("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	પાછા 0;
}
#endif
