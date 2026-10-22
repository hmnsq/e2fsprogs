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

#شامل "config.h"

#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  انجامبده {printf(f, ## a); fflush(stdout); } وقتی (0)
#وگرنه
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

ایستا تهی dbg_print_extent(ثابت نویسه *desc EXT2FS_ATTR((unused)),
		ثابت ساختار ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	اگر (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	اگر (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#endif
}

ایستا errcode_t claim_range(ext2_filsys fs, ساختار ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	بازگشت ext2fs_iblk_add_blocks(fs, inode, clusters);
}

ایستا errcode_t ext_falloc_helper(ext2_filsys fs,
				   صحیح flags,
				   ext2_ino_t ino,
				   ساختار ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   ساختار ext2fs_extent *چپ_ext,
				   ساختار ext2fs_extent *راست_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	ساختار ext2fs_extent	newex, ex;
	صحیح			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	printf("%s: ", __func__);
	اگر (چپ_ext)
		printf("left_ext=%llu--%llu, ", چپ_ext->e_lblk,
		       چپ_ext->e_lblk + چپ_ext->e_len - 1);
	اگر (راست_ext)
		printf("right_ext=%llu--%llu, ", راست_ext->e_lblk,
		       راست_ext->e_lblk + راست_ext->e_len - 1);
	printf("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#endif
	/* Can't create initialized extents past EOF? */
	اگر (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_SIZE(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	اگر (چپ_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		اگر (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - چپ_ext->e_len;
		وگرنه
			fillable = max_init_len - چپ_ext->e_len;

		اگر (fillable > range_len)
			fillable = range_len;
		اگر (fillable == 0)
			برو expand_راست;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		اگر (cluster_fill == 0)
			برو expand_راست;

		اگر (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		اگر (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = چپ_ext->e_lblk + چپ_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			اگر (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			اگر (cluster_fill == 0)
				برو expand_راست;
		}

		err = ext2fs_extent_goto(handle, چپ_ext->e_lblk);
		اگر (err)
			برو expand_راست;
		چپ_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", چپ_ext);
		err = ext2fs_extent_replace(handle, 0, چپ_ext);
		اگر (err)
			برو out;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			برو out;

		/* Zero blocks */
		اگر (!(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, چپ_ext->e_pblk +
						  چپ_ext->e_len -
						  cluster_fill, cluster_fill,
						  پوچ, پوچ);
			اگر (err)
				برو out;
		}
	}

expand_راست:
	/* We must lengthen the right extent to the beginning of the cluster */
	اگر (راست_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		اگر (راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - راست_ext->e_len;
		وگرنه
			fillable = max_init_len - راست_ext->e_len;

		اگر (fillable > range_len)
			fillable = range_len;
		اگر (fillable == 0)
			برو try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = راست_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		اگر (cluster_fill == 0)
			برو try_merge;

		err = ext2fs_extent_goto(handle, راست_ext->e_lblk);
		اگر (err)
			برو out;

		اگر (cluster_fill > fillable)
			cluster_fill = fillable;
		راست_ext->e_lblk -= cluster_fill;
		راست_ext->e_pblk -= cluster_fill;
		راست_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", راست_ext);
		err = ext2fs_extent_replace(handle, 0, راست_ext);
		اگر (err)
			برو out;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			برو out;

		/* Zero blocks if necessary */
		اگر (!(راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, راست_ext->e_pblk,
						  cluster_fill, پوچ, پوچ);
			اگر (err)
				برو out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	اگر (چپ_ext && راست_ext) {
		/* Are the two extents mergeable? */
		اگر ((چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			برو try_چپ;

		/* User requires init/uninit but extent is uninit/init. */
		اگر (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			برو try_چپ;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		اگر (!(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			برو try_چپ;

		/* Will it even fit? */
		x = چپ_ext->e_len + range_len + راست_ext->e_len;
		اگر (x > (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			برو try_چپ;

		/* Would they even be physically contiguous if merged? */
		اگر (چپ_ext->e_pblk + چپ_ext->e_len + range_len !=
				راست_ext->e_pblk)
			برو try_چپ;

		err = ext2fs_extent_goto(handle, چپ_ext->e_lblk);
		اگر (err)
			برو try_چپ;

		/* Allocate blocks */
		y = چپ_ext->e_pblk + چپ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_LENGTH, y,
				       راست_ext->e_pblk - y + 1, پوچ,
				       &pblk, &plen);
		اگر (err)
			برو try_چپ;
		اگر (pblk + plen != راست_ext->e_pblk)
			برو try_چپ;
		err = claim_range(fs, inode, pblk, plen);
		اگر (err)
			برو out;

		/* Modify extents */
		چپ_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", چپ_ext);
		err = ext2fs_extent_replace(handle, 0, چپ_ext);
		اگر (err)
			برو out;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			برو out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &newex);
		اگر (err)
			برو out;
		err = ext2fs_extent_delete(handle, 0);
		اگر (err)
			برو out;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			برو out;
		*راست_ext = *چپ_ext;

		/* Zero blocks */
		اگر (!(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  پوچ, پوچ);
			اگر (err)
				برو out;
		}

		بازگشت 0;
	}

try_چپ:
	/* Extend the left extent */
	اگر (چپ_ext) {
		/* How many more blocks can be attached to left_ext? */
		اگر (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - چپ_ext->e_len;
		وگرنه اگر (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - چپ_ext->e_len;
		وگرنه
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		اگر (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			برو try_راست;

		اگر (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = چپ_ext->e_lblk + چپ_ext->e_len - 1;
		اگر (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			اگر (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		اگر (fillable == 0)
			برو try_راست;

		/* Test if the right edge of the range is already mapped? */
		اگر (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			اگر (err)
				برو out;
			اگر (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			اگر (fillable == 0)
				برو try_راست;
		}

		/* Allocate range of blocks */
		x = چپ_ext->e_pblk + چپ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				x, fillable, پوچ, &pblk, &plen);
		اگر (err)
			برو try_راست;
		err = claim_range(fs, inode, pblk, plen);
		اگر (err)
			برو out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, چپ_ext->e_lblk);
		اگر (err)
			برو out;
		range_start += plen;
		range_len -= plen;
		چپ_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", چپ_ext);
		err = ext2fs_extent_replace(handle, 0, چپ_ext);
		اگر (err)
			برو out;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			برو out;

		/* Zero blocks if necessary */
		اگر (!(چپ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, پوچ, پوچ);
			اگر (err)
				برو out;
		}
	}

try_راست:
	/* Extend the right extent */
	اگر (راست_ext) {
		/* How much can we attach to right_ext? */
		اگر (راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - راست_ext->e_len;
		وگرنه اگر (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - راست_ext->e_len;
		وگرنه
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		اگر (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			برو try_anywhere;

		اگر (fillable > range_len)
			fillable = range_len;
		اگر (fillable == 0)
			برو try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		اگر (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					راست_ext->e_lblk - fillable, &pblk);
			اگر (err)
				برو out;
			اگر (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((راست_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			اگر (fillable == 0)
				برو try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				راست_ext->e_pblk - fillable,
				fillable, پوچ, &pblk, &plen);
		اگر (err)
			برو try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		اگر (err)
			برو out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, راست_ext->e_lblk);
		اگر (err)
			برو out;
		range_len -= plen;
		راست_ext->e_lblk -= plen;
		راست_ext->e_pblk -= plen;
		راست_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", راست_ext);
		err = ext2fs_extent_replace(handle, 0, راست_ext);
		اگر (err)
			برو out;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			برو out;

		/* Zero blocks if necessary */
		اگر (!(راست_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, پوچ, پوچ);
			اگر (err)
				برو out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	اگر (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		اگر (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		اگر (err)
			برو out;
		اگر (pblk == 0)
			برو try_راست_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		اگر (err == EXT2_ET_NO_CURRENT_گره)
			ex.e_lblk = 0;
		وگرنه اگر (err)
			برو out;

		اگر (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		وگرنه
			op = EXT2_EXTENT_درج_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_درج(handle, op, &newex);
		اگر (err)
			برو out;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			برو out;

		اگر (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, پوچ, پوچ);
			اگر (err)
				برو out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_راست_implied:
	y = range_start + range_len;
	اگر (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		اگر (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		اگر (err)
			برو out;
		اگر (pblk == 0)
			برو no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		اگر (err == EXT2_ET_NO_CURRENT_گره)
			ex.e_lblk = 0;
		وگرنه اگر (err)
			برو out;

		اگر (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		وگرنه
			op = EXT2_EXTENT_درج_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_درج(handle, op, &newex);
		اگر (err)
			برو out;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			برو out;

		اگر (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, پوچ, پوچ);
			اگر (err)
				برو out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	اگر (range_len == 0)
		بازگشت 0;

	newex.e_lblk = range_start;
	اگر (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} وگرنه {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	برای (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       پوچ, &pblk, &plen);
		اگر (err)
			برو out;
		err = claim_range(fs, inode, pblk, plen);
		اگر (err)
			برو out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		اگر (err == EXT2_ET_NO_CURRENT_گره)
			ex.e_lblk = 0;
		وگرنه اگر (err)
			برو out;

		اگر (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		وگرنه
			op = EXT2_EXTENT_درج_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_درج(handle, op, &newex);
		اگر (err)
			برو out;
		err = ext2fs_extent_fix_parents(handle);
		اگر (err)
			برو out;

		اگر (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, پوچ, پوچ);
			اگر (err)
				برو out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		اگر (pblk >= ext2fs_blocks_شمار(fs->super))
			pblk = fs->super->s_first_data_block;
	}

out:
	بازگشت err;
}

ایستا errcode_t extent_fallocate(ext2_filsys fs, صحیح flags, ext2_ino_t ino,
				      ساختار ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	ساختار ext2fs_extent	چپ_extent, راست_extent;
	ساختار ext2fs_extent	*چپ_adjacent, *راست_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, next;
	blk64_t			شمار, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	اگر (err)
		بازگشت err;

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
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &چپ_extent);
	اگر (err == EXT2_ET_NO_CURRENT_گره) {
		blk64_t max_blocks = ext2fs_blocks_شمار(fs->super);

		اگر (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, پوچ,
					پوچ, start, len, goal);
		برو errout;
	} وگرنه اگر (err)
		برو errout;

	dbg_print_extent("ext_falloc initial", &چپ_extent);
	next = چپ_extent.e_lblk + چپ_extent.e_len;
	اگر (چپ_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = چپ_extent.e_pblk - (چپ_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, پوچ,
					&چپ_extent, start,
					min(len, چپ_extent.e_lblk - start),
					goal);
		اگر (err)
			برو errout;

		برو start_again;
	} وگرنه اگر (next >= start) {
		range_start = next;
		چپ_adjacent = &چپ_extent;
	} وگرنه {
		range_start = start;
		چپ_adjacent = پوچ;
	}
	goal = چپ_extent.e_pblk + (range_start - چپ_extent.e_lblk);

	انجامبده {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF,
					   &راست_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (صحیح)err);
		dbg_print_extent("ext_falloc next", &راست_extent);
		/* Stop if we've seen this extent before */
		اگر (!err && راست_extent.e_lblk <= چپ_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_NEXT;

		اگر (err && err != EXT2_ET_EXTENT_NO_NEXT)
			برو errout;
		اگر (err == EXT2_ET_EXTENT_NO_NEXT ||
		    راست_extent.e_lblk > end + 1) {
			range_end = end;
			راست_adjacent = پوچ;
		} وگرنه {
			/* Handle right_extent.e_lblk <= end */
			range_end = راست_extent.e_lblk - 1;
			راست_adjacent = &راست_extent;
		}
		goal_distance = range_start - next;
		اگر (err != EXT2_ET_EXTENT_NO_NEXT &&
		    goal_distance > (range_end - راست_extent.e_lblk))
			goal = راست_extent.e_pblk -
					(راست_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		اگر (range_start <= range_end) {
			شمار = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						چپ_adjacent, راست_adjacent,
						range_start, شمار, goal);
			اگر (err)
				برو errout;
		}

		اگر (range_end == end)
			بشکن;

		err = ext2fs_extent_goto(handle, راست_extent.e_lblk);
		اگر (err)
			برو errout;
		next = راست_extent.e_lblk + راست_extent.e_len;
		چپ_extent = راست_extent;
		چپ_adjacent = &چپ_extent;
		range_start = next;
		goal = چپ_extent.e_pblk + (range_start - چپ_extent.e_lblk);
	} وقتی (range_end < end);

errout:
	ext2fs_extent_free(handle);
	بازگشت err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, صحیح flags, ext2_ino_t ino,
			   ساختار ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	ساختار ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	صحیح			zero_len = 0;
	errcode_t		err;

	اگر (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	اگر (len > ext2fs_blocks_شمار(fs->super))
		بازگشت EXT2_ET_BLOCK_ALLOC_FAIL;
	وگرنه اگر (len == 0)
		بازگشت 0;

	/* Read inode structure if necessary */
	اگر (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		اگر (err)
			بازگشت err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	اگر (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		برو out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	برای (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, پوچ, 0, blk, 0, &x);
		اگر (err)
			بازگشت err;
		اگر (x)
			ادامه;

		err = ext2fs_bmap2(fs, ino, inode, پوچ, BMAP_ALLOC,
				   blk, 0, &x);
		اگر (err)
			برو errout;
		اگر ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  پوچ, پوچ);
			zero_len = 0;
			اگر (err)
				برو errout;
		}
		اگر (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} وگرنه {
			zero_len++;
		}
		last = x;
	}

out:
	اگر (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	اگر (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, پوچ, پوچ);
	بازگشت err;
}
