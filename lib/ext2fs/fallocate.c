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

#അന്തര്ഭാവ "config.h"

#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  ചെയ്യുക {printf(f, ## a); fflush(stdout); } ആകുമ്പോൾ (0)
#അല്ലെങ്കിൽ
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

സ്ഥിതം ശൂന്യം dbg_print_extent(സ്ഥിരം അക്ഷരം *desc EXT2FS_ATTR((unused)),
		സ്ഥിരം ഘടന ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	എങ്കിൽ (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	എങ്കിൽ (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	എങ്കിൽ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	എങ്കിൽ (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	എങ്കിൽ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#endif
}

സ്ഥിതം errcode_t claim_range(ext2_filsys fs, ഘടന ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	തിരികെ ext2fs_iblk_add_blocks(fs, inode, clusters);
}

സ്ഥിതം errcode_t ext_falloc_helper(ext2_filsys fs,
				   പൂർണ്ണസംഖ്യ flags,
				   ext2_ino_t ino,
				   ഘടന ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   ഘടന ext2fs_extent *ഇടത്_ext,
				   ഘടന ext2fs_extent *വലത്_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	ഘടന ext2fs_extent	newex, ex;
	പൂർണ്ണസംഖ്യ			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	printf("%s: ", __func__);
	എങ്കിൽ (ഇടത്_ext)
		printf("left_ext=%llu--%llu, ", ഇടത്_ext->e_lblk,
		       ഇടത്_ext->e_lblk + ഇടത്_ext->e_len - 1);
	എങ്കിൽ (വലത്_ext)
		printf("right_ext=%llu--%llu, ", വലത്_ext->e_lblk,
		       വലത്_ext->e_lblk + വലത്_ext->e_len - 1);
	printf("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#endif
	/* Can't create initialized extents past EOF? */
	എങ്കിൽ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_SIZE(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	എങ്കിൽ (ഇടത്_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		എങ്കിൽ (ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ഇടത്_ext->e_len;
		അല്ലെങ്കിൽ
			fillable = max_init_len - ഇടത്_ext->e_len;

		എങ്കിൽ (fillable > range_len)
			fillable = range_len;
		എങ്കിൽ (fillable == 0)
			പോകുക expand_വലത്;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		എങ്കിൽ (cluster_fill == 0)
			പോകുക expand_വലത്;

		എങ്കിൽ (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		എങ്കിൽ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = ഇടത്_ext->e_lblk + ഇടത്_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			എങ്കിൽ (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			എങ്കിൽ (cluster_fill == 0)
				പോകുക expand_വലത്;
		}

		err = ext2fs_extent_goto(handle, ഇടത്_ext->e_lblk);
		എങ്കിൽ (err)
			പോകുക expand_വലത്;
		ഇടത്_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", ഇടത്_ext);
		err = ext2fs_extent_replace(handle, 0, ഇടത്_ext);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (err)
			പോകുക out;

		/* Zero blocks */
		എങ്കിൽ (!(ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ഇടത്_ext->e_pblk +
						  ഇടത്_ext->e_len -
						  cluster_fill, cluster_fill,
						  ശൂന്യ, ശൂന്യ);
			എങ്കിൽ (err)
				പോകുക out;
		}
	}

expand_വലത്:
	/* We must lengthen the right extent to the beginning of the cluster */
	എങ്കിൽ (വലത്_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		എങ്കിൽ (വലത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - വലത്_ext->e_len;
		അല്ലെങ്കിൽ
			fillable = max_init_len - വലത്_ext->e_len;

		എങ്കിൽ (fillable > range_len)
			fillable = range_len;
		എങ്കിൽ (fillable == 0)
			പോകുക try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = വലത്_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		എങ്കിൽ (cluster_fill == 0)
			പോകുക try_merge;

		err = ext2fs_extent_goto(handle, വലത്_ext->e_lblk);
		എങ്കിൽ (err)
			പോകുക out;

		എങ്കിൽ (cluster_fill > fillable)
			cluster_fill = fillable;
		വലത്_ext->e_lblk -= cluster_fill;
		വലത്_ext->e_pblk -= cluster_fill;
		വലത്_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", വലത്_ext);
		err = ext2fs_extent_replace(handle, 0, വലത്_ext);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (err)
			പോകുക out;

		/* Zero blocks if necessary */
		എങ്കിൽ (!(വലത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, വലത്_ext->e_pblk,
						  cluster_fill, ശൂന്യ, ശൂന്യ);
			എങ്കിൽ (err)
				പോകുക out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	എങ്കിൽ (ഇടത്_ext && വലത്_ext) {
		/* Are the two extents mergeable? */
		എങ്കിൽ ((ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (വലത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			പോകുക try_ഇടത്;

		/* User requires init/uninit but extent is uninit/init. */
		എങ്കിൽ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			പോകുക try_ഇടത്;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		എങ്കിൽ (!(ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			പോകുക try_ഇടത്;

		/* Will it even fit? */
		x = ഇടത്_ext->e_len + range_len + വലത്_ext->e_len;
		എങ്കിൽ (x > (ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			പോകുക try_ഇടത്;

		/* Would they even be physically contiguous if merged? */
		എങ്കിൽ (ഇടത്_ext->e_pblk + ഇടത്_ext->e_len + range_len !=
				വലത്_ext->e_pblk)
			പോകുക try_ഇടത്;

		err = ext2fs_extent_goto(handle, ഇടത്_ext->e_lblk);
		എങ്കിൽ (err)
			പോകുക try_ഇടത്;

		/* Allocate blocks */
		y = ഇടത്_ext->e_pblk + ഇടത്_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_LENGTH, y,
				       വലത്_ext->e_pblk - y + 1, ശൂന്യ,
				       &pblk, &plen);
		എങ്കിൽ (err)
			പോകുക try_ഇടത്;
		എങ്കിൽ (pblk + plen != വലത്_ext->e_pblk)
			പോകുക try_ഇടത്;
		err = claim_range(fs, inode, pblk, plen);
		എങ്കിൽ (err)
			പോകുക out;

		/* Modify extents */
		ഇടത്_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", ഇടത്_ext);
		err = ext2fs_extent_replace(handle, 0, ഇടത്_ext);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &newex);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_delete(handle, 0);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (err)
			പോകുക out;
		*വലത്_ext = *ഇടത്_ext;

		/* Zero blocks */
		എങ്കിൽ (!(ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  ശൂന്യ, ശൂന്യ);
			എങ്കിൽ (err)
				പോകുക out;
		}

		തിരികെ 0;
	}

try_ഇടത്:
	/* Extend the left extent */
	എങ്കിൽ (ഇടത്_ext) {
		/* How many more blocks can be attached to left_ext? */
		എങ്കിൽ (ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ഇടത്_ext->e_len;
		അല്ലെങ്കിൽ എങ്കിൽ (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - ഇടത്_ext->e_len;
		അല്ലെങ്കിൽ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		എങ്കിൽ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			പോകുക try_വലത്;

		എങ്കിൽ (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = ഇടത്_ext->e_lblk + ഇടത്_ext->e_len - 1;
		എങ്കിൽ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			എങ്കിൽ (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		എങ്കിൽ (fillable == 0)
			പോകുക try_വലത്;

		/* Test if the right edge of the range is already mapped? */
		എങ്കിൽ (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			എങ്കിൽ (err)
				പോകുക out;
			എങ്കിൽ (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			എങ്കിൽ (fillable == 0)
				പോകുക try_വലത്;
		}

		/* Allocate range of blocks */
		x = ഇടത്_ext->e_pblk + ഇടത്_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				x, fillable, ശൂന്യ, &pblk, &plen);
		എങ്കിൽ (err)
			പോകുക try_വലത്;
		err = claim_range(fs, inode, pblk, plen);
		എങ്കിൽ (err)
			പോകുക out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, ഇടത്_ext->e_lblk);
		എങ്കിൽ (err)
			പോകുക out;
		range_start += plen;
		range_len -= plen;
		ഇടത്_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", ഇടത്_ext);
		err = ext2fs_extent_replace(handle, 0, ഇടത്_ext);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (err)
			പോകുക out;

		/* Zero blocks if necessary */
		എങ്കിൽ (!(ഇടത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ശൂന്യ, ശൂന്യ);
			എങ്കിൽ (err)
				പോകുക out;
		}
	}

try_വലത്:
	/* Extend the right extent */
	എങ്കിൽ (വലത്_ext) {
		/* How much can we attach to right_ext? */
		എങ്കിൽ (വലത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - വലത്_ext->e_len;
		അല്ലെങ്കിൽ എങ്കിൽ (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - വലത്_ext->e_len;
		അല്ലെങ്കിൽ
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		എങ്കിൽ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (വലത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(വലത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			പോകുക try_anywhere;

		എങ്കിൽ (fillable > range_len)
			fillable = range_len;
		എങ്കിൽ (fillable == 0)
			പോകുക try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		എങ്കിൽ (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					വലത്_ext->e_lblk - fillable, &pblk);
			എങ്കിൽ (err)
				പോകുക out;
			എങ്കിൽ (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((വലത്_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			എങ്കിൽ (fillable == 0)
				പോകുക try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				വലത്_ext->e_pblk - fillable,
				fillable, ശൂന്യ, &pblk, &plen);
		എങ്കിൽ (err)
			പോകുക try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		എങ്കിൽ (err)
			പോകുക out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, വലത്_ext->e_lblk);
		എങ്കിൽ (err)
			പോകുക out;
		range_len -= plen;
		വലത്_ext->e_lblk -= plen;
		വലത്_ext->e_pblk -= plen;
		വലത്_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", വലത്_ext);
		err = ext2fs_extent_replace(handle, 0, വലത്_ext);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (err)
			പോകുക out;

		/* Zero blocks if necessary */
		എങ്കിൽ (!(വലത്_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, ശൂന്യ, ശൂന്യ);
			എങ്കിൽ (err)
				പോകുക out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	എങ്കിൽ (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		എങ്കിൽ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		എങ്കിൽ (err)
			പോകുക out;
		എങ്കിൽ (pblk == 0)
			പോകുക try_വലത്_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		എങ്കിൽ (err == EXT2_ET_NO_CURRENT_കണ്ണി)
			ex.e_lblk = 0;
		അല്ലെങ്കിൽ എങ്കിൽ (err)
			പോകുക out;

		എങ്കിൽ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		അല്ലെങ്കിൽ
			op = EXT2_EXTENT_തിരുകുക_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_തിരുകുക(handle, op, &newex);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (err)
			പോകുക out;

		എങ്കിൽ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ശൂന്യ, ശൂന്യ);
			എങ്കിൽ (err)
				പോകുക out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_വലത്_implied:
	y = range_start + range_len;
	എങ്കിൽ (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		എങ്കിൽ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		എങ്കിൽ (err)
			പോകുക out;
		എങ്കിൽ (pblk == 0)
			പോകുക no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		എങ്കിൽ (err == EXT2_ET_NO_CURRENT_കണ്ണി)
			ex.e_lblk = 0;
		അല്ലെങ്കിൽ എങ്കിൽ (err)
			പോകുക out;

		എങ്കിൽ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		അല്ലെങ്കിൽ
			op = EXT2_EXTENT_തിരുകുക_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_തിരുകുക(handle, op, &newex);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (err)
			പോകുക out;

		എങ്കിൽ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ശൂന്യ, ശൂന്യ);
			എങ്കിൽ (err)
				പോകുക out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	എങ്കിൽ (range_len == 0)
		തിരികെ 0;

	newex.e_lblk = range_start;
	എങ്കിൽ (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} അല്ലെങ്കിൽ {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	വേണ്ടി (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       ശൂന്യ, &pblk, &plen);
		എങ്കിൽ (err)
			പോകുക out;
		err = claim_range(fs, inode, pblk, plen);
		എങ്കിൽ (err)
			പോകുക out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		എങ്കിൽ (err == EXT2_ET_NO_CURRENT_കണ്ണി)
			ex.e_lblk = 0;
		അല്ലെങ്കിൽ എങ്കിൽ (err)
			പോകുക out;

		എങ്കിൽ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		അല്ലെങ്കിൽ
			op = EXT2_EXTENT_തിരുകുക_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_തിരുകുക(handle, op, &newex);
		എങ്കിൽ (err)
			പോകുക out;
		err = ext2fs_extent_fix_parents(handle);
		എങ്കിൽ (err)
			പോകുക out;

		എങ്കിൽ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ശൂന്യ, ശൂന്യ);
			എങ്കിൽ (err)
				പോകുക out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		എങ്കിൽ (pblk >= ext2fs_blocks_എണ്ണം(fs->super))
			pblk = fs->super->s_first_data_block;
	}

out:
	തിരികെ err;
}

സ്ഥിതം errcode_t extent_fallocate(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, ext2_ino_t ino,
				      ഘടന ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	ഘടന ext2fs_extent	ഇടത്_extent, വലത്_extent;
	ഘടന ext2fs_extent	*ഇടത്_adjacent, *വലത്_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, next;
	blk64_t			എണ്ണം, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	എങ്കിൽ (err)
		തിരികെ err;

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
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &ഇടത്_extent);
	എങ്കിൽ (err == EXT2_ET_NO_CURRENT_കണ്ണി) {
		blk64_t max_blocks = ext2fs_blocks_എണ്ണം(fs->super);

		എങ്കിൽ (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ശൂന്യ,
					ശൂന്യ, start, len, goal);
		പോകുക errout;
	} അല്ലെങ്കിൽ എങ്കിൽ (err)
		പോകുക errout;

	dbg_print_extent("ext_falloc initial", &ഇടത്_extent);
	next = ഇടത്_extent.e_lblk + ഇടത്_extent.e_len;
	എങ്കിൽ (ഇടത്_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = ഇടത്_extent.e_pblk - (ഇടത്_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ശൂന്യ,
					&ഇടത്_extent, start,
					min(len, ഇടത്_extent.e_lblk - start),
					goal);
		എങ്കിൽ (err)
			പോകുക errout;

		പോകുക start_again;
	} അല്ലെങ്കിൽ എങ്കിൽ (next >= start) {
		range_start = next;
		ഇടത്_adjacent = &ഇടത്_extent;
	} അല്ലെങ്കിൽ {
		range_start = start;
		ഇടത്_adjacent = ശൂന്യ;
	}
	goal = ഇടത്_extent.e_pblk + (range_start - ഇടത്_extent.e_lblk);

	ചെയ്യുക {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF,
					   &വലത്_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (പൂർണ്ണസംഖ്യ)err);
		dbg_print_extent("ext_falloc next", &വലത്_extent);
		/* Stop if we've seen this extent before */
		എങ്കിൽ (!err && വലത്_extent.e_lblk <= ഇടത്_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_NEXT;

		എങ്കിൽ (err && err != EXT2_ET_EXTENT_NO_NEXT)
			പോകുക errout;
		എങ്കിൽ (err == EXT2_ET_EXTENT_NO_NEXT ||
		    വലത്_extent.e_lblk > end + 1) {
			range_end = end;
			വലത്_adjacent = ശൂന്യ;
		} അല്ലെങ്കിൽ {
			/* Handle right_extent.e_lblk <= end */
			range_end = വലത്_extent.e_lblk - 1;
			വലത്_adjacent = &വലത്_extent;
		}
		goal_distance = range_start - next;
		എങ്കിൽ (err != EXT2_ET_EXTENT_NO_NEXT &&
		    goal_distance > (range_end - വലത്_extent.e_lblk))
			goal = വലത്_extent.e_pblk -
					(വലത്_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		എങ്കിൽ (range_start <= range_end) {
			എണ്ണം = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						ഇടത്_adjacent, വലത്_adjacent,
						range_start, എണ്ണം, goal);
			എങ്കിൽ (err)
				പോകുക errout;
		}

		എങ്കിൽ (range_end == end)
			നിർത്തുക;

		err = ext2fs_extent_goto(handle, വലത്_extent.e_lblk);
		എങ്കിൽ (err)
			പോകുക errout;
		next = വലത്_extent.e_lblk + വലത്_extent.e_len;
		ഇടത്_extent = വലത്_extent;
		ഇടത്_adjacent = &ഇടത്_extent;
		range_start = next;
		goal = ഇടത്_extent.e_pblk + (range_start - ഇടത്_extent.e_lblk);
	} ആകുമ്പോൾ (range_end < end);

errout:
	ext2fs_extent_free(handle);
	തിരികെ err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, പൂർണ്ണസംഖ്യ flags, ext2_ino_t ino,
			   ഘടന ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	ഘടന ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	പൂർണ്ണസംഖ്യ			zero_len = 0;
	errcode_t		err;

	എങ്കിൽ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	എങ്കിൽ (len > ext2fs_blocks_എണ്ണം(fs->super))
		തിരികെ EXT2_ET_BLOCK_ALLOC_FAIL;
	അല്ലെങ്കിൽ എങ്കിൽ (len == 0)
		തിരികെ 0;

	/* Read inode structure if necessary */
	എങ്കിൽ (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		എങ്കിൽ (err)
			തിരികെ err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	എങ്കിൽ (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		പോകുക out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	വേണ്ടി (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, ശൂന്യ, 0, blk, 0, &x);
		എങ്കിൽ (err)
			തിരികെ err;
		എങ്കിൽ (x)
			തുടരുക;

		err = ext2fs_bmap2(fs, ino, inode, ശൂന്യ, BMAP_ALLOC,
				   blk, 0, &x);
		എങ്കിൽ (err)
			പോകുക errout;
		എങ്കിൽ ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  ശൂന്യ, ശൂന്യ);
			zero_len = 0;
			എങ്കിൽ (err)
				പോകുക errout;
		}
		എങ്കിൽ (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} അല്ലെങ്കിൽ {
			zero_len++;
		}
		last = x;
	}

out:
	എങ്കിൽ (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	എങ്കിൽ (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, ശൂന്യ, ശൂന്യ);
	തിരികെ err;
}
