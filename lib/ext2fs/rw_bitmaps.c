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
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر_تعریف_شدہ HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_PTHREAD_H
#شامل <pthread.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "e2image.h"

#اگر_تعریف_شدہ HAVE_PTHREAD
قسمتعریف pthread_mutex_t mutex_t;

ساکن خالی unix_pthread_mutex_تالا(mutex_t *mutex)
{
	اگر (mutex)
		pthread_mutex_تالا(mutex);
}
ساکن خالی unix_pthread_mutex_unlock(mutex_t *mutex)
{
	اگر (mutex)
		pthread_mutex_unlock(mutex);
}
#ورنہ
قسمتعریف صحیح mutex_t;
#تعریف_کرو unix_pthread_mutex_تالا(mutex_t) کرو {} جب (0)
#تعریف_کرو unix_pthread_mutex_unlock(mutex_t) کرو {} جب (0)
#اگر_ختم

ساکن errcode_t write_bitmaps(ext2_filsys fs, صحیح do_inode, صحیح do_بلاک)
{
	dgrp_t 		i;
	غیرعلامتی صحیح	j;
	صحیح		بلاک_nbytes, inode_nbytes;
	غیرعلامتی صحیح	nbits;
	errcode_t	retval;
	حرف		*بلاک_buf = تہی, *inode_buf = تہی;
	صحیح		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_پہلا_ڈیٹا_بلاک);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!(fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_گروہ_desc_csum(fs);

	inode_nbytes = بلاک_nbytes = 0;
	اگر (do_بلاک) {
		بلاک_nbytes = EXT2_CLUSTERS_فی_گروہ(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &بلاک_buf);
		اگر (retval)
			جاؤ errout;
		memset(بلاک_buf, 0xff, fs->blocksize);
	}
	اگر (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_فی_گروہ(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		اگر (retval)
			جاؤ errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	برائے (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		اگر (!do_بلاک)
			جاؤ skip_بلاک_bitmap;

		اگر (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاک_UNINIT)
		    )
			جاؤ skip_this_بلاک_bitmap;

		retval = ext2fs_get_بلاک_bitmap_range2(fs->بلاک_map,
				blk_itr, بلاک_nbytes << 3, بلاک_buf);
		اگر (retval)
			جاؤ errout;

		اگر (i == fs->گروہ_desc_گنتی - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_عدد_B2_C(fs,
				((ext2fs_بلاکیں_گنتی(fs->super)
				  - (__u64) fs->super->s_پہلا_ڈیٹا_بلاک)
				 % (__u64) EXT2_بلاکیں_فی_گروہ(fs->super)));
			اگر (nbits)
				برائے (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_سیٹ_ثنائی_ہندسہ(j, بلاک_buf);
		}

		retval = ext2fs_بلاک_bitmap_csum_سیٹ(fs, i, بلاک_buf,
						      بلاک_nbytes);
		اگر (retval)
			واپس retval;
		ext2fs_گروہ_desc_csum_سیٹ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_بلاک_bitmap_loc(fs, i);
		اگر (blk && blk < ext2fs_بلاکیں_گنتی(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							بلاک_buf);
			اگر (retval) {
				retval = EXT2_ET_بلاک_BITMAP_WRITE;
				جاؤ errout;
			}
		}
	skip_this_بلاک_bitmap:
		blk_itr += بلاک_nbytes << 3;
	skip_بلاک_bitmap:

		اگر (!do_inode)
			جاری;

		اگر (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			جاؤ skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		اگر (retval)
			جاؤ errout;

		retval = ext2fs_inode_bitmap_csum_سیٹ(fs, i, inode_buf,
						      inode_nbytes);
		اگر (retval)
			جاؤ errout;
		ext2fs_گروہ_desc_csum_سیٹ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		اگر (blk && blk < ext2fs_بلاکیں_گنتی(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			اگر (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				جاؤ errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	اگر (do_بلاک) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&بلاک_buf);
	}
	اگر (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	واپس 0;
errout:
	اگر (inode_buf)
		ext2fs_free_mem(&inode_buf);
	اگر (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
	واپس retval;
}

ساکن errcode_t mark_uninit_bg_گروہ_بلاکیں(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_بلاک_bitmap	bmap = fs->بلاک_map;

	برائے (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		اگر (!ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاک_UNINIT))
			جاری;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		اگر (blk)
			ext2fs_mark_بلاک_bitmap_range2(bmap, blk,
						fs->inode_بلاکیں_فی_گروہ);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_بلاک_bitmap_loc(fs, i);
		اگر (blk && blk < ext2fs_بلاکیں_گنتی(fs->super))
			ext2fs_mark_بلاک_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		اگر (blk && blk < ext2fs_بلاکیں_گنتی(fs->super))
			ext2fs_mark_بلاک_bitmap2(bmap, blk);
	}
	واپس 0;
}

ساکن صحیح bitmap_دم_verify(غیرعلامتی حرف *bitmap, صحیح پہلا, صحیح آخری)
{
	صحیح i;

	برائے (i = پہلا; i <= آخری; i++)
		اگر (bitmap[i] != 0xff)
			واپس 0;
	واپس 1;
}

ساکن errcode_t read_bitmaps_range_prepare(ext2_filsys fs, صحیح flags)
{
	errcode_t retval;
	صحیح بلاک_nbytes = EXT2_CLUSTERS_فی_گروہ(fs->super) / 8;
	صحیح inode_nbytes = EXT2_INODES_فی_گروہ(fs->super) / 8;
	حرف *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر ((بلاک_nbytes > (صحیح) fs->blocksize) ||
	    (inode_nbytes > (صحیح) fs->blocksize))
		واپس EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(لڑی_لمبائی(fs->device_name) + 80, &buf);
	اگر (retval)
		واپس retval;

	اگر (flags & EXT2_FS_BITMAPS_بلاک) {
		اگر (fs->بلاک_map)
			ext2fs_free_بلاک_bitmap(fs->بلاک_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_بلاک_bitmap(fs, buf, &fs->بلاک_map);
		اگر (retval)
			جاؤ cleanup;
	}

	اگر (flags & EXT2FS_BITMAPS_INODE) {
		اگر (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		اگر (retval)
			جاؤ cleanup;
	}
	ext2fs_free_mem(&buf);
	واپس retval;

cleanup:
	اگر (flags & EXT2_FS_BITMAPS_بلاک) {
		ext2fs_free_بلاک_bitmap(fs->بلاک_map);
		fs->بلاک_map = 0;
	}
	اگر (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	واپس retval;
}

ساکن errcode_t read_bitmaps_range_آغاز(ext2_filsys fs, صحیح flags,
					  dgrp_t آغاز, dgrp_t انتہا,
					  mutex_t *mutex,
					  صحیح *دم_flags)
{
	dgrp_t i;
	حرف *بلاک_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	صحیح بلاک_nbytes = EXT2_CLUSTERS_فی_گروہ(fs->super) / 8;
	صحیح inode_nbytes = EXT2_INODES_فی_گروہ(fs->super) / 8;
	صحیح csum_flag;
	غیرعلامتی صحیح	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_پہلا_ڈیٹا_بلاک);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_گروہ_desc_csum(fs);

	اگر (flags & EXT2_FS_BITMAPS_بلاک) {
		retval = io_channel_alloc_buf(fs->io, 0, &بلاک_bitmap);
		اگر (retval)
			جاؤ cleanup;
	} ورنہ {
		بلاک_nbytes = 0;
	}

	اگر (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		اگر (retval)
			جاؤ cleanup;
	} ورنہ {
		inode_nbytes = 0;
	}

	/* io should be null */
	اگر (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_مقصد_cpu(fs->image_سرخی->انحراف_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_گنتی;
		جب (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			اگر (retval)
				جاؤ cleanup;
			cnt = fs->blocksize << 3;
			اگر (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_سیٹ_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			اگر (retval)
				جاؤ cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_مقصد_cpu(fs->image_سرخی->انحراف_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_مقصد_CLUSTERS(fs->super,
						  fs->گروہ_desc_گنتی);
		جب (بلاک_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, بلاک_bitmap);
			اگر (retval)
				جاؤ cleanup;
			cnt = fs->blocksize << 3;
			اگر (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_سیٹ_بلاک_bitmap_range2(fs->بلاک_map,
				       blk_itr, cnt, بلاک_bitmap);
			اگر (retval)
				جاؤ cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		جاؤ cleanup;
	}

	blk_itr += ((blk64_t)آغاز * (بلاک_nbytes << 3));
	ino_itr += ((blk64_t)آغاز * (inode_nbytes << 3));
	برائے (i = آغاز; i <= انتہا; i++) {
		اگر (بلاک_bitmap) {
			blk = ext2fs_بلاک_bitmap_loc(fs, i);
			اگر ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاک_UNINIT) &&
			     ext2fs_گروہ_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلاکیں_گنتی(fs->super)))
				blk = 0;
			اگر (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, بلاک_bitmap);
				اگر (retval) {
					retval = EXT2_ET_بلاک_BITMAP_READ;
					جاؤ cleanup;
				}
				/* verify block bitmap checksum */
				اگر (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_بلاک_bitmap_csum_verify(fs, i,
						بلاک_bitmap, بلاک_nbytes)) {
					retval =
					EXT2_ET_بلاک_BITMAP_CSUM_INVALID;
					جاؤ cleanup;
				}
				اگر (!bitmap_دم_verify((غیرعلامتی حرف *) بلاک_bitmap,
							بلاک_nbytes, fs->blocksize - 1))
					*دم_flags |= EXT2_FLAG_BBITMAP_دم_PROBLEM;
			} ورنہ
				memset(بلاک_bitmap, 0, بلاک_nbytes);
			cnt = بلاک_nbytes << 3;
			unix_pthread_mutex_تالا(mutex);
			retval = ext2fs_سیٹ_بلاک_bitmap_range2(fs->بلاک_map,
					       blk_itr, cnt, بلاک_bitmap);
			unix_pthread_mutex_unlock(mutex);
			اگر (retval)
				جاؤ cleanup;
			blk_itr += بلاک_nbytes << 3;
		}
		اگر (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			اگر ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_گروہ_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلاکیں_گنتی(fs->super)))
				blk = 0;
			اگر (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				اگر (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					جاؤ cleanup;
				}

				/* verify inode bitmap checksum */
				اگر (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					جاؤ cleanup;
				}
				اگر (!bitmap_دم_verify((غیرعلامتی حرف *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*دم_flags |= EXT2_FLAG_IBITMAP_دم_PROBLEM;
			} ورنہ
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_تالا(mutex);
			retval = ext2fs_سیٹ_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			اگر (retval)
				جاؤ cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	اگر (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	اگر (بلاک_bitmap)
		ext2fs_free_mem(&بلاک_bitmap);
	واپس retval;
}

ساکن errcode_t read_bitmaps_range_انتہا(ext2_filsys fs, صحیح flags,
					صحیح دم_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	اگر (flags & EXT2_FS_BITMAPS_بلاک) {
		retval = mark_uninit_bg_گروہ_بلاکیں(fs);
		اگر (retval)
			واپس retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_دم_PROBLEM;
	}
	اگر (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_دم_PROBLEM;
	fs->flags |= دم_flags;

	واپس 0;
}

ساکن خالی read_bitmaps_cleanup_on_error(ext2_filsys fs, صحیح flags)
{
	اگر (flags & EXT2_FS_BITMAPS_بلاک) {
		ext2fs_free_بلاک_bitmap(fs->بلاک_map);
		fs->بلاک_map = 0;
	}
	اگر (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ساکن errcode_t read_bitmaps_range(ext2_filsys fs, صحیح flags,
				    dgrp_t آغاز, dgrp_t انتہا)
{
	errcode_t retval;
	صحیح دم_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	اگر (retval)
		واپس retval;

	retval = read_bitmaps_range_آغاز(fs, flags, آغاز, انتہا,
					  تہی, &دم_flags);
	اگر (retval == 0)
		retval = read_bitmaps_range_انتہا(fs, flags, دم_flags);
	اگر (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	واپس retval;
}

#اگر_تعریف_شدہ HAVE_PTHREAD
ساخت read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	صحیح		rbt_flags;
	dgrp_t		rbt_grp_آغاز;
	dgrp_t		rbt_grp_انتہا;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	صحیح		rbt_دم_flags;
};

ساکن خالی *read_bitmaps_thread(خالی *ڈیٹا)
{
	ساخت read_bitmaps_thread_info *rbt = ڈیٹا;

	rbt->rbt_retval = read_bitmaps_range_آغاز(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_آغاز, rbt->rbt_grp_انتہا,
				rbt->rbt_mutex, &rbt->rbt_دم_flags);
	واپس تہی;
}
#اگر_ختم

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحیح flags, صحیح عدد_دھاگے)
{
#اگر_تعریف_شدہ HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = تہی;
	ساخت read_bitmaps_thread_info *thread_infos = تہی;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	غیرعلامتی flexbg_سائز = 1U << fs->super->s_log_groups_فی_flex;
	dgrp_t اوسط_قدر_گروہ;
	صحیح i, دم_flags = 0;
#اگر_ختم

	اگر (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		واپس EXT2_ET_INVALID_دلیل;

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		واپس EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	اگر (flags & EXT2FS_BITMAPS_WRITE)
		واپس write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2_FS_BITMAPS_بلاک);

#اگر_تعریف_شدہ HAVE_PTHREAD
	اگر (((fs->io->flags & CHANNEL_FLAGS_دھاگے) == 0) ||
	    (عدد_دھاگے == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		جاؤ fallback;

#اگر defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	اگر (عدد_دھاگے < 0)
		عدد_دھاگے = sysconf(_SC_NPROCESSORS_CONF);
#اگر_ختم
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	اگر (عدد_دھاگے <= 0)
		عدد_دھاگے = 4;

	اگر ((غیرعلامتی) عدد_دھاگے > fs->گروہ_desc_گنتی)
		عدد_دھاگے = fs->گروہ_desc_گنتی;
	اوسط_قدر_گروہ = fs->گروہ_desc_گنتی / عدد_دھاگے;
	اگر (ext2fs_has_feature_flex_bg(fs->super)) {
		اوسط_قدر_گروہ = (اوسط_قدر_گروہ / flexbg_سائز) * flexbg_سائز;
	}
	اگر ((عدد_دھاگے <= 1) || (اوسط_قدر_گروہ == 0))
		جاؤ fallback;

	io_channel_سیٹ_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	اگر (retval)
		واپس retval;

	thread_ids = صفر_حافظہ_دو(حجم(pthread_t), عدد_دھاگے);
	اگر (!thread_ids)
		واپس ENOMEM;

	thread_infos = صفر_حافظہ_دو(حجم(ساخت read_bitmaps_thread_info),
				عدد_دھاگے);
	اگر (!thread_infos)
		جاؤ برآمد;

	retval = read_bitmaps_range_prepare(fs, flags);
	اگر (retval)
		جاؤ برآمد;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	برائے (i = 0; i < عدد_دھاگے; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_دم_flags = 0;
		اگر (i == 0)
			thread_infos[i].rbt_grp_آغاز = 0;
		ورنہ
			thread_infos[i].rbt_grp_آغاز = اوسط_قدر_گروہ * i + 1;

		اگر (i == عدد_دھاگے - 1)
			thread_infos[i].rbt_grp_انتہا = fs->گروہ_desc_گنتی - 1;
		ورنہ
			thread_infos[i].rbt_grp_انتہا = اوسط_قدر_گروہ * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		اگر (retval)
			توڑو;
	}
	برائے (i = 0; i < عدد_دھاگے; i++) {
		اگر (!thread_ids[i])
			توڑو;
		rc = pthread_join(thread_ids[i], تہی);
		اگر (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		اگر (rc && !retval)
			retval = rc;
		دم_flags |= thread_infos[i].rbt_دم_flags;
	}
برآمد:
	rc = pthread_attr_destroy(&attr);
	اگر (rc && !retval)
		retval = rc;
	آزاد(thread_infos);
	آزاد(thread_ids);

	اگر (retval == 0)
		retval = read_bitmaps_range_انتہا(fs, flags, دم_flags);
	اگر (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_سیٹ_options(fs->io, "cache=on");
	واپس retval;
fallback:
#اگر_ختم /* HAVE_PTHREAD */
	واپس read_bitmaps_range(fs, flags, 0, fs->گروہ_desc_گنتی - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	واپس ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_بلاک_bitmap(ext2_filsys fs)
{
	واپس ext2fs_rw_bitmaps(fs, EXT2_FS_BITMAPS_بلاک, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	واپس write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_بلاک_bitmap (ext2_filsys fs)
{
	واپس write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	صحیح flags = 0;

	اگر (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	اگر (!fs->بلاک_map)
		flags |= EXT2_FS_BITMAPS_بلاک;
	اگر (flags == 0)
		واپس 0;
	واپس ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	صحیح do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	صحیح do_بلاک = fs->بلاک_map && ext2fs_test_bb_dirty(fs);

	اگر (!do_inode && !do_بلاک)
		واپس 0;

	واپس write_bitmaps(fs, do_inode, do_بلاک);
}
