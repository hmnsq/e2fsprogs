/*
 * bb_inode.c --- routines to update the bad block inode.
 *
 * WARNING: This routine modifies a lot of state in the filesystem; if
 * this routine returns an error, the bad block inode may be in an
 * inconsistent state.
 *
 * Copyright (C) 1994, 1995 Theodore Ts'o.
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
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਬਣਤਰ ਸੈੱਟ_badblock_ਰਿਕਾਰਡ {
	ext2_badblocks_iterate	bb_iter;
	ਪੂਰਨ		bad_ਖੰਡ_ਗਿਣਤੀ;
	blk_t		*ind_ਖੰਡਾਂ;
	ਪੂਰਨ		max_ind_ਖੰਡਾਂ;
	ਪੂਰਨ		ind_ਖੰਡਾਂ_ਮਾਪ;
	ਪੂਰਨ		ind_ਖੰਡਾਂ_ptr;
	ਅੱਖਰ		*ਖੰਡ_buf;
	errcode_t	err;
};

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_bad_ਖੰਡ_proc(ext2_filsys fs, blk_t *ਖੰਡ_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_ਖੰਡ, ਪੂਰਨ ref_ਵਿੱਥ,
			      ਖਾਲੀ *priv_ਅੰਕੜੇ);
ਸਥਿਰ ਪੂਰਨ clear_bad_ਖੰਡ_proc(ext2_filsys fs, blk_t *ਖੰਡ_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_ਖੰਡ, ਪੂਰਨ ref_ਵਿੱਥ,
				ਖਾਲੀ *priv_ਅੰਕੜੇ);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	ਬਣਤਰ ਸੈੱਟ_badblock_ਰਿਕਾਰਡ	rec;
	ਬਣਤਰ ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!fs->ਖੰਡ_map)
		ਵਾਪਸ EXT2_ET_NO_ਖੰਡ_BITMAP;

	memset(&rec, 0, ਆਕਾਰ(rec));
	rec.max_ind_ਖੰਡਾਂ = 10;
	retval = ext2fs_get_array(rec.max_ind_ਖੰਡਾਂ, ਆਕਾਰ(blk_t),
				&rec.ind_ਖੰਡਾਂ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	memset(rec.ind_ਖੰਡਾਂ, 0, rec.max_ind_ਖੰਡਾਂ * ਆਕਾਰ(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.ਖੰਡ_buf);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	memset(rec.ਖੰਡ_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_ਖੰਡ_iterate2(fs, EXT2_BAD_INO,
				       ਖੰਡ_FLAG_ਡੂੰਘਾਈ_TRAVERSE, 0,
				       clear_bad_ਖੰਡ_proc, &rec);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	ਜੇ (rec.err) {
		retval = rec.err;
		ਜਾਓ cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	ਜੇ (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		ਜੇ (retval)
			ਜਾਓ cleanup;
		retval = ext2fs_ਖੰਡ_iterate2(fs, EXT2_BAD_INO,
					       ਖੰਡ_FLAG_APPEND, 0,
					       ਸੈੱਟ_bad_ਖੰਡ_proc, &rec);
		ext2fs_badblocks_list_iterate_ਸਿਰਾ(rec.bb_iter);
		ਜੇ (retval)
			ਜਾਓ cleanup;
		ਜੇ (rec.err) {
			retval = rec.err;
			ਜਾਓ cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	now = ext2fs_P_get_ਸਮਾਂ(fs);
	ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_atime, now);
	ਜੇ (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_ctime, now);
	ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_mtime, now);
	ext2fs_iblk_ਸੈੱਟ(fs, &inode, rec.bad_ਖੰਡ_ਗਿਣਤੀ);
	retval = ext2fs_inode_ਮਾਪ_ਸੈੱਟ(fs, &inode,
				       rec.bad_ਖੰਡ_ਗਿਣਤੀ * fs->blocksize);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	ਜੇ (retval)
		ਜਾਓ cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_ਖੰਡਾਂ);
	ext2fs_free_mem(&rec.ਖੰਡ_buf);
	ਵਾਪਸ retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਪੂਰਨ clear_bad_ਖੰਡ_proc(ext2_filsys fs, blk_t *ਖੰਡ_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_ਖੰਡ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
				ਪੂਰਨ ref_ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
				ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ ਸੈੱਟ_badblock_ਰਿਕਾਰਡ *rec = (ਬਣਤਰ ਸੈੱਟ_badblock_ਰਿਕਾਰਡ *)
		priv_ਅੰਕੜੇ;
	errcode_t	retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ 	old_ਮਾਪ;

	ਜੇ (!*ਖੰਡ_nr)
		ਵਾਪਸ 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	ਜੇ (*ਖੰਡ_nr >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) ||
	    *ਖੰਡ_nr < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) {
		*ਖੰਡ_nr = 0;
		ਵਾਪਸ ਖੰਡ_ਬਦਲਿਆ;
	}

	ਜੇ (blockcnt < 0) {
		ਜੇ (rec->ind_ਖੰਡਾਂ_ਮਾਪ >= rec->max_ind_ਖੰਡਾਂ) {
			old_ਮਾਪ = rec->max_ind_ਖੰਡਾਂ * ਆਕਾਰ(blk_t);
			rec->max_ind_ਖੰਡਾਂ += 10;
			retval = ext2fs_resize_mem(old_ਮਾਪ,
				   rec->max_ind_ਖੰਡਾਂ * ਆਕਾਰ(blk_t),
				   &rec->ind_ਖੰਡਾਂ);
			ਜੇ (retval) {
				rec->max_ind_ਖੰਡਾਂ -= 10;
				rec->err = retval;
				ਵਾਪਸ ਖੰਡ_ABORT;
			}
		}
		rec->ind_ਖੰਡਾਂ[rec->ind_ਖੰਡਾਂ_ਮਾਪ++] = *ਖੰਡ_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_ਖੰਡ_alloc_stats2(fs, *ਖੰਡ_nr, -1);

	*ਖੰਡ_nr = 0;
	ਵਾਪਸ ਖੰਡ_ਬਦਲਿਆ;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_bad_ਖੰਡ_proc(ext2_filsys fs, blk_t *ਖੰਡ_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_ਖੰਡ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			      ਪੂਰਨ ref_ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			      ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ ਸੈੱਟ_badblock_ਰਿਕਾਰਡ *rec = (ਬਣਤਰ ਸੈੱਟ_badblock_ਰਿਕਾਰਡ *)
		priv_ਅੰਕੜੇ;
	errcode_t	retval;
	blk_t		blk;

	ਜੇ (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		ਜੇ (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			ਵਾਪਸ ਖੰਡ_ABORT;
		rec->bad_ਖੰਡ_ਗਿਣਤੀ++;
	} ਨਹੀਂਤਾਂ {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		ਜੇ (rec->ind_ਖੰਡਾਂ_ptr < rec->ind_ਖੰਡਾਂ_ਮਾਪ) {
			blk = rec->ind_ਖੰਡਾਂ[rec->ind_ਖੰਡਾਂ_ptr++];
			ਜੇ (ext2fs_test_ਖੰਡ_bitmap2(fs->ਖੰਡ_map, blk))
				ਜਾਓ retry;
		} ਨਹੀਂਤਾਂ {
			retval = ext2fs_new_ਖੰਡ(fs, 0, 0, &blk);
			ਜੇ (retval) {
				rec->err = retval;
				ਵਾਪਸ ਖੰਡ_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->ਖੰਡ_buf);
		ਜੇ (retval) {
			rec->err = retval;
			ਵਾਪਸ ਖੰਡ_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_ਖੰਡ_alloc_stats2(fs, blk, +1);

	*ਖੰਡ_nr = blk;
	ਵਾਪਸ ਖੰਡ_ਬਦਲਿਆ;
}






