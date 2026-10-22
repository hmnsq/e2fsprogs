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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <errno.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#تعریف_منسوخ PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ساکن صحیح check_zero_بلاک(حرف *buf, صحیح blocksize)
{
	حرف	*cp = buf;
	صحیح	بایاں = blocksize;

	جب (بایاں > 0) {
		اگر (*cp++)
			واپس 0;
		بایاں--;
	}
	واپس 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
ساکن errcode_t ind_punch(ext2_filsys fs, ساخت ext2_inode *inode,
			   حرف *بلاک_buf, blk_t *p, صحیح level,
			   blk64_t آغاز, blk64_t گنتی, صحیح max)
{
	errcode_t	retval;
	blk_t		b;
	صحیح		i;
	blk64_t		انحراف, incr;
	ثابت blk64_t	انتہا = آغاز + گنتی;
	صحیح		freed = 0;

#اگر_تعریف_شدہ PUNCH_DEBUG
	لکھو("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, آغاز, گنتی, max);
#اگر_ختم
	incr = 1ULL << ((EXT2_بلاک_سائز_بٹ(fs->super) - 2) * level);
	برائے (i = 0, انحراف = 0; i < max; i++, p++, انحراف += incr) {
		اگر (انحراف >= انتہا)
			توڑو;
		اگر (*p == 0 || (انحراف+incr) <= آغاز)
			جاری;
		b = *p;
		اگر (level > 0) {
			blk_t start2;
			blk_t count2;
#اگر_تعریف_شدہ PUNCH_DEBUG
			لکھو("Reading indirect block %u\n", b);
#اگر_ختم
			retval = ext2fs_read_ind_بلاک(fs, b, بلاک_buf);
			اگر (retval)
				واپس retval;
			start2 = (آغاز > انحراف) ? آغاز - انحراف : 0;
			count2 = انتہا - ((آغاز > انحراف) ? آغاز : انحراف);
#اگر_تعریف_شدہ PUNCH_DEBUG
			لکھو("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", آغاز,
			       انحراف, گنتی, انتہا, incr, start2, count2);
#اگر_ختم
			retval = ind_punch(fs, inode, بلاک_buf + fs->blocksize,
					   (blk_t *) بلاک_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			اگر (retval)
				واپس retval;
			retval = ext2fs_write_ind_بلاک(fs, b, بلاک_buf);
			اگر (retval)
				واپس retval;
			اگر (!check_zero_بلاک(بلاک_buf, fs->blocksize))
				جاری;
		}
#اگر_تعریف_شدہ PUNCH_DEBUG
		لکھو("Freeing block %u (offset %llu)\n", b, انحراف);
#اگر_ختم
		ext2fs_بلاک_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#اگر_تعریف_شدہ PUNCH_DEBUG
	لکھو("Freed %d blocks\n", freed);
#اگر_ختم
	واپس ext2fs_iblk_sub_بلاکیں(fs, inode, freed);
}

#تعریف_کرو BLK_T_MAX ((blk_t)~0ULL)
ساکن errcode_t ext2fs_punch_ind(ext2_filsys fs, ساخت ext2_inode *inode,
				  حرف *بلاک_buf, blk64_t آغاز, blk64_t انتہا)
{
	errcode_t		retval;
	حرف			*buf = 0;
	صحیح			level;
	صحیح			عدد = EXT2_NDIR_بلاکیں;
	blk_t			*bp = inode->i_بلاک;
	blk_t			addr_فی_بلاک;
	blk64_t			max = EXT2_NDIR_بلاکیں;
	blk_t			گنتی;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	اگر (آغاز > BLK_T_MAX)
		واپس 0;
	اگر (انتہا >= BLK_T_MAX || انتہا - آغاز + 1 >= BLK_T_MAX)
		گنتی = BLK_T_MAX - آغاز;
	ورنہ
		گنتی = انتہا - آغاز + 1;

	اگر (!بلاک_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		اگر (retval)
			واپس retval;
		بلاک_buf = buf;
	}

	addr_فی_بلاک = (blk_t)fs->blocksize >> 2;

	برائے (level = 0; level < 4; level++, max *= (blk64_t)addr_فی_بلاک) {
#اگر_تعریف_شدہ PUNCH_DEBUG
		لکھو("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, آغاز, گنتی, max, عدد);
#اگر_ختم
		اگر (آغاز < max) {
			retval = ind_punch(fs, inode, بلاک_buf, bp, level,
					   آغاز, گنتی, عدد);
			اگر (retval)
				جاؤ errout;
			اگر (گنتی > max)
				گنتی -= max - آغاز;
			ورنہ
				توڑو;
			آغاز = 0;
		} ورنہ
			آغاز -= max;
		bp += عدد;
		اگر (level == 0) {
			عدد = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	اگر (buf)
		ext2fs_free_mem(&buf);
	واپس retval;
}
#تعریف_منسوخ BLK_T_MAX

#اگر_تعریف_شدہ PUNCH_DEBUG

#تعریف_کرو dbg_printf(f, a...)  لکھو(f, ## a)

ساکن خالی dbg_print_extent(حرف *desc, ساخت ext2fs_extent *extent)
{
	اگر (desc)
		لکھو("%s: ", desc);
	لکھو("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_برگ)
		fputs("LEAF ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	اگر (extent->e_flags & EXT2_EXTENT_FLAGS_دوسرا_دورہ)
		fputs("2ND_VISIT ", stdout);
	اگر (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#ورنہ
#تعریف_کرو dbg_print_extent(desc, ex)	کرو { } جب (0)
#تعریف_کرو dbg_printf(f, a...)		کرو { } جب (0)
#اگر_ختم

/* Free a range of blocks, respecting cluster boundaries */
ساکن errcode_t punch_extent_بلاکیں(ext2_filsys fs, ext2_ino_t ino,
				     ساخت ext2_inode *inode,
				     blk64_t lfree_آغاز, blk64_t free_آغاز,
				     __u32 free_گنتی, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	اگر (free_آغاز < fs->super->s_پہلا_ڈیٹا_بلاک ||
	    (free_آغاز + free_گنتی) > ext2fs_بلاکیں_گنتی(fs->super))
		واپس EXT2_ET_BAD_بلاک_عدد;

	/* No bigalloc?  Just free each block. */
	اگر (EXT2_FS_CLUSTER_تناسب(fs) == 1) {
		*freed += free_گنتی;
		جب (free_گنتی-- > 0)
			ext2fs_بلاک_alloc_stats2(fs, free_آغاز++, -1);
		واپس retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	اگر (free_آغاز & EXT2_FS_CLUSTER_نقاب(fs)) {
		retval = ext2fs_map_cluster_بلاک(fs, ino, inode,
						  lfree_آغاز, &pblk);
		اگر (retval)
			جاؤ errout;
		اگر (!pblk) {
			ext2fs_بلاک_alloc_stats2(fs, free_آغاز, -1);
			freed_now++;
		}
		cluster_freed = EXT2_FS_CLUSTER_تناسب(fs) -
			(free_آغاز & EXT2_FS_CLUSTER_نقاب(fs));
		اگر (cluster_freed > free_گنتی)
			cluster_freed = free_گنتی;
		free_گنتی -= cluster_freed;
		free_آغاز += cluster_freed;
		lfree_آغاز += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	جب (free_گنتی > 0 && free_گنتی >= (غیرعلامتی) EXT2_FS_CLUSTER_تناسب(fs)) {
		ext2fs_بلاک_alloc_stats2(fs, free_آغاز, -1);
		freed_now++;
		cluster_freed = EXT2_FS_CLUSTER_تناسب(fs);
		free_گنتی -= cluster_freed;
		free_آغاز += cluster_freed;
		lfree_آغاز += cluster_freed;
	}

	/* Try to free the last cluster. */
	اگر (free_گنتی > 0) {
		retval = ext2fs_map_cluster_بلاک(fs, ino, inode,
						  lfree_آغاز, &pblk);
		اگر (retval)
			جاؤ errout;
		اگر (!pblk) {
			ext2fs_بلاک_alloc_stats2(fs, free_آغاز, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	واپس retval;
}

ساکن errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     ساخت ext2_inode *inode,
				     blk64_t آغاز, blk64_t انتہا)
{
	ext2_extent_handle_t	handle = 0;
	ساخت ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_آغاز, اگلا, lfree_آغاز;
	__u32			free_گنتی, newlen;
	blk64_t			freed = 0;
	صحیح			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	اگر (retval)
		واپس retval;
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
	ext2fs_extent_goto(handle, آغاز);
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval == EXT2_ET_NO_CURRENT_گرہ) {
		retval = 0;
		جاؤ errout;
	} ورنہ اگر (retval)
		جاؤ errout;
	جب (1) {
		op = EXT2_EXTENT_اگلا_برگ;
		dbg_print_extent("main loop", &extent);
		اگلا = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (غیرعلامتی طویل طویل) آغاز,
			   (غیرعلامتی طویل طویل) انتہا,
			   (غیرعلامتی طویل طویل) اگلا);
		اگر (آغاز <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			اگر (انتہا < extent.e_lblk)
				توڑو;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_آغاز = extent.e_pblk;
			lfree_آغاز = extent.e_lblk;
			اگر (اگلا > انتہا)
				free_گنتی = انتہا - extent.e_lblk + 1;
			ورنہ
				free_گنتی = extent.e_len;
			extent.e_len -= free_گنتی;
			extent.e_lblk += free_گنتی;
			extent.e_pblk += free_گنتی;
		} ورنہ اگر (انتہا >= اگلا-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			اگر (آغاز >= اگلا)
				جاؤ اگلا_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = آغاز - extent.e_lblk;
			free_آغاز = extent.e_pblk + newlen;
			lfree_آغاز = extent.e_lblk + newlen;
			free_گنتی = extent.e_len - newlen;
			extent.e_len = newlen;
		} ورنہ {
			ساخت ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(انتہا + 1 - extent.e_lblk);
			newex.e_lblk = انتہا + 1;
			newex.e_len = اگلا - انتہا - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = آغاز - extent.e_lblk;
			free_آغاز = extent.e_pblk + extent.e_len;
			lfree_آغاز = extent.e_lblk + extent.e_len;
			free_گنتی = انتہا - آغاز + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_داخل_کرو(handle,
					EXT2_EXTENT_داخل_کرو_AFTER, &newex);
			اگر (retval)
				جاؤ errout;
			retval = ext2fs_extent_fix_parents(handle);
			اگر (retval)
				جاؤ errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			اگر (retval)
				جاؤ errout;
		} 
		اگر (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			اگر (retval)
				جاؤ errout;
			retval = ext2fs_extent_fix_parents(handle);
		} ورنہ {
			ساخت ext2fs_extent	newex;
			blk64_t			old_lblk, اگلا_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			اگر (retval)
				جاؤ errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_اگلا_برگ,
						   &newex);
			اگر (retval == EXT2_ET_EXTENT_NO_اگلا)
				اگلا_lblk = old_lblk;
			ورنہ اگر (retval)
				جاؤ errout;
			ورنہ
				اگلا_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			اگر (retval)
				جاؤ errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			اگر (retval)
				جاؤ errout;

			retval = ext2fs_extent_fix_parents(handle);
			اگر (retval && retval != EXT2_ET_NO_CURRENT_گرہ)
				جاؤ errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(خالی)ext2fs_extent_goto(handle, اگلا_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		اگر (retval)
			جاؤ errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_آغاز, free_گنتی);
		retval = punch_extent_بلاکیں(fs, ino, inode, lfree_آغاز,
					     free_آغاز, free_گنتی, &freed);
		اگر (retval)
			جاؤ errout;
	اگلا_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		اگر (retval == EXT2_ET_EXTENT_NO_اگلا ||
		    retval == EXT2_ET_NO_CURRENT_گرہ)
			توڑو;
		اگر (retval)
			جاؤ errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_بلاکیں(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	واپس retval;
}
	
ساکن errcode_t ext2fs_punch_inline_ڈیٹا(ext2_filsys fs, ext2_ino_t ino,
					  ساخت ext2_inode *inode,
					  blk64_t آغاز,
					  blk64_t انتہا EXT2FS_ATTR((غیر_مستعمل)))
{
	errcode_t retval;

	/*
	 * In libext2fs ext2fs_punch is based on block unit.  So that
	 * means that if start > 0 we don't need to do nothing.  Due
	 * to this we will remove all inline data in ext2fs_punch()
	 * now.
	 */
	اگر (آغاز > 0)
		واپس 0;

	memset((حرف *)inode->i_بلاک, 0, EXT4_MIN_INLINE_ڈیٹا_سائز);
	inode->i_سائز = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	اگر (retval)
		واپس retval;

	واپس ext2fs_inline_ڈیٹا_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       ساخت ext2_inode *inode,
		       حرف *بلاک_buf, blk64_t آغاز,
		       blk64_t انتہا)
{
	errcode_t		retval;
	ساخت ext2_inode	inode_buf;

	اگر (آغاز > انتہا)
		واپس EINVAL;

	/* Read inode structure if necessary */
	اگر (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		اگر (retval)
			واپس retval;
		inode = &inode_buf;
	}
	اگر (inode->i_flags & EXT4_INLINE_ڈیٹا_FL)
		واپس ext2fs_punch_inline_ڈیٹا(fs, ino, inode, آغاز, انتہا);
	ورنہ اگر (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, آغاز, انتہا);
	ورنہ
		retval = ext2fs_punch_ind(fs, inode, بلاک_buf, آغاز, انتہا);
	اگر (retval)
		واپس retval;

#اگر_تعریف_شدہ PUNCH_DEBUG
	لکھو("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_سائز(inode), inode->i_بلاکیں);
#اگر_ختم
	واپس ext2fs_write_inode(fs, ino, inode);
}
