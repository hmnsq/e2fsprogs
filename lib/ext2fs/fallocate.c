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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  କର {printf(f, ## a); fflush(stdout); } ଯେତେବେଳେ (0)
#ଅନ୍ୟଥା
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

ସ୍ଥିର ଶୂନ୍ୟ dbg_print_extent(ଅଚଳ ଅକ୍ଷର *desc EXT2FS_ATTR((unused)),
		ଅଚଳ ସଂରଚନା ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	ଯଦି (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ଯଦି (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	ଯଦି (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ଯଦି (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	ଯଦି (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#endif
}

ସ୍ଥିର errcode_t claim_range(ext2_filsys fs, ସଂରଚନା ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	ଫେରନ୍ତୁ ext2fs_iblk_add_blocks(fs, inode, clusters);
}

ସ୍ଥିର errcode_t ext_falloc_helper(ext2_filsys fs,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags,
				   ext2_ino_t ino,
				   ସଂରଚନା ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   ସଂରଚନା ext2fs_extent *ବାମ_ext,
				   ସଂରଚନା ext2fs_extent *ଡାହାଣ_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	ସଂରଚନା ext2fs_extent	newex, ex;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	printf("%s: ", __func__);
	ଯଦି (ବାମ_ext)
		printf("left_ext=%llu--%llu, ", ବାମ_ext->e_lblk,
		       ବାମ_ext->e_lblk + ବାମ_ext->e_len - 1);
	ଯଦି (ଡାହାଣ_ext)
		printf("right_ext=%llu--%llu, ", ଡାହାଣ_ext->e_lblk,
		       ଡାହାଣ_ext->e_lblk + ଡାହାଣ_ext->e_len - 1);
	printf("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#endif
	/* Can't create initialized extents past EOF? */
	ଯଦି (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_SIZE(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	ଯଦି (ବାମ_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		ଯଦି (ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ବାମ_ext->e_len;
		ଅନ୍ୟଥା
			fillable = max_init_len - ବାମ_ext->e_len;

		ଯଦି (fillable > range_len)
			fillable = range_len;
		ଯଦି (fillable == 0)
			ଯାଅ expand_ଡାହାଣ;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		ଯଦି (cluster_fill == 0)
			ଯାଅ expand_ଡାହାଣ;

		ଯଦି (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		ଯଦି (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = ବାମ_ext->e_lblk + ବାମ_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			ଯଦି (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			ଯଦି (cluster_fill == 0)
				ଯାଅ expand_ଡାହାଣ;
		}

		err = ext2fs_extent_goto(handle, ବାମ_ext->e_lblk);
		ଯଦି (err)
			ଯାଅ expand_ଡାହାଣ;
		ବାମ_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", ବାମ_ext);
		err = ext2fs_extent_replace(handle, 0, ବାମ_ext);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_fix_parents(handle);
		ଯଦି (err)
			ଯାଅ out;

		/* Zero blocks */
		ଯଦି (!(ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ବାମ_ext->e_pblk +
						  ବାମ_ext->e_len -
						  cluster_fill, cluster_fill,
						  ଖାଲି, ଖାଲି);
			ଯଦି (err)
				ଯାଅ out;
		}
	}

expand_ଡାହାଣ:
	/* We must lengthen the right extent to the beginning of the cluster */
	ଯଦି (ଡାହାଣ_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		ଯଦି (ଡାହାଣ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ଡାହାଣ_ext->e_len;
		ଅନ୍ୟଥା
			fillable = max_init_len - ଡାହାଣ_ext->e_len;

		ଯଦି (fillable > range_len)
			fillable = range_len;
		ଯଦି (fillable == 0)
			ଯାଅ try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = ଡାହାଣ_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		ଯଦି (cluster_fill == 0)
			ଯାଅ try_merge;

		err = ext2fs_extent_goto(handle, ଡାହାଣ_ext->e_lblk);
		ଯଦି (err)
			ଯାଅ out;

		ଯଦି (cluster_fill > fillable)
			cluster_fill = fillable;
		ଡାହାଣ_ext->e_lblk -= cluster_fill;
		ଡାହାଣ_ext->e_pblk -= cluster_fill;
		ଡାହାଣ_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", ଡାହାଣ_ext);
		err = ext2fs_extent_replace(handle, 0, ଡାହାଣ_ext);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_fix_parents(handle);
		ଯଦି (err)
			ଯାଅ out;

		/* Zero blocks if necessary */
		ଯଦି (!(ଡାହାଣ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ଡାହାଣ_ext->e_pblk,
						  cluster_fill, ଖାଲି, ଖାଲି);
			ଯଦି (err)
				ଯାଅ out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	ଯଦି (ବାମ_ext && ଡାହାଣ_ext) {
		/* Are the two extents mergeable? */
		ଯଦି ((ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (ଡାହାଣ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			ଯାଅ try_ବାମ;

		/* User requires init/uninit but extent is uninit/init. */
		ଯଦି (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ଯାଅ try_ବାମ;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		ଯଦି (!(ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			ଯାଅ try_ବାମ;

		/* Will it even fit? */
		x = ବାମ_ext->e_len + range_len + ଡାହାଣ_ext->e_len;
		ଯଦି (x > (ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			ଯାଅ try_ବାମ;

		/* Would they even be physically contiguous if merged? */
		ଯଦି (ବାମ_ext->e_pblk + ବାମ_ext->e_len + range_len !=
				ଡାହାଣ_ext->e_pblk)
			ଯାଅ try_ବାମ;

		err = ext2fs_extent_goto(handle, ବାମ_ext->e_lblk);
		ଯଦି (err)
			ଯାଅ try_ବାମ;

		/* Allocate blocks */
		y = ବାମ_ext->e_pblk + ବାମ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_LENGTH, y,
				       ଡାହାଣ_ext->e_pblk - y + 1, ଖାଲି,
				       &pblk, &plen);
		ଯଦି (err)
			ଯାଅ try_ବାମ;
		ଯଦି (pblk + plen != ଡାହାଣ_ext->e_pblk)
			ଯାଅ try_ବାମ;
		err = claim_range(fs, inode, pblk, plen);
		ଯଦି (err)
			ଯାଅ out;

		/* Modify extents */
		ବାମ_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", ବାମ_ext);
		err = ext2fs_extent_replace(handle, 0, ବାମ_ext);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_fix_parents(handle);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &newex);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_delete(handle, 0);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_fix_parents(handle);
		ଯଦି (err)
			ଯାଅ out;
		*ଡାହାଣ_ext = *ବାମ_ext;

		/* Zero blocks */
		ଯଦି (!(ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  ଖାଲି, ଖାଲି);
			ଯଦି (err)
				ଯାଅ out;
		}

		ଫେରନ୍ତୁ 0;
	}

try_ବାମ:
	/* Extend the left extent */
	ଯଦି (ବାମ_ext) {
		/* How many more blocks can be attached to left_ext? */
		ଯଦି (ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ବାମ_ext->e_len;
		ଅନ୍ୟଥା ଯଦି (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - ବାମ_ext->e_len;
		ଅନ୍ୟଥା
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ଯଦି (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ଯାଅ try_ଡାହାଣ;

		ଯଦି (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = ବାମ_ext->e_lblk + ବାମ_ext->e_len - 1;
		ଯଦି (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			ଯଦି (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		ଯଦି (fillable == 0)
			ଯାଅ try_ଡାହାଣ;

		/* Test if the right edge of the range is already mapped? */
		ଯଦି (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			ଯଦି (err)
				ଯାଅ out;
			ଯଦି (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			ଯଦି (fillable == 0)
				ଯାଅ try_ଡାହାଣ;
		}

		/* Allocate range of blocks */
		x = ବାମ_ext->e_pblk + ବାମ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				x, fillable, ଖାଲି, &pblk, &plen);
		ଯଦି (err)
			ଯାଅ try_ଡାହାଣ;
		err = claim_range(fs, inode, pblk, plen);
		ଯଦି (err)
			ଯାଅ out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, ବାମ_ext->e_lblk);
		ଯଦି (err)
			ଯାଅ out;
		range_start += plen;
		range_len -= plen;
		ବାମ_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", ବାମ_ext);
		err = ext2fs_extent_replace(handle, 0, ବାମ_ext);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_fix_parents(handle);
		ଯଦି (err)
			ଯାଅ out;

		/* Zero blocks if necessary */
		ଯଦି (!(ବାମ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ଖାଲି, ଖାଲି);
			ଯଦି (err)
				ଯାଅ out;
		}
	}

try_ଡାହାଣ:
	/* Extend the right extent */
	ଯଦି (ଡାହାଣ_ext) {
		/* How much can we attach to right_ext? */
		ଯଦି (ଡାହାଣ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ଡାହାଣ_ext->e_len;
		ଅନ୍ୟଥା ଯଦି (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - ଡାହାଣ_ext->e_len;
		ଅନ୍ୟଥା
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ଯଦି (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ଡାହାଣ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ଡାହାଣ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			ଯାଅ try_anywhere;

		ଯଦି (fillable > range_len)
			fillable = range_len;
		ଯଦି (fillable == 0)
			ଯାଅ try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		ଯଦି (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					ଡାହାଣ_ext->e_lblk - fillable, &pblk);
			ଯଦି (err)
				ଯାଅ out;
			ଯଦି (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((ଡାହାଣ_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			ଯଦି (fillable == 0)
				ଯାଅ try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				ଡାହାଣ_ext->e_pblk - fillable,
				fillable, ଖାଲି, &pblk, &plen);
		ଯଦି (err)
			ଯାଅ try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		ଯଦି (err)
			ଯାଅ out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, ଡାହାଣ_ext->e_lblk);
		ଯଦି (err)
			ଯାଅ out;
		range_len -= plen;
		ଡାହାଣ_ext->e_lblk -= plen;
		ଡାହାଣ_ext->e_pblk -= plen;
		ଡାହାଣ_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", ଡାହାଣ_ext);
		err = ext2fs_extent_replace(handle, 0, ଡାହାଣ_ext);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_fix_parents(handle);
		ଯଦି (err)
			ଯାଅ out;

		/* Zero blocks if necessary */
		ଯଦି (!(ଡାହାଣ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, ଖାଲି, ଖାଲି);
			ଯଦି (err)
				ଯାଅ out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	ଯଦି (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		ଯଦି (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ଯଦି (err)
			ଯାଅ out;
		ଯଦି (pblk == 0)
			ଯାଅ try_ଡାହାଣ_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ଯଦି (err == EXT2_ET_NO_CURRENT_ଗଣ୍ଠି)
			ex.e_lblk = 0;
		ଅନ୍ୟଥା ଯଦି (err)
			ଯାଅ out;

		ଯଦି (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ଅନ୍ୟଥା
			op = EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ଭର୍ତ୍ତି(handle, op, &newex);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_fix_parents(handle);
		ଯଦି (err)
			ଯାଅ out;

		ଯଦି (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ଖାଲି, ଖାଲି);
			ଯଦି (err)
				ଯାଅ out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_ଡାହାଣ_implied:
	y = range_start + range_len;
	ଯଦି (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		ଯଦି (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ଯଦି (err)
			ଯାଅ out;
		ଯଦି (pblk == 0)
			ଯାଅ no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ଯଦି (err == EXT2_ET_NO_CURRENT_ଗଣ୍ଠି)
			ex.e_lblk = 0;
		ଅନ୍ୟଥା ଯଦି (err)
			ଯାଅ out;

		ଯଦି (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ଅନ୍ୟଥା
			op = EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ଭର୍ତ୍ତି(handle, op, &newex);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_fix_parents(handle);
		ଯଦି (err)
			ଯାଅ out;

		ଯଦି (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ଖାଲି, ଖାଲି);
			ଯଦି (err)
				ଯାଅ out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	ଯଦି (range_len == 0)
		ଫେରନ୍ତୁ 0;

	newex.e_lblk = range_start;
	ଯଦି (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} ଅନ୍ୟଥା {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	ପାଇଁ (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       ଖାଲି, &pblk, &plen);
		ଯଦି (err)
			ଯାଅ out;
		err = claim_range(fs, inode, pblk, plen);
		ଯଦି (err)
			ଯାଅ out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ଯଦି (err == EXT2_ET_NO_CURRENT_ଗଣ୍ଠି)
			ex.e_lblk = 0;
		ଅନ୍ୟଥା ଯଦି (err)
			ଯାଅ out;

		ଯଦି (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		ଅନ୍ୟଥା
			op = EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ଭର୍ତ୍ତି(handle, op, &newex);
		ଯଦି (err)
			ଯାଅ out;
		err = ext2fs_extent_fix_parents(handle);
		ଯଦି (err)
			ଯାଅ out;

		ଯଦି (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ଖାଲି, ଖାଲି);
			ଯଦି (err)
				ଯାଅ out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		ଯଦି (pblk >= ext2fs_blocks_ଗଣନା(fs->super))
			pblk = fs->super->s_first_data_block;
	}

out:
	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର errcode_t extent_fallocate(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ext2_ino_t ino,
				      ସଂରଚନା ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	ସଂରଚନା ext2fs_extent	ବାମ_extent, ଡାହାଣ_extent;
	ସଂରଚନା ext2fs_extent	*ବାମ_adjacent, *ଡାହାଣ_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, next;
	blk64_t			ଗଣନା_2, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

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
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &ବାମ_extent);
	ଯଦି (err == EXT2_ET_NO_CURRENT_ଗଣ୍ଠି) {
		blk64_t max_blocks = ext2fs_blocks_ଗଣନା(fs->super);

		ଯଦି (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ଖାଲି,
					ଖାଲି, start, len, goal);
		ଯାଅ errout;
	} ଅନ୍ୟଥା ଯଦି (err)
		ଯାଅ errout;

	dbg_print_extent("ext_falloc initial", &ବାମ_extent);
	next = ବାମ_extent.e_lblk + ବାମ_extent.e_len;
	ଯଦି (ବାମ_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = ବାମ_extent.e_pblk - (ବାମ_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ଖାଲି,
					&ବାମ_extent, start,
					min(len, ବାମ_extent.e_lblk - start),
					goal);
		ଯଦି (err)
			ଯାଅ errout;

		ଯାଅ start_again;
	} ଅନ୍ୟଥା ଯଦି (next >= start) {
		range_start = next;
		ବାମ_adjacent = &ବାମ_extent;
	} ଅନ୍ୟଥା {
		range_start = start;
		ବାମ_adjacent = ଖାଲି;
	}
	goal = ବାମ_extent.e_pblk + (range_start - ବାମ_extent.e_lblk);

	କର {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF,
					   &ଡାହାଣ_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)err);
		dbg_print_extent("ext_falloc next", &ଡାହାଣ_extent);
		/* Stop if we've seen this extent before */
		ଯଦି (!err && ଡାହାଣ_extent.e_lblk <= ବାମ_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_NEXT;

		ଯଦି (err && err != EXT2_ET_EXTENT_NO_NEXT)
			ଯାଅ errout;
		ଯଦି (err == EXT2_ET_EXTENT_NO_NEXT ||
		    ଡାହାଣ_extent.e_lblk > end + 1) {
			range_end = end;
			ଡାହାଣ_adjacent = ଖାଲି;
		} ଅନ୍ୟଥା {
			/* Handle right_extent.e_lblk <= end */
			range_end = ଡାହାଣ_extent.e_lblk - 1;
			ଡାହାଣ_adjacent = &ଡାହାଣ_extent;
		}
		goal_distance = range_start - next;
		ଯଦି (err != EXT2_ET_EXTENT_NO_NEXT &&
		    goal_distance > (range_end - ଡାହାଣ_extent.e_lblk))
			goal = ଡାହାଣ_extent.e_pblk -
					(ଡାହାଣ_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		ଯଦି (range_start <= range_end) {
			ଗଣନା_2 = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						ବାମ_adjacent, ଡାହାଣ_adjacent,
						range_start, ଗଣନା_2, goal);
			ଯଦି (err)
				ଯାଅ errout;
		}

		ଯଦି (range_end == end)
			ଭାଙ୍ଗ;

		err = ext2fs_extent_goto(handle, ଡାହାଣ_extent.e_lblk);
		ଯଦି (err)
			ଯାଅ errout;
		next = ଡାହାଣ_extent.e_lblk + ଡାହାଣ_extent.e_len;
		ବାମ_extent = ଡାହାଣ_extent;
		ବାମ_adjacent = &ବାମ_extent;
		range_start = next;
		goal = ବାମ_extent.e_pblk + (range_start - ବାମ_extent.e_lblk);
	} ଯେତେବେଳେ (range_end < end);

errout:
	ext2fs_extent_free(handle);
	ଫେରନ୍ତୁ err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ext2_ino_t ino,
			   ସଂରଚନା ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	ସଂରଚନା ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			zero_len = 0;
	errcode_t		err;

	ଯଦି (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ଯଦି (len > ext2fs_blocks_ଗଣନା(fs->super))
		ଫେରନ୍ତୁ EXT2_ET_BLOCK_ALLOC_FAIL;
	ଅନ୍ୟଥା ଯଦି (len == 0)
		ଫେରନ୍ତୁ 0;

	/* Read inode structure if necessary */
	ଯଦି (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		ଯଦି (err)
			ଫେରନ୍ତୁ err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	ଯଦି (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		ଯାଅ out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	ପାଇଁ (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, ଖାଲି, 0, blk, 0, &x);
		ଯଦି (err)
			ଫେରନ୍ତୁ err;
		ଯଦି (x)
			ଜାରି;

		err = ext2fs_bmap2(fs, ino, inode, ଖାଲି, BMAP_ALLOC,
				   blk, 0, &x);
		ଯଦି (err)
			ଯାଅ errout;
		ଯଦି ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  ଖାଲି, ଖାଲି);
			zero_len = 0;
			ଯଦି (err)
				ଯାଅ errout;
		}
		ଯଦି (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} ଅନ୍ୟଥା {
			zero_len++;
		}
		last = x;
	}

out:
	ଯଦି (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	ଯଦି (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, ଖାଲି, ଖାଲି);
	ଫେରନ୍ତୁ err;
}
