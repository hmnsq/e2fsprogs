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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ifdef HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#ضمن <pthread.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "e2image.h"

#ifdef HAVE_PTHREAD
تعريفنوع pthread_mutex_t mutex_t;

ساكن فراغ unix_pthread_mutex_lock(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_lock(mutex);
}
ساكن فراغ unix_pthread_mutex_unlock(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_unlock(mutex);
}
#وإلا
تعريفنوع صحيح mutex_t;
#define unix_pthread_mutex_lock(mutex_t) افعل {} طالما (0)
#define unix_pthread_mutex_unlock(mutex_t) افعل {} طالما (0)
#endif

ساكن errcode_t write_bitmaps(ext2_filsys fs, صحيح do_inode, صحيح do_block)
{
	dgrp_t 		i;
	unsigned صحيح	j;
	صحيح		block_nbytes, inode_nbytes;
	unsigned صحيح	nbits;
	errcode_t	retval;
	حرف		*block_buf = عدم, *inode_buf = عدم;
	صحيح		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	إذا (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		إذا (retval)
			اذهب errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	إذا (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		إذا (retval)
			اذهب errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	لكل (i = 0; i < fs->group_desc_کيراءن; i++) {
		إذا (!do_block)
			اذهب skip_block_bitmap;

		إذا (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			اذهب skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		إذا (retval)
			اذهب errout;

		إذا (i == fs->group_desc_کيراءن - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_اڠک_B2_C(fs,
				((ext2fs_blocks_کيراءن(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			إذا (nbits)
				لكل (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		إذا (retval)
			أرجع retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_blocks_کيراءن(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			إذا (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				اذهب errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		إذا (!do_inode)
			استمر;

		إذا (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			اذهب skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		إذا (retval)
			اذهب errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		إذا (retval)
			اذهب errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_blocks_کيراءن(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			إذا (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				اذهب errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	إذا (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	إذا (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	أرجع 0;
errout:
	إذا (inode_buf)
		ext2fs_free_mem(&inode_buf);
	إذا (block_buf)
		ext2fs_free_mem(&block_buf);
	أرجع retval;
}

ساكن errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	لكل (i = 0; i < fs->group_desc_کيراءن; i++) {
		إذا (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			استمر;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		إذا (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_blocks_کيراءن(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_blocks_کيراءن(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	أرجع 0;
}

ساكن صحيح bitmap_tail_verify(unsigned حرف *bitmap, صحيح first, صحيح last)
{
	صحيح i;

	لكل (i = first; i <= last; i++)
		إذا (bitmap[i] != 0xff)
			أرجع 0;
	أرجع 1;
}

ساكن errcode_t read_bitmaps_range_prepare(ext2_filsys fs, صحيح flags)
{
	errcode_t retval;
	صحيح block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	حرف *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا ((block_nbytes > (صحيح) fs->blocksize) ||
	    (inode_nbytes > (صحيح) fs->blocksize))
		أرجع EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	إذا (retval)
		أرجع retval;

	إذا (flags & EXT2FS_BITMAPS_BLOCK) {
		إذا (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		إذا (retval)
			اذهب cleanup;
	}

	إذا (flags & EXT2FS_BITMAPS_INODE) {
		إذا (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		إذا (retval)
			اذهب cleanup;
	}
	ext2fs_free_mem(&buf);
	أرجع retval;

cleanup:
	إذا (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	أرجع retval;
}

ساكن errcode_t read_bitmaps_range_start(ext2_filsys fs, صحيح flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  صحيح *tail_flags)
{
	dgrp_t i;
	حرف *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	صحيح block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	صحيح csum_flag;
	unsigned صحيح	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	إذا (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		block_nbytes = 0;
	}

	إذا (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		inode_nbytes = 0;
	}

	/* io should be null */
	إذا (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_کيراءن;
		طالما (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_کيراءن);
		طالما (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			إذا (retval)
				اذهب cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		اذهب cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	لكل (i = start; i <= end; i++) {
		إذا (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			إذا ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_کيراءن(fs->super)))
				blk = 0;
			إذا (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				إذا (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					اذهب cleanup;
				}
				/* verify block bitmap checksum */
				إذا (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					اذهب cleanup;
				}
				إذا (!bitmap_tail_verify((unsigned حرف *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} وإلا
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			إذا (retval)
				اذهب cleanup;
			blk_itr += block_nbytes << 3;
		}
		إذا (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			إذا ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_کيراءن(fs->super)))
				blk = 0;
			إذا (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				إذا (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					اذهب cleanup;
				}

				/* verify inode bitmap checksum */
				إذا (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					اذهب cleanup;
				}
				إذا (!bitmap_tail_verify((unsigned حرف *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} وإلا
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			إذا (retval)
				اذهب cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	إذا (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	إذا (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	أرجع retval;
}

ساكن errcode_t read_bitmaps_range_end(ext2_filsys fs, صحيح flags,
					صحيح tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	إذا (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		إذا (retval)
			أرجع retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	أرجع 0;
}

ساكن فراغ read_bitmaps_cleanup_on_error(ext2_filsys fs, صحيح flags)
{
	إذا (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ساكن errcode_t read_bitmaps_range(ext2_filsys fs, صحيح flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	صحيح tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		أرجع retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  عدم, &tail_flags);
	إذا (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	أرجع retval;
}

#ifdef HAVE_PTHREAD
بنية read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	صحيح		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	صحيح		rbt_tail_flags;
};

ساكن فراغ *read_bitmaps_thread(فراغ *data)
{
	بنية read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	أرجع عدم;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحيح flags, صحيح اڠک_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = عدم;
	بنية read_bitmaps_thread_info *thread_infos = عدم;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	صحيح i, tail_flags = 0;
#endif

	إذا (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		أرجع EXT2_ET_INVALID_ARGUMENT;

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (flags & EXT2FS_BITMAPS_WRITE)
		أرجع write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	إذا (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (اڠک_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		اذهب fallback;

#إذا defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	إذا (اڠک_threads < 0)
		اڠک_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	إذا (اڠک_threads <= 0)
		اڠک_threads = 4;

	إذا ((unsigned) اڠک_threads > fs->group_desc_کيراءن)
		اڠک_threads = fs->group_desc_کيراءن;
	average_group = fs->group_desc_کيراءن / اڠک_threads;
	إذا (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	إذا ((اڠک_threads <= 1) || (average_group == 0))
		اذهب fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	إذا (retval)
		أرجع retval;

	thread_ids = calloc(حجم(pthread_t), اڠک_threads);
	إذا (!thread_ids)
		أرجع ENOMEM;

	thread_infos = calloc(حجم(بنية read_bitmaps_thread_info),
				اڠک_threads);
	إذا (!thread_infos)
		اذهب out;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		اذهب out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	لكل (i = 0; i < اڠک_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		إذا (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		وإلا
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		إذا (i == اڠک_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_کيراءن - 1;
		وإلا
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		إذا (retval)
			اكسر;
	}
	لكل (i = 0; i < اڠک_threads; i++) {
		إذا (!thread_ids[i])
			اكسر;
		rc = pthread_join(thread_ids[i], عدم);
		إذا (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		إذا (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	إذا (rc && !retval)
		retval = rc;
	حرر(thread_infos);
	حرر(thread_ids);

	إذا (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	أرجع retval;
fallback:
#endif /* HAVE_PTHREAD */
	أرجع read_bitmaps_range(fs, flags, 0, fs->group_desc_کيراءن - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	أرجع ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	أرجع ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	أرجع write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	أرجع write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	صحيح flags = 0;

	إذا (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	إذا (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	إذا (flags == 0)
		أرجع 0;
	أرجع ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	صحيح do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	صحيح do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	إذا (!do_inode && !do_block)
		أرجع 0;

	أرجع write_bitmaps(fs, do_inode, do_block);
}
