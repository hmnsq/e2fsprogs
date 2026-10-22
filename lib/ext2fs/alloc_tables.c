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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
స్థిర blk64_t flexbg_స్థానభ్రంశం(ext2_filsys fs, dgrp_t సమూహం, blk64_t ప్రారంభం_blk,
			     ext2fs_ఖండం_bitmap bmap, పూర్ణాంకం rem_grp,
			     పూర్ణాంకం elem_కొలత)
{
	పూర్ణాంకం		flexbg, flexbg_కొలత, కొలత;
	blk64_t		చివరి_blk, మొదటి_free = 0;
	dgrp_t	       	చివరి_grp;

	flexbg_కొలత = 1U << fs->super->s_log_groups_చొప్పున_flex;
	flexbg = సమూహం / flexbg_కొలత;
	కొలత = rem_grp * elem_కొలత;

	ఒకవేళ (కొలత > (పూర్ణాంకం) (fs->super->s_ఖండాలు_చొప్పున_సమూహం / 4))
		కొలత = (పూర్ణాంకం) fs->super->s_ఖండాలు_చొప్పున_సమూహం / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	ఒకవేళ (ప్రారంభం_blk && ప్రారంభం_blk < ext2fs_ఖండాలు_లెక్క(fs->super) &&
	    ext2fs_get_free_blocks2(fs, ప్రారంభం_blk, ప్రారంభం_blk + కొలత, elem_కొలత,
				    bmap, &మొదటి_free) == 0)
		తిరిగి మొదటి_free;

	ప్రారంభం_blk = ext2fs_సమూహం_మొదటి_block2(fs, flexbg_కొలత * flexbg);
	చివరి_grp = సమూహం | (flexbg_కొలత - 1);
	ఒకవేళ (చివరి_grp > fs->సమూహం_desc_లెక్క-1)
		చివరి_grp = fs->సమూహం_desc_లెక్క-1;
	చివరి_blk = ext2fs_సమూహం_చివరి_block2(fs, చివరి_grp);

	/* Find the first available block */
	ఒకవేళ (ext2fs_get_free_blocks2(fs, ప్రారంభం_blk, చివరి_blk, కొలత,
				    bmap, &మొదటి_free) == 0)
		తిరిగి మొదటి_free;

	ఒకవేళ (ext2fs_get_free_blocks2(fs, ప్రారంభం_blk, చివరి_blk, elem_కొలత,
				   bmap, &మొదటి_free) == 0)
		తిరిగి మొదటి_free;

	ఒకవేళ (ext2fs_get_free_blocks2(fs, 0, చివరి_blk, elem_కొలత, bmap,
				    &మొదటి_free) == 0)
		తిరిగి మొదటి_free;

	తిరిగి మొదటి_free;
}

errcode_t ext2fs_allocate_సమూహం_పట్టిక(ext2_filsys fs, dgrp_t సమూహం,
				      ext2fs_ఖండం_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		సమూహం_blk, ప్రారంభం_blk, చివరి_blk, new_blk;
	dgrp_t		చివరి_grp = 0;
	పూర్ణాంకం		rem_grps = 0, flexbg_కొలత = 0, పట్టిక_స్థానభ్రంశం = 0;

	సమూహం_blk = ext2fs_సమూహం_మొదటి_block2(fs, సమూహం);
	చివరి_blk = ext2fs_సమూహం_చివరి_block2(fs, సమూహం);

	ఒకవేళ (!bmap)
		bmap = fs->ఖండం_map;

	ఒకవేళ (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_చొప్పున_flex) {
		flexbg_కొలత = 1U << fs->super->s_log_groups_చొప్పున_flex;
		చివరి_grp = సమూహం | (flexbg_కొలత - 1);
		ఒకవేళ (చివరి_grp > fs->సమూహం_desc_లెక్క-1)
			చివరి_grp = fs->సమూహం_desc_లెక్క-1;
		rem_grps = చివరి_grp - సమూహం + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	ఒకవేళ (fs->stride && !flexbg_కొలత) {
		retval = ext2fs_get_free_blocks2(fs, సమూహం_blk, చివరి_blk,
						 1, bmap, &ప్రారంభం_blk);
		ఒకవేళ (retval)
			తిరిగి retval;
		ప్రారంభం_blk += fs->inode_ఖండాలు_చొప్పున_సమూహం;
		ప్రారంభం_blk += ((fs->stride * సమూహం) %
			      (చివరి_blk - ప్రారంభం_blk + 1));
		ఒకవేళ (ప్రారంభం_blk >= చివరి_blk)
			ప్రారంభం_blk = సమూహం_blk;
	} లేకపోతే
		ప్రారంభం_blk = సమూహం_blk;

	ఒకవేళ (flexbg_కొలత) {
		blk64_t prev_ఖండం = 0;

		పట్టిక_స్థానభ్రంశం = flexbg_కొలత;
		ఒకవేళ (సమూహం % flexbg_కొలత)
			prev_ఖండం = ext2fs_ఖండం_bitmap_loc(fs, సమూహం - 1) + 1;
		లేకపోతే ఒకవేళ (చివరి_grp == fs->సమూహం_desc_లెక్క-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			పట్టిక_స్థానభ్రంశం = చివరి_grp & (flexbg_కొలత - 1);
			ఒకవేళ (పట్టిక_స్థానభ్రంశం == 0)
				పట్టిక_స్థానభ్రంశం = flexbg_కొలత;
			లేకపోతే
				పట్టిక_స్థానభ్రంశం++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		ప్రారంభం_blk = flexbg_స్థానభ్రంశం(fs, సమూహం, prev_ఖండం, bmap,
					  rem_grps, 1);
		చివరి_blk = ext2fs_సమూహం_చివరి_block2(fs, చివరి_grp);
	}

	ఒకవేళ (!ext2fs_ఖండం_bitmap_loc(fs, సమూహం)) {
		retval = ext2fs_get_free_blocks2(fs, ప్రారంభం_blk, చివరి_blk,
						 1, bmap, &new_blk);
		ఒకవేళ (retval == EXT2_ET_ఖండం_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, సమూహం_blk,
					చివరి_blk, 1, bmap, &new_blk);
		ఒకవేళ (retval)
			తిరిగి retval;
		ext2fs_mark_ఖండం_bitmap2(bmap, new_blk);
		ext2fs_ఖండం_bitmap_loc_సమితి(fs, సమూహం, new_blk);
		ఒకవేళ (flexbg_కొలత) {
			dgrp_t gr = ext2fs_సమూహం_యొక్క_blk2(fs, new_blk);
			ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(fs, gr, ext2fs_bg_free_ఖండాలు_లెక్క(fs, gr) - 1);
			ext2fs_free_ఖండాలు_లెక్క_కూడు(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_ఖండం_UNINIT);
			ext2fs_సమూహం_desc_csum_సమితి(fs, gr);
		}
	}

	ఒకవేళ (flexbg_కొలత) {
		blk64_t prev_ఖండం = 0;
		ఒకవేళ (సమూహం % flexbg_కొలత)
			prev_ఖండం = ext2fs_inode_bitmap_loc(fs, సమూహం - 1) + 1;
		లేకపోతే
			prev_ఖండం = ext2fs_ఖండం_bitmap_loc(fs, సమూహం) +
				పట్టిక_స్థానభ్రంశం;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		ప్రారంభం_blk = flexbg_స్థానభ్రంశం(fs, సమూహం, prev_ఖండం, bmap,
					  rem_grps, 1);
		చివరి_blk = ext2fs_సమూహం_చివరి_block2(fs, చివరి_grp);
	}

	ఒకవేళ (!ext2fs_inode_bitmap_loc(fs, సమూహం)) {
		retval = ext2fs_get_free_blocks2(fs, ప్రారంభం_blk, చివరి_blk,
						 1, bmap, &new_blk);
		ఒకవేళ (retval == EXT2_ET_ఖండం_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, సమూహం_blk,
					 చివరి_blk, 1, bmap, &new_blk);
		ఒకవేళ (retval)
			తిరిగి retval;
		ext2fs_mark_ఖండం_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_సమితి(fs, సమూహం, new_blk);
		ఒకవేళ (flexbg_కొలత) {
			dgrp_t gr = ext2fs_సమూహం_యొక్క_blk2(fs, new_blk);
			ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(fs, gr, ext2fs_bg_free_ఖండాలు_లెక్క(fs, gr) - 1);
			ext2fs_free_ఖండాలు_లెక్క_కూడు(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_ఖండం_UNINIT);
			ext2fs_సమూహం_desc_csum_సమితి(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	ఒకవేళ (flexbg_కొలత) {
		blk64_t prev_ఖండం = 0;

		ఒకవేళ (సమూహం % flexbg_కొలత)
			prev_ఖండం = ext2fs_inode_పట్టిక_loc(fs, సమూహం - 1) +
				fs->inode_ఖండాలు_చొప్పున_సమూహం;
		లేకపోతే
			prev_ఖండం = ext2fs_inode_bitmap_loc(fs, సమూహం) +
				పట్టిక_స్థానభ్రంశం;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		సమూహం_blk = flexbg_స్థానభ్రంశం(fs, సమూహం, prev_ఖండం, bmap,
					  rem_grps, fs->inode_ఖండాలు_చొప్పున_సమూహం);
		చివరి_blk = ext2fs_సమూహం_చివరి_block2(fs, చివరి_grp);
	}

	ఒకవేళ (!ext2fs_inode_పట్టిక_loc(fs, సమూహం)) {
		retval = ext2fs_get_free_blocks2(fs, సమూహం_blk, చివరి_blk,
						fs->inode_ఖండాలు_చొప్పున_సమూహం,
						bmap, &new_blk);
		ఒకవేళ (retval)
			తిరిగి retval;

		ext2fs_mark_ఖండం_bitmap_range2(bmap,
			new_blk, fs->inode_ఖండాలు_చొప్పున_సమూహం);
		ఒకవేళ (flexbg_కొలత) {
			blk64_t అంకె, blk;
			అంకె = fs->inode_ఖండాలు_చొప్పున_సమూహం;
			blk = new_blk;
			అయితే (అంకె) {
				పూర్ణాంకం gr = ext2fs_సమూహం_యొక్క_blk2(fs, blk);
				చివరి_blk = ext2fs_సమూహం_చివరి_block2(fs, gr);
				blk64_t n = అంకె;

				ఒకవేళ (blk + అంకె > చివరి_blk)
					n = చివరి_blk - blk + 1;

				ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(fs, gr,
					ext2fs_bg_free_ఖండాలు_లెక్క(fs, gr) -
					n/EXT2_FS_CLUSTER_నిష్పత్తి(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_ఖండం_UNINIT);
				ext2fs_సమూహం_desc_csum_సమితి(fs, gr);
				ext2fs_free_ఖండాలు_లెక్క_కూడు(fs->super, -n);
				blk += n;
				అంకె -= n;
			}
		}
		ext2fs_inode_పట్టిక_loc_సమితి(fs, సమూహం, new_blk);
	}
	ext2fs_సమూహం_desc_csum_సమితి(fs, సమూహం);
	తిరిగి 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	నిర్మాణం ext2fs_numeric_పురోగతి_struct పురోగతి;

	ఒకవేళ (fs->పురోగతి_ops && fs->పురోగతి_ops->init)
		(fs->పురోగతి_ops->init)(fs, &పురోగతి, ఖాళీ,
					 fs->సమూహం_desc_లెక్క);

	కొరకు (i = 0; i < fs->సమూహం_desc_లెక్క; i++) {
		ఒకవేళ (fs->పురోగతి_ops && fs->పురోగతి_ops->update)
			(fs->పురోగతి_ops->update)(fs, &పురోగతి, i);
		retval = ext2fs_allocate_సమూహం_పట్టిక(fs, i, fs->ఖండం_map);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	ఒకవేళ (fs->పురోగతి_ops && fs->పురోగతి_ops->close)
		(fs->పురోగతి_ops->close)(fs, &పురోగతి, ఖాళీ);
	తిరిగి 0;
}

