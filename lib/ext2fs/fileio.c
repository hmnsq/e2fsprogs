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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

কাঠামো ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	কাঠামো ext2_inode	inode;
	পূর্ণসংখ্যা 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	অক্ষর 			*buf;
};

কাঠামো block_entry {
	blk64_t		physblock;
	unsigned অক্ষর 	sha[EXT2_FS_SHA512_দৈর্ঘ্য];
};
প্রকারলেখ কাঠামো block_entry *block_entry_t;

#define BMAP_বাফার (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    কাঠামো ext2_inode *inode,
			    পূর্ণসংখ্যা flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	যদি ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		ফেরত EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(আকার(কাঠামো ext2_file), &file);
	যদি (retval)
		ফেরত retval;

	memset(file, 0, আকার(কাঠামো ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	যদি (inode) {
		memcpy(&file->inode, inode, আকার(কাঠামো ext2_inode));
	} অন্যথা {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		যদি (retval)
			যাও fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	যদি (retval)
		যাও fail;

	*ret = file;
	ফেরত 0;

fail:
	যদি (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	ফেরত retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   পূর্ণসংখ্যা flags, ext2_file_t *ret)
{
	ফেরত ext2fs_file_open2(fs, ino, ফাঁকা, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	যদি (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ফেরত 0;
	ফেরত file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
কাঠামো ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	যদি (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ফেরত ফাঁকা;
	ফেরত &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_অঙ্ক(ext2_file_t file)
{
	যদি (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ফেরত 0;
	ফেরত file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	পূর্ণসংখ্যা		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	যদি (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		ফেরত 0;

	/* Is this an uninit block? */
	যদি (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_বাফার,
				      0, file->blockno, &ret_flags, &dontcare);
		যদি (retval)
			ফেরত retval;
		যদি (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_বাফার, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			যদি (retval)
				ফেরত retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	যদি (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_বাফার, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		যদি (retval)
			ফেরত retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	যদি (retval)
		ফেরত retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	ফেরত retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
স্থির errcode_t sync_বাফার_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	যদি (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		যদি (retval)
			ফেরত retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	ফেরত 0;
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
স্থির errcode_t load_বাফার(ext2_file_t file, পূর্ণসংখ্যা dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	পূর্ণসংখ্যা		ret_flags;

	যদি (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_বাফার, 0, file->blockno, &ret_flags,
				     &file->physblock);
		যদি (retval)
			ফেরত retval;
		যদি (!dontfill) {
			যদি (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				যদি (retval)
					ফেরত retval;
			} অন্যথা
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	ফেরত 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	যদি (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	ফেরত retval;
}


স্থির errcode_t
ext2fs_file_read_inline_তথ্য(ext2_file_t file, শূন্য *buf,
			     unsigned পূর্ণসংখ্যা wanted, unsigned পূর্ণসংখ্যা *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned পূর্ণসংখ্যা গুনতি = 0;
	uint64_t isize = EXT2_I_আকার(&file->inode);
	size_t আকার_2;

	যদি (file->pos >= isize)
		যাও out;

	fs = file->fs;
	retval = ext2fs_inline_তথ্য_get(fs, file->ino, &file->inode,
					file->buf, &আকার_2);
	যদি (retval)
		ফেরত retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	যদি (আকার_2 > isize)
		আকার_2 = isize;

	গুনতি = আকার_2 - file->pos;
	যদি (গুনতি > wanted)
		গুনতি = wanted;
	memcpy(buf, file->buf + file->pos, গুনতি);
	file->pos += গুনতি;
	buf = (অক্ষর *) buf + গুনতি;

	/* zero-fill the rest of the buffer */
	wanted -= গুনতি;
	যদি (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		গুনতি += wanted;
	}

out:
	যদি (got)
		*got = গুনতি;
	ফেরত retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, শূন্য *buf,
			   unsigned পূর্ণসংখ্যা wanted, unsigned পূর্ণসংখ্যা *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned পূর্ণসংখ্যা	start, c, গুনতি = 0;
	__u64		বাম;
	অক্ষর		*ptr = (অক্ষর *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	যদি (file->inode.i_flags & EXT4_INLINE_তথ্য_FL)
		ফেরত ext2fs_file_read_inline_তথ্য(file, buf, wanted, got);

	যখন ((file->pos < EXT2_I_আকার(&file->inode)) && (wanted > 0)) {
		retval = sync_বাফার_position(file);
		যদি (retval)
			যাও fail;
		retval = load_বাফার(file, 0);
		যদি (retval)
			যাও fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		যদি (c > wanted)
			c = wanted;
		বাম = EXT2_I_আকার(&file->inode) - file->pos ;
		যদি (c > বাম)
			c = বাম;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		গুনতি += c;
		wanted -= c;
	}

fail:
	যদি (got)
		*got = গুনতি;
	ফেরত retval;
}


স্থির errcode_t
ext2fs_file_write_inline_তথ্য(ext2_file_t file, ধ্রুবক শূন্য *buf,
			      unsigned পূর্ণসংখ্যা nbytes, unsigned পূর্ণসংখ্যা *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_আকার(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t আকার_2;

	যদি (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_তথ্য_get(fs, file->ino, &file->inode,
					file->buf, &আকার_2);
	যদি (retval)
		ফেরত retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	যদি (new_isize < fs->blocksize) {
		যদি (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_তথ্য_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		যদি (retval == EXT2_ET_INLINE_তথ্য_NO_SPACE)
			যাও expand;
		যদি (retval)
			ফেরত retval;

		file->pos += nbytes;

		/* Update inode size */
		যদি (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			যদি (retval == 0)
				retval = rc;
		}

		যদি (written)
			*written = nbytes;
		ফেরত 0;
	}

expand:
	retval = ext2fs_inline_তথ্য_expand(fs, file->ino);
	যদি (retval)
		ফেরত retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	যদি (retval)
		ফেরত retval;
	ফেরত EXT2_ET_INLINE_তথ্য_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ধ্রুবক শূন্য *buf,
			    unsigned পূর্ণসংখ্যা nbytes, unsigned পূর্ণসংখ্যা *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned পূর্ণসংখ্যা	start, c, গুনতি = 0;
	ধ্রুবক অক্ষর	*ptr = (ধ্রুবক অক্ষর *) buf;
	block_entry_t	new_block = ফাঁকা, old_block = ফাঁকা;
	পূর্ণসংখ্যা		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	যদি (!(file->flags & EXT2_FILE_WRITE))
		ফেরত EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	যদি (file->inode.i_flags & EXT4_INLINE_তথ্য_FL) {
		retval = ext2fs_file_write_inline_তথ্য(file, buf, nbytes,
						       written);
		যদি (retval != EXT2_ET_INLINE_তথ্য_NO_SPACE)
			ফেরত retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	যখন (nbytes > 0) {
		retval = sync_বাফার_position(file);
		যদি (retval)
			যাও fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		যদি (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_বাফার(file, (c == fs->blocksize));
		যদি (retval)
			যাও fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		যদি (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			যদি (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, আকার(*new_block));
				যদি (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					যাও fail;
				}
				ext2fs_sha512((ধ্রুবক unsigned অক্ষর*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							আকার(new_block->sha));
			}

			যদি (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				মুক্ত(new_block);
				new_block = ফাঁকা;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_বাফার,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			যদি (retval) {
				মুক্ত(new_block);
				new_block = ফাঁকা;
				যাও fail;
			}

			যদি (new_block) {
				new_block->physblock = file->physblock;
				পূর্ণসংখ্যা ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						আকার(new_block->sha));
				যদি (ret) {
					retval = EXT2_ET_NO_MEMORY;
					মুক্ত(new_block);
					যাও fail;
				}
			}

			যদি (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		গুনতি += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	যদি (গুনতি != 0 && EXT2_I_আকার(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		যদি (retval == 0)
			retval = rc;
	}

	যদি (written)
		*written = গুনতি;
	ফেরত retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    পূর্ণসংখ্যা whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	যদি (whence == EXT2_SEEK_SET)
		file->pos = offset;
	অন্যথা যদি (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	অন্যথা যদি (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_আকার(&file->inode) + offset;
	অন্যথা
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	যদি (ret_pos)
		*ret_pos = file->pos;

	ফেরত 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    পূর্ণসংখ্যা whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	যদি (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	ফেরত retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_আকার)
{
	যদি (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ফেরত EXT2_ET_MAGIC_EXT2_FILE;
	*ret_আকার = EXT2_I_আকার(&file->inode);
	ফেরত 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_আকার(ext2_file_t file)
{
	__u64	আকার_2;

	যদি (ext2fs_file_get_lsize(file, &আকার_2))
		ফেরত 0;
	যদি ((আকার_2 >> 32) != 0)
		ফেরত 0;
	ফেরত আকার_2;
}

/* Zero the parts of the last block that are past EOF. */
স্থির errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	অক্ষর *b = ফাঁকা;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	পূর্ণসংখ্যা ret_flags;
	errcode_t retval;

	যদি (off == 0)
		ফেরত 0;

	retval = sync_বাফার_position(file);
	যদি (retval)
		ফেরত retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, ফাঁকা, ফাঁকা, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	যদি (retval)
		ফেরত retval;
	যদি ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		ফেরত 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	যদি (retval)
		ফেরত retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	যদি (retval)
		যাও out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	যদি (retval)
		যাও out;

out:
	ext2fs_free_mem(&b);
	ফেরত retval;
}

/*
 * This function sets the size of the file, truncating it if necessary
 *
 */
errcode_t ext2fs_file_set_size2(ext2_file_t file, ext2_off64_t আকার_2)
{
	ext2_off64_t	old_আকার;
	errcode_t	retval;
	blk64_t		old_truncate, truncate_block;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	যদি (আকার_2 && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(আকার_2 - 1) / file->fs->blocksize))
		ফেরত EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((আকার_2 + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_আকার_BITS(file->fs->super));
	old_আকার = EXT2_I_আকার(&file->inode);
	old_truncate = ((old_আকার + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_আকার_BITS(file->fs->super));

	retval = ext2fs_inode_আকার_set(file->fs, &file->inode, আকার_2);
	যদি (retval)
		ফেরত retval;

	যদি (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		যদি (retval)
			ফেরত retval;
	}

	retval = ext2fs_file_zero_past_offset(file, আকার_2);
	যদি (retval)
		ফেরত retval;

	যদি (truncate_block >= old_truncate)
		ফেরত 0;

	ফেরত ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_আকার(ext2_file_t file, ext2_off_t আকার_2)
{
	ফেরত ext2fs_file_set_size2(file, আকার_2);
}
