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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PUNCH_DEBUG

/*
 * This function returns 1 if the specified block is all zeros
 */
ਸਥਿਰ ਪੂਰਨ check_zero_ਖੰਡ(ਅੱਖਰ *buf, ਪੂਰਨ blocksize)
{
	ਅੱਖਰ	*cp = buf;
	ਪੂਰਨ	ਖੱਬਾ = blocksize;

	ਜਦੋਂ (ਖੱਬਾ > 0) {
		ਜੇ (*cp++)
			ਵਾਪਸ 0;
		ਖੱਬਾ--;
	}
	ਵਾਪਸ 1;
}

/*
 * This clever recursive function handles i_blocks[] as well as
 * indirect, double indirect, and triple indirect blocks.  It iterates
 * over the entries in the i_blocks array or indirect blocks, and for
 * each one, will recursively handle any indirect blocks and then
 * frees and deallocates the blocks.
 */
ਸਥਿਰ errcode_t ind_punch(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
			   ਅੱਖਰ *ਖੰਡ_buf, blk_t *p, ਪੂਰਨ level,
			   blk64_t ਸ਼ੁਰੂ, blk64_t ਗਿਣਤੀ_2, ਪੂਰਨ max)
{
	errcode_t	retval;
	blk_t		b;
	ਪੂਰਨ		i;
	blk64_t		ਵਿੱਥ, incr;
	ਅਚਲ blk64_t	ਸਿਰਾ = ਸ਼ੁਰੂ + ਗਿਣਤੀ_2;
	ਪੂਰਨ		freed = 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ PUNCH_DEBUG
	ਲਿਖੋ("Entering ind_punch, level %d, start %llu, count %llu, "
	       "max %d\n", level, ਸ਼ੁਰੂ, ਗਿਣਤੀ_2, max);
#ਜੇ_ਅੰਤ
	incr = 1ULL << ((EXT2_ਖੰਡ_ਮਾਪ_ਬਿੱਟ(fs->super) - 2) * level);
	ਲਈ (i = 0, ਵਿੱਥ = 0; i < max; i++, p++, ਵਿੱਥ += incr) {
		ਜੇ (ਵਿੱਥ >= ਸਿਰਾ)
			ਰੋਕੋ;
		ਜੇ (*p == 0 || (ਵਿੱਥ+incr) <= ਸ਼ੁਰੂ)
			ਜਾਰੀ;
		b = *p;
		ਜੇ (level > 0) {
			blk_t start2;
			blk_t count2;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ PUNCH_DEBUG
			ਲਿਖੋ("Reading indirect block %u\n", b);
#ਜੇ_ਅੰਤ
			retval = ext2fs_read_ind_ਖੰਡ(fs, b, ਖੰਡ_buf);
			ਜੇ (retval)
				ਵਾਪਸ retval;
			start2 = (ਸ਼ੁਰੂ > ਵਿੱਥ) ? ਸ਼ੁਰੂ - ਵਿੱਥ : 0;
			count2 = ਸਿਰਾ - ((ਸ਼ੁਰੂ > ਵਿੱਥ) ? ਸ਼ੁਰੂ : ਵਿੱਥ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ PUNCH_DEBUG
			ਲਿਖੋ("start %llu offset %llu count %llu end %llu "
			       "incr %llu start2 %u count2 %u\n", ਸ਼ੁਰੂ,
			       ਵਿੱਥ, ਗਿਣਤੀ_2, ਸਿਰਾ, incr, start2, count2);
#ਜੇ_ਅੰਤ
			retval = ind_punch(fs, inode, ਖੰਡ_buf + fs->blocksize,
					   (blk_t *) ਖੰਡ_buf, level - 1,
					   start2, count2,
					   fs->blocksize >> 2);
			ਜੇ (retval)
				ਵਾਪਸ retval;
			retval = ext2fs_write_ind_ਖੰਡ(fs, b, ਖੰਡ_buf);
			ਜੇ (retval)
				ਵਾਪਸ retval;
			ਜੇ (!check_zero_ਖੰਡ(ਖੰਡ_buf, fs->blocksize))
				ਜਾਰੀ;
		}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ PUNCH_DEBUG
		ਲਿਖੋ("Freeing block %u (offset %llu)\n", b, ਵਿੱਥ);
#ਜੇ_ਅੰਤ
		ext2fs_ਖੰਡ_alloc_stats(fs, b, -1);
		*p = 0;
		freed++;
	}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ PUNCH_DEBUG
	ਲਿਖੋ("Freed %d blocks\n", freed);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ ext2fs_iblk_sub_ਖੰਡਾਂ(fs, inode, freed);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLK_T_MAX ((blk_t)~0ULL)
ਸਥਿਰ errcode_t ext2fs_punch_ind(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode,
				  ਅੱਖਰ *ਖੰਡ_buf, blk64_t ਸ਼ੁਰੂ, blk64_t ਸਿਰਾ)
{
	errcode_t		retval;
	ਅੱਖਰ			*buf = 0;
	ਪੂਰਨ			level;
	ਪੂਰਨ			ਅੰਕ = EXT2_NDIR_ਖੰਡਾਂ;
	blk_t			*bp = inode->i_ਖੰਡ;
	blk_t			addr_ਪ੍ਰਤੀ_ਖੰਡ;
	blk64_t			max = EXT2_NDIR_ਖੰਡਾਂ;
	blk_t			ਗਿਣਤੀ_2;

	/* Check start/end don't overflow the 2^32-1 indirect block limit */
	ਜੇ (ਸ਼ੁਰੂ > BLK_T_MAX)
		ਵਾਪਸ 0;
	ਜੇ (ਸਿਰਾ >= BLK_T_MAX || ਸਿਰਾ - ਸ਼ੁਰੂ + 1 >= BLK_T_MAX)
		ਗਿਣਤੀ_2 = BLK_T_MAX - ਸ਼ੁਰੂ;
	ਨਹੀਂਤਾਂ
		ਗਿਣਤੀ_2 = ਸਿਰਾ - ਸ਼ੁਰੂ + 1;

	ਜੇ (!ਖੰਡ_buf) {
		retval = ext2fs_get_array(3, fs->blocksize, &buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਖੰਡ_buf = buf;
	}

	addr_ਪ੍ਰਤੀ_ਖੰਡ = (blk_t)fs->blocksize >> 2;

	ਲਈ (level = 0; level < 4; level++, max *= (blk64_t)addr_ਪ੍ਰਤੀ_ਖੰਡ) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ PUNCH_DEBUG
		ਲਿਖੋ("Main loop level %d, start %llu count %u "
		       "max %llu num %d\n", level, ਸ਼ੁਰੂ, ਗਿਣਤੀ_2, max, ਅੰਕ);
#ਜੇ_ਅੰਤ
		ਜੇ (ਸ਼ੁਰੂ < max) {
			retval = ind_punch(fs, inode, ਖੰਡ_buf, bp, level,
					   ਸ਼ੁਰੂ, ਗਿਣਤੀ_2, ਅੰਕ);
			ਜੇ (retval)
				ਜਾਓ errout;
			ਜੇ (ਗਿਣਤੀ_2 > max)
				ਗਿਣਤੀ_2 -= max - ਸ਼ੁਰੂ;
			ਨਹੀਂਤਾਂ
				ਰੋਕੋ;
			ਸ਼ੁਰੂ = 0;
		} ਨਹੀਂਤਾਂ
			ਸ਼ੁਰੂ -= max;
		bp += ਅੰਕ;
		ਜੇ (level == 0) {
			ਅੰਕ = 1;
			max = 1;
		}
	}
	retval = 0;
errout:
	ਜੇ (buf)
		ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
}
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ BLK_T_MAX

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ PUNCH_DEBUG

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dbg_printf(f, a...)  ਲਿਖੋ(f, ## a)

ਸਥਿਰ ਖਾਲੀ dbg_print_extent(ਅੱਖਰ *desc, ਬਣਤਰ ext2fs_extent *extent)
{
	ਜੇ (desc)
		ਲਿਖੋ("%s: ", desc);
	ਲਿਖੋ("extent: lblk %llu--%llu, len %u, pblk %llu, flags: ",
	       extent->e_lblk, extent->e_lblk + extent->e_len - 1,
	       extent->e_len, extent->e_pblk);
	ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_ਪੱਤੀ)
		fputs("LEAF ", stdout);
	ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_UNINIT)
		fputs("UNINIT ", stdout);
	ਜੇ (extent->e_flags & EXT2_EXTENT_FLAGS_ਦੂਜਾ_ਦੇਖੋ)
		fputs("2ND_VISIT ", stdout);
	ਜੇ (!extent->e_flags)
		fputs("(none)", stdout);
	fputc('\n', stdout);

}
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dbg_print_extent(desc, ex)	ਕਰੋ { } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ dbg_printf(f, a...)		ਕਰੋ { } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

/* Free a range of blocks, respecting cluster boundaries */
ਸਥਿਰ errcode_t punch_extent_ਖੰਡਾਂ(ext2_filsys fs, ext2_ino_t ino,
				     ਬਣਤਰ ext2_inode *inode,
				     blk64_t lfree_ਸ਼ੁਰੂ, blk64_t free_ਸ਼ੁਰੂ,
				     __u32 free_ਗਿਣਤੀ, blk64_t *freed)
{
	blk64_t		pblk;
	__u32		freed_now = 0;
	__u32		cluster_freed;
	errcode_t	retval = 0;

	ਜੇ (free_ਸ਼ੁਰੂ < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ ||
	    (free_ਸ਼ੁਰੂ + free_ਗਿਣਤੀ) > ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))
		ਵਾਪਸ EXT2_ET_BAD_ਖੰਡ_ਅੰਕ;

	/* No bigalloc?  Just free each block. */
	ਜੇ (EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) == 1) {
		*freed += free_ਗਿਣਤੀ;
		ਜਦੋਂ (free_ਗਿਣਤੀ-- > 0)
			ext2fs_ਖੰਡ_alloc_stats2(fs, free_ਸ਼ੁਰੂ++, -1);
		ਵਾਪਸ retval;
	}

	/*
	 * Try to free up to the next cluster boundary.  We assume that all
	 * blocks in a logical cluster map to blocks from the same physical
	 * cluster, and that the offsets within the [pl]clusters match.
	 */
	ਜੇ (free_ਸ਼ੁਰੂ & EXT2_FS_CLUSTER_ਨਕਾਬ(fs)) {
		retval = ext2fs_map_cluster_ਖੰਡ(fs, ino, inode,
						  lfree_ਸ਼ੁਰੂ, &pblk);
		ਜੇ (retval)
			ਜਾਓ errout;
		ਜੇ (!pblk) {
			ext2fs_ਖੰਡ_alloc_stats2(fs, free_ਸ਼ੁਰੂ, -1);
			freed_now++;
		}
		cluster_freed = EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) -
			(free_ਸ਼ੁਰੂ & EXT2_FS_CLUSTER_ਨਕਾਬ(fs));
		ਜੇ (cluster_freed > free_ਗਿਣਤੀ)
			cluster_freed = free_ਗਿਣਤੀ;
		free_ਗਿਣਤੀ -= cluster_freed;
		free_ਸ਼ੁਰੂ += cluster_freed;
		lfree_ਸ਼ੁਰੂ += cluster_freed;
	}

	/* Free whole clusters from the middle of the range. */
	ਜਦੋਂ (free_ਗਿਣਤੀ > 0 && free_ਗਿਣਤੀ >= (ਬਿਨਾਨਿਸ਼ਾਨੀ) EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs)) {
		ext2fs_ਖੰਡ_alloc_stats2(fs, free_ਸ਼ੁਰੂ, -1);
		freed_now++;
		cluster_freed = EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs);
		free_ਗਿਣਤੀ -= cluster_freed;
		free_ਸ਼ੁਰੂ += cluster_freed;
		lfree_ਸ਼ੁਰੂ += cluster_freed;
	}

	/* Try to free the last cluster. */
	ਜੇ (free_ਗਿਣਤੀ > 0) {
		retval = ext2fs_map_cluster_ਖੰਡ(fs, ino, inode,
						  lfree_ਸ਼ੁਰੂ, &pblk);
		ਜੇ (retval)
			ਜਾਓ errout;
		ਜੇ (!pblk) {
			ext2fs_ਖੰਡ_alloc_stats2(fs, free_ਸ਼ੁਰੂ, -1);
			freed_now++;
		}
	}

errout:
	*freed += freed_now;
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t ext2fs_punch_extent(ext2_filsys fs, ext2_ino_t ino,
				     ਬਣਤਰ ext2_inode *inode,
				     blk64_t ਸ਼ੁਰੂ, blk64_t ਸਿਰਾ)
{
	ext2_extent_handle_t	handle = 0;
	ਬਣਤਰ ext2fs_extent	extent;
	errcode_t		retval;
	blk64_t			free_ਸ਼ੁਰੂ, ਅਗਲਾ, lfree_ਸ਼ੁਰੂ;
	__u32			free_ਗਿਣਤੀ, newlen;
	blk64_t			freed = 0;
	ਪੂਰਨ			op;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ਜੇ (retval)
		ਵਾਪਸ retval;
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
	ext2fs_extent_goto(handle, ਸ਼ੁਰੂ);
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ਜੇ (retval == EXT2_ET_NO_CURRENT_ਗੰਢ) {
		retval = 0;
		ਜਾਓ errout;
	} ਨਹੀਂਤਾਂ ਜੇ (retval)
		ਜਾਓ errout;
	ਜਦੋਂ (1) {
		op = EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ;
		dbg_print_extent("main loop", &extent);
		ਅਗਲਾ = extent.e_lblk + extent.e_len;
		dbg_printf("start %llu, end %llu, next %llu\n",
			   (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਸ਼ੁਰੂ,
			   (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਸਿਰਾ,
			   (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਅਗਲਾ);
		ਜੇ (ਸ਼ੁਰੂ <= extent.e_lblk) {
			/*
			 * Have we iterated past the end of the punch region?
			 * If so, we can stop.
			 */
			ਜੇ (ਸਿਰਾ < extent.e_lblk)
				ਰੋਕੋ;
			dbg_printf("Case #%d\n", 1);
			/* Start of deleted region before extent; 
			   adjust beginning of extent */
			free_ਸ਼ੁਰੂ = extent.e_pblk;
			lfree_ਸ਼ੁਰੂ = extent.e_lblk;
			ਜੇ (ਅਗਲਾ > ਸਿਰਾ)
				free_ਗਿਣਤੀ = ਸਿਰਾ - extent.e_lblk + 1;
			ਨਹੀਂਤਾਂ
				free_ਗਿਣਤੀ = extent.e_len;
			extent.e_len -= free_ਗਿਣਤੀ;
			extent.e_lblk += free_ਗਿਣਤੀ;
			extent.e_pblk += free_ਗਿਣਤੀ;
		} ਨਹੀਂਤਾਂ ਜੇ (ਸਿਰਾ >= ਅਗਲਾ-1) {
			/*
			 * Is the punch region beyond this extent?  This can
			 * happen if start is already inside a hole.  Try to
			 * advance to the next extent if this is the case.
			 */
			ਜੇ (ਸ਼ੁਰੂ >= ਅਗਲਾ)
				ਜਾਓ ਅਗਲਾ_extent;
			/* End of deleted region after extent;
			   adjust end of extent */
			dbg_printf("Case #%d\n", 2);
			newlen = ਸ਼ੁਰੂ - extent.e_lblk;
			free_ਸ਼ੁਰੂ = extent.e_pblk + newlen;
			lfree_ਸ਼ੁਰੂ = extent.e_lblk + newlen;
			free_ਗਿਣਤੀ = extent.e_len - newlen;
			extent.e_len = newlen;
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ ext2fs_extent	newex;

			dbg_printf("Case #%d\n", 3);
			/* The hard case; we need to split the extent */
			newex.e_pblk = extent.e_pblk +
				(ਸਿਰਾ + 1 - extent.e_lblk);
			newex.e_lblk = ਸਿਰਾ + 1;
			newex.e_len = ਅਗਲਾ - ਸਿਰਾ - 1;
			newex.e_flags = extent.e_flags;

			extent.e_len = ਸ਼ੁਰੂ - extent.e_lblk;
			free_ਸ਼ੁਰੂ = extent.e_pblk + extent.e_len;
			lfree_ਸ਼ੁਰੂ = extent.e_lblk + extent.e_len;
			free_ਗਿਣਤੀ = ਸਿਰਾ - ਸ਼ੁਰੂ + 1;

			dbg_print_extent("inserting", &newex);
			retval = ext2fs_extent_ਪਾਓ(handle,
					EXT2_EXTENT_ਪਾਓ_AFTER, &newex);
			ਜੇ (retval)
				ਜਾਓ errout;
			retval = ext2fs_extent_fix_parents(handle);
			ਜੇ (retval)
				ਜਾਓ errout;
			/*
			 * Now pointing at inserted extent; so go back.
			 *
			 * We cannot use EXT2_EXTENT_PREV to go back; note the
			 * subtlety in the comment for fix_parents().
			 */
			retval = ext2fs_extent_goto(handle, extent.e_lblk);
			ਜੇ (retval)
				ਜਾਓ errout;
		} 
		ਜੇ (extent.e_len) {
			dbg_print_extent("replacing", &extent);
			retval = ext2fs_extent_replace(handle, 0, &extent);
			ਜੇ (retval)
				ਜਾਓ errout;
			retval = ext2fs_extent_fix_parents(handle);
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ ext2fs_extent	newex;
			blk64_t			old_lblk, ਅਗਲਾ_lblk;
			dbg_printf("deleting current extent%s\n", "");

			/*
			 * Save the location of the next leaf, then slip
			 * back to the current extent.
			 */
			retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT,
						   &newex);
			ਜੇ (retval)
				ਜਾਓ errout;
			old_lblk = newex.e_lblk;

			retval = ext2fs_extent_get(handle,
						   EXT2_EXTENT_ਅਗਲਾ_ਪੱਤੀ,
						   &newex);
			ਜੇ (retval == EXT2_ET_EXTENT_NO_ਅਗਲਾ)
				ਅਗਲਾ_lblk = old_lblk;
			ਨਹੀਂਤਾਂ ਜੇ (retval)
				ਜਾਓ errout;
			ਨਹੀਂਤਾਂ
				ਅਗਲਾ_lblk = newex.e_lblk;

			retval = ext2fs_extent_goto(handle, old_lblk);
			ਜੇ (retval)
				ਜਾਓ errout;

			/* Now delete the extent. */
			retval = ext2fs_extent_delete(handle, 0);
			ਜੇ (retval)
				ਜਾਓ errout;

			retval = ext2fs_extent_fix_parents(handle);
			ਜੇ (retval && retval != EXT2_ET_NO_CURRENT_ਗੰਢ)
				ਜਾਓ errout;
			retval = 0;

			/*
			 * Jump forward to the next extent.  If there are
			 * errors, the ext2fs_extent_get down below will
			 * capture them for us.
			 */
			(ਖਾਲੀ)ext2fs_extent_goto(handle, ਅਗਲਾ_lblk);
			op = EXT2_EXTENT_CURRENT;
		}
		ਜੇ (retval)
			ਜਾਓ errout;
		dbg_printf("Free start %llu, free count = %u\n",
		       free_ਸ਼ੁਰੂ, free_ਗਿਣਤੀ);
		retval = punch_extent_ਖੰਡਾਂ(fs, ino, inode, lfree_ਸ਼ੁਰੂ,
					     free_ਸ਼ੁਰੂ, free_ਗਿਣਤੀ, &freed);
		ਜੇ (retval)
			ਜਾਓ errout;
	ਅਗਲਾ_extent:
		retval = ext2fs_extent_get(handle, op,
					   &extent);
		ਜੇ (retval == EXT2_ET_EXTENT_NO_ਅਗਲਾ ||
		    retval == EXT2_ET_NO_CURRENT_ਗੰਢ)
			ਰੋਕੋ;
		ਜੇ (retval)
			ਜਾਓ errout;
	}
	dbg_printf("Freed %llu blocks\n", freed);
	retval = ext2fs_iblk_sub_ਖੰਡਾਂ(fs, inode, freed);
errout:
	ext2fs_extent_free(handle);
	ਵਾਪਸ retval;
}
	
ਸਥਿਰ errcode_t ext2fs_punch_inline_ਅੰਕੜੇ(ext2_filsys fs, ext2_ino_t ino,
					  ਬਣਤਰ ext2_inode *inode,
					  blk64_t ਸ਼ੁਰੂ,
					  blk64_t ਸਿਰਾ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	errcode_t retval;

	/*
	 * In libext2fs ext2fs_punch is based on block unit.  So that
	 * means that if start > 0 we don't need to do nothing.  Due
	 * to this we will remove all inline data in ext2fs_punch()
	 * now.
	 */
	ਜੇ (ਸ਼ੁਰੂ > 0)
		ਵਾਪਸ 0;

	memset((ਅੱਖਰ *)inode->i_ਖੰਡ, 0, EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ);
	inode->i_ਮਾਪ = 0;
	retval = ext2fs_write_inode(fs, ino, inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਵਾਪਸ ext2fs_inline_ਅੰਕੜੇ_ea_remove(fs, ino);
}

/*
 * Deallocate all logical _blocks_ starting at start to end, inclusive.
 * If end is ~0ULL, then this is effectively truncate.
 */
errcode_t ext2fs_punch(ext2_filsys fs, ext2_ino_t ino,
		       ਬਣਤਰ ext2_inode *inode,
		       ਅੱਖਰ *ਖੰਡ_buf, blk64_t ਸ਼ੁਰੂ,
		       blk64_t ਸਿਰਾ)
{
	errcode_t		retval;
	ਬਣਤਰ ext2_inode	inode_buf;

	ਜੇ (ਸ਼ੁਰੂ > ਸਿਰਾ)
		ਵਾਪਸ EINVAL;

	/* Read inode structure if necessary */
	ਜੇ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		inode = &inode_buf;
	}
	ਜੇ (inode->i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL)
		ਵਾਪਸ ext2fs_punch_inline_ਅੰਕੜੇ(fs, ino, inode, ਸ਼ੁਰੂ, ਸਿਰਾ);
	ਨਹੀਂਤਾਂ ਜੇ (inode->i_flags & EXT4_EXTENTS_FL)
		retval = ext2fs_punch_extent(fs, ino, inode, ਸ਼ੁਰੂ, ਸਿਰਾ);
	ਨਹੀਂਤਾਂ
		retval = ext2fs_punch_ind(fs, inode, ਖੰਡ_buf, ਸ਼ੁਰੂ, ਸਿਰਾ);
	ਜੇ (retval)
		ਵਾਪਸ retval;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ PUNCH_DEBUG
	ਲਿਖੋ("%u: write inode size now %lu blocks %u\n",
		ino, EXT2_I_ਮਾਪ(inode), inode->i_ਖੰਡਾਂ);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ ext2fs_write_inode(fs, ino, inode);
}
