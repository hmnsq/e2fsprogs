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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <time.h>
#समावेश <limits.h> /* for PATH_MAX */

#समावेश "ext2_fs.h"
#समावेश "ext2_ext_attr.h"

#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

काष्ठा ext2_inline_तथ्य {
	ext2_filsys fs;
	ext2_ino_t ino;
	माप_प्रकार ea_आकार;	/* the size of inline data in ea area */
	व्योम *ea_तथ्य;
};

अटल errcode_t ext2fs_inline_तथ्य_ea_set(काष्ठा ext2_inline_तथ्य *तथ्य)
{
	काष्ठा ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(तथ्य->fs, तथ्य->ino, &handle);
	अगर (retval)
		वापस retval;

	retval = ext2fs_xattrs_read(handle);
	अगर (retval)
		जाओ err;

	retval = ext2fs_xattr_set(handle, "system.data",
				  तथ्य->ea_तथ्य, तथ्य->ea_आकार);
err:
	(व्योम) ext2fs_xattrs_close(&handle);
	वापस retval;
}

अटल errcode_t ext2fs_inline_तथ्य_ea_get(काष्ठा ext2_inline_तथ्य *तथ्य)
{
	काष्ठा ext2_xattr_handle *handle;
	errcode_t retval;

	तथ्य->ea_आकार = 0;
	तथ्य->ea_तथ्य = 0;

	retval = ext2fs_xattrs_open(तथ्य->fs, तथ्य->ino, &handle);
	अगर (retval)
		वापस retval;

	retval = ext2fs_xattrs_read(handle);
	अगर (retval)
		जाओ err;

	retval = ext2fs_xattr_get(handle, "system.data",
				  (व्योम **)&तथ्य->ea_तथ्य, &तथ्य->ea_आकार);
	अगर (retval == EXT2_ET_EA_कुंजी_NOT_FOUND) {
		तथ्य->ea_आकार = 0;
		तथ्य->ea_तथ्य = शून्य;
		retval = 0;
	} अन्यथा अगर (retval)
		जाओ err;

err:
	(व्योम) ext2fs_xattrs_close(&handle);
	वापस retval;
}

errcode_t ext2fs_inline_तथ्य_init(ext2_filsys fs, ext2_ino_t ino)
{
	काष्ठा ext2_inline_तथ्य तथ्य;
	अक्षर empty[1] = { '\0' };

	तथ्य.fs = fs;
	तथ्य.ino = ino;
	तथ्य.ea_आकार = 0;
	तथ्य.ea_तथ्य = empty;
	वापस ext2fs_inline_तथ्य_ea_set(&तथ्य);
}

errcode_t ext2fs_inline_तथ्य_आकार(ext2_filsys fs, ext2_ino_t ino, माप_प्रकार *आकार)
{
	काष्ठा ext2_inode inode;
	काष्ठा ext2_inline_तथ्य तथ्य;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	अगर (retval)
		वापस retval;

	अगर (!(inode.i_flags & EXT4_INLINE_तथ्य_FL))
		वापस EXT2_ET_NO_INLINE_तथ्य;

	तथ्य.fs = fs;
	तथ्य.ino = ino;
	retval = ext2fs_inline_तथ्य_ea_get(&तथ्य);
	अगर (retval)
		वापस retval;

	*आकार = EXT4_MIN_INLINE_तथ्य_आकार + तथ्य.ea_आकार;
	वापस ext2fs_free_mem(&तथ्य.ea_तथ्य);
}

पूर्णांक ext2fs_inline_तथ्य_dir_iterate(ext2_filsys fs, ext2_ino_t ino,
				   व्योम *priv_तथ्य)
{
	काष्ठा dir_context *ctx;
	काष्ठा ext2_inode inode;
	काष्ठा ext2_dir_entry dirent;
	काष्ठा ext2_inline_तथ्य तथ्य;
	पूर्णांक ret = BLOCK_ABORT;
	e2_blkcnt_t blockcnt = 0;
	अक्षर *old_buf;
	अचिन्हित पूर्णांक old_buflen;
	पूर्णांक old_flags;

	ctx = (काष्ठा dir_context *)priv_तथ्य;
	old_buf = ctx->buf;
	old_buflen = ctx->buflen;
	old_flags = ctx->flags;
	ctx->flags |= DIRENT_FLAG_INCLUDE_INLINE_तथ्य;

	ctx->errcode = ext2fs_read_inode(fs, ino, &inode);
	अगर (ctx->errcode)
		जाओ out;

	अगर (!(inode.i_flags & EXT4_INLINE_तथ्य_FL)) {
		ctx->errcode = EXT2_ET_NO_INLINE_तथ्य;
		जाओ out;
	}

	अगर (!LINUX_S_ISDIR(inode.i_mode)) {
		ctx->errcode = EXT2_ET_NO_DIRECTORY;
		जाओ out;
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
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_तथ्य);
	अगर (ret & BLOCK_ABORT)
		जाओ out;

	dirent.inode = ext2fs_le32_to_cpu(inode.i_block[0]);
	dirent.name_len = 2;
	ext2fs_set_rec_len(fs, EXT2_DIR_REC_LEN(3), &dirent);
	dirent.name[0] = '.';
	dirent.name[1] = '.';
	dirent.name[2] = '\0';
	ctx->buf = (अक्षर *)&dirent;
	ext2fs_get_rec_len(fs, &dirent, &ctx->buflen);
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_तथ्य);
	अगर (ret & BLOCK_INLINE_तथ्य_CHANGED) {
		errcode_t err;

		inode.i_block[0] = ext2fs_cpu_to_le32(dirent.inode);
		err = ext2fs_write_inode(fs, ino, &inode);
		अगर (err)
			जाओ out;
		ret &= ~BLOCK_INLINE_तथ्य_CHANGED;
	}
	अगर (ret & BLOCK_ABORT)
		जाओ out;

	ctx->buf = (अक्षर *)inode.i_block + EXT4_INLINE_तथ्य_DOTDOT_आकार;
	ctx->buflen = EXT4_MIN_INLINE_तथ्य_आकार - EXT4_INLINE_तथ्य_DOTDOT_आकार;
#अगर_घोषित शब्दगण_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	अगर (ctx->errcode) {
		ret |= BLOCK_ABORT;
		जाओ out;
	}
#पूर्ण_अगर
	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_तथ्य);
	अगर (ret & BLOCK_INLINE_तथ्य_CHANGED) {
#अगर_घोषित शब्दगण_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						       ctx->buflen, 0);
		अगर (ctx->errcode) {
			ret |= BLOCK_ABORT;
			जाओ out;
		}
#पूर्ण_अगर
		ctx->errcode = ext2fs_write_inode(fs, ino, &inode);
		अगर (ctx->errcode)
			ret |= BLOCK_ABORT;
		ret &= ~BLOCK_INLINE_तथ्य_CHANGED;
	}
	अगर (ret & BLOCK_ABORT)
		जाओ out;

	तथ्य.fs = fs;
	तथ्य.ino = ino;
	ctx->errcode = ext2fs_inline_तथ्य_ea_get(&तथ्य);
	अगर (ctx->errcode) {
		ret |= BLOCK_ABORT;
		जाओ out;
	}
	अगर (तथ्य.ea_आकार <= 0)
		जाओ out1;

	ctx->buf = तथ्य.ea_तथ्य;
	ctx->buflen = तथ्य.ea_आकार;
#अगर_घोषित शब्दगण_BIGENDIAN
	ctx->errcode = ext2fs_dirent_swab_in2(fs, ctx->buf, ctx->buflen, 0);
	अगर (ctx->errcode) {
		ret |= BLOCK_ABORT;
		जाओ out1;
	}
#पूर्ण_अगर

	ret |= ext2fs_process_dir_block(fs, 0, blockcnt++, 0, 0, priv_तथ्य);
	अगर (ret & BLOCK_INLINE_तथ्य_CHANGED) {
#अगर_घोषित शब्दगण_BIGENDIAN
		ctx->errcode = ext2fs_dirent_swab_out2(fs, ctx->buf,
						      ctx->buflen, 0);
		अगर (ctx->errcode) {
			ret |= BLOCK_ABORT;
			जाओ out1;
		}
#पूर्ण_अगर
		ctx->errcode = ext2fs_inline_तथ्य_ea_set(&तथ्य);
		अगर (ctx->errcode)
			ret |= BLOCK_ABORT;
	}

out1:
	ext2fs_free_mem(&तथ्य.ea_तथ्य);
out:
	ctx->buf = old_buf;
	ctx->buflen = old_buflen;
	ctx->flags = old_flags;
	ret &= ~(BLOCK_ABORT | BLOCK_INLINE_तथ्य_CHANGED);
	वापस ret;
}

errcode_t ext2fs_inline_तथ्य_ea_remove(ext2_filsys fs, ext2_ino_t ino)
{
	काष्ठा ext2_xattr_handle *handle;
	errcode_t retval;

	retval = ext2fs_xattrs_open(fs, ino, &handle);
	अगर (retval)
		वापस retval;

	retval = ext2fs_xattrs_read(handle);
	अगर (retval)
		जाओ err;

	retval = ext2fs_xattr_remove(handle, "system.data");
err:
	(व्योम) ext2fs_xattrs_close(&handle);
	वापस retval;
}

अटल errcode_t ext2fs_inline_तथ्य_convert_dir(ext2_filsys fs, ext2_ino_t ino,
						अक्षर *bbuf, अक्षर *ibuf, पूर्णांक आकार)
{
	काष्ठा ext2_dir_entry *dir, *dir2;
	काष्ठा ext2_dir_entry_पुच्छ *t;
	errcode_t retval;
	पूर्णांक offset;
	अचिन्हित पूर्णांक rec_len;
	पूर्णांक csum_आकार = 0;
	पूर्णांक filetype = 0;

	अगर (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = माप(काष्ठा ext2_dir_entry_पुच्छ);

	/* Create '.' and '..' */
	अगर (ext2fs_has_feature_filetype(fs->super))
		filetype = EXT2_FT_DIR;

	/*
	 * Set up entry for '.'
	 */
	dir = (काष्ठा ext2_dir_entry *) bbuf;
	dir->inode = ino;
	ext2fs_dirent_set_name_len(dir, 1);
	ext2fs_dirent_set_file_type(dir, filetype);
	dir->name[0] = '.';
	rec_len = (fs->blocksize - csum_आकार) - EXT2_DIR_REC_LEN(1);
	dir->rec_len = EXT2_DIR_REC_LEN(1);

	/*
	 * Set up entry for '..'
	 */
	dir = (काष्ठा ext2_dir_entry *) (bbuf + dir->rec_len);
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
	dir = (काष्ठा ext2_dir_entry *) (bbuf + offset);
	स_नकल(bbuf + offset, ibuf + EXT4_INLINE_तथ्य_DOTDOT_आकार,
	       आकार - EXT4_INLINE_तथ्य_DOTDOT_आकार);
	आकार += EXT2_DIR_REC_LEN(1) + EXT2_DIR_REC_LEN(2) -
		EXT4_INLINE_तथ्य_DOTDOT_आकार;

	करो {
		dir2 = dir;
		retval = ext2fs_get_rec_len(fs, dir, &rec_len);
		अगर (retval)
			जाओ err;
		offset += rec_len;
		dir = (काष्ठा ext2_dir_entry *) (bbuf + offset);
	} जबतक (offset < आकार);
	rec_len += fs->blocksize - csum_आकार - offset;
	retval = ext2fs_set_rec_len(fs, rec_len, dir2);
	अगर (retval)
		जाओ err;

	अगर (csum_आकार) {
		t = EXT2_DIRENT_पुच्छ(bbuf, fs->blocksize);
		ext2fs_initialize_dirent_पुच्छ(fs, t);
	}

err:
	वापस retval;
}

अटल errcode_t
ext2fs_inline_तथ्य_dir_expand(ext2_filsys fs, ext2_ino_t ino,
			      काष्ठा ext2_inode *inode, अक्षर *buf, माप_प्रकार आकार)
{
	errcode_t retval;
	blk64_t blk;
	अक्षर *blk_buf;

	retval = ext2fs_get_memzero(fs->blocksize, &blk_buf);
	अगर (retval)
		वापस retval;

#अगर_घोषित शब्दगण_BIGENDIAN
	retval = ext2fs_dirent_swab_in2(fs, buf + EXT4_INLINE_तथ्य_DOTDOT_आकार,
					आकार, 0);
	अगर (retval)
		जाओ errout;
#पूर्ण_अगर

	/* Adjust the rec_len */
	retval = ext2fs_inline_तथ्य_convert_dir(fs, ino, blk_buf, buf, आकार);
	अगर (retval)
		जाओ errout;
	/* Allocate a new block */
	retval = ext2fs_new_block2(fs, 0, 0, &blk);
	अगर (retval)
		जाओ errout;
	retval = ext2fs_write_dir_block4(fs, blk, blk_buf, 0, ino);
	अगर (retval)
		जाओ errout;

	/* Update inode */
	अगर (ext2fs_has_feature_extents(fs->super))
		inode->i_flags |= EXT4_EXTENTS_FL;
	inode->i_flags &= ~EXT4_INLINE_तथ्य_FL;
	retval = ext2fs_iblk_add_blocks(fs, inode, 1);
	अगर (retval)
		जाओ errout;
	inode->i_आकार = fs->blocksize;
	retval = ext2fs_bmap2(fs, ino, inode, 0, BMAP_SET, 0, 0, &blk);
	अगर (retval)
		जाओ errout;
	retval = ext2fs_write_inode(fs, ino, inode);
	अगर (retval)
		जाओ errout;
	ext2fs_block_alloc_stats(fs, blk, +1);

errout:
	ext2fs_free_mem(&blk_buf);
	वापस retval;
}

अटल errcode_t
ext2fs_inline_तथ्य_file_expand(ext2_filsys fs, ext2_ino_t ino,
			       काष्ठा ext2_inode *inode, अक्षर *buf, माप_प्रकार आकार)
{
	ext2_file_t e2_file;
	errcode_t retval;

	/* Update inode */
	स_रखो(inode->i_block, 0, माप(inode->i_block));
	अगर (ext2fs_has_feature_extents(fs->super)) {
		ext2_extent_handle_t handle;

		inode->i_flags &= ~EXT4_EXTENTS_FL;
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		अगर (retval)
			वापस retval;
		ext2fs_extent_free(handle);
	}
	inode->i_flags &= ~EXT4_INLINE_तथ्य_FL;
	inode->i_आकार = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	अगर (retval)
		वापस retval;

	/* Write out the block buffer */
	retval = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &e2_file);
	अगर (retval)
		वापस retval;
	retval = ext2fs_file_write(e2_file, buf, आकार, 0);
	ext2fs_file_close(e2_file);
	वापस retval;
}

errcode_t ext2fs_inline_तथ्य_expand(ext2_filsys fs, ext2_ino_t ino)
{
	काष्ठा ext2_inode inode;
	काष्ठा ext2_inline_तथ्य तथ्य;
	errcode_t retval;
	माप_प्रकार inline_आकार;
	अक्षर *inline_buf = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	retval = ext2fs_read_inode(fs, ino, &inode);
	अगर (retval)
		वापस retval;

	अगर (!(inode.i_flags & EXT4_INLINE_तथ्य_FL))
		वापस EXT2_ET_NO_INLINE_तथ्य;

	तथ्य.fs = fs;
	तथ्य.ino = ino;
	retval = ext2fs_inline_तथ्य_ea_get(&तथ्य);
	अगर (retval)
		वापस retval;
	inline_आकार = तथ्य.ea_आकार + EXT4_MIN_INLINE_तथ्य_आकार;
	retval = ext2fs_get_mem(inline_आकार, &inline_buf);
	अगर (retval)
		जाओ errout;

	स_नकल(inline_buf, (व्योम *)inode.i_block, EXT4_MIN_INLINE_तथ्य_आकार);
	अगर (तथ्य.ea_आकार > 0) {
		स_नकल(inline_buf + EXT4_MIN_INLINE_तथ्य_आकार,
		       तथ्य.ea_तथ्य, तथ्य.ea_आकार);
	}

	स_रखो((व्योम *)inode.i_block, 0, EXT4_MIN_INLINE_तथ्य_आकार);
	/*
	 * NOTE: We must do this write -> ea_remove -> read cycle here because
	 * removing the inline data EA can free the EA block, which is a change
	 * that our stack copy of the inode will never see.  If that happens,
	 * we can end up with the EA block and lblk 0 pointing to the same
	 * pblk, which is bad news.
	 */
	retval = ext2fs_write_inode(fs, ino, &inode);
	अगर (retval)
		जाओ errout;
	retval = ext2fs_inline_तथ्य_ea_remove(fs, ino);
	अगर (retval)
		जाओ errout;
	retval = ext2fs_read_inode(fs, ino, &inode);
	अगर (retval)
		जाओ errout;

	अगर (LINUX_S_ISDIR(inode.i_mode)) {
		retval = ext2fs_inline_तथ्य_dir_expand(fs, ino, &inode,
						inline_buf, inline_आकार);
	} अन्यथा {
		retval = ext2fs_inline_तथ्य_file_expand(fs, ino, &inode,
						inline_buf, inline_आकार);
	}

errout:
	अगर (inline_buf)
		ext2fs_free_mem(&inline_buf);
	ext2fs_free_mem(&तथ्य.ea_तथ्य);
	वापस retval;
}

/*
 * When caller uses this function to retrieve the inline data, it must
 * allocate a buffer which has the size of inline data.  The size of
 * inline data can be know by ext2fs_inline_data_get_size().
 */
errcode_t ext2fs_inline_तथ्य_get(ext2_filsys fs, ext2_ino_t ino,
				 काष्ठा ext2_inode *inode,
				 व्योम *buf, माप_प्रकार *आकार)
{
	काष्ठा ext2_inode inode_buf;
	काष्ठा ext2_inline_तथ्य तथ्य;
	errcode_t retval;

	अगर (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		अगर (retval)
			वापस retval;
		inode = &inode_buf;
	}

	तथ्य.fs = fs;
	तथ्य.ino = ino;
	retval = ext2fs_inline_तथ्य_ea_get(&तथ्य);
	अगर (retval)
		वापस retval;

	स_नकल(buf, (व्योम *)inode->i_block, EXT4_MIN_INLINE_तथ्य_आकार);
	अगर (तथ्य.ea_आकार > 0)
		स_नकल((अक्षर *) buf + EXT4_MIN_INLINE_तथ्य_आकार,
		       तथ्य.ea_तथ्य, तथ्य.ea_आकार);

	अगर (आकार)
		*आकार = EXT4_MIN_INLINE_तथ्य_आकार + तथ्य.ea_आकार;
	ext2fs_free_mem(&तथ्य.ea_तथ्य);
	वापस 0;
}

errcode_t ext2fs_inline_तथ्य_set(ext2_filsys fs, ext2_ino_t ino,
				 काष्ठा ext2_inode *inode,
				 व्योम *buf, माप_प्रकार आकार)
{
	काष्ठा ext2_inode inode_buf;
	काष्ठा ext2_inline_तथ्य तथ्य = {
		.fs = fs,
		.ino = ino,
	};
	errcode_t retval;
	माप_प्रकार free_ea_आकार, existing_आकार, free_inode_आकार;

	अगर (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		अगर (retval)
			वापस retval;
		inode = &inode_buf;
	}

	अगर (आकार <= EXT4_MIN_INLINE_तथ्य_आकार) {
		स_नकल((व्योम *)inode->i_block, buf, आकार);
	} अन्यथा {
		retval = ext2fs_xattr_inode_max_आकार(fs, ino, &free_ea_आकार);
		अगर (retval)
			वापस retval;

		retval = ext2fs_inline_तथ्य_आकार(fs, ino, &existing_आकार);
		अगर (retval)
			वापस retval;

		अगर (existing_आकार < EXT4_MIN_INLINE_तथ्य_आकार) {
			free_inode_आकार = EXT4_MIN_INLINE_तथ्य_आकार -
					  existing_आकार;
		} अन्यथा {
			free_inode_आकार = 0;
		}

		अगर (आकार != existing_आकार &&
		    आकार > existing_आकार + free_ea_आकार + free_inode_आकार)
			वापस EXT2_ET_INLINE_तथ्य_NO_SPACE;

		स_नकल((व्योम *)inode->i_block, buf, EXT4_MIN_INLINE_तथ्य_आकार);
		अगर (आकार > EXT4_MIN_INLINE_तथ्य_आकार)
			तथ्य.ea_आकार = आकार - EXT4_MIN_INLINE_तथ्य_आकार;
		तथ्य.ea_तथ्य = (अक्षर *) buf + EXT4_MIN_INLINE_तथ्य_आकार;
	}
	retval = ext2fs_write_inode(fs, ino, inode);
	अगर (retval)
		वापस retval;
	वापस ext2fs_inline_तथ्य_ea_set(&तथ्य);
}

#अगर_घोषित DEBUG
#समावेश "e2p/e2p.h"

/*
 * The length of buffer is set to 64 because in inode's i_block member it only
 * can save 60 bytes.  Thus this value can let the data being saved in extra
 * space.
 */
#घोषणा BUFF_आकार (64)

अटल errcode_t file_test(ext2_filsys fs)
{
	काष्ठा ext2_inode inode;
	ext2_ino_t newfile;
	errcode_t retval;
	माप_प्रकार आकार;
	अक्षर *buf = 0, *cmpbuf = 0;

	/* create a new file */
	retval = ext2fs_new_inode(fs, 2, 010755, 0, &newfile);
	अगर (retval) {
		com_err("file_test", retval, "while allocating a new inode");
		वापस 1;
	}

	स_रखो(&inode, 0, माप(inode));
	inode.i_flags |= EXT4_INLINE_तथ्य_FL;
	inode.i_आकार = EXT4_MIN_INLINE_तथ्य_आकार;
	inode.i_mode = LINUX_S_IFREG;
	retval = ext2fs_write_new_inode(fs, newfile, &inode);
	अगर (retval) {
		com_err("file_test", retval, "while writing a new inode");
		वापस 1;
	}

	retval = ext2fs_inline_तथ्य_init(fs, newfile);
	अगर (retval) {
		com_err("file_test", retval, "while init 'system.data'");
		वापस 1;
	}

	retval = ext2fs_inline_तथ्य_आकार(fs, newfile, &आकार);
	अगर (retval) {
		com_err("file_test", retval, "while getting size");
		वापस 1;
	}

	अगर (आकार != EXT4_MIN_INLINE_तथ्य_आकार) {
		ख_लिखो(मानक_त्रुटि,
			"tst_inline_data: size of inline data is wrong\n");
		वापस 1;
	}

	ext2fs_get_mem(BUFF_आकार, &buf);
	स_रखो(buf, 'a', BUFF_आकार);
	retval = ext2fs_inline_तथ्य_set(fs, newfile, 0, buf, BUFF_आकार);
	अगर (retval) {
		com_err("file_test", retval,
			"while setting inline data %s", buf);
		जाओ err;
	}

	ext2fs_get_mem(BUFF_आकार, &cmpbuf);
	retval = ext2fs_inline_तथ्य_get(fs, newfile, 0, cmpbuf, &आकार);
	अगर (retval) {
		com_err("file_test", retval, "while getting inline data");
		जाओ err;
	}

	अगर (आकार != BUFF_आकार) {
		ख_लिखो(मानक_त्रुटि,
			"tst_inline_data: size %zu != buflen %u\n",
			आकार, BUFF_आकार);
		retval = 1;
		जाओ err;
	}

	अगर (स_भेद(buf, cmpbuf, BUFF_आकार)) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data: buf != cmpbuf\n");
		retval = 1;
		जाओ err;
	}

	retval = ext2fs_punch(fs, newfile, 0, 0, 0, ~0ULL);
	अगर (retval) {
		com_err("file_test", retval, "while truncating inode");
		जाओ err;
	}

	/* reload inode and check isize */
	ext2fs_read_inode(fs, newfile, &inode);
	अगर (inode.i_आकार != 0) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data: i_size should be 0\n");
		retval = 1;
	}

err:
	अगर (cmpbuf)
		ext2fs_free_mem(&cmpbuf);
	अगर (buf)
		ext2fs_free_mem(&buf);
	वापस retval;
}

अटल errcode_t dir_test(ext2_filsys fs)
{
	स्थिर अक्षर *dot_name = ".";
	स्थिर अक्षर *stub_name = "stub";
	स्थिर अक्षर *parent_name = "test";
	ext2_ino_t parent, dir, tmp;
	errcode_t retval;
	अक्षर स_नाम[32];
	पूर्णांक i;

	retval = ext2fs_mkdir(fs, 11, 11, stub_name);
	अगर (retval) {
		com_err("dir_test", retval, "while creating %s dir", stub_name);
		वापस retval;
	}

	retval = ext2fs_mkdir(fs, 11, 0, parent_name);
	अगर (retval) {
		com_err("dir_test", retval,
			"while creating %s dir", parent_name);
		वापस retval;
	}

	retval = ext2fs_lookup(fs, 11, parent_name, म_माप(parent_name),
			       0, &parent);
	अगर (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		वापस retval;
	}

	retval = ext2fs_lookup(fs, parent, dot_name, म_माप(dot_name),
			       0, &tmp);
	अगर (retval) {
		com_err("dir_test", retval,
			"while looking up %s dir", parent_name);
		वापस retval;
	}

	अगर (parent != tmp) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data: parent (%u) != tmp (%u)\n",
			parent, tmp);
		वापस 1;
	}

	क्रम (i = 0, dir = 13; i < 4; i++, dir++) {
		tmp = 0;
		snprintf(स_नाम, माप(स_नाम), "%d", i);
		retval = ext2fs_mkdir(fs, parent, 0, स_नाम);
		अगर (retval) {
			com_err("dir_test", retval,
				"while creating %s dir", स_नाम);
			वापस retval;
		}

		retval = ext2fs_lookup(fs, parent, स_नाम, म_माप(स_नाम),
				       0, &tmp);
		अगर (retval) {
			com_err("dir_test", retval,
				"while looking up %s dir", parent_name);
			वापस retval;
		}

		अगर (dir != tmp) {
			ख_लिखो(मानक_त्रुटि,
				"tst_inline_data: dir (%u) != tmp (%u)\n",
				dir, tmp);
			वापस 1;
		}
	}

	snprintf(स_नाम, माप(स_नाम), "%d", i);
	retval = ext2fs_mkdir(fs, parent, 0, स_नाम);
	अगर (retval && retval != EXT2_ET_DIR_NO_SPACE) {
		com_err("dir_test", retval, "while creating %s dir", स_नाम);
		वापस retval;
	}

	retval = ext2fs_expand_dir(fs, parent);
	अगर (retval) {
		com_err("dir_test", retval, "while expanding %s dir", parent_name);
		वापस retval;
	}

	वापस 0;
}

पूर्णांक मुख्य(पूर्णांक argc, अक्षर *argv[])
{
	ext2_filsys		fs;
	काष्ठा ext2_super_block param;
	errcode_t		retval;

	/* setup */
	initialize_ext2_error_table();

	स_रखो(&param, 0, माप(param));
	ext2fs_blocks_गिनती_set(&param, 32768);
	param.s_inodes_गिनती = 100;

	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_INLINE_तथ्य;
	param.s_rev_level = EXT2_DYNAMIC_REV;
	param.s_inode_आकार = 256;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	अगर (retval) {
		com_err("setup", retval,
			"while initializing filesystem");
		निकास(1);
	}

	retval = ext2fs_allocate_tables(fs);
	अगर (retval) {
		com_err("setup", retval,
			"while allocating tables for test filesystem");
		निकास(1);
	}

	/* initialize inode cache */
	अगर (!fs->icache) {
		ext2_ino_t first_ino = EXT2_FIRST_INO(fs->super);
		पूर्णांक i;

		/* we just want to init inode cache.  So ignore error */
		ext2fs_create_inode_cache(fs, 16);
		अगर (!fs->icache) {
			ख_लिखो(मानक_त्रुटि,
				"tst_inline_data: init inode cache failed\n");
			निकास(1);
		}

		/* setup inode cache */
		क्रम (i = 0; i < fs->icache->cache_आकार; i++)
			fs->icache->cache[i].ino = first_ino++;
	}

	/* test */
	अगर (file_test(fs)) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data(FILE): FAILED\n");
		वापस 1;
	}
	म_लिखो("tst_inline_data(FILE): OK\n");

	अगर (dir_test(fs)) {
		ख_लिखो(मानक_त्रुटि, "tst_inline_data(DIR): FAILED\n");
		वापस 1;
	}
	म_लिखो("tst_inline_data(DIR): OK\n");
	ext2fs_free(fs);

	वापस 0;
}
#पूर्ण_अगर
