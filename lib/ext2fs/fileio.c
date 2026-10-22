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
#ᱡᱩᱫᱤ HAVE_UNISTD_H
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
	ᱡᱩᱫᱤ ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_file), &file);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	memset(file, 0, parimaa_na(sa_nracanaa ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	ᱡᱩᱫᱤ (inode) {
		memcpy(&file->inode, inode, parimaa_na(sa_nracanaa ext2_inode));
	} ᱵᱟᱝ {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		ᱡᱩᱫᱤ (retval)
			goto fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	ᱡᱩᱫᱤ (retval)
		goto fail;

	*ret = file;
	ᱨᱩᱣᱟᱹᱲ 0;

fail:
	ᱡᱩᱫᱤ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	ᱨᱩᱣᱟᱹᱲ retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   puur_naa_nka flags, ext2_file_t *ret)
{
	ᱨᱩᱣᱟᱹᱲ ext2fs_file_open2(fs, ino, NULL, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	ᱡᱩᱫᱤ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ᱨᱩᱣᱟᱹᱲ 0;
	ᱨᱩᱣᱟᱹᱲ file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
sa_nracanaa ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	ᱡᱩᱫᱤ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ᱨᱩᱣᱟᱹᱲ NULL;
	ᱨᱩᱣᱟᱹᱲ &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_num(ext2_file_t file)
{
	ᱡᱩᱫᱤ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ᱨᱩᱣᱟᱹᱲ 0;
	ᱨᱩᱣᱟᱹᱲ file->ino;
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

	ᱡᱩᱫᱤ (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		ᱨᱩᱣᱟᱹᱲ 0;

	/* Is this an uninit block? */
	ᱡᱩᱫᱤ (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
		ᱡᱩᱫᱤ (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			ᱡᱩᱫᱤ (retval)
				ᱨᱩᱣᱟᱹᱲ retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	ᱡᱩᱫᱤ (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	ᱨᱩᱣᱟᱹᱲ retval;
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
	ᱡᱩᱫᱤ (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	ᱨᱩᱣᱟᱹᱲ 0;
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

	ᱡᱩᱫᱤ (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
		ᱡᱩᱫᱤ (!dontfill) {
			ᱡᱩᱫᱤ (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				ᱡᱩᱫᱤ (retval)
					ᱨᱩᱣᱟᱹᱲ retval;
			} ᱵᱟᱝ
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	ᱡᱩᱫᱤ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	ᱨᱩᱣᱟᱹᱲ retval;
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

	ᱡᱩᱫᱤ (file->pos >= isize)
		goto out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	ᱡᱩᱫᱤ (size > isize)
		size = isize;

	count = size - file->pos;
	ᱡᱩᱫᱤ (count > wanted)
		count = wanted;
	memcpy(buf, file->buf + file->pos, count);
	file->pos += count;
	buf = (var_na *) buf + count;

	/* zero-fill the rest of the buffer */
	wanted -= count;
	ᱡᱩᱫᱤ (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		count += wanted;
	}

out:
	ᱡᱩᱫᱤ (got)
		*got = count;
	ᱨᱩᱣᱟᱹᱲ retval;
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
	ᱡᱩᱫᱤ (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		ᱨᱩᱣᱟᱹᱲ ext2fs_file_read_inline_data(file, buf, wanted, got);

	ᱡᱚᱠᱷᱚᱱ ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		ᱡᱩᱫᱤ (retval)
			goto fail;
		retval = load_buffer(file, 0);
		ᱡᱩᱫᱤ (retval)
			goto fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		ᱡᱩᱫᱤ (c > wanted)
			c = wanted;
		left = EXT2_I_SIZE(&file->inode) - file->pos ;
		ᱡᱩᱫᱤ (c > left)
			c = left;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		count += c;
		wanted -= c;
	}

fail:
	ᱡᱩᱫᱤ (got)
		*got = count;
	ᱨᱩᱣᱟᱹᱲ retval;
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

	ᱡᱩᱫᱤ (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	ᱡᱩᱫᱤ (new_isize < fs->blocksize) {
		ᱡᱩᱫᱤ (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		ᱡᱩᱫᱤ (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			goto expand;
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;

		file->pos += nbytes;

		/* Update inode size */
		ᱡᱩᱫᱤ (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			ᱡᱩᱫᱤ (retval == 0)
				retval = rc;
		}

		ᱡᱩᱫᱤ (written)
			*written = nbytes;
		ᱨᱩᱣᱟᱹᱲ 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;
	ᱨᱩᱣᱟᱹᱲ EXT2_ET_INLINE_DATA_NO_SPACE;
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

	ᱡᱩᱫᱤ (!(file->flags & EXT2_FILE_WRITE))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	ᱡᱩᱫᱤ (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		ᱡᱩᱫᱤ (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			ᱨᱩᱣᱟᱹᱲ retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	ᱡᱚᱠᱷᱚᱱ (nbytes > 0) {
		retval = sync_buffer_position(file);
		ᱡᱩᱫᱤ (retval)
			goto fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		ᱡᱩᱫᱤ (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		ᱡᱩᱫᱤ (retval)
			goto fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		ᱡᱩᱫᱤ (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			ᱡᱩᱫᱤ (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, parimaa_na(*new_block));
				ᱡᱩᱫᱤ (!new_block) {
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

			ᱡᱩᱫᱤ (old_block) {
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
			ᱡᱩᱫᱤ (retval) {
				free(new_block);
				new_block = NULL;
				goto fail;
			}

			ᱡᱩᱫᱤ (new_block) {
				new_block->physblock = file->physblock;
				puur_naa_nka ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						parimaa_na(new_block->sha));
				ᱡᱩᱫᱤ (ret) {
					retval = EXT2_ET_NO_MEMORY;
					free(new_block);
					goto fail;
				}
			}

			ᱡᱩᱫᱤ (bmap_flags & BMAP_SET) {
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
	ᱡᱩᱫᱤ (count != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		ᱡᱩᱫᱤ (retval == 0)
			retval = rc;
	}

	ᱡᱩᱫᱤ (written)
		*written = count;
	ᱨᱩᱣᱟᱹᱲ retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    puur_naa_nka whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	ᱡᱩᱫᱤ (whence == EXT2_SEEK_SET)
		file->pos = offset;
	ᱵᱟᱝ ᱡᱩᱫᱤ (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	ᱵᱟᱝ ᱡᱩᱫᱤ (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	ᱵᱟᱝ
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_INVALID_ARGUMENT;

	ᱡᱩᱫᱤ (ret_pos)
		*ret_pos = file->pos;

	ᱨᱩᱣᱟᱹᱲ 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    puur_naa_nka whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	ᱡᱩᱫᱤ (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	ᱨᱩᱣᱟᱹᱲ retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	ᱡᱩᱫᱤ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	ᱨᱩᱣᱟᱹᱲ 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	ᱡᱩᱫᱤ (ext2fs_file_get_lsize(file, &size))
		ᱨᱩᱣᱟᱹᱲ 0;
	ᱡᱩᱫᱤ ((size >> 32) != 0)
		ᱨᱩᱣᱟᱹᱲ 0;
	ᱨᱩᱣᱟᱹᱲ size;
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

	ᱡᱩᱫᱤ (off == 0)
		ᱨᱩᱣᱟᱹᱲ 0;

	retval = sync_buffer_position(file);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, NULL, NULL, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;
	ᱡᱩᱫᱤ ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		ᱨᱩᱣᱟᱹᱲ 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	ᱡᱩᱫᱤ (retval)
		goto out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	ᱡᱩᱫᱤ (retval)
		goto out;

out:
	ext2fs_free_mem(&b);
	ᱨᱩᱣᱟᱹᱲ retval;
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

	ᱡᱩᱫᱤ (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	ᱡᱩᱫᱤ (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	ᱡᱩᱫᱤ (truncate_block >= old_truncate)
		ᱨᱩᱣᱟᱹᱲ 0;

	ᱨᱩᱣᱟᱹᱲ ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	ᱨᱩᱣᱟᱹᱲ ext2fs_file_set_size2(file, size);
}
