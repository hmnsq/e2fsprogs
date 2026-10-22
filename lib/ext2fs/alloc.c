/*
 * alloc.c --- allocate new inodes, blocks for ext2fs
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ವ್ಯಾಖ್ಯಾನಿಸು min(a, b) ((a) < (b) ? (a) : (b))

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DEBUG

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
# define dbg_printf(f, a...)  ಮಾಡು {ಬರೆ(f, ## a); fflush(stdout); } ಆಗ (0)
#ಇಲ್ಲದಿದ್ದರೆ
# define dbg_printf(f, a...)
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Clear the uninit block bitmap flag if necessary
 */
ಶೂನ್ಯ ext2fs_clear_ಖಂಡ_uninit(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ಆದರೆ (ಗುಂಪು >= fs->ಗುಂಪು_desc_ಎಣಿಕೆ ||
	    !ext2fs_has_ಗುಂಪು_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, ಗುಂಪು, EXT2_BG_ಖಂಡ_UNINIT)))
		ಹಿಂತಿರುಗಿ;

	/* uninit block bitmaps are now initialized in read_bitmaps() */

	ext2fs_bg_flags_clear(fs, ಗುಂಪು, EXT2_BG_ಖಂಡ_UNINIT);
	ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, ಗುಂಪು);
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
}

/*
 * Check for uninit inode bitmaps and deal with them appropriately
 */
ಸ್ಥಿರ ಶೂನ್ಯ check_inode_uninit(ext2_filsys fs, ext2fs_inode_bitmap map,
			  dgrp_t ಗುಂಪು)
{
	ext2_ino_t	i, ino;

	ಆದರೆ (ಗುಂಪು >= fs->ಗುಂಪು_desc_ಎಣಿಕೆ ||
	    !ext2fs_has_ಗುಂಪು_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, ಗುಂಪು, EXT2_BG_INODE_UNINIT)))
		ಹಿಂತಿರುಗಿ;

	ino = (ಗುಂಪು * fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು) + 1;
	ಗಾಗಿ (i=0; i < fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು; i++, ino++)
		ext2fs_fast_unmark_inode_bitmap2(map, ino);

	ext2fs_bg_flags_clear(fs, ಗುಂಪು, EXT2_BG_INODE_UNINIT);
	/* Mimics what the kernel does */
	ext2fs_bg_flags_clear(fs, ಗುಂಪು, EXT2_BG_ಖಂಡ_UNINIT);
	ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, ಗುಂಪು);
	ext2fs_mark_ib_dirty(fs);
	ext2fs_mark_super_dirty(fs);
}

/*
 * Right now, just search forward from the parent directory's block
 * group to find the next free inode.
 *
 * Should have a special policy for directories.
 */
errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir,
			   ಪೂರ್ಣಾಂಕ ಬಹುಲಕ EXT2FS_ATTR((ಬಳಸದ)),
			   ext2fs_inode_bitmap map, ext2_ino_t *ret)
{
	ext2_ino_t	ಆರಂಭ_inode = 0;
	ext2_ino_t	i, ino_ಒಳ_ಗುಂಪು, upto, ಮೊದಲ_zero;
	errcode_t	retval;
	dgrp_t		ಗುಂಪು;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!map)
		map = fs->inode_map;
	ಆದರೆ (!map)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_INODE_BITMAP;

	ಆದರೆ (dir > 0) {
		ಗುಂಪು = (dir - 1) / EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super);
		ಆರಂಭ_inode = (ಗುಂಪು * EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super)) + 1;
	}
	ಆದರೆ (ಆರಂಭ_inode < EXT2_ಮೊದಲ_INODE(fs->super))
		ಆರಂಭ_inode = EXT2_ಮೊದಲ_INODE(fs->super);
	ಆದರೆ (ಆರಂಭ_inode > fs->super->s_inodes_ಎಣಿಕೆ)
		ಹಿಂತಿರುಗಿ EXT2_ET_INODE_ALLOC_FAIL;
	i = ಆರಂಭ_inode;
	ಮಾಡು {
		ino_ಒಳ_ಗುಂಪು = (i - 1) % EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super);
		ಗುಂಪು = (i - 1) / EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super);

		check_inode_uninit(fs, map, ಗುಂಪು);
		upto = i + (EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) - ino_ಒಳ_ಗುಂಪು);
		ಆದರೆ (i < ಆರಂಭ_inode && upto >= ಆರಂಭ_inode)
			upto = ಆರಂಭ_inode - 1;
		ಆದರೆ (upto > fs->super->s_inodes_ಎಣಿಕೆ)
			upto = fs->super->s_inodes_ಎಣಿಕೆ;

		retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_inode_bitmap2(map, i, upto,
							      &ಮೊದಲ_zero);
		ಆದರೆ (retval == 0) {
			i = ಮೊದಲ_zero;
			ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (retval != ENOENT)
			ಹಿಂತಿರುಗಿ EXT2_ET_INODE_ALLOC_FAIL;
		i = upto + 1;
		ಆದರೆ (i > fs->super->s_inodes_ಎಣಿಕೆ)
			i = EXT2_ಮೊದಲ_INODE(fs->super);
	} ಆಗ (i != ಆರಂಭ_inode);

	ಆದರೆ (ext2fs_test_inode_bitmap2(map, i))
		ಹಿಂತಿರುಗಿ EXT2_ET_INODE_ALLOC_FAIL;
	*ret = i;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Stupid algorithm --- we now just search forward starting from the
 * goal.  Should put in a smarter one someday....
 */
errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
			    ext2fs_ಖಂಡ_bitmap map, blk64_t *ret,
			    ರಚನೆ blk_alloc_ctx *ctx)
{
	errcode_t retval;
	blk64_t	b = 0;
	errcode_t (*gab)(ext2_filsys fs, blk64_t goal, blk64_t *ret);
	errcode_t (*gab2)(ext2_filsys, blk64_t, blk64_t *,
			  ರಚನೆ blk_alloc_ctx *);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!map) {
		/*
		 * In case there are clients out there whose get_alloc_block
		 * handlers call ext2fs_new_block2 with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		ಆದರೆ (fs->get_alloc_block2) {
			gab2 = fs->get_alloc_block2;
			fs->get_alloc_block2 = ಖಾಲಿ;
			retval = gab2(fs, goal, &b, ctx);
			fs->get_alloc_block2 = gab2;
			ಹೋಗು allocated;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fs->get_alloc_ಖಂಡ) {
			gab = fs->get_alloc_ಖಂಡ;
			fs->get_alloc_ಖಂಡ = ಖಾಲಿ;
			retval = gab(fs, goal, &b);
			fs->get_alloc_ಖಂಡ = gab;
			ಹೋಗು allocated;
		}
	}
	ಆದರೆ (!map)
		map = fs->ಖಂಡ_map;
	ಆದರೆ (!map)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಖಂಡ_BITMAP;
	ಆದರೆ (!goal || (goal >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)))
		goal = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
	goal &= ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);

	retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_ಖಂಡ_bitmap2(map,
			goal, ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) - 1, &b);
	ಆದರೆ ((retval == ENOENT) && (goal != fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ))
		retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_ಖಂಡ_bitmap2(map,
			fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ, goal - 1, &b);
allocated:
	ಆದರೆ (retval == ENOENT)
		ಹಿಂತಿರುಗಿ EXT2_ET_ಖಂಡ_ALLOC_FAIL;
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ext2fs_clear_ಖಂಡ_uninit(fs, ext2fs_ಗುಂಪು_ದ_blk2(fs, b));
	*ret = b;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
			   ext2fs_ಖಂಡ_bitmap map, blk64_t *ret)
{
	ಹಿಂತಿರುಗಿ ext2fs_new_block3(fs, goal, map, ret, ಖಾಲಿ);
}

errcode_t ext2fs_new_ಖಂಡ(ext2_filsys fs, blk_t goal,
			   ext2fs_ಖಂಡ_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_new_block2(fs, goal, map, &val);
	ಆದರೆ (!retval)
		*ret = (blk_t) val;
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * This function zeros out the allocated block, and updates all of the
 * appropriate filesystem records.
 */
errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal, ಅಕ್ಷರ *ಖಂಡ_buf,
			      blk64_t *ret, ರಚನೆ blk_alloc_ctx *ctx)
{
	errcode_t	retval;
	blk64_t		ಖಂಡ;

	ಆದರೆ (fs->get_alloc_block2) {
		retval = (fs->get_alloc_block2)(fs, goal, &ಖಂಡ, ctx);
		ಆದರೆ (retval)
			ಹೋಗು fail;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fs->get_alloc_ಖಂಡ) {
		retval = (fs->get_alloc_ಖಂಡ)(fs, goal, &ಖಂಡ);
		ಆದರೆ (retval)
			ಹೋಗು fail;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!fs->ಖಂಡ_map) {
			retval = ext2fs_read_ಖಂಡ_bitmap(fs);
			ಆದರೆ (retval)
				ಹೋಗು fail;
		}

		retval = ext2fs_new_block3(fs, goal, 0, &ಖಂಡ, ctx);
		ಆದರೆ (retval)
			ಹೋಗು fail;
	}

	ಆದರೆ (ಖಂಡ_buf) {
		memset(ಖಂಡ_buf, 0, fs->blocksize);
		retval = io_channel_write_blk64(fs->io, ಖಂಡ, 1, ಖಂಡ_buf);
	} ಇಲ್ಲದಿದ್ದರೆ
		retval = ext2fs_zero_blocks2(fs, ಖಂಡ, 1, ಖಾಲಿ, ಖಾಲಿ);
	ಆದರೆ (retval)
		ಹೋಗು fail;

	ext2fs_ಖಂಡ_alloc_stats2(fs, ಖಂಡ, +1);
	*ret = ಖಂಡ;

fail:
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
			     ಅಕ್ಷರ *ಖಂಡ_buf, blk64_t *ret)
{
	ಹಿಂತಿರುಗಿ ext2fs_alloc_block3(fs, goal, ಖಂಡ_buf, ret, ಖಾಲಿ);
}

errcode_t ext2fs_alloc_ಖಂಡ(ext2_filsys fs, blk_t goal,
			     ಅಕ್ಷರ *ಖಂಡ_buf, blk_t *ret)
{
	errcode_t retval;
	blk64_t ret64, goal64 = goal;
	retval = ext2fs_alloc_block3(fs, goal64, ಖಂಡ_buf, &ret64, ಖಾಲಿ);
	ಆದರೆ (!retval)
		*ret = (blk_t)ret64;
        ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t ಆರಂಭ, blk64_t ಮುಕ್ತಾಯ,
				 ಪೂರ್ಣಾಂಕ ಅಂಕೆ, ext2fs_ಖಂಡ_bitmap map, blk64_t *ret)
{
	blk64_t	b = ಆರಂಭ;
	ಪೂರ್ಣಾಂಕ	c_ಅನುಪಾತ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!map)
		map = fs->ಖಂಡ_map;
	ಆದರೆ (!map)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಖಂಡ_BITMAP;
	ಆದರೆ (!b)
		b = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
	ಆದರೆ (!ಮುಕ್ತಾಯ)
		ಮುಕ್ತಾಯ = ಆರಂಭ;
	ಆದರೆ (!ಅಂಕೆ)
		ಅಂಕೆ = 1;
	c_ಅನುಪಾತ = 1 << ext2fs_get_bitmap_granularity(map);
	b &= ~(c_ಅನುಪಾತ - 1);
	ಮುಕ್ತಾಯ &= ~(c_ಅನುಪಾತ -1);
	ಮಾಡು {
		ಆದರೆ (b + ಅಂಕೆ - 1 >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)) {
			ಆದರೆ (ಮುಕ್ತಾಯ > ಆರಂಭ)
				ಹಿಂತಿರುಗಿ EXT2_ET_ಖಂಡ_ALLOC_FAIL;
			b = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
		}
		ಆದರೆ (ext2fs_fast_test_ಖಂಡ_bitmap_range2(map, b, ಅಂಕೆ)) {
			*ret = b;
			ಹಿಂತಿರುಗಿ 0;
		}
		b += c_ಅನುಪಾತ;
	} ಆಗ (b != ಮುಕ್ತಾಯ);
	ಹಿಂತಿರುಗಿ EXT2_ET_ಖಂಡ_ALLOC_FAIL;
}

errcode_t ext2fs_get_free_ಖಂಡಗಳು(ext2_filsys fs, blk_t ಆರಂಭ, blk_t ಮುಕ್ತಾಯ,
				 ಪೂರ್ಣಾಂಕ ಅಂಕೆ, ext2fs_ಖಂಡ_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_get_free_blocks2(fs, ಆರಂಭ, ಮುಕ್ತಾಯ, ಅಂಕೆ, map, &val);
	ಆದರೆ(!retval)
		*ret = (blk_t) val;
	ಹಿಂತಿರುಗಿ retval;
}

ಶೂನ್ಯ ext2fs_ಗಣ_alloc_ಖಂಡ_callback(ext2_filsys fs,
				     errcode_t (*func)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret),
				     errcode_t (**old)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret))
{
	ಆದರೆ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (old)
		*old = fs->get_alloc_ಖಂಡ;

	fs->get_alloc_ಖಂಡ = func;
}

blk64_t ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       ರಚನೆ ext2_inode *inode, blk64_t lblk)
{
	dgrp_t			ಗುಂಪು;
	__u8			log_flex;
	ರಚನೆ ext2fs_extent	extent;
	ext2_extent_handle_t	handle = ಖಾಲಿ;
	errcode_t		err;

	/* Make sure data stored in inode->i_block is neither fast symlink nor
	 * inline data.
	 */
	ಆದರೆ (inode == ಖಾಲಿ || ext2fs_is_fast_symlink(inode) ||
	    inode->i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL)
		ಹೋಗು no_ಖಂಡಗಳು;

	ಆದರೆ (inode->i_flags & EXT4_EXTENTS_FL) {
		err = ext2fs_extent_open2(fs, ino, inode, &handle);
		ಆದರೆ (err)
			ಹೋಗು no_ಖಂಡಗಳು;
		err = ext2fs_extent_goto2(handle, 0, lblk);
		ಆದರೆ (err)
			ಹೋಗು no_ಖಂಡಗಳು;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
		ಆದರೆ (err)
			ಹೋಗು no_ಖಂಡಗಳು;
		ext2fs_extent_free(handle);
		ಹಿಂತಿರುಗಿ extent.e_pblk + (lblk - extent.e_lblk);
	}

	/* block mapped file; see if block zero is mapped? */
	ಆದರೆ (inode->i_ಖಂಡ[0])
		ಹಿಂತಿರುಗಿ inode->i_ಖಂಡ[0];

no_ಖಂಡಗಳು:
	ext2fs_extent_free(handle);
	log_flex = fs->super->s_log_groups_ಪ್ರತಿ_flex;
	ಗುಂಪು = ext2fs_ಗುಂಪು_ದ_ino(fs, ino);
	ಆದರೆ (log_flex)
		ಗುಂಪು = ಗುಂಪು & ~((1 << (log_flex)) - 1);
	ಹಿಂತಿರುಗಿ ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, ಗುಂಪು);
}

/*
 * Starting at _goal_, scan around the filesystem to find a run of free blocks
 * that's at least _len_ blocks long.  Possible flags:
 * - EXT2_NEWRANGE_EXACT_GOAL: The range of blocks must start at _goal_.
 * - EXT2_NEWRANGE_MIN_LENGTH: do not return a allocation shorter than _len_.
 * - EXT2_NEWRANGE_ZERO_BLOCKS: Zero blocks pblk to pblk+plen before returning.
 *
 * The starting block is returned in _pblk_ and the length is returned via
 * _plen_.  The blocks are not marked in the bitmap; the caller must mark
 * however much of the returned run they actually use, hopefully via
 * ext2fs_block_alloc_stats_range().
 *
 * This function can return a range that is longer than what was requested.
 */
errcode_t ext2fs_new_range(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			   blk64_t len, ext2fs_ಖಂಡ_bitmap map, blk64_t *pblk,
			   blk64_t *plen)
{
	errcode_t retval;
	blk64_t ಆರಂಭ, ಕೊನೆ, b;
	ಪೂರ್ಣಾಂಕ looped = 0;
	blk64_t max_ಖಂಡಗಳು = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super);
	errcode_t (*nrf)(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			 blk64_t len, blk64_t *pblk, blk64_t *plen);

	dbg_printf("%s: flags=0x%x goal=%llu len=%llu\n", __func__, flags,
		   goal, len);
	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	ಆದರೆ (len == 0 || (flags & ~EXT2_NEWRANGE_ALL_FLAGS))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (!map && fs->new_range) {
		/*
		 * In case there are clients out there whose new_range
		 * handlers call ext2fs_new_range with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		nrf = fs->new_range;
		fs->new_range = ಖಾಲಿ;
		retval = nrf(fs, flags, goal, len, pblk, plen);
		fs->new_range = nrf;
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಆರಂಭ = *pblk;
		ಕೊನೆ = *pblk + *plen;
		ಹೋಗು allocated;
	}
	ಆದರೆ (!map)
		map = fs->ಖಂಡ_map;
	ಆದರೆ (!map)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಖಂಡ_BITMAP;
	ಆದರೆ (!goal || goal >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))
		goal = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;

	ಆರಂಭ = goal;
	ಆಗ (!looped || ಆರಂಭ <= goal) {
		retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_ಖಂಡ_bitmap2(map, ಆರಂಭ,
							      max_ಖಂಡಗಳು - 1,
							      &ಆರಂಭ);
		ಆದರೆ (retval == ENOENT) {
			/*
			 * If there are no free blocks beyond the starting
			 * point, try scanning the whole filesystem, unless the
			 * user told us only to allocate from _goal_, or if
			 * we're already scanning the whole filesystem.
			 */
			ಆದರೆ (flags & EXT2_NEWRANGE_FIXED_GOAL ||
			    ಆರಂಭ == fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ)
				ಹೋಗು fail;
			ಆರಂಭ = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
			ಮುಂದುವರಿ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (retval)
			ಹೋಗು errout;

		ಆದರೆ (flags & EXT2_NEWRANGE_FIXED_GOAL && ಆರಂಭ != goal)
			ಹೋಗು fail;

		b = min(ಆರಂಭ + len - 1, max_ಖಂಡಗಳು - 1);
		retval =  ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_ಖಂಡ_bitmap2(map, ಆರಂಭ, b,
							      &ಕೊನೆ);
		ಆದರೆ (retval == ENOENT)
			ಕೊನೆ = b + 1;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (retval)
			ಹೋಗು errout;

		ಆದರೆ (!(flags & EXT2_NEWRANGE_MIN_ಉದ್ದಳತೆ) ||
		    (ಕೊನೆ - ಆರಂಭ) >= len) {
			/* Success! */
			*pblk = ಆರಂಭ;
			*plen = ಕೊನೆ - ಆರಂಭ;
			dbg_printf("%s: new_range goal=%llu--%llu "
				   "blk=%llu--%llu %llu\n",
				   __func__, goal, goal + len - 1,
				   *pblk, *pblk + *plen - 1, *plen);
allocated:
			ಗಾಗಿ (b = ಆರಂಭ; b < ಕೊನೆ;
			     b += fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು)
				ext2fs_clear_ಖಂಡ_uninit(fs,
						ext2fs_ಗುಂಪು_ದ_blk2(fs, b));
			ಹಿಂತಿರುಗಿ 0;
		}

		ಆದರೆ (flags & EXT2_NEWRANGE_FIXED_GOAL)
			ಹೋಗು fail;
		ಆರಂಭ = ಕೊನೆ;
		ಆದರೆ (ಆರಂಭ >= max_ಖಂಡಗಳು) {
			ಆದರೆ (looped)
				ಹೋಗು fail;
			looped = 1;
			ಆರಂಭ = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
		}
	}

fail:
	retval = EXT2_ET_ಖಂಡ_ALLOC_FAIL;
errout:
	ಹಿಂತಿರುಗಿ retval;
}

ಶೂನ್ಯ ext2fs_ಗಣ_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen))
{
	ಆದರೆ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (old)
		*old = fs->new_range;

	fs->new_range = func;
}

errcode_t ext2fs_alloc_range(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags, blk64_t goal,
			     blk_t len, blk64_t *ret)
{
	ಪೂರ್ಣಾಂಕ newr_flags = EXT2_NEWRANGE_MIN_ಉದ್ದಳತೆ;
	errcode_t retval;
	blk64_t plen;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	ಆದರೆ (len == 0 || (flags & ~EXT2_ALLOCRANGE_ALL_FLAGS))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (flags & EXT2_ALLOCRANGE_FIXED_GOAL)
		newr_flags |= EXT2_NEWRANGE_FIXED_GOAL;

	retval = ext2fs_new_range(fs, newr_flags, goal, len, ಖಾಲಿ, ret, &plen);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (plen < len)
		ಹಿಂತಿರುಗಿ EXT2_ET_ಖಂಡ_ALLOC_FAIL;

	ಆದರೆ (flags & EXT2_ALLOCRANGE_ZERO_ಖಂಡಗಳು) {
		retval = ext2fs_zero_blocks2(fs, *ret, len, ಖಾಲಿ, ಖಾಲಿ);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	ext2fs_ಖಂಡ_alloc_stats_range(fs, *ret, len, +1);
	ಹಿಂತಿರುಗಿ retval;
}
