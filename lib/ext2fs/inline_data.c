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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <time.h>
#ضمن <limits.h> /* for PATH_MAX */

#ضمن "ext2_fs.h"
#ضمن "ext2_ext_attr.h"

#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية ext2_inline_data {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_size;	/* the size of inline data in ea area */
	فراغ *ea_data;
};

ساكن errcode_t ext2fs_inline_data_ea_set(بنية ext2_inline_data *data)
{
	بنية ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_xattrs_read(handle);
	إذا (retval)
		اذهب err;

	retval = ext2fs_xattr_set(handle, "system.data",
				  data->ea_data, data->ea_size);
err:
	(فراغ) ext2fs_xattrs_close(&handle);
	أرجع retval;
}

ساكن errcode_t ext2fs_inline_data_ea_get(بنية ext2_inline_data *data)
{
	بنية ext2_xattr_handle *handle;
	errcode_t retval;

	data->ea_size = 0;
	data->ea_data = 0;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_xattrs_read(handle);
	إذا (retval)
		اذهب err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (فراغ **)&data->ea_data, &data->ea_size);
	إذا (retval == EXT2_ET_EA_ئاچقۇچ_NOT_FOUND) {
		data->ea_size = 0;
		data->ea_data = عدم;
		retval = 0;
	} وإلا إذا (retval)
		اذهب err;

err:
	(فراغ) ext2fs_xattrs_close(&handle);
	أرجع retval;
}

errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_inline_data data;
	حرف empty[1] = { '\0' };

	data.fs = fs;
	data.ino = ino;
	data.ea_size = 0;
	data.ea_data = empty;
	أرجع ext2fs_inline_data_ea_set(&data);
}

errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino, size_t *size)
{
	بنية ext2_inode inode;
	بنية ext2_inline_data data;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;

	إذا (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		أرجع EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	إذا (retval)
		أرجع retval;

	*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	أرجع ext2fs_free_mem(&data.ea_data);
}

صحيح ext2fs_inline_data_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   فراغ *priv_data)
{
	بنية dir_context *ctx;
	بنية ext2_inode inode;
	بنية ext2_dir_entry dirent;
	بنية ext2_inline_data data;
	صحيح ret = BLOCK_ABORT;
	e2_blkcnt_t blockcnt = 0;
	حرف *old_buf;
	unsigned صحيح old_buflen;
	صحيح old_flags;

	ctx = (بنية dir_context *)priv_data;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_DATA;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	إذا (ctx->errcode)
		اذهب out;

	إذا (!(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_DATA;
		اذهب out;
	}

	إذا (!LINUX_S_ISDIR(inode.i_mode)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		اذهب out;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (حرف *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	إذا (ret & BLOCK_ABORT)
		اذهب out;

	dirent.inode = ext2fs_le32_to_cpu(inode.i_block[0]);
	dirent.name_len = 2;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (حرف *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	إذا (ret & BLOCK_INLINE_DATA_CHANGED) {
		errcode_t err;

		inode.i_block[0] = ext2fs_cpu_to_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		إذا (err)
			اذهب out;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	إذا (ret & BLOCK_ABORT)
		اذهب out;

	ctx->buf = (حرف *)inode.i_block + EXT4_INLINE_DATA_DOTDOT_SIZE;
	ctx->buflen = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
#ifdef سۆزلەر_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	إذا (ctx->errcode) {
		ret |= BLOCK_ABORT;
		اذهب out;
	}
#endif
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	إذا (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef سۆزلەر_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		إذا (ctx->errcode) {
			ret |= BLOCK_ABORT;
			اذهب out;
		}
#endif
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		إذا (ctx->errcode)
			ret |= BLOCK_ABORT;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	إذا (ret & BLOCK_ABORT)
		اذهب out;

	data.fs = fs;
	data.ino = ino;
	ctx->errcode = ext2fs_inline_data_ea_get(&data);
	إذا (ctx->errcode) {
		ret |= BLOCK_ABORT;
		اذهب out;
	}
	إذا (data.ea_size <= 0)
		اذهب out1;

	ctx->buf = data.ea_data;
	ctx->buflen = data.ea_size;
#ifdef سۆزلەر_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	إذا (ctx->errcode) {
		ret |= BLOCK_ABORT;
		اذهب out1;
	}
#endif

	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	إذا (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef سۆزلەر_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		إذا (ctx->errcode) {
			ret |= BLOCK_ABORT;
			اذهب out1;
		}
#endif
		ctx->errcode = ext2fs_inline_data_ea_set(&data);
		إذا (ctx->errcode)
			ret |= BLOCK_ABORT;
	}

out1:
	ext2fs_free_mem(&data.ea_data);
out:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(BLOCK_ABORT | BLOCK_INLINE_DATA_CHANGED);
	أرجع ret;
}

errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_xattrs_read(handle);
	إذا (retval)
		اذهب err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(فراغ) ext2fs_xattrs_close(&handle);
	أرجع retval;
}

ساكن errcode_t ext2fs_inline_data_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						حرف *bbuf, حرف *ibuf, صحيح size)
{
	بنية ext2_dir_entry *dir, *dir2;
	بنية ext2_dir_entry_tail *t;
	errcode_t retval;
	صحيح offset;
	unsigned صحيح rec_len;
	صحيح csum_size = 0;
	صحيح filetype = 0;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = حجم(بنية ext2_dir_entry_tail);

	/* Create '.' and '..' */
	إذا (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (بنية ext2_dir_entry *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_set_name_len(dir, 1);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_size) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (بنية ext2_dir_entry *) (bbuf + dir->rec_len);
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
	dir = (بنية ext2_dir_entry *) (bbuf + offset);
	memcpy(bbuf + offset, ibuf + EXT4_INLINE_DATA_DOTDOT_SIZE,
	       size - EXT4_INLINE_DATA_DOTDOT_SIZE);
	size += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_DATA_DOTDOT_SIZE;

	افعل {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		إذا (retval)
			اذهب err;
		offset += rec_len;
		dir = (بنية ext2_dir_entry *) (bbuf + offset);
	} طالما (offset < size);
	rec_len += fs->blocksize - csum_size - offset;
	retval = ext2fs_set_rec_len(fs, rec_len, dir2);
	إذا (retval)
		اذهب err;

	إذا (csum_size) {
		t = EXT2_DIRENT_TAIL(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}

err:
	أرجع retval;
}

ساكن errcode_t
ext2fs_inline_data_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode, حرف *buf, size_t size)
{
	errcode_t retval;
	blk64_t blk;
	حرف *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	إذا (retval)
		أرجع retval;

#ifdef سۆزلەر_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_DATA_DOTDOT_SIZE,
					size, 0);
	إذا (retval)
		اذهب errout;
#endif

	/* Adjust the rec_len */
	retval = ext2fs_inline_data_convert_dir(fs, ino, blk_buf, buf, size);
	إذا (retval)
		اذهب errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	إذا (retval)
		اذهب errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	إذا (retval)
		اذهب errout;

	/* Update inode */
	إذا (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	retval = ext2fs_iblk_add_blocks(fs, inode, 1);
	إذا (retval)
		اذهب errout;
	inode->i_size = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_SET, 0, 0, &blk);
	إذا (retval)
		اذهب errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		اذهب errout;
	ext2fs_block_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	أرجع retval;
}

ساكن errcode_t
ext2fs_inline_data_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode *inode, حرف *buf, size_t size)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_block, 0, حجم(inode->i_block));
	إذا (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		إذا (retval)
			أرجع retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		أرجع retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_file_write(e2_file, buf, size, 0);
	ext2fs_file_close(e2_file);
	أرجع retval;
}

errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_inode inode;
	بنية ext2_inline_data data;
	errcode_t retval;
	size_t inline_size;
	حرف *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;

	إذا (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		أرجع EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	إذا (retval)
		أرجع retval;
	inline_size = data.ea_size + EXT4_MIN_INLINE_DATA_SIZE;
	retval = ext2fs_get_mem(inline_size, &inline_buf);
	إذا (retval)
		اذهب errout;

	memcpy(inline_buf, (فراغ *)inode.i_block, EXT4_MIN_INLINE_DATA_SIZE);
	إذا (data.ea_size > 0) {
		memcpy(inline_buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);
	}

	memset((فراغ *)inode.i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	إذا (retval)
		اذهب errout;
	retval = ext2fs_inline_data_ea_remove(fs, ino);
	إذا (retval)
		اذهب errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		اذهب errout;

	إذا (LINUX_S_ISDIR(inode.i_mode)) {
		retval = ext2fs_inline_data_dir_expand(fs, ino, &inode,
						inline_buf, inline_size);
	} وإلا {
		retval = ext2fs_inline_data_file_expand(fs, ino, &inode,
						inline_buf, inline_size);
	}

errout:
	إذا (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&data.ea_data);
	أرجع retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
				 بنية ext2_inode *inode,
				 فراغ *buf, size_t *size)
{
	بنية ext2_inode inode_buf;
	بنية ext2_inline_data data;
	errcode_t retval;

	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	إذا (retval)
		أرجع retval;

	memcpy(buf, (فراغ *)inode->i_block, EXT4_MIN_INLINE_DATA_SIZE);
	إذا (data.ea_size > 0)
		memcpy((حرف *) buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);

	إذا (size)
		*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	ext2fs_free_mem(&data.ea_data);
	أرجع 0;
}

errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
				 بنية ext2_inode *inode,
				 فراغ *buf, size_t size)
{
	بنية ext2_inode inode_buf;
	بنية ext2_inline_data data = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_size, existing_size, free_inode_size;

	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}

	إذا (size <= EXT4_MIN_INLINE_DATA_SIZE) {
		memcpy((فراغ *)inode->i_block, buf, size);
	} وإلا {
		retval = ext2fs_xattr_inode_max_size(fs, ino, &free_ea_size);
		إذا (retval)
			أرجع retval;

		retval = ext2fs_inline_data_size(fs, ino, &existing_size);
		إذا (retval)
			أرجع retval;

		إذا (existing_size < EXT4_MIN_INLINE_DATA_SIZE) {
			free_inode_size = EXT4_MIN_INLINE_DATA_SIZE -
					  existing_size;
		} وإلا {
			free_inode_size = 0;
		}

		إذا (size != existing_size &&
		    size > existing_size + free_ea_size + free_inode_size)
			أرجع EXT2_ET_INLINE_DATA_NO_SPACE;

		memcpy((فراغ *)inode->i_block, buf, EXT4_MIN_INLINE_DATA_SIZE);
		إذا (size > EXT4_MIN_INLINE_DATA_SIZE)
			data.ea_size = size - EXT4_MIN_INLINE_DATA_SIZE;
		data.ea_data = (حرف *) buf + EXT4_MIN_INLINE_DATA_SIZE;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		أرجع retval;
	أرجع ext2fs_inline_data_ea_set(&data);
}

#ifdef DEBUG
#ضمن "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#define BUFF_SIZE (64)

ساكن errcode_t file_test(ext2_filsys fs)
{
	بنية ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t size;
	حرف *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	إذا (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		أرجع 1;
	}

	memset(&inode, 0, حجم(inode));
	inode.i_flags |= EXT4_INLINE_DATA_FL;
	inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	inode.i_mode = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	إذا (retval) {
		com_err("file_test", retval, "while writing a new inode");
		أرجع 1;
	}

	retval = ext2fs_inline_data_init(fs, newfile);
	إذا (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		أرجع 1;
	}

	retval = ext2fs_inline_data_size(fs, newfile, &size);
	إذا (retval) {
		com_err("file_test", retval, "while getting size");
		أرجع 1;
	}

	إذا (size != EXT4_MIN_INLINE_DATA_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		أرجع 1;
	}

	ext2fs_get_mem(BUFF_SIZE, &buf);
	memset(buf, 'a', BUFF_SIZE);
	retval = ext2fs_inline_data_set(fs, newfile, 0, buf, BUFF_SIZE);
	إذا (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		اذهب err;
	}

	ext2fs_get_mem(BUFF_SIZE, &cmpbuf);
	retval = ext2fs_inline_data_get(fs, newfile, 0, cmpbuf, &size);
	إذا (retval) {
		com_err("file_test", retval, "while getting inline data");
		اذهب err;
	}

	إذا (size != BUFF_SIZE) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			size, BUFF_SIZE);
		retval = 1;
		اذهب err;
	}

	إذا (memcmp(buf, cmpbuf, BUFF_SIZE)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		اذهب err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	إذا (retval) {
		com_err("file_test", retval, "while truncating inode");
		اذهب err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	إذا (inode.i_size != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	إذا (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	إذا (buf)
		ext2fs_free_mem(&buf);
	أرجع retval;
}

ساكن errcode_t dir_test(ext2_filsys fs)
{
	ثابت حرف *dot_name = ".";
	ثابت حرف *stub_name = "stub";
	ثابت حرف *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	حرف dirname[32];
	صحيح i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	إذا (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		أرجع retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	إذا (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		أرجع retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, strlen(parent_name),
			       0, &parent);
	إذا (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		أرجع retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, strlen(dot_name),
			       0, &tmp);
	إذا (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		أرجع retval;
	}

	إذا (parent != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		أرجع 1;
	}

	لكل (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, حجم(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, dirname);
		إذا (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			أرجع retval;
		}

		retval = ext2fs_lookup(fs, parent, dirname, strlen(dirname),
				       0, &tmp);
		إذا (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			أرجع retval;
		}

		إذا (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			أرجع 1;
		}
	}

	snprintf(dirname, حجم(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, dirname);
	إذا (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		أرجع retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	إذا (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		أرجع retval;
	}

	أرجع 0;
}

صحيح main(صحيح argc, حرف *argv[])
{
	ext2_filsys		fs;
	بنية ext2_super_block param;
	errcode_t		retval;

	/* setup */
	initialize_ext2_error_table();

	memset(&param, 0, حجم(param));
	ext2fs_blocks_سانى_set(&param, 32768);
	param.s_inodes_سانى = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_DATA;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_size = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	إذا (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		exit(1);
	}

	retval = ext2fs_allocate_tables(fs);
	إذا (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		exit(1);
	}

	/* initialize inode cache */
	إذا (!fs->icache) {
		ext2_ino_t first_ino = EXT2_FIRST_INO(fs->super);
		صحيح i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		إذا (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			exit(1);
		}

		/* setup inode cache */
		لكل (i = 0; i < fs->icache->cache_size; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	إذا (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		أرجع 1;
	}
	printf("tst_inline_data(FILE): OK\n");

	إذا (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		أرجع 1;
	}
	printf("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	أرجع 0;
}
#endif
