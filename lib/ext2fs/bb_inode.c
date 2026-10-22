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
#د_که_پای
#ضمن <fcntl.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية سېټ_badblock_ریکارډ {
	ext2_badblocks_iterate	bb_iter;
	صحيح		bad_بلاک_شمېر;
	blk_t		*ind_بلاکونه;
	صحيح		max_ind_بلاکونه;
	صحيح		ind_بلاکونه_اندازه;
	صحيح		ind_بلاکونه_ptr;
	حرف		*بلاک_buf;
	errcode_t	err;
};

ساكن صحيح سېټ_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلاک, صحيح ref_دننه_واټن,
			      فراغ *priv_معلومات);
ساكن صحيح clear_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلاک, صحيح ref_دننه_واټن,
				فراغ *priv_معلومات);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	بنية سېټ_badblock_ریکارډ	rec;
	بنية ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!fs->بلاک_map)
		أرجع EXT2_ET_NO_بلاک_BITMAP;

	memset(&rec, 0, حجم(rec));
	rec.max_ind_بلاکونه = 10;
	retval = ext2fs_get_array(rec.max_ind_بلاکونه, حجم(blk_t),
				&rec.ind_بلاکونه);
	إذا (retval)
		أرجع retval;
	memset(rec.ind_بلاکونه, 0, rec.max_ind_بلاکونه * حجم(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.بلاک_buf);
	إذا (retval)
		اذهب cleanup;
	memset(rec.بلاک_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_بلاک_iterate2(fs, EXT2_BAD_INO,
				       بلاک_FLAG_ژوروالی_TRAVERSE, 0,
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
					       سېټ_bad_بلاک_proc, &rec);
		ext2fs_badblocks_list_iterate_پای_ټکی(rec.bb_iter);
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

	now = ext2fs_P_get_وخت(fs);
	ext2fs_inode_xtime_سېټ(&inode, i_atime, now);
	إذا (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_سېټ(&inode, i_ctime, now);
	ext2fs_inode_xtime_سېټ(&inode, i_mtime, now);
	ext2fs_iblk_سېټ(fs, &inode, rec.bad_بلاک_شمېر);
	retval = ext2fs_inode_اندازه_سېټ(fs, &inode,
				       rec.bad_بلاک_شمېر * fs->blocksize);
	إذا (retval)
		اذهب cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	إذا (retval)
		اذهب cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_بلاکونه);
	ext2fs_free_mem(&rec.بلاک_buf);
	أرجع retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#که_تعریف_شوی __TURBOC__
 #لارښوونه argsused
#د_که_پای
ساكن صحيح clear_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلاک EXT2FS_ATTR((ناکارول_شوی)),
				صحيح ref_دننه_واټن EXT2FS_ATTR((ناکارول_شوی)),
				فراغ *priv_معلومات)
{
	بنية سېټ_badblock_ریکارډ *rec = (بنية سېټ_badblock_ریکارډ *)
		priv_معلومات;
	errcode_t	retval;
	بینښه اوږد 	old_اندازه;

	إذا (!*بلاک_nr)
		أرجع 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	إذا (*بلاک_nr >= ext2fs_بلاکونه_شمېر(fs->super) ||
	    *بلاک_nr < fs->super->s_لومړی_معلومات_بلاک) {
		*بلاک_nr = 0;
		أرجع بلاک_بدل_شوی;
	}

	إذا (blockcnt < 0) {
		إذا (rec->ind_بلاکونه_اندازه >= rec->max_ind_بلاکونه) {
			old_اندازه = rec->max_ind_بلاکونه * حجم(blk_t);
			rec->max_ind_بلاکونه += 10;
			retval = ext2fs_resize_mem(old_اندازه,
				   rec->max_ind_بلاکونه * حجم(blk_t),
				   &rec->ind_بلاکونه);
			إذا (retval) {
				rec->max_ind_بلاکونه -= 10;
				rec->err = retval;
				أرجع بلاک_ABORT;
			}
		}
		rec->ind_بلاکونه[rec->ind_بلاکونه_اندازه++] = *بلاک_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_بلاک_alloc_stats2(fs, *بلاک_nr, -1);

	*بلاک_nr = 0;
	أرجع بلاک_بدل_شوی;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#که_تعریف_شوی __TURBOC__
 #لارښوونه argsused
#د_که_پای
ساكن صحيح سېټ_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلاک EXT2FS_ATTR((ناکارول_شوی)),
			      صحيح ref_دننه_واټن EXT2FS_ATTR((ناکارول_شوی)),
			      فراغ *priv_معلومات)
{
	بنية سېټ_badblock_ریکارډ *rec = (بنية سېټ_badblock_ریکارډ *)
		priv_معلومات;
	errcode_t	retval;
	blk_t		blk;

	إذا (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		إذا (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			أرجع بلاک_ABORT;
		rec->bad_بلاک_شمېر++;
	} وإلا {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		إذا (rec->ind_بلاکونه_ptr < rec->ind_بلاکونه_اندازه) {
			blk = rec->ind_بلاکونه[rec->ind_بلاکونه_ptr++];
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
	أرجع بلاک_بدل_شوی;
}






