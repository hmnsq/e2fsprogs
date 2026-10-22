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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि_परिभाषितम् HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_PTHREAD_H
#अन्तर्भाव <pthread.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "e2image.h"

#यदि_परिभाषितम् HAVE_PTHREAD
प्रकारनिर्देश pthread_mutex_t mutex_t;

स्थिर शून्य unix_pthread_mutex_तालकम्(mutex_t *mutex)
{
	यदि (mutex)
		pthread_mutex_तालकम्(mutex);
}
स्थिर शून्य unix_pthread_mutex_unlock(mutex_t *mutex)
{
	यदि (mutex)
		pthread_mutex_unlock(mutex);
}
#अन्यथा
प्रकारनिर्देश पूर्णाङ्क mutex_t;
#परिभाषय unix_pthread_mutex_तालकम्(mutex_t) कुरु {} यावत् (0)
#परिभाषय unix_pthread_mutex_unlock(mutex_t) कुरु {} यावत् (0)
#यदि_अन्तः

स्थिर errcode_t write_bitmaps(ext2_filsys fs, पूर्णाङ्क do_inode, पूर्णाङ्क do_खण्डम्)
{
	dgrp_t 		i;
	अचिह्नित पूर्णाङ्क	j;
	पूर्णाङ्क		खण्डम्_nbytes, inode_nbytes;
	अचिह्नित पूर्णाङ्क	nbits;
	errcode_t	retval;
	वर्ण		*खण्डम्_buf = रिक्त, *inode_buf = रिक्त;
	पूर्णाङ्क		csum_flag;
	blk64_t		blk;
	blk64_t		blk_itr = EXT2FS_B2C(fs, fs->super->s_first_दत्त_खण्डम्);
	ext2_ino_t	ino_itr = 1;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!(fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	csum_flag = ext2fs_has_गणः_desc_csum(fs);

	inode_nbytes = खण्डम्_nbytes = 0;
	यदि (do_खण्डम्) {
		खण्डम्_nbytes = EXT2_CLUSTERS_प्रत्येकम्_गणः(fs->super) / 8;
		retval = io_channel_alloc_buf(fs->io, 0, &खण्डम्_buf);
		यदि (retval)
			गच्छ errout;
		स्मृतिं_पूरय(खण्डम्_buf, 0xff, fs->blocksize);
	}
	यदि (do_inode) {
		inode_nbytes = (size_t)
			((EXT2_INODES_प्रत्येकम्_गणः(fs->super)+7) / 8);
		retval = io_channel_alloc_buf(fs->io, 0, &inode_buf);
		यदि (retval)
			गच्छ errout;
		स्मृतिं_पूरय(inode_buf, 0xff, fs->blocksize);
	}

	प्रति (i = 0; i < fs->गणः_desc_गणनम्; i++) {
		यदि (!do_खण्डम्)
			गच्छ skip_खण्डम्_bitmap;

		यदि (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_खण्डम्_UNINIT)
		    )
			गच्छ skip_this_खण्डम्_bitmap;

		retval = ext2fs_get_खण्डम्_bitmap_range2(fs->खण्डम्_map,
				blk_itr, खण्डम्_nbytes << 3, खण्डम्_buf);
		यदि (retval)
			गच्छ errout;

		यदि (i == fs->गणः_desc_गणनम् - 1) {
			/* Force bitmap padding for the last group */
			nbits = EXT2_FS_अङ्कः_B2_C(fs,
				((ext2fs_खण्डानि_गणनम्(fs->super)
				  - (__u64) fs->super->s_first_दत्त_खण्डम्)
				 % (__u64) EXT2_खण्डानि_प्रत्येकम्_गणः(fs->super)));
			यदि (nbits)
				प्रति (j = nbits; j < fs->blocksize * 8; j++)
					ext2fs_समुच्चयः_bit(j, खण्डम्_buf);
		}

		retval = ext2fs_खण्डम्_bitmap_csum_समुच्चयः(fs, i, खण्डम्_buf,
						      खण्डम्_nbytes);
		यदि (retval)
			प्रत्यागम retval;
		ext2fs_गणः_desc_csum_समुच्चयः(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_खण्डम्_bitmap_loc(fs, i);
		यदि (blk && blk < ext2fs_खण्डानि_गणनम्(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
							खण्डम्_buf);
			यदि (retval) {
				retval = EXT2_ET_खण्डम्_BITMAP_WRITE;
				गच्छ errout;
			}
		}
	skip_this_खण्डम्_bitmap:
		blk_itr += खण्डम्_nbytes << 3;
	skip_खण्डम्_bitmap:

		यदि (!do_inode)
			अनुवर्त;

		यदि (csum_flag && ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT)
		    )
			गच्छ skip_this_inode_bitmap;

		retval = ext2fs_get_inode_bitmap_range2(fs->inode_map,
				ino_itr, inode_nbytes << 3, inode_buf);
		यदि (retval)
			गच्छ errout;

		retval = ext2fs_inode_bitmap_csum_समुच्चयः(fs, i, inode_buf,
						      inode_nbytes);
		यदि (retval)
			गच्छ errout;
		ext2fs_गणः_desc_csum_समुच्चयः(fs, i);
		fs->flags |= EXT2_FLAG_DIRTY;

		blk = ext2fs_inode_bitmap_loc(fs, i);
		यदि (blk && blk < ext2fs_खण्डानि_गणनम्(fs->super)) {
			retval = io_channel_write_blk64(fs->io, blk, 1,
						      inode_buf);
			यदि (retval) {
				retval = EXT2_ET_INODE_BITMAP_WRITE;
				गच्छ errout;
			}
		}
	skip_this_inode_bitmap:
		ino_itr += inode_nbytes << 3;

	}
	यदि (do_खण्डम्) {
		fs->flags &= ~EXT2_FLAG_BB_DIRTY;
		ext2fs_free_mem(&खण्डम्_buf);
	}
	यदि (do_inode) {
		fs->flags &= ~EXT2_FLAG_IB_DIRTY;
		ext2fs_free_mem(&inode_buf);
	}
	प्रत्यागम 0;
errout:
	यदि (inode_buf)
		ext2fs_free_mem(&inode_buf);
	यदि (खण्डम्_buf)
		ext2fs_free_mem(&खण्डम्_buf);
	प्रत्यागम retval;
}

स्थिर errcode_t mark_uninit_bg_गणः_खण्डानि(ext2_filsys fs)
{
	dgrp_t			i;
	blk64_t			blk;
	ext2fs_खण्डम्_bitmap	bmap = fs->खण्डम्_map;

	प्रति (i = 0; i < fs->गणः_desc_गणनम्; i++) {
		यदि (!ext2fs_bg_flags_test(fs, i, EXT2_BG_खण्डम्_UNINIT))
			अनुवर्त;

		ext2fs_reserve_super_and_bgd(fs, i, bmap);

		/*
		 * Mark the blocks used for the inode table
		 */
		blk = ext2fs_inode_सारणी_loc(fs, i);
		यदि (blk)
			ext2fs_mark_खण्डम्_bitmap_range2(bmap, blk,
						fs->inode_खण्डानि_प्रत्येकम्_गणः);

		/*
		 * Mark block used for the block bitmap
		 */
		blk = ext2fs_खण्डम्_bitmap_loc(fs, i);
		यदि (blk && blk < ext2fs_खण्डानि_गणनम्(fs->super))
			ext2fs_mark_खण्डम्_bitmap2(bmap, blk);

		/*
		 * Mark block used for the inode bitmap
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		यदि (blk && blk < ext2fs_खण्डानि_गणनम्(fs->super))
			ext2fs_mark_खण्डम्_bitmap2(bmap, blk);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क bitmap_पुच्छ_verify(अचिह्नित वर्ण *bitmap, पूर्णाङ्क first, पूर्णाङ्क चरमः)
{
	पूर्णाङ्क i;

	प्रति (i = first; i <= चरमः; i++)
		यदि (bitmap[i] != 0xff)
			प्रत्यागम 0;
	प्रत्यागम 1;
}

स्थिर errcode_t read_bitmaps_range_prepare(ext2_filsys fs, पूर्णाङ्क flags)
{
	errcode_t retval;
	पूर्णाङ्क खण्डम्_nbytes = EXT2_CLUSTERS_प्रत्येकम्_गणः(fs->super) / 8;
	पूर्णाङ्क inode_nbytes = EXT2_INODES_प्रत्येकम्_गणः(fs->super) / 8;
	वर्ण *buf;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि ((खण्डम्_nbytes > (पूर्णाङ्क) fs->blocksize) ||
	    (inode_nbytes > (पूर्णाङ्क) fs->blocksize))
		प्रत्यागम EXT2_ET_CORRUPT_SUPERBLOCK;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(fs->device_name) + 80, &buf);
	यदि (retval)
		प्रत्यागम retval;

	यदि (flags & EXT2_FS_BITMAPS_खण्डम्) {
		यदि (fs->खण्डम्_map)
			ext2fs_free_खण्डम्_bitmap(fs->खण्डम्_map);
		वाक्यं_प्रतिलिख(buf, "block bitmap for ");
		वाक्यं_योजय(buf, fs->device_name);
		retval = ext2fs_allocate_खण्डम्_bitmap(fs, buf, &fs->खण्डम्_map);
		यदि (retval)
			गच्छ cleanup;
	}

	यदि (flags & EXT2FS_BITMAPS_INODE) {
		यदि (fs->inode_map)
			ext2fs_free_inode_bitmap(fs->inode_map);
		वाक्यं_प्रतिलिख(buf, "inode bitmap for ");
		वाक्यं_योजय(buf, fs->device_name);
		retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
		यदि (retval)
			गच्छ cleanup;
	}
	ext2fs_free_mem(&buf);
	प्रत्यागम retval;

cleanup:
	यदि (flags & EXT2_FS_BITMAPS_खण्डम्) {
		ext2fs_free_खण्डम्_bitmap(fs->खण्डम्_map);
		fs->खण्डम्_map = 0;
	}
	यदि (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

स्थिर errcode_t read_bitmaps_range_start(ext2_filsys fs, पूर्णाङ्क flags,
					  dgrp_t start, dgrp_t अन्तः,
					  mutex_t *mutex,
					  पूर्णाङ्क *पुच्छ_flags)
{
	dgrp_t i;
	वर्ण *खण्डम्_bitmap = 0, *inode_bitmap = 0;
	errcode_t retval = 0;
	पूर्णाङ्क खण्डम्_nbytes = EXT2_CLUSTERS_प्रत्येकम्_गणः(fs->super) / 8;
	पूर्णाङ्क inode_nbytes = EXT2_INODES_प्रत्येकम्_गणः(fs->super) / 8;
	पूर्णाङ्क csum_flag;
	अचिह्नित पूर्णाङ्क	cnt;
	blk64_t	blk;
	blk64_t	blk_itr = EXT2FS_B2C(fs, fs->super->s_first_दत्त_खण्डम्);
	blk64_t   blk_cnt;
	ext2_ino_t ino_itr = 1;
	ext2_ino_t ino_cnt;

	csum_flag = ext2fs_has_गणः_desc_csum(fs);

	यदि (flags & EXT2_FS_BITMAPS_खण्डम्) {
		retval = io_channel_alloc_buf(fs->io, 0, &खण्डम्_bitmap);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा {
		खण्डम्_nbytes = 0;
	}

	यदि (flags & EXT2FS_BITMAPS_INODE) {
		retval = io_channel_alloc_buf(fs->io, 0, &inode_bitmap);
		यदि (retval)
			गच्छ cleanup;
	} अन्यथा {
		inode_nbytes = 0;
	}

	/* io should be null */
	यदि (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		blk = (ext2fs_le32_गन्तव्यम्_cpu(fs->image_शीर्षकम्->विस्थापनम्_inodemap) / fs->blocksize);
		ino_cnt = fs->super->s_inodes_गणनम्;
		यावत् (inode_bitmap && ino_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, inode_bitmap);
			यदि (retval)
				गच्छ cleanup;
			cnt = fs->blocksize << 3;
			यदि (cnt > ino_cnt)
				cnt = ino_cnt;
			retval = ext2fs_समुच्चयः_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			यदि (retval)
				गच्छ cleanup;
			ino_itr += cnt;
			ino_cnt -= cnt;
		}
		blk = (ext2fs_le32_गन्तव्यम्_cpu(fs->image_शीर्षकम्->विस्थापनम्_blockmap) /
		       fs->blocksize);
		blk_cnt = EXT2_GROUPS_गन्तव्यम्_CLUSTERS(fs->super,
						  fs->गणः_desc_गणनम्);
		यावत् (खण्डम्_bitmap && blk_cnt > 0) {
			retval = io_channel_read_blk64(fs->image_io, blk++,
						     1, खण्डम्_bitmap);
			यदि (retval)
				गच्छ cleanup;
			cnt = fs->blocksize << 3;
			यदि (cnt > blk_cnt)
				cnt = blk_cnt;
			retval = ext2fs_समुच्चयः_खण्डम्_bitmap_range2(fs->खण्डम्_map,
				       blk_itr, cnt, खण्डम्_bitmap);
			यदि (retval)
				गच्छ cleanup;
			blk_itr += cnt;
			blk_cnt -= cnt;
		}
		गच्छ cleanup;
	}

	blk_itr += ((blk64_t)start * (खण्डम्_nbytes << 3));
	ino_itr += ((blk64_t)start * (inode_nbytes << 3));
	प्रति (i = start; i <= अन्तः; i++) {
		यदि (खण्डम्_bitmap) {
			blk = ext2fs_खण्डम्_bitmap_loc(fs, i);
			यदि ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_खण्डम्_UNINIT) &&
			     ext2fs_गणः_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_खण्डानि_गणनम्(fs->super)))
				blk = 0;
			यदि (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, खण्डम्_bitmap);
				यदि (retval) {
					retval = EXT2_ET_खण्डम्_BITMAP_READ;
					गच्छ cleanup;
				}
				/* verify block bitmap checksum */
				यदि (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_खण्डम्_bitmap_csum_verify(fs, i,
						खण्डम्_bitmap, खण्डम्_nbytes)) {
					retval =
					EXT2_ET_खण्डम्_BITMAP_CSUM_INVALID;
					गच्छ cleanup;
				}
				यदि (!bitmap_पुच्छ_verify((अचिह्नित वर्ण *) खण्डम्_bitmap,
							खण्डम्_nbytes, fs->blocksize - 1))
					*पुच्छ_flags |= EXT2_FLAG_BBITMAP_पुच्छ_PROBLEM;
			} अन्यथा
				स्मृतिं_पूरय(खण्डम्_bitmap, 0, खण्डम्_nbytes);
			cnt = खण्डम्_nbytes << 3;
			unix_pthread_mutex_तालकम्(mutex);
			retval = ext2fs_समुच्चयः_खण्डम्_bitmap_range2(fs->खण्डम्_map,
					       blk_itr, cnt, खण्डम्_bitmap);
			unix_pthread_mutex_unlock(mutex);
			यदि (retval)
				गच्छ cleanup;
			blk_itr += खण्डम्_nbytes << 3;
		}
		यदि (inode_bitmap) {
			blk = ext2fs_inode_bitmap_loc(fs, i);
			यदि ((csum_flag &&
			     ext2fs_bg_flags_test(fs, i, EXT2_BG_INODE_UNINIT) &&
			     ext2fs_गणः_desc_csum_verify(fs, i)) ||
			    (blk >= ext2fs_खण्डानि_गणनम्(fs->super)))
				blk = 0;
			यदि (blk) {
				retval = io_channel_read_blk64(fs->io, blk,
							       1, inode_bitmap);
				यदि (retval) {
					retval = EXT2_ET_INODE_BITMAP_READ;
					गच्छ cleanup;
				}

				/* verify inode bitmap checksum */
				यदि (!(fs->flags &
				      EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
				    !ext2fs_inode_bitmap_csum_verify(fs, i,
						inode_bitmap, inode_nbytes)) {
					retval =
					EXT2_ET_INODE_BITMAP_CSUM_INVALID;
					गच्छ cleanup;
				}
				यदि (!bitmap_पुच्छ_verify((अचिह्नित वर्ण *) inode_bitmap,
							inode_nbytes, fs->blocksize - 1))
					*पुच्छ_flags |= EXT2_FLAG_IBITMAP_पुच्छ_PROBLEM;
			} अन्यथा
				स्मृतिं_पूरय(inode_bitmap, 0, inode_nbytes);
			cnt = inode_nbytes << 3;
			unix_pthread_mutex_तालकम्(mutex);
			retval = ext2fs_समुच्चयः_inode_bitmap_range2(fs->inode_map,
					       ino_itr, cnt, inode_bitmap);
			unix_pthread_mutex_unlock(mutex);
			यदि (retval)
				गच्छ cleanup;
			ino_itr += inode_nbytes << 3;
		}
	}

cleanup:
	यदि (inode_bitmap)
		ext2fs_free_mem(&inode_bitmap);
	यदि (खण्डम्_bitmap)
		ext2fs_free_mem(&खण्डम्_bitmap);
	प्रत्यागम retval;
}

स्थिर errcode_t read_bitmaps_range_अन्तः(ext2_filsys fs, पूर्णाङ्क flags,
					पूर्णाङ्क पुच्छ_flags)
{
	errcode_t retval;

	/* Mark group blocks for any BLOCK_UNINIT groups */
	यदि (flags & EXT2_FS_BITMAPS_खण्डम्) {
		retval = mark_uninit_bg_गणः_खण्डानि(fs);
		यदि (retval)
			प्रत्यागम retval;
		fs->flags &= ~EXT2_FLAG_BBITMAP_पुच्छ_PROBLEM;
	}
	यदि (flags & EXT2FS_BITMAPS_INODE)
		fs->flags &= ~EXT2_FLAG_IBITMAP_पुच्छ_PROBLEM;
	fs->flags |= पुच्छ_flags;

	प्रत्यागम 0;
}

स्थिर शून्य read_bitmaps_cleanup_on_error(ext2_filsys fs, पूर्णाङ्क flags)
{
	यदि (flags & EXT2_FS_BITMAPS_खण्डम्) {
		ext2fs_free_खण्डम्_bitmap(fs->खण्डम्_map);
		fs->खण्डम्_map = 0;
	}
	यदि (flags & EXT2FS_BITMAPS_INODE) {
		ext2fs_free_inode_bitmap(fs->inode_map);
		fs->inode_map = 0;
	}
}

स्थिर errcode_t read_bitmaps_range(ext2_filsys fs, पूर्णाङ्क flags,
				    dgrp_t start, dgrp_t अन्तः)
{
	errcode_t retval;
	पूर्णाङ्क पुच्छ_flags = 0;

	retval = read_bitmaps_range_prepare(fs, flags);
	यदि (retval)
		प्रत्यागम retval;

	retval = read_bitmaps_range_start(fs, flags, start, अन्तः,
					  रिक्त, &पुच्छ_flags);
	यदि (retval == 0)
		retval = read_bitmaps_range_अन्तः(fs, flags, पुच्छ_flags);
	यदि (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	प्रत्यागम retval;
}

#यदि_परिभाषितम् HAVE_PTHREAD
संरचना read_bitmaps_thread_info {
	ext2_filsys	rbt_fs;
	पूर्णाङ्क		rbt_flags;
	dgrp_t		rbt_grp_start;
	dgrp_t		rbt_grp_अन्तः;
	errcode_t	rbt_retval;
	pthread_mutex_t *rbt_mutex;
	पूर्णाङ्क		rbt_पुच्छ_flags;
};

स्थिर शून्य *read_bitmaps_thread(शून्य *दत्त)
{
	संरचना read_bitmaps_thread_info *rbt = दत्त;

	rbt->rbt_retval = read_bitmaps_range_start(rbt->rbt_fs, rbt->rbt_flags,
				rbt->rbt_grp_start, rbt->rbt_grp_अन्तः,
				rbt->rbt_mutex, &rbt->rbt_पुच्छ_flags);
	प्रत्यागम रिक्त;
}
#यदि_अन्तः

errcode_t ext2fs_rw_bitmaps(ext2_filsys fs, पूर्णाङ्क flags, पूर्णाङ्क अङ्कः_तन्तवः)
{
#यदि_परिभाषितम् HAVE_PTHREAD
	pthread_attr_t	attr;
	pthread_t *thread_ids = रिक्त;
	संरचना read_bitmaps_thread_info *thread_infos = रिक्त;
	pthread_mutex_t rbt_mutex = PTHREAD_MUTEX_INITIALIZER;
	errcode_t retval;
	errcode_t rc;
	अचिह्नित flexbg_आकार = 1U << fs->super->s_log_groups_प्रत्येकम्_flex;
	dgrp_t मध्यमानम्_गणः;
	पूर्णाङ्क i, पुच्छ_flags = 0;
#यदि_अन्तः

	यदि (flags & ~EXT2FS_BITMAPS_VALID_FLAGS)
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	यदि (flags & EXT2FS_BITMAPS_WRITE)
		प्रत्यागम write_bitmaps(fs, flags & EXT2FS_BITMAPS_INODE,
				     flags & EXT2_FS_BITMAPS_खण्डम्);

#यदि_परिभाषितम् HAVE_PTHREAD
	यदि (((fs->io->flags & CHANNEL_FLAGS_तन्तवः) == 0) ||
	    (अङ्कः_तन्तवः == 1) || (fs->flags & EXT2_FLAG_IMAGE_FILE))
		गच्छ fallback;

#यदि defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)
	यदि (अङ्कः_तन्तवः < 0)
		अङ्कः_तन्तवः = sysconf(_SC_NPROCESSORS_CONF);
#यदि_अन्तः
	/*
	 * Guess for now; eventually we should probably define
	 * ext2fs_get_num_cpus() and teach it how to get this info on
	 * MacOS, FreeBSD, etc.
	 * ref: https://stackoverflow.com/questions/150355
	 */
	यदि (अङ्कः_तन्तवः <= 0)
		अङ्कः_तन्तवः = 4;

	यदि ((अचिह्नित) अङ्कः_तन्तवः > fs->गणः_desc_गणनम्)
		अङ्कः_तन्तवः = fs->गणः_desc_गणनम्;
	मध्यमानम्_गणः = fs->गणः_desc_गणनम् / अङ्कः_तन्तवः;
	यदि (ext2fs_has_feature_flex_bg(fs->super)) {
		मध्यमानम्_गणः = (मध्यमानम्_गणः / flexbg_आकार) * flexbg_आकार;
	}
	यदि ((अङ्कः_तन्तवः <= 1) || (मध्यमानम्_गणः == 0))
		गच्छ fallback;

	io_channel_समुच्चयः_options(fs->io, "cache=off");
	retval = pthread_attr_init(&attr);
	यदि (retval)
		प्रत्यागम retval;

	thread_ids = शून्यस्मृतिं_देहि(परिमाण(pthread_t), अङ्कः_तन्तवः);
	यदि (!thread_ids)
		प्रत्यागम ENOMEM;

	thread_infos = शून्यस्मृतिं_देहि(परिमाण(संरचना read_bitmaps_thread_info),
				अङ्कः_तन्तवः);
	यदि (!thread_infos)
		गच्छ बहिर्गमनम्;

	retval = read_bitmaps_range_prepare(fs, flags);
	यदि (retval)
		गच्छ बहिर्गमनम्;

//	fprintf(stdout, "Multiple threads triggered to read bitmaps\n");
	प्रति (i = 0; i < अङ्कः_तन्तवः; i++) {
		thread_infos[i].rbt_fs = fs;
		thread_infos[i].rbt_flags = flags;
		thread_infos[i].rbt_mutex = &rbt_mutex;
		thread_infos[i].rbt_पुच्छ_flags = 0;
		यदि (i == 0)
			thread_infos[i].rbt_grp_start = 0;
		अन्यथा
			thread_infos[i].rbt_grp_start = मध्यमानम्_गणः * i + 1;

		यदि (i == अङ्कः_तन्तवः - 1)
			thread_infos[i].rbt_grp_अन्तः = fs->गणः_desc_गणनम् - 1;
		अन्यथा
			thread_infos[i].rbt_grp_अन्तः = मध्यमानम्_गणः * (i + 1);
		retval = pthread_create(&thread_ids[i], &attr,
					&read_bitmaps_thread, &thread_infos[i]);
		यदि (retval)
			भङ्ग;
	}
	प्रति (i = 0; i < अङ्कः_तन्तवः; i++) {
		यदि (!thread_ids[i])
			भङ्ग;
		rc = pthread_join(thread_ids[i], रिक्त);
		यदि (rc && !retval)
			retval = rc;
		rc = thread_infos[i].rbt_retval;
		यदि (rc && !retval)
			retval = rc;
		पुच्छ_flags |= thread_infos[i].rbt_पुच्छ_flags;
	}
बहिर्गमनम्:
	rc = pthread_attr_destroy(&attr);
	यदि (rc && !retval)
		retval = rc;
	मुक्त(thread_infos);
	मुक्त(thread_ids);

	यदि (retval == 0)
		retval = read_bitmaps_range_अन्तः(fs, flags, पुच्छ_flags);
	यदि (retval)
		read_bitmaps_cleanup_on_error(fs, flags);
	/* XXX should save and restore cache setting */
	io_channel_समुच्चयः_options(fs->io, "cache=on");
	प्रत्यागम retval;
fallback:
#यदि_अन्तः /* HAVE_PTHREAD */
	प्रत्यागम read_bitmaps_range(fs, flags, 0, fs->गणः_desc_गणनम् - 1);
}

errcode_t ext2fs_read_inode_bitmap(ext2_filsys fs)
{
	प्रत्यागम ext2fs_rw_bitmaps(fs, EXT2FS_BITMAPS_INODE, -1);
}

errcode_t ext2fs_read_खण्डम्_bitmap(ext2_filsys fs)
{
	प्रत्यागम ext2fs_rw_bitmaps(fs, EXT2_FS_BITMAPS_खण्डम्, -1);
}

errcode_t ext2fs_write_inode_bitmap(ext2_filsys fs)
{
	प्रत्यागम write_bitmaps(fs, 1, 0);
}

errcode_t ext2fs_write_खण्डम्_bitmap (ext2_filsys fs)
{
	प्रत्यागम write_bitmaps(fs, 0, 1);
}

errcode_t ext2fs_read_bitmaps(ext2_filsys fs)
{
	पूर्णाङ्क flags = 0;

	यदि (!fs->inode_map)
		flags |= EXT2FS_BITMAPS_INODE;
	यदि (!fs->खण्डम्_map)
		flags |= EXT2_FS_BITMAPS_खण्डम्;
	यदि (flags == 0)
		प्रत्यागम 0;
	प्रत्यागम ext2fs_rw_bitmaps(fs, flags, -1);
}

errcode_t ext2fs_write_bitmaps(ext2_filsys fs)
{
	पूर्णाङ्क do_inode = fs->inode_map && ext2fs_test_ib_dirty(fs);
	पूर्णाङ्क do_खण्डम् = fs->खण्डम्_map && ext2fs_test_bb_dirty(fs);

	यदि (!do_inode && !do_खण्डम्)
		प्रत्यागम 0;

	प्रत्यागम write_bitmaps(fs, do_inode, do_खण्डम्);
}
