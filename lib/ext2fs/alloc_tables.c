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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
ਸਥਿਰ blk64_t flexbg_ਵਿੱਥ(ext2_filsys fs, dgrp_t ਸਮੂਹ, blk64_t ਸ਼ੁਰੂ_blk,
			     ext2fs_ਖੰਡ_bitmap bmap, ਪੂਰਨ rem_grp,
			     ਪੂਰਨ elem_ਮਾਪ)
{
	ਪੂਰਨ		flexbg, flexbg_ਮਾਪ, ਮਾਪ;
	blk64_t		ਆਖਰੀ_blk, ਪਹਿਲਾ_free = 0;
	dgrp_t	       	ਆਖਰੀ_grp;

	flexbg_ਮਾਪ = 1U << fs->super->s_log_groups_ਪ੍ਰਤੀ_flex;
	flexbg = ਸਮੂਹ / flexbg_ਮਾਪ;
	ਮਾਪ = rem_grp * elem_ਮਾਪ;

	ਜੇ (ਮਾਪ > (ਪੂਰਨ) (fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ / 4))
		ਮਾਪ = (ਪੂਰਨ) fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	ਜੇ (ਸ਼ੁਰੂ_blk && ਸ਼ੁਰੂ_blk < ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) &&
	    ext2fs_get_free_blocks2(fs, ਸ਼ੁਰੂ_blk, ਸ਼ੁਰੂ_blk + ਮਾਪ, elem_ਮਾਪ,
				    bmap, &ਪਹਿਲਾ_free) == 0)
		ਵਾਪਸ ਪਹਿਲਾ_free;

	ਸ਼ੁਰੂ_blk = ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, flexbg_ਮਾਪ * flexbg);
	ਆਖਰੀ_grp = ਸਮੂਹ | (flexbg_ਮਾਪ - 1);
	ਜੇ (ਆਖਰੀ_grp > fs->ਸਮੂਹ_desc_ਗਿਣਤੀ-1)
		ਆਖਰੀ_grp = fs->ਸਮੂਹ_desc_ਗਿਣਤੀ-1;
	ਆਖਰੀ_blk = ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, ਆਖਰੀ_grp);

	/* Find the first available block */
	ਜੇ (ext2fs_get_free_blocks2(fs, ਸ਼ੁਰੂ_blk, ਆਖਰੀ_blk, ਮਾਪ,
				    bmap, &ਪਹਿਲਾ_free) == 0)
		ਵਾਪਸ ਪਹਿਲਾ_free;

	ਜੇ (ext2fs_get_free_blocks2(fs, ਸ਼ੁਰੂ_blk, ਆਖਰੀ_blk, elem_ਮਾਪ,
				   bmap, &ਪਹਿਲਾ_free) == 0)
		ਵਾਪਸ ਪਹਿਲਾ_free;

	ਜੇ (ext2fs_get_free_blocks2(fs, 0, ਆਖਰੀ_blk, elem_ਮਾਪ, bmap,
				    &ਪਹਿਲਾ_free) == 0)
		ਵਾਪਸ ਪਹਿਲਾ_free;

	ਵਾਪਸ ਪਹਿਲਾ_free;
}

errcode_t ext2fs_allocate_ਸਮੂਹ_ਸਾਰਣੀ(ext2_filsys fs, dgrp_t ਸਮੂਹ,
				      ext2fs_ਖੰਡ_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		ਸਮੂਹ_blk, ਸ਼ੁਰੂ_blk, ਆਖਰੀ_blk, new_blk;
	dgrp_t		ਆਖਰੀ_grp = 0;
	ਪੂਰਨ		rem_grps = 0, flexbg_ਮਾਪ = 0, ਸਾਰਣੀ_ਵਿੱਥ = 0;

	ਸਮੂਹ_blk = ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, ਸਮੂਹ);
	ਆਖਰੀ_blk = ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, ਸਮੂਹ);

	ਜੇ (!bmap)
		bmap = fs->ਖੰਡ_map;

	ਜੇ (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_ਪ੍ਰਤੀ_flex) {
		flexbg_ਮਾਪ = 1U << fs->super->s_log_groups_ਪ੍ਰਤੀ_flex;
		ਆਖਰੀ_grp = ਸਮੂਹ | (flexbg_ਮਾਪ - 1);
		ਜੇ (ਆਖਰੀ_grp > fs->ਸਮੂਹ_desc_ਗਿਣਤੀ-1)
			ਆਖਰੀ_grp = fs->ਸਮੂਹ_desc_ਗਿਣਤੀ-1;
		rem_grps = ਆਖਰੀ_grp - ਸਮੂਹ + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	ਜੇ (fs->stride && !flexbg_ਮਾਪ) {
		retval = ext2fs_get_free_blocks2(fs, ਸਮੂਹ_blk, ਆਖਰੀ_blk,
						 1, bmap, &ਸ਼ੁਰੂ_blk);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਸ਼ੁਰੂ_blk += fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
		ਸ਼ੁਰੂ_blk += ((fs->stride * ਸਮੂਹ) %
			      (ਆਖਰੀ_blk - ਸ਼ੁਰੂ_blk + 1));
		ਜੇ (ਸ਼ੁਰੂ_blk >= ਆਖਰੀ_blk)
			ਸ਼ੁਰੂ_blk = ਸਮੂਹ_blk;
	} ਨਹੀਂਤਾਂ
		ਸ਼ੁਰੂ_blk = ਸਮੂਹ_blk;

	ਜੇ (flexbg_ਮਾਪ) {
		blk64_t prev_ਖੰਡ = 0;

		ਸਾਰਣੀ_ਵਿੱਥ = flexbg_ਮਾਪ;
		ਜੇ (ਸਮੂਹ % flexbg_ਮਾਪ)
			prev_ਖੰਡ = ext2fs_ਖੰਡ_bitmap_loc(fs, ਸਮੂਹ - 1) + 1;
		ਨਹੀਂਤਾਂ ਜੇ (ਆਖਰੀ_grp == fs->ਸਮੂਹ_desc_ਗਿਣਤੀ-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			ਸਾਰਣੀ_ਵਿੱਥ = ਆਖਰੀ_grp & (flexbg_ਮਾਪ - 1);
			ਜੇ (ਸਾਰਣੀ_ਵਿੱਥ == 0)
				ਸਾਰਣੀ_ਵਿੱਥ = flexbg_ਮਾਪ;
			ਨਹੀਂਤਾਂ
				ਸਾਰਣੀ_ਵਿੱਥ++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		ਸ਼ੁਰੂ_blk = flexbg_ਵਿੱਥ(fs, ਸਮੂਹ, prev_ਖੰਡ, bmap,
					  rem_grps, 1);
		ਆਖਰੀ_blk = ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, ਆਖਰੀ_grp);
	}

	ਜੇ (!ext2fs_ਖੰਡ_bitmap_loc(fs, ਸਮੂਹ)) {
		retval = ext2fs_get_free_blocks2(fs, ਸ਼ੁਰੂ_blk, ਆਖਰੀ_blk,
						 1, bmap, &new_blk);
		ਜੇ (retval == EXT2_ET_ਖੰਡ_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, ਸਮੂਹ_blk,
					ਆਖਰੀ_blk, 1, bmap, &new_blk);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ext2fs_mark_ਖੰਡ_bitmap2(bmap, new_blk);
		ext2fs_ਖੰਡ_bitmap_loc_ਸੈੱਟ(fs, ਸਮੂਹ, new_blk);
		ਜੇ (flexbg_ਮਾਪ) {
			dgrp_t gr = ext2fs_ਸਮੂਹ_ਦਾ_blk2(fs, new_blk);
			ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs, gr, ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, gr) - 1);
			ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_ਖੰਡ_UNINIT);
			ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, gr);
		}
	}

	ਜੇ (flexbg_ਮਾਪ) {
		blk64_t prev_ਖੰਡ = 0;
		ਜੇ (ਸਮੂਹ % flexbg_ਮਾਪ)
			prev_ਖੰਡ = ext2fs_inode_bitmap_loc(fs, ਸਮੂਹ - 1) + 1;
		ਨਹੀਂਤਾਂ
			prev_ਖੰਡ = ext2fs_ਖੰਡ_bitmap_loc(fs, ਸਮੂਹ) +
				ਸਾਰਣੀ_ਵਿੱਥ;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		ਸ਼ੁਰੂ_blk = flexbg_ਵਿੱਥ(fs, ਸਮੂਹ, prev_ਖੰਡ, bmap,
					  rem_grps, 1);
		ਆਖਰੀ_blk = ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, ਆਖਰੀ_grp);
	}

	ਜੇ (!ext2fs_inode_bitmap_loc(fs, ਸਮੂਹ)) {
		retval = ext2fs_get_free_blocks2(fs, ਸ਼ੁਰੂ_blk, ਆਖਰੀ_blk,
						 1, bmap, &new_blk);
		ਜੇ (retval == EXT2_ET_ਖੰਡ_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, ਸਮੂਹ_blk,
					 ਆਖਰੀ_blk, 1, bmap, &new_blk);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ext2fs_mark_ਖੰਡ_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_ਸੈੱਟ(fs, ਸਮੂਹ, new_blk);
		ਜੇ (flexbg_ਮਾਪ) {
			dgrp_t gr = ext2fs_ਸਮੂਹ_ਦਾ_blk2(fs, new_blk);
			ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs, gr, ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, gr) - 1);
			ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_ਖੰਡ_UNINIT);
			ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	ਜੇ (flexbg_ਮਾਪ) {
		blk64_t prev_ਖੰਡ = 0;

		ਜੇ (ਸਮੂਹ % flexbg_ਮਾਪ)
			prev_ਖੰਡ = ext2fs_inode_ਸਾਰਣੀ_loc(fs, ਸਮੂਹ - 1) +
				fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
		ਨਹੀਂਤਾਂ
			prev_ਖੰਡ = ext2fs_inode_bitmap_loc(fs, ਸਮੂਹ) +
				ਸਾਰਣੀ_ਵਿੱਥ;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		ਸਮੂਹ_blk = flexbg_ਵਿੱਥ(fs, ਸਮੂਹ, prev_ਖੰਡ, bmap,
					  rem_grps, fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ);
		ਆਖਰੀ_blk = ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, ਆਖਰੀ_grp);
	}

	ਜੇ (!ext2fs_inode_ਸਾਰਣੀ_loc(fs, ਸਮੂਹ)) {
		retval = ext2fs_get_free_blocks2(fs, ਸਮੂਹ_blk, ਆਖਰੀ_blk,
						fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ,
						bmap, &new_blk);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		ext2fs_mark_ਖੰਡ_bitmap_range2(bmap,
			new_blk, fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ);
		ਜੇ (flexbg_ਮਾਪ) {
			blk64_t ਅੰਕ, blk;
			ਅੰਕ = fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
			blk = new_blk;
			ਜਦੋਂ (ਅੰਕ) {
				ਪੂਰਨ gr = ext2fs_ਸਮੂਹ_ਦਾ_blk2(fs, blk);
				ਆਖਰੀ_blk = ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, gr);
				blk64_t n = ਅੰਕ;

				ਜੇ (blk + ਅੰਕ > ਆਖਰੀ_blk)
					n = ਆਖਰੀ_blk - blk + 1;

				ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs, gr,
					ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, gr) -
					n/EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_ਖੰਡ_UNINIT);
				ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, gr);
				ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(fs->super, -n);
				blk += n;
				ਅੰਕ -= n;
			}
		}
		ext2fs_inode_ਸਾਰਣੀ_loc_ਸੈੱਟ(fs, ਸਮੂਹ, new_blk);
	}
	ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, ਸਮੂਹ);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct ਤਰੱਕੀ;

	ਜੇ (fs->ਤਰੱਕੀ_ops && fs->ਤਰੱਕੀ_ops->init)
		(fs->ਤਰੱਕੀ_ops->init)(fs, &ਤਰੱਕੀ, ਨਿਰਾ,
					 fs->ਸਮੂਹ_desc_ਗਿਣਤੀ);

	ਲਈ (i = 0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		ਜੇ (fs->ਤਰੱਕੀ_ops && fs->ਤਰੱਕੀ_ops->update)
			(fs->ਤਰੱਕੀ_ops->update)(fs, &ਤਰੱਕੀ, i);
		retval = ext2fs_allocate_ਸਮੂਹ_ਸਾਰਣੀ(fs, i, fs->ਖੰਡ_map);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ਜੇ (fs->ਤਰੱਕੀ_ops && fs->ਤਰੱਕੀ_ops->close)
		(fs->ਤਰੱਕੀ_ops->close)(fs, &ਤਰੱਕੀ, ਨਿਰਾ);
	ਵਾਪਸ 0;
}

