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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ರಚನೆ ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	ರಚನೆ ext2_inode	inode;
	ಪೂರ್ಣಾಂಕ 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	ಅಕ್ಷರ 			*buf;
};

ರಚನೆ ಖಂಡ_ನಮೂದು {
	blk64_t		physblock;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ 	sha[EXT2_FS_SHA512_ಉದ್ದಳತೆ];
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಖಂಡ_ನಮೂದು *ಖಂಡ_ನಮೂದು_t;

#ವ್ಯಾಖ್ಯಾನಿಸು BMAP_ಬಫರ್ (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    ರಚನೆ ext2_inode *inode,
			    ಪೂರ್ಣಾಂಕ flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	ಆದರೆ ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_file), &file);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	memset(file, 0, ಗಾತ್ರ(ರಚನೆ ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_ಮುಖವಾಡ;

	ಆದರೆ (inode) {
		ಸ್ಮರಣೆ_ನಕಲು(&file->inode, inode, ಗಾತ್ರ(ರಚನೆ ext2_inode));
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		ಆದರೆ (retval)
			ಹೋಗು fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	ಆದರೆ (retval)
		ಹೋಗು fail;

	*ret = file;
	ಹಿಂತಿರುಗಿ 0;

fail:
	ಆದರೆ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   ಪೂರ್ಣಾಂಕ flags, ext2_file_t *ret)
{
	ಹಿಂತಿರುಗಿ ext2fs_file_open2(fs, ino, ಖಾಲಿ, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	ಆದರೆ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
ರಚನೆ ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	ಆದರೆ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_ಅಂಕೆ(ext2_file_t file)
{
	ಆದರೆ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	ಪೂರ್ಣಾಂಕ		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ಆದರೆ (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		ಹಿಂತಿರುಗಿ 0;

	/* Is this an uninit block? */
	ಆದರೆ (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_ಬಫರ್,
				      0, file->blockno, &ret_flags, &dontcare);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಆದರೆ (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_ಬಫರ್, BMAP_ಗಣ,
					      file->blockno, 0,
					      &file->physblock);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	ಆದರೆ (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_ಬಫರ್, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	ಹಿಂತಿರುಗಿ retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
ಸ್ಥಿರ errcode_t sync_ಬಫರ್_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	ಆದರೆ (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * This function loads the file's block buffer with valid data from
 * the disk as necessary.
 *
 * If dontfill is true, then skip initializing the buffer since we're
 * going to be replacing its entire contents anyway.  If set, then the
 * function basically only sets file->physblock and EXT2_FILE_BUF_VALID
 */
#ವ್ಯಾಖ್ಯಾನಿಸು DONTFILL 1
ಸ್ಥಿರ errcode_t ತುಂಬು_ಬಫರ್(ext2_file_t file, ಪೂರ್ಣಾಂಕ dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	ಪೂರ್ಣಾಂಕ		ret_flags;

	ಆದರೆ (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_ಬಫರ್, 0, file->blockno, &ret_flags,
				     &file->physblock);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಆದರೆ (!dontfill) {
			ಆದರೆ (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				ಆದರೆ (retval)
					ಹಿಂತಿರುಗಿ retval;
			} ಇಲ್ಲದಿದ್ದರೆ
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	ಹಿಂತಿರುಗಿ 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	ಆದರೆ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	ಹಿಂತಿರುಗಿ retval;
}


ಸ್ಥಿರ errcode_t
ext2fs_file_read_inline_ದತ್ತಾಂಶ(ext2_file_t file, ಶೂನ್ಯ *buf,
			     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ wanted, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
	uint64_t isize = EXT2_I_ಗಾತ್ರ(&file->inode);
	size_t ಗಾತ್ರ_2;

	ಆದರೆ (file->pos >= isize)
		ಹೋಗು ಹೊರಹೋಗು;

	fs = file->fs;
	retval = ext2fs_inline_ದತ್ತಾಂಶ_get(fs, file->ino, &file->inode,
					file->buf, &ಗಾತ್ರ_2);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	ಆದರೆ (ಗಾತ್ರ_2 > isize)
		ಗಾತ್ರ_2 = isize;

	ಎಣಿಕೆ_2 = ಗಾತ್ರ_2 - file->pos;
	ಆದರೆ (ಎಣಿಕೆ_2 > wanted)
		ಎಣಿಕೆ_2 = wanted;
	ಸ್ಮರಣೆ_ನಕಲು(buf, file->buf + file->pos, ಎಣಿಕೆ_2);
	file->pos += ಎಣಿಕೆ_2;
	buf = (ಅಕ್ಷರ *) buf + ಎಣಿಕೆ_2;

	/* zero-fill the rest of the buffer */
	wanted -= ಎಣಿಕೆ_2;
	ಆದರೆ (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		ಎಣಿಕೆ_2 += wanted;
	}

ಹೊರಹೋಗು:
	ಆದರೆ (got)
		*got = ಎಣಿಕೆ_2;
	ಹಿಂತಿರುಗಿ retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, ಶೂನ್ಯ *buf,
			   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ wanted, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ಆರಂಭ, c, ಎಣಿಕೆ_2 = 0;
	__u64		ಎಡ;
	ಅಕ್ಷರ		*ptr = (ಅಕ್ಷರ *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	ಆದರೆ (file->inode.i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL)
		ಹಿಂತಿರುಗಿ ext2fs_file_read_inline_ದತ್ತಾಂಶ(file, buf, wanted, got);

	ಆಗ ((file->pos < EXT2_I_ಗಾತ್ರ(&file->inode)) && (wanted > 0)) {
		retval = sync_ಬಫರ್_position(file);
		ಆದರೆ (retval)
			ಹೋಗು fail;
		retval = ತುಂಬು_ಬಫರ್(file, 0);
		ಆದರೆ (retval)
			ಹೋಗು fail;

		ಆರಂಭ = file->pos % fs->blocksize;
		c = fs->blocksize - ಆರಂಭ;
		ಆದರೆ (c > wanted)
			c = wanted;
		ಎಡ = EXT2_I_ಗಾತ್ರ(&file->inode) - file->pos ;
		ಆದರೆ (c > ಎಡ)
			c = ಎಡ;

		ಸ್ಮರಣೆ_ನಕಲು(ptr, file->buf+ಆರಂಭ, c);
		file->pos += c;
		ptr += c;
		ಎಣಿಕೆ_2 += c;
		wanted -= c;
	}

fail:
	ಆದರೆ (got)
		*got = ಎಣಿಕೆ_2;
	ಹಿಂತಿರುಗಿ retval;
}


ಸ್ಥಿರ errcode_t
ext2fs_file_write_inline_ದತ್ತಾಂಶ(ext2_file_t file, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf,
			      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nbytes, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_ಗಾತ್ರ(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t ಗಾತ್ರ_2;

	ಆದರೆ (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_ದತ್ತಾಂಶ_get(fs, file->ino, &file->inode,
					file->buf, &ಗಾತ್ರ_2);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	ಆದರೆ (new_isize < fs->blocksize) {
		ಆದರೆ (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		ಸ್ಮರಣೆ_ನಕಲು(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_ದತ್ತಾಂಶ_ಗಣ(fs, file->ino, &file->inode,
						file->buf, new_isize);
		ಆದರೆ (retval == EXT2_ET_INLINE_ದತ್ತಾಂಶ_NO_SPACE)
			ಹೋಗು expand;
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		file->pos += nbytes;

		/* Update inode size */
		ಆದರೆ (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_ಗಣ_size2(file, file->pos);
			ಆದರೆ (retval == 0)
				retval = rc;
		}

		ಆದರೆ (written)
			*written = nbytes;
		ಹಿಂತಿರುಗಿ 0;
	}

expand:
	retval = ext2fs_inline_ದತ್ತಾಂಶ_expand(fs, file->ino);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಹಿಂತಿರುಗಿ EXT2_ET_INLINE_ದತ್ತಾಂಶ_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf,
			    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nbytes, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ಆರಂಭ, c, ಎಣಿಕೆ_2 = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*ptr = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) buf;
	ಖಂಡ_ನಮೂದು_t	new_ಖಂಡ = ಖಾಲಿ, old_ಖಂಡ = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ಆದರೆ (!(file->flags & EXT2_FILE_WRITE))
		ಹಿಂತಿರುಗಿ EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	ಆದರೆ (file->inode.i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL) {
		retval = ext2fs_file_write_inline_ದತ್ತಾಂಶ(file, buf, nbytes,
						       written);
		ಆದರೆ (retval != EXT2_ET_INLINE_ದತ್ತಾಂಶ_NO_SPACE)
			ಹಿಂತಿರುಗಿ retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	ಆಗ (nbytes > 0) {
		retval = sync_ಬಫರ್_position(file);
		ಆದರೆ (retval)
			ಹೋಗು fail;

		ಆರಂಭ = file->pos % fs->blocksize;
		c = fs->blocksize - ಆರಂಭ;
		ಆದರೆ (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = ತುಂಬು_ಬಫರ್(file, (c == fs->blocksize));
		ಆದರೆ (retval)
			ಹೋಗು fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		ಸ್ಮರಣೆ_ನಕಲು(file->buf+ಆರಂಭ, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		ಆದರೆ (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			ಆದರೆ (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_ಖಂಡ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*new_ಖಂಡ));
				ಆದರೆ (!new_ಖಂಡ) {
					retval = EXT2_ET_NO_ಸ್ಮರಣೆ;
					ಹೋಗು fail;
				}
				ext2fs_sha512((ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*)file->buf,
						fs->blocksize, new_ಖಂಡ->sha);
				old_ಖಂಡ = ext2fs_hashmap_lookup(
							fs->ಖಂಡ_sha_map,
							new_ಖಂಡ->sha,
							ಗಾತ್ರ(new_ಖಂಡ->sha));
			}

			ಆದರೆ (old_ಖಂಡ) {
				file->physblock = old_ಖಂಡ->physblock;
				bmap_flags |= BMAP_ಗಣ;
				ಬಿಡುಗಡೆ(new_ಖಂಡ);
				new_ಖಂಡ = ಖಾಲಿ;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_ಬಫರ್,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			ಆದರೆ (retval) {
				ಬಿಡುಗಡೆ(new_ಖಂಡ);
				new_ಖಂಡ = ಖಾಲಿ;
				ಹೋಗು fail;
			}

			ಆದರೆ (new_ಖಂಡ) {
				new_ಖಂಡ->physblock = file->physblock;
				ಪೂರ್ಣಾಂಕ ret = ext2fs_hashmap_ಕೂಡಿಸು(fs->ಖಂಡ_sha_map,
						new_ಖಂಡ, new_ಖಂಡ->sha,
						ಗಾತ್ರ(new_ಖಂಡ->sha));
				ಆದರೆ (ret) {
					retval = EXT2_ET_NO_ಸ್ಮರಣೆ;
					ಬಿಡುಗಡೆ(new_ಖಂಡ);
					ಹೋಗು fail;
				}
			}

			ಆದರೆ (bmap_flags & BMAP_ಗಣ) {
				ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		ಎಣಿಕೆ_2 += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	ಆದರೆ (ಎಣಿಕೆ_2 != 0 && EXT2_I_ಗಾತ್ರ(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_ಗಣ_size2(file, file->pos);
		ಆದರೆ (retval == 0)
			retval = rc;
	}

	ಆದರೆ (written)
		*written = ಎಣಿಕೆ_2;
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 ಸ್ಥಾನಾಂತರ,
			    ಪೂರ್ಣಾಂಕ whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	ಆದರೆ (whence == EXT2_SEEK_ಗಣ)
		file->pos = ಸ್ಥಾನಾಂತರ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (whence == EXT2_SEEK_CUR)
		file->pos += ಸ್ಥಾನಾಂತರ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (whence == EXT2_SEEK_ಕೊನೆ)
		file->pos = EXT2_I_ಗಾತ್ರ(&file->inode) + ಸ್ಥಾನಾಂತರ;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (ret_pos)
		*ret_pos = file->pos;

	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t ಸ್ಥಾನಾಂತರ,
			    ಪೂರ್ಣಾಂಕ whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = ಸ್ಥಾನಾಂತರ;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	ಆದರೆ (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	ಹಿಂತಿರುಗಿ retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_ಗಾತ್ರ)
{
	ಆದರೆ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		ಹಿಂತಿರುಗಿ EXT2_ET_MAGIC_EXT2_FILE;
	*ret_ಗಾತ್ರ = EXT2_I_ಗಾತ್ರ(&file->inode);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_ಗಾತ್ರ(ext2_file_t file)
{
	__u64	ಗಾತ್ರ_2;

	ಆದರೆ (ext2fs_file_get_lsize(file, &ಗಾತ್ರ_2))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((ಗಾತ್ರ_2 >> 32) != 0)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2;
}

/* Zero the parts of the last block that are past EOF. */
ಸ್ಥಿರ errcode_t ext2fs_file_zero_past_ಸ್ಥಾನಾಂತರ(ext2_file_t file,
					      ext2_off64_t ಸ್ಥಾನಾಂತರ)
{
	ext2_filsys fs = file->fs;
	ಅಕ್ಷರ *b = ಖಾಲಿ;
	ext2_off64_t off = ಸ್ಥಾನಾಂತರ % fs->blocksize;
	blk64_t blk;
	ಪೂರ್ಣಾಂಕ ret_flags;
	errcode_t retval;

	ಆದರೆ (off == 0)
		ಹಿಂತಿರುಗಿ 0;

	retval = sync_ಬಫರ್_position(file);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, ಖಾಲಿ, ಖಾಲಿ, 0,
			      ಸ್ಥಾನಾಂತರ / fs->blocksize, &ret_flags, &blk);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		ಹಿಂತಿರುಗಿ 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;

ಹೊರಹೋಗು:
	ext2fs_free_mem(&b);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * This function sets the size of the file, truncating it if necessary
 *
 */
errcode_t ext2fs_file_ಗಣ_size2(ext2_file_t file, ext2_off64_t ಗಾತ್ರ_2)
{
	ext2_off64_t	old_ಗಾತ್ರ;
	errcode_t	retval;
	blk64_t		old_truncate, truncate_ಖಂಡ;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	ಆದರೆ (ಗಾತ್ರ_2 && ext2fs_file_ಖಂಡ_ಸ್ಥಾನಾಂತರ_too_big(file->fs, &file->inode,
					(ಗಾತ್ರ_2 - 1) / file->fs->blocksize))
		ಹಿಂತಿರುಗಿ EXT2_ET_FILE_TOO_BIG;
	truncate_ಖಂಡ = ((ಗಾತ್ರ_2 + file->fs->blocksize - 1) >>
			  EXT2_ಖಂಡ_ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು(file->fs->super));
	old_ಗಾತ್ರ = EXT2_I_ಗಾತ್ರ(&file->inode);
	old_truncate = ((old_ಗಾತ್ರ + file->fs->blocksize - 1) >>
		      EXT2_ಖಂಡ_ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು(file->fs->super));

	retval = ext2fs_inode_ಗಾತ್ರ_ಗಣ(file->fs, &file->inode, ಗಾತ್ರ_2);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	retval = ext2fs_file_zero_past_ಸ್ಥಾನಾಂತರ(file, ಗಾತ್ರ_2);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (truncate_ಖಂಡ >= old_truncate)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_ಖಂಡ, ~0ULL);
}

errcode_t ext2fs_file_ಗಣ_ಗಾತ್ರ(ext2_file_t file, ext2_off_t ಗಾತ್ರ_2)
{
	ಹಿಂತಿರುಗಿ ext2fs_file_ಗಣ_size2(file, ಗಾತ್ರ_2);
}
