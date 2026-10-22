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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية سيٽ_badblock_رڪارڊ {
	ext2_badblocks_iterate	bb_iter;
	صحيح		bad_بلاڪ_ڳڻپ;
	blk_t		*ind_بلاڪون;
	صحيح		max_ind_بلاڪون;
	صحيح		ind_بلاڪون_سائيز;
	صحيح		ind_بلاڪون_ptr;
	حرف		*بلاڪ_buf;
	errcode_t	err;
};

ساكن صحيح سيٽ_bad_بلاڪ_proc(ext2_filsys fs, blk_t *بلاڪ_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلاڪ, صحيح ref_وٿي,
			      فراغ *priv_معلومات);
ساكن صحيح clear_bad_بلاڪ_proc(ext2_filsys fs, blk_t *بلاڪ_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلاڪ, صحيح ref_وٿي,
				فراغ *priv_معلومات);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	بنية سيٽ_badblock_رڪارڊ	rec;
	بنية ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!fs->بلاڪ_map)
		أرجع EXT2_ET_NO_بلاڪ_BITMAP;

	memset(&rec, 0, حجم(rec));
	rec.max_ind_بلاڪون = 10;
	retval = ext2fs_get_array(rec.max_ind_بلاڪون, حجم(blk_t),
				&rec.ind_بلاڪون);
	إذا (retval)
		أرجع retval;
	memset(rec.ind_بلاڪون, 0, rec.max_ind_بلاڪون * حجم(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.بلاڪ_buf);
	إذا (retval)
		اذهب cleanup;
	memset(rec.بلاڪ_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_بلاڪ_iterate2(fs, EXT2_BAD_INO,
				       بلاڪ_FLAG_اونهائي_TRAVERSE, 0,
				       clear_bad_بلاڪ_proc, &rec);
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
		retval = ext2fs_بلاڪ_iterate2(fs, EXT2_BAD_INO,
					       بلاڪ_FLAG_APPEND, 0,
					       سيٽ_bad_بلاڪ_proc, &rec);
		ext2fs_badblocks_list_iterate_پڇاڙي(rec.bb_iter);
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

	now = ext2fs_P_get_وقت(fs);
	ext2fs_inode_xtime_سيٽ(&inode, i_atime, now);
	إذا (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_سيٽ(&inode, i_ctime, now);
	ext2fs_inode_xtime_سيٽ(&inode, i_mtime, now);
	ext2fs_iblk_سيٽ(fs, &inode, rec.bad_بلاڪ_ڳڻپ);
	retval = ext2fs_inode_سائيز_سيٽ(fs, &inode,
				       rec.bad_بلاڪ_ڳڻپ * fs->blocksize);
	إذا (retval)
		اذهب cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	إذا (retval)
		اذهب cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_بلاڪون);
	ext2fs_free_mem(&rec.بلاڪ_buf);
	أرجع retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#جيڪڏهن_وصف_ٿيل __TURBOC__
 #هدايت argsused
#جيڪڏهن_پورو
ساكن صحيح clear_bad_بلاڪ_proc(ext2_filsys fs, blk_t *بلاڪ_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلاڪ EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
				صحيح ref_وٿي EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
				فراغ *priv_معلومات)
{
	بنية سيٽ_badblock_رڪارڊ *rec = (بنية سيٽ_badblock_رڪارڊ *)
		priv_معلومات;
	errcode_t	retval;
	بنانشان ڊگهو 	old_سائيز;

	إذا (!*بلاڪ_nr)
		أرجع 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	إذا (*بلاڪ_nr >= ext2fs_بلاڪون_ڳڻپ(fs->super) ||
	    *بلاڪ_nr < fs->super->s_پهريون_معلومات_بلاڪ) {
		*بلاڪ_nr = 0;
		أرجع بلاڪ_بدليو;
	}

	إذا (blockcnt < 0) {
		إذا (rec->ind_بلاڪون_سائيز >= rec->max_ind_بلاڪون) {
			old_سائيز = rec->max_ind_بلاڪون * حجم(blk_t);
			rec->max_ind_بلاڪون += 10;
			retval = ext2fs_resize_mem(old_سائيز,
				   rec->max_ind_بلاڪون * حجم(blk_t),
				   &rec->ind_بلاڪون);
			إذا (retval) {
				rec->max_ind_بلاڪون -= 10;
				rec->err = retval;
				أرجع بلاڪ_ABORT;
			}
		}
		rec->ind_بلاڪون[rec->ind_بلاڪون_سائيز++] = *بلاڪ_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_بلاڪ_alloc_stats2(fs, *بلاڪ_nr, -1);

	*بلاڪ_nr = 0;
	أرجع بلاڪ_بدليو;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#جيڪڏهن_وصف_ٿيل __TURBOC__
 #هدايت argsused
#جيڪڏهن_پورو
ساكن صحيح سيٽ_bad_بلاڪ_proc(ext2_filsys fs, blk_t *بلاڪ_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلاڪ EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			      صحيح ref_وٿي EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			      فراغ *priv_معلومات)
{
	بنية سيٽ_badblock_رڪارڊ *rec = (بنية سيٽ_badblock_رڪارڊ *)
		priv_معلومات;
	errcode_t	retval;
	blk_t		blk;

	إذا (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		إذا (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			أرجع بلاڪ_ABORT;
		rec->bad_بلاڪ_ڳڻپ++;
	} وإلا {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		إذا (rec->ind_بلاڪون_ptr < rec->ind_بلاڪون_سائيز) {
			blk = rec->ind_بلاڪون[rec->ind_بلاڪون_ptr++];
			إذا (ext2fs_test_بلاڪ_bitmap2(fs->بلاڪ_map, blk))
				اذهب retry;
		} وإلا {
			retval = ext2fs_new_بلاڪ(fs, 0, 0, &blk);
			إذا (retval) {
				rec->err = retval;
				أرجع بلاڪ_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->بلاڪ_buf);
		إذا (retval) {
			rec->err = retval;
			أرجع بلاڪ_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_بلاڪ_alloc_stats2(fs, blk, +1);

	*بلاڪ_nr = blk;
	أرجع بلاڪ_بدليو;
}






