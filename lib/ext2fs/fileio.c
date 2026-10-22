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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساخت ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	ساخت ext2_inode	inode;
	صحیح 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	حرف 			*buf;
};

ساخت بلاک_اندراج {
	blk64_t		physblock;
	غیرعلامتی حرف 	sha[EXT2_FS_SHA512_لمبائی];
};
قسمتعریف ساخت بلاک_اندراج *بلاک_اندراج_t;

#تعریف_کرو BMAP_بفر (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    ساخت ext2_inode *inode,
			    صحیح flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	اگر ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(حجم(ساخت ext2_file), &file);
	اگر (retval)
		واپس retval;

	memset(file, 0, حجم(ساخت ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_نقاب;

	اگر (inode) {
		حافظہ_نقل(&file->inode, inode, حجم(ساخت ext2_inode));
	} ورنہ {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		اگر (retval)
			جاؤ fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	اگر (retval)
		جاؤ fail;

	*ret = file;
	واپس 0;

fail:
	اگر (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	واپس retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   صحیح flags, ext2_file_t *ret)
{
	واپس ext2fs_file_open2(fs, ino, تہی, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	اگر (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		واپس 0;
	واپس file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
ساخت ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	اگر (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		واپس تہی;
	واپس &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_عدد(ext2_file_t file)
{
	اگر (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		واپس 0;
	واپس file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	صحیح		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	اگر (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		واپس 0;

	/* Is this an uninit block? */
	اگر (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_بفر,
				      0, file->blockno, &ret_flags, &dontcare);
		اگر (retval)
			واپس retval;
		اگر (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_بفر, BMAP_سیٹ,
					      file->blockno, 0,
					      &file->physblock);
			اگر (retval)
				واپس retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	اگر (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_بفر, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		اگر (retval)
			واپس retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	اگر (retval)
		واپس retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	واپس retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
ساکن errcode_t sync_بفر_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	اگر (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		اگر (retval)
			واپس retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	واپس 0;
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
ساکن errcode_t بھرو_بفر(ext2_file_t file, صحیح dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	صحیح		ret_flags;

	اگر (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_بفر, 0, file->blockno, &ret_flags,
				     &file->physblock);
		اگر (retval)
			واپس retval;
		اگر (!dontfill) {
			اگر (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				اگر (retval)
					واپس retval;
			} ورنہ
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	واپس 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	اگر (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	واپس retval;
}


ساکن errcode_t
ext2fs_file_read_inline_ڈیٹا(ext2_file_t file, خالی *buf,
			     غیرعلامتی صحیح wanted, غیرعلامتی صحیح *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	غیرعلامتی صحیح گنتی = 0;
	uint64_t isize = EXT2_I_سائز(&file->inode);
	size_t سائز;

	اگر (file->pos >= isize)
		جاؤ برآمد;

	fs = file->fs;
	retval = ext2fs_inline_ڈیٹا_get(fs, file->ino, &file->inode,
					file->buf, &سائز);
	اگر (retval)
		واپس retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	اگر (سائز > isize)
		سائز = isize;

	گنتی = سائز - file->pos;
	اگر (گنتی > wanted)
		گنتی = wanted;
	حافظہ_نقل(buf, file->buf + file->pos, گنتی);
	file->pos += گنتی;
	buf = (حرف *) buf + گنتی;

	/* zero-fill the rest of the buffer */
	wanted -= گنتی;
	اگر (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		گنتی += wanted;
	}

برآمد:
	اگر (got)
		*got = گنتی;
	واپس retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, خالی *buf,
			   غیرعلامتی صحیح wanted, غیرعلامتی صحیح *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	غیرعلامتی صحیح	آغاز, c, گنتی = 0;
	__u64		بایاں;
	حرف		*ptr = (حرف *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	اگر (file->inode.i_flags & EXT4_INLINE_ڈیٹا_FL)
		واپس ext2fs_file_read_inline_ڈیٹا(file, buf, wanted, got);

	جب ((file->pos < EXT2_I_سائز(&file->inode)) && (wanted > 0)) {
		retval = sync_بفر_position(file);
		اگر (retval)
			جاؤ fail;
		retval = بھرو_بفر(file, 0);
		اگر (retval)
			جاؤ fail;

		آغاز = file->pos % fs->blocksize;
		c = fs->blocksize - آغاز;
		اگر (c > wanted)
			c = wanted;
		بایاں = EXT2_I_سائز(&file->inode) - file->pos ;
		اگر (c > بایاں)
			c = بایاں;

		حافظہ_نقل(ptr, file->buf+آغاز, c);
		file->pos += c;
		ptr += c;
		گنتی += c;
		wanted -= c;
	}

fail:
	اگر (got)
		*got = گنتی;
	واپس retval;
}


ساکن errcode_t
ext2fs_file_write_inline_ڈیٹا(ext2_file_t file, ثابت خالی *buf,
			      غیرعلامتی صحیح nbytes, غیرعلامتی صحیح *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_سائز(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t سائز;

	اگر (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_ڈیٹا_get(fs, file->ino, &file->inode,
					file->buf, &سائز);
	اگر (retval)
		واپس retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	اگر (new_isize < fs->blocksize) {
		اگر (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		حافظہ_نقل(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_ڈیٹا_سیٹ(fs, file->ino, &file->inode,
						file->buf, new_isize);
		اگر (retval == EXT2_ET_INLINE_ڈیٹا_NO_SPACE)
			جاؤ expand;
		اگر (retval)
			واپس retval;

		file->pos += nbytes;

		/* Update inode size */
		اگر (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_سیٹ_size2(file, file->pos);
			اگر (retval == 0)
				retval = rc;
		}

		اگر (written)
			*written = nbytes;
		واپس 0;
	}

expand:
	retval = ext2fs_inline_ڈیٹا_expand(fs, file->ino);
	اگر (retval)
		واپس retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	اگر (retval)
		واپس retval;
	واپس EXT2_ET_INLINE_ڈیٹا_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ثابت خالی *buf,
			    غیرعلامتی صحیح nbytes, غیرعلامتی صحیح *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	غیرعلامتی صحیح	آغاز, c, گنتی = 0;
	ثابت حرف	*ptr = (ثابت حرف *) buf;
	بلاک_اندراج_t	new_بلاک = تہی, old_بلاک = تہی;
	صحیح		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	اگر (!(file->flags & EXT2_FILE_WRITE))
		واپس EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	اگر (file->inode.i_flags & EXT4_INLINE_ڈیٹا_FL) {
		retval = ext2fs_file_write_inline_ڈیٹا(file, buf, nbytes,
						       written);
		اگر (retval != EXT2_ET_INLINE_ڈیٹا_NO_SPACE)
			واپس retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	جب (nbytes > 0) {
		retval = sync_بفر_position(file);
		اگر (retval)
			جاؤ fail;

		آغاز = file->pos % fs->blocksize;
		c = fs->blocksize - آغاز;
		اگر (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = بھرو_بفر(file, (c == fs->blocksize));
		اگر (retval)
			جاؤ fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		حافظہ_نقل(file->buf+آغاز, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		اگر (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			اگر (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_بلاک = صفر_حافظہ_دو(1, حجم(*new_بلاک));
				اگر (!new_بلاک) {
					retval = EXT2_ET_NO_حافظہ;
					جاؤ fail;
				}
				ext2fs_sha512((ثابت غیرعلامتی حرف*)file->buf,
						fs->blocksize, new_بلاک->sha);
				old_بلاک = ext2fs_hashmap_lookup(
							fs->بلاک_sha_map,
							new_بلاک->sha,
							حجم(new_بلاک->sha));
			}

			اگر (old_بلاک) {
				file->physblock = old_بلاک->physblock;
				bmap_flags |= BMAP_سیٹ;
				آزاد(new_بلاک);
				new_بلاک = تہی;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_بفر,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			اگر (retval) {
				آزاد(new_بلاک);
				new_بلاک = تہی;
				جاؤ fail;
			}

			اگر (new_بلاک) {
				new_بلاک->physblock = file->physblock;
				صحیح ret = ext2fs_hashmap_جوڑو(fs->بلاک_sha_map,
						new_بلاک, new_بلاک->sha,
						حجم(new_بلاک->sha));
				اگر (ret) {
					retval = EXT2_ET_NO_حافظہ;
					آزاد(new_بلاک);
					جاؤ fail;
				}
			}

			اگر (bmap_flags & BMAP_سیٹ) {
				ext2fs_iblk_جوڑو_بلاکیں(fs, &file->inode, 1);
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
	اگر (گنتی != 0 && EXT2_I_سائز(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_سیٹ_size2(file, file->pos);
		اگر (retval == 0)
			retval = rc;
	}

	اگر (written)
		*written = گنتی;
	واپس retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 انحراف,
			    صحیح whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	اگر (whence == EXT2_SEEK_سیٹ)
		file->pos = انحراف;
	ورنہ اگر (whence == EXT2_SEEK_CUR)
		file->pos += انحراف;
	ورنہ اگر (whence == EXT2_SEEK_انتہا)
		file->pos = EXT2_I_سائز(&file->inode) + انحراف;
	ورنہ
		واپس EXT2_ET_INVALID_دلیل;

	اگر (ret_pos)
		*ret_pos = file->pos;

	واپس 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t انحراف,
			    صحیح whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = انحراف;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	اگر (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	واپس retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_سائز)
{
	اگر (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		واپس EXT2_ET_MAGIC_EXT2_FILE;
	*ret_سائز = EXT2_I_سائز(&file->inode);
	واپس 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_سائز(ext2_file_t file)
{
	__u64	سائز;

	اگر (ext2fs_file_get_lsize(file, &سائز))
		واپس 0;
	اگر ((سائز >> 32) != 0)
		واپس 0;
	واپس سائز;
}

/* Zero the parts of the last block that are past EOF. */
ساکن errcode_t ext2fs_file_zero_past_انحراف(ext2_file_t file,
					      ext2_off64_t انحراف)
{
	ext2_filsys fs = file->fs;
	حرف *b = تہی;
	ext2_off64_t off = انحراف % fs->blocksize;
	blk64_t blk;
	صحیح ret_flags;
	errcode_t retval;

	اگر (off == 0)
		واپس 0;

	retval = sync_بفر_position(file);
	اگر (retval)
		واپس retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, تہی, تہی, 0,
			      انحراف / fs->blocksize, &ret_flags, &blk);
	اگر (retval)
		واپس retval;
	اگر ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		واپس 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	اگر (retval)
		واپس retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	اگر (retval)
		جاؤ برآمد;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	اگر (retval)
		جاؤ برآمد;

برآمد:
	ext2fs_free_mem(&b);
	واپس retval;
}

/*
 * This function sets the size of the file, truncating it if necessary
 *
 */
errcode_t ext2fs_file_سیٹ_size2(ext2_file_t file, ext2_off64_t سائز)
{
	ext2_off64_t	old_سائز;
	errcode_t	retval;
	blk64_t		old_truncate, truncate_بلاک;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	اگر (سائز && ext2fs_file_بلاک_انحراف_too_big(file->fs, &file->inode,
					(سائز - 1) / file->fs->blocksize))
		واپس EXT2_ET_FILE_TOO_BIG;
	truncate_بلاک = ((سائز + file->fs->blocksize - 1) >>
			  EXT2_بلاک_سائز_بٹ(file->fs->super));
	old_سائز = EXT2_I_سائز(&file->inode);
	old_truncate = ((old_سائز + file->fs->blocksize - 1) >>
		      EXT2_بلاک_سائز_بٹ(file->fs->super));

	retval = ext2fs_inode_سائز_سیٹ(file->fs, &file->inode, سائز);
	اگر (retval)
		واپس retval;

	اگر (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		اگر (retval)
			واپس retval;
	}

	retval = ext2fs_file_zero_past_انحراف(file, سائز);
	اگر (retval)
		واپس retval;

	اگر (truncate_بلاک >= old_truncate)
		واپس 0;

	واپس ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_بلاک, ~0ULL);
}

errcode_t ext2fs_file_سیٹ_سائز(ext2_file_t file, ext2_off_t سائز)
{
	واپس ext2fs_file_سیٹ_size2(file, سائز);
}
