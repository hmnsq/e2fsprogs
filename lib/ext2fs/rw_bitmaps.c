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
#जर HAVE_UNISTD_H
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
प्रकारव्याख्या pthread_mutex_t mutex_t;

स्थिर शून्य unix_pthread_mutex_lock(mutex_t *mutex)
{
	जर (mutex)
		pthread_mutex_lock(mutex);
}
स्थिर शून्य unix_pthread_mutex_unlock(mutex_t *mutex)
{
	जर (mutex)
		pthread_mutex_unlock(mutex);
}
#अन्यथा
प्रकारव्याख्या पूर्णांक mutex_t;
#define unix_pthread_mutex_lock(mutex_t) करा {} पर्यंत (0)
#define unix_pthread_mutex_unlock(mutex_t) करा {} पर्यंत (0)
#endif

स्थिर errcode_t write_bitmaps(ext2_filsys fs, पूर्णांक do_inode, पूर्णांक do_block)
{
	dgrp_t 		i;
	अचिन्हित पूर्णांक	j;
	पूर्णांक		block_nbytes, inode_nbytes;
	अचिन्हित पूर्णांक	nbits;
	errcode_t	retval;
	अक्षर		*block_buf = रिक्त, *inode_buf = रिक्त;
	पूर्णांक		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर (!(fs->flags & EXT2_FLAG_RW))
		परत EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	जर (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		जर (retval)
			जा errout;
		स_रखो(block_buf, 0xff, fs->blocksize);
	}
	जर (do_inode) {
		inode_nbytes = (माप_प्रकार)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		जर (retval)
			जा errout;
		स_रखो(inode_buf, 0xff, fs->blocksize);
	}

	साठी (i = 0; i < fs->group_desc_मोजणी; i++) {
		जर (!do_block)
			जा skip_block_bitmap;

		जर (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			जा skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		जर (retval)
			जा errout;

		जर (i == fs->group_desc_मोजणी - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_अंक_B2_C(fs,
				((ext2fs_blocks_मोजणी(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			जर (nbits)
				साठी (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		जर (retval)
			परत retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		जर (blk && blk < ext2fs_blocks_मोजणी(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			जर (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				जा errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		जर (!do_inode)
			सुरूठेवा;

		जर (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			जा skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		जर (retval)
			जा errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		जर (retval)
			जा errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		जर (blk && blk < ext2fs_blocks_मोजणी(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			जर (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				जा errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	जर (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	जर (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	परत 0;
errout:
	जर (inode_buf)
		ext2fs_free_mem(&inode_buf);
	जर (block_buf)
		ext2fs_free_mem(&block_buf);
	परत retval;
}

स्थिर errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	साठी (i = 0; i < fs->group_desc_मोजणी; i++) {
		जर (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			सुरूठेवा;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		जर (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		जर (blk && blk < ext2fs_blocks_मोजणी(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		जर (blk && blk < ext2fs_blocks_मोजणी(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	परत 0;
}

स्थिर पूर्णांक bitmap_tail_verify(अचिन्हित अक्षर *bitmap, पूर्णांक first, पूर्णांक last)
{
	पूर्णांक i;

	साठी (i = first; i <= last; i++)
		जर (bitmap[i] != 0xff)
			परत 0;
	परत 1;
}

स्थिर errcode_t read_bitmaps_range_prepare(ext2_filsys fs, पूर्णांक flags)
{
	errcode_t retval;
	पूर्णांक block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	पूर्णांक inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	अक्षर *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर ((block_nbytes > (पूर्णांक) fs->blocksize) ||
	    (inode_nbytes > (पूर्णांक) fs->blocksize))
		परत EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(म_माप(fs->device_name) + 80, &buf);
	जर (retval)
		परत retval;

	जर (flags & EXT2FS_BITMAPS_BLOCK) {
		जर (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		म_नकल(buf, "block bitmap for ");
		म_जोड़ो(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		जर (retval)
			जा cleanup;
	}

	जर (flags & EXT2FS_BITMAPS_INODE) {
		जर (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		म_नकल(buf, "inode bitmap for ");
		म_जोड़ो(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		जर (retval)
			जा cleanup;
	}
	ext2fs_free_mem(&buf);
	परत retval;

cleanup:
	जर (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	जर (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	परत retval;
}

स्थिर errcode_t read_bitmaps_range_start(ext2_filsys fs, पूर्णांक flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  पूर्णांक *tail_flags)
{
	dgrp_t i;
	अक्षर *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	पूर्णांक block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	पूर्णांक inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	पूर्णांक csum_flag;
	अचिन्हित पूर्णांक	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	जर (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		जर (retval)
			जा cleanup;
	} अन्यथा {
		block_nbytes = 0;
	}

	जर (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		जर (retval)
			जा cleanup;
	} अन्यथा {
		inode_nbytes = 0;
	}

	/* io should be null */
	जर (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_मोजणी;
		पर्यंत (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			जर (retval)
				जा cleanup;
			cnt = fs->blocksize << 3;
			जर (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			जर (retval)
				जा cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_मोजणी);
		पर्यंत (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			जर (retval)
				जा cleanup;
			cnt = fs->blocksize << 3;
			जर (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			जर (retval)
				जा cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		जा cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	साठी (i = start; i <= end; i++) {
		जर (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			जर ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_मोजणी(fs->super)))
				blk = 0;
			जर (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				जर (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					जा cleanup;
				}
				/* verify block bitmap checksum */
				जर (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					जा cleanup;
				}
				जर (!bitmap_tail_verify((अचिन्हित अक्षर *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} अन्यथा
				स_रखो(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			जर (retval)
				जा cleanup;
			blk_itr += block_nbytes << 3;
		}
		जर (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			जर ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_मोजणी(fs->super)))
				blk = 0;
			जर (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				जर (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					जा cleanup;
				}

				/* verify inode bitmap checksum */
				जर (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					जा cleanup;
				}
				जर (!bitmap_tail_verify((अचिन्हित अक्षर *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} अन्यथा
				स_रखो(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			जर (retval)
				जा cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	जर (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	जर (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	परत retval;
}

स्थिर errcode_t read_bitmaps_range_end(ext2_filsys fs, पूर्णांक flags,
					पूर्णांक tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	जर (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		जर (retval)
			परत retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	जर (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	परत 0;
}

स्थिर शून्य read_bitmaps_cleanup_on_error(ext2_filsys fs, पूर्णांक flags)
{
	जर (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	जर (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

स्थिर errcode_t read_bitmaps_range(ext2_filsys fs, पूर्णांक flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	पूर्णांक tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	जर (retval)
		परत retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  रिक्त, &tail_flags);
	जर (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	जर (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	परत retval;
}

#ifdef HAVE_PTHREAD
संरचना read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	पूर्णांक		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	पूर्णांक		rbt_tail_flags;
};

स्थिर शून्य *read_bitmaps_thread(शून्य *data)
{
	संरचना read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	परत रिक्त;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, पूर्णांक flags, पूर्णांक अंक_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = रिक्त;
	संरचना read_bitmaps_thread_info *thread_infos = रिक्त;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	अचिन्हित flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	पूर्णांक i, tail_flags = 0;
#endif

	जर (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		परत EXT2_ET_INVALID_ARGUMENT;

	जर (ext2fs_has_feature_journal_dev(fs->super))
		परत EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	जर (flags & EXT2FS_BITMAPS_WRITE)
		परत write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	जर (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (अंक_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		जा fallback;

#जर defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	जर (अंक_threads < 0)
		अंक_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	जर (अंक_threads <= 0)
		अंक_threads = 4;

	जर ((अचिन्हित) अंक_threads > fs->group_desc_मोजणी)
		अंक_threads = fs->group_desc_मोजणी;
	average_group = fs->group_desc_मोजणी / अंक_threads;
	जर (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	जर ((अंक_threads <= 1) || (average_group == 0))
		जा fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	जर (retval)
		परत retval;

	thread_ids = सुस्मृति(आकार(pthread_t), अंक_threads);
	जर (!thread_ids)
		परत ENOMEM;

	thread_infos = सुस्मृति(आकार(संरचना read_bitmaps_thread_info),
				अंक_threads);
	जर (!thread_infos)
		जा out;

	retval = read_bitmaps_range_prepare(fs, flags);
	जर (retval)
		जा out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	साठी (i = 0; i < अंक_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		जर (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		अन्यथा
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		जर (i == अंक_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_मोजणी - 1;
		अन्यथा
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		जर (retval)
			थांबा;
	}
	साठी (i = 0; i < अंक_threads; i++) {
		जर (!thread_ids[i])
			थांबा;
		rc = pthread_join(thread_ids[i], रिक्त);
		जर (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		जर (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	जर (rc && !retval)
		retval = rc;
	मुक्त(thread_infos);
	मुक्त(thread_ids);

	जर (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	जर (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	परत retval;
fallback:
#endif /* HAVE_PTHREAD */
	परत read_bitmaps_range(fs, flags, 0, fs->group_desc_मोजणी - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	परत ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	परत ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	परत write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	परत write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	पूर्णांक flags = 0;

	जर (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	जर (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	जर (flags == 0)
		परत 0;
	परत ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	पूर्णांक do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	पूर्णांक do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	जर (!do_inode && !do_block)
		परत 0;

	परत write_bitmaps(fs, do_inode, do_block);
}
