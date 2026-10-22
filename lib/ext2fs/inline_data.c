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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <limits.h> /* for PATH_MAX */

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2_ext_attr.h"

#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना ext2_inline_data {
	ext2_filsys fs;
	ext2_ino_t ino;
	माप_प्रकार ea_size;	/* the size of inline data in ea area */
	शून्य *ea_data;
};

स्थिर errcode_t ext2fs_inline_data_ea_set(संरचना ext2_inline_data *data)
{
	संरचना ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	जर (retval)
		परत retval;

	retval = ext2fs_xattrs_read(handle);
	जर (retval)
		जा err;

	retval = ext2fs_xattr_set(handle, "system.data",
				  data->ea_data, data->ea_size);
err:
	(शून्य) ext2fs_xattrs_close(&handle);
	परत retval;
}

स्थिर errcode_t ext2fs_inline_data_ea_get(संरचना ext2_inline_data *data)
{
	संरचना ext2_xattr_handle *handle;
	errcode_t retval;

	data->ea_size = 0;
	data->ea_data = 0;

	retval = ext2fs_xattrs_open(data->fs, data->ino, &handle);
	जर (retval)
		परत retval;

	retval = ext2fs_xattrs_read(handle);
	जर (retval)
		जा err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (शून्य **)&data->ea_data, &data->ea_size);
	जर (retval == EXT2_ET_EA_किल्ली_NOT_FOUND) {
		data->ea_size = 0;
		data->ea_data = रिक्त;
		retval = 0;
	} अन्यथा जर (retval)
		जा err;

err:
	(शून्य) ext2fs_xattrs_close(&handle);
	परत retval;
}

errcode_t ext2fs_inline_data_init(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना ext2_inline_data data;
	अक्षर empty[1] = { '\0' };

	data.fs = fs;
	data.ino = ino;
	data.ea_size = 0;
	data.ea_data = empty;
	परत ext2fs_inline_data_ea_set(&data);
}

errcode_t ext2fs_inline_data_size(ext2_filsys fs, ext2_ino_t ino, माप_प्रकार *size)
{
	संरचना ext2_inode inode;
	संरचना ext2_inline_data data;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	जर (retval)
		परत retval;

	जर (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		परत EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	जर (retval)
		परत retval;

	*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	परत ext2fs_free_mem(&data.ea_data);
}

पूर्णांक ext2fs_inline_data_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   शून्य *priv_data)
{
	संरचना dir_context *ctx;
	संरचना ext2_inode inode;
	संरचना ext2_dir_entry dirent;
	संरचना ext2_inline_data data;
	पूर्णांक ret = BLOCK_ABORT;
	e2_blkcnt_t blockcnt = 0;
	अक्षर *old_buf;
	अचिन्हित पूर्णांक old_buflen;
	पूर्णांक old_flags;

	ctx = (संरचना dir_context *)priv_data;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_DATA;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	जर (ctx->errcode)
		जा out;

	जर (!(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_DATA;
		जा out;
	}

	जर (!LINUX_S_ISDIR(inode.i_mode)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		जा out;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (अक्षर *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	जर (ret & BLOCK_ABORT)
		जा out;

	dirent.inode = ext2fs_le32_to_cpu(inode.i_block[0]);
	dirent.name_len = 2;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (अक्षर *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	जर (ret & BLOCK_INLINE_DATA_CHANGED) {
		errcode_t err;

		inode.i_block[0] = ext2fs_cpu_to_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		जर (err)
			जा out;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	जर (ret & BLOCK_ABORT)
		जा out;

	ctx->buf = (अक्षर *)inode.i_block + EXT4_INLINE_DATA_DOTDOT_SIZE;
	ctx->buflen = EXT4_MIN_INLINE_DATA_SIZE - EXT4_INLINE_DATA_DOTDOT_SIZE;
#ifdef शब्द_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	जर (ctx->errcode) {
		ret |= BLOCK_ABORT;
		जा out;
	}
#endif
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	जर (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef शब्द_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		जर (ctx->errcode) {
			ret |= BLOCK_ABORT;
			जा out;
		}
#endif
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		जर (ctx->errcode)
			ret |= BLOCK_ABORT;
		ret &= ~BLOCK_INLINE_DATA_CHANGED;
	}
	जर (ret & BLOCK_ABORT)
		जा out;

	data.fs = fs;
	data.ino = ino;
	ctx->errcode = ext2fs_inline_data_ea_get(&data);
	जर (ctx->errcode) {
		ret |= BLOCK_ABORT;
		जा out;
	}
	जर (data.ea_size <= 0)
		जा out1;

	ctx->buf = data.ea_data;
	ctx->buflen = data.ea_size;
#ifdef शब्द_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	जर (ctx->errcode) {
		ret |= BLOCK_ABORT;
		जा out1;
	}
#endif

	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_data);
	जर (ret & BLOCK_INLINE_DATA_CHANGED) {
#ifdef शब्द_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		जर (ctx->errcode) {
			ret |= BLOCK_ABORT;
			जा out1;
		}
#endif
		ctx->errcode = ext2fs_inline_data_ea_set(&data);
		जर (ctx->errcode)
			ret |= BLOCK_ABORT;
	}

out1:
	ext2fs_free_mem(&data.ea_data);
out:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(BLOCK_ABORT | BLOCK_INLINE_DATA_CHANGED);
	परत ret;
}

errcode_t ext2fs_inline_data_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	जर (retval)
		परत retval;

	retval = ext2fs_xattrs_read(handle);
	जर (retval)
		जा err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(शून्य) ext2fs_xattrs_close(&handle);
	परत retval;
}

स्थिर errcode_t ext2fs_inline_data_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						अक्षर *bbuf, अक्षर *ibuf, पूर्णांक size)
{
	संरचना ext2_dir_entry *dir, *dir2;
	संरचना ext2_dir_entry_tail *t;
	errcode_t retval;
	पूर्णांक offset;
	अचिन्हित पूर्णांक rec_len;
	पूर्णांक csum_size = 0;
	पूर्णांक filetype = 0;

	जर (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = आकार(संरचना ext2_dir_entry_tail);

	/* Create '.' and '..' */
	जर (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (संरचना ext2_dir_entry *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_set_name_len(dir, 1);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_size) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (संरचना ext2_dir_entry *) (bbuf + dir->rec_len);
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
	dir = (संरचना ext2_dir_entry *) (bbuf + offset);
	स_नकल(bbuf + offset, ibuf + EXT4_INLINE_DATA_DOTDOT_SIZE,
	       size - EXT4_INLINE_DATA_DOTDOT_SIZE);
	size += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_DATA_DOTDOT_SIZE;

	करा {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		जर (retval)
			जा err;
		offset += rec_len;
		dir = (संरचना ext2_dir_entry *) (bbuf + offset);
	} पर्यंत (offset < size);
	rec_len += fs->blocksize - csum_size - offset;
	retval = ext2fs_set_rec_len(fs, rec_len, dir2);
	जर (retval)
		जा err;

	जर (csum_size) {
		t = EXT2_DIRENT_TAIL(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_tail(fs, t);
	}

err:
	परत retval;
}

स्थिर errcode_t
ext2fs_inline_data_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode *inode, अक्षर *buf, माप_प्रकार size)
{
	errcode_t retval;
	blk64_t blk;
	अक्षर *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	जर (retval)
		परत retval;

#ifdef शब्द_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_DATA_DOTDOT_SIZE,
					size, 0);
	जर (retval)
		जा errout;
#endif

	/* Adjust the rec_len */
	retval = ext2fs_inline_data_convert_dir(fs, ino, blk_buf, buf, size);
	जर (retval)
		जा errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	जर (retval)
		जा errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	जर (retval)
		जा errout;

	/* Update inode */
	जर (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	retval = ext2fs_iblk_add_blocks(fs, inode, 1);
	जर (retval)
		जा errout;
	inode->i_size = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_SET, 0, 0, &blk);
	जर (retval)
		जा errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	जर (retval)
		जा errout;
	ext2fs_block_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	परत retval;
}

स्थिर errcode_t
ext2fs_inline_data_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       संरचना ext2_inode *inode, अक्षर *buf, माप_प्रकार size)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	स_रखो(inode->i_block, 0, आकार(inode->i_block));
	जर (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		जर (retval)
			परत retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_DATA_FL;
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	जर (retval)
		परत retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	जर (retval)
		परत retval;
	retval = ext2fs_file_write(e2_file, buf, size, 0);
	ext2fs_file_close(e2_file);
	परत retval;
}

errcode_t ext2fs_inline_data_expand(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना ext2_inode inode;
	संरचना ext2_inline_data data;
	errcode_t retval;
	माप_प्रकार inline_size;
	अक्षर *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	जर (retval)
		परत retval;

	जर (!(inode.i_flags & EXT4_INLINE_DATA_FL))
		परत EXT2_ET_NO_INLINE_DATA;

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	जर (retval)
		परत retval;
	inline_size = data.ea_size + EXT4_MIN_INLINE_DATA_SIZE;
	retval = ext2fs_get_mem(inline_size, &inline_buf);
	जर (retval)
		जा errout;

	स_नकल(inline_buf, (शून्य *)inode.i_block, EXT4_MIN_INLINE_DATA_SIZE);
	जर (data.ea_size > 0) {
		स_नकल(inline_buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);
	}

	स_रखो((शून्य *)inode.i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	जर (retval)
		जा errout;
	retval = ext2fs_inline_data_ea_remove(fs, ino);
	जर (retval)
		जा errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	जर (retval)
		जा errout;

	जर (LINUX_S_ISDIR(inode.i_mode)) {
		retval = ext2fs_inline_data_dir_expand(fs, ino, &inode,
						inline_buf, inline_size);
	} अन्यथा {
		retval = ext2fs_inline_data_file_expand(fs, ino, &inode,
						inline_buf, inline_size);
	}

errout:
	जर (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&data.ea_data);
	परत retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_data_get(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode *inode,
				 शून्य *buf, माप_प्रकार *size)
{
	संरचना ext2_inode inode_buf;
	संरचना ext2_inline_data data;
	errcode_t retval;

	जर (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		जर (retval)
			परत retval;
		inode = &inode_buf;
	}

	data.fs = fs;
	data.ino = ino;
	retval = ext2fs_inline_data_ea_get(&data);
	जर (retval)
		परत retval;

	स_नकल(buf, (शून्य *)inode->i_block, EXT4_MIN_INLINE_DATA_SIZE);
	जर (data.ea_size > 0)
		स_नकल((अक्षर *) buf + EXT4_MIN_INLINE_DATA_SIZE,
		       data.ea_data, data.ea_size);

	जर (size)
		*size = EXT4_MIN_INLINE_DATA_SIZE + data.ea_size;
	ext2fs_free_mem(&data.ea_data);
	परत 0;
}

errcode_t ext2fs_inline_data_set(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode *inode,
				 शून्य *buf, माप_प्रकार size)
{
	संरचना ext2_inode inode_buf;
	संरचना ext2_inline_data data = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	माप_प्रकार free_ea_size, existing_size, free_inode_size;

	जर (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		जर (retval)
			परत retval;
		inode = &inode_buf;
	}

	जर (size <= EXT4_MIN_INLINE_DATA_SIZE) {
		स_नकल((शून्य *)inode->i_block, buf, size);
	} अन्यथा {
		retval = ext2fs_xattr_inode_max_size(fs, ino, &free_ea_size);
		जर (retval)
			परत retval;

		retval = ext2fs_inline_data_size(fs, ino, &existing_size);
		जर (retval)
			परत retval;

		जर (existing_size < EXT4_MIN_INLINE_DATA_SIZE) {
			free_inode_size = EXT4_MIN_INLINE_DATA_SIZE -
					  existing_size;
		} अन्यथा {
			free_inode_size = 0;
		}

		जर (size != existing_size &&
		    size > existing_size + free_ea_size + free_inode_size)
			परत EXT2_ET_INLINE_DATA_NO_SPACE;

		स_नकल((शून्य *)inode->i_block, buf, EXT4_MIN_INLINE_DATA_SIZE);
		जर (size > EXT4_MIN_INLINE_DATA_SIZE)
			data.ea_size = size - EXT4_MIN_INLINE_DATA_SIZE;
		data.ea_data = (अक्षर *) buf + EXT4_MIN_INLINE_DATA_SIZE;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	जर (retval)
		परत retval;
	परत ext2fs_inline_data_ea_set(&data);
}

#ifdef DEBUG
#अन्तर्भाव "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#define BUFF_SIZE (64)

स्थिर errcode_t file_test(ext2_filsys fs)
{
	संरचना ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	माप_प्रकार size;
	अक्षर *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	जर (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		परत 1;
	}

	स_रखो(&inode, 0, आकार(inode));
	inode.i_flags |= EXT4_INLINE_DATA_FL;
	inode.i_size = EXT4_MIN_INLINE_DATA_SIZE;
	inode.i_mode = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	जर (retval) {
		com_err("file_test", retval, "while writing a new inode");
		परत 1;
	}

	retval = ext2fs_inline_data_init(fs, newfile);
	जर (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		परत 1;
	}

	retval = ext2fs_inline_data_size(fs, newfile, &size);
	जर (retval) {
		com_err("file_test", retval, "while getting size");
		परत 1;
	}

	जर (size != EXT4_MIN_INLINE_DATA_SIZE) {
		ख_लिखो(मानक_त्रुटि,
			"tst_inline_data: size of inline data is wrong\n");
		परत 1;
	}

	ext2fs_get_mem(BUFF_SIZE, &buf);
	स_रखो(buf, 'a', BUFF_SIZE);
	retval = ext2fs_inline_data_set(fs, newfile, 0, buf, BUFF_SIZE);
	जर (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		जा err;
	}

	ext2fs_get_mem(BUFF_SIZE, &cmpbuf);
	retval = ext2fs_inline_data_get(fs, newfile, 0, cmpbuf, &size);
	जर (retval) {
		com_err("file_test", retval, "while getting inline data");
		जा err;
	}

	जर (size != BUFF_SIZE) {
		ख_लिखो(मानक_त्रुटि,
			"tst_inline_data: size %zu != buflen %u\n",
			size, BUFF_SIZE);
		retval = 1;
		जा err;
	}

	जर (स_भेद(buf, cmpbuf, BUFF_SIZE)) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		जा err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	जर (retval) {
		com_err("file_test", retval, "while truncating inode");
		जा err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	जर (inode.i_size != 0) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	जर (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	जर (buf)
		ext2fs_free_mem(&buf);
	परत retval;
}

स्थिर errcode_t dir_test(ext2_filsys fs)
{
	स्थिरांक अक्षर *dot_name = ".";
	स्थिरांक अक्षर *stub_name = "stub";
	स्थिरांक अक्षर *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	अक्षर स_नाम[32];
	पूर्णांक i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	जर (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		परत retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	जर (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		परत retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, म_माप(parent_name),
			       0, &parent);
	जर (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		परत retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, म_माप(dot_name),
			       0, &tmp);
	जर (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		परत retval;
	}

	जर (parent != tmp) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		परत 1;
	}

	साठी (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(स_नाम, आकार(स_नाम), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, स_नाम);
		जर (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", स_नाम);
			परत retval;
		}

		retval = ext2fs_lookup(fs, parent, स_नाम, म_माप(स_नाम),
				       0, &tmp);
		जर (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			परत retval;
		}

		जर (dir != tmp) {
			ख_लिखो(मानक_त्रुटि,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			परत 1;
		}
	}

	snprintf(स_नाम, आकार(स_नाम), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, स_नाम);
	जर (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", स_नाम);
		परत retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	जर (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		परत retval;
	}

	परत 0;
}

पूर्णांक main(पूर्णांक argc, अक्षर *argv[])
{
	ext2_filsys		fs;
	संरचना ext2_super_block param;
	errcode_t		retval;

	/* setup */
	initialize_ext2_error_table();

	स_रखो(&param, 0, आकार(param));
	ext2fs_blocks_मोजणी_set(&param, 32768);
	param.s_inodes_मोजणी = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_DATA;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_size = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	जर (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		निकास(1);
	}

	retval = ext2fs_allocate_tables(fs);
	जर (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		निकास(1);
	}

	/* initialize inode cache */
	जर (!fs->icache) {
		ext2_ino_t first_ino = EXT2_FIRST_INO(fs->super);
		पूर्णांक i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		जर (!fs->icache) {
			ख_लिखो(मानक_त्रुटि,
				"tst_inline_data: init inode cache failed\n");
			निकास(1);
		}

		/* setup inode cache */
		साठी (i = 0; i < fs->icache->cache_size; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	जर (file_test(fs)) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data(FILE): FAILED\n");
		परत 1;
	}
	म_लिखो("tst_inline_data(FILE): OK\n");

	जर (dir_test(fs)) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data(DIR): FAILED\n");
		परत 1;
	}
	म_लिखो("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	परत 0;
}
#endif
