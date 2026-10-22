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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <errno.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#undef PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ایستا صحیح check_zero_block(نویسه *buf, صحیح blocksize)
{
	نویسه	*cp = buf;
	صحیح	چپ = blocksize;

	وقتی (چپ > 0) {
		اگر (*cp++)
			بازگشت 0;
		چپ--;
	}
	بازگشت 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
ایستا errcode_t ind_punch(ext2_filsys fs, ساختار ext2_inode *inode,
			   نویسه *block_buf, blk_t *p, صحیح level,
			   blk64_t start, blk64_t شمار, صحیح max)
{
	errcode_t	retval;
	blk_t		b;
	صحیح		i;
	blk64_t		offset, incr;
	ثابت blk64_t	end = start + شمار;
	صحیح		freed = 0;

#ifdef PUNCH_DEBUG
	printf("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, شمار, max);
#endif
	incr = 1ULL << ((EXT2_BLOCK_SIZE_BITS(fs->super) - 2) * level);
	برای (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		اگر (offset >= end)
			بشکن;
		اگر (*p == 0 || (offset+incr) <= start)
			ادامه;
		b = *p;
		اگر (level > 0) {
			blk_t start2;
			blk_t count2;
#ifdef PUNCH_DEBUG
			printf("Reading indirect block %u\n", b);
#endif
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			اگر (retval)
				بازگشت retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#ifdef PUNCH_DEBUG
			printf("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, شمار, end, incr, start2, count2);
#endif
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			اگر (retval)
				بازگشت retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			اگر (retval)
				بازگشت retval;
			اگر (!check_zero_block(block_buf, fs->blocksize))
				ادامه;
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
	بازگشت ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#define BLK_T_MAX ((blk_t)~0ULL)
ایستا errcode_t ext2fs_punch_ind(ext2_filsys fs, ساختار ext2_inode *inode,
				  نویسه *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	نویسه			*buf = 0;
	صحیح			level;
	صحیح			رقم = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			شمار;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	اگر (start > BLK_T_MAX)
		بازگشت 0;
	اگر (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		شمار = BLK_T_MAX - start;
	وگرنه
		شمار = end - start + 1;

	اگر (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		اگر (retval)
			بازگشت retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	برای (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#ifdef PUNCH_DEBUG
		printf("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, شمار, max, رقم);
#endif
		اگر (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, شمار, رقم);
			اگر (retval)
				برو errout;
			اگر (شمار > max)
				شمار -= max - start;
			وگرنه
				بشکن;
			start = 0;
		} وگرنه
			start -= max;
		bp += رقم;
		اگر (level == 0) {
			رقم = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	اگر (buf)
		ext2fs_free_mem(&buf);
	بازگشت retval;
}
#undef BLK_T_MAX

#ifdef PUNCH_DEBUG

#define dbg_printf(f, a...)  printf(f, ## a)

ایستا تهی dbg_print_extent(نویسه *desc, ساختار ext2fs_extent *extent)
{
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

}
#وگرنه
#define dbg_print_extent(desc, ex)	انجامبده { } وقتی (0)
#define dbg_printf(f, a...)		انجامبده { } وقتی (0)
#endif

/* Free a range of blocks, respecting cluster boundaries */
ایستا errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     ساختار ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_شمار, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	اگر (free_start < fs->super->s_first_data_block ||
	    (free_start + free_شمار) > ext2fs_blocks_شمار(fs->super))
		بازگشت EXT2_ET_BAD_BLOCK_رقم;

	/* No bigalloc?  Just free each block. */
	اگر (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_شمار;
		وقتی (free_شمار-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		بازگشت retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	اگر (free_start & EXT2FS_CLUSTER_MASK(fs)) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		اگر (retval)
			برو errout;
		اگر (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs) -
			(free_start & EXT2FS_CLUSTER_MASK(fs));
		اگر (cluster_freed > free_شمار)
			cluster_freed = free_شمار;
		free_شمار -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	وقتی (free_شمار > 0 && free_شمار >= (unsigned) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_شمار -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	اگر (free_شمار > 0) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		اگر (retval)
			برو errout;
		اگر (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	بازگشت retval;
}

ایستا errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     ساختار ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	ساختار ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, next, lfree_start;
	__u32			free_شمار, newlen;
	blk64_t			freed = 0;
	صحیح			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	اگر (retval)
		بازگشت retval;
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
	اگر (retval == EXT2_ET_NO_CURRENT_گره) {
		retval = 0;
		برو errout;
	} وگرنه اگر (retval)
		برو errout;
	وقتی (1) {
		op = EXT2_EXTENT_NEXT_LEAF;
		dbg_print_extent("main loop", &extent);
		next = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (unsigned long long) start,
			   (unsigned long long) end,
			   (unsigned long long) next);
		اگر (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			اگر (end < extent.e_lblk)
				بشکن;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			اگر (next > end)
				free_شمار = end - extent.e_lblk + 1;
			وگرنه
				free_شمار = extent.e_len;
			extent.e_len -= free_شمار;
			extent.e_lblk += free_شمار;
			extent.e_pblk += free_شمار;
		} وگرنه اگر (end >= next-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			اگر (start >= next)
				برو next_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_شمار = extent.e_len - newlen;
			extent.e_len = newlen;
		} وگرنه {
			ساختار ext2fs_extent	newex;

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
			free_شمار = end - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_درج(handle,
					EXT2_EXTENT_درج_AFTER, &newex);
			اگر (retval)
				برو errout;
			retval = ext2fs_extent_fix_parents(handle);
			اگر (retval)
				برو errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			اگر (retval)
				برو errout;
		} 
		اگر (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			اگر (retval)
				برو errout;
			retval = ext2fs_extent_fix_parents(handle);
		} وگرنه {
			ساختار ext2fs_extent	newex;
			blk64_t			old_lblk, next_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			اگر (retval)
				برو errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &newex);
			اگر (retval == EXT2_ET_EXTENT_NO_NEXT)
				next_lblk = old_lblk;
			وگرنه اگر (retval)
				برو errout;
			وگرنه
				next_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			اگر (retval)
				برو errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			اگر (retval)
				برو errout;

			retval = ext2fs_extent_fix_parents(handle);
			اگر (retval && retval != EXT2_ET_NO_CURRENT_گره)
				برو errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(تهی)ext2fs_extent_goto(handle, next_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		اگر (retval)
			برو errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_شمار);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_شمار, &freed);
		اگر (retval)
			برو errout;
	next_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		اگر (retval == EXT2_ET_EXTENT_NO_NEXT ||
		    retval == EXT2_ET_NO_CURRENT_گره)
			بشکن;
		اگر (retval)
			برو errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	بازگشت retval;
}
	
ایستا errcode_t ext2fs_punch_inline_data(ext2_filsys fs, ext2_ino_t ino,
					  ساختار ext2_inode *inode,
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
	اگر (start > 0)
		بازگشت 0;

	memset((نویسه *)inode->i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	اگر (retval)
		بازگشت retval;

	بازگشت ext2fs_inline_data_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       ساختار ext2_inode *inode,
		       نویسه *block_buf, blk64_t start,
		       blk64_t end)
{
	errcode_t		retval;
	ساختار ext2_inode	inode_buf;

	اگر (start > end)
		بازگشت EINVAL;

	/* Read inode structure if necessary */
	اگر (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		اگر (retval)
			بازگشت retval;
		inode = &inode_buf;
	}
	اگر (inode->i_flags & EXT4_INLINE_DATA_FL)
		بازگشت ext2fs_punch_inline_data(fs, ino, inode, start, end);
	وگرنه اگر (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	وگرنه
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	اگر (retval)
		بازگشت retval;

#ifdef PUNCH_DEBUG
	printf("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_SIZE(inode), inode->i_blocks);
#endif
	بازگشت ext2fs_write_inode(fs, ino, inode);
}
