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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"

সংরচনা set_badblock_record {
	ext2_badblocks_iterate	bb_iter;
	পূর্ণাঙ্ক		bad_block_গুনতি;
	blk_t		*ind_blocks;
	পূর্ণাঙ্ক		max_ind_blocks;
	পূর্ণাঙ্ক		ind_blocks_size;
	পূর্ণাঙ্ক		ind_blocks_ptr;
	বর্ণ		*block_buf;
	errcode_t	err;
};

স্থির পূর্ণাঙ্ক set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block, পূর্ণাঙ্ক ref_offset,
			      শূন্য *priv_data);
স্থির পূর্ণাঙ্ক clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block, পূর্ণাঙ্ক ref_offset,
				শূন্য *priv_data);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	সংরচনা set_badblock_record	rec;
	সংরচনা ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!fs->block_map)
		প্রত্যাগম EXT2_ET_NO_BLOCK_BITMAP;

	memset(&rec, 0, পরিমাণ(rec));
	rec.max_ind_blocks = 10;
	retval = ext2fs_get_array(rec.max_ind_blocks, পরিমাণ(blk_t),
				&rec.ind_blocks);
	যদি (retval)
		প্রত্যাগম retval;
	memset(rec.ind_blocks, 0, rec.max_ind_blocks * পরিমাণ(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.block_buf);
	যদি (retval)
		গচ্ছ cleanup;
	memset(rec.block_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
				       BLOCK_FLAG_DEPTH_TRAVERSE, 0,
				       clear_bad_block_proc, &rec);
	যদি (retval)
		গচ্ছ cleanup;
	যদি (rec.err) {
		retval = rec.err;
		গচ্ছ cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	যদি (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		যদি (retval)
			গচ্ছ cleanup;
		retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
					       BLOCK_FLAG_APPEND, 0,
					       set_bad_block_proc, &rec);
		ext2fs_badblocks_list_iterate_end(rec.bb_iter);
		যদি (retval)
			গচ্ছ cleanup;
		যদি (rec.err) {
			retval = rec.err;
			গচ্ছ cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	যদি (retval)
		গচ্ছ cleanup;

	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	যদি (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_iblk_set(fs, &inode, rec.bad_block_গুনতি);
	retval = ext2fs_inode_size_set(fs, &inode,
				       rec.bad_block_গুনতি * fs->blocksize);
	যদি (retval)
		গচ্ছ cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	যদি (retval)
		গচ্ছ cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_blocks);
	ext2fs_free_mem(&rec.block_buf);
	প্রত্যাগম retval;
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
স্থির পূর্ণাঙ্ক clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block EXT2FS_ATTR((unused)),
				পূর্ণাঙ্ক ref_offset EXT2FS_ATTR((unused)),
				শূন্য *priv_data)
{
	সংরচনা set_badblock_record *rec = (সংরচনা set_badblock_record *)
		priv_data;
	errcode_t	retval;
	unsigned long 	old_size;

	যদি (!*block_nr)
		প্রত্যাগম 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	যদি (*block_nr >= ext2fs_blocks_গুনতি(fs->super) ||
	    *block_nr < fs->super->s_first_data_block) {
		*block_nr = 0;
		প্রত্যাগম BLOCK_CHANGED;
	}

	যদি (blockcnt < 0) {
		যদি (rec->ind_blocks_size >= rec->max_ind_blocks) {
			old_size = rec->max_ind_blocks * পরিমাণ(blk_t);
			rec->max_ind_blocks += 10;
			retval = ext2fs_resize_mem(old_size,
				   rec->max_ind_blocks * পরিমাণ(blk_t),
				   &rec->ind_blocks);
			যদি (retval) {
				rec->max_ind_blocks -= 10;
				rec->err = retval;
				প্রত্যাগম BLOCK_ABORT;
			}
		}
		rec->ind_blocks[rec->ind_blocks_size++] = *block_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_block_alloc_stats2(fs, *block_nr, -1);

	*block_nr = 0;
	প্রত্যাগম BLOCK_CHANGED;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
স্থির পূর্ণাঙ্ক set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block EXT2FS_ATTR((unused)),
			      পূর্ণাঙ্ক ref_offset EXT2FS_ATTR((unused)),
			      শূন্য *priv_data)
{
	সংরচনা set_badblock_record *rec = (সংরচনা set_badblock_record *)
		priv_data;
	errcode_t	retval;
	blk_t		blk;

	যদি (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		যদি (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			প্রত্যাগম BLOCK_ABORT;
		rec->bad_block_গুনতি++;
	} অন্যথা {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		যদি (rec->ind_blocks_ptr < rec->ind_blocks_size) {
			blk = rec->ind_blocks[rec->ind_blocks_ptr++];
			যদি (ext2fs_test_block_bitmap2(fs->block_map, blk))
				গচ্ছ retry;
		} অন্যথা {
			retval = ext2fs_new_block(fs, 0, 0, &blk);
			যদি (retval) {
				rec->err = retval;
				প্রত্যাগম BLOCK_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->block_buf);
		যদি (retval) {
			rec->err = retval;
			প্রত্যাগম BLOCK_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_block_alloc_stats2(fs, blk, +1);

	*block_nr = blk;
	প্রত্যাগম BLOCK_CHANGED;
}






