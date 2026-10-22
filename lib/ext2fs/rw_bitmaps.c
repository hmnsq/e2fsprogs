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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर_घोषित HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_PTHREAD_H
#समावेश <pthread.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "e2image.h"

#अगर_घोषित HAVE_PTHREAD
प्रकार pthread_mutex_t mutex_t;

अटल व्योम unix_pthread_mutex_lock(mutex_t *mutex)
{
	अगर (mutex)
		pthread_mutex_lock(mutex);
}
अटल व्योम unix_pthread_mutex_unlock(mutex_t *mutex)
{
	अगर (mutex)
		pthread_mutex_unlock(mutex);
}
#अन्यथा
प्रकार पूर्णांक mutex_t;
#घोषणा unix_pthread_mutex_lock(mutex_t) करो {} जबतक (0)
#घोषणा unix_pthread_mutex_unlock(mutex_t) करो {} जबतक (0)
#पूर्ण_अगर

अटल errcode_t write_bitmaps(ext2_filsys fs, पूर्णांक do_inode, पूर्णांक do_block)
{
	dgrp_t 		i;
	अचिन्हित पूर्णांक	j;
	पूर्णांक		block_nbytes, inode_nbytes;
	अचिन्हित पूर्णांक	nbits;
	errcode_t	retval;
	अक्षर		*block_buf = शून्य, *inode_buf = शून्य;
	पूर्णांक		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_तथ्य_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (!(fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	अगर (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		अगर (retval)
			जाओ errout;
		स_रखो(block_buf, 0xff, fs->blocksize);
	}
	अगर (do_inode) {
		inode_nbytes = (माप_प्रकार)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		अगर (retval)
			जाओ errout;
		स_रखो(inode_buf, 0xff, fs->blocksize);
	}

	क्रम (i = 0; i < fs->group_desc_गिनती; i++) {
		अगर (!do_block)
			जाओ skip_block_bitmap;

		अगर (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			जाओ skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		अगर (retval)
			जाओ errout;

		अगर (i == fs->group_desc_गिनती - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_अंक_B2_C(fs,
				((ext2fs_blocks_गिनती(fs->super)
				  - (__u64) fs->super->s_first_तथ्य_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			अगर (nbits)
				क्रम (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		अगर (retval)
			वापस retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		अगर (blk && blk < ext2fs_blocks_गिनती(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			अगर (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				जाओ errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		अगर (!do_inode)
			जारी;

		अगर (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			जाओ skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		अगर (retval)
			जाओ errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		अगर (retval)
			जाओ errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		अगर (blk && blk < ext2fs_blocks_गिनती(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			अगर (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				जाओ errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	अगर (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	अगर (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	वापस 0;
errout:
	अगर (inode_buf)
		ext2fs_free_mem(&inode_buf);
	अगर (block_buf)
		ext2fs_free_mem(&block_buf);
	वापस retval;
}

अटल errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	क्रम (i = 0; i < fs->group_desc_गिनती; i++) {
		अगर (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			जारी;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		अगर (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		अगर (blk && blk < ext2fs_blocks_गिनती(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		अगर (blk && blk < ext2fs_blocks_गिनती(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	वापस 0;
}

अटल पूर्णांक bitmap_पुच्छ_verify(अचिन्हित अक्षर *bitmap, पूर्णांक first, पूर्णांक last)
{
	पूर्णांक i;

	क्रम (i = first; i <= last; i++)
		अगर (bitmap[i] != 0xff)
			वापस 0;
	वापस 1;
}

अटल errcode_t read_bitmaps_range_prepare(ext2_filsys fs, पूर्णांक flags)
{
	errcode_t retval;
	पूर्णांक block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	पूर्णांक inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	अक्षर *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर ((block_nbytes > (पूर्णांक) fs->blocksize) ||
	    (inode_nbytes > (पूर्णांक) fs->blocksize))
		वापस EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(म_माप(fs->device_name) + 80, &buf);
	अगर (retval)
		वापस retval;

	अगर (flags & EXT2FS_BITMAPS_BLOCK) {
		अगर (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		म_नकल(buf, "block bitmap for ");
		म_जोड़ो(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		अगर (retval)
			जाओ cleanup;
	}

	अगर (flags & EXT2FS_BITMAPS_INODE) {
		अगर (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		म_नकल(buf, "inode bitmap for ");
		म_जोड़ो(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		अगर (retval)
			जाओ cleanup;
	}
	ext2fs_free_mem(&buf);
	वापस retval;

cleanup:
	अगर (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	अगर (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	वापस retval;
}

अटल errcode_t read_bitmaps_range_start(ext2_filsys fs, पूर्णांक flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  पूर्णांक *पुच्छ_flags)
{
	dgrp_t i;
	अक्षर *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	पूर्णांक block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	पूर्णांक inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	पूर्णांक csum_flag;
	अचिन्हित पूर्णांक	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_तथ्य_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	अगर (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		अगर (retval)
			जाओ cleanup;
	} अन्यथा {
		block_nbytes = 0;
	}

	अगर (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		अगर (retval)
			जाओ cleanup;
	} अन्यथा {
		inode_nbytes = 0;
	}

	/* io should be null */
	अगर (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_गिनती;
		जबतक (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			अगर (retval)
				जाओ cleanup;
			cnt = fs->blocksize << 3;
			अगर (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			अगर (retval)
				जाओ cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_गिनती);
		जबतक (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			अगर (retval)
				जाओ cleanup;
			cnt = fs->blocksize << 3;
			अगर (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			अगर (retval)
				जाओ cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		जाओ cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	क्रम (i = start; i <= end; i++) {
		अगर (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			अगर ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_गिनती(fs->super)))
				blk = 0;
			अगर (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				अगर (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					जाओ cleanup;
				}
				/* verify block bitmap checksum */
				अगर (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					जाओ cleanup;
				}
				अगर (!bitmap_पुच्छ_verify((अचिन्हित अक्षर *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*पुच्छ_flags |= EXT2_FLAG_BBITMAP_पुच्छ_PROBLEM;
			} अन्यथा
				स_रखो(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			अगर (retval)
				जाओ cleanup;
			blk_itr += block_nbytes << 3;
		}
		अगर (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			अगर ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_गिनती(fs->super)))
				blk = 0;
			अगर (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				अगर (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					जाओ cleanup;
				}

				/* verify inode bitmap checksum */
				अगर (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					जाओ cleanup;
				}
				अगर (!bitmap_पुच्छ_verify((अचिन्हित अक्षर *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*पुच्छ_flags |= EXT2_FLAG_IBITMAP_पुच्छ_PROBLEM;
			} अन्यथा
				स_रखो(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			अगर (retval)
				जाओ cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	अगर (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	अगर (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	वापस retval;
}

अटल errcode_t read_bitmaps_range_end(ext2_filsys fs, पूर्णांक flags,
					पूर्णांक पुच्छ_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	अगर (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		अगर (retval)
			वापस retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_पुच्छ_PROBLEM;
	}
	अगर (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_पुच्छ_PROBLEM;
	fs->flags |= पुच्छ_flags;

	वापस 0;
}

अटल व्योम read_bitmaps_cleanup_on_error(ext2_filsys fs, पूर्णांक flags)
{
	अगर (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	अगर (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

अटल errcode_t read_bitmaps_range(ext2_filsys fs, पूर्णांक flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	पूर्णांक पुच्छ_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	अगर (retval)
		वापस retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  शून्य, &पुच्छ_flags);
	अगर (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, पुच्छ_flags);
	अगर (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	वापस retval;
}

#अगर_घोषित HAVE_PTHREAD
काष्ठा read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	पूर्णांक		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	पूर्णांक		rbt_पुच्छ_flags;
};

अटल व्योम *read_bitmaps_thread(व्योम *तथ्य)
{
	काष्ठा read_bitmaps_thread_info *rbt = तथ्य;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_पुच्छ_flags);
	वापस शून्य;
}
#पूर्ण_अगर

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, पूर्णांक flags, पूर्णांक अंक_threads)
{
#अगर_घोषित HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = शून्य;
	काष्ठा read_bitmaps_thread_info *thread_infos = शून्य;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	अचिन्हित flexbg_आकार = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	पूर्णांक i, पुच्छ_flags = 0;
#पूर्ण_अगर

	अगर (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		वापस EXT2_ET_INVALID_ARGUMENT;

	अगर (ext2fs_has_feature_journal_dev(fs->super))
		वापस EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	अगर (flags & EXT2FS_BITMAPS_WRITE)
		वापस write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#अगर_घोषित HAVE_PTHREAD
	अगर (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (अंक_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		जाओ fallback;

#अगर defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	अगर (अंक_threads < 0)
		अंक_threads = sysconf(_SC_NPROCESSORS_CONF);
#पूर्ण_अगर
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	अगर (अंक_threads <= 0)
		अंक_threads = 4;

	अगर ((अचिन्हित) अंक_threads > fs->group_desc_गिनती)
		अंक_threads = fs->group_desc_गिनती;
	average_group = fs->group_desc_गिनती / अंक_threads;
	अगर (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_आकार) * flexbg_आकार;
	}
	अगर ((अंक_threads <= 1) || (average_group == 0))
		जाओ fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	अगर (retval)
		वापस retval;

	thread_ids = सुस्मृति(माप(pthread_t), अंक_threads);
	अगर (!thread_ids)
		वापस ENOMEM;

	thread_infos = सुस्मृति(माप(काष्ठा read_bitmaps_thread_info),
				अंक_threads);
	अगर (!thread_infos)
		जाओ out;

	retval = read_bitmaps_range_prepare(fs, flags);
	अगर (retval)
		जाओ out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	क्रम (i = 0; i < अंक_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_पुच्छ_flags = 0;
		अगर (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		अन्यथा
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		अगर (i == अंक_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_गिनती - 1;
		अन्यथा
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		अगर (retval)
			अवरोध;
	}
	क्रम (i = 0; i < अंक_threads; i++) {
		अगर (!thread_ids[i])
			अवरोध;
		rc = pthread_join(thread_ids[i], शून्य);
		अगर (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		अगर (rc && !retval)
			retval = rc;
		पुच्छ_flags |= thread_infos[i].rbt_पुच्छ_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	अगर (rc && !retval)
		retval = rc;
	मुक्त(thread_infos);
	मुक्त(thread_ids);

	अगर (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, पुच्छ_flags);
	अगर (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	वापस retval;
fallback:
#पूर्ण_अगर /* HAVE_PTHREAD */
	वापस read_bitmaps_range(fs, flags, 0, fs->group_desc_गिनती - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	वापस ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	वापस ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	वापस write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	वापस write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	पूर्णांक flags = 0;

	अगर (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	अगर (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	अगर (flags == 0)
		वापस 0;
	वापस ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	पूर्णांक do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	पूर्णांक do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	अगर (!do_inode && !do_block)
		वापस 0;

	वापस write_bitmaps(fs, do_inode, do_block);
}
