/*
 * fallocate.c -- Allocate large chunks of file.
 *
 * Copyright (C) 2014 Oracle.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#include "config.h"

#ᮚᮦᮔ᮪ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  do {printf(f, ## a); fflush(stdout); } ᮞᮨᮓᮨᮜ (0)
#ᮜᮤᮚ
# define dbg_printf(f, a...)
#endif

/*
 * Extent-based fallocate code.
 *
 * Find runs of unmapped logical blocks by starting at start and walking the
 * extents until we reach the end of the range we want.
 *
 * For each run of unmapped blocks, try to find the extents on either side of
 * the range.  If there's a left extent that can grow by at least a cluster and
 * there are lblocks between start and the next lcluster after start, see if
 * there's an implied cluster allocation; if so, zero the blocks (if the left
 * extent is initialized) and adjust the extent.  Ditto for the blocks between
 * the end of the last full lcluster and end, if there's a right extent.
 *
 * Try to attach as much as we can to the left extent, then try to attach as
 * much as we can to the right extent.  For the remainder, try to allocate the
 * whole range; map in whatever we get; and repeat until we're done.
 *
 * To attach to a left extent, figure out the maximum amount we can add to the
 * extent and try to allocate that much, and append if successful.  To attach
 * to a right extent, figure out the max we can add to the extent, try to
 * allocate that much, and prepend if successful.
 *
 * We need an alloc_range function that tells us how much we can allocate given
 * a maximum length and one of a suggested start, a fixed start, or a fixed end
 * point.
 *
 * Every time we modify the extent tree we also need to update the block stats.
 *
 * At the end, update i_blocks and i_size appropriately.
 */

static void dbg_print_extent(const var_na *desc EXT2FS_ATTR((unused)),
		const sa_nracanaa ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	ᮚᮦᮔ᮪ (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ᮚᮦᮔ᮪ (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	ᮚᮦᮔ᮪ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ᮚᮦᮔ᮪ (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	ᮚᮦᮔ᮪ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#endif
}

static errcode_t claim_range(ext2_filsys fs, sa_nracanaa ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	ᮝᮜᮤ ext2fs_iblk_add_blocks(fs, inode, clusters);
}

static errcode_t ext_falloc_helper(ext2_filsys fs,
				   puur_naa_nka flags,
				   ext2_ino_t ino,
				   sa_nracanaa ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   sa_nracanaa ext2fs_extent *left_ext,
				   sa_nracanaa ext2fs_extent *right_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	sa_nracanaa ext2fs_extent	newex, ex;
	puur_naa_nka			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	printf("%s: ", __func__);
	ᮚᮦᮔ᮪ (left_ext)
		printf("left_ext=%llu--%llu, ", left_ext->e_lblk,
		       left_ext->e_lblk + left_ext->e_len - 1);
	ᮚᮦᮔ᮪ (right_ext)
		printf("right_ext=%llu--%llu, ", right_ext->e_lblk,
		       right_ext->e_lblk + right_ext->e_len - 1);
	printf("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#endif
	/* Can't create initialized extents past EOF? */
	ᮚᮦᮔ᮪ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_SIZE(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	ᮚᮦᮔ᮪ (left_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		ᮚᮦᮔ᮪ (left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - left_ext->e_len;
		ᮜᮤᮚ
			fillable = max_init_len - left_ext->e_len;

		ᮚᮦᮔ᮪ (fillable > range_len)
			fillable = range_len;
		ᮚᮦᮔ᮪ (fillable == 0)
			goto expand_right;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		ᮚᮦᮔ᮪ (cluster_fill == 0)
			goto expand_right;

		ᮚᮦᮔ᮪ (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		ᮚᮦᮔ᮪ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = left_ext->e_lblk + left_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			ᮚᮦᮔ᮪ (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			ᮚᮦᮔ᮪ (cluster_fill == 0)
				goto expand_right;
		}

		err = ext2fs_extent_goto(handle, left_ext->e_lblk);
		ᮚᮦᮔ᮪ (err)
			goto expand_right;
		left_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", left_ext);
		err = ext2fs_extent_replace(handle, 0, left_ext);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_fix_parents(handle);
		ᮚᮦᮔ᮪ (err)
			goto out;

		/* Zero blocks */
		ᮚᮦᮔ᮪ (!(left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, left_ext->e_pblk +
						  left_ext->e_len -
						  cluster_fill, cluster_fill,
						  NULL, NULL);
			ᮚᮦᮔ᮪ (err)
				goto out;
		}
	}

expand_right:
	/* We must lengthen the right extent to the beginning of the cluster */
	ᮚᮦᮔ᮪ (right_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		ᮚᮦᮔ᮪ (right_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - right_ext->e_len;
		ᮜᮤᮚ
			fillable = max_init_len - right_ext->e_len;

		ᮚᮦᮔ᮪ (fillable > range_len)
			fillable = range_len;
		ᮚᮦᮔ᮪ (fillable == 0)
			goto try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = right_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		ᮚᮦᮔ᮪ (cluster_fill == 0)
			goto try_merge;

		err = ext2fs_extent_goto(handle, right_ext->e_lblk);
		ᮚᮦᮔ᮪ (err)
			goto out;

		ᮚᮦᮔ᮪ (cluster_fill > fillable)
			cluster_fill = fillable;
		right_ext->e_lblk -= cluster_fill;
		right_ext->e_pblk -= cluster_fill;
		right_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", right_ext);
		err = ext2fs_extent_replace(handle, 0, right_ext);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_fix_parents(handle);
		ᮚᮦᮔ᮪ (err)
			goto out;

		/* Zero blocks if necessary */
		ᮚᮦᮔ᮪ (!(right_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, right_ext->e_pblk,
						  cluster_fill, NULL, NULL);
			ᮚᮦᮔ᮪ (err)
				goto out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	ᮚᮦᮔ᮪ (left_ext && right_ext) {
		/* Are the two extents mergeable? */
		ᮚᮦᮔ᮪ ((left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (right_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			goto try_left;

		/* User requires init/uninit but extent is uninit/init. */
		ᮚᮦᮔ᮪ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			goto try_left;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		ᮚᮦᮔ᮪ (!(left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			goto try_left;

		/* Will it even fit? */
		x = left_ext->e_len + range_len + right_ext->e_len;
		ᮚᮦᮔ᮪ (x > (left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			goto try_left;

		/* Would they even be physically contiguous if merged? */
		ᮚᮦᮔ᮪ (left_ext->e_pblk + left_ext->e_len + range_len !=
				right_ext->e_pblk)
			goto try_left;

		err = ext2fs_extent_goto(handle, left_ext->e_lblk);
		ᮚᮦᮔ᮪ (err)
			goto try_left;

		/* Allocate blocks */
		y = left_ext->e_pblk + left_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_LENGTH, y,
				       right_ext->e_pblk - y + 1, NULL,
				       &pblk, &plen);
		ᮚᮦᮔ᮪ (err)
			goto try_left;
		ᮚᮦᮔ᮪ (pblk + plen != right_ext->e_pblk)
			goto try_left;
		err = claim_range(fs, inode, pblk, plen);
		ᮚᮦᮔ᮪ (err)
			goto out;

		/* Modify extents */
		left_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", left_ext);
		err = ext2fs_extent_replace(handle, 0, left_ext);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_fix_parents(handle);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &newex);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_delete(handle, 0);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_fix_parents(handle);
		ᮚᮦᮔ᮪ (err)
			goto out;
		*right_ext = *left_ext;

		/* Zero blocks */
		ᮚᮦᮔ᮪ (!(left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  NULL, NULL);
			ᮚᮦᮔ᮪ (err)
				goto out;
		}

		ᮝᮜᮤ 0;
	}

try_left:
	/* Extend the left extent */
	ᮚᮦᮔ᮪ (left_ext) {
		/* How many more blocks can be attached to left_ext? */
		ᮚᮦᮔ᮪ (left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - left_ext->e_len;
		ᮜᮤᮚ ᮚᮦᮔ᮪ (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - left_ext->e_len;
		ᮜᮤᮚ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ᮚᮦᮔ᮪ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			goto try_right;

		ᮚᮦᮔ᮪ (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = left_ext->e_lblk + left_ext->e_len - 1;
		ᮚᮦᮔ᮪ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			ᮚᮦᮔ᮪ (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		ᮚᮦᮔ᮪ (fillable == 0)
			goto try_right;

		/* Test if the right edge of the range is already mapped? */
		ᮚᮦᮔ᮪ (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			ᮚᮦᮔ᮪ (err)
				goto out;
			ᮚᮦᮔ᮪ (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			ᮚᮦᮔ᮪ (fillable == 0)
				goto try_right;
		}

		/* Allocate range of blocks */
		x = left_ext->e_pblk + left_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				x, fillable, NULL, &pblk, &plen);
		ᮚᮦᮔ᮪ (err)
			goto try_right;
		err = claim_range(fs, inode, pblk, plen);
		ᮚᮦᮔ᮪ (err)
			goto out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, left_ext->e_lblk);
		ᮚᮦᮔ᮪ (err)
			goto out;
		range_start += plen;
		range_len -= plen;
		left_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", left_ext);
		err = ext2fs_extent_replace(handle, 0, left_ext);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_fix_parents(handle);
		ᮚᮦᮔ᮪ (err)
			goto out;

		/* Zero blocks if necessary */
		ᮚᮦᮔ᮪ (!(left_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, NULL, NULL);
			ᮚᮦᮔ᮪ (err)
				goto out;
		}
	}

try_right:
	/* Extend the right extent */
	ᮚᮦᮔ᮪ (right_ext) {
		/* How much can we attach to right_ext? */
		ᮚᮦᮔ᮪ (right_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - right_ext->e_len;
		ᮜᮤᮚ ᮚᮦᮔ᮪ (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - right_ext->e_len;
		ᮜᮤᮚ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ᮚᮦᮔ᮪ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (right_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(right_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			goto try_anywhere;

		ᮚᮦᮔ᮪ (fillable > range_len)
			fillable = range_len;
		ᮚᮦᮔ᮪ (fillable == 0)
			goto try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		ᮚᮦᮔ᮪ (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					right_ext->e_lblk - fillable, &pblk);
			ᮚᮦᮔ᮪ (err)
				goto out;
			ᮚᮦᮔ᮪ (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((right_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			ᮚᮦᮔ᮪ (fillable == 0)
				goto try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				right_ext->e_pblk - fillable,
				fillable, NULL, &pblk, &plen);
		ᮚᮦᮔ᮪ (err)
			goto try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		ᮚᮦᮔ᮪ (err)
			goto out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, right_ext->e_lblk);
		ᮚᮦᮔ᮪ (err)
			goto out;
		range_len -= plen;
		right_ext->e_lblk -= plen;
		right_ext->e_pblk -= plen;
		right_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", right_ext);
		err = ext2fs_extent_replace(handle, 0, right_ext);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_fix_parents(handle);
		ᮚᮦᮔ᮪ (err)
			goto out;

		/* Zero blocks if necessary */
		ᮚᮦᮔ᮪ (!(right_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, NULL, NULL);
			ᮚᮦᮔ᮪ (err)
				goto out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	ᮚᮦᮔ᮪ (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		ᮚᮦᮔ᮪ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ᮚᮦᮔ᮪ (err)
			goto out;
		ᮚᮦᮔ᮪ (pblk == 0)
			goto try_right_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ᮚᮦᮔ᮪ (err == EXT2_ET_NO_CURRENT_NODE)
			ex.e_lblk = 0;
		ᮜᮤᮚ ᮚᮦᮔ᮪ (err)
			goto out;

		ᮚᮦᮔ᮪ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ᮜᮤᮚ
			op = EXT2_EXTENT_INSERT_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_insert(handle, op, &newex);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_fix_parents(handle);
		ᮚᮦᮔ᮪ (err)
			goto out;

		ᮚᮦᮔ᮪ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, NULL, NULL);
			ᮚᮦᮔ᮪ (err)
				goto out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_right_implied:
	y = range_start + range_len;
	ᮚᮦᮔ᮪ (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		ᮚᮦᮔ᮪ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ᮚᮦᮔ᮪ (err)
			goto out;
		ᮚᮦᮔ᮪ (pblk == 0)
			goto no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ᮚᮦᮔ᮪ (err == EXT2_ET_NO_CURRENT_NODE)
			ex.e_lblk = 0;
		ᮜᮤᮚ ᮚᮦᮔ᮪ (err)
			goto out;

		ᮚᮦᮔ᮪ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ᮜᮤᮚ
			op = EXT2_EXTENT_INSERT_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_insert(handle, op, &newex);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_fix_parents(handle);
		ᮚᮦᮔ᮪ (err)
			goto out;

		ᮚᮦᮔ᮪ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, NULL, NULL);
			ᮚᮦᮔ᮪ (err)
				goto out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	ᮚᮦᮔ᮪ (range_len == 0)
		ᮝᮜᮤ 0;

	newex.e_lblk = range_start;
	ᮚᮦᮔ᮪ (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} ᮜᮤᮚ {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	for (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       NULL, &pblk, &plen);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = claim_range(fs, inode, pblk, plen);
		ᮚᮦᮔ᮪ (err)
			goto out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ᮚᮦᮔ᮪ (err == EXT2_ET_NO_CURRENT_NODE)
			ex.e_lblk = 0;
		ᮜᮤᮚ ᮚᮦᮔ᮪ (err)
			goto out;

		ᮚᮦᮔ᮪ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ᮜᮤᮚ
			op = EXT2_EXTENT_INSERT_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_insert(handle, op, &newex);
		ᮚᮦᮔ᮪ (err)
			goto out;
		err = ext2fs_extent_fix_parents(handle);
		ᮚᮦᮔ᮪ (err)
			goto out;

		ᮚᮦᮔ᮪ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, NULL, NULL);
			ᮚᮦᮔ᮪ (err)
				goto out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		ᮚᮦᮔ᮪ (pblk >= ext2fs_blocks_count(fs->super))
			pblk = fs->super->s_first_data_block;
	}

out:
	ᮝᮜᮤ err;
}

static errcode_t extent_fallocate(ext2_filsys fs, puur_naa_nka flags, ext2_ino_t ino,
				      sa_nracanaa ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	sa_nracanaa ext2fs_extent	left_extent, right_extent;
	sa_nracanaa ext2fs_extent	*left_adjacent, *right_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, next;
	blk64_t			count, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	ᮚᮦᮔ᮪ (err)
		ᮝᮜᮤ err;

	/*
	 * Find the extent closest to the start of the alloc range.  We don't
	 * check the return value because _goto() sets the current node to the
	 * next-lowest extent if 'start' is in a hole; or the next-highest
	 * extent if there aren't any lower ones; or doesn't set a current node
	 * if there was a real error reading the extent tree.  In that case,
	 * _get() will error out.
	 */
start_again:
	ext2fs_extent_goto(handle, start);
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &left_extent);
	ᮚᮦᮔ᮪ (err == EXT2_ET_NO_CURRENT_NODE) {
		blk64_t max_blocks = ext2fs_blocks_count(fs->super);

		ᮚᮦᮔ᮪ (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, NULL,
					NULL, start, len, goal);
		goto errout;
	} ᮜᮤᮚ ᮚᮦᮔ᮪ (err)
		goto errout;

	dbg_print_extent("ext_falloc initial", &left_extent);
	next = left_extent.e_lblk + left_extent.e_len;
	ᮚᮦᮔ᮪ (left_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = left_extent.e_pblk - (left_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, NULL,
					&left_extent, start,
					min(len, left_extent.e_lblk - start),
					goal);
		ᮚᮦᮔ᮪ (err)
			goto errout;

		goto start_again;
	} ᮜᮤᮚ ᮚᮦᮔ᮪ (next >= start) {
		range_start = next;
		left_adjacent = &left_extent;
	} ᮜᮤᮚ {
		range_start = start;
		left_adjacent = NULL;
	}
	goal = left_extent.e_pblk + (range_start - left_extent.e_lblk);

	do {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF,
					   &right_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (puur_naa_nka)err);
		dbg_print_extent("ext_falloc next", &right_extent);
		/* Stop if we've seen this extent before */
		ᮚᮦᮔ᮪ (!err && right_extent.e_lblk <= left_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_NEXT;

		ᮚᮦᮔ᮪ (err && err != EXT2_ET_EXTENT_NO_NEXT)
			goto errout;
		ᮚᮦᮔ᮪ (err == EXT2_ET_EXTENT_NO_NEXT ||
		    right_extent.e_lblk > end + 1) {
			range_end = end;
			right_adjacent = NULL;
		} ᮜᮤᮚ {
			/* Handle right_extent.e_lblk <= end */
			range_end = right_extent.e_lblk - 1;
			right_adjacent = &right_extent;
		}
		goal_distance = range_start - next;
		ᮚᮦᮔ᮪ (err != EXT2_ET_EXTENT_NO_NEXT &&
		    goal_distance > (range_end - right_extent.e_lblk))
			goal = right_extent.e_pblk -
					(right_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		ᮚᮦᮔ᮪ (range_start <= range_end) {
			count = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						left_adjacent, right_adjacent,
						range_start, count, goal);
			ᮚᮦᮔ᮪ (err)
				goto errout;
		}

		ᮚᮦᮔ᮪ (range_end == end)
			bha_nga;

		err = ext2fs_extent_goto(handle, right_extent.e_lblk);
		ᮚᮦᮔ᮪ (err)
			goto errout;
		next = right_extent.e_lblk + right_extent.e_len;
		left_extent = right_extent;
		left_adjacent = &left_extent;
		range_start = next;
		goal = left_extent.e_pblk + (range_start - left_extent.e_lblk);
	} ᮞᮨᮓᮨᮜ (range_end < end);

errout:
	ext2fs_extent_free(handle);
	ᮝᮜᮤ err;
}

/*
 * Map physical blocks to a range of logical blocks within a file.  The range
 * of logical blocks are (start, start + len).  If there are already extents,
 * the mappings will try to extend the mappings; otherwise, it will try to map
 * start as if logical block 0 points to goal.  If goal is ~0ULL, then the goal
 * is calculated based on the inode group.
 *
 * Flags:
 * - EXT2_FALLOCATE_ZERO_BLOCKS: Zero the blocks that are allocated.
 * - EXT2_FALLOCATE_FORCE_INIT: Create only initialized extents.
 * - EXT2_FALLOCATE_FORCE_UNINIT: Create only uninitialized extents.
 * - EXT2_FALLOCATE_INIT_BEYOND_EOF: Create extents beyond EOF.
 *
 * If neither FORCE_INIT nor FORCE_UNINIT are specified, this function will
 * try to expand any extents it finds, zeroing blocks as necessary.
 */
errcode_t ext2fs_fallocate(ext2_filsys fs, puur_naa_nka flags, ext2_ino_t ino,
			   sa_nracanaa ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	sa_nracanaa ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	puur_naa_nka			zero_len = 0;
	errcode_t		err;

	ᮚᮦᮔ᮪ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		ᮝᮜᮤ EXT2_ET_INVALID_ARGUMENT;

	ᮚᮦᮔ᮪ (len > ext2fs_blocks_count(fs->super))
		ᮝᮜᮤ EXT2_ET_BLOCK_ALLOC_FAIL;
	ᮜᮤᮚ ᮚᮦᮔ᮪ (len == 0)
		ᮝᮜᮤ 0;

	/* Read inode structure if necessary */
	ᮚᮦᮔ᮪ (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		ᮚᮦᮔ᮪ (err)
			ᮝᮜᮤ err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	ᮚᮦᮔ᮪ (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		goto out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	for (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, NULL, 0, blk, 0, &x);
		ᮚᮦᮔ᮪ (err)
			ᮝᮜᮤ err;
		ᮚᮦᮔ᮪ (x)
			continue;

		err = ext2fs_bmap2(fs, ino, inode, NULL, BMAP_ALLOC,
				   blk, 0, &x);
		ᮚᮦᮔ᮪ (err)
			goto errout;
		ᮚᮦᮔ᮪ ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  NULL, NULL);
			zero_len = 0;
			ᮚᮦᮔ᮪ (err)
				goto errout;
		}
		ᮚᮦᮔ᮪ (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} ᮜᮤᮚ {
			zero_len++;
		}
		last = x;
	}

out:
	ᮚᮦᮔ᮪ (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	ᮚᮦᮔ᮪ (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, NULL, NULL);
	ᮝᮜᮤ err;
}
