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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

கட்டமைப்பு ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	கட்டமைப்பு ext2_inode	inode;
	முழு 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	எழுத்து 			*buf;
};

கட்டமைப்பு block_entry {
	blk64_t		physblock;
	unsigned எழுத்து 	sha[EXT2FS_SHA512_LENGTH];
};
வகைவரை கட்டமைப்பு block_entry *block_entry_t;

#define BMAP_BUFFER (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    கட்டமைப்பு ext2_inode *inode,
			    முழு flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	எனில் ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_file), &file);
	எனில் (retval)
		திரும்பு retval;

	memset(file, 0, அளவு(கட்டமைப்பு ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	எனில் (inode) {
		memcpy(&file->inode, inode, அளவு(கட்டமைப்பு ext2_inode));
	} இல்லையெனில் {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		எனில் (retval)
			செல் fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	எனில் (retval)
		செல் fail;

	*ret = file;
	திரும்பு 0;

fail:
	எனில் (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	திரும்பு retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   முழு flags, ext2_file_t *ret)
{
	திரும்பு ext2fs_file_open2(fs, ino, வெற்று, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	எனில் (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		திரும்பு 0;
	திரும்பு file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
கட்டமைப்பு ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	எனில் (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		திரும்பு வெற்று;
	திரும்பு &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_இலக்கம்(ext2_file_t file)
{
	எனில் (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		திரும்பு 0;
	திரும்பு file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	முழு		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	எனில் (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		திரும்பு 0;

	/* Is this an uninit block? */
	எனில் (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		எனில் (retval)
			திரும்பு retval;
		எனில் (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			எனில் (retval)
				திரும்பு retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	எனில் (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		எனில் (retval)
			திரும்பு retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	எனில் (retval)
		திரும்பு retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	திரும்பு retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
நிலையான errcode_t sync_buffer_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	எனில் (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		எனில் (retval)
			திரும்பு retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	திரும்பு 0;
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
நிலையான errcode_t load_buffer(ext2_file_t file, முழு dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	முழு		ret_flags;

	எனில் (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		எனில் (retval)
			திரும்பு retval;
		எனில் (!dontfill) {
			எனில் (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				எனில் (retval)
					திரும்பு retval;
			} இல்லையெனில்
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	திரும்பு 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	எனில் (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	திரும்பு retval;
}


நிலையான errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, வெறுமை *buf,
			     unsigned முழு wanted, unsigned முழு *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned முழு மொத்தம் = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	size_t size;

	எனில் (file->pos >= isize)
		செல் out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	எனில் (retval)
		திரும்பு retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	எனில் (size > isize)
		size = isize;

	மொத்தம் = size - file->pos;
	எனில் (மொத்தம் > wanted)
		மொத்தம் = wanted;
	memcpy(buf, file->buf + file->pos, மொத்தம்);
	file->pos += மொத்தம்;
	buf = (எழுத்து *) buf + மொத்தம்;

	/* zero-fill the rest of the buffer */
	wanted -= மொத்தம்;
	எனில் (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		மொத்தம் += wanted;
	}

out:
	எனில் (got)
		*got = மொத்தம்;
	திரும்பு retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, வெறுமை *buf,
			   unsigned முழு wanted, unsigned முழு *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned முழு	start, c, மொத்தம் = 0;
	__u64		இடது;
	எழுத்து		*ptr = (எழுத்து *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	எனில் (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		திரும்பு ext2fs_file_read_inline_data(file, buf, wanted, got);

	போது ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		எனில் (retval)
			செல் fail;
		retval = load_buffer(file, 0);
		எனில் (retval)
			செல் fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		எனில் (c > wanted)
			c = wanted;
		இடது = EXT2_I_SIZE(&file->inode) - file->pos ;
		எனில் (c > இடது)
			c = இடது;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		மொத்தம் += c;
		wanted -= c;
	}

fail:
	எனில் (got)
		*got = மொத்தம்;
	திரும்பு retval;
}


நிலையான errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, மாறிலி வெறுமை *buf,
			      unsigned முழு nbytes, unsigned முழு *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t size;

	எனில் (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	எனில் (retval)
		திரும்பு retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	எனில் (new_isize < fs->blocksize) {
		எனில் (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		எனில் (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			செல் expand;
		எனில் (retval)
			திரும்பு retval;

		file->pos += nbytes;

		/* Update inode size */
		எனில் (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			எனில் (retval == 0)
				retval = rc;
		}

		எனில் (written)
			*written = nbytes;
		திரும்பு 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	எனில் (retval)
		திரும்பு retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	எனில் (retval)
		திரும்பு retval;
	திரும்பு EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, மாறிலி வெறுமை *buf,
			    unsigned முழு nbytes, unsigned முழு *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned முழு	start, c, மொத்தம் = 0;
	மாறிலி எழுத்து	*ptr = (மாறிலி எழுத்து *) buf;
	block_entry_t	new_block = வெற்று, old_block = வெற்று;
	முழு		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	எனில் (!(file->flags & EXT2_FILE_WRITE))
		திரும்பு EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	எனில் (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		எனில் (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			திரும்பு retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	போது (nbytes > 0) {
		retval = sync_buffer_position(file);
		எனில் (retval)
			செல் fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		எனில் (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		எனில் (retval)
			செல் fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		எனில் (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			எனில் (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, அளவு(*new_block));
				எனில் (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					செல் fail;
				}
				ext2fs_sha512((மாறிலி unsigned எழுத்து*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							அளவு(new_block->sha));
			}

			எனில் (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				விடுவி(new_block);
				new_block = வெற்று;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			எனில் (retval) {
				விடுவி(new_block);
				new_block = வெற்று;
				செல் fail;
			}

			எனில் (new_block) {
				new_block->physblock = file->physblock;
				முழு ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						அளவு(new_block->sha));
				எனில் (ret) {
					retval = EXT2_ET_NO_MEMORY;
					விடுவி(new_block);
					செல் fail;
				}
			}

			எனில் (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		மொத்தம் += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	எனில் (மொத்தம் != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		எனில் (retval == 0)
			retval = rc;
	}

	எனில் (written)
		*written = மொத்தம்;
	திரும்பு retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    முழு whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	எனில் (whence == EXT2_SEEK_SET)
		file->pos = offset;
	இல்லையெனில் எனில் (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	இல்லையெனில் எனில் (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	இல்லையெனில்
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	எனில் (ret_pos)
		*ret_pos = file->pos;

	திரும்பு 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    முழு whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	எனில் (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	திரும்பு retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	எனில் (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		திரும்பு EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	திரும்பு 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	எனில் (ext2fs_file_get_lsize(file, &size))
		திரும்பு 0;
	எனில் ((size >> 32) != 0)
		திரும்பு 0;
	திரும்பு size;
}

/* Zero the parts of the last block that are past EOF. */
நிலையான errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	எழுத்து *b = வெற்று;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	முழு ret_flags;
	errcode_t retval;

	எனில் (off == 0)
		திரும்பு 0;

	retval = sync_buffer_position(file);
	எனில் (retval)
		திரும்பு retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, வெற்று, வெற்று, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	எனில் (retval)
		திரும்பு retval;
	எனில் ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		திரும்பு 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	எனில் (retval)
		திரும்பு retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	எனில் (retval)
		செல் out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	எனில் (retval)
		செல் out;

out:
	ext2fs_free_mem(&b);
	திரும்பு retval;
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

	எனில் (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		திரும்பு EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	எனில் (retval)
		திரும்பு retval;

	எனில் (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		எனில் (retval)
			திரும்பு retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	எனில் (retval)
		திரும்பு retval;

	எனில் (truncate_block >= old_truncate)
		திரும்பு 0;

	திரும்பு ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	திரும்பு ext2fs_file_set_size2(file, size);
}
