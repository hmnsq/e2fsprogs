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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_zero_ಖಂಡ(ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ blocksize)
{
	ಅಕ್ಷರ	*cp = buf;
	ಪೂರ್ಣಾಂಕ	ಎಡ = blocksize;

	ಆಗ (ಎಡ > 0) {
		ಆದರೆ (*cp++)
			ಹಿಂತಿರುಗಿ 0;
		ಎಡ--;
	}
	ಹಿಂತಿರುಗಿ 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
ಸ್ಥಿರ errcode_t ind_punch(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
			   ಅಕ್ಷರ *ಖಂಡ_buf, blk_t *p, ಪೂರ್ಣಾಂಕ level,
			   blk64_t ಆರಂಭ, blk64_t ಎಣಿಕೆ_2, ಪೂರ್ಣಾಂಕ max)
{
	errcode_t	retval;
	blk_t		b;
	ಪೂರ್ಣಾಂಕ		i;
	blk64_t		ಸ್ಥಾನಾಂತರ, incr;
	ಸ್ಥಿರಾಂಕ blk64_t	ಕೊನೆ = ಆರಂಭ + ಎಣಿಕೆ_2;
	ಪೂರ್ಣಾಂಕ		freed = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PUNCH_DEBUG
	ಬರೆ("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, ಆರಂಭ, ಎಣಿಕೆ_2, max);
#ಆದರೆ_ಅಂತ್ಯ
	incr = 1ULL << ((EXT2_ಖಂಡ_ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು(fs->super) - 2) * level);
	ಗಾಗಿ (i = 0, ಸ್ಥಾನಾಂತರ = 0; i < max; i++, p++, ಸ್ಥಾನಾಂತರ += incr) {
		ಆದರೆ (ಸ್ಥಾನಾಂತರ >= ಕೊನೆ)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (*p == 0 || (ಸ್ಥಾನಾಂತರ+incr) <= ಆರಂಭ)
			ಮುಂದುವರಿ;
		b = *p;
		ಆದರೆ (level > 0) {
			blk_t start2;
			blk_t count2;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PUNCH_DEBUG
			ಬರೆ("Reading indirect block %u\n", b);
#ಆದರೆ_ಅಂತ್ಯ
			retval = ext2fs_read_ind_ಖಂಡ(fs, b, ಖಂಡ_buf);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
			start2 = (ಆರಂಭ > ಸ್ಥಾನಾಂತರ) ? ಆರಂಭ - ಸ್ಥಾನಾಂತರ : 0;
			count2 = ಕೊನೆ - ((ಆರಂಭ > ಸ್ಥಾನಾಂತರ) ? ಆರಂಭ : ಸ್ಥಾನಾಂತರ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PUNCH_DEBUG
			ಬರೆ("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", ಆರಂಭ,
			       ಸ್ಥಾನಾಂತರ, ಎಣಿಕೆ_2, ಕೊನೆ, incr, start2, count2);
#ಆದರೆ_ಅಂತ್ಯ
			retval = ind_punch(fs, inode, ಖಂಡ_buf + fs->blocksize,
					   (blk_t *) ಖಂಡ_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
			retval = ext2fs_write_ind_ಖಂಡ(fs, b, ಖಂಡ_buf);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
			ಆದರೆ (!check_zero_ಖಂಡ(ಖಂಡ_buf, fs->blocksize))
				ಮುಂದುವರಿ;
		}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PUNCH_DEBUG
		ಬರೆ("Freeing block %u (offset %llu)\n", b, ಸ್ಥಾನಾಂತರ);
#ಆದರೆ_ಅಂತ್ಯ
		ext2fs_ಖಂಡ_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PUNCH_DEBUG
	ಬರೆ("Freed %d blocks\n", freed);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ ext2fs_iblk_sub_ಖಂಡಗಳು(fs, inode, freed);
}

#ವ್ಯಾಖ್ಯಾನಿಸು BLK_T_MAX ((blk_t)~0ULL)
ಸ್ಥಿರ errcode_t ext2fs_punch_ind(ext2_filsys fs, ರಚನೆ ext2_inode *inode,
				  ಅಕ್ಷರ *ಖಂಡ_buf, blk64_t ಆರಂಭ, blk64_t ಕೊನೆ)
{
	errcode_t		retval;
	ಅಕ್ಷರ			*buf = 0;
	ಪೂರ್ಣಾಂಕ			level;
	ಪೂರ್ಣಾಂಕ			ಅಂಕೆ = EXT2_NDIR_ಖಂಡಗಳು;
	blk_t			*bp = inode->i_ಖಂಡ;
	blk_t			addr_ಪ್ರತಿ_ಖಂಡ;
	blk64_t			max = EXT2_NDIR_ಖಂಡಗಳು;
	blk_t			ಎಣಿಕೆ_2;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	ಆದರೆ (ಆರಂಭ > BLK_T_MAX)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (ಕೊನೆ >= BLK_T_MAX || ಕೊನೆ - ಆರಂಭ + 1 >= BLK_T_MAX)
		ಎಣಿಕೆ_2 = BLK_T_MAX - ಆರಂಭ;
	ಇಲ್ಲದಿದ್ದರೆ
		ಎಣಿಕೆ_2 = ಕೊನೆ - ಆರಂಭ + 1;

	ಆದರೆ (!ಖಂಡ_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಖಂಡ_buf = buf;
	}

	addr_ಪ್ರತಿ_ಖಂಡ = (blk_t)fs->blocksize >> 2;

	ಗಾಗಿ (level = 0; level < 4; level++, max *= (blk64_t)addr_ಪ್ರತಿ_ಖಂಡ) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PUNCH_DEBUG
		ಬರೆ("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, ಆರಂಭ, ಎಣಿಕೆ_2, max, ಅಂಕೆ);
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ (ಆರಂಭ < max) {
			retval = ind_punch(fs, inode, ಖಂಡ_buf, bp, level,
					   ಆರಂಭ, ಎಣಿಕೆ_2, ಅಂಕೆ);
			ಆದರೆ (retval)
				ಹೋಗು errout;
			ಆದರೆ (ಎಣಿಕೆ_2 > max)
				ಎಣಿಕೆ_2 -= max - ಆರಂಭ;
			ಇಲ್ಲದಿದ್ದರೆ
				ನಿಲ್ಲಿಸು;
			ಆರಂಭ = 0;
		} ಇಲ್ಲದಿದ್ದರೆ
			ಆರಂಭ -= max;
		bp += ಅಂಕೆ;
		ಆದರೆ (level == 0) {
			ಅಂಕೆ = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	ಆದರೆ (buf)
		ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
}
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು BLK_T_MAX

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PUNCH_DEBUG

#ವ್ಯಾಖ್ಯಾನಿಸು dbg_printf(f, a...)  ಬರೆ(f, ## a)

ಸ್ಥಿರ ಶೂನ್ಯ dbg_print_extent(ಅಕ್ಷರ *desc, ರಚನೆ ext2fs_extent *extent)
{
	ಆದರೆ (desc)
		ಬರೆ("%s: ", desc);
	ಬರೆ("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_ಎಲೆ)
		fputs("LEAF ", stdout);
	ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ಆದರೆ (extent->e_flags & EXT2_EXTENT_FLAGS_ಎರಡನೇ_ಭೇಟಿ)
		fputs("2ND_VISIT ", stdout);
	ಆದರೆ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು dbg_print_extent(desc, ex)	ಮಾಡು { } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು dbg_printf(f, a...)		ಮಾಡು { } ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ

/* Free a range of blocks, respecting cluster boundaries */
ಸ್ಥಿರ errcode_t punch_extent_ಖಂಡಗಳು(ext2_filsys fs, ext2_ino_t ino,
				     ರಚನೆ ext2_inode *inode,
				     blk64_t lfree_ಆರಂಭ, blk64_t free_ಆರಂಭ,
				     __u32 free_ಎಣಿಕೆ, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	ಆದರೆ (free_ಆರಂಭ < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ ||
	    (free_ಆರಂಭ + free_ಎಣಿಕೆ) > ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_ಖಂಡ_ಅಂಕೆ;

	/* No bigalloc?  Just free each block. */
	ಆದರೆ (EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) == 1) {
		*freed += free_ಎಣಿಕೆ;
		ಆಗ (free_ಎಣಿಕೆ-- > 0)
			ext2fs_ಖಂಡ_alloc_stats2(fs, free_ಆರಂಭ++, -1);
		ಹಿಂತಿರುಗಿ retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	ಆದರೆ (free_ಆರಂಭ & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs)) {
		retval = ext2fs_map_cluster_ಖಂಡ(fs, ino, inode,
						  lfree_ಆರಂಭ, &pblk);
		ಆದರೆ (retval)
			ಹೋಗು errout;
		ಆದರೆ (!pblk) {
			ext2fs_ಖಂಡ_alloc_stats2(fs, free_ಆರಂಭ, -1);
			freed_now++;
		}
		cluster_freed = EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) -
			(free_ಆರಂಭ & EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs));
		ಆದರೆ (cluster_freed > free_ಎಣಿಕೆ)
			cluster_freed = free_ಎಣಿಕೆ;
		free_ಎಣಿಕೆ -= cluster_freed;
		free_ಆರಂಭ += cluster_freed;
		lfree_ಆರಂಭ += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	ಆಗ (free_ಎಣಿಕೆ > 0 && free_ಎಣಿಕೆ >= (ಚಿಹ್ನೆರಹಿತ) EXT2_FS_CLUSTER_ಅನುಪಾತ(fs)) {
		ext2fs_ಖಂಡ_alloc_stats2(fs, free_ಆರಂಭ, -1);
		freed_now++;
		cluster_freed = EXT2_FS_CLUSTER_ಅನುಪಾತ(fs);
		free_ಎಣಿಕೆ -= cluster_freed;
		free_ಆರಂಭ += cluster_freed;
		lfree_ಆರಂಭ += cluster_freed;
	}

	/* Try to free the last cluster. */
	ಆದರೆ (free_ಎಣಿಕೆ > 0) {
		retval = ext2fs_map_cluster_ಖಂಡ(fs, ino, inode,
						  lfree_ಆರಂಭ, &pblk);
		ಆದರೆ (retval)
			ಹೋಗು errout;
		ಆದರೆ (!pblk) {
			ext2fs_ಖಂಡ_alloc_stats2(fs, free_ಆರಂಭ, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     ರಚನೆ ext2_inode *inode,
				     blk64_t ಆರಂಭ, blk64_t ಕೊನೆ)
{
	ext2_extent_handle_t	handle = 0;
	ರಚನೆ ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_ಆರಂಭ, ಮುಂದಿನ, lfree_ಆರಂಭ;
	__u32			free_ಎಣಿಕೆ, newlen;
	blk64_t			freed = 0;
	ಪೂರ್ಣಾಂಕ			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
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
	ext2fs_extent_goto(handle, ಆರಂಭ);
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ಆದರೆ (retval == EXT2_ET_NO_CURRENT_ಗಂಟು) {
		retval = 0;
		ಹೋಗು errout;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (retval)
		ಹೋಗು errout;
	ಆಗ (1) {
		op = EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ;
		dbg_print_extent("main loop", &extent);
		ಮುಂದಿನ = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಆರಂಭ,
			   (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಕೊನೆ,
			   (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಮುಂದಿನ);
		ಆದರೆ (ಆರಂಭ <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			ಆದರೆ (ಕೊನೆ < extent.e_lblk)
				ನಿಲ್ಲಿಸು;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_ಆರಂಭ = extent.e_pblk;
			lfree_ಆರಂಭ = extent.e_lblk;
			ಆದರೆ (ಮುಂದಿನ > ಕೊನೆ)
				free_ಎಣಿಕೆ = ಕೊನೆ - extent.e_lblk + 1;
			ಇಲ್ಲದಿದ್ದರೆ
				free_ಎಣಿಕೆ = extent.e_len;
			extent.e_len -= free_ಎಣಿಕೆ;
			extent.e_lblk += free_ಎಣಿಕೆ;
			extent.e_pblk += free_ಎಣಿಕೆ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೊನೆ >= ಮುಂದಿನ-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			ಆದರೆ (ಆರಂಭ >= ಮುಂದಿನ)
				ಹೋಗು ಮುಂದಿನ_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = ಆರಂಭ - extent.e_lblk;
			free_ಆರಂಭ = extent.e_pblk + newlen;
			lfree_ಆರಂಭ = extent.e_lblk + newlen;
			free_ಎಣಿಕೆ = extent.e_len - newlen;
			extent.e_len = newlen;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(ಕೊನೆ + 1 - extent.e_lblk);
			newex.e_lblk = ಕೊನೆ + 1;
			newex.e_len = ಮುಂದಿನ - ಕೊನೆ - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = ಆರಂಭ - extent.e_lblk;
			free_ಆರಂಭ = extent.e_pblk + extent.e_len;
			lfree_ಆರಂಭ = extent.e_lblk + extent.e_len;
			free_ಎಣಿಕೆ = ಕೊನೆ - ಆರಂಭ + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_ಸೇರಿಸು(handle,
					EXT2_EXTENT_ಸೇರಿಸು_AFTER, &newex);
			ಆದರೆ (retval)
				ಹೋಗು errout;
			retval = ext2fs_extent_fix_parents(handle);
			ಆದರೆ (retval)
				ಹೋಗು errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			ಆದರೆ (retval)
				ಹೋಗು errout;
		} 
		ಆದರೆ (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			ಆದರೆ (retval)
				ಹೋಗು errout;
			retval = ext2fs_extent_fix_parents(handle);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ ext2fs_extent	newex;
			blk64_t			old_lblk, ಮುಂದಿನ_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			ಆದರೆ (retval)
				ಹೋಗು errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ಮುಂದಿನ_ಎಲೆ,
						   &newex);
			ಆದರೆ (retval == EXT2_ET_EXTENT_NO_ಮುಂದಿನ)
				ಮುಂದಿನ_lblk = old_lblk;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (retval)
				ಹೋಗು errout;
			ಇಲ್ಲದಿದ್ದರೆ
				ಮುಂದಿನ_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			ಆದರೆ (retval)
				ಹೋಗು errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			ಆದರೆ (retval)
				ಹೋಗು errout;

			retval = ext2fs_extent_fix_parents(handle);
			ಆದರೆ (retval && retval != EXT2_ET_NO_CURRENT_ಗಂಟು)
				ಹೋಗು errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(ಶೂನ್ಯ)ext2fs_extent_goto(handle, ಮುಂದಿನ_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		ಆದರೆ (retval)
			ಹೋಗು errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_ಆರಂಭ, free_ಎಣಿಕೆ);
		retval = punch_extent_ಖಂಡಗಳು(fs, ino, inode, lfree_ಆರಂಭ,
					     free_ಆರಂಭ, free_ಎಣಿಕೆ, &freed);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	ಮುಂದಿನ_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		ಆದರೆ (retval == EXT2_ET_EXTENT_NO_ಮುಂದಿನ ||
		    retval == EXT2_ET_NO_CURRENT_ಗಂಟು)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_ಖಂಡಗಳು(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	ಹಿಂತಿರುಗಿ retval;
}
	
ಸ್ಥಿರ errcode_t ext2fs_punch_inline_ದತ್ತಾಂಶ(ext2_filsys fs, ext2_ino_t ino,
					  ರಚನೆ ext2_inode *inode,
					  blk64_t ಆರಂಭ,
					  blk64_t ಕೊನೆ EXT2FS_ATTR((ಬಳಸದ)))
{
	errcode_t retval;

	/*
	 * In libext2fs ext2fs_punch is based on block unit.  So that
	 * means that if start > 0 we don't need to do nothing.  Due
	 * to this we will remove all inline data in ext2fs_punch()
	 * now.
	 */
	ಆದರೆ (ಆರಂಭ > 0)
		ಹಿಂತಿರುಗಿ 0;

	memset((ಅಕ್ಷರ *)inode->i_ಖಂಡ, 0, EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ);
	inode->i_ಗಾತ್ರ = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಹಿಂತಿರುಗಿ ext2fs_inline_ದತ್ತಾಂಶ_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       ರಚನೆ ext2_inode *inode,
		       ಅಕ್ಷರ *ಖಂಡ_buf, blk64_t ಆರಂಭ,
		       blk64_t ಕೊನೆ)
{
	errcode_t		retval;
	ರಚನೆ ext2_inode	inode_buf;

	ಆದರೆ (ಆರಂಭ > ಕೊನೆ)
		ಹಿಂತಿರುಗಿ EINVAL;

	/* Read inode structure if necessary */
	ಆದರೆ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		inode = &inode_buf;
	}
	ಆದರೆ (inode->i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL)
		ಹಿಂತಿರುಗಿ ext2fs_punch_inline_ದತ್ತಾಂಶ(fs, ino, inode, ಆರಂಭ, ಕೊನೆ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, ಆರಂಭ, ಕೊನೆ);
	ಇಲ್ಲದಿದ್ದರೆ
		retval = ext2fs_punch_ind(fs, inode, ಖಂಡ_buf, ಆರಂಭ, ಕೊನೆ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PUNCH_DEBUG
	ಬರೆ("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_ಗಾತ್ರ(inode), inode->i_ಖಂಡಗಳು);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ ext2fs_write_inode(fs, ino, inode);
}
