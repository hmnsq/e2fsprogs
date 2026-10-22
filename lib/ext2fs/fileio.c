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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସଂରଚନା ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	ସଂରଚନା ext2_inode	inode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	ଅକ୍ଷର 			*buf;
};

ସଂରଚନା block_entry {
	blk64_t		physblock;
	unsigned ଅକ୍ଷର 	sha[EXT2FS_SHA512_LENGTH];
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା block_entry *block_entry_t;

#define BMAP_BUFFER (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    ସଂରଚନା ext2_inode *inode,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	ଯଦି ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_file), &file);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	memset(file, 0, ଆକାର(ସଂରଚନା ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_MASK;

	ଯଦି (inode) {
		memcpy(&file->inode, inode, ଆକାର(ସଂରଚନା ext2_inode));
	} ଅନ୍ୟଥା {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		ଯଦି (retval)
			ଯାଅ fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	ଯଦି (retval)
		ଯାଅ fail;

	*ret = file;
	ଫେରନ୍ତୁ 0;

fail:
	ଯଦି (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ext2_file_t *ret)
{
	ଫେରନ୍ତୁ ext2fs_file_open2(fs, ino, ଖାଲି, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	ଯଦି (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
ସଂରଚନା ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	ଯଦି (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ଫେରନ୍ତୁ ଖାଲି;
	ଫେରନ୍ତୁ &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_ଅଙ୍କ(ext2_file_t file)
{
	ଯଦି (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ଯଦି (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		ଫେରନ୍ତୁ 0;

	/* Is this an uninit block? */
	ଯଦି (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_BUFFER,
				      0, file->blockno, &ret_flags, &dontcare);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		ଯଦି (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER, BMAP_SET,
					      file->blockno, 0,
					      &file->physblock);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	ଯଦି (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	ଫେରନ୍ତୁ retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
ସ୍ଥିର errcode_t sync_buffer_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	ଯଦି (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର errcode_t load_buffer(ext2_file_t file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ret_flags;

	ଯଦି (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_BUFFER, 0, file->blockno, &ret_flags,
				     &file->physblock);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		ଯଦି (!dontfill) {
			ଯଦି (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				ଯଦି (retval)
					ଫେରନ୍ତୁ retval;
			} ଅନ୍ୟଥା
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	ଫେରନ୍ତୁ 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	ଯଦି (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	ଫେରନ୍ତୁ retval;
}


ସ୍ଥିର errcode_t
ext2fs_file_read_inline_data(ext2_file_t file, ଶୂନ୍ୟ *buf,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା wanted, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
	uint64_t isize = EXT2_I_SIZE(&file->inode);
	size_t size;

	ଯଦି (file->pos >= isize)
		ଯାଅ out;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	ଯଦି (size > isize)
		size = isize;

	ଗଣନା_2 = size - file->pos;
	ଯଦି (ଗଣନା_2 > wanted)
		ଗଣନା_2 = wanted;
	memcpy(buf, file->buf + file->pos, ଗଣନା_2);
	file->pos += ଗଣନା_2;
	buf = (ଅକ୍ଷର *) buf + ଗଣନା_2;

	/* zero-fill the rest of the buffer */
	wanted -= ଗଣନା_2;
	ଯଦି (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		ଗଣନା_2 += wanted;
	}

out:
	ଯଦି (got)
		*got = ଗଣନା_2;
	ଫେରନ୍ତୁ retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, ଶୂନ୍ୟ *buf,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା wanted, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	start, c, ଗଣନା_2 = 0;
	__u64		ବାମ;
	ଅକ୍ଷର		*ptr = (ଅକ୍ଷର *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	ଯଦି (file->inode.i_flags & EXT4_INLINE_DATA_FL)
		ଫେରନ୍ତୁ ext2fs_file_read_inline_data(file, buf, wanted, got);

	ଯେତେବେଳେ ((file->pos < EXT2_I_SIZE(&file->inode)) && (wanted > 0)) {
		retval = sync_buffer_position(file);
		ଯଦି (retval)
			ଯାଅ fail;
		retval = load_buffer(file, 0);
		ଯଦି (retval)
			ଯାଅ fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		ଯଦି (c > wanted)
			c = wanted;
		ବାମ = EXT2_I_SIZE(&file->inode) - file->pos ;
		ଯଦି (c > ବାମ)
			c = ବାମ;

		memcpy(ptr, file->buf+start, c);
		file->pos += c;
		ptr += c;
		ଗଣନା_2 += c;
		wanted -= c;
	}

fail:
	ଯଦି (got)
		*got = ଗଣନା_2;
	ଫେରନ୍ତୁ retval;
}


ସ୍ଥିର errcode_t
ext2fs_file_write_inline_data(ext2_file_t file, ଅଚଳ ଶୂନ୍ୟ *buf,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nbytes, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_SIZE(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t size;

	ଯଦି (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_data_get(fs, file->ino, &file->inode,
					file->buf, &size);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	ଯଦି (new_isize < fs->blocksize) {
		ଯଦି (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		memcpy(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_data_set(fs, file->ino, &file->inode,
						file->buf, new_isize);
		ଯଦି (retval == EXT2_ET_INLINE_DATA_NO_SPACE)
			ଯାଅ expand;
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		file->pos += nbytes;

		/* Update inode size */
		ଯଦି (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_set_size2(file, file->pos);
			ଯଦି (retval == 0)
				retval = rc;
		}

		ଯଦି (written)
			*written = nbytes;
		ଫେରନ୍ତୁ 0;
	}

expand:
	retval = ext2fs_inline_data_expand(fs, file->ino);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଫେରନ୍ତୁ EXT2_ET_INLINE_DATA_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ଅଚଳ ଶୂନ୍ୟ *buf,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nbytes, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	start, c, ଗଣନା_2 = 0;
	ଅଚଳ ଅକ୍ଷର	*ptr = (ଅଚଳ ଅକ୍ଷର *) buf;
	block_entry_t	new_block = ଖାଲି, old_block = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ଯଦି (!(file->flags & EXT2_FILE_WRITE))
		ଫେରନ୍ତୁ EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	ଯଦି (file->inode.i_flags & EXT4_INLINE_DATA_FL) {
		retval = ext2fs_file_write_inline_data(file, buf, nbytes,
						       written);
		ଯଦି (retval != EXT2_ET_INLINE_DATA_NO_SPACE)
			ଫେରନ୍ତୁ retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	ଯେତେବେଳେ (nbytes > 0) {
		retval = sync_buffer_position(file);
		ଯଦି (retval)
			ଯାଅ fail;

		start = file->pos % fs->blocksize;
		c = fs->blocksize - start;
		ଯଦି (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = load_buffer(file, (c == fs->blocksize));
		ଯଦି (retval)
			ଯାଅ fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		memcpy(file->buf+start, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		ଯଦି (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			ଯଦି (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_block = calloc(1, ଆକାର(*new_block));
				ଯଦି (!new_block) {
					retval = EXT2_ET_NO_MEMORY;
					ଯାଅ fail;
				}
				ext2fs_sha512((ଅଚଳ unsigned ଅକ୍ଷର*)file->buf,
						fs->blocksize, new_block->sha);
				old_block = ext2fs_hashmap_lookup(
							fs->block_sha_map,
							new_block->sha,
							ଆକାର(new_block->sha));
			}

			ଯଦି (old_block) {
				file->physblock = old_block->physblock;
				bmap_flags |= BMAP_SET;
				ମୁକ୍ତ(new_block);
				new_block = ଖାଲି;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_BUFFER,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			ଯଦି (retval) {
				ମୁକ୍ତ(new_block);
				new_block = ଖାଲି;
				ଯାଅ fail;
			}

			ଯଦି (new_block) {
				new_block->physblock = file->physblock;
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = ext2fs_hashmap_add(fs->block_sha_map,
						new_block, new_block->sha,
						ଆକାର(new_block->sha));
				ଯଦି (ret) {
					retval = EXT2_ET_NO_MEMORY;
					ମୁକ୍ତ(new_block);
					ଯାଅ fail;
				}
			}

			ଯଦି (bmap_flags & BMAP_SET) {
				ext2fs_iblk_add_blocks(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		ଗଣନା_2 += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	ଯଦି (ଗଣନା_2 != 0 && EXT2_I_SIZE(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_set_size2(file, file->pos);
		ଯଦି (retval == 0)
			retval = rc;
	}

	ଯଦି (written)
		*written = ଗଣନା_2;
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 offset,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	ଯଦି (whence == EXT2_SEEK_SET)
		file->pos = offset;
	ଅନ୍ୟଥା ଯଦି (whence == EXT2_SEEK_CUR)
		file->pos += offset;
	ଅନ୍ୟଥା ଯଦି (whence == EXT2_SEEK_END)
		file->pos = EXT2_I_SIZE(&file->inode) + offset;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ଯଦି (ret_pos)
		*ret_pos = file->pos;

	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t offset,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = offset;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	ଯଦି (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	ଫେରନ୍ତୁ retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_size)
{
	ଯଦି (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ଫେରନ୍ତୁ EXT2_ET_MAGIC_EXT2_FILE;
	*ret_size = EXT2_I_SIZE(&file->inode);
	ଫେରନ୍ତୁ 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_size(ext2_file_t file)
{
	__u64	size;

	ଯଦି (ext2fs_file_get_lsize(file, &size))
		ଫେରନ୍ତୁ 0;
	ଯଦି ((size >> 32) != 0)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ size;
}

/* Zero the parts of the last block that are past EOF. */
ସ୍ଥିର errcode_t ext2fs_file_zero_past_offset(ext2_file_t file,
					      ext2_off64_t offset)
{
	ext2_filsys fs = file->fs;
	ଅକ୍ଷର *b = ଖାଲି;
	ext2_off64_t off = offset % fs->blocksize;
	blk64_t blk;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret_flags;
	errcode_t retval;

	ଯଦି (off == 0)
		ଫେରନ୍ତୁ 0;

	retval = sync_buffer_position(file);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, ଖାଲି, ଖାଲି, 0,
			      offset / fs->blocksize, &ret_flags, &blk);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		ଫେରନ୍ତୁ 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	ଯଦି (retval)
		ଯାଅ out;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	ଯଦି (retval)
		ଯାଅ out;

out:
	ext2fs_free_mem(&b);
	ଫେରନ୍ତୁ retval;
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

	ଯଦି (size && ext2fs_file_block_offset_too_big(file->fs, &file->inode,
					(size - 1) / file->fs->blocksize))
		ଫେରନ୍ତୁ EXT2_ET_FILE_TOO_BIG;
	truncate_block = ((size + file->fs->blocksize - 1) >>
			  EXT2_BLOCK_SIZE_BITS(file->fs->super));
	old_size = EXT2_I_SIZE(&file->inode);
	old_truncate = ((old_size + file->fs->blocksize - 1) >>
		      EXT2_BLOCK_SIZE_BITS(file->fs->super));

	retval = ext2fs_inode_size_set(file->fs, &file->inode, size);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}

	retval = ext2fs_file_zero_past_offset(file, size);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (truncate_block >= old_truncate)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_block, ~0ULL);
}

errcode_t ext2fs_file_set_size(ext2_file_t file, ext2_off_t size)
{
	ଫେରନ୍ତୁ ext2fs_file_set_size2(file, size);
}
