/*
 * fileio.c --- Simple file I/O routines
 *
 * Copyright (C) 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

काष्ठा ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	काष्ठा ext2_inode	inode;
	पूर्णांक 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	अक्षर 			*buf;
};

काष्ठा block_entry {
	blk64_t		physblock;
	अचिन्हित अक्षर 	sha[EXT2_FS_SHA512_लंबाई];
};
प्रकार काष्ठा block_entry *block_entry_t;

#घोषणा BMAP_बफर (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    काष्ठा ext2_inode *inode,
			    पूर्णांक flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	अगर ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(माप(काष्ठा ext2_file), &file);
	अगर (retval)
		वापस retval;

	स_रखो(file, 0, माप(काष्ठा ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	अगर (inode) {
		स_नकल(&file->inode, inode, माप(काष्ठा ext2_inode));
	} अन्यथा {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		अगर (retval)
			जाओ fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	अगर (retval)
		जाओ fail;

	*ret = file;
	वापस 0;

fail:
	अगर (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	वापस retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   पूर्णांक flags, ext2_file_t *ret)
{
	वापस ext2fs_file_open2(fs, ino, शून्य, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	अगर (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		वापस 0;
	वापस file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
काष्ठा ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	अगर (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		वापस शून्य;
	वापस &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_अंक(ext2_file_t file)
{
	अगर (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		वापस 0;
	वापस file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	पूर्णांक		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	अगर (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		वापस 0;

	/* Is this an uninit block? */
	अगर (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_बफर,
				      0, file->blockno, &ret_flags, &dontcare);
		अगर (retval)
			वापस retval;
		अगर (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_बफर, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			अगर (retval)
				वापस retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	अगर (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_बफर, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		अगर (retval)
			वापस retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	अगर (retval)
		वापस retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	वापस retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
अटल errcode_t sync_बफर_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	अगर (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		अगर (retval)
			वापस retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	वापस 0;
}

/*
 * This function loads the file's block buffer with valid data from
 * the disk as necessary.
 *
 * If dontfill is true, then skip initializing the buffer since we're
 * going to be replacing its entire contents anyway.  If set, then the
 * function basically only sets file->physblock and EXT2_FILE_BUF_VALID
 */
#घोषणा DONTFILL 1
अटल errcode_t load_बफर(ext2_file_t file, पूर्णांक dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	पूर्णांक		ret_flags;

	अगर (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_बफर, 0, file->blockno, &ret_flags,
				     &file->physblock);
		अगर (retval)
			वापस retval;
		अगर (!dontfill) {
			अगर (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				अगर (retval)
					वापस retval;
			} अन्यथा
				स_रखो(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	वापस 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	अगर (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	वापस retval;
}


अटल errcode_t
ext2fs_file_read_inline_तथ्य(ext2_file_t file, व्योम *buf,
			     अचिन्हित पूर्णांक wanted, अचिन्हित पूर्णांक *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	अचिन्हित पूर्णांक गिनती = 0;
	uint64_t isize = EXT2_I_आकार(&file->inode);
	माप_प्रकार आकार;

	अगर (file->pos >= isize)
		जाओ out;

	fs = file->fs;
	retval = ext2fs_inline_तथ्य_get(fs, file->ino, &file->inode,
					file->buf, &आकार);
	अगर (retval)
		वापस retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	अगर (आकार > isize)
		आकार = isize;

	गिनती = आकार - file->pos;
	अगर (गिनती > wanted)
		गिनती = wanted;
	स_नकल(buf, file->buf + file->pos, गिनती);
	file->pos += गिनती;
	buf = (अक्षर *) buf + गिनती;

	/* zero-fill the rest of the buffer */
	wanted -= गिनती;
	अगर (wanted > 0) {
		स_रखो(buf, 0, wanted);
		file->pos += wanted;
		गिनती += wanted;
	}

out:
	अगर (got)
		*got = गिनती;
	वापस retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, व्योम *buf,
			   अचिन्हित पूर्णांक wanted, अचिन्हित पूर्णांक *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	अचिन्हित पूर्णांक	start, c, गिनती = 0;
	__u64		बायाँ;
	अक्षर		*ptr = (अक्षर *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	अगर (file->inode.i_flags & EXT4_INLINE_तथ्य_FL)
		वापस ext2fs_file_read_inline_तथ्य(file, buf, wanted, got);

	जबतक ((file->pos < EXT2_I_आकार(&file->inode)) && (wanted > 0)) {
		retval = sync_बफर_position(file);
		अगर (retval)
			जाओ fail;
		retval = load_बफर(file, 0);
		अगर (retval)
			जाओ fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		अगर (c > wanted)
			c = wanted;
		बायाँ = EXT2_I_आकार(&file->inode) - file->pos ;
		अगर (c > बायाँ)
			c = बायाँ;

		स_नकल(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		गिनती += c;
		wanted -= c;
	}

fail:
	अगर (got)
		*got = गिनती;
	वापस retval;
}


अटल errcode_t
ext2fs_file_write_inline_तथ्य(ext2_file_t file, स्थिर व्योम *buf,
			      अचिन्हित पूर्णांक nbytes, अचिन्हित पूर्णांक *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_आकार(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	माप_प्रकार आकार;

	अगर (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_तथ्य_get(fs, file->ino, &file->inode,
					file->buf, &आकार);
	अगर (retval)
		वापस retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	अगर (new_isize < fs->blocksize) {
		अगर (file->pos > old_isize)
			स_रखो(file->buf + old_isize, 0, file->pos - old_isize);

		स_नकल(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_तथ्य_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		अगर (retval == EXT2_ET_INLINE_तथ्य_NO_SPACE)
			जाओ expand;
		अगर (retval)
			वापस retval;

		file->pos += nbytes;

		/* Update inode size */
		अगर (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			अगर (retval == 0)
				retval = rc;
		}

		अगर (written)
			*written = nbytes;
		वापस 0;
	}

expand:
	retval = ext2fs_inline_तथ्य_expand(fs, file->ino);
	अगर (retval)
		वापस retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	अगर (retval)
		वापस retval;
	वापस EXT2_ET_INLINE_तथ्य_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, स्थिर व्योम *buf,
			    अचिन्हित पूर्णांक nbytes, अचिन्हित पूर्णांक *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	अचिन्हित पूर्णांक	start, c, गिनती = 0;
	स्थिर अक्षर	*ptr = (स्थिर अक्षर *) buf;
	block_entry_t	new_block = शून्य, old_block = शून्य;
	पूर्णांक		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	अगर (!(file->flags & EXT2_FILE_WRITE))
		वापस EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	अगर (file->inode.i_flags & EXT4_INLINE_तथ्य_FL) {
		retval = ext2fs_file_write_inline_तथ्य(file, buf, nbytes,
						       written);
		अगर (retval != EXT2_ET_INLINE_तथ्य_NO_SPACE)
			वापस retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	जबतक (nbytes > 0) {
		retval = sync_बफर_position(file);
		अगर (retval)
			जाओ fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		अगर (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_बफर(file, (c == fs->blocksize));
		अगर (retval)
			जाओ fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		स_नकल(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		अगर (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			अगर (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = सुस्मृति(1, माप(*new_block));
				अगर (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					जाओ fail;
				}
				ext2fs_sha512((स्थिर अचिन्हित अक्षर*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							माप(new_block->sha));
			}

			अगर (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				मुक्त(new_block);
				new_block = शून्य;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_बफर,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			अगर (retval) {
				मुक्त(new_block);
				new_block = शून्य;
				जाओ fail;
			}

			अगर (new_block) {
				new_block->physblock = file->physblock;
				पूर्णांक ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						माप(new_block->sha));
				अगर (ret) {
					retval = EXT2_ET_NO_MEMORY;
					मुक्त(new_block);
					जाओ fail;
				}
			}

			अगर (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		गिनती += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	अगर (गिनती != 0 && EXT2_I_आकार(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		अगर (retval == 0)
			retval = rc;
	}

	अगर (written)
		*written = गिनती;
	वापस retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    पूर्णांक whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	अगर (whence == EXT2_SEEK_SET)
		file->pos = offset;
	अन्यथा अगर (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	अन्यथा अगर (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_आकार(&file->inode) + offset;
	अन्यथा
		वापस EXT2_ET_INVALID_ARGUMENT;

	अगर (ret_pos)
		*ret_pos = file->pos;

	वापस 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    पूर्णांक whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	अगर (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	वापस retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_आकार)
{
	अगर (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		वापस EXT2_ET_MAGIC_EXT2_FILE;
	*ret_आकार = EXT2_I_आकार(&file->inode);
	वापस 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_आकार(ext2_file_t file)
{
	__u64	आकार;

	अगर (ext2fs_file_get_lsize(file, &आकार))
		वापस 0;
	अगर ((आकार >> 32) != 0)
		वापस 0;
	वापस आकार;
}

/* Zero the parts of the last block that are past EOF. */
अटल errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	अक्षर *b = शून्य;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	पूर्णांक ret_flags;
	errcode_t retval;

	अगर (off == 0)
		वापस 0;

	retval = sync_बफर_position(file);
	अगर (retval)
		वापस retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, शून्य, शून्य, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	अगर (retval)
		वापस retval;
	अगर ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		वापस 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	अगर (retval)
		वापस retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	अगर (retval)
		जाओ out;

	स_रखो(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	अगर (retval)
		जाओ out;

out:
	ext2fs_free_mem(&b);
	वापस retval;
}

/*
 * This function sets the size of the file, truncating it if necessary
 *
 */
errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t आकार)
{
	ext2_off64_t	old_आकार;
	errcode_t	retval;
	blk64_t		old_truncate, truncate_block;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	अगर (आकार && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(आकार - 1) / file->fs->blocksize))
		वापस EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((आकार + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_आकार_BITS(file->fs->super));
	old_आकार = EXT2_I_आकार(&file->inode);
	old_truncate = ((old_आकार + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_आकार_BITS(file->fs->super));

	retval = ext2fs_inode_आकार_set(file->fs, &file->inode, आकार);
	अगर (retval)
		वापस retval;

	अगर (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		अगर (retval)
			वापस retval;
	}

	retval = ext2fs_file_zero_past_offset(file, आकार);
	अगर (retval)
		वापस retval;

	अगर (truncate_block >= old_truncate)
		वापस 0;

	वापस ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_आकार(ext2_file_t file, ext2_off_t आकार)
{
	वापस ext2fs_file_set_size2(file, आकार);
}
