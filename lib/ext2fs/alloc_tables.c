/*
 * alloc_tables.c --- Allocate tables for a newly initialized
 * filesystem.  Used by mke2fs when initializing a filesystem
 *
 * Copyright (C) 1996 Theodore Ts'o.
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
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
ساکن blk64_t flexbg_انحراف(ext2_filsys fs, dgrp_t گروہ, blk64_t آغاز_blk,
			     ext2fs_بلاک_bitmap bmap, صحیح rem_grp,
			     صحیح elem_سائز)
{
	صحیح		flexbg, flexbg_سائز, سائز;
	blk64_t		آخری_blk, پہلا_free = 0;
	dgrp_t	       	آخری_grp;

	flexbg_سائز = 1U << fs->super->s_log_groups_فی_flex;
	flexbg = گروہ / flexbg_سائز;
	سائز = rem_grp * elem_سائز;

	اگر (سائز > (صحیح) (fs->super->s_بلاکیں_فی_گروہ / 4))
		سائز = (صحیح) fs->super->s_بلاکیں_فی_گروہ / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	اگر (آغاز_blk && آغاز_blk < ext2fs_بلاکیں_گنتی(fs->super) &&
	    ext2fs_get_free_blocks2(fs, آغاز_blk, آغاز_blk + سائز, elem_سائز,
				    bmap, &پہلا_free) == 0)
		واپس پہلا_free;

	آغاز_blk = ext2fs_گروہ_پہلا_block2(fs, flexbg_سائز * flexbg);
	آخری_grp = گروہ | (flexbg_سائز - 1);
	اگر (آخری_grp > fs->گروہ_desc_گنتی-1)
		آخری_grp = fs->گروہ_desc_گنتی-1;
	آخری_blk = ext2fs_گروہ_آخری_block2(fs, آخری_grp);

	/* Find the first available block */
	اگر (ext2fs_get_free_blocks2(fs, آغاز_blk, آخری_blk, سائز,
				    bmap, &پہلا_free) == 0)
		واپس پہلا_free;

	اگر (ext2fs_get_free_blocks2(fs, آغاز_blk, آخری_blk, elem_سائز,
				   bmap, &پہلا_free) == 0)
		واپس پہلا_free;

	اگر (ext2fs_get_free_blocks2(fs, 0, آخری_blk, elem_سائز, bmap,
				    &پہلا_free) == 0)
		واپس پہلا_free;

	واپس پہلا_free;
}

errcode_t ext2fs_allocate_گروہ_جدول(ext2_filsys fs, dgrp_t گروہ,
				      ext2fs_بلاک_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		گروہ_blk, آغاز_blk, آخری_blk, new_blk;
	dgrp_t		آخری_grp = 0;
	صحیح		rem_grps = 0, flexbg_سائز = 0, جدول_انحراف = 0;

	گروہ_blk = ext2fs_گروہ_پہلا_block2(fs, گروہ);
	آخری_blk = ext2fs_گروہ_آخری_block2(fs, گروہ);

	اگر (!bmap)
		bmap = fs->بلاک_map;

	اگر (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_فی_flex) {
		flexbg_سائز = 1U << fs->super->s_log_groups_فی_flex;
		آخری_grp = گروہ | (flexbg_سائز - 1);
		اگر (آخری_grp > fs->گروہ_desc_گنتی-1)
			آخری_grp = fs->گروہ_desc_گنتی-1;
		rem_grps = آخری_grp - گروہ + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	اگر (fs->stride && !flexbg_سائز) {
		retval = ext2fs_get_free_blocks2(fs, گروہ_blk, آخری_blk,
						 1, bmap, &آغاز_blk);
		اگر (retval)
			واپس retval;
		آغاز_blk += fs->inode_بلاکیں_فی_گروہ;
		آغاز_blk += ((fs->stride * گروہ) %
			      (آخری_blk - آغاز_blk + 1));
		اگر (آغاز_blk >= آخری_blk)
			آغاز_blk = گروہ_blk;
	} ورنہ
		آغاز_blk = گروہ_blk;

	اگر (flexbg_سائز) {
		blk64_t پچھلا_بلاک = 0;

		جدول_انحراف = flexbg_سائز;
		اگر (گروہ % flexbg_سائز)
			پچھلا_بلاک = ext2fs_بلاک_bitmap_loc(fs, گروہ - 1) + 1;
		ورنہ اگر (آخری_grp == fs->گروہ_desc_گنتی-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			جدول_انحراف = آخری_grp & (flexbg_سائز - 1);
			اگر (جدول_انحراف == 0)
				جدول_انحراف = flexbg_سائز;
			ورنہ
				جدول_انحراف++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		آغاز_blk = flexbg_انحراف(fs, گروہ, پچھلا_بلاک, bmap,
					  rem_grps, 1);
		آخری_blk = ext2fs_گروہ_آخری_block2(fs, آخری_grp);
	}

	اگر (!ext2fs_بلاک_bitmap_loc(fs, گروہ)) {
		retval = ext2fs_get_free_blocks2(fs, آغاز_blk, آخری_blk,
						 1, bmap, &new_blk);
		اگر (retval == EXT2_ET_بلاک_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, گروہ_blk,
					آخری_blk, 1, bmap, &new_blk);
		اگر (retval)
			واپس retval;
		ext2fs_mark_بلاک_bitmap2(bmap, new_blk);
		ext2fs_بلاک_bitmap_loc_سیٹ(fs, گروہ, new_blk);
		اگر (flexbg_سائز) {
			dgrp_t gr = ext2fs_گروہ_کا_blk2(fs, new_blk);
			ext2fs_bg_free_بلاکیں_گنتی_سیٹ(fs, gr, ext2fs_bg_free_بلاکیں_گنتی(fs, gr) - 1);
			ext2fs_free_بلاکیں_گنتی_جوڑو(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلاک_UNINIT);
			ext2fs_گروہ_desc_csum_سیٹ(fs, gr);
		}
	}

	اگر (flexbg_سائز) {
		blk64_t پچھلا_بلاک = 0;
		اگر (گروہ % flexbg_سائز)
			پچھلا_بلاک = ext2fs_inode_bitmap_loc(fs, گروہ - 1) + 1;
		ورنہ
			پچھلا_بلاک = ext2fs_بلاک_bitmap_loc(fs, گروہ) +
				جدول_انحراف;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		آغاز_blk = flexbg_انحراف(fs, گروہ, پچھلا_بلاک, bmap,
					  rem_grps, 1);
		آخری_blk = ext2fs_گروہ_آخری_block2(fs, آخری_grp);
	}

	اگر (!ext2fs_inode_bitmap_loc(fs, گروہ)) {
		retval = ext2fs_get_free_blocks2(fs, آغاز_blk, آخری_blk,
						 1, bmap, &new_blk);
		اگر (retval == EXT2_ET_بلاک_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, گروہ_blk,
					 آخری_blk, 1, bmap, &new_blk);
		اگر (retval)
			واپس retval;
		ext2fs_mark_بلاک_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_سیٹ(fs, گروہ, new_blk);
		اگر (flexbg_سائز) {
			dgrp_t gr = ext2fs_گروہ_کا_blk2(fs, new_blk);
			ext2fs_bg_free_بلاکیں_گنتی_سیٹ(fs, gr, ext2fs_bg_free_بلاکیں_گنتی(fs, gr) - 1);
			ext2fs_free_بلاکیں_گنتی_جوڑو(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلاک_UNINIT);
			ext2fs_گروہ_desc_csum_سیٹ(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	اگر (flexbg_سائز) {
		blk64_t پچھلا_بلاک = 0;

		اگر (گروہ % flexbg_سائز)
			پچھلا_بلاک = ext2fs_inode_جدول_loc(fs, گروہ - 1) +
				fs->inode_بلاکیں_فی_گروہ;
		ورنہ
			پچھلا_بلاک = ext2fs_inode_bitmap_loc(fs, گروہ) +
				جدول_انحراف;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		گروہ_blk = flexbg_انحراف(fs, گروہ, پچھلا_بلاک, bmap,
					  rem_grps, fs->inode_بلاکیں_فی_گروہ);
		آخری_blk = ext2fs_گروہ_آخری_block2(fs, آخری_grp);
	}

	اگر (!ext2fs_inode_جدول_loc(fs, گروہ)) {
		retval = ext2fs_get_free_blocks2(fs, گروہ_blk, آخری_blk,
						fs->inode_بلاکیں_فی_گروہ,
						bmap, &new_blk);
		اگر (retval)
			واپس retval;

		ext2fs_mark_بلاک_bitmap_range2(bmap,
			new_blk, fs->inode_بلاکیں_فی_گروہ);
		اگر (flexbg_سائز) {
			blk64_t عدد, blk;
			عدد = fs->inode_بلاکیں_فی_گروہ;
			blk = new_blk;
			جب (عدد) {
				صحیح gr = ext2fs_گروہ_کا_blk2(fs, blk);
				آخری_blk = ext2fs_گروہ_آخری_block2(fs, gr);
				blk64_t n = عدد;

				اگر (blk + عدد > آخری_blk)
					n = آخری_blk - blk + 1;

				ext2fs_bg_free_بلاکیں_گنتی_سیٹ(fs, gr,
					ext2fs_bg_free_بلاکیں_گنتی(fs, gr) -
					n/EXT2_FS_CLUSTER_تناسب(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_بلاک_UNINIT);
				ext2fs_گروہ_desc_csum_سیٹ(fs, gr);
				ext2fs_free_بلاکیں_گنتی_جوڑو(fs->super, -n);
				blk += n;
				عدد -= n;
			}
		}
		ext2fs_inode_جدول_loc_سیٹ(fs, گروہ, new_blk);
	}
	ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);
	واپس 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	ساخت ext2fs_numeric_پیش_رفت_struct پیش_رفت;

	اگر (fs->پیش_رفت_ops && fs->پیش_رفت_ops->init)
		(fs->پیش_رفت_ops->init)(fs, &پیش_رفت, تہی,
					 fs->گروہ_desc_گنتی);

	برائے (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		اگر (fs->پیش_رفت_ops && fs->پیش_رفت_ops->update)
			(fs->پیش_رفت_ops->update)(fs, &پیش_رفت, i);
		retval = ext2fs_allocate_گروہ_جدول(fs, i, fs->بلاک_map);
		اگر (retval)
			واپس retval;
	}
	اگر (fs->پیش_رفت_ops && fs->پیش_رفت_ops->close)
		(fs->پیش_رفت_ops->close)(fs, &پیش_رفت, تہی);
	واپس 0;
}

