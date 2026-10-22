/*
 * punch.c --- deallocate blocks allocated to an inode
 *
 * Copyright (C) 2010 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#undef PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_zero_block(ವರ್ಣ *buf, ಪೂರ್ಣಾಙ್ಕ blocksize)
{
	ವರ್ಣ	*cp = buf;
	ಪೂರ್ಣಾಙ್ಕ	ಎಡ = blocksize;

	ಯಾವತ್ (ಎಡ > 0) {
		ಯದಿ (*cp++)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಎಡ--;
	}
	ಪ್ರತ್ಯಾಗಮ 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
ಸ್ಥಿರ errcode_t ind_punch(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode,
			   ವರ್ಣ *block_buf, blk_t *p, ಪೂರ್ಣಾಙ್ಕ level,
			   blk64_t start, blk64_t ಎಣಿಕೆ, ಪೂರ್ಣಾಙ್ಕ max)
{
	errcode_t	retval;
	blk_t		b;
	ಪೂರ್ಣಾಙ್ಕ		i;
	blk64_t		offset, incr;
	ಧ್ರುವ blk64_t	end = start + ಎಣಿಕೆ;
	ಪೂರ್ಣಾಙ್ಕ		freed = 0;

#ifdef PUNCH_DEBUG
	printf("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, ಎಣಿಕೆ, max);
#endif
	incr = 1ULL << ((EXT2_BLOCK_SIZE_BITS(fs->super) - 2) * level);
	ಪ್ರತಿ (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		ಯದಿ (offset >= end)
			ಭಙ್ಗ;
		ಯದಿ (*p == 0 || (offset+incr) <= start)
			ಅನುವರ್ತ;
		b = *p;
		ಯದಿ (level > 0) {
			blk_t start2;
			blk_t count2;
#ifdef PUNCH_DEBUG
			printf("Reading indirect block %u\n", b);
#endif
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#ifdef PUNCH_DEBUG
			printf("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, ಎಣಿಕೆ, end, incr, start2, count2);
#endif
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
			ಯದಿ (!check_zero_block(block_buf, fs->blocksize))
				ಅನುವರ್ತ;
		}
#ifdef PUNCH_DEBUG
		printf("Freeing block %u (offset %llu)\n", b, offset);
#endif
		ext2fs_block_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#ifdef PUNCH_DEBUG
	printf("Freed %d blocks\n", freed);
#endif
	ಪ್ರತ್ಯಾಗಮ ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#define BLK_T_MAX ((blk_t)~0ULL)
ಸ್ಥಿರ errcode_t ext2fs_punch_ind(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode,
				  ವರ್ಣ *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	ವರ್ಣ			*buf = 0;
	ಪೂರ್ಣಾಙ್ಕ			level;
	ಪೂರ್ಣಾಙ್ಕ			ಅಂಕೆ = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			ಎಣಿಕೆ;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	ಯದಿ (start > BLK_T_MAX)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		ಎಣಿಕೆ = BLK_T_MAX - start;
	ಅನ್ಯಥಾ
		ಎಣಿಕೆ = end - start + 1;

	ಯದಿ (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	ಪ್ರತಿ (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#ifdef PUNCH_DEBUG
		printf("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, ಎಣಿಕೆ, max, ಅಂಕೆ);
#endif
		ಯದಿ (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, ಎಣಿಕೆ, ಅಂಕೆ);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;
			ಯದಿ (ಎಣಿಕೆ > max)
				ಎಣಿಕೆ -= max - start;
			ಅನ್ಯಥಾ
				ಭಙ್ಗ;
			start = 0;
		} ಅನ್ಯಥಾ
			start -= max;
		bp += ಅಂಕೆ;
		ಯದಿ (level == 0) {
			ಅಂಕೆ = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	ಯದಿ (buf)
		ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}
#undef BLK_T_MAX

#ifdef PUNCH_DEBUG

#define dbg_printf(f, a...)  printf(f, ## a)

ಸ್ಥಿರ ಶೂನ್ಯ dbg_print_extent(ವರ್ಣ *desc, ಸಂರಚನಾ ext2fs_extent *extent)
{
	ಯದಿ (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ಯದಿ (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	ಯದಿ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ಯದಿ (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	ಯದಿ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#ಅನ್ಯಥಾ
#define dbg_print_extent(desc, ex)	ಕುರು { } ಯಾವತ್ (0)
#define dbg_printf(f, a...)		ಕುರು { } ಯಾವತ್ (0)
#endif

/* Free a range of blocks, respecting cluster boundaries */
ಸ್ಥಿರ errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     ಸಂರಚನಾ ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_ಎಣಿಕೆ, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	ಯದಿ (free_start < fs->super->s_first_data_block ||
	    (free_start + free_ಎಣಿಕೆ) > ext2fs_blocks_ಎಣಿಕೆ(fs->super))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_BLOCK_ಅಂಕೆ;

	/* No bigalloc?  Just free each block. */
	ಯದಿ (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_ಎಣಿಕೆ;
		ಯಾವತ್ (free_ಎಣಿಕೆ-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		ಪ್ರತ್ಯಾಗಮ retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	ಯದಿ (free_start & EXT2FS_CLUSTER_MASK(fs)) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
		ಯದಿ (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs) -
			(free_start & EXT2FS_CLUSTER_MASK(fs));
		ಯದಿ (cluster_freed > free_ಎಣಿಕೆ)
			cluster_freed = free_ಎಣಿಕೆ;
		free_ಎಣಿಕೆ -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	ಯಾವತ್ (free_ಎಣಿಕೆ > 0 && free_ಎಣಿಕೆ >= (unsigned) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_ಎಣಿಕೆ -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	ಯದಿ (free_ಎಣಿಕೆ > 0) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
		ಯದಿ (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     ಸಂರಚನಾ ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	ಸಂರಚನಾ ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, next, lfree_start;
	__u32			free_ಎಣಿಕೆ, newlen;
	blk64_t			freed = 0;
	ಪೂರ್ಣಾಙ್ಕ			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	/*
	 * Find the extent closest to the start of the punch range.  We don't
	 * check the return value because _goto() sets the current node to the
	 * next-lowest extent if 'start' is in a hole, and doesn't set a
	 * current node if there was a real error reading the extent tree.
	 * In that case, _get() will error out.
	 *
	 * Note: If _get() returns 'no current node', that simply means that
	 * there aren't any blocks mapped past this point in the file, so we're
	 * done.
	 */
	ext2fs_extent_goto(handle, start);
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ಯದಿ (retval == EXT2_ET_NO_CURRENT_ಗಂಟು) {
		retval = 0;
		ಗಚ್ಛ errout;
	} ಅನ್ಯಥಾ ಯದಿ (retval)
		ಗಚ್ಛ errout;
	ಯಾವತ್ (1) {
		op = EXT2_EXTENT_NEXT_LEAF;
		dbg_print_extent("main loop", &extent);
		next = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (unsigned long long) start,
			   (unsigned long long) end,
			   (unsigned long long) next);
		ಯದಿ (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			ಯದಿ (end < extent.e_lblk)
				ಭಙ್ಗ;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			ಯದಿ (next > end)
				free_ಎಣಿಕೆ = end - extent.e_lblk + 1;
			ಅನ್ಯಥಾ
				free_ಎಣಿಕೆ = extent.e_len;
			extent.e_len -= free_ಎಣಿಕೆ;
			extent.e_lblk += free_ಎಣಿಕೆ;
			extent.e_pblk += free_ಎಣಿಕೆ;
		} ಅನ್ಯಥಾ ಯದಿ (end >= next-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			ಯದಿ (start >= next)
				ಗಚ್ಛ next_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_ಎಣಿಕೆ = extent.e_len - newlen;
			extent.e_len = newlen;
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(end + 1 - extent.e_lblk);
			newex.e_lblk = end + 1;
			newex.e_len = next - end - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = start - extent.e_lblk;
			free_start = extent.e_pblk + extent.e_len;
			lfree_start = extent.e_lblk + extent.e_len;
			free_ಎಣಿಕೆ = end - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_ಸೇರಿಸು(handle,
					EXT2_EXTENT_ಸೇರಿಸು_AFTER, &newex);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;
			retval = ext2fs_extent_fix_parents(handle);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;
		} 
		ಯದಿ (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;
			retval = ext2fs_extent_fix_parents(handle);
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ ext2fs_extent	newex;
			blk64_t			old_lblk, next_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &newex);
			ಯದಿ (retval == EXT2_ET_EXTENT_NO_NEXT)
				next_lblk = old_lblk;
			ಅನ್ಯಥಾ ಯದಿ (retval)
				ಗಚ್ಛ errout;
			ಅನ್ಯಥಾ
				next_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;

			retval = ext2fs_extent_fix_parents(handle);
			ಯದಿ (retval && retval != EXT2_ET_NO_CURRENT_ಗಂಟು)
				ಗಚ್ಛ errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(ಶೂನ್ಯ)ext2fs_extent_goto(handle, next_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_ಎಣಿಕೆ);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_ಎಣಿಕೆ, &freed);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
	next_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		ಯದಿ (retval == EXT2_ET_EXTENT_NO_NEXT ||
		    retval == EXT2_ET_NO_CURRENT_ಗಂಟು)
			ಭಙ್ಗ;
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	ಪ್ರತ್ಯಾಗಮ retval;
}
	
ಸ್ಥಿರ errcode_t ext2fs_punch_inline_data(ext2_filsys fs, ext2_ino_t ino,
					  ಸಂರಚನಾ ext2_inode *inode,
					  blk64_t start,
					  blk64_t end EXT2FS_ATTR((unused)))
{
	errcode_t retval;

	/*
	 * In libext2fs ext2fs_punch is based on block unit.  So that
	 * means that if start > 0 we don't need to do nothing.  Due
	 * to this we will remove all inline data in ext2fs_punch()
	 * now.
	 */
	ಯದಿ (start > 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	memset((ವರ್ಣ *)inode->i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಪ್ರತ್ಯಾಗಮ ext2fs_inline_data_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       ಸಂರಚನಾ ext2_inode *inode,
		       ವರ್ಣ *block_buf, blk64_t start,
		       blk64_t end)
{
	errcode_t		retval;
	ಸಂರಚನಾ ext2_inode	inode_buf;

	ಯದಿ (start > end)
		ಪ್ರತ್ಯಾಗಮ EINVAL;

	/* Read inode structure if necessary */
	ಯದಿ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		inode = &inode_buf;
	}
	ಯದಿ (inode->i_flags & EXT4_INLINE_DATA_FL)
		ಪ್ರತ್ಯಾಗಮ ext2fs_punch_inline_data(fs, ino, inode, start, end);
	ಅನ್ಯಥಾ ಯದಿ (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	ಅನ್ಯಥಾ
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

#ifdef PUNCH_DEBUG
	printf("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_SIZE(inode), inode->i_blocks);
#endif
	ಪ್ರತ್ಯಾಗಮ ext2fs_write_inode(fs, ino, inode);
}
