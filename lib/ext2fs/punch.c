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
#د_که_پای
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#تعریف_لغوه PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ساكن صحيح check_zero_بلاک(حرف *buf, صحيح blocksize)
{
	حرف	*cp = buf;
	صحيح	کیڼ = blocksize;

	طالما (کیڼ > 0) {
		إذا (*cp++)
			أرجع 0;
		کیڼ--;
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
			   حرف *بلاک_buf, blk_t *p, صحيح level,
			   blk64_t پیل, blk64_t شمېر, صحيح max)
{
	errcode_t	retval;
	blk_t		b;
	صحيح		i;
	blk64_t		دننه_واټن, incr;
	ثابت blk64_t	پای_ټکی = پیل + شمېر;
	صحيح		freed = 0;

#که_تعریف_شوی PUNCH_DEBUG
	چاپ_کړه("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, پیل, شمېر, max);
#د_که_پای
	incr = 1ULL << ((EXT2_بلاک_اندازه_بټونه(fs->super) - 2) * level);
	لكل (i = 0, دننه_واټن = 0; i < max; i++, p++, دننه_واټن += incr) {
		إذا (دننه_واټن >= پای_ټکی)
			اكسر;
		إذا (*p == 0 || (دننه_واټن+incr) <= پیل)
			استمر;
		b = *p;
		إذا (level > 0) {
			blk_t start2;
			blk_t count2;
#که_تعریف_شوی PUNCH_DEBUG
			چاپ_کړه("Reading indirect block %u\n", b);
#د_که_پای
			retval = ext2fs_read_ind_بلاک(fs, b, بلاک_buf);
			إذا (retval)
				أرجع retval;
			start2 = (پیل > دننه_واټن) ? پیل - دننه_واټن : 0;
			count2 = پای_ټکی - ((پیل > دننه_واټن) ? پیل : دننه_واټن);
#که_تعریف_شوی PUNCH_DEBUG
			چاپ_کړه("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", پیل,
			       دننه_واټن, شمېر, پای_ټکی, incr, start2, count2);
#د_که_پای
			retval = ind_punch(fs, inode, بلاک_buf + fs->blocksize,
					   (blk_t *) بلاک_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			إذا (retval)
				أرجع retval;
			retval = ext2fs_write_ind_بلاک(fs, b, بلاک_buf);
			إذا (retval)
				أرجع retval;
			إذا (!check_zero_بلاک(بلاک_buf, fs->blocksize))
				استمر;
		}
#که_تعریف_شوی PUNCH_DEBUG
		چاپ_کړه("Freeing block %u (offset %llu)\n", b, دننه_واټن);
#د_که_پای
		ext2fs_بلاک_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#که_تعریف_شوی PUNCH_DEBUG
	چاپ_کړه("Freed %d blocks\n", freed);
#د_که_پای
	أرجع ext2fs_iblk_sub_بلاکونه(fs, inode, freed);
}

#تعریف_کړه BLK_T_MAX ((blk_t)~0ULL)
ساكن errcode_t ext2fs_punch_ind(ext2_filsys fs, بنية ext2_inode *inode,
				  حرف *بلاک_buf, blk64_t پیل, blk64_t پای_ټکی)
{
	errcode_t		retval;
	حرف			*buf = 0;
	صحيح			level;
	صحيح			عدد = EXT2_NDIR_بلاکونه;
	blk_t			*bp = inode->i_بلاک;
	blk_t			addr_فی_بلاک;
	blk64_t			max = EXT2_NDIR_بلاکونه;
	blk_t			شمېر;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	إذا (پیل > BLK_T_MAX)
		أرجع 0;
	إذا (پای_ټکی >= BLK_T_MAX || پای_ټکی - پیل + 1 >= BLK_T_MAX)
		شمېر = BLK_T_MAX - پیل;
	وإلا
		شمېر = پای_ټکی - پیل + 1;

	إذا (!بلاک_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		بلاک_buf = buf;
	}

	addr_فی_بلاک = (blk_t)fs->blocksize >> 2;

	لكل (level = 0; level < 4; level++, max *= (blk64_t)addr_فی_بلاک) {
#که_تعریف_شوی PUNCH_DEBUG
		چاپ_کړه("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, پیل, شمېر, max, عدد);
#د_که_پای
		إذا (پیل < max) {
			retval = ind_punch(fs, inode, بلاک_buf, bp, level,
					   پیل, شمېر, عدد);
			إذا (retval)
				اذهب errout;
			إذا (شمېر > max)
				شمېر -= max - پیل;
			وإلا
				اكسر;
			پیل = 0;
		} وإلا
			پیل -= max;
		bp += عدد;
		إذا (level == 0) {
			عدد = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	إذا (buf)
		ext2fs_free_mem(&buf);
	أرجع retval;
}
#تعریف_لغوه BLK_T_MAX

#که_تعریف_شوی PUNCH_DEBUG

#تعریف_کړه dbg_printf(f, a...)  چاپ_کړه(f, ## a)

ساكن فراغ dbg_print_extent(حرف *desc, بنية ext2fs_extent *extent)
{
	إذا (desc)
		چاپ_کړه("%s: ", desc);
	چاپ_کړه("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_ونې_پاڼه)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_دوهم_لیدنه)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#وإلا
#تعریف_کړه dbg_print_extent(desc, ex)	افعل { } طالما (0)
#تعریف_کړه dbg_printf(f, a...)		افعل { } طالما (0)
#د_که_پای

/* Free a range of blocks, respecting cluster boundaries */
ساكن errcode_t punch_extent_بلاکونه(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode,
				     blk64_t lfree_پیل, blk64_t free_پیل,
				     __u32 free_شمېر, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	إذا (free_پیل < fs->super->s_لومړی_معلومات_بلاک ||
	    (free_پیل + free_شمېر) > ext2fs_بلاکونه_شمېر(fs->super))
		أرجع EXT2_ET_BAD_بلاک_عدد;

	/* No bigalloc?  Just free each block. */
	إذا (EXT2_FS_CLUSTER_نسبت(fs) == 1) {
		*freed += free_شمېر;
		طالما (free_شمېر-- > 0)
			ext2fs_بلاک_alloc_stats2(fs, free_پیل++, -1);
		أرجع retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	إذا (free_پیل & EXT2_FS_CLUSTER_ماسک(fs)) {
		retval = ext2fs_map_cluster_بلاک(fs, ino, inode,
						  lfree_پیل, &pblk);
		إذا (retval)
			اذهب errout;
		إذا (!pblk) {
			ext2fs_بلاک_alloc_stats2(fs, free_پیل, -1);
			freed_now++;
		}
		cluster_freed = EXT2_FS_CLUSTER_نسبت(fs) -
			(free_پیل & EXT2_FS_CLUSTER_ماسک(fs));
		إذا (cluster_freed > free_شمېر)
			cluster_freed = free_شمېر;
		free_شمېر -= cluster_freed;
		free_پیل += cluster_freed;
		lfree_پیل += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	طالما (free_شمېر > 0 && free_شمېر >= (بینښه) EXT2_FS_CLUSTER_نسبت(fs)) {
		ext2fs_بلاک_alloc_stats2(fs, free_پیل, -1);
		freed_now++;
		cluster_freed = EXT2_FS_CLUSTER_نسبت(fs);
		free_شمېر -= cluster_freed;
		free_پیل += cluster_freed;
		lfree_پیل += cluster_freed;
	}

	/* Try to free the last cluster. */
	إذا (free_شمېر > 0) {
		retval = ext2fs_map_cluster_بلاک(fs, ino, inode,
						  lfree_پیل, &pblk);
		إذا (retval)
			اذهب errout;
		إذا (!pblk) {
			ext2fs_بلاک_alloc_stats2(fs, free_پیل, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	أرجع retval;
}

ساكن errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode,
				     blk64_t پیل, blk64_t پای_ټکی)
{
	ext2_extent_handle_t	handle = 0;
	بنية ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_پیل, بل, lfree_پیل;
	__u32			free_شمېر, newlen;
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
	ext2fs_extent_goto(handle, پیل);
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval == EXT2_ET_NO_CURRENT_غوټه) {
		retval = 0;
		اذهب errout;
	} وإلا إذا (retval)
		اذهب errout;
	طالما (1) {
		op = EXT2_EXTENT_بل_ونې_پاڼه;
		dbg_print_extent("main loop", &extent);
		بل = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (بینښه اوږد اوږد) پیل,
			   (بینښه اوږد اوږد) پای_ټکی,
			   (بینښه اوږد اوږد) بل);
		إذا (پیل <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			إذا (پای_ټکی < extent.e_lblk)
				اكسر;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_پیل = extent.e_pblk;
			lfree_پیل = extent.e_lblk;
			إذا (بل > پای_ټکی)
				free_شمېر = پای_ټکی - extent.e_lblk + 1;
			وإلا
				free_شمېر = extent.e_len;
			extent.e_len -= free_شمېر;
			extent.e_lblk += free_شمېر;
			extent.e_pblk += free_شمېر;
		} وإلا إذا (پای_ټکی >= بل-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			إذا (پیل >= بل)
				اذهب بل_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = پیل - extent.e_lblk;
			free_پیل = extent.e_pblk + newlen;
			lfree_پیل = extent.e_lblk + newlen;
			free_شمېر = extent.e_len - newlen;
			extent.e_len = newlen;
		} وإلا {
			بنية ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(پای_ټکی + 1 - extent.e_lblk);
			newex.e_lblk = پای_ټکی + 1;
			newex.e_len = بل - پای_ټکی - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = پیل - extent.e_lblk;
			free_پیل = extent.e_pblk + extent.e_len;
			lfree_پیل = extent.e_lblk + extent.e_len;
			free_شمېر = پای_ټکی - پیل + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_ورننباسه(handle,
					EXT2_EXTENT_ورننباسه_AFTER, &newex);
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
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			إذا (retval)
				اذهب errout;
			retval = ext2fs_extent_fix_parents(handle);
		} وإلا {
			بنية ext2fs_extent	newex;
			blk64_t			old_lblk, بل_lblk;
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
						   EXT2_EXTENT_بل_ونې_پاڼه,
						   &newex);
			إذا (retval == EXT2_ET_EXTENT_NO_بل)
				بل_lblk = old_lblk;
			وإلا إذا (retval)
				اذهب errout;
			وإلا
				بل_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			إذا (retval)
				اذهب errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			إذا (retval)
				اذهب errout;

			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval && retval != EXT2_ET_NO_CURRENT_غوټه)
				اذهب errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(فراغ)ext2fs_extent_goto(handle, بل_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		إذا (retval)
			اذهب errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_پیل, free_شمېر);
		retval = punch_extent_بلاکونه(fs, ino, inode, lfree_پیل,
					     free_پیل, free_شمېر, &freed);
		إذا (retval)
			اذهب errout;
	بل_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		إذا (retval == EXT2_ET_EXTENT_NO_بل ||
		    retval == EXT2_ET_NO_CURRENT_غوټه)
			اكسر;
		إذا (retval)
			اذهب errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_بلاکونه(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	أرجع retval;
}
	
ساكن errcode_t ext2fs_punch_inline_معلومات(ext2_filsys fs, ext2_ino_t ino,
					  بنية ext2_inode *inode,
					  blk64_t پیل,
					  blk64_t پای_ټکی EXT2FS_ATTR((ناکارول_شوی)))
{
	errcode_t retval;

	/*
	 * In libext2fs ext2fs_punch is based on block unit.  So that
	 * means that if start > 0 we don't need to do nothing.  Due
	 * to this we will remove all inline data in ext2fs_punch()
	 * now.
	 */
	إذا (پیل > 0)
		أرجع 0;

	memset((حرف *)inode->i_بلاک, 0, EXT4_MIN_INLINE_معلومات_اندازه);
	inode->i_اندازه = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		أرجع retval;

	أرجع ext2fs_inline_معلومات_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       بنية ext2_inode *inode,
		       حرف *بلاک_buf, blk64_t پیل,
		       blk64_t پای_ټکی)
{
	errcode_t		retval;
	بنية ext2_inode	inode_buf;

	إذا (پیل > پای_ټکی)
		أرجع EINVAL;

	/* Read inode structure if necessary */
	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}
	إذا (inode->i_flags & EXT4_INLINE_معلومات_FL)
		أرجع ext2fs_punch_inline_معلومات(fs, ino, inode, پیل, پای_ټکی);
	وإلا إذا (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, پیل, پای_ټکی);
	وإلا
		retval = ext2fs_punch_ind(fs, inode, بلاک_buf, پیل, پای_ټکی);
	إذا (retval)
		أرجع retval;

#که_تعریف_شوی PUNCH_DEBUG
	چاپ_کړه("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_اندازه(inode), inode->i_بلاکونه);
#د_که_پای
	أرجع ext2fs_write_inode(fs, ino, inode);
}
