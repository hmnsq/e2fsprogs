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

संरचना ext2_inline_दत्त {
	ext2_filsys fs;
	ext2_ino_t ino;
	size_t ea_आकार;	/* the size of inline data in ea area */
	शून्य *ea_दत्त;
};

स्थिर errcode_t ext2fs_inline_दत्त_ea_समुच्चयः(संरचना ext2_inline_दत्त *दत्त)
{
	संरचना ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(दत्त->fs, दत्त->ino, &handle);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_xattrs_read(handle);
	यदि (retval)
		गच्छ err;

	retval = ext2fs_xattr_समुच्चयः(handle, "system.data",
				  दत्त->ea_दत्त, दत्त->ea_आकार);
err:
	(शून्य) ext2fs_xattrs_close(&handle);
	प्रत्यागम retval;
}

स्थिर errcode_t ext2fs_inline_दत्त_ea_get(संरचना ext2_inline_दत्त *दत्त)
{
	संरचना ext2_xattr_handle *handle;
	errcode_t retval;

	दत्त->ea_आकार = 0;
	दत्त->ea_दत्त = 0;

	retval = ext2fs_xattrs_open(दत्त->fs, दत्त->ino, &handle);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_xattrs_read(handle);
	यदि (retval)
		गच्छ err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (शून्य **)&दत्त->ea_दत्त, &दत्त->ea_आकार);
	यदि (retval == EXT2_ET_EA_कुञ्चिका_NOT_प्राप्तम्) {
		दत्त->ea_आकार = 0;
		दत्त->ea_दत्त = रिक्त;
		retval = 0;
	} अन्यथा यदि (retval)
		गच्छ err;

err:
	(शून्य) ext2fs_xattrs_close(&handle);
	प्रत्यागम retval;
}

errcode_t ext2fs_inline_दत्त_init(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना ext2_inline_दत्त दत्त;
	वर्ण रिक्तानि[1] = { '\0' };

	दत्त.fs = fs;
	दत्त.ino = ino;
	दत्त.ea_आकार = 0;
	दत्त.ea_दत्त = रिक्तानि;
	प्रत्यागम ext2fs_inline_दत्त_ea_समुच्चयः(&दत्त);
}

errcode_t ext2fs_inline_दत्त_आकार(ext2_filsys fs, ext2_ino_t ino, size_t *आकार)
{
	संरचना ext2_inode inode;
	संरचना ext2_inline_दत्त दत्त;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	यदि (retval)
		प्रत्यागम retval;

	यदि (!(inode.i_flags & EXT4_INLINE_दत्त_FL))
		प्रत्यागम EXT2_ET_NO_INLINE_दत्त;

	दत्त.fs = fs;
	दत्त.ino = ino;
	retval = ext2fs_inline_दत्त_ea_get(&दत्त);
	यदि (retval)
		प्रत्यागम retval;

	*आकार = EXT4_MIN_INLINE_दत्त_आकार + दत्त.ea_आकार;
	प्रत्यागम ext2fs_free_mem(&दत्त.ea_दत्त);
}

पूर्णाङ्क ext2fs_inline_दत्त_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   शून्य *priv_दत्त)
{
	संरचना dir_context *ctx;
	संरचना ext2_inode inode;
	संरचना ext2_dir_प्रविष्टिः dirent;
	संरचना ext2_inline_दत्त दत्त;
	पूर्णाङ्क ret = खण्डम्_ABORT;
	e2_blkcnt_t blockcnt = 0;
	वर्ण *old_buf;
	अचिह्नित पूर्णाङ्क old_buflen;
	पूर्णाङ्क old_flags;

	ctx = (संरचना dir_context *)priv_दत्त;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_दत्त;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	यदि (ctx->errcode)
		गच्छ बहिर्गमनम्;

	यदि (!(inode.i_flags & EXT4_INLINE_दत्त_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_दत्त;
		गच्छ बहिर्गमनम्;
	}

	यदि (!LINUX_S_ISDIR(inode.i_बहुलकम्)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		गच्छ बहिर्गमनम्;
	}
	ret = 0;

	/* we first check '.' and '..' dir */
	dirent.inode = ino;
	dirent.name_len = 1;
	ext2fs_समुच्चयः_rec_len(fs, EXT2_DIR_REC_LEN(2), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '\0';
	ctx->buf = (वर्ण *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_खण्डम्(fs, 0, blockcnt++, 0, 0, priv_दत्त);
	यदि (ret & खण्डम्_ABORT)
		गच्छ बहिर्गमनम्;

	dirent.inode = ext2fs_le32_गन्तव्यम्_cpu(inode.i_खण्डम्[0]);
	dirent.name_len = 2;
	ext2fs_समुच्चयः_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (वर्ण *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_खण्डम्(fs, 0, blockcnt++, 0, 0, priv_दत्त);
	यदि (ret & खण्डम्_INLINE_दत्त_परिवर्तितम्) {
		errcode_t err;

		inode.i_खण्डम्[0] = ext2fs_cpu_गन्तव्यम्_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		यदि (err)
			गच्छ बहिर्गमनम्;
		ret &= ~खण्डम्_INLINE_दत्त_परिवर्तितम्;
	}
	यदि (ret & खण्डम्_ABORT)
		गच्छ बहिर्गमनम्;

	ctx->buf = (वर्ण *)inode.i_खण्डम् + EXT4_INLINE_दत्त_DOTDOT_आकार;
	ctx->buflen = EXT4_MIN_INLINE_दत्त_आकार - EXT4_INLINE_दत्त_DOTDOT_आकार;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	यदि (ctx->errcode) {
		ret |= खण्डम्_ABORT;
		गच्छ बहिर्गमनम्;
	}
#यदि_अन्तः
	ret |= ext2fs_process_dir_खण्डम्(fs, 0, blockcnt++, 0, 0, priv_दत्त);
	यदि (ret & खण्डम्_INLINE_दत्त_परिवर्तितम्) {
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		यदि (ctx->errcode) {
			ret |= खण्डम्_ABORT;
			गच्छ बहिर्गमनम्;
		}
#यदि_अन्तः
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		यदि (ctx->errcode)
			ret |= खण्डम्_ABORT;
		ret &= ~खण्डम्_INLINE_दत्त_परिवर्तितम्;
	}
	यदि (ret & खण्डम्_ABORT)
		गच्छ बहिर्गमनम्;

	दत्त.fs = fs;
	दत्त.ino = ino;
	ctx->errcode = ext2fs_inline_दत्त_ea_get(&दत्त);
	यदि (ctx->errcode) {
		ret |= खण्डम्_ABORT;
		गच्छ बहिर्गमनम्;
	}
	यदि (दत्त.ea_आकार <= 0)
		गच्छ out1;

	ctx->buf = दत्त.ea_दत्त;
	ctx->buflen = दत्त.ea_आकार;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	यदि (ctx->errcode) {
		ret |= खण्डम्_ABORT;
		गच्छ out1;
	}
#यदि_अन्तः

	ret |= ext2fs_process_dir_खण्डम्(fs, 0, blockcnt++, 0, 0, priv_दत्त);
	यदि (ret & खण्डम्_INLINE_दत्त_परिवर्तितम्) {
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		यदि (ctx->errcode) {
			ret |= खण्डम्_ABORT;
			गच्छ out1;
		}
#यदि_अन्तः
		ctx->errcode = ext2fs_inline_दत्त_ea_समुच्चयः(&दत्त);
		यदि (ctx->errcode)
			ret |= खण्डम्_ABORT;
	}

out1:
	ext2fs_free_mem(&दत्त.ea_दत्त);
बहिर्गमनम्:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(खण्डम्_ABORT | खण्डम्_INLINE_दत्त_परिवर्तितम्);
	प्रत्यागम ret;
}

errcode_t ext2fs_inline_दत्त_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_xattrs_read(handle);
	यदि (retval)
		गच्छ err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(शून्य) ext2fs_xattrs_close(&handle);
	प्रत्यागम retval;
}

स्थिर errcode_t ext2fs_inline_दत्त_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						वर्ण *bbuf, वर्ण *ibuf, पूर्णाङ्क आकार)
{
	संरचना ext2_dir_प्रविष्टिः *dir, *dir2;
	संरचना ext2_dir_प्रविष्टिः_पुच्छ *t;
	errcode_t retval;
	पूर्णाङ्क विस्थापनम्;
	अचिह्नित पूर्णाङ्क rec_len;
	पूर्णाङ्क csum_आकार = 0;
	पूर्णाङ्क filetype = 0;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ);

	/* Create '.' and '..' */
	यदि (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (संरचना ext2_dir_प्रविष्टिः *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_समुच्चयः_name_len(dir, 1);
	ext2fs_dirent_समुच्चयः_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_आकार) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (संरचना ext2_dir_प्रविष्टिः *) (bbuf + dir->rec_len);
	dir->rec_len = EXT2_DIR_REC_LEN(2);
	dir->inode = ext2fs_le32_गन्तव्यम्_cpu(((__u32 *)ibuf)[0]);
	ext2fs_dirent_समुच्चयः_name_len(dir, 2);
	ext2fs_dirent_समुच्चयः_file_type(dir, filetype);
	dir->name[0] = '.';
	dir->name[1] = '.';

	/*
	 * Adjust the last rec_len
	 */
	विस्थापनम् = EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2);
	dir = (संरचना ext2_dir_प्रविष्टिः *) (bbuf + विस्थापनम्);
	स्मृतिं_प्रतिलिख(bbuf + विस्थापनम्, ibuf + EXT4_INLINE_दत्त_DOTDOT_आकार,
	       आकार - EXT4_INLINE_दत्त_DOTDOT_आकार);
	आकार += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_दत्त_DOTDOT_आकार;

	कुरु {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		यदि (retval)
			गच्छ err;
		विस्थापनम् += rec_len;
		dir = (संरचना ext2_dir_प्रविष्टिः *) (bbuf + विस्थापनम्);
	} यावत् (विस्थापनम् < आकार);
	rec_len += fs->blocksize - csum_आकार - विस्थापनम्;
	retval = ext2fs_समुच्चयः_rec_len(fs, rec_len, dir2);
	यदि (retval)
		गच्छ err;

	यदि (csum_आकार) {
		t = EXT2_DIRENT_पुच्छ(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_पुच्छ(fs, t);
	}

err:
	प्रत्यागम retval;
}

स्थिर errcode_t
ext2fs_inline_दत्त_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode *inode, वर्ण *buf, size_t आकार)
{
	errcode_t retval;
	blk64_t blk;
	वर्ण *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	यदि (retval)
		प्रत्यागम retval;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_दत्त_DOTDOT_आकार,
					आकार, 0);
	यदि (retval)
		गच्छ errout;
#यदि_अन्तः

	/* Adjust the rec_len */
	retval = ext2fs_inline_दत्त_convert_dir(fs, ino, blk_buf, buf, आकार);
	यदि (retval)
		गच्छ errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	यदि (retval)
		गच्छ errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	यदि (retval)
		गच्छ errout;

	/* Update inode */
	यदि (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_दत्त_FL;
	retval = ext2fs_iblk_सङ्कलय_खण्डानि(fs, inode, 1);
	यदि (retval)
		गच्छ errout;
	inode->i_आकार = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_समुच्चयः, 0, 0, &blk);
	यदि (retval)
		गच्छ errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	यदि (retval)
		गच्छ errout;
	ext2fs_खण्डम्_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	प्रत्यागम retval;
}

स्थिर errcode_t
ext2fs_inline_दत्त_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       संरचना ext2_inode *inode, वर्ण *buf, size_t आकार)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	स्मृतिं_पूरय(inode->i_खण्डम्, 0, परिमाण(inode->i_खण्डम्));
	यदि (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		यदि (retval)
			प्रत्यागम retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_दत्त_FL;
	inode->i_आकार = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	यदि (retval)
		प्रत्यागम retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	यदि (retval)
		प्रत्यागम retval;
	retval = ext2fs_file_write(e2_file, buf, आकार, 0);
	ext2fs_file_close(e2_file);
	प्रत्यागम retval;
}

errcode_t ext2fs_inline_दत्त_expand(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना ext2_inode inode;
	संरचना ext2_inline_दत्त दत्त;
	errcode_t retval;
	size_t inline_आकार;
	वर्ण *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	यदि (retval)
		प्रत्यागम retval;

	यदि (!(inode.i_flags & EXT4_INLINE_दत्त_FL))
		प्रत्यागम EXT2_ET_NO_INLINE_दत्त;

	दत्त.fs = fs;
	दत्त.ino = ino;
	retval = ext2fs_inline_दत्त_ea_get(&दत्त);
	यदि (retval)
		प्रत्यागम retval;
	inline_आकार = दत्त.ea_आकार + EXT4_MIN_INLINE_दत्त_आकार;
	retval = ext2fs_get_mem(inline_आकार, &inline_buf);
	यदि (retval)
		गच्छ errout;

	स्मृतिं_प्रतिलिख(inline_buf, (शून्य *)inode.i_खण्डम्, EXT4_MIN_INLINE_दत्त_आकार);
	यदि (दत्त.ea_आकार > 0) {
		स्मृतिं_प्रतिलिख(inline_buf + EXT4_MIN_INLINE_दत्त_आकार,
		       दत्त.ea_दत्त, दत्त.ea_आकार);
	}

	स्मृतिं_पूरय((शून्य *)inode.i_खण्डम्, 0, EXT4_MIN_INLINE_दत्त_आकार);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	यदि (retval)
		गच्छ errout;
	retval = ext2fs_inline_दत्त_ea_remove(fs, ino);
	यदि (retval)
		गच्छ errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	यदि (retval)
		गच्छ errout;

	यदि (LINUX_S_ISDIR(inode.i_बहुलकम्)) {
		retval = ext2fs_inline_दत्त_dir_expand(fs, ino, &inode,
						inline_buf, inline_आकार);
	} अन्यथा {
		retval = ext2fs_inline_दत्त_file_expand(fs, ino, &inode,
						inline_buf, inline_आकार);
	}

errout:
	यदि (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&दत्त.ea_दत्त);
	प्रत्यागम retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_दत्त_get(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode *inode,
				 शून्य *buf, size_t *आकार)
{
	संरचना ext2_inode inode_buf;
	संरचना ext2_inline_दत्त दत्त;
	errcode_t retval;

	यदि (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		यदि (retval)
			प्रत्यागम retval;
		inode = &inode_buf;
	}

	दत्त.fs = fs;
	दत्त.ino = ino;
	retval = ext2fs_inline_दत्त_ea_get(&दत्त);
	यदि (retval)
		प्रत्यागम retval;

	स्मृतिं_प्रतिलिख(buf, (शून्य *)inode->i_खण्डम्, EXT4_MIN_INLINE_दत्त_आकार);
	यदि (दत्त.ea_आकार > 0)
		स्मृतिं_प्रतिलिख((वर्ण *) buf + EXT4_MIN_INLINE_दत्त_आकार,
		       दत्त.ea_दत्त, दत्त.ea_आकार);

	यदि (आकार)
		*आकार = EXT4_MIN_INLINE_दत्त_आकार + दत्त.ea_आकार;
	ext2fs_free_mem(&दत्त.ea_दत्त);
	प्रत्यागम 0;
}

errcode_t ext2fs_inline_दत्त_समुच्चयः(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode *inode,
				 शून्य *buf, size_t आकार)
{
	संरचना ext2_inode inode_buf;
	संरचना ext2_inline_दत्त दत्त = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	size_t free_ea_आकार, existing_आकार, free_inode_आकार;

	यदि (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		यदि (retval)
			प्रत्यागम retval;
		inode = &inode_buf;
	}

	यदि (आकार <= EXT4_MIN_INLINE_दत्त_आकार) {
		स्मृतिं_प्रतिलिख((शून्य *)inode->i_खण्डम्, buf, आकार);
	} अन्यथा {
		retval = ext2fs_xattr_inode_max_आकार(fs, ino, &free_ea_आकार);
		यदि (retval)
			प्रत्यागम retval;

		retval = ext2fs_inline_दत्त_आकार(fs, ino, &existing_आकार);
		यदि (retval)
			प्रत्यागम retval;

		यदि (existing_आकार < EXT4_MIN_INLINE_दत्त_आकार) {
			free_inode_आकार = EXT4_MIN_INLINE_दत्त_आकार -
					  existing_आकार;
		} अन्यथा {
			free_inode_आकार = 0;
		}

		यदि (आकार != existing_आकार &&
		    आकार > existing_आकार + free_ea_आकार + free_inode_आकार)
			प्रत्यागम EXT2_ET_INLINE_दत्त_NO_SPACE;

		स्मृतिं_प्रतिलिख((शून्य *)inode->i_खण्डम्, buf, EXT4_MIN_INLINE_दत्त_आकार);
		यदि (आकार > EXT4_MIN_INLINE_दत्त_आकार)
			दत्त.ea_आकार = आकार - EXT4_MIN_INLINE_दत्त_आकार;
		दत्त.ea_दत्त = (वर्ण *) buf + EXT4_MIN_INLINE_दत्त_आकार;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	यदि (retval)
		प्रत्यागम retval;
	प्रत्यागम ext2fs_inline_दत्त_ea_समुच्चयः(&दत्त);
}

#यदि_परिभाषितम् DEBUG
#अन्तर्भाव "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#परिभाषय BUFF_आकार (64)

स्थिर errcode_t file_test(ext2_filsys fs)
{
	संरचना ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	size_t आकार;
	वर्ण *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	यदि (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		प्रत्यागम 1;
	}

	स्मृतिं_पूरय(&inode, 0, परिमाण(inode));
	inode.i_flags |= EXT4_INLINE_दत्त_FL;
	inode.i_आकार = EXT4_MIN_INLINE_दत्त_आकार;
	inode.i_बहुलकम् = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	यदि (retval) {
		com_err("file_test", retval, "while writing a new inode");
		प्रत्यागम 1;
	}

	retval = ext2fs_inline_दत्त_init(fs, newfile);
	यदि (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		प्रत्यागम 1;
	}

	retval = ext2fs_inline_दत्त_आकार(fs, newfile, &आकार);
	यदि (retval) {
		com_err("file_test", retval, "while getting size");
		प्रत्यागम 1;
	}

	यदि (आकार != EXT4_MIN_INLINE_दत्त_आकार) {
		fprintf(stderr,
			"tst_inline_data: size of inline data is wrong\n");
		प्रत्यागम 1;
	}

	ext2fs_get_mem(BUFF_आकार, &buf);
	स्मृतिं_पूरय(buf, 'a', BUFF_आकार);
	retval = ext2fs_inline_दत्त_समुच्चयः(fs, newfile, 0, buf, BUFF_आकार);
	यदि (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		गच्छ err;
	}

	ext2fs_get_mem(BUFF_आकार, &cmpbuf);
	retval = ext2fs_inline_दत्त_get(fs, newfile, 0, cmpbuf, &आकार);
	यदि (retval) {
		com_err("file_test", retval, "while getting inline data");
		गच्छ err;
	}

	यदि (आकार != BUFF_आकार) {
		fprintf(stderr,
			"tst_inline_data: size %zu != buflen %u\n",
			आकार, BUFF_आकार);
		retval = 1;
		गच्छ err;
	}

	यदि (memcmp(buf, cmpbuf, BUFF_आकार)) {
		fprintf(stderr, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		गच्छ err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	यदि (retval) {
		com_err("file_test", retval, "while truncating inode");
		गच्छ err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	यदि (inode.i_आकार != 0) {
		fprintf(stderr, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	यदि (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	यदि (buf)
		ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

स्थिर errcode_t dir_test(ext2_filsys fs)
{
	ध्रुव वर्ण *dot_name = ".";
	ध्रुव वर्ण *stub_name = "stub";
	ध्रुव वर्ण *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	वर्ण dirname[32];
	पूर्णाङ्क i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	यदि (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		प्रत्यागम retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	यदि (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		प्रत्यागम retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, वाक्यदैर्घ्यम्(parent_name),
			       0, &parent);
	यदि (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		प्रत्यागम retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, वाक्यदैर्घ्यम्(dot_name),
			       0, &tmp);
	यदि (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		प्रत्यागम retval;
	}

	यदि (parent != tmp) {
		fprintf(stderr, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		प्रत्यागम 1;
	}

	प्रति (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(dirname, परिमाण(dirname), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, dirname);
		यदि (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", dirname);
			प्रत्यागम retval;
		}

		retval = ext2fs_lookup(fs, parent, dirname, वाक्यदैर्घ्यम्(dirname),
				       0, &tmp);
		यदि (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			प्रत्यागम retval;
		}

		यदि (dir != tmp) {
			fprintf(stderr,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			प्रत्यागम 1;
		}
	}

	snprintf(dirname, परिमाण(dirname), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, dirname);
	यदि (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", dirname);
		प्रत्यागम retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	यदि (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		प्रत्यागम retval;
	}

	प्रत्यागम 0;
}

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण *argv[])
{
	ext2_filsys		fs;
	संरचना ext2_super_खण्डम् param;
	errcode_t		retval;

	/*सज्जता*/
	initialize_ext2_error_सारणी();

	स्मृतिं_पूरय(&param, 0, परिमाण(param));
	ext2fs_खण्डानि_गणनम्_समुच्चयः(&param, 32768);
	param.s_inodes_गणनम् = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_दत्त;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_आकार = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_द्वयङ्काः, &param,
				   test_io_manager, &fs);
	यदि (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		निर्गच्छ(1);
	}

	retval = ext2fs_allocate_tables(fs);
	यदि (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		निर्गच्छ(1);
	}

	/* initialize inode cache */
	यदि (!fs->icache) {
		ext2_ino_t first_ino = EXT2_FIRST_INO(fs->super);
		पूर्णाङ्क i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		यदि (!fs->icache) {
			fprintf(stderr,
				"tst_inline_data: init inode cache failed\n");
			निर्गच्छ(1);
		}

		/* setup inode cache */
		प्रति (i = 0; i < fs->icache->cache_आकार; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	यदि (file_test(fs)) {
		fprintf(stderr, "tst_inline_data(FILE): FAILED\n");
		प्रत्यागम 1;
	}
	प्रारूप_लिख("tst_inline_data(FILE): OK\n");

	यदि (dir_test(fs)) {
		fprintf(stderr, "tst_inline_data(DIR): FAILED\n");
		प्रत्यागम 1;
	}
	प्रारूप_लिख("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	प्रत्यागम 0;
}
#यदि_अन्तः
