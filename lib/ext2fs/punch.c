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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#undef PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
स्थिर पूर्णाङ्क check_zero_block(वर्ण *buf, पूर्णाङ्क blocksize)
{
	वर्ण	*cp = buf;
	पूर्णाङ्क	बायाँ = blocksize;

	यावत् (बायाँ > 0) {
		यदि (*cp++)
			प्रत्यागम 0;
		बायाँ--;
	}
	प्रत्यागम 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
स्थिर errcode_t ind_punch(ext2_filsys fs, संरचना ext2_inode *inode,
			   वर्ण *block_buf, blk_t *p, पूर्णाङ्क level,
			   blk64_t start, blk64_t गिनती, पूर्णाङ्क max)
{
	errcode_t	retval;
	blk_t		b;
	पूर्णाङ्क		i;
	blk64_t		offset, incr;
	ध्रुव blk64_t	end = start + गिनती;
	पूर्णाङ्क		freed = 0;

#ifdef PUNCH_DEBUG
	printf("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, गिनती, max);
#endif
	incr = 1ULL << ((EXT2_BLOCK_SIZE_BITS(fs->super) - 2) * level);
	प्रति (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		यदि (offset >= end)
			भङ्ग;
		यदि (*p == 0 || (offset+incr) <= start)
			अनुवर्त;
		b = *p;
		यदि (level > 0) {
			blk_t start2;
			blk_t count2;
#ifdef PUNCH_DEBUG
			printf("Reading indirect block %u\n", b);
#endif
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			यदि (retval)
				प्रत्यागम retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#ifdef PUNCH_DEBUG
			printf("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, गिनती, end, incr, start2, count2);
#endif
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			यदि (retval)
				प्रत्यागम retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			यदि (retval)
				प्रत्यागम retval;
			यदि (!check_zero_block(block_buf, fs->blocksize))
				अनुवर्त;
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
	प्रत्यागम ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#define BLK_T_MAX ((blk_t)~0ULL)
स्थिर errcode_t ext2fs_punch_ind(ext2_filsys fs, संरचना ext2_inode *inode,
				  वर्ण *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	वर्ण			*buf = 0;
	पूर्णाङ्क			level;
	पूर्णाङ्क			अंक = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			गिनती;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	यदि (start > BLK_T_MAX)
		प्रत्यागम 0;
	यदि (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		गिनती = BLK_T_MAX - start;
	अन्यथा
		गिनती = end - start + 1;

	यदि (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		यदि (retval)
			प्रत्यागम retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	प्रति (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#ifdef PUNCH_DEBUG
		printf("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, गिनती, max, अंक);
#endif
		यदि (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, गिनती, अंक);
			यदि (retval)
				गच्छ errout;
			यदि (गिनती > max)
				गिनती -= max - start;
			अन्यथा
				भङ्ग;
			start = 0;
		} अन्यथा
			start -= max;
		bp += अंक;
		यदि (level == 0) {
			अंक = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	यदि (buf)
		ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}
#undef BLK_T_MAX

#ifdef PUNCH_DEBUG

#define dbg_printf(f, a...)  printf(f, ## a)

स्थिर शून्य dbg_print_extent(वर्ण *desc, संरचना ext2fs_extent *extent)
{
	यदि (desc)
		printf("%s: ", desc);
	printf("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		fputs("LEAF ", stdout);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		fputs("2ND_VISIT ", stdout);
	यदि (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#अन्यथा
#define dbg_print_extent(desc, ex)	कुरु { } यावत् (0)
#define dbg_printf(f, a...)		कुरु { } यावत् (0)
#endif

/* Free a range of blocks, respecting cluster boundaries */
स्थिर errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_गिनती, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	यदि (free_start < fs->super->s_first_data_block ||
	    (free_start + free_गिनती) > ext2fs_blocks_गिनती(fs->super))
		प्रत्यागम EXT2_ET_BAD_BLOCK_अंक;

	/* No bigalloc?  Just free each block. */
	यदि (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_गिनती;
		यावत् (free_गिनती-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		प्रत्यागम retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	यदि (free_start & EXT2FS_CLUSTER_MASK(fs)) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		यदि (retval)
			गच्छ errout;
		यदि (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs) -
			(free_start & EXT2FS_CLUSTER_MASK(fs));
		यदि (cluster_freed > free_गिनती)
			cluster_freed = free_गिनती;
		free_गिनती -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	यावत् (free_गिनती > 0 && free_गिनती >= (unsigned) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_गिनती -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	यदि (free_गिनती > 0) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		यदि (retval)
			गच्छ errout;
		यदि (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	प्रत्यागम retval;
}

स्थिर errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	संरचना ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, next, lfree_start;
	__u32			free_गिनती, newlen;
	blk64_t			freed = 0;
	पूर्णाङ्क			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	यदि (retval)
		प्रत्यागम retval;
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
	यदि (retval == EXT2_ET_NO_CURRENT_गाँठ) {
		retval = 0;
		गच्छ errout;
	} अन्यथा यदि (retval)
		गच्छ errout;
	यावत् (1) {
		op = EXT2_EXTENT_NEXT_LEAF;
		dbg_print_extent("main loop", &extent);
		next = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (unsigned long long) start,
			   (unsigned long long) end,
			   (unsigned long long) next);
		यदि (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			यदि (end < extent.e_lblk)
				भङ्ग;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			यदि (next > end)
				free_गिनती = end - extent.e_lblk + 1;
			अन्यथा
				free_गिनती = extent.e_len;
			extent.e_len -= free_गिनती;
			extent.e_lblk += free_गिनती;
			extent.e_pblk += free_गिनती;
		} अन्यथा यदि (end >= next-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			यदि (start >= next)
				गच्छ next_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_गिनती = extent.e_len - newlen;
			extent.e_len = newlen;
		} अन्यथा {
			संरचना ext2fs_extent	newex;

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
			free_गिनती = end - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_जोड़ो(handle,
					EXT2_EXTENT_जोड़ो_AFTER, &newex);
			यदि (retval)
				गच्छ errout;
			retval = ext2fs_extent_fix_parents(handle);
			यदि (retval)
				गच्छ errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			यदि (retval)
				गच्छ errout;
		} 
		यदि (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			यदि (retval)
				गच्छ errout;
			retval = ext2fs_extent_fix_parents(handle);
		} अन्यथा {
			संरचना ext2fs_extent	newex;
			blk64_t			old_lblk, next_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			यदि (retval)
				गच्छ errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_NEXT_LEAF,
						   &newex);
			यदि (retval == EXT2_ET_EXTENT_NO_NEXT)
				next_lblk = old_lblk;
			अन्यथा यदि (retval)
				गच्छ errout;
			अन्यथा
				next_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			यदि (retval)
				गच्छ errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			यदि (retval)
				गच्छ errout;

			retval = ext2fs_extent_fix_parents(handle);
			यदि (retval && retval != EXT2_ET_NO_CURRENT_गाँठ)
				गच्छ errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(शून्य)ext2fs_extent_goto(handle, next_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		यदि (retval)
			गच्छ errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_गिनती);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_गिनती, &freed);
		यदि (retval)
			गच्छ errout;
	next_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		यदि (retval == EXT2_ET_EXTENT_NO_NEXT ||
		    retval == EXT2_ET_NO_CURRENT_गाँठ)
			भङ्ग;
		यदि (retval)
			गच्छ errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	प्रत्यागम retval;
}
	
स्थिर errcode_t ext2fs_punch_inline_data(ext2_filsys fs, ext2_ino_t ino,
					  संरचना ext2_inode *inode,
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
	यदि (start > 0)
		प्रत्यागम 0;

	memset((वर्ण *)inode->i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम ext2fs_inline_data_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       संरचना ext2_inode *inode,
		       वर्ण *block_buf, blk64_t start,
		       blk64_t end)
{
	errcode_t		retval;
	संरचना ext2_inode	inode_buf;

	यदि (start > end)
		प्रत्यागम EINVAL;

	/* Read inode structure if necessary */
	यदि (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		यदि (retval)
			प्रत्यागम retval;
		inode = &inode_buf;
	}
	यदि (inode->i_flags & EXT4_INLINE_DATA_FL)
		प्रत्यागम ext2fs_punch_inline_data(fs, ino, inode, start, end);
	अन्यथा यदि (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	अन्यथा
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	यदि (retval)
		प्रत्यागम retval;

#ifdef PUNCH_DEBUG
	printf("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_SIZE(inode), inode->i_blocks);
#endif
	प्रत्यागम ext2fs_write_inode(fs, ino, inode);
}
