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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ifdef HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ifdef HAVE_PTHREAD_H
#ଅନ୍ତର୍ଭାଵ <pthread.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "e2image.h"

#ifdef HAVE_PTHREAD
ପ୍ରକାରବ୍ୟାଖ୍ୟା pthread_mutex_t mutex_t;

ସ୍ଥିର ଶୂନ୍ୟ unix_pthread_mutex_lock(mutex_t *mutex)
{
	ଯଦି (mutex)
		pthread_mutex_lock(mutex);
}
ସ୍ଥିର ଶୂନ୍ୟ unix_pthread_mutex_unlock(mutex_t *mutex)
{
	ଯଦି (mutex)
		pthread_mutex_unlock(mutex);
}
#ଅନ୍ୟଥା
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା mutex_t;
#define unix_pthread_mutex_lock(mutex_t) କର {} ଯେତେବେଳେ (0)
#define unix_pthread_mutex_unlock(mutex_t) କର {} ଯେତେବେଳେ (0)
#endif

ସ୍ଥିର errcode_t write_bitmaps(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_inode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_block)
{
	dgrp_t 		i;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	j;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		block_nbytes, inode_nbytes;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	nbits;
	errcode_t	retval;
	ଅକ୍ଷର		*block_buf = ଖାଲି, *inode_buf = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (!(fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	inode_nbytes = block_nbytes = 0;
	ଯଦି (do_block) {
		block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &block_buf);
		ଯଦି (retval)
			ଯାଅ errout;
		memset(block_buf, 0xff, fs->blocksize);
	}
	ଯଦି (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_PER_GROUP(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		ଯଦି (retval)
			ଯାଅ errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	ପାଇଁ (i = 0; i < fs->group_desc_ଗଣନା; i++) {
		ଯଦି (!do_block)
			ଯାଅ skip_block_bitmap;

		ଯଦି (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT)
		    )
			ଯାଅ skip_this_block_bitmap;

		retval = ext2fs_get_block_bitmap_range2(fs->block_map,
				blk_itr, block_nbytes << 3, block_buf);
		ଯଦି (retval)
			ଯାଅ errout;

		ଯଦି (i == fs->group_desc_ଗଣନା - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_ଅଙ୍କ_B2_C(fs,
				((ext2fs_blocks_ଗଣନା(fs->super)
				  - (__u64) fs->super->s_first_data_block)
				 % (__u64) EXT2_BLOCKS_PER_GROUP(fs->super)));
			ଯଦି (nbits)
				ପାଇଁ (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_set_bit(j, block_buf);
		}

		retval = ext2fs_block_bitmap_csum_set(fs, i, block_buf,
						      block_nbytes);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_block_bitmap_loc(fs, i);
		ଯଦି (blk && blk < ext2fs_blocks_ଗଣନା(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							block_buf);
			ଯଦି (retval) {
				retval = EXT2_ET_BLOCK_BITMAP_WRITE;
				ଯାଅ errout;
			}
		}
	skip_this_block_bitmap:
		blk_itr += block_nbytes << 3;
	skip_block_bitmap:

		ଯଦି (!do_inode)
			ଜାରି;

		ଯଦି (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			ଯାଅ skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		ଯଦି (retval)
			ଯାଅ errout;

		retval = ext2fs_inode_bitmap_csum_set(fs, i, inode_buf,
						      inode_nbytes);
		ଯଦି (retval)
			ଯାଅ errout;
		ext2fs_group_desc_csum_set(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		ଯଦି (blk && blk < ext2fs_blocks_ଗଣନା(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			ଯଦି (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				ଯାଅ errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	ଯଦି (do_block) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&block_buf);
	}
	ଯଦି (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	ଫେରନ୍ତୁ 0;
errout:
	ଯଦି (inode_buf)
		ext2fs_free_mem(&inode_buf);
	ଯଦି (block_buf)
		ext2fs_free_mem(&block_buf);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t mark_uninit_bg_group_blocks(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_block_bitmap	bmap = fs->block_map;

	ପାଇଁ (i = 0; i < fs->group_desc_ଗଣନା; i++) {
		ଯଦି (!ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT))
			ଜାରି;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		ଯଦି (blk)
			ext2fs_mark_block_bitmap_range2(bmap, blk,
						fs->inode_blocks_per_group);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		ଯଦି (blk && blk < ext2fs_blocks_ଗଣନା(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ଯଦି (blk && blk < ext2fs_blocks_ଗଣନା(fs->super))
			ext2fs_mark_block_bitmap2(bmap, blk);
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା bitmap_tail_verify(unsigned ଅକ୍ଷର *bitmap, ପୂର୍ଣ୍ଣସଂଖ୍ୟା first, ପୂର୍ଣ୍ଣସଂଖ୍ୟା last)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = first; i <= last; i++)
		ଯଦି (bitmap[i] != 0xff)
			ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର errcode_t read_bitmaps_range_prepare(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	errcode_t retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	ଅକ୍ଷର *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି ((block_nbytes > (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) fs->blocksize) ||
	    (inode_nbytes > (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) fs->blocksize))
		ଫେରନ୍ତୁ EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (flags & EXT2FS_BITMAPS_BLOCK) {
		ଯଦି (fs->block_map)
			ext2fs_free_block_bitmap(fs->block_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_block_bitmap(fs, buf, &fs->block_map);
		ଯଦି (retval)
			ଯାଅ cleanup;
	}

	ଯଦି (flags & EXT2FS_BITMAPS_INODE) {
		ଯଦି (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		ଯଦି (retval)
			ଯାଅ cleanup;
	}
	ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;

cleanup:
	ଯଦି (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	ଯଦି (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t read_bitmaps_range_start(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
					  dgrp_t start, dgrp_t end,
					  mutex_t *mutex,
					  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *tail_flags)
{
	dgrp_t i;
	ଅକ୍ଷର *block_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_nbytes = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା inode_nbytes = EXT2_INODES_PER_GROUP(fs->super) / 8;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା csum_flag;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_group_desc_csum(fs);

	ଯଦି (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = io_channel_alloc_buf(fs->io, 0, &block_bitmap);
		ଯଦି (retval)
			ଯାଅ cleanup;
	} ଅନ୍ୟଥା {
		block_nbytes = 0;
	}

	ଯଦି (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		ଯଦି (retval)
			ଯାଅ cleanup;
	} ଅନ୍ୟଥା {
		inode_nbytes = 0;
	}

	/* io should be null */
	ଯଦି (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_ଗଣନା;
		ଯେତେବେଳେ (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			ଯଦି (retval)
				ଯାଅ cleanup;
			cnt = fs->blocksize << 3;
			ଯଦି (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			ଯଦି (retval)
				ଯାଅ cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_to_cpu(fs->image_header->offset_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super,
						  fs->group_desc_ଗଣନା);
		ଯେତେବେଳେ (block_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, block_bitmap);
			ଯଦି (retval)
				ଯାଅ cleanup;
			cnt = fs->blocksize << 3;
			ଯଦି (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
				       blk_itr, cnt, block_bitmap);
			ଯଦି (retval)
				ଯାଅ cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		ଯାଅ cleanup;
	}

	blk_itr += ((blk64_t)start * (block_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	ପାଇଁ (i = start; i <= end; i++) {
		ଯଦି (block_bitmap) {
			blk = ext2fs_block_bitmap_loc(fs, i);
			ଯଦି ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_BLOCK_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_ଗଣନା(fs->super)))
				blk = 0;
			ଯଦି (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, block_bitmap);
				ଯଦି (retval) {
					retval = EXT2_ET_BLOCK_BITMAP_READ;
					ଯାଅ cleanup;
				}
				/* verify block bitmap checksum */
				ଯଦି (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_block_bitmap_csum_verify(fs, i,
						block_bitmap, block_nbytes)) {
					retval =
					EXT2_ET_BLOCK_BITMAP_CSUM_INVALID;
					ଯାଅ cleanup;
				}
				ଯଦି (!bitmap_tail_verify((unsigned ଅକ୍ଷର *) block_bitmap,
							block_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
			} ଅନ୍ୟଥା
				memset(block_bitmap, 0, block_nbytes);
			cnt = block_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_block_bitmap_range2(fs->block_map,
					       blk_itr, cnt, block_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ଯଦି (retval)
				ଯାଅ cleanup;
			blk_itr += block_nbytes << 3;
		}
		ଯଦି (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			ଯଦି ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_group_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_blocks_ଗଣନା(fs->super)))
				blk = 0;
			ଯଦି (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				ଯଦି (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					ଯାଅ cleanup;
				}

				/* verify inode bitmap checksum */
				ଯଦି (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					ଯାଅ cleanup;
				}
				ଯଦି (!bitmap_tail_verify((unsigned ଅକ୍ଷର *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*tail_flags |= EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
			} ଅନ୍ୟଥା
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_lock(mutex);
			retval = ext2fs_set_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ଯଦି (retval)
				ଯାଅ cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	ଯଦି (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	ଯଦି (block_bitmap)
		ext2fs_free_mem(&block_bitmap);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t read_bitmaps_range_end(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
					ପୂର୍ଣ୍ଣସଂଖ୍ୟା tail_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	ଯଦି (flags & EXT2FS_BITMAPS_BLOCK) {
		retval = mark_uninit_bg_group_blocks(fs);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_TAIL_PROBLEM;
	}
	ଯଦି (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_TAIL_PROBLEM;
	fs->flags |= tail_flags;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ read_bitmaps_cleanup_on_error(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଯଦି (flags & EXT2FS_BITMAPS_BLOCK) {
		ext2fs_free_block_bitmap(fs->block_map);
		fs->block_map = 0;
	}
	ଯଦି (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ସ୍ଥିର errcode_t read_bitmaps_range(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
				    dgrp_t start, dgrp_t end)
{
	errcode_t retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା tail_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = read_bitmaps_range_start(fs, flags, start, end,
					  ଖାଲି, &tail_flags);
	ଯଦି (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	ଯଦି (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	ଫେରନ୍ତୁ retval;
}

#ifdef HAVE_PTHREAD
ସଂରଚନା read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_end;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		rbt_tail_flags;
};

ସ୍ଥିର ଶୂନ୍ୟ *read_bitmaps_thread(ଶୂନ୍ୟ *data)
{
	ସଂରଚନା read_bitmaps_thread_info *rbt = data;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_end,
				rbt->rbt_mutex, &rbt->rbt_tail_flags);
	ଫେରନ୍ତୁ ଖାଲି;
}
#endif

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_threads)
{
#ifdef HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = ଖାଲି;
	ସଂରଚନା read_bitmaps_thread_info *thread_infos = ଖାଲି;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	unsigned flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	dgrp_t average_group;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, tail_flags = 0;
#endif

	ଯଦି (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ଯଦି (ext2fs_has_feature_journal_dev(fs->super))
		ଫେରନ୍ତୁ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ଯଦି (flags & EXT2FS_BITMAPS_WRITE)
		ଫେରନ୍ତୁ write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2FS_BITMAPS_BLOCK);

#ifdef HAVE_PTHREAD
	ଯଦି (((fs->io->flags & CHANNEL_FLAGS_THREADS) == 0) ||
	    (ଅଙ୍କ_threads == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		ଯାଅ fallback;

#ଯଦି defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	ଯଦି (ଅଙ୍କ_threads < 0)
		ଅଙ୍କ_threads = sysconf(_SC_NPROCESSORS_CONF);
#endif
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	ଯଦି (ଅଙ୍କ_threads <= 0)
		ଅଙ୍କ_threads = 4;

	ଯଦି ((unsigned) ଅଙ୍କ_threads > fs->group_desc_ଗଣନା)
		ଅଙ୍କ_threads = fs->group_desc_ଗଣନା;
	average_group = fs->group_desc_ଗଣନା / ଅଙ୍କ_threads;
	ଯଦି (ext2fs_has_feature_flex_bg(fs->super)) {
		average_group = (average_group / flexbg_size) * flexbg_size;
	}
	ଯଦି ((ଅଙ୍କ_threads <= 1) || (average_group == 0))
		ଯାଅ fallback;

	io_channel_set_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	thread_ids = calloc(ଆକାର(pthread_t), ଅଙ୍କ_threads);
	ଯଦି (!thread_ids)
		ଫେରନ୍ତୁ ENOMEM;

	thread_infos = calloc(ଆକାର(ସଂରଚନା read_bitmaps_thread_info),
				ଅଙ୍କ_threads);
	ଯଦି (!thread_infos)
		ଯାଅ out;

	retval = read_bitmaps_range_prepare(fs, flags);
	ଯଦି (retval)
		ଯାଅ out;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	ପାଇଁ (i = 0; i < ଅଙ୍କ_threads; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_tail_flags = 0;
		ଯଦି (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		ଅନ୍ୟଥା
			thread_infos[i].rbt_grp_start = average_group * i + 1;

		ଯଦି (i == ଅଙ୍କ_threads - 1)
			thread_infos[i].rbt_grp_end = fs->group_desc_ଗଣନା - 1;
		ଅନ୍ୟଥା
			thread_infos[i].rbt_grp_end = average_group * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		ଯଦି (retval)
			ଭାଙ୍ଗ;
	}
	ପାଇଁ (i = 0; i < ଅଙ୍କ_threads; i++) {
		ଯଦି (!thread_ids[i])
			ଭାଙ୍ଗ;
		rc = pthread_join(thread_ids[i], ଖାଲି);
		ଯଦି (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		ଯଦି (rc && !retval)
			retval = rc;
		tail_flags |= thread_infos[i].rbt_tail_flags;
	}
out:
	rc = pthread_attr_destroy(&attr);
	ଯଦି (rc && !retval)
		retval = rc;
	ମୁକ୍ତ(thread_infos);
	ମୁକ୍ତ(thread_ids);

	ଯଦି (retval == 0)
		retval = read_bitmaps_range_end(fs, flags, tail_flags);
	ଯଦି (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_set_options(fs->io, "cache=on");
	ଫେରନ୍ତୁ retval;
fallback:
#endif /* HAVE_PTHREAD */
	ଫେରନ୍ତୁ read_bitmaps_range(fs, flags, 0, fs->group_desc_ଗଣନା - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	ଫେରନ୍ତୁ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_block_bitmap(ext2_filsys fs)
{
	ଫେରନ୍ତୁ ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_BLOCK, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	ଫେରନ୍ତୁ write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_block_bitmap (ext2_filsys fs)
{
	ଫେରନ୍ତୁ write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;

	ଯଦି (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	ଯଦି (!fs->block_map)
		flags |= EXT2FS_BITMAPS_BLOCK;
	ଯଦି (flags == 0)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_block = fs->block_map && ext2fs_test_bb_dirty(fs);

	ଯଦି (!do_inode && !do_block)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ write_bitmaps(fs, do_inode, do_block);
}
