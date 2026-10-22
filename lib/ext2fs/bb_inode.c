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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸಂರಚನಾ set_badblock_record {
	ext2_badblocks_iterate	bb_iter;
	ಪೂರ್ಣಾಙ್ಕ		bad_block_ಎಣಿಕೆ;
	blk_t		*ind_blocks;
	ಪೂರ್ಣಾಙ್ಕ		max_ind_blocks;
	ಪೂರ್ಣಾಙ್ಕ		ind_blocks_size;
	ಪೂರ್ಣಾಙ್ಕ		ind_blocks_ptr;
	ವರ್ಣ		*block_buf;
	errcode_t	err;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block, ಪೂರ್ಣಾಙ್ಕ ref_offset,
			      ಶೂನ್ಯ *priv_data);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block, ಪೂರ್ಣಾಙ್ಕ ref_offset,
				ಶೂನ್ಯ *priv_data);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	ಸಂರಚನಾ set_badblock_record	rec;
	ಸಂರಚನಾ ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (!fs->block_map)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_BLOCK_BITMAP;

	memset(&rec, 0, ಪರಿಮಾಣ(rec));
	rec.max_ind_blocks = 10;
	retval = ext2fs_get_array(rec.max_ind_blocks, ಪರಿಮಾಣ(blk_t),
				&rec.ind_blocks);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	memset(rec.ind_blocks, 0, rec.max_ind_blocks * ಪರಿಮಾಣ(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.block_buf);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	memset(rec.block_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
				       BLOCK_FLAG_DEPTH_TRAVERSE, 0,
				       clear_bad_block_proc, &rec);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;
	ಯದಿ (rec.err) {
		retval = rec.err;
		ಗಚ್ಛ cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	ಯದಿ (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
		retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
					       BLOCK_FLAG_APPEND, 0,
					       set_bad_block_proc, &rec);
		ext2fs_badblocks_list_iterate_end(rec.bb_iter);
		ಯದಿ (retval)
			ಗಚ್ಛ cleanup;
		ಯದಿ (rec.err) {
			retval = rec.err;
			ಗಚ್ಛ cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ಯದಿ (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_iblk_set(fs, &inode, rec.bad_block_ಎಣಿಕೆ);
	retval = ext2fs_inode_size_set(fs, &inode,
				       rec.bad_block_ಎಣಿಕೆ * fs->blocksize);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	ಯದಿ (retval)
		ಗಚ್ಛ cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_blocks);
	ext2fs_free_mem(&rec.block_buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block EXT2FS_ATTR((unused)),
				ಪೂರ್ಣಾಙ್ಕ ref_offset EXT2FS_ATTR((unused)),
				ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ set_badblock_record *rec = (ಸಂರಚನಾ set_badblock_record *)
		priv_data;
	errcode_t	retval;
	unsigned long 	old_size;

	ಯದಿ (!*block_nr)
		ಪ್ರತ್ಯಾಗಮ 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	ಯದಿ (*block_nr >= ext2fs_blocks_ಎಣಿಕೆ(fs->super) ||
	    *block_nr < fs->super->s_first_data_block) {
		*block_nr = 0;
		ಪ್ರತ್ಯಾಗಮ BLOCK_CHANGED;
	}

	ಯದಿ (blockcnt < 0) {
		ಯದಿ (rec->ind_blocks_size >= rec->max_ind_blocks) {
			old_size = rec->max_ind_blocks * ಪರಿಮಾಣ(blk_t);
			rec->max_ind_blocks += 10;
			retval = ext2fs_resize_mem(old_size,
				   rec->max_ind_blocks * ಪರಿಮಾಣ(blk_t),
				   &rec->ind_blocks);
			ಯದಿ (retval) {
				rec->max_ind_blocks -= 10;
				rec->err = retval;
				ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
			}
		}
		rec->ind_blocks[rec->ind_blocks_size++] = *block_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_block_alloc_stats2(fs, *block_nr, -1);

	*block_nr = 0;
	ಪ್ರತ್ಯಾಗಮ BLOCK_CHANGED;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block EXT2FS_ATTR((unused)),
			      ಪೂರ್ಣಾಙ್ಕ ref_offset EXT2FS_ATTR((unused)),
			      ಶೂನ್ಯ *priv_data)
{
	ಸಂರಚನಾ set_badblock_record *rec = (ಸಂರಚನಾ set_badblock_record *)
		priv_data;
	errcode_t	retval;
	blk_t		blk;

	ಯದಿ (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		ಯದಿ (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		rec->bad_block_ಎಣಿಕೆ++;
	} ಅನ್ಯಥಾ {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		ಯದಿ (rec->ind_blocks_ptr < rec->ind_blocks_size) {
			blk = rec->ind_blocks[rec->ind_blocks_ptr++];
			ಯದಿ (ext2fs_test_block_bitmap2(fs->block_map, blk))
				ಗಚ್ಛ retry;
		} ಅನ್ಯಥಾ {
			retval = ext2fs_new_block(fs, 0, 0, &blk);
			ಯದಿ (retval) {
				rec->err = retval;
				ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->block_buf);
		ಯದಿ (retval) {
			rec->err = retval;
			ಪ್ರತ್ಯಾಗಮ BLOCK_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_block_alloc_stats2(fs, blk, +1);

	*block_nr = blk;
	ಪ್ರತ್ಯಾಗಮ BLOCK_CHANGED;
}






