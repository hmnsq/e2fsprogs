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

بنية ext2_inline_انکڑے {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_ناپ;	/* the size of inline data in ea area */
	فراغ *ea_انکڑے;
};

ساكن errcode_t ext2fs_inline_انکڑے_ea_سیٹ(بنية ext2_inline_انکڑے *انکڑے)
{
	بنية ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(انکڑے->fs, انکڑے->ino, &handle);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_xattrs_read(handle);
	إذا (retval)
		اذهب err;

	retval = ext2fs_xattr_سیٹ(handle, "system.data",
				  انکڑے->ea_انکڑے, انکڑے->ea_ناپ);
err:
	(فراغ) ext2fs_xattrs_close(&handle);
	أرجع retval;
}

ساكن errcode_t ext2fs_inline_انکڑے_ea_get(بنية ext2_inline_انکڑے *انکڑے)
{
	بنية ext2_xattr_handle *handle;
	errcode_t retval;

	انکڑے->ea_ناپ = 0;
	انکڑے->ea_انکڑے = 0;

	retval = ext2fs_xattrs_open(انکڑے->fs, انکڑے->ino, &handle);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_xattrs_read(handle);
	إذا (retval)
		اذهب err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (فراغ **)&انکڑے->ea_انکڑے, &انکڑے->ea_ناپ);
	إذا (retval == EXT2_ET_EA_کلید_NOT_لبھیا) {
		انکڑے->ea_ناپ = 0;
		انکڑے->ea_انکڑے = عدم;
		retval = 0;
	} وإلا إذا (retval)
		اذهب err;

err:
	(فراغ) ext2fs_xattrs_close(&handle);
	أرجع retval;
}

errcode_t ext2fs_inline_انکڑے_init(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_inline_انکڑے انکڑے;
	حرف خالی_تھاواں[1] = { '\0' };

	انکڑے.fs = fs;
	انکڑے.ino = ino;
	انکڑے.ea_ناپ = 0;
	انکڑے.ea_انکڑے = خالی_تھاواں;
	أرجع ext2fs_inline_انکڑے_ea_سیٹ(&انکڑے);
}

errcode_t ext2fs_inline_انکڑے_ناپ(ext2_filsys fs, ext2_ino_t ino, size_t *ناپ)
{
	بنية ext2_inode inode;
	بنية ext2_inline_انکڑے انکڑے;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;

	إذا (!(inode.i_flags & EXT4_INLINE_انکڑے_FL))
		أرجع EXT2_ET_NO_INLINE_انکڑے;

	انکڑے.fs = fs;
	انکڑے.ino = ino;
	retval = ext2fs_inline_انکڑے_ea_get(&انکڑے);
	إذا (retval)
		أرجع retval;

	*ناپ = EXT4_MIN_INLINE_انکڑے_ناپ + انکڑے.ea_ناپ;
	أرجع ext2fs_free_mem(&انکڑے.ea_انکڑے);
}

صحيح ext2fs_inline_انکڑے_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   فراغ *priv_انکڑے)
{
	بنية dir_context *ctx;
	بنية ext2_inode inode;
	بنية ext2_dir_اندراج dirent;
	بنية ext2_inline_انکڑے انکڑے;
	صحيح ret = بلاک_ABORT;
	e2_blkcnt_t blockcnt = 0;
	حرف *old_buf;
	بنانشانی صحيح old_buflen;
	صحيح old_flags;

	ctx = (بنية dir_context *)priv_انکڑے;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_انکڑے;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	إذا (ctx->errcode)
		اذهب باہرلا;

	إذا (!(inode.i_flags & EXT4_INLINE_انکڑے_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_انکڑے;
		اذهب باہرلا;
	}

	إذا (!LINUX_S_ISDIR(inode.i_منوال)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		اذهب باہرلا;
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
	ret |= ext2fs_process_dir_بلاک(fs, 0, blockcnt++, 0, 0, priv_انکڑے);
	إذا (ret & بلاک_ABORT)
		اذهب باہرلا;

	dirent.inode = ext2fs_le32_منزل_cpu(inode.i_بلاک[0]);
	dirent.name_len = 2;
	ext2fs_سیٹ_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (حرف *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_بلاک(fs, 0, blockcnt++, 0, 0, priv_انکڑے);
	إذا (ret & بلاک_INLINE_انکڑے_بدلیا) {
		errcode_t err;

		inode.i_بلاک[0] = ext2fs_cpu_منزل_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		إذا (err)
			اذهب باہرلا;
		ret &= ~بلاک_INLINE_انکڑے_بدلیا;
	}
	إذا (ret & بلاک_ABORT)
		اذهب باہرلا;

	ctx->buf = (حرف *)inode.i_بلاک + EXT4_INLINE_انکڑے_DOTDOT_ناپ;
	ctx->buflen = EXT4_MIN_INLINE_انکڑے_ناپ - EXT4_INLINE_انکڑے_DOTDOT_ناپ;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	إذا (ctx->errcode) {
		ret |= بلاک_ABORT;
		اذهب باہرلا;
	}
#جے_انت
	ret |= ext2fs_process_dir_بلاک(fs, 0, blockcnt++, 0, 0, priv_انکڑے);
	إذا (ret & بلاک_INLINE_انکڑے_بدلیا) {
#جے_تعریف_شدہ الفاظ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		إذا (ctx->errcode) {
			ret |= بلاک_ABORT;
			اذهب باہرلا;
		}
#جے_انت
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		إذا (ctx->errcode)
			ret |= بلاک_ABORT;
		ret &= ~بلاک_INLINE_انکڑے_بدلیا;
	}
	إذا (ret & بلاک_ABORT)
		اذهب باہرلا;

	انکڑے.fs = fs;
	انکڑے.ino = ino;
	ctx->errcode = ext2fs_inline_انکڑے_ea_get(&انکڑے);
	إذا (ctx->errcode) {
		ret |= بلاک_ABORT;
		اذهب باہرلا;
	}
	إذا (انکڑے.ea_ناپ <= 0)
		اذهب out1;

	ctx->buf = انکڑے.ea_انکڑے;
	ctx->buflen = انکڑے.ea_ناپ;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	إذا (ctx->errcode) {
		ret |= بلاک_ABORT;
		اذهب out1;
	}
#جے_انت

	ret |= ext2fs_process_dir_بلاک(fs, 0, blockcnt++, 0, 0, priv_انکڑے);
	إذا (ret & بلاک_INLINE_انکڑے_بدلیا) {
#جے_تعریف_شدہ الفاظ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		إذا (ctx->errcode) {
			ret |= بلاک_ABORT;
			اذهب out1;
		}
#جے_انت
		ctx->errcode = ext2fs_inline_انکڑے_ea_سیٹ(&انکڑے);
		إذا (ctx->errcode)
			ret |= بلاک_ABORT;
	}

out1:
	ext2fs_free_mem(&انکڑے.ea_انکڑے);
باہرلا:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(بلاک_ABORT | بلاک_INLINE_انکڑے_بدلیا);
	أرجع ret;
}

errcode_t ext2fs_inline_انکڑے_ea_remove(ext2_filsys fs, ext2_ino_t ino)
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

ساكن errcode_t ext2fs_inline_انکڑے_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						حرف *bbuf, حرف *ibuf, صحيح ناپ)
{
	بنية ext2_dir_اندراج *dir, *dir2;
	بنية ext2_dir_اندراج_پوچھ *t;
	errcode_t retval;
	صحيح وتھ;
	بنانشانی صحيح rec_len;
	صحيح csum_ناپ = 0;
	صحيح filetype = 0;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ناپ = حجم(بنية ext2_dir_اندراج_پوچھ);

	/* Create '.' and '..' */
	إذا (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (بنية ext2_dir_اندراج *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_سیٹ_name_len(dir, 1);
	ext2fs_dirent_سیٹ_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_ناپ) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (بنية ext2_dir_اندراج *) (bbuf + dir->rec_len);
	dir->rec_len = EXT2_DIR_REC_LEN(2);
	dir->inode = ext2fs_le32_منزل_cpu(((__u32 *)ibuf)[0]);
	ext2fs_dirent_سیٹ_name_len(dir, 2);
	ext2fs_dirent_سیٹ_file_type(dir, filetype);
	dir->name[0] = '.';
	dir->name[1] = '.';

	/*
	 * Adjust the last rec_len
	 */
	وتھ = EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2);
	dir = (بنية ext2_dir_اندراج *) (bbuf + وتھ);
	حافظہ_نقل(bbuf + وتھ, ibuf + EXT4_INLINE_انکڑے_DOTDOT_ناپ,
	       ناپ - EXT4_INLINE_انکڑے_DOTDOT_ناپ);
	ناپ += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_انکڑے_DOTDOT_ناپ;

	افعل {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		إذا (retval)
			اذهب err;
		وتھ += rec_len;
		dir = (بنية ext2_dir_اندراج *) (bbuf + وتھ);
	} طالما (وتھ < ناپ);
	rec_len += fs->blocksize - csum_ناپ - وتھ;
	retval = ext2fs_سیٹ_rec_len(fs, rec_len, dir2);
	إذا (retval)
		اذهب err;

	إذا (csum_ناپ) {
		t = EXT2_DIRENT_پوچھ(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_پوچھ(fs, t);
	}

err:
	أرجع retval;
}

ساكن errcode_t
ext2fs_inline_انکڑے_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode, حرف *buf, size_t ناپ)
{
	errcode_t retval;
	blk64_t blk;
	حرف *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	إذا (retval)
		أرجع retval;

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_انکڑے_DOTDOT_ناپ,
					ناپ, 0);
	إذا (retval)
		اذهب errout;
#جے_انت

	/* Adjust the rec_len */
	retval = ext2fs_inline_انکڑے_convert_dir(fs, ino, blk_buf, buf, ناپ);
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
	inode->i_flags &= ~EXT4_INLINE_انکڑے_FL;
	retval = ext2fs_iblk_جوڑو_بلاکاں(fs, inode, 1);
	إذا (retval)
		اذهب errout;
	inode->i_ناپ = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_سیٹ, 0, 0, &blk);
	إذا (retval)
		اذهب errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		اذهب errout;
	ext2fs_بلاک_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	أرجع retval;
}

ساكن errcode_t
ext2fs_inline_انکڑے_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode *inode, حرف *buf, size_t ناپ)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_بلاک, 0, حجم(inode->i_بلاک));
	إذا (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		إذا (retval)
			أرجع retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_انکڑے_FL;
	inode->i_ناپ = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		أرجع retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_file_write(e2_file, buf, ناپ, 0);
	ext2fs_file_close(e2_file);
	أرجع retval;
}

errcode_t ext2fs_inline_انکڑے_expand(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_inode inode;
	بنية ext2_inline_انکڑے انکڑے;
	errcode_t retval;
	size_t inline_ناپ;
	حرف *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;

	إذا (!(inode.i_flags & EXT4_INLINE_انکڑے_FL))
		أرجع EXT2_ET_NO_INLINE_انکڑے;

	انکڑے.fs = fs;
	انکڑے.ino = ino;
	retval = ext2fs_inline_انکڑے_ea_get(&انکڑے);
	إذا (retval)
		أرجع retval;
	inline_ناپ = انکڑے.ea_ناپ + EXT4_MIN_INLINE_انکڑے_ناپ;
	retval = ext2fs_get_mem(inline_ناپ, &inline_buf);
	إذا (retval)
		اذهب errout;

	حافظہ_نقل(inline_buf, (فراغ *)inode.i_بلاک, EXT4_MIN_INLINE_انکڑے_ناپ);
	إذا (انکڑے.ea_ناپ > 0) {
		حافظہ_نقل(inline_buf + EXT4_MIN_INLINE_انکڑے_ناپ,
		       انکڑے.ea_انکڑے, انکڑے.ea_ناپ);
	}

	memset((فراغ *)inode.i_بلاک, 0, EXT4_MIN_INLINE_انکڑے_ناپ);
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
	retval = ext2fs_inline_انکڑے_ea_remove(fs, ino);
	إذا (retval)
		اذهب errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		اذهب errout;

	إذا (LINUX_S_ISDIR(inode.i_منوال)) {
		retval = ext2fs_inline_انکڑے_dir_expand(fs, ino, &inode,
						inline_buf, inline_ناپ);
	} وإلا {
		retval = ext2fs_inline_انکڑے_file_expand(fs, ino, &inode,
						inline_buf, inline_ناپ);
	}

errout:
	إذا (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&انکڑے.ea_انکڑے);
	أرجع retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_انکڑے_get(ext2_filsys fs, ext2_ino_t ino,
				 بنية ext2_inode *inode,
				 فراغ *buf, size_t *ناپ)
{
	بنية ext2_inode inode_buf;
	بنية ext2_inline_انکڑے انکڑے;
	errcode_t retval;

	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}

	انکڑے.fs = fs;
	انکڑے.ino = ino;
	retval = ext2fs_inline_انکڑے_ea_get(&انکڑے);
	إذا (retval)
		أرجع retval;

	حافظہ_نقل(buf, (فراغ *)inode->i_بلاک, EXT4_MIN_INLINE_انکڑے_ناپ);
	إذا (انکڑے.ea_ناپ > 0)
		حافظہ_نقل((حرف *) buf + EXT4_MIN_INLINE_انکڑے_ناپ,
		       انکڑے.ea_انکڑے, انکڑے.ea_ناپ);

	إذا (ناپ)
		*ناپ = EXT4_MIN_INLINE_انکڑے_ناپ + انکڑے.ea_ناپ;
	ext2fs_free_mem(&انکڑے.ea_انکڑے);
	أرجع 0;
}

errcode_t ext2fs_inline_انکڑے_سیٹ(ext2_filsys fs, ext2_ino_t ino,
				 بنية ext2_inode *inode,
				 فراغ *buf, size_t ناپ)
{
	بنية ext2_inode inode_buf;
	بنية ext2_inline_انکڑے انکڑے = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_ناپ, existing_ناپ, free_inode_ناپ;

	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}

	إذا (ناپ <= EXT4_MIN_INLINE_انکڑے_ناپ) {
		حافظہ_نقل((فراغ *)inode->i_بلاک, buf, ناپ);
	} وإلا {
		retval = ext2fs_xattr_inode_max_ناپ(fs, ino, &free_ea_ناپ);
		إذا (retval)
			أرجع retval;

		retval = ext2fs_inline_انکڑے_ناپ(fs, ino, &existing_ناپ);
		إذا (retval)
			أرجع retval;

		إذا (existing_ناپ < EXT4_MIN_INLINE_انکڑے_ناپ) {
			free_inode_ناپ = EXT4_MIN_INLINE_انکڑے_ناپ -
					  existing_ناپ;
		} وإلا {
			free_inode_ناپ = 0;
		}

		إذا (ناپ != existing_ناپ &&
		    ناپ > existing_ناپ + free_ea_ناپ + free_inode_ناپ)
			أرجع EXT2_ET_INLINE_انکڑے_NO_SPACE;

		حافظہ_نقل((فراغ *)inode->i_بلاک, buf, EXT4_MIN_INLINE_انکڑے_ناپ);
		إذا (ناپ > EXT4_MIN_INLINE_انکڑے_ناپ)
			انکڑے.ea_ناپ = ناپ - EXT4_MIN_INLINE_انکڑے_ناپ;
		انکڑے.ea_انکڑے = (حرف *) buf + EXT4_MIN_INLINE_انکڑے_ناپ;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		أرجع retval;
	أرجع ext2fs_inline_انکڑے_ea_سیٹ(&انکڑے);
}

#جے_تعریف_شدہ DEBUG
#ضمن "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#تعریف_کرو BUFF_ناپ (64)

ساكن errcode_t file_test(ext2_filsys fs)
{
	بنية ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t ناپ;
	حرف *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	إذا (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		أرجع 1;
	}

	memset(&inode, 0, حجم(inode));
	inode.i_flags |= EXT4_INLINE_انکڑے_FL;
	inode.i_ناپ = EXT4_MIN_INLINE_انکڑے_ناپ;
	inode.i_منوال = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	إذا (retval) {
		com_err("file_test", retval, "while writing a new inode");
		أرجع 1;
	}

	retval = ext2fs_inline_انکڑے_init(fs, newfile);
	إذا (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		أرجع 1;
	}

	retval = ext2fs_inline_انکڑے_ناپ(fs, newfile, &ناپ);
	إذا (retval) {
		com_err("file_test", retval, "while getting size");
		أرجع 1;
	}

	إذا (ناپ != EXT4_MIN_INLINE_انکڑے_ناپ) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		أرجع 1;
	}

	ext2fs_get_mem(BUFF_ناپ, &buf);
	memset(buf, 'a', BUFF_ناپ);
	retval = ext2fs_inline_انکڑے_سیٹ(fs, newfile, 0, buf, BUFF_ناپ);
	إذا (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		اذهب err;
	}

	ext2fs_get_mem(BUFF_ناپ, &cmpbuf);
	retval = ext2fs_inline_انکڑے_get(fs, newfile, 0, cmpbuf, &ناپ);
	إذا (retval) {
		com_err("file_test", retval, "while getting inline data");
		اذهب err;
	}

	إذا (ناپ != BUFF_ناپ) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			ناپ, BUFF_ناپ);
		retval = 1;
		اذهب err;
	}

	إذا (memcmp(buf, cmpbuf, BUFF_ناپ)) {
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
	إذا (inode.i_ناپ != 0) {
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
	ثابت حرف *ماپا_name = "test";
	ext2_ino_t ماپا, dir, tmp;
	errcode_t retval;
	حرف dirname[32];
	صحيح i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	إذا (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		أرجع retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, ماپا_name);
	إذا (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", ماپا_name);
		أرجع retval;
	}

	retval = ext2fs_lookup(fs, 11, ماپا_name, لڑی_لمبائی(ماپا_name),
			       0, &ماپا);
	إذا (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", ماپا_name);
		أرجع retval;
	}

	retval = ext2fs_lookup(fs, ماپا, dot_name, لڑی_لمبائی(dot_name),
			       0, &tmp);
	إذا (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", ماپا_name);
		أرجع retval;
	}

	إذا (ماپا != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			ماپا, tmp);
		أرجع 1;
	}

	لكل (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, حجم(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, ماپا, 0, dirname);
		إذا (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			أرجع retval;
		}

		retval = ext2fs_lookup(fs, ماپا, dirname, لڑی_لمبائی(dirname),
				       0, &tmp);
		إذا (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", ماپا_name);
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
	retval = ext2fs_mkdir(fs, ماپا, 0, dirname);
	إذا (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		أرجع retval;
	}

	retval = ext2fs_expand_dir(fs, ماپا);
	إذا (retval) {
		com_err("dir_test", retval, "while expanding %s dir", ماپا_name);
		أرجع retval;
	}

	أرجع 0;
}

صحيح مکھ(صحيح argc, حرف *argv[])
{
	ext2_filsys		fs;
	بنية ext2_super_بلاک param;
	errcode_t		retval;

	/*تیاری*/
	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلاکاں_گنتی_سیٹ(&param, 32768);
	param.s_inodes_گنتی = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_انکڑے;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_ناپ = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٹ, &param,
				   test_io_manager, &fs);
	إذا (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		باہر_نکلو(1);
	}

	retval = ext2fs_allocate_tables(fs);
	إذا (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		باہر_نکلو(1);
	}

	/* initialize inode cache */
	إذا (!fs->icache) {
		ext2_ino_t پہلا_ino = EXT2_پہلا_INO(fs->super);
		صحيح i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		إذا (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			باہر_نکلو(1);
		}

		/* setup inode cache */
		لكل (i = 0; i < fs->icache->cache_ناپ; i++)
			fs->icache->cache[i].ino = پہلا_ino++;
	}

	/* test */
	إذا (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		أرجع 1;
	}
	لکھو("tst_inline_data(FILE): OK\n");

	إذا (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		أرجع 1;
	}
	لکھو("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	أرجع 0;
}
#جے_انت
