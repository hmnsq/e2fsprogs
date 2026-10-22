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

#समावेश "config.h"

#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#घोषणा min(a, b) ((a) < (b) ? (a) : (b))

#अघोषित DEBUG

#अगर_घोषित DEBUG
# define dbg_printf(f, a...)  करो {म_लिखो(f, ## a); ख_साफ(मानक_निकास); } जबतक (0)
#अन्यथा
# define dbg_printf(f, a...)
#पूर्ण_अगर

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

अटल व्योम dbg_print_extent(स्थिर अक्षर *desc EXT2FS_ATTR((unused)),
		स्थिर काष्ठा ext2fs_extent *extent EXT2FS_ATTR((unused)))
{
#अगर_घोषित DEBUG
	अगर (desc)
		म_लिखो("%s: ", desc);
	म_लिखो("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	अगर (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		ख_माला_दो("LEAF ", मानक_निकास);
	अगर (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		ख_माला_दो("UNINIT ", मानक_निकास);
	अगर (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		ख_माला_दो("2ND_VISIT ", मानक_निकास);
	अगर (!extent->e_flags)
		ख_माला_दो("(none)", मानक_निकास);
	ख_अक्षर_दो('\n', मानक_निकास);
	ख_साफ(मानक_निकास);
#पूर्ण_अगर
}

अटल errcode_t claim_range(ext2_filsys fs, काष्ठा ext2_inode *inode,
			     blk64_t blk, blk64_t len)
{
	blk64_t	clusters;

	clusters = (len + EXT2FS_CLUSTER_RATIO(fs) - 1) /
		   EXT2FS_CLUSTER_RATIO(fs);
	ext2fs_block_alloc_stats_range(fs, blk,
			clusters * EXT2FS_CLUSTER_RATIO(fs), +1);
	वापस ext2fs_iblk_add_blocks(fs, inode, clusters);
}

अटल errcode_t ext_falloc_helper(ext2_filsys fs,
				   पूर्णांक flags,
				   ext2_ino_t ino,
				   काष्ठा ext2_inode *inode,
				   ext2_extent_handle_t handle,
				   काष्ठा ext2fs_extent *बायाँ_ext,
				   काष्ठा ext2fs_extent *दायाँ_ext,
				   blk64_t range_start, blk64_t range_len,
				   blk64_t alloc_goal)
{
	काष्ठा ext2fs_extent	newex, ex;
	पूर्णांक			op;
	blk64_t			fillable, pblk, plen, x, y;
	blk64_t			eof_blk = 0, cluster_fill = 0;
	errcode_t		err;
	blk_t			max_extent_len, max_uninit_len, max_init_len;

#अगर_घोषित DEBUG
	म_लिखो("%s: ", __func__);
	अगर (बायाँ_ext)
		म_लिखो("left_ext=%llu--%llu, ", बायाँ_ext->e_lblk,
		       बायाँ_ext->e_lblk + बायाँ_ext->e_len - 1);
	अगर (दायाँ_ext)
		म_लिखो("right_ext=%llu--%llu, ", दायाँ_ext->e_lblk,
		       दायाँ_ext->e_lblk + दायाँ_ext->e_len - 1);
	म_लिखो("start=%llu len=%llu, goal=%llu\n", range_start, range_len,
	       alloc_goal);
	ख_साफ(मानक_निकास);
#पूर्ण_अगर
	/* Can't create initialized extents past EOF? */
	अगर (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF))
		eof_blk = EXT2_I_आकार(inode) / fs->blocksize;

	/* The allocation goal must be as far into a cluster as range_start. */
	alloc_goal = (alloc_goal & ~EXT2FS_CLUSTER_MASK(fs)) |
		     (range_start & EXT2FS_CLUSTER_MASK(fs));

	max_uninit_len = EXT_UNINIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);
	max_init_len = EXT_INIT_MAX_LEN & ~EXT2FS_CLUSTER_MASK(fs);

	/* We must lengthen the left extent to the end of the cluster */
	अगर (बायाँ_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How many more blocks can be attached to left_ext? */
		अगर (बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - बायाँ_ext->e_len;
		अन्यथा
			fillable = max_init_len - बायाँ_ext->e_len;

		अगर (fillable > range_len)
			fillable = range_len;
		अगर (fillable == 0)
			जाओ expand_दायाँ;

		/*
		 * If range_start isn't on a cluster boundary, try an
		 * implied cluster allocation for left_ext.
		 */
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		अगर (cluster_fill == 0)
			जाओ expand_दायाँ;

		अगर (cluster_fill > fillable)
			cluster_fill = fillable;

		/* Don't expand an initialized left_ext beyond EOF */
		अगर (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			x = बायाँ_ext->e_lblk + बायाँ_ext->e_len - 1;
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + cluster_fill, eof_blk);
			अगर (eof_blk >= x && eof_blk <= x + cluster_fill)
				cluster_fill = eof_blk - x;
			अगर (cluster_fill == 0)
				जाओ expand_दायाँ;
		}

		err = ext2fs_extent_goto(handle, बायाँ_ext->e_lblk);
		अगर (err)
			जाओ expand_दायाँ;
		बायाँ_ext->e_len += cluster_fill;
		range_start += cluster_fill;
		range_len -= cluster_fill;
		alloc_goal += cluster_fill;

		dbg_print_extent("ext_falloc clus left+", बायाँ_ext);
		err = ext2fs_extent_replace(handle, 0, बायाँ_ext);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_fix_parents(handle);
		अगर (err)
			जाओ out;

		/* Zero blocks */
		अगर (!(बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, बायाँ_ext->e_pblk +
						  बायाँ_ext->e_len -
						  cluster_fill, cluster_fill,
						  शून्य, शून्य);
			अगर (err)
				जाओ out;
		}
	}

expand_दायाँ:
	/* We must lengthen the right extent to the beginning of the cluster */
	अगर (दायाँ_ext && EXT2FS_CLUSTER_RATIO(fs) > 1) {
		/* How much can we attach to right_ext? */
		अगर (दायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - दायाँ_ext->e_len;
		अन्यथा
			fillable = max_init_len - दायाँ_ext->e_len;

		अगर (fillable > range_len)
			fillable = range_len;
		अगर (fillable == 0)
			जाओ try_merge;

		/*
		 * If range_end isn't on a cluster boundary, try an implied
		 * cluster allocation for right_ext.
		 */
		cluster_fill = दायाँ_ext->e_lblk & EXT2FS_CLUSTER_MASK(fs);
		अगर (cluster_fill == 0)
			जाओ try_merge;

		err = ext2fs_extent_goto(handle, दायाँ_ext->e_lblk);
		अगर (err)
			जाओ out;

		अगर (cluster_fill > fillable)
			cluster_fill = fillable;
		दायाँ_ext->e_lblk -= cluster_fill;
		दायाँ_ext->e_pblk -= cluster_fill;
		दायाँ_ext->e_len += cluster_fill;
		range_len -= cluster_fill;

		dbg_print_extent("ext_falloc clus right+", दायाँ_ext);
		err = ext2fs_extent_replace(handle, 0, दायाँ_ext);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_fix_parents(handle);
		अगर (err)
			जाओ out;

		/* Zero blocks if necessary */
		अगर (!(दायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) {
			err = ext2fs_zero_blocks2(fs, दायाँ_ext->e_pblk,
						  cluster_fill, शून्य, शून्य);
			अगर (err)
				जाओ out;
		}
	}

try_merge:
	/* Merge both extents together, perhaps? */
	अगर (बायाँ_ext && दायाँ_ext) {
		/* Are the two extents mergeable? */
		अगर ((बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) !=
		    (दायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT))
			जाओ try_बायाँ;

		/* User requires init/uninit but extent is uninit/init. */
		अगर (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			जाओ try_बायाँ;

		/*
		 * Skip initialized extent unless user wants to zero blocks
		 * or requires init extent.
		 */
		अगर (!(बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (!(flags & EXT2_FALLOCATE_ZERO_BLOCKS) ||
		     !(flags & EXT2_FALLOCATE_FORCE_INIT)))
			जाओ try_बायाँ;

		/* Will it even fit? */
		x = बायाँ_ext->e_len + range_len + दायाँ_ext->e_len;
		अगर (x > (बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT ?
				max_uninit_len : max_init_len))
			जाओ try_बायाँ;

		/* Would they even be physically contiguous if merged? */
		अगर (बायाँ_ext->e_pblk + बायाँ_ext->e_len + range_len !=
				दायाँ_ext->e_pblk)
			जाओ try_बायाँ;

		err = ext2fs_extent_goto(handle, बायाँ_ext->e_lblk);
		अगर (err)
			जाओ try_बायाँ;

		/* Allocate blocks */
		y = बायाँ_ext->e_pblk + बायाँ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				       EXT2_NEWRANGE_MIN_लंबाई, y,
				       दायाँ_ext->e_pblk - y + 1, शून्य,
				       &pblk, &plen);
		अगर (err)
			जाओ try_बायाँ;
		अगर (pblk + plen != दायाँ_ext->e_pblk)
			जाओ try_बायाँ;
		err = claim_range(fs, inode, pblk, plen);
		अगर (err)
			जाओ out;

		/* Modify extents */
		बायाँ_ext->e_len = x;
		dbg_print_extent("ext_falloc merge", बायाँ_ext);
		err = ext2fs_extent_replace(handle, 0, बायाँ_ext);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_fix_parents(handle);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र_LEAF, &newex);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_delete(handle, 0);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_fix_parents(handle);
		अगर (err)
			जाओ out;
		*दायाँ_ext = *बायाँ_ext;

		/* Zero blocks */
		अगर (!(बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, range_start, range_len,
						  शून्य, शून्य);
			अगर (err)
				जाओ out;
		}

		वापस 0;
	}

try_बायाँ:
	/* Extend the left extent */
	अगर (बायाँ_ext) {
		/* How many more blocks can be attached to left_ext? */
		अगर (बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - बायाँ_ext->e_len;
		अन्यथा अगर (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - बायाँ_ext->e_len;
		अन्यथा
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		अगर (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			जाओ try_दायाँ;

		अगर (fillable > range_len)
			fillable = range_len;

		/* Don't expand an initialized left_ext beyond EOF */
		x = बायाँ_ext->e_lblk + बायाँ_ext->e_len - 1;
		अगर (!(flags & EXT2_FALLOCATE_INIT_BEYOND_EOF)) {
			dbg_printf("%s: lend=%llu newlend=%llu eofblk=%llu\n",
				   __func__, x, x + fillable, eof_blk);
			अगर (eof_blk >= x && eof_blk <= x + fillable)
				fillable = eof_blk - x;
		}

		अगर (fillable == 0)
			जाओ try_दायाँ;

		/* Test if the right edge of the range is already mapped? */
		अगर (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					x + fillable, &pblk);
			अगर (err)
				जाओ out;
			अगर (pblk)
				fillable -= 1 + ((x + fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			अगर (fillable == 0)
				जाओ try_दायाँ;
		}

		/* Allocate range of blocks */
		x = बायाँ_ext->e_pblk + बायाँ_ext->e_len;
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_लंबाई,
				x, fillable, शून्य, &pblk, &plen);
		अगर (err)
			जाओ try_दायाँ;
		err = claim_range(fs, inode, pblk, plen);
		अगर (err)
			जाओ out;

		/* Modify left_ext */
		err = ext2fs_extent_goto(handle, बायाँ_ext->e_lblk);
		अगर (err)
			जाओ out;
		range_start += plen;
		range_len -= plen;
		बायाँ_ext->e_len += plen;
		dbg_print_extent("ext_falloc left+", बायाँ_ext);
		err = ext2fs_extent_replace(handle, 0, बायाँ_ext);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_fix_parents(handle);
		अगर (err)
			जाओ out;

		/* Zero blocks if necessary */
		अगर (!(बायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, शून्य, शून्य);
			अगर (err)
				जाओ out;
		}
	}

try_दायाँ:
	/* Extend the right extent */
	अगर (दायाँ_ext) {
		/* How much can we attach to right_ext? */
		अगर (दायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			fillable = max_uninit_len - दायाँ_ext->e_len;
		अन्यथा अगर (flags & EXT2_FALLOCATE_ZERO_BLOCKS)
			fillable = max_init_len - दायाँ_ext->e_len;
		अन्यथा
			fillable = 0;

		/* User requires init/uninit but extent is uninit/init. */
		अगर (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
		     (दायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)) ||
		    ((flags & EXT2_FALLOCATE_FORCE_UNINIT) &&
		     !(दायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT)))
			जाओ try_anywhere;

		अगर (fillable > range_len)
			fillable = range_len;
		अगर (fillable == 0)
			जाओ try_anywhere;

		/* Test if the left edge of the range is already mapped? */
		अगर (EXT2FS_CLUSTER_RATIO(fs) > 1) {
			err = ext2fs_map_cluster_block(fs, ino, inode,
					दायाँ_ext->e_lblk - fillable, &pblk);
			अगर (err)
				जाओ out;
			अगर (pblk)
				fillable -= EXT2FS_CLUSTER_RATIO(fs) -
						((दायाँ_ext->e_lblk - fillable)
						 & EXT2FS_CLUSTER_MASK(fs));
			अगर (fillable == 0)
				जाओ try_anywhere;
		}

		/*
		 * FIXME: It would be nice if we could handle allocating a
		 * variable range from a fixed end point instead of just
		 * skipping to the general allocator if the whole range is
		 * unavailable.
		 */
		err = ext2fs_new_range(fs, EXT2_NEWRANGE_FIXED_GOAL |
				EXT2_NEWRANGE_MIN_लंबाई,
				दायाँ_ext->e_pblk - fillable,
				fillable, शून्य, &pblk, &plen);
		अगर (err)
			जाओ try_anywhere;
		err = claim_range(fs, inode,
			      pblk & ~EXT2FS_CLUSTER_MASK(fs),
			      plen + (pblk & EXT2FS_CLUSTER_MASK(fs)));
		अगर (err)
			जाओ out;

		/* Modify right_ext */
		err = ext2fs_extent_goto(handle, दायाँ_ext->e_lblk);
		अगर (err)
			जाओ out;
		range_len -= plen;
		दायाँ_ext->e_lblk -= plen;
		दायाँ_ext->e_pblk -= plen;
		दायाँ_ext->e_len += plen;
		dbg_print_extent("ext_falloc right+", दायाँ_ext);
		err = ext2fs_extent_replace(handle, 0, दायाँ_ext);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_fix_parents(handle);
		अगर (err)
			जाओ out;

		/* Zero blocks if necessary */
		अगर (!(दायाँ_ext->e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk,
					plen + cluster_fill, शून्य, शून्य);
			अगर (err)
				जाओ out;
		}
	}

try_anywhere:
	/* Try implied cluster alloc on the left and right ends */
	अगर (range_len > 0 && (range_start & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = EXT2FS_CLUSTER_RATIO(fs) -
			       (range_start & EXT2FS_CLUSTER_MASK(fs));
		cluster_fill &= EXT2FS_CLUSTER_MASK(fs);
		अगर (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = range_start;
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		अगर (err)
			जाओ out;
		अगर (pblk == 0)
			जाओ try_दायाँ_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus left+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		अगर (err == EXT2_ET_NO_CURRENT_गाँठ)
			ex.e_lblk = 0;
		अन्यथा अगर (err)
			जाओ out;

		अगर (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अन्यथा
			op = EXT2_EXTENT_जोड़ो_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_जोड़ो(handle, op, &newex);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_fix_parents(handle);
		अगर (err)
			जाओ out;

		अगर (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, शून्य, शून्य);
			अगर (err)
				जाओ out;
		}

		range_start += cluster_fill;
		range_len -= cluster_fill;
	}

try_दायाँ_implied:
	y = range_start + range_len;
	अगर (range_len > 0 && (y & EXT2FS_CLUSTER_MASK(fs))) {
		cluster_fill = y & EXT2FS_CLUSTER_MASK(fs);
		अगर (cluster_fill > range_len)
			cluster_fill = range_len;
		newex.e_lblk = y & ~EXT2FS_CLUSTER_MASK(fs);
		err = ext2fs_map_cluster_block(fs, ino, inode, newex.e_lblk,
					       &pblk);
		अगर (err)
			जाओ out;
		अगर (pblk == 0)
			जाओ no_implied;
		newex.e_pblk = pblk;
		newex.e_len = cluster_fill;
		newex.e_flags = (flags & EXT2_FALLOCATE_FORCE_INIT ? 0 :
				 EXT2_EXTENT_FLAGS_UNINIT);
		dbg_print_extent("ext_falloc iclus right+", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		अगर (err == EXT2_ET_NO_CURRENT_गाँठ)
			ex.e_lblk = 0;
		अन्यथा अगर (err)
			जाओ out;

		अगर (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अन्यथा
			op = EXT2_EXTENT_जोड़ो_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_जोड़ो(handle, op, &newex);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_fix_parents(handle);
		अगर (err)
			जाओ out;

		अगर (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, newex.e_pblk,
						  newex.e_len, शून्य, शून्य);
			अगर (err)
				जाओ out;
		}

		range_len -= cluster_fill;
	}

no_implied:
	अगर (range_len == 0)
		वापस 0;

	newex.e_lblk = range_start;
	अगर (flags & EXT2_FALLOCATE_FORCE_INIT) {
		max_extent_len = max_init_len;
		newex.e_flags = 0;
	} अन्यथा {
		max_extent_len = max_uninit_len;
		newex.e_flags = EXT2_EXTENT_FLAGS_UNINIT;
	}
	pblk = alloc_goal;
	y = range_len;
	क्रम (x = 0; x < y;) {
		cluster_fill = newex.e_lblk & EXT2FS_CLUSTER_MASK(fs);
		fillable = min(range_len + cluster_fill, max_extent_len);
		err = ext2fs_new_range(fs, 0, pblk & ~EXT2FS_CLUSTER_MASK(fs),
				       fillable,
				       शून्य, &pblk, &plen);
		अगर (err)
			जाओ out;
		err = claim_range(fs, inode, pblk, plen);
		अगर (err)
			जाओ out;

		/* Create extent */
		newex.e_pblk = pblk + cluster_fill;
		newex.e_len = plen - cluster_fill;
		dbg_print_extent("ext_falloc create", &newex);
		ext2fs_extent_goto(handle, newex.e_lblk);
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
					&ex);
		अगर (err == EXT2_ET_NO_CURRENT_गाँठ)
			ex.e_lblk = 0;
		अन्यथा अगर (err)
			जाओ out;

		अगर (ex.e_lblk > newex.e_lblk)
			op = 0; /* insert before */
		अन्यथा
			op = EXT2_EXTENT_जोड़ो_AFTER;
		dbg_printf("%s: inserting %s lblk %llu newex=%llu\n",
			   __func__, op ? "after" : "before", ex.e_lblk,
			   newex.e_lblk);
		err = ext2fs_extent_जोड़ो(handle, op, &newex);
		अगर (err)
			जाओ out;
		err = ext2fs_extent_fix_parents(handle);
		अगर (err)
			जाओ out;

		अगर (!(newex.e_flags & EXT2_EXTENT_FLAGS_UNINIT) &&
		    (flags & EXT2_FALLOCATE_ZERO_BLOCKS)) {
			err = ext2fs_zero_blocks2(fs, pblk, plen, शून्य, शून्य);
			अगर (err)
				जाओ out;
		}

		/* Update variables at end of loop */
		x += plen - cluster_fill;
		range_len -= plen - cluster_fill;
		newex.e_lblk += plen - cluster_fill;
		pblk += plen - cluster_fill;
		अगर (pblk >= ext2fs_blocks_गिनती(fs->super))
			pblk = fs->super->s_first_तथ्य_block;
	}

out:
	वापस err;
}

अटल errcode_t extent_fallocate(ext2_filsys fs, पूर्णांक flags, ext2_ino_t ino,
				      काष्ठा ext2_inode *inode, blk64_t goal,
				      blk64_t start, blk64_t len)
{
	ext2_extent_handle_t	handle;
	काष्ठा ext2fs_extent	बायाँ_extent, दायाँ_extent;
	काष्ठा ext2fs_extent	*बायाँ_adjacent, *दायाँ_adjacent;
	errcode_t		err;
	blk64_t			range_start, range_end = 0, end, अग्र;
	blk64_t			गिनती, goal_distance;

	end = start + len - 1;
	err = ext2fs_extent_open2(fs, ino, inode, &handle);
	अगर (err)
		वापस err;

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
	err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &बायाँ_extent);
	अगर (err == EXT2_ET_NO_CURRENT_गाँठ) {
		blk64_t max_blocks = ext2fs_blocks_गिनती(fs->super);

		अगर (goal == ~0ULL)
			goal = ext2fs_find_inode_goal(fs, ino, inode, start);
		err = ext2fs_find_first_zero_block_bitmap2(fs->block_map,
						goal, max_blocks - 1, &goal);
		goal += start;
		err = ext_falloc_helper(fs, flags, ino, inode, handle, शून्य,
					शून्य, start, len, goal);
		जाओ errout;
	} अन्यथा अगर (err)
		जाओ errout;

	dbg_print_extent("ext_falloc initial", &बायाँ_extent);
	अग्र = बायाँ_extent.e_lblk + बायाँ_extent.e_len;
	अगर (बायाँ_extent.e_lblk > start) {
		/* The nearest extent we found was beyond start??? */
		goal = बायाँ_extent.e_pblk - (बायाँ_extent.e_lblk - start);
		err = ext_falloc_helper(fs, flags, ino, inode, handle, शून्य,
					&बायाँ_extent, start,
					min(len, बायाँ_extent.e_lblk - start),
					goal);
		अगर (err)
			जाओ errout;

		जाओ start_again;
	} अन्यथा अगर (अग्र >= start) {
		range_start = अग्र;
		बायाँ_adjacent = &बायाँ_extent;
	} अन्यथा {
		range_start = start;
		बायाँ_adjacent = शून्य;
	}
	goal = बायाँ_extent.e_pblk + (range_start - बायाँ_extent.e_lblk);

	करो {
		err = ext2fs_extent_get(handle, EXT2_EXTENT_अग्र_LEAF,
					   &दायाँ_extent);
		dbg_printf("%s: ino=%d get next =%d\n", __func__, ino,
			   (पूर्णांक)err);
		dbg_print_extent("ext_falloc next", &दायाँ_extent);
		/* Stop if we've seen this extent before */
		अगर (!err && दायाँ_extent.e_lblk <= बायाँ_extent.e_lblk)
			err = EXT2_ET_EXTENT_NO_अग्र;

		अगर (err && err != EXT2_ET_EXTENT_NO_अग्र)
			जाओ errout;
		अगर (err == EXT2_ET_EXTENT_NO_अग्र ||
		    दायाँ_extent.e_lblk > end + 1) {
			range_end = end;
			दायाँ_adjacent = शून्य;
		} अन्यथा {
			/* Handle right_extent.e_lblk <= end */
			range_end = दायाँ_extent.e_lblk - 1;
			दायाँ_adjacent = &दायाँ_extent;
		}
		goal_distance = range_start - अग्र;
		अगर (err != EXT2_ET_EXTENT_NO_अग्र &&
		    goal_distance > (range_end - दायाँ_extent.e_lblk))
			goal = दायाँ_extent.e_pblk -
					(दायाँ_extent.e_lblk - range_start);

		dbg_printf("%s: ino=%d rstart=%llu rend=%llu\n", __func__, ino,
			   range_start, range_end);
		err = 0;
		अगर (range_start <= range_end) {
			गिनती = range_end - range_start + 1;
			err = ext_falloc_helper(fs, flags, ino, inode, handle,
						बायाँ_adjacent, दायाँ_adjacent,
						range_start, गिनती, goal);
			अगर (err)
				जाओ errout;
		}

		अगर (range_end == end)
			अवरोध;

		err = ext2fs_extent_goto(handle, दायाँ_extent.e_lblk);
		अगर (err)
			जाओ errout;
		अग्र = दायाँ_extent.e_lblk + दायाँ_extent.e_len;
		बायाँ_extent = दायाँ_extent;
		बायाँ_adjacent = &बायाँ_extent;
		range_start = अग्र;
		goal = बायाँ_extent.e_pblk + (range_start - बायाँ_extent.e_lblk);
	} जबतक (range_end < end);

errout:
	ext2fs_extent_free(handle);
	वापस err;
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
errcode_t ext2fs_fallocate(ext2_filsys fs, पूर्णांक flags, ext2_ino_t ino,
			   काष्ठा ext2_inode *inode, blk64_t goal,
			   blk64_t start, blk64_t len)
{
	काष्ठा ext2_inode	inode_buf;
	blk64_t			blk, x, zero_blk, last = 0;
	पूर्णांक			zero_len = 0;
	errcode_t		err;

	अगर (((flags & EXT2_FALLOCATE_FORCE_INIT) &&
	    (flags & EXT2_FALLOCATE_FORCE_UNINIT)) ||
	   (flags & ~EXT2_FALLOCATE_ALL_FLAGS))
		वापस EXT2_ET_INVALID_ARGUMENT;

	अगर (len > ext2fs_blocks_गिनती(fs->super))
		वापस EXT2_ET_BLOCK_ALLOC_FAIL;
	अन्यथा अगर (len == 0)
		वापस 0;

	/* Read inode structure if necessary */
	अगर (!inode) {
		err = ext2fs_read_inode(fs, ino, &inode_buf);
		अगर (err)
			वापस err;
		inode = &inode_buf;
	}
	dbg_printf("%s: ino=%d start=%llu len=%llu goal=%llu\n", __func__, ino,
		   start, len, goal);

	अगर (inode->i_flags & EXT4_EXTENTS_FL) {
		err = extent_fallocate(fs, flags, ino, inode, goal, start, len);
		जाओ out;
	}

	/* XXX: Allocate a bunch of blocks the slow way */
	क्रम (blk = start; blk < start + len; blk++) {
		err = ext2fs_bmap2(fs, ino, inode, शून्य, 0, blk, 0, &x);
		अगर (err)
			वापस err;
		अगर (x)
			जारी;

		err = ext2fs_bmap2(fs, ino, inode, शून्य, BMAP_ALLOC,
				   blk, 0, &x);
		अगर (err)
			जाओ errout;
		अगर ((zero_len && (x != last+1)) ||
		    (zero_len >= 65536)) {
			err = ext2fs_zero_blocks2(fs, zero_blk, zero_len,
						  शून्य, शून्य);
			zero_len = 0;
			अगर (err)
				जाओ errout;
		}
		अगर (zero_len == 0) {
			zero_blk = x;
			zero_len = 1;
		} अन्यथा {
			zero_len++;
		}
		last = x;
	}

out:
	अगर (inode == &inode_buf)
		ext2fs_write_inode(fs, ino, inode);
errout:
	अगर (zero_len)
		ext2fs_zero_blocks2(fs, zero_blk, zero_len, शून्य, शून्य);
	वापस err;
}
