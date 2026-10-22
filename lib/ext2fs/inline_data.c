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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <limits.h> /* for PATH_MAX */

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2_ext_attr.h"

#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ರಚನೆ ext2_inline_ದತ್ತಾಂಶ {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_ಗಾತ್ರ;	/* the size of inline data in ea area */
	ಶೂನ್ಯ *ea_ದತ್ತಾಂಶ;
};

ಸ್ಥಿರ errcode_t ext2fs_inline_ದತ್ತಾಂಶ_ea_ಗಣ(ರಚನೆ ext2_inline_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ರಚನೆ ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(ದತ್ತಾಂಶ->fs, ದತ್ತಾಂಶ->ino, &handle);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = ext2fs_xattrs_read(handle);
	ಆದರೆ (retval)
		ಹೋಗು err;

	retval = ext2fs_xattr_ಗಣ(handle, "system.data",
				  ದತ್ತಾಂಶ->ea_ದತ್ತಾಂಶ, ದತ್ತಾಂಶ->ea_ಗಾತ್ರ);
err:
	(ಶೂನ್ಯ) ext2fs_xattrs_close(&handle);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t ext2fs_inline_ದತ್ತಾಂಶ_ea_get(ರಚನೆ ext2_inline_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ)
{
	ರಚನೆ ext2_xattr_handle *handle;
	errcode_t retval;

	ದತ್ತಾಂಶ->ea_ಗಾತ್ರ = 0;
	ದತ್ತಾಂಶ->ea_ದತ್ತಾಂಶ = 0;

	retval = ext2fs_xattrs_open(ದತ್ತಾಂಶ->fs, ದತ್ತಾಂಶ->ino, &handle);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = ext2fs_xattrs_read(handle);
	ಆದರೆ (retval)
		ಹೋಗು err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (ಶೂನ್ಯ **)&ದತ್ತಾಂಶ->ea_ದತ್ತಾಂಶ, &ದತ್ತಾಂಶ->ea_ಗಾತ್ರ);
	ಆದರೆ (retval == EXT2_ET_EA_ಕೀಲಿ_NOT_ಸಿಕ್ಕಿತು) {
		ದತ್ತಾಂಶ->ea_ಗಾತ್ರ = 0;
		ದತ್ತಾಂಶ->ea_ದತ್ತಾಂಶ = ಖಾಲಿ;
		retval = 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (retval)
		ಹೋಗು err;

err:
	(ಶೂನ್ಯ) ext2fs_xattrs_close(&handle);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_inline_ದತ್ತಾಂಶ_init(ext2_filsys fs, ext2_ino_t ino)
{
	ರಚನೆ ext2_inline_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;
	ಅಕ್ಷರ ಖಾಲಿಗಳು[1] = { '\0' };

	ದತ್ತಾಂಶ.fs = fs;
	ದತ್ತಾಂಶ.ino = ino;
	ದತ್ತಾಂಶ.ea_ಗಾತ್ರ = 0;
	ದತ್ತಾಂಶ.ea_ದತ್ತಾಂಶ = ಖಾಲಿಗಳು;
	ಹಿಂತಿರುಗಿ ext2fs_inline_ದತ್ತಾಂಶ_ea_ಗಣ(&ದತ್ತಾಂಶ);
}

errcode_t ext2fs_inline_ದತ್ತಾಂಶ_ಗಾತ್ರ(ext2_filsys fs, ext2_ino_t ino, size_t *ಗಾತ್ರ_2)
{
	ರಚನೆ ext2_inode inode;
	ರಚನೆ ext2_inline_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (!(inode.i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL))
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_INLINE_ದತ್ತಾಂಶ;

	ದತ್ತಾಂಶ.fs = fs;
	ದತ್ತಾಂಶ.ino = ino;
	retval = ext2fs_inline_ದತ್ತಾಂಶ_ea_get(&ದತ್ತಾಂಶ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	*ಗಾತ್ರ_2 = EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ + ದತ್ತಾಂಶ.ea_ಗಾತ್ರ;
	ಹಿಂತಿರುಗಿ ext2fs_free_mem(&ದತ್ತಾಂಶ.ea_ದತ್ತಾಂಶ);
}

ಪೂರ್ಣಾಂಕ ext2fs_inline_ದತ್ತಾಂಶ_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ dir_context *ctx;
	ರಚನೆ ext2_inode inode;
	ರಚನೆ ext2_dir_ನಮೂದು dirent;
	ರಚನೆ ext2_inline_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ ret = ಖಂಡ_ABORT;
	e2_blkcnt_t blockcnt = 0;
	ಅಕ್ಷರ *old_buf;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ old_buflen;
	ಪೂರ್ಣಾಂಕ old_flags;

	ctx = (ರಚನೆ dir_context *)priv_ದತ್ತಾಂಶ;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_ದತ್ತಾಂಶ;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (ctx->errcode)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (!(inode.i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_ದತ್ತಾಂಶ;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (!LINUX_S_ISDIR(inode.i_ಬಹುಲಕ)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		ಹೋಗು ಹೊರಹೋಗು;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_ಗಣ_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (ಅಕ್ಷರ *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_ಖಂಡ(fs, 0, blockcnt++, 0, 0, priv_ದತ್ತಾಂಶ);
	ಆದರೆ (ret & ಖಂಡ_ABORT)
		ಹೋಗು ಹೊರಹೋಗು;

	dirent.inode = ext2fs_le32_ಗಮ್ಯ_cpu(inode.i_ಖಂಡ[0]);
	dirent.name_len = 2;
	ext2fs_ಗಣ_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (ಅಕ್ಷರ *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_ಖಂಡ(fs, 0, blockcnt++, 0, 0, priv_ದತ್ತಾಂಶ);
	ಆದರೆ (ret & ಖಂಡ_INLINE_ದತ್ತಾಂಶ_ಬದಲಾಯಿತು) {
		errcode_t err;

		inode.i_ಖಂಡ[0] = ext2fs_cpu_ಗಮ್ಯ_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
		ret &= ~ಖಂಡ_INLINE_ದತ್ತಾಂಶ_ಬದಲಾಯಿತು;
	}
	ಆದರೆ (ret & ಖಂಡ_ABORT)
		ಹೋಗು ಹೊರಹೋಗು;

	ctx->buf = (ಅಕ್ಷರ *)inode.i_ಖಂಡ + EXT4_INLINE_ದತ್ತಾಂಶ_DOTDOT_ಗಾತ್ರ;
	ctx->buflen = EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ - EXT4_INLINE_ದತ್ತಾಂಶ_DOTDOT_ಗಾತ್ರ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	ಆದರೆ (ctx->errcode) {
		ret |= ಖಂಡ_ABORT;
		ಹೋಗು ಹೊರಹೋಗು;
	}
#ಆದರೆ_ಅಂತ್ಯ
	ret |= ext2fs_process_dir_ಖಂಡ(fs, 0, blockcnt++, 0, 0, priv_ದತ್ತಾಂಶ);
	ಆದರೆ (ret & ಖಂಡ_INLINE_ದತ್ತಾಂಶ_ಬದಲಾಯಿತು) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		ಆದರೆ (ctx->errcode) {
			ret |= ಖಂಡ_ABORT;
			ಹೋಗು ಹೊರಹೋಗು;
		}
#ಆದರೆ_ಅಂತ್ಯ
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		ಆದರೆ (ctx->errcode)
			ret |= ಖಂಡ_ABORT;
		ret &= ~ಖಂಡ_INLINE_ದತ್ತಾಂಶ_ಬದಲಾಯಿತು;
	}
	ಆದರೆ (ret & ಖಂಡ_ABORT)
		ಹೋಗು ಹೊರಹೋಗು;

	ದತ್ತಾಂಶ.fs = fs;
	ದತ್ತಾಂಶ.ino = ino;
	ctx->errcode = ext2fs_inline_ದತ್ತಾಂಶ_ea_get(&ದತ್ತಾಂಶ);
	ಆದರೆ (ctx->errcode) {
		ret |= ಖಂಡ_ABORT;
		ಹೋಗು ಹೊರಹೋಗು;
	}
	ಆದರೆ (ದತ್ತಾಂಶ.ea_ಗಾತ್ರ <= 0)
		ಹೋಗು out1;

	ctx->buf = ದತ್ತಾಂಶ.ea_ದತ್ತಾಂಶ;
	ctx->buflen = ದತ್ತಾಂಶ.ea_ಗಾತ್ರ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	ಆದರೆ (ctx->errcode) {
		ret |= ಖಂಡ_ABORT;
		ಹೋಗು out1;
	}
#ಆದರೆ_ಅಂತ್ಯ

	ret |= ext2fs_process_dir_ಖಂಡ(fs, 0, blockcnt++, 0, 0, priv_ದತ್ತಾಂಶ);
	ಆದರೆ (ret & ಖಂಡ_INLINE_ದತ್ತಾಂಶ_ಬದಲಾಯಿತು) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		ಆದರೆ (ctx->errcode) {
			ret |= ಖಂಡ_ABORT;
			ಹೋಗು out1;
		}
#ಆದರೆ_ಅಂತ್ಯ
		ctx->errcode = ext2fs_inline_ದತ್ತಾಂಶ_ea_ಗಣ(&ದತ್ತಾಂಶ);
		ಆದರೆ (ctx->errcode)
			ret |= ಖಂಡ_ABORT;
	}

out1:
	ext2fs_free_mem(&ದತ್ತಾಂಶ.ea_ದತ್ತಾಂಶ);
ಹೊರಹೋಗು:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(ಖಂಡ_ABORT | ಖಂಡ_INLINE_ದತ್ತಾಂಶ_ಬದಲಾಯಿತು);
	ಹಿಂತಿರುಗಿ ret;
}

errcode_t ext2fs_inline_ದತ್ತಾಂಶ_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	ರಚನೆ ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = ext2fs_xattrs_read(handle);
	ಆದರೆ (retval)
		ಹೋಗು err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(ಶೂನ್ಯ) ext2fs_xattrs_close(&handle);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t ext2fs_inline_ದತ್ತಾಂಶ_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						ಅಕ್ಷರ *bbuf, ಅಕ್ಷರ *ibuf, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ರಚನೆ ext2_dir_ನಮೂದು *dir, *dir2;
	ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ *t;
	errcode_t retval;
	ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rec_len;
	ಪೂರ್ಣಾಂಕ csum_ಗಾತ್ರ = 0;
	ಪೂರ್ಣಾಂಕ filetype = 0;

	ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ);

	/* Create '.' and '..' */
	ಆದರೆ (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (ರಚನೆ ext2_dir_ನಮೂದು *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_ಗಣ_name_len(dir, 1);
	ext2fs_dirent_ಗಣ_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_ಗಾತ್ರ) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (ರಚನೆ ext2_dir_ನಮೂದು *) (bbuf + dir->rec_len);
	dir->rec_len = EXT2_DIR_REC_LEN(2);
	dir->inode = ext2fs_le32_ಗಮ್ಯ_cpu(((__u32 *)ibuf)[0]);
	ext2fs_dirent_ಗಣ_name_len(dir, 2);
	ext2fs_dirent_ಗಣ_file_type(dir, filetype);
	dir->name[0] = '.';
	dir->name[1] = '.';

	/*
	 * Adjust the last rec_len
	 */
	ಸ್ಥಾನಾಂತರ = EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2);
	dir = (ರಚನೆ ext2_dir_ನಮೂದು *) (bbuf + ಸ್ಥಾನಾಂತರ);
	ಸ್ಮರಣೆ_ನಕಲು(bbuf + ಸ್ಥಾನಾಂತರ, ibuf + EXT4_INLINE_ದತ್ತಾಂಶ_DOTDOT_ಗಾತ್ರ,
	       ಗಾತ್ರ_2 - EXT4_INLINE_ದತ್ತಾಂಶ_DOTDOT_ಗಾತ್ರ);
	ಗಾತ್ರ_2 += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_ದತ್ತಾಂಶ_DOTDOT_ಗಾತ್ರ;

	ಮಾಡು {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		ಆದರೆ (retval)
			ಹೋಗು err;
		ಸ್ಥಾನಾಂತರ += rec_len;
		dir = (ರಚನೆ ext2_dir_ನಮೂದು *) (bbuf + ಸ್ಥಾನಾಂತರ);
	} ಆಗ (ಸ್ಥಾನಾಂತರ < ಗಾತ್ರ_2);
	rec_len += fs->blocksize - csum_ಗಾತ್ರ - ಸ್ಥಾನಾಂತರ;
	retval = ext2fs_ಗಣ_rec_len(fs, rec_len, dir2);
	ಆದರೆ (retval)
		ಹೋಗು err;

	ಆದರೆ (csum_ಗಾತ್ರ) {
		t = EXT2_DIRENT_ಬಾಲ(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_ಬಾಲ(fs, t);
	}

err:
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t
ext2fs_inline_ದತ್ತಾಂಶ_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      ರಚನೆ ext2_inode *inode, ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2)
{
	errcode_t retval;
	blk64_t blk;
	ಅಕ್ಷರ *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_ದತ್ತಾಂಶ_DOTDOT_ಗಾತ್ರ,
					ಗಾತ್ರ_2, 0);
	ಆದರೆ (retval)
		ಹೋಗು errout;
#ಆದರೆ_ಅಂತ್ಯ

	/* Adjust the rec_len */
	retval = ext2fs_inline_ದತ್ತಾಂಶ_convert_dir(fs, ino, blk_buf, buf, ಗಾತ್ರ_2);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	/* Update inode */
	ಆದರೆ (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_ದತ್ತಾಂಶ_FL;
	retval = ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(fs, inode, 1);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	inode->i_ಗಾತ್ರ = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_ಗಣ, 0, 0, &blk);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	ext2fs_ಖಂಡ_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t
ext2fs_inline_ದತ್ತಾಂಶ_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       ರಚನೆ ext2_inode *inode, ಅಕ್ಷರ *buf, size_t ಗಾತ್ರ_2)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_ಖಂಡ, 0, ಗಾತ್ರ(inode->i_ಖಂಡ));
	ಆದರೆ (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_ದತ್ತಾಂಶ_FL;
	inode->i_ಗಾತ್ರ = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	retval = ext2fs_file_write(e2_file, buf, ಗಾತ್ರ_2, 0);
	ext2fs_file_close(e2_file);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_inline_ದತ್ತಾಂಶ_expand(ext2_filsys fs, ext2_ino_t ino)
{
	ರಚನೆ ext2_inode inode;
	ರಚನೆ ext2_inline_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;
	errcode_t retval;
	size_t inline_ಗಾತ್ರ;
	ಅಕ್ಷರ *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (!(inode.i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL))
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_INLINE_ದತ್ತಾಂಶ;

	ದತ್ತಾಂಶ.fs = fs;
	ದತ್ತಾಂಶ.ino = ino;
	retval = ext2fs_inline_ದತ್ತಾಂಶ_ea_get(&ದತ್ತಾಂಶ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	inline_ಗಾತ್ರ = ದತ್ತಾಂಶ.ea_ಗಾತ್ರ + EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ;
	retval = ext2fs_get_mem(inline_ಗಾತ್ರ, &inline_buf);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	ಸ್ಮರಣೆ_ನಕಲು(inline_buf, (ಶೂನ್ಯ *)inode.i_ಖಂಡ, EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ);
	ಆದರೆ (ದತ್ತಾಂಶ.ea_ಗಾತ್ರ > 0) {
		ಸ್ಮರಣೆ_ನಕಲು(inline_buf + EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ,
		       ದತ್ತಾಂಶ.ea_ದತ್ತಾಂಶ, ದತ್ತಾಂಶ.ea_ಗಾತ್ರ);
	}

	memset((ಶೂನ್ಯ *)inode.i_ಖಂಡ, 0, EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	retval = ext2fs_inline_ದತ್ತಾಂಶ_ea_remove(fs, ino);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	ಆದರೆ (LINUX_S_ISDIR(inode.i_ಬಹುಲಕ)) {
		retval = ext2fs_inline_ದತ್ತಾಂಶ_dir_expand(fs, ino, &inode,
						inline_buf, inline_ಗಾತ್ರ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_inline_ದತ್ತಾಂಶ_file_expand(fs, ino, &inode,
						inline_buf, inline_ಗಾತ್ರ);
	}

errout:
	ಆದರೆ (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&ದತ್ತಾಂಶ.ea_ದತ್ತಾಂಶ);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_ದತ್ತಾಂಶ_get(ext2_filsys fs, ext2_ino_t ino,
				 ರಚನೆ ext2_inode *inode,
				 ಶೂನ್ಯ *buf, size_t *ಗಾತ್ರ_2)
{
	ರಚನೆ ext2_inode inode_buf;
	ರಚನೆ ext2_inline_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;
	errcode_t retval;

	ಆದರೆ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		inode = &inode_buf;
	}

	ದತ್ತಾಂಶ.fs = fs;
	ದತ್ತಾಂಶ.ino = ino;
	retval = ext2fs_inline_ದತ್ತಾಂಶ_ea_get(&ದತ್ತಾಂಶ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಸ್ಮರಣೆ_ನಕಲು(buf, (ಶೂನ್ಯ *)inode->i_ಖಂಡ, EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ);
	ಆದರೆ (ದತ್ತಾಂಶ.ea_ಗಾತ್ರ > 0)
		ಸ್ಮರಣೆ_ನಕಲು((ಅಕ್ಷರ *) buf + EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ,
		       ದತ್ತಾಂಶ.ea_ದತ್ತಾಂಶ, ದತ್ತಾಂಶ.ea_ಗಾತ್ರ);

	ಆದರೆ (ಗಾತ್ರ_2)
		*ಗಾತ್ರ_2 = EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ + ದತ್ತಾಂಶ.ea_ಗಾತ್ರ;
	ext2fs_free_mem(&ದತ್ತಾಂಶ.ea_ದತ್ತಾಂಶ);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_inline_ದತ್ತಾಂಶ_ಗಣ(ext2_filsys fs, ext2_ino_t ino,
				 ರಚನೆ ext2_inode *inode,
				 ಶೂನ್ಯ *buf, size_t ಗಾತ್ರ_2)
{
	ರಚನೆ ext2_inode inode_buf;
	ರಚನೆ ext2_inline_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_ಗಾತ್ರ, existing_ಗಾತ್ರ, free_inode_ಗಾತ್ರ;

	ಆದರೆ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		inode = &inode_buf;
	}

	ಆದರೆ (ಗಾತ್ರ_2 <= EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ) {
		ಸ್ಮರಣೆ_ನಕಲು((ಶೂನ್ಯ *)inode->i_ಖಂಡ, buf, ಗಾತ್ರ_2);
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_xattr_inode_max_ಗಾತ್ರ(fs, ino, &free_ea_ಗಾತ್ರ);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		retval = ext2fs_inline_ದತ್ತಾಂಶ_ಗಾತ್ರ(fs, ino, &existing_ಗಾತ್ರ);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		ಆದರೆ (existing_ಗಾತ್ರ < EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ) {
			free_inode_ಗಾತ್ರ = EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ -
					  existing_ಗಾತ್ರ;
		} ಇಲ್ಲದಿದ್ದರೆ {
			free_inode_ಗಾತ್ರ = 0;
		}

		ಆದರೆ (ಗಾತ್ರ_2 != existing_ಗಾತ್ರ &&
		    ಗಾತ್ರ_2 > existing_ಗಾತ್ರ + free_ea_ಗಾತ್ರ + free_inode_ಗಾತ್ರ)
			ಹಿಂತಿರುಗಿ EXT2_ET_INLINE_ದತ್ತಾಂಶ_NO_SPACE;

		ಸ್ಮರಣೆ_ನಕಲು((ಶೂನ್ಯ *)inode->i_ಖಂಡ, buf, EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ);
		ಆದರೆ (ಗಾತ್ರ_2 > EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ)
			ದತ್ತಾಂಶ.ea_ಗಾತ್ರ = ಗಾತ್ರ_2 - EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ;
		ದತ್ತಾಂಶ.ea_ದತ್ತಾಂಶ = (ಅಕ್ಷರ *) buf + EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಹಿಂತಿರುಗಿ ext2fs_inline_ದತ್ತಾಂಶ_ea_ಗಣ(&ದತ್ತಾಂಶ);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
#ಅನ್ತರ್ಭಾವ "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು BUFF_ಗಾತ್ರ (64)

ಸ್ಥಿರ errcode_t file_test(ext2_filsys fs)
{
	ರಚನೆ ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t ಗಾತ್ರ_2;
	ಅಕ್ಷರ *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		ಹಿಂತಿರುಗಿ 1;
	}

	memset(&inode, 0, ಗಾತ್ರ(inode));
	inode.i_flags |= EXT4_INLINE_ದತ್ತಾಂಶ_FL;
	inode.i_ಗಾತ್ರ = EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ;
	inode.i_ಬಹುಲಕ = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while writing a new inode");
		ಹಿಂತಿರುಗಿ 1;
	}

	retval = ext2fs_inline_ದತ್ತಾಂಶ_init(fs, newfile);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		ಹಿಂತಿರುಗಿ 1;
	}

	retval = ext2fs_inline_ದತ್ತಾಂಶ_ಗಾತ್ರ(fs, newfile, &ಗಾತ್ರ_2);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while getting size");
		ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (ಗಾತ್ರ_2 != EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		ಹಿಂತಿರುಗಿ 1;
	}

	ext2fs_get_mem(BUFF_ಗಾತ್ರ, &buf);
	memset(buf, 'a', BUFF_ಗಾತ್ರ);
	retval = ext2fs_inline_ದತ್ತಾಂಶ_ಗಣ(fs, newfile, 0, buf, BUFF_ಗಾತ್ರ);
	ಆದರೆ (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		ಹೋಗು err;
	}

	ext2fs_get_mem(BUFF_ಗಾತ್ರ, &cmpbuf);
	retval = ext2fs_inline_ದತ್ತಾಂಶ_get(fs, newfile, 0, cmpbuf, &ಗಾತ್ರ_2);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while getting inline data");
		ಹೋಗು err;
	}

	ಆದರೆ (ಗಾತ್ರ_2 != BUFF_ಗಾತ್ರ) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			ಗಾತ್ರ_2, BUFF_ಗಾತ್ರ);
		retval = 1;
		ಹೋಗು err;
	}

	ಆದರೆ (memcmp(buf, cmpbuf, BUFF_ಗಾತ್ರ)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		ಹೋಗು err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while truncating inode");
		ಹೋಗು err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	ಆದರೆ (inode.i_ಗಾತ್ರ != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	ಆದರೆ (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	ಆದರೆ (buf)
		ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t dir_test(ext2_filsys fs)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dot_name = ".";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stub_name = "stub";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೋಷಕ_name = "test";
	ext2_ino_t ಪೋಷಕ, dir, tmp;
	errcode_t retval;
	ಅಕ್ಷರ dirname[32];
	ಪೂರ್ಣಾಂಕ i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	ಆದರೆ (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		ಹಿಂತಿರುಗಿ retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, ಪೋಷಕ_name);
	ಆದರೆ (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", ಪೋಷಕ_name);
		ಹಿಂತಿರುಗಿ retval;
	}

	retval = ext2fs_lookup(fs, 11, ಪೋಷಕ_name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೋಷಕ_name),
			       0, &ಪೋಷಕ);
	ಆದರೆ (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", ಪೋಷಕ_name);
		ಹಿಂತಿರುಗಿ retval;
	}

	retval = ext2fs_lookup(fs, ಪೋಷಕ, dot_name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dot_name),
			       0, &tmp);
	ಆದರೆ (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", ಪೋಷಕ_name);
		ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ (ಪೋಷಕ != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			ಪೋಷಕ, tmp);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಗಾಗಿ (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, ಗಾತ್ರ(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, ಪೋಷಕ, 0, dirname);
		ಆದರೆ (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			ಹಿಂತಿರುಗಿ retval;
		}

		retval = ext2fs_lookup(fs, ಪೋಷಕ, dirname, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(dirname),
				       0, &tmp);
		ಆದರೆ (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", ಪೋಷಕ_name);
			ಹಿಂತಿರುಗಿ retval;
		}

		ಆದರೆ (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			ಹಿಂತಿರುಗಿ 1;
		}
	}

	snprintf(dirname, ಗಾತ್ರ(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, ಪೋಷಕ, 0, dirname);
	ಆದರೆ (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		ಹಿಂತಿರುಗಿ retval;
	}

	retval = ext2fs_expand_dir(fs, ಪೋಷಕ);
	ಆದರೆ (retval) {
		com_err("dir_test", retval, "while expanding %s dir", ಪೋಷಕ_name);
		ಹಿಂತಿರುಗಿ retval;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ *argv[])
{
	ext2_filsys		fs;
	ರಚನೆ ext2_super_ಖಂಡ param;
	errcode_t		retval;

	/*ಸಿದ್ಧತೆ*/
	initialize_ext2_error_ಕೋಷ್ಟಕ();

	memset(&param, 0, ಗಾತ್ರ(param));
	ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(&param, 32768);
	param.s_inodes_ಎಣಿಕೆ = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_ದತ್ತಾಂಶ;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_ಗಾತ್ರ = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು, &param,
				   test_io_manager, &fs);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		ನಿರ್ಗಮಿಸು(1);
	}

	retval = ext2fs_allocate_tables(fs);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		ನಿರ್ಗಮಿಸು(1);
	}

	/* initialize inode cache */
	ಆದರೆ (!fs->icache) {
		ext2_ino_t ಮೊದಲ_ino = EXT2_ಮೊದಲ_INO(fs->super);
		ಪೂರ್ಣಾಂಕ i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		ಆದರೆ (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			ನಿರ್ಗಮಿಸು(1);
		}

		/* setup inode cache */
		ಗಾಗಿ (i = 0; i < fs->icache->cache_ಗಾತ್ರ; i++)
			fs->icache->cache[i].ino = ಮೊದಲ_ino++;
	}

	/* test */
	ಆದರೆ (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		ಹಿಂತಿರುಗಿ 1;
	}
	ಬರೆ("tst_inline_data(FILE): OK\n");

	ಆದರೆ (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		ಹಿಂತಿರುಗಿ 1;
	}
	ಬರೆ("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ
