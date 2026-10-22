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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ᤀᤡ᤺ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "e2image.h"

#ifdef HAVE_PTHREAD
typedef pthread_mutex_t mutex_t;

static void unix_pthread_mutex_lock(mutex_t *mutex)
{
	ᤀᤡ᤺ (mutex)
		pthread_mutex_lock(mutex);
}
static void unix_pthread_mutex_unlock(mutex_t *mutex)
{
	ᤀᤡ᤺ (mutex)
		pthread_mutex_unlock(mutex);
}
#ᤀᤡ᤺ᤆᤡ
typedef puur_naa_nka mutex_t;
#define unix_pthread_mutex_lock(mutex_t) do {} ᤀᤠ᤺ (0)
#define unix_pthread_mutex_unlock(mutex_t) do {} ᤀᤠ᤺ (0)
#endif

static errcode_t write_bitmaps(ext2_filsys fs, puur_naa_nka do_inode, puur_naa_nka do_block)
{
	dgrp_t 		i;
	unsigned puur_naa_nka	j;
	puur_naa_nka		block_nbytes, inode_nbytes;
	unsigned puur_naa_nka	nbits;
	errcode_t	retval;
	var_na		*block_buf = NULL, *inode_buf = NULL;
	puur_naa_nka		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ᤀᤡ᤺ (!(fs->flags & EXT2_FLAG_RW))
		ᤀᤡᤴ EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	ᤀᤡ᤺ (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		ᤀᤡ᤺ (retval)
			goto errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	ᤀᤡ᤺ (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		ᤀᤡ᤺ (retval)
			goto errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	for (i = 0; i < fs->group_desc_count; i++) {
		ᤀᤡ᤺ (!do_block)
			goto skip_block_bitmap;

		ᤀᤡ᤺ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			goto skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		ᤀᤡ᤺ (retval)
			goto errout;

		ᤀᤡ᤺ (i == fs->group_desc_count - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2FS_NUM_B2C(fs,
				((ext2fs_blocks_count(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			ᤀᤡ᤺ (nbits)
				for (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		ᤀᤡ᤺ (retval)
			ᤀᤡᤴ retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		ᤀᤡ᤺ (blk && blk < ext2fs_blocks_count(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			ᤀᤡ᤺ (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				goto errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		ᤀᤡ᤺ (!do_inode)
			continue;

		ᤀᤡ᤺ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			goto skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		ᤀᤡ᤺ (retval)
			goto errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		ᤀᤡ᤺ (retval)
			goto errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		ᤀᤡ᤺ (blk && blk < ext2fs_blocks_count(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			ᤀᤡ᤺ (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				goto errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	ᤀᤡ᤺ (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	ᤀᤡ᤺ (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	ᤀᤡᤴ 0;
errout:
	ᤀᤡ᤺ (inode_buf)
		ext2fs_free_mem(&inode_buf);
	ᤀᤡ᤺ (block_buf)
		ext2fs_free_mem(&block_buf);
	ᤀᤡᤴ retval;
}

static errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	for (i = 0; i < fs->group_desc_count; i++) {
		ᤀᤡ᤺ (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			continue;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		ᤀᤡ᤺ (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		ᤀᤡ᤺ (blk && blk < ext2fs_blocks_count(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ᤀᤡ᤺ (blk && blk < ext2fs_blocks_count(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	ᤀᤡᤴ 0;
}

static puur_naa_nka bitmap_tail_verify(unsigned var_na *bitmap, puur_naa_nka first, puur_naa_nka last)
{
	puur_naa_nka i;

	for (i = first; i <= last; i++)
		ᤀᤡ᤺ (bitmap[i] != 0xff)
			ᤀᤡᤴ 0;
	ᤀᤡᤴ 1;
}

static errcode_t read_bitmaps_range_prepare(ext2_filsys fs, puur_naa_nka flags)
{
	errcode_t retval;
	puur_naa_nka block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	puur_naa_nka inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	var_na *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ᤀᤡ᤺ ((block_nbytes > (puur_naa_nka) fs->blocksize) ||
	    (inode_nbytes > (puur_naa_nka) fs->blocksize))
		ᤀᤡᤴ EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	ᤀᤡ᤺ (retval)
		ᤀᤡᤴ retval;

	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_BLOCK) {
		ᤀᤡ᤺ (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		ᤀᤡ᤺ (retval)
			goto cleanup;
	}

	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_INODE) {
		ᤀᤡ᤺ (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		ᤀᤡ᤺ (retval)
			goto cleanup;
	}
	ext2fs_free_mem(&buf);
	ᤀᤡᤴ retval;

cleanup:
	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	ᤀᤡᤴ retval;
}

static errcode_t read_bitmaps_range_start(ext2_filsys fs, puur_naa_nka flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  puur_naa_nka *tail_flags)
{
	dgrp_t i;
	var_na *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	puur_naa_nka block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	puur_naa_nka inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	puur_naa_nka csum_flag;
	unsigned puur_naa_nka	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		ᤀᤡ᤺ (retval)
			goto cleanup;
	} ᤀᤡ᤺ᤆᤡ {
		block_nbytes = 0;
	}

	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		ᤀᤡ᤺ (retval)
			goto cleanup;
	} ᤀᤡ᤺ᤆᤡ {
		inode_nbytes = 0;
	}

	/* io should be null */
	ᤀᤡ᤺ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_count;
		ᤀᤠ᤺ (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			ᤀᤡ᤺ (retval)
				goto cleanup;
			cnt = fs->blocksize << 3;
			ᤀᤡ᤺ (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			ᤀᤡ᤺ (retval)
				goto cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_count);
		ᤀᤠ᤺ (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			ᤀᤡ᤺ (retval)
				goto cleanup;
			cnt = fs->blocksize << 3;
			ᤀᤡ᤺ (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			ᤀᤡ᤺ (retval)
				goto cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		goto cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	for (i = start; i <= end; i++) {
		ᤀᤡ᤺ (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			ᤀᤡ᤺ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_count(fs->super)))
				blk = 0;
			ᤀᤡ᤺ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				ᤀᤡ᤺ (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					goto cleanup;
				}
				/* verify block bitmap checksum */
				ᤀᤡ᤺ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					goto cleanup;
				}
				ᤀᤡ᤺ (!bitmap_tail_verify((unsigned var_na *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} ᤀᤡ᤺ᤆᤡ
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ᤀᤡ᤺ (retval)
				goto cleanup;
			blk_itr += block_nbytes << 3;
		}
		ᤀᤡ᤺ (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			ᤀᤡ᤺ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_count(fs->super)))
				blk = 0;
			ᤀᤡ᤺ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				ᤀᤡ᤺ (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					goto cleanup;
				}

				/* verify inode bitmap checksum */
				ᤀᤡ᤺ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					goto cleanup;
				}
				ᤀᤡ᤺ (!bitmap_tail_verify((unsigned var_na *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} ᤀᤡ᤺ᤆᤡ
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ᤀᤡ᤺ (retval)
				goto cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	ᤀᤡ᤺ (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	ᤀᤡ᤺ (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	ᤀᤡᤴ retval;
}

static errcode_t read_bitmaps_range_end(ext2_filsys fs, puur_naa_nka flags,
					puur_naa_nka tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		ᤀᤡ᤺ (retval)
			ᤀᤡᤴ retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	ᤀᤡᤴ 0;
}

static void read_bitmaps_cleanup_on_error(ext2_filsys fs, puur_naa_nka flags)
{
	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

static errcode_t read_bitmaps_range(ext2_filsys fs, puur_naa_nka flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	puur_naa_nka tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	ᤀᤡ᤺ (retval)
		ᤀᤡᤴ retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  NULL, &tail_flags);
	ᤀᤡ᤺ (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	ᤀᤡ᤺ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	ᤀᤡᤴ retval;
}

#ifdef HAVE_PTHREAD
sa_nracanaa read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	puur_naa_nka		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	puur_naa_nka		rbt_tail_flags;
};

static void *read_bitmaps_thread(void *data)
{
	sa_nracanaa read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	ᤀᤡᤴ NULL;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, puur_naa_nka flags, puur_naa_nka num_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = NULL;
	sa_nracanaa read_bitmaps_thread_info *thread_infos = NULL;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	puur_naa_nka i, tail_flags = 0;
#endif

	ᤀᤡ᤺ (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		ᤀᤡᤴ EXT2_ET_INVALID_ARGUMENT;

	ᤀᤡ᤺ (ext2fs_has_feature_journal_dev(fs->super))
		ᤀᤡᤴ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ᤀᤡ᤺ (flags & EXT2FS_BITMAPS_WRITE)
		ᤀᤡᤴ write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	ᤀᤡ᤺ (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (num_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		goto fallback;

#ᤀᤡ᤺ defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	ᤀᤡ᤺ (num_threads < 0)
		num_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	ᤀᤡ᤺ (num_threads <= 0)
		num_threads = 4;

	ᤀᤡ᤺ ((unsigned) num_threads > fs->group_desc_count)
		num_threads = fs->group_desc_count;
	average_group = fs->group_desc_count / num_threads;
	ᤀᤡ᤺ (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	ᤀᤡ᤺ ((num_threads <= 1) || (average_group == 0))
		goto fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	ᤀᤡ᤺ (retval)
		ᤀᤡᤴ retval;

	thread_ids = calloc(parimaa_na(pthread_t), num_threads);
	ᤀᤡ᤺ (!thread_ids)
		ᤀᤡᤴ ENOMEM;

	thread_infos = calloc(parimaa_na(sa_nracanaa read_bitmaps_thread_info),
				num_threads);
	ᤀᤡ᤺ (!thread_infos)
		goto out;

	retval = read_bitmaps_range_prepare(fs, flags);
	ᤀᤡ᤺ (retval)
		goto out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	for (i = 0; i < num_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		ᤀᤡ᤺ (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		ᤀᤡ᤺ᤆᤡ
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		ᤀᤡ᤺ (i == num_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_count - 1;
		ᤀᤡ᤺ᤆᤡ
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		ᤀᤡ᤺ (retval)
			bha_nga;
	}
	for (i = 0; i < num_threads; i++) {
		ᤀᤡ᤺ (!thread_ids[i])
			bha_nga;
		rc = pthread_join(thread_ids[i], NULL);
		ᤀᤡ᤺ (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		ᤀᤡ᤺ (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	ᤀᤡ᤺ (rc && !retval)
		retval = rc;
	free(thread_infos);
	free(thread_ids);

	ᤀᤡ᤺ (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	ᤀᤡ᤺ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	ᤀᤡᤴ retval;
fallback:
#endif /* HAVE_PTHREAD */
	ᤀᤡᤴ read_bitmaps_range(fs, flags, 0, fs->group_desc_count - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	ᤀᤡᤴ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	ᤀᤡᤴ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	ᤀᤡᤴ write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	ᤀᤡᤴ write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	puur_naa_nka flags = 0;

	ᤀᤡ᤺ (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	ᤀᤡ᤺ (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	ᤀᤡ᤺ (flags == 0)
		ᤀᤡᤴ 0;
	ᤀᤡᤴ ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	puur_naa_nka do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	puur_naa_nka do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	ᤀᤡ᤺ (!do_inode && !do_block)
		ᤀᤡᤴ 0;

	ᤀᤡᤴ write_bitmaps(fs, do_inode, do_block);
}
