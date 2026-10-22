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

	जबसम्म (बायाँ > 0) {
		यदि (*cp++)
			फर्काऊ 0;
		बायाँ--;
	}
	फर्काऊ 1;
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
			   blk64_t start, blk64_t गन्ती, पूर्णाङ्क max)
{
	errcode_t	retval;
	blk_t		b;
	पूर्णाङ्क		i;
	blk64_t		offset, incr;
	अचर blk64_t	end = start + गन्ती;
	पूर्णाङ्क		freed = 0;

#ifdef PUNCH_DEBUG
	म_लिखो("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, गन्ती, max);
#endif
	incr = 1ULL << ((EXT2_BLOCK_SIZE_BITS(fs->super) - 2) * level);
	लागि (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		यदि (offset >= end)
			रोक;
		यदि (*p == 0 || (offset+incr) <= start)
			जारी;
		b = *p;
		यदि (level > 0) {
			blk_t start2;
			blk_t count2;
#ifdef PUNCH_DEBUG
			म_लिखो("Reading indirect block %u\n", b);
#endif
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			यदि (retval)
				फर्काऊ retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#ifdef PUNCH_DEBUG
			म_लिखो("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, गन्ती, end, incr, start2, count2);
#endif
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			यदि (retval)
				फर्काऊ retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			यदि (retval)
				फर्काऊ retval;
			यदि (!check_zero_block(block_buf, fs->blocksize))
				जारी;
		}
#ifdef PUNCH_DEBUG
		म_लिखो("Freeing block %u (offset %llu)\n", b, offset);
#endif
		ext2fs_block_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#ifdef PUNCH_DEBUG
	म_लिखो("Freed %d blocks\n", freed);
#endif
	फर्काऊ ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#define BLK_T_MAX ((blk_t)~0ULL)
स्थिर errcode_t ext2fs_punch_ind(ext2_filsys fs, संरचना ext2_inode *inode,
				  वर्ण *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	वर्ण			*buf = 0;
	पूर्णाङ्क			level;
	पूर्णाङ्क			अङ्क = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			गन्ती;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	यदि (start > BLK_T_MAX)
		फर्काऊ 0;
	यदि (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		गन्ती = BLK_T_MAX - start;
	अन्यथा
		गन्ती = end - start + 1;

	यदि (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		यदि (retval)
			फर्काऊ retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	लागि (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#ifdef PUNCH_DEBUG
		म_लिखो("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, गन्ती, max, अङ्क);
#endif
		यदि (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, गन्ती, अङ्क);
			यदि (retval)
				गच्छ errout;
			यदि (गन्ती > max)
				गन्ती -= max - start;
			अन्यथा
				रोक;
			start = 0;
		} अन्यथा
			start -= max;
		bp += अङ्क;
		यदि (level == 0) {
			अङ्क = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	यदि (buf)
		ext2fs_free_mem(&buf);
	फर्काऊ retval;
}
#undef BLK_T_MAX

#ifdef PUNCH_DEBUG

#define dbg_printf(f, a...)  म_लिखो(f, ## a)

स्थिर शून्य dbg_print_extent(वर्ण *desc, संरचना ext2fs_extent *extent)
{
	यदि (desc)
		म_लिखो("%s: ", desc);
	म_लिखो("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_LEAF)
		ख_माला_दो("LEAF ", मानक_निकास);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		ख_माला_दो("UNINIT ", मानक_निकास);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_VISIT)
		ख_माला_दो("2ND_VISIT ", मानक_निकास);
	यदि (!extent->e_flags)
		ख_माला_दो("(none)", मानक_निकास);
	ख_अक्षर_दो('\n', मानक_निकास);

}
#अन्यथा
#define dbg_print_extent(desc, ex)	गर { } जबसम्म (0)
#define dbg_printf(f, a...)		गर { } जबसम्म (0)
#endif

/* Free a range of blocks, respecting cluster boundaries */
स्थिर errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_गन्ती, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	यदि (free_start < fs->super->s_first_data_block ||
	    (free_start + free_गन्ती) > ext2fs_blocks_गन्ती(fs->super))
		फर्काऊ EXT2_ET_BAD_BLOCK_अङ्क;

	/* No bigalloc?  Just free each block. */
	यदि (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_गन्ती;
		जबसम्म (free_गन्ती-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		फर्काऊ retval;
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
		यदि (cluster_freed > free_गन्ती)
			cluster_freed = free_गन्ती;
		free_गन्ती -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	जबसम्म (free_गन्ती > 0 && free_गन्ती >= (अचिन्हित) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_गन्ती -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	यदि (free_गन्ती > 0) {
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
	फर्काऊ retval;
}

स्थिर errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	संरचना ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, next, lfree_start;
	__u32			free_गन्ती, newlen;
	blk64_t			freed = 0;
	पूर्णाङ्क			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	यदि (retval)
		फर्काऊ retval;
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
	यदि (retval == EXT2_ET_NO_CURRENT_गाँठो) {
		retval = 0;
		गच्छ errout;
	} अन्यथा यदि (retval)
		गच्छ errout;
	जबसम्म (1) {
		op = EXT2_EXTENT_NEXT_LEAF;
		dbg_print_extent("main loop", &extent);
		next = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (अचिन्हित दीर्घ दीर्घ) start,
			   (अचिन्हित दीर्घ दीर्घ) end,
			   (अचिन्हित दीर्घ दीर्घ) next);
		यदि (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			यदि (end < extent.e_lblk)
				रोक;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			यदि (next > end)
				free_गन्ती = end - extent.e_lblk + 1;
			अन्यथा
				free_गन्ती = extent.e_len;
			extent.e_len -= free_गन्ती;
			extent.e_lblk += free_गन्ती;
			extent.e_pblk += free_गन्ती;
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
			free_गन्ती = extent.e_len - newlen;
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
			free_गन्ती = end - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_घुसाउ(handle,
					EXT2_EXTENT_घुसाउ_AFTER, &newex);
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
			यदि (retval && retval != EXT2_ET_NO_CURRENT_गाँठो)
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
		       free_start, free_गन्ती);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_गन्ती, &freed);
		यदि (retval)
			गच्छ errout;
	next_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		यदि (retval == EXT2_ET_EXTENT_NO_NEXT ||
		    retval == EXT2_ET_NO_CURRENT_गाँठो)
			रोक;
		यदि (retval)
			गच्छ errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	फर्काऊ retval;
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
		फर्काऊ 0;

	स_रखो((वर्ण *)inode->i_block, 0, EXT4_MIN_INLINE_DATA_SIZE);
	inode->i_size = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	यदि (retval)
		फर्काऊ retval;

	फर्काऊ ext2fs_inline_data_ea_remove(fs, ino);
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
		फर्काऊ EINVAL;

	/* Read inode structure if necessary */
	यदि (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		यदि (retval)
			फर्काऊ retval;
		inode = &inode_buf;
	}
	यदि (inode->i_flags & EXT4_INLINE_DATA_FL)
		फर्काऊ ext2fs_punch_inline_data(fs, ino, inode, start, end);
	अन्यथा यदि (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	अन्यथा
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	यदि (retval)
		फर्काऊ retval;

#ifdef PUNCH_DEBUG
	म_लिखो("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_SIZE(inode), inode->i_blocks);
#endif
	फर्काऊ ext2fs_write_inode(fs, ino, inode);
}
