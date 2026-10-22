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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <time.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

#నిర్వచించు min(a, b) ((a) < (b) ? (a) : (b))

#నిర్వచనం_రద్దు DEBUG

#నిర్వచితమైతే DEBUG
# define dbg_printf(f, a...)  చేయి {రాయి(f, ## a); fflush(stdout); } అయితే (0)
#లేకపోతే
# define dbg_printf(f, a...)
#అయితే_ముగింపు

/*
 * Clear the uninit block bitmap flag if necessary
 */
శూన్య ext2fs_clear_ఖండం_uninit(ext2_filsys fs, dgrp_t సమూహం)
{
	ఒకవేళ (సమూహం >= fs->సమూహం_desc_లెక్క ||
	    !ext2fs_has_సమూహం_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, సమూహం, EXT2_BG_ఖండం_UNINIT)))
		తిరిగి;

	/* uninit block bitmaps are now initialized in read_bitmaps() */

	ext2fs_bg_flags_clear(fs, సమూహం, EXT2_BG_ఖండం_UNINIT);
	ext2fs_సమూహం_desc_csum_సమితి(fs, సమూహం);
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
}

/*
 * Check for uninit inode bitmaps and deal with them appropriately
 */
స్థిర శూన్య check_inode_uninit(ext2_filsys fs, ext2fs_inode_bitmap map,
			  dgrp_t సమూహం)
{
	ext2_ino_t	i, ino;

	ఒకవేళ (సమూహం >= fs->సమూహం_desc_లెక్క ||
	    !ext2fs_has_సమూహం_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, సమూహం, EXT2_BG_INODE_UNINIT)))
		తిరిగి;

	ino = (సమూహం * fs->super->s_inodes_చొప్పున_సమూహం) + 1;
	కొరకు (i=0; i < fs->super->s_inodes_చొప్పున_సమూహం; i++, ino++)
		ext2fs_fast_unmark_inode_bitmap2(map, ino);

	ext2fs_bg_flags_clear(fs, సమూహం, EXT2_BG_INODE_UNINIT);
	/* Mimics what the kernel does */
	ext2fs_bg_flags_clear(fs, సమూహం, EXT2_BG_ఖండం_UNINIT);
	ext2fs_సమూహం_desc_csum_సమితి(fs, సమూహం);
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
			   పూర్ణాంకం బహుళకం EXT2FS_ATTR((వాడని)),
			   ext2fs_inode_bitmap map, ext2_ino_t *ret)
{
	ext2_ino_t	ప్రారంభం_inode = 0;
	ext2_ino_t	i, ino_లోపల_సమూహం, upto, మొదటి_zero;
	errcode_t	retval;
	dgrp_t		సమూహం;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!map)
		map = fs->inode_map;
	ఒకవేళ (!map)
		తిరిగి EXT2_ET_NO_INODE_BITMAP;

	ఒకవేళ (dir > 0) {
		సమూహం = (dir - 1) / EXT2_INODES_చొప్పున_సమూహం(fs->super);
		ప్రారంభం_inode = (సమూహం * EXT2_INODES_చొప్పున_సమూహం(fs->super)) + 1;
	}
	ఒకవేళ (ప్రారంభం_inode < EXT2_మొదటి_INODE(fs->super))
		ప్రారంభం_inode = EXT2_మొదటి_INODE(fs->super);
	ఒకవేళ (ప్రారంభం_inode > fs->super->s_inodes_లెక్క)
		తిరిగి EXT2_ET_INODE_ALLOC_FAIL;
	i = ప్రారంభం_inode;
	చేయి {
		ino_లోపల_సమూహం = (i - 1) % EXT2_INODES_చొప్పున_సమూహం(fs->super);
		సమూహం = (i - 1) / EXT2_INODES_చొప్పున_సమూహం(fs->super);

		check_inode_uninit(fs, map, సమూహం);
		upto = i + (EXT2_INODES_చొప్పున_సమూహం(fs->super) - ino_లోపల_సమూహం);
		ఒకవేళ (i < ప్రారంభం_inode && upto >= ప్రారంభం_inode)
			upto = ప్రారంభం_inode - 1;
		ఒకవేళ (upto > fs->super->s_inodes_లెక్క)
			upto = fs->super->s_inodes_లెక్క;

		retval = ext2fs_కనుగొను_మొదటి_zero_inode_bitmap2(map, i, upto,
							      &మొదటి_zero);
		ఒకవేళ (retval == 0) {
			i = మొదటి_zero;
			ఆపు;
		}
		ఒకవేళ (retval != ENOENT)
			తిరిగి EXT2_ET_INODE_ALLOC_FAIL;
		i = upto + 1;
		ఒకవేళ (i > fs->super->s_inodes_లెక్క)
			i = EXT2_మొదటి_INODE(fs->super);
	} అయితే (i != ప్రారంభం_inode);

	ఒకవేళ (ext2fs_test_inode_bitmap2(map, i))
		తిరిగి EXT2_ET_INODE_ALLOC_FAIL;
	*ret = i;
	తిరిగి 0;
}

/*
 * Stupid algorithm --- we now just search forward starting from the
 * goal.  Should put in a smarter one someday....
 */
errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
			    ext2fs_ఖండం_bitmap map, blk64_t *ret,
			    నిర్మాణం blk_alloc_ctx *ctx)
{
	errcode_t retval;
	blk64_t	b = 0;
	errcode_t (*gab)(ext2_filsys fs, blk64_t goal, blk64_t *ret);
	errcode_t (*gab2)(ext2_filsys, blk64_t, blk64_t *,
			  నిర్మాణం blk_alloc_ctx *);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!map) {
		/*
		 * In case there are clients out there whose get_alloc_block
		 * handlers call ext2fs_new_block2 with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		ఒకవేళ (fs->get_alloc_block2) {
			gab2 = fs->get_alloc_block2;
			fs->get_alloc_block2 = ఖాళీ;
			retval = gab2(fs, goal, &b, ctx);
			fs->get_alloc_block2 = gab2;
			వెళ్ళు allocated;
		} లేకపోతే ఒకవేళ (fs->get_alloc_ఖండం) {
			gab = fs->get_alloc_ఖండం;
			fs->get_alloc_ఖండం = ఖాళీ;
			retval = gab(fs, goal, &b);
			fs->get_alloc_ఖండం = gab;
			వెళ్ళు allocated;
		}
	}
	ఒకవేళ (!map)
		map = fs->ఖండం_map;
	ఒకవేళ (!map)
		తిరిగి EXT2_ET_NO_ఖండం_BITMAP;
	ఒకవేళ (!goal || (goal >= ext2fs_ఖండాలు_లెక్క(fs->super)))
		goal = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
	goal &= ~EXT2_FS_CLUSTER_ముసుగు(fs);

	retval = ext2fs_కనుగొను_మొదటి_zero_ఖండం_bitmap2(map,
			goal, ext2fs_ఖండాలు_లెక్క(fs->super) - 1, &b);
	ఒకవేళ ((retval == ENOENT) && (goal != fs->super->s_మొదటి_దత్తాంశం_ఖండం))
		retval = ext2fs_కనుగొను_మొదటి_zero_ఖండం_bitmap2(map,
			fs->super->s_మొదటి_దత్తాంశం_ఖండం, goal - 1, &b);
allocated:
	ఒకవేళ (retval == ENOENT)
		తిరిగి EXT2_ET_ఖండం_ALLOC_FAIL;
	ఒకవేళ (retval)
		తిరిగి retval;

	ext2fs_clear_ఖండం_uninit(fs, ext2fs_సమూహం_యొక్క_blk2(fs, b));
	*ret = b;
	తిరిగి 0;
}

errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
			   ext2fs_ఖండం_bitmap map, blk64_t *ret)
{
	తిరిగి ext2fs_new_block3(fs, goal, map, ret, ఖాళీ);
}

errcode_t ext2fs_new_ఖండం(ext2_filsys fs, blk_t goal,
			   ext2fs_ఖండం_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_new_block2(fs, goal, map, &val);
	ఒకవేళ (!retval)
		*ret = (blk_t) val;
	తిరిగి retval;
}

/*
 * This function zeros out the allocated block, and updates all of the
 * appropriate filesystem records.
 */
errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal, అక్షరం *ఖండం_buf,
			      blk64_t *ret, నిర్మాణం blk_alloc_ctx *ctx)
{
	errcode_t	retval;
	blk64_t		ఖండం;

	ఒకవేళ (fs->get_alloc_block2) {
		retval = (fs->get_alloc_block2)(fs, goal, &ఖండం, ctx);
		ఒకవేళ (retval)
			వెళ్ళు fail;
	} లేకపోతే ఒకవేళ (fs->get_alloc_ఖండం) {
		retval = (fs->get_alloc_ఖండం)(fs, goal, &ఖండం);
		ఒకవేళ (retval)
			వెళ్ళు fail;
	} లేకపోతే {
		ఒకవేళ (!fs->ఖండం_map) {
			retval = ext2fs_read_ఖండం_bitmap(fs);
			ఒకవేళ (retval)
				వెళ్ళు fail;
		}

		retval = ext2fs_new_block3(fs, goal, 0, &ఖండం, ctx);
		ఒకవేళ (retval)
			వెళ్ళు fail;
	}

	ఒకవేళ (ఖండం_buf) {
		memset(ఖండం_buf, 0, fs->blocksize);
		retval = io_channel_write_blk64(fs->io, ఖండం, 1, ఖండం_buf);
	} లేకపోతే
		retval = ext2fs_zero_blocks2(fs, ఖండం, 1, ఖాళీ, ఖాళీ);
	ఒకవేళ (retval)
		వెళ్ళు fail;

	ext2fs_ఖండం_alloc_stats2(fs, ఖండం, +1);
	*ret = ఖండం;

fail:
	తిరిగి retval;
}

errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
			     అక్షరం *ఖండం_buf, blk64_t *ret)
{
	తిరిగి ext2fs_alloc_block3(fs, goal, ఖండం_buf, ret, ఖాళీ);
}

errcode_t ext2fs_alloc_ఖండం(ext2_filsys fs, blk_t goal,
			     అక్షరం *ఖండం_buf, blk_t *ret)
{
	errcode_t retval;
	blk64_t ret64, goal64 = goal;
	retval = ext2fs_alloc_block3(fs, goal64, ఖండం_buf, &ret64, ఖాళీ);
	ఒకవేళ (!retval)
		*ret = (blk_t)ret64;
        తిరిగి retval;
}

errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t ప్రారంభం, blk64_t ముగింపు,
				 పూర్ణాంకం అంకె, ext2fs_ఖండం_bitmap map, blk64_t *ret)
{
	blk64_t	b = ప్రారంభం;
	పూర్ణాంకం	c_నిష్పత్తి;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!map)
		map = fs->ఖండం_map;
	ఒకవేళ (!map)
		తిరిగి EXT2_ET_NO_ఖండం_BITMAP;
	ఒకవేళ (!b)
		b = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
	ఒకవేళ (!ముగింపు)
		ముగింపు = ప్రారంభం;
	ఒకవేళ (!అంకె)
		అంకె = 1;
	c_నిష్పత్తి = 1 << ext2fs_get_bitmap_granularity(map);
	b &= ~(c_నిష్పత్తి - 1);
	ముగింపు &= ~(c_నిష్పత్తి -1);
	చేయి {
		ఒకవేళ (b + అంకె - 1 >= ext2fs_ఖండాలు_లెక్క(fs->super)) {
			ఒకవేళ (ముగింపు > ప్రారంభం)
				తిరిగి EXT2_ET_ఖండం_ALLOC_FAIL;
			b = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
		}
		ఒకవేళ (ext2fs_fast_test_ఖండం_bitmap_range2(map, b, అంకె)) {
			*ret = b;
			తిరిగి 0;
		}
		b += c_నిష్పత్తి;
	} అయితే (b != ముగింపు);
	తిరిగి EXT2_ET_ఖండం_ALLOC_FAIL;
}

errcode_t ext2fs_get_free_ఖండాలు(ext2_filsys fs, blk_t ప్రారంభం, blk_t ముగింపు,
				 పూర్ణాంకం అంకె, ext2fs_ఖండం_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_get_free_blocks2(fs, ప్రారంభం, ముగింపు, అంకె, map, &val);
	ఒకవేళ(!retval)
		*ret = (blk_t) val;
	తిరిగి retval;
}

శూన్య ext2fs_సమితి_alloc_ఖండం_callback(ext2_filsys fs,
				     errcode_t (*func)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret),
				     errcode_t (**old)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret))
{
	ఒకవేళ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		తిరిగి;

	ఒకవేళ (old)
		*old = fs->get_alloc_ఖండం;

	fs->get_alloc_ఖండం = func;
}

blk64_t ext2fs_కనుగొను_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       నిర్మాణం ext2_inode *inode, blk64_t lblk)
{
	dgrp_t			సమూహం;
	__u8			log_flex;
	నిర్మాణం ext2fs_extent	extent;
	ext2_extent_handle_t	handle = ఖాళీ;
	errcode_t		err;

	/* Make sure data stored in inode->i_block is neither fast symlink nor
	 * inline data.
	 */
	ఒకవేళ (inode == ఖాళీ || ext2fs_is_fast_symlink(inode) ||
	    inode->i_flags & EXT4_INLINE_దత్తాంశం_FL)
		వెళ్ళు no_ఖండాలు;

	ఒకవేళ (inode->i_flags & EXT4_EXTENTS_FL) {
		err = ext2fs_extent_open2(fs, ino, inode, &handle);
		ఒకవేళ (err)
			వెళ్ళు no_ఖండాలు;
		err = ext2fs_extent_goto2(handle, 0, lblk);
		ఒకవేళ (err)
			వెళ్ళు no_ఖండాలు;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
		ఒకవేళ (err)
			వెళ్ళు no_ఖండాలు;
		ext2fs_extent_free(handle);
		తిరిగి extent.e_pblk + (lblk - extent.e_lblk);
	}

	/* block mapped file; see if block zero is mapped? */
	ఒకవేళ (inode->i_ఖండం[0])
		తిరిగి inode->i_ఖండం[0];

no_ఖండాలు:
	ext2fs_extent_free(handle);
	log_flex = fs->super->s_log_groups_చొప్పున_flex;
	సమూహం = ext2fs_సమూహం_యొక్క_ino(fs, ino);
	ఒకవేళ (log_flex)
		సమూహం = సమూహం & ~((1 << (log_flex)) - 1);
	తిరిగి ext2fs_సమూహం_మొదటి_block2(fs, సమూహం);
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
errcode_t ext2fs_new_range(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			   blk64_t len, ext2fs_ఖండం_bitmap map, blk64_t *pblk,
			   blk64_t *plen)
{
	errcode_t retval;
	blk64_t ప్రారంభం, చివర, b;
	పూర్ణాంకం looped = 0;
	blk64_t max_ఖండాలు = ext2fs_ఖండాలు_లెక్క(fs->super);
	errcode_t (*nrf)(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			 blk64_t len, blk64_t *pblk, blk64_t *plen);

	dbg_printf("%s: flags=0x%x goal=%llu len=%llu\n", __func__, flags,
		   goal, len);
	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	ఒకవేళ (len == 0 || (flags & ~EXT2_NEWRANGE_ALL_FLAGS))
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (!map && fs->new_range) {
		/*
		 * In case there are clients out there whose new_range
		 * handlers call ext2fs_new_range with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		nrf = fs->new_range;
		fs->new_range = ఖాళీ;
		retval = nrf(fs, flags, goal, len, pblk, plen);
		fs->new_range = nrf;
		ఒకవేళ (retval)
			తిరిగి retval;
		ప్రారంభం = *pblk;
		చివర = *pblk + *plen;
		వెళ్ళు allocated;
	}
	ఒకవేళ (!map)
		map = fs->ఖండం_map;
	ఒకవేళ (!map)
		తిరిగి EXT2_ET_NO_ఖండం_BITMAP;
	ఒకవేళ (!goal || goal >= ext2fs_ఖండాలు_లెక్క(fs->super))
		goal = fs->super->s_మొదటి_దత్తాంశం_ఖండం;

	ప్రారంభం = goal;
	అయితే (!looped || ప్రారంభం <= goal) {
		retval = ext2fs_కనుగొను_మొదటి_zero_ఖండం_bitmap2(map, ప్రారంభం,
							      max_ఖండాలు - 1,
							      &ప్రారంభం);
		ఒకవేళ (retval == ENOENT) {
			/*
			 * If there are no free blocks beyond the starting
			 * point, try scanning the whole filesystem, unless the
			 * user told us only to allocate from _goal_, or if
			 * we're already scanning the whole filesystem.
			 */
			ఒకవేళ (flags & EXT2_NEWRANGE_FIXED_GOAL ||
			    ప్రారంభం == fs->super->s_మొదటి_దత్తాంశం_ఖండం)
				వెళ్ళు fail;
			ప్రారంభం = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
			కొనసాగు;
		} లేకపోతే ఒకవేళ (retval)
			వెళ్ళు errout;

		ఒకవేళ (flags & EXT2_NEWRANGE_FIXED_GOAL && ప్రారంభం != goal)
			వెళ్ళు fail;

		b = min(ప్రారంభం + len - 1, max_ఖండాలు - 1);
		retval =  ext2fs_కనుగొను_మొదటి_సమితి_ఖండం_bitmap2(map, ప్రారంభం, b,
							      &చివర);
		ఒకవేళ (retval == ENOENT)
			చివర = b + 1;
		లేకపోతే ఒకవేళ (retval)
			వెళ్ళు errout;

		ఒకవేళ (!(flags & EXT2_NEWRANGE_MIN_నిడివి) ||
		    (చివర - ప్రారంభం) >= len) {
			/* Success! */
			*pblk = ప్రారంభం;
			*plen = చివర - ప్రారంభం;
			dbg_printf("%s: new_range goal=%llu--%llu "
				   "blk=%llu--%llu %llu\n",
				   __func__, goal, goal + len - 1,
				   *pblk, *pblk + *plen - 1, *plen);
allocated:
			కొరకు (b = ప్రారంభం; b < చివర;
			     b += fs->super->s_ఖండాలు_చొప్పున_సమూహం)
				ext2fs_clear_ఖండం_uninit(fs,
						ext2fs_సమూహం_యొక్క_blk2(fs, b));
			తిరిగి 0;
		}

		ఒకవేళ (flags & EXT2_NEWRANGE_FIXED_GOAL)
			వెళ్ళు fail;
		ప్రారంభం = చివర;
		ఒకవేళ (ప్రారంభం >= max_ఖండాలు) {
			ఒకవేళ (looped)
				వెళ్ళు fail;
			looped = 1;
			ప్రారంభం = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
		}
	}

fail:
	retval = EXT2_ET_ఖండం_ALLOC_FAIL;
errout:
	తిరిగి retval;
}

శూన్య ext2fs_సమితి_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen))
{
	ఒకవేళ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		తిరిగి;

	ఒకవేళ (old)
		*old = fs->new_range;

	fs->new_range = func;
}

errcode_t ext2fs_alloc_range(ext2_filsys fs, పూర్ణాంకం flags, blk64_t goal,
			     blk_t len, blk64_t *ret)
{
	పూర్ణాంకం newr_flags = EXT2_NEWRANGE_MIN_నిడివి;
	errcode_t retval;
	blk64_t plen;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	ఒకవేళ (len == 0 || (flags & ~EXT2_ALLOCRANGE_ALL_FLAGS))
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (flags & EXT2_ALLOCRANGE_FIXED_GOAL)
		newr_flags |= EXT2_NEWRANGE_FIXED_GOAL;

	retval = ext2fs_new_range(fs, newr_flags, goal, len, ఖాళీ, ret, &plen);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (plen < len)
		తిరిగి EXT2_ET_ఖండం_ALLOC_FAIL;

	ఒకవేళ (flags & EXT2_ALLOCRANGE_ZERO_ఖండాలు) {
		retval = ext2fs_zero_blocks2(fs, *ret, len, ఖాళీ, ఖాళీ);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	ext2fs_ఖండం_alloc_stats_range(fs, *ret, len, +1);
	తిరిగి retval;
}
