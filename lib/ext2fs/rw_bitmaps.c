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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ifdef HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#ಅನ್ತರ್ಭಾವ <pthread.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "e2image.h"

#ifdef HAVE_PTHREAD
ಪ್ರಕಾರನಿರ್ದೇಶ pthread_mutex_t mutex_t;

ಸ್ಥಿರ ಶೂನ್ಯ unix_pthread_mutex_lock(mutex_t *mutex)
{
	ಯದಿ (mutex)
		pthread_mutex_lock(mutex);
}
ಸ್ಥಿರ ಶೂನ್ಯ unix_pthread_mutex_unlock(mutex_t *mutex)
{
	ಯದಿ (mutex)
		pthread_mutex_unlock(mutex);
}
#ಅನ್ಯಥಾ
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ mutex_t;
#define unix_pthread_mutex_lock(mutex_t) ಕುರು {} ಯಾವತ್ (0)
#define unix_pthread_mutex_unlock(mutex_t) ಕುರು {} ಯಾವತ್ (0)
#endif

ಸ್ಥಿರ errcode_t write_bitmaps(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ do_inode, ಪೂರ್ಣಾಙ್ಕ do_block)
{
	dgrp_t 		i;
	unsigned ಪೂರ್ಣಾಙ್ಕ	j;
	ಪೂರ್ಣಾಙ್ಕ		block_nbytes, inode_nbytes;
	unsigned ಪೂರ್ಣಾಙ್ಕ	nbits;
	errcode_t	retval;
	ವರ್ಣ		*block_buf = ರಿಕ್ತ, *inode_buf = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (!(fs->flags & EXT2_FLAG_RW))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	ಯದಿ (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	ಯದಿ (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	ಪ್ರತಿ (i = 0; i < fs->group_desc_ಎಣಿಕೆ; i++) {
		ಯದಿ (!do_block)
			ಗಚ್ಛ skip_block_bitmap;

		ಯದಿ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			ಗಚ್ಛ skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;

		ಯದಿ (i == fs->group_desc_ಎಣಿಕೆ - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_ಅಂಕೆ_B2_C(fs,
				((ext2fs_blocks_ಎಣಿಕೆ(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			ಯದಿ (nbits)
				ಪ್ರತಿ (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		ಯದಿ (blk && blk < ext2fs_blocks_ಎಣಿಕೆ(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			ಯದಿ (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				ಗಚ್ಛ errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		ಯದಿ (!do_inode)
			ಅನುವರ್ತ;

		ಯದಿ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			ಗಚ್ಛ skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		ಯದಿ (blk && blk < ext2fs_blocks_ಎಣಿಕೆ(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			ಯದಿ (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				ಗಚ್ಛ errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	ಯದಿ (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	ಯದಿ (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
errout:
	ಯದಿ (inode_buf)
		ext2fs_free_mem(&inode_buf);
	ಯದಿ (block_buf)
		ext2fs_free_mem(&block_buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	ಪ್ರತಿ (i = 0; i < fs->group_desc_ಎಣಿಕೆ; i++) {
		ಯದಿ (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			ಅನುವರ್ತ;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		ಯದಿ (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		ಯದಿ (blk && blk < ext2fs_blocks_ಎಣಿಕೆ(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ಯದಿ (blk && blk < ext2fs_blocks_ಎಣಿಕೆ(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ bitmap_tail_verify(unsigned ವರ್ಣ *bitmap, ಪೂರ್ಣಾಙ್ಕ first, ಪೂರ್ಣಾಙ್ಕ last)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = first; i <= last; i++)
		ಯದಿ (bitmap[i] != 0xff)
			ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ errcode_t read_bitmaps_range_prepare(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags)
{
	errcode_t retval;
	ಪೂರ್ಣಾಙ್ಕ block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	ಪೂರ್ಣಾಙ್ಕ inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	ವರ್ಣ *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ ((block_nbytes > (ಪೂರ್ಣಾಙ್ಕ) fs->blocksize) ||
	    (inode_nbytes > (ಪೂರ್ಣಾಙ್ಕ) fs->blocksize))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (flags & EXT2FS_BITMAPS_BLOCK) {
		ಯದಿ (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	}

	ಯದಿ (flags & EXT2FS_BITMAPS_INODE) {
		ಯದಿ (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	}
	ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;

cleanup:
	ಯದಿ (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	ಯದಿ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t read_bitmaps_range_start(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  ಪೂರ್ಣಾಙ್ಕ *tail_flags)
{
	dgrp_t i;
	ವರ್ಣ *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	ಪೂರ್ಣಾಙ್ಕ block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	ಪೂರ್ಣಾಙ್ಕ inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	ಪೂರ್ಣಾಙ್ಕ csum_flag;
	unsigned ಪೂರ್ಣಾಙ್ಕ	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	ಯದಿ (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	} ಅನ್ಯಥಾ {
		block_nbytes = 0;
	}

	ಯದಿ (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
	} ಅನ್ಯಥಾ {
		inode_nbytes = 0;
	}

	/* io should be null */
	ಯದಿ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_ಎಣಿಕೆ;
		ಯಾವತ್ (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			ಯದಿ (retval)
				ಗಚ್ಛ cleanup;
			cnt = fs->blocksize << 3;
			ಯದಿ (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			ಯದಿ (retval)
				ಗಚ್ಛ cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_ಎಣಿಕೆ);
		ಯಾವತ್ (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			ಯದಿ (retval)
				ಗಚ್ಛ cleanup;
			cnt = fs->blocksize << 3;
			ಯದಿ (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			ಯದಿ (retval)
				ಗಚ್ಛ cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		ಗಚ್ಛ cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	ಪ್ರತಿ (i = start; i <= end; i++) {
		ಯದಿ (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			ಯದಿ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_ಎಣಿಕೆ(fs->super)))
				blk = 0;
			ಯದಿ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				ಯದಿ (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					ಗಚ್ಛ cleanup;
				}
				/* verify block bitmap checksum */
				ಯದಿ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					ಗಚ್ಛ cleanup;
				}
				ಯದಿ (!bitmap_tail_verify((unsigned ವರ್ಣ *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} ಅನ್ಯಥಾ
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ಯದಿ (retval)
				ಗಚ್ಛ cleanup;
			blk_itr += block_nbytes << 3;
		}
		ಯದಿ (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			ಯದಿ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_ಎಣಿಕೆ(fs->super)))
				blk = 0;
			ಯದಿ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				ಯದಿ (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					ಗಚ್ಛ cleanup;
				}

				/* verify inode bitmap checksum */
				ಯದಿ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					ಗಚ್ಛ cleanup;
				}
				ಯದಿ (!bitmap_tail_verify((unsigned ವರ್ಣ *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} ಅನ್ಯಥಾ
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ಯದಿ (retval)
				ಗಚ್ಛ cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	ಯದಿ (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	ಯದಿ (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t read_bitmaps_range_end(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags,
					ಪೂರ್ಣಾಙ್ಕ tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	ಯದಿ (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	ಯದಿ (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_bitmaps_cleanup_on_error(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಯದಿ (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	ಯದಿ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ಸ್ಥಿರ errcode_t read_bitmaps_range(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	ಪೂರ್ಣಾಙ್ಕ tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  ರಿಕ್ತ, &tail_flags);
	ಯದಿ (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	ಯದಿ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	ಪ್ರತ್ಯಾಗಮ retval;
}

#ifdef HAVE_PTHREAD
ಸಂರಚನಾ read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	ಪೂರ್ಣಾಙ್ಕ		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	ಪೂರ್ಣಾಙ್ಕ		rbt_tail_flags;
};

ಸ್ಥಿರ ಶೂನ್ಯ *read_bitmaps_thread(ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = ರಿಕ್ತ;
	ಸಂರಚನಾ read_bitmaps_thread_info *thread_infos = ರಿಕ್ತ;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	ಪೂರ್ಣಾಙ್ಕ i, tail_flags = 0;
#endif

	ಯದಿ (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	ಯದಿ (ext2fs_has_feature_journal_dev(fs->super))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ಯದಿ (flags & EXT2FS_BITMAPS_WRITE)
		ಪ್ರತ್ಯಾಗಮ write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	ಯದಿ (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (ಅಂಕೆ_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		ಗಚ್ಛ fallback;

#ಯದಿ defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	ಯದಿ (ಅಂಕೆ_threads < 0)
		ಅಂಕೆ_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	ಯದಿ (ಅಂಕೆ_threads <= 0)
		ಅಂಕೆ_threads = 4;

	ಯದಿ ((unsigned) ಅಂಕೆ_threads > fs->group_desc_ಎಣಿಕೆ)
		ಅಂಕೆ_threads = fs->group_desc_ಎಣಿಕೆ;
	average_group = fs->group_desc_ಎಣಿಕೆ / ಅಂಕೆ_threads;
	ಯದಿ (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	ಯದಿ ((ಅಂಕೆ_threads <= 1) || (average_group == 0))
		ಗಚ್ಛ fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	thread_ids = calloc(ಪರಿಮಾಣ(pthread_t), ಅಂಕೆ_threads);
	ಯದಿ (!thread_ids)
		ಪ್ರತ್ಯಾಗಮ ENOMEM;

	thread_infos = calloc(ಪರಿಮಾಣ(ಸಂರಚನಾ read_bitmaps_thread_info),
				ಅಂಕೆ_threads);
	ಯದಿ (!thread_infos)
		ಗಚ್ಛ out;

	retval = read_bitmaps_range_prepare(fs, flags);
	ಯದಿ (retval)
		ಗಚ್ಛ out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		ಯದಿ (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		ಅನ್ಯಥಾ
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		ಯದಿ (i == ಅಂಕೆ_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_ಎಣಿಕೆ - 1;
		ಅನ್ಯಥಾ
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		ಯದಿ (retval)
			ಭಙ್ಗ;
	}
	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_threads; i++) {
		ಯದಿ (!thread_ids[i])
			ಭಙ್ಗ;
		rc = pthread_join(thread_ids[i], ರಿಕ್ತ);
		ಯದಿ (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		ಯದಿ (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	ಯದಿ (rc && !retval)
		retval = rc;
	ಮುಕ್ತ(thread_infos);
	ಮುಕ್ತ(thread_ids);

	ಯದಿ (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	ಯದಿ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	ಪ್ರತ್ಯಾಗಮ retval;
fallback:
#endif /* HAVE_PTHREAD */
	ಪ್ರತ್ಯಾಗಮ read_bitmaps_range(fs, flags, 0, fs->group_desc_ಎಣಿಕೆ - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	ಪ್ರತ್ಯಾಗಮ write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	ಪೂರ್ಣಾಙ್ಕ flags = 0;

	ಯದಿ (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	ಯದಿ (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	ಯದಿ (flags == 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	ಪೂರ್ಣಾಙ್ಕ do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	ಪೂರ್ಣಾಙ್ಕ do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	ಯದಿ (!do_inode && !do_block)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ write_bitmaps(fs, do_inode, do_block);
}
