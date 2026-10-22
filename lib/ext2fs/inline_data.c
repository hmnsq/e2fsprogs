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

#شامل "config.h"
#شامل <stdio.h>
#شامل <time.h>
#شامل <limits.h> /* for PATH_MAX */

#شامل "ext2_fs.h"
#شامل "ext2_ext_attr.h"

#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساخت ext2_inline_ڈیٹا {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_سائز;	/* the size of inline data in ea area */
	خالی *ea_ڈیٹا;
};

ساکن errcode_t ext2fs_inline_ڈیٹا_ea_سیٹ(ساخت ext2_inline_ڈیٹا *ڈیٹا)
{
	ساخت ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(ڈیٹا->fs, ڈیٹا->ino, &handle);
	اگر (retval)
		واپس retval;

	retval = ext2fs_xattrs_read(handle);
	اگر (retval)
		جاؤ err;

	retval = ext2fs_xattr_سیٹ(handle, "system.data",
				  ڈیٹا->ea_ڈیٹا, ڈیٹا->ea_سائز);
err:
	(خالی) ext2fs_xattrs_close(&handle);
	واپس retval;
}

ساکن errcode_t ext2fs_inline_ڈیٹا_ea_get(ساخت ext2_inline_ڈیٹا *ڈیٹا)
{
	ساخت ext2_xattr_handle *handle;
	errcode_t retval;

	ڈیٹا->ea_سائز = 0;
	ڈیٹا->ea_ڈیٹا = 0;

	retval = ext2fs_xattrs_open(ڈیٹا->fs, ڈیٹا->ino, &handle);
	اگر (retval)
		واپس retval;

	retval = ext2fs_xattrs_read(handle);
	اگر (retval)
		جاؤ err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (خالی **)&ڈیٹا->ea_ڈیٹا, &ڈیٹا->ea_سائز);
	اگر (retval == EXT2_ET_EA_کلید_NOT_ملا) {
		ڈیٹا->ea_سائز = 0;
		ڈیٹا->ea_ڈیٹا = تہی;
		retval = 0;
	} ورنہ اگر (retval)
		جاؤ err;

err:
	(خالی) ext2fs_xattrs_close(&handle);
	واپس retval;
}

errcode_t ext2fs_inline_ڈیٹا_init(ext2_filsys fs, ext2_ino_t ino)
{
	ساخت ext2_inline_ڈیٹا ڈیٹا;
	حرف خالی_خانے[1] = { '\0' };

	ڈیٹا.fs = fs;
	ڈیٹا.ino = ino;
	ڈیٹا.ea_سائز = 0;
	ڈیٹا.ea_ڈیٹا = خالی_خانے;
	واپس ext2fs_inline_ڈیٹا_ea_سیٹ(&ڈیٹا);
}

errcode_t ext2fs_inline_ڈیٹا_سائز(ext2_filsys fs, ext2_ino_t ino, size_t *سائز)
{
	ساخت ext2_inode inode;
	ساخت ext2_inline_ڈیٹا ڈیٹا;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		واپس retval;

	اگر (!(inode.i_flags & EXT4_INLINE_ڈیٹا_FL))
		واپس EXT2_ET_NO_INLINE_ڈیٹا;

	ڈیٹا.fs = fs;
	ڈیٹا.ino = ino;
	retval = ext2fs_inline_ڈیٹا_ea_get(&ڈیٹا);
	اگر (retval)
		واپس retval;

	*سائز = EXT4_MIN_INLINE_ڈیٹا_سائز + ڈیٹا.ea_سائز;
	واپس ext2fs_free_mem(&ڈیٹا.ea_ڈیٹا);
}

صحیح ext2fs_inline_ڈیٹا_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   خالی *priv_ڈیٹا)
{
	ساخت dir_context *ctx;
	ساخت ext2_inode inode;
	ساخت ext2_dir_اندراج dirent;
	ساخت ext2_inline_ڈیٹا ڈیٹا;
	صحیح ret = بلاک_ABORT;
	e2_blkcnt_t blockcnt = 0;
	حرف *old_buf;
	غیرعلامتی صحیح old_buflen;
	صحیح old_flags;

	ctx = (ساخت dir_context *)priv_ڈیٹا;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_ڈیٹا;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	اگر (ctx->errcode)
		جاؤ برآمد;

	اگر (!(inode.i_flags & EXT4_INLINE_ڈیٹا_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_ڈیٹا;
		جاؤ برآمد;
	}

	اگر (!LINUX_S_ISDIR(inode.i_منوال)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		جاؤ برآمد;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_سیٹ_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (حرف *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_بلاک(fs, 0, blockcnt++, 0, 0, priv_ڈیٹا);
	اگر (ret & بلاک_ABORT)
		جاؤ برآمد;

	dirent.inode = ext2fs_le32_مقصد_cpu(inode.i_بلاک[0]);
	dirent.name_len = 2;
	ext2fs_سیٹ_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (حرف *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_بلاک(fs, 0, blockcnt++, 0, 0, priv_ڈیٹا);
	اگر (ret & بلاک_INLINE_ڈیٹا_بدلا) {
		errcode_t err;

		inode.i_بلاک[0] = ext2fs_cpu_مقصد_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		اگر (err)
			جاؤ برآمد;
		ret &= ~بلاک_INLINE_ڈیٹا_بدلا;
	}
	اگر (ret & بلاک_ABORT)
		جاؤ برآمد;

	ctx->buf = (حرف *)inode.i_بلاک + EXT4_INLINE_ڈیٹا_DOTDOT_سائز;
	ctx->buflen = EXT4_MIN_INLINE_ڈیٹا_سائز - EXT4_INLINE_ڈیٹا_DOTDOT_سائز;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	اگر (ctx->errcode) {
		ret |= بلاک_ABORT;
		جاؤ برآمد;
	}
#اگر_ختم
	ret |= ext2fs_process_dir_بلاک(fs, 0, blockcnt++, 0, 0, priv_ڈیٹا);
	اگر (ret & بلاک_INLINE_ڈیٹا_بدلا) {
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		اگر (ctx->errcode) {
			ret |= بلاک_ABORT;
			جاؤ برآمد;
		}
#اگر_ختم
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		اگر (ctx->errcode)
			ret |= بلاک_ABORT;
		ret &= ~بلاک_INLINE_ڈیٹا_بدلا;
	}
	اگر (ret & بلاک_ABORT)
		جاؤ برآمد;

	ڈیٹا.fs = fs;
	ڈیٹا.ino = ino;
	ctx->errcode = ext2fs_inline_ڈیٹا_ea_get(&ڈیٹا);
	اگر (ctx->errcode) {
		ret |= بلاک_ABORT;
		جاؤ برآمد;
	}
	اگر (ڈیٹا.ea_سائز <= 0)
		جاؤ out1;

	ctx->buf = ڈیٹا.ea_ڈیٹا;
	ctx->buflen = ڈیٹا.ea_سائز;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	اگر (ctx->errcode) {
		ret |= بلاک_ABORT;
		جاؤ out1;
	}
#اگر_ختم

	ret |= ext2fs_process_dir_بلاک(fs, 0, blockcnt++, 0, 0, priv_ڈیٹا);
	اگر (ret & بلاک_INLINE_ڈیٹا_بدلا) {
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		اگر (ctx->errcode) {
			ret |= بلاک_ABORT;
			جاؤ out1;
		}
#اگر_ختم
		ctx->errcode = ext2fs_inline_ڈیٹا_ea_سیٹ(&ڈیٹا);
		اگر (ctx->errcode)
			ret |= بلاک_ABORT;
	}

out1:
	ext2fs_free_mem(&ڈیٹا.ea_ڈیٹا);
برآمد:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(بلاک_ABORT | بلاک_INLINE_ڈیٹا_بدلا);
	واپس ret;
}

errcode_t ext2fs_inline_ڈیٹا_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	ساخت ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	اگر (retval)
		واپس retval;

	retval = ext2fs_xattrs_read(handle);
	اگر (retval)
		جاؤ err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(خالی) ext2fs_xattrs_close(&handle);
	واپس retval;
}

ساکن errcode_t ext2fs_inline_ڈیٹا_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						حرف *bbuf, حرف *ibuf, صحیح سائز)
{
	ساخت ext2_dir_اندراج *dir, *dir2;
	ساخت ext2_dir_اندراج_دم *t;
	errcode_t retval;
	صحیح انحراف;
	غیرعلامتی صحیح rec_len;
	صحیح csum_سائز = 0;
	صحیح filetype = 0;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائز = حجم(ساخت ext2_dir_اندراج_دم);

	/* Create '.' and '..' */
	اگر (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (ساخت ext2_dir_اندراج *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_سیٹ_name_len(dir, 1);
	ext2fs_dirent_سیٹ_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_سائز) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (ساخت ext2_dir_اندراج *) (bbuf + dir->rec_len);
	dir->rec_len = EXT2_DIR_REC_LEN(2);
	dir->inode = ext2fs_le32_مقصد_cpu(((__u32 *)ibuf)[0]);
	ext2fs_dirent_سیٹ_name_len(dir, 2);
	ext2fs_dirent_سیٹ_file_type(dir, filetype);
	dir->name[0] = '.';
	dir->name[1] = '.';

	/*
	 * Adjust the last rec_len
	 */
	انحراف = EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2);
	dir = (ساخت ext2_dir_اندراج *) (bbuf + انحراف);
	حافظہ_نقل(bbuf + انحراف, ibuf + EXT4_INLINE_ڈیٹا_DOTDOT_سائز,
	       سائز - EXT4_INLINE_ڈیٹا_DOTDOT_سائز);
	سائز += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_ڈیٹا_DOTDOT_سائز;

	کرو {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		اگر (retval)
			جاؤ err;
		انحراف += rec_len;
		dir = (ساخت ext2_dir_اندراج *) (bbuf + انحراف);
	} جب (انحراف < سائز);
	rec_len += fs->blocksize - csum_سائز - انحراف;
	retval = ext2fs_سیٹ_rec_len(fs, rec_len, dir2);
	اگر (retval)
		جاؤ err;

	اگر (csum_سائز) {
		t = EXT2_DIRENT_دم(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_دم(fs, t);
	}

err:
	واپس retval;
}

ساکن errcode_t
ext2fs_inline_ڈیٹا_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      ساخت ext2_inode *inode, حرف *buf, size_t سائز)
{
	errcode_t retval;
	blk64_t blk;
	حرف *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	اگر (retval)
		واپس retval;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_ڈیٹا_DOTDOT_سائز,
					سائز, 0);
	اگر (retval)
		جاؤ errout;
#اگر_ختم

	/* Adjust the rec_len */
	retval = ext2fs_inline_ڈیٹا_convert_dir(fs, ino, blk_buf, buf, سائز);
	اگر (retval)
		جاؤ errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	اگر (retval)
		جاؤ errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	اگر (retval)
		جاؤ errout;

	/* Update inode */
	اگر (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_ڈیٹا_FL;
	retval = ext2fs_iblk_جوڑو_بلاکیں(fs, inode, 1);
	اگر (retval)
		جاؤ errout;
	inode->i_سائز = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_سیٹ, 0, 0, &blk);
	اگر (retval)
		جاؤ errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	اگر (retval)
		جاؤ errout;
	ext2fs_بلاک_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	واپس retval;
}

ساکن errcode_t
ext2fs_inline_ڈیٹا_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       ساخت ext2_inode *inode, حرف *buf, size_t سائز)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_بلاک, 0, حجم(inode->i_بلاک));
	اگر (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		اگر (retval)
			واپس retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_ڈیٹا_FL;
	inode->i_سائز = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	اگر (retval)
		واپس retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	اگر (retval)
		واپس retval;
	retval = ext2fs_file_write(e2_file, buf, سائز, 0);
	ext2fs_file_close(e2_file);
	واپس retval;
}

errcode_t ext2fs_inline_ڈیٹا_expand(ext2_filsys fs, ext2_ino_t ino)
{
	ساخت ext2_inode inode;
	ساخت ext2_inline_ڈیٹا ڈیٹا;
	errcode_t retval;
	size_t inline_سائز;
	حرف *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		واپس retval;

	اگر (!(inode.i_flags & EXT4_INLINE_ڈیٹا_FL))
		واپس EXT2_ET_NO_INLINE_ڈیٹا;

	ڈیٹا.fs = fs;
	ڈیٹا.ino = ino;
	retval = ext2fs_inline_ڈیٹا_ea_get(&ڈیٹا);
	اگر (retval)
		واپس retval;
	inline_سائز = ڈیٹا.ea_سائز + EXT4_MIN_INLINE_ڈیٹا_سائز;
	retval = ext2fs_get_mem(inline_سائز, &inline_buf);
	اگر (retval)
		جاؤ errout;

	حافظہ_نقل(inline_buf, (خالی *)inode.i_بلاک, EXT4_MIN_INLINE_ڈیٹا_سائز);
	اگر (ڈیٹا.ea_سائز > 0) {
		حافظہ_نقل(inline_buf + EXT4_MIN_INLINE_ڈیٹا_سائز,
		       ڈیٹا.ea_ڈیٹا, ڈیٹا.ea_سائز);
	}

	memset((خالی *)inode.i_بلاک, 0, EXT4_MIN_INLINE_ڈیٹا_سائز);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	اگر (retval)
		جاؤ errout;
	retval = ext2fs_inline_ڈیٹا_ea_remove(fs, ino);
	اگر (retval)
		جاؤ errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		جاؤ errout;

	اگر (LINUX_S_ISDIR(inode.i_منوال)) {
		retval = ext2fs_inline_ڈیٹا_dir_expand(fs, ino, &inode,
						inline_buf, inline_سائز);
	} ورنہ {
		retval = ext2fs_inline_ڈیٹا_file_expand(fs, ino, &inode,
						inline_buf, inline_سائز);
	}

errout:
	اگر (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&ڈیٹا.ea_ڈیٹا);
	واپس retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_ڈیٹا_get(ext2_filsys fs, ext2_ino_t ino,
				 ساخت ext2_inode *inode,
				 خالی *buf, size_t *سائز)
{
	ساخت ext2_inode inode_buf;
	ساخت ext2_inline_ڈیٹا ڈیٹا;
	errcode_t retval;

	اگر (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		اگر (retval)
			واپس retval;
		inode = &inode_buf;
	}

	ڈیٹا.fs = fs;
	ڈیٹا.ino = ino;
	retval = ext2fs_inline_ڈیٹا_ea_get(&ڈیٹا);
	اگر (retval)
		واپس retval;

	حافظہ_نقل(buf, (خالی *)inode->i_بلاک, EXT4_MIN_INLINE_ڈیٹا_سائز);
	اگر (ڈیٹا.ea_سائز > 0)
		حافظہ_نقل((حرف *) buf + EXT4_MIN_INLINE_ڈیٹا_سائز,
		       ڈیٹا.ea_ڈیٹا, ڈیٹا.ea_سائز);

	اگر (سائز)
		*سائز = EXT4_MIN_INLINE_ڈیٹا_سائز + ڈیٹا.ea_سائز;
	ext2fs_free_mem(&ڈیٹا.ea_ڈیٹا);
	واپس 0;
}

errcode_t ext2fs_inline_ڈیٹا_سیٹ(ext2_filsys fs, ext2_ino_t ino,
				 ساخت ext2_inode *inode,
				 خالی *buf, size_t سائز)
{
	ساخت ext2_inode inode_buf;
	ساخت ext2_inline_ڈیٹا ڈیٹا = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_سائز, existing_سائز, free_inode_سائز;

	اگر (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		اگر (retval)
			واپس retval;
		inode = &inode_buf;
	}

	اگر (سائز <= EXT4_MIN_INLINE_ڈیٹا_سائز) {
		حافظہ_نقل((خالی *)inode->i_بلاک, buf, سائز);
	} ورنہ {
		retval = ext2fs_xattr_inode_max_سائز(fs, ino, &free_ea_سائز);
		اگر (retval)
			واپس retval;

		retval = ext2fs_inline_ڈیٹا_سائز(fs, ino, &existing_سائز);
		اگر (retval)
			واپس retval;

		اگر (existing_سائز < EXT4_MIN_INLINE_ڈیٹا_سائز) {
			free_inode_سائز = EXT4_MIN_INLINE_ڈیٹا_سائز -
					  existing_سائز;
		} ورنہ {
			free_inode_سائز = 0;
		}

		اگر (سائز != existing_سائز &&
		    سائز > existing_سائز + free_ea_سائز + free_inode_سائز)
			واپس EXT2_ET_INLINE_ڈیٹا_NO_SPACE;

		حافظہ_نقل((خالی *)inode->i_بلاک, buf, EXT4_MIN_INLINE_ڈیٹا_سائز);
		اگر (سائز > EXT4_MIN_INLINE_ڈیٹا_سائز)
			ڈیٹا.ea_سائز = سائز - EXT4_MIN_INLINE_ڈیٹا_سائز;
		ڈیٹا.ea_ڈیٹا = (حرف *) buf + EXT4_MIN_INLINE_ڈیٹا_سائز;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	اگر (retval)
		واپس retval;
	واپس ext2fs_inline_ڈیٹا_ea_سیٹ(&ڈیٹا);
}

#اگر_تعریف_شدہ DEBUG
#شامل "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#تعریف_کرو BUFF_سائز (64)

ساکن errcode_t file_test(ext2_filsys fs)
{
	ساخت ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t سائز;
	حرف *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	اگر (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		واپس 1;
	}

	memset(&inode, 0, حجم(inode));
	inode.i_flags |= EXT4_INLINE_ڈیٹا_FL;
	inode.i_سائز = EXT4_MIN_INLINE_ڈیٹا_سائز;
	inode.i_منوال = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	اگر (retval) {
		com_err("file_test", retval, "while writing a new inode");
		واپس 1;
	}

	retval = ext2fs_inline_ڈیٹا_init(fs, newfile);
	اگر (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		واپس 1;
	}

	retval = ext2fs_inline_ڈیٹا_سائز(fs, newfile, &سائز);
	اگر (retval) {
		com_err("file_test", retval, "while getting size");
		واپس 1;
	}

	اگر (سائز != EXT4_MIN_INLINE_ڈیٹا_سائز) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		واپس 1;
	}

	ext2fs_get_mem(BUFF_سائز, &buf);
	memset(buf, 'a', BUFF_سائز);
	retval = ext2fs_inline_ڈیٹا_سیٹ(fs, newfile, 0, buf, BUFF_سائز);
	اگر (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		جاؤ err;
	}

	ext2fs_get_mem(BUFF_سائز, &cmpbuf);
	retval = ext2fs_inline_ڈیٹا_get(fs, newfile, 0, cmpbuf, &سائز);
	اگر (retval) {
		com_err("file_test", retval, "while getting inline data");
		جاؤ err;
	}

	اگر (سائز != BUFF_سائز) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			سائز, BUFF_سائز);
		retval = 1;
		جاؤ err;
	}

	اگر (memcmp(buf, cmpbuf, BUFF_سائز)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		جاؤ err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	اگر (retval) {
		com_err("file_test", retval, "while truncating inode");
		جاؤ err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	اگر (inode.i_سائز != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	اگر (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	اگر (buf)
		ext2fs_free_mem(&buf);
	واپس retval;
}

ساکن errcode_t dir_test(ext2_filsys fs)
{
	ثابت حرف *dot_name = ".";
	ثابت حرف *stub_name = "stub";
	ثابت حرف *والد_name = "test";
	ext2_ino_t والد, dir, tmp;
	errcode_t retval;
	حرف dirname[32];
	صحیح i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	اگر (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		واپس retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, والد_name);
	اگر (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", والد_name);
		واپس retval;
	}

	retval = ext2fs_lookup(fs, 11, والد_name, لڑی_لمبائی(والد_name),
			       0, &والد);
	اگر (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", والد_name);
		واپس retval;
	}

	retval = ext2fs_lookup(fs, والد, dot_name, لڑی_لمبائی(dot_name),
			       0, &tmp);
	اگر (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", والد_name);
		واپس retval;
	}

	اگر (والد != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			والد, tmp);
		واپس 1;
	}

	برائے (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, حجم(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, والد, 0, dirname);
		اگر (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			واپس retval;
		}

		retval = ext2fs_lookup(fs, والد, dirname, لڑی_لمبائی(dirname),
				       0, &tmp);
		اگر (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", والد_name);
			واپس retval;
		}

		اگر (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			واپس 1;
		}
	}

	snprintf(dirname, حجم(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, والد, 0, dirname);
	اگر (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		واپس retval;
	}

	retval = ext2fs_expand_dir(fs, والد);
	اگر (retval) {
		com_err("dir_test", retval, "while expanding %s dir", والد_name);
		واپس retval;
	}

	واپس 0;
}

صحیح مین(صحیح argc, حرف *argv[])
{
	ext2_filsys		fs;
	ساخت ext2_super_بلاک param;
	errcode_t		retval;

	/*تیاری*/
	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلاکیں_گنتی_سیٹ(&param, 32768);
	param.s_inodes_گنتی = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_ڈیٹا;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_سائز = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٹ, &param,
				   test_io_manager, &fs);
	اگر (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		باہر_نکلو(1);
	}

	retval = ext2fs_allocate_tables(fs);
	اگر (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		باہر_نکلو(1);
	}

	/* initialize inode cache */
	اگر (!fs->icache) {
		ext2_ino_t پہلا_ino = EXT2_پہلا_INO(fs->super);
		صحیح i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		اگر (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			باہر_نکلو(1);
		}

		/* setup inode cache */
		برائے (i = 0; i < fs->icache->cache_سائز; i++)
			fs->icache->cache[i].ino = پہلا_ino++;
	}

	/* test */
	اگر (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		واپس 1;
	}
	لکھو("tst_inline_data(FILE): OK\n");

	اگر (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		واپس 1;
	}
	لکھو("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	واپس 0;
}
#اگر_ختم
