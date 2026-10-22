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
#නම් HAVE_UNISTD_H
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
	නම් (mutex)
		pthread_mutex_lock(mutex);
}
static void unix_pthread_mutex_unlock(mutex_t *mutex)
{
	නම් (mutex)
		pthread_mutex_unlock(mutex);
}
#නැතහොත්
typedef පූර්ණ_සංඛ්‍යා mutex_t;
#define unix_pthread_mutex_lock(mutex_t) do {} අතරතුර (0)
#define unix_pthread_mutex_unlock(mutex_t) do {} අතරතුර (0)
#endif

static errcode_t write_bitmaps(ext2_filsys fs, පූර්ණ_සංඛ්‍යා do_inode, පූර්ණ_සංඛ්‍යා do_block)
{
	dgrp_t 		i;
	unsigned පූර්ණ_සංඛ්‍යා	j;
	පූර්ණ_සංඛ්‍යා		block_nbytes, inode_nbytes;
	unsigned පූර්ණ_සංඛ්‍යා	nbits;
	errcode_t	retval;
	var_na		*block_buf = NULL, *inode_buf = NULL;
	පූර්ණ_සංඛ්‍යා		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	නම් (!(fs->flags & EXT2_FLAG_RW))
		ආපසු EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	නම් (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		නම් (retval)
			goto errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	නම් (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		නම් (retval)
			goto errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	සඳහා (i = 0; i < fs->group_desc_count; i++) {
		නම් (!do_block)
			goto skip_block_bitmap;

		නම් (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			goto skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		නම් (retval)
			goto errout;

		නම් (i == fs->group_desc_count - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2FS_NUM_B2C(fs,
				((ext2fs_blocks_count(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			නම් (nbits)
				සඳහා (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		නම් (retval)
			ආපසු retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		නම් (blk && blk < ext2fs_blocks_count(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			නම් (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				goto errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		නම් (!do_inode)
			continue;

		නම් (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			goto skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		නම් (retval)
			goto errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		නම් (retval)
			goto errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		නම් (blk && blk < ext2fs_blocks_count(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			නම් (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				goto errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	නම් (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	නම් (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	ආපසු 0;
errout:
	නම් (inode_buf)
		ext2fs_free_mem(&inode_buf);
	නම් (block_buf)
		ext2fs_free_mem(&block_buf);
	ආපසු retval;
}

static errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	සඳහා (i = 0; i < fs->group_desc_count; i++) {
		නම් (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			continue;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		නම් (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		නම් (blk && blk < ext2fs_blocks_count(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		නම් (blk && blk < ext2fs_blocks_count(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා bitmap_tail_verify(unsigned var_na *bitmap, පූර්ණ_සංඛ්‍යා first, පූර්ණ_සංඛ්‍යා last)
{
	පූර්ණ_සංඛ්‍යා i;

	සඳහා (i = first; i <= last; i++)
		නම් (bitmap[i] != 0xff)
			ආපසු 0;
	ආපසු 1;
}

static errcode_t read_bitmaps_range_prepare(ext2_filsys fs, පූර්ණ_සංඛ්‍යා flags)
{
	errcode_t retval;
	පූර්ණ_සංඛ්‍යා block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	පූර්ණ_සංඛ්‍යා inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	var_na *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	නම් ((block_nbytes > (පූර්ණ_සංඛ්‍යා) fs->blocksize) ||
	    (inode_nbytes > (පූර්ණ_සංඛ්‍යා) fs->blocksize))
		ආපසු EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	නම් (retval)
		ආපසු retval;

	නම් (flags & EXT2FS_BITMAPS_BLOCK) {
		නම් (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		නම් (retval)
			goto cleanup;
	}

	නම් (flags & EXT2FS_BITMAPS_INODE) {
		නම් (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		නම් (retval)
			goto cleanup;
	}
	ext2fs_free_mem(&buf);
	ආපසු retval;

cleanup:
	නම් (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	නම් (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	ආපසු retval;
}

static errcode_t read_bitmaps_range_start(ext2_filsys fs, පූර්ණ_සංඛ්‍යා flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  පූර්ණ_සංඛ්‍යා *tail_flags)
{
	dgrp_t i;
	var_na *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	පූර්ණ_සංඛ්‍යා block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	පූර්ණ_සංඛ්‍යා inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	පූර්ණ_සංඛ්‍යා csum_flag;
	unsigned පූර්ණ_සංඛ්‍යා	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	නම් (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		නම් (retval)
			goto cleanup;
	} නැතහොත් {
		block_nbytes = 0;
	}

	නම් (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		නම් (retval)
			goto cleanup;
	} නැතහොත් {
		inode_nbytes = 0;
	}

	/* io should be null */
	නම් (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_count;
		අතරතුර (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			නම් (retval)
				goto cleanup;
			cnt = fs->blocksize << 3;
			නම් (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			නම් (retval)
				goto cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_count);
		අතරතුර (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			නම් (retval)
				goto cleanup;
			cnt = fs->blocksize << 3;
			නම් (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			නම් (retval)
				goto cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		goto cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	සඳහා (i = start; i <= end; i++) {
		නම් (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			නම් ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_count(fs->super)))
				blk = 0;
			නම් (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				නම් (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					goto cleanup;
				}
				/* verify block bitmap checksum */
				නම් (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					goto cleanup;
				}
				නම් (!bitmap_tail_verify((unsigned var_na *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} නැතහොත්
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			නම් (retval)
				goto cleanup;
			blk_itr += block_nbytes << 3;
		}
		නම් (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			නම් ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_count(fs->super)))
				blk = 0;
			නම් (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				නම් (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					goto cleanup;
				}

				/* verify inode bitmap checksum */
				නම් (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					goto cleanup;
				}
				නම් (!bitmap_tail_verify((unsigned var_na *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} නැතහොත්
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			නම් (retval)
				goto cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	නම් (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	නම් (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	ආපසු retval;
}

static errcode_t read_bitmaps_range_end(ext2_filsys fs, පූර්ණ_සංඛ්‍යා flags,
					පූර්ණ_සංඛ්‍යා tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	නම් (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		නම් (retval)
			ආපසු retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	නම් (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	ආපසු 0;
}

static void read_bitmaps_cleanup_on_error(ext2_filsys fs, පූර්ණ_සංඛ්‍යා flags)
{
	නම් (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	නම් (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

static errcode_t read_bitmaps_range(ext2_filsys fs, පූර්ණ_සංඛ්‍යා flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	පූර්ණ_සංඛ්‍යා tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	නම් (retval)
		ආපසු retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  NULL, &tail_flags);
	නම් (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	නම් (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	ආපසු retval;
}

#ifdef HAVE_PTHREAD
sa_nracanaa read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	පූර්ණ_සංඛ්‍යා		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	පූර්ණ_සංඛ්‍යා		rbt_tail_flags;
};

static void *read_bitmaps_thread(void *data)
{
	sa_nracanaa read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	ආපසු NULL;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, පූර්ණ_සංඛ්‍යා flags, පූර්ණ_සංඛ්‍යා num_threads)
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
	පූර්ණ_සංඛ්‍යා i, tail_flags = 0;
#endif

	නම් (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		ආපසු EXT2_ET_INVALID_ARGUMENT;

	නම් (ext2fs_has_feature_journal_dev(fs->super))
		ආපසු EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	නම් (flags & EXT2FS_BITMAPS_WRITE)
		ආපසු write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	නම් (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (num_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		goto fallback;

#නම් defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	නම් (num_threads < 0)
		num_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	නම් (num_threads <= 0)
		num_threads = 4;

	නම් ((unsigned) num_threads > fs->group_desc_count)
		num_threads = fs->group_desc_count;
	average_group = fs->group_desc_count / num_threads;
	නම් (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	නම් ((num_threads <= 1) || (average_group == 0))
		goto fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	නම් (retval)
		ආපසු retval;

	thread_ids = calloc(parimaa_na(pthread_t), num_threads);
	නම් (!thread_ids)
		ආපසු ENOMEM;

	thread_infos = calloc(parimaa_na(sa_nracanaa read_bitmaps_thread_info),
				num_threads);
	නම් (!thread_infos)
		goto out;

	retval = read_bitmaps_range_prepare(fs, flags);
	නම් (retval)
		goto out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	සඳහා (i = 0; i < num_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		නම් (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		නැතහොත්
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		නම් (i == num_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_count - 1;
		නැතහොත්
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		නම් (retval)
			bha_nga;
	}
	සඳහා (i = 0; i < num_threads; i++) {
		නම් (!thread_ids[i])
			bha_nga;
		rc = pthread_join(thread_ids[i], NULL);
		නම් (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		නම් (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	නම් (rc && !retval)
		retval = rc;
	free(thread_infos);
	free(thread_ids);

	නම් (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	නම් (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	ආපසු retval;
fallback:
#endif /* HAVE_PTHREAD */
	ආපසු read_bitmaps_range(fs, flags, 0, fs->group_desc_count - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	ආපසු ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	ආපසු ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	ආපසු write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	ආපසු write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	පූර්ණ_සංඛ්‍යා flags = 0;

	නම් (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	නම් (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	නම් (flags == 0)
		ආපසු 0;
	ආපසු ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	පූර්ණ_සංඛ්‍යා do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	පූර්ණ_සංඛ්‍යා do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	නම් (!do_inode && !do_block)
		ආපසු 0;

	ආපසු write_bitmaps(fs, do_inode, do_block);
}
