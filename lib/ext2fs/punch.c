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
#جيڪڏهن_پورو
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#وصف_رد PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ساكن صحيح check_zero_بلاڪ(حرف *buf, صحيح blocksize)
{
	حرف	*cp = buf;
	صحيح	کاٻو = blocksize;

	طالما (کاٻو > 0) {
		إذا (*cp++)
			أرجع 0;
		کاٻو--;
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
			   حرف *بلاڪ_buf, blk_t *p, صحيح level,
			   blk64_t شروعات, blk64_t ڳڻپ, صحيح max)
{
	errcode_t	retval;
	blk_t		b;
	صحيح		i;
	blk64_t		وٿي, incr;
	ثابت blk64_t	پڇاڙي = شروعات + ڳڻپ;
	صحيح		freed = 0;

#جيڪڏهن_وصف_ٿيل PUNCH_DEBUG
	لکو("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, شروعات, ڳڻپ, max);
#جيڪڏهن_پورو
	incr = 1ULL << ((EXT2_بلاڪ_سائيز_بٽ(fs->super) - 2) * level);
	لكل (i = 0, وٿي = 0; i < max; i++, p++, وٿي += incr) {
		إذا (وٿي >= پڇاڙي)
			اكسر;
		إذا (*p == 0 || (وٿي+incr) <= شروعات)
			استمر;
		b = *p;
		إذا (level > 0) {
			blk_t start2;
			blk_t count2;
#جيڪڏهن_وصف_ٿيل PUNCH_DEBUG
			لکو("Reading indirect block %u\n", b);
#جيڪڏهن_پورو
			retval = ext2fs_read_ind_بلاڪ(fs, b, بلاڪ_buf);
			إذا (retval)
				أرجع retval;
			start2 = (شروعات > وٿي) ? شروعات - وٿي : 0;
			count2 = پڇاڙي - ((شروعات > وٿي) ? شروعات : وٿي);
#جيڪڏهن_وصف_ٿيل PUNCH_DEBUG
			لکو("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", شروعات,
			       وٿي, ڳڻپ, پڇاڙي, incr, start2, count2);
#جيڪڏهن_پورو
			retval = ind_punch(fs, inode, بلاڪ_buf + fs->blocksize,
					   (blk_t *) بلاڪ_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			إذا (retval)
				أرجع retval;
			retval = ext2fs_write_ind_بلاڪ(fs, b, بلاڪ_buf);
			إذا (retval)
				أرجع retval;
			إذا (!check_zero_بلاڪ(بلاڪ_buf, fs->blocksize))
				استمر;
		}
#جيڪڏهن_وصف_ٿيل PUNCH_DEBUG
		لکو("Freeing block %u (offset %llu)\n", b, وٿي);
#جيڪڏهن_پورو
		ext2fs_بلاڪ_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#جيڪڏهن_وصف_ٿيل PUNCH_DEBUG
	لکو("Freed %d blocks\n", freed);
#جيڪڏهن_پورو
	أرجع ext2fs_iblk_sub_بلاڪون(fs, inode, freed);
}

#وصف_ڪريو BLK_T_MAX ((blk_t)~0ULL)
ساكن errcode_t ext2fs_punch_ind(ext2_filsys fs, بنية ext2_inode *inode,
				  حرف *بلاڪ_buf, blk64_t شروعات, blk64_t پڇاڙي)
{
	errcode_t		retval;
	حرف			*buf = 0;
	صحيح			level;
	صحيح			انگ = EXT2_NDIR_بلاڪون;
	blk_t			*bp = inode->i_بلاڪ;
	blk_t			addr_في_بلاڪ;
	blk64_t			max = EXT2_NDIR_بلاڪون;
	blk_t			ڳڻپ;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	إذا (شروعات > BLK_T_MAX)
		أرجع 0;
	إذا (پڇاڙي >= BLK_T_MAX || پڇاڙي - شروعات + 1 >= BLK_T_MAX)
		ڳڻپ = BLK_T_MAX - شروعات;
	وإلا
		ڳڻپ = پڇاڙي - شروعات + 1;

	إذا (!بلاڪ_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		بلاڪ_buf = buf;
	}

	addr_في_بلاڪ = (blk_t)fs->blocksize >> 2;

	لكل (level = 0; level < 4; level++, max *= (blk64_t)addr_في_بلاڪ) {
#جيڪڏهن_وصف_ٿيل PUNCH_DEBUG
		لکو("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, شروعات, ڳڻپ, max, انگ);
#جيڪڏهن_پورو
		إذا (شروعات < max) {
			retval = ind_punch(fs, inode, بلاڪ_buf, bp, level,
					   شروعات, ڳڻپ, انگ);
			إذا (retval)
				اذهب errout;
			إذا (ڳڻپ > max)
				ڳڻپ -= max - شروعات;
			وإلا
				اكسر;
			شروعات = 0;
		} وإلا
			شروعات -= max;
		bp += انگ;
		إذا (level == 0) {
			انگ = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	إذا (buf)
		ext2fs_free_mem(&buf);
	أرجع retval;
}
#وصف_رد BLK_T_MAX

#جيڪڏهن_وصف_ٿيل PUNCH_DEBUG

#وصف_ڪريو dbg_printf(f, a...)  لکو(f, ## a)

ساكن فراغ dbg_print_extent(حرف *desc, بنية ext2fs_extent *extent)
{
	إذا (desc)
		لکو("%s: ", desc);
	لکو("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_پن)
		fputs("LEAF ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	إذا (extent->e_flags & EXT2_EXTENT_FLAGS_ٻيو_گهمو)
		fputs("2ND_VISIT ", stdout);
	إذا (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#وإلا
#وصف_ڪريو dbg_print_extent(desc, ex)	افعل { } طالما (0)
#وصف_ڪريو dbg_printf(f, a...)		افعل { } طالما (0)
#جيڪڏهن_پورو

/* Free a range of blocks, respecting cluster boundaries */
ساكن errcode_t punch_extent_بلاڪون(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode,
				     blk64_t lfree_شروعات, blk64_t free_شروعات,
				     __u32 free_ڳڻپ, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	إذا (free_شروعات < fs->super->s_پهريون_معلومات_بلاڪ ||
	    (free_شروعات + free_ڳڻپ) > ext2fs_بلاڪون_ڳڻپ(fs->super))
		أرجع EXT2_ET_BAD_بلاڪ_انگ;

	/* No bigalloc?  Just free each block. */
	إذا (EXT2_FS_CLUSTER_تناسب(fs) == 1) {
		*freed += free_ڳڻپ;
		طالما (free_ڳڻپ-- > 0)
			ext2fs_بلاڪ_alloc_stats2(fs, free_شروعات++, -1);
		أرجع retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	إذا (free_شروعات & EXT2_FS_CLUSTER_نقاب(fs)) {
		retval = ext2fs_map_cluster_بلاڪ(fs, ino, inode,
						  lfree_شروعات, &pblk);
		إذا (retval)
			اذهب errout;
		إذا (!pblk) {
			ext2fs_بلاڪ_alloc_stats2(fs, free_شروعات, -1);
			freed_now++;
		}
		cluster_freed = EXT2_FS_CLUSTER_تناسب(fs) -
			(free_شروعات & EXT2_FS_CLUSTER_نقاب(fs));
		إذا (cluster_freed > free_ڳڻپ)
			cluster_freed = free_ڳڻپ;
		free_ڳڻپ -= cluster_freed;
		free_شروعات += cluster_freed;
		lfree_شروعات += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	طالما (free_ڳڻپ > 0 && free_ڳڻپ >= (بنانشان) EXT2_FS_CLUSTER_تناسب(fs)) {
		ext2fs_بلاڪ_alloc_stats2(fs, free_شروعات, -1);
		freed_now++;
		cluster_freed = EXT2_FS_CLUSTER_تناسب(fs);
		free_ڳڻپ -= cluster_freed;
		free_شروعات += cluster_freed;
		lfree_شروعات += cluster_freed;
	}

	/* Try to free the last cluster. */
	إذا (free_ڳڻپ > 0) {
		retval = ext2fs_map_cluster_بلاڪ(fs, ino, inode,
						  lfree_شروعات, &pblk);
		إذا (retval)
			اذهب errout;
		إذا (!pblk) {
			ext2fs_بلاڪ_alloc_stats2(fs, free_شروعات, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	أرجع retval;
}

ساكن errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     بنية ext2_inode *inode,
				     blk64_t شروعات, blk64_t پڇاڙي)
{
	ext2_extent_handle_t	handle = 0;
	بنية ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_شروعات, ايندڙ, lfree_شروعات;
	__u32			free_ڳڻپ, newlen;
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
	ext2fs_extent_goto(handle, شروعات);
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval == EXT2_ET_NO_CURRENT_ڳنڍ) {
		retval = 0;
		اذهب errout;
	} وإلا إذا (retval)
		اذهب errout;
	طالما (1) {
		op = EXT2_EXTENT_ايندڙ_پن;
		dbg_print_extent("main loop", &extent);
		ايندڙ = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (بنانشان ڊگهو ڊگهو) شروعات,
			   (بنانشان ڊگهو ڊگهو) پڇاڙي,
			   (بنانشان ڊگهو ڊگهو) ايندڙ);
		إذا (شروعات <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			إذا (پڇاڙي < extent.e_lblk)
				اكسر;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_شروعات = extent.e_pblk;
			lfree_شروعات = extent.e_lblk;
			إذا (ايندڙ > پڇاڙي)
				free_ڳڻپ = پڇاڙي - extent.e_lblk + 1;
			وإلا
				free_ڳڻپ = extent.e_len;
			extent.e_len -= free_ڳڻپ;
			extent.e_lblk += free_ڳڻپ;
			extent.e_pblk += free_ڳڻپ;
		} وإلا إذا (پڇاڙي >= ايندڙ-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			إذا (شروعات >= ايندڙ)
				اذهب ايندڙ_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = شروعات - extent.e_lblk;
			free_شروعات = extent.e_pblk + newlen;
			lfree_شروعات = extent.e_lblk + newlen;
			free_ڳڻپ = extent.e_len - newlen;
			extent.e_len = newlen;
		} وإلا {
			بنية ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(پڇاڙي + 1 - extent.e_lblk);
			newex.e_lblk = پڇاڙي + 1;
			newex.e_len = ايندڙ - پڇاڙي - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = شروعات - extent.e_lblk;
			free_شروعات = extent.e_pblk + extent.e_len;
			lfree_شروعات = extent.e_lblk + extent.e_len;
			free_ڳڻپ = پڇاڙي - شروعات + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_وجهو(handle,
					EXT2_EXTENT_وجهو_AFTER, &newex);
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
			blk64_t			old_lblk, ايندڙ_lblk;
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
						   EXT2_EXTENT_ايندڙ_پن,
						   &newex);
			إذا (retval == EXT2_ET_EXTENT_NO_ايندڙ)
				ايندڙ_lblk = old_lblk;
			وإلا إذا (retval)
				اذهب errout;
			وإلا
				ايندڙ_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			إذا (retval)
				اذهب errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			إذا (retval)
				اذهب errout;

			retval = ext2fs_extent_fix_parents(handle);
			إذا (retval && retval != EXT2_ET_NO_CURRENT_ڳنڍ)
				اذهب errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(فراغ)ext2fs_extent_goto(handle, ايندڙ_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		إذا (retval)
			اذهب errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_شروعات, free_ڳڻپ);
		retval = punch_extent_بلاڪون(fs, ino, inode, lfree_شروعات,
					     free_شروعات, free_ڳڻپ, &freed);
		إذا (retval)
			اذهب errout;
	ايندڙ_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		إذا (retval == EXT2_ET_EXTENT_NO_ايندڙ ||
		    retval == EXT2_ET_NO_CURRENT_ڳنڍ)
			اكسر;
		إذا (retval)
			اذهب errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_بلاڪون(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	أرجع retval;
}
	
ساكن errcode_t ext2fs_punch_inline_معلومات(ext2_filsys fs, ext2_ino_t ino,
					  بنية ext2_inode *inode,
					  blk64_t شروعات,
					  blk64_t پڇاڙي EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	errcode_t retval;

	/*
	 * In libext2fs ext2fs_punch is based on block unit.  So that
	 * means that if start > 0 we don't need to do nothing.  Due
	 * to this we will remove all inline data in ext2fs_punch()
	 * now.
	 */
	إذا (شروعات > 0)
		أرجع 0;

	memset((حرف *)inode->i_بلاڪ, 0, EXT4_MIN_INLINE_معلومات_سائيز);
	inode->i_سائيز = 0;
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
		       حرف *بلاڪ_buf, blk64_t شروعات,
		       blk64_t پڇاڙي)
{
	errcode_t		retval;
	بنية ext2_inode	inode_buf;

	إذا (شروعات > پڇاڙي)
		أرجع EINVAL;

	/* Read inode structure if necessary */
	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}
	إذا (inode->i_flags & EXT4_INLINE_معلومات_FL)
		أرجع ext2fs_punch_inline_معلومات(fs, ino, inode, شروعات, پڇاڙي);
	وإلا إذا (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, شروعات, پڇاڙي);
	وإلا
		retval = ext2fs_punch_ind(fs, inode, بلاڪ_buf, شروعات, پڇاڙي);
	إذا (retval)
		أرجع retval;

#جيڪڏهن_وصف_ٿيل PUNCH_DEBUG
	لکو("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_سائيز(inode), inode->i_بلاڪون);
#جيڪڏهن_پورو
	أرجع ext2fs_write_inode(fs, ino, inode);
}
