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
#གལ་ཏེ HAVE_UNISTD_H
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
	ཧྲིལ 			flags;
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
			    ཧྲིལ flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	གལ་ཏེ ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		ཕྱིར EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_file), &file);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	memset(file, 0, parimaa_na(sa_nracanaa ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	གལ་ཏེ (inode) {
		memcpy(&file->inode, inode, parimaa_na(sa_nracanaa ext2_inode));
	} དེ་མིན {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		གལ་ཏེ (retval)
			goto fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	གལ་ཏེ (retval)
		goto fail;

	*ret = file;
	ཕྱིར 0;

fail:
	གལ་ཏེ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	ཕྱིར retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   ཧྲིལ flags, ext2_file_t *ret)
{
	ཕྱིར ext2fs_file_open2(fs, ino, NULL, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	གལ་ཏེ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ཕྱིར 0;
	ཕྱིར file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
sa_nracanaa ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	གལ་ཏེ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ཕྱིར NULL;
	ཕྱིར &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_num(ext2_file_t file)
{
	གལ་ཏེ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ཕྱིར 0;
	ཕྱིར file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	ཧྲིལ		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	གལ་ཏེ (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		ཕྱིར 0;

	/* Is this an uninit block? */
	གལ་ཏེ (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		གལ་ཏེ (retval)
			ཕྱིར retval;
		གལ་ཏེ (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			གལ་ཏེ (retval)
				ཕྱིར retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	གལ་ཏེ (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		གལ་ཏེ (retval)
			ཕྱིར retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	ཕྱིར retval;
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
	གལ་ཏེ (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		གལ་ཏེ (retval)
			ཕྱིར retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	ཕྱིར 0;
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
static errcode_t load_buffer(ext2_file_t file, ཧྲིལ dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	ཧྲིལ		ret_flags;

	གལ་ཏེ (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		གལ་ཏེ (retval)
			ཕྱིར retval;
		གལ་ཏེ (!dontfill) {
			གལ་ཏེ (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				གལ་ཏེ (retval)
					ཕྱིར retval;
			} དེ་མིན
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	ཕྱིར 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	གལ་ཏེ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	ཕྱིར retval;
}


static errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, void *buf,
			     unsigned ཧྲིལ wanted, unsigned ཧྲིལ *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned ཧྲིལ count = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	size_t size;

	གལ་ཏེ (file->pos >= isize)
		goto out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	གལ་ཏེ (size > isize)
		size = isize;

	count = size - file->pos;
	གལ་ཏེ (count > wanted)
		count = wanted;
	memcpy(buf, file->buf + file->pos, count);
	file->pos += count;
	buf = (var_na *) buf + count;

	/* zero-fill the rest of the buffer */
	wanted -= count;
	གལ་ཏེ (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		count += wanted;
	}

out:
	གལ་ཏེ (got)
		*got = count;
	ཕྱིར retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, void *buf,
			   unsigned ཧྲིལ wanted, unsigned ཧྲིལ *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned ཧྲིལ	start, c, count = 0;
	__u64		left;
	var_na		*ptr = (var_na *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	གལ་ཏེ (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		ཕྱིར ext2fs_file_read_inline_data(file, buf, wanted, got);

	སྐབས ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		གལ་ཏེ (retval)
			goto fail;
		retval = load_buffer(file, 0);
		གལ་ཏེ (retval)
			goto fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		གལ་ཏེ (c > wanted)
			c = wanted;
		left = EXT2_I_SIZE(&file->inode) - file->pos ;
		གལ་ཏེ (c > left)
			c = left;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		count += c;
		wanted -= c;
	}

fail:
	གལ་ཏེ (got)
		*got = count;
	ཕྱིར retval;
}


static errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, const void *buf,
			      unsigned ཧྲིལ nbytes, unsigned ཧྲིལ *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t size;

	གལ་ཏེ (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	གལ་ཏེ (new_isize < fs->blocksize) {
		གལ་ཏེ (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		གལ་ཏེ (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			goto expand;
		གལ་ཏེ (retval)
			ཕྱིར retval;

		file->pos += nbytes;

		/* Update inode size */
		གལ་ཏེ (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			གལ་ཏེ (retval == 0)
				retval = rc;
		}

		གལ་ཏེ (written)
			*written = nbytes;
		ཕྱིར 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	ཕྱིར EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, const void *buf,
			    unsigned ཧྲིལ nbytes, unsigned ཧྲིལ *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned ཧྲིལ	start, c, count = 0;
	const var_na	*ptr = (const var_na *) buf;
	block_entry_t	new_block = NULL, old_block = NULL;
	ཧྲིལ		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	གལ་ཏེ (!(file->flags & EXT2_FILE_WRITE))
		ཕྱིར EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	གལ་ཏེ (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		གལ་ཏེ (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			ཕྱིར retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	སྐབས (nbytes > 0) {
		retval = sync_buffer_position(file);
		གལ་ཏེ (retval)
			goto fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		གལ་ཏེ (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		གལ་ཏེ (retval)
			goto fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		གལ་ཏེ (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			གལ་ཏེ (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, parimaa_na(*new_block));
				གལ་ཏེ (!new_block) {
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

			གལ་ཏེ (old_block) {
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
			གལ་ཏེ (retval) {
				free(new_block);
				new_block = NULL;
				goto fail;
			}

			གལ་ཏེ (new_block) {
				new_block->physblock = file->physblock;
				ཧྲིལ ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						parimaa_na(new_block->sha));
				གལ་ཏེ (ret) {
					retval = EXT2_ET_NO_MEMORY;
					free(new_block);
					goto fail;
				}
			}

			གལ་ཏེ (bmap_flags & BMAP_SET) {
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
	གལ་ཏེ (count != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		གལ་ཏེ (retval == 0)
			retval = rc;
	}

	གལ་ཏེ (written)
		*written = count;
	ཕྱིར retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    ཧྲིལ whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	གལ་ཏེ (whence == EXT2_SEEK_SET)
		file->pos = offset;
	དེ་མིན གལ་ཏེ (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	དེ་མིན གལ་ཏེ (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	དེ་མིན
		ཕྱིར EXT2_ET_INVALID_ARGUMENT;

	གལ་ཏེ (ret_pos)
		*ret_pos = file->pos;

	ཕྱིར 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    ཧྲིལ whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	གལ་ཏེ (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	ཕྱིར retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	གལ་ཏེ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ཕྱིར EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	ཕྱིར 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	གལ་ཏེ (ext2fs_file_get_lsize(file, &size))
		ཕྱིར 0;
	གལ་ཏེ ((size >> 32) != 0)
		ཕྱིར 0;
	ཕྱིར size;
}

/* Zero the parts of the last block that are past EOF. */
static errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	var_na *b = NULL;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	ཧྲིལ ret_flags;
	errcode_t retval;

	གལ་ཏེ (off == 0)
		ཕྱིར 0;

	retval = sync_buffer_position(file);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, NULL, NULL, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	གལ་ཏེ ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		ཕྱིར 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	གལ་ཏེ (retval)
		goto out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	གལ་ཏེ (retval)
		goto out;

out:
	ext2fs_free_mem(&b);
	ཕྱིར retval;
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

	གལ་ཏེ (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		ཕྱིར EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	གལ་ཏེ (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		གལ་ཏེ (retval)
			ཕྱིར retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	གལ་ཏེ (truncate_block >= old_truncate)
		ཕྱིར 0;

	ཕྱིར ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	ཕྱིར ext2fs_file_set_size2(file, size);
}
