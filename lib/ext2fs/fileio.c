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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᰀᰧ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

sa_nracanaa ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	sa_nracanaa ext2_inode	inode;
	puur_naa_nka 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	var_na 			*buf;
};

sa_nracanaa block_entry {
	blk64_t		physblock;
	unsigned var_na 	sha[EXT2FS_SHA512_LENGTH];
};
typedef sa_nracanaa block_entry *block_entry_t;

#define BMAP_BUFFER (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    sa_nracanaa ext2_inode *inode,
			    puur_naa_nka flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	ᰀᰧ ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		ᰀᰧᰌ EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_file), &file);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	memset(file, 0, parimaa_na(sa_nracanaa ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	ᰀᰧ (inode) {
		memcpy(&file->inode, inode, parimaa_na(sa_nracanaa ext2_inode));
	} ᰀᰧᰀ {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		ᰀᰧ (retval)
			goto fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	ᰀᰧ (retval)
		goto fail;

	*ret = file;
	ᰀᰧᰌ 0;

fail:
	ᰀᰧ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	ᰀᰧᰌ retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   puur_naa_nka flags, ext2_file_t *ret)
{
	ᰀᰧᰌ ext2fs_file_open2(fs, ino, NULL, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	ᰀᰧ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ᰀᰧᰌ 0;
	ᰀᰧᰌ file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
sa_nracanaa ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	ᰀᰧ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ᰀᰧᰌ NULL;
	ᰀᰧᰌ &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_num(ext2_file_t file)
{
	ᰀᰧ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ᰀᰧᰌ 0;
	ᰀᰧᰌ file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	puur_naa_nka		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ᰀᰧ (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		ᰀᰧᰌ 0;

	/* Is this an uninit block? */
	ᰀᰧ (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		ᰀᰧ (retval)
			ᰀᰧᰌ retval;
		ᰀᰧ (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			ᰀᰧ (retval)
				ᰀᰧᰌ retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	ᰀᰧ (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		ᰀᰧ (retval)
			ᰀᰧᰌ retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	ᰀᰧᰌ retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
static errcode_t sync_buffer_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	ᰀᰧ (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		ᰀᰧ (retval)
			ᰀᰧᰌ retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	ᰀᰧᰌ 0;
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
static errcode_t load_buffer(ext2_file_t file, puur_naa_nka dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	puur_naa_nka		ret_flags;

	ᰀᰧ (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		ᰀᰧ (retval)
			ᰀᰧᰌ retval;
		ᰀᰧ (!dontfill) {
			ᰀᰧ (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				ᰀᰧ (retval)
					ᰀᰧᰌ retval;
			} ᰀᰧᰀ
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	ᰀᰧᰌ 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	ᰀᰧ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	ᰀᰧᰌ retval;
}


static errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, void *buf,
			     unsigned puur_naa_nka wanted, unsigned puur_naa_nka *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned puur_naa_nka count = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	size_t size;

	ᰀᰧ (file->pos >= isize)
		goto out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	ᰀᰧ (size > isize)
		size = isize;

	count = size - file->pos;
	ᰀᰧ (count > wanted)
		count = wanted;
	memcpy(buf, file->buf + file->pos, count);
	file->pos += count;
	buf = (var_na *) buf + count;

	/* zero-fill the rest of the buffer */
	wanted -= count;
	ᰀᰧ (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		count += wanted;
	}

out:
	ᰀᰧ (got)
		*got = count;
	ᰀᰧᰌ retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, void *buf,
			   unsigned puur_naa_nka wanted, unsigned puur_naa_nka *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned puur_naa_nka	start, c, count = 0;
	__u64		left;
	var_na		*ptr = (var_na *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	ᰀᰧ (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		ᰀᰧᰌ ext2fs_file_read_inline_data(file, buf, wanted, got);

	ᰀᰬ ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		ᰀᰧ (retval)
			goto fail;
		retval = load_buffer(file, 0);
		ᰀᰧ (retval)
			goto fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		ᰀᰧ (c > wanted)
			c = wanted;
		left = EXT2_I_SIZE(&file->inode) - file->pos ;
		ᰀᰧ (c > left)
			c = left;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		count += c;
		wanted -= c;
	}

fail:
	ᰀᰧ (got)
		*got = count;
	ᰀᰧᰌ retval;
}


static errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, const void *buf,
			      unsigned puur_naa_nka nbytes, unsigned puur_naa_nka *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t size;

	ᰀᰧ (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	ᰀᰧ (new_isize < fs->blocksize) {
		ᰀᰧ (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		ᰀᰧ (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			goto expand;
		ᰀᰧ (retval)
			ᰀᰧᰌ retval;

		file->pos += nbytes;

		/* Update inode size */
		ᰀᰧ (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			ᰀᰧ (retval == 0)
				retval = rc;
		}

		ᰀᰧ (written)
			*written = nbytes;
		ᰀᰧᰌ 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	ᰀᰧᰌ EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, const void *buf,
			    unsigned puur_naa_nka nbytes, unsigned puur_naa_nka *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned puur_naa_nka	start, c, count = 0;
	const var_na	*ptr = (const var_na *) buf;
	block_entry_t	new_block = NULL, old_block = NULL;
	puur_naa_nka		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ᰀᰧ (!(file->flags & EXT2_FILE_WRITE))
		ᰀᰧᰌ EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	ᰀᰧ (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		ᰀᰧ (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			ᰀᰧᰌ retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	ᰀᰬ (nbytes > 0) {
		retval = sync_buffer_position(file);
		ᰀᰧ (retval)
			goto fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		ᰀᰧ (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		ᰀᰧ (retval)
			goto fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		ᰀᰧ (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			ᰀᰧ (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, parimaa_na(*new_block));
				ᰀᰧ (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					goto fail;
				}
				ext2fs_sha512((const unsigned var_na*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							parimaa_na(new_block->sha));
			}

			ᰀᰧ (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				free(new_block);
				new_block = NULL;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			ᰀᰧ (retval) {
				free(new_block);
				new_block = NULL;
				goto fail;
			}

			ᰀᰧ (new_block) {
				new_block->physblock = file->physblock;
				puur_naa_nka ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						parimaa_na(new_block->sha));
				ᰀᰧ (ret) {
					retval = EXT2_ET_NO_MEMORY;
					free(new_block);
					goto fail;
				}
			}

			ᰀᰧ (bmap_flags & BMAP_SET) {
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
	ᰀᰧ (count != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		ᰀᰧ (retval == 0)
			retval = rc;
	}

	ᰀᰧ (written)
		*written = count;
	ᰀᰧᰌ retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    puur_naa_nka whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	ᰀᰧ (whence == EXT2_SEEK_SET)
		file->pos = offset;
	ᰀᰧᰀ ᰀᰧ (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	ᰀᰧᰀ ᰀᰧ (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	ᰀᰧᰀ
		ᰀᰧᰌ EXT2_ET_INVALID_ARGUMENT;

	ᰀᰧ (ret_pos)
		*ret_pos = file->pos;

	ᰀᰧᰌ 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    puur_naa_nka whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	ᰀᰧ (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	ᰀᰧᰌ retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	ᰀᰧ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ᰀᰧᰌ EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	ᰀᰧᰌ 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	ᰀᰧ (ext2fs_file_get_lsize(file, &size))
		ᰀᰧᰌ 0;
	ᰀᰧ ((size >> 32) != 0)
		ᰀᰧᰌ 0;
	ᰀᰧᰌ size;
}

/* Zero the parts of the last block that are past EOF. */
static errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	var_na *b = NULL;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	puur_naa_nka ret_flags;
	errcode_t retval;

	ᰀᰧ (off == 0)
		ᰀᰧᰌ 0;

	retval = sync_buffer_position(file);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, NULL, NULL, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;
	ᰀᰧ ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		ᰀᰧᰌ 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	ᰀᰧ (retval)
		goto out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	ᰀᰧ (retval)
		goto out;

out:
	ext2fs_free_mem(&b);
	ᰀᰧᰌ retval;
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

	ᰀᰧ (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		ᰀᰧᰌ EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	ᰀᰧ (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		ᰀᰧ (retval)
			ᰀᰧᰌ retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	ᰀᰧ (retval)
		ᰀᰧᰌ retval;

	ᰀᰧ (truncate_block >= old_truncate)
		ᰀᰧᰌ 0;

	ᰀᰧᰌ ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	ᰀᰧᰌ ext2fs_file_set_size2(file, size);
}
