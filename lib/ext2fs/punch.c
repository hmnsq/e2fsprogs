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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#undef PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ساكن صحيح check_zero_block(حرف *buf, صحيح blocksize)
{
	حرف	*cp = buf;
	صحيح	هغو = blocksize;

	طالما (هغو > 0) {
		إذا (*cp++)
			أرجع 0;
		هغو--;
	}
	أرجع 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
ساكن errcode_t ind_punch(ext2_filsys fs, بنية ext2_inode *inode,
			   حرف *block_buf, blk_t *p, صحيح level,
			   blk64_t start, blk64_t كدي, صحيح max)
{
	errcode_t	retval;
	blk_t		b;
	صحيح		i;
	blk64_t		offset, incr;
	ثابت blk64_t	end = start + كدي;
	صحيح		freed = 0;

#ifdef PUNCH_DEBUG
	printf("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, كدي, max);
#endif
	incr = 1ULL << ((EXT2_BLOCK_SIZE_BITS(fs->super) - 2) * level);
	لكل (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		إذا (offset >= end)
			اكسر;
		إذا (*p == 0 || (offset+incr) <= start)
			استمر;
		b = *p;
		إذا (level > 0) {
			blk_t start2;
			blk_t count2;
#ifdef PUNCH_DEBUG
			printf("Reading indirect block %u\n", b);
#endif
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			إذا (retval)
				أرجع retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#ifdef PUNCH_DEBUG
			printf("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, كدي, end, incr, start2, count2);
#endif
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			إذا (retval)
				أرجع retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			إذا (retval)
				أرجع retval;
			إذا (!check_zero_block(block_buf, fs->blocksize))
				استمر;
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
	أرجع ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#define BLK_T_MAX ((blk_t)~0ULL)
ساكن errcode_t ext2fs_punch_ind(ext2_filsys fs, بنية ext2_inode *inode,
				  حرف *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	حرف			*buf = 0;
	صحيح			level;
	صحيح			لمب = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			كدي;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	إذا (start > BLK_T_MAX)
		أرجع 0;
	إذا (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		كدي = BLK_T_MAX - start;
	وإلا
		كدي = end - start + 1;

	إذا (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	لكل (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#ifdef PUNCH_DEBUG
		printf("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, كدي, max, لمب);
#endif
		إذا (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, كدي, لمب);
			إذا (retval)
				اذهب errout;
			إذا (كدي > max)
				كدي -= max - start;
			وإلا
				اكسر;
			start = 0;
		} وإلا
			start -= max;
		bp += لمب;
		إذا (level == 0) {
			لمب = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	إذا (buf)
		ext2fs_free_mem(&buf);
	أرجع retval;
}
#undef BLK_T_MAX

#ifdef PUNCH_DEBUG

#define dbg_printf(f, a...)  printf(f, ## a)

ساكن فراغ dbg_بغ_extent(حرف *desc, بنية ext2fs_extent *extent)
{
	إذا (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#وإلا
#define dbg_بغ_extent(desc, ex)	افعل { } طالما (0)
#define dbg_printf(f, a...)		افعل { } طالما (0)
#endif

/* Free a range of blocks, respecting cluster boundaries */
ساكن errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_كدي, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	إذا (free_start < fs->super->s_first_data_block ||
	    (free_start + free_كدي) > ext2fs_blocks_كدي(fs->super))
		أرجع EXT2_ET_BAD_BLOCK_لمب;

	/* No bigalloc?  Just free each block. */
	إذا (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_كدي;
		طالما (free_كدي-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		أرجع retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	إذا (free_start & EXT2FS_CLUSTER_MASK(fs)) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		إذا (retval)
			اذهب errout;
		إذا (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs) -
			(free_start & EXT2FS_CLUSTER_MASK(fs));
		إذا (cluster_freed > free_كدي)
			cluster_freed = free_كدي;
		free_كدي -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	طالما (free_كدي > 0 && free_كدي >= (unsigned) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_كدي -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	إذا (free_كدي > 0) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		إذا (retval)
			اذهب errout;
		إذا (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	أرجع retval;
}

ساكن errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	بنية ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, next, lfree_start;
	__u32			free_كدي, newlen;
	blk64_t			freed = 0;
	صحيح			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	إذا (retval)
		أرجع retval;
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
	إذا (retval == EXT2_ET_NO_CURRENT_كلي) {
		retval = 0;
		اذهب errout;
	} وإلا إذا (retval)
		اذهب errout;
	طالما (1) {
		op = EXT2_EXTENT_NEXT_LEAF;
		dbg_بغ_extent("main loop", &extent);
		next = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (unsigned long long) start,
			   (unsigned long long) end,
			   (unsigned long long) next);
		إذا (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			إذا (end < extent.e_lblk)
				اكسر;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			إذا (next > end)
				free_كدي = end - extent.e_lblk + 1;
			وإلا
				free_كدي = extent.e_len;
			extent.e_len -= free_كدي;
			extent.e_lblk += free_كدي;
			extent.e_pblk += free_كدي;
		} وإلا إذا (end >= next-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			إذا (start >= next)
				اذهب next_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_كدي = extent.e_len - newlen;
			extent.e_len = newlen;
		} وإلا {
			بنية ext2fs_extent	newex;

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
			free_كدي = end - start + 1;

			dbg_بغ_extent("inserting", &newex);
			retval = ext2fs_extent_سك(handle,
					EXT2_EXTENT_سك_AFTER, &newex);
			إذا (retval)
				اذهب errout;
			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval)
				اذهب errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			إذا (retval)
				اذهب errout;
		} 
		إذا (extent.e_len) {
			dbg_بغ_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			إذا (retval)
				اذهب errout;
			retval = ext2fs_extent_fix_parents(handle);
		} وإلا {
			بنية ext2fs_extent	newex;
			blk64_t			old_lblk, next_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			إذا (retval)
				اذهب errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &newex);
			إذا (retval == EXT2_ET_EXTENT_NO_NEXT)
				next_lblk = old_lblk;
			وإلا إذا (retval)
				اذهب errout;
			وإلا
				next_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			إذا (retval)
				اذهب errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			إذا (retval)
				اذهب errout;

			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval && retval != EXT2_ET_NO_CURRENT_كلي)
				اذهب errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(فراغ)ext2fs_extent_goto(handle, next_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		إذا (retval)
			اذهب errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_كدي);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_كدي, &freed);
		إذا (retval)
			اذهب errout;
	next_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		إذا (retval == EXT2_ET_EXTENT_NO_NEXT ||
		    retval == EXT2_ET_NO_CURRENT_كلي)
			اكسر;
		إذا (retval)
			اذهب errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	أرجع retval;
}
	
ساكن errcode_t ext2fs_punch_inline_data(ext2_filsys fs, ext2_ino_t ino,
					  بنية ext2_inode *inode,
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
	إذا (start > 0)
		أرجع 0;

	memset((حرف *)inode->i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		أرجع retval;

	أرجع ext2fs_inline_data_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       بنية ext2_inode *inode,
		       حرف *block_buf, blk64_t start,
		       blk64_t end)
{
	errcode_t		retval;
	بنية ext2_inode	inode_buf;

	إذا (start > end)
		أرجع EINVAL;

	/* Read inode structure if necessary */
	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}
	إذا (inode->i_flags & EXT4_INLINE_DATA_FL)
		أرجع ext2fs_punch_inline_data(fs, ino, inode, start, end);
	وإلا إذا (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	وإلا
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	إذا (retval)
		أرجع retval;

#ifdef PUNCH_DEBUG
	printf("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_SIZE(inode), inode->i_blocks);
#endif
	أرجع ext2fs_write_inode(fs, ino, inode);
}
