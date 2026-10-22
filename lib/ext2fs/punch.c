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
#پایان_اگر
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#لغو_تعریف PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ساكن صحيح check_zero_بلوک(حرف *buf, صحيح blocksize)
{
	حرف	*cp = buf;
	صحيح	چپ = blocksize;

	طالما (چپ > 0) {
		إذا (*cp++)
			أرجع 0;
		چپ--;
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
			   حرف *بلوک_buf, blk_t *p, صحيح level,
			   blk64_t شروع, blk64_t شمار, صحيح max)
{
	errcode_t	retval;
	blk_t		b;
	صحيح		i;
	blk64_t		جابه_جایی, incr;
	ثابت blk64_t	انتها = شروع + شمار;
	صحيح		freed = 0;

#اگر_تعریف_شده PUNCH_DEBUG
	چاپ_کن("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, شروع, شمار, max);
#پایان_اگر
	incr = 1ULL << ((EXT2_بلوک_اندازه_بیتها(fs->super) - 2) * level);
	لكل (i = 0, جابه_جایی = 0; i < max; i++, p++, جابه_جایی += incr) {
		إذا (جابه_جایی >= انتها)
			اكسر;
		إذا (*p == 0 || (جابه_جایی+incr) <= شروع)
			استمر;
		b = *p;
		إذا (level > 0) {
			blk_t start2;
			blk_t count2;
#اگر_تعریف_شده PUNCH_DEBUG
			چاپ_کن("Reading indirect block %u\n", b);
#پایان_اگر
			retval = ext2fs_read_ind_بلوک(fs, b, بلوک_buf);
			إذا (retval)
				أرجع retval;
			start2 = (شروع > جابه_جایی) ? شروع - جابه_جایی : 0;
			count2 = انتها - ((شروع > جابه_جایی) ? شروع : جابه_جایی);
#اگر_تعریف_شده PUNCH_DEBUG
			چاپ_کن("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", شروع,
			       جابه_جایی, شمار, انتها, incr, start2, count2);
#پایان_اگر
			retval = ind_punch(fs, inode, بلوک_buf + fs->blocksize,
					   (blk_t *) بلوک_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			إذا (retval)
				أرجع retval;
			retval = ext2fs_write_ind_بلوک(fs, b, بلوک_buf);
			إذا (retval)
				أرجع retval;
			إذا (!check_zero_بلوک(بلوک_buf, fs->blocksize))
				استمر;
		}
#اگر_تعریف_شده PUNCH_DEBUG
		چاپ_کن("Freeing block %u (offset %llu)\n", b, جابه_جایی);
#پایان_اگر
		ext2fs_بلوک_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#اگر_تعریف_شده PUNCH_DEBUG
	چاپ_کن("Freed %d blocks\n", freed);
#پایان_اگر
	أرجع ext2fs_iblk_sub_بلوکها(fs, inode, freed);
}

#تعریف_کن BLK_T_MAX ((blk_t)~0ULL)
ساكن errcode_t ext2fs_punch_ind(ext2_filsys fs, بنية ext2_inode *inode,
				  حرف *بلوک_buf, blk64_t شروع, blk64_t انتها)
{
	errcode_t		retval;
	حرف			*buf = 0;
	صحيح			level;
	صحيح			رقم = EXT2_NDIR_بلوکها;
	blk_t			*bp = inode->i_بلوک;
	blk_t			addr_در_هر_بلوک;
	blk64_t			max = EXT2_NDIR_بلوکها;
	blk_t			شمار;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	إذا (شروع > BLK_T_MAX)
		أرجع 0;
	إذا (انتها >= BLK_T_MAX || انتها - شروع + 1 >= BLK_T_MAX)
		شمار = BLK_T_MAX - شروع;
	وإلا
		شمار = انتها - شروع + 1;

	إذا (!بلوک_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		بلوک_buf = buf;
	}

	addr_در_هر_بلوک = (blk_t)fs->blocksize >> 2;

	لكل (level = 0; level < 4; level++, max *= (blk64_t)addr_در_هر_بلوک) {
#اگر_تعریف_شده PUNCH_DEBUG
		چاپ_کن("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, شروع, شمار, max, رقم);
#پایان_اگر
		إذا (شروع < max) {
			retval = ind_punch(fs, inode, بلوک_buf, bp, level,
					   شروع, شمار, رقم);
			إذا (retval)
				اذهب errout;
			إذا (شمار > max)
				شمار -= max - شروع;
			وإلا
				اكسر;
			شروع = 0;
		} وإلا
			شروع -= max;
		bp += رقم;
		إذا (level == 0) {
			رقم = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	إذا (buf)
		ext2fs_free_mem(&buf);
	أرجع retval;
}
#لغو_تعریف BLK_T_MAX

#اگر_تعریف_شده PUNCH_DEBUG

#تعریف_کن dbg_printf(f, a...)  چاپ_کن(f, ## a)

ساكن فراغ dbg_print_extent(حرف *desc, بنية ext2fs_extent *extent)
{
	إذا (desc)
		چاپ_کن("%s: ", desc);
	چاپ_کن("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_برگ)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_دومی_بازدید)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#وإلا
#تعریف_کن dbg_print_extent(desc, ex)	افعل { } طالما (0)
#تعریف_کن dbg_printf(f, a...)		افعل { } طالما (0)
#پایان_اگر

/* Free a range of blocks, respecting cluster boundaries */
ساكن errcode_t punch_extent_بلوکها(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode,
				     blk64_t lfree_شروع, blk64_t free_شروع,
				     __u32 free_شمار, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	إذا (free_شروع < fs->super->s_اولی_داده_بلوک ||
	    (free_شروع + free_شمار) > ext2fs_بلوکها_شمار(fs->super))
		أرجع EXT2_ET_BAD_بلوک_رقم;

	/* No bigalloc?  Just free each block. */
	إذا (EXT2_FS_CLUSTER_نسبت(fs) == 1) {
		*freed += free_شمار;
		طالما (free_شمار-- > 0)
			ext2fs_بلوک_alloc_stats2(fs, free_شروع++, -1);
		أرجع retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	إذا (free_شروع & EXT2_FS_CLUSTER_ماسک(fs)) {
		retval = ext2fs_map_cluster_بلوک(fs, ino, inode,
						  lfree_شروع, &pblk);
		إذا (retval)
			اذهب errout;
		إذا (!pblk) {
			ext2fs_بلوک_alloc_stats2(fs, free_شروع, -1);
			freed_now++;
		}
		cluster_freed = EXT2_FS_CLUSTER_نسبت(fs) -
			(free_شروع & EXT2_FS_CLUSTER_ماسک(fs));
		إذا (cluster_freed > free_شمار)
			cluster_freed = free_شمار;
		free_شمار -= cluster_freed;
		free_شروع += cluster_freed;
		lfree_شروع += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	طالما (free_شمار > 0 && free_شمار >= (بیعلامت) EXT2_FS_CLUSTER_نسبت(fs)) {
		ext2fs_بلوک_alloc_stats2(fs, free_شروع, -1);
		freed_now++;
		cluster_freed = EXT2_FS_CLUSTER_نسبت(fs);
		free_شمار -= cluster_freed;
		free_شروع += cluster_freed;
		lfree_شروع += cluster_freed;
	}

	/* Try to free the last cluster. */
	إذا (free_شمار > 0) {
		retval = ext2fs_map_cluster_بلوک(fs, ino, inode,
						  lfree_شروع, &pblk);
		إذا (retval)
			اذهب errout;
		إذا (!pblk) {
			ext2fs_بلوک_alloc_stats2(fs, free_شروع, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	أرجع retval;
}

ساكن errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode,
				     blk64_t شروع, blk64_t انتها)
{
	ext2_extent_handle_t	handle = 0;
	بنية ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_شروع, بعدی, lfree_شروع;
	__u32			free_شمار, newlen;
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
	ext2fs_extent_goto(handle, شروع);
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval == EXT2_ET_NO_CURRENT_گره) {
		retval = 0;
		اذهب errout;
	} وإلا إذا (retval)
		اذهب errout;
	طالما (1) {
		op = EXT2_EXTENT_بعدی_برگ;
		dbg_print_extent("main loop", &extent);
		بعدی = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (بیعلامت دراز دراز) شروع,
			   (بیعلامت دراز دراز) انتها,
			   (بیعلامت دراز دراز) بعدی);
		إذا (شروع <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			إذا (انتها < extent.e_lblk)
				اكسر;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_شروع = extent.e_pblk;
			lfree_شروع = extent.e_lblk;
			إذا (بعدی > انتها)
				free_شمار = انتها - extent.e_lblk + 1;
			وإلا
				free_شمار = extent.e_len;
			extent.e_len -= free_شمار;
			extent.e_lblk += free_شمار;
			extent.e_pblk += free_شمار;
		} وإلا إذا (انتها >= بعدی-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			إذا (شروع >= بعدی)
				اذهب بعدی_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = شروع - extent.e_lblk;
			free_شروع = extent.e_pblk + newlen;
			lfree_شروع = extent.e_lblk + newlen;
			free_شمار = extent.e_len - newlen;
			extent.e_len = newlen;
		} وإلا {
			بنية ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(انتها + 1 - extent.e_lblk);
			newex.e_lblk = انتها + 1;
			newex.e_len = بعدی - انتها - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = شروع - extent.e_lblk;
			free_شروع = extent.e_pblk + extent.e_len;
			lfree_شروع = extent.e_lblk + extent.e_len;
			free_شمار = انتها - شروع + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_درج(handle,
					EXT2_EXTENT_درج_AFTER, &newex);
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
			blk64_t			old_lblk, بعدی_lblk;
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
						   EXT2_EXTENT_بعدی_برگ,
						   &newex);
			إذا (retval == EXT2_ET_EXTENT_NO_بعدی)
				بعدی_lblk = old_lblk;
			وإلا إذا (retval)
				اذهب errout;
			وإلا
				بعدی_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			إذا (retval)
				اذهب errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			إذا (retval)
				اذهب errout;

			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval && retval != EXT2_ET_NO_CURRENT_گره)
				اذهب errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(فراغ)ext2fs_extent_goto(handle, بعدی_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		إذا (retval)
			اذهب errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_شروع, free_شمار);
		retval = punch_extent_بلوکها(fs, ino, inode, lfree_شروع,
					     free_شروع, free_شمار, &freed);
		إذا (retval)
			اذهب errout;
	بعدی_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		إذا (retval == EXT2_ET_EXTENT_NO_بعدی ||
		    retval == EXT2_ET_NO_CURRENT_گره)
			اكسر;
		إذا (retval)
			اذهب errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_بلوکها(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	أرجع retval;
}
	
ساكن errcode_t ext2fs_punch_inline_داده(ext2_filsys fs, ext2_ino_t ino,
					  بنية ext2_inode *inode,
					  blk64_t شروع,
					  blk64_t انتها EXT2FS_ATTR((استفادهنشده)))
{
	errcode_t retval;

	/*
	 * In libext2fs ext2fs_punch is based on block unit.  So that
	 * means that if start > 0 we don't need to do nothing.  Due
	 * to this we will remove all inline data in ext2fs_punch()
	 * now.
	 */
	إذا (شروع > 0)
		أرجع 0;

	memset((حرف *)inode->i_بلوک, 0, EXT4_MIN_INLINE_داده_اندازه);
	inode->i_اندازه = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	إذا (retval)
		أرجع retval;

	أرجع ext2fs_inline_داده_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       بنية ext2_inode *inode,
		       حرف *بلوک_buf, blk64_t شروع,
		       blk64_t انتها)
{
	errcode_t		retval;
	بنية ext2_inode	inode_buf;

	إذا (شروع > انتها)
		أرجع EINVAL;

	/* Read inode structure if necessary */
	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}
	إذا (inode->i_flags & EXT4_INLINE_داده_FL)
		أرجع ext2fs_punch_inline_داده(fs, ino, inode, شروع, انتها);
	وإلا إذا (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, شروع, انتها);
	وإلا
		retval = ext2fs_punch_ind(fs, inode, بلوک_buf, شروع, انتها);
	إذا (retval)
		أرجع retval;

#اگر_تعریف_شده PUNCH_DEBUG
	چاپ_کن("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_اندازه(inode), inode->i_بلوکها);
#پایان_اگر
	أرجع ext2fs_write_inode(fs, ino, inode);
}
