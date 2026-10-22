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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#ifdef HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#अन्तर्भाव <pthread.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "e2image.h"

#ifdef HAVE_PTHREAD
प्रकारनिर्देश pthread_mutex_t mutex_t;

स्थिर शून्य unix_pthread_mutex_lock(mutex_t *mutex)
{
	यदि (mutex)
		pthread_mutex_lock(mutex);
}
स्थिर शून्य unix_pthread_mutex_unlock(mutex_t *mutex)
{
	यदि (mutex)
		pthread_mutex_unlock(mutex);
}
#अन्यथा
प्रकारनिर्देश पूर्णाङ्क mutex_t;
#define unix_pthread_mutex_lock(mutex_t) कुरु {} यावत् (0)
#define unix_pthread_mutex_unlock(mutex_t) कुरु {} यावत् (0)
#endif

स्थिर errcode_t write_bitmaps(ext2_filsys fs, पूर्णाङ्क do_inode, पूर्णाङ्क do_block)
{
	dgrp_t 		i;
	unsigned पूर्णाङ्क	j;
	पूर्णाङ्क		block_nbytes, inode_nbytes;
	unsigned पूर्णाङ्क	nbits;
	errcode_t	retval;
	वर्ण		*block_buf = रिक्त, *inode_buf = रिक्त;
	पूर्णाङ्क		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!(fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	यदि (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		यदि (retval)
			गच्छ errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	यदि (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		यदि (retval)
			गच्छ errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	प्रति (i = 0; i < fs->group_desc_गिनती; i++) {
		यदि (!do_block)
			गच्छ skip_block_bitmap;

		यदि (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			गच्छ skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		यदि (retval)
			गच्छ errout;

		यदि (i == fs->group_desc_गिनती - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_अंक_B2_C(fs,
				((ext2fs_blocks_गिनती(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			यदि (nbits)
				प्रति (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		यदि (retval)
			प्रत्यागम retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		यदि (blk && blk < ext2fs_blocks_गिनती(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			यदि (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				गच्छ errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		यदि (!do_inode)
			अनुवर्त;

		यदि (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			गच्छ skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		यदि (retval)
			गच्छ errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		यदि (retval)
			गच्छ errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		यदि (blk && blk < ext2fs_blocks_गिनती(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			यदि (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				गच्छ errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	यदि (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	यदि (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	प्रत्यागम 0;
errout:
	यदि (inode_buf)
		ext2fs_free_mem(&inode_buf);
	यदि (block_buf)
		ext2fs_free_mem(&block_buf);
	प्रत्यागम retval;
}

स्थिर errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	प्रति (i = 0; i < fs->group_desc_गिनती; i++) {
		यदि (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			अनुवर्त;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		यदि (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		यदि (blk && blk < ext2fs_blocks_गिनती(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		यदि (blk && blk < ext2fs_blocks_गिनती(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क bitmap_tail_verify(unsigned वर्ण *bitmap, पूर्णाङ्क first, पूर्णाङ्क last)
{
	पूर्णाङ्क i;

	प्रति (i = first; i <= last; i++)
		यदि (bitmap[i] != 0xff)
			प्रत्यागम 0;
	प्रत्यागम 1;
}

स्थिर errcode_t read_bitmaps_range_prepare(ext2_filsys fs, पूर्णाङ्क flags)
{
	errcode_t retval;
	पूर्णाङ्क block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	पूर्णाङ्क inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	वर्ण *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि ((block_nbytes > (पूर्णाङ्क) fs->blocksize) ||
	    (inode_nbytes > (पूर्णाङ्क) fs->blocksize))
		प्रत्यागम EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	यदि (retval)
		प्रत्यागम retval;

	यदि (flags & EXT2FS_BITMAPS_BLOCK) {
		यदि (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		यदि (retval)
			गच्छ cleanup;
	}

	यदि (flags & EXT2FS_BITMAPS_INODE) {
		यदि (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		यदि (retval)
			गच्छ cleanup;
	}
	ext2fs_free_mem(&buf);
	प्रत्यागम retval;

cleanup:
	यदि (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	यदि (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

स्थिर errcode_t read_bitmaps_range_start(ext2_filsys fs, पूर्णाङ्क flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  पूर्णाङ्क *tail_flags)
{
	dgrp_t i;
	वर्ण *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	पूर्णाङ्क block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	पूर्णाङ्क inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	पूर्णाङ्क csum_flag;
	unsigned पूर्णाङ्क	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	यदि (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा {
		block_nbytes = 0;
	}

	यदि (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा {
		inode_nbytes = 0;
	}

	/* io should be null */
	यदि (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_गिनती;
		यावत् (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			यदि (retval)
				गच्छ cleanup;
			cnt = fs->blocksize << 3;
			यदि (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			यदि (retval)
				गच्छ cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_गिनती);
		यावत् (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			यदि (retval)
				गच्छ cleanup;
			cnt = fs->blocksize << 3;
			यदि (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			यदि (retval)
				गच्छ cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		गच्छ cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	प्रति (i = start; i <= end; i++) {
		यदि (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			यदि ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_गिनती(fs->super)))
				blk = 0;
			यदि (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				यदि (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					गच्छ cleanup;
				}
				/* verify block bitmap checksum */
				यदि (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					गच्छ cleanup;
				}
				यदि (!bitmap_tail_verify((unsigned वर्ण *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} अन्यथा
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			यदि (retval)
				गच्छ cleanup;
			blk_itr += block_nbytes << 3;
		}
		यदि (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			यदि ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_गिनती(fs->super)))
				blk = 0;
			यदि (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				यदि (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					गच्छ cleanup;
				}

				/* verify inode bitmap checksum */
				यदि (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					गच्छ cleanup;
				}
				यदि (!bitmap_tail_verify((unsigned वर्ण *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} अन्यथा
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			यदि (retval)
				गच्छ cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	यदि (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	यदि (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	प्रत्यागम retval;
}

स्थिर errcode_t read_bitmaps_range_end(ext2_filsys fs, पूर्णाङ्क flags,
					पूर्णाङ्क tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	यदि (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		यदि (retval)
			प्रत्यागम retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	यदि (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	प्रत्यागम 0;
}

स्थिर शून्य read_bitmaps_cleanup_on_error(ext2_filsys fs, पूर्णाङ्क flags)
{
	यदि (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	यदि (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

स्थिर errcode_t read_bitmaps_range(ext2_filsys fs, पूर्णाङ्क flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	पूर्णाङ्क tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	यदि (retval)
		प्रत्यागम retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  रिक्त, &tail_flags);
	यदि (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	यदि (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	प्रत्यागम retval;
}

#ifdef HAVE_PTHREAD
संरचना read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	पूर्णाङ्क		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	पूर्णाङ्क		rbt_tail_flags;
};

स्थिर शून्य *read_bitmaps_thread(शून्य *data)
{
	संरचना read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	प्रत्यागम रिक्त;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, पूर्णाङ्क flags, पूर्णाङ्क अंक_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = रिक्त;
	संरचना read_bitmaps_thread_info *thread_infos = रिक्त;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	पूर्णाङ्क i, tail_flags = 0;
#endif

	यदि (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	यदि (flags & EXT2FS_BITMAPS_WRITE)
		प्रत्यागम write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	यदि (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (अंक_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		गच्छ fallback;

#यदि defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	यदि (अंक_threads < 0)
		अंक_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	यदि (अंक_threads <= 0)
		अंक_threads = 4;

	यदि ((unsigned) अंक_threads > fs->group_desc_गिनती)
		अंक_threads = fs->group_desc_गिनती;
	average_group = fs->group_desc_गिनती / अंक_threads;
	यदि (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	यदि ((अंक_threads <= 1) || (average_group == 0))
		गच्छ fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	यदि (retval)
		प्रत्यागम retval;

	thread_ids = calloc(परिमाण(pthread_t), अंक_threads);
	यदि (!thread_ids)
		प्रत्यागम ENOMEM;

	thread_infos = calloc(परिमाण(संरचना read_bitmaps_thread_info),
				अंक_threads);
	यदि (!thread_infos)
		गच्छ out;

	retval = read_bitmaps_range_prepare(fs, flags);
	यदि (retval)
		गच्छ out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	प्रति (i = 0; i < अंक_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		यदि (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		अन्यथा
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		यदि (i == अंक_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_गिनती - 1;
		अन्यथा
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		यदि (retval)
			भङ्ग;
	}
	प्रति (i = 0; i < अंक_threads; i++) {
		यदि (!thread_ids[i])
			भङ्ग;
		rc = pthread_join(thread_ids[i], रिक्त);
		यदि (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		यदि (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	यदि (rc && !retval)
		retval = rc;
	मुक्त(thread_infos);
	मुक्त(thread_ids);

	यदि (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	यदि (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	प्रत्यागम retval;
fallback:
#endif /* HAVE_PTHREAD */
	प्रत्यागम read_bitmaps_range(fs, flags, 0, fs->group_desc_गिनती - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	प्रत्यागम ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	प्रत्यागम ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	प्रत्यागम write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	प्रत्यागम write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	पूर्णाङ्क flags = 0;

	यदि (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	यदि (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	यदि (flags == 0)
		प्रत्यागम 0;
	प्रत्यागम ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	पूर्णाङ्क do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	पूर्णाङ्क do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	यदि (!do_inode && !do_block)
		प्रत्यागम 0;

	प्रत्यागम write_bitmaps(fs, do_inode, do_block);
}
