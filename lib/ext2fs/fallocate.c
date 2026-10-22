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

#অন্তর্ভাব "config.h"

#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  করো {printf(f, ## a); fflush(stdout); } যখন (0)
#অন্যথা
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

স্থির শূন্য dbg_print_extent(ধ্রুবক অক্ষর *desc EXT2FS_ATTR((unused)),
		ধ্রুবক কাঠামো ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	যদি (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	যদি (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	যদি (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	যদি (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	যদি (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#endif
}

স্থির errcode_t claim_range(ext2_filsys fs, কাঠামো ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	ফেরত ext2fs_iblk_add_blocks(fs, inode, clusters);
}

স্থির errcode_t ext_falloc_helper(ext2_filsys fs,
				   পূর্ণসংখ্যা flags,
				   ext2_ino_t ino,
				   কাঠামো ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   কাঠামো ext2fs_extent *বাম_ext,
				   কাঠামো ext2fs_extent *ডান_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	কাঠামো ext2fs_extent	newex, ex;
	পূর্ণসংখ্যা			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	printf("%s: ", __func__);
	যদি (বাম_ext)
		printf("left_ext=%llu--%llu, ", বাম_ext->e_lblk,
		       বাম_ext->e_lblk + বাম_ext->e_len - 1);
	যদি (ডান_ext)
		printf("right_ext=%llu--%llu, ", ডান_ext->e_lblk,
		       ডান_ext->e_lblk + ডান_ext->e_len - 1);
	printf("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#endif
	/* Can't create initialized extents past EOF? */
	যদি (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_আকার(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	যদি (বাম_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		যদি (বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - বাম_ext->e_len;
		অন্যথা
			fillable = max_init_len - বাম_ext->e_len;

		যদি (fillable > range_len)
			fillable = range_len;
		যদি (fillable == 0)
			যাও expand_ডান;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		যদি (cluster_fill == 0)
			যাও expand_ডান;

		যদি (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		যদি (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = বাম_ext->e_lblk + বাম_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			যদি (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			যদি (cluster_fill == 0)
				যাও expand_ডান;
		}

		err = ext2fs_extent_goto(handle, বাম_ext->e_lblk);
		যদি (err)
			যাও expand_ডান;
		বাম_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", বাম_ext);
		err = ext2fs_extent_replace(handle, 0, বাম_ext);
		যদি (err)
			যাও out;
		err = ext2fs_extent_fix_parents(handle);
		যদি (err)
			যাও out;

		/* Zero blocks */
		যদি (!(বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, বাম_ext->e_pblk +
						  বাম_ext->e_len -
						  cluster_fill, cluster_fill,
						  ফাঁকা, ফাঁকা);
			যদি (err)
				যাও out;
		}
	}

expand_ডান:
	/* We must lengthen the right extent to the beginning of the cluster */
	যদি (ডান_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		যদি (ডান_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ডান_ext->e_len;
		অন্যথা
			fillable = max_init_len - ডান_ext->e_len;

		যদি (fillable > range_len)
			fillable = range_len;
		যদি (fillable == 0)
			যাও try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = ডান_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		যদি (cluster_fill == 0)
			যাও try_merge;

		err = ext2fs_extent_goto(handle, ডান_ext->e_lblk);
		যদি (err)
			যাও out;

		যদি (cluster_fill > fillable)
			cluster_fill = fillable;
		ডান_ext->e_lblk -= cluster_fill;
		ডান_ext->e_pblk -= cluster_fill;
		ডান_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", ডান_ext);
		err = ext2fs_extent_replace(handle, 0, ডান_ext);
		যদি (err)
			যাও out;
		err = ext2fs_extent_fix_parents(handle);
		যদি (err)
			যাও out;

		/* Zero blocks if necessary */
		যদি (!(ডান_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ডান_ext->e_pblk,
						  cluster_fill, ফাঁকা, ফাঁকা);
			যদি (err)
				যাও out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	যদি (বাম_ext && ডান_ext) {
		/* Are the two extents mergeable? */
		যদি ((বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (ডান_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			যাও try_বাম;

		/* User requires init/uninit but extent is uninit/init. */
		যদি (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			যাও try_বাম;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		যদি (!(বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			যাও try_বাম;

		/* Will it even fit? */
		x = বাম_ext->e_len + range_len + ডান_ext->e_len;
		যদি (x > (বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			যাও try_বাম;

		/* Would they even be physically contiguous if merged? */
		যদি (বাম_ext->e_pblk + বাম_ext->e_len + range_len !=
				ডান_ext->e_pblk)
			যাও try_বাম;

		err = ext2fs_extent_goto(handle, বাম_ext->e_lblk);
		যদি (err)
			যাও try_বাম;

		/* Allocate blocks */
		y = বাম_ext->e_pblk + বাম_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_দৈর্ঘ্য, y,
				       ডান_ext->e_pblk - y + 1, ফাঁকা,
				       &pblk, &plen);
		যদি (err)
			যাও try_বাম;
		যদি (pblk + plen != ডান_ext->e_pblk)
			যাও try_বাম;
		err = claim_range(fs, inode, pblk, plen);
		যদি (err)
			যাও out;

		/* Modify extents */
		বাম_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", বাম_ext);
		err = ext2fs_extent_replace(handle, 0, বাম_ext);
		যদি (err)
			যাও out;
		err = ext2fs_extent_fix_parents(handle);
		যদি (err)
			যাও out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_পরবর্তী_LEAF, &newex);
		যদি (err)
			যাও out;
		err = ext2fs_extent_delete(handle, 0);
		যদি (err)
			যাও out;
		err = ext2fs_extent_fix_parents(handle);
		যদি (err)
			যাও out;
		*ডান_ext = *বাম_ext;

		/* Zero blocks */
		যদি (!(বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  ফাঁকা, ফাঁকা);
			যদি (err)
				যাও out;
		}

		ফেরত 0;
	}

try_বাম:
	/* Extend the left extent */
	যদি (বাম_ext) {
		/* How many more blocks can be attached to left_ext? */
		যদি (বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - বাম_ext->e_len;
		অন্যথা যদি (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - বাম_ext->e_len;
		অন্যথা
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		যদি (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			যাও try_ডান;

		যদি (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = বাম_ext->e_lblk + বাম_ext->e_len - 1;
		যদি (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			যদি (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		যদি (fillable == 0)
			যাও try_ডান;

		/* Test if the right edge of the range is already mapped? */
		যদি (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			যদি (err)
				যাও out;
			যদি (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			যদি (fillable == 0)
				যাও try_ডান;
		}

		/* Allocate range of blocks */
		x = বাম_ext->e_pblk + বাম_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_দৈর্ঘ্য,
				x, fillable, ফাঁকা, &pblk, &plen);
		যদি (err)
			যাও try_ডান;
		err = claim_range(fs, inode, pblk, plen);
		যদি (err)
			যাও out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, বাম_ext->e_lblk);
		যদি (err)
			যাও out;
		range_start += plen;
		range_len -= plen;
		বাম_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", বাম_ext);
		err = ext2fs_extent_replace(handle, 0, বাম_ext);
		যদি (err)
			যাও out;
		err = ext2fs_extent_fix_parents(handle);
		যদি (err)
			যাও out;

		/* Zero blocks if necessary */
		যদি (!(বাম_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ফাঁকা, ফাঁকা);
			যদি (err)
				যাও out;
		}
	}

try_ডান:
	/* Extend the right extent */
	যদি (ডান_ext) {
		/* How much can we attach to right_ext? */
		যদি (ডান_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ডান_ext->e_len;
		অন্যথা যদি (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - ডান_ext->e_len;
		অন্যথা
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		যদি (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ডান_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ডান_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			যাও try_anywhere;

		যদি (fillable > range_len)
			fillable = range_len;
		যদি (fillable == 0)
			যাও try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		যদি (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					ডান_ext->e_lblk - fillable, &pblk);
			যদি (err)
				যাও out;
			যদি (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((ডান_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			যদি (fillable == 0)
				যাও try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_দৈর্ঘ্য,
				ডান_ext->e_pblk - fillable,
				fillable, ফাঁকা, &pblk, &plen);
		যদি (err)
			যাও try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		যদি (err)
			যাও out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, ডান_ext->e_lblk);
		যদি (err)
			যাও out;
		range_len -= plen;
		ডান_ext->e_lblk -= plen;
		ডান_ext->e_pblk -= plen;
		ডান_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", ডান_ext);
		err = ext2fs_extent_replace(handle, 0, ডান_ext);
		যদি (err)
			যাও out;
		err = ext2fs_extent_fix_parents(handle);
		যদি (err)
			যাও out;

		/* Zero blocks if necessary */
		যদি (!(ডান_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, ফাঁকা, ফাঁকা);
			যদি (err)
				যাও out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	যদি (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		যদি (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		যদি (err)
			যাও out;
		যদি (pblk == 0)
			যাও try_ডান_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		যদি (err == EXT2_ET_NO_CURRENT_গ্রন্থি)
			ex.e_lblk = 0;
		অন্যথা যদি (err)
			যাও out;

		যদি (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		অন্যথা
			op = EXT2_EXTENT_ঢোকাও_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ঢোকাও(handle, op, &newex);
		যদি (err)
			যাও out;
		err = ext2fs_extent_fix_parents(handle);
		যদি (err)
			যাও out;

		যদি (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ফাঁকা, ফাঁকা);
			যদি (err)
				যাও out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_ডান_implied:
	y = range_start + range_len;
	যদি (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		যদি (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		যদি (err)
			যাও out;
		যদি (pblk == 0)
			যাও no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		যদি (err == EXT2_ET_NO_CURRENT_গ্রন্থি)
			ex.e_lblk = 0;
		অন্যথা যদি (err)
			যাও out;

		যদি (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		অন্যথা
			op = EXT2_EXTENT_ঢোকাও_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ঢোকাও(handle, op, &newex);
		যদি (err)
			যাও out;
		err = ext2fs_extent_fix_parents(handle);
		যদি (err)
			যাও out;

		যদি (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ফাঁকা, ফাঁকা);
			যদি (err)
				যাও out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	যদি (range_len == 0)
		ফেরত 0;

	newex.e_lblk = range_start;
	যদি (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} অন্যথা {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	জন্য (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       ফাঁকা, &pblk, &plen);
		যদি (err)
			যাও out;
		err = claim_range(fs, inode, pblk, plen);
		যদি (err)
			যাও out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		যদি (err == EXT2_ET_NO_CURRENT_গ্রন্থি)
			ex.e_lblk = 0;
		অন্যথা যদি (err)
			যাও out;

		যদি (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		অন্যথা
			op = EXT2_EXTENT_ঢোকাও_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ঢোকাও(handle, op, &newex);
		যদি (err)
			যাও out;
		err = ext2fs_extent_fix_parents(handle);
		যদি (err)
			যাও out;

		যদি (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ফাঁকা, ফাঁকা);
			যদি (err)
				যাও out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		যদি (pblk >= ext2fs_blocks_গুনতি(fs->super))
			pblk = fs->super->s_first_তথ্য_block;
	}

out:
	ফেরত err;
}

স্থির errcode_t extent_fallocate(ext2_filsys fs, পূর্ণসংখ্যা flags, ext2_ino_t ino,
				      কাঠামো ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	কাঠামো ext2fs_extent	বাম_extent, ডান_extent;
	কাঠামো ext2fs_extent	*বাম_adjacent, *ডান_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, পরবর্তী;
	blk64_t			গুনতি, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	যদি (err)
		ফেরত err;

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
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &বাম_extent);
	যদি (err == EXT2_ET_NO_CURRENT_গ্রন্থি) {
		blk64_t max_blocks = ext2fs_blocks_গুনতি(fs->super);

		যদি (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ফাঁকা,
					ফাঁকা, start, len, goal);
		যাও errout;
	} অন্যথা যদি (err)
		যাও errout;

	dbg_print_extent("ext_falloc initial", &বাম_extent);
	পরবর্তী = বাম_extent.e_lblk + বাম_extent.e_len;
	যদি (বাম_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = বাম_extent.e_pblk - (বাম_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ফাঁকা,
					&বাম_extent, start,
					min(len, বাম_extent.e_lblk - start),
					goal);
		যদি (err)
			যাও errout;

		যাও start_again;
	} অন্যথা যদি (পরবর্তী >= start) {
		range_start = পরবর্তী;
		বাম_adjacent = &বাম_extent;
	} অন্যথা {
		range_start = start;
		বাম_adjacent = ফাঁকা;
	}
	goal = বাম_extent.e_pblk + (range_start - বাম_extent.e_lblk);

	করো {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_পরবর্তী_LEAF,
					   &ডান_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (পূর্ণসংখ্যা)err);
		dbg_print_extent("ext_falloc next", &ডান_extent);
		/* Stop if we've seen this extent before */
		যদি (!err && ডান_extent.e_lblk <= বাম_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_পরবর্তী;

		যদি (err && err != EXT2_ET_EXTENT_NO_পরবর্তী)
			যাও errout;
		যদি (err == EXT2_ET_EXTENT_NO_পরবর্তী ||
		    ডান_extent.e_lblk > end + 1) {
			range_end = end;
			ডান_adjacent = ফাঁকা;
		} অন্যথা {
			/* Handle right_extent.e_lblk <= end */
			range_end = ডান_extent.e_lblk - 1;
			ডান_adjacent = &ডান_extent;
		}
		goal_distance = range_start - পরবর্তী;
		যদি (err != EXT2_ET_EXTENT_NO_পরবর্তী &&
		    goal_distance > (range_end - ডান_extent.e_lblk))
			goal = ডান_extent.e_pblk -
					(ডান_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		যদি (range_start <= range_end) {
			গুনতি = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						বাম_adjacent, ডান_adjacent,
						range_start, গুনতি, goal);
			যদি (err)
				যাও errout;
		}

		যদি (range_end == end)
			বিরতি;

		err = ext2fs_extent_goto(handle, ডান_extent.e_lblk);
		যদি (err)
			যাও errout;
		পরবর্তী = ডান_extent.e_lblk + ডান_extent.e_len;
		বাম_extent = ডান_extent;
		বাম_adjacent = &বাম_extent;
		range_start = পরবর্তী;
		goal = বাম_extent.e_pblk + (range_start - বাম_extent.e_lblk);
	} যখন (range_end < end);

errout:
	ext2fs_extent_free(handle);
	ফেরত err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, পূর্ণসংখ্যা flags, ext2_ino_t ino,
			   কাঠামো ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	কাঠামো ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	পূর্ণসংখ্যা			zero_len = 0;
	errcode_t		err;

	যদি (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	যদি (len > ext2fs_blocks_গুনতি(fs->super))
		ফেরত EXT2_ET_BLOCK_ALLOC_FAIL;
	অন্যথা যদি (len == 0)
		ফেরত 0;

	/* Read inode structure if necessary */
	যদি (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		যদি (err)
			ফেরত err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	যদি (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		যাও out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	জন্য (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, ফাঁকা, 0, blk, 0, &x);
		যদি (err)
			ফেরত err;
		যদি (x)
			চালু;

		err = ext2fs_bmap2(fs, ino, inode, ফাঁকা, BMAP_ALLOC,
				   blk, 0, &x);
		যদি (err)
			যাও errout;
		যদি ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  ফাঁকা, ফাঁকা);
			zero_len = 0;
			যদি (err)
				যাও errout;
		}
		যদি (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} অন্যথা {
			zero_len++;
		}
		last = x;
	}

out:
	যদি (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	যদি (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, ফাঁকা, ফাঁকা);
	ফেরত err;
}
