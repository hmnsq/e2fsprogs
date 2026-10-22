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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساختار ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	ساختار ext2_inode	inode;
	صحیح 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	نویسه 			*buf;
};

ساختار block_entry {
	blk64_t		physblock;
	unsigned نویسه 	sha[EXT2FS_SHA512_LENGTH];
};
تعریفنوع ساختار block_entry *block_entry_t;

#define BMAP_BUFFER (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    ساختار ext2_inode *inode,
			    صحیح flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	اگر ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(اندازه(ساختار ext2_file), &file);
	اگر (retval)
		بازگشت retval;

	memset(file, 0, اندازه(ساختار ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	اگر (inode) {
		memcpy(&file->inode, inode, اندازه(ساختار ext2_inode));
	} وگرنه {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		اگر (retval)
			برو fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	اگر (retval)
		برو fail;

	*ret = file;
	بازگشت 0;

fail:
	اگر (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	بازگشت retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   صحیح flags, ext2_file_t *ret)
{
	بازگشت ext2fs_file_open2(fs, ino, پوچ, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	اگر (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		بازگشت 0;
	بازگشت file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
ساختار ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	اگر (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		بازگشت پوچ;
	بازگشت &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_رقم(ext2_file_t file)
{
	اگر (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		بازگشت 0;
	بازگشت file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	صحیح		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	اگر (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		بازگشت 0;

	/* Is this an uninit block? */
	اگر (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		اگر (retval)
			بازگشت retval;
		اگر (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			اگر (retval)
				بازگشت retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	اگر (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		اگر (retval)
			بازگشت retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	اگر (retval)
		بازگشت retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	بازگشت retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
ایستا errcode_t sync_buffer_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	اگر (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		اگر (retval)
			بازگشت retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	بازگشت 0;
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
ایستا errcode_t load_buffer(ext2_file_t file, صحیح dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	صحیح		ret_flags;

	اگر (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		اگر (retval)
			بازگشت retval;
		اگر (!dontfill) {
			اگر (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				اگر (retval)
					بازگشت retval;
			} وگرنه
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	بازگشت 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	اگر (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	بازگشت retval;
}


ایستا errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, تهی *buf,
			     unsigned صحیح wanted, unsigned صحیح *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned صحیح شمار = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	size_t size;

	اگر (file->pos >= isize)
		برو out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	اگر (retval)
		بازگشت retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	اگر (size > isize)
		size = isize;

	شمار = size - file->pos;
	اگر (شمار > wanted)
		شمار = wanted;
	memcpy(buf, file->buf + file->pos, شمار);
	file->pos += شمار;
	buf = (نویسه *) buf + شمار;

	/* zero-fill the rest of the buffer */
	wanted -= شمار;
	اگر (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		شمار += wanted;
	}

out:
	اگر (got)
		*got = شمار;
	بازگشت retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, تهی *buf,
			   unsigned صحیح wanted, unsigned صحیح *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned صحیح	start, c, شمار = 0;
	__u64		چپ;
	نویسه		*ptr = (نویسه *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	اگر (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		بازگشت ext2fs_file_read_inline_data(file, buf, wanted, got);

	وقتی ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		اگر (retval)
			برو fail;
		retval = load_buffer(file, 0);
		اگر (retval)
			برو fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		اگر (c > wanted)
			c = wanted;
		چپ = EXT2_I_SIZE(&file->inode) - file->pos ;
		اگر (c > چپ)
			c = چپ;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		شمار += c;
		wanted -= c;
	}

fail:
	اگر (got)
		*got = شمار;
	بازگشت retval;
}


ایستا errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, ثابت تهی *buf,
			      unsigned صحیح nbytes, unsigned صحیح *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t size;

	اگر (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	اگر (retval)
		بازگشت retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	اگر (new_isize < fs->blocksize) {
		اگر (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		اگر (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			برو expand;
		اگر (retval)
			بازگشت retval;

		file->pos += nbytes;

		/* Update inode size */
		اگر (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			اگر (retval == 0)
				retval = rc;
		}

		اگر (written)
			*written = nbytes;
		بازگشت 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	اگر (retval)
		بازگشت retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	اگر (retval)
		بازگشت retval;
	بازگشت EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ثابت تهی *buf,
			    unsigned صحیح nbytes, unsigned صحیح *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned صحیح	start, c, شمار = 0;
	ثابت نویسه	*ptr = (ثابت نویسه *) buf;
	block_entry_t	new_block = پوچ, old_block = پوچ;
	صحیح		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	اگر (!(file->flags & EXT2_FILE_WRITE))
		بازگشت EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	اگر (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		اگر (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			بازگشت retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	وقتی (nbytes > 0) {
		retval = sync_buffer_position(file);
		اگر (retval)
			برو fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		اگر (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		اگر (retval)
			برو fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		اگر (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			اگر (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, اندازه(*new_block));
				اگر (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					برو fail;
				}
				ext2fs_sha512((ثابت unsigned نویسه*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							اندازه(new_block->sha));
			}

			اگر (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				آزاد(new_block);
				new_block = پوچ;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			اگر (retval) {
				آزاد(new_block);
				new_block = پوچ;
				برو fail;
			}

			اگر (new_block) {
				new_block->physblock = file->physblock;
				صحیح ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						اندازه(new_block->sha));
				اگر (ret) {
					retval = EXT2_ET_NO_MEMORY;
					آزاد(new_block);
					برو fail;
				}
			}

			اگر (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		شمار += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	اگر (شمار != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		اگر (retval == 0)
			retval = rc;
	}

	اگر (written)
		*written = شمار;
	بازگشت retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    صحیح whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	اگر (whence == EXT2_SEEK_SET)
		file->pos = offset;
	وگرنه اگر (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	وگرنه اگر (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	وگرنه
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	اگر (ret_pos)
		*ret_pos = file->pos;

	بازگشت 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    صحیح whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	اگر (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	بازگشت retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	اگر (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		بازگشت EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	بازگشت 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	اگر (ext2fs_file_get_lsize(file, &size))
		بازگشت 0;
	اگر ((size >> 32) != 0)
		بازگشت 0;
	بازگشت size;
}

/* Zero the parts of the last block that are past EOF. */
ایستا errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	نویسه *b = پوچ;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	صحیح ret_flags;
	errcode_t retval;

	اگر (off == 0)
		بازگشت 0;

	retval = sync_buffer_position(file);
	اگر (retval)
		بازگشت retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, پوچ, پوچ, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	اگر (retval)
		بازگشت retval;
	اگر ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		بازگشت 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	اگر (retval)
		بازگشت retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	اگر (retval)
		برو out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	اگر (retval)
		برو out;

out:
	ext2fs_free_mem(&b);
	بازگشت retval;
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

	اگر (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		بازگشت EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	اگر (retval)
		بازگشت retval;

	اگر (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		اگر (retval)
			بازگشت retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	اگر (retval)
		بازگشت retval;

	اگر (truncate_block >= old_truncate)
		بازگشت 0;

	بازگشت ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	بازگشت ext2fs_file_set_size2(file, size);
}
