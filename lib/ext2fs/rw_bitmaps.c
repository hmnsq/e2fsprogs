/*
 * rw_bitmaps.c --- routines to read and write the  inode and block bitmaps.
 *
 * Copyright (C) 1993, 1994, 1994, 1996 Theodore Ts'o.
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
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "e2image.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
ਕਿਸਮਪਰਿਭਾਸ਼ਾ pthread_mutex_t mutex_t;

ਸਥਿਰ ਖਾਲੀ unix_pthread_mutex_ਤਾਲਾ(mutex_t *mutex)
{
	ਜੇ (mutex)
		pthread_mutex_ਤਾਲਾ(mutex);
}
ਸਥਿਰ ਖਾਲੀ unix_pthread_mutex_unlock(mutex_t *mutex)
{
	ਜੇ (mutex)
		pthread_mutex_unlock(mutex);
}
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ mutex_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ unix_pthread_mutex_ਤਾਲਾ(mutex_t) ਕਰੋ {} ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ unix_pthread_mutex_unlock(mutex_t) ਕਰੋ {} ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

ਸਥਿਰ errcode_t write_bitmaps(ext2_filsys fs, ਪੂਰਨ do_inode, ਪੂਰਨ do_ਖੰਡ)
{
	dgrp_t 		i;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	j;
	ਪੂਰਨ		ਖੰਡ_nbytes, inode_nbytes;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	nbits;
	errcode_t	retval;
	ਅੱਖਰ		*ਖੰਡ_buf = ਨਿਰਾ, *inode_buf = ਨਿਰਾ;
	ਪੂਰਨ		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!(fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_ਸਮੂਹ_desc_csum(fs);

	inode_nbytes = ਖੰਡ_nbytes = 0;
	ਜੇ (do_ਖੰਡ) {
		ਖੰਡ_nbytes = EXT2_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &ਖੰਡ_buf);
		ਜੇ (retval)
			ਜਾਓ errout;
		memset(ਖੰਡ_buf, 0xff, fs->blocksize);
	}
	ਜੇ (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		ਜੇ (retval)
			ਜਾਓ errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	ਲਈ (i = 0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		ਜੇ (!do_ਖੰਡ)
			ਜਾਓ skip_ਖੰਡ_bitmap;

		ਜੇ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_ਖੰਡ_UNINIT)
		    )
			ਜਾਓ skip_this_ਖੰਡ_bitmap;

		retval = ext2fs_get_ਖੰਡ_bitmap_range2(fs->ਖੰਡ_map,
				blk_itr, ਖੰਡ_nbytes << 3, ਖੰਡ_buf);
		ਜੇ (retval)
			ਜਾਓ errout;

		ਜੇ (i == fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_ਅੰਕ_B2_C(fs,
				((ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)
				  - (__u64) fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ)
				 % (__u64) EXT2_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super)));
			ਜੇ (nbits)
				ਲਈ (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(j, ਖੰਡ_buf);
		}

		retval = ext2fs_ਖੰਡ_bitmap_csum_ਸੈੱਟ(fs, i, ਖੰਡ_buf,
						      ਖੰਡ_nbytes);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_ਖੰਡ_bitmap_loc(fs, i);
		ਜੇ (blk && blk < ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							ਖੰਡ_buf);
			ਜੇ (retval) {
				retval = EXT2_ET_ਖੰਡ_BITMAP_WRITE;
				ਜਾਓ errout;
			}
		}
	skip_this_ਖੰਡ_bitmap:
		blk_itr += ਖੰਡ_nbytes << 3;
	skip_ਖੰਡ_bitmap:

		ਜੇ (!do_inode)
			ਜਾਰੀ;

		ਜੇ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			ਜਾਓ skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		ਜੇ (retval)
			ਜਾਓ errout;

		retval = ext2fs_inode_bitmap_csum_ਸੈੱਟ(fs, i, inode_buf,
						      inode_nbytes);
		ਜੇ (retval)
			ਜਾਓ errout;
		ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		ਜੇ (blk && blk < ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			ਜੇ (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				ਜਾਓ errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	ਜੇ (do_ਖੰਡ) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&ਖੰਡ_buf);
	}
	ਜੇ (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	ਵਾਪਸ 0;
errout:
	ਜੇ (inode_buf)
		ext2fs_free_mem(&inode_buf);
	ਜੇ (ਖੰਡ_buf)
		ext2fs_free_mem(&ਖੰਡ_buf);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t mark_uninit_bg_ਸਮੂਹ_ਖੰਡਾਂ(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_ਖੰਡ_bitmap	bmap = fs->ਖੰਡ_map;

	ਲਈ (i = 0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		ਜੇ (!ext2fs_bg_flags_test(fs, i, EXT2_BG_ਖੰਡ_UNINIT))
			ਜਾਰੀ;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_ਸਾਰਣੀ_loc(fs, i);
		ਜੇ (blk)
			ext2fs_mark_ਖੰਡ_bitmap_range2(bmap, blk,
						fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_ਖੰਡ_bitmap_loc(fs, i);
		ਜੇ (blk && blk < ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))
			ext2fs_mark_ਖੰਡ_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ਜੇ (blk && blk < ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))
			ext2fs_mark_ਖੰਡ_bitmap2(bmap, blk);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ bitmap_ਪੂਛ_verify(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *bitmap, ਪੂਰਨ ਪਹਿਲਾ, ਪੂਰਨ ਆਖਰੀ)
{
	ਪੂਰਨ i;

	ਲਈ (i = ਪਹਿਲਾ; i <= ਆਖਰੀ; i++)
		ਜੇ (bitmap[i] != 0xff)
			ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

ਸਥਿਰ errcode_t read_bitmaps_range_prepare(ext2_filsys fs, ਪੂਰਨ flags)
{
	errcode_t retval;
	ਪੂਰਨ ਖੰਡ_nbytes = EXT2_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) / 8;
	ਪੂਰਨ inode_nbytes = EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) / 8;
	ਅੱਖਰ *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ ((ਖੰਡ_nbytes > (ਪੂਰਨ) fs->blocksize) ||
	    (inode_nbytes > (ਪੂਰਨ) fs->blocksize))
		ਵਾਪਸ EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(fs->device_name) + 80, &buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (flags & EXT2_FS_BITMAPS_ਖੰਡ) {
		ਜੇ (fs->ਖੰਡ_map)
			ext2fs_free_ਖੰਡ_bitmap(fs->ਖੰਡ_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_ਖੰਡ_bitmap(fs, buf, &fs->ਖੰਡ_map);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}

	ਜੇ (flags & EXT2FS_BITMAPS_INODE) {
		ਜੇ (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}
	ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;

cleanup:
	ਜੇ (flags & EXT2_FS_BITMAPS_ਖੰਡ) {
		ext2fs_free_ਖੰਡ_bitmap(fs->ਖੰਡ_map);
		fs->ਖੰਡ_map = 0;
	}
	ਜੇ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t read_bitmaps_range_ਸ਼ੁਰੂ(ext2_filsys fs, ਪੂਰਨ flags,
					  dgrp_t ਸ਼ੁਰੂ, dgrp_t ਸਿਰਾ,
					  mutex_t *mutex,
					  ਪੂਰਨ *ਪੂਛ_flags)
{
	dgrp_t i;
	ਅੱਖਰ *ਖੰਡ_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	ਪੂਰਨ ਖੰਡ_nbytes = EXT2_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) / 8;
	ਪੂਰਨ inode_nbytes = EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) / 8;
	ਪੂਰਨ csum_flag;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_ਸਮੂਹ_desc_csum(fs);

	ਜੇ (flags & EXT2_FS_BITMAPS_ਖੰਡ) {
		retval = io_channel_alloc_buf(fs->io, 0, &ਖੰਡ_bitmap);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	} ਨਹੀਂਤਾਂ {
		ਖੰਡ_nbytes = 0;
	}

	ਜੇ (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	} ਨਹੀਂਤਾਂ {
		inode_nbytes = 0;
	}

	/* io should be null */
	ਜੇ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(fs->image_ਸਿਰਲੇਖ->ਵਿੱਥ_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_ਗਿਣਤੀ;
		ਜਦੋਂ (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			ਜੇ (retval)
				ਜਾਓ cleanup;
			cnt = fs->blocksize << 3;
			ਜੇ (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_ਸੈੱਟ_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			ਜੇ (retval)
				ਜਾਓ cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(fs->image_ਸਿਰਲੇਖ->ਵਿੱਥ_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_ਮੰਜ਼ਿਲ_CLUSTERS(fs->super,
						  fs->ਸਮੂਹ_desc_ਗਿਣਤੀ);
		ਜਦੋਂ (ਖੰਡ_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, ਖੰਡ_bitmap);
			ਜੇ (retval)
				ਜਾਓ cleanup;
			cnt = fs->blocksize << 3;
			ਜੇ (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_ਸੈੱਟ_ਖੰਡ_bitmap_range2(fs->ਖੰਡ_map,
				       blk_itr, cnt, ਖੰਡ_bitmap);
			ਜੇ (retval)
				ਜਾਓ cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		ਜਾਓ cleanup;
	}

	blk_itr += ((blk64_t)ਸ਼ੁਰੂ * (ਖੰਡ_nbytes << 3));
	ino_itr += ((blk64_t)ਸ਼ੁਰੂ * (inode_nbytes << 3));
	ਲਈ (i = ਸ਼ੁਰੂ; i <= ਸਿਰਾ; i++) {
		ਜੇ (ਖੰਡ_bitmap) {
			blk = ext2fs_ਖੰਡ_bitmap_loc(fs, i);
			ਜੇ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_ਖੰਡ_UNINIT) &&
			     ext2fs_ਸਮੂਹ_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)))
				blk = 0;
			ਜੇ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, ਖੰਡ_bitmap);
				ਜੇ (retval) {
					retval = EXT2_ET_ਖੰਡ_BITMAP_READ;
					ਜਾਓ cleanup;
				}
				/* verify block bitmap checksum */
				ਜੇ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_ਖੰਡ_bitmap_csum_verify(fs, i,
						ਖੰਡ_bitmap, ਖੰਡ_nbytes)) {
					retval =
					EXT2_ET_ਖੰਡ_BITMAP_CSUM_INVALID;
					ਜਾਓ cleanup;
				}
				ਜੇ (!bitmap_ਪੂਛ_verify((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) ਖੰਡ_bitmap,
							ਖੰਡ_nbytes, fs->blocksize - 1))
					*ਪੂਛ_flags |= EXT2_FLAG_BBITMAP_ਪੂਛ_PROBLEM;
			} ਨਹੀਂਤਾਂ
				memset(ਖੰਡ_bitmap, 0, ਖੰਡ_nbytes);
			cnt = ਖੰਡ_nbytes << 3;
			unix_pthread_mutex_ਤਾਲਾ(mutex);
			retval = ext2fs_ਸੈੱਟ_ਖੰਡ_bitmap_range2(fs->ਖੰਡ_map,
					       blk_itr, cnt, ਖੰਡ_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ਜੇ (retval)
				ਜਾਓ cleanup;
			blk_itr += ਖੰਡ_nbytes << 3;
		}
		ਜੇ (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			ਜੇ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_ਸਮੂਹ_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)))
				blk = 0;
			ਜੇ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				ਜੇ (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					ਜਾਓ cleanup;
				}

				/* verify inode bitmap checksum */
				ਜੇ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					ਜਾਓ cleanup;
				}
				ਜੇ (!bitmap_ਪੂਛ_verify((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*ਪੂਛ_flags |= EXT2_FLAG_IBITMAP_ਪੂਛ_PROBLEM;
			} ਨਹੀਂਤਾਂ
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_ਤਾਲਾ(mutex);
			retval = ext2fs_ਸੈੱਟ_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ਜੇ (retval)
				ਜਾਓ cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	ਜੇ (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	ਜੇ (ਖੰਡ_bitmap)
		ext2fs_free_mem(&ਖੰਡ_bitmap);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t read_bitmaps_range_ਸਿਰਾ(ext2_filsys fs, ਪੂਰਨ flags,
					ਪੂਰਨ ਪੂਛ_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	ਜੇ (flags & EXT2_FS_BITMAPS_ਖੰਡ) {
		retval = mark_uninit_bg_ਸਮੂਹ_ਖੰਡਾਂ(fs);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_ਪੂਛ_PROBLEM;
	}
	ਜੇ (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_ਪੂਛ_PROBLEM;
	fs->flags |= ਪੂਛ_flags;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ read_bitmaps_cleanup_on_error(ext2_filsys fs, ਪੂਰਨ flags)
{
	ਜੇ (flags & EXT2_FS_BITMAPS_ਖੰਡ) {
		ext2fs_free_ਖੰਡ_bitmap(fs->ਖੰਡ_map);
		fs->ਖੰਡ_map = 0;
	}
	ਜੇ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ਸਥਿਰ errcode_t read_bitmaps_range(ext2_filsys fs, ਪੂਰਨ flags,
				    dgrp_t ਸ਼ੁਰੂ, dgrp_t ਸਿਰਾ)
{
	errcode_t retval;
	ਪੂਰਨ ਪੂਛ_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = read_bitmaps_range_ਸ਼ੁਰੂ(fs, flags, ਸ਼ੁਰੂ, ਸਿਰਾ,
					  ਨਿਰਾ, &ਪੂਛ_flags);
	ਜੇ (retval == 0)
		retval = read_bitmaps_range_ਸਿਰਾ(fs, flags, ਪੂਛ_flags);
	ਜੇ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	ਵਾਪਸ retval;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
ਬਣਤਰ read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	ਪੂਰਨ		rbt_flags;
	dgrp_t		rbt_grp_ਸ਼ੁਰੂ;
	dgrp_t		rbt_grp_ਸਿਰਾ;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	ਪੂਰਨ		rbt_ਪੂਛ_flags;
};

ਸਥਿਰ ਖਾਲੀ *read_bitmaps_thread(ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ read_bitmaps_thread_info *rbt = ਅੰਕੜੇ;

	rbt->rbt_retval = read_bitmaps_range_ਸ਼ੁਰੂ(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_ਸ਼ੁਰੂ, rbt->rbt_grp_ਸਿਰਾ,
				rbt->rbt_mutex, &rbt->rbt_ਪੂਛ_flags);
	ਵਾਪਸ ਨਿਰਾ;
}
#ਜੇ_ਅੰਤ

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, ਪੂਰਨ flags, ਪੂਰਨ ਅੰਕ_ਧਾਗੇ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = ਨਿਰਾ;
	ਬਣਤਰ read_bitmaps_thread_info *thread_infos = ਨਿਰਾ;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	ਬਿਨਾਨਿਸ਼ਾਨੀ flexbg_ਮਾਪ = 1U << fs->super->s_log_groups_ਪ੍ਰਤੀ_flex;
	dgrp_t ਔਸਤਨ_ਸਮੂਹ;
	ਪੂਰਨ i, ਪੂਛ_flags = 0;
#ਜੇ_ਅੰਤ

	ਜੇ (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (ext2fs_has_feature_journal_dev(fs->super))
		ਵਾਪਸ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ਜੇ (flags & EXT2FS_BITMAPS_WRITE)
		ਵਾਪਸ write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2_FS_BITMAPS_ਖੰਡ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PTHREAD
	ਜੇ (((fs->io->flags & CHANNEL_FLAGS_ਧਾਗੇ) == 0) ||
	    (ਅੰਕ_ਧਾਗੇ == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		ਜਾਓ fallback;

#ਜੇ defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	ਜੇ (ਅੰਕ_ਧਾਗੇ < 0)
		ਅੰਕ_ਧਾਗੇ = sysconf(_SC_NPROCESSORS_CONF);
#ਜੇ_ਅੰਤ
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	ਜੇ (ਅੰਕ_ਧਾਗੇ <= 0)
		ਅੰਕ_ਧਾਗੇ = 4;

	ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕ_ਧਾਗੇ > fs->ਸਮੂਹ_desc_ਗਿਣਤੀ)
		ਅੰਕ_ਧਾਗੇ = fs->ਸਮੂਹ_desc_ਗਿਣਤੀ;
	ਔਸਤਨ_ਸਮੂਹ = fs->ਸਮੂਹ_desc_ਗਿਣਤੀ / ਅੰਕ_ਧਾਗੇ;
	ਜੇ (ext2fs_has_feature_flex_bg(fs->super)) {
		ਔਸਤਨ_ਸਮੂਹ = (ਔਸਤਨ_ਸਮੂਹ / flexbg_ਮਾਪ) * flexbg_ਮਾਪ;
	}
	ਜੇ ((ਅੰਕ_ਧਾਗੇ <= 1) || (ਔਸਤਨ_ਸਮੂਹ == 0))
		ਜਾਓ fallback;

	io_channel_ਸੈੱਟ_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	thread_ids = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(pthread_t), ਅੰਕ_ਧਾਗੇ);
	ਜੇ (!thread_ids)
		ਵਾਪਸ ENOMEM;

	thread_infos = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(ਬਣਤਰ read_bitmaps_thread_info),
				ਅੰਕ_ਧਾਗੇ);
	ਜੇ (!thread_infos)
		ਜਾਓ ਬਾਹਰਲਾ;

	retval = read_bitmaps_range_prepare(fs, flags);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	ਲਈ (i = 0; i < ਅੰਕ_ਧਾਗੇ; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_ਪੂਛ_flags = 0;
		ਜੇ (i == 0)
			thread_infos[i].rbt_grp_ਸ਼ੁਰੂ = 0;
		ਨਹੀਂਤਾਂ
			thread_infos[i].rbt_grp_ਸ਼ੁਰੂ = ਔਸਤਨ_ਸਮੂਹ * i + 1;

		ਜੇ (i == ਅੰਕ_ਧਾਗੇ - 1)
			thread_infos[i].rbt_grp_ਸਿਰਾ = fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1;
		ਨਹੀਂਤਾਂ
			thread_infos[i].rbt_grp_ਸਿਰਾ = ਔਸਤਨ_ਸਮੂਹ * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		ਜੇ (retval)
			ਰੋਕੋ;
	}
	ਲਈ (i = 0; i < ਅੰਕ_ਧਾਗੇ; i++) {
		ਜੇ (!thread_ids[i])
			ਰੋਕੋ;
		rc = pthread_join(thread_ids[i], ਨਿਰਾ);
		ਜੇ (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		ਜੇ (rc && !retval)
			retval = rc;
		ਪੂਛ_flags |= thread_infos[i].rbt_ਪੂਛ_flags;
	}
ਬਾਹਰਲਾ:
	rc = pthread_attr_destroy(&attr);
	ਜੇ (rc && !retval)
		retval = rc;
	ਮੁਕਤ(thread_infos);
	ਮੁਕਤ(thread_ids);

	ਜੇ (retval == 0)
		retval = read_bitmaps_range_ਸਿਰਾ(fs, flags, ਪੂਛ_flags);
	ਜੇ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_ਸੈੱਟ_options(fs->io, "cache=on");
	ਵਾਪਸ retval;
fallback:
#ਜੇ_ਅੰਤ /* HAVE_PTHREAD */
	ਵਾਪਸ read_bitmaps_range(fs, flags, 0, fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	ਵਾਪਸ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_ਖੰਡ_bitmap(ext2_filsys fs)
{
	ਵਾਪਸ ext2fs_rw_bitmaps(fs, EXT2_FS_BITMAPS_ਖੰਡ, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	ਵਾਪਸ write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_ਖੰਡ_bitmap (ext2_filsys fs)
{
	ਵਾਪਸ write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	ਪੂਰਨ flags = 0;

	ਜੇ (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	ਜੇ (!fs->ਖੰਡ_map)
		flags |= EXT2_FS_BITMAPS_ਖੰਡ;
	ਜੇ (flags == 0)
		ਵਾਪਸ 0;
	ਵਾਪਸ ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	ਪੂਰਨ do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	ਪੂਰਨ do_ਖੰਡ = fs->ਖੰਡ_map && ext2fs_test_bb_dirty(fs);

	ਜੇ (!do_inode && !do_ਖੰਡ)
		ਵਾਪਸ 0;

	ਵਾਪਸ write_bitmaps(fs, do_inode, do_ਖੰਡ);
}
