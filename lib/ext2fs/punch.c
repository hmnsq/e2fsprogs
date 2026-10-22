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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <errno.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

#నిర్వచనం_రద్దు PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
స్థిర పూర్ణాంకం check_zero_ఖండం(అక్షరం *buf, పూర్ణాంకం blocksize)
{
	అక్షరం	*cp = buf;
	పూర్ణాంకం	ఎడమ = blocksize;

	అయితే (ఎడమ > 0) {
		ఒకవేళ (*cp++)
			తిరిగి 0;
		ఎడమ--;
	}
	తిరిగి 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
స్థిర errcode_t ind_punch(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
			   అక్షరం *ఖండం_buf, blk_t *p, పూర్ణాంకం level,
			   blk64_t ప్రారంభం, blk64_t లెక్క, పూర్ణాంకం max)
{
	errcode_t	retval;
	blk_t		b;
	పూర్ణాంకం		i;
	blk64_t		స్థానభ్రంశం, incr;
	స్థిరరాశి blk64_t	చివర = ప్రారంభం + లెక్క;
	పూర్ణాంకం		freed = 0;

#నిర్వచితమైతే PUNCH_DEBUG
	రాయి("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, ప్రారంభం, లెక్క, max);
#అయితే_ముగింపు
	incr = 1ULL << ((EXT2_ఖండం_కొలత_బిట్లు(fs->super) - 2) * level);
	కొరకు (i = 0, స్థానభ్రంశం = 0; i < max; i++, p++, స్థానభ్రంశం += incr) {
		ఒకవేళ (స్థానభ్రంశం >= చివర)
			ఆపు;
		ఒకవేళ (*p == 0 || (స్థానభ్రంశం+incr) <= ప్రారంభం)
			కొనసాగు;
		b = *p;
		ఒకవేళ (level > 0) {
			blk_t start2;
			blk_t count2;
#నిర్వచితమైతే PUNCH_DEBUG
			రాయి("Reading indirect block %u\n", b);
#అయితే_ముగింపు
			retval = ext2fs_read_ind_ఖండం(fs, b, ఖండం_buf);
			ఒకవేళ (retval)
				తిరిగి retval;
			start2 = (ప్రారంభం > స్థానభ్రంశం) ? ప్రారంభం - స్థానభ్రంశం : 0;
			count2 = చివర - ((ప్రారంభం > స్థానభ్రంశం) ? ప్రారంభం : స్థానభ్రంశం);
#నిర్వచితమైతే PUNCH_DEBUG
			రాయి("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", ప్రారంభం,
			       స్థానభ్రంశం, లెక్క, చివర, incr, start2, count2);
#అయితే_ముగింపు
			retval = ind_punch(fs, inode, ఖండం_buf + fs->blocksize,
					   (blk_t *) ఖండం_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			ఒకవేళ (retval)
				తిరిగి retval;
			retval = ext2fs_write_ind_ఖండం(fs, b, ఖండం_buf);
			ఒకవేళ (retval)
				తిరిగి retval;
			ఒకవేళ (!check_zero_ఖండం(ఖండం_buf, fs->blocksize))
				కొనసాగు;
		}
#నిర్వచితమైతే PUNCH_DEBUG
		రాయి("Freeing block %u (offset %llu)\n", b, స్థానభ్రంశం);
#అయితే_ముగింపు
		ext2fs_ఖండం_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#నిర్వచితమైతే PUNCH_DEBUG
	రాయి("Freed %d blocks\n", freed);
#అయితే_ముగింపు
	తిరిగి ext2fs_iblk_sub_ఖండాలు(fs, inode, freed);
}

#నిర్వచించు BLK_T_MAX ((blk_t)~0ULL)
స్థిర errcode_t ext2fs_punch_ind(ext2_filsys fs, నిర్మాణం ext2_inode *inode,
				  అక్షరం *ఖండం_buf, blk64_t ప్రారంభం, blk64_t చివర)
{
	errcode_t		retval;
	అక్షరం			*buf = 0;
	పూర్ణాంకం			level;
	పూర్ణాంకం			అంకె = EXT2_NDIR_ఖండాలు;
	blk_t			*bp = inode->i_ఖండం;
	blk_t			addr_చొప్పున_ఖండం;
	blk64_t			max = EXT2_NDIR_ఖండాలు;
	blk_t			లెక్క;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	ఒకవేళ (ప్రారంభం > BLK_T_MAX)
		తిరిగి 0;
	ఒకవేళ (చివర >= BLK_T_MAX || చివర - ప్రారంభం + 1 >= BLK_T_MAX)
		లెక్క = BLK_T_MAX - ప్రారంభం;
	లేకపోతే
		లెక్క = చివర - ప్రారంభం + 1;

	ఒకవేళ (!ఖండం_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		ఒకవేళ (retval)
			తిరిగి retval;
		ఖండం_buf = buf;
	}

	addr_చొప్పున_ఖండం = (blk_t)fs->blocksize >> 2;

	కొరకు (level = 0; level < 4; level++, max *= (blk64_t)addr_చొప్పున_ఖండం) {
#నిర్వచితమైతే PUNCH_DEBUG
		రాయి("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, ప్రారంభం, లెక్క, max, అంకె);
#అయితే_ముగింపు
		ఒకవేళ (ప్రారంభం < max) {
			retval = ind_punch(fs, inode, ఖండం_buf, bp, level,
					   ప్రారంభం, లెక్క, అంకె);
			ఒకవేళ (retval)
				వెళ్ళు errout;
			ఒకవేళ (లెక్క > max)
				లెక్క -= max - ప్రారంభం;
			లేకపోతే
				ఆపు;
			ప్రారంభం = 0;
		} లేకపోతే
			ప్రారంభం -= max;
		bp += అంకె;
		ఒకవేళ (level == 0) {
			అంకె = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	ఒకవేళ (buf)
		ext2fs_free_mem(&buf);
	తిరిగి retval;
}
#నిర్వచనం_రద్దు BLK_T_MAX

#నిర్వచితమైతే PUNCH_DEBUG

#నిర్వచించు dbg_printf(f, a...)  రాయి(f, ## a)

స్థిర శూన్య dbg_print_extent(అక్షరం *desc, నిర్మాణం ext2fs_extent *extent)
{
	ఒకవేళ (desc)
		రాయి("%s: ", desc);
	రాయి("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_ఆకు)
		fputs("LEAF ", stdout);
	ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ఒకవేళ (extent->e_flags & EXT2_EXTENT_FLAGS_రెండవ_సందర్శించు)
		fputs("2ND_VISIT ", stdout);
	ఒకవేళ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#లేకపోతే
#నిర్వచించు dbg_print_extent(desc, ex)	చేయి { } అయితే (0)
#నిర్వచించు dbg_printf(f, a...)		చేయి { } అయితే (0)
#అయితే_ముగింపు

/* Free a range of blocks, respecting cluster boundaries */
స్థిర errcode_t punch_extent_ఖండాలు(ext2_filsys fs, ext2_ino_t ino,
				     నిర్మాణం ext2_inode *inode,
				     blk64_t lfree_ప్రారంభం, blk64_t free_ప్రారంభం,
				     __u32 free_లెక్క, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	ఒకవేళ (free_ప్రారంభం < fs->super->s_మొదటి_దత్తాంశం_ఖండం ||
	    (free_ప్రారంభం + free_లెక్క) > ext2fs_ఖండాలు_లెక్క(fs->super))
		తిరిగి EXT2_ET_BAD_ఖండం_అంకె;

	/* No bigalloc?  Just free each block. */
	ఒకవేళ (EXT2_FS_CLUSTER_నిష్పత్తి(fs) == 1) {
		*freed += free_లెక్క;
		అయితే (free_లెక్క-- > 0)
			ext2fs_ఖండం_alloc_stats2(fs, free_ప్రారంభం++, -1);
		తిరిగి retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	ఒకవేళ (free_ప్రారంభం & EXT2_FS_CLUSTER_ముసుగు(fs)) {
		retval = ext2fs_map_cluster_ఖండం(fs, ino, inode,
						  lfree_ప్రారంభం, &pblk);
		ఒకవేళ (retval)
			వెళ్ళు errout;
		ఒకవేళ (!pblk) {
			ext2fs_ఖండం_alloc_stats2(fs, free_ప్రారంభం, -1);
			freed_now++;
		}
		cluster_freed = EXT2_FS_CLUSTER_నిష్పత్తి(fs) -
			(free_ప్రారంభం & EXT2_FS_CLUSTER_ముసుగు(fs));
		ఒకవేళ (cluster_freed > free_లెక్క)
			cluster_freed = free_లెక్క;
		free_లెక్క -= cluster_freed;
		free_ప్రారంభం += cluster_freed;
		lfree_ప్రారంభం += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	అయితే (free_లెక్క > 0 && free_లెక్క >= (గుర్తులేని) EXT2_FS_CLUSTER_నిష్పత్తి(fs)) {
		ext2fs_ఖండం_alloc_stats2(fs, free_ప్రారంభం, -1);
		freed_now++;
		cluster_freed = EXT2_FS_CLUSTER_నిష్పత్తి(fs);
		free_లెక్క -= cluster_freed;
		free_ప్రారంభం += cluster_freed;
		lfree_ప్రారంభం += cluster_freed;
	}

	/* Try to free the last cluster. */
	ఒకవేళ (free_లెక్క > 0) {
		retval = ext2fs_map_cluster_ఖండం(fs, ino, inode,
						  lfree_ప్రారంభం, &pblk);
		ఒకవేళ (retval)
			వెళ్ళు errout;
		ఒకవేళ (!pblk) {
			ext2fs_ఖండం_alloc_stats2(fs, free_ప్రారంభం, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	తిరిగి retval;
}

స్థిర errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     నిర్మాణం ext2_inode *inode,
				     blk64_t ప్రారంభం, blk64_t చివర)
{
	ext2_extent_handle_t	handle = 0;
	నిర్మాణం ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_ప్రారంభం, తదుపరి, lfree_ప్రారంభం;
	__u32			free_లెక్క, newlen;
	blk64_t			freed = 0;
	పూర్ణాంకం			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ఒకవేళ (retval)
		తిరిగి retval;
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
	ext2fs_extent_goto(handle, ప్రారంభం);
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ఒకవేళ (retval == EXT2_ET_NO_CURRENT_కణుపు) {
		retval = 0;
		వెళ్ళు errout;
	} లేకపోతే ఒకవేళ (retval)
		వెళ్ళు errout;
	అయితే (1) {
		op = EXT2_EXTENT_తదుపరి_ఆకు;
		dbg_print_extent("main loop", &extent);
		తదుపరి = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (గుర్తులేని దీర్ఘ దీర్ఘ) ప్రారంభం,
			   (గుర్తులేని దీర్ఘ దీర్ఘ) చివర,
			   (గుర్తులేని దీర్ఘ దీర్ఘ) తదుపరి);
		ఒకవేళ (ప్రారంభం <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			ఒకవేళ (చివర < extent.e_lblk)
				ఆపు;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_ప్రారంభం = extent.e_pblk;
			lfree_ప్రారంభం = extent.e_lblk;
			ఒకవేళ (తదుపరి > చివర)
				free_లెక్క = చివర - extent.e_lblk + 1;
			లేకపోతే
				free_లెక్క = extent.e_len;
			extent.e_len -= free_లెక్క;
			extent.e_lblk += free_లెక్క;
			extent.e_pblk += free_లెక్క;
		} లేకపోతే ఒకవేళ (చివర >= తదుపరి-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			ఒకవేళ (ప్రారంభం >= తదుపరి)
				వెళ్ళు తదుపరి_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = ప్రారంభం - extent.e_lblk;
			free_ప్రారంభం = extent.e_pblk + newlen;
			lfree_ప్రారంభం = extent.e_lblk + newlen;
			free_లెక్క = extent.e_len - newlen;
			extent.e_len = newlen;
		} లేకపోతే {
			నిర్మాణం ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(చివర + 1 - extent.e_lblk);
			newex.e_lblk = చివర + 1;
			newex.e_len = తదుపరి - చివర - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = ప్రారంభం - extent.e_lblk;
			free_ప్రారంభం = extent.e_pblk + extent.e_len;
			lfree_ప్రారంభం = extent.e_lblk + extent.e_len;
			free_లెక్క = చివర - ప్రారంభం + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_చొప్పించు(handle,
					EXT2_EXTENT_చొప్పించు_AFTER, &newex);
			ఒకవేళ (retval)
				వెళ్ళు errout;
			retval = ext2fs_extent_fix_parents(handle);
			ఒకవేళ (retval)
				వెళ్ళు errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			ఒకవేళ (retval)
				వెళ్ళు errout;
		} 
		ఒకవేళ (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			ఒకవేళ (retval)
				వెళ్ళు errout;
			retval = ext2fs_extent_fix_parents(handle);
		} లేకపోతే {
			నిర్మాణం ext2fs_extent	newex;
			blk64_t			old_lblk, తదుపరి_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			ఒకవేళ (retval)
				వెళ్ళు errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_తదుపరి_ఆకు,
						   &newex);
			ఒకవేళ (retval == EXT2_ET_EXTENT_NO_తదుపరి)
				తదుపరి_lblk = old_lblk;
			లేకపోతే ఒకవేళ (retval)
				వెళ్ళు errout;
			లేకపోతే
				తదుపరి_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			ఒకవేళ (retval)
				వెళ్ళు errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			ఒకవేళ (retval)
				వెళ్ళు errout;

			retval = ext2fs_extent_fix_parents(handle);
			ఒకవేళ (retval && retval != EXT2_ET_NO_CURRENT_కణుపు)
				వెళ్ళు errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(శూన్య)ext2fs_extent_goto(handle, తదుపరి_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		ఒకవేళ (retval)
			వెళ్ళు errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_ప్రారంభం, free_లెక్క);
		retval = punch_extent_ఖండాలు(fs, ino, inode, lfree_ప్రారంభం,
					     free_ప్రారంభం, free_లెక్క, &freed);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	తదుపరి_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		ఒకవేళ (retval == EXT2_ET_EXTENT_NO_తదుపరి ||
		    retval == EXT2_ET_NO_CURRENT_కణుపు)
			ఆపు;
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_ఖండాలు(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	తిరిగి retval;
}
	
స్థిర errcode_t ext2fs_punch_inline_దత్తాంశం(ext2_filsys fs, ext2_ino_t ino,
					  నిర్మాణం ext2_inode *inode,
					  blk64_t ప్రారంభం,
					  blk64_t చివర EXT2FS_ATTR((వాడని)))
{
	errcode_t retval;

	/*
	 * In libext2fs ext2fs_punch is based on block unit.  So that
	 * means that if start > 0 we don't need to do nothing.  Due
	 * to this we will remove all inline data in ext2fs_punch()
	 * now.
	 */
	ఒకవేళ (ప్రారంభం > 0)
		తిరిగి 0;

	memset((అక్షరం *)inode->i_ఖండం, 0, EXT4_MIN_INLINE_దత్తాంశం_కొలత);
	inode->i_కొలత = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	తిరిగి ext2fs_inline_దత్తాంశం_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       నిర్మాణం ext2_inode *inode,
		       అక్షరం *ఖండం_buf, blk64_t ప్రారంభం,
		       blk64_t చివర)
{
	errcode_t		retval;
	నిర్మాణం ext2_inode	inode_buf;

	ఒకవేళ (ప్రారంభం > చివర)
		తిరిగి EINVAL;

	/* Read inode structure if necessary */
	ఒకవేళ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ఒకవేళ (retval)
			తిరిగి retval;
		inode = &inode_buf;
	}
	ఒకవేళ (inode->i_flags & EXT4_INLINE_దత్తాంశం_FL)
		తిరిగి ext2fs_punch_inline_దత్తాంశం(fs, ino, inode, ప్రారంభం, చివర);
	లేకపోతే ఒకవేళ (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, ప్రారంభం, చివర);
	లేకపోతే
		retval = ext2fs_punch_ind(fs, inode, ఖండం_buf, ప్రారంభం, చివర);
	ఒకవేళ (retval)
		తిరిగి retval;

#నిర్వచితమైతే PUNCH_DEBUG
	రాయి("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_కొలత(inode), inode->i_ఖండాలు);
#అయితే_ముగింపు
	తిరిగి ext2fs_write_inode(fs, ino, inode);
}
