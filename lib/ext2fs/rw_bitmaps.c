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
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#که_تعریف_شوی HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#که_تعریف_شوی HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای
#که_تعریف_شوی HAVE_PTHREAD_H
#ضمن <pthread.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "e2image.h"

#که_تعریف_شوی HAVE_PTHREAD
تعريفنوع pthread_mutex_t mutex_t;

ساكن فراغ unix_pthread_mutex_قلف(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_قلف(mutex);
}
ساكن فراغ unix_pthread_mutex_unlock(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_unlock(mutex);
}
#وإلا
تعريفنوع صحيح mutex_t;
#تعریف_کړه unix_pthread_mutex_قلف(mutex_t) افعل {} طالما (0)
#تعریف_کړه unix_pthread_mutex_unlock(mutex_t) افعل {} طالما (0)
#د_که_پای

ساكن errcode_t write_bitmaps(ext2_filsys fs, صحيح do_inode, صحيح do_بلاک)
{
	dgrp_t 		i;
	بینښه صحيح	j;
	صحيح		بلاک_nbytes, inode_nbytes;
	بینښه صحيح	nbits;
	errcode_t	retval;
	حرف		*بلاک_buf = عدم, *inode_buf = عدم;
	صحيح		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_لومړی_معلومات_بلاک);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_ډله_desc_csum(fs);

	inode_nbytes = بلاک_nbytes = 0;
	إذا (do_بلاک) {
		بلاک_nbytes = EXT2_CLUSTERS_فی_ډله(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &بلاک_buf);
		إذا (retval)
			اذهب errout;
		memset(بلاک_buf, 0xff, fs->blocksize);
	}
	إذا (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_فی_ډله(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		إذا (retval)
			اذهب errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	لكل (i = 0; i < fs->ډله_desc_شمېر; i++) {
		إذا (!do_بلاک)
			اذهب skip_بلاک_bitmap;

		إذا (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاک_UNINIT)
		    )
			اذهب skip_this_بلاک_bitmap;

		retval = ext2fs_get_بلاک_bitmap_range2(fs->بلاک_map,
				blk_itr, بلاک_nbytes << 3, بلاک_buf);
		إذا (retval)
			اذهب errout;

		إذا (i == fs->ډله_desc_شمېر - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_عدد_B2_C(fs,
				((ext2fs_بلاکونه_شمېر(fs->super)
				  - (__u64) fs->super->s_لومړی_معلومات_بلاک)
				 % (__u64) EXT2_بلاکونه_فی_ډله(fs->super)));
			إذا (nbits)
				لكل (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_سېټ_بټ(j, بلاک_buf);
		}

		retval = ext2fs_بلاک_bitmap_csum_سېټ(fs, i, بلاک_buf,
						      بلاک_nbytes);
		إذا (retval)
			أرجع retval;
		ext2fs_ډله_desc_csum_سېټ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_بلاک_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاکونه_شمېر(fs->super)) {
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

		retval = ext2fs_inode_bitmap_csum_سېټ(fs, i, inode_buf,
						      inode_nbytes);
		إذا (retval)
			اذهب errout;
		ext2fs_ډله_desc_csum_سېټ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاکونه_شمېر(fs->super)) {
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

ساكن errcode_t mark_uninit_bg_ډله_بلاکونه(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_بلاک_bitmap	bmap = fs->بلاک_map;

	لكل (i = 0; i < fs->ډله_desc_شمېر; i++) {
		إذا (!ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاک_UNINIT))
			استمر;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		إذا (blk)
			ext2fs_mark_بلاک_bitmap_range2(bmap, blk,
						fs->inode_بلاکونه_فی_ډله);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_بلاک_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاکونه_شمېر(fs->super))
			ext2fs_mark_بلاک_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاکونه_شمېر(fs->super))
			ext2fs_mark_بلاک_bitmap2(bmap, blk);
	}
	أرجع 0;
}

ساكن صحيح bitmap_لکۍ_verify(بینښه حرف *bitmap, صحيح لومړی, صحيح وروستی)
{
	صحيح i;

	لكل (i = لومړی; i <= وروستی; i++)
		إذا (bitmap[i] != 0xff)
			أرجع 0;
	أرجع 1;
}

ساكن errcode_t read_bitmaps_range_prepare(ext2_filsys fs, صحيح flags)
{
	errcode_t retval;
	صحيح بلاک_nbytes = EXT2_CLUSTERS_فی_ډله(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_فی_ډله(fs->super) / 8;
	حرف *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا ((بلاک_nbytes > (صحيح) fs->blocksize) ||
	    (inode_nbytes > (صحيح) fs->blocksize))
		أرجع EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(مزي_اوږدوالی(fs->device_name) + 80, &buf);
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

ساكن errcode_t read_bitmaps_range_پیل(ext2_filsys fs, صحيح flags,
					  dgrp_t پیل, dgrp_t پای_ټکی,
					  mutex_t *mutex,
					  صحيح *لکۍ_flags)
{
	dgrp_t i;
	حرف *بلاک_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	صحيح بلاک_nbytes = EXT2_CLUSTERS_فی_ډله(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_فی_ډله(fs->super) / 8;
	صحيح csum_flag;
	بینښه صحيح	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_لومړی_معلومات_بلاک);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_ډله_desc_csum(fs);

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
		blk = (ext2fs_le32_منزل_cpu(fs->image_سرلیک->دننه_واټن_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_شمېر;
		طالما (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_سېټ_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_منزل_cpu(fs->image_سرلیک->دننه_واټن_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_منزل_CLUSTERS(fs->super,
						  fs->ډله_desc_شمېر);
		طالما (بلاک_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, بلاک_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_سېټ_بلاک_bitmap_range2(fs->بلاک_map,
				       blk_itr, cnt, بلاک_bitmap);
			إذا (retval)
				اذهب cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		اذهب cleanup;
	}

	blk_itr += ((blk64_t)پیل * (بلاک_nbytes << 3));
	ino_itr += ((blk64_t)پیل * (inode_nbytes << 3));
	لكل (i = پیل; i <= پای_ټکی; i++) {
		إذا (بلاک_bitmap) {
			blk = ext2fs_بلاک_bitmap_loc(fs, i);
			إذا ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاک_UNINIT) &&
			     ext2fs_ډله_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلاکونه_شمېر(fs->super)))
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
				إذا (!bitmap_لکۍ_verify((بینښه حرف *) بلاک_bitmap,
							بلاک_nbytes, fs->blocksize - 1))
					*لکۍ_flags |= EXT2_FLAG_BBITMAP_لکۍ_PROBLEM;
			} وإلا
				memset(بلاک_bitmap, 0, بلاک_nbytes);
			cnt = بلاک_nbytes << 3;
			unix_pthread_mutex_قلف(mutex);
			retval = ext2fs_سېټ_بلاک_bitmap_range2(fs->بلاک_map,
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
			     ext2fs_ډله_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلاکونه_شمېر(fs->super)))
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
				إذا (!bitmap_لکۍ_verify((بینښه حرف *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*لکۍ_flags |= EXT2_FLAG_IBITMAP_لکۍ_PROBLEM;
			} وإلا
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_قلف(mutex);
			retval = ext2fs_سېټ_inode_bitmap_range2(fs->inode_map,
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

ساكن errcode_t read_bitmaps_range_پای_ټکی(ext2_filsys fs, صحيح flags,
					صحيح لکۍ_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	إذا (flags & EXT2_FS_BITMAPS_بلاک) {
		retval = mark_uninit_bg_ډله_بلاکونه(fs);
		إذا (retval)
			أرجع retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_لکۍ_PROBLEM;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_لکۍ_PROBLEM;
	fs->flags |= لکۍ_flags;

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
				    dgrp_t پیل, dgrp_t پای_ټکی)
{
	errcode_t retval;
	صحيح لکۍ_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		أرجع retval;

	retval = read_bitmaps_range_پیل(fs, flags, پیل, پای_ټکی,
					  عدم, &لکۍ_flags);
	إذا (retval == 0)
		retval = read_bitmaps_range_پای_ټکی(fs, flags, لکۍ_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	أرجع retval;
}

#که_تعریف_شوی HAVE_PTHREAD
بنية read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	صحيح		rbt_flags;
	dgrp_t		rbt_grp_پیل;
	dgrp_t		rbt_grp_پای_ټکی;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	صحيح		rbt_لکۍ_flags;
};

ساكن فراغ *read_bitmaps_thread(فراغ *معلومات)
{
	بنية read_bitmaps_thread_info *rbt = معلومات;

	rbt->rbt_retval = read_bitmaps_range_پیل(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_پیل, rbt->rbt_grp_پای_ټکی,
				rbt->rbt_mutex, &rbt->rbt_لکۍ_flags);
	أرجع عدم;
}
#د_که_پای

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحيح flags, صحيح عدد_تارونه)
{
#که_تعریف_شوی HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = عدم;
	بنية read_bitmaps_thread_info *thread_infos = عدم;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	بینښه flexbg_اندازه = 1U << fs->super->s_log_groups_فی_flex;
	dgrp_t اوسط_ډله;
	صحيح i, لکۍ_flags = 0;
#د_که_پای

	إذا (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		أرجع EXT2_ET_INVALID_دلیل;

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (flags & EXT2FS_BITMAPS_WRITE)
		أرجع write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2_FS_BITMAPS_بلاک);

#که_تعریف_شوی HAVE_PTHREAD
	إذا (((fs->io->flags & CHANNEL_FLAGS_تارونه) == 0) ||
	    (عدد_تارونه == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		اذهب fallback;

#إذا defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	إذا (عدد_تارونه < 0)
		عدد_تارونه = sysconf(_SC_NPROCESSORS_CONF);
#د_که_پای
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	إذا (عدد_تارونه <= 0)
		عدد_تارونه = 4;

	إذا ((بینښه) عدد_تارونه > fs->ډله_desc_شمېر)
		عدد_تارونه = fs->ډله_desc_شمېر;
	اوسط_ډله = fs->ډله_desc_شمېر / عدد_تارونه;
	إذا (ext2fs_has_feature_flex_bg(fs->super)) {
		اوسط_ډله = (اوسط_ډله / flexbg_اندازه) * flexbg_اندازه;
	}
	إذا ((عدد_تارونه <= 1) || (اوسط_ډله == 0))
		اذهب fallback;

	io_channel_سېټ_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	إذا (retval)
		أرجع retval;

	thread_ids = حافظه_صفر_ورکړه(حجم(pthread_t), عدد_تارونه);
	إذا (!thread_ids)
		أرجع ENOMEM;

	thread_infos = حافظه_صفر_ورکړه(حجم(بنية read_bitmaps_thread_info),
				عدد_تارونه);
	إذا (!thread_infos)
		اذهب بهر;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		اذهب بهر;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	لكل (i = 0; i < عدد_تارونه; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_لکۍ_flags = 0;
		إذا (i == 0)
			thread_infos[i].rbt_grp_پیل = 0;
		وإلا
			thread_infos[i].rbt_grp_پیل = اوسط_ډله * i + 1;

		إذا (i == عدد_تارونه - 1)
			thread_infos[i].rbt_grp_پای_ټکی = fs->ډله_desc_شمېر - 1;
		وإلا
			thread_infos[i].rbt_grp_پای_ټکی = اوسط_ډله * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		إذا (retval)
			اكسر;
	}
	لكل (i = 0; i < عدد_تارونه; i++) {
		إذا (!thread_ids[i])
			اكسر;
		rc = pthread_join(thread_ids[i], عدم);
		إذا (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		إذا (rc && !retval)
			retval = rc;
		لکۍ_flags |= thread_infos[i].rbt_لکۍ_flags;
	}
بهر:
	rc = pthread_attr_destroy(&attr);
	إذا (rc && !retval)
		retval = rc;
	حرر(thread_infos);
	حرر(thread_ids);

	إذا (retval == 0)
		retval = read_bitmaps_range_پای_ټکی(fs, flags, لکۍ_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_سېټ_options(fs->io, "cache=on");
	أرجع retval;
fallback:
#د_که_پای /* HAVE_PTHREAD */
	أرجع read_bitmaps_range(fs, flags, 0, fs->ډله_desc_شمېر - 1);
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
