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
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو

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

بنية بلاڪ_داخلا {
	blk64_t		physblock;
	بنانشان حرف 	sha[EXT2_FS_SHA512_ڊيگهائي];
};
تعريفنوع بنية بلاڪ_داخلا *بلاڪ_داخلا_t;

#وصف_ڪريو BMAP_بفر (file->buf + fs->blocksize)

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
	إذا ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(حجم(بنية ext2_file), &file);
	إذا (retval)
		أرجع retval;

	memset(file, 0, حجم(بنية ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_نقاب;

	إذا (inode) {
		يادگيري_نقل(&file->inode, inode, حجم(بنية ext2_inode));
	} وإلا {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		إذا (retval)
			اذهب fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	إذا (retval)
		اذهب fail;

	*ret = file;
	أرجع 0;

fail:
	إذا (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	أرجع retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   صحيح flags, ext2_file_t *ret)
{
	أرجع ext2fs_file_open2(fs, ino, عدم, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	إذا (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		أرجع 0;
	أرجع file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
بنية ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	إذا (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		أرجع عدم;
	أرجع &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_انگ(ext2_file_t file)
{
	إذا (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		أرجع 0;
	أرجع file->ino;
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

	إذا (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		أرجع 0;

	/* Is this an uninit block? */
	إذا (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_بفر,
				      0, file->blockno, &ret_flags, &dontcare);
		إذا (retval)
			أرجع retval;
		إذا (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_بفر, BMAP_سيٽ,
					      file->blockno, 0,
					      &file->physblock);
			إذا (retval)
				أرجع retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	إذا (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_بفر, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		إذا (retval)
			أرجع retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	إذا (retval)
		أرجع retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	أرجع retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
ساكن errcode_t sync_بفر_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	إذا (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		إذا (retval)
			أرجع retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	أرجع 0;
}

/*
 * This function loads the file's block buffer with valid data from
 * the disk as necessary.
 *
 * If dontfill is true, then skip initializing the buffer since we're
 * going to be replacing its entire contents anyway.  If set, then the
 * function basically only sets file->physblock and EXT2_FILE_BUF_VALID
 */
#وصف_ڪريو DONTFILL 1
ساكن errcode_t ڀريو_بفر(ext2_file_t file, صحيح dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	صحيح		ret_flags;

	إذا (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_بفر, 0, file->blockno, &ret_flags,
				     &file->physblock);
		إذا (retval)
			أرجع retval;
		إذا (!dontfill) {
			إذا (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				إذا (retval)
					أرجع retval;
			} وإلا
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	أرجع 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	إذا (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	أرجع retval;
}


ساكن errcode_t
ext2fs_file_read_inline_معلومات(ext2_file_t file, فراغ *buf,
			     بنانشان صحيح wanted, بنانشان صحيح *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	بنانشان صحيح ڳڻپ = 0;
	uint64_t isize = EXT2_I_سائيز(&file->inode);
	size_t سائيز;

	إذا (file->pos >= isize)
		اذهب ٻاهريون;

	fs = file->fs;
	retval = ext2fs_inline_معلومات_get(fs, file->ino, &file->inode,
					file->buf, &سائيز);
	إذا (retval)
		أرجع retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	إذا (سائيز > isize)
		سائيز = isize;

	ڳڻپ = سائيز - file->pos;
	إذا (ڳڻپ > wanted)
		ڳڻپ = wanted;
	يادگيري_نقل(buf, file->buf + file->pos, ڳڻپ);
	file->pos += ڳڻپ;
	buf = (حرف *) buf + ڳڻپ;

	/* zero-fill the rest of the buffer */
	wanted -= ڳڻپ;
	إذا (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		ڳڻپ += wanted;
	}

ٻاهريون:
	إذا (got)
		*got = ڳڻپ;
	أرجع retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, فراغ *buf,
			   بنانشان صحيح wanted, بنانشان صحيح *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	بنانشان صحيح	شروعات, c, ڳڻپ = 0;
	__u64		کاٻو;
	حرف		*ptr = (حرف *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	إذا (file->inode.i_flags & EXT4_INLINE_معلومات_FL)
		أرجع ext2fs_file_read_inline_معلومات(file, buf, wanted, got);

	طالما ((file->pos < EXT2_I_سائيز(&file->inode)) && (wanted > 0)) {
		retval = sync_بفر_position(file);
		إذا (retval)
			اذهب fail;
		retval = ڀريو_بفر(file, 0);
		إذا (retval)
			اذهب fail;

		شروعات = file->pos % fs->blocksize;
		c = fs->blocksize - شروعات;
		إذا (c > wanted)
			c = wanted;
		کاٻو = EXT2_I_سائيز(&file->inode) - file->pos ;
		إذا (c > کاٻو)
			c = کاٻو;

		يادگيري_نقل(ptr, file->buf+شروعات, c);
		file->pos += c;
		ptr += c;
		ڳڻپ += c;
		wanted -= c;
	}

fail:
	إذا (got)
		*got = ڳڻپ;
	أرجع retval;
}


ساكن errcode_t
ext2fs_file_write_inline_معلومات(ext2_file_t file, ثابت فراغ *buf,
			      بنانشان صحيح nbytes, بنانشان صحيح *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_سائيز(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t سائيز;

	إذا (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_معلومات_get(fs, file->ino, &file->inode,
					file->buf, &سائيز);
	إذا (retval)
		أرجع retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	إذا (new_isize < fs->blocksize) {
		إذا (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		يادگيري_نقل(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_معلومات_سيٽ(fs, file->ino, &file->inode,
						file->buf, new_isize);
		إذا (retval == EXT2_ET_INLINE_معلومات_NO_SPACE)
			اذهب expand;
		إذا (retval)
			أرجع retval;

		file->pos += nbytes;

		/* Update inode size */
		إذا (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_سيٽ_size2(file, file->pos);
			إذا (retval == 0)
				retval = rc;
		}

		إذا (written)
			*written = nbytes;
		أرجع 0;
	}

expand:
	retval = ext2fs_inline_معلومات_expand(fs, file->ino);
	إذا (retval)
		أرجع retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	إذا (retval)
		أرجع retval;
	أرجع EXT2_ET_INLINE_معلومات_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ثابت فراغ *buf,
			    بنانشان صحيح nbytes, بنانشان صحيح *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	بنانشان صحيح	شروعات, c, ڳڻپ = 0;
	ثابت حرف	*ptr = (ثابت حرف *) buf;
	بلاڪ_داخلا_t	new_بلاڪ = عدم, old_بلاڪ = عدم;
	صحيح		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	إذا (!(file->flags & EXT2_FILE_WRITE))
		أرجع EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	إذا (file->inode.i_flags & EXT4_INLINE_معلومات_FL) {
		retval = ext2fs_file_write_inline_معلومات(file, buf, nbytes,
						       written);
		إذا (retval != EXT2_ET_INLINE_معلومات_NO_SPACE)
			أرجع retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	طالما (nbytes > 0) {
		retval = sync_بفر_position(file);
		إذا (retval)
			اذهب fail;

		شروعات = file->pos % fs->blocksize;
		c = fs->blocksize - شروعات;
		إذا (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = ڀريو_بفر(file, (c == fs->blocksize));
		إذا (retval)
			اذهب fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		يادگيري_نقل(file->buf+شروعات, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		إذا (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			إذا (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_بلاڪ = صفر_يادگيري_ڏيو(1, حجم(*new_بلاڪ));
				إذا (!new_بلاڪ) {
					retval = EXT2_ET_NO_يادگيري;
					اذهب fail;
				}
				ext2fs_sha512((ثابت بنانشان حرف*)file->buf,
						fs->blocksize, new_بلاڪ->sha);
				old_بلاڪ = ext2fs_hashmap_lookup(
							fs->بلاڪ_sha_map,
							new_بلاڪ->sha,
							حجم(new_بلاڪ->sha));
			}

			إذا (old_بلاڪ) {
				file->physblock = old_بلاڪ->physblock;
				bmap_flags |= BMAP_سيٽ;
				حرر(new_بلاڪ);
				new_بلاڪ = عدم;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_بفر,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			إذا (retval) {
				حرر(new_بلاڪ);
				new_بلاڪ = عدم;
				اذهب fail;
			}

			إذا (new_بلاڪ) {
				new_بلاڪ->physblock = file->physblock;
				صحيح ret = ext2fs_hashmap_جوڙيو(fs->بلاڪ_sha_map,
						new_بلاڪ, new_بلاڪ->sha,
						حجم(new_بلاڪ->sha));
				إذا (ret) {
					retval = EXT2_ET_NO_يادگيري;
					حرر(new_بلاڪ);
					اذهب fail;
				}
			}

			إذا (bmap_flags & BMAP_سيٽ) {
				ext2fs_iblk_جوڙيو_بلاڪون(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		ڳڻپ += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	إذا (ڳڻپ != 0 && EXT2_I_سائيز(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_سيٽ_size2(file, file->pos);
		إذا (retval == 0)
			retval = rc;
	}

	إذا (written)
		*written = ڳڻپ;
	أرجع retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 وٿي,
			    صحيح whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	إذا (whence == EXT2_SEEK_سيٽ)
		file->pos = وٿي;
	وإلا إذا (whence == EXT2_SEEK_CUR)
		file->pos += وٿي;
	وإلا إذا (whence == EXT2_SEEK_پڇاڙي)
		file->pos = EXT2_I_سائيز(&file->inode) + وٿي;
	وإلا
		أرجع EXT2_ET_INVALID_دليل;

	إذا (ret_pos)
		*ret_pos = file->pos;

	أرجع 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t وٿي,
			    صحيح whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = وٿي;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	إذا (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	أرجع retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_سائيز)
{
	إذا (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		أرجع EXT2_ET_MAGIC_EXT2_FILE;
	*ret_سائيز = EXT2_I_سائيز(&file->inode);
	أرجع 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_سائيز(ext2_file_t file)
{
	__u64	سائيز;

	إذا (ext2fs_file_get_lsize(file, &سائيز))
		أرجع 0;
	إذا ((سائيز >> 32) != 0)
		أرجع 0;
	أرجع سائيز;
}

/* Zero the parts of the last block that are past EOF. */
ساكن errcode_t ext2fs_file_zero_past_وٿي(ext2_file_t file,
					      ext2_off64_t وٿي)
{
	ext2_filsys fs = file->fs;
	حرف *b = عدم;
	ext2_off64_t off = وٿي % fs->blocksize;
	blk64_t blk;
	صحيح ret_flags;
	errcode_t retval;

	إذا (off == 0)
		أرجع 0;

	retval = sync_بفر_position(file);
	إذا (retval)
		أرجع retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, عدم, عدم, 0,
			      وٿي / fs->blocksize, &ret_flags, &blk);
	إذا (retval)
		أرجع retval;
	إذا ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		أرجع 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	إذا (retval)
		أرجع retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	إذا (retval)
		اذهب ٻاهريون;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	إذا (retval)
		اذهب ٻاهريون;

ٻاهريون:
	ext2fs_free_mem(&b);
	أرجع retval;
}

/*
 * This function sets the size of the file, truncating it if necessary
 *
 */
errcode_t ext2fs_file_سيٽ_size2(ext2_file_t file, ext2_off64_t سائيز)
{
	ext2_off64_t	old_سائيز;
	errcode_t	retval;
	blk64_t		old_truncate, truncate_بلاڪ;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	إذا (سائيز && ext2fs_file_بلاڪ_وٿي_too_big(file->fs, &file->inode,
					(سائيز - 1) / file->fs->blocksize))
		أرجع EXT2_ET_FILE_TOO_BIG;
	truncate_بلاڪ = ((سائيز + file->fs->blocksize - 1) >>
			  EXT2_بلاڪ_سائيز_بٽ(file->fs->super));
	old_سائيز = EXT2_I_سائيز(&file->inode);
	old_truncate = ((old_سائيز + file->fs->blocksize - 1) >>
		      EXT2_بلاڪ_سائيز_بٽ(file->fs->super));

	retval = ext2fs_inode_سائيز_سيٽ(file->fs, &file->inode, سائيز);
	إذا (retval)
		أرجع retval;

	إذا (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		إذا (retval)
			أرجع retval;
	}

	retval = ext2fs_file_zero_past_وٿي(file, سائيز);
	إذا (retval)
		أرجع retval;

	إذا (truncate_بلاڪ >= old_truncate)
		أرجع 0;

	أرجع ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_بلاڪ, ~0ULL);
}

errcode_t ext2fs_file_سيٽ_سائيز(ext2_file_t file, ext2_off_t سائيز)
{
	أرجع ext2fs_file_سيٽ_size2(file, سائيز);
}
