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
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना समुच्चयः_badblock_लेखः {
	ext2_badblocks_iterate	bb_iter;
	पूर्णाङ्क		bad_खण्डम्_गणनम्;
	blk_t		*ind_खण्डानि;
	पूर्णाङ्क		max_ind_खण्डानि;
	पूर्णाङ्क		ind_खण्डानि_आकार;
	पूर्णाङ्क		ind_खण्डानि_ptr;
	वर्ण		*खण्डम्_buf;
	errcode_t	err;
};

स्थिर पूर्णाङ्क समुच्चयः_bad_खण्डम्_proc(ext2_filsys fs, blk_t *खण्डम्_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_खण्डम्, पूर्णाङ्क ref_विस्थापनम्,
			      शून्य *priv_दत्त);
स्थिर पूर्णाङ्क clear_bad_खण्डम्_proc(ext2_filsys fs, blk_t *खण्डम्_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_खण्डम्, पूर्णाङ्क ref_विस्थापनम्,
				शून्य *priv_दत्त);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	संरचना समुच्चयः_badblock_लेखः	rec;
	संरचना ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!fs->खण्डम्_map)
		प्रत्यागम EXT2_ET_NO_खण्डम्_BITMAP;

	स्मृतिं_पूरय(&rec, 0, परिमाण(rec));
	rec.max_ind_खण्डानि = 10;
	retval = ext2fs_get_array(rec.max_ind_खण्डानि, परिमाण(blk_t),
				&rec.ind_खण्डानि);
	यदि (retval)
		प्रत्यागम retval;
	स्मृतिं_पूरय(rec.ind_खण्डानि, 0, rec.max_ind_खण्डानि * परिमाण(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.खण्डम्_buf);
	यदि (retval)
		गच्छ cleanup;
	स्मृतिं_पूरय(rec.खण्डम्_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_खण्डम्_iterate2(fs, EXT2_BAD_INO,
				       खण्डम्_FLAG_गभीरता_TRAVERSE, 0,
				       clear_bad_खण्डम्_proc, &rec);
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
		retval = ext2fs_खण्डम्_iterate2(fs, EXT2_BAD_INO,
					       खण्डम्_FLAG_APPEND, 0,
					       समुच्चयः_bad_खण्डम्_proc, &rec);
		ext2fs_badblocks_list_iterate_अन्तः(rec.bb_iter);
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
	ext2fs_inode_xtime_समुच्चयः(&inode, i_atime, now);
	यदि (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_समुच्चयः(&inode, i_ctime, now);
	ext2fs_inode_xtime_समुच्चयः(&inode, i_mtime, now);
	ext2fs_iblk_समुच्चयः(fs, &inode, rec.bad_खण्डम्_गणनम्);
	retval = ext2fs_inode_आकार_समुच्चयः(fs, &inode,
				       rec.bad_खण्डम्_गणनम् * fs->blocksize);
	यदि (retval)
		गच्छ cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	यदि (retval)
		गच्छ cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_खण्डानि);
	ext2fs_free_mem(&rec.खण्डम्_buf);
	प्रत्यागम retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
स्थिर पूर्णाङ्क clear_bad_खण्डम्_proc(ext2_filsys fs, blk_t *खण्डम्_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_खण्डम् EXT2FS_ATTR((अप्रयुक्तम्)),
				पूर्णाङ्क ref_विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
				शून्य *priv_दत्त)
{
	संरचना समुच्चयः_badblock_लेखः *rec = (संरचना समुच्चयः_badblock_लेखः *)
		priv_दत्त;
	errcode_t	retval;
	अचिह्नित दीर्घ 	old_आकार;

	यदि (!*खण्डम्_nr)
		प्रत्यागम 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	यदि (*खण्डम्_nr >= ext2fs_खण्डानि_गणनम्(fs->super) ||
	    *खण्डम्_nr < fs->super->s_first_दत्त_खण्डम्) {
		*खण्डम्_nr = 0;
		प्रत्यागम खण्डम्_परिवर्तितम्;
	}

	यदि (blockcnt < 0) {
		यदि (rec->ind_खण्डानि_आकार >= rec->max_ind_खण्डानि) {
			old_आकार = rec->max_ind_खण्डानि * परिमाण(blk_t);
			rec->max_ind_खण्डानि += 10;
			retval = ext2fs_resize_mem(old_आकार,
				   rec->max_ind_खण्डानि * परिमाण(blk_t),
				   &rec->ind_खण्डानि);
			यदि (retval) {
				rec->max_ind_खण्डानि -= 10;
				rec->err = retval;
				प्रत्यागम खण्डम्_ABORT;
			}
		}
		rec->ind_खण्डानि[rec->ind_खण्डानि_आकार++] = *खण्डम्_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_खण्डम्_alloc_stats2(fs, *खण्डम्_nr, -1);

	*खण्डम्_nr = 0;
	प्रत्यागम खण्डम्_परिवर्तितम्;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
स्थिर पूर्णाङ्क समुच्चयः_bad_खण्डम्_proc(ext2_filsys fs, blk_t *खण्डम्_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_खण्डम् EXT2FS_ATTR((अप्रयुक्तम्)),
			      पूर्णाङ्क ref_विस्थापनम् EXT2FS_ATTR((अप्रयुक्तम्)),
			      शून्य *priv_दत्त)
{
	संरचना समुच्चयः_badblock_लेखः *rec = (संरचना समुच्चयः_badblock_लेखः *)
		priv_दत्त;
	errcode_t	retval;
	blk_t		blk;

	यदि (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		यदि (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			प्रत्यागम खण्डम्_ABORT;
		rec->bad_खण्डम्_गणनम्++;
	} अन्यथा {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		यदि (rec->ind_खण्डानि_ptr < rec->ind_खण्डानि_आकार) {
			blk = rec->ind_खण्डानि[rec->ind_खण्डानि_ptr++];
			यदि (ext2fs_test_खण्डम्_bitmap2(fs->खण्डम्_map, blk))
				गच्छ retry;
		} अन्यथा {
			retval = ext2fs_new_खण्डम्(fs, 0, 0, &blk);
			यदि (retval) {
				rec->err = retval;
				प्रत्यागम खण्डम्_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->खण्डम्_buf);
		यदि (retval) {
			rec->err = retval;
			प्रत्यागम खण्डम्_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_खण्डम्_alloc_stats2(fs, blk, +1);

	*खण्डम्_nr = blk;
	प्रत्यागम खण्डम्_परिवर्तितम्;
}






