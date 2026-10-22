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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
ساكن blk64_t flexbg_وٿي(ext2_filsys fs, dgrp_t گروهه, blk64_t شروعات_blk,
			     ext2fs_بلاڪ_bitmap bmap, صحيح rem_grp,
			     صحيح elem_سائيز)
{
	صحيح		flexbg, flexbg_سائيز, سائيز;
	blk64_t		آخري_blk, پهريون_free = 0;
	dgrp_t	       	آخري_grp;

	flexbg_سائيز = 1U << fs->super->s_log_groups_في_flex;
	flexbg = گروهه / flexbg_سائيز;
	سائيز = rem_grp * elem_سائيز;

	إذا (سائيز > (صحيح) (fs->super->s_بلاڪون_في_گروهه / 4))
		سائيز = (صحيح) fs->super->s_بلاڪون_في_گروهه / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	إذا (شروعات_blk && شروعات_blk < ext2fs_بلاڪون_ڳڻپ(fs->super) &&
	    ext2fs_get_free_blocks2(fs, شروعات_blk, شروعات_blk + سائيز, elem_سائيز,
				    bmap, &پهريون_free) == 0)
		أرجع پهريون_free;

	شروعات_blk = ext2fs_گروهه_پهريون_block2(fs, flexbg_سائيز * flexbg);
	آخري_grp = گروهه | (flexbg_سائيز - 1);
	إذا (آخري_grp > fs->گروهه_desc_ڳڻپ-1)
		آخري_grp = fs->گروهه_desc_ڳڻپ-1;
	آخري_blk = ext2fs_گروهه_آخري_block2(fs, آخري_grp);

	/* Find the first available block */
	إذا (ext2fs_get_free_blocks2(fs, شروعات_blk, آخري_blk, سائيز,
				    bmap, &پهريون_free) == 0)
		أرجع پهريون_free;

	إذا (ext2fs_get_free_blocks2(fs, شروعات_blk, آخري_blk, elem_سائيز,
				   bmap, &پهريون_free) == 0)
		أرجع پهريون_free;

	إذا (ext2fs_get_free_blocks2(fs, 0, آخري_blk, elem_سائيز, bmap,
				    &پهريون_free) == 0)
		أرجع پهريون_free;

	أرجع پهريون_free;
}

errcode_t ext2fs_allocate_گروهه_جدول(ext2_filsys fs, dgrp_t گروهه,
				      ext2fs_بلاڪ_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		گروهه_blk, شروعات_blk, آخري_blk, new_blk;
	dgrp_t		آخري_grp = 0;
	صحيح		rem_grps = 0, flexbg_سائيز = 0, جدول_وٿي = 0;

	گروهه_blk = ext2fs_گروهه_پهريون_block2(fs, گروهه);
	آخري_blk = ext2fs_گروهه_آخري_block2(fs, گروهه);

	إذا (!bmap)
		bmap = fs->بلاڪ_map;

	إذا (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_في_flex) {
		flexbg_سائيز = 1U << fs->super->s_log_groups_في_flex;
		آخري_grp = گروهه | (flexbg_سائيز - 1);
		إذا (آخري_grp > fs->گروهه_desc_ڳڻپ-1)
			آخري_grp = fs->گروهه_desc_ڳڻپ-1;
		rem_grps = آخري_grp - گروهه + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	إذا (fs->stride && !flexbg_سائيز) {
		retval = ext2fs_get_free_blocks2(fs, گروهه_blk, آخري_blk,
						 1, bmap, &شروعات_blk);
		إذا (retval)
			أرجع retval;
		شروعات_blk += fs->inode_بلاڪون_في_گروهه;
		شروعات_blk += ((fs->stride * گروهه) %
			      (آخري_blk - شروعات_blk + 1));
		إذا (شروعات_blk >= آخري_blk)
			شروعات_blk = گروهه_blk;
	} وإلا
		شروعات_blk = گروهه_blk;

	إذا (flexbg_سائيز) {
		blk64_t prev_بلاڪ = 0;

		جدول_وٿي = flexbg_سائيز;
		إذا (گروهه % flexbg_سائيز)
			prev_بلاڪ = ext2fs_بلاڪ_bitmap_loc(fs, گروهه - 1) + 1;
		وإلا إذا (آخري_grp == fs->گروهه_desc_ڳڻپ-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			جدول_وٿي = آخري_grp & (flexbg_سائيز - 1);
			إذا (جدول_وٿي == 0)
				جدول_وٿي = flexbg_سائيز;
			وإلا
				جدول_وٿي++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		شروعات_blk = flexbg_وٿي(fs, گروهه, prev_بلاڪ, bmap,
					  rem_grps, 1);
		آخري_blk = ext2fs_گروهه_آخري_block2(fs, آخري_grp);
	}

	إذا (!ext2fs_بلاڪ_bitmap_loc(fs, گروهه)) {
		retval = ext2fs_get_free_blocks2(fs, شروعات_blk, آخري_blk,
						 1, bmap, &new_blk);
		إذا (retval == EXT2_ET_بلاڪ_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, گروهه_blk,
					آخري_blk, 1, bmap, &new_blk);
		إذا (retval)
			أرجع retval;
		ext2fs_mark_بلاڪ_bitmap2(bmap, new_blk);
		ext2fs_بلاڪ_bitmap_loc_سيٽ(fs, گروهه, new_blk);
		إذا (flexbg_سائيز) {
			dgrp_t gr = ext2fs_گروهه_جو_blk2(fs, new_blk);
			ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(fs, gr, ext2fs_bg_free_بلاڪون_ڳڻپ(fs, gr) - 1);
			ext2fs_free_بلاڪون_ڳڻپ_جوڙيو(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلاڪ_UNINIT);
			ext2fs_گروهه_desc_csum_سيٽ(fs, gr);
		}
	}

	إذا (flexbg_سائيز) {
		blk64_t prev_بلاڪ = 0;
		إذا (گروهه % flexbg_سائيز)
			prev_بلاڪ = ext2fs_inode_bitmap_loc(fs, گروهه - 1) + 1;
		وإلا
			prev_بلاڪ = ext2fs_بلاڪ_bitmap_loc(fs, گروهه) +
				جدول_وٿي;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		شروعات_blk = flexbg_وٿي(fs, گروهه, prev_بلاڪ, bmap,
					  rem_grps, 1);
		آخري_blk = ext2fs_گروهه_آخري_block2(fs, آخري_grp);
	}

	إذا (!ext2fs_inode_bitmap_loc(fs, گروهه)) {
		retval = ext2fs_get_free_blocks2(fs, شروعات_blk, آخري_blk,
						 1, bmap, &new_blk);
		إذا (retval == EXT2_ET_بلاڪ_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, گروهه_blk,
					 آخري_blk, 1, bmap, &new_blk);
		إذا (retval)
			أرجع retval;
		ext2fs_mark_بلاڪ_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_سيٽ(fs, گروهه, new_blk);
		إذا (flexbg_سائيز) {
			dgrp_t gr = ext2fs_گروهه_جو_blk2(fs, new_blk);
			ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(fs, gr, ext2fs_bg_free_بلاڪون_ڳڻپ(fs, gr) - 1);
			ext2fs_free_بلاڪون_ڳڻپ_جوڙيو(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_بلاڪ_UNINIT);
			ext2fs_گروهه_desc_csum_سيٽ(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	إذا (flexbg_سائيز) {
		blk64_t prev_بلاڪ = 0;

		إذا (گروهه % flexbg_سائيز)
			prev_بلاڪ = ext2fs_inode_جدول_loc(fs, گروهه - 1) +
				fs->inode_بلاڪون_في_گروهه;
		وإلا
			prev_بلاڪ = ext2fs_inode_bitmap_loc(fs, گروهه) +
				جدول_وٿي;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		گروهه_blk = flexbg_وٿي(fs, گروهه, prev_بلاڪ, bmap,
					  rem_grps, fs->inode_بلاڪون_في_گروهه);
		آخري_blk = ext2fs_گروهه_آخري_block2(fs, آخري_grp);
	}

	إذا (!ext2fs_inode_جدول_loc(fs, گروهه)) {
		retval = ext2fs_get_free_blocks2(fs, گروهه_blk, آخري_blk,
						fs->inode_بلاڪون_في_گروهه,
						bmap, &new_blk);
		إذا (retval)
			أرجع retval;

		ext2fs_mark_بلاڪ_bitmap_range2(bmap,
			new_blk, fs->inode_بلاڪون_في_گروهه);
		إذا (flexbg_سائيز) {
			blk64_t انگ, blk;
			انگ = fs->inode_بلاڪون_في_گروهه;
			blk = new_blk;
			طالما (انگ) {
				صحيح gr = ext2fs_گروهه_جو_blk2(fs, blk);
				آخري_blk = ext2fs_گروهه_آخري_block2(fs, gr);
				blk64_t n = انگ;

				إذا (blk + انگ > آخري_blk)
					n = آخري_blk - blk + 1;

				ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(fs, gr,
					ext2fs_bg_free_بلاڪون_ڳڻپ(fs, gr) -
					n/EXT2_FS_CLUSTER_تناسب(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_بلاڪ_UNINIT);
				ext2fs_گروهه_desc_csum_سيٽ(fs, gr);
				ext2fs_free_بلاڪون_ڳڻپ_جوڙيو(fs->super, -n);
				blk += n;
				انگ -= n;
			}
		}
		ext2fs_inode_جدول_loc_سيٽ(fs, گروهه, new_blk);
	}
	ext2fs_گروهه_desc_csum_سيٽ(fs, گروهه);
	أرجع 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	بنية ext2fs_numeric_ترقي_struct ترقي;

	إذا (fs->ترقي_ops && fs->ترقي_ops->init)
		(fs->ترقي_ops->init)(fs, &ترقي, عدم,
					 fs->گروهه_desc_ڳڻپ);

	لكل (i = 0; i < fs->گروهه_desc_ڳڻپ; i++) {
		إذا (fs->ترقي_ops && fs->ترقي_ops->update)
			(fs->ترقي_ops->update)(fs, &ترقي, i);
		retval = ext2fs_allocate_گروهه_جدول(fs, i, fs->بلاڪ_map);
		إذا (retval)
			أرجع retval;
	}
	إذا (fs->ترقي_ops && fs->ترقي_ops->close)
		(fs->ترقي_ops->close)(fs, &ترقي, عدم);
	أرجع 0;
}

