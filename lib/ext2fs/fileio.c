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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સંરચના ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	સંરચના ext2_inode	inode;
	પૂર્ણાંક 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	અક્ષર 			*buf;
};

સંરચના block_entry {
	blk64_t		physblock;
	unsigned અક્ષર 	sha[EXT2FS_SHA512_LENGTH];
};
પ્રકારવ્યાખ્યા સંરચના block_entry *block_entry_t;

#define BMAP_BUFFER (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    સંરચના ext2_inode *inode,
			    પૂર્ણાંક flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	જો ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(કદ(સંરચના ext2_file), &file);
	જો (retval)
		પાછા retval;

	memset(file, 0, કદ(સંરચના ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	જો (inode) {
		memcpy(&file->inode, inode, કદ(સંરચના ext2_inode));
	} નહીંતર {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		જો (retval)
			જાઓ fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	જો (retval)
		જાઓ fail;

	*ret = file;
	પાછા 0;

fail:
	જો (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	પાછા retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   પૂર્ણાંક flags, ext2_file_t *ret)
{
	પાછા ext2fs_file_open2(fs, ino, ખાલી, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	જો (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		પાછા 0;
	પાછા file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
સંરચના ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	જો (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		પાછા ખાલી;
	પાછા &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_અંક(ext2_file_t file)
{
	જો (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		પાછા 0;
	પાછા file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	પૂર્ણાંક		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	જો (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		પાછા 0;

	/* Is this an uninit block? */
	જો (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		જો (retval)
			પાછા retval;
		જો (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			જો (retval)
				પાછા retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	જો (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		જો (retval)
			પાછા retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	જો (retval)
		પાછા retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	પાછા retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
સ્થિર errcode_t sync_buffer_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	જો (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		જો (retval)
			પાછા retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	પાછા 0;
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
સ્થિર errcode_t load_buffer(ext2_file_t file, પૂર્ણાંક dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	પૂર્ણાંક		ret_flags;

	જો (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		જો (retval)
			પાછા retval;
		જો (!dontfill) {
			જો (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				જો (retval)
					પાછા retval;
			} નહીંતર
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	પાછા 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	જો (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	પાછા retval;
}


સ્થિર errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, શૂન્ય *buf,
			     unsigned પૂર્ણાંક wanted, unsigned પૂર્ણાંક *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned પૂર્ણાંક ગણતરી = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	size_t size;

	જો (file->pos >= isize)
		જાઓ out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	જો (retval)
		પાછા retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	જો (size > isize)
		size = isize;

	ગણતરી = size - file->pos;
	જો (ગણતરી > wanted)
		ગણતરી = wanted;
	memcpy(buf, file->buf + file->pos, ગણતરી);
	file->pos += ગણતરી;
	buf = (અક્ષર *) buf + ગણતરી;

	/* zero-fill the rest of the buffer */
	wanted -= ગણતરી;
	જો (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		ગણતરી += wanted;
	}

out:
	જો (got)
		*got = ગણતરી;
	પાછા retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, શૂન્ય *buf,
			   unsigned પૂર્ણાંક wanted, unsigned પૂર્ણાંક *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned પૂર્ણાંક	start, c, ગણતરી = 0;
	__u64		ડાબું;
	અક્ષર		*ptr = (અક્ષર *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	જો (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		પાછા ext2fs_file_read_inline_data(file, buf, wanted, got);

	જ્યારે ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		જો (retval)
			જાઓ fail;
		retval = load_buffer(file, 0);
		જો (retval)
			જાઓ fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		જો (c > wanted)
			c = wanted;
		ડાબું = EXT2_I_SIZE(&file->inode) - file->pos ;
		જો (c > ડાબું)
			c = ડાબું;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		ગણતરી += c;
		wanted -= c;
	}

fail:
	જો (got)
		*got = ગણતરી;
	પાછા retval;
}


સ્થિર errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, અચલ શૂન્ય *buf,
			      unsigned પૂર્ણાંક nbytes, unsigned પૂર્ણાંક *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t size;

	જો (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	જો (retval)
		પાછા retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	જો (new_isize < fs->blocksize) {
		જો (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		જો (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			જાઓ expand;
		જો (retval)
			પાછા retval;

		file->pos += nbytes;

		/* Update inode size */
		જો (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			જો (retval == 0)
				retval = rc;
		}

		જો (written)
			*written = nbytes;
		પાછા 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	જો (retval)
		પાછા retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	જો (retval)
		પાછા retval;
	પાછા EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, અચલ શૂન્ય *buf,
			    unsigned પૂર્ણાંક nbytes, unsigned પૂર્ણાંક *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned પૂર્ણાંક	start, c, ગણતરી = 0;
	અચલ અક્ષર	*ptr = (અચલ અક્ષર *) buf;
	block_entry_t	new_block = ખાલી, old_block = ખાલી;
	પૂર્ણાંક		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	જો (!(file->flags & EXT2_FILE_WRITE))
		પાછા EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	જો (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		જો (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			પાછા retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	જ્યારે (nbytes > 0) {
		retval = sync_buffer_position(file);
		જો (retval)
			જાઓ fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		જો (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		જો (retval)
			જાઓ fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		જો (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			જો (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, કદ(*new_block));
				જો (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					જાઓ fail;
				}
				ext2fs_sha512((અચલ unsigned અક્ષર*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							કદ(new_block->sha));
			}

			જો (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				મુક્ત(new_block);
				new_block = ખાલી;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			જો (retval) {
				મુક્ત(new_block);
				new_block = ખાલી;
				જાઓ fail;
			}

			જો (new_block) {
				new_block->physblock = file->physblock;
				પૂર્ણાંક ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						કદ(new_block->sha));
				જો (ret) {
					retval = EXT2_ET_NO_MEMORY;
					મુક્ત(new_block);
					જાઓ fail;
				}
			}

			જો (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		ગણતરી += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	જો (ગણતરી != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		જો (retval == 0)
			retval = rc;
	}

	જો (written)
		*written = ગણતરી;
	પાછા retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    પૂર્ણાંક whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	જો (whence == EXT2_SEEK_SET)
		file->pos = offset;
	નહીંતર જો (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	નહીંતર જો (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	નહીંતર
		પાછા EXT2_ET_INVALID_ARGUMENT;

	જો (ret_pos)
		*ret_pos = file->pos;

	પાછા 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    પૂર્ણાંક whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	જો (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	પાછા retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	જો (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		પાછા EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	પાછા 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	જો (ext2fs_file_get_lsize(file, &size))
		પાછા 0;
	જો ((size >> 32) != 0)
		પાછા 0;
	પાછા size;
}

/* Zero the parts of the last block that are past EOF. */
સ્થિર errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	અક્ષર *b = ખાલી;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	પૂર્ણાંક ret_flags;
	errcode_t retval;

	જો (off == 0)
		પાછા 0;

	retval = sync_buffer_position(file);
	જો (retval)
		પાછા retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, ખાલી, ખાલી, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	જો (retval)
		પાછા retval;
	જો ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		પાછા 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	જો (retval)
		પાછા retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	જો (retval)
		જાઓ out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	જો (retval)
		જાઓ out;

out:
	ext2fs_free_mem(&b);
	પાછા retval;
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

	જો (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		પાછા EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	જો (retval)
		પાછા retval;

	જો (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		જો (retval)
			પાછા retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	જો (retval)
		પાછા retval;

	જો (truncate_block >= old_truncate)
		પાછા 0;

	પાછા ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	પાછા ext2fs_file_set_size2(file, size);
}
