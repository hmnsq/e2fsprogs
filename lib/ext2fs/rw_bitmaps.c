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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD_H
#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "e2image.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ pthread_mutex_t mutex_t;

ಸ್ಥಿರ ಶೂನ್ಯ unix_pthread_mutex_ಬೀಗ(mutex_t *mutex)
{
	ಆದರೆ (mutex)
		pthread_mutex_ಬೀಗ(mutex);
}
ಸ್ಥಿರ ಶೂನ್ಯ unix_pthread_mutex_unlock(mutex_t *mutex)
{
	ಆದರೆ (mutex)
		pthread_mutex_unlock(mutex);
}
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ mutex_t;
#ವ್ಯಾಖ್ಯಾನಿಸು unix_pthread_mutex_ಬೀಗ(mutex_t) ಮಾಡು {} ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು unix_pthread_mutex_unlock(mutex_t) ಮಾಡು {} ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ errcode_t write_bitmaps(ext2_filsys fs, ಪೂರ್ಣಾಂಕ do_inode, ಪೂರ್ಣಾಂಕ do_ಖಂಡ)
{
	dgrp_t 		i;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	j;
	ಪೂರ್ಣಾಂಕ		ಖಂಡ_nbytes, inode_nbytes;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	nbits;
	errcode_t	retval;
	ಅಕ್ಷರ		*ಖಂಡ_buf = ಖಾಲಿ, *inode_buf = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!(fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_ಗುಂಪು_desc_csum(fs);

	inode_nbytes = ಖಂಡ_nbytes = 0;
	ಆದರೆ (do_ಖಂಡ) {
		ಖಂಡ_nbytes = EXT2_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &ಖಂಡ_buf);
		ಆದರೆ (retval)
			ಹೋಗು errout;
		memset(ಖಂಡ_buf, 0xff, fs->blocksize);
	}
	ಆದರೆ (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		ಆದರೆ (retval)
			ಹೋಗು errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	ಗಾಗಿ (i = 0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		ಆದರೆ (!do_ಖಂಡ)
			ಹೋಗು skip_ಖಂಡ_bitmap;

		ಆದರೆ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_ಖಂಡ_UNINIT)
		    )
			ಹೋಗು skip_this_ಖಂಡ_bitmap;

		retval = ext2fs_get_ಖಂಡ_bitmap_range2(fs->ಖಂಡ_map,
				blk_itr, ಖಂಡ_nbytes << 3, ಖಂಡ_buf);
		ಆದರೆ (retval)
			ಹೋಗು errout;

		ಆದರೆ (i == fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_ಅಂಕೆ_B2_C(fs,
				((ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)
				  - (__u64) fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ)
				 % (__u64) EXT2_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(fs->super)));
			ಆದರೆ (nbits)
				ಗಾಗಿ (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_ಗಣ_ಬಿಟ್(j, ಖಂಡ_buf);
		}

		retval = ext2fs_ಖಂಡ_bitmap_csum_ಗಣ(fs, i, ಖಂಡ_buf,
						      ಖಂಡ_nbytes);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_ಖಂಡ_bitmap_loc(fs, i);
		ಆದರೆ (blk && blk < ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							ಖಂಡ_buf);
			ಆದರೆ (retval) {
				retval = EXT2_ET_ಖಂಡ_BITMAP_WRITE;
				ಹೋಗು errout;
			}
		}
	skip_this_ಖಂಡ_bitmap:
		blk_itr += ಖಂಡ_nbytes << 3;
	skip_ಖಂಡ_bitmap:

		ಆದರೆ (!do_inode)
			ಮುಂದುವರಿ;

		ಆದರೆ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			ಹೋಗು skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		ಆದರೆ (retval)
			ಹೋಗು errout;

		retval = ext2fs_inode_bitmap_csum_ಗಣ(fs, i, inode_buf,
						      inode_nbytes);
		ಆದರೆ (retval)
			ಹೋಗು errout;
		ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		ಆದರೆ (blk && blk < ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			ಆದರೆ (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				ಹೋಗು errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	ಆದರೆ (do_ಖಂಡ) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&ಖಂಡ_buf);
	}
	ಆದರೆ (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	ಹಿಂತಿರುಗಿ 0;
errout:
	ಆದರೆ (inode_buf)
		ext2fs_free_mem(&inode_buf);
	ಆದರೆ (ಖಂಡ_buf)
		ext2fs_free_mem(&ಖಂಡ_buf);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t mark_uninit_bg_ಗುಂಪು_ಖಂಡಗಳು(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_ಖಂಡ_bitmap	bmap = fs->ಖಂಡ_map;

	ಗಾಗಿ (i = 0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		ಆದರೆ (!ext2fs_bg_flags_test(fs, i, EXT2_BG_ಖಂಡ_UNINIT))
			ಮುಂದುವರಿ;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_ಕೋಷ್ಟಕ_loc(fs, i);
		ಆದರೆ (blk)
			ext2fs_mark_ಖಂಡ_bitmap_range2(bmap, blk,
						fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_ಖಂಡ_bitmap_loc(fs, i);
		ಆದರೆ (blk && blk < ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))
			ext2fs_mark_ಖಂಡ_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ಆದರೆ (blk && blk < ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))
			ext2fs_mark_ಖಂಡ_bitmap2(bmap, blk);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bitmap_ಬಾಲ_verify(ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *bitmap, ಪೂರ್ಣಾಂಕ ಮೊದಲ, ಪೂರ್ಣಾಂಕ ಕೊನೆಯ)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = ಮೊದಲ; i <= ಕೊನೆಯ; i++)
		ಆದರೆ (bitmap[i] != 0xff)
			ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ errcode_t read_bitmaps_range_prepare(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags)
{
	errcode_t retval;
	ಪೂರ್ಣಾಂಕ ಖಂಡ_nbytes = EXT2_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(fs->super) / 8;
	ಪೂರ್ಣಾಂಕ inode_nbytes = EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) / 8;
	ಅಕ್ಷರ *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ ((ಖಂಡ_nbytes > (ಪೂರ್ಣಾಂಕ) fs->blocksize) ||
	    (inode_nbytes > (ಪೂರ್ಣಾಂಕ) fs->blocksize))
		ಹಿಂತಿರುಗಿ EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(fs->device_name) + 80, &buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (flags & EXT2_FS_BITMAPS_ಖಂಡ) {
		ಆದರೆ (fs->ಖಂಡ_map)
			ext2fs_free_ಖಂಡ_bitmap(fs->ಖಂಡ_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_ಖಂಡ_bitmap(fs, buf, &fs->ಖಂಡ_map);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}

	ಆದರೆ (flags & EXT2FS_BITMAPS_INODE) {
		ಆದರೆ (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}
	ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;

cleanup:
	ಆದರೆ (flags & EXT2_FS_BITMAPS_ಖಂಡ) {
		ext2fs_free_ಖಂಡ_bitmap(fs->ಖಂಡ_map);
		fs->ಖಂಡ_map = 0;
	}
	ಆದರೆ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t read_bitmaps_range_ಆರಂಭ(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags,
					  dgrp_t ಆರಂಭ, dgrp_t ಕೊನೆ,
					  mutex_t *mutex,
					  ಪೂರ್ಣಾಂಕ *ಬಾಲ_flags)
{
	dgrp_t i;
	ಅಕ್ಷರ *ಖಂಡ_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	ಪೂರ್ಣಾಂಕ ಖಂಡ_nbytes = EXT2_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(fs->super) / 8;
	ಪೂರ್ಣಾಂಕ inode_nbytes = EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) / 8;
	ಪೂರ್ಣಾಂಕ csum_flag;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_ಗುಂಪು_desc_csum(fs);

	ಆದರೆ (flags & EXT2_FS_BITMAPS_ಖಂಡ) {
		retval = io_channel_alloc_buf(fs->io, 0, &ಖಂಡ_bitmap);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಖಂಡ_nbytes = 0;
	}

	ಆದರೆ (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	} ಇಲ್ಲದಿದ್ದರೆ {
		inode_nbytes = 0;
	}

	/* io should be null */
	ಆದರೆ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_ಗಮ್ಯ_cpu(fs->image_ಶೀರ್ಷಿಕೆ->ಸ್ಥಾನಾಂತರ_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_ಎಣಿಕೆ;
		ಆಗ (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			ಆದರೆ (retval)
				ಹೋಗು cleanup;
			cnt = fs->blocksize << 3;
			ಆದರೆ (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_ಗಣ_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			ಆದರೆ (retval)
				ಹೋಗು cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_ಗಮ್ಯ_cpu(fs->image_ಶೀರ್ಷಿಕೆ->ಸ್ಥಾನಾಂತರ_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_ಗಮ್ಯ_CLUSTERS(fs->super,
						  fs->ಗುಂಪು_desc_ಎಣಿಕೆ);
		ಆಗ (ಖಂಡ_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, ಖಂಡ_bitmap);
			ಆದರೆ (retval)
				ಹೋಗು cleanup;
			cnt = fs->blocksize << 3;
			ಆದರೆ (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_ಗಣ_ಖಂಡ_bitmap_range2(fs->ಖಂಡ_map,
				       blk_itr, cnt, ಖಂಡ_bitmap);
			ಆದರೆ (retval)
				ಹೋಗು cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		ಹೋಗು cleanup;
	}

	blk_itr += ((blk64_t)ಆರಂಭ * (ಖಂಡ_nbytes << 3));
	ino_itr += ((blk64_t)ಆರಂಭ * (inode_nbytes << 3));
	ಗಾಗಿ (i = ಆರಂಭ; i <= ಕೊನೆ; i++) {
		ಆದರೆ (ಖಂಡ_bitmap) {
			blk = ext2fs_ಖಂಡ_bitmap_loc(fs, i);
			ಆದರೆ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_ಖಂಡ_UNINIT) &&
			     ext2fs_ಗುಂಪು_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)))
				blk = 0;
			ಆದರೆ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, ಖಂಡ_bitmap);
				ಆದರೆ (retval) {
					retval = EXT2_ET_ಖಂಡ_BITMAP_READ;
					ಹೋಗು cleanup;
				}
				/* verify block bitmap checksum */
				ಆದರೆ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_ಖಂಡ_bitmap_csum_verify(fs, i,
						ಖಂಡ_bitmap, ಖಂಡ_nbytes)) {
					retval =
					EXT2_ET_ಖಂಡ_BITMAP_CSUM_INVALID;
					ಹೋಗು cleanup;
				}
				ಆದರೆ (!bitmap_ಬಾಲ_verify((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) ಖಂಡ_bitmap,
							ಖಂಡ_nbytes, fs->blocksize - 1))
					*ಬಾಲ_flags |= EXT2_FLAG_BBITMAP_ಬಾಲ_PROBLEM;
			} ಇಲ್ಲದಿದ್ದರೆ
				memset(ಖಂಡ_bitmap, 0, ಖಂಡ_nbytes);
			cnt = ಖಂಡ_nbytes << 3;
			unix_pthread_mutex_ಬೀಗ(mutex);
			retval = ext2fs_ಗಣ_ಖಂಡ_bitmap_range2(fs->ಖಂಡ_map,
					       blk_itr, cnt, ಖಂಡ_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ಆದರೆ (retval)
				ಹೋಗು cleanup;
			blk_itr += ಖಂಡ_nbytes << 3;
		}
		ಆದರೆ (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			ಆದರೆ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_ಗುಂಪು_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)))
				blk = 0;
			ಆದರೆ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				ಆದರೆ (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					ಹೋಗು cleanup;
				}

				/* verify inode bitmap checksum */
				ಆದರೆ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					ಹೋಗು cleanup;
				}
				ಆದರೆ (!bitmap_ಬಾಲ_verify((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*ಬಾಲ_flags |= EXT2_FLAG_IBITMAP_ಬಾಲ_PROBLEM;
			} ಇಲ್ಲದಿದ್ದರೆ
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_ಬೀಗ(mutex);
			retval = ext2fs_ಗಣ_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ಆದರೆ (retval)
				ಹೋಗು cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	ಆದರೆ (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	ಆದರೆ (ಖಂಡ_bitmap)
		ext2fs_free_mem(&ಖಂಡ_bitmap);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t read_bitmaps_range_ಕೊನೆ(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags,
					ಪೂರ್ಣಾಂಕ ಬಾಲ_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	ಆದರೆ (flags & EXT2_FS_BITMAPS_ಖಂಡ) {
		retval = mark_uninit_bg_ಗುಂಪು_ಖಂಡಗಳು(fs);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_ಬಾಲ_PROBLEM;
	}
	ಆದರೆ (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_ಬಾಲ_PROBLEM;
	fs->flags |= ಬಾಲ_flags;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_bitmaps_cleanup_on_error(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (flags & EXT2_FS_BITMAPS_ಖಂಡ) {
		ext2fs_free_ಖಂಡ_bitmap(fs->ಖಂಡ_map);
		fs->ಖಂಡ_map = 0;
	}
	ಆದರೆ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ಸ್ಥಿರ errcode_t read_bitmaps_range(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags,
				    dgrp_t ಆರಂಭ, dgrp_t ಕೊನೆ)
{
	errcode_t retval;
	ಪೂರ್ಣಾಂಕ ಬಾಲ_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = read_bitmaps_range_ಆರಂಭ(fs, flags, ಆರಂಭ, ಕೊನೆ,
					  ಖಾಲಿ, &ಬಾಲ_flags);
	ಆದರೆ (retval == 0)
		retval = read_bitmaps_range_ಕೊನೆ(fs, flags, ಬಾಲ_flags);
	ಆದರೆ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	ಹಿಂತಿರುಗಿ retval;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
ರಚನೆ read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	ಪೂರ್ಣಾಂಕ		rbt_flags;
	dgrp_t		rbt_grp_ಆರಂಭ;
	dgrp_t		rbt_grp_ಕೊನೆ;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	ಪೂರ್ಣಾಂಕ		rbt_ಬಾಲ_flags;
};

ಸ್ಥಿರ ಶೂನ್ಯ *read_bitmaps_thread(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ read_bitmaps_thread_info *rbt = ದತ್ತಾಂಶ;

	rbt->rbt_retval = read_bitmaps_range_ಆರಂಭ(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_ಆರಂಭ, rbt->rbt_grp_ಕೊನೆ,
				rbt->rbt_mutex, &rbt->rbt_ಬಾಲ_flags);
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}
#ಆದರೆ_ಅಂತ್ಯ

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಎಳೆಗಳು)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = ಖಾಲಿ;
	ರಚನೆ read_bitmaps_thread_info *thread_infos = ಖಾಲಿ;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	ಚಿಹ್ನೆರಹಿತ flexbg_ಗಾತ್ರ = 1U << fs->super->s_log_groups_ಪ್ರತಿ_flex;
	dgrp_t ಸರಾಸರಿ_ಮೌಲ್ಯ_ಗುಂಪು;
	ಪೂರ್ಣಾಂಕ i, ಬಾಲ_flags = 0;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ಆದರೆ (flags & EXT2FS_BITMAPS_WRITE)
		ಹಿಂತಿರುಗಿ write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2_FS_BITMAPS_ಖಂಡ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PTHREAD
	ಆದರೆ (((fs->io->flags & CHANNEL_FLAGS_ಎಳೆಗಳು) == 0) ||
	    (ಅಂಕೆ_ಎಳೆಗಳು == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		ಹೋಗು fallback;

#ಆದರೆ defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು < 0)
		ಅಂಕೆ_ಎಳೆಗಳು = sysconf(_SC_NPROCESSORS_CONF);
#ಆದರೆ_ಅಂತ್ಯ
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	ಆದರೆ (ಅಂಕೆ_ಎಳೆಗಳು <= 0)
		ಅಂಕೆ_ಎಳೆಗಳು = 4;

	ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ) ಅಂಕೆ_ಎಳೆಗಳು > fs->ಗುಂಪು_desc_ಎಣಿಕೆ)
		ಅಂಕೆ_ಎಳೆಗಳು = fs->ಗುಂಪು_desc_ಎಣಿಕೆ;
	ಸರಾಸರಿ_ಮೌಲ್ಯ_ಗುಂಪು = fs->ಗುಂಪು_desc_ಎಣಿಕೆ / ಅಂಕೆ_ಎಳೆಗಳು;
	ಆದರೆ (ext2fs_has_feature_flex_bg(fs->super)) {
		ಸರಾಸರಿ_ಮೌಲ್ಯ_ಗುಂಪು = (ಸರಾಸರಿ_ಮೌಲ್ಯ_ಗುಂಪು / flexbg_ಗಾತ್ರ) * flexbg_ಗಾತ್ರ;
	}
	ಆದರೆ ((ಅಂಕೆ_ಎಳೆಗಳು <= 1) || (ಸರಾಸರಿ_ಮೌಲ್ಯ_ಗುಂಪು == 0))
		ಹೋಗು fallback;

	io_channel_ಗಣ_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	thread_ids = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(pthread_t), ಅಂಕೆ_ಎಳೆಗಳು);
	ಆದರೆ (!thread_ids)
		ಹಿಂತಿರುಗಿ ENOMEM;

	thread_infos = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(ರಚನೆ read_bitmaps_thread_info),
				ಅಂಕೆ_ಎಳೆಗಳು);
	ಆದರೆ (!thread_infos)
		ಹೋಗು ಹೊರಹೋಗು;

	retval = read_bitmaps_range_prepare(fs, flags);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	ಗಾಗಿ (i = 0; i < ಅಂಕೆ_ಎಳೆಗಳು; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_ಬಾಲ_flags = 0;
		ಆದರೆ (i == 0)
			thread_infos[i].rbt_grp_ಆರಂಭ = 0;
		ಇಲ್ಲದಿದ್ದರೆ
			thread_infos[i].rbt_grp_ಆರಂಭ = ಸರಾಸರಿ_ಮೌಲ್ಯ_ಗುಂಪು * i + 1;

		ಆದರೆ (i == ಅಂಕೆ_ಎಳೆಗಳು - 1)
			thread_infos[i].rbt_grp_ಕೊನೆ = fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1;
		ಇಲ್ಲದಿದ್ದರೆ
			thread_infos[i].rbt_grp_ಕೊನೆ = ಸರಾಸರಿ_ಮೌಲ್ಯ_ಗುಂಪು * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		ಆದರೆ (retval)
			ನಿಲ್ಲಿಸು;
	}
	ಗಾಗಿ (i = 0; i < ಅಂಕೆ_ಎಳೆಗಳು; i++) {
		ಆದರೆ (!thread_ids[i])
			ನಿಲ್ಲಿಸು;
		rc = pthread_join(thread_ids[i], ಖಾಲಿ);
		ಆದರೆ (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		ಆದರೆ (rc && !retval)
			retval = rc;
		ಬಾಲ_flags |= thread_infos[i].rbt_ಬಾಲ_flags;
	}
ಹೊರಹೋಗು:
	rc = pthread_attr_destroy(&attr);
	ಆದರೆ (rc && !retval)
		retval = rc;
	ಬಿಡುಗಡೆ(thread_infos);
	ಬಿಡುಗಡೆ(thread_ids);

	ಆದರೆ (retval == 0)
		retval = read_bitmaps_range_ಕೊನೆ(fs, flags, ಬಾಲ_flags);
	ಆದರೆ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_ಗಣ_options(fs->io, "cache=on");
	ಹಿಂತಿರುಗಿ retval;
fallback:
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_PTHREAD */
	ಹಿಂತಿರುಗಿ read_bitmaps_range(fs, flags, 0, fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_ಖಂಡ_bitmap(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ ext2fs_rw_bitmaps(fs, EXT2_FS_BITMAPS_ಖಂಡ, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_ಖಂಡ_bitmap (ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	ಪೂರ್ಣಾಂಕ flags = 0;

	ಆದರೆ (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	ಆದರೆ (!fs->ಖಂಡ_map)
		flags |= EXT2_FS_BITMAPS_ಖಂಡ;
	ಆದರೆ (flags == 0)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	ಪೂರ್ಣಾಂಕ do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	ಪೂರ್ಣಾಂಕ do_ಖಂಡ = fs->ಖಂಡ_map && ext2fs_test_bb_dirty(fs);

	ಆದರೆ (!do_inode && !do_ಖಂಡ)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ write_bitmaps(fs, do_inode, do_ಖಂಡ);
}
