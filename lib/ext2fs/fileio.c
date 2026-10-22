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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	بنية ext2_inode	inode;
	صحيح 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	حرف 			*buf;
};

بنية block_entry {
	blk64_t		physblock;
	unsigned حرف 	sha[EXT2FS_SHA512_LENGTH];
};
تعريفنوع بنية block_entry *block_entry_t;

#define BMAP_BUFFER (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode *inode,
			    صحيح flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	אם ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		החזר EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(حجم(بنية ext2_file), &file);
	אם (retval)
		החזר retval;

	memset(file, 0, حجم(بنية ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	אם (inode) {
		memcpy(&file->inode, inode, حجم(بنية ext2_inode));
	} אחרת {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		אם (retval)
			اذهب fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	אם (retval)
		اذهب fail;

	*ret = file;
	החזר 0;

fail:
	אם (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	החזר retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   صحيح flags, ext2_file_t *ret)
{
	החזר ext2fs_file_open2(fs, ino, عدم, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	אם (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		החזר 0;
	החזר file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
بنية ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	אם (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		החזר عدم;
	החזר &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_num(ext2_file_t file)
{
	אם (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		החזר 0;
	החזר file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	صحيح		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	אם (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		החזר 0;

	/* Is this an uninit block? */
	אם (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		אם (retval)
			החזר retval;
		אם (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			אם (retval)
				החזר retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	אם (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		אם (retval)
			החזר retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	אם (retval)
		החזר retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	החזר retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
ساكن errcode_t sync_buffer_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	אם (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		אם (retval)
			החזר retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	החזר 0;
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
ساكن errcode_t load_buffer(ext2_file_t file, صحيح dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	صحيح		ret_flags;

	אם (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		אם (retval)
			החזר retval;
		אם (!dontfill) {
			אם (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				אם (retval)
					החזר retval;
			} אחרת
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	החזר 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	אם (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	החזר retval;
}


ساكن errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, فراغ *buf,
			     unsigned صحيح wanted, unsigned صحيح *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned صحيح count = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	size_t size;

	אם (file->pos >= isize)
		اذهب out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	אם (retval)
		החזר retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	אם (size > isize)
		size = isize;

	count = size - file->pos;
	אם (count > wanted)
		count = wanted;
	memcpy(buf, file->buf + file->pos, count);
	file->pos += count;
	buf = (حرف *) buf + count;

	/* zero-fill the rest of the buffer */
	wanted -= count;
	אם (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		count += wanted;
	}

out:
	אם (got)
		*got = count;
	החזר retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, فراغ *buf,
			   unsigned صحيح wanted, unsigned صحيح *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned صحيح	start, c, count = 0;
	__u64		left;
	حرف		*ptr = (حرف *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	אם (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		החזר ext2fs_file_read_inline_data(file, buf, wanted, got);

	כלעוד ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		אם (retval)
			اذهب fail;
		retval = load_buffer(file, 0);
		אם (retval)
			اذهب fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		אם (c > wanted)
			c = wanted;
		left = EXT2_I_SIZE(&file->inode) - file->pos ;
		אם (c > left)
			c = left;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		count += c;
		wanted -= c;
	}

fail:
	אם (got)
		*got = count;
	החזר retval;
}


ساكن errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, ثابت فراغ *buf,
			      unsigned صحيح nbytes, unsigned صحيح *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t size;

	אם (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	אם (retval)
		החזר retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	אם (new_isize < fs->blocksize) {
		אם (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		אם (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			اذهب expand;
		אם (retval)
			החזר retval;

		file->pos += nbytes;

		/* Update inode size */
		אם (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			אם (retval == 0)
				retval = rc;
		}

		אם (written)
			*written = nbytes;
		החזר 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	אם (retval)
		החזר retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	אם (retval)
		החזר retval;
	החזר EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ثابت فراغ *buf,
			    unsigned صحيح nbytes, unsigned صحيح *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned صحيح	start, c, count = 0;
	ثابت حرف	*ptr = (ثابت حرف *) buf;
	block_entry_t	new_block = عدم, old_block = عدم;
	صحيح		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	אם (!(file->flags & EXT2_FILE_WRITE))
		החזר EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	אם (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		אם (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			החזר retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	כלעוד (nbytes > 0) {
		retval = sync_buffer_position(file);
		אם (retval)
			اذهب fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		אם (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		אם (retval)
			اذهب fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		אם (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			אם (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, حجم(*new_block));
				אם (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					اذهب fail;
				}
				ext2fs_sha512((ثابت unsigned حرف*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							حجم(new_block->sha));
			}

			אם (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				حرر(new_block);
				new_block = عدم;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			אם (retval) {
				حرر(new_block);
				new_block = عدم;
				اذهب fail;
			}

			אם (new_block) {
				new_block->physblock = file->physblock;
				صحيح ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						حجم(new_block->sha));
				אם (ret) {
					retval = EXT2_ET_NO_MEMORY;
					حرر(new_block);
					اذهب fail;
				}
			}

			אם (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		count += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	אם (count != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		אם (retval == 0)
			retval = rc;
	}

	אם (written)
		*written = count;
	החזר retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    صحيح whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	אם (whence == EXT2_SEEK_SET)
		file->pos = offset;
	אחרת אם (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	אחרת אם (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	אחרת
		החזר EXT2_ET_INVALID_ARGUMENT;

	אם (ret_pos)
		*ret_pos = file->pos;

	החזר 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    صحيح whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	אם (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	החזר retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	אם (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		החזר EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	החזר 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	אם (ext2fs_file_get_lsize(file, &size))
		החזר 0;
	אם ((size >> 32) != 0)
		החזר 0;
	החזר size;
}

/* Zero the parts of the last block that are past EOF. */
ساكن errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	حرف *b = عدم;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	صحيح ret_flags;
	errcode_t retval;

	אם (off == 0)
		החזר 0;

	retval = sync_buffer_position(file);
	אם (retval)
		החזר retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, عدم, عدم, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	אם (retval)
		החזר retval;
	אם ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		החזר 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	אם (retval)
		החזר retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	אם (retval)
		اذهب out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	אם (retval)
		اذهب out;

out:
	ext2fs_free_mem(&b);
	החזר retval;
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

	אם (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		החזר EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	אם (retval)
		החזר retval;

	אם (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		אם (retval)
			החזר retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	אם (retval)
		החזר retval;

	אם (truncate_block >= old_truncate)
		החזר 0;

	החזר ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	החזר ext2fs_file_set_size2(file, size);
}
