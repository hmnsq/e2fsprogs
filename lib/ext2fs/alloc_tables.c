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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
ساكن blk64_t flexbg_دننه_واټن(ext2_filsys fs, dgrp_t ډله, blk64_t پیل_blk,
			     ext2fs_بلاک_bitmap bmap, صحيح rem_grp,
			     صحيح elem_اندازه)
{
	صحيح		flexbg, flexbg_اندازه, اندازه;
	blk64_t		وروستی_blk, لومړی_free = 0;
	dgrp_t	       	وروستی_grp;

	flexbg_اندازه = 1U << fs->super->s_log_groups_فی_flex;
	flexbg = ډله / flexbg_اندازه;
	اندازه = rem_grp * elem_اندازه;

	إذا (اندازه > (صحيح) (fs->super->s_بلاکونه_فی_ډله / 4))
		اندازه = (صحيح) fs->super->s_بلاکونه_فی_ډله / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	إذا (پیل_blk && پیل_blk < ext2fs_بلاکونه_شمېر(fs->super) &&
	    ext2fs_get_free_blocks2(fs, پیل_blk, پیل_blk + اندازه, elem_اندازه,
				    bmap, &لومړی_free) == 0)
		أرجع لومړی_free;

	پیل_blk = ext2fs_ډله_لومړی_block2(fs, flexbg_اندازه * flexbg);
	وروستی_grp = ډله | (flexbg_اندازه - 1);
	إذا (وروستی_grp > fs->ډله_desc_شمېر-1)
		وروستی_grp = fs->ډله_desc_شمېر-1;
	وروستی_blk = ext2fs_ډله_وروستی_block2(fs, وروستی_grp);

	/* Find the first available block */
	إذا (ext2fs_get_free_blocks2(fs, پیل_blk, وروستی_blk, اندازه,
				    bmap, &لومړی_free) == 0)
		أرجع لومړی_free;

	إذا (ext2fs_get_free_blocks2(fs, پیل_blk, وروستی_blk, elem_اندازه,
				   bmap, &لومړی_free) == 0)
		أرجع لومړی_free;

	إذا (ext2fs_get_free_blocks2(fs, 0, وروستی_blk, elem_اندازه, bmap,
				    &لومړی_free) == 0)
		أرجع لومړی_free;

	أرجع لومړی_free;
}

errcode_t ext2fs_allocate_ډله_جدول(ext2_filsys fs, dgrp_t ډله,
				      ext2fs_بلاک_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		ډله_blk, پیل_blk, وروستی_blk, new_blk;
	dgrp_t		وروستی_grp = 0;
	صحيح		rem_grps = 0, flexbg_اندازه = 0, جدول_دننه_واټن = 0;

	ډله_blk = ext2fs_ډله_لومړی_block2(fs, ډله);
	وروستی_blk = ext2fs_ډله_وروستی_block2(fs, ډله);

	إذا (!bmap)
		bmap = fs->بلاک_map;

	إذا (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_فی_flex) {
		flexbg_اندازه = 1U << fs->super->s_log_groups_فی_flex;
		وروستی_grp = ډله | (flexbg_اندازه - 1);
		إذا (وروستی_grp > fs->ډله_desc_شمېر-1)
			وروستی_grp = fs->ډله_desc_شمېر-1;
		rem_grps = وروستی_grp - ډله + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	إذا (fs->stride && !flexbg_اندازه) {
		retval = ext2fs_get_free_blocks2(fs, ډله_blk, وروستی_blk,
						 1, bmap, &پیل_blk);
		إذا (retval)
			أرجع retval;
		پیل_blk += fs->inode_بلاکونه_فی_ډله;
		پیل_blk += ((fs->stride * ډله) %
			      (وروستی_blk - پیل_blk + 1));
		إذا (پیل_blk >= وروستی_blk)
			پیل_blk = ډله_blk;
	} وإلا
		پیل_blk = ډله_blk;

	إذا (flexbg_اندازه) {
		blk64_t prev_بلاک = 0;

		جدول_دننه_واټن = flexbg_اندازه;
		إذا (ډله % flexbg_اندازه)
			prev_بلاک = ext2fs_بلاک_bitmap_loc(fs, ډله - 1) + 1;
		وإلا إذا (وروستی_grp == fs->ډله_desc_شمېر-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			جدول_دننه_واټن = وروستی_grp & (flexbg_اندازه - 1);
			إذا (جدول_دننه_واټن == 0)
				جدول_دننه_واټن = flexbg_اندازه;
			وإلا
				جدول_دننه_واټن++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		پیل_blk = flexbg_دننه_واټن(fs, ډله, prev_بلاک, bmap,
					  rem_grps, 1);
		وروستی_blk = ext2fs_ډله_وروستی_block2(fs, وروستی_grp);
	}

	إذا (!ext2fs_بلاک_bitmap_loc(fs, ډله)) {
		retval = ext2fs_get_free_blocks2(fs, پیل_blk, وروستی_blk,
						 1, bmap, &new_blk);
		إذا (retval == EXT2_ET_بلاک_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, ډله_blk,
					وروستی_blk, 1, bmap, &new_blk);
		إذا (retval)
			أرجع retval;
		ext2fs_mark_بلاک_bitmap2(bmap, new_blk);
		ext2fs_بلاک_bitmap_loc_سېټ(fs, ډله, new_blk);
		إذا (flexbg_اندازه) {
			dgrp_t gr = ext2fs_ډله_د_blk2(fs, new_blk);
			ext2fs_bg_free_بلاکونه_شمېر_سېټ(fs, gr, ext2fs_bg_free_بلاکونه_شمېر(fs, gr) - 1);
			ext2fs_free_بلاکونه_شمېر_جمع_کړه(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلاک_UNINIT);
			ext2fs_ډله_desc_csum_سېټ(fs, gr);
		}
	}

	إذا (flexbg_اندازه) {
		blk64_t prev_بلاک = 0;
		إذا (ډله % flexbg_اندازه)
			prev_بلاک = ext2fs_inode_bitmap_loc(fs, ډله - 1) + 1;
		وإلا
			prev_بلاک = ext2fs_بلاک_bitmap_loc(fs, ډله) +
				جدول_دننه_واټن;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		پیل_blk = flexbg_دننه_واټن(fs, ډله, prev_بلاک, bmap,
					  rem_grps, 1);
		وروستی_blk = ext2fs_ډله_وروستی_block2(fs, وروستی_grp);
	}

	إذا (!ext2fs_inode_bitmap_loc(fs, ډله)) {
		retval = ext2fs_get_free_blocks2(fs, پیل_blk, وروستی_blk,
						 1, bmap, &new_blk);
		إذا (retval == EXT2_ET_بلاک_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, ډله_blk,
					 وروستی_blk, 1, bmap, &new_blk);
		إذا (retval)
			أرجع retval;
		ext2fs_mark_بلاک_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_سېټ(fs, ډله, new_blk);
		إذا (flexbg_اندازه) {
			dgrp_t gr = ext2fs_ډله_د_blk2(fs, new_blk);
			ext2fs_bg_free_بلاکونه_شمېر_سېټ(fs, gr, ext2fs_bg_free_بلاکونه_شمېر(fs, gr) - 1);
			ext2fs_free_بلاکونه_شمېر_جمع_کړه(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلاک_UNINIT);
			ext2fs_ډله_desc_csum_سېټ(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	إذا (flexbg_اندازه) {
		blk64_t prev_بلاک = 0;

		إذا (ډله % flexbg_اندازه)
			prev_بلاک = ext2fs_inode_جدول_loc(fs, ډله - 1) +
				fs->inode_بلاکونه_فی_ډله;
		وإلا
			prev_بلاک = ext2fs_inode_bitmap_loc(fs, ډله) +
				جدول_دننه_واټن;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		ډله_blk = flexbg_دننه_واټن(fs, ډله, prev_بلاک, bmap,
					  rem_grps, fs->inode_بلاکونه_فی_ډله);
		وروستی_blk = ext2fs_ډله_وروستی_block2(fs, وروستی_grp);
	}

	إذا (!ext2fs_inode_جدول_loc(fs, ډله)) {
		retval = ext2fs_get_free_blocks2(fs, ډله_blk, وروستی_blk,
						fs->inode_بلاکونه_فی_ډله,
						bmap, &new_blk);
		إذا (retval)
			أرجع retval;

		ext2fs_mark_بلاک_bitmap_range2(bmap,
			new_blk, fs->inode_بلاکونه_فی_ډله);
		إذا (flexbg_اندازه) {
			blk64_t عدد, blk;
			عدد = fs->inode_بلاکونه_فی_ډله;
			blk = new_blk;
			طالما (عدد) {
				صحيح gr = ext2fs_ډله_د_blk2(fs, blk);
				وروستی_blk = ext2fs_ډله_وروستی_block2(fs, gr);
				blk64_t n = عدد;

				إذا (blk + عدد > وروستی_blk)
					n = وروستی_blk - blk + 1;

				ext2fs_bg_free_بلاکونه_شمېر_سېټ(fs, gr,
					ext2fs_bg_free_بلاکونه_شمېر(fs, gr) -
					n/EXT2_FS_CLUSTER_نسبت(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_بلاک_UNINIT);
				ext2fs_ډله_desc_csum_سېټ(fs, gr);
				ext2fs_free_بلاکونه_شمېر_جمع_کړه(fs->super, -n);
				blk += n;
				عدد -= n;
			}
		}
		ext2fs_inode_جدول_loc_سېټ(fs, ډله, new_blk);
	}
	ext2fs_ډله_desc_csum_سېټ(fs, ډله);
	أرجع 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	بنية ext2fs_numeric_پرمختګ_struct پرمختګ;

	إذا (fs->پرمختګ_ops && fs->پرمختګ_ops->init)
		(fs->پرمختګ_ops->init)(fs, &پرمختګ, عدم,
					 fs->ډله_desc_شمېر);

	لكل (i = 0; i < fs->ډله_desc_شمېر; i++) {
		إذا (fs->پرمختګ_ops && fs->پرمختګ_ops->update)
			(fs->پرمختګ_ops->update)(fs, &پرمختګ, i);
		retval = ext2fs_allocate_ډله_جدول(fs, i, fs->بلاک_map);
		إذا (retval)
			أرجع retval;
	}
	إذا (fs->پرمختګ_ops && fs->پرمختګ_ops->close)
		(fs->پرمختګ_ops->close)(fs, &پرمختګ, عدم);
	أرجع 0;
}

