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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

నిర్మాణం సమితి_badblock_రికార్డు {
	ext2_badblocks_iterate	bb_iter;
	పూర్ణాంకం		bad_ఖండం_లెక్క;
	blk_t		*ind_ఖండాలు;
	పూర్ణాంకం		max_ind_ఖండాలు;
	పూర్ణాంకం		ind_ఖండాలు_కొలత;
	పూర్ణాంకం		ind_ఖండాలు_ptr;
	అక్షరం		*ఖండం_buf;
	errcode_t	err;
};

స్థిర పూర్ణాంకం సమితి_bad_ఖండం_proc(ext2_filsys fs, blk_t *ఖండం_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_ఖండం, పూర్ణాంకం ref_స్థానభ్రంశం,
			      శూన్య *priv_దత్తాంశం);
స్థిర పూర్ణాంకం clear_bad_ఖండం_proc(ext2_filsys fs, blk_t *ఖండం_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_ఖండం, పూర్ణాంకం ref_స్థానభ్రంశం,
				శూన్య *priv_దత్తాంశం);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	నిర్మాణం సమితి_badblock_రికార్డు	rec;
	నిర్మాణం ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!fs->ఖండం_map)
		తిరిగి EXT2_ET_NO_ఖండం_BITMAP;

	memset(&rec, 0, పరిమాణం(rec));
	rec.max_ind_ఖండాలు = 10;
	retval = ext2fs_get_array(rec.max_ind_ఖండాలు, పరిమాణం(blk_t),
				&rec.ind_ఖండాలు);
	ఒకవేళ (retval)
		తిరిగి retval;
	memset(rec.ind_ఖండాలు, 0, rec.max_ind_ఖండాలు * పరిమాణం(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.ఖండం_buf);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	memset(rec.ఖండం_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_ఖండం_iterate2(fs, EXT2_BAD_INO,
				       ఖండం_FLAG_లోతు_TRAVERSE, 0,
				       clear_bad_ఖండం_proc, &rec);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	ఒకవేళ (rec.err) {
		retval = rec.err;
		వెళ్ళు cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	ఒకవేళ (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
		retval = ext2fs_ఖండం_iterate2(fs, EXT2_BAD_INO,
					       ఖండం_FLAG_APPEND, 0,
					       సమితి_bad_ఖండం_proc, &rec);
		ext2fs_badblocks_list_iterate_చివర(rec.bb_iter);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
		ఒకవేళ (rec.err) {
			retval = rec.err;
			వెళ్ళు cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	now = ext2fs_P_get_సమయం(fs);
	ext2fs_inode_xtime_సమితి(&inode, i_atime, now);
	ఒకవేళ (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_సమితి(&inode, i_ctime, now);
	ext2fs_inode_xtime_సమితి(&inode, i_mtime, now);
	ext2fs_iblk_సమితి(fs, &inode, rec.bad_ఖండం_లెక్క);
	retval = ext2fs_inode_కొలత_సమితి(fs, &inode,
				       rec.bad_ఖండం_లెక్క * fs->blocksize);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_ఖండాలు);
	ext2fs_free_mem(&rec.ఖండం_buf);
	తిరిగి retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
స్థిర పూర్ణాంకం clear_bad_ఖండం_proc(ext2_filsys fs, blk_t *ఖండం_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_ఖండం EXT2FS_ATTR((వాడని)),
				పూర్ణాంకం ref_స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
				శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం సమితి_badblock_రికార్డు *rec = (నిర్మాణం సమితి_badblock_రికార్డు *)
		priv_దత్తాంశం;
	errcode_t	retval;
	గుర్తులేని దీర్ఘ 	old_కొలత;

	ఒకవేళ (!*ఖండం_nr)
		తిరిగి 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	ఒకవేళ (*ఖండం_nr >= ext2fs_ఖండాలు_లెక్క(fs->super) ||
	    *ఖండం_nr < fs->super->s_మొదటి_దత్తాంశం_ఖండం) {
		*ఖండం_nr = 0;
		తిరిగి ఖండం_మారింది;
	}

	ఒకవేళ (blockcnt < 0) {
		ఒకవేళ (rec->ind_ఖండాలు_కొలత >= rec->max_ind_ఖండాలు) {
			old_కొలత = rec->max_ind_ఖండాలు * పరిమాణం(blk_t);
			rec->max_ind_ఖండాలు += 10;
			retval = ext2fs_resize_mem(old_కొలత,
				   rec->max_ind_ఖండాలు * పరిమాణం(blk_t),
				   &rec->ind_ఖండాలు);
			ఒకవేళ (retval) {
				rec->max_ind_ఖండాలు -= 10;
				rec->err = retval;
				తిరిగి ఖండం_ABORT;
			}
		}
		rec->ind_ఖండాలు[rec->ind_ఖండాలు_కొలత++] = *ఖండం_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_ఖండం_alloc_stats2(fs, *ఖండం_nr, -1);

	*ఖండం_nr = 0;
	తిరిగి ఖండం_మారింది;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
స్థిర పూర్ణాంకం సమితి_bad_ఖండం_proc(ext2_filsys fs, blk_t *ఖండం_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_ఖండం EXT2FS_ATTR((వాడని)),
			      పూర్ణాంకం ref_స్థానభ్రంశం EXT2FS_ATTR((వాడని)),
			      శూన్య *priv_దత్తాంశం)
{
	నిర్మాణం సమితి_badblock_రికార్డు *rec = (నిర్మాణం సమితి_badblock_రికార్డు *)
		priv_దత్తాంశం;
	errcode_t	retval;
	blk_t		blk;

	ఒకవేళ (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		ఒకవేళ (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			తిరిగి ఖండం_ABORT;
		rec->bad_ఖండం_లెక్క++;
	} లేకపోతే {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		ఒకవేళ (rec->ind_ఖండాలు_ptr < rec->ind_ఖండాలు_కొలత) {
			blk = rec->ind_ఖండాలు[rec->ind_ఖండాలు_ptr++];
			ఒకవేళ (ext2fs_test_ఖండం_bitmap2(fs->ఖండం_map, blk))
				వెళ్ళు retry;
		} లేకపోతే {
			retval = ext2fs_new_ఖండం(fs, 0, 0, &blk);
			ఒకవేళ (retval) {
				rec->err = retval;
				తిరిగి ఖండం_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->ఖండం_buf);
		ఒకవేళ (retval) {
			rec->err = retval;
			తిరిగి ఖండం_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_ఖండం_alloc_stats2(fs, blk, +1);

	*ఖండం_nr = blk;
	తిరిగి ఖండం_మారింది;
}






