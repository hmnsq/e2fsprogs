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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h> /* for PATH_MAX */

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_ext_attr.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਬਣਤਰ ext2_inline_ਅੰਕੜੇ {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_ਮਾਪ;	/* the size of inline data in ea area */
	ਖਾਲੀ *ea_ਅੰਕੜੇ;
};

ਸਥਿਰ errcode_t ext2fs_inline_ਅੰਕੜੇ_ea_ਸੈੱਟ(ਬਣਤਰ ext2_inline_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਬਣਤਰ ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(ਅੰਕੜੇ->fs, ਅੰਕੜੇ->ino, &handle);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = ext2fs_xattrs_read(handle);
	ਜੇ (retval)
		ਜਾਓ err;

	retval = ext2fs_xattr_ਸੈੱਟ(handle, "system.data",
				  ਅੰਕੜੇ->ea_ਅੰਕੜੇ, ਅੰਕੜੇ->ea_ਮਾਪ);
err:
	(ਖਾਲੀ) ext2fs_xattrs_close(&handle);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t ext2fs_inline_ਅੰਕੜੇ_ea_get(ਬਣਤਰ ext2_inline_ਅੰਕੜੇ *ਅੰਕੜੇ)
{
	ਬਣਤਰ ext2_xattr_handle *handle;
	errcode_t retval;

	ਅੰਕੜੇ->ea_ਮਾਪ = 0;
	ਅੰਕੜੇ->ea_ਅੰਕੜੇ = 0;

	retval = ext2fs_xattrs_open(ਅੰਕੜੇ->fs, ਅੰਕੜੇ->ino, &handle);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = ext2fs_xattrs_read(handle);
	ਜੇ (retval)
		ਜਾਓ err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (ਖਾਲੀ **)&ਅੰਕੜੇ->ea_ਅੰਕੜੇ, &ਅੰਕੜੇ->ea_ਮਾਪ);
	ਜੇ (retval == EXT2_ET_EA_ਕੁੰਜੀ_NOT_ਮਿਲਿਆ) {
		ਅੰਕੜੇ->ea_ਮਾਪ = 0;
		ਅੰਕੜੇ->ea_ਅੰਕੜੇ = ਨਿਰਾ;
		retval = 0;
	} ਨਹੀਂਤਾਂ ਜੇ (retval)
		ਜਾਓ err;

err:
	(ਖਾਲੀ) ext2fs_xattrs_close(&handle);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_inline_ਅੰਕੜੇ_init(ext2_filsys fs, ext2_ino_t ino)
{
	ਬਣਤਰ ext2_inline_ਅੰਕੜੇ ਅੰਕੜੇ;
	ਅੱਖਰ ਖਾਲੀ_ਥਾਵਾਂ[1] = { '\0' };

	ਅੰਕੜੇ.fs = fs;
	ਅੰਕੜੇ.ino = ino;
	ਅੰਕੜੇ.ea_ਮਾਪ = 0;
	ਅੰਕੜੇ.ea_ਅੰਕੜੇ = ਖਾਲੀ_ਥਾਵਾਂ;
	ਵਾਪਸ ext2fs_inline_ਅੰਕੜੇ_ea_ਸੈੱਟ(&ਅੰਕੜੇ);
}

errcode_t ext2fs_inline_ਅੰਕੜੇ_ਮਾਪ(ext2_filsys fs, ext2_ino_t ino, size_t *ਮਾਪ)
{
	ਬਣਤਰ ext2_inode inode;
	ਬਣਤਰ ext2_inline_ਅੰਕੜੇ ਅੰਕੜੇ;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (!(inode.i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL))
		ਵਾਪਸ EXT2_ET_NO_INLINE_ਅੰਕੜੇ;

	ਅੰਕੜੇ.fs = fs;
	ਅੰਕੜੇ.ino = ino;
	retval = ext2fs_inline_ਅੰਕੜੇ_ea_get(&ਅੰਕੜੇ);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	*ਮਾਪ = EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ + ਅੰਕੜੇ.ea_ਮਾਪ;
	ਵਾਪਸ ext2fs_free_mem(&ਅੰਕੜੇ.ea_ਅੰਕੜੇ);
}

ਪੂਰਨ ext2fs_inline_ਅੰਕੜੇ_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ dir_context *ctx;
	ਬਣਤਰ ext2_inode inode;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ dirent;
	ਬਣਤਰ ext2_inline_ਅੰਕੜੇ ਅੰਕੜੇ;
	ਪੂਰਨ ret = ਖੰਡ_ABORT;
	e2_blkcnt_t blockcnt = 0;
	ਅੱਖਰ *old_buf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_buflen;
	ਪੂਰਨ old_flags;

	ctx = (ਬਣਤਰ dir_context *)priv_ਅੰਕੜੇ;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_ਅੰਕੜੇ;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (ctx->errcode)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (!(inode.i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_ਅੰਕੜੇ;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (!LINUX_S_ISDIR(inode.i_ਬਹੁਲਕ)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_ਸੈੱਟ_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (ਅੱਖਰ *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_ਖੰਡ(fs, 0, blockcnt++, 0, 0, priv_ਅੰਕੜੇ);
	ਜੇ (ret & ਖੰਡ_ABORT)
		ਜਾਓ ਬਾਹਰਲਾ;

	dirent.inode = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(inode.i_ਖੰਡ[0]);
	dirent.name_len = 2;
	ext2fs_ਸੈੱਟ_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (ਅੱਖਰ *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_ਖੰਡ(fs, 0, blockcnt++, 0, 0, priv_ਅੰਕੜੇ);
	ਜੇ (ret & ਖੰਡ_INLINE_ਅੰਕੜੇ_ਬਦਲਿਆ) {
		errcode_t err;

		inode.i_ਖੰਡ[0] = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
		ret &= ~ਖੰਡ_INLINE_ਅੰਕੜੇ_ਬਦਲਿਆ;
	}
	ਜੇ (ret & ਖੰਡ_ABORT)
		ਜਾਓ ਬਾਹਰਲਾ;

	ctx->buf = (ਅੱਖਰ *)inode.i_ਖੰਡ + EXT4_INLINE_ਅੰਕੜੇ_DOTDOT_ਮਾਪ;
	ctx->buflen = EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ - EXT4_INLINE_ਅੰਕੜੇ_DOTDOT_ਮਾਪ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	ਜੇ (ctx->errcode) {
		ret |= ਖੰਡ_ABORT;
		ਜਾਓ ਬਾਹਰਲਾ;
	}
#ਜੇ_ਅੰਤ
	ret |= ext2fs_process_dir_ਖੰਡ(fs, 0, blockcnt++, 0, 0, priv_ਅੰਕੜੇ);
	ਜੇ (ret & ਖੰਡ_INLINE_ਅੰਕੜੇ_ਬਦਲਿਆ) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		ਜੇ (ctx->errcode) {
			ret |= ਖੰਡ_ABORT;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
#ਜੇ_ਅੰਤ
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		ਜੇ (ctx->errcode)
			ret |= ਖੰਡ_ABORT;
		ret &= ~ਖੰਡ_INLINE_ਅੰਕੜੇ_ਬਦਲਿਆ;
	}
	ਜੇ (ret & ਖੰਡ_ABORT)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਅੰਕੜੇ.fs = fs;
	ਅੰਕੜੇ.ino = ino;
	ctx->errcode = ext2fs_inline_ਅੰਕੜੇ_ea_get(&ਅੰਕੜੇ);
	ਜੇ (ctx->errcode) {
		ret |= ਖੰਡ_ABORT;
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	ਜੇ (ਅੰਕੜੇ.ea_ਮਾਪ <= 0)
		ਜਾਓ out1;

	ctx->buf = ਅੰਕੜੇ.ea_ਅੰਕੜੇ;
	ctx->buflen = ਅੰਕੜੇ.ea_ਮਾਪ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	ਜੇ (ctx->errcode) {
		ret |= ਖੰਡ_ABORT;
		ਜਾਓ out1;
	}
#ਜੇ_ਅੰਤ

	ret |= ext2fs_process_dir_ਖੰਡ(fs, 0, blockcnt++, 0, 0, priv_ਅੰਕੜੇ);
	ਜੇ (ret & ਖੰਡ_INLINE_ਅੰਕੜੇ_ਬਦਲਿਆ) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		ਜੇ (ctx->errcode) {
			ret |= ਖੰਡ_ABORT;
			ਜਾਓ out1;
		}
#ਜੇ_ਅੰਤ
		ctx->errcode = ext2fs_inline_ਅੰਕੜੇ_ea_ਸੈੱਟ(&ਅੰਕੜੇ);
		ਜੇ (ctx->errcode)
			ret |= ਖੰਡ_ABORT;
	}

out1:
	ext2fs_free_mem(&ਅੰਕੜੇ.ea_ਅੰਕੜੇ);
ਬਾਹਰਲਾ:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(ਖੰਡ_ABORT | ਖੰਡ_INLINE_ਅੰਕੜੇ_ਬਦਲਿਆ);
	ਵਾਪਸ ret;
}

errcode_t ext2fs_inline_ਅੰਕੜੇ_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	ਬਣਤਰ ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = ext2fs_xattrs_read(handle);
	ਜੇ (retval)
		ਜਾਓ err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(ਖਾਲੀ) ext2fs_xattrs_close(&handle);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t ext2fs_inline_ਅੰਕੜੇ_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						ਅੱਖਰ *bbuf, ਅੱਖਰ *ibuf, ਪੂਰਨ ਮਾਪ)
{
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dir, *dir2;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ *t;
	errcode_t retval;
	ਪੂਰਨ ਵਿੱਥ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rec_len;
	ਪੂਰਨ csum_ਮਾਪ = 0;
	ਪੂਰਨ filetype = 0;

	ਜੇ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ);

	/* Create '.' and '..' */
	ਜੇ (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_ਸੈੱਟ_name_len(dir, 1);
	ext2fs_dirent_ਸੈੱਟ_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_ਮਾਪ) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) (bbuf + dir->rec_len);
	dir->rec_len = EXT2_DIR_REC_LEN(2);
	dir->inode = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(((__u32 *)ibuf)[0]);
	ext2fs_dirent_ਸੈੱਟ_name_len(dir, 2);
	ext2fs_dirent_ਸੈੱਟ_file_type(dir, filetype);
	dir->name[0] = '.';
	dir->name[1] = '.';

	/*
	 * Adjust the last rec_len
	 */
	ਵਿੱਥ = EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2);
	dir = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) (bbuf + ਵਿੱਥ);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(bbuf + ਵਿੱਥ, ibuf + EXT4_INLINE_ਅੰਕੜੇ_DOTDOT_ਮਾਪ,
	       ਮਾਪ - EXT4_INLINE_ਅੰਕੜੇ_DOTDOT_ਮਾਪ);
	ਮਾਪ += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_ਅੰਕੜੇ_DOTDOT_ਮਾਪ;

	ਕਰੋ {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		ਜੇ (retval)
			ਜਾਓ err;
		ਵਿੱਥ += rec_len;
		dir = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) (bbuf + ਵਿੱਥ);
	} ਜਦੋਂ (ਵਿੱਥ < ਮਾਪ);
	rec_len += fs->blocksize - csum_ਮਾਪ - ਵਿੱਥ;
	retval = ext2fs_ਸੈੱਟ_rec_len(fs, rec_len, dir2);
	ਜੇ (retval)
		ਜਾਓ err;

	ਜੇ (csum_ਮਾਪ) {
		t = EXT2_DIRENT_ਪੂਛ(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_ਪੂਛ(fs, t);
	}

err:
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t
ext2fs_inline_ਅੰਕੜੇ_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      ਬਣਤਰ ext2_inode *inode, ਅੱਖਰ *buf, size_t ਮਾਪ)
{
	errcode_t retval;
	blk64_t blk;
	ਅੱਖਰ *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_ਅੰਕੜੇ_DOTDOT_ਮਾਪ,
					ਮਾਪ, 0);
	ਜੇ (retval)
		ਜਾਓ errout;
#ਜੇ_ਅੰਤ

	/* Adjust the rec_len */
	retval = ext2fs_inline_ਅੰਕੜੇ_convert_dir(fs, ino, blk_buf, buf, ਮਾਪ);
	ਜੇ (retval)
		ਜਾਓ errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	ਜੇ (retval)
		ਜਾਓ errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	ਜੇ (retval)
		ਜਾਓ errout;

	/* Update inode */
	ਜੇ (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_ਅੰਕੜੇ_FL;
	retval = ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(fs, inode, 1);
	ਜੇ (retval)
		ਜਾਓ errout;
	inode->i_ਮਾਪ = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_ਸੈੱਟ, 0, 0, &blk);
	ਜੇ (retval)
		ਜਾਓ errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	ਜੇ (retval)
		ਜਾਓ errout;
	ext2fs_ਖੰਡ_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t
ext2fs_inline_ਅੰਕੜੇ_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       ਬਣਤਰ ext2_inode *inode, ਅੱਖਰ *buf, size_t ਮਾਪ)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_ਖੰਡ, 0, ਆਕਾਰ(inode->i_ਖੰਡ));
	ਜੇ (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_ਅੰਕੜੇ_FL;
	inode->i_ਮਾਪ = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	retval = ext2fs_file_write(e2_file, buf, ਮਾਪ, 0);
	ext2fs_file_close(e2_file);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_inline_ਅੰਕੜੇ_expand(ext2_filsys fs, ext2_ino_t ino)
{
	ਬਣਤਰ ext2_inode inode;
	ਬਣਤਰ ext2_inline_ਅੰਕੜੇ ਅੰਕੜੇ;
	errcode_t retval;
	size_t inline_ਮਾਪ;
	ਅੱਖਰ *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (!(inode.i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL))
		ਵਾਪਸ EXT2_ET_NO_INLINE_ਅੰਕੜੇ;

	ਅੰਕੜੇ.fs = fs;
	ਅੰਕੜੇ.ino = ino;
	retval = ext2fs_inline_ਅੰਕੜੇ_ea_get(&ਅੰਕੜੇ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	inline_ਮਾਪ = ਅੰਕੜੇ.ea_ਮਾਪ + EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ;
	retval = ext2fs_get_mem(inline_ਮਾਪ, &inline_buf);
	ਜੇ (retval)
		ਜਾਓ errout;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(inline_buf, (ਖਾਲੀ *)inode.i_ਖੰਡ, EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ);
	ਜੇ (ਅੰਕੜੇ.ea_ਮਾਪ > 0) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(inline_buf + EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ,
		       ਅੰਕੜੇ.ea_ਅੰਕੜੇ, ਅੰਕੜੇ.ea_ਮਾਪ);
	}

	memset((ਖਾਲੀ *)inode.i_ਖੰਡ, 0, EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਜਾਓ errout;
	retval = ext2fs_inline_ਅੰਕੜੇ_ea_remove(fs, ino);
	ਜੇ (retval)
		ਜਾਓ errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਜਾਓ errout;

	ਜੇ (LINUX_S_ISDIR(inode.i_ਬਹੁਲਕ)) {
		retval = ext2fs_inline_ਅੰਕੜੇ_dir_expand(fs, ino, &inode,
						inline_buf, inline_ਮਾਪ);
	} ਨਹੀਂਤਾਂ {
		retval = ext2fs_inline_ਅੰਕੜੇ_file_expand(fs, ino, &inode,
						inline_buf, inline_ਮਾਪ);
	}

errout:
	ਜੇ (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&ਅੰਕੜੇ.ea_ਅੰਕੜੇ);
	ਵਾਪਸ retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_ਅੰਕੜੇ_get(ext2_filsys fs, ext2_ino_t ino,
				 ਬਣਤਰ ext2_inode *inode,
				 ਖਾਲੀ *buf, size_t *ਮਾਪ)
{
	ਬਣਤਰ ext2_inode inode_buf;
	ਬਣਤਰ ext2_inline_ਅੰਕੜੇ ਅੰਕੜੇ;
	errcode_t retval;

	ਜੇ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		inode = &inode_buf;
	}

	ਅੰਕੜੇ.fs = fs;
	ਅੰਕੜੇ.ino = ino;
	retval = ext2fs_inline_ਅੰਕੜੇ_ea_get(&ਅੰਕੜੇ);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, (ਖਾਲੀ *)inode->i_ਖੰਡ, EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ);
	ਜੇ (ਅੰਕੜੇ.ea_ਮਾਪ > 0)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ((ਅੱਖਰ *) buf + EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ,
		       ਅੰਕੜੇ.ea_ਅੰਕੜੇ, ਅੰਕੜੇ.ea_ਮਾਪ);

	ਜੇ (ਮਾਪ)
		*ਮਾਪ = EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ + ਅੰਕੜੇ.ea_ਮਾਪ;
	ext2fs_free_mem(&ਅੰਕੜੇ.ea_ਅੰਕੜੇ);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_inline_ਅੰਕੜੇ_ਸੈੱਟ(ext2_filsys fs, ext2_ino_t ino,
				 ਬਣਤਰ ext2_inode *inode,
				 ਖਾਲੀ *buf, size_t ਮਾਪ)
{
	ਬਣਤਰ ext2_inode inode_buf;
	ਬਣਤਰ ext2_inline_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_ਮਾਪ, existing_ਮਾਪ, free_inode_ਮਾਪ;

	ਜੇ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		inode = &inode_buf;
	}

	ਜੇ (ਮਾਪ <= EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ((ਖਾਲੀ *)inode->i_ਖੰਡ, buf, ਮਾਪ);
	} ਨਹੀਂਤਾਂ {
		retval = ext2fs_xattr_inode_max_ਮਾਪ(fs, ino, &free_ea_ਮਾਪ);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		retval = ext2fs_inline_ਅੰਕੜੇ_ਮਾਪ(fs, ino, &existing_ਮਾਪ);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		ਜੇ (existing_ਮਾਪ < EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ) {
			free_inode_ਮਾਪ = EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ -
					  existing_ਮਾਪ;
		} ਨਹੀਂਤਾਂ {
			free_inode_ਮਾਪ = 0;
		}

		ਜੇ (ਮਾਪ != existing_ਮਾਪ &&
		    ਮਾਪ > existing_ਮਾਪ + free_ea_ਮਾਪ + free_inode_ਮਾਪ)
			ਵਾਪਸ EXT2_ET_INLINE_ਅੰਕੜੇ_NO_SPACE;

		ਸਮ੍ਰਿਤੀ_ਨਕਲ((ਖਾਲੀ *)inode->i_ਖੰਡ, buf, EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ);
		ਜੇ (ਮਾਪ > EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ)
			ਅੰਕੜੇ.ea_ਮਾਪ = ਮਾਪ - EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ;
		ਅੰਕੜੇ.ea_ਅੰਕੜੇ = (ਅੱਖਰ *) buf + EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਵਾਪਸ ext2fs_inline_ਅੰਕੜੇ_ea_ਸੈੱਟ(&ਅੰਕੜੇ);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
#ਅਨ੍ਤਰ੍ਭਾਵ "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUFF_ਮਾਪ (64)

ਸਥਿਰ errcode_t file_test(ext2_filsys fs)
{
	ਬਣਤਰ ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t ਮਾਪ;
	ਅੱਖਰ *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	ਜੇ (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		ਵਾਪਸ 1;
	}

	memset(&inode, 0, ਆਕਾਰ(inode));
	inode.i_flags |= EXT4_INLINE_ਅੰਕੜੇ_FL;
	inode.i_ਮਾਪ = EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ;
	inode.i_ਬਹੁਲਕ = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	ਜੇ (retval) {
		com_err("file_test", retval, "while writing a new inode");
		ਵਾਪਸ 1;
	}

	retval = ext2fs_inline_ਅੰਕੜੇ_init(fs, newfile);
	ਜੇ (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		ਵਾਪਸ 1;
	}

	retval = ext2fs_inline_ਅੰਕੜੇ_ਮਾਪ(fs, newfile, &ਮਾਪ);
	ਜੇ (retval) {
		com_err("file_test", retval, "while getting size");
		ਵਾਪਸ 1;
	}

	ਜੇ (ਮਾਪ != EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		ਵਾਪਸ 1;
	}

	ext2fs_get_mem(BUFF_ਮਾਪ, &buf);
	memset(buf, 'a', BUFF_ਮਾਪ);
	retval = ext2fs_inline_ਅੰਕੜੇ_ਸੈੱਟ(fs, newfile, 0, buf, BUFF_ਮਾਪ);
	ਜੇ (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		ਜਾਓ err;
	}

	ext2fs_get_mem(BUFF_ਮਾਪ, &cmpbuf);
	retval = ext2fs_inline_ਅੰਕੜੇ_get(fs, newfile, 0, cmpbuf, &ਮਾਪ);
	ਜੇ (retval) {
		com_err("file_test", retval, "while getting inline data");
		ਜਾਓ err;
	}

	ਜੇ (ਮਾਪ != BUFF_ਮਾਪ) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			ਮਾਪ, BUFF_ਮਾਪ);
		retval = 1;
		ਜਾਓ err;
	}

	ਜੇ (memcmp(buf, cmpbuf, BUFF_ਮਾਪ)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		ਜਾਓ err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	ਜੇ (retval) {
		com_err("file_test", retval, "while truncating inode");
		ਜਾਓ err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	ਜੇ (inode.i_ਮਾਪ != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	ਜੇ (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	ਜੇ (buf)
		ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t dir_test(ext2_filsys fs)
{
	ਅਚਲ ਅੱਖਰ *dot_name = ".";
	ਅਚਲ ਅੱਖਰ *stub_name = "stub";
	ਅਚਲ ਅੱਖਰ *ਮਾਪਾ_name = "test";
	ext2_ino_t ਮਾਪਾ, dir, tmp;
	errcode_t retval;
	ਅੱਖਰ dirname[32];
	ਪੂਰਨ i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	ਜੇ (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		ਵਾਪਸ retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, ਮਾਪਾ_name);
	ਜੇ (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", ਮਾਪਾ_name);
		ਵਾਪਸ retval;
	}

	retval = ext2fs_lookup(fs, 11, ਮਾਪਾ_name, ਲੜੀ_ਲੰਬਾਈ(ਮਾਪਾ_name),
			       0, &ਮਾਪਾ);
	ਜੇ (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", ਮਾਪਾ_name);
		ਵਾਪਸ retval;
	}

	retval = ext2fs_lookup(fs, ਮਾਪਾ, dot_name, ਲੜੀ_ਲੰਬਾਈ(dot_name),
			       0, &tmp);
	ਜੇ (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", ਮਾਪਾ_name);
		ਵਾਪਸ retval;
	}

	ਜੇ (ਮਾਪਾ != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			ਮਾਪਾ, tmp);
		ਵਾਪਸ 1;
	}

	ਲਈ (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, ਆਕਾਰ(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, ਮਾਪਾ, 0, dirname);
		ਜੇ (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			ਵਾਪਸ retval;
		}

		retval = ext2fs_lookup(fs, ਮਾਪਾ, dirname, ਲੜੀ_ਲੰਬਾਈ(dirname),
				       0, &tmp);
		ਜੇ (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", ਮਾਪਾ_name);
			ਵਾਪਸ retval;
		}

		ਜੇ (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			ਵਾਪਸ 1;
		}
	}

	snprintf(dirname, ਆਕਾਰ(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, ਮਾਪਾ, 0, dirname);
	ਜੇ (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		ਵਾਪਸ retval;
	}

	retval = ext2fs_expand_dir(fs, ਮਾਪਾ);
	ਜੇ (retval) {
		com_err("dir_test", retval, "while expanding %s dir", ਮਾਪਾ_name);
		ਵਾਪਸ retval;
	}

	ਵਾਪਸ 0;
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ *argv[])
{
	ext2_filsys		fs;
	ਬਣਤਰ ext2_super_ਖੰਡ param;
	errcode_t		retval;

	/*ਤਿਆਰੀ*/
	initialize_ext2_error_ਸਾਰਣੀ();

	memset(&param, 0, ਆਕਾਰ(param));
	ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(&param, 32768);
	param.s_inodes_ਗਿਣਤੀ = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_ਅੰਕੜੇ;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_ਮਾਪ = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_ਬਿੱਟ, &param,
				   test_io_manager, &fs);
	ਜੇ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	retval = ext2fs_allocate_tables(fs);
	ਜੇ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	/* initialize inode cache */
	ਜੇ (!fs->icache) {
		ext2_ino_t ਪਹਿਲਾ_ino = EXT2_ਪਹਿਲਾ_INO(fs->super);
		ਪੂਰਨ i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		ਜੇ (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}

		/* setup inode cache */
		ਲਈ (i = 0; i < fs->icache->cache_ਮਾਪ; i++)
			fs->icache->cache[i].ino = ਪਹਿਲਾ_ino++;
	}

	/* test */
	ਜੇ (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		ਵਾਪਸ 1;
	}
	ਲਿਖੋ("tst_inline_data(FILE): OK\n");

	ਜੇ (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		ਵਾਪਸ 1;
	}
	ਲਿਖੋ("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ
