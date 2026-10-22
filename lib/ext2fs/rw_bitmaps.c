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
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#جے_تعریف_شدہ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#جے_تعریف_شدہ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#جے_تعریف_شدہ HAVE_PTHREAD_H
#ضمن <pthread.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "e2image.h"

#جے_تعریف_شدہ HAVE_PTHREAD
تعريفنوع pthread_mutex_t mutex_t;

ساكن فراغ unix_pthread_mutex_جندرا(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_جندرا(mutex);
}
ساكن فراغ unix_pthread_mutex_unlock(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_unlock(mutex);
}
#وإلا
تعريفنوع صحيح mutex_t;
#تعریف_کرو unix_pthread_mutex_جندرا(mutex_t) افعل {} طالما (0)
#تعریف_کرو unix_pthread_mutex_unlock(mutex_t) افعل {} طالما (0)
#جے_انت

ساكن errcode_t write_bitmaps(ext2_filsys fs, صحيح do_inode, صحيح do_بلاک)
{
	dgrp_t 		i;
	بنانشانی صحيح	j;
	صحيح		بلاک_nbytes, inode_nbytes;
	بنانشانی صحيح	nbits;
	errcode_t	retval;
	حرف		*بلاک_buf = عدم, *inode_buf = عدم;
	صحيح		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_پہلا_انکڑے_بلاک);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_گروہ_desc_csum(fs);

	inode_nbytes = بلاک_nbytes = 0;
	إذا (do_بلاک) {
		بلاک_nbytes = EXT2_CLUSTERS_فی_گروہ(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &بلاک_buf);
		إذا (retval)
			اذهب errout;
		memset(بلاک_buf, 0xff, fs->blocksize);
	}
	إذا (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_فی_گروہ(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		إذا (retval)
			اذهب errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	لكل (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		إذا (!do_بلاک)
			اذهب skip_بلاک_bitmap;

		إذا (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاک_UNINIT)
		    )
			اذهب skip_this_بلاک_bitmap;

		retval = ext2fs_get_بلاک_bitmap_range2(fs->بلاک_map,
				blk_itr, بلاک_nbytes << 3, بلاک_buf);
		إذا (retval)
			اذهب errout;

		إذا (i == fs->گروہ_desc_گنتی - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_عدد_B2_C(fs,
				((ext2fs_بلاکاں_گنتی(fs->super)
				  - (__u64) fs->super->s_پہلا_انکڑے_بلاک)
				 % (__u64) EXT2_بلاکاں_فی_گروہ(fs->super)));
			إذا (nbits)
				لكل (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_سیٹ_ثنائی_ہندسہ(j, بلاک_buf);
		}

		retval = ext2fs_بلاک_bitmap_csum_سیٹ(fs, i, بلاک_buf,
						      بلاک_nbytes);
		إذا (retval)
			أرجع retval;
		ext2fs_گروہ_desc_csum_سیٹ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_بلاک_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاکاں_گنتی(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							بلاک_buf);
			إذا (retval) {
				retval = EXT2_ET_بلاک_BITMAP_WRITE;
				اذهب errout;
			}
		}
	skip_this_بلاک_bitmap:
		blk_itr += بلاک_nbytes << 3;
	skip_بلاک_bitmap:

		إذا (!do_inode)
			استمر;

		إذا (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			اذهب skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		إذا (retval)
			اذهب errout;

		retval = ext2fs_inode_bitmap_csum_سیٹ(fs, i, inode_buf,
						      inode_nbytes);
		إذا (retval)
			اذهب errout;
		ext2fs_گروہ_desc_csum_سیٹ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاکاں_گنتی(fs->super)) {
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
	إذا (do_بلاک) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&بلاک_buf);
	}
	إذا (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	أرجع 0;
errout:
	إذا (inode_buf)
		ext2fs_free_mem(&inode_buf);
	إذا (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
	أرجع retval;
}

ساكن errcode_t mark_uninit_bg_گروہ_بلاکاں(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_بلاک_bitmap	bmap = fs->بلاک_map;

	لكل (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		إذا (!ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاک_UNINIT))
			استمر;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		إذا (blk)
			ext2fs_mark_بلاک_bitmap_range2(bmap, blk,
						fs->inode_بلاکاں_فی_گروہ);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_بلاک_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاکاں_گنتی(fs->super))
			ext2fs_mark_بلاک_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاکاں_گنتی(fs->super))
			ext2fs_mark_بلاک_bitmap2(bmap, blk);
	}
	أرجع 0;
}

ساكن صحيح bitmap_پوچھ_verify(بنانشانی حرف *bitmap, صحيح پہلا, صحيح آخری)
{
	صحيح i;

	لكل (i = پہلا; i <= آخری; i++)
		إذا (bitmap[i] != 0xff)
			أرجع 0;
	أرجع 1;
}

ساكن errcode_t read_bitmaps_range_prepare(ext2_filsys fs, صحيح flags)
{
	errcode_t retval;
	صحيح بلاک_nbytes = EXT2_CLUSTERS_فی_گروہ(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_فی_گروہ(fs->super) / 8;
	حرف *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا ((بلاک_nbytes > (صحيح) fs->blocksize) ||
	    (inode_nbytes > (صحيح) fs->blocksize))
		أرجع EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(لڑی_لمبائی(fs->device_name) + 80, &buf);
	إذا (retval)
		أرجع retval;

	إذا (flags & EXT2_FS_BITMAPS_بلاک) {
		إذا (fs->بلاک_map)
			ext2fs_free_بلاک_bitmap(fs->بلاک_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_بلاک_bitmap(fs, buf, &fs->بلاک_map);
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
	إذا (flags & EXT2_FS_BITMAPS_بلاک) {
		ext2fs_free_بلاک_bitmap(fs->بلاک_map);
		fs->بلاک_map = 0;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	أرجع retval;
}

ساكن errcode_t read_bitmaps_range_شروع(ext2_filsys fs, صحيح flags,
					  dgrp_t شروع, dgrp_t سرا,
					  mutex_t *mutex,
					  صحيح *پوچھ_flags)
{
	dgrp_t i;
	حرف *بلاک_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	صحيح بلاک_nbytes = EXT2_CLUSTERS_فی_گروہ(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_فی_گروہ(fs->super) / 8;
	صحيح csum_flag;
	بنانشانی صحيح	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_پہلا_انکڑے_بلاک);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_گروہ_desc_csum(fs);

	إذا (flags & EXT2_FS_BITMAPS_بلاک) {
		retval = io_channel_alloc_buf(fs->io, 0, &بلاک_bitmap);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		بلاک_nbytes = 0;
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
		blk = (ext2fs_le32_منزل_cpu(fs->image_سرخی->وتھ_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_گنتی;
		طالما (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_سیٹ_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_منزل_cpu(fs->image_سرخی->وتھ_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_منزل_CLUSTERS(fs->super,
						  fs->گروہ_desc_گنتی);
		طالما (بلاک_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, بلاک_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_سیٹ_بلاک_bitmap_range2(fs->بلاک_map,
				       blk_itr, cnt, بلاک_bitmap);
			إذا (retval)
				اذهب cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		اذهب cleanup;
	}

	blk_itr += ((blk64_t)شروع * (بلاک_nbytes << 3));
	ino_itr += ((blk64_t)شروع * (inode_nbytes << 3));
	لكل (i = شروع; i <= سرا; i++) {
		إذا (بلاک_bitmap) {
			blk = ext2fs_بلاک_bitmap_loc(fs, i);
			إذا ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاک_UNINIT) &&
			     ext2fs_گروہ_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلاکاں_گنتی(fs->super)))
				blk = 0;
			إذا (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, بلاک_bitmap);
				إذا (retval) {
					retval = EXT2_ET_بلاک_BITMAP_READ;
					اذهب cleanup;
				}
				/* verify block bitmap checksum */
				إذا (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_بلاک_bitmap_csum_verify(fs, i,
						بلاک_bitmap, بلاک_nbytes)) {
					retval =
					EXT2_ET_بلاک_BITMAP_CSUM_INVALID;
					اذهب cleanup;
				}
				إذا (!bitmap_پوچھ_verify((بنانشانی حرف *) بلاک_bitmap,
							بلاک_nbytes, fs->blocksize - 1))
					*پوچھ_flags |= EXT2_FLAG_BBITMAP_پوچھ_PROBLEM;
			} وإلا
				memset(بلاک_bitmap, 0, بلاک_nbytes);
			cnt = بلاک_nbytes << 3;
			unix_pthread_mutex_جندرا(mutex);
			retval = ext2fs_سیٹ_بلاک_bitmap_range2(fs->بلاک_map,
					       blk_itr, cnt, بلاک_bitmap);
			unix_pthread_mutex_unlock(mutex);
			إذا (retval)
				اذهب cleanup;
			blk_itr += بلاک_nbytes << 3;
		}
		إذا (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			إذا ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_گروہ_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلاکاں_گنتی(fs->super)))
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
				إذا (!bitmap_پوچھ_verify((بنانشانی حرف *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*پوچھ_flags |= EXT2_FLAG_IBITMAP_پوچھ_PROBLEM;
			} وإلا
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_جندرا(mutex);
			retval = ext2fs_سیٹ_inode_bitmap_range2(fs->inode_map,
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
	إذا (بلاک_bitmap)
		ext2fs_free_mem(&بلاک_bitmap);
	أرجع retval;
}

ساكن errcode_t read_bitmaps_range_سرا(ext2_filsys fs, صحيح flags,
					صحيح پوچھ_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	إذا (flags & EXT2_FS_BITMAPS_بلاک) {
		retval = mark_uninit_bg_گروہ_بلاکاں(fs);
		إذا (retval)
			أرجع retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_پوچھ_PROBLEM;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_پوچھ_PROBLEM;
	fs->flags |= پوچھ_flags;

	أرجع 0;
}

ساكن فراغ read_bitmaps_cleanup_on_error(ext2_filsys fs, صحيح flags)
{
	إذا (flags & EXT2_FS_BITMAPS_بلاک) {
		ext2fs_free_بلاک_bitmap(fs->بلاک_map);
		fs->بلاک_map = 0;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ساكن errcode_t read_bitmaps_range(ext2_filsys fs, صحيح flags,
				    dgrp_t شروع, dgrp_t سرا)
{
	errcode_t retval;
	صحيح پوچھ_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		أرجع retval;

	retval = read_bitmaps_range_شروع(fs, flags, شروع, سرا,
					  عدم, &پوچھ_flags);
	إذا (retval == 0)
		retval = read_bitmaps_range_سرا(fs, flags, پوچھ_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	أرجع retval;
}

#جے_تعریف_شدہ HAVE_PTHREAD
بنية read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	صحيح		rbt_flags;
	dgrp_t		rbt_grp_شروع;
	dgrp_t		rbt_grp_سرا;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	صحيح		rbt_پوچھ_flags;
};

ساكن فراغ *read_bitmaps_thread(فراغ *انکڑے)
{
	بنية read_bitmaps_thread_info *rbt = انکڑے;

	rbt->rbt_retval = read_bitmaps_range_شروع(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_شروع, rbt->rbt_grp_سرا,
				rbt->rbt_mutex, &rbt->rbt_پوچھ_flags);
	أرجع عدم;
}
#جے_انت

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحيح flags, صحيح عدد_دھاگے)
{
#جے_تعریف_شدہ HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = عدم;
	بنية read_bitmaps_thread_info *thread_infos = عدم;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	بنانشانی flexbg_ناپ = 1U << fs->super->s_log_groups_فی_flex;
	dgrp_t اوسطن_گروہ;
	صحيح i, پوچھ_flags = 0;
#جے_انت

	إذا (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		أرجع EXT2_ET_INVALID_دلیل;

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (flags & EXT2FS_BITMAPS_WRITE)
		أرجع write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2_FS_BITMAPS_بلاک);

#جے_تعریف_شدہ HAVE_PTHREAD
	إذا (((fs->io->flags & CHANNEL_FLAGS_دھاگے) == 0) ||
	    (عدد_دھاگے == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		اذهب fallback;

#إذا defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	إذا (عدد_دھاگے < 0)
		عدد_دھاگے = sysconf(_SC_NPROCESSORS_CONF);
#جے_انت
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	إذا (عدد_دھاگے <= 0)
		عدد_دھاگے = 4;

	إذا ((بنانشانی) عدد_دھاگے > fs->گروہ_desc_گنتی)
		عدد_دھاگے = fs->گروہ_desc_گنتی;
	اوسطن_گروہ = fs->گروہ_desc_گنتی / عدد_دھاگے;
	إذا (ext2fs_has_feature_flex_bg(fs->super)) {
		اوسطن_گروہ = (اوسطن_گروہ / flexbg_ناپ) * flexbg_ناپ;
	}
	إذا ((عدد_دھاگے <= 1) || (اوسطن_گروہ == 0))
		اذهب fallback;

	io_channel_سیٹ_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	إذا (retval)
		أرجع retval;

	thread_ids = صفر_حافظہ_دیو(حجم(pthread_t), عدد_دھاگے);
	إذا (!thread_ids)
		أرجع ENOMEM;

	thread_infos = صفر_حافظہ_دیو(حجم(بنية read_bitmaps_thread_info),
				عدد_دھاگے);
	إذا (!thread_infos)
		اذهب باہرلا;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		اذهب باہرلا;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	لكل (i = 0; i < عدد_دھاگے; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_پوچھ_flags = 0;
		إذا (i == 0)
			thread_infos[i].rbt_grp_شروع = 0;
		وإلا
			thread_infos[i].rbt_grp_شروع = اوسطن_گروہ * i + 1;

		إذا (i == عدد_دھاگے - 1)
			thread_infos[i].rbt_grp_سرا = fs->گروہ_desc_گنتی - 1;
		وإلا
			thread_infos[i].rbt_grp_سرا = اوسطن_گروہ * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		إذا (retval)
			اكسر;
	}
	لكل (i = 0; i < عدد_دھاگے; i++) {
		إذا (!thread_ids[i])
			اكسر;
		rc = pthread_join(thread_ids[i], عدم);
		إذا (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		إذا (rc && !retval)
			retval = rc;
		پوچھ_flags |= thread_infos[i].rbt_پوچھ_flags;
	}
باہرلا:
	rc = pthread_attr_destroy(&attr);
	إذا (rc && !retval)
		retval = rc;
	حرر(thread_infos);
	حرر(thread_ids);

	إذا (retval == 0)
		retval = read_bitmaps_range_سرا(fs, flags, پوچھ_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_سیٹ_options(fs->io, "cache=on");
	أرجع retval;
fallback:
#جے_انت /* HAVE_PTHREAD */
	أرجع read_bitmaps_range(fs, flags, 0, fs->گروہ_desc_گنتی - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	أرجع ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_بلاک_bitmap(ext2_filsys fs)
{
	أرجع ext2fs_rw_bitmaps(fs, EXT2_FS_BITMAPS_بلاک, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	أرجع write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_بلاک_bitmap (ext2_filsys fs)
{
	أرجع write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	صحيح flags = 0;

	إذا (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	إذا (!fs->بلاک_map)
		flags |= EXT2_FS_BITMAPS_بلاک;
	إذا (flags == 0)
		أرجع 0;
	أرجع ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	صحيح do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	صحيح do_بلاک = fs->بلاک_map && ext2fs_test_bb_dirty(fs);

	إذا (!do_inode && !do_بلاک)
		أرجع 0;

	أرجع write_bitmaps(fs, do_inode, do_بلاک);
}
