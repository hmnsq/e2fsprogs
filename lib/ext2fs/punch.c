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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <errno.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

#अघोषित PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
अटल पूर्णांक check_zero_block(अक्षर *buf, पूर्णांक blocksize)
{
	अक्षर	*cp = buf;
	पूर्णांक	बायाँ = blocksize;

	जबतक (बायाँ > 0) {
		अगर (*cp++)
			वापस 0;
		बायाँ--;
	}
	वापस 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
अटल errcode_t ind_punch(ext2_filsys fs, काष्ठा ext2_inode *inode,
			   अक्षर *block_buf, blk_t *p, पूर्णांक level,
			   blk64_t start, blk64_t गिनती, पूर्णांक max)
{
	errcode_t	retval;
	blk_t		b;
	पूर्णांक		i;
	blk64_t		offset, incr;
	स्थिर blk64_t	end = start + गिनती;
	पूर्णांक		freed = 0;

#अगर_घोषित PUNCH_DEBUG
	म_लिखो("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, गिनती, max);
#पूर्ण_अगर
	incr = 1ULL << ((EXT2_BLOCK_आकार_BITS(fs->super) - 2) * level);
	क्रम (i = 0, offset = 0; i < max; i++, p++, offset += incr) {
		अगर (offset >= end)
			अवरोध;
		अगर (*p == 0 || (offset+incr) <= start)
			जारी;
		b = *p;
		अगर (level > 0) {
			blk_t start2;
			blk_t count2;
#अगर_घोषित PUNCH_DEBUG
			म_लिखो("Reading indirect block %u\n", b);
#पूर्ण_अगर
			retval = ext2fs_read_ind_block(fs, b, block_buf);
			अगर (retval)
				वापस retval;
			start2 = (start > offset) ? start - offset : 0;
			count2 = end - ((start > offset) ? start : offset);
#अगर_घोषित PUNCH_DEBUG
			म_लिखो("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       offset, गिनती, end, incr, start2, count2);
#पूर्ण_अगर
			retval = ind_punch(fs, inode, block_buf + fs->blocksize,
					   (blk_t *) block_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			अगर (retval)
				वापस retval;
			retval = ext2fs_write_ind_block(fs, b, block_buf);
			अगर (retval)
				वापस retval;
			अगर (!check_zero_block(block_buf, fs->blocksize))
				जारी;
		}
#अगर_घोषित PUNCH_DEBUG
		म_लिखो("Freeing block %u (offset %llu)\n", b, offset);
#पूर्ण_अगर
		ext2fs_block_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#अगर_घोषित PUNCH_DEBUG
	म_लिखो("Freed %d blocks\n", freed);
#पूर्ण_अगर
	वापस ext2fs_iblk_sub_blocks(fs, inode, freed);
}

#घोषणा BLK_T_MAX ((blk_t)~0ULL)
अटल errcode_t ext2fs_punch_ind(ext2_filsys fs, काष्ठा ext2_inode *inode,
				  अक्षर *block_buf, blk64_t start, blk64_t end)
{
	errcode_t		retval;
	अक्षर			*buf = 0;
	पूर्णांक			level;
	पूर्णांक			अंक = EXT2_NDIR_BLOCKS;
	blk_t			*bp = inode->i_block;
	blk_t			addr_per_block;
	blk64_t			max = EXT2_NDIR_BLOCKS;
	blk_t			गिनती;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	अगर (start > BLK_T_MAX)
		वापस 0;
	अगर (end >= BLK_T_MAX || end - start + 1 >= BLK_T_MAX)
		गिनती = BLK_T_MAX - start;
	अन्यथा
		गिनती = end - start + 1;

	अगर (!block_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		अगर (retval)
			वापस retval;
		block_buf = buf;
	}

	addr_per_block = (blk_t)fs->blocksize >> 2;

	क्रम (level = 0; level < 4; level++, max *= (blk64_t)addr_per_block) {
#अगर_घोषित PUNCH_DEBUG
		म_लिखो("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, गिनती, max, अंक);
#पूर्ण_अगर
		अगर (start < max) {
			retval = ind_punch(fs, inode, block_buf, bp, level,
					   start, गिनती, अंक);
			अगर (retval)
				जाओ errout;
			अगर (गिनती > max)
				गिनती -= max - start;
			अन्यथा
				अवरोध;
			start = 0;
		} अन्यथा
			start -= max;
		bp += अंक;
		अगर (level == 0) {
			अंक = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	अगर (buf)
		ext2fs_free_mem(&buf);
	वापस retval;
}
#अघोषित BLK_T_MAX

#अगर_घोषित PUNCH_DEBUG

#घोषणा dbg_printf(f, a...)  म_लिखो(f, ## a)

अटल व्योम dbg_print_extent(अक्षर *desc, काष्ठा ext2fs_extent *extent)
{
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

}
#अन्यथा
#घोषणा dbg_print_extent(desc, ex)	करो { } जबतक (0)
#घोषणा dbg_printf(f, a...)		करो { } जबतक (0)
#पूर्ण_अगर

/* Free a range of blocks, respecting cluster boundaries */
अटल errcode_t punch_extent_blocks(ext2_filsys fs, ext2_ino_t ino,
				     काष्ठा ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_गिनती, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	अगर (free_start < fs->super->s_first_तथ्य_block ||
	    (free_start + free_गिनती) > ext2fs_blocks_गिनती(fs->super))
		वापस EXT2_ET_BAD_BLOCK_अंक;

	/* No bigalloc?  Just free each block. */
	अगर (EXT2FS_CLUSTER_RATIO(fs) == 1) {
		*freed += free_गिनती;
		जबतक (free_गिनती-- > 0)
			ext2fs_block_alloc_stats2(fs, free_start++, -1);
		वापस retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	अगर (free_start & EXT2FS_CLUSTER_MASK(fs)) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		अगर (retval)
			जाओ errout;
		अगर (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs) -
			(free_start & EXT2FS_CLUSTER_MASK(fs));
		अगर (cluster_freed > free_गिनती)
			cluster_freed = free_गिनती;
		free_गिनती -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	जबतक (free_गिनती > 0 && free_गिनती >= (अचिन्हित) EXT2FS_CLUSTER_RATIO(fs)) {
		ext2fs_block_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2FS_CLUSTER_RATIO(fs);
		free_गिनती -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	अगर (free_गिनती > 0) {
		retval = ext2fs_map_cluster_block(fs, ino, inode,
						  lfree_start, &pblk);
		अगर (retval)
			जाओ errout;
		अगर (!pblk) {
			ext2fs_block_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	वापस retval;
}

अटल errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     काष्ठा ext2_inode *inode,
				     blk64_t start, blk64_t end)
{
	ext2_extent_handle_t	handle = 0;
	काष्ठा ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, अग्र, lfree_start;
	__u32			free_गिनती, newlen;
	blk64_t			freed = 0;
	पूर्णांक			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	अगर (retval)
		वापस retval;
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
	अगर (retval == EXT2_ET_NO_CURRENT_गाँठ) {
		retval = 0;
		जाओ errout;
	} अन्यथा अगर (retval)
		जाओ errout;
	जबतक (1) {
		op = EXT2_EXTENT_अग्र_LEAF;
		dbg_print_extent("main loop", &extent);
		अग्र = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (अचिन्हित दीर्घ दीर्घ) start,
			   (अचिन्हित दीर्घ दीर्घ) end,
			   (अचिन्हित दीर्घ दीर्घ) अग्र);
		अगर (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			अगर (end < extent.e_lblk)
				अवरोध;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			अगर (अग्र > end)
				free_गिनती = end - extent.e_lblk + 1;
			अन्यथा
				free_गिनती = extent.e_len;
			extent.e_len -= free_गिनती;
			extent.e_lblk += free_गिनती;
			extent.e_pblk += free_गिनती;
		} अन्यथा अगर (end >= अग्र-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			अगर (start >= अग्र)
				जाओ अग्र_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_गिनती = extent.e_len - newlen;
			extent.e_len = newlen;
		} अन्यथा {
			काष्ठा ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(end + 1 - extent.e_lblk);
			newex.e_lblk = end + 1;
			newex.e_len = अग्र - end - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = start - extent.e_lblk;
			free_start = extent.e_pblk + extent.e_len;
			lfree_start = extent.e_lblk + extent.e_len;
			free_गिनती = end - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_जोड़ो(handle,
					EXT2_EXTENT_जोड़ो_AFTER, &newex);
			अगर (retval)
				जाओ errout;
			retval = ext2fs_extent_fix_parents(handle);
			अगर (retval)
				जाओ errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			अगर (retval)
				जाओ errout;
		} 
		अगर (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			अगर (retval)
				जाओ errout;
			retval = ext2fs_extent_fix_parents(handle);
		} अन्यथा {
			काष्ठा ext2fs_extent	newex;
			blk64_t			old_lblk, अग्र_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			अगर (retval)
				जाओ errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_अग्र_LEAF,
						   &newex);
			अगर (retval == EXT2_ET_EXTENT_NO_अग्र)
				अग्र_lblk = old_lblk;
			अन्यथा अगर (retval)
				जाओ errout;
			अन्यथा
				अग्र_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			अगर (retval)
				जाओ errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			अगर (retval)
				जाओ errout;

			retval = ext2fs_extent_fix_parents(handle);
			अगर (retval && retval != EXT2_ET_NO_CURRENT_गाँठ)
				जाओ errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(व्योम)ext2fs_extent_goto(handle, अग्र_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		अगर (retval)
			जाओ errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_गिनती);
		retval = punch_extent_blocks(fs, ino, inode, lfree_start,
					     free_start, free_गिनती, &freed);
		अगर (retval)
			जाओ errout;
	अग्र_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		अगर (retval == EXT2_ET_EXTENT_NO_अग्र ||
		    retval == EXT2_ET_NO_CURRENT_गाँठ)
			अवरोध;
		अगर (retval)
			जाओ errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_blocks(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	वापस retval;
}
	
अटल errcode_t ext2fs_punch_inline_तथ्य(ext2_filsys fs, ext2_ino_t ino,
					  काष्ठा ext2_inode *inode,
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
	अगर (start > 0)
		वापस 0;

	स_रखो((अक्षर *)inode->i_block, 0, EXT4_MIN_INLINE_तथ्य_आकार);
	inode->i_आकार = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	अगर (retval)
		वापस retval;

	वापस ext2fs_inline_तथ्य_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       काष्ठा ext2_inode *inode,
		       अक्षर *block_buf, blk64_t start,
		       blk64_t end)
{
	errcode_t		retval;
	काष्ठा ext2_inode	inode_buf;

	अगर (start > end)
		वापस EINVAL;

	/* Read inode structure if necessary */
	अगर (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		अगर (retval)
			वापस retval;
		inode = &inode_buf;
	}
	अगर (inode->i_flags & EXT4_INLINE_तथ्य_FL)
		वापस ext2fs_punch_inline_तथ्य(fs, ino, inode, start, end);
	अन्यथा अगर (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, end);
	अन्यथा
		retval = ext2fs_punch_ind(fs, inode, block_buf, start, end);
	अगर (retval)
		वापस retval;

#अगर_घोषित PUNCH_DEBUG
	म_लिखो("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_आकार(inode), inode->i_blocks);
#पूर्ण_अगर
	वापस ext2fs_write_inode(fs, ino, inode);
}
