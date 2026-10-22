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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

கட்டமைப்பு set_badblock_record {
	ext2_badblocks_iterate	bb_iter;
	முழு		bad_block_மொத்தம்;
	blk_t		*ind_blocks;
	முழு		max_ind_blocks;
	முழு		ind_blocks_size;
	முழு		ind_blocks_ptr;
	எழுத்து		*block_buf;
	errcode_t	err;
};

நிலையான முழு set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block, முழு ref_offset,
			      வெறுமை *priv_data);
நிலையான முழு clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block, முழு ref_offset,
				வெறுமை *priv_data);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	கட்டமைப்பு set_badblock_record	rec;
	கட்டமைப்பு ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (!fs->block_map)
		திரும்பு EXT2_ET_NO_BLOCK_BITMAP;

	memset(&rec, 0, அளவு(rec));
	rec.max_ind_blocks = 10;
	retval = ext2fs_get_array(rec.max_ind_blocks, அளவு(blk_t),
				&rec.ind_blocks);
	எனில் (retval)
		திரும்பு retval;
	memset(rec.ind_blocks, 0, rec.max_ind_blocks * அளவு(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.block_buf);
	எனில் (retval)
		செல் cleanup;
	memset(rec.block_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
				       BLOCK_FLAG_DEPTH_TRAVERSE, 0,
				       clear_bad_block_proc, &rec);
	எனில் (retval)
		செல் cleanup;
	எனில் (rec.err) {
		retval = rec.err;
		செல் cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	எனில் (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		எனில் (retval)
			செல் cleanup;
		retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
					       BLOCK_FLAG_APPEND, 0,
					       set_bad_block_proc, &rec);
		ext2fs_badblocks_list_iterate_end(rec.bb_iter);
		எனில் (retval)
			செல் cleanup;
		எனில் (rec.err) {
			retval = rec.err;
			செல் cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	எனில் (retval)
		செல் cleanup;

	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	எனில் (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_iblk_set(fs, &inode, rec.bad_block_மொத்தம்);
	retval = ext2fs_inode_size_set(fs, &inode,
				       rec.bad_block_மொத்தம் * fs->blocksize);
	எனில் (retval)
		செல் cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	எனில் (retval)
		செல் cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_blocks);
	ext2fs_free_mem(&rec.block_buf);
	திரும்பு retval;
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
நிலையான முழு clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block EXT2FS_ATTR((unused)),
				முழு ref_offset EXT2FS_ATTR((unused)),
				வெறுமை *priv_data)
{
	கட்டமைப்பு set_badblock_record *rec = (கட்டமைப்பு set_badblock_record *)
		priv_data;
	errcode_t	retval;
	unsigned long 	old_size;

	எனில் (!*block_nr)
		திரும்பு 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	எனில் (*block_nr >= ext2fs_blocks_மொத்தம்(fs->super) ||
	    *block_nr < fs->super->s_first_data_block) {
		*block_nr = 0;
		திரும்பு BLOCK_CHANGED;
	}

	எனில் (blockcnt < 0) {
		எனில் (rec->ind_blocks_size >= rec->max_ind_blocks) {
			old_size = rec->max_ind_blocks * அளவு(blk_t);
			rec->max_ind_blocks += 10;
			retval = ext2fs_resize_mem(old_size,
				   rec->max_ind_blocks * அளவு(blk_t),
				   &rec->ind_blocks);
			எனில் (retval) {
				rec->max_ind_blocks -= 10;
				rec->err = retval;
				திரும்பு BLOCK_ABORT;
			}
		}
		rec->ind_blocks[rec->ind_blocks_size++] = *block_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_block_alloc_stats2(fs, *block_nr, -1);

	*block_nr = 0;
	திரும்பு BLOCK_CHANGED;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
நிலையான முழு set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block EXT2FS_ATTR((unused)),
			      முழு ref_offset EXT2FS_ATTR((unused)),
			      வெறுமை *priv_data)
{
	கட்டமைப்பு set_badblock_record *rec = (கட்டமைப்பு set_badblock_record *)
		priv_data;
	errcode_t	retval;
	blk_t		blk;

	எனில் (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		எனில் (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			திரும்பு BLOCK_ABORT;
		rec->bad_block_மொத்தம்++;
	} இல்லையெனில் {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		எனில் (rec->ind_blocks_ptr < rec->ind_blocks_size) {
			blk = rec->ind_blocks[rec->ind_blocks_ptr++];
			எனில் (ext2fs_test_block_bitmap2(fs->block_map, blk))
				செல் retry;
		} இல்லையெனில் {
			retval = ext2fs_new_block(fs, 0, 0, &blk);
			எனில் (retval) {
				rec->err = retval;
				திரும்பு BLOCK_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->block_buf);
		எனில் (retval) {
			rec->err = retval;
			திரும்பு BLOCK_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_block_alloc_stats2(fs, blk, +1);

	*block_nr = blk;
	திரும்பு BLOCK_CHANGED;
}






