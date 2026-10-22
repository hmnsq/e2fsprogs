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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना set_badblock_record {
	ext2_badblocks_iterate	bb_iter;
	पूर्णाङ्क		bad_block_गिनती;
	blk_t		*ind_blocks;
	पूर्णाङ्क		max_ind_blocks;
	पूर्णाङ्क		ind_blocks_size;
	पूर्णाङ्क		ind_blocks_ptr;
	वर्ण		*block_buf;
	errcode_t	err;
};

स्थिर पूर्णाङ्क set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block, पूर्णाङ्क ref_offset,
			      शून्य *priv_data);
स्थिर पूर्णाङ्क clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block, पूर्णाङ्क ref_offset,
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

	यदि (!fs->block_map)
		प्रत्यागम EXT2_ET_NO_BLOCK_BITMAP;

	स_रखो(&rec, 0, परिमाण(rec));
	rec.max_ind_blocks = 10;
	retval = ext2fs_get_array(rec.max_ind_blocks, परिमाण(blk_t),
				&rec.ind_blocks);
	यदि (retval)
		प्रत्यागम retval;
	स_रखो(rec.ind_blocks, 0, rec.max_ind_blocks * परिमाण(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.block_buf);
	यदि (retval)
		गच्छ cleanup;
	स_रखो(rec.block_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
				       BLOCK_FLAG_DEPTH_TRAVERSE, 0,
				       clear_bad_block_proc, &rec);
	यदि (retval)
		गच्छ cleanup;
	यदि (rec.err) {
		retval = rec.err;
		गच्छ cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	यदि (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		यदि (retval)
			गच्छ cleanup;
		retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
					       BLOCK_FLAG_APPEND, 0,
					       set_bad_block_proc, &rec);
		ext2fs_badblocks_list_iterate_end(rec.bb_iter);
		यदि (retval)
			गच्छ cleanup;
		यदि (rec.err) {
			retval = rec.err;
			गच्छ cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	यदि (retval)
		गच्छ cleanup;

	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	यदि (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_iblk_set(fs, &inode, rec.bad_block_गिनती);
	retval = ext2fs_inode_size_set(fs, &inode,
				       rec.bad_block_गिनती * fs->blocksize);
	यदि (retval)
		गच्छ cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	यदि (retval)
		गच्छ cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_blocks);
	ext2fs_free_mem(&rec.block_buf);
	प्रत्यागम retval;
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
स्थिर पूर्णाङ्क clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block EXT2FS_ATTR((unused)),
				पूर्णाङ्क ref_offset EXT2FS_ATTR((unused)),
				शून्य *priv_data)
{
	संरचना set_badblock_record *rec = (संरचना set_badblock_record *)
		priv_data;
	errcode_t	retval;
	अचिन्हित दीर्घ 	old_size;

	यदि (!*block_nr)
		प्रत्यागम 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	यदि (*block_nr >= ext2fs_blocks_गिनती(fs->super) ||
	    *block_nr < fs->super->s_first_data_block) {
		*block_nr = 0;
		प्रत्यागम BLOCK_CHANGED;
	}

	यदि (blockcnt < 0) {
		यदि (rec->ind_blocks_size >= rec->max_ind_blocks) {
			old_size = rec->max_ind_blocks * परिमाण(blk_t);
			rec->max_ind_blocks += 10;
			retval = ext2fs_resize_mem(old_size,
				   rec->max_ind_blocks * परिमाण(blk_t),
				   &rec->ind_blocks);
			यदि (retval) {
				rec->max_ind_blocks -= 10;
				rec->err = retval;
				प्रत्यागम BLOCK_ABORT;
			}
		}
		rec->ind_blocks[rec->ind_blocks_size++] = *block_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_block_alloc_stats2(fs, *block_nr, -1);

	*block_nr = 0;
	प्रत्यागम BLOCK_CHANGED;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
स्थिर पूर्णाङ्क set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block EXT2FS_ATTR((unused)),
			      पूर्णाङ्क ref_offset EXT2FS_ATTR((unused)),
			      शून्य *priv_data)
{
	संरचना set_badblock_record *rec = (संरचना set_badblock_record *)
		priv_data;
	errcode_t	retval;
	blk_t		blk;

	यदि (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		यदि (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			प्रत्यागम BLOCK_ABORT;
		rec->bad_block_गिनती++;
	} अन्यथा {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		यदि (rec->ind_blocks_ptr < rec->ind_blocks_size) {
			blk = rec->ind_blocks[rec->ind_blocks_ptr++];
			यदि (ext2fs_test_block_bitmap2(fs->block_map, blk))
				गच्छ retry;
		} अन्यथा {
			retval = ext2fs_new_block(fs, 0, 0, &blk);
			यदि (retval) {
				rec->err = retval;
				प्रत्यागम BLOCK_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->block_buf);
		यदि (retval) {
			rec->err = retval;
			प्रत्यागम BLOCK_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_block_alloc_stats2(fs, blk, +1);

	*block_nr = blk;
	प्रत्यागम BLOCK_CHANGED;
}






