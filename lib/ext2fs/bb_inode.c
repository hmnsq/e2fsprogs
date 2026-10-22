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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ರಚನೆ ಗಣ_badblock_ದಾಖಲೆ {
	ext2_badblocks_iterate	bb_iter;
	ಪೂರ್ಣಾಂಕ		bad_ಖಂಡ_ಎಣಿಕೆ;
	blk_t		*ind_ಖಂಡಗಳು;
	ಪೂರ್ಣಾಂಕ		max_ind_ಖಂಡಗಳು;
	ಪೂರ್ಣಾಂಕ		ind_ಖಂಡಗಳು_ಗಾತ್ರ;
	ಪೂರ್ಣಾಂಕ		ind_ಖಂಡಗಳು_ptr;
	ಅಕ್ಷರ		*ಖಂಡ_buf;
	errcode_t	err;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_bad_ಖಂಡ_proc(ext2_filsys fs, blk_t *ಖಂಡ_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_ಖಂಡ, ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ,
			      ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ clear_bad_ಖಂಡ_proc(ext2_filsys fs, blk_t *ಖಂಡ_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_ಖಂಡ, ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ,
				ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	ರಚನೆ ಗಣ_badblock_ದಾಖಲೆ	rec;
	ರಚನೆ ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!fs->ಖಂಡ_map)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಖಂಡ_BITMAP;

	memset(&rec, 0, ಗಾತ್ರ(rec));
	rec.max_ind_ಖಂಡಗಳು = 10;
	retval = ext2fs_get_array(rec.max_ind_ಖಂಡಗಳು, ಗಾತ್ರ(blk_t),
				&rec.ind_ಖಂಡಗಳು);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	memset(rec.ind_ಖಂಡಗಳು, 0, rec.max_ind_ಖಂಡಗಳು * ಗಾತ್ರ(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.ಖಂಡ_buf);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	memset(rec.ಖಂಡ_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_ಖಂಡ_iterate2(fs, EXT2_BAD_INO,
				       ಖಂಡ_FLAG_ಆಳ_TRAVERSE, 0,
				       clear_bad_ಖಂಡ_proc, &rec);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	ಆದರೆ (rec.err) {
		retval = rec.err;
		ಹೋಗು cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	ಆದರೆ (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
		retval = ext2fs_ಖಂಡ_iterate2(fs, EXT2_BAD_INO,
					       ಖಂಡ_FLAG_APPEND, 0,
					       ಗಣ_bad_ಖಂಡ_proc, &rec);
		ext2fs_badblocks_list_iterate_ಕೊನೆ(rec.bb_iter);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
		ಆದರೆ (rec.err) {
			retval = rec.err;
			ಹೋಗು cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	now = ext2fs_P_get_ಸಮಯ(fs);
	ext2fs_inode_xtime_ಗಣ(&inode, i_atime, now);
	ಆದರೆ (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_ಗಣ(&inode, i_ctime, now);
	ext2fs_inode_xtime_ಗಣ(&inode, i_mtime, now);
	ext2fs_iblk_ಗಣ(fs, &inode, rec.bad_ಖಂಡ_ಎಣಿಕೆ);
	retval = ext2fs_inode_ಗಾತ್ರ_ಗಣ(fs, &inode,
				       rec.bad_ಖಂಡ_ಎಣಿಕೆ * fs->blocksize);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_ಖಂಡಗಳು);
	ext2fs_free_mem(&rec.ಖಂಡ_buf);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ clear_bad_ಖಂಡ_proc(ext2_filsys fs, blk_t *ಖಂಡ_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_ಖಂಡ EXT2FS_ATTR((ಬಳಸದ)),
				ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
				ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ ಗಣ_badblock_ದಾಖಲೆ *rec = (ರಚನೆ ಗಣ_badblock_ದಾಖಲೆ *)
		priv_ದತ್ತಾಂಶ;
	errcode_t	retval;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ 	old_ಗಾತ್ರ;

	ಆದರೆ (!*ಖಂಡ_nr)
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	ಆದರೆ (*ಖಂಡ_nr >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) ||
	    *ಖಂಡ_nr < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) {
		*ಖಂಡ_nr = 0;
		ಹಿಂತಿರುಗಿ ಖಂಡ_ಬದಲಾಯಿತು;
	}

	ಆದರೆ (blockcnt < 0) {
		ಆದರೆ (rec->ind_ಖಂಡಗಳು_ಗಾತ್ರ >= rec->max_ind_ಖಂಡಗಳು) {
			old_ಗಾತ್ರ = rec->max_ind_ಖಂಡಗಳು * ಗಾತ್ರ(blk_t);
			rec->max_ind_ಖಂಡಗಳು += 10;
			retval = ext2fs_resize_mem(old_ಗಾತ್ರ,
				   rec->max_ind_ಖಂಡಗಳು * ಗಾತ್ರ(blk_t),
				   &rec->ind_ಖಂಡಗಳು);
			ಆದರೆ (retval) {
				rec->max_ind_ಖಂಡಗಳು -= 10;
				rec->err = retval;
				ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
			}
		}
		rec->ind_ಖಂಡಗಳು[rec->ind_ಖಂಡಗಳು_ಗಾತ್ರ++] = *ಖಂಡ_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_ಖಂಡ_alloc_stats2(fs, *ಖಂಡ_nr, -1);

	*ಖಂಡ_nr = 0;
	ಹಿಂತಿರುಗಿ ಖಂಡ_ಬದಲಾಯಿತು;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_bad_ಖಂಡ_proc(ext2_filsys fs, blk_t *ಖಂಡ_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_ಖಂಡ EXT2FS_ATTR((ಬಳಸದ)),
			      ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
			      ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ ಗಣ_badblock_ದಾಖಲೆ *rec = (ರಚನೆ ಗಣ_badblock_ದಾಖಲೆ *)
		priv_ದತ್ತಾಂಶ;
	errcode_t	retval;
	blk_t		blk;

	ಆದರೆ (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		ಆದರೆ (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		rec->bad_ಖಂಡ_ಎಣಿಕೆ++;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		ಆದರೆ (rec->ind_ಖಂಡಗಳು_ptr < rec->ind_ಖಂಡಗಳು_ಗಾತ್ರ) {
			blk = rec->ind_ಖಂಡಗಳು[rec->ind_ಖಂಡಗಳು_ptr++];
			ಆದರೆ (ext2fs_test_ಖಂಡ_bitmap2(fs->ಖಂಡ_map, blk))
				ಹೋಗು retry;
		} ಇಲ್ಲದಿದ್ದರೆ {
			retval = ext2fs_new_ಖಂಡ(fs, 0, 0, &blk);
			ಆದರೆ (retval) {
				rec->err = retval;
				ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->ಖಂಡ_buf);
		ಆದರೆ (retval) {
			rec->err = retval;
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_ಖಂಡ_alloc_stats2(fs, blk, +1);

	*ಖಂಡ_nr = blk;
	ಹಿಂತಿರುಗಿ ಖಂಡ_ಬದಲಾಯಿತು;
}






