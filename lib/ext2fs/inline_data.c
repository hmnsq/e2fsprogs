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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <limits.h> /* for PATH_MAX */

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2_ext_attr.h"

#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

నిర్మాణం ext2_inline_దత్తాంశం {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_కొలత;	/* the size of inline data in ea area */
	శూన్య *ea_దత్తాంశం;
};

స్థిర errcode_t ext2fs_inline_దత్తాంశం_ea_సమితి(నిర్మాణం ext2_inline_దత్తాంశం *దత్తాంశం)
{
	నిర్మాణం ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(దత్తాంశం->fs, దత్తాంశం->ino, &handle);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = ext2fs_xattrs_read(handle);
	ఒకవేళ (retval)
		వెళ్ళు err;

	retval = ext2fs_xattr_సమితి(handle, "system.data",
				  దత్తాంశం->ea_దత్తాంశం, దత్తాంశం->ea_కొలత);
err:
	(శూన్య) ext2fs_xattrs_close(&handle);
	తిరిగి retval;
}

స్థిర errcode_t ext2fs_inline_దత్తాంశం_ea_get(నిర్మాణం ext2_inline_దత్తాంశం *దత్తాంశం)
{
	నిర్మాణం ext2_xattr_handle *handle;
	errcode_t retval;

	దత్తాంశం->ea_కొలత = 0;
	దత్తాంశం->ea_దత్తాంశం = 0;

	retval = ext2fs_xattrs_open(దత్తాంశం->fs, దత్తాంశం->ino, &handle);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = ext2fs_xattrs_read(handle);
	ఒకవేళ (retval)
		వెళ్ళు err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (శూన్య **)&దత్తాంశం->ea_దత్తాంశం, &దత్తాంశం->ea_కొలత);
	ఒకవేళ (retval == EXT2_ET_EA_కీలకం_NOT_దొరికింది) {
		దత్తాంశం->ea_కొలత = 0;
		దత్తాంశం->ea_దత్తాంశం = ఖాళీ;
		retval = 0;
	} లేకపోతే ఒకవేళ (retval)
		వెళ్ళు err;

err:
	(శూన్య) ext2fs_xattrs_close(&handle);
	తిరిగి retval;
}

errcode_t ext2fs_inline_దత్తాంశం_init(ext2_filsys fs, ext2_ino_t ino)
{
	నిర్మాణం ext2_inline_దత్తాంశం దత్తాంశం;
	అక్షరం ఖాళీలు[1] = { '\0' };

	దత్తాంశం.fs = fs;
	దత్తాంశం.ino = ino;
	దత్తాంశం.ea_కొలత = 0;
	దత్తాంశం.ea_దత్తాంశం = ఖాళీలు;
	తిరిగి ext2fs_inline_దత్తాంశం_ea_సమితి(&దత్తాంశం);
}

errcode_t ext2fs_inline_దత్తాంశం_కొలత(ext2_filsys fs, ext2_ino_t ino, size_t *కొలత)
{
	నిర్మాణం ext2_inode inode;
	నిర్మాణం ext2_inline_దత్తాంశం దత్తాంశం;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (!(inode.i_flags & EXT4_INLINE_దత్తాంశం_FL))
		తిరిగి EXT2_ET_NO_INLINE_దత్తాంశం;

	దత్తాంశం.fs = fs;
	దత్తాంశం.ino = ino;
	retval = ext2fs_inline_దత్తాంశం_ea_get(&దత్తాంశం);
	ఒకవేళ (retval)
		తిరిగి retval;

	*కొలత = EXT4_MIN_INLINE_దత్తాంశం_కొలత + దత్తాంశం.ea_కొలత;
	తిరిగి ext2fs_free_mem(&దత్తాంశం.ea_దత్తాంశం);
}

పూర్ణాంకం ext2fs_inline_దత్తాంశం_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం dir_context *ctx;
	నిర్మాణం ext2_inode inode;
	నిర్మాణం ext2_dir_నమోదు dirent;
	నిర్మాణం ext2_inline_దత్తాంశం దత్తాంశం;
	పూర్ణాంకం ret = ఖండం_ABORT;
	e2_blkcnt_t blockcnt = 0;
	అక్షరం *old_buf;
	గుర్తులేని పూర్ణాంకం old_buflen;
	పూర్ణాంకం old_flags;

	ctx = (నిర్మాణం dir_context *)priv_దత్తాంశం;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_దత్తాంశం;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (ctx->errcode)
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (!(inode.i_flags & EXT4_INLINE_దత్తాంశం_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_దత్తాంశం;
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (!LINUX_S_ISDIR(inode.i_బహుళకం)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		వెళ్ళు బహిర్గతం;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_సమితి_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (అక్షరం *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_ఖండం(fs, 0, blockcnt++, 0, 0, priv_దత్తాంశం);
	ఒకవేళ (ret & ఖండం_ABORT)
		వెళ్ళు బహిర్గతం;

	dirent.inode = ext2fs_le32_గమ్యం_cpu(inode.i_ఖండం[0]);
	dirent.name_len = 2;
	ext2fs_సమితి_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (అక్షరం *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_ఖండం(fs, 0, blockcnt++, 0, 0, priv_దత్తాంశం);
	ఒకవేళ (ret & ఖండం_INLINE_దత్తాంశం_మారింది) {
		errcode_t err;

		inode.i_ఖండం[0] = ext2fs_cpu_గమ్యం_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		ret &= ~ఖండం_INLINE_దత్తాంశం_మారింది;
	}
	ఒకవేళ (ret & ఖండం_ABORT)
		వెళ్ళు బహిర్గతం;

	ctx->buf = (అక్షరం *)inode.i_ఖండం + EXT4_INLINE_దత్తాంశం_DOTDOT_కొలత;
	ctx->buflen = EXT4_MIN_INLINE_దత్తాంశం_కొలత - EXT4_INLINE_దత్తాంశం_DOTDOT_కొలత;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	ఒకవేళ (ctx->errcode) {
		ret |= ఖండం_ABORT;
		వెళ్ళు బహిర్గతం;
	}
#అయితే_ముగింపు
	ret |= ext2fs_process_dir_ఖండం(fs, 0, blockcnt++, 0, 0, priv_దత్తాంశం);
	ఒకవేళ (ret & ఖండం_INLINE_దత్తాంశం_మారింది) {
#నిర్వచితమైతే పదాలు_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		ఒకవేళ (ctx->errcode) {
			ret |= ఖండం_ABORT;
			వెళ్ళు బహిర్గతం;
		}
#అయితే_ముగింపు
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		ఒకవేళ (ctx->errcode)
			ret |= ఖండం_ABORT;
		ret &= ~ఖండం_INLINE_దత్తాంశం_మారింది;
	}
	ఒకవేళ (ret & ఖండం_ABORT)
		వెళ్ళు బహిర్గతం;

	దత్తాంశం.fs = fs;
	దత్తాంశం.ino = ino;
	ctx->errcode = ext2fs_inline_దత్తాంశం_ea_get(&దత్తాంశం);
	ఒకవేళ (ctx->errcode) {
		ret |= ఖండం_ABORT;
		వెళ్ళు బహిర్గతం;
	}
	ఒకవేళ (దత్తాంశం.ea_కొలత <= 0)
		వెళ్ళు out1;

	ctx->buf = దత్తాంశం.ea_దత్తాంశం;
	ctx->buflen = దత్తాంశం.ea_కొలత;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	ఒకవేళ (ctx->errcode) {
		ret |= ఖండం_ABORT;
		వెళ్ళు out1;
	}
#అయితే_ముగింపు

	ret |= ext2fs_process_dir_ఖండం(fs, 0, blockcnt++, 0, 0, priv_దత్తాంశం);
	ఒకవేళ (ret & ఖండం_INLINE_దత్తాంశం_మారింది) {
#నిర్వచితమైతే పదాలు_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		ఒకవేళ (ctx->errcode) {
			ret |= ఖండం_ABORT;
			వెళ్ళు out1;
		}
#అయితే_ముగింపు
		ctx->errcode = ext2fs_inline_దత్తాంశం_ea_సమితి(&దత్తాంశం);
		ఒకవేళ (ctx->errcode)
			ret |= ఖండం_ABORT;
	}

out1:
	ext2fs_free_mem(&దత్తాంశం.ea_దత్తాంశం);
బహిర్గతం:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(ఖండం_ABORT | ఖండం_INLINE_దత్తాంశం_మారింది);
	తిరిగి ret;
}

errcode_t ext2fs_inline_దత్తాంశం_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	నిర్మాణం ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = ext2fs_xattrs_read(handle);
	ఒకవేళ (retval)
		వెళ్ళు err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(శూన్య) ext2fs_xattrs_close(&handle);
	తిరిగి retval;
}

స్థిర errcode_t ext2fs_inline_దత్తాంశం_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						అక్షరం *bbuf, అక్షరం *ibuf, పూర్ణాంకం కొలత)
{
	నిర్మాణం ext2_dir_నమోదు *dir, *dir2;
	నిర్మాణం ext2_dir_నమోదు_తోక *t;
	errcode_t retval;
	పూర్ణాంకం స్థానభ్రంశం;
	గుర్తులేని పూర్ణాంకం rec_len;
	పూర్ణాంకం csum_కొలత = 0;
	పూర్ణాంకం filetype = 0;

	ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_కొలత = పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక);

	/* Create '.' and '..' */
	ఒకవేళ (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (నిర్మాణం ext2_dir_నమోదు *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_సమితి_name_len(dir, 1);
	ext2fs_dirent_సమితి_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_కొలత) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (నిర్మాణం ext2_dir_నమోదు *) (bbuf + dir->rec_len);
	dir->rec_len = EXT2_DIR_REC_LEN(2);
	dir->inode = ext2fs_le32_గమ్యం_cpu(((__u32 *)ibuf)[0]);
	ext2fs_dirent_సమితి_name_len(dir, 2);
	ext2fs_dirent_సమితి_file_type(dir, filetype);
	dir->name[0] = '.';
	dir->name[1] = '.';

	/*
	 * Adjust the last rec_len
	 */
	స్థానభ్రంశం = EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2);
	dir = (నిర్మాణం ext2_dir_నమోదు *) (bbuf + స్థానభ్రంశం);
	జ్ఞాపకం_నకలు(bbuf + స్థానభ్రంశం, ibuf + EXT4_INLINE_దత్తాంశం_DOTDOT_కొలత,
	       కొలత - EXT4_INLINE_దత్తాంశం_DOTDOT_కొలత);
	కొలత += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_దత్తాంశం_DOTDOT_కొలత;

	చేయి {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		ఒకవేళ (retval)
			వెళ్ళు err;
		స్థానభ్రంశం += rec_len;
		dir = (నిర్మాణం ext2_dir_నమోదు *) (bbuf + స్థానభ్రంశం);
	} అయితే (స్థానభ్రంశం < కొలత);
	rec_len += fs->blocksize - csum_కొలత - స్థానభ్రంశం;
	retval = ext2fs_సమితి_rec_len(fs, rec_len, dir2);
	ఒకవేళ (retval)
		వెళ్ళు err;

	ఒకవేళ (csum_కొలత) {
		t = EXT2_DIRENT_తోక(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_తోక(fs, t);
	}

err:
	తిరిగి retval;
}

స్థిర errcode_t
ext2fs_inline_దత్తాంశం_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      నిర్మాణం ext2_inode *inode, అక్షరం *buf, size_t కొలత)
{
	errcode_t retval;
	blk64_t blk;
	అక్షరం *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	ఒకవేళ (retval)
		తిరిగి retval;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_దత్తాంశం_DOTDOT_కొలత,
					కొలత, 0);
	ఒకవేళ (retval)
		వెళ్ళు errout;
#అయితే_ముగింపు

	/* Adjust the rec_len */
	retval = ext2fs_inline_దత్తాంశం_convert_dir(fs, ino, blk_buf, buf, కొలత);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	/* Update inode */
	ఒకవేళ (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_దత్తాంశం_FL;
	retval = ext2fs_iblk_కూడు_ఖండాలు(fs, inode, 1);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	inode->i_కొలత = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_సమితి, 0, 0, &blk);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	ext2fs_ఖండం_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	తిరిగి retval;
}

స్థిర errcode_t
ext2fs_inline_దత్తాంశం_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       నిర్మాణం ext2_inode *inode, అక్షరం *buf, size_t కొలత)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	memset(inode->i_ఖండం, 0, పరిమాణం(inode->i_ఖండం));
	ఒకవేళ (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		ఒకవేళ (retval)
			తిరిగి retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_దత్తాంశం_FL;
	inode->i_కొలత = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	ఒకవేళ (retval)
		తిరిగి retval;
	retval = ext2fs_file_write(e2_file, buf, కొలత, 0);
	ext2fs_file_close(e2_file);
	తిరిగి retval;
}

errcode_t ext2fs_inline_దత్తాంశం_expand(ext2_filsys fs, ext2_ino_t ino)
{
	నిర్మాణం ext2_inode inode;
	నిర్మాణం ext2_inline_దత్తాంశం దత్తాంశం;
	errcode_t retval;
	size_t inline_కొలత;
	అక్షరం *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (!(inode.i_flags & EXT4_INLINE_దత్తాంశం_FL))
		తిరిగి EXT2_ET_NO_INLINE_దత్తాంశం;

	దత్తాంశం.fs = fs;
	దత్తాంశం.ino = ino;
	retval = ext2fs_inline_దత్తాంశం_ea_get(&దత్తాంశం);
	ఒకవేళ (retval)
		తిరిగి retval;
	inline_కొలత = దత్తాంశం.ea_కొలత + EXT4_MIN_INLINE_దత్తాంశం_కొలత;
	retval = ext2fs_get_mem(inline_కొలత, &inline_buf);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	జ్ఞాపకం_నకలు(inline_buf, (శూన్య *)inode.i_ఖండం, EXT4_MIN_INLINE_దత్తాంశం_కొలత);
	ఒకవేళ (దత్తాంశం.ea_కొలత > 0) {
		జ్ఞాపకం_నకలు(inline_buf + EXT4_MIN_INLINE_దత్తాంశం_కొలత,
		       దత్తాంశం.ea_దత్తాంశం, దత్తాంశం.ea_కొలత);
	}

	memset((శూన్య *)inode.i_ఖండం, 0, EXT4_MIN_INLINE_దత్తాంశం_కొలత);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	retval = ext2fs_inline_దత్తాంశం_ea_remove(fs, ino);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	ఒకవేళ (LINUX_S_ISDIR(inode.i_బహుళకం)) {
		retval = ext2fs_inline_దత్తాంశం_dir_expand(fs, ino, &inode,
						inline_buf, inline_కొలత);
	} లేకపోతే {
		retval = ext2fs_inline_దత్తాంశం_file_expand(fs, ino, &inode,
						inline_buf, inline_కొలత);
	}

errout:
	ఒకవేళ (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&దత్తాంశం.ea_దత్తాంశం);
	తిరిగి retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_దత్తాంశం_get(ext2_filsys fs, ext2_ino_t ino,
				 నిర్మాణం ext2_inode *inode,
				 శూన్య *buf, size_t *కొలత)
{
	నిర్మాణం ext2_inode inode_buf;
	నిర్మాణం ext2_inline_దత్తాంశం దత్తాంశం;
	errcode_t retval;

	ఒకవేళ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ఒకవేళ (retval)
			తిరిగి retval;
		inode = &inode_buf;
	}

	దత్తాంశం.fs = fs;
	దత్తాంశం.ino = ino;
	retval = ext2fs_inline_దత్తాంశం_ea_get(&దత్తాంశం);
	ఒకవేళ (retval)
		తిరిగి retval;

	జ్ఞాపకం_నకలు(buf, (శూన్య *)inode->i_ఖండం, EXT4_MIN_INLINE_దత్తాంశం_కొలత);
	ఒకవేళ (దత్తాంశం.ea_కొలత > 0)
		జ్ఞాపకం_నకలు((అక్షరం *) buf + EXT4_MIN_INLINE_దత్తాంశం_కొలత,
		       దత్తాంశం.ea_దత్తాంశం, దత్తాంశం.ea_కొలత);

	ఒకవేళ (కొలత)
		*కొలత = EXT4_MIN_INLINE_దత్తాంశం_కొలత + దత్తాంశం.ea_కొలత;
	ext2fs_free_mem(&దత్తాంశం.ea_దత్తాంశం);
	తిరిగి 0;
}

errcode_t ext2fs_inline_దత్తాంశం_సమితి(ext2_filsys fs, ext2_ino_t ino,
				 నిర్మాణం ext2_inode *inode,
				 శూన్య *buf, size_t కొలత)
{
	నిర్మాణం ext2_inode inode_buf;
	నిర్మాణం ext2_inline_దత్తాంశం దత్తాంశం = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_కొలత, existing_కొలత, free_inode_కొలత;

	ఒకవేళ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ఒకవేళ (retval)
			తిరిగి retval;
		inode = &inode_buf;
	}

	ఒకవేళ (కొలత <= EXT4_MIN_INLINE_దత్తాంశం_కొలత) {
		జ్ఞాపకం_నకలు((శూన్య *)inode->i_ఖండం, buf, కొలత);
	} లేకపోతే {
		retval = ext2fs_xattr_inode_max_కొలత(fs, ino, &free_ea_కొలత);
		ఒకవేళ (retval)
			తిరిగి retval;

		retval = ext2fs_inline_దత్తాంశం_కొలత(fs, ino, &existing_కొలత);
		ఒకవేళ (retval)
			తిరిగి retval;

		ఒకవేళ (existing_కొలత < EXT4_MIN_INLINE_దత్తాంశం_కొలత) {
			free_inode_కొలత = EXT4_MIN_INLINE_దత్తాంశం_కొలత -
					  existing_కొలత;
		} లేకపోతే {
			free_inode_కొలత = 0;
		}

		ఒకవేళ (కొలత != existing_కొలత &&
		    కొలత > existing_కొలత + free_ea_కొలత + free_inode_కొలత)
			తిరిగి EXT2_ET_INLINE_దత్తాంశం_NO_SPACE;

		జ్ఞాపకం_నకలు((శూన్య *)inode->i_ఖండం, buf, EXT4_MIN_INLINE_దత్తాంశం_కొలత);
		ఒకవేళ (కొలత > EXT4_MIN_INLINE_దత్తాంశం_కొలత)
			దత్తాంశం.ea_కొలత = కొలత - EXT4_MIN_INLINE_దత్తాంశం_కొలత;
		దత్తాంశం.ea_దత్తాంశం = (అక్షరం *) buf + EXT4_MIN_INLINE_దత్తాంశం_కొలత;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	ఒకవేళ (retval)
		తిరిగి retval;
	తిరిగి ext2fs_inline_దత్తాంశం_ea_సమితి(&దత్తాంశం);
}

#నిర్వచితమైతే DEBUG
#అన్తర్భావ "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#నిర్వచించు BUFF_కొలత (64)

స్థిర errcode_t file_test(ext2_filsys fs)
{
	నిర్మాణం ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t కొలత;
	అక్షరం *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		తిరిగి 1;
	}

	memset(&inode, 0, పరిమాణం(inode));
	inode.i_flags |= EXT4_INLINE_దత్తాంశం_FL;
	inode.i_కొలత = EXT4_MIN_INLINE_దత్తాంశం_కొలత;
	inode.i_బహుళకం = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while writing a new inode");
		తిరిగి 1;
	}

	retval = ext2fs_inline_దత్తాంశం_init(fs, newfile);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		తిరిగి 1;
	}

	retval = ext2fs_inline_దత్తాంశం_కొలత(fs, newfile, &కొలత);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while getting size");
		తిరిగి 1;
	}

	ఒకవేళ (కొలత != EXT4_MIN_INLINE_దత్తాంశం_కొలత) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		తిరిగి 1;
	}

	ext2fs_get_mem(BUFF_కొలత, &buf);
	memset(buf, 'a', BUFF_కొలత);
	retval = ext2fs_inline_దత్తాంశం_సమితి(fs, newfile, 0, buf, BUFF_కొలత);
	ఒకవేళ (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		వెళ్ళు err;
	}

	ext2fs_get_mem(BUFF_కొలత, &cmpbuf);
	retval = ext2fs_inline_దత్తాంశం_get(fs, newfile, 0, cmpbuf, &కొలత);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while getting inline data");
		వెళ్ళు err;
	}

	ఒకవేళ (కొలత != BUFF_కొలత) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			కొలత, BUFF_కొలత);
		retval = 1;
		వెళ్ళు err;
	}

	ఒకవేళ (memcmp(buf, cmpbuf, BUFF_కొలత)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		వెళ్ళు err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while truncating inode");
		వెళ్ళు err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	ఒకవేళ (inode.i_కొలత != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	ఒకవేళ (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	ఒకవేళ (buf)
		ext2fs_free_mem(&buf);
	తిరిగి retval;
}

స్థిర errcode_t dir_test(ext2_filsys fs)
{
	స్థిరరాశి అక్షరం *dot_name = ".";
	స్థిరరాశి అక్షరం *stub_name = "stub";
	స్థిరరాశి అక్షరం *మాతృ_name = "test";
	ext2_ino_t మాతృ, dir, tmp;
	errcode_t retval;
	అక్షరం dirname[32];
	పూర్ణాంకం i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	ఒకవేళ (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		తిరిగి retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, మాతృ_name);
	ఒకవేళ (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", మాతృ_name);
		తిరిగి retval;
	}

	retval = ext2fs_lookup(fs, 11, మాతృ_name, అక్షరశ్రేణి_పొడవు(మాతృ_name),
			       0, &మాతృ);
	ఒకవేళ (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", మాతృ_name);
		తిరిగి retval;
	}

	retval = ext2fs_lookup(fs, మాతృ, dot_name, అక్షరశ్రేణి_పొడవు(dot_name),
			       0, &tmp);
	ఒకవేళ (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", మాతృ_name);
		తిరిగి retval;
	}

	ఒకవేళ (మాతృ != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			మాతృ, tmp);
		తిరిగి 1;
	}

	కొరకు (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, పరిమాణం(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, మాతృ, 0, dirname);
		ఒకవేళ (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			తిరిగి retval;
		}

		retval = ext2fs_lookup(fs, మాతృ, dirname, అక్షరశ్రేణి_పొడవు(dirname),
				       0, &tmp);
		ఒకవేళ (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", మాతృ_name);
			తిరిగి retval;
		}

		ఒకవేళ (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			తిరిగి 1;
		}
	}

	snprintf(dirname, పరిమాణం(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, మాతృ, 0, dirname);
	ఒకవేళ (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		తిరిగి retval;
	}

	retval = ext2fs_expand_dir(fs, మాతృ);
	ఒకవేళ (retval) {
		com_err("dir_test", retval, "while expanding %s dir", మాతృ_name);
		తిరిగి retval;
	}

	తిరిగి 0;
}

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం *argv[])
{
	ext2_filsys		fs;
	నిర్మాణం ext2_super_ఖండం param;
	errcode_t		retval;

	/*అమరిక*/
	initialize_ext2_error_పట్టిక();

	memset(&param, 0, పరిమాణం(param));
	ext2fs_ఖండాలు_లెక్క_సమితి(&param, 32768);
	param.s_inodes_లెక్క = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_దత్తాంశం;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_కొలత = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_బిట్లు, &param,
				   test_io_manager, &fs);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		నిష్క్రమించు(1);
	}

	retval = ext2fs_allocate_tables(fs);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		నిష్క్రమించు(1);
	}

	/* initialize inode cache */
	ఒకవేళ (!fs->icache) {
		ext2_ino_t మొదటి_ino = EXT2_మొదటి_INO(fs->super);
		పూర్ణాంకం i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		ఒకవేళ (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			నిష్క్రమించు(1);
		}

		/* setup inode cache */
		కొరకు (i = 0; i < fs->icache->cache_కొలత; i++)
			fs->icache->cache[i].ino = మొదటి_ino++;
	}

	/* test */
	ఒకవేళ (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		తిరిగి 1;
	}
	రాయి("tst_inline_data(FILE): OK\n");

	ఒకవేళ (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		తిరిగి 1;
	}
	రాయి("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	తిరిగి 0;
}
#అయితే_ముగింపు
