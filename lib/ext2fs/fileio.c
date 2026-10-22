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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

నిర్మాణం ext2_file {
	errcode_t		magic;
	ext2_filsys 		fs;
	ext2_ino_t		ino;
	నిర్మాణం ext2_inode	inode;
	పూర్ణాంకం 			flags;
	__u64			pos;
	blk64_t			blockno;
	blk64_t			physblock;
	అక్షరం 			*buf;
};

నిర్మాణం ఖండం_నమోదు {
	blk64_t		physblock;
	గుర్తులేని అక్షరం 	sha[EXT2_FS_SHA512_నిడివి];
};
రకనిర్వచనం నిర్మాణం ఖండం_నమోదు *ఖండం_నమోదు_t;

#నిర్వచించు BMAP_బఫరు (file->buf + fs->blocksize)

errcode_t ext2fs_file_open2(ext2_filsys fs, ext2_ino_t ino,
			    నిర్మాణం ext2_inode *inode,
			    పూర్ణాంకం flags, ext2_file_t *ret)
{
	ext2_file_t 	file;
	errcode_t	retval;

	/*
	 * Don't let caller create or open a file for writing if the
	 * filesystem is read-only.
	 */
	ఒకవేళ ((flags & (EXT2_FILE_WRITE | EXT2_FILE_CREATE)) &&
	    !(fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_file), &file);
	ఒకవేళ (retval)
		తిరిగి retval;

	memset(file, 0, పరిమాణం(నిర్మాణం ext2_file));
	file->magic = EXT2_ET_MAGIC_EXT2_FILE;
	file->fs = fs;
	file->ino = ino;
	file->flags = flags & EXT2_FILE_ముసుగు;

	ఒకవేళ (inode) {
		జ్ఞాపకం_నకలు(&file->inode, inode, పరిమాణం(నిర్మాణం ext2_inode));
	} లేకపోతే {
		retval = ext2fs_read_inode(fs, ino, &file->inode);
		ఒకవేళ (retval)
			వెళ్ళు fail;
	}

	retval = ext2fs_get_array(3, fs->blocksize, &file->buf);
	ఒకవేళ (retval)
		వెళ్ళు fail;

	*ret = file;
	తిరిగి 0;

fail:
	ఒకవేళ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);
	తిరిగి retval;
}

errcode_t ext2fs_file_open(ext2_filsys fs, ext2_ino_t ino,
			   పూర్ణాంకం flags, ext2_file_t *ret)
{
	తిరిగి ext2fs_file_open2(fs, ino, ఖాళీ, flags, ret);
}

/*
 * This function returns the filesystem handle of a file from the structure
 */
ext2_filsys ext2fs_file_get_fs(ext2_file_t file)
{
	ఒకవేళ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		తిరిగి 0;
	తిరిగి file->fs;
}

/*
 * This function returns the pointer to the inode of a file from the structure
 */
నిర్మాణం ext2_inode *ext2fs_file_get_inode(ext2_file_t file)
{
	ఒకవేళ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		తిరిగి ఖాళీ;
	తిరిగి &file->inode;
}

/* This function returns the inode number from the structure */
ext2_ino_t ext2fs_file_get_inode_అంకె(ext2_file_t file)
{
	ఒకవేళ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		తిరిగి 0;
	తిరిగి file->ino;
}

/*
 * This function flushes the dirty block buffer out to disk if
 * necessary.
 */
errcode_t ext2fs_file_flush(ext2_file_t file)
{
	errcode_t	retval;
	ext2_filsys fs;
	పూర్ణాంకం		ret_flags;
	blk64_t		dontcare;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ఒకవేళ (!(file->flags & EXT2_FILE_BUF_VALID) ||
	    !(file->flags & EXT2_FILE_BUF_DIRTY))
		తిరిగి 0;

	/* Is this an uninit block? */
	ఒకవేళ (file->physblock && file->inode.i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode, BMAP_బఫరు,
				      0, file->blockno, &ret_flags, &dontcare);
		ఒకవేళ (retval)
			తిరిగి retval;
		ఒకవేళ (ret_flags & BMAP_RET_UNINIT) {
			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_బఫరు, BMAP_సమితి,
					      file->blockno, 0,
					      &file->physblock);
			ఒకవేళ (retval)
				తిరిగి retval;
		}
	}

	/*
	 * OK, the physical block hasn't been allocated yet.
	 * Allocate it.
	 */
	ఒకవేళ (!file->physblock) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_బఫరు, file->ino ? BMAP_ALLOC : 0,
				     file->blockno, 0, &file->physblock);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	retval = io_channel_write_blk64(fs->io, file->physblock, 1, file->buf);
	ఒకవేళ (retval)
		తిరిగి retval;

	file->flags &= ~EXT2_FILE_BUF_DIRTY;

	తిరిగి retval;
}

/*
 * This function synchronizes the file's block buffer and the current
 * file position, possibly invalidating block buffer if necessary
 */
స్థిర errcode_t sync_బఫరు_position(ext2_file_t file)
{
	blk64_t	b;
	errcode_t	retval;

	b = file->pos / file->fs->blocksize;
	ఒకవేళ (b != file->blockno) {
		retval = ext2fs_file_flush(file);
		ఒకవేళ (retval)
			తిరిగి retval;
		file->flags &= ~EXT2_FILE_BUF_VALID;
	}
	file->blockno = b;
	తిరిగి 0;
}

/*
 * This function loads the file's block buffer with valid data from
 * the disk as necessary.
 *
 * If dontfill is true, then skip initializing the buffer since we're
 * going to be replacing its entire contents anyway.  If set, then the
 * function basically only sets file->physblock and EXT2_FILE_BUF_VALID
 */
#నిర్వచించు DONTFILL 1
స్థిర errcode_t ఎక్కించు_బఫరు(ext2_file_t file, పూర్ణాంకం dontfill)
{
	ext2_filsys	fs = file->fs;
	errcode_t	retval;
	పూర్ణాంకం		ret_flags;

	ఒకవేళ (!(file->flags & EXT2_FILE_BUF_VALID)) {
		retval = ext2fs_bmap2(fs, file->ino, &file->inode,
				     BMAP_బఫరు, 0, file->blockno, &ret_flags,
				     &file->physblock);
		ఒకవేళ (retval)
			తిరిగి retval;
		ఒకవేళ (!dontfill) {
			ఒకవేళ (file->physblock &&
			    !(ret_flags & BMAP_RET_UNINIT)) {
				retval = io_channel_read_blk64(fs->io,
							       file->physblock,
							       1, file->buf);
				ఒకవేళ (retval)
					తిరిగి retval;
			} లేకపోతే
				memset(file->buf, 0, fs->blocksize);
		}
		file->flags |= EXT2_FILE_BUF_VALID;
	}
	తిరిగి 0;
}


errcode_t ext2fs_file_close(ext2_file_t file)
{
	errcode_t	retval;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	retval = ext2fs_file_flush(file);

	ఒకవేళ (file->buf)
		ext2fs_free_mem(&file->buf);
	ext2fs_free_mem(&file);

	తిరిగి retval;
}


స్థిర errcode_t
ext2fs_file_read_inline_దత్తాంశం(ext2_file_t file, శూన్య *buf,
			     గుర్తులేని పూర్ణాంకం wanted, గుర్తులేని పూర్ణాంకం *got)
{
	ext2_filsys fs;
	errcode_t retval = 0;
	గుర్తులేని పూర్ణాంకం లెక్క = 0;
	uint64_t isize = EXT2_I_కొలత(&file->inode);
	size_t కొలత;

	ఒకవేళ (file->pos >= isize)
		వెళ్ళు బహిర్గతం;

	fs = file->fs;
	retval = ext2fs_inline_దత్తాంశం_get(fs, file->ino, &file->inode,
					file->buf, &కొలత);
	ఒకవేళ (retval)
		తిరిగి retval;

	/*
	 * size is the number of bytes available for inline data storage, which
	 * means it can exceed isize.
	 */
	ఒకవేళ (కొలత > isize)
		కొలత = isize;

	లెక్క = కొలత - file->pos;
	ఒకవేళ (లెక్క > wanted)
		లెక్క = wanted;
	జ్ఞాపకం_నకలు(buf, file->buf + file->pos, లెక్క);
	file->pos += లెక్క;
	buf = (అక్షరం *) buf + లెక్క;

	/* zero-fill the rest of the buffer */
	wanted -= లెక్క;
	ఒకవేళ (wanted > 0) {
		memset(buf, 0, wanted);
		file->pos += wanted;
		లెక్క += wanted;
	}

బహిర్గతం:
	ఒకవేళ (got)
		*got = లెక్క;
	తిరిగి retval;
}


errcode_t ext2fs_file_read(ext2_file_t file, శూన్య *buf,
			   గుర్తులేని పూర్ణాంకం wanted, గుర్తులేని పూర్ణాంకం *got)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	గుర్తులేని పూర్ణాంకం	ప్రారంభం, c, లెక్క = 0;
	__u64		ఎడమ;
	అక్షరం		*ptr = (అక్షరం *) buf;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	/* If an inode has inline data, things get complicated. */
	ఒకవేళ (file->inode.i_flags & EXT4_INLINE_దత్తాంశం_FL)
		తిరిగి ext2fs_file_read_inline_దత్తాంశం(file, buf, wanted, got);

	అయితే ((file->pos < EXT2_I_కొలత(&file->inode)) && (wanted > 0)) {
		retval = sync_బఫరు_position(file);
		ఒకవేళ (retval)
			వెళ్ళు fail;
		retval = ఎక్కించు_బఫరు(file, 0);
		ఒకవేళ (retval)
			వెళ్ళు fail;

		ప్రారంభం = file->pos % fs->blocksize;
		c = fs->blocksize - ప్రారంభం;
		ఒకవేళ (c > wanted)
			c = wanted;
		ఎడమ = EXT2_I_కొలత(&file->inode) - file->pos ;
		ఒకవేళ (c > ఎడమ)
			c = ఎడమ;

		జ్ఞాపకం_నకలు(ptr, file->buf+ప్రారంభం, c);
		file->pos += c;
		ptr += c;
		లెక్క += c;
		wanted -= c;
	}

fail:
	ఒకవేళ (got)
		*got = లెక్క;
	తిరిగి retval;
}


స్థిర errcode_t
ext2fs_file_write_inline_దత్తాంశం(ext2_file_t file, స్థిరరాశి శూన్య *buf,
			      గుర్తులేని పూర్ణాంకం nbytes, గుర్తులేని పూర్ణాంకం *written)
{
	ext2_filsys fs;
	uint64_t old_isize = EXT2_I_కొలత(&file->inode);
	uint64_t new_isize = old_isize;
	errcode_t retval;
	size_t కొలత;

	ఒకవేళ (file->pos + nbytes > old_isize)
		new_isize = file->pos + nbytes;

	fs = file->fs;
	retval = ext2fs_inline_దత్తాంశం_get(fs, file->ino, &file->inode,
					file->buf, &కొలత);
	ఒకవేళ (retval)
		తిరిగి retval;

	/*
	 * Only try to set new inline data if it won't go past the end of
	 * @file->buf; if there's not enough space in the ondisk inode, we'll
	 * jump out to the expand code.
	 */
	ఒకవేళ (new_isize < fs->blocksize) {
		ఒకవేళ (file->pos > old_isize)
			memset(file->buf + old_isize, 0, file->pos - old_isize);

		జ్ఞాపకం_నకలు(file->buf + file->pos, buf, nbytes);

		retval = ext2fs_inline_దత్తాంశం_సమితి(fs, file->ino, &file->inode,
						file->buf, new_isize);
		ఒకవేళ (retval == EXT2_ET_INLINE_దత్తాంశం_NO_SPACE)
			వెళ్ళు expand;
		ఒకవేళ (retval)
			తిరిగి retval;

		file->pos += nbytes;

		/* Update inode size */
		ఒకవేళ (old_isize < new_isize) {
			errcode_t	rc;

			rc = ext2fs_file_సమితి_size2(file, file->pos);
			ఒకవేళ (retval == 0)
				retval = rc;
		}

		ఒకవేళ (written)
			*written = nbytes;
		తిరిగి 0;
	}

expand:
	retval = ext2fs_inline_దత్తాంశం_expand(fs, file->ino);
	ఒకవేళ (retval)
		తిరిగి retval;
	/*
	 * reload inode and return no space error
	 *
	 * XXX: file->inode could be copied from the outside
	 * in ext2fs_file_open2().  We have no way to modify
	 * the outside inode.
	 */
	retval = ext2fs_read_inode(fs, file->ino, &file->inode);
	ఒకవేళ (retval)
		తిరిగి retval;
	తిరిగి EXT2_ET_INLINE_దత్తాంశం_NO_SPACE;
}


errcode_t ext2fs_file_write(ext2_file_t file, స్థిరరాశి శూన్య *buf,
			    గుర్తులేని పూర్ణాంకం nbytes, గుర్తులేని పూర్ణాంకం *written)
{
	ext2_filsys	fs;
	errcode_t	retval = 0;
	గుర్తులేని పూర్ణాంకం	ప్రారంభం, c, లెక్క = 0;
	స్థిరరాశి అక్షరం	*ptr = (స్థిరరాశి అక్షరం *) buf;
	ఖండం_నమోదు_t	new_ఖండం = ఖాళీ, old_ఖండం = ఖాళీ;
	పూర్ణాంకం		bmap_flags = 0;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);
	fs = file->fs;

	ఒకవేళ (!(file->flags & EXT2_FILE_WRITE))
		తిరిగి EXT2_ET_FILE_RO;

	/* If an inode has inline data, things get complicated. */
	ఒకవేళ (file->inode.i_flags & EXT4_INLINE_దత్తాంశం_FL) {
		retval = ext2fs_file_write_inline_దత్తాంశం(file, buf, nbytes,
						       written);
		ఒకవేళ (retval != EXT2_ET_INLINE_దత్తాంశం_NO_SPACE)
			తిరిగి retval;
		/* fall through to read data from the block */
		retval = 0;
	}

	అయితే (nbytes > 0) {
		retval = sync_బఫరు_position(file);
		ఒకవేళ (retval)
			వెళ్ళు fail;

		ప్రారంభం = file->pos % fs->blocksize;
		c = fs->blocksize - ప్రారంభం;
		ఒకవేళ (c > nbytes)
			c = nbytes;

		/*
		 * We only need to do a read-modify-update cycle if
		 * we're doing a partial write.
		 */
		retval = ఎక్కించు_బఫరు(file, (c == fs->blocksize));
		ఒకవేళ (retval)
			వెళ్ళు fail;

		file->flags |= EXT2_FILE_BUF_DIRTY;
		జ్ఞాపకం_నకలు(file->buf+ప్రారంభం, ptr, c);

		/*
		 * OK, the physical block hasn't been allocated yet.
		 * Allocate it.
		 */
		ఒకవేళ (!file->physblock) {
			bmap_flags = (file->ino ? BMAP_ALLOC : 0);
			ఒకవేళ (fs->flags & EXT2_FLAG_SHARE_DUP) {
				new_ఖండం = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*new_ఖండం));
				ఒకవేళ (!new_ఖండం) {
					retval = EXT2_ET_NO_జ్ఞాపకం;
					వెళ్ళు fail;
				}
				ext2fs_sha512((స్థిరరాశి గుర్తులేని అక్షరం*)file->buf,
						fs->blocksize, new_ఖండం->sha);
				old_ఖండం = ext2fs_hashmap_lookup(
							fs->ఖండం_sha_map,
							new_ఖండం->sha,
							పరిమాణం(new_ఖండం->sha));
			}

			ఒకవేళ (old_ఖండం) {
				file->physblock = old_ఖండం->physblock;
				bmap_flags |= BMAP_సమితి;
				విడుదల(new_ఖండం);
				new_ఖండం = ఖాళీ;
			}

			retval = ext2fs_bmap2(fs, file->ino, &file->inode,
					      BMAP_బఫరు,
					      bmap_flags,
					      file->blockno, 0,
					      &file->physblock);
			ఒకవేళ (retval) {
				విడుదల(new_ఖండం);
				new_ఖండం = ఖాళీ;
				వెళ్ళు fail;
			}

			ఒకవేళ (new_ఖండం) {
				new_ఖండం->physblock = file->physblock;
				పూర్ణాంకం ret = ext2fs_hashmap_కూడు(fs->ఖండం_sha_map,
						new_ఖండం, new_ఖండం->sha,
						పరిమాణం(new_ఖండం->sha));
				ఒకవేళ (ret) {
					retval = EXT2_ET_NO_జ్ఞాపకం;
					విడుదల(new_ఖండం);
					వెళ్ళు fail;
				}
			}

			ఒకవేళ (bmap_flags & BMAP_సమితి) {
				ext2fs_iblk_కూడు_ఖండాలు(fs, &file->inode, 1);
				ext2fs_write_inode(fs, file->ino, &file->inode);
			}
		}

		file->pos += c;
		ptr += c;
		లెక్క += c;
		nbytes -= c;
	}

fail:
	/* Update inode size */
	ఒకవేళ (లెక్క != 0 && EXT2_I_కొలత(&file->inode) < file->pos) {
		errcode_t	rc;

		rc = ext2fs_file_సమితి_size2(file, file->pos);
		ఒకవేళ (retval == 0)
			retval = rc;
	}

	ఒకవేళ (written)
		*written = లెక్క;
	తిరిగి retval;
}

errcode_t ext2fs_file_llseek(ext2_file_t file, __u64 స్థానభ్రంశం,
			    పూర్ణాంకం whence, __u64 *ret_pos)
{
	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	ఒకవేళ (whence == EXT2_SEEK_సమితి)
		file->pos = స్థానభ్రంశం;
	లేకపోతే ఒకవేళ (whence == EXT2_SEEK_CUR)
		file->pos += స్థానభ్రంశం;
	లేకపోతే ఒకవేళ (whence == EXT2_SEEK_చివర)
		file->pos = EXT2_I_కొలత(&file->inode) + స్థానభ్రంశం;
	లేకపోతే
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (ret_pos)
		*ret_pos = file->pos;

	తిరిగి 0;
}

errcode_t ext2fs_file_lseek(ext2_file_t file, ext2_off_t స్థానభ్రంశం,
			    పూర్ణాంకం whence, ext2_off_t *ret_pos)
{
	__u64		loffset, ret_loffset = 0;
	errcode_t	retval;

	loffset = స్థానభ్రంశం;
	retval = ext2fs_file_llseek(file, loffset, whence, &ret_loffset);
	ఒకవేళ (ret_pos)
		*ret_pos = (ext2_off_t) ret_loffset;
	తిరిగి retval;
}


/*
 * This function returns the size of the file, according to the inode
 */
errcode_t ext2fs_file_get_lsize(ext2_file_t file, __u64 *ret_కొలత)
{
	ఒకవేళ (file->magic != EXT2_ET_MAGIC_EXT2_FILE)
		తిరిగి EXT2_ET_MAGIC_EXT2_FILE;
	*ret_కొలత = EXT2_I_కొలత(&file->inode);
	తిరిగి 0;
}

/*
 * This function returns the size of the file, according to the inode
 */
ext2_off_t ext2fs_file_get_కొలత(ext2_file_t file)
{
	__u64	కొలత;

	ఒకవేళ (ext2fs_file_get_lsize(file, &కొలత))
		తిరిగి 0;
	ఒకవేళ ((కొలత >> 32) != 0)
		తిరిగి 0;
	తిరిగి కొలత;
}

/* Zero the parts of the last block that are past EOF. */
స్థిర errcode_t ext2fs_file_zero_past_స్థానభ్రంశం(ext2_file_t file,
					      ext2_off64_t స్థానభ్రంశం)
{
	ext2_filsys fs = file->fs;
	అక్షరం *b = ఖాళీ;
	ext2_off64_t off = స్థానభ్రంశం % fs->blocksize;
	blk64_t blk;
	పూర్ణాంకం ret_flags;
	errcode_t retval;

	ఒకవేళ (off == 0)
		తిరిగి 0;

	retval = sync_బఫరు_position(file);
	ఒకవేళ (retval)
		తిరిగి retval;

	/* Is there an initialized block at the end? */
	retval = ext2fs_bmap2(fs, file->ino, ఖాళీ, ఖాళీ, 0,
			      స్థానభ్రంశం / fs->blocksize, &ret_flags, &blk);
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ ((blk == 0) || (ret_flags & BMAP_RET_UNINIT))
		తిరిగి 0;

	/* Zero to the end of the block */
	retval = ext2fs_get_mem(fs->blocksize, &b);
	ఒకవేళ (retval)
		తిరిగి retval;

	/* Read/zero/write block */
	retval = io_channel_read_blk64(fs->io, blk, 1, b);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;

	memset(b + off, 0, fs->blocksize - off);

	retval = io_channel_write_blk64(fs->io, blk, 1, b);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;

బహిర్గతం:
	ext2fs_free_mem(&b);
	తిరిగి retval;
}

/*
 * This function sets the size of the file, truncating it if necessary
 *
 */
errcode_t ext2fs_file_సమితి_size2(ext2_file_t file, ext2_off64_t కొలత)
{
	ext2_off64_t	old_కొలత;
	errcode_t	retval;
	blk64_t		old_truncate, truncate_ఖండం;

	EXT2_CHECK_MAGIC(file, EXT2_ET_MAGIC_EXT2_FILE);

	ఒకవేళ (కొలత && ext2fs_file_ఖండం_స్థానభ్రంశం_too_big(file->fs, &file->inode,
					(కొలత - 1) / file->fs->blocksize))
		తిరిగి EXT2_ET_FILE_TOO_BIG;
	truncate_ఖండం = ((కొలత + file->fs->blocksize - 1) >>
			  EXT2_ఖండం_కొలత_బిట్లు(file->fs->super));
	old_కొలత = EXT2_I_కొలత(&file->inode);
	old_truncate = ((old_కొలత + file->fs->blocksize - 1) >>
		      EXT2_ఖండం_కొలత_బిట్లు(file->fs->super));

	retval = ext2fs_inode_కొలత_సమితి(file->fs, &file->inode, కొలత);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (file->ino) {
		retval = ext2fs_write_inode(file->fs, file->ino, &file->inode);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	retval = ext2fs_file_zero_past_స్థానభ్రంశం(file, కొలత);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (truncate_ఖండం >= old_truncate)
		తిరిగి 0;

	తిరిగి ext2fs_punch(file->fs, file->ino, &file->inode, 0,
			    truncate_ఖండం, ~0ULL);
}

errcode_t ext2fs_file_సమితి_కొలత(ext2_file_t file, ext2_off_t కొలత)
{
	తిరిగి ext2fs_file_సమితి_size2(file, కొలత);
}
