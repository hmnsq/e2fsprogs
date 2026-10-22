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
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
ساكن blk64_t flexbg_وتھ(ext2_filsys fs, dgrp_t گروہ, blk64_t شروع_blk,
			     ext2fs_بلاک_bitmap bmap, صحيح rem_grp,
			     صحيح elem_ناپ)
{
	صحيح		flexbg, flexbg_ناپ, ناپ;
	blk64_t		آخری_blk, پہلا_free = 0;
	dgrp_t	       	آخری_grp;

	flexbg_ناپ = 1U << fs->super->s_log_groups_فی_flex;
	flexbg = گروہ / flexbg_ناپ;
	ناپ = rem_grp * elem_ناپ;

	إذا (ناپ > (صحيح) (fs->super->s_بلاکاں_فی_گروہ / 4))
		ناپ = (صحيح) fs->super->s_بلاکاں_فی_گروہ / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	إذا (شروع_blk && شروع_blk < ext2fs_بلاکاں_گنتی(fs->super) &&
	    ext2fs_get_free_blocks2(fs, شروع_blk, شروع_blk + ناپ, elem_ناپ,
				    bmap, &پہلا_free) == 0)
		أرجع پہلا_free;

	شروع_blk = ext2fs_گروہ_پہلا_block2(fs, flexbg_ناپ * flexbg);
	آخری_grp = گروہ | (flexbg_ناپ - 1);
	إذا (آخری_grp > fs->گروہ_desc_گنتی-1)
		آخری_grp = fs->گروہ_desc_گنتی-1;
	آخری_blk = ext2fs_گروہ_آخری_block2(fs, آخری_grp);

	/* Find the first available block */
	إذا (ext2fs_get_free_blocks2(fs, شروع_blk, آخری_blk, ناپ,
				    bmap, &پہلا_free) == 0)
		أرجع پہلا_free;

	إذا (ext2fs_get_free_blocks2(fs, شروع_blk, آخری_blk, elem_ناپ,
				   bmap, &پہلا_free) == 0)
		أرجع پہلا_free;

	إذا (ext2fs_get_free_blocks2(fs, 0, آخری_blk, elem_ناپ, bmap,
				    &پہلا_free) == 0)
		أرجع پہلا_free;

	أرجع پہلا_free;
}

errcode_t ext2fs_allocate_گروہ_جدول(ext2_filsys fs, dgrp_t گروہ,
				      ext2fs_بلاک_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		گروہ_blk, شروع_blk, آخری_blk, new_blk;
	dgrp_t		آخری_grp = 0;
	صحيح		rem_grps = 0, flexbg_ناپ = 0, جدول_وتھ = 0;

	گروہ_blk = ext2fs_گروہ_پہلا_block2(fs, گروہ);
	آخری_blk = ext2fs_گروہ_آخری_block2(fs, گروہ);

	إذا (!bmap)
		bmap = fs->بلاک_map;

	إذا (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_فی_flex) {
		flexbg_ناپ = 1U << fs->super->s_log_groups_فی_flex;
		آخری_grp = گروہ | (flexbg_ناپ - 1);
		إذا (آخری_grp > fs->گروہ_desc_گنتی-1)
			آخری_grp = fs->گروہ_desc_گنتی-1;
		rem_grps = آخری_grp - گروہ + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	إذا (fs->stride && !flexbg_ناپ) {
		retval = ext2fs_get_free_blocks2(fs, گروہ_blk, آخری_blk,
						 1, bmap, &شروع_blk);
		إذا (retval)
			أرجع retval;
		شروع_blk += fs->inode_بلاکاں_فی_گروہ;
		شروع_blk += ((fs->stride * گروہ) %
			      (آخری_blk - شروع_blk + 1));
		إذا (شروع_blk >= آخری_blk)
			شروع_blk = گروہ_blk;
	} وإلا
		شروع_blk = گروہ_blk;

	إذا (flexbg_ناپ) {
		blk64_t prev_بلاک = 0;

		جدول_وتھ = flexbg_ناپ;
		إذا (گروہ % flexbg_ناپ)
			prev_بلاک = ext2fs_بلاک_bitmap_loc(fs, گروہ - 1) + 1;
		وإلا إذا (آخری_grp == fs->گروہ_desc_گنتی-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			جدول_وتھ = آخری_grp & (flexbg_ناپ - 1);
			إذا (جدول_وتھ == 0)
				جدول_وتھ = flexbg_ناپ;
			وإلا
				جدول_وتھ++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		شروع_blk = flexbg_وتھ(fs, گروہ, prev_بلاک, bmap,
					  rem_grps, 1);
		آخری_blk = ext2fs_گروہ_آخری_block2(fs, آخری_grp);
	}

	إذا (!ext2fs_بلاک_bitmap_loc(fs, گروہ)) {
		retval = ext2fs_get_free_blocks2(fs, شروع_blk, آخری_blk,
						 1, bmap, &new_blk);
		إذا (retval == EXT2_ET_بلاک_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, گروہ_blk,
					آخری_blk, 1, bmap, &new_blk);
		إذا (retval)
			أرجع retval;
		ext2fs_mark_بلاک_bitmap2(bmap, new_blk);
		ext2fs_بلاک_bitmap_loc_سیٹ(fs, گروہ, new_blk);
		إذا (flexbg_ناپ) {
			dgrp_t gr = ext2fs_گروہ_دا_blk2(fs, new_blk);
			ext2fs_bg_free_بلاکاں_گنتی_سیٹ(fs, gr, ext2fs_bg_free_بلاکاں_گنتی(fs, gr) - 1);
			ext2fs_free_بلاکاں_گنتی_جوڑو(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلاک_UNINIT);
			ext2fs_گروہ_desc_csum_سیٹ(fs, gr);
		}
	}

	إذا (flexbg_ناپ) {
		blk64_t prev_بلاک = 0;
		إذا (گروہ % flexbg_ناپ)
			prev_بلاک = ext2fs_inode_bitmap_loc(fs, گروہ - 1) + 1;
		وإلا
			prev_بلاک = ext2fs_بلاک_bitmap_loc(fs, گروہ) +
				جدول_وتھ;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		شروع_blk = flexbg_وتھ(fs, گروہ, prev_بلاک, bmap,
					  rem_grps, 1);
		آخری_blk = ext2fs_گروہ_آخری_block2(fs, آخری_grp);
	}

	إذا (!ext2fs_inode_bitmap_loc(fs, گروہ)) {
		retval = ext2fs_get_free_blocks2(fs, شروع_blk, آخری_blk,
						 1, bmap, &new_blk);
		إذا (retval == EXT2_ET_بلاک_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, گروہ_blk,
					 آخری_blk, 1, bmap, &new_blk);
		إذا (retval)
			أرجع retval;
		ext2fs_mark_بلاک_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_سیٹ(fs, گروہ, new_blk);
		إذا (flexbg_ناپ) {
			dgrp_t gr = ext2fs_گروہ_دا_blk2(fs, new_blk);
			ext2fs_bg_free_بلاکاں_گنتی_سیٹ(fs, gr, ext2fs_bg_free_بلاکاں_گنتی(fs, gr) - 1);
			ext2fs_free_بلاکاں_گنتی_جوڑو(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلاک_UNINIT);
			ext2fs_گروہ_desc_csum_سیٹ(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	إذا (flexbg_ناپ) {
		blk64_t prev_بلاک = 0;

		إذا (گروہ % flexbg_ناپ)
			prev_بلاک = ext2fs_inode_جدول_loc(fs, گروہ - 1) +
				fs->inode_بلاکاں_فی_گروہ;
		وإلا
			prev_بلاک = ext2fs_inode_bitmap_loc(fs, گروہ) +
				جدول_وتھ;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		گروہ_blk = flexbg_وتھ(fs, گروہ, prev_بلاک, bmap,
					  rem_grps, fs->inode_بلاکاں_فی_گروہ);
		آخری_blk = ext2fs_گروہ_آخری_block2(fs, آخری_grp);
	}

	إذا (!ext2fs_inode_جدول_loc(fs, گروہ)) {
		retval = ext2fs_get_free_blocks2(fs, گروہ_blk, آخری_blk,
						fs->inode_بلاکاں_فی_گروہ,
						bmap, &new_blk);
		إذا (retval)
			أرجع retval;

		ext2fs_mark_بلاک_bitmap_range2(bmap,
			new_blk, fs->inode_بلاکاں_فی_گروہ);
		إذا (flexbg_ناپ) {
			blk64_t عدد, blk;
			عدد = fs->inode_بلاکاں_فی_گروہ;
			blk = new_blk;
			طالما (عدد) {
				صحيح gr = ext2fs_گروہ_دا_blk2(fs, blk);
				آخری_blk = ext2fs_گروہ_آخری_block2(fs, gr);
				blk64_t n = عدد;

				إذا (blk + عدد > آخری_blk)
					n = آخری_blk - blk + 1;

				ext2fs_bg_free_بلاکاں_گنتی_سیٹ(fs, gr,
					ext2fs_bg_free_بلاکاں_گنتی(fs, gr) -
					n/EXT2_FS_CLUSTER_تناسب(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_بلاک_UNINIT);
				ext2fs_گروہ_desc_csum_سیٹ(fs, gr);
				ext2fs_free_بلاکاں_گنتی_جوڑو(fs->super, -n);
				blk += n;
				عدد -= n;
			}
		}
		ext2fs_inode_جدول_loc_سیٹ(fs, گروہ, new_blk);
	}
	ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);
	أرجع 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	بنية ext2fs_numeric_ترقی_struct ترقی;

	إذا (fs->ترقی_ops && fs->ترقی_ops->init)
		(fs->ترقی_ops->init)(fs, &ترقی, عدم,
					 fs->گروہ_desc_گنتی);

	لكل (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		إذا (fs->ترقی_ops && fs->ترقی_ops->update)
			(fs->ترقی_ops->update)(fs, &ترقی, i);
		retval = ext2fs_allocate_گروہ_جدول(fs, i, fs->بلاک_map);
		إذا (retval)
			أرجع retval;
	}
	إذا (fs->ترقی_ops && fs->ترقی_ops->close)
		(fs->ترقی_ops->close)(fs, &ترقی, عدم);
	أرجع 0;
}

