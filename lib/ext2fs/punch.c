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
#यदि_अन्तः
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#अपरिभाषय PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
स्थिर पूर्णाङ्क check_zero_खण्डम्(वर्ण *buf, पूर्णाङ्क blocksize)
{
	वर्ण	*cp = buf;
	पूर्णाङ्क	वामः = blocksize;

	यावत् (वामः > 0) {
		यदि (*cp++)
			प्रत्यागम 0;
		वामः--;
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
			   वर्ण *खण्डम्_buf, blk_t *p, पूर्णाङ्क level,
			   blk64_t start, blk64_t गणनम्, पूर्णाङ्क max)
{
	errcode_t	retval;
	blk_t		b;
	पूर्णाङ्क		i;
	blk64_t		विस्थापनम्, incr;
	ध्रुव blk64_t	अन्तः = start + गणनम्;
	पूर्णाङ्क		freed = 0;

#यदि_परिभाषितम् PUNCH_DEBUG
	प्रारूप_लिख("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, start, गणनम्, max);
#यदि_अन्तः
	incr = 1ULL << ((EXT2_खण्डम्_आकार_द्वयङ्काः(fs->super) - 2) * level);
	प्रति (i = 0, विस्थापनम् = 0; i < max; i++, p++, विस्थापनम् += incr) {
		यदि (विस्थापनम् >= अन्तः)
			भङ्ग;
		यदि (*p == 0 || (विस्थापनम्+incr) <= start)
			अनुवर्त;
		b = *p;
		यदि (level > 0) {
			blk_t start2;
			blk_t count2;
#यदि_परिभाषितम् PUNCH_DEBUG
			प्रारूप_लिख("Reading indirect block %u\n", b);
#यदि_अन्तः
			retval = ext2fs_read_ind_खण्डम्(fs, b, खण्डम्_buf);
			यदि (retval)
				प्रत्यागम retval;
			start2 = (start > विस्थापनम्) ? start - विस्थापनम् : 0;
			count2 = अन्तः - ((start > विस्थापनम्) ? start : विस्थापनम्);
#यदि_परिभाषितम् PUNCH_DEBUG
			प्रारूप_लिख("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", start,
			       विस्थापनम्, गणनम्, अन्तः, incr, start2, count2);
#यदि_अन्तः
			retval = ind_punch(fs, inode, खण्डम्_buf + fs->blocksize,
					   (blk_t *) खण्डम्_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			यदि (retval)
				प्रत्यागम retval;
			retval = ext2fs_write_ind_खण्डम्(fs, b, खण्डम्_buf);
			यदि (retval)
				प्रत्यागम retval;
			यदि (!check_zero_खण्डम्(खण्डम्_buf, fs->blocksize))
				अनुवर्त;
		}
#यदि_परिभाषितम् PUNCH_DEBUG
		प्रारूप_लिख("Freeing block %u (offset %llu)\n", b, विस्थापनम्);
#यदि_अन्तः
		ext2fs_खण्डम्_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#यदि_परिभाषितम् PUNCH_DEBUG
	प्रारूप_लिख("Freed %d blocks\n", freed);
#यदि_अन्तः
	प्रत्यागम ext2fs_iblk_sub_खण्डानि(fs, inode, freed);
}

#परिभाषय BLK_T_MAX ((blk_t)~0ULL)
स्थिर errcode_t ext2fs_punch_ind(ext2_filsys fs, संरचना ext2_inode *inode,
				  वर्ण *खण्डम्_buf, blk64_t start, blk64_t अन्तः)
{
	errcode_t		retval;
	वर्ण			*buf = 0;
	पूर्णाङ्क			level;
	पूर्णाङ्क			अङ्कः = EXT2_NDIR_खण्डानि;
	blk_t			*bp = inode->i_खण्डम्;
	blk_t			addr_प्रत्येकम्_खण्डम्;
	blk64_t			max = EXT2_NDIR_खण्डानि;
	blk_t			गणनम्;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	यदि (start > BLK_T_MAX)
		प्रत्यागम 0;
	यदि (अन्तः >= BLK_T_MAX || अन्तः - start + 1 >= BLK_T_MAX)
		गणनम् = BLK_T_MAX - start;
	अन्यथा
		गणनम् = अन्तः - start + 1;

	यदि (!खण्डम्_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		यदि (retval)
			प्रत्यागम retval;
		खण्डम्_buf = buf;
	}

	addr_प्रत्येकम्_खण्डम् = (blk_t)fs->blocksize >> 2;

	प्रति (level = 0; level < 4; level++, max *= (blk64_t)addr_प्रत्येकम्_खण्डम्) {
#यदि_परिभाषितम् PUNCH_DEBUG
		प्रारूप_लिख("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, start, गणनम्, max, अङ्कः);
#यदि_अन्तः
		यदि (start < max) {
			retval = ind_punch(fs, inode, खण्डम्_buf, bp, level,
					   start, गणनम्, अङ्कः);
			यदि (retval)
				गच्छ errout;
			यदि (गणनम् > max)
				गणनम् -= max - start;
			अन्यथा
				भङ्ग;
			start = 0;
		} अन्यथा
			start -= max;
		bp += अङ्कः;
		यदि (level == 0) {
			अङ्कः = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	यदि (buf)
		ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}
#अपरिभाषय BLK_T_MAX

#यदि_परिभाषितम् PUNCH_DEBUG

#परिभाषय dbg_printf(f, a...)  प्रारूप_लिख(f, ## a)

स्थिर शून्य dbg_print_extent(वर्ण *desc, संरचना ext2fs_extent *extent)
{
	यदि (desc)
		प्रारूप_लिख("%s: ", desc);
	प्रारूप_लिख("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_पर्णम्)
		fputs("LEAF ", stdout);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	यदि (extent->e_flags & EXT2_EXTENT_FLAGS_SECOND_दर्शय)
		fputs("2ND_VISIT ", stdout);
	यदि (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#अन्यथा
#परिभाषय dbg_print_extent(desc, ex)	कुरु { } यावत् (0)
#परिभाषय dbg_printf(f, a...)		कुरु { } यावत् (0)
#यदि_अन्तः

/* Free a range of blocks, respecting cluster boundaries */
स्थिर errcode_t punch_extent_खण्डानि(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode *inode,
				     blk64_t lfree_start, blk64_t free_start,
				     __u32 free_गणनम्, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	यदि (free_start < fs->super->s_first_दत्त_खण्डम् ||
	    (free_start + free_गणनम्) > ext2fs_खण्डानि_गणनम्(fs->super))
		प्रत्यागम EXT2_ET_BAD_खण्डम्_अङ्कः;

	/* No bigalloc?  Just free each block. */
	यदि (EXT2_FS_CLUSTER_अनुपातः(fs) == 1) {
		*freed += free_गणनम्;
		यावत् (free_गणनम्-- > 0)
			ext2fs_खण्डम्_alloc_stats2(fs, free_start++, -1);
		प्रत्यागम retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	यदि (free_start & EXT2_FS_CLUSTER_आवरणम्(fs)) {
		retval = ext2fs_map_cluster_खण्डम्(fs, ino, inode,
						  lfree_start, &pblk);
		यदि (retval)
			गच्छ errout;
		यदि (!pblk) {
			ext2fs_खण्डम्_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
		cluster_freed = EXT2_FS_CLUSTER_अनुपातः(fs) -
			(free_start & EXT2_FS_CLUSTER_आवरणम्(fs));
		यदि (cluster_freed > free_गणनम्)
			cluster_freed = free_गणनम्;
		free_गणनम् -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	यावत् (free_गणनम् > 0 && free_गणनम् >= (अचिह्नित) EXT2_FS_CLUSTER_अनुपातः(fs)) {
		ext2fs_खण्डम्_alloc_stats2(fs, free_start, -1);
		freed_now++;
		cluster_freed = EXT2_FS_CLUSTER_अनुपातः(fs);
		free_गणनम् -= cluster_freed;
		free_start += cluster_freed;
		lfree_start += cluster_freed;
	}

	/* Try to free the last cluster. */
	यदि (free_गणनम् > 0) {
		retval = ext2fs_map_cluster_खण्डम्(fs, ino, inode,
						  lfree_start, &pblk);
		यदि (retval)
			गच्छ errout;
		यदि (!pblk) {
			ext2fs_खण्डम्_alloc_stats2(fs, free_start, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	प्रत्यागम retval;
}

स्थिर errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     संरचना ext2_inode *inode,
				     blk64_t start, blk64_t अन्तः)
{
	ext2_extent_handle_t	handle = 0;
	संरचना ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_start, अग्र, lfree_start;
	__u32			free_गणनम्, newlen;
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
	यदि (retval == EXT2_ET_NO_CURRENT_ग्रन्थिः) {
		retval = 0;
		गच्छ errout;
	} अन्यथा यदि (retval)
		गच्छ errout;
	यावत् (1) {
		op = EXT2_EXTENT_अग्र_पर्णम्;
		dbg_print_extent("main loop", &extent);
		अग्र = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (अचिह्नित दीर्घ दीर्घ) start,
			   (अचिह्नित दीर्घ दीर्घ) अन्तः,
			   (अचिह्नित दीर्घ दीर्घ) अग्र);
		यदि (start <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			यदि (अन्तः < extent.e_lblk)
				भङ्ग;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_start = extent.e_pblk;
			lfree_start = extent.e_lblk;
			यदि (अग्र > अन्तः)
				free_गणनम् = अन्तः - extent.e_lblk + 1;
			अन्यथा
				free_गणनम् = extent.e_len;
			extent.e_len -= free_गणनम्;
			extent.e_lblk += free_गणनम्;
			extent.e_pblk += free_गणनम्;
		} अन्यथा यदि (अन्तः >= अग्र-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			यदि (start >= अग्र)
				गच्छ अग्र_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = start - extent.e_lblk;
			free_start = extent.e_pblk + newlen;
			lfree_start = extent.e_lblk + newlen;
			free_गणनम् = extent.e_len - newlen;
			extent.e_len = newlen;
		} अन्यथा {
			संरचना ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(अन्तः + 1 - extent.e_lblk);
			newex.e_lblk = अन्तः + 1;
			newex.e_len = अग्र - अन्तः - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = start - extent.e_lblk;
			free_start = extent.e_pblk + extent.e_len;
			lfree_start = extent.e_lblk + extent.e_len;
			free_गणनम् = अन्तः - start + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_निवेशय(handle,
					EXT2_EXTENT_निवेशय_AFTER, &newex);
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
			blk64_t			old_lblk, अग्र_lblk;
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
						   EXT2_EXTENT_अग्र_पर्णम्,
						   &newex);
			यदि (retval == EXT2_ET_EXTENT_NO_अग्र)
				अग्र_lblk = old_lblk;
			अन्यथा यदि (retval)
				गच्छ errout;
			अन्यथा
				अग्र_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			यदि (retval)
				गच्छ errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			यदि (retval)
				गच्छ errout;

			retval = ext2fs_extent_fix_parents(handle);
			यदि (retval && retval != EXT2_ET_NO_CURRENT_ग्रन्थिः)
				गच्छ errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(शून्य)ext2fs_extent_goto(handle, अग्र_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		यदि (retval)
			गच्छ errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_start, free_गणनम्);
		retval = punch_extent_खण्डानि(fs, ino, inode, lfree_start,
					     free_start, free_गणनम्, &freed);
		यदि (retval)
			गच्छ errout;
	अग्र_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		यदि (retval == EXT2_ET_EXTENT_NO_अग्र ||
		    retval == EXT2_ET_NO_CURRENT_ग्रन्थिः)
			भङ्ग;
		यदि (retval)
			गच्छ errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_खण्डानि(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	प्रत्यागम retval;
}
	
स्थिर errcode_t ext2fs_punch_inline_दत्त(ext2_filsys fs, ext2_ino_t ino,
					  संरचना ext2_inode *inode,
					  blk64_t start,
					  blk64_t अन्तः EXT2FS_ATTR((अप्रयुक्तम्)))
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

	स्मृतिं_पूरय((वर्ण *)inode->i_खण्डम्, 0, EXT4_MIN_INLINE_दत्त_आकार);
	inode->i_आकार = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम ext2fs_inline_दत्त_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       संरचना ext2_inode *inode,
		       वर्ण *खण्डम्_buf, blk64_t start,
		       blk64_t अन्तः)
{
	errcode_t		retval;
	संरचना ext2_inode	inode_buf;

	यदि (start > अन्तः)
		प्रत्यागम EINVAL;

	/* Read inode structure if necessary */
	यदि (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		यदि (retval)
			प्रत्यागम retval;
		inode = &inode_buf;
	}
	यदि (inode->i_flags & EXT4_INLINE_दत्त_FL)
		प्रत्यागम ext2fs_punch_inline_दत्त(fs, ino, inode, start, अन्तः);
	अन्यथा यदि (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, start, अन्तः);
	अन्यथा
		retval = ext2fs_punch_ind(fs, inode, खण्डम्_buf, start, अन्तः);
	यदि (retval)
		प्रत्यागम retval;

#यदि_परिभाषितम् PUNCH_DEBUG
	प्रारूप_लिख("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_आकार(inode), inode->i_खण्डानि);
#यदि_अन्तः
	प्रत्यागम ext2fs_write_inode(fs, ino, inode);
}
