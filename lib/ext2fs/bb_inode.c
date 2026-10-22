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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#ضمن <fcntl.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية سیٹ_badblock_ریکارڈ {
	ext2_badblocks_iterate	bb_iter;
	صحيح		bad_بلاک_گنتی;
	blk_t		*ind_بلاکاں;
	صحيح		max_ind_بلاکاں;
	صحيح		ind_بلاکاں_ناپ;
	صحيح		ind_بلاکاں_ptr;
	حرف		*بلاک_buf;
	errcode_t	err;
};

ساكن صحيح سیٹ_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلاک, صحيح ref_وتھ,
			      فراغ *priv_انکڑے);
ساكن صحيح clear_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلاک, صحيح ref_وتھ,
				فراغ *priv_انکڑے);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	بنية سیٹ_badblock_ریکارڈ	rec;
	بنية ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!fs->بلاک_map)
		أرجع EXT2_ET_NO_بلاک_BITMAP;

	memset(&rec, 0, حجم(rec));
	rec.max_ind_بلاکاں = 10;
	retval = ext2fs_get_array(rec.max_ind_بلاکاں, حجم(blk_t),
				&rec.ind_بلاکاں);
	إذا (retval)
		أرجع retval;
	memset(rec.ind_بلاکاں, 0, rec.max_ind_بلاکاں * حجم(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.بلاک_buf);
	إذا (retval)
		اذهب cleanup;
	memset(rec.بلاک_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_بلاک_iterate2(fs, EXT2_BAD_INO,
				       بلاک_FLAG_ڈونگھائی_TRAVERSE, 0,
				       clear_bad_بلاک_proc, &rec);
	إذا (retval)
		اذهب cleanup;
	إذا (rec.err) {
		retval = rec.err;
		اذهب cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	إذا (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		إذا (retval)
			اذهب cleanup;
		retval = ext2fs_بلاک_iterate2(fs, EXT2_BAD_INO,
					       بلاک_FLAG_APPEND, 0,
					       سیٹ_bad_بلاک_proc, &rec);
		ext2fs_badblocks_list_iterate_سرا(rec.bb_iter);
		إذا (retval)
			اذهب cleanup;
		إذا (rec.err) {
			retval = rec.err;
			اذهب cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	إذا (retval)
		اذهب cleanup;

	now = ext2fs_P_get_ویلا(fs);
	ext2fs_inode_xtime_سیٹ(&inode, i_atime, now);
	إذا (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_سیٹ(&inode, i_ctime, now);
	ext2fs_inode_xtime_سیٹ(&inode, i_mtime, now);
	ext2fs_iblk_سیٹ(fs, &inode, rec.bad_بلاک_گنتی);
	retval = ext2fs_inode_ناپ_سیٹ(fs, &inode,
				       rec.bad_بلاک_گنتی * fs->blocksize);
	إذا (retval)
		اذهب cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	إذا (retval)
		اذهب cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_بلاکاں);
	ext2fs_free_mem(&rec.بلاک_buf);
	أرجع retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#جے_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#جے_انت
ساكن صحيح clear_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلاک EXT2FS_ATTR((بن_ورتیا)),
				صحيح ref_وتھ EXT2FS_ATTR((بن_ورتیا)),
				فراغ *priv_انکڑے)
{
	بنية سیٹ_badblock_ریکارڈ *rec = (بنية سیٹ_badblock_ریکارڈ *)
		priv_انکڑے;
	errcode_t	retval;
	بنانشانی لما 	old_ناپ;

	إذا (!*بلاک_nr)
		أرجع 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	إذا (*بلاک_nr >= ext2fs_بلاکاں_گنتی(fs->super) ||
	    *بلاک_nr < fs->super->s_پہلا_انکڑے_بلاک) {
		*بلاک_nr = 0;
		أرجع بلاک_بدلیا;
	}

	إذا (blockcnt < 0) {
		إذا (rec->ind_بلاکاں_ناپ >= rec->max_ind_بلاکاں) {
			old_ناپ = rec->max_ind_بلاکاں * حجم(blk_t);
			rec->max_ind_بلاکاں += 10;
			retval = ext2fs_resize_mem(old_ناپ,
				   rec->max_ind_بلاکاں * حجم(blk_t),
				   &rec->ind_بلاکاں);
			إذا (retval) {
				rec->max_ind_بلاکاں -= 10;
				rec->err = retval;
				أرجع بلاک_ABORT;
			}
		}
		rec->ind_بلاکاں[rec->ind_بلاکاں_ناپ++] = *بلاک_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_بلاک_alloc_stats2(fs, *بلاک_nr, -1);

	*بلاک_nr = 0;
	أرجع بلاک_بدلیا;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#جے_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#جے_انت
ساكن صحيح سیٹ_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلاک EXT2FS_ATTR((بن_ورتیا)),
			      صحيح ref_وتھ EXT2FS_ATTR((بن_ورتیا)),
			      فراغ *priv_انکڑے)
{
	بنية سیٹ_badblock_ریکارڈ *rec = (بنية سیٹ_badblock_ریکارڈ *)
		priv_انکڑے;
	errcode_t	retval;
	blk_t		blk;

	إذا (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		إذا (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			أرجع بلاک_ABORT;
		rec->bad_بلاک_گنتی++;
	} وإلا {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		إذا (rec->ind_بلاکاں_ptr < rec->ind_بلاکاں_ناپ) {
			blk = rec->ind_بلاکاں[rec->ind_بلاکاں_ptr++];
			إذا (ext2fs_test_بلاک_bitmap2(fs->بلاک_map, blk))
				اذهب retry;
		} وإلا {
			retval = ext2fs_new_بلاک(fs, 0, 0, &blk);
			إذا (retval) {
				rec->err = retval;
				أرجع بلاک_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->بلاک_buf);
		إذا (retval) {
			rec->err = retval;
			أرجع بلاک_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_بلاک_alloc_stats2(fs, blk, +1);

	*بلاک_nr = blk;
	أرجع بلاک_بدلیا;
}






