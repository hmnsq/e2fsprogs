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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#ifdef HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#অন্তর্ভাব <pthread.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "e2image.h"

#ifdef HAVE_PTHREAD
প্রকারনির্দেশ pthread_mutex_t mutex_t;

স্থির শূন্য unix_pthread_mutex_lock(mutex_t *mutex)
{
	যদি (mutex)
		pthread_mutex_lock(mutex);
}
স্থির শূন্য unix_pthread_mutex_unlock(mutex_t *mutex)
{
	যদি (mutex)
		pthread_mutex_unlock(mutex);
}
#অন্যথা
প্রকারনির্দেশ পূর্ণাঙ্ক mutex_t;
#define unix_pthread_mutex_lock(mutex_t) কুরু {} যাবত্ (0)
#define unix_pthread_mutex_unlock(mutex_t) কুরু {} যাবত্ (0)
#endif

স্থির errcode_t write_bitmaps(ext2_filsys fs, পূর্ণাঙ্ক do_inode, পূর্ণাঙ্ক do_block)
{
	dgrp_t 		i;
	unsigned পূর্ণাঙ্ক	j;
	পূর্ণাঙ্ক		block_nbytes, inode_nbytes;
	unsigned পূর্ণাঙ্ক	nbits;
	errcode_t	retval;
	বর্ণ		*block_buf = রিক্ত, *inode_buf = রিক্ত;
	পূর্ণাঙ্ক		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!(fs->flags & EXT2_FLAG_RW))
		প্রত্যাগম EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	যদি (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		যদি (retval)
			গচ্ছ errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	যদি (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		যদি (retval)
			গচ্ছ errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	প্রতি (i = 0; i < fs->group_desc_গুনতি; i++) {
		যদি (!do_block)
			গচ্ছ skip_block_bitmap;

		যদি (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			গচ্ছ skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		যদি (retval)
			গচ্ছ errout;

		যদি (i == fs->group_desc_গুনতি - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_অঙ্ক_B2_C(fs,
				((ext2fs_blocks_গুনতি(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			যদি (nbits)
				প্রতি (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		যদি (retval)
			প্রত্যাগম retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		যদি (blk && blk < ext2fs_blocks_গুনতি(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			যদি (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				গচ্ছ errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		যদি (!do_inode)
			অনুবর্ত;

		যদি (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			গচ্ছ skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		যদি (retval)
			গচ্ছ errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		যদি (retval)
			গচ্ছ errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		যদি (blk && blk < ext2fs_blocks_গুনতি(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			যদি (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				গচ্ছ errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	যদি (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	যদি (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	প্রত্যাগম 0;
errout:
	যদি (inode_buf)
		ext2fs_free_mem(&inode_buf);
	যদি (block_buf)
		ext2fs_free_mem(&block_buf);
	প্রত্যাগম retval;
}

স্থির errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	প্রতি (i = 0; i < fs->group_desc_গুনতি; i++) {
		যদি (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			অনুবর্ত;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		যদি (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		যদি (blk && blk < ext2fs_blocks_গুনতি(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		যদি (blk && blk < ext2fs_blocks_গুনতি(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক bitmap_tail_verify(unsigned বর্ণ *bitmap, পূর্ণাঙ্ক first, পূর্ণাঙ্ক last)
{
	পূর্ণাঙ্ক i;

	প্রতি (i = first; i <= last; i++)
		যদি (bitmap[i] != 0xff)
			প্রত্যাগম 0;
	প্রত্যাগম 1;
}

স্থির errcode_t read_bitmaps_range_prepare(ext2_filsys fs, পূর্ণাঙ্ক flags)
{
	errcode_t retval;
	পূর্ণাঙ্ক block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	পূর্ণাঙ্ক inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	বর্ণ *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি ((block_nbytes > (পূর্ণাঙ্ক) fs->blocksize) ||
	    (inode_nbytes > (পূর্ণাঙ্ক) fs->blocksize))
		প্রত্যাগম EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	যদি (retval)
		প্রত্যাগম retval;

	যদি (flags & EXT2FS_BITMAPS_BLOCK) {
		যদি (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		যদি (retval)
			গচ্ছ cleanup;
	}

	যদি (flags & EXT2FS_BITMAPS_INODE) {
		যদি (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		যদি (retval)
			গচ্ছ cleanup;
	}
	ext2fs_free_mem(&buf);
	প্রত্যাগম retval;

cleanup:
	যদি (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	যদি (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	প্রত্যাগম retval;
}

স্থির errcode_t read_bitmaps_range_start(ext2_filsys fs, পূর্ণাঙ্ক flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  পূর্ণাঙ্ক *tail_flags)
{
	dgrp_t i;
	বর্ণ *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	পূর্ণাঙ্ক block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	পূর্ণাঙ্ক inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	পূর্ণাঙ্ক csum_flag;
	unsigned পূর্ণাঙ্ক	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	যদি (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		যদি (retval)
			গচ্ছ cleanup;
	} অন্যথা {
		block_nbytes = 0;
	}

	যদি (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		যদি (retval)
			গচ্ছ cleanup;
	} অন্যথা {
		inode_nbytes = 0;
	}

	/* io should be null */
	যদি (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_গুনতি;
		যাবত্ (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			যদি (retval)
				গচ্ছ cleanup;
			cnt = fs->blocksize << 3;
			যদি (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			যদি (retval)
				গচ্ছ cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_গুনতি);
		যাবত্ (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			যদি (retval)
				গচ্ছ cleanup;
			cnt = fs->blocksize << 3;
			যদি (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			যদি (retval)
				গচ্ছ cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		গচ্ছ cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	প্রতি (i = start; i <= end; i++) {
		যদি (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			যদি ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_গুনতি(fs->super)))
				blk = 0;
			যদি (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				যদি (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					গচ্ছ cleanup;
				}
				/* verify block bitmap checksum */
				যদি (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					গচ্ছ cleanup;
				}
				যদি (!bitmap_tail_verify((unsigned বর্ণ *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} অন্যথা
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			যদি (retval)
				গচ্ছ cleanup;
			blk_itr += block_nbytes << 3;
		}
		যদি (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			যদি ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_গুনতি(fs->super)))
				blk = 0;
			যদি (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				যদি (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					গচ্ছ cleanup;
				}

				/* verify inode bitmap checksum */
				যদি (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					গচ্ছ cleanup;
				}
				যদি (!bitmap_tail_verify((unsigned বর্ণ *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} অন্যথা
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			যদি (retval)
				গচ্ছ cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	যদি (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	যদি (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	প্রত্যাগম retval;
}

স্থির errcode_t read_bitmaps_range_end(ext2_filsys fs, পূর্ণাঙ্ক flags,
					পূর্ণাঙ্ক tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	যদি (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		যদি (retval)
			প্রত্যাগম retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	যদি (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	প্রত্যাগম 0;
}

স্থির শূন্য read_bitmaps_cleanup_on_error(ext2_filsys fs, পূর্ণাঙ্ক flags)
{
	যদি (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	যদি (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

স্থির errcode_t read_bitmaps_range(ext2_filsys fs, পূর্ণাঙ্ক flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	পূর্ণাঙ্ক tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	যদি (retval)
		প্রত্যাগম retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  রিক্ত, &tail_flags);
	যদি (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	যদি (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	প্রত্যাগম retval;
}

#ifdef HAVE_PTHREAD
সংরচনা read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	পূর্ণাঙ্ক		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	পূর্ণাঙ্ক		rbt_tail_flags;
};

স্থির শূন্য *read_bitmaps_thread(শূন্য *data)
{
	সংরচনা read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	প্রত্যাগম রিক্ত;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক অঙ্ক_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = রিক্ত;
	সংরচনা read_bitmaps_thread_info *thread_infos = রিক্ত;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	পূর্ণাঙ্ক i, tail_flags = 0;
#endif

	যদি (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		প্রত্যাগম EXT2_ET_INVALID_ARGUMENT;

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		প্রত্যাগম EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	যদি (flags & EXT2FS_BITMAPS_WRITE)
		প্রত্যাগম write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	যদি (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (অঙ্ক_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		গচ্ছ fallback;

#যদি defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	যদি (অঙ্ক_threads < 0)
		অঙ্ক_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	যদি (অঙ্ক_threads <= 0)
		অঙ্ক_threads = 4;

	যদি ((unsigned) অঙ্ক_threads > fs->group_desc_গুনতি)
		অঙ্ক_threads = fs->group_desc_গুনতি;
	average_group = fs->group_desc_গুনতি / অঙ্ক_threads;
	যদি (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	যদি ((অঙ্ক_threads <= 1) || (average_group == 0))
		গচ্ছ fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	যদি (retval)
		প্রত্যাগম retval;

	thread_ids = calloc(পরিমাণ(pthread_t), অঙ্ক_threads);
	যদি (!thread_ids)
		প্রত্যাগম ENOMEM;

	thread_infos = calloc(পরিমাণ(সংরচনা read_bitmaps_thread_info),
				অঙ্ক_threads);
	যদি (!thread_infos)
		গচ্ছ out;

	retval = read_bitmaps_range_prepare(fs, flags);
	যদি (retval)
		গচ্ছ out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	প্রতি (i = 0; i < অঙ্ক_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		যদি (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		অন্যথা
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		যদি (i == অঙ্ক_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_গুনতি - 1;
		অন্যথা
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		যদি (retval)
			ভঙ্গ;
	}
	প্রতি (i = 0; i < অঙ্ক_threads; i++) {
		যদি (!thread_ids[i])
			ভঙ্গ;
		rc = pthread_join(thread_ids[i], রিক্ত);
		যদি (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		যদি (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	যদি (rc && !retval)
		retval = rc;
	মুক্ত(thread_infos);
	মুক্ত(thread_ids);

	যদি (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	যদি (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	প্রত্যাগম retval;
fallback:
#endif /* HAVE_PTHREAD */
	প্রত্যাগম read_bitmaps_range(fs, flags, 0, fs->group_desc_গুনতি - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	প্রত্যাগম ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	প্রত্যাগম ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	প্রত্যাগম write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	প্রত্যাগম write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	পূর্ণাঙ্ক flags = 0;

	যদি (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	যদি (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	যদি (flags == 0)
		প্রত্যাগম 0;
	প্রত্যাগম ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	পূর্ণাঙ্ক do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	পূর্ণাঙ্ক do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	যদি (!do_inode && !do_block)
		প্রত্যাগম 0;

	প্রত্যাগম write_bitmaps(fs, do_inode, do_block);
}
