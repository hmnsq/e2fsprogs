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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#నిర్వచితమైతే HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_PTHREAD_H
#అన్తర్భావ <pthread.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "e2image.h"

#నిర్వచితమైతే HAVE_PTHREAD
రకనిర్వచనం pthread_mutex_t mutex_t;

స్థిర శూన్య unix_pthread_mutex_తాళం(mutex_t *mutex)
{
	ఒకవేళ (mutex)
		pthread_mutex_తాళం(mutex);
}
స్థిర శూన్య unix_pthread_mutex_unlock(mutex_t *mutex)
{
	ఒకవేళ (mutex)
		pthread_mutex_unlock(mutex);
}
#లేకపోతే
రకనిర్వచనం పూర్ణాంకం mutex_t;
#నిర్వచించు unix_pthread_mutex_తాళం(mutex_t) చేయి {} అయితే (0)
#నిర్వచించు unix_pthread_mutex_unlock(mutex_t) చేయి {} అయితే (0)
#అయితే_ముగింపు

స్థిర errcode_t write_bitmaps(ext2_filsys fs, పూర్ణాంకం do_inode, పూర్ణాంకం do_ఖండం)
{
	dgrp_t 		i;
	గుర్తులేని పూర్ణాంకం	j;
	పూర్ణాంకం		ఖండం_nbytes, inode_nbytes;
	గుర్తులేని పూర్ణాంకం	nbits;
	errcode_t	retval;
	అక్షరం		*ఖండం_buf = ఖాళీ, *inode_buf = ఖాళీ;
	పూర్ణాంకం		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_మొదటి_దత్తాంశం_ఖండం);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!(fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_సమూహం_desc_csum(fs);

	inode_nbytes = ఖండం_nbytes = 0;
	ఒకవేళ (do_ఖండం) {
		ఖండం_nbytes = EXT2_CLUSTERS_చొప్పున_సమూహం(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &ఖండం_buf);
		ఒకవేళ (retval)
			వెళ్ళు errout;
		memset(ఖండం_buf, 0xff, fs->blocksize);
	}
	ఒకవేళ (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_చొప్పున_సమూహం(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		ఒకవేళ (retval)
			వెళ్ళు errout;
		memset(inode_buf, 0xff, fs->blocksize);
	}

	కొరకు (i = 0; i < fs->సమూహం_desc_లెక్క; i++) {
		ఒకవేళ (!do_ఖండం)
			వెళ్ళు skip_ఖండం_bitmap;

		ఒకవేళ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_ఖండం_UNINIT)
		    )
			వెళ్ళు skip_this_ఖండం_bitmap;

		retval = ext2fs_get_ఖండం_bitmap_range2(fs->ఖండం_map,
				blk_itr, ఖండం_nbytes << 3, ఖండం_buf);
		ఒకవేళ (retval)
			వెళ్ళు errout;

		ఒకవేళ (i == fs->సమూహం_desc_లెక్క - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_అంకె_B2_C(fs,
				((ext2fs_ఖండాలు_లెక్క(fs->super)
				  - (__u64) fs->super->s_మొదటి_దత్తాంశం_ఖండం)
				 % (__u64) EXT2_ఖండాలు_చొప్పున_సమూహం(fs->super)));
			ఒకవేళ (nbits)
				కొరకు (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_సమితి_బిట్టు(j, ఖండం_buf);
		}

		retval = ext2fs_ఖండం_bitmap_csum_సమితి(fs, i, ఖండం_buf,
						      ఖండం_nbytes);
		ఒకవేళ (retval)
			తిరిగి retval;
		ext2fs_సమూహం_desc_csum_సమితి(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_ఖండం_bitmap_loc(fs, i);
		ఒకవేళ (blk && blk < ext2fs_ఖండాలు_లెక్క(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							ఖండం_buf);
			ఒకవేళ (retval) {
				retval = EXT2_ET_ఖండం_BITMAP_WRITE;
				వెళ్ళు errout;
			}
		}
	skip_this_ఖండం_bitmap:
		blk_itr += ఖండం_nbytes << 3;
	skip_ఖండం_bitmap:

		ఒకవేళ (!do_inode)
			కొనసాగు;

		ఒకవేళ (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			వెళ్ళు skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		ఒకవేళ (retval)
			వెళ్ళు errout;

		retval = ext2fs_inode_bitmap_csum_సమితి(fs, i, inode_buf,
						      inode_nbytes);
		ఒకవేళ (retval)
			వెళ్ళు errout;
		ext2fs_సమూహం_desc_csum_సమితి(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		ఒకవేళ (blk && blk < ext2fs_ఖండాలు_లెక్క(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			ఒకవేళ (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				వెళ్ళు errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	ఒకవేళ (do_ఖండం) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&ఖండం_buf);
	}
	ఒకవేళ (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	తిరిగి 0;
errout:
	ఒకవేళ (inode_buf)
		ext2fs_free_mem(&inode_buf);
	ఒకవేళ (ఖండం_buf)
		ext2fs_free_mem(&ఖండం_buf);
	తిరిగి retval;
}

స్థిర errcode_t mark_uninit_bg_సమూహం_ఖండాలు(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_ఖండం_bitmap	bmap = fs->ఖండం_map;

	కొరకు (i = 0; i < fs->సమూహం_desc_లెక్క; i++) {
		ఒకవేళ (!ext2fs_bg_flags_test(fs, i, EXT2_BG_ఖండం_UNINIT))
			కొనసాగు;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_పట్టిక_loc(fs, i);
		ఒకవేళ (blk)
			ext2fs_mark_ఖండం_bitmap_range2(bmap, blk,
						fs->inode_ఖండాలు_చొప్పున_సమూహం);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_ఖండం_bitmap_loc(fs, i);
		ఒకవేళ (blk && blk < ext2fs_ఖండాలు_లెక్క(fs->super))
			ext2fs_mark_ఖండం_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ఒకవేళ (blk && blk < ext2fs_ఖండాలు_లెక్క(fs->super))
			ext2fs_mark_ఖండం_bitmap2(bmap, blk);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం bitmap_తోక_verify(గుర్తులేని అక్షరం *bitmap, పూర్ణాంకం మొదటి, పూర్ణాంకం చివరి)
{
	పూర్ణాంకం i;

	కొరకు (i = మొదటి; i <= చివరి; i++)
		ఒకవేళ (bitmap[i] != 0xff)
			తిరిగి 0;
	తిరిగి 1;
}

స్థిర errcode_t read_bitmaps_range_prepare(ext2_filsys fs, పూర్ణాంకం flags)
{
	errcode_t retval;
	పూర్ణాంకం ఖండం_nbytes = EXT2_CLUSTERS_చొప్పున_సమూహం(fs->super) / 8;
	పూర్ణాంకం inode_nbytes = EXT2_INODES_చొప్పున_సమూహం(fs->super) / 8;
	అక్షరం *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ ((ఖండం_nbytes > (పూర్ణాంకం) fs->blocksize) ||
	    (inode_nbytes > (పూర్ణాంకం) fs->blocksize))
		తిరిగి EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(fs->device_name) + 80, &buf);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (flags & EXT2_FS_BITMAPS_ఖండం) {
		ఒకవేళ (fs->ఖండం_map)
			ext2fs_free_ఖండం_bitmap(fs->ఖండం_map);
		strcpy(buf, "block bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_ఖండం_bitmap(fs, buf, &fs->ఖండం_map);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}

	ఒకవేళ (flags & EXT2FS_BITMAPS_INODE) {
		ఒకవేళ (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		strcpy(buf, "inode bitmap for ");
		strcat(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}
	ext2fs_free_mem(&buf);
	తిరిగి retval;

cleanup:
	ఒకవేళ (flags & EXT2_FS_BITMAPS_ఖండం) {
		ext2fs_free_ఖండం_bitmap(fs->ఖండం_map);
		fs->ఖండం_map = 0;
	}
	ఒకవేళ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	తిరిగి retval;
}

స్థిర errcode_t read_bitmaps_range_ప్రారంభం(ext2_filsys fs, పూర్ణాంకం flags,
					  dgrp_t ప్రారంభం, dgrp_t చివర,
					  mutex_t *mutex,
					  పూర్ణాంకం *తోక_flags)
{
	dgrp_t i;
	అక్షరం *ఖండం_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	పూర్ణాంకం ఖండం_nbytes = EXT2_CLUSTERS_చొప్పున_సమూహం(fs->super) / 8;
	పూర్ణాంకం inode_nbytes = EXT2_INODES_చొప్పున_సమూహం(fs->super) / 8;
	పూర్ణాంకం csum_flag;
	గుర్తులేని పూర్ణాంకం	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_మొదటి_దత్తాంశం_ఖండం);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_సమూహం_desc_csum(fs);

	ఒకవేళ (flags & EXT2_FS_BITMAPS_ఖండం) {
		retval = io_channel_alloc_buf(fs->io, 0, &ఖండం_bitmap);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	} లేకపోతే {
		ఖండం_nbytes = 0;
	}

	ఒకవేళ (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	} లేకపోతే {
		inode_nbytes = 0;
	}

	/* io should be null */
	ఒకవేళ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_గమ్యం_cpu(fs->image_శీర్షిక->స్థానభ్రంశం_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_లెక్క;
		అయితే (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			ఒకవేళ (retval)
				వెళ్ళు cleanup;
			cnt = fs->blocksize << 3;
			ఒకవేళ (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_సమితి_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			ఒకవేళ (retval)
				వెళ్ళు cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_గమ్యం_cpu(fs->image_శీర్షిక->స్థానభ్రంశం_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_గమ్యం_CLUSTERS(fs->super,
						  fs->సమూహం_desc_లెక్క);
		అయితే (ఖండం_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, ఖండం_bitmap);
			ఒకవేళ (retval)
				వెళ్ళు cleanup;
			cnt = fs->blocksize << 3;
			ఒకవేళ (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_సమితి_ఖండం_bitmap_range2(fs->ఖండం_map,
				       blk_itr, cnt, ఖండం_bitmap);
			ఒకవేళ (retval)
				వెళ్ళు cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		వెళ్ళు cleanup;
	}

	blk_itr += ((blk64_t)ప్రారంభం * (ఖండం_nbytes << 3));
	ino_itr += ((blk64_t)ప్రారంభం * (inode_nbytes << 3));
	కొరకు (i = ప్రారంభం; i <= చివర; i++) {
		ఒకవేళ (ఖండం_bitmap) {
			blk = ext2fs_ఖండం_bitmap_loc(fs, i);
			ఒకవేళ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_ఖండం_UNINIT) &&
			     ext2fs_సమూహం_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_ఖండాలు_లెక్క(fs->super)))
				blk = 0;
			ఒకవేళ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, ఖండం_bitmap);
				ఒకవేళ (retval) {
					retval = EXT2_ET_ఖండం_BITMAP_READ;
					వెళ్ళు cleanup;
				}
				/* verify block bitmap checksum */
				ఒకవేళ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_ఖండం_bitmap_csum_verify(fs, i,
						ఖండం_bitmap, ఖండం_nbytes)) {
					retval =
					EXT2_ET_ఖండం_BITMAP_CSUM_INVALID;
					వెళ్ళు cleanup;
				}
				ఒకవేళ (!bitmap_తోక_verify((గుర్తులేని అక్షరం *) ఖండం_bitmap,
							ఖండం_nbytes, fs->blocksize - 1))
					*తోక_flags |= EXT2_FLAG_BBITMAP_తోక_PROBLEM;
			} లేకపోతే
				memset(ఖండం_bitmap, 0, ఖండం_nbytes);
			cnt = ఖండం_nbytes << 3;
			unix_pthread_mutex_తాళం(mutex);
			retval = ext2fs_సమితి_ఖండం_bitmap_range2(fs->ఖండం_map,
					       blk_itr, cnt, ఖండం_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ఒకవేళ (retval)
				వెళ్ళు cleanup;
			blk_itr += ఖండం_nbytes << 3;
		}
		ఒకవేళ (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			ఒకవేళ ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_సమూహం_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_ఖండాలు_లెక్క(fs->super)))
				blk = 0;
			ఒకవేళ (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				ఒకవేళ (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					వెళ్ళు cleanup;
				}

				/* verify inode bitmap checksum */
				ఒకవేళ (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					వెళ్ళు cleanup;
				}
				ఒకవేళ (!bitmap_తోక_verify((గుర్తులేని అక్షరం *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*తోక_flags |= EXT2_FLAG_IBITMAP_తోక_PROBLEM;
			} లేకపోతే
				memset(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_తాళం(mutex);
			retval = ext2fs_సమితి_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			ఒకవేళ (retval)
				వెళ్ళు cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	ఒకవేళ (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	ఒకవేళ (ఖండం_bitmap)
		ext2fs_free_mem(&ఖండం_bitmap);
	తిరిగి retval;
}

స్థిర errcode_t read_bitmaps_range_చివర(ext2_filsys fs, పూర్ణాంకం flags,
					పూర్ణాంకం తోక_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	ఒకవేళ (flags & EXT2_FS_BITMAPS_ఖండం) {
		retval = mark_uninit_bg_సమూహం_ఖండాలు(fs);
		ఒకవేళ (retval)
			తిరిగి retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_తోక_PROBLEM;
	}
	ఒకవేళ (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_తోక_PROBLEM;
	fs->flags |= తోక_flags;

	తిరిగి 0;
}

స్థిర శూన్య read_bitmaps_cleanup_on_error(ext2_filsys fs, పూర్ణాంకం flags)
{
	ఒకవేళ (flags & EXT2_FS_BITMAPS_ఖండం) {
		ext2fs_free_ఖండం_bitmap(fs->ఖండం_map);
		fs->ఖండం_map = 0;
	}
	ఒకవేళ (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

స్థిర errcode_t read_bitmaps_range(ext2_filsys fs, పూర్ణాంకం flags,
				    dgrp_t ప్రారంభం, dgrp_t చివర)
{
	errcode_t retval;
	పూర్ణాంకం తోక_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = read_bitmaps_range_ప్రారంభం(fs, flags, ప్రారంభం, చివర,
					  ఖాళీ, &తోక_flags);
	ఒకవేళ (retval == 0)
		retval = read_bitmaps_range_చివర(fs, flags, తోక_flags);
	ఒకవేళ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	తిరిగి retval;
}

#నిర్వచితమైతే HAVE_PTHREAD
నిర్మాణం read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	పూర్ణాంకం		rbt_flags;
	dgrp_t		rbt_grp_ప్రారంభం;
	dgrp_t		rbt_grp_చివర;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	పూర్ణాంకం		rbt_తోక_flags;
};

స్థిర శూన్య *read_bitmaps_thread(శూన్య *దత్తాంశం)
{
	నిర్మాణం read_bitmaps_thread_info *rbt = దత్తాంశం;

	rbt->rbt_retval = read_bitmaps_range_ప్రారంభం(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_ప్రారంభం, rbt->rbt_grp_చివర,
				rbt->rbt_mutex, &rbt->rbt_తోక_flags);
	తిరిగి ఖాళీ;
}
#అయితే_ముగింపు

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, పూర్ణాంకం flags, పూర్ణాంకం అంకె_దారాలు)
{
#నిర్వచితమైతే HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = ఖాళీ;
	నిర్మాణం read_bitmaps_thread_info *thread_infos = ఖాళీ;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	గుర్తులేని flexbg_కొలత = 1U << fs->super->s_log_groups_చొప్పున_flex;
	dgrp_t సరాసరి_సమూహం;
	పూర్ణాంకం i, తోక_flags = 0;
#అయితే_ముగింపు

	ఒకవేళ (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super))
		తిరిగి EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ఒకవేళ (flags & EXT2FS_BITMAPS_WRITE)
		తిరిగి write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2_FS_BITMAPS_ఖండం);

#నిర్వచితమైతే HAVE_PTHREAD
	ఒకవేళ (((fs->io->flags & CHANNEL_FLAGS_దారాలు) == 0) ||
	    (అంకె_దారాలు == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		వెళ్ళు fallback;

#ఒకవేళ defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	ఒకవేళ (అంకె_దారాలు < 0)
		అంకె_దారాలు = sysconf(_SC_NPROCESSORS_CONF);
#అయితే_ముగింపు
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	ఒకవేళ (అంకె_దారాలు <= 0)
		అంకె_దారాలు = 4;

	ఒకవేళ ((గుర్తులేని) అంకె_దారాలు > fs->సమూహం_desc_లెక్క)
		అంకె_దారాలు = fs->సమూహం_desc_లెక్క;
	సరాసరి_సమూహం = fs->సమూహం_desc_లెక్క / అంకె_దారాలు;
	ఒకవేళ (ext2fs_has_feature_flex_bg(fs->super)) {
		సరాసరి_సమూహం = (సరాసరి_సమూహం / flexbg_కొలత) * flexbg_కొలత;
	}
	ఒకవేళ ((అంకె_దారాలు <= 1) || (సరాసరి_సమూహం == 0))
		వెళ్ళు fallback;

	io_channel_సమితి_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	ఒకవేళ (retval)
		తిరిగి retval;

	thread_ids = సున్నా_జ్ఞాపకం_ఇవ్వు(పరిమాణం(pthread_t), అంకె_దారాలు);
	ఒకవేళ (!thread_ids)
		తిరిగి ENOMEM;

	thread_infos = సున్నా_జ్ఞాపకం_ఇవ్వు(పరిమాణం(నిర్మాణం read_bitmaps_thread_info),
				అంకె_దారాలు);
	ఒకవేళ (!thread_infos)
		వెళ్ళు బహిర్గతం;

	retval = read_bitmaps_range_prepare(fs, flags);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	కొరకు (i = 0; i < అంకె_దారాలు; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_తోక_flags = 0;
		ఒకవేళ (i == 0)
			thread_infos[i].rbt_grp_ప్రారంభం = 0;
		లేకపోతే
			thread_infos[i].rbt_grp_ప్రారంభం = సరాసరి_సమూహం * i + 1;

		ఒకవేళ (i == అంకె_దారాలు - 1)
			thread_infos[i].rbt_grp_చివర = fs->సమూహం_desc_లెక్క - 1;
		లేకపోతే
			thread_infos[i].rbt_grp_చివర = సరాసరి_సమూహం * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		ఒకవేళ (retval)
			ఆపు;
	}
	కొరకు (i = 0; i < అంకె_దారాలు; i++) {
		ఒకవేళ (!thread_ids[i])
			ఆపు;
		rc = pthread_join(thread_ids[i], ఖాళీ);
		ఒకవేళ (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		ఒకవేళ (rc && !retval)
			retval = rc;
		తోక_flags |= thread_infos[i].rbt_తోక_flags;
	}
బహిర్గతం:
	rc = pthread_attr_destroy(&attr);
	ఒకవేళ (rc && !retval)
		retval = rc;
	విడుదల(thread_infos);
	విడుదల(thread_ids);

	ఒకవేళ (retval == 0)
		retval = read_bitmaps_range_చివర(fs, flags, తోక_flags);
	ఒకవేళ (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_సమితి_options(fs->io, "cache=on");
	తిరిగి retval;
fallback:
#అయితే_ముగింపు /* HAVE_PTHREAD */
	తిరిగి read_bitmaps_range(fs, flags, 0, fs->సమూహం_desc_లెక్క - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	తిరిగి ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_ఖండం_bitmap(ext2_filsys fs)
{
	తిరిగి ext2fs_rw_bitmaps(fs, EXT2_FS_BITMAPS_ఖండం, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	తిరిగి write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_ఖండం_bitmap (ext2_filsys fs)
{
	తిరిగి write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	పూర్ణాంకం flags = 0;

	ఒకవేళ (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	ఒకవేళ (!fs->ఖండం_map)
		flags |= EXT2_FS_BITMAPS_ఖండం;
	ఒకవేళ (flags == 0)
		తిరిగి 0;
	తిరిగి ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	పూర్ణాంకం do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	పూర్ణాంకం do_ఖండం = fs->ఖండం_map && ext2fs_test_bb_dirty(fs);

	ఒకవేళ (!do_inode && !do_ఖండం)
		తిరిగి 0;

	తిరిగి write_bitmaps(fs, do_inode, do_ఖండం);
}
