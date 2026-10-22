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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਬਣਤਰ ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	ਬਣਤਰ ext2_inode	inode;
	ਪੂਰਨ 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	ਅੱਖਰ 			*buf;
};

ਬਣਤਰ ਖੰਡ_ਇੰਦਰਾਜ {
	blk64_t		physblock;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ 	sha[EXT2_FS_SHA512_ਲੰਮਾਈ];
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਖੰਡ_ਇੰਦਰਾਜ *ਖੰਡ_ਇੰਦਰਾਜ_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BMAP_ਬਫਰ (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    ਬਣਤਰ ext2_inode *inode,
			    ਪੂਰਨ flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	ਜੇ ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_file), &file);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	memset(file, 0, ਆਕਾਰ(ਬਣਤਰ ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_ਨਕਾਬ;

	ਜੇ (inode) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(&file->inode, inode, ਆਕਾਰ(ਬਣਤਰ ext2_inode));
	} ਨਹੀਂਤਾਂ {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		ਜੇ (retval)
			ਜਾਓ fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	ਜੇ (retval)
		ਜਾਓ fail;

	*ret = file;
	ਵਾਪਸ 0;

fail:
	ਜੇ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   ਪੂਰਨ flags, ext2_file_t *ret)
{
	ਵਾਪਸ ext2fs_file_open2(fs, ino, ਨਿਰਾ, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	ਜੇ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ਵਾਪਸ 0;
	ਵਾਪਸ file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
ਬਣਤਰ ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	ਜੇ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ਵਾਪਸ ਨਿਰਾ;
	ਵਾਪਸ &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_ਅੰਕ(ext2_file_t file)
{
	ਜੇ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ਵਾਪਸ 0;
	ਵਾਪਸ file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	ਪੂਰਨ		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ਜੇ (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		ਵਾਪਸ 0;

	/* Is this an uninit block? */
	ਜੇ (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_ਬਫਰ,
				      0, file->blockno, &ret_flags, &dontcare);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਜੇ (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_ਬਫਰ, BMAP_ਸੈੱਟ,
					      file->blockno, 0,
					      &file->physblock);
			ਜੇ (retval)
				ਵਾਪਸ retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	ਜੇ (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_ਬਫਰ, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	ਵਾਪਸ retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
ਸਥਿਰ errcode_t sync_ਬਫਰ_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	ਜੇ (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	ਵਾਪਸ 0;
}

/*
 * This function loads the file's block buffer with valid data from
 * the disk as necessary.
 *
 * If dontfill is true, then skip initializing the buffer since we're
 * going to be replacing its entire contents anyway.  If set, then the
 * function basically only sets file->physblock and EXT2_FILE_BUF_VALID
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DONTFILL 1
ਸਥਿਰ errcode_t ਲੱਦੋ_ਬਫਰ(ext2_file_t file, ਪੂਰਨ dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	ਪੂਰਨ		ret_flags;

	ਜੇ (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_ਬਫਰ, 0, file->blockno, &ret_flags,
				     &file->physblock);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਜੇ (!dontfill) {
			ਜੇ (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				ਜੇ (retval)
					ਵਾਪਸ retval;
			} ਨਹੀਂਤਾਂ
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	ਵਾਪਸ 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	ਜੇ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	ਵਾਪਸ retval;
}


ਸਥਿਰ errcode_t
ext2fs_file_read_inline_ਅੰਕੜੇ(ext2_file_t file, ਖਾਲੀ *buf,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ wanted, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
	uint64_t isize = EXT2_I_ਮਾਪ(&file->inode);
	size_t ਮਾਪ;

	ਜੇ (file->pos >= isize)
		ਜਾਓ ਬਾਹਰਲਾ;

	fs = file->fs;
	retval = ext2fs_inline_ਅੰਕੜੇ_get(fs, file->ino, &file->inode,
					file->buf, &ਮਾਪ);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	ਜੇ (ਮਾਪ > isize)
		ਮਾਪ = isize;

	ਗਿਣਤੀ_2 = ਮਾਪ - file->pos;
	ਜੇ (ਗਿਣਤੀ_2 > wanted)
		ਗਿਣਤੀ_2 = wanted;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, file->buf + file->pos, ਗਿਣਤੀ_2);
	file->pos += ਗਿਣਤੀ_2;
	buf = (ਅੱਖਰ *) buf + ਗਿਣਤੀ_2;

	/* zero-fill the rest of the buffer */
	wanted -= ਗਿਣਤੀ_2;
	ਜੇ (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		ਗਿਣਤੀ_2 += wanted;
	}

ਬਾਹਰਲਾ:
	ਜੇ (got)
		*got = ਗਿਣਤੀ_2;
	ਵਾਪਸ retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, ਖਾਲੀ *buf,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ wanted, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਸ਼ੁਰੂ, c, ਗਿਣਤੀ_2 = 0;
	__u64		ਖੱਬਾ;
	ਅੱਖਰ		*ptr = (ਅੱਖਰ *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	ਜੇ (file->inode.i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL)
		ਵਾਪਸ ext2fs_file_read_inline_ਅੰਕੜੇ(file, buf, wanted, got);

	ਜਦੋਂ ((file->pos < EXT2_I_ਮਾਪ(&file->inode)) && (wanted > 0)) {
		retval = sync_ਬਫਰ_position(file);
		ਜੇ (retval)
			ਜਾਓ fail;
		retval = ਲੱਦੋ_ਬਫਰ(file, 0);
		ਜੇ (retval)
			ਜਾਓ fail;

		ਸ਼ੁਰੂ = file->pos % fs->blocksize;
		c = fs->blocksize - ਸ਼ੁਰੂ;
		ਜੇ (c > wanted)
			c = wanted;
		ਖੱਬਾ = EXT2_I_ਮਾਪ(&file->inode) - file->pos ;
		ਜੇ (c > ਖੱਬਾ)
			c = ਖੱਬਾ;

		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptr, file->buf+ਸ਼ੁਰੂ, c);
		file->pos += c;
		ptr += c;
		ਗਿਣਤੀ_2 += c;
		wanted -= c;
	}

fail:
	ਜੇ (got)
		*got = ਗਿਣਤੀ_2;
	ਵਾਪਸ retval;
}


ਸਥਿਰ errcode_t
ext2fs_file_write_inline_ਅੰਕੜੇ(ext2_file_t file, ਅਚਲ ਖਾਲੀ *buf,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nbytes, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_ਮਾਪ(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t ਮਾਪ;

	ਜੇ (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_ਅੰਕੜੇ_get(fs, file->ino, &file->inode,
					file->buf, &ਮਾਪ);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	ਜੇ (new_isize < fs->blocksize) {
		ਜੇ (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		ਸਮ੍ਰਿਤੀ_ਨਕਲ(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_ਅੰਕੜੇ_ਸੈੱਟ(fs, file->ino, &file->inode,
						file->buf, new_isize);
		ਜੇ (retval == EXT2_ET_INLINE_ਅੰਕੜੇ_NO_SPACE)
			ਜਾਓ expand;
		ਜੇ (retval)
			ਵਾਪਸ retval;

		file->pos += nbytes;

		/* Update inode size */
		ਜੇ (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_ਸੈੱਟ_size2(file, file->pos);
			ਜੇ (retval == 0)
				retval = rc;
		}

		ਜੇ (written)
			*written = nbytes;
		ਵਾਪਸ 0;
	}

expand:
	retval = ext2fs_inline_ਅੰਕੜੇ_expand(fs, file->ino);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਵਾਪਸ EXT2_ET_INLINE_ਅੰਕੜੇ_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ਅਚਲ ਖਾਲੀ *buf,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nbytes, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਸ਼ੁਰੂ, c, ਗਿਣਤੀ_2 = 0;
	ਅਚਲ ਅੱਖਰ	*ptr = (ਅਚਲ ਅੱਖਰ *) buf;
	ਖੰਡ_ਇੰਦਰਾਜ_t	new_ਖੰਡ = ਨਿਰਾ, old_ਖੰਡ = ਨਿਰਾ;
	ਪੂਰਨ		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ਜੇ (!(file->flags & EXT2_FILE_WRITE))
		ਵਾਪਸ EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	ਜੇ (file->inode.i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL) {
		retval = ext2fs_file_write_inline_ਅੰਕੜੇ(file, buf, nbytes,
						       written);
		ਜੇ (retval != EXT2_ET_INLINE_ਅੰਕੜੇ_NO_SPACE)
			ਵਾਪਸ retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	ਜਦੋਂ (nbytes > 0) {
		retval = sync_ਬਫਰ_position(file);
		ਜੇ (retval)
			ਜਾਓ fail;

		ਸ਼ੁਰੂ = file->pos % fs->blocksize;
		c = fs->blocksize - ਸ਼ੁਰੂ;
		ਜੇ (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = ਲੱਦੋ_ਬਫਰ(file, (c == fs->blocksize));
		ਜੇ (retval)
			ਜਾਓ fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(file->buf+ਸ਼ੁਰੂ, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		ਜੇ (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			ਜੇ (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_ਖੰਡ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*new_ਖੰਡ));
				ਜੇ (!new_ਖੰਡ) {
					retval = EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
					ਜਾਓ fail;
				}
				ext2fs_sha512((ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*)file->buf,
						fs->blocksize, new_ਖੰਡ->sha);
				old_ਖੰਡ = ext2fs_hashmap_lookup(
							fs->ਖੰਡ_sha_map,
							new_ਖੰਡ->sha,
							ਆਕਾਰ(new_ਖੰਡ->sha));
			}

			ਜੇ (old_ਖੰਡ) {
				file->physblock = old_ਖੰਡ->physblock;
				bmap_flags |= BMAP_ਸੈੱਟ;
				ਮੁਕਤ(new_ਖੰਡ);
				new_ਖੰਡ = ਨਿਰਾ;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_ਬਫਰ,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			ਜੇ (retval) {
				ਮੁਕਤ(new_ਖੰਡ);
				new_ਖੰਡ = ਨਿਰਾ;
				ਜਾਓ fail;
			}

			ਜੇ (new_ਖੰਡ) {
				new_ਖੰਡ->physblock = file->physblock;
				ਪੂਰਨ ret = ext2fs_hashmap_ਜੋੜੋ(fs->ਖੰਡ_sha_map,
						new_ਖੰਡ, new_ਖੰਡ->sha,
						ਆਕਾਰ(new_ਖੰਡ->sha));
				ਜੇ (ret) {
					retval = EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
					ਮੁਕਤ(new_ਖੰਡ);
					ਜਾਓ fail;
				}
			}

			ਜੇ (bmap_flags & BMAP_ਸੈੱਟ) {
				ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		ਗਿਣਤੀ_2 += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	ਜੇ (ਗਿਣਤੀ_2 != 0 && EXT2_I_ਮਾਪ(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_ਸੈੱਟ_size2(file, file->pos);
		ਜੇ (retval == 0)
			retval = rc;
	}

	ਜੇ (written)
		*written = ਗਿਣਤੀ_2;
	ਵਾਪਸ retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 ਵਿੱਥ,
			    ਪੂਰਨ whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	ਜੇ (whence == EXT2_SEEK_ਸੈੱਟ)
		file->pos = ਵਿੱਥ;
	ਨਹੀਂਤਾਂ ਜੇ (whence == EXT2_SEEK_CUR)
		file->pos += ਵਿੱਥ;
	ਨਹੀਂਤਾਂ ਜੇ (whence == EXT2_SEEK_ਸਿਰਾ)
		file->pos = EXT2_I_ਮਾਪ(&file->inode) + ਵਿੱਥ;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (ret_pos)
		*ret_pos = file->pos;

	ਵਾਪਸ 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t ਵਿੱਥ,
			    ਪੂਰਨ whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = ਵਿੱਥ;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	ਜੇ (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	ਵਾਪਸ retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_ਮਾਪ)
{
	ਜੇ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ਵਾਪਸ EXT2_ET_MAGIC_EXT2_FILE;
	*ret_ਮਾਪ = EXT2_I_ਮਾਪ(&file->inode);
	ਵਾਪਸ 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_ਮਾਪ(ext2_file_t file)
{
	__u64	ਮਾਪ;

	ਜੇ (ext2fs_file_get_lsize(file, &ਮਾਪ))
		ਵਾਪਸ 0;
	ਜੇ ((ਮਾਪ >> 32) != 0)
		ਵਾਪਸ 0;
	ਵਾਪਸ ਮਾਪ;
}

/* Zero the parts of the last block that are past EOF. */
ਸਥਿਰ errcode_t ext2fs_file_zero_past_ਵਿੱਥ(ext2_file_t file,
					      ext2_off64_t ਵਿੱਥ)
{
	ext2_filsys fs = file->fs;
	ਅੱਖਰ *b = ਨਿਰਾ;
	ext2_off64_t off = ਵਿੱਥ % fs->blocksize;
	blk64_t blk;
	ਪੂਰਨ ret_flags;
	errcode_t retval;

	ਜੇ (off == 0)
		ਵਾਪਸ 0;

	retval = sync_ਬਫਰ_position(file);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, ਨਿਰਾ, ਨਿਰਾ, 0,
			      ਵਿੱਥ / fs->blocksize, &ret_flags, &blk);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		ਵਾਪਸ 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;

ਬਾਹਰਲਾ:
	ext2fs_free_mem(&b);
	ਵਾਪਸ retval;
}

/*
 * This function sets the size of the file, truncating it if necessary
 *
 */
errcode_t ext2fs_file_ਸੈੱਟ_size2(ext2_file_t file, ext2_off64_t ਮਾਪ)
{
	ext2_off64_t	old_ਮਾਪ;
	errcode_t	retval;
	blk64_t		old_truncate, truncate_ਖੰਡ;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	ਜੇ (ਮਾਪ && ext2fs_file_ਖੰਡ_ਵਿੱਥ_too_big(file->fs, &file->inode,
					(ਮਾਪ - 1) / file->fs->blocksize))
		ਵਾਪਸ EXT2_ET_FILE_TOO_BIG;
	truncate_ਖੰਡ = ((ਮਾਪ + file->fs->blocksize - 1) >>
			  EXT2_ਖੰਡ_ਮਾਪ_ਬਿੱਟ(file->fs->super));
	old_ਮਾਪ = EXT2_I_ਮਾਪ(&file->inode);
	old_truncate = ((old_ਮਾਪ + file->fs->blocksize - 1) >>
		      EXT2_ਖੰਡ_ਮਾਪ_ਬਿੱਟ(file->fs->super));

	retval = ext2fs_inode_ਮਾਪ_ਸੈੱਟ(file->fs, &file->inode, ਮਾਪ);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	retval = ext2fs_file_zero_past_ਵਿੱਥ(file, ਮਾਪ);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (truncate_ਖੰਡ >= old_truncate)
		ਵਾਪਸ 0;

	ਵਾਪਸ ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_ਖੰਡ, ~0ULL);
}

errcode_t ext2fs_file_ਸੈੱਟ_ਮਾਪ(ext2_file_t file, ext2_off_t ਮਾਪ)
{
	ਵਾਪਸ ext2fs_file_ਸੈੱਟ_size2(file, ਮਾਪ);
}
