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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସଂରଚନା set_badblock_record {
	ext2_badblocks_iterate	bb_iter;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		bad_block_ଗଣନା;
	blk_t		*ind_blocks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		max_ind_blocks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ind_blocks_size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ind_blocks_ptr;
	ଅକ୍ଷର		*block_buf;
	errcode_t	err;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset,
			      ଶୂନ୍ୟ *priv_data);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset,
				ଶୂନ୍ୟ *priv_data);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	ସଂରଚନା set_badblock_record	rec;
	ସଂରଚନା ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (!fs->block_map)
		ଫେରନ୍ତୁ EXT2_ET_NO_BLOCK_BITMAP;

	memset(&rec, 0, ଆକାର(rec));
	rec.max_ind_blocks = 10;
	retval = ext2fs_get_array(rec.max_ind_blocks, ଆକାର(blk_t),
				&rec.ind_blocks);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	memset(rec.ind_blocks, 0, rec.max_ind_blocks * ଆକାର(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.block_buf);
	ଯଦି (retval)
		ଯାଅ cleanup;
	memset(rec.block_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
				       BLOCK_FLAG_DEPTH_TRAVERSE, 0,
				       clear_bad_block_proc, &rec);
	ଯଦି (retval)
		ଯାଅ cleanup;
	ଯଦି (rec.err) {
		retval = rec.err;
		ଯାଅ cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	ଯଦି (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		ଯଦି (retval)
			ଯାଅ cleanup;
		retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
					       BLOCK_FLAG_APPEND, 0,
					       set_bad_block_proc, &rec);
		ext2fs_badblocks_list_iterate_end(rec.bb_iter);
		ଯଦି (retval)
			ଯାଅ cleanup;
		ଯଦି (rec.err) {
			retval = rec.err;
			ଯାଅ cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	ଯଦି (retval)
		ଯାଅ cleanup;

	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	ଯଦି (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_iblk_set(fs, &inode, rec.bad_block_ଗଣନା);
	retval = ext2fs_inode_size_set(fs, &inode,
				       rec.bad_block_ଗଣନା * fs->blocksize);
	ଯଦି (retval)
		ଯାଅ cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	ଯଦି (retval)
		ଯାଅ cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_blocks);
	ext2fs_free_mem(&rec.block_buf);
	ଫେରନ୍ତୁ retval;
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block EXT2FS_ATTR((unused)),
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset EXT2FS_ATTR((unused)),
				ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା set_badblock_record *rec = (ସଂରଚନା set_badblock_record *)
		priv_data;
	errcode_t	retval;
	unsigned long 	old_size;

	ଯଦି (!*block_nr)
		ଫେରନ୍ତୁ 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	ଯଦି (*block_nr >= ext2fs_blocks_ଗଣନା(fs->super) ||
	    *block_nr < fs->super->s_first_data_block) {
		*block_nr = 0;
		ଫେରନ୍ତୁ BLOCK_CHANGED;
	}

	ଯଦି (blockcnt < 0) {
		ଯଦି (rec->ind_blocks_size >= rec->max_ind_blocks) {
			old_size = rec->max_ind_blocks * ଆକାର(blk_t);
			rec->max_ind_blocks += 10;
			retval = ext2fs_resize_mem(old_size,
				   rec->max_ind_blocks * ଆକାର(blk_t),
				   &rec->ind_blocks);
			ଯଦି (retval) {
				rec->max_ind_blocks -= 10;
				rec->err = retval;
				ଫେରନ୍ତୁ BLOCK_ABORT;
			}
		}
		rec->ind_blocks[rec->ind_blocks_size++] = *block_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_block_alloc_stats2(fs, *block_nr, -1);

	*block_nr = 0;
	ଫେରନ୍ତୁ BLOCK_CHANGED;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block EXT2FS_ATTR((unused)),
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset EXT2FS_ATTR((unused)),
			      ଶୂନ୍ୟ *priv_data)
{
	ସଂରଚନା set_badblock_record *rec = (ସଂରଚନା set_badblock_record *)
		priv_data;
	errcode_t	retval;
	blk_t		blk;

	ଯଦି (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		ଯଦି (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			ଫେରନ୍ତୁ BLOCK_ABORT;
		rec->bad_block_ଗଣନା++;
	} ଅନ୍ୟଥା {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		ଯଦି (rec->ind_blocks_ptr < rec->ind_blocks_size) {
			blk = rec->ind_blocks[rec->ind_blocks_ptr++];
			ଯଦି (ext2fs_test_block_bitmap2(fs->block_map, blk))
				ଯାଅ retry;
		} ଅନ୍ୟଥା {
			retval = ext2fs_new_block(fs, 0, 0, &blk);
			ଯଦି (retval) {
				rec->err = retval;
				ଫେରନ୍ତୁ BLOCK_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->block_buf);
		ଯଦି (retval) {
			rec->err = retval;
			ଫେରନ୍ତୁ BLOCK_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_block_alloc_stats2(fs, blk, +1);

	*block_nr = blk;
	ଫେରନ୍ତୁ BLOCK_CHANGED;
}






