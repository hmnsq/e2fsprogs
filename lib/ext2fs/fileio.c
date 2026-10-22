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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	संरचना ext2_inode	inode;
	पूर्णांक 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	अक्षर 			*buf;
};

संरचना block_entry {
	blk64_t		physblock;
	अचिन्हित अक्षर 	sha[EXT2FS_SHA512_LENGTH];
};
प्रकारव्याख्या संरचना block_entry *block_entry_t;

#define BMAP_BUFFER (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode *inode,
			    पूर्णांक flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	जर ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		परत EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(आकार(संरचना ext2_file), &file);
	जर (retval)
		परत retval;

	स_रखो(file, 0, आकार(संरचना ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	जर (inode) {
		स_नकल(&file->inode, inode, आकार(संरचना ext2_inode));
	} अन्यथा {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		जर (retval)
			जा fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	जर (retval)
		जा fail;

	*ret = file;
	परत 0;

fail:
	जर (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	परत retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   पूर्णांक flags, ext2_file_t *ret)
{
	परत ext2fs_file_open2(fs, ino, रिक्त, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	जर (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		परत 0;
	परत file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
संरचना ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	जर (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		परत रिक्त;
	परत &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_अंक(ext2_file_t file)
{
	जर (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		परत 0;
	परत file->ino;
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

	जर (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		परत 0;

	/* Is this an uninit block? */
	जर (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		जर (retval)
			परत retval;
		जर (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			जर (retval)
				परत retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	जर (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		जर (retval)
			परत retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	जर (retval)
		परत retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	परत retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
स्थिर errcode_t sync_buffer_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	जर (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		जर (retval)
			परत retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	परत 0;
}

/*
 * This function loads the file's block buffer with valid data from
 * the disk as necessary.
 *
 * If dontfill is true, then skip initializing the buffer since we're
 * going to be replacing its entire contents anyway.  If set, then the
 * function basically only sets file->physblock and EXT2_FILE_BUF_VALID
 */
#define DONTFILL 1
स्थिर errcode_t load_buffer(ext2_file_t file, पूर्णांक dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	पूर्णांक		ret_flags;

	जर (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		जर (retval)
			परत retval;
		जर (!dontfill) {
			जर (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				जर (retval)
					परत retval;
			} अन्यथा
				स_रखो(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	परत 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	जर (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	परत retval;
}


स्थिर errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, शून्य *buf,
			     अचिन्हित पूर्णांक wanted, अचिन्हित पूर्णांक *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	अचिन्हित पूर्णांक मोजणी = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	माप_प्रकार size;

	जर (file->pos >= isize)
		जा out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	जर (retval)
		परत retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	जर (size > isize)
		size = isize;

	मोजणी = size - file->pos;
	जर (मोजणी > wanted)
		मोजणी = wanted;
	स_नकल(buf, file->buf + file->pos, मोजणी);
	file->pos += मोजणी;
	buf = (अक्षर *) buf + मोजणी;

	/* zero-fill the rest of the buffer */
	wanted -= मोजणी;
	जर (wanted > 0) {
		स_रखो(buf, 0, wanted);
		file->pos += wanted;
		मोजणी += wanted;
	}

out:
	जर (got)
		*got = मोजणी;
	परत retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, शून्य *buf,
			   अचिन्हित पूर्णांक wanted, अचिन्हित पूर्णांक *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	अचिन्हित पूर्णांक	start, c, मोजणी = 0;
	__u64		डावा;
	अक्षर		*ptr = (अक्षर *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	जर (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		परत ext2fs_file_read_inline_data(file, buf, wanted, got);

	पर्यंत ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		जर (retval)
			जा fail;
		retval = load_buffer(file, 0);
		जर (retval)
			जा fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		जर (c > wanted)
			c = wanted;
		डावा = EXT2_I_SIZE(&file->inode) - file->pos ;
		जर (c > डावा)
			c = डावा;

		स_नकल(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		मोजणी += c;
		wanted -= c;
	}

fail:
	जर (got)
		*got = मोजणी;
	परत retval;
}


स्थिर errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, स्थिरांक शून्य *buf,
			      अचिन्हित पूर्णांक nbytes, अचिन्हित पूर्णांक *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	माप_प्रकार size;

	जर (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	जर (retval)
		परत retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	जर (new_isize < fs->blocksize) {
		जर (file->pos > old_isize)
			स_रखो(file->buf + old_isize, 0, file->pos - old_isize);

		स_नकल(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		जर (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			जा expand;
		जर (retval)
			परत retval;

		file->pos += nbytes;

		/* Update inode size */
		जर (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			जर (retval == 0)
				retval = rc;
		}

		जर (written)
			*written = nbytes;
		परत 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	जर (retval)
		परत retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	जर (retval)
		परत retval;
	परत EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, स्थिरांक शून्य *buf,
			    अचिन्हित पूर्णांक nbytes, अचिन्हित पूर्णांक *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	अचिन्हित पूर्णांक	start, c, मोजणी = 0;
	स्थिरांक अक्षर	*ptr = (स्थिरांक अक्षर *) buf;
	block_entry_t	new_block = रिक्त, old_block = रिक्त;
	पूर्णांक		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	जर (!(file->flags & EXT2_FILE_WRITE))
		परत EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	जर (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		जर (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			परत retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	पर्यंत (nbytes > 0) {
		retval = sync_buffer_position(file);
		जर (retval)
			जा fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		जर (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		जर (retval)
			जा fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		स_नकल(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		जर (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			जर (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = सुस्मृति(1, आकार(*new_block));
				जर (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					जा fail;
				}
				ext2fs_sha512((स्थिरांक अचिन्हित अक्षर*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							आकार(new_block->sha));
			}

			जर (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				मुक्त(new_block);
				new_block = रिक्त;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			जर (retval) {
				मुक्त(new_block);
				new_block = रिक्त;
				जा fail;
			}

			जर (new_block) {
				new_block->physblock = file->physblock;
				पूर्णांक ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						आकार(new_block->sha));
				जर (ret) {
					retval = EXT2_ET_NO_MEMORY;
					मुक्त(new_block);
					जा fail;
				}
			}

			जर (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		मोजणी += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	जर (मोजणी != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		जर (retval == 0)
			retval = rc;
	}

	जर (written)
		*written = मोजणी;
	परत retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    पूर्णांक whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	जर (whence == EXT2_SEEK_SET)
		file->pos = offset;
	अन्यथा जर (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	अन्यथा जर (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	अन्यथा
		परत EXT2_ET_INVALID_ARGUMENT;

	जर (ret_pos)
		*ret_pos = file->pos;

	परत 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    पूर्णांक whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	जर (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	परत retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	जर (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		परत EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	परत 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	जर (ext2fs_file_get_lsize(file, &size))
		परत 0;
	जर ((size >> 32) != 0)
		परत 0;
	परत size;
}

/* Zero the parts of the last block that are past EOF. */
स्थिर errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	अक्षर *b = रिक्त;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	पूर्णांक ret_flags;
	errcode_t retval;

	जर (off == 0)
		परत 0;

	retval = sync_buffer_position(file);
	जर (retval)
		परत retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, रिक्त, रिक्त, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	जर (retval)
		परत retval;
	जर ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		परत 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	जर (retval)
		परत retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	जर (retval)
		जा out;

	स_रखो(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	जर (retval)
		जा out;

out:
	ext2fs_free_mem(&b);
	परत retval;
}

/*
 * This function sets the size of the file, truncating it if necessary
 *
 */
errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t size)
{
	ext2_off64_t	old_size;
	errcode_t	retval;
	blk64_t		old_truncate, truncate_block;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	जर (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		परत EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	जर (retval)
		परत retval;

	जर (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		जर (retval)
			परत retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	जर (retval)
		परत retval;

	जर (truncate_block >= old_truncate)
		परत 0;

	परत ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	परत ext2fs_file_set_size2(file, size);
}
