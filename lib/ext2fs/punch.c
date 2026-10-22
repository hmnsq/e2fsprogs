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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

#undef PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
স্থির পূর্ণসংখ্যা check_zero_block(অক্ষর *buf, পূর্ণসংখ্যা blocksize)
{
	অক্ষর	*cp = buf;
	পূর্ণসংখ্যা	বাম = blocksize;

	যখন (বাম > 0) {
		যদি (*cp++)
			ফেরত 0;
		বাম--;
	}
	ফেরত 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
স্থির errcode_t ind_punch(ext2_filsys fs, কাঠামো ext2_inode *inode,
			   অক্ষর *block_buf, blk_t *p, পূর্ণসংখ্যা level,
			   blk64_t start, blk64_t গুনতি, পূর্ণসংখ্যা max)
{
	errcode_t	retval;
	blk_t		b;
	পূর্ণসংখ্যা		i;
	blk64_t		offset, incr;
	ধ্রুবক blk64_t	end = start + গুনতি;
	পূর্ণসংখ্যা		freed = 0;

#ifdef PUNCH_DEBUG
	printf("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, গুনতি, max);
#endif
	incr = 1ULL << ((EXT2_BLOCK_আকার_BITS(fs->super) - 2) * level);
	জন্য (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		যদি (offset >= end)
			বিরতি;
		যদি (*p == 0 || (offset+incr) <= start)
			চালু;
		b = *p;
		যদি (level > 0) {
			blk_t start2;
			blk_t count2;
#ifdef PUNCH_DEBUG
			printf("Reading indirect block %u\n", b);
#endif
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			যদি (retval)
				ফেরত retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#ifdef PUNCH_DEBUG
			printf("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, গুনতি, end, incr, start2, count2);
#endif
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			যদি (retval)
				ফেরত retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			যদি (retval)
				ফেরত retval;
			যদি (!check_zero_block(block_buf, fs->blocksize))
				চালু;
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
	ফেরত ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#define BLK_T_MAX ((blk_t)~0ULL)
স্থির errcode_t ext2fs_punch_ind(ext2_filsys fs, কাঠামো ext2_inode *inode,
				  অক্ষর *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	অক্ষর			*buf = 0;
	পূর্ণসংখ্যা			level;
	পূর্ণসংখ্যা			অঙ্ক = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			গুনতি;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	যদি (start > BLK_T_MAX)
		ফেরত 0;
	যদি (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		গুনতি = BLK_T_MAX - start;
	অন্যথা
		গুনতি = end - start + 1;

	যদি (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		যদি (retval)
			ফেরত retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	জন্য (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#ifdef PUNCH_DEBUG
		printf("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, গুনতি, max, অঙ্ক);
#endif
		যদি (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, গুনতি, অঙ্ক);
			যদি (retval)
				যাও errout;
			যদি (গুনতি > max)
				গুনতি -= max - start;
			অন্যথা
				বিরতি;
			start = 0;
		} অন্যথা
			start -= max;
		bp += অঙ্ক;
		যদি (level == 0) {
			অঙ্ক = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	যদি (buf)
		ext2fs_free_mem(&buf);
	ফেরত retval;
}
#undef BLK_T_MAX

#ifdef PUNCH_DEBUG

#define dbg_printf(f, a...)  printf(f, ## a)

স্থির শূন্য dbg_print_extent(অক্ষর *desc, কাঠামো ext2fs_extent *extent)
{
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

}
#অন্যথা
#define dbg_print_extent(desc, ex)	করো { } যখন (0)
#define dbg_printf(f, a...)		করো { } যখন (0)
#endif

/* Free a range of blocks, respecting cluster boundaries */
স্থির errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     কাঠামো ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_গুনতি, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	যদি (free_start < fs->super->s_first_তথ্য_block ||
	    (free_start + free_গুনতি) > ext2fs_blocks_গুনতি(fs->super))
		ফেরত EXT2_ET_BAD_BLOCK_অঙ্ক;

	/* No bigalloc?  Just free each block. */
	যদি (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_গুনতি;
		যখন (free_গুনতি-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		ফেরত retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	যদি (free_start & EXT2FS_CLUSTER_MASK(fs)) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		যদি (retval)
			যাও errout;
		যদি (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs) -
			(free_start & EXT2FS_CLUSTER_MASK(fs));
		যদি (cluster_freed > free_গুনতি)
			cluster_freed = free_গুনতি;
		free_গুনতি -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	যখন (free_গুনতি > 0 && free_গুনতি >= (unsigned) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_গুনতি -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	যদি (free_গুনতি > 0) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		যদি (retval)
			যাও errout;
		যদি (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	ফেরত retval;
}

স্থির errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     কাঠামো ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	কাঠামো ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, পরবর্তী, lfree_start;
	__u32			free_গুনতি, newlen;
	blk64_t			freed = 0;
	পূর্ণসংখ্যা			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	যদি (retval)
		ফেরত retval;
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
	যদি (retval == EXT2_ET_NO_CURRENT_গ্রন্থি) {
		retval = 0;
		যাও errout;
	} অন্যথা যদি (retval)
		যাও errout;
	যখন (1) {
		op = EXT2_EXTENT_পরবর্তী_LEAF;
		dbg_print_extent("main loop", &extent);
		পরবর্তী = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (unsigned long long) start,
			   (unsigned long long) end,
			   (unsigned long long) পরবর্তী);
		যদি (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			যদি (end < extent.e_lblk)
				বিরতি;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			যদি (পরবর্তী > end)
				free_গুনতি = end - extent.e_lblk + 1;
			অন্যথা
				free_গুনতি = extent.e_len;
			extent.e_len -= free_গুনতি;
			extent.e_lblk += free_গুনতি;
			extent.e_pblk += free_গুনতি;
		} অন্যথা যদি (end >= পরবর্তী-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			যদি (start >= পরবর্তী)
				যাও পরবর্তী_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_গুনতি = extent.e_len - newlen;
			extent.e_len = newlen;
		} অন্যথা {
			কাঠামো ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(end + 1 - extent.e_lblk);
			newex.e_lblk = end + 1;
			newex.e_len = পরবর্তী - end - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = start - extent.e_lblk;
			free_start = extent.e_pblk + extent.e_len;
			lfree_start = extent.e_lblk + extent.e_len;
			free_গুনতি = end - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_ঢোকাও(handle,
					EXT2_EXTENT_ঢোকাও_AFTER, &newex);
			যদি (retval)
				যাও errout;
			retval = ext2fs_extent_fix_parents(handle);
			যদি (retval)
				যাও errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			যদি (retval)
				যাও errout;
		} 
		যদি (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			যদি (retval)
				যাও errout;
			retval = ext2fs_extent_fix_parents(handle);
		} অন্যথা {
			কাঠামো ext2fs_extent	newex;
			blk64_t			old_lblk, পরবর্তী_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			যদি (retval)
				যাও errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_পরবর্তী_LEAF,
						   &newex);
			যদি (retval == EXT2_ET_EXTENT_NO_পরবর্তী)
				পরবর্তী_lblk = old_lblk;
			অন্যথা যদি (retval)
				যাও errout;
			অন্যথা
				পরবর্তী_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			যদি (retval)
				যাও errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			যদি (retval)
				যাও errout;

			retval = ext2fs_extent_fix_parents(handle);
			যদি (retval && retval != EXT2_ET_NO_CURRENT_গ্রন্থি)
				যাও errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(শূন্য)ext2fs_extent_goto(handle, পরবর্তী_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		যদি (retval)
			যাও errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_গুনতি);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_গুনতি, &freed);
		যদি (retval)
			যাও errout;
	পরবর্তী_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		যদি (retval == EXT2_ET_EXTENT_NO_পরবর্তী ||
		    retval == EXT2_ET_NO_CURRENT_গ্রন্থি)
			বিরতি;
		যদি (retval)
			যাও errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	ফেরত retval;
}
	
স্থির errcode_t ext2fs_punch_inline_তথ্য(ext2_filsys fs, ext2_ino_t ino,
					  কাঠামো ext2_inode *inode,
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
	যদি (start > 0)
		ফেরত 0;

	memset((অক্ষর *)inode->i_block, 0, EXT4_MIN_INLINE_তথ্য_আকার);
	inode->i_আকার = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	যদি (retval)
		ফেরত retval;

	ফেরত ext2fs_inline_তথ্য_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       কাঠামো ext2_inode *inode,
		       অক্ষর *block_buf, blk64_t start,
		       blk64_t end)
{
	errcode_t		retval;
	কাঠামো ext2_inode	inode_buf;

	যদি (start > end)
		ফেরত EINVAL;

	/* Read inode structure if necessary */
	যদি (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		যদি (retval)
			ফেরত retval;
		inode = &inode_buf;
	}
	যদি (inode->i_flags & EXT4_INLINE_তথ্য_FL)
		ফেরত ext2fs_punch_inline_তথ্য(fs, ino, inode, start, end);
	অন্যথা যদি (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	অন্যথা
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	যদি (retval)
		ফেরত retval;

#ifdef PUNCH_DEBUG
	printf("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_আকার(inode), inode->i_blocks);
#endif
	ফেরত ext2fs_write_inode(fs, ino, inode);
}
