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

#અન્તર્ભાવ "config.h"

#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

#undef DEBUG

#ifdef DEBUG
# define dbg_printf(f, a...)  કરો {printf(f, ## a); fflush(stdout); } જ્યારે (0)
#નહીંતર
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

સ્થિર શૂન્ય dbg_print_extent(અચલ અક્ષર *desc EXT2FS_ATTR((unused)),
		અચલ સંરચના ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#ifdef DEBUG
	જો (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	જો (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	જો (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	જો (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	જો (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#endif
}

સ્થિર errcode_t claim_range(ext2_filsys fs, સંરચના ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	પાછા ext2fs_iblk_add_blocks(fs, inode, clusters);
}

સ્થિર errcode_t ext_falloc_helper(ext2_filsys fs,
				   પૂર્ણાંક flags,
				   ext2_ino_t ino,
				   સંરચના ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   સંરચના ext2fs_extent *ડાબું_ext,
				   સંરચના ext2fs_extent *જમણું_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	સંરચના ext2fs_extent	newex, ex;
	પૂર્ણાંક			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#ifdef DEBUG
	printf("%s: ", __func__);
	જો (ડાબું_ext)
		printf("left_ext=%llu--%llu, ", ડાબું_ext->e_lblk,
		       ડાબું_ext->e_lblk + ડાબું_ext->e_len - 1);
	જો (જમણું_ext)
		printf("right_ext=%llu--%llu, ", જમણું_ext->e_lblk,
		       જમણું_ext->e_lblk + જમણું_ext->e_len - 1);
	printf("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	fflush(stdout);
#endif
	/* Can't create initialized extents past EOF? */
	જો (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_SIZE(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	જો (ડાબું_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		જો (ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ડાબું_ext->e_len;
		નહીંતર
			fillable = max_init_len - ડાબું_ext->e_len;

		જો (fillable > range_len)
			fillable = range_len;
		જો (fillable == 0)
			જાઓ expand_જમણું;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		જો (cluster_fill == 0)
			જાઓ expand_જમણું;

		જો (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		જો (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = ડાબું_ext->e_lblk + ડાબું_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			જો (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			જો (cluster_fill == 0)
				જાઓ expand_જમણું;
		}

		err = ext2fs_extent_goto(handle, ડાબું_ext->e_lblk);
		જો (err)
			જાઓ expand_જમણું;
		ડાબું_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", ડાબું_ext);
		err = ext2fs_extent_replace(handle, 0, ડાબું_ext);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_fix_parents(handle);
		જો (err)
			જાઓ out;

		/* Zero blocks */
		જો (!(ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ડાબું_ext->e_pblk +
						  ડાબું_ext->e_len -
						  cluster_fill, cluster_fill,
						  ખાલી, ખાલી);
			જો (err)
				જાઓ out;
		}
	}

expand_જમણું:
	/* We must lengthen the right extent to the beginning of the cluster */
	જો (જમણું_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		જો (જમણું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - જમણું_ext->e_len;
		નહીંતર
			fillable = max_init_len - જમણું_ext->e_len;

		જો (fillable > range_len)
			fillable = range_len;
		જો (fillable == 0)
			જાઓ try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = જમણું_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		જો (cluster_fill == 0)
			જાઓ try_merge;

		err = ext2fs_extent_goto(handle, જમણું_ext->e_lblk);
		જો (err)
			જાઓ out;

		જો (cluster_fill > fillable)
			cluster_fill = fillable;
		જમણું_ext->e_lblk -= cluster_fill;
		જમણું_ext->e_pblk -= cluster_fill;
		જમણું_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", જમણું_ext);
		err = ext2fs_extent_replace(handle, 0, જમણું_ext);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_fix_parents(handle);
		જો (err)
			જાઓ out;

		/* Zero blocks if necessary */
		જો (!(જમણું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, જમણું_ext->e_pblk,
						  cluster_fill, ખાલી, ખાલી);
			જો (err)
				જાઓ out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	જો (ડાબું_ext && જમણું_ext) {
		/* Are the two extents mergeable? */
		જો ((ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (જમણું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			જાઓ try_ડાબું;

		/* User requires init/uninit but extent is uninit/init. */
		જો (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			જાઓ try_ડાબું;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		જો (!(ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			જાઓ try_ડાબું;

		/* Will it even fit? */
		x = ડાબું_ext->e_len + range_len + જમણું_ext->e_len;
		જો (x > (ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			જાઓ try_ડાબું;

		/* Would they even be physically contiguous if merged? */
		જો (ડાબું_ext->e_pblk + ડાબું_ext->e_len + range_len !=
				જમણું_ext->e_pblk)
			જાઓ try_ડાબું;

		err = ext2fs_extent_goto(handle, ડાબું_ext->e_lblk);
		જો (err)
			જાઓ try_ડાબું;

		/* Allocate blocks */
		y = ડાબું_ext->e_pblk + ડાબું_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_LENGTH, y,
				       જમણું_ext->e_pblk - y + 1, ખાલી,
				       &pblk, &plen);
		જો (err)
			જાઓ try_ડાબું;
		જો (pblk + plen != જમણું_ext->e_pblk)
			જાઓ try_ડાબું;
		err = claim_range(fs, inode, pblk, plen);
		જો (err)
			જાઓ out;

		/* Modify extents */
		ડાબું_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", ડાબું_ext);
		err = ext2fs_extent_replace(handle, 0, ડાબું_ext);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_fix_parents(handle);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF, &newex);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_delete(handle, 0);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_fix_parents(handle);
		જો (err)
			જાઓ out;
		*જમણું_ext = *ડાબું_ext;

		/* Zero blocks */
		જો (!(ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  ખાલી, ખાલી);
			જો (err)
				જાઓ out;
		}

		પાછા 0;
	}

try_ડાબું:
	/* Extend the left extent */
	જો (ડાબું_ext) {
		/* How many more blocks can be attached to left_ext? */
		જો (ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ડાબું_ext->e_len;
		નહીંતર જો (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - ડાબું_ext->e_len;
		નહીંતર
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		જો (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			જાઓ try_જમણું;

		જો (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = ડાબું_ext->e_lblk + ડાબું_ext->e_len - 1;
		જો (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			જો (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		જો (fillable == 0)
			જાઓ try_જમણું;

		/* Test if the right edge of the range is already mapped? */
		જો (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			જો (err)
				જાઓ out;
			જો (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			જો (fillable == 0)
				જાઓ try_જમણું;
		}

		/* Allocate range of blocks */
		x = ડાબું_ext->e_pblk + ડાબું_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				x, fillable, ખાલી, &pblk, &plen);
		જો (err)
			જાઓ try_જમણું;
		err = claim_range(fs, inode, pblk, plen);
		જો (err)
			જાઓ out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, ડાબું_ext->e_lblk);
		જો (err)
			જાઓ out;
		range_start += plen;
		range_len -= plen;
		ડાબું_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", ડાબું_ext);
		err = ext2fs_extent_replace(handle, 0, ડાબું_ext);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_fix_parents(handle);
		જો (err)
			જાઓ out;

		/* Zero blocks if necessary */
		જો (!(ડાબું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ખાલી, ખાલી);
			જો (err)
				જાઓ out;
		}
	}

try_જમણું:
	/* Extend the right extent */
	જો (જમણું_ext) {
		/* How much can we attach to right_ext? */
		જો (જમણું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - જમણું_ext->e_len;
		નહીંતર જો (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - જમણું_ext->e_len;
		નહીંતર
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		જો (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (જમણું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(જમણું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			જાઓ try_anywhere;

		જો (fillable > range_len)
			fillable = range_len;
		જો (fillable == 0)
			જાઓ try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		જો (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					જમણું_ext->e_lblk - fillable, &pblk);
			જો (err)
				જાઓ out;
			જો (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((જમણું_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			જો (fillable == 0)
				જાઓ try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_LENGTH,
				જમણું_ext->e_pblk - fillable,
				fillable, ખાલી, &pblk, &plen);
		જો (err)
			જાઓ try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		જો (err)
			જાઓ out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, જમણું_ext->e_lblk);
		જો (err)
			જાઓ out;
		range_len -= plen;
		જમણું_ext->e_lblk -= plen;
		જમણું_ext->e_pblk -= plen;
		જમણું_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", જમણું_ext);
		err = ext2fs_extent_replace(handle, 0, જમણું_ext);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_fix_parents(handle);
		જો (err)
			જાઓ out;

		/* Zero blocks if necessary */
		જો (!(જમણું_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, ખાલી, ખાલી);
			જો (err)
				જાઓ out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	જો (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		જો (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		જો (err)
			જાઓ out;
		જો (pblk == 0)
			જાઓ try_જમણું_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		જો (err == EXT2_ET_NO_CURRENT_ગાંઠ)
			ex.e_lblk = 0;
		નહીંતર જો (err)
			જાઓ out;

		જો (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		નહીંતર
			op = EXT2_EXTENT_ઉમેરો_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ઉમેરો(handle, op, &newex);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_fix_parents(handle);
		જો (err)
			જાઓ out;

		જો (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ખાલી, ખાલી);
			જો (err)
				જાઓ out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_જમણું_implied:
	y = range_start + range_len;
	જો (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		જો (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		જો (err)
			જાઓ out;
		જો (pblk == 0)
			જાઓ no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		જો (err == EXT2_ET_NO_CURRENT_ગાંઠ)
			ex.e_lblk = 0;
		નહીંતર જો (err)
			જાઓ out;

		જો (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		નહીંતર
			op = EXT2_EXTENT_ઉમેરો_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ઉમેરો(handle, op, &newex);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_fix_parents(handle);
		જો (err)
			જાઓ out;

		જો (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ખાલી, ખાલી);
			જો (err)
				જાઓ out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	જો (range_len == 0)
		પાછા 0;

	newex.e_lblk = range_start;
	જો (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} નહીંતર {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	માટે (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       ખાલી, &pblk, &plen);
		જો (err)
			જાઓ out;
		err = claim_range(fs, inode, pblk, plen);
		જો (err)
			જાઓ out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		જો (err == EXT2_ET_NO_CURRENT_ગાંઠ)
			ex.e_lblk = 0;
		નહીંતર જો (err)
			જાઓ out;

		જો (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		નહીંતર
			op = EXT2_EXTENT_ઉમેરો_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_ઉમેરો(handle, op, &newex);
		જો (err)
			જાઓ out;
		err = ext2fs_extent_fix_parents(handle);
		જો (err)
			જાઓ out;

		જો (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ખાલી, ખાલી);
			જો (err)
				જાઓ out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		જો (pblk >= ext2fs_blocks_ગણતરી(fs->super))
			pblk = fs->super->s_first_data_block;
	}

out:
	પાછા err;
}

સ્થિર errcode_t extent_fallocate(ext2_filsys fs, પૂર્ણાંક flags, ext2_ino_t ino,
				      સંરચના ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	સંરચના ext2fs_extent	ડાબું_extent, જમણું_extent;
	સંરચના ext2fs_extent	*ડાબું_adjacent, *જમણું_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, next;
	blk64_t			ગણતરી, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	જો (err)
		પાછા err;

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
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &ડાબું_extent);
	જો (err == EXT2_ET_NO_CURRENT_ગાંઠ) {
		blk64_t max_blocks = ext2fs_blocks_ગણતરી(fs->super);

		જો (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ખાલી,
					ખાલી, start, len, goal);
		જાઓ errout;
	} નહીંતર જો (err)
		જાઓ errout;

	dbg_print_extent("ext_falloc initial", &ડાબું_extent);
	next = ડાબું_extent.e_lblk + ડાબું_extent.e_len;
	જો (ડાબું_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = ડાબું_extent.e_pblk - (ડાબું_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ખાલી,
					&ડાબું_extent, start,
					min(len, ડાબું_extent.e_lblk - start),
					goal);
		જો (err)
			જાઓ errout;

		જાઓ start_again;
	} નહીંતર જો (next >= start) {
		range_start = next;
		ડાબું_adjacent = &ડાબું_extent;
	} નહીંતર {
		range_start = start;
		ડાબું_adjacent = ખાલી;
	}
	goal = ડાબું_extent.e_pblk + (range_start - ડાબું_extent.e_lblk);

	કરો {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_NEXT_LEAF,
					   &જમણું_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (પૂર્ણાંક)err);
		dbg_print_extent("ext_falloc next", &જમણું_extent);
		/* Stop if we've seen this extent before */
		જો (!err && જમણું_extent.e_lblk <= ડાબું_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_NEXT;

		જો (err && err != EXT2_ET_EXTENT_NO_NEXT)
			જાઓ errout;
		જો (err == EXT2_ET_EXTENT_NO_NEXT ||
		    જમણું_extent.e_lblk > end + 1) {
			range_end = end;
			જમણું_adjacent = ખાલી;
		} નહીંતર {
			/* Handle right_extent.e_lblk <= end */
			range_end = જમણું_extent.e_lblk - 1;
			જમણું_adjacent = &જમણું_extent;
		}
		goal_distance = range_start - next;
		જો (err != EXT2_ET_EXTENT_NO_NEXT &&
		    goal_distance > (range_end - જમણું_extent.e_lblk))
			goal = જમણું_extent.e_pblk -
					(જમણું_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		જો (range_start <= range_end) {
			ગણતરી = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						ડાબું_adjacent, જમણું_adjacent,
						range_start, ગણતરી, goal);
			જો (err)
				જાઓ errout;
		}

		જો (range_end == end)
			તોડો;

		err = ext2fs_extent_goto(handle, જમણું_extent.e_lblk);
		જો (err)
			જાઓ errout;
		next = જમણું_extent.e_lblk + જમણું_extent.e_len;
		ડાબું_extent = જમણું_extent;
		ડાબું_adjacent = &ડાબું_extent;
		range_start = next;
		goal = ડાબું_extent.e_pblk + (range_start - ડાબું_extent.e_lblk);
	} જ્યારે (range_end < end);

errout:
	ext2fs_extent_free(handle);
	પાછા err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, પૂર્ણાંક flags, ext2_ino_t ino,
			   સંરચના ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	સંરચના ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	પૂર્ણાંક			zero_len = 0;
	errcode_t		err;

	જો (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		પાછા EXT2_ET_INVALID_ARGUMENT;

	જો (len > ext2fs_blocks_ગણતરી(fs->super))
		પાછા EXT2_ET_BLOCK_ALLOC_FAIL;
	નહીંતર જો (len == 0)
		પાછા 0;

	/* Read inode structure if necessary */
	જો (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		જો (err)
			પાછા err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	જો (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		જાઓ out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	માટે (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, ખાલી, 0, blk, 0, &x);
		જો (err)
			પાછા err;
		જો (x)
			ચાલુ;

		err = ext2fs_bmap2(fs, ino, inode, ખાલી, BMAP_ALLOC,
				   blk, 0, &x);
		જો (err)
			જાઓ errout;
		જો ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  ખાલી, ખાલી);
			zero_len = 0;
			જો (err)
				જાઓ errout;
		}
		જો (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} નહીંતર {
			zero_len++;
		}
		last = x;
	}

out:
	જો (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	જો (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, ખાલી, ખાલી);
	પાછા err;
}
