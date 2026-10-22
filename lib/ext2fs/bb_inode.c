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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

ساخت سیٹ_badblock_ریکارڈ {
	ext2_badblocks_iterate	bb_iter;
	صحیح		bad_بلاک_گنتی;
	blk_t		*ind_بلاکیں;
	صحیح		max_ind_بلاکیں;
	صحیح		ind_بلاکیں_سائز;
	صحیح		ind_بلاکیں_ptr;
	حرف		*بلاک_buf;
	errcode_t	err;
};

ساکن صحیح سیٹ_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلاک, صحیح ref_انحراف,
			      خالی *priv_ڈیٹا);
ساکن صحیح clear_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلاک, صحیح ref_انحراف,
				خالی *priv_ڈیٹا);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	ساخت سیٹ_badblock_ریکارڈ	rec;
	ساخت ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!fs->بلاک_map)
		واپس EXT2_ET_NO_بلاک_BITMAP;

	memset(&rec, 0, حجم(rec));
	rec.max_ind_بلاکیں = 10;
	retval = ext2fs_get_array(rec.max_ind_بلاکیں, حجم(blk_t),
				&rec.ind_بلاکیں);
	اگر (retval)
		واپس retval;
	memset(rec.ind_بلاکیں, 0, rec.max_ind_بلاکیں * حجم(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.بلاک_buf);
	اگر (retval)
		جاؤ cleanup;
	memset(rec.بلاک_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_بلاک_iterate2(fs, EXT2_BAD_INO,
				       بلاک_FLAG_گہرائی_TRAVERSE, 0,
				       clear_bad_بلاک_proc, &rec);
	اگر (retval)
		جاؤ cleanup;
	اگر (rec.err) {
		retval = rec.err;
		جاؤ cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	اگر (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		اگر (retval)
			جاؤ cleanup;
		retval = ext2fs_بلاک_iterate2(fs, EXT2_BAD_INO,
					       بلاک_FLAG_APPEND, 0,
					       سیٹ_bad_بلاک_proc, &rec);
		ext2fs_badblocks_list_iterate_انتہا(rec.bb_iter);
		اگر (retval)
			جاؤ cleanup;
		اگر (rec.err) {
			retval = rec.err;
			جاؤ cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	اگر (retval)
		جاؤ cleanup;

	now = ext2fs_P_get_وقت(fs);
	ext2fs_inode_xtime_سیٹ(&inode, i_atime, now);
	اگر (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_سیٹ(&inode, i_ctime, now);
	ext2fs_inode_xtime_سیٹ(&inode, i_mtime, now);
	ext2fs_iblk_سیٹ(fs, &inode, rec.bad_بلاک_گنتی);
	retval = ext2fs_inode_سائز_سیٹ(fs, &inode,
				       rec.bad_بلاک_گنتی * fs->blocksize);
	اگر (retval)
		جاؤ cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	اگر (retval)
		جاؤ cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_بلاکیں);
	ext2fs_free_mem(&rec.بلاک_buf);
	واپس retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
ساکن صحیح clear_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_بلاک EXT2FS_ATTR((غیر_مستعمل)),
				صحیح ref_انحراف EXT2FS_ATTR((غیر_مستعمل)),
				خالی *priv_ڈیٹا)
{
	ساخت سیٹ_badblock_ریکارڈ *rec = (ساخت سیٹ_badblock_ریکارڈ *)
		priv_ڈیٹا;
	errcode_t	retval;
	غیرعلامتی طویل 	old_سائز;

	اگر (!*بلاک_nr)
		واپس 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	اگر (*بلاک_nr >= ext2fs_بلاکیں_گنتی(fs->super) ||
	    *بلاک_nr < fs->super->s_پہلا_ڈیٹا_بلاک) {
		*بلاک_nr = 0;
		واپس بلاک_بدلا;
	}

	اگر (blockcnt < 0) {
		اگر (rec->ind_بلاکیں_سائز >= rec->max_ind_بلاکیں) {
			old_سائز = rec->max_ind_بلاکیں * حجم(blk_t);
			rec->max_ind_بلاکیں += 10;
			retval = ext2fs_resize_mem(old_سائز,
				   rec->max_ind_بلاکیں * حجم(blk_t),
				   &rec->ind_بلاکیں);
			اگر (retval) {
				rec->max_ind_بلاکیں -= 10;
				rec->err = retval;
				واپس بلاک_ABORT;
			}
		}
		rec->ind_بلاکیں[rec->ind_بلاکیں_سائز++] = *بلاک_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_بلاک_alloc_stats2(fs, *بلاک_nr, -1);

	*بلاک_nr = 0;
	واپس بلاک_بدلا;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
ساکن صحیح سیٹ_bad_بلاک_proc(ext2_filsys fs, blk_t *بلاک_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_بلاک EXT2FS_ATTR((غیر_مستعمل)),
			      صحیح ref_انحراف EXT2FS_ATTR((غیر_مستعمل)),
			      خالی *priv_ڈیٹا)
{
	ساخت سیٹ_badblock_ریکارڈ *rec = (ساخت سیٹ_badblock_ریکارڈ *)
		priv_ڈیٹا;
	errcode_t	retval;
	blk_t		blk;

	اگر (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		اگر (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			واپس بلاک_ABORT;
		rec->bad_بلاک_گنتی++;
	} ورنہ {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		اگر (rec->ind_بلاکیں_ptr < rec->ind_بلاکیں_سائز) {
			blk = rec->ind_بلاکیں[rec->ind_بلاکیں_ptr++];
			اگر (ext2fs_test_بلاک_bitmap2(fs->بلاک_map, blk))
				جاؤ retry;
		} ورنہ {
			retval = ext2fs_new_بلاک(fs, 0, 0, &blk);
			اگر (retval) {
				rec->err = retval;
				واپس بلاک_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->بلاک_buf);
		اگر (retval) {
			rec->err = retval;
			واپس بلاک_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_بلاک_alloc_stats2(fs, blk, +1);

	*بلاک_nr = blk;
	واپس بلاک_بدلا;
}






