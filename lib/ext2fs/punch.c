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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

#undef PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ check_zero_block(അക്ഷരം *buf, പൂർണ്ണസംഖ്യ blocksize)
{
	അക്ഷരം	*cp = buf;
	പൂർണ്ണസംഖ്യ	ഇടത് = blocksize;

	ആകുമ്പോൾ (ഇടത് > 0) {
		എങ്കിൽ (*cp++)
			തിരികെ 0;
		ഇടത്--;
	}
	തിരികെ 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
സ്ഥിതം errcode_t ind_punch(ext2_filsys fs, ഘടന ext2_inode *inode,
			   അക്ഷരം *block_buf, blk_t *p, പൂർണ്ണസംഖ്യ level,
			   blk64_t start, blk64_t എണ്ണം, പൂർണ്ണസംഖ്യ max)
{
	errcode_t	retval;
	blk_t		b;
	പൂർണ്ണസംഖ്യ		i;
	blk64_t		offset, incr;
	സ്ഥിരം blk64_t	end = start + എണ്ണം;
	പൂർണ്ണസംഖ്യ		freed = 0;

#ifdef PUNCH_DEBUG
	printf("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, എണ്ണം, max);
#endif
	incr = 1ULL << ((EXT2_BLOCK_SIZE_BITS(fs->super) - 2) * level);
	വേണ്ടി (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		എങ്കിൽ (offset >= end)
			നിർത്തുക;
		എങ്കിൽ (*p == 0 || (offset+incr) <= start)
			തുടരുക;
		b = *p;
		എങ്കിൽ (level > 0) {
			blk_t start2;
			blk_t count2;
#ifdef PUNCH_DEBUG
			printf("Reading indirect block %u\n", b);
#endif
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			എങ്കിൽ (retval)
				തിരികെ retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#ifdef PUNCH_DEBUG
			printf("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, എണ്ണം, end, incr, start2, count2);
#endif
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			എങ്കിൽ (retval)
				തിരികെ retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			എങ്കിൽ (retval)
				തിരികെ retval;
			എങ്കിൽ (!check_zero_block(block_buf, fs->blocksize))
				തുടരുക;
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
	തിരികെ ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#define BLK_T_MAX ((blk_t)~0ULL)
സ്ഥിതം errcode_t ext2fs_punch_ind(ext2_filsys fs, ഘടന ext2_inode *inode,
				  അക്ഷരം *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	അക്ഷരം			*buf = 0;
	പൂർണ്ണസംഖ്യ			level;
	പൂർണ്ണസംഖ്യ			അക്കം = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			എണ്ണം;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	എങ്കിൽ (start > BLK_T_MAX)
		തിരികെ 0;
	എങ്കിൽ (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		എണ്ണം = BLK_T_MAX - start;
	അല്ലെങ്കിൽ
		എണ്ണം = end - start + 1;

	എങ്കിൽ (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	വേണ്ടി (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#ifdef PUNCH_DEBUG
		printf("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, എണ്ണം, max, അക്കം);
#endif
		എങ്കിൽ (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, എണ്ണം, അക്കം);
			എങ്കിൽ (retval)
				പോകുക errout;
			എങ്കിൽ (എണ്ണം > max)
				എണ്ണം -= max - start;
			അല്ലെങ്കിൽ
				നിർത്തുക;
			start = 0;
		} അല്ലെങ്കിൽ
			start -= max;
		bp += അക്കം;
		എങ്കിൽ (level == 0) {
			അക്കം = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	എങ്കിൽ (buf)
		ext2fs_free_mem(&buf);
	തിരികെ retval;
}
#undef BLK_T_MAX

#ifdef PUNCH_DEBUG

#define dbg_printf(f, a...)  printf(f, ## a)

സ്ഥിതം ശൂന്യം dbg_print_extent(അക്ഷരം *desc, ഘടന ext2fs_extent *extent)
{
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

}
#അല്ലെങ്കിൽ
#define dbg_print_extent(desc, ex)	ചെയ്യുക { } ആകുമ്പോൾ (0)
#define dbg_printf(f, a...)		ചെയ്യുക { } ആകുമ്പോൾ (0)
#endif

/* Free a range of blocks, respecting cluster boundaries */
സ്ഥിതം errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     ഘടന ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_എണ്ണം, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	എങ്കിൽ (free_start < fs->super->s_first_data_block ||
	    (free_start + free_എണ്ണം) > ext2fs_blocks_എണ്ണം(fs->super))
		തിരികെ EXT2_ET_BAD_BLOCK_അക്കം;

	/* No bigalloc?  Just free each block. */
	എങ്കിൽ (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_എണ്ണം;
		ആകുമ്പോൾ (free_എണ്ണം-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		തിരികെ retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	എങ്കിൽ (free_start & EXT2FS_CLUSTER_MASK(fs)) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		എങ്കിൽ (retval)
			പോകുക errout;
		എങ്കിൽ (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs) -
			(free_start & EXT2FS_CLUSTER_MASK(fs));
		എങ്കിൽ (cluster_freed > free_എണ്ണം)
			cluster_freed = free_എണ്ണം;
		free_എണ്ണം -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	ആകുമ്പോൾ (free_എണ്ണം > 0 && free_എണ്ണം >= (unsigned) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_എണ്ണം -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	എങ്കിൽ (free_എണ്ണം > 0) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		എങ്കിൽ (retval)
			പോകുക errout;
		എങ്കിൽ (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	തിരികെ retval;
}

സ്ഥിതം errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     ഘടന ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	ഘടന ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, next, lfree_start;
	__u32			free_എണ്ണം, newlen;
	blk64_t			freed = 0;
	പൂർണ്ണസംഖ്യ			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	എങ്കിൽ (retval)
		തിരികെ retval;
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
	എങ്കിൽ (retval == EXT2_ET_NO_CURRENT_കണ്ണി) {
		retval = 0;
		പോകുക errout;
	} അല്ലെങ്കിൽ എങ്കിൽ (retval)
		പോകുക errout;
	ആകുമ്പോൾ (1) {
		op = EXT2_EXTENT_NEXT_LEAF;
		dbg_print_extent("main loop", &extent);
		next = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (unsigned long long) start,
			   (unsigned long long) end,
			   (unsigned long long) next);
		എങ്കിൽ (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			എങ്കിൽ (end < extent.e_lblk)
				നിർത്തുക;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			എങ്കിൽ (next > end)
				free_എണ്ണം = end - extent.e_lblk + 1;
			അല്ലെങ്കിൽ
				free_എണ്ണം = extent.e_len;
			extent.e_len -= free_എണ്ണം;
			extent.e_lblk += free_എണ്ണം;
			extent.e_pblk += free_എണ്ണം;
		} അല്ലെങ്കിൽ എങ്കിൽ (end >= next-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			എങ്കിൽ (start >= next)
				പോകുക next_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_എണ്ണം = extent.e_len - newlen;
			extent.e_len = newlen;
		} അല്ലെങ്കിൽ {
			ഘടന ext2fs_extent	newex;

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
			free_എണ്ണം = end - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_തിരുകുക(handle,
					EXT2_EXTENT_തിരുകുക_AFTER, &newex);
			എങ്കിൽ (retval)
				പോകുക errout;
			retval = ext2fs_extent_fix_parents(handle);
			എങ്കിൽ (retval)
				പോകുക errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			എങ്കിൽ (retval)
				പോകുക errout;
		} 
		എങ്കിൽ (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			എങ്കിൽ (retval)
				പോകുക errout;
			retval = ext2fs_extent_fix_parents(handle);
		} അല്ലെങ്കിൽ {
			ഘടന ext2fs_extent	newex;
			blk64_t			old_lblk, next_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			എങ്കിൽ (retval)
				പോകുക errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &newex);
			എങ്കിൽ (retval == EXT2_ET_EXTENT_NO_NEXT)
				next_lblk = old_lblk;
			അല്ലെങ്കിൽ എങ്കിൽ (retval)
				പോകുക errout;
			അല്ലെങ്കിൽ
				next_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			എങ്കിൽ (retval)
				പോകുക errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			എങ്കിൽ (retval)
				പോകുക errout;

			retval = ext2fs_extent_fix_parents(handle);
			എങ്കിൽ (retval && retval != EXT2_ET_NO_CURRENT_കണ്ണി)
				പോകുക errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(ശൂന്യം)ext2fs_extent_goto(handle, next_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		എങ്കിൽ (retval)
			പോകുക errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_എണ്ണം);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_എണ്ണം, &freed);
		എങ്കിൽ (retval)
			പോകുക errout;
	next_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		എങ്കിൽ (retval == EXT2_ET_EXTENT_NO_NEXT ||
		    retval == EXT2_ET_NO_CURRENT_കണ്ണി)
			നിർത്തുക;
		എങ്കിൽ (retval)
			പോകുക errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	തിരികെ retval;
}
	
സ്ഥിതം errcode_t ext2fs_punch_inline_data(ext2_filsys fs, ext2_ino_t ino,
					  ഘടന ext2_inode *inode,
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
	എങ്കിൽ (start > 0)
		തിരികെ 0;

	memset((അക്ഷരം *)inode->i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	തിരികെ ext2fs_inline_data_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       ഘടന ext2_inode *inode,
		       അക്ഷരം *block_buf, blk64_t start,
		       blk64_t end)
{
	errcode_t		retval;
	ഘടന ext2_inode	inode_buf;

	എങ്കിൽ (start > end)
		തിരികെ EINVAL;

	/* Read inode structure if necessary */
	എങ്കിൽ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
		inode = &inode_buf;
	}
	എങ്കിൽ (inode->i_flags & EXT4_INLINE_DATA_FL)
		തിരികെ ext2fs_punch_inline_data(fs, ino, inode, start, end);
	അല്ലെങ്കിൽ എങ്കിൽ (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	അല്ലെങ്കിൽ
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	എങ്കിൽ (retval)
		തിരികെ retval;

#ifdef PUNCH_DEBUG
	printf("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_SIZE(inode), inode->i_blocks);
#endif
	തിരികെ ext2fs_write_inode(fs, ino, inode);
}
