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
#جے_انت

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

بنية بلاک_اندراج {
	blk64_t		physblock;
	بنانشانی حرف 	sha[EXT2_FS_SHA512_لمائی];
};
تعريفنوع بنية بلاک_اندراج *بلاک_اندراج_t;

#تعریف_کرو BMAP_بفر (file->buf + fs->blocksize)

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
		حافظہ_نقل(&file->inode, inode, حجم(بنية ext2_inode));
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
ext2_ino_t ext2fs_file_get_inode_عدد(ext2_file_t file)
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
					      BMAP_بفر, BMAP_سیٹ,
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
#تعریف_کرو DONTFILL 1
ساكن errcode_t لدو_بفر(ext2_file_t file, صحيح dontfill)
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
ext2fs_file_read_inline_انکڑے(ext2_file_t file, فراغ *buf,
			     بنانشانی صحيح wanted, بنانشانی صحيح *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	بنانشانی صحيح گنتی = 0;
	uint64_t isize = EXT2_I_ناپ(&file->inode);
	size_t ناپ;

	إذا (file->pos >= isize)
		اذهب باہرلا;

	fs = file->fs;
	retval = ext2fs_inline_انکڑے_get(fs, file->ino, &file->inode,
					file->buf, &ناپ);
	إذا (retval)
		أرجع retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	إذا (ناپ > isize)
		ناپ = isize;

	گنتی = ناپ - file->pos;
	إذا (گنتی > wanted)
		گنتی = wanted;
	حافظہ_نقل(buf, file->buf + file->pos, گنتی);
	file->pos += گنتی;
	buf = (حرف *) buf + گنتی;

	/* zero-fill the rest of the buffer */
	wanted -= گنتی;
	إذا (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		گنتی += wanted;
	}

باہرلا:
	إذا (got)
		*got = گنتی;
	أرجع retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, فراغ *buf,
			   بنانشانی صحيح wanted, بنانشانی صحيح *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	بنانشانی صحيح	شروع, c, گنتی = 0;
	__u64		بایاں;
	حرف		*ptr = (حرف *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	إذا (file->inode.i_flags & EXT4_INLINE_انکڑے_FL)
		أرجع ext2fs_file_read_inline_انکڑے(file, buf, wanted, got);

	طالما ((file->pos < EXT2_I_ناپ(&file->inode)) && (wanted > 0)) {
		retval = sync_بفر_position(file);
		إذا (retval)
			اذهب fail;
		retval = لدو_بفر(file, 0);
		إذا (retval)
			اذهب fail;

		شروع = file->pos % fs->blocksize;
		c = fs->blocksize - شروع;
		إذا (c > wanted)
			c = wanted;
		بایاں = EXT2_I_ناپ(&file->inode) - file->pos ;
		إذا (c > بایاں)
			c = بایاں;

		حافظہ_نقل(ptr, file->buf+شروع, c);
		file->pos += c;
		ptr += c;
		گنتی += c;
		wanted -= c;
	}

fail:
	إذا (got)
		*got = گنتی;
	أرجع retval;
}


ساكن errcode_t
ext2fs_file_write_inline_انکڑے(ext2_file_t file, ثابت فراغ *buf,
			      بنانشانی صحيح nbytes, بنانشانی صحيح *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_ناپ(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t ناپ;

	إذا (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_انکڑے_get(fs, file->ino, &file->inode,
					file->buf, &ناپ);
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

		حافظہ_نقل(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_انکڑے_سیٹ(fs, file->ino, &file->inode,
						file->buf, new_isize);
		إذا (retval == EXT2_ET_INLINE_انکڑے_NO_SPACE)
			اذهب expand;
		إذا (retval)
			أرجع retval;

		file->pos += nbytes;

		/* Update inode size */
		إذا (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_سیٹ_size2(file, file->pos);
			إذا (retval == 0)
				retval = rc;
		}

		إذا (written)
			*written = nbytes;
		أرجع 0;
	}

expand:
	retval = ext2fs_inline_انکڑے_expand(fs, file->ino);
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
	أرجع EXT2_ET_INLINE_انکڑے_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ثابت فراغ *buf,
			    بنانشانی صحيح nbytes, بنانشانی صحيح *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	بنانشانی صحيح	شروع, c, گنتی = 0;
	ثابت حرف	*ptr = (ثابت حرف *) buf;
	بلاک_اندراج_t	new_بلاک = عدم, old_بلاک = عدم;
	صحيح		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	إذا (!(file->flags & EXT2_FILE_WRITE))
		أرجع EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	إذا (file->inode.i_flags & EXT4_INLINE_انکڑے_FL) {
		retval = ext2fs_file_write_inline_انکڑے(file, buf, nbytes,
						       written);
		إذا (retval != EXT2_ET_INLINE_انکڑے_NO_SPACE)
			أرجع retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	طالما (nbytes > 0) {
		retval = sync_بفر_position(file);
		إذا (retval)
			اذهب fail;

		شروع = file->pos % fs->blocksize;
		c = fs->blocksize - شروع;
		إذا (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = لدو_بفر(file, (c == fs->blocksize));
		إذا (retval)
			اذهب fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		حافظہ_نقل(file->buf+شروع, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		إذا (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			إذا (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_بلاک = صفر_حافظہ_دیو(1, حجم(*new_بلاک));
				إذا (!new_بلاک) {
					retval = EXT2_ET_NO_حافظہ;
					اذهب fail;
				}
				ext2fs_sha512((ثابت بنانشانی حرف*)file->buf,
						fs->blocksize, new_بلاک->sha);
				old_بلاک = ext2fs_hashmap_lookup(
							fs->بلاک_sha_map,
							new_بلاک->sha,
							حجم(new_بلاک->sha));
			}

			إذا (old_بلاک) {
				file->physblock = old_بلاک->physblock;
				bmap_flags |= BMAP_سیٹ;
				حرر(new_بلاک);
				new_بلاک = عدم;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_بفر,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			إذا (retval) {
				حرر(new_بلاک);
				new_بلاک = عدم;
				اذهب fail;
			}

			إذا (new_بلاک) {
				new_بلاک->physblock = file->physblock;
				صحيح ret = ext2fs_hashmap_جوڑو(fs->بلاک_sha_map,
						new_بلاک, new_بلاک->sha,
						حجم(new_بلاک->sha));
				إذا (ret) {
					retval = EXT2_ET_NO_حافظہ;
					حرر(new_بلاک);
					اذهب fail;
				}
			}

			إذا (bmap_flags & BMAP_سیٹ) {
				ext2fs_iblk_جوڑو_بلاکاں(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		گنتی += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	إذا (گنتی != 0 && EXT2_I_ناپ(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_سیٹ_size2(file, file->pos);
		إذا (retval == 0)
			retval = rc;
	}

	إذا (written)
		*written = گنتی;
	أرجع retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 وتھ,
			    صحيح whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	إذا (whence == EXT2_SEEK_سیٹ)
		file->pos = وتھ;
	وإلا إذا (whence == EXT2_SEEK_CUR)
		file->pos += وتھ;
	وإلا إذا (whence == EXT2_SEEK_سرا)
		file->pos = EXT2_I_ناپ(&file->inode) + وتھ;
	وإلا
		أرجع EXT2_ET_INVALID_دلیل;

	إذا (ret_pos)
		*ret_pos = file->pos;

	أرجع 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t وتھ,
			    صحيح whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = وتھ;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	إذا (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	أرجع retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_ناپ)
{
	إذا (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		أرجع EXT2_ET_MAGIC_EXT2_FILE;
	*ret_ناپ = EXT2_I_ناپ(&file->inode);
	أرجع 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_ناپ(ext2_file_t file)
{
	__u64	ناپ;

	إذا (ext2fs_file_get_lsize(file, &ناپ))
		أرجع 0;
	إذا ((ناپ >> 32) != 0)
		أرجع 0;
	أرجع ناپ;
}

/* Zero the parts of the last block that are past EOF. */
ساكن errcode_t ext2fs_file_zero_past_وتھ(ext2_file_t file,
					      ext2_off64_t وتھ)
{
	ext2_filsys fs = file->fs;
	حرف *b = عدم;
	ext2_off64_t off = وتھ % fs->blocksize;
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
			      وتھ / fs->blocksize, &ret_flags, &blk);
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
		اذهب باہرلا;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	إذا (retval)
		اذهب باہرلا;

باہرلا:
	ext2fs_free_mem(&b);
	أرجع retval;
}

/*
 * This function sets the size of the file, truncating it if necessary
 *
 */
errcode_t ext2fs_file_سیٹ_size2(ext2_file_t file, ext2_off64_t ناپ)
{
	ext2_off64_t	old_ناپ;
	errcode_t	retval;
	blk64_t		old_truncate, truncate_بلاک;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	إذا (ناپ && ext2fs_file_بلاک_وتھ_too_big(file->fs, &file->inode,
					(ناپ - 1) / file->fs->blocksize))
		أرجع EXT2_ET_FILE_TOO_BIG;
	truncate_بلاک = ((ناپ + file->fs->blocksize - 1) >>
			  EXT2_بلاک_ناپ_بٹ(file->fs->super));
	old_ناپ = EXT2_I_ناپ(&file->inode);
	old_truncate = ((old_ناپ + file->fs->blocksize - 1) >>
		      EXT2_بلاک_ناپ_بٹ(file->fs->super));

	retval = ext2fs_inode_ناپ_سیٹ(file->fs, &file->inode, ناپ);
	إذا (retval)
		أرجع retval;

	إذا (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		إذا (retval)
			أرجع retval;
	}

	retval = ext2fs_file_zero_past_وتھ(file, ناپ);
	إذا (retval)
		أرجع retval;

	إذا (truncate_بلاک >= old_truncate)
		أرجع 0;

	أرجع ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_بلاک, ~0ULL);
}

errcode_t ext2fs_file_سیٹ_ناپ(ext2_file_t file, ext2_off_t ناپ)
{
	أرجع ext2fs_file_سیٹ_size2(file, ناپ);
}
