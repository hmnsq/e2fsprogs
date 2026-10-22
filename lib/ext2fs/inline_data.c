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

بنية ext2_inline_معلومات {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_سائيز;	/* the size of inline data in ea area */
	فراغ *ea_معلومات;
};

ساكن errcode_t ext2fs_inline_معلومات_ea_سيٽ(بنية ext2_inline_معلومات *معلومات)
{
	بنية ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(معلومات->fs, معلومات->ino, &handle);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_xattrs_read(handle);
	إذا (retval)
		اذهب err;

	retval = ext2fs_xattr_سيٽ(handle, "system.data",
				  معلومات->ea_معلومات, معلومات->ea_سائيز);
err:
	(فراغ) ext2fs_xattrs_close(&handle);
	أرجع retval;
}

ساكن errcode_t ext2fs_inline_معلومات_ea_get(بنية ext2_inline_معلومات *معلومات)
{
	بنية ext2_xattr_handle *handle;
	errcode_t retval;

	معلومات->ea_سائيز = 0;
	معلومات->ea_معلومات = 0;

	retval = ext2fs_xattrs_open(معلومات->fs, معلومات->ino, &handle);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_xattrs_read(handle);
	إذا (retval)
		اذهب err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (فراغ **)&معلومات->ea_معلومات, &معلومات->ea_سائيز);
	إذا (retval == EXT2_ET_EA_ڪنجي_NOT_مليو) {
		معلومات->ea_سائيز = 0;
		معلومات->ea_معلومات = عدم;
		retval = 0;
	} وإلا إذا (retval)
		اذهب err;

err:
	(فراغ) ext2fs_xattrs_close(&handle);
	أرجع retval;
}

errcode_t ext2fs_inline_معلومات_init(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_inline_معلومات معلومات;
	حرف خالي_جايون[1] = { '\0' };

	معلومات.fs = fs;
	معلومات.ino = ino;
	معلومات.ea_سائيز = 0;
	معلومات.ea_معلومات = خالي_جايون;
	أرجع ext2fs_inline_معلومات_ea_سيٽ(&معلومات);
}

errcode_t ext2fs_inline_معلومات_سائيز(ext2_filsys fs, ext2_ino_t ino, size_t *سائيز)
{
	بنية ext2_inode inode;
	بنية ext2_inline_معلومات معلومات;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;

	إذا (!(inode.i_flags & EXT4_INLINE_معلومات_FL))
		أرجع EXT2_ET_NO_INLINE_معلومات;

	معلومات.fs = fs;
	معلومات.ino = ino;
	retval = ext2fs_inline_معلومات_ea_get(&معلومات);
	إذا (retval)
		أرجع retval;

	*سائيز = EXT4_MIN_INLINE_معلومات_سائيز + معلومات.ea_سائيز;
	أرجع ext2fs_free_mem(&معلومات.ea_معلومات);
}

صحيح ext2fs_inline_معلومات_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   فراغ *priv_معلومات)
{
	بنية dir_context *ctx;
	بنية ext2_inode inode;
	بنية ext2_dir_داخلا dirent;
	بنية ext2_inline_معلومات معلومات;
	صحيح ret = بلاڪ_ABORT;
	e2_blkcnt_t blockcnt = 0;
	حرف *old_buf;
	بنانشان صحيح old_buflen;
	صحيح old_flags;

	ctx = (بنية dir_context *)priv_معلومات;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_معلومات;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	إذا (ctx->errcode)
		اذهب ٻاهريون;

	إذا (!(inode.i_flags & EXT4_INLINE_معلومات_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_معلومات;
		اذهب ٻاهريون;
	}

	إذا (!LINUX_S_ISDIR(inode.i_گهڻائي)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		اذهب ٻاهريون;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_سيٽ_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (حرف *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_بلاڪ(fs, 0, blockcnt++, 0, 0, priv_معلومات);
	إذا (ret & بلاڪ_ABORT)
		اذهب ٻاهريون;

	dirent.inode = ext2fs_le32_منزل_cpu(inode.i_بلاڪ[0]);
	dirent.name_len = 2;
	ext2fs_سيٽ_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (حرف *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_بلاڪ(fs, 0, blockcnt++, 0, 0, priv_معلومات);
	إذا (ret & بلاڪ_INLINE_معلومات_بدليو) {
		errcode_t err;

		inode.i_بلاڪ[0] = ext2fs_cpu_منزل_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		إذا (err)
			اذهب ٻاهريون;
		ret &= ~بلاڪ_INLINE_معلومات_بدليو;
	}
	إذا (ret & بلاڪ_ABORT)
		اذهب ٻاهريون;

	ctx->buf = (حرف *)inode.i_بلاڪ + EXT4_INLINE_معلومات_DOTDOT_سائيز;
	ctx->buflen = EXT4_MIN_INLINE_معلومات_سائيز - EXT4_INLINE_معلومات_DOTDOT_سائيز;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	إذا (ctx->errcode) {
		ret |= بلاڪ_ABORT;
		اذهب ٻاهريون;
	}
#جيڪڏهن_پورو
	ret |= ext2fs_process_dir_بلاڪ(fs, 0, blockcnt++, 0, 0, priv_معلومات);
	إذا (ret & بلاڪ_INLINE_معلومات_بدليو) {
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		إذا (ctx->errcode) {
			ret |= بلاڪ_ABORT;
			اذهب ٻاهريون;
		}
#جيڪڏهن_پورو
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		إذا (ctx->errcode)
			ret |= بلاڪ_ABORT;
		ret &= ~بلاڪ_INLINE_معلومات_بدليو;
	}
	إذا (ret & بلاڪ_ABORT)
		اذهب ٻاهريون;

	معلومات.fs = fs;
	معلومات.ino = ino;
	ctx->errcode = ext2fs_inline_معلومات_ea_get(&معلومات);
	إذا (ctx->errcode) {
		ret |= بلاڪ_ABORT;
		اذهب ٻاهريون;
	}
	إذا (معلومات.ea_سائيز <= 0)
		اذهب out1;

	ctx->buf = معلومات.ea_معلومات;
	ctx->buflen = معلومات.ea_سائيز;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	إذا (ctx->errcode) {
		ret |= بلاڪ_ABORT;
		اذهب out1;
	}
#جيڪڏهن_پورو

	ret |= ext2fs_process_dir_بلاڪ(fs, 0, blockcnt++, 0, 0, priv_معلومات);
	إذا (ret & بلاڪ_INLINE_معلومات_بدليو) {
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		إذا (ctx->errcode) {
			ret |= بلاڪ_ABORT;
			اذهب out1;
		}
#جيڪڏهن_پورو
		ctx->errcode = ext2fs_inline_معلومات_ea_سيٽ(&معلومات);
		إذا (ctx->errcode)
			ret |= بلاڪ_ABORT;
	}

out1:
	ext2fs_free_mem(&معلومات.ea_معلومات);
ٻاهريون:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(بلاڪ_ABORT | بلاڪ_INLINE_معلومات_بدليو);
	أرجع ret;
}

errcode_t ext2fs_inline_معلومات_ea_remove(ext2_filsys fs, ext2_ino_t ino)
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

ساكن errcode_t ext2fs_inline_معلومات_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						حرف *bbuf, حرف *ibuf, صحيح سائيز)
{
	بنية ext2_dir_داخلا *dir, *dir2;
	بنية ext2_dir_داخلا_پڇ *t;
	errcode_t retval;
	صحيح وٿي;
	بنانشان صحيح rec_len;
	صحيح csum_سائيز = 0;
	صحيح filetype = 0;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائيز = حجم(بنية ext2_dir_داخلا_پڇ);

	/* Create '.' and '..' */
	إذا (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (بنية ext2_dir_داخلا *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_سيٽ_name_len(dir, 1);
	ext2fs_dirent_سيٽ_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_سائيز) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (بنية ext2_dir_داخلا *) (bbuf + dir->rec_len);
	dir->rec_len = EXT2_DIR_REC_LEN(2);
	dir->inode = ext2fs_le32_منزل_cpu(((__u32 *)ibuf)[0]);
	ext2fs_dirent_سيٽ_name_len(dir, 2);
	ext2fs_dirent_سيٽ_file_type(dir, filetype);
	dir->name[0] = '.';
	dir->name[1] = '.';

	/*
	 * Adjust the last rec_len
	 */
	وٿي = EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2);
	dir = (بنية ext2_dir_داخلا *) (bbuf + وٿي);
	يادگيري_نقل(bbuf + وٿي, ibuf + EXT4_INLINE_معلومات_DOTDOT_سائيز,
	       سائيز - EXT4_INLINE_معلومات_DOTDOT_سائيز);
	سائيز += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_معلومات_DOTDOT_سائيز;

	افعل {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		إذا (retval)
			اذهب err;
		وٿي += rec_len;
		dir = (بنية ext2_dir_داخلا *) (bbuf + وٿي);
	} طالما (وٿي < سائيز);
	rec_len += fs->blocksize - csum_سائيز - وٿي;
	retval = ext2fs_سيٽ_rec_len(fs, rec_len, dir2);
	إذا (retval)
		اذهب err;

	إذا (csum_سائيز) {
		t = EXT2_DIRENT_پڇ(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_پڇ(fs, t);
	}

err:
	أرجع retval;
}

ساكن errcode_t
ext2fs_inline_معلومات_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode *inode, حرف *buf, size_t سائيز)
{
	errcode_t retval;
	blk64_t blk;
	حرف *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	إذا (retval)
		أرجع retval;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_معلومات_DOTDOT_سائيز,
					سائيز, 0);
	إذا (retval)
		اذهب errout;
#جيڪڏهن_پورو

	/* Adjust the rec_len */
	retval = ext2fs_inline_معلومات_convert_dir(fs, ino, blk_buf, buf, سائيز);
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
	inode->i_flags &= ~EXT4_INLINE_معلومات_FL;
	retval = ext2fs_iblk_جوڙيو_بلاڪون(fs, inode, 1);
	إذا (retval)
		اذهب errout;
	inode->i_سائيز = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_سيٽ, 0, 0, &blk);
	إذا (retval)
		اذهب errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		اذهب errout;
	ext2fs_بلاڪ_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	أرجع retval;
}

ساكن errcode_t
ext2fs_inline_معلومات_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode *inode, حرف *buf, size_t سائيز)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_بلاڪ, 0, حجم(inode->i_بلاڪ));
	إذا (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		إذا (retval)
			أرجع retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_معلومات_FL;
	inode->i_سائيز = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		أرجع retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_file_write(e2_file, buf, سائيز, 0);
	ext2fs_file_close(e2_file);
	أرجع retval;
}

errcode_t ext2fs_inline_معلومات_expand(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_inode inode;
	بنية ext2_inline_معلومات معلومات;
	errcode_t retval;
	size_t inline_سائيز;
	حرف *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;

	إذا (!(inode.i_flags & EXT4_INLINE_معلومات_FL))
		أرجع EXT2_ET_NO_INLINE_معلومات;

	معلومات.fs = fs;
	معلومات.ino = ino;
	retval = ext2fs_inline_معلومات_ea_get(&معلومات);
	إذا (retval)
		أرجع retval;
	inline_سائيز = معلومات.ea_سائيز + EXT4_MIN_INLINE_معلومات_سائيز;
	retval = ext2fs_get_mem(inline_سائيز, &inline_buf);
	إذا (retval)
		اذهب errout;

	يادگيري_نقل(inline_buf, (فراغ *)inode.i_بلاڪ, EXT4_MIN_INLINE_معلومات_سائيز);
	إذا (معلومات.ea_سائيز > 0) {
		يادگيري_نقل(inline_buf + EXT4_MIN_INLINE_معلومات_سائيز,
		       معلومات.ea_معلومات, معلومات.ea_سائيز);
	}

	memset((فراغ *)inode.i_بلاڪ, 0, EXT4_MIN_INLINE_معلومات_سائيز);
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
	retval = ext2fs_inline_معلومات_ea_remove(fs, ino);
	إذا (retval)
		اذهب errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		اذهب errout;

	إذا (LINUX_S_ISDIR(inode.i_گهڻائي)) {
		retval = ext2fs_inline_معلومات_dir_expand(fs, ino, &inode,
						inline_buf, inline_سائيز);
	} وإلا {
		retval = ext2fs_inline_معلومات_file_expand(fs, ino, &inode,
						inline_buf, inline_سائيز);
	}

errout:
	إذا (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&معلومات.ea_معلومات);
	أرجع retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_معلومات_get(ext2_filsys fs, ext2_ino_t ino,
				 بنية ext2_inode *inode,
				 فراغ *buf, size_t *سائيز)
{
	بنية ext2_inode inode_buf;
	بنية ext2_inline_معلومات معلومات;
	errcode_t retval;

	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}

	معلومات.fs = fs;
	معلومات.ino = ino;
	retval = ext2fs_inline_معلومات_ea_get(&معلومات);
	إذا (retval)
		أرجع retval;

	يادگيري_نقل(buf, (فراغ *)inode->i_بلاڪ, EXT4_MIN_INLINE_معلومات_سائيز);
	إذا (معلومات.ea_سائيز > 0)
		يادگيري_نقل((حرف *) buf + EXT4_MIN_INLINE_معلومات_سائيز,
		       معلومات.ea_معلومات, معلومات.ea_سائيز);

	إذا (سائيز)
		*سائيز = EXT4_MIN_INLINE_معلومات_سائيز + معلومات.ea_سائيز;
	ext2fs_free_mem(&معلومات.ea_معلومات);
	أرجع 0;
}

errcode_t ext2fs_inline_معلومات_سيٽ(ext2_filsys fs, ext2_ino_t ino,
				 بنية ext2_inode *inode,
				 فراغ *buf, size_t سائيز)
{
	بنية ext2_inode inode_buf;
	بنية ext2_inline_معلومات معلومات = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_سائيز, existing_سائيز, free_inode_سائيز;

	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}

	إذا (سائيز <= EXT4_MIN_INLINE_معلومات_سائيز) {
		يادگيري_نقل((فراغ *)inode->i_بلاڪ, buf, سائيز);
	} وإلا {
		retval = ext2fs_xattr_inode_max_سائيز(fs, ino, &free_ea_سائيز);
		إذا (retval)
			أرجع retval;

		retval = ext2fs_inline_معلومات_سائيز(fs, ino, &existing_سائيز);
		إذا (retval)
			أرجع retval;

		إذا (existing_سائيز < EXT4_MIN_INLINE_معلومات_سائيز) {
			free_inode_سائيز = EXT4_MIN_INLINE_معلومات_سائيز -
					  existing_سائيز;
		} وإلا {
			free_inode_سائيز = 0;
		}

		إذا (سائيز != existing_سائيز &&
		    سائيز > existing_سائيز + free_ea_سائيز + free_inode_سائيز)
			أرجع EXT2_ET_INLINE_معلومات_NO_SPACE;

		يادگيري_نقل((فراغ *)inode->i_بلاڪ, buf, EXT4_MIN_INLINE_معلومات_سائيز);
		إذا (سائيز > EXT4_MIN_INLINE_معلومات_سائيز)
			معلومات.ea_سائيز = سائيز - EXT4_MIN_INLINE_معلومات_سائيز;
		معلومات.ea_معلومات = (حرف *) buf + EXT4_MIN_INLINE_معلومات_سائيز;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		أرجع retval;
	أرجع ext2fs_inline_معلومات_ea_سيٽ(&معلومات);
}

#جيڪڏهن_وصف_ٿيل DEBUG
#ضمن "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#وصف_ڪريو BUFF_سائيز (64)

ساكن errcode_t file_test(ext2_filsys fs)
{
	بنية ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t سائيز;
	حرف *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	إذا (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		أرجع 1;
	}

	memset(&inode, 0, حجم(inode));
	inode.i_flags |= EXT4_INLINE_معلومات_FL;
	inode.i_سائيز = EXT4_MIN_INLINE_معلومات_سائيز;
	inode.i_گهڻائي = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	إذا (retval) {
		com_err("file_test", retval, "while writing a new inode");
		أرجع 1;
	}

	retval = ext2fs_inline_معلومات_init(fs, newfile);
	إذا (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		أرجع 1;
	}

	retval = ext2fs_inline_معلومات_سائيز(fs, newfile, &سائيز);
	إذا (retval) {
		com_err("file_test", retval, "while getting size");
		أرجع 1;
	}

	إذا (سائيز != EXT4_MIN_INLINE_معلومات_سائيز) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		أرجع 1;
	}

	ext2fs_get_mem(BUFF_سائيز, &buf);
	memset(buf, 'a', BUFF_سائيز);
	retval = ext2fs_inline_معلومات_سيٽ(fs, newfile, 0, buf, BUFF_سائيز);
	إذا (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		اذهب err;
	}

	ext2fs_get_mem(BUFF_سائيز, &cmpbuf);
	retval = ext2fs_inline_معلومات_get(fs, newfile, 0, cmpbuf, &سائيز);
	إذا (retval) {
		com_err("file_test", retval, "while getting inline data");
		اذهب err;
	}

	إذا (سائيز != BUFF_سائيز) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			سائيز, BUFF_سائيز);
		retval = 1;
		اذهب err;
	}

	إذا (memcmp(buf, cmpbuf, BUFF_سائيز)) {
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
	إذا (inode.i_سائيز != 0) {
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
	ثابت حرف *والد_name = "test";
	ext2_ino_t والد, dir, tmp;
	errcode_t retval;
	حرف dirname[32];
	صحيح i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	إذا (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		أرجع retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, والد_name);
	إذا (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", والد_name);
		أرجع retval;
	}

	retval = ext2fs_lookup(fs, 11, والد_name, لڙي_ڊيگهه(والد_name),
			       0, &والد);
	إذا (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", والد_name);
		أرجع retval;
	}

	retval = ext2fs_lookup(fs, والد, dot_name, لڙي_ڊيگهه(dot_name),
			       0, &tmp);
	إذا (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", والد_name);
		أرجع retval;
	}

	إذا (والد != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			والد, tmp);
		أرجع 1;
	}

	لكل (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, حجم(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, والد, 0, dirname);
		إذا (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			أرجع retval;
		}

		retval = ext2fs_lookup(fs, والد, dirname, لڙي_ڊيگهه(dirname),
				       0, &tmp);
		إذا (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", والد_name);
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
	retval = ext2fs_mkdir(fs, والد, 0, dirname);
	إذا (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		أرجع retval;
	}

	retval = ext2fs_expand_dir(fs, والد);
	إذا (retval) {
		com_err("dir_test", retval, "while expanding %s dir", والد_name);
		أرجع retval;
	}

	أرجع 0;
}

صحيح مکيه(صحيح argc, حرف *argv[])
{
	ext2_filsys		fs;
	بنية ext2_super_بلاڪ param;
	errcode_t		retval;

	/*تياري*/
	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلاڪون_ڳڻپ_سيٽ(&param, 32768);
	param.s_inodes_ڳڻپ = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_معلومات;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_سائيز = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٽ, &param,
				   test_io_manager, &fs);
	إذا (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		ٻاهر_نڪرو(1);
	}

	retval = ext2fs_allocate_tables(fs);
	إذا (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		ٻاهر_نڪرو(1);
	}

	/* initialize inode cache */
	إذا (!fs->icache) {
		ext2_ino_t پهريون_ino = EXT2_پهريون_INO(fs->super);
		صحيح i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		إذا (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			ٻاهر_نڪرو(1);
		}

		/* setup inode cache */
		لكل (i = 0; i < fs->icache->cache_سائيز; i++)
			fs->icache->cache[i].ino = پهريون_ino++;
	}

	/* test */
	إذا (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		أرجع 1;
	}
	لکو("tst_inline_data(FILE): OK\n");

	إذا (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		أرجع 1;
	}
	لکو("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	أرجع 0;
}
#جيڪڏهن_پورو
