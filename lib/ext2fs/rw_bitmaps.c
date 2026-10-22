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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#ifdef HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#അന്തര്ഭാവ <pthread.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "e2image.h"

#ifdef HAVE_PTHREAD
തരനിർവചനം pthread_mutex_t mutex_t;

സ്ഥിതം ശൂന്യം unix_pthread_mutex_lock(mutex_t *mutex)
{
	എങ്കിൽ (mutex)
		pthread_mutex_lock(mutex);
}
സ്ഥിതം ശൂന്യം unix_pthread_mutex_unlock(mutex_t *mutex)
{
	എങ്കിൽ (mutex)
		pthread_mutex_unlock(mutex);
}
#അല്ലെങ്കിൽ
തരനിർവചനം പൂർണ്ണസംഖ്യ mutex_t;
#define unix_pthread_mutex_lock(mutex_t) ചെയ്യുക {} ആകുമ്പോൾ (0)
#define unix_pthread_mutex_unlock(mutex_t) ചെയ്യുക {} ആകുമ്പോൾ (0)
#endif

സ്ഥിതം errcode_t write_bitmaps(ext2_filsys fs, പൂർണ്ണസംഖ്യ do_inode, പൂർണ്ണസംഖ്യ do_block)
{
	dgrp_t 		i;
	unsigned പൂർണ്ണസംഖ്യ	j;
	പൂർണ്ണസംഖ്യ		block_nbytes, inode_nbytes;
	unsigned പൂർണ്ണസംഖ്യ	nbits;
	errcode_t	retval;
	അക്ഷരം		*block_buf = ശൂന്യ, *inode_buf = ശൂന്യ;
	പൂർണ്ണസംഖ്യ		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (!(fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	എങ്കിൽ (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		എങ്കിൽ (retval)
			പോകുക errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	എങ്കിൽ (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		എങ്കിൽ (retval)
			പോകുക errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	വേണ്ടി (i = 0; i < fs->group_desc_എണ്ണം; i++) {
		എങ്കിൽ (!do_block)
			പോകുക skip_block_bitmap;

		എങ്കിൽ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			പോകുക skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		എങ്കിൽ (retval)
			പോകുക errout;

		എങ്കിൽ (i == fs->group_desc_എണ്ണം - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_അക്കം_B2_C(fs,
				((ext2fs_blocks_എണ്ണം(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			എങ്കിൽ (nbits)
				വേണ്ടി (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		എങ്കിൽ (retval)
			തിരികെ retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		എങ്കിൽ (blk && blk < ext2fs_blocks_എണ്ണം(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			എങ്കിൽ (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				പോകുക errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		എങ്കിൽ (!do_inode)
			തുടരുക;

		എങ്കിൽ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			പോകുക skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		എങ്കിൽ (retval)
			പോകുക errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		എങ്കിൽ (retval)
			പോകുക errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		എങ്കിൽ (blk && blk < ext2fs_blocks_എണ്ണം(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			എങ്കിൽ (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				പോകുക errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	എങ്കിൽ (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	എങ്കിൽ (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	തിരികെ 0;
errout:
	എങ്കിൽ (inode_buf)
		ext2fs_free_mem(&inode_buf);
	എങ്കിൽ (block_buf)
		ext2fs_free_mem(&block_buf);
	തിരികെ retval;
}

സ്ഥിതം errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	വേണ്ടി (i = 0; i < fs->group_desc_എണ്ണം; i++) {
		എങ്കിൽ (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			തുടരുക;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		എങ്കിൽ (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		എങ്കിൽ (blk && blk < ext2fs_blocks_എണ്ണം(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		എങ്കിൽ (blk && blk < ext2fs_blocks_എണ്ണം(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ bitmap_tail_verify(unsigned അക്ഷരം *bitmap, പൂർണ്ണസംഖ്യ first, പൂർണ്ണസംഖ്യ last)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = first; i <= last; i++)
		എങ്കിൽ (bitmap[i] != 0xff)
			തിരികെ 0;
	തിരികെ 1;
}

സ്ഥിതം errcode_t read_bitmaps_range_prepare(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags)
{
	errcode_t retval;
	പൂർണ്ണസംഖ്യ block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	പൂർണ്ണസംഖ്യ inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	അക്ഷരം *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ ((block_nbytes > (പൂർണ്ണസംഖ്യ) fs->blocksize) ||
	    (inode_nbytes > (പൂർണ്ണസംഖ്യ) fs->blocksize))
		തിരികെ EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (flags & EXT2FS_BITMAPS_BLOCK) {
		എങ്കിൽ (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}

	എങ്കിൽ (flags & EXT2FS_BITMAPS_INODE) {
		എങ്കിൽ (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	}
	ext2fs_free_mem(&buf);
	തിരികെ retval;

cleanup:
	എങ്കിൽ (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	എങ്കിൽ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	തിരികെ retval;
}

സ്ഥിതം errcode_t read_bitmaps_range_start(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  പൂർണ്ണസംഖ്യ *tail_flags)
{
	dgrp_t i;
	അക്ഷരം *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	പൂർണ്ണസംഖ്യ block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	പൂർണ്ണസംഖ്യ inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	പൂർണ്ണസംഖ്യ csum_flag;
	unsigned പൂർണ്ണസംഖ്യ	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	എങ്കിൽ (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	} അല്ലെങ്കിൽ {
		block_nbytes = 0;
	}

	എങ്കിൽ (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		എങ്കിൽ (retval)
			പോകുക cleanup;
	} അല്ലെങ്കിൽ {
		inode_nbytes = 0;
	}

	/* io should be null */
	എങ്കിൽ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_എണ്ണം;
		ആകുമ്പോൾ (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			എങ്കിൽ (retval)
				പോകുക cleanup;
			cnt = fs->blocksize << 3;
			എങ്കിൽ (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			എങ്കിൽ (retval)
				പോകുക cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_എണ്ണം);
		ആകുമ്പോൾ (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			എങ്കിൽ (retval)
				പോകുക cleanup;
			cnt = fs->blocksize << 3;
			എങ്കിൽ (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			എങ്കിൽ (retval)
				പോകുക cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		പോകുക cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	വേണ്ടി (i = start; i <= end; i++) {
		എങ്കിൽ (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			എങ്കിൽ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_എണ്ണം(fs->super)))
				blk = 0;
			എങ്കിൽ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				എങ്കിൽ (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					പോകുക cleanup;
				}
				/* verify block bitmap checksum */
				എങ്കിൽ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					പോകുക cleanup;
				}
				എങ്കിൽ (!bitmap_tail_verify((unsigned അക്ഷരം *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} അല്ലെങ്കിൽ
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			എങ്കിൽ (retval)
				പോകുക cleanup;
			blk_itr += block_nbytes << 3;
		}
		എങ്കിൽ (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			എങ്കിൽ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_എണ്ണം(fs->super)))
				blk = 0;
			എങ്കിൽ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				എങ്കിൽ (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					പോകുക cleanup;
				}

				/* verify inode bitmap checksum */
				എങ്കിൽ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					പോകുക cleanup;
				}
				എങ്കിൽ (!bitmap_tail_verify((unsigned അക്ഷരം *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} അല്ലെങ്കിൽ
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			എങ്കിൽ (retval)
				പോകുക cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	എങ്കിൽ (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	എങ്കിൽ (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	തിരികെ retval;
}

സ്ഥിതം errcode_t read_bitmaps_range_end(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags,
					പൂർണ്ണസംഖ്യ tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	എങ്കിൽ (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		എങ്കിൽ (retval)
			തിരികെ retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	എങ്കിൽ (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം read_bitmaps_cleanup_on_error(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags)
{
	എങ്കിൽ (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	എങ്കിൽ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

സ്ഥിതം errcode_t read_bitmaps_range(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	പൂർണ്ണസംഖ്യ tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  ശൂന്യ, &tail_flags);
	എങ്കിൽ (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	എങ്കിൽ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	തിരികെ retval;
}

#ifdef HAVE_PTHREAD
ഘടന read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	പൂർണ്ണസംഖ്യ		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	പൂർണ്ണസംഖ്യ		rbt_tail_flags;
};

സ്ഥിതം ശൂന്യം *read_bitmaps_thread(ശൂന്യം *data)
{
	ഘടന read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	തിരികെ ശൂന്യ;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ അക്കം_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = ശൂന്യ;
	ഘടന read_bitmaps_thread_info *thread_infos = ശൂന്യ;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	പൂർണ്ണസംഖ്യ i, tail_flags = 0;
#endif

	എങ്കിൽ (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	എങ്കിൽ (ext2fs_has_feature_journal_dev(fs->super))
		തിരികെ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	എങ്കിൽ (flags & EXT2FS_BITMAPS_WRITE)
		തിരികെ write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	എങ്കിൽ (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (അക്കം_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		പോകുക fallback;

#എങ്കിൽ defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	എങ്കിൽ (അക്കം_threads < 0)
		അക്കം_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	എങ്കിൽ (അക്കം_threads <= 0)
		അക്കം_threads = 4;

	എങ്കിൽ ((unsigned) അക്കം_threads > fs->group_desc_എണ്ണം)
		അക്കം_threads = fs->group_desc_എണ്ണം;
	average_group = fs->group_desc_എണ്ണം / അക്കം_threads;
	എങ്കിൽ (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	എങ്കിൽ ((അക്കം_threads <= 1) || (average_group == 0))
		പോകുക fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	എങ്കിൽ (retval)
		തിരികെ retval;

	thread_ids = calloc(വലുപ്പം(pthread_t), അക്കം_threads);
	എങ്കിൽ (!thread_ids)
		തിരികെ ENOMEM;

	thread_infos = calloc(വലുപ്പം(ഘടന read_bitmaps_thread_info),
				അക്കം_threads);
	എങ്കിൽ (!thread_infos)
		പോകുക out;

	retval = read_bitmaps_range_prepare(fs, flags);
	എങ്കിൽ (retval)
		പോകുക out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	വേണ്ടി (i = 0; i < അക്കം_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		എങ്കിൽ (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		അല്ലെങ്കിൽ
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		എങ്കിൽ (i == അക്കം_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_എണ്ണം - 1;
		അല്ലെങ്കിൽ
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		എങ്കിൽ (retval)
			നിർത്തുക;
	}
	വേണ്ടി (i = 0; i < അക്കം_threads; i++) {
		എങ്കിൽ (!thread_ids[i])
			നിർത്തുക;
		rc = pthread_join(thread_ids[i], ശൂന്യ);
		എങ്കിൽ (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		എങ്കിൽ (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	എങ്കിൽ (rc && !retval)
		retval = rc;
	സ്വതന്ത്രം(thread_infos);
	സ്വതന്ത്രം(thread_ids);

	എങ്കിൽ (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	എങ്കിൽ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	തിരികെ retval;
fallback:
#endif /* HAVE_PTHREAD */
	തിരികെ read_bitmaps_range(fs, flags, 0, fs->group_desc_എണ്ണം - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	തിരികെ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	തിരികെ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	തിരികെ write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	തിരികെ write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	പൂർണ്ണസംഖ്യ flags = 0;

	എങ്കിൽ (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	എങ്കിൽ (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	എങ്കിൽ (flags == 0)
		തിരികെ 0;
	തിരികെ ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	പൂർണ്ണസംഖ്യ do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	പൂർണ്ണസംഖ്യ do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	എങ്കിൽ (!do_inode && !do_block)
		തിരികെ 0;

	തിരികെ write_bitmaps(fs, do_inode, do_block);
}
