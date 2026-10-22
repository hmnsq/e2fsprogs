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
#सचे HAVE_UNISTD_H
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
	पूर्णाङ्क 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	वर्ण 			*buf;
};

संरचना block_entry {
	blk64_t		physblock;
	unsigned वर्ण 	sha[EXT2FS_SHA512_LENGTH];
};
प्रकारनिर्देश संरचना block_entry *block_entry_t;

#define BMAP_BUFFER (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode *inode,
			    पूर्णाङ्क flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	सचे ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		पटिनिवत्त EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_file), &file);
	सचे (retval)
		पटिनिवत्त retval;

	memset(file, 0, परिमाण(संरचना ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	सचे (inode) {
		memcpy(&file->inode, inode, परिमाण(संरचना ext2_inode));
	} अञ्ञथा {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		सचे (retval)
			गच्छ fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	सचे (retval)
		गच्छ fail;

	*ret = file;
	पटिनिवत्त 0;

fail:
	सचे (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	पटिनिवत्त retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   पूर्णाङ्क flags, ext2_file_t *ret)
{
	पटिनिवत्त ext2fs_file_open2(fs, ino, रिक्त, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	सचे (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		पटिनिवत्त 0;
	पटिनिवत्त file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
संरचना ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	सचे (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		पटिनिवत्त रिक्त;
	पटिनिवत्त &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_अङ्कः(ext2_file_t file)
{
	सचे (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		पटिनिवत्त 0;
	पटिनिवत्त file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	पूर्णाङ्क		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	सचे (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		पटिनिवत्त 0;

	/* Is this an uninit block? */
	सचे (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		सचे (retval)
			पटिनिवत्त retval;
		सचे (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			सचे (retval)
				पटिनिवत्त retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	सचे (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		सचे (retval)
			पटिनिवत्त retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	सचे (retval)
		पटिनिवत्त retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	पटिनिवत्त retval;
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
	सचे (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		सचे (retval)
			पटिनिवत्त retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	पटिनिवत्त 0;
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
स्थिर errcode_t load_buffer(ext2_file_t file, पूर्णाङ्क dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	पूर्णाङ्क		ret_flags;

	सचे (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		सचे (retval)
			पटिनिवत्त retval;
		सचे (!dontfill) {
			सचे (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				सचे (retval)
					पटिनिवत्त retval;
			} अञ्ञथा
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	पटिनिवत्त 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	सचे (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	पटिनिवत्त retval;
}


स्थिर errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, शून्य *buf,
			     unsigned पूर्णाङ्क wanted, unsigned पूर्णाङ्क *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned पूर्णाङ्क गणनम् = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	size_t size;

	सचे (file->pos >= isize)
		गच्छ out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	सचे (retval)
		पटिनिवत्त retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	सचे (size > isize)
		size = isize;

	गणनम् = size - file->pos;
	सचे (गणनम् > wanted)
		गणनम् = wanted;
	memcpy(buf, file->buf + file->pos, गणनम्);
	file->pos += गणनम्;
	buf = (वर्ण *) buf + गणनम्;

	/* zero-fill the rest of the buffer */
	wanted -= गणनम्;
	सचे (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		गणनम् += wanted;
	}

out:
	सचे (got)
		*got = गणनम्;
	पटिनिवत्त retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, शून्य *buf,
			   unsigned पूर्णाङ्क wanted, unsigned पूर्णाङ्क *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned पूर्णाङ्क	start, c, गणनम् = 0;
	__u64		वामः;
	वर्ण		*ptr = (वर्ण *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	सचे (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		पटिनिवत्त ext2fs_file_read_inline_data(file, buf, wanted, got);

	याव ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		सचे (retval)
			गच्छ fail;
		retval = load_buffer(file, 0);
		सचे (retval)
			गच्छ fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		सचे (c > wanted)
			c = wanted;
		वामः = EXT2_I_SIZE(&file->inode) - file->pos ;
		सचे (c > वामः)
			c = वामः;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		गणनम् += c;
		wanted -= c;
	}

fail:
	सचे (got)
		*got = गणनम्;
	पटिनिवत्त retval;
}


स्थिर errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, ध्रुव शून्य *buf,
			      unsigned पूर्णाङ्क nbytes, unsigned पूर्णाङ्क *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t size;

	सचे (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	सचे (retval)
		पटिनिवत्त retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	सचे (new_isize < fs->blocksize) {
		सचे (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		सचे (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			गच्छ expand;
		सचे (retval)
			पटिनिवत्त retval;

		file->pos += nbytes;

		/* Update inode size */
		सचे (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			सचे (retval == 0)
				retval = rc;
		}

		सचे (written)
			*written = nbytes;
		पटिनिवत्त 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	सचे (retval)
		पटिनिवत्त retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	सचे (retval)
		पटिनिवत्त retval;
	पटिनिवत्त EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ध्रुव शून्य *buf,
			    unsigned पूर्णाङ्क nbytes, unsigned पूर्णाङ्क *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned पूर्णाङ्क	start, c, गणनम् = 0;
	ध्रुव वर्ण	*ptr = (ध्रुव वर्ण *) buf;
	block_entry_t	new_block = रिक्त, old_block = रिक्त;
	पूर्णाङ्क		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	सचे (!(file->flags & EXT2_FILE_WRITE))
		पटिनिवत्त EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	सचे (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		सचे (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			पटिनिवत्त retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	याव (nbytes > 0) {
		retval = sync_buffer_position(file);
		सचे (retval)
			गच्छ fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		सचे (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		सचे (retval)
			गच्छ fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		सचे (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			सचे (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, परिमाण(*new_block));
				सचे (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					गच्छ fail;
				}
				ext2fs_sha512((ध्रुव unsigned वर्ण*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							परिमाण(new_block->sha));
			}

			सचे (old_block) {
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
			सचे (retval) {
				मुक्त(new_block);
				new_block = रिक्त;
				गच्छ fail;
			}

			सचे (new_block) {
				new_block->physblock = file->physblock;
				पूर्णाङ्क ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						परिमाण(new_block->sha));
				सचे (ret) {
					retval = EXT2_ET_NO_MEMORY;
					मुक्त(new_block);
					गच्छ fail;
				}
			}

			सचे (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		गणनम् += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	सचे (गणनम् != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		सचे (retval == 0)
			retval = rc;
	}

	सचे (written)
		*written = गणनम्;
	पटिनिवत्त retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    पूर्णाङ्क whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	सचे (whence == EXT2_SEEK_SET)
		file->pos = offset;
	अञ्ञथा सचे (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	अञ्ञथा सचे (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	अञ्ञथा
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	सचे (ret_pos)
		*ret_pos = file->pos;

	पटिनिवत्त 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    पूर्णाङ्क whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	सचे (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	पटिनिवत्त retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	सचे (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		पटिनिवत्त EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	पटिनिवत्त 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	सचे (ext2fs_file_get_lsize(file, &size))
		पटिनिवत्त 0;
	सचे ((size >> 32) != 0)
		पटिनिवत्त 0;
	पटिनिवत्त size;
}

/* Zero the parts of the last block that are past EOF. */
स्थिर errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	वर्ण *b = रिक्त;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	पूर्णाङ्क ret_flags;
	errcode_t retval;

	सचे (off == 0)
		पटिनिवत्त 0;

	retval = sync_buffer_position(file);
	सचे (retval)
		पटिनिवत्त retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, रिक्त, रिक्त, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	सचे (retval)
		पटिनिवत्त retval;
	सचे ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		पटिनिवत्त 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	सचे (retval)
		पटिनिवत्त retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	सचे (retval)
		गच्छ out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	सचे (retval)
		गच्छ out;

out:
	ext2fs_free_mem(&b);
	पटिनिवत्त retval;
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

	सचे (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		पटिनिवत्त EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	सचे (retval)
		पटिनिवत्त retval;

	सचे (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		सचे (retval)
			पटिनिवत्त retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	सचे (retval)
		पटिनिवत्त retval;

	सचे (truncate_block >= old_truncate)
		पटिनिवत्त 0;

	पटिनिवत्त ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	पटिनिवत्त ext2fs_file_set_size2(file, size);
}
