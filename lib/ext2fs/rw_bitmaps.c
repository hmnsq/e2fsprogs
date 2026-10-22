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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#ifdef HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#અન્તર્ભાવ <pthread.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "e2image.h"

#ifdef HAVE_PTHREAD
પ્રકારવ્યાખ્યા pthread_mutex_t mutex_t;

સ્થિર શૂન્ય unix_pthread_mutex_lock(mutex_t *mutex)
{
	જો (mutex)
		pthread_mutex_lock(mutex);
}
સ્થિર શૂન્ય unix_pthread_mutex_unlock(mutex_t *mutex)
{
	જો (mutex)
		pthread_mutex_unlock(mutex);
}
#નહીંતર
પ્રકારવ્યાખ્યા પૂર્ણાંક mutex_t;
#define unix_pthread_mutex_lock(mutex_t) કરો {} જ્યારે (0)
#define unix_pthread_mutex_unlock(mutex_t) કરો {} જ્યારે (0)
#endif

સ્થિર errcode_t write_bitmaps(ext2_filsys fs, પૂર્ણાંક do_inode, પૂર્ણાંક do_block)
{
	dgrp_t 		i;
	unsigned પૂર્ણાંક	j;
	પૂર્ણાંક		block_nbytes, inode_nbytes;
	unsigned પૂર્ણાંક	nbits;
	errcode_t	retval;
	અક્ષર		*block_buf = ખાલી, *inode_buf = ખાલી;
	પૂર્ણાંક		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (!(fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	જો (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		જો (retval)
			જાઓ errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	જો (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		જો (retval)
			જાઓ errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	માટે (i = 0; i < fs->group_desc_ગણતરી; i++) {
		જો (!do_block)
			જાઓ skip_block_bitmap;

		જો (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			જાઓ skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		જો (retval)
			જાઓ errout;

		જો (i == fs->group_desc_ગણતરી - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_અંક_B2_C(fs,
				((ext2fs_blocks_ગણતરી(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			જો (nbits)
				માટે (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		જો (retval)
			પાછા retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		જો (blk && blk < ext2fs_blocks_ગણતરી(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			જો (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				જાઓ errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		જો (!do_inode)
			ચાલુ;

		જો (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			જાઓ skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		જો (retval)
			જાઓ errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		જો (retval)
			જાઓ errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		જો (blk && blk < ext2fs_blocks_ગણતરી(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			જો (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				જાઓ errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	જો (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	જો (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	પાછા 0;
errout:
	જો (inode_buf)
		ext2fs_free_mem(&inode_buf);
	જો (block_buf)
		ext2fs_free_mem(&block_buf);
	પાછા retval;
}

સ્થિર errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	માટે (i = 0; i < fs->group_desc_ગણતરી; i++) {
		જો (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			ચાલુ;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		જો (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		જો (blk && blk < ext2fs_blocks_ગણતરી(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		જો (blk && blk < ext2fs_blocks_ગણતરી(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક bitmap_tail_verify(unsigned અક્ષર *bitmap, પૂર્ણાંક first, પૂર્ણાંક last)
{
	પૂર્ણાંક i;

	માટે (i = first; i <= last; i++)
		જો (bitmap[i] != 0xff)
			પાછા 0;
	પાછા 1;
}

સ્થિર errcode_t read_bitmaps_range_prepare(ext2_filsys fs, પૂર્ણાંક flags)
{
	errcode_t retval;
	પૂર્ણાંક block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	પૂર્ણાંક inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	અક્ષર *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો ((block_nbytes > (પૂર્ણાંક) fs->blocksize) ||
	    (inode_nbytes > (પૂર્ણાંક) fs->blocksize))
		પાછા EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	જો (retval)
		પાછા retval;

	જો (flags & EXT2FS_BITMAPS_BLOCK) {
		જો (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		જો (retval)
			જાઓ cleanup;
	}

	જો (flags & EXT2FS_BITMAPS_INODE) {
		જો (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		જો (retval)
			જાઓ cleanup;
	}
	ext2fs_free_mem(&buf);
	પાછા retval;

cleanup:
	જો (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	જો (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	પાછા retval;
}

સ્થિર errcode_t read_bitmaps_range_start(ext2_filsys fs, પૂર્ણાંક flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  પૂર્ણાંક *tail_flags)
{
	dgrp_t i;
	અક્ષર *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	પૂર્ણાંક block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	પૂર્ણાંક inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	પૂર્ણાંક csum_flag;
	unsigned પૂર્ણાંક	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	જો (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		જો (retval)
			જાઓ cleanup;
	} નહીંતર {
		block_nbytes = 0;
	}

	જો (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		જો (retval)
			જાઓ cleanup;
	} નહીંતર {
		inode_nbytes = 0;
	}

	/* io should be null */
	જો (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_ગણતરી;
		જ્યારે (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			જો (retval)
				જાઓ cleanup;
			cnt = fs->blocksize << 3;
			જો (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			જો (retval)
				જાઓ cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_ગણતરી);
		જ્યારે (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			જો (retval)
				જાઓ cleanup;
			cnt = fs->blocksize << 3;
			જો (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			જો (retval)
				જાઓ cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		જાઓ cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	માટે (i = start; i <= end; i++) {
		જો (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			જો ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_ગણતરી(fs->super)))
				blk = 0;
			જો (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				જો (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					જાઓ cleanup;
				}
				/* verify block bitmap checksum */
				જો (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					જાઓ cleanup;
				}
				જો (!bitmap_tail_verify((unsigned અક્ષર *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} નહીંતર
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			જો (retval)
				જાઓ cleanup;
			blk_itr += block_nbytes << 3;
		}
		જો (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			જો ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_ગણતરી(fs->super)))
				blk = 0;
			જો (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				જો (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					જાઓ cleanup;
				}

				/* verify inode bitmap checksum */
				જો (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					જાઓ cleanup;
				}
				જો (!bitmap_tail_verify((unsigned અક્ષર *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} નહીંતર
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			જો (retval)
				જાઓ cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	જો (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	જો (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	પાછા retval;
}

સ્થિર errcode_t read_bitmaps_range_end(ext2_filsys fs, પૂર્ણાંક flags,
					પૂર્ણાંક tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	જો (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		જો (retval)
			પાછા retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	જો (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	પાછા 0;
}

સ્થિર શૂન્ય read_bitmaps_cleanup_on_error(ext2_filsys fs, પૂર્ણાંક flags)
{
	જો (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	જો (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

સ્થિર errcode_t read_bitmaps_range(ext2_filsys fs, પૂર્ણાંક flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	પૂર્ણાંક tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	જો (retval)
		પાછા retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  ખાલી, &tail_flags);
	જો (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	જો (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	પાછા retval;
}

#ifdef HAVE_PTHREAD
સંરચના read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	પૂર્ણાંક		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	પૂર્ણાંક		rbt_tail_flags;
};

સ્થિર શૂન્ય *read_bitmaps_thread(શૂન્ય *data)
{
	સંરચના read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	પાછા ખાલી;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, પૂર્ણાંક flags, પૂર્ણાંક અંક_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = ખાલી;
	સંરચના read_bitmaps_thread_info *thread_infos = ખાલી;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	પૂર્ણાંક i, tail_flags = 0;
#endif

	જો (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		પાછા EXT2_ET_INVALID_ARGUMENT;

	જો (ext2fs_has_feature_journal_dev(fs->super))
		પાછા EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	જો (flags & EXT2FS_BITMAPS_WRITE)
		પાછા write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	જો (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (અંક_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		જાઓ fallback;

#જો defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	જો (અંક_threads < 0)
		અંક_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	જો (અંક_threads <= 0)
		અંક_threads = 4;

	જો ((unsigned) અંક_threads > fs->group_desc_ગણતરી)
		અંક_threads = fs->group_desc_ગણતરી;
	average_group = fs->group_desc_ગણતરી / અંક_threads;
	જો (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	જો ((અંક_threads <= 1) || (average_group == 0))
		જાઓ fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	જો (retval)
		પાછા retval;

	thread_ids = calloc(કદ(pthread_t), અંક_threads);
	જો (!thread_ids)
		પાછા ENOMEM;

	thread_infos = calloc(કદ(સંરચના read_bitmaps_thread_info),
				અંક_threads);
	જો (!thread_infos)
		જાઓ out;

	retval = read_bitmaps_range_prepare(fs, flags);
	જો (retval)
		જાઓ out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	માટે (i = 0; i < અંક_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		જો (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		નહીંતર
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		જો (i == અંક_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_ગણતરી - 1;
		નહીંતર
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		જો (retval)
			તોડો;
	}
	માટે (i = 0; i < અંક_threads; i++) {
		જો (!thread_ids[i])
			તોડો;
		rc = pthread_join(thread_ids[i], ખાલી);
		જો (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		જો (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	જો (rc && !retval)
		retval = rc;
	મુક્ત(thread_infos);
	મુક્ત(thread_ids);

	જો (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	જો (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	પાછા retval;
fallback:
#endif /* HAVE_PTHREAD */
	પાછા read_bitmaps_range(fs, flags, 0, fs->group_desc_ગણતરી - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	પાછા ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	પાછા ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	પાછા write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	પાછા write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	પૂર્ણાંક flags = 0;

	જો (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	જો (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	જો (flags == 0)
		પાછા 0;
	પાછા ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	પૂર્ણાંક do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	પૂર્ણાંક do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	જો (!do_inode && !do_block)
		પાછા 0;

	પાછા write_bitmaps(fs, do_inode, do_block);
}
