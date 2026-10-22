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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना set_badblock_record {
	ext2_badblocks_iterate	bb_iter;
	पूर्णांक		bad_block_मोजणी;
	blk_t		*ind_blocks;
	पूर्णांक		max_ind_blocks;
	पूर्णांक		ind_blocks_size;
	पूर्णांक		ind_blocks_ptr;
	अक्षर		*block_buf;
	errcode_t	err;
};

स्थिर पूर्णांक set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block, पूर्णांक ref_offset,
			      शून्य *priv_data);
स्थिर पूर्णांक clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block, पूर्णांक ref_offset,
				शून्य *priv_data);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	संरचना set_badblock_record	rec;
	संरचना ext2_inode		inode;
	समय_प्रकार				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर (!fs->block_map)
		परत EXT2_ET_NO_BLOCK_BITMAP;

	स_रखो(&rec, 0, आकार(rec));
	rec.max_ind_blocks = 10;
	retval = ext2fs_get_array(rec.max_ind_blocks, आकार(blk_t),
				&rec.ind_blocks);
	जर (retval)
		परत retval;
	स_रखो(rec.ind_blocks, 0, rec.max_ind_blocks * आकार(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.block_buf);
	जर (retval)
		जा cleanup;
	स_रखो(rec.block_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
				       BLOCK_FLAG_DEPTH_TRAVERSE, 0,
				       clear_bad_block_proc, &rec);
	जर (retval)
		जा cleanup;
	जर (rec.err) {
		retval = rec.err;
		जा cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	जर (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		जर (retval)
			जा cleanup;
		retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
					       BLOCK_FLAG_APPEND, 0,
					       set_bad_block_proc, &rec);
		ext2fs_badblocks_list_iterate_end(rec.bb_iter);
		जर (retval)
			जा cleanup;
		जर (rec.err) {
			retval = rec.err;
			जा cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	जर (retval)
		जा cleanup;

	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	जर (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_iblk_set(fs, &inode, rec.bad_block_मोजणी);
	retval = ext2fs_inode_size_set(fs, &inode,
				       rec.bad_block_मोजणी * fs->blocksize);
	जर (retval)
		जा cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	जर (retval)
		जा cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_blocks);
	ext2fs_free_mem(&rec.block_buf);
	परत retval;
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
स्थिर पूर्णांक clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block EXT2FS_ATTR((unused)),
				पूर्णांक ref_offset EXT2FS_ATTR((unused)),
				शून्य *priv_data)
{
	संरचना set_badblock_record *rec = (संरचना set_badblock_record *)
		priv_data;
	errcode_t	retval;
	अचिन्हित दीर्घ 	old_size;

	जर (!*block_nr)
		परत 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	जर (*block_nr >= ext2fs_blocks_मोजणी(fs->super) ||
	    *block_nr < fs->super->s_first_data_block) {
		*block_nr = 0;
		परत BLOCK_CHANGED;
	}

	जर (blockcnt < 0) {
		जर (rec->ind_blocks_size >= rec->max_ind_blocks) {
			old_size = rec->max_ind_blocks * आकार(blk_t);
			rec->max_ind_blocks += 10;
			retval = ext2fs_resize_mem(old_size,
				   rec->max_ind_blocks * आकार(blk_t),
				   &rec->ind_blocks);
			जर (retval) {
				rec->max_ind_blocks -= 10;
				rec->err = retval;
				परत BLOCK_ABORT;
			}
		}
		rec->ind_blocks[rec->ind_blocks_size++] = *block_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_block_alloc_stats2(fs, *block_nr, -1);

	*block_nr = 0;
	परत BLOCK_CHANGED;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
स्थिर पूर्णांक set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block EXT2FS_ATTR((unused)),
			      पूर्णांक ref_offset EXT2FS_ATTR((unused)),
			      शून्य *priv_data)
{
	संरचना set_badblock_record *rec = (संरचना set_badblock_record *)
		priv_data;
	errcode_t	retval;
	blk_t		blk;

	जर (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		जर (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			परत BLOCK_ABORT;
		rec->bad_block_मोजणी++;
	} अन्यथा {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		जर (rec->ind_blocks_ptr < rec->ind_blocks_size) {
			blk = rec->ind_blocks[rec->ind_blocks_ptr++];
			जर (ext2fs_test_block_bitmap2(fs->block_map, blk))
				जा retry;
		} अन्यथा {
			retval = ext2fs_new_block(fs, 0, 0, &blk);
			जर (retval) {
				rec->err = retval;
				परत BLOCK_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->block_buf);
		जर (retval) {
			rec->err = retval;
			परत BLOCK_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_block_alloc_stats2(fs, blk, +1);

	*block_nr = blk;
	परत BLOCK_CHANGED;
}






