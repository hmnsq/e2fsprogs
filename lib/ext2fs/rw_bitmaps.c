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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#ifdef HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#شامل <pthread.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "e2image.h"

#ifdef HAVE_PTHREAD
تعریفنوع pthread_mutex_t mutex_t;

ایستا تهی unix_pthread_mutex_lock(mutex_t *mutex)
{
	اگر (mutex)
		pthread_mutex_lock(mutex);
}
ایستا تهی unix_pthread_mutex_unlock(mutex_t *mutex)
{
	اگر (mutex)
		pthread_mutex_unlock(mutex);
}
#وگرنه
تعریفنوع صحیح mutex_t;
#define unix_pthread_mutex_lock(mutex_t) انجامبده {} وقتی (0)
#define unix_pthread_mutex_unlock(mutex_t) انجامبده {} وقتی (0)
#endif

ایستا errcode_t write_bitmaps(ext2_filsys fs, صحیح do_inode, صحیح do_block)
{
	dgrp_t 		i;
	unsigned صحیح	j;
	صحیح		block_nbytes, inode_nbytes;
	unsigned صحیح	nbits;
	errcode_t	retval;
	نویسه		*block_buf = پوچ, *inode_buf = پوچ;
	صحیح		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!(fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	اگر (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		اگر (retval)
			برو errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	اگر (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		اگر (retval)
			برو errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	برای (i = 0; i < fs->group_desc_شمار; i++) {
		اگر (!do_block)
			برو skip_block_bitmap;

		اگر (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			برو skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		اگر (retval)
			برو errout;

		اگر (i == fs->group_desc_شمار - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_رقم_B2_C(fs,
				((ext2fs_blocks_شمار(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			اگر (nbits)
				برای (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		اگر (retval)
			بازگشت retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		اگر (blk && blk < ext2fs_blocks_شمار(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			اگر (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				برو errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		اگر (!do_inode)
			ادامه;

		اگر (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			برو skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		اگر (retval)
			برو errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		اگر (retval)
			برو errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		اگر (blk && blk < ext2fs_blocks_شمار(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			اگر (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				برو errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	اگر (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	اگر (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	بازگشت 0;
errout:
	اگر (inode_buf)
		ext2fs_free_mem(&inode_buf);
	اگر (block_buf)
		ext2fs_free_mem(&block_buf);
	بازگشت retval;
}

ایستا errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	برای (i = 0; i < fs->group_desc_شمار; i++) {
		اگر (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			ادامه;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		اگر (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		اگر (blk && blk < ext2fs_blocks_شمار(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		اگر (blk && blk < ext2fs_blocks_شمار(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	بازگشت 0;
}

ایستا صحیح bitmap_tail_verify(unsigned نویسه *bitmap, صحیح first, صحیح last)
{
	صحیح i;

	برای (i = first; i <= last; i++)
		اگر (bitmap[i] != 0xff)
			بازگشت 0;
	بازگشت 1;
}

ایستا errcode_t read_bitmaps_range_prepare(ext2_filsys fs, صحیح flags)
{
	errcode_t retval;
	صحیح block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	صحیح inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	نویسه *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر ((block_nbytes > (صحیح) fs->blocksize) ||
	    (inode_nbytes > (صحیح) fs->blocksize))
		بازگشت EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	اگر (retval)
		بازگشت retval;

	اگر (flags & EXT2FS_BITMAPS_BLOCK) {
		اگر (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		اگر (retval)
			برو cleanup;
	}

	اگر (flags & EXT2FS_BITMAPS_INODE) {
		اگر (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		اگر (retval)
			برو cleanup;
	}
	ext2fs_free_mem(&buf);
	بازگشت retval;

cleanup:
	اگر (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	اگر (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	بازگشت retval;
}

ایستا errcode_t read_bitmaps_range_start(ext2_filsys fs, صحیح flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  صحیح *tail_flags)
{
	dgrp_t i;
	نویسه *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	صحیح block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	صحیح inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	صحیح csum_flag;
	unsigned صحیح	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	اگر (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		اگر (retval)
			برو cleanup;
	} وگرنه {
		block_nbytes = 0;
	}

	اگر (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		اگر (retval)
			برو cleanup;
	} وگرنه {
		inode_nbytes = 0;
	}

	/* io should be null */
	اگر (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_شمار;
		وقتی (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			اگر (retval)
				برو cleanup;
			cnt = fs->blocksize << 3;
			اگر (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			اگر (retval)
				برو cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_شمار);
		وقتی (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			اگر (retval)
				برو cleanup;
			cnt = fs->blocksize << 3;
			اگر (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			اگر (retval)
				برو cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		برو cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	برای (i = start; i <= end; i++) {
		اگر (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			اگر ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_شمار(fs->super)))
				blk = 0;
			اگر (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				اگر (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					برو cleanup;
				}
				/* verify block bitmap checksum */
				اگر (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					برو cleanup;
				}
				اگر (!bitmap_tail_verify((unsigned نویسه *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} وگرنه
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			اگر (retval)
				برو cleanup;
			blk_itr += block_nbytes << 3;
		}
		اگر (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			اگر ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_شمار(fs->super)))
				blk = 0;
			اگر (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				اگر (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					برو cleanup;
				}

				/* verify inode bitmap checksum */
				اگر (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					برو cleanup;
				}
				اگر (!bitmap_tail_verify((unsigned نویسه *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} وگرنه
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			اگر (retval)
				برو cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	اگر (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	اگر (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	بازگشت retval;
}

ایستا errcode_t read_bitmaps_range_end(ext2_filsys fs, صحیح flags,
					صحیح tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	اگر (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		اگر (retval)
			بازگشت retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	اگر (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	بازگشت 0;
}

ایستا تهی read_bitmaps_cleanup_on_error(ext2_filsys fs, صحیح flags)
{
	اگر (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	اگر (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ایستا errcode_t read_bitmaps_range(ext2_filsys fs, صحیح flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	صحیح tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	اگر (retval)
		بازگشت retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  پوچ, &tail_flags);
	اگر (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	اگر (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	بازگشت retval;
}

#ifdef HAVE_PTHREAD
ساختار read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	صحیح		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	صحیح		rbt_tail_flags;
};

ایستا تهی *read_bitmaps_thread(تهی *data)
{
	ساختار read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	بازگشت پوچ;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحیح flags, صحیح رقم_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = پوچ;
	ساختار read_bitmaps_thread_info *thread_infos = پوچ;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	صحیح i, tail_flags = 0;
#endif

	اگر (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		بازگشت EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	اگر (flags & EXT2FS_BITMAPS_WRITE)
		بازگشت write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	اگر (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (رقم_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		برو fallback;

#اگر defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	اگر (رقم_threads < 0)
		رقم_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	اگر (رقم_threads <= 0)
		رقم_threads = 4;

	اگر ((unsigned) رقم_threads > fs->group_desc_شمار)
		رقم_threads = fs->group_desc_شمار;
	average_group = fs->group_desc_شمار / رقم_threads;
	اگر (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	اگر ((رقم_threads <= 1) || (average_group == 0))
		برو fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	اگر (retval)
		بازگشت retval;

	thread_ids = calloc(اندازه(pthread_t), رقم_threads);
	اگر (!thread_ids)
		بازگشت ENOMEM;

	thread_infos = calloc(اندازه(ساختار read_bitmaps_thread_info),
				رقم_threads);
	اگر (!thread_infos)
		برو out;

	retval = read_bitmaps_range_prepare(fs, flags);
	اگر (retval)
		برو out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	برای (i = 0; i < رقم_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		اگر (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		وگرنه
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		اگر (i == رقم_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_شمار - 1;
		وگرنه
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		اگر (retval)
			بشکن;
	}
	برای (i = 0; i < رقم_threads; i++) {
		اگر (!thread_ids[i])
			بشکن;
		rc = pthread_join(thread_ids[i], پوچ);
		اگر (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		اگر (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	اگر (rc && !retval)
		retval = rc;
	آزاد(thread_infos);
	آزاد(thread_ids);

	اگر (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	اگر (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	بازگشت retval;
fallback:
#endif /* HAVE_PTHREAD */
	بازگشت read_bitmaps_range(fs, flags, 0, fs->group_desc_شمار - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	بازگشت ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	بازگشت ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	بازگشت write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	بازگشت write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	صحیح flags = 0;

	اگر (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	اگر (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	اگر (flags == 0)
		بازگشت 0;
	بازگشت ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	صحیح do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	صحیح do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	اگر (!do_inode && !do_block)
		بازگشت 0;

	بازگشت write_bitmaps(fs, do_inode, do_block);
}
