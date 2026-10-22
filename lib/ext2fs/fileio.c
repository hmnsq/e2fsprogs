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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

ഘടന ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	ഘടന ext2_inode	inode;
	പൂർണ്ണസംഖ്യ 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	അക്ഷരം 			*buf;
};

ഘടന block_entry {
	blk64_t		physblock;
	unsigned അക്ഷരം 	sha[EXT2FS_SHA512_LENGTH];
};
തരനിർവചനം ഘടന block_entry *block_entry_t;

#define BMAP_BUFFER (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    ഘടന ext2_inode *inode,
			    പൂർണ്ണസംഖ്യ flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	എങ്കിൽ ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_file), &file);
	എങ്കിൽ (retval)
		തിരികെ retval;

	memset(file, 0, വലുപ്പം(ഘടന ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	എങ്കിൽ (inode) {
		memcpy(&file->inode, inode, വലുപ്പം(ഘടന ext2_inode));
	} അല്ലെങ്കിൽ {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		എങ്കിൽ (retval)
			പോകുക fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	എങ്കിൽ (retval)
		പോകുക fail;

	*ret = file;
	തിരികെ 0;

fail:
	എങ്കിൽ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	തിരികെ retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   പൂർണ്ണസംഖ്യ flags, ext2_file_t *ret)
{
	തിരികെ ext2fs_file_open2(fs, ino, ശൂന്യ, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	എങ്കിൽ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		തിരികെ 0;
	തിരികെ file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
ഘടന ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	എങ്കിൽ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		തിരികെ ശൂന്യ;
	തിരികെ &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_അക്കം(ext2_file_t file)
{
	എങ്കിൽ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		തിരികെ 0;
	തിരികെ file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	പൂർണ്ണസംഖ്യ		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	എങ്കിൽ (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		തിരികെ 0;

	/* Is this an uninit block? */
	എങ്കിൽ (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		എങ്കിൽ (retval)
			തിരികെ retval;
		എങ്കിൽ (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			എങ്കിൽ (retval)
				തിരികെ retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	എങ്കിൽ (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	എങ്കിൽ (retval)
		തിരികെ retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	തിരികെ retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
സ്ഥിതം errcode_t sync_buffer_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	എങ്കിൽ (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		എങ്കിൽ (retval)
			തിരികെ retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	തിരികെ 0;
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
സ്ഥിതം errcode_t load_buffer(ext2_file_t file, പൂർണ്ണസംഖ്യ dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	പൂർണ്ണസംഖ്യ		ret_flags;

	എങ്കിൽ (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		എങ്കിൽ (retval)
			തിരികെ retval;
		എങ്കിൽ (!dontfill) {
			എങ്കിൽ (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				എങ്കിൽ (retval)
					തിരികെ retval;
			} അല്ലെങ്കിൽ
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	തിരികെ 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	എങ്കിൽ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	തിരികെ retval;
}


സ്ഥിതം errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, ശൂന്യം *buf,
			     unsigned പൂർണ്ണസംഖ്യ wanted, unsigned പൂർണ്ണസംഖ്യ *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned പൂർണ്ണസംഖ്യ എണ്ണം = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	size_t size;

	എങ്കിൽ (file->pos >= isize)
		പോകുക out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	എങ്കിൽ (retval)
		തിരികെ retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	എങ്കിൽ (size > isize)
		size = isize;

	എണ്ണം = size - file->pos;
	എങ്കിൽ (എണ്ണം > wanted)
		എണ്ണം = wanted;
	memcpy(buf, file->buf + file->pos, എണ്ണം);
	file->pos += എണ്ണം;
	buf = (അക്ഷരം *) buf + എണ്ണം;

	/* zero-fill the rest of the buffer */
	wanted -= എണ്ണം;
	എങ്കിൽ (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		എണ്ണം += wanted;
	}

out:
	എങ്കിൽ (got)
		*got = എണ്ണം;
	തിരികെ retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, ശൂന്യം *buf,
			   unsigned പൂർണ്ണസംഖ്യ wanted, unsigned പൂർണ്ണസംഖ്യ *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned പൂർണ്ണസംഖ്യ	start, c, എണ്ണം = 0;
	__u64		ഇടത്;
	അക്ഷരം		*ptr = (അക്ഷരം *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	എങ്കിൽ (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		തിരികെ ext2fs_file_read_inline_data(file, buf, wanted, got);

	ആകുമ്പോൾ ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		എങ്കിൽ (retval)
			പോകുക fail;
		retval = load_buffer(file, 0);
		എങ്കിൽ (retval)
			പോകുക fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		എങ്കിൽ (c > wanted)
			c = wanted;
		ഇടത് = EXT2_I_SIZE(&file->inode) - file->pos ;
		എങ്കിൽ (c > ഇടത്)
			c = ഇടത്;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		എണ്ണം += c;
		wanted -= c;
	}

fail:
	എങ്കിൽ (got)
		*got = എണ്ണം;
	തിരികെ retval;
}


സ്ഥിതം errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, സ്ഥിരം ശൂന്യം *buf,
			      unsigned പൂർണ്ണസംഖ്യ nbytes, unsigned പൂർണ്ണസംഖ്യ *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t size;

	എങ്കിൽ (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	എങ്കിൽ (retval)
		തിരികെ retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	എങ്കിൽ (new_isize < fs->blocksize) {
		എങ്കിൽ (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		എങ്കിൽ (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			പോകുക expand;
		എങ്കിൽ (retval)
			തിരികെ retval;

		file->pos += nbytes;

		/* Update inode size */
		എങ്കിൽ (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			എങ്കിൽ (retval == 0)
				retval = rc;
		}

		എങ്കിൽ (written)
			*written = nbytes;
		തിരികെ 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	എങ്കിൽ (retval)
		തിരികെ retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	എങ്കിൽ (retval)
		തിരികെ retval;
	തിരികെ EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, സ്ഥിരം ശൂന്യം *buf,
			    unsigned പൂർണ്ണസംഖ്യ nbytes, unsigned പൂർണ്ണസംഖ്യ *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned പൂർണ്ണസംഖ്യ	start, c, എണ്ണം = 0;
	സ്ഥിരം അക്ഷരം	*ptr = (സ്ഥിരം അക്ഷരം *) buf;
	block_entry_t	new_block = ശൂന്യ, old_block = ശൂന്യ;
	പൂർണ്ണസംഖ്യ		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	എങ്കിൽ (!(file->flags & EXT2_FILE_WRITE))
		തിരികെ EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	എങ്കിൽ (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		എങ്കിൽ (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			തിരികെ retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	ആകുമ്പോൾ (nbytes > 0) {
		retval = sync_buffer_position(file);
		എങ്കിൽ (retval)
			പോകുക fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		എങ്കിൽ (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		എങ്കിൽ (retval)
			പോകുക fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		എങ്കിൽ (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			എങ്കിൽ (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, വലുപ്പം(*new_block));
				എങ്കിൽ (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					പോകുക fail;
				}
				ext2fs_sha512((സ്ഥിരം unsigned അക്ഷരം*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							വലുപ്പം(new_block->sha));
			}

			എങ്കിൽ (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				സ്വതന്ത്രം(new_block);
				new_block = ശൂന്യ;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			എങ്കിൽ (retval) {
				സ്വതന്ത്രം(new_block);
				new_block = ശൂന്യ;
				പോകുക fail;
			}

			എങ്കിൽ (new_block) {
				new_block->physblock = file->physblock;
				പൂർണ്ണസംഖ്യ ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						വലുപ്പം(new_block->sha));
				എങ്കിൽ (ret) {
					retval = EXT2_ET_NO_MEMORY;
					സ്വതന്ത്രം(new_block);
					പോകുക fail;
				}
			}

			എങ്കിൽ (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		എണ്ണം += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	എങ്കിൽ (എണ്ണം != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		എങ്കിൽ (retval == 0)
			retval = rc;
	}

	എങ്കിൽ (written)
		*written = എണ്ണം;
	തിരികെ retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    പൂർണ്ണസംഖ്യ whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	എങ്കിൽ (whence == EXT2_SEEK_SET)
		file->pos = offset;
	അല്ലെങ്കിൽ എങ്കിൽ (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	അല്ലെങ്കിൽ എങ്കിൽ (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	അല്ലെങ്കിൽ
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	എങ്കിൽ (ret_pos)
		*ret_pos = file->pos;

	തിരികെ 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    പൂർണ്ണസംഖ്യ whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	എങ്കിൽ (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	തിരികെ retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	എങ്കിൽ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		തിരികെ EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	തിരികെ 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	എങ്കിൽ (ext2fs_file_get_lsize(file, &size))
		തിരികെ 0;
	എങ്കിൽ ((size >> 32) != 0)
		തിരികെ 0;
	തിരികെ size;
}

/* Zero the parts of the last block that are past EOF. */
സ്ഥിതം errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	അക്ഷരം *b = ശൂന്യ;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	പൂർണ്ണസംഖ്യ ret_flags;
	errcode_t retval;

	എങ്കിൽ (off == 0)
		തിരികെ 0;

	retval = sync_buffer_position(file);
	എങ്കിൽ (retval)
		തിരികെ retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, ശൂന്യ, ശൂന്യ, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		തിരികെ 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	എങ്കിൽ (retval)
		തിരികെ retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	എങ്കിൽ (retval)
		പോകുക out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	എങ്കിൽ (retval)
		പോകുക out;

out:
	ext2fs_free_mem(&b);
	തിരികെ retval;
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

	എങ്കിൽ (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		തിരികെ EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (truncate_block >= old_truncate)
		തിരികെ 0;

	തിരികെ ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	തിരികെ ext2fs_file_set_size2(file, size);
}
