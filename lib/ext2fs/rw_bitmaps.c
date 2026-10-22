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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#جيڪڏهن_وصف_ٿيل HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD_H
#ضمن <pthread.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "e2image.h"

#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
تعريفنوع pthread_mutex_t mutex_t;

ساكن فراغ unix_pthread_mutex_تالو(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_تالو(mutex);
}
ساكن فراغ unix_pthread_mutex_unlock(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_unlock(mutex);
}
#وإلا
تعريفنوع صحيح mutex_t;
#وصف_ڪريو unix_pthread_mutex_تالو(mutex_t) افعل {} طالما (0)
#وصف_ڪريو unix_pthread_mutex_unlock(mutex_t) افعل {} طالما (0)
#جيڪڏهن_پورو

ساكن errcode_t write_bitmaps(ext2_filsys fs, صحيح do_inode, صحيح do_بلاڪ)
{
	dgrp_t 		i;
	بنانشان صحيح	j;
	صحيح		بلاڪ_nbytes, inode_nbytes;
	بنانشان صحيح	nbits;
	errcode_t	retval;
	حرف		*بلاڪ_buf = عدم, *inode_buf = عدم;
	صحيح		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_پهريون_معلومات_بلاڪ);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_گروهه_desc_csum(fs);

	inode_nbytes = بلاڪ_nbytes = 0;
	إذا (do_بلاڪ) {
		بلاڪ_nbytes = EXT2_CLUSTERS_في_گروهه(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &بلاڪ_buf);
		إذا (retval)
			اذهب errout;
		memset(بلاڪ_buf, 0xff, fs->blocksize);
	}
	إذا (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_في_گروهه(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		إذا (retval)
			اذهب errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	لكل (i = 0; i < fs->گروهه_desc_ڳڻپ; i++) {
		إذا (!do_بلاڪ)
			اذهب skip_بلاڪ_bitmap;

		إذا (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاڪ_UNINIT)
		    )
			اذهب skip_this_بلاڪ_bitmap;

		retval = ext2fs_get_بلاڪ_bitmap_range2(fs->بلاڪ_map,
				blk_itr, بلاڪ_nbytes << 3, بلاڪ_buf);
		إذا (retval)
			اذهب errout;

		إذا (i == fs->گروهه_desc_ڳڻپ - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_انگ_B2_C(fs,
				((ext2fs_بلاڪون_ڳڻپ(fs->super)
				  - (__u64) fs->super->s_پهريون_معلومات_بلاڪ)
				 % (__u64) EXT2_بلاڪون_في_گروهه(fs->super)));
			إذا (nbits)
				لكل (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_سيٽ_ٻٽو_انگ(j, بلاڪ_buf);
		}

		retval = ext2fs_بلاڪ_bitmap_csum_سيٽ(fs, i, بلاڪ_buf,
						      بلاڪ_nbytes);
		إذا (retval)
			أرجع retval;
		ext2fs_گروهه_desc_csum_سيٽ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_بلاڪ_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاڪون_ڳڻپ(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							بلاڪ_buf);
			إذا (retval) {
				retval = EXT2_ET_بلاڪ_BITMAP_WRITE;
				اذهب errout;
			}
		}
	skip_this_بلاڪ_bitmap:
		blk_itr += بلاڪ_nbytes << 3;
	skip_بلاڪ_bitmap:

		إذا (!do_inode)
			استمر;

		إذا (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			اذهب skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		إذا (retval)
			اذهب errout;

		retval = ext2fs_inode_bitmap_csum_سيٽ(fs, i, inode_buf,
						      inode_nbytes);
		إذا (retval)
			اذهب errout;
		ext2fs_گروهه_desc_csum_سيٽ(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاڪون_ڳڻپ(fs->super)) {
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
	إذا (do_بلاڪ) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&بلاڪ_buf);
	}
	إذا (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	أرجع 0;
errout:
	إذا (inode_buf)
		ext2fs_free_mem(&inode_buf);
	إذا (بلاڪ_buf)
		ext2fs_free_mem(&بلاڪ_buf);
	أرجع retval;
}

ساكن errcode_t mark_uninit_bg_گروهه_بلاڪون(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_بلاڪ_bitmap	bmap = fs->بلاڪ_map;

	لكل (i = 0; i < fs->گروهه_desc_ڳڻپ; i++) {
		إذا (!ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاڪ_UNINIT))
			استمر;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		إذا (blk)
			ext2fs_mark_بلاڪ_bitmap_range2(bmap, blk,
						fs->inode_بلاڪون_في_گروهه);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_بلاڪ_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاڪون_ڳڻپ(fs->super))
			ext2fs_mark_بلاڪ_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلاڪون_ڳڻپ(fs->super))
			ext2fs_mark_بلاڪ_bitmap2(bmap, blk);
	}
	أرجع 0;
}

ساكن صحيح bitmap_پڇ_verify(بنانشان حرف *bitmap, صحيح پهريون, صحيح آخري)
{
	صحيح i;

	لكل (i = پهريون; i <= آخري; i++)
		إذا (bitmap[i] != 0xff)
			أرجع 0;
	أرجع 1;
}

ساكن errcode_t read_bitmaps_range_prepare(ext2_filsys fs, صحيح flags)
{
	errcode_t retval;
	صحيح بلاڪ_nbytes = EXT2_CLUSTERS_في_گروهه(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_في_گروهه(fs->super) / 8;
	حرف *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا ((بلاڪ_nbytes > (صحيح) fs->blocksize) ||
	    (inode_nbytes > (صحيح) fs->blocksize))
		أرجع EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(لڙي_ڊيگهه(fs->device_name) + 80, &buf);
	إذا (retval)
		أرجع retval;

	إذا (flags & EXT2_FS_BITMAPS_بلاڪ) {
		إذا (fs->بلاڪ_map)
			ext2fs_free_بلاڪ_bitmap(fs->بلاڪ_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_بلاڪ_bitmap(fs, buf, &fs->بلاڪ_map);
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
	إذا (flags & EXT2_FS_BITMAPS_بلاڪ) {
		ext2fs_free_بلاڪ_bitmap(fs->بلاڪ_map);
		fs->بلاڪ_map = 0;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	أرجع retval;
}

ساكن errcode_t read_bitmaps_range_شروعات(ext2_filsys fs, صحيح flags,
					  dgrp_t شروعات, dgrp_t پڇاڙي,
					  mutex_t *mutex,
					  صحيح *پڇ_flags)
{
	dgrp_t i;
	حرف *بلاڪ_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	صحيح بلاڪ_nbytes = EXT2_CLUSTERS_في_گروهه(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_في_گروهه(fs->super) / 8;
	صحيح csum_flag;
	بنانشان صحيح	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_پهريون_معلومات_بلاڪ);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_گروهه_desc_csum(fs);

	إذا (flags & EXT2_FS_BITMAPS_بلاڪ) {
		retval = io_channel_alloc_buf(fs->io, 0, &بلاڪ_bitmap);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		بلاڪ_nbytes = 0;
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
		blk = (ext2fs_le32_منزل_cpu(fs->image_سرو->وٿي_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_ڳڻپ;
		طالما (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_سيٽ_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_منزل_cpu(fs->image_سرو->وٿي_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_منزل_CLUSTERS(fs->super,
						  fs->گروهه_desc_ڳڻپ);
		طالما (بلاڪ_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, بلاڪ_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_سيٽ_بلاڪ_bitmap_range2(fs->بلاڪ_map,
				       blk_itr, cnt, بلاڪ_bitmap);
			إذا (retval)
				اذهب cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		اذهب cleanup;
	}

	blk_itr += ((blk64_t)شروعات * (بلاڪ_nbytes << 3));
	ino_itr += ((blk64_t)شروعات * (inode_nbytes << 3));
	لكل (i = شروعات; i <= پڇاڙي; i++) {
		إذا (بلاڪ_bitmap) {
			blk = ext2fs_بلاڪ_bitmap_loc(fs, i);
			إذا ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_بلاڪ_UNINIT) &&
			     ext2fs_گروهه_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلاڪون_ڳڻپ(fs->super)))
				blk = 0;
			إذا (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, بلاڪ_bitmap);
				إذا (retval) {
					retval = EXT2_ET_بلاڪ_BITMAP_READ;
					اذهب cleanup;
				}
				/* verify block bitmap checksum */
				إذا (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_بلاڪ_bitmap_csum_verify(fs, i,
						بلاڪ_bitmap, بلاڪ_nbytes)) {
					retval =
					EXT2_ET_بلاڪ_BITMAP_CSUM_INVALID;
					اذهب cleanup;
				}
				إذا (!bitmap_پڇ_verify((بنانشان حرف *) بلاڪ_bitmap,
							بلاڪ_nbytes, fs->blocksize - 1))
					*پڇ_flags |= EXT2_FLAG_BBITMAP_پڇ_PROBLEM;
			} وإلا
				memset(بلاڪ_bitmap, 0, بلاڪ_nbytes);
			cnt = بلاڪ_nbytes << 3;
			unix_pthread_mutex_تالو(mutex);
			retval = ext2fs_سيٽ_بلاڪ_bitmap_range2(fs->بلاڪ_map,
					       blk_itr, cnt, بلاڪ_bitmap);
			unix_pthread_mutex_unlock(mutex);
			إذا (retval)
				اذهب cleanup;
			blk_itr += بلاڪ_nbytes << 3;
		}
		إذا (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			إذا ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_گروهه_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلاڪون_ڳڻپ(fs->super)))
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
				إذا (!bitmap_پڇ_verify((بنانشان حرف *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*پڇ_flags |= EXT2_FLAG_IBITMAP_پڇ_PROBLEM;
			} وإلا
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_تالو(mutex);
			retval = ext2fs_سيٽ_inode_bitmap_range2(fs->inode_map,
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
	إذا (بلاڪ_bitmap)
		ext2fs_free_mem(&بلاڪ_bitmap);
	أرجع retval;
}

ساكن errcode_t read_bitmaps_range_پڇاڙي(ext2_filsys fs, صحيح flags,
					صحيح پڇ_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	إذا (flags & EXT2_FS_BITMAPS_بلاڪ) {
		retval = mark_uninit_bg_گروهه_بلاڪون(fs);
		إذا (retval)
			أرجع retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_پڇ_PROBLEM;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_پڇ_PROBLEM;
	fs->flags |= پڇ_flags;

	أرجع 0;
}

ساكن فراغ read_bitmaps_cleanup_on_error(ext2_filsys fs, صحيح flags)
{
	إذا (flags & EXT2_FS_BITMAPS_بلاڪ) {
		ext2fs_free_بلاڪ_bitmap(fs->بلاڪ_map);
		fs->بلاڪ_map = 0;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ساكن errcode_t read_bitmaps_range(ext2_filsys fs, صحيح flags,
				    dgrp_t شروعات, dgrp_t پڇاڙي)
{
	errcode_t retval;
	صحيح پڇ_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		أرجع retval;

	retval = read_bitmaps_range_شروعات(fs, flags, شروعات, پڇاڙي,
					  عدم, &پڇ_flags);
	إذا (retval == 0)
		retval = read_bitmaps_range_پڇاڙي(fs, flags, پڇ_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	أرجع retval;
}

#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
بنية read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	صحيح		rbt_flags;
	dgrp_t		rbt_grp_شروعات;
	dgrp_t		rbt_grp_پڇاڙي;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	صحيح		rbt_پڇ_flags;
};

ساكن فراغ *read_bitmaps_thread(فراغ *معلومات)
{
	بنية read_bitmaps_thread_info *rbt = معلومات;

	rbt->rbt_retval = read_bitmaps_range_شروعات(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_شروعات, rbt->rbt_grp_پڇاڙي,
				rbt->rbt_mutex, &rbt->rbt_پڇ_flags);
	أرجع عدم;
}
#جيڪڏهن_پورو

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحيح flags, صحيح انگ_ڌاڳا)
{
#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = عدم;
	بنية read_bitmaps_thread_info *thread_infos = عدم;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	بنانشان flexbg_سائيز = 1U << fs->super->s_log_groups_في_flex;
	dgrp_t اوسط_گروهه;
	صحيح i, پڇ_flags = 0;
#جيڪڏهن_پورو

	إذا (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		أرجع EXT2_ET_INVALID_دليل;

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (flags & EXT2FS_BITMAPS_WRITE)
		أرجع write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2_FS_BITMAPS_بلاڪ);

#جيڪڏهن_وصف_ٿيل HAVE_PTHREAD
	إذا (((fs->io->flags & CHANNEL_FLAGS_ڌاڳا) == 0) ||
	    (انگ_ڌاڳا == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		اذهب fallback;

#إذا defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	إذا (انگ_ڌاڳا < 0)
		انگ_ڌاڳا = sysconf(_SC_NPROCESSORS_CONF);
#جيڪڏهن_پورو
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	إذا (انگ_ڌاڳا <= 0)
		انگ_ڌاڳا = 4;

	إذا ((بنانشان) انگ_ڌاڳا > fs->گروهه_desc_ڳڻپ)
		انگ_ڌاڳا = fs->گروهه_desc_ڳڻپ;
	اوسط_گروهه = fs->گروهه_desc_ڳڻپ / انگ_ڌاڳا;
	إذا (ext2fs_has_feature_flex_bg(fs->super)) {
		اوسط_گروهه = (اوسط_گروهه / flexbg_سائيز) * flexbg_سائيز;
	}
	إذا ((انگ_ڌاڳا <= 1) || (اوسط_گروهه == 0))
		اذهب fallback;

	io_channel_سيٽ_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	إذا (retval)
		أرجع retval;

	thread_ids = صفر_يادگيري_ڏيو(حجم(pthread_t), انگ_ڌاڳا);
	إذا (!thread_ids)
		أرجع ENOMEM;

	thread_infos = صفر_يادگيري_ڏيو(حجم(بنية read_bitmaps_thread_info),
				انگ_ڌاڳا);
	إذا (!thread_infos)
		اذهب ٻاهريون;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		اذهب ٻاهريون;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	لكل (i = 0; i < انگ_ڌاڳا; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_پڇ_flags = 0;
		إذا (i == 0)
			thread_infos[i].rbt_grp_شروعات = 0;
		وإلا
			thread_infos[i].rbt_grp_شروعات = اوسط_گروهه * i + 1;

		إذا (i == انگ_ڌاڳا - 1)
			thread_infos[i].rbt_grp_پڇاڙي = fs->گروهه_desc_ڳڻپ - 1;
		وإلا
			thread_infos[i].rbt_grp_پڇاڙي = اوسط_گروهه * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		إذا (retval)
			اكسر;
	}
	لكل (i = 0; i < انگ_ڌاڳا; i++) {
		إذا (!thread_ids[i])
			اكسر;
		rc = pthread_join(thread_ids[i], عدم);
		إذا (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		إذا (rc && !retval)
			retval = rc;
		پڇ_flags |= thread_infos[i].rbt_پڇ_flags;
	}
ٻاهريون:
	rc = pthread_attr_destroy(&attr);
	إذا (rc && !retval)
		retval = rc;
	حرر(thread_infos);
	حرر(thread_ids);

	إذا (retval == 0)
		retval = read_bitmaps_range_پڇاڙي(fs, flags, پڇ_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_سيٽ_options(fs->io, "cache=on");
	أرجع retval;
fallback:
#جيڪڏهن_پورو /* HAVE_PTHREAD */
	أرجع read_bitmaps_range(fs, flags, 0, fs->گروهه_desc_ڳڻپ - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	أرجع ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_بلاڪ_bitmap(ext2_filsys fs)
{
	أرجع ext2fs_rw_bitmaps(fs, EXT2_FS_BITMAPS_بلاڪ, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	أرجع write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_بلاڪ_bitmap (ext2_filsys fs)
{
	أرجع write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	صحيح flags = 0;

	إذا (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	إذا (!fs->بلاڪ_map)
		flags |= EXT2_FS_BITMAPS_بلاڪ;
	إذا (flags == 0)
		أرجع 0;
	أرجع ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	صحيح do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	صحيح do_بلاڪ = fs->بلاڪ_map && ext2fs_test_bb_dirty(fs);

	إذا (!do_inode && !do_بلاڪ)
		أرجع 0;

	أرجع write_bitmaps(fs, do_inode, do_بلاڪ);
}
