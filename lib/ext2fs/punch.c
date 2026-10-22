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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#undef PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_zero_block(ଅକ୍ଷର *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize)
{
	ଅକ୍ଷର	*cp = buf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ବାମ = blocksize;

	ଯେତେବେଳେ (ବାମ > 0) {
		ଯଦି (*cp++)
			ଫେରନ୍ତୁ 0;
		ବାମ--;
	}
	ଫେରନ୍ତୁ 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
ସ୍ଥିର errcode_t ind_punch(ext2_filsys fs, ସଂରଚନା ext2_inode *inode,
			   ଅକ୍ଷର *block_buf, blk_t *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level,
			   blk64_t start, blk64_t ଗଣନା_2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
	errcode_t	retval;
	blk_t		b;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;
	blk64_t		offset, incr;
	ଅଚଳ blk64_t	end = start + ଗଣନା_2;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		freed = 0;

#ifdef PUNCH_DEBUG
	printf("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, ଗଣନା_2, max);
#endif
	incr = 1ULL << ((EXT2_BLOCK_SIZE_BITS(fs->super) - 2) * level);
	ପାଇଁ (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		ଯଦି (offset >= end)
			ଭାଙ୍ଗ;
		ଯଦି (*p == 0 || (offset+incr) <= start)
			ଜାରି;
		b = *p;
		ଯଦି (level > 0) {
			blk_t start2;
			blk_t count2;
#ifdef PUNCH_DEBUG
			printf("Reading indirect block %u\n", b);
#endif
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#ifdef PUNCH_DEBUG
			printf("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, ଗଣନା_2, end, incr, start2, count2);
#endif
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
			ଯଦି (!check_zero_block(block_buf, fs->blocksize))
				ଜାରି;
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
	ଫେରନ୍ତୁ ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#define BLK_T_MAX ((blk_t)~0ULL)
ସ୍ଥିର errcode_t ext2fs_punch_ind(ext2_filsys fs, ସଂରଚନା ext2_inode *inode,
				  ଅକ୍ଷର *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	ଅକ୍ଷର			*buf = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			level;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			ଅଙ୍କ = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			ଗଣନା_2;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	ଯଦି (start > BLK_T_MAX)
		ଫେରନ୍ତୁ 0;
	ଯଦି (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		ଗଣନା_2 = BLK_T_MAX - start;
	ଅନ୍ୟଥା
		ଗଣନା_2 = end - start + 1;

	ଯଦି (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	ପାଇଁ (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#ifdef PUNCH_DEBUG
		printf("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, ଗଣନା_2, max, ଅଙ୍କ);
#endif
		ଯଦି (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, ଗଣନା_2, ଅଙ୍କ);
			ଯଦି (retval)
				ଯାଅ errout;
			ଯଦି (ଗଣନା_2 > max)
				ଗଣନା_2 -= max - start;
			ଅନ୍ୟଥା
				ଭାଙ୍ଗ;
			start = 0;
		} ଅନ୍ୟଥା
			start -= max;
		bp += ଅଙ୍କ;
		ଯଦି (level == 0) {
			ଅଙ୍କ = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	ଯଦି (buf)
		ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
}
#undef BLK_T_MAX

#ifdef PUNCH_DEBUG

#define dbg_printf(f, a...)  printf(f, ## a)

ସ୍ଥିର ଶୂନ୍ୟ dbg_print_extent(ଅକ୍ଷର *desc, ସଂରଚନା ext2fs_extent *extent)
{
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

}
#ଅନ୍ୟଥା
#define dbg_print_extent(desc, ex)	କର { } ଯେତେବେଳେ (0)
#define dbg_printf(f, a...)		କର { } ଯେତେବେଳେ (0)
#endif

/* Free a range of blocks, respecting cluster boundaries */
ସ୍ଥିର errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     ସଂରଚନା ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_ଗଣନା, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	ଯଦି (free_start < fs->super->s_first_data_block ||
	    (free_start + free_ଗଣନା) > ext2fs_blocks_ଗଣନା(fs->super))
		ଫେରନ୍ତୁ EXT2_ET_BAD_BLOCK_ଅଙ୍କ;

	/* No bigalloc?  Just free each block. */
	ଯଦି (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_ଗଣନା;
		ଯେତେବେଳେ (free_ଗଣନା-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		ଫେରନ୍ତୁ retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	ଯଦି (free_start & EXT2FS_CLUSTER_MASK(fs)) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		ଯଦି (retval)
			ଯାଅ errout;
		ଯଦି (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs) -
			(free_start & EXT2FS_CLUSTER_MASK(fs));
		ଯଦି (cluster_freed > free_ଗଣନା)
			cluster_freed = free_ଗଣନା;
		free_ଗଣନା -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	ଯେତେବେଳେ (free_ଗଣନା > 0 && free_ଗଣନା >= (unsigned) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_ଗଣନା -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	ଯଦି (free_ଗଣନା > 0) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		ଯଦି (retval)
			ଯାଅ errout;
		ଯଦି (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     ସଂରଚନା ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	ସଂରଚନା ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, next, lfree_start;
	__u32			free_ଗଣନା, newlen;
	blk64_t			freed = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
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
	ଯଦି (retval == EXT2_ET_NO_CURRENT_ଗଣ୍ଠି) {
		retval = 0;
		ଯାଅ errout;
	} ଅନ୍ୟଥା ଯଦି (retval)
		ଯାଅ errout;
	ଯେତେବେଳେ (1) {
		op = EXT2_EXTENT_NEXT_LEAF;
		dbg_print_extent("main loop", &extent);
		next = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (unsigned long long) start,
			   (unsigned long long) end,
			   (unsigned long long) next);
		ଯଦି (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			ଯଦି (end < extent.e_lblk)
				ଭାଙ୍ଗ;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			ଯଦି (next > end)
				free_ଗଣନା = end - extent.e_lblk + 1;
			ଅନ୍ୟଥା
				free_ଗଣନା = extent.e_len;
			extent.e_len -= free_ଗଣନା;
			extent.e_lblk += free_ଗଣନା;
			extent.e_pblk += free_ଗଣନା;
		} ଅନ୍ୟଥା ଯଦି (end >= next-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			ଯଦି (start >= next)
				ଯାଅ next_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_ଗଣନା = extent.e_len - newlen;
			extent.e_len = newlen;
		} ଅନ୍ୟଥା {
			ସଂରଚନା ext2fs_extent	newex;

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
			free_ଗଣନା = end - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_ଭର୍ତ୍ତି(handle,
					EXT2_EXTENT_ଭର୍ତ୍ତି_AFTER, &newex);
			ଯଦି (retval)
				ଯାଅ errout;
			retval = ext2fs_extent_fix_parents(handle);
			ଯଦି (retval)
				ଯାଅ errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			ଯଦି (retval)
				ଯାଅ errout;
		} 
		ଯଦି (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			ଯଦି (retval)
				ଯାଅ errout;
			retval = ext2fs_extent_fix_parents(handle);
		} ଅନ୍ୟଥା {
			ସଂରଚନା ext2fs_extent	newex;
			blk64_t			old_lblk, next_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			ଯଦି (retval)
				ଯାଅ errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &newex);
			ଯଦି (retval == EXT2_ET_EXTENT_NO_NEXT)
				next_lblk = old_lblk;
			ଅନ୍ୟଥା ଯଦି (retval)
				ଯାଅ errout;
			ଅନ୍ୟଥା
				next_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			ଯଦି (retval)
				ଯାଅ errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			ଯଦି (retval)
				ଯାଅ errout;

			retval = ext2fs_extent_fix_parents(handle);
			ଯଦି (retval && retval != EXT2_ET_NO_CURRENT_ଗଣ୍ଠି)
				ଯାଅ errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(ଶୂନ୍ୟ)ext2fs_extent_goto(handle, next_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		ଯଦି (retval)
			ଯାଅ errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_ଗଣନା);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_ଗଣନା, &freed);
		ଯଦି (retval)
			ଯାଅ errout;
	next_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		ଯଦି (retval == EXT2_ET_EXTENT_NO_NEXT ||
		    retval == EXT2_ET_NO_CURRENT_ଗଣ୍ଠି)
			ଭାଙ୍ଗ;
		ଯଦି (retval)
			ଯାଅ errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	ଫେରନ୍ତୁ retval;
}
	
ସ୍ଥିର errcode_t ext2fs_punch_inline_data(ext2_filsys fs, ext2_ino_t ino,
					  ସଂରଚନା ext2_inode *inode,
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
	ଯଦି (start > 0)
		ଫେରନ୍ତୁ 0;

	memset((ଅକ୍ଷର *)inode->i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଫେରନ୍ତୁ ext2fs_inline_data_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       ସଂରଚନା ext2_inode *inode,
		       ଅକ୍ଷର *block_buf, blk64_t start,
		       blk64_t end)
{
	errcode_t		retval;
	ସଂରଚନା ext2_inode	inode_buf;

	ଯଦି (start > end)
		ଫେରନ୍ତୁ EINVAL;

	/* Read inode structure if necessary */
	ଯଦି (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		inode = &inode_buf;
	}
	ଯଦି (inode->i_flags & EXT4_INLINE_DATA_FL)
		ଫେରନ୍ତୁ ext2fs_punch_inline_data(fs, ino, inode, start, end);
	ଅନ୍ୟଥା ଯଦି (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	ଅନ୍ୟଥା
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

#ifdef PUNCH_DEBUG
	printf("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_SIZE(inode), inode->i_blocks);
#endif
	ଫେରନ୍ତୁ ext2fs_write_inode(fs, ino, inode);
}
