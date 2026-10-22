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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <errno.h>

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

#undef PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
static ចំនួនគត់ check_zero_block(var_na *buf, ចំនួនគត់ blocksize)
{
	var_na	*cp = buf;
	ចំនួនគត់	left = blocksize;

	ខណៈពេល (left > 0) {
		ប្រសិនបើ (*cp++)
			ត្រឡប់ 0;
		left--;
	}
	ត្រឡប់ 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
static errcode_t ind_punch(ext2_filsys fs, sa_nracanaa ext2_inode *inode,
			   var_na *block_buf, blk_t *p, ចំនួនគត់ level,
			   blk64_t start, blk64_t count, ចំនួនគត់ max)
{
	errcode_t	retval;
	blk_t		b;
	ចំនួនគត់		i;
	blk64_t		offset, incr;
	const blk64_t	end = start + count;
	ចំនួនគត់		freed = 0;

#ifdef PUNCH_DEBUG
	printf("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, count, max);
#endif
	incr = 1ULL << ((EXT2_BLOCK_SIZE_BITS(fs->super) - 2) * level);
	សម្រាប់ (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		ប្រសិនបើ (offset >= end)
			bha_nga;
		ប្រសិនបើ (*p == 0 || (offset+incr) <= start)
			continue;
		b = *p;
		ប្រសិនបើ (level > 0) {
			blk_t start2;
			blk_t count2;
#ifdef PUNCH_DEBUG
			printf("Reading indirect block %u\n", b);
#endif
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			ប្រសិនបើ (retval)
				ត្រឡប់ retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#ifdef PUNCH_DEBUG
			printf("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, count, end, incr, start2, count2);
#endif
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			ប្រសិនបើ (retval)
				ត្រឡប់ retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			ប្រសិនបើ (retval)
				ត្រឡប់ retval;
			ប្រសិនបើ (!check_zero_block(block_buf, fs->blocksize))
				continue;
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
	ត្រឡប់ ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#define BLK_T_MAX ((blk_t)~0ULL)
static errcode_t ext2fs_punch_ind(ext2_filsys fs, sa_nracanaa ext2_inode *inode,
				  var_na *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	var_na			*buf = 0;
	ចំនួនគត់			level;
	ចំនួនគត់			num = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			count;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	ប្រសិនបើ (start > BLK_T_MAX)
		ត្រឡប់ 0;
	ប្រសិនបើ (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		count = BLK_T_MAX - start;
	បើមិនដូច្នេះ
		count = end - start + 1;

	ប្រសិនបើ (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		ប្រសិនបើ (retval)
			ត្រឡប់ retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	សម្រាប់ (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#ifdef PUNCH_DEBUG
		printf("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, count, max, num);
#endif
		ប្រសិនបើ (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, count, num);
			ប្រសិនបើ (retval)
				goto errout;
			ប្រសិនបើ (count > max)
				count -= max - start;
			បើមិនដូច្នេះ
				bha_nga;
			start = 0;
		} បើមិនដូច្នេះ
			start -= max;
		bp += num;
		ប្រសិនបើ (level == 0) {
			num = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	ប្រសិនបើ (buf)
		ext2fs_free_mem(&buf);
	ត្រឡប់ retval;
}
#undef BLK_T_MAX

#ifdef PUNCH_DEBUG

#define dbg_printf(f, a...)  printf(f, ## a)

static void dbg_print_extent(var_na *desc, sa_nracanaa ext2fs_extent *extent)
{
	ប្រសិនបើ (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ប្រសិនបើ (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	ប្រសិនបើ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ប្រសិនបើ (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	ប្រសិនបើ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#បើមិនដូច្នេះ
#define dbg_print_extent(desc, ex)	do { } ខណៈពេល (0)
#define dbg_printf(f, a...)		do { } ខណៈពេល (0)
#endif

/* Free a range of blocks, respecting cluster boundaries */
static errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     sa_nracanaa ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_count, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	ប្រសិនបើ (free_start < fs->super->s_first_data_block ||
	    (free_start + free_count) > ext2fs_blocks_count(fs->super))
		ត្រឡប់ EXT2_ET_BAD_BLOCK_NUM;

	/* No bigalloc?  Just free each block. */
	ប្រសិនបើ (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_count;
		ខណៈពេល (free_count-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		ត្រឡប់ retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	ប្រសិនបើ (free_start & EXT2FS_CLUSTER_MASK(fs)) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		ប្រសិនបើ (retval)
			goto errout;
		ប្រសិនបើ (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs) -
			(free_start & EXT2FS_CLUSTER_MASK(fs));
		ប្រសិនបើ (cluster_freed > free_count)
			cluster_freed = free_count;
		free_count -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	ខណៈពេល (free_count > 0 && free_count >= (unsigned) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_count -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	ប្រសិនបើ (free_count > 0) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		ប្រសិនបើ (retval)
			goto errout;
		ប្រសិនបើ (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	ត្រឡប់ retval;
}

static errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     sa_nracanaa ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	sa_nracanaa ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, next, lfree_start;
	__u32			free_count, newlen;
	blk64_t			freed = 0;
	ចំនួនគត់			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;
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
	ប្រសិនបើ (retval == EXT2_ET_NO_CURRENT_NODE) {
		retval = 0;
		goto errout;
	} បើមិនដូច្នេះ ប្រសិនបើ (retval)
		goto errout;
	ខណៈពេល (1) {
		op = EXT2_EXTENT_NEXT_LEAF;
		dbg_print_extent("main loop", &extent);
		next = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (unsigned long long) start,
			   (unsigned long long) end,
			   (unsigned long long) next);
		ប្រសិនបើ (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			ប្រសិនបើ (end < extent.e_lblk)
				bha_nga;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			ប្រសិនបើ (next > end)
				free_count = end - extent.e_lblk + 1;
			បើមិនដូច្នេះ
				free_count = extent.e_len;
			extent.e_len -= free_count;
			extent.e_lblk += free_count;
			extent.e_pblk += free_count;
		} បើមិនដូច្នេះ ប្រសិនបើ (end >= next-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			ប្រសិនបើ (start >= next)
				goto next_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_count = extent.e_len - newlen;
			extent.e_len = newlen;
		} បើមិនដូច្នេះ {
			sa_nracanaa ext2fs_extent	newex;

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
			free_count = end - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_insert(handle,
					EXT2_EXTENT_INSERT_AFTER, &newex);
			ប្រសិនបើ (retval)
				goto errout;
			retval = ext2fs_extent_fix_parents(handle);
			ប្រសិនបើ (retval)
				goto errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			ប្រសិនបើ (retval)
				goto errout;
		} 
		ប្រសិនបើ (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			ប្រសិនបើ (retval)
				goto errout;
			retval = ext2fs_extent_fix_parents(handle);
		} បើមិនដូច្នេះ {
			sa_nracanaa ext2fs_extent	newex;
			blk64_t			old_lblk, next_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			ប្រសិនបើ (retval)
				goto errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &newex);
			ប្រសិនបើ (retval == EXT2_ET_EXTENT_NO_NEXT)
				next_lblk = old_lblk;
			បើមិនដូច្នេះ ប្រសិនបើ (retval)
				goto errout;
			បើមិនដូច្នេះ
				next_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			ប្រសិនបើ (retval)
				goto errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			ប្រសិនបើ (retval)
				goto errout;

			retval = ext2fs_extent_fix_parents(handle);
			ប្រសិនបើ (retval && retval != EXT2_ET_NO_CURRENT_NODE)
				goto errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(void)ext2fs_extent_goto(handle, next_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		ប្រសិនបើ (retval)
			goto errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_count);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_count, &freed);
		ប្រសិនបើ (retval)
			goto errout;
	next_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		ប្រសិនបើ (retval == EXT2_ET_EXTENT_NO_NEXT ||
		    retval == EXT2_ET_NO_CURRENT_NODE)
			bha_nga;
		ប្រសិនបើ (retval)
			goto errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	ត្រឡប់ retval;
}
	
static errcode_t ext2fs_punch_inline_data(ext2_filsys fs, ext2_ino_t ino,
					  sa_nracanaa ext2_inode *inode,
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
	ប្រសិនបើ (start > 0)
		ត្រឡប់ 0;

	memset((var_na *)inode->i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

	ត្រឡប់ ext2fs_inline_data_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       sa_nracanaa ext2_inode *inode,
		       var_na *block_buf, blk64_t start,
		       blk64_t end)
{
	errcode_t		retval;
	sa_nracanaa ext2_inode	inode_buf;

	ប្រសិនបើ (start > end)
		ត្រឡប់ EINVAL;

	/* Read inode structure if necessary */
	ប្រសិនបើ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ប្រសិនបើ (retval)
			ត្រឡប់ retval;
		inode = &inode_buf;
	}
	ប្រសិនបើ (inode->i_flags & EXT4_INLINE_DATA_FL)
		ត្រឡប់ ext2fs_punch_inline_data(fs, ino, inode, start, end);
	បើមិនដូច្នេះ ប្រសិនបើ (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	បើមិនដូច្នេះ
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	ប្រសិនបើ (retval)
		ត្រឡប់ retval;

#ifdef PUNCH_DEBUG
	printf("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_SIZE(inode), inode->i_blocks);
#endif
	ត្រឡប់ ext2fs_write_inode(fs, ino, inode);
}
