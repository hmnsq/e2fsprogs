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
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#اگر_تعریف_شده HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر
#اگر_تعریف_شده HAVE_PTHREAD_H
#ضمن <pthread.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "e2image.h"

#اگر_تعریف_شده HAVE_PTHREAD
تعريفنوع pthread_mutex_t mutex_t;

ساكن فراغ unix_pthread_mutex_قفل(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_قفل(mutex);
}
ساكن فراغ unix_pthread_mutex_unlock(mutex_t *mutex)
{
	إذا (mutex)
		pthread_mutex_unlock(mutex);
}
#وإلا
تعريفنوع صحيح mutex_t;
#تعریف_کن unix_pthread_mutex_قفل(mutex_t) افعل {} طالما (0)
#تعریف_کن unix_pthread_mutex_unlock(mutex_t) افعل {} طالما (0)
#پایان_اگر

ساكن errcode_t write_bitmaps(ext2_filsys fs, صحيح do_inode, صحيح do_بلوک)
{
	dgrp_t 		i;
	بیعلامت صحيح	j;
	صحيح		بلوک_nbytes, inode_nbytes;
	بیعلامت صحيح	nbits;
	errcode_t	retval;
	حرف		*بلوک_buf = عدم, *inode_buf = عدم;
	صحيح		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_اولی_داده_بلوک);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_گروه_desc_csum(fs);

	inode_nbytes = بلوک_nbytes = 0;
	إذا (do_بلوک) {
		بلوک_nbytes = EXT2_CLUSTERS_در_هر_گروه(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &بلوک_buf);
		إذا (retval)
			اذهب errout;
		memset(بلوک_buf, 0xff, fs->blocksize);
	}
	إذا (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_در_هر_گروه(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		إذا (retval)
			اذهب errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	لكل (i = 0; i < fs->گروه_desc_شمار; i++) {
		إذا (!do_بلوک)
			اذهب skip_بلوک_bitmap;

		إذا (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_بلوک_UNINIT)
		    )
			اذهب skip_this_بلوک_bitmap;

		retval = ext2fs_get_بلوک_bitmap_range2(fs->بلوک_map,
				blk_itr, بلوک_nbytes << 3, بلوک_buf);
		إذا (retval)
			اذهب errout;

		إذا (i == fs->گروه_desc_شمار - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_رقم_B2_C(fs,
				((ext2fs_بلوکها_شمار(fs->super)
				  - (__u64) fs->super->s_اولی_داده_بلوک)
				 % (__u64) EXT2_بلوکها_در_هر_گروه(fs->super)));
			إذا (nbits)
				لكل (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_مجموعه_بیت(j, بلوک_buf);
		}

		retval = ext2fs_بلوک_bitmap_csum_مجموعه(fs, i, بلوک_buf,
						      بلوک_nbytes);
		إذا (retval)
			أرجع retval;
		ext2fs_گروه_desc_csum_مجموعه(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_بلوک_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلوکها_شمار(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							بلوک_buf);
			إذا (retval) {
				retval = EXT2_ET_بلوک_BITMAP_WRITE;
				اذهب errout;
			}
		}
	skip_this_بلوک_bitmap:
		blk_itr += بلوک_nbytes << 3;
	skip_بلوک_bitmap:

		إذا (!do_inode)
			استمر;

		إذا (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			اذهب skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		إذا (retval)
			اذهب errout;

		retval = ext2fs_inode_bitmap_csum_مجموعه(fs, i, inode_buf,
						      inode_nbytes);
		إذا (retval)
			اذهب errout;
		ext2fs_گروه_desc_csum_مجموعه(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلوکها_شمار(fs->super)) {
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
	إذا (do_بلوک) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&بلوک_buf);
	}
	إذا (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	أرجع 0;
errout:
	إذا (inode_buf)
		ext2fs_free_mem(&inode_buf);
	إذا (بلوک_buf)
		ext2fs_free_mem(&بلوک_buf);
	أرجع retval;
}

ساكن errcode_t mark_uninit_bg_گروه_بلوکها(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_بلوک_bitmap	bmap = fs->بلوک_map;

	لكل (i = 0; i < fs->گروه_desc_شمار; i++) {
		إذا (!ext2fs_bg_flags_test(fs, i, EXT2_BG_بلوک_UNINIT))
			استمر;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_جدول_loc(fs, i);
		إذا (blk)
			ext2fs_mark_بلوک_bitmap_range2(bmap, blk,
						fs->inode_بلوکها_در_هر_گروه);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_بلوک_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلوکها_شمار(fs->super))
			ext2fs_mark_بلوک_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		إذا (blk && blk < ext2fs_بلوکها_شمار(fs->super))
			ext2fs_mark_بلوک_bitmap2(bmap, blk);
	}
	أرجع 0;
}

ساكن صحيح bitmap_دم_verify(بیعلامت حرف *bitmap, صحيح اولی, صحيح آخرین)
{
	صحيح i;

	لكل (i = اولی; i <= آخرین; i++)
		إذا (bitmap[i] != 0xff)
			أرجع 0;
	أرجع 1;
}

ساكن errcode_t read_bitmaps_range_prepare(ext2_filsys fs, صحيح flags)
{
	errcode_t retval;
	صحيح بلوک_nbytes = EXT2_CLUSTERS_در_هر_گروه(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_در_هر_گروه(fs->super) / 8;
	حرف *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا ((بلوک_nbytes > (صحيح) fs->blocksize) ||
	    (inode_nbytes > (صحيح) fs->blocksize))
		أرجع EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(طول_رشته(fs->device_name) + 80, &buf);
	إذا (retval)
		أرجع retval;

	إذا (flags & EXT2_FS_BITMAPS_بلوک) {
		إذا (fs->بلوک_map)
			ext2fs_free_بلوک_bitmap(fs->بلوک_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_بلوک_bitmap(fs, buf, &fs->بلوک_map);
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
	إذا (flags & EXT2_FS_BITMAPS_بلوک) {
		ext2fs_free_بلوک_bitmap(fs->بلوک_map);
		fs->بلوک_map = 0;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	أرجع retval;
}

ساكن errcode_t read_bitmaps_range_شروع(ext2_filsys fs, صحيح flags,
					  dgrp_t شروع, dgrp_t انتها,
					  mutex_t *mutex,
					  صحيح *دم_flags)
{
	dgrp_t i;
	حرف *بلوک_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	صحيح بلوک_nbytes = EXT2_CLUSTERS_در_هر_گروه(fs->super) / 8;
	صحيح inode_nbytes = EXT2_INODES_در_هر_گروه(fs->super) / 8;
	صحيح csum_flag;
	بیعلامت صحيح	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_اولی_داده_بلوک);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_گروه_desc_csum(fs);

	إذا (flags & EXT2_FS_BITMAPS_بلوک) {
		retval = io_channel_alloc_buf(fs->io, 0, &بلوک_bitmap);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
		بلوک_nbytes = 0;
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
		blk = (ext2fs_le32_مقصد_cpu(fs->image_سرآیند->جابه_جایی_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_شمار;
		طالما (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_مجموعه_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			إذا (retval)
				اذهب cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_مقصد_cpu(fs->image_سرآیند->جابه_جایی_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_مقصد_CLUSTERS(fs->super,
						  fs->گروه_desc_شمار);
		طالما (بلوک_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, بلوک_bitmap);
			إذا (retval)
				اذهب cleanup;
			cnt = fs->blocksize << 3;
			إذا (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_مجموعه_بلوک_bitmap_range2(fs->بلوک_map,
				       blk_itr, cnt, بلوک_bitmap);
			إذا (retval)
				اذهب cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		اذهب cleanup;
	}

	blk_itr += ((blk64_t)شروع * (بلوک_nbytes << 3));
	ino_itr += ((blk64_t)شروع * (inode_nbytes << 3));
	لكل (i = شروع; i <= انتها; i++) {
		إذا (بلوک_bitmap) {
			blk = ext2fs_بلوک_bitmap_loc(fs, i);
			إذا ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_بلوک_UNINIT) &&
			     ext2fs_گروه_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلوکها_شمار(fs->super)))
				blk = 0;
			إذا (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, بلوک_bitmap);
				إذا (retval) {
					retval = EXT2_ET_بلوک_BITMAP_READ;
					اذهب cleanup;
				}
				/* verify block bitmap checksum */
				إذا (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_بلوک_bitmap_csum_verify(fs, i,
						بلوک_bitmap, بلوک_nbytes)) {
					retval =
					EXT2_ET_بلوک_BITMAP_CSUM_INVALID;
					اذهب cleanup;
				}
				إذا (!bitmap_دم_verify((بیعلامت حرف *) بلوک_bitmap,
							بلوک_nbytes, fs->blocksize - 1))
					*دم_flags |= EXT2_FLAG_BBITMAP_دم_PROBLEM;
			} وإلا
				memset(بلوک_bitmap, 0, بلوک_nbytes);
			cnt = بلوک_nbytes << 3;
			unix_pthread_mutex_قفل(mutex);
			retval = ext2fs_مجموعه_بلوک_bitmap_range2(fs->بلوک_map,
					       blk_itr, cnt, بلوک_bitmap);
			unix_pthread_mutex_unlock(mutex);
			إذا (retval)
				اذهب cleanup;
			blk_itr += بلوک_nbytes << 3;
		}
		إذا (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			إذا ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_گروه_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_بلوکها_شمار(fs->super)))
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
				إذا (!bitmap_دم_verify((بیعلامت حرف *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*دم_flags |= EXT2_FLAG_IBITMAP_دم_PROBLEM;
			} وإلا
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_قفل(mutex);
			retval = ext2fs_مجموعه_inode_bitmap_range2(fs->inode_map,
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
	إذا (بلوک_bitmap)
		ext2fs_free_mem(&بلوک_bitmap);
	أرجع retval;
}

ساكن errcode_t read_bitmaps_range_انتها(ext2_filsys fs, صحيح flags,
					صحيح دم_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	إذا (flags & EXT2_FS_BITMAPS_بلوک) {
		retval = mark_uninit_bg_گروه_بلوکها(fs);
		إذا (retval)
			أرجع retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_دم_PROBLEM;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_دم_PROBLEM;
	fs->flags |= دم_flags;

	أرجع 0;
}

ساكن فراغ read_bitmaps_cleanup_on_error(ext2_filsys fs, صحيح flags)
{
	إذا (flags & EXT2_FS_BITMAPS_بلوک) {
		ext2fs_free_بلوک_bitmap(fs->بلوک_map);
		fs->بلوک_map = 0;
	}
	إذا (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

ساكن errcode_t read_bitmaps_range(ext2_filsys fs, صحيح flags,
				    dgrp_t شروع, dgrp_t انتها)
{
	errcode_t retval;
	صحيح دم_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		أرجع retval;

	retval = read_bitmaps_range_شروع(fs, flags, شروع, انتها,
					  عدم, &دم_flags);
	إذا (retval == 0)
		retval = read_bitmaps_range_انتها(fs, flags, دم_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	أرجع retval;
}

#اگر_تعریف_شده HAVE_PTHREAD
بنية read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	صحيح		rbt_flags;
	dgrp_t		rbt_grp_شروع;
	dgrp_t		rbt_grp_انتها;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	صحيح		rbt_دم_flags;
};

ساكن فراغ *read_bitmaps_thread(فراغ *داده)
{
	بنية read_bitmaps_thread_info *rbt = داده;

	rbt->rbt_retval = read_bitmaps_range_شروع(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_شروع, rbt->rbt_grp_انتها,
				rbt->rbt_mutex, &rbt->rbt_دم_flags);
	أرجع عدم;
}
#پایان_اگر

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, صحيح flags, صحيح رقم_نخها)
{
#اگر_تعریف_شده HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = عدم;
	بنية read_bitmaps_thread_info *thread_infos = عدم;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	بیعلامت flexbg_اندازه = 1U << fs->super->s_log_groups_در_هر_flex;
	dgrp_t معدل_گروه;
	صحيح i, دم_flags = 0;
#پایان_اگر

	إذا (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		أرجع EXT2_ET_INVALID_آرگومان;

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (flags & EXT2FS_BITMAPS_WRITE)
		أرجع write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2_FS_BITMAPS_بلوک);

#اگر_تعریف_شده HAVE_PTHREAD
	إذا (((fs->io->flags & CHANNEL_FLAGS_نخها) == 0) ||
	    (رقم_نخها == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		اذهب fallback;

#إذا defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	إذا (رقم_نخها < 0)
		رقم_نخها = sysconf(_SC_NPROCESSORS_CONF);
#پایان_اگر
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	إذا (رقم_نخها <= 0)
		رقم_نخها = 4;

	إذا ((بیعلامت) رقم_نخها > fs->گروه_desc_شمار)
		رقم_نخها = fs->گروه_desc_شمار;
	معدل_گروه = fs->گروه_desc_شمار / رقم_نخها;
	إذا (ext2fs_has_feature_flex_bg(fs->super)) {
		معدل_گروه = (معدل_گروه / flexbg_اندازه) * flexbg_اندازه;
	}
	إذا ((رقم_نخها <= 1) || (معدل_گروه == 0))
		اذهب fallback;

	io_channel_مجموعه_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	إذا (retval)
		أرجع retval;

	thread_ids = حافظه_صفر_بده(حجم(pthread_t), رقم_نخها);
	إذا (!thread_ids)
		أرجع ENOMEM;

	thread_infos = حافظه_صفر_بده(حجم(بنية read_bitmaps_thread_info),
				رقم_نخها);
	إذا (!thread_infos)
		اذهب بیرون;

	retval = read_bitmaps_range_prepare(fs, flags);
	إذا (retval)
		اذهب بیرون;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	لكل (i = 0; i < رقم_نخها; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_دم_flags = 0;
		إذا (i == 0)
			thread_infos[i].rbt_grp_شروع = 0;
		وإلا
			thread_infos[i].rbt_grp_شروع = معدل_گروه * i + 1;

		إذا (i == رقم_نخها - 1)
			thread_infos[i].rbt_grp_انتها = fs->گروه_desc_شمار - 1;
		وإلا
			thread_infos[i].rbt_grp_انتها = معدل_گروه * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		إذا (retval)
			اكسر;
	}
	لكل (i = 0; i < رقم_نخها; i++) {
		إذا (!thread_ids[i])
			اكسر;
		rc = pthread_join(thread_ids[i], عدم);
		إذا (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		إذا (rc && !retval)
			retval = rc;
		دم_flags |= thread_infos[i].rbt_دم_flags;
	}
بیرون:
	rc = pthread_attr_destroy(&attr);
	إذا (rc && !retval)
		retval = rc;
	حرر(thread_infos);
	حرر(thread_ids);

	إذا (retval == 0)
		retval = read_bitmaps_range_انتها(fs, flags, دم_flags);
	إذا (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_مجموعه_options(fs->io, "cache=on");
	أرجع retval;
fallback:
#پایان_اگر /* HAVE_PTHREAD */
	أرجع read_bitmaps_range(fs, flags, 0, fs->گروه_desc_شمار - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	أرجع ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_بلوک_bitmap(ext2_filsys fs)
{
	أرجع ext2fs_rw_bitmaps(fs, EXT2_FS_BITMAPS_بلوک, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	أرجع write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_بلوک_bitmap (ext2_filsys fs)
{
	أرجع write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	صحيح flags = 0;

	إذا (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	إذا (!fs->بلوک_map)
		flags |= EXT2_FS_BITMAPS_بلوک;
	إذا (flags == 0)
		أرجع 0;
	أرجع ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	صحيح do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	صحيح do_بلوک = fs->بلوک_map && ext2fs_test_bb_dirty(fs);

	إذا (!do_inode && !do_بلوک)
		أرجع 0;

	أرجع write_bitmaps(fs, do_inode, do_بلوک);
}
