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
#پایان_اگر
#ضمن <fcntl.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية مجموعه_badblock_رکورد {
	ext2_badblocks_iterate	bb_iter;
	صحيح		bad_بلوک_شمار;
	blk_t		*ind_بلوکها;
	صحيح		max_ind_بلوکها;
	صحيح		ind_بلوکها_اندازه;
	صحيح		ind_بلوکها_ptr;
	حرف		*بلوک_buf;
	errcode_t	err;
};

ساكن صحيح مجموعه_bad_بلوک_proc(ext2_filsys fs, blk_t *بلوک_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلوک, صحيح ref_جابه_جایی,
			      فراغ *priv_داده);
ساكن صحيح clear_bad_بلوک_proc(ext2_filsys fs, blk_t *بلوک_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلوک, صحيح ref_جابه_جایی,
				فراغ *priv_داده);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	بنية مجموعه_badblock_رکورد	rec;
	بنية ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!fs->بلوک_map)
		أرجع EXT2_ET_NO_بلوک_BITMAP;

	memset(&rec, 0, حجم(rec));
	rec.max_ind_بلوکها = 10;
	retval = ext2fs_get_array(rec.max_ind_بلوکها, حجم(blk_t),
				&rec.ind_بلوکها);
	إذا (retval)
		أرجع retval;
	memset(rec.ind_بلوکها, 0, rec.max_ind_بلوکها * حجم(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.بلوک_buf);
	إذا (retval)
		اذهب cleanup;
	memset(rec.بلوک_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_بلوک_iterate2(fs, EXT2_BAD_INO,
				       بلوک_FLAG_عمق_TRAVERSE, 0,
				       clear_bad_بلوک_proc, &rec);
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
		retval = ext2fs_بلوک_iterate2(fs, EXT2_BAD_INO,
					       بلوک_FLAG_APPEND, 0,
					       مجموعه_bad_بلوک_proc, &rec);
		ext2fs_badblocks_list_iterate_انتها(rec.bb_iter);
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

	now = ext2fs_P_get_زمان(fs);
	ext2fs_inode_xtime_مجموعه(&inode, i_atime, now);
	إذا (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_مجموعه(&inode, i_ctime, now);
	ext2fs_inode_xtime_مجموعه(&inode, i_mtime, now);
	ext2fs_iblk_مجموعه(fs, &inode, rec.bad_بلوک_شمار);
	retval = ext2fs_inode_اندازه_مجموعه(fs, &inode,
				       rec.bad_بلوک_شمار * fs->blocksize);
	إذا (retval)
		اذهب cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	إذا (retval)
		اذهب cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_بلوکها);
	ext2fs_free_mem(&rec.بلوک_buf);
	أرجع retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#اگر_تعریف_شده __TURBOC__
 #رهنمود argsused
#پایان_اگر
ساكن صحيح clear_bad_بلوک_proc(ext2_filsys fs, blk_t *بلوک_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلوک EXT2FS_ATTR((استفادهنشده)),
				صحيح ref_جابه_جایی EXT2FS_ATTR((استفادهنشده)),
				فراغ *priv_داده)
{
	بنية مجموعه_badblock_رکورد *rec = (بنية مجموعه_badblock_رکورد *)
		priv_داده;
	errcode_t	retval;
	بیعلامت دراز 	old_اندازه;

	إذا (!*بلوک_nr)
		أرجع 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	إذا (*بلوک_nr >= ext2fs_بلوکها_شمار(fs->super) ||
	    *بلوک_nr < fs->super->s_اولی_داده_بلوک) {
		*بلوک_nr = 0;
		أرجع بلوک_تغییریافته;
	}

	إذا (blockcnt < 0) {
		إذا (rec->ind_بلوکها_اندازه >= rec->max_ind_بلوکها) {
			old_اندازه = rec->max_ind_بلوکها * حجم(blk_t);
			rec->max_ind_بلوکها += 10;
			retval = ext2fs_resize_mem(old_اندازه,
				   rec->max_ind_بلوکها * حجم(blk_t),
				   &rec->ind_بلوکها);
			إذا (retval) {
				rec->max_ind_بلوکها -= 10;
				rec->err = retval;
				أرجع بلوک_ABORT;
			}
		}
		rec->ind_بلوکها[rec->ind_بلوکها_اندازه++] = *بلوک_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_بلوک_alloc_stats2(fs, *بلوک_nr, -1);

	*بلوک_nr = 0;
	أرجع بلوک_تغییریافته;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#اگر_تعریف_شده __TURBOC__
 #رهنمود argsused
#پایان_اگر
ساكن صحيح مجموعه_bad_بلوک_proc(ext2_filsys fs, blk_t *بلوک_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلوک EXT2FS_ATTR((استفادهنشده)),
			      صحيح ref_جابه_جایی EXT2FS_ATTR((استفادهنشده)),
			      فراغ *priv_داده)
{
	بنية مجموعه_badblock_رکورد *rec = (بنية مجموعه_badblock_رکورد *)
		priv_داده;
	errcode_t	retval;
	blk_t		blk;

	إذا (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		إذا (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			أرجع بلوک_ABORT;
		rec->bad_بلوک_شمار++;
	} وإلا {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		إذا (rec->ind_بلوکها_ptr < rec->ind_بلوکها_اندازه) {
			blk = rec->ind_بلوکها[rec->ind_بلوکها_ptr++];
			إذا (ext2fs_test_بلوک_bitmap2(fs->بلوک_map, blk))
				اذهب retry;
		} وإلا {
			retval = ext2fs_new_بلوک(fs, 0, 0, &blk);
			إذا (retval) {
				rec->err = retval;
				أرجع بلوک_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->بلوک_buf);
		إذا (retval) {
			rec->err = retval;
			أرجع بلوک_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_بلوک_alloc_stats2(fs, blk, +1);

	*بلوک_nr = blk;
	أرجع بلوک_تغییریافته;
}






