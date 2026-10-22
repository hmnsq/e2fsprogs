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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#ifdef HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#அந்தர்பாவ <pthread.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "e2image.h"

#ifdef HAVE_PTHREAD
வகைவரை pthread_mutex_t mutex_t;

நிலையான வெறுமை unix_pthread_mutex_lock(mutex_t *mutex)
{
	எனில் (mutex)
		pthread_mutex_lock(mutex);
}
நிலையான வெறுமை unix_pthread_mutex_unlock(mutex_t *mutex)
{
	எனில் (mutex)
		pthread_mutex_unlock(mutex);
}
#இல்லையெனில்
வகைவரை முழு mutex_t;
#define unix_pthread_mutex_lock(mutex_t) செய் {} போது (0)
#define unix_pthread_mutex_unlock(mutex_t) செய் {} போது (0)
#endif

நிலையான errcode_t write_bitmaps(ext2_filsys fs, முழு do_inode, முழு do_block)
{
	dgrp_t 		i;
	unsigned முழு	j;
	முழு		block_nbytes, inode_nbytes;
	unsigned முழு	nbits;
	errcode_t	retval;
	எழுத்து		*block_buf = வெற்று, *inode_buf = வெற்று;
	முழு		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (!(fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	எனில் (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		எனில் (retval)
			செல் errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	எனில் (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		எனில் (retval)
			செல் errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	க்கு (i = 0; i < fs->group_desc_மொத்தம்; i++) {
		எனில் (!do_block)
			செல் skip_block_bitmap;

		எனில் (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			செல் skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		எனில் (retval)
			செல் errout;

		எனில் (i == fs->group_desc_மொத்தம் - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_இலக்கம்_B2_C(fs,
				((ext2fs_blocks_மொத்தம்(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			எனில் (nbits)
				க்கு (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		எனில் (retval)
			திரும்பு retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		எனில் (blk && blk < ext2fs_blocks_மொத்தம்(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			எனில் (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				செல் errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		எனில் (!do_inode)
			தொடர்;

		எனில் (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			செல் skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		எனில் (retval)
			செல் errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		எனில் (retval)
			செல் errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		எனில் (blk && blk < ext2fs_blocks_மொத்தம்(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			எனில் (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				செல் errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	எனில் (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	எனில் (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	திரும்பு 0;
errout:
	எனில் (inode_buf)
		ext2fs_free_mem(&inode_buf);
	எனில் (block_buf)
		ext2fs_free_mem(&block_buf);
	திரும்பு retval;
}

நிலையான errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	க்கு (i = 0; i < fs->group_desc_மொத்தம்; i++) {
		எனில் (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			தொடர்;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		எனில் (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		எனில் (blk && blk < ext2fs_blocks_மொத்தம்(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		எனில் (blk && blk < ext2fs_blocks_மொத்தம்(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	திரும்பு 0;
}

நிலையான முழு bitmap_tail_verify(unsigned எழுத்து *bitmap, முழு first, முழு last)
{
	முழு i;

	க்கு (i = first; i <= last; i++)
		எனில் (bitmap[i] != 0xff)
			திரும்பு 0;
	திரும்பு 1;
}

நிலையான errcode_t read_bitmaps_range_prepare(ext2_filsys fs, முழு flags)
{
	errcode_t retval;
	முழு block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	முழு inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	எழுத்து *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் ((block_nbytes > (முழு) fs->blocksize) ||
	    (inode_nbytes > (முழு) fs->blocksize))
		திரும்பு EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	எனில் (retval)
		திரும்பு retval;

	எனில் (flags & EXT2FS_BITMAPS_BLOCK) {
		எனில் (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		எனில் (retval)
			செல் cleanup;
	}

	எனில் (flags & EXT2FS_BITMAPS_INODE) {
		எனில் (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		எனில் (retval)
			செல் cleanup;
	}
	ext2fs_free_mem(&buf);
	திரும்பு retval;

cleanup:
	எனில் (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	எனில் (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	திரும்பு retval;
}

நிலையான errcode_t read_bitmaps_range_start(ext2_filsys fs, முழு flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  முழு *tail_flags)
{
	dgrp_t i;
	எழுத்து *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	முழு block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	முழு inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	முழு csum_flag;
	unsigned முழு	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	எனில் (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		எனில் (retval)
			செல் cleanup;
	} இல்லையெனில் {
		block_nbytes = 0;
	}

	எனில் (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		எனில் (retval)
			செல் cleanup;
	} இல்லையெனில் {
		inode_nbytes = 0;
	}

	/* io should be null */
	எனில் (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_மொத்தம்;
		போது (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			எனில் (retval)
				செல் cleanup;
			cnt = fs->blocksize << 3;
			எனில் (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			எனில் (retval)
				செல் cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_மொத்தம்);
		போது (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			எனில் (retval)
				செல் cleanup;
			cnt = fs->blocksize << 3;
			எனில் (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			எனில் (retval)
				செல் cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		செல் cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	க்கு (i = start; i <= end; i++) {
		எனில் (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			எனில் ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_மொத்தம்(fs->super)))
				blk = 0;
			எனில் (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				எனில் (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					செல் cleanup;
				}
				/* verify block bitmap checksum */
				எனில் (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					செல் cleanup;
				}
				எனில் (!bitmap_tail_verify((unsigned எழுத்து *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} இல்லையெனில்
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			எனில் (retval)
				செல் cleanup;
			blk_itr += block_nbytes << 3;
		}
		எனில் (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			எனில் ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_மொத்தம்(fs->super)))
				blk = 0;
			எனில் (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				எனில் (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					செல் cleanup;
				}

				/* verify inode bitmap checksum */
				எனில் (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					செல் cleanup;
				}
				எனில் (!bitmap_tail_verify((unsigned எழுத்து *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} இல்லையெனில்
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			எனில் (retval)
				செல் cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	எனில் (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	எனில் (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	திரும்பு retval;
}

நிலையான errcode_t read_bitmaps_range_end(ext2_filsys fs, முழு flags,
					முழு tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	எனில் (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		எனில் (retval)
			திரும்பு retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	எனில் (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	திரும்பு 0;
}

நிலையான வெறுமை read_bitmaps_cleanup_on_error(ext2_filsys fs, முழு flags)
{
	எனில் (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	எனில் (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

நிலையான errcode_t read_bitmaps_range(ext2_filsys fs, முழு flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	முழு tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	எனில் (retval)
		திரும்பு retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  வெற்று, &tail_flags);
	எனில் (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	எனில் (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	திரும்பு retval;
}

#ifdef HAVE_PTHREAD
கட்டமைப்பு read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	முழு		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	முழு		rbt_tail_flags;
};

நிலையான வெறுமை *read_bitmaps_thread(வெறுமை *data)
{
	கட்டமைப்பு read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	திரும்பு வெற்று;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, முழு flags, முழு இலக்கம்_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = வெற்று;
	கட்டமைப்பு read_bitmaps_thread_info *thread_infos = வெற்று;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	முழு i, tail_flags = 0;
#endif

	எனில் (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	எனில் (ext2fs_has_feature_journal_dev(fs->super))
		திரும்பு EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	எனில் (flags & EXT2FS_BITMAPS_WRITE)
		திரும்பு write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	எனில் (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (இலக்கம்_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		செல் fallback;

#எனில் defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	எனில் (இலக்கம்_threads < 0)
		இலக்கம்_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	எனில் (இலக்கம்_threads <= 0)
		இலக்கம்_threads = 4;

	எனில் ((unsigned) இலக்கம்_threads > fs->group_desc_மொத்தம்)
		இலக்கம்_threads = fs->group_desc_மொத்தம்;
	average_group = fs->group_desc_மொத்தம் / இலக்கம்_threads;
	எனில் (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	எனில் ((இலக்கம்_threads <= 1) || (average_group == 0))
		செல் fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	எனில் (retval)
		திரும்பு retval;

	thread_ids = calloc(அளவு(pthread_t), இலக்கம்_threads);
	எனில் (!thread_ids)
		திரும்பு ENOMEM;

	thread_infos = calloc(அளவு(கட்டமைப்பு read_bitmaps_thread_info),
				இலக்கம்_threads);
	எனில் (!thread_infos)
		செல் out;

	retval = read_bitmaps_range_prepare(fs, flags);
	எனில் (retval)
		செல் out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	க்கு (i = 0; i < இலக்கம்_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		எனில் (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		இல்லையெனில்
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		எனில் (i == இலக்கம்_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_மொத்தம் - 1;
		இல்லையெனில்
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		எனில் (retval)
			நிறுத்து;
	}
	க்கு (i = 0; i < இலக்கம்_threads; i++) {
		எனில் (!thread_ids[i])
			நிறுத்து;
		rc = pthread_join(thread_ids[i], வெற்று);
		எனில் (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		எனில் (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	எனில் (rc && !retval)
		retval = rc;
	விடுவி(thread_infos);
	விடுவி(thread_ids);

	எனில் (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	எனில் (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	திரும்பு retval;
fallback:
#endif /* HAVE_PTHREAD */
	திரும்பு read_bitmaps_range(fs, flags, 0, fs->group_desc_மொத்தம் - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	திரும்பு ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	திரும்பு ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	திரும்பு write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	திரும்பு write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	முழு flags = 0;

	எனில் (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	எனில் (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	எனில் (flags == 0)
		திரும்பு 0;
	திரும்பு ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	முழு do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	முழு do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	எனில் (!do_inode && !do_block)
		திரும்பு 0;

	திரும்பு write_bitmaps(fs, do_inode, do_block);
}
