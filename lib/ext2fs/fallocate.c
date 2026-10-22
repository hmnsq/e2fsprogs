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

#అన్తర్భావ "config.h"

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

స్థిర శూన్య dbg_print_extent(స్థిరరాశి అక్షరం *desc EXT2FS_ATTR((వాడని)),
		స్థిరరాశి నిర్మాణం ext2fs_extent *extent EXT2FS_ATTR((వాడని)))
{
#నిర్వచితమైతే DEBUG
	ఒకవేళ (desc)
		రాయి("%s: ", desc);
	రాయి("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_ఆకు)
		fputs("LEAF ", stdout);
	ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_రెండవ_సందర్శించు)
		fputs("2ND_VISIT ", stdout);
	ఒకవేళ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);
	fflush(stdout);
#అయితే_ముగింపు
}

స్థిర errcode_t claim_range(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2_FS_CLUSTER_నిష్పత్తి(fs) - 1) /
		   EXT2_FS_CLUSTER_నిష్పత్తి(fs);
	ext2fs_ఖండం_alloc_stats_range(fs, blk,
			clusters * EXT2_FS_CLUSTER_నిష్పత్తి(fs), +1);
	తిరిగి ext2fs_iblk_కూడు_ఖండాలు(fs, inode, clusters);
}

స్థిర errcode_t ext_falloc_helper(ext2_filsys fs,
				   పూర్ణాంకం flags,
				   ext2_ino_t ino,
				   నిర్మాణం ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   నిర్మాణం ext2fs_extent *ఎడమ_ext,
				   నిర్మాణం ext2fs_extent *కుడి_ext,
				   blk64_t range_ప్రారంభం, blk64_t range_len,
				   blk64_t alloc_goal)
{
	నిర్మాణం ext2fs_extent	newex, ex;
	పూర్ణాంకం			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#నిర్వచితమైతే DEBUG
	రాయి("%s: ", __func__);
	ఒకవేళ (ఎడమ_ext)
		రాయి("left_ext=%llu--%llu, ", ఎడమ_ext->e_lblk,
		       ఎడమ_ext->e_lblk + ఎడమ_ext->e_len - 1);
	ఒకవేళ (కుడి_ext)
		రాయి("right_ext=%llu--%llu, ", కుడి_ext->e_lblk,
		       కుడి_ext->e_lblk + కుడి_ext->e_len - 1);
	రాయి("start=%llu len=%llu, goal=%llu\n", range_ప్రారంభం, range_len,
	       alloc_goal);
	fflush(stdout);
#అయితే_ముగింపు
	/* Can't create initialized extents past EOF? */
	ఒకవేళ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_కొలత(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2_FS_CLUSTER_ముసుగు(fs)) |
		     (range_ప్రారంభం & EXT2_FS_CLUSTER_ముసుగు(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2_FS_CLUSTER_ముసుగు(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2_FS_CLUSTER_ముసుగు(fs);

	/* We must lengthen the left extent to the end of the cluster */
	ఒకవేళ (ఎడమ_ext && EXT2_FS_CLUSTER_నిష్పత్తి(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		ఒకవేళ (ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ఎడమ_ext->e_len;
		లేకపోతే
			fillable = max_init_len - ఎడమ_ext->e_len;

		ఒకవేళ (fillable > range_len)
			fillable = range_len;
		ఒకవేళ (fillable == 0)
			వెళ్ళు expand_కుడి;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2_FS_CLUSTER_నిష్పత్తి(fs) -
			       (range_ప్రారంభం & EXT2_FS_CLUSTER_ముసుగు(fs));
		cluster_fill &= EXT2_FS_CLUSTER_ముసుగు(fs);
		ఒకవేళ (cluster_fill == 0)
			వెళ్ళు expand_కుడి;

		ఒకవేళ (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		ఒకవేళ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = ఎడమ_ext->e_lblk + ఎడమ_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			ఒకవేళ (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			ఒకవేళ (cluster_fill == 0)
				వెళ్ళు expand_కుడి;
		}

		err = ext2fs_extent_goto(handle, ఎడమ_ext->e_lblk);
		ఒకవేళ (err)
			వెళ్ళు expand_కుడి;
		ఎడమ_ext->e_len += cluster_fill;
		range_ప్రారంభం += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", ఎడమ_ext);
		err = ext2fs_extent_replace(handle, 0, ఎడమ_ext);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		/* Zero blocks */
		ఒకవేళ (!(ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, ఎడమ_ext->e_pblk +
						  ఎడమ_ext->e_len -
						  cluster_fill, cluster_fill,
						  ఖాళీ, ఖాళీ);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
		}
	}

expand_కుడి:
	/* We must lengthen the right extent to the beginning of the cluster */
	ఒకవేళ (కుడి_ext && EXT2_FS_CLUSTER_నిష్పత్తి(fs) > 1) {
		/* How much can we attach to right_ext? */
		ఒకవేళ (కుడి_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - కుడి_ext->e_len;
		లేకపోతే
			fillable = max_init_len - కుడి_ext->e_len;

		ఒకవేళ (fillable > range_len)
			fillable = range_len;
		ఒకవేళ (fillable == 0)
			వెళ్ళు try_విలీనం;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = కుడి_ext->e_lblk & EXT2_FS_CLUSTER_ముసుగు(fs);
		ఒకవేళ (cluster_fill == 0)
			వెళ్ళు try_విలీనం;

		err = ext2fs_extent_goto(handle, కుడి_ext->e_lblk);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (cluster_fill > fillable)
			cluster_fill = fillable;
		కుడి_ext->e_lblk -= cluster_fill;
		కుడి_ext->e_pblk -= cluster_fill;
		కుడి_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", కుడి_ext);
		err = ext2fs_extent_replace(handle, 0, కుడి_ext);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		/* Zero blocks if necessary */
		ఒకవేళ (!(కుడి_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, కుడి_ext->e_pblk,
						  cluster_fill, ఖాళీ, ఖాళీ);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
		}
	}

try_విలీనం:
	/* Merge both extents together, perhaps? */
	ఒకవేళ (ఎడమ_ext && కుడి_ext) {
		/* Are the two extents mergeable? */
		ఒకవేళ ((ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (కుడి_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			వెళ్ళు try_ఎడమ;

		/* User requires init/uninit but extent is uninit/init. */
		ఒకవేళ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			వెళ్ళు try_ఎడమ;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		ఒకవేళ (!(ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_ఖండాలు) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			వెళ్ళు try_ఎడమ;

		/* Will it even fit? */
		x = ఎడమ_ext->e_len + range_len + కుడి_ext->e_len;
		ఒకవేళ (x > (ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			వెళ్ళు try_ఎడమ;

		/* Would they even be physically contiguous if merged? */
		ఒకవేళ (ఎడమ_ext->e_pblk + ఎడమ_ext->e_len + range_len !=
				కుడి_ext->e_pblk)
			వెళ్ళు try_ఎడమ;

		err = ext2fs_extent_goto(handle, ఎడమ_ext->e_lblk);
		ఒకవేళ (err)
			వెళ్ళు try_ఎడమ;

		/* Allocate blocks */
		y = ఎడమ_ext->e_pblk + ఎడమ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_నిడివి, y,
				       కుడి_ext->e_pblk - y + 1, ఖాళీ,
				       &pblk, &plen);
		ఒకవేళ (err)
			వెళ్ళు try_ఎడమ;
		ఒకవేళ (pblk + plen != కుడి_ext->e_pblk)
			వెళ్ళు try_ఎడమ;
		err = claim_range(fs, inode, pblk, plen);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		/* Modify extents */
		ఎడమ_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", ఎడమ_ext);
		err = ext2fs_extent_replace(handle, 0, ఎడమ_ext);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_తదుపరి_ఆకు, &newex);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_delete(handle, 0);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		*కుడి_ext = *ఎడమ_ext;

		/* Zero blocks */
		ఒకవేళ (!(ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ఖండాలు)) {
			err = ext2fs_zero_blocks2(fs, range_ప్రారంభం, range_len,
						  ఖాళీ, ఖాళీ);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
		}

		తిరిగి 0;
	}

try_ఎడమ:
	/* Extend the left extent */
	ఒకవేళ (ఎడమ_ext) {
		/* How many more blocks can be attached to left_ext? */
		ఒకవేళ (ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - ఎడమ_ext->e_len;
		లేకపోతే ఒకవేళ (flags & EXT2_FALLOCATE_ZERO_ఖండాలు)
			fillable = max_init_len - ఎడమ_ext->e_len;
		లేకపోతే
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ఒకవేళ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			వెళ్ళు try_కుడి;

		ఒకవేళ (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = ఎడమ_ext->e_lblk + ఎడమ_ext->e_len - 1;
		ఒకవేళ (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			ఒకవేళ (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		ఒకవేళ (fillable == 0)
			వెళ్ళు try_కుడి;

		/* Test if the right edge of the range is already mapped? */
		ఒకవేళ (EXT2_FS_CLUSTER_నిష్పత్తి(fs) > 1) {
			err = ext2fs_map_cluster_ఖండం(fs, ino, inode,
					x + fillable, &pblk);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
			ఒకవేళ (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2_FS_CLUSTER_ముసుగు(fs));
			ఒకవేళ (fillable == 0)
				వెళ్ళు try_కుడి;
		}

		/* Allocate range of blocks */
		x = ఎడమ_ext->e_pblk + ఎడమ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_నిడివి,
				x, fillable, ఖాళీ, &pblk, &plen);
		ఒకవేళ (err)
			వెళ్ళు try_కుడి;
		err = claim_range(fs, inode, pblk, plen);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, ఎడమ_ext->e_lblk);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		range_ప్రారంభం += plen;
		range_len -= plen;
		ఎడమ_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", ఎడమ_ext);
		err = ext2fs_extent_replace(handle, 0, ఎడమ_ext);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		/* Zero blocks if necessary */
		ఒకవేళ (!(ఎడమ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ఖండాలు)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ఖాళీ, ఖాళీ);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
		}
	}

try_కుడి:
	/* Extend the right extent */
	ఒకవేళ (కుడి_ext) {
		/* How much can we attach to right_ext? */
		ఒకవేళ (కుడి_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - కుడి_ext->e_len;
		లేకపోతే ఒకవేళ (flags & EXT2_FALLOCATE_ZERO_ఖండాలు)
			fillable = max_init_len - కుడి_ext->e_len;
		లేకపోతే
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		ఒకవేళ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (కుడి_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(కుడి_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			వెళ్ళు try_anywhere;

		ఒకవేళ (fillable > range_len)
			fillable = range_len;
		ఒకవేళ (fillable == 0)
			వెళ్ళు try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		ఒకవేళ (EXT2_FS_CLUSTER_నిష్పత్తి(fs) > 1) {
			err = ext2fs_map_cluster_ఖండం(fs, ino, inode,
					కుడి_ext->e_lblk - fillable, &pblk);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
			ఒకవేళ (pblk)
				fillable -= EXT2_FS_CLUSTER_నిష్పత్తి(fs) -
						((కుడి_ext->e_lblk - fillable)
						 & EXT2_FS_CLUSTER_ముసుగు(fs));
			ఒకవేళ (fillable == 0)
				వెళ్ళు try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_నిడివి,
				కుడి_ext->e_pblk - fillable,
				fillable, ఖాళీ, &pblk, &plen);
		ఒకవేళ (err)
			వెళ్ళు try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2_FS_CLUSTER_ముసుగు(fs),
			      plen + (pblk & EXT2_FS_CLUSTER_ముసుగు(fs)));
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, కుడి_ext->e_lblk);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		range_len -= plen;
		కుడి_ext->e_lblk -= plen;
		కుడి_ext->e_pblk -= plen;
		కుడి_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", కుడి_ext);
		err = ext2fs_extent_replace(handle, 0, కుడి_ext);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		/* Zero blocks if necessary */
		ఒకవేళ (!(కుడి_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ఖండాలు)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, ఖాళీ, ఖాళీ);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	ఒకవేళ (range_len > 0 && (range_ప్రారంభం & EXT2_FS_CLUSTER_ముసుగు(fs))) {
		cluster_fill = EXT2_FS_CLUSTER_నిష్పత్తి(fs) -
			       (range_ప్రారంభం & EXT2_FS_CLUSTER_ముసుగు(fs));
		cluster_fill &= EXT2_FS_CLUSTER_ముసుగు(fs);
		ఒకవేళ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_ప్రారంభం;
		err = ext2fs_map_cluster_ఖండం(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		ఒకవేళ (pblk == 0)
			వెళ్ళు try_కుడి_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ఒకవేళ (err == EXT2_ET_NO_CURRENT_కణుపు)
			ex.e_lblk = 0;
		లేకపోతే ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		లేకపోతే
			op = EXT2_EXTENT_చొప్పించు_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_చొప్పించు(handle, op, &newex);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ఖండాలు)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ఖాళీ, ఖాళీ);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
		}

		range_ప్రారంభం += cluster_fill;
		range_len -= cluster_fill;
	}

try_కుడి_implied:
	y = range_ప్రారంభం + range_len;
	ఒకవేళ (range_len > 0 && (y & EXT2_FS_CLUSTER_ముసుగు(fs))) {
		cluster_fill = y & EXT2_FS_CLUSTER_ముసుగు(fs);
		ఒకవేళ (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2_FS_CLUSTER_ముసుగు(fs);
		err = ext2fs_map_cluster_ఖండం(fs, ino, inode, newex.e_lblk,
					       &pblk);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		ఒకవేళ (pblk == 0)
			వెళ్ళు no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ఒకవేళ (err == EXT2_ET_NO_CURRENT_కణుపు)
			ex.e_lblk = 0;
		లేకపోతే ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		లేకపోతే
			op = EXT2_EXTENT_చొప్పించు_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_చొప్పించు(handle, op, &newex);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ఖండాలు)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, ఖాళీ, ఖాళీ);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
		}

		range_len -= cluster_fill;
	}

no_implied:
	ఒకవేళ (range_len == 0)
		తిరిగి 0;

	newex.e_lblk = range_ప్రారంభం;
	ఒకవేళ (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} లేకపోతే {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	కొరకు (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2_FS_CLUSTER_ముసుగు(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2_FS_CLUSTER_ముసుగు(fs),
				       fillable,
				       ఖాళీ, &pblk, &plen);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = claim_range(fs, inode, pblk, plen);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		ఒకవేళ (err == EXT2_ET_NO_CURRENT_కణుపు)
			ex.e_lblk = 0;
		లేకపోతే ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		లేకపోతే
			op = EXT2_EXTENT_చొప్పించు_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_చొప్పించు(handle, op, &newex);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
		err = ext2fs_extent_fix_parents(handle);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		ఒకవేళ (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_ఖండాలు)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, ఖాళీ, ఖాళీ);
			ఒకవేళ (err)
				వెళ్ళు బహిర్గతం;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		ఒకవేళ (pblk >= ext2fs_ఖండాలు_లెక్క(fs->super))
			pblk = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
	}

బహిర్గతం:
	తిరిగి err;
}

స్థిర errcode_t extent_fallocate(ext2_filsys fs, పూర్ణాంకం flags, ext2_ino_t ino,
				      నిర్మాణం ext2_inode *inode, blk64_t goal,
				      blk64_t ప్రారంభం, blk64_t len)
{
	ext2_extent_handle_t	handle;
	నిర్మాణం ext2fs_extent	ఎడమ_extent, కుడి_extent;
	నిర్మాణం ext2fs_extent	*ఎడమ_adjacent, *కుడి_adjacent;
	errcode_t		err;
	blk64_t			range_ప్రారంభం, range_చివర = 0, చివర, తదుపరి;
	blk64_t			లెక్క, goal_దూరం;

	చివర = ప్రారంభం + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	ఒకవేళ (err)
		తిరిగి err;

	/*
	 * Find the extent closest to the start of the alloc range.  We don't
	 * check the return value because _goto() sets the current node to the
	 * next-lowest extent if 'start' is in a hole; or the next-highest
	 * extent if there aren't any lower ones; or doesn't set a current node
	 * if there was a real error reading the extent tree.  In that case,
	 * _get() will error out.
	 */
ప్రారంభం_again:
	ext2fs_extent_goto(handle, ప్రారంభం);
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &ఎడమ_extent);
	ఒకవేళ (err == EXT2_ET_NO_CURRENT_కణుపు) {
		blk64_t max_ఖండాలు = ext2fs_ఖండాలు_లెక్క(fs->super);

		ఒకవేళ (goal == ~0ULL)
			goal = ext2fs_కనుగొను_inode_goal(fs, ino, inode, ప్రారంభం);
		err = ext2fs_కనుగొను_మొదటి_zero_ఖండం_bitmap2(fs->ఖండం_map,
						goal, max_ఖండాలు - 1, &goal);
		goal += ప్రారంభం;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ఖాళీ,
					ఖాళీ, ప్రారంభం, len, goal);
		వెళ్ళు errout;
	} లేకపోతే ఒకవేళ (err)
		వెళ్ళు errout;

	dbg_print_extent("ext_falloc initial", &ఎడమ_extent);
	తదుపరి = ఎడమ_extent.e_lblk + ఎడమ_extent.e_len;
	ఒకవేళ (ఎడమ_extent.e_lblk > ప్రారంభం) {
		/* The nearest extent we found was beyond start??? */
		goal = ఎడమ_extent.e_pblk - (ఎడమ_extent.e_lblk - ప్రారంభం);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, ఖాళీ,
					&ఎడమ_extent, ప్రారంభం,
					min(len, ఎడమ_extent.e_lblk - ప్రారంభం),
					goal);
		ఒకవేళ (err)
			వెళ్ళు errout;

		వెళ్ళు ప్రారంభం_again;
	} లేకపోతే ఒకవేళ (తదుపరి >= ప్రారంభం) {
		range_ప్రారంభం = తదుపరి;
		ఎడమ_adjacent = &ఎడమ_extent;
	} లేకపోతే {
		range_ప్రారంభం = ప్రారంభం;
		ఎడమ_adjacent = ఖాళీ;
	}
	goal = ఎడమ_extent.e_pblk + (range_ప్రారంభం - ఎడమ_extent.e_lblk);

	చేయి {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_తదుపరి_ఆకు,
					   &కుడి_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (పూర్ణాంకం)err);
		dbg_print_extent("ext_falloc next", &కుడి_extent);
		/* Stop if we've seen this extent before */
		ఒకవేళ (!err && కుడి_extent.e_lblk <= ఎడమ_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_తదుపరి;

		ఒకవేళ (err && err != EXT2_ET_EXTENT_NO_తదుపరి)
			వెళ్ళు errout;
		ఒకవేళ (err == EXT2_ET_EXTENT_NO_తదుపరి ||
		    కుడి_extent.e_lblk > చివర + 1) {
			range_చివర = చివర;
			కుడి_adjacent = ఖాళీ;
		} లేకపోతే {
			/* Handle right_extent.e_lblk <= end */
			range_చివర = కుడి_extent.e_lblk - 1;
			కుడి_adjacent = &కుడి_extent;
		}
		goal_దూరం = range_ప్రారంభం - తదుపరి;
		ఒకవేళ (err != EXT2_ET_EXTENT_NO_తదుపరి &&
		    goal_దూరం > (range_చివర - కుడి_extent.e_lblk))
			goal = కుడి_extent.e_pblk -
					(కుడి_extent.e_lblk - range_ప్రారంభం);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_ప్రారంభం, range_చివర);
		err = 0;
		ఒకవేళ (range_ప్రారంభం <= range_చివర) {
			లెక్క = range_చివర - range_ప్రారంభం + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						ఎడమ_adjacent, కుడి_adjacent,
						range_ప్రారంభం, లెక్క, goal);
			ఒకవేళ (err)
				వెళ్ళు errout;
		}

		ఒకవేళ (range_చివర == చివర)
			ఆపు;

		err = ext2fs_extent_goto(handle, కుడి_extent.e_lblk);
		ఒకవేళ (err)
			వెళ్ళు errout;
		తదుపరి = కుడి_extent.e_lblk + కుడి_extent.e_len;
		ఎడమ_extent = కుడి_extent;
		ఎడమ_adjacent = &ఎడమ_extent;
		range_ప్రారంభం = తదుపరి;
		goal = ఎడమ_extent.e_pblk + (range_ప్రారంభం - ఎడమ_extent.e_lblk);
	} అయితే (range_చివర < చివర);

errout:
	ext2fs_extent_free(handle);
	తిరిగి err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, పూర్ణాంకం flags, ext2_ino_t ino,
			   నిర్మాణం ext2_inode *inode, blk64_t goal,
			   blk64_t ప్రారంభం, blk64_t len)
{
	నిర్మాణం ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, చివరి = 0;
	పూర్ణాంకం			zero_len = 0;
	errcode_t		err;

	ఒకవేళ (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (len > ext2fs_ఖండాలు_లెక్క(fs->super))
		తిరిగి EXT2_ET_ఖండం_ALLOC_FAIL;
	లేకపోతే ఒకవేళ (len == 0)
		తిరిగి 0;

	/* Read inode structure if necessary */
	ఒకవేళ (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		ఒకవేళ (err)
			తిరిగి err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   ప్రారంభం, len, goal);

	ఒకవేళ (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, ప్రారంభం, len);
		వెళ్ళు బహిర్గతం;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	కొరకు (blk = ప్రారంభం; blk < ప్రారంభం + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, ఖాళీ, 0, blk, 0, &x);
		ఒకవేళ (err)
			తిరిగి err;
		ఒకవేళ (x)
			కొనసాగు;

		err = ext2fs_bmap2(fs, ino, inode, ఖాళీ, BMAP_ALLOC,
				   blk, 0, &x);
		ఒకవేళ (err)
			వెళ్ళు errout;
		ఒకవేళ ((zero_len && (x != చివరి+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  ఖాళీ, ఖాళీ);
			zero_len = 0;
			ఒకవేళ (err)
				వెళ్ళు errout;
		}
		ఒకవేళ (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} లేకపోతే {
			zero_len++;
		}
		చివరి = x;
	}

బహిర్గతం:
	ఒకవేళ (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	ఒకవేళ (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, ఖాళీ, ఖాళీ);
	తిరిగి err;
}
