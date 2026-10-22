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
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
ساكن blk64_t flexbg_جابه_جایی(ext2_filsys fs, dgrp_t گروه, blk64_t شروع_blk,
			     ext2fs_بلوک_bitmap bmap, صحيح rem_grp,
			     صحيح elem_اندازه)
{
	صحيح		flexbg, flexbg_اندازه, اندازه;
	blk64_t		آخرین_blk, اولی_free = 0;
	dgrp_t	       	آخرین_grp;

	flexbg_اندازه = 1U << fs->super->s_log_groups_در_هر_flex;
	flexbg = گروه / flexbg_اندازه;
	اندازه = rem_grp * elem_اندازه;

	إذا (اندازه > (صحيح) (fs->super->s_بلوکها_در_هر_گروه / 4))
		اندازه = (صحيح) fs->super->s_بلوکها_در_هر_گروه / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	إذا (شروع_blk && شروع_blk < ext2fs_بلوکها_شمار(fs->super) &&
	    ext2fs_get_free_blocks2(fs, شروع_blk, شروع_blk + اندازه, elem_اندازه,
				    bmap, &اولی_free) == 0)
		أرجع اولی_free;

	شروع_blk = ext2fs_گروه_اولی_block2(fs, flexbg_اندازه * flexbg);
	آخرین_grp = گروه | (flexbg_اندازه - 1);
	إذا (آخرین_grp > fs->گروه_desc_شمار-1)
		آخرین_grp = fs->گروه_desc_شمار-1;
	آخرین_blk = ext2fs_گروه_آخرین_block2(fs, آخرین_grp);

	/* Find the first available block */
	إذا (ext2fs_get_free_blocks2(fs, شروع_blk, آخرین_blk, اندازه,
				    bmap, &اولی_free) == 0)
		أرجع اولی_free;

	إذا (ext2fs_get_free_blocks2(fs, شروع_blk, آخرین_blk, elem_اندازه,
				   bmap, &اولی_free) == 0)
		أرجع اولی_free;

	إذا (ext2fs_get_free_blocks2(fs, 0, آخرین_blk, elem_اندازه, bmap,
				    &اولی_free) == 0)
		أرجع اولی_free;

	أرجع اولی_free;
}

errcode_t ext2fs_allocate_گروه_جدول(ext2_filsys fs, dgrp_t گروه,
				      ext2fs_بلوک_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		گروه_blk, شروع_blk, آخرین_blk, new_blk;
	dgrp_t		آخرین_grp = 0;
	صحيح		rem_grps = 0, flexbg_اندازه = 0, جدول_جابه_جایی = 0;

	گروه_blk = ext2fs_گروه_اولی_block2(fs, گروه);
	آخرین_blk = ext2fs_گروه_آخرین_block2(fs, گروه);

	إذا (!bmap)
		bmap = fs->بلوک_map;

	إذا (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_در_هر_flex) {
		flexbg_اندازه = 1U << fs->super->s_log_groups_در_هر_flex;
		آخرین_grp = گروه | (flexbg_اندازه - 1);
		إذا (آخرین_grp > fs->گروه_desc_شمار-1)
			آخرین_grp = fs->گروه_desc_شمار-1;
		rem_grps = آخرین_grp - گروه + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	إذا (fs->stride && !flexbg_اندازه) {
		retval = ext2fs_get_free_blocks2(fs, گروه_blk, آخرین_blk,
						 1, bmap, &شروع_blk);
		إذا (retval)
			أرجع retval;
		شروع_blk += fs->inode_بلوکها_در_هر_گروه;
		شروع_blk += ((fs->stride * گروه) %
			      (آخرین_blk - شروع_blk + 1));
		إذا (شروع_blk >= آخرین_blk)
			شروع_blk = گروه_blk;
	} وإلا
		شروع_blk = گروه_blk;

	إذا (flexbg_اندازه) {
		blk64_t prev_بلوک = 0;

		جدول_جابه_جایی = flexbg_اندازه;
		إذا (گروه % flexbg_اندازه)
			prev_بلوک = ext2fs_بلوک_bitmap_loc(fs, گروه - 1) + 1;
		وإلا إذا (آخرین_grp == fs->گروه_desc_شمار-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			جدول_جابه_جایی = آخرین_grp & (flexbg_اندازه - 1);
			إذا (جدول_جابه_جایی == 0)
				جدول_جابه_جایی = flexbg_اندازه;
			وإلا
				جدول_جابه_جایی++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		شروع_blk = flexbg_جابه_جایی(fs, گروه, prev_بلوک, bmap,
					  rem_grps, 1);
		آخرین_blk = ext2fs_گروه_آخرین_block2(fs, آخرین_grp);
	}

	إذا (!ext2fs_بلوک_bitmap_loc(fs, گروه)) {
		retval = ext2fs_get_free_blocks2(fs, شروع_blk, آخرین_blk,
						 1, bmap, &new_blk);
		إذا (retval == EXT2_ET_بلوک_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, گروه_blk,
					آخرین_blk, 1, bmap, &new_blk);
		إذا (retval)
			أرجع retval;
		ext2fs_mark_بلوک_bitmap2(bmap, new_blk);
		ext2fs_بلوک_bitmap_loc_مجموعه(fs, گروه, new_blk);
		إذا (flexbg_اندازه) {
			dgrp_t gr = ext2fs_گروه_از_blk2(fs, new_blk);
			ext2fs_bg_free_بلوکها_شمار_مجموعه(fs, gr, ext2fs_bg_free_بلوکها_شمار(fs, gr) - 1);
			ext2fs_free_بلوکها_شمار_جمع_کن(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلوک_UNINIT);
			ext2fs_گروه_desc_csum_مجموعه(fs, gr);
		}
	}

	إذا (flexbg_اندازه) {
		blk64_t prev_بلوک = 0;
		إذا (گروه % flexbg_اندازه)
			prev_بلوک = ext2fs_inode_bitmap_loc(fs, گروه - 1) + 1;
		وإلا
			prev_بلوک = ext2fs_بلوک_bitmap_loc(fs, گروه) +
				جدول_جابه_جایی;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		شروع_blk = flexbg_جابه_جایی(fs, گروه, prev_بلوک, bmap,
					  rem_grps, 1);
		آخرین_blk = ext2fs_گروه_آخرین_block2(fs, آخرین_grp);
	}

	إذا (!ext2fs_inode_bitmap_loc(fs, گروه)) {
		retval = ext2fs_get_free_blocks2(fs, شروع_blk, آخرین_blk,
						 1, bmap, &new_blk);
		إذا (retval == EXT2_ET_بلوک_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, گروه_blk,
					 آخرین_blk, 1, bmap, &new_blk);
		إذا (retval)
			أرجع retval;
		ext2fs_mark_بلوک_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_مجموعه(fs, گروه, new_blk);
		إذا (flexbg_اندازه) {
			dgrp_t gr = ext2fs_گروه_از_blk2(fs, new_blk);
			ext2fs_bg_free_بلوکها_شمار_مجموعه(fs, gr, ext2fs_bg_free_بلوکها_شمار(fs, gr) - 1);
			ext2fs_free_بلوکها_شمار_جمع_کن(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلوک_UNINIT);
			ext2fs_گروه_desc_csum_مجموعه(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	إذا (flexbg_اندازه) {
		blk64_t prev_بلوک = 0;

		إذا (گروه % flexbg_اندازه)
			prev_بلوک = ext2fs_inode_جدول_loc(fs, گروه - 1) +
				fs->inode_بلوکها_در_هر_گروه;
		وإلا
			prev_بلوک = ext2fs_inode_bitmap_loc(fs, گروه) +
				جدول_جابه_جایی;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		گروه_blk = flexbg_جابه_جایی(fs, گروه, prev_بلوک, bmap,
					  rem_grps, fs->inode_بلوکها_در_هر_گروه);
		آخرین_blk = ext2fs_گروه_آخرین_block2(fs, آخرین_grp);
	}

	إذا (!ext2fs_inode_جدول_loc(fs, گروه)) {
		retval = ext2fs_get_free_blocks2(fs, گروه_blk, آخرین_blk,
						fs->inode_بلوکها_در_هر_گروه,
						bmap, &new_blk);
		إذا (retval)
			أرجع retval;

		ext2fs_mark_بلوک_bitmap_range2(bmap,
			new_blk, fs->inode_بلوکها_در_هر_گروه);
		إذا (flexbg_اندازه) {
			blk64_t رقم, blk;
			رقم = fs->inode_بلوکها_در_هر_گروه;
			blk = new_blk;
			طالما (رقم) {
				صحيح gr = ext2fs_گروه_از_blk2(fs, blk);
				آخرین_blk = ext2fs_گروه_آخرین_block2(fs, gr);
				blk64_t n = رقم;

				إذا (blk + رقم > آخرین_blk)
					n = آخرین_blk - blk + 1;

				ext2fs_bg_free_بلوکها_شمار_مجموعه(fs, gr,
					ext2fs_bg_free_بلوکها_شمار(fs, gr) -
					n/EXT2_FS_CLUSTER_نسبت(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_بلوک_UNINIT);
				ext2fs_گروه_desc_csum_مجموعه(fs, gr);
				ext2fs_free_بلوکها_شمار_جمع_کن(fs->super, -n);
				blk += n;
				رقم -= n;
			}
		}
		ext2fs_inode_جدول_loc_مجموعه(fs, گروه, new_blk);
	}
	ext2fs_گروه_desc_csum_مجموعه(fs, گروه);
	أرجع 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	بنية ext2fs_numeric_پیشرفت_struct پیشرفت;

	إذا (fs->پیشرفت_ops && fs->پیشرفت_ops->init)
		(fs->پیشرفت_ops->init)(fs, &پیشرفت, عدم,
					 fs->گروه_desc_شمار);

	لكل (i = 0; i < fs->گروه_desc_شمار; i++) {
		إذا (fs->پیشرفت_ops && fs->پیشرفت_ops->update)
			(fs->پیشرفت_ops->update)(fs, &پیشرفت, i);
		retval = ext2fs_allocate_گروه_جدول(fs, i, fs->بلوک_map);
		إذا (retval)
			أرجع retval;
	}
	إذا (fs->پیشرفت_ops && fs->پیشرفت_ops->close)
		(fs->پیشرفت_ops->close)(fs, &پیشرفت, عدم);
	أرجع 0;
}

