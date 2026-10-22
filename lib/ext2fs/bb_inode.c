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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

ഘടന set_badblock_record {
	ext2_badblocks_iterate	bb_iter;
	പൂർണ്ണസംഖ്യ		bad_block_എണ്ണം;
	blk_t		*ind_blocks;
	പൂർണ്ണസംഖ്യ		max_ind_blocks;
	പൂർണ്ണസംഖ്യ		ind_blocks_size;
	പൂർണ്ണസംഖ്യ		ind_blocks_ptr;
	അക്ഷരം		*block_buf;
	errcode_t	err;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block, പൂർണ്ണസംഖ്യ ref_offset,
			      ശൂന്യം *priv_data);
സ്ഥിതം പൂർണ്ണസംഖ്യ clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block, പൂർണ്ണസംഖ്യ ref_offset,
				ശൂന്യം *priv_data);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	ഘടന set_badblock_record	rec;
	ഘടന ext2_inode		inode;
	time_t				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (!fs->block_map)
		തിരികെ EXT2_ET_NO_BLOCK_BITMAP;

	memset(&rec, 0, വലുപ്പം(rec));
	rec.max_ind_blocks = 10;
	retval = ext2fs_get_array(rec.max_ind_blocks, വലുപ്പം(blk_t),
				&rec.ind_blocks);
	എങ്കിൽ (retval)
		തിരികെ retval;
	memset(rec.ind_blocks, 0, rec.max_ind_blocks * വലുപ്പം(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.block_buf);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	memset(rec.block_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
				       BLOCK_FLAG_DEPTH_TRAVERSE, 0,
				       clear_bad_block_proc, &rec);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	എങ്കിൽ (rec.err) {
		retval = rec.err;
		പോകുക cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	എങ്കിൽ (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		എങ്കിൽ (retval)
			പോകുക cleanup;
		retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
					       BLOCK_FLAG_APPEND, 0,
					       set_bad_block_proc, &rec);
		ext2fs_badblocks_list_iterate_end(rec.bb_iter);
		എങ്കിൽ (retval)
			പോകുക cleanup;
		എങ്കിൽ (rec.err) {
			retval = rec.err;
			പോകുക cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	എങ്കിൽ (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_iblk_set(fs, &inode, rec.bad_block_എണ്ണം);
	retval = ext2fs_inode_size_set(fs, &inode,
				       rec.bad_block_എണ്ണം * fs->blocksize);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	എങ്കിൽ (retval)
		പോകുക cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_blocks);
	ext2fs_free_mem(&rec.block_buf);
	തിരികെ retval;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block EXT2FS_ATTR((unused)),
				പൂർണ്ണസംഖ്യ ref_offset EXT2FS_ATTR((unused)),
				ശൂന്യം *priv_data)
{
	ഘടന set_badblock_record *rec = (ഘടന set_badblock_record *)
		priv_data;
	errcode_t	retval;
	unsigned long 	old_size;

	എങ്കിൽ (!*block_nr)
		തിരികെ 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	എങ്കിൽ (*block_nr >= ext2fs_blocks_എണ്ണം(fs->super) ||
	    *block_nr < fs->super->s_first_data_block) {
		*block_nr = 0;
		തിരികെ BLOCK_CHANGED;
	}

	എങ്കിൽ (blockcnt < 0) {
		എങ്കിൽ (rec->ind_blocks_size >= rec->max_ind_blocks) {
			old_size = rec->max_ind_blocks * വലുപ്പം(blk_t);
			rec->max_ind_blocks += 10;
			retval = ext2fs_resize_mem(old_size,
				   rec->max_ind_blocks * വലുപ്പം(blk_t),
				   &rec->ind_blocks);
			എങ്കിൽ (retval) {
				rec->max_ind_blocks -= 10;
				rec->err = retval;
				തിരികെ BLOCK_ABORT;
			}
		}
		rec->ind_blocks[rec->ind_blocks_size++] = *block_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_block_alloc_stats2(fs, *block_nr, -1);

	*block_nr = 0;
	തിരികെ BLOCK_CHANGED;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
സ്ഥിതം പൂർണ്ണസംഖ്യ set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block EXT2FS_ATTR((unused)),
			      പൂർണ്ണസംഖ്യ ref_offset EXT2FS_ATTR((unused)),
			      ശൂന്യം *priv_data)
{
	ഘടന set_badblock_record *rec = (ഘടന set_badblock_record *)
		priv_data;
	errcode_t	retval;
	blk_t		blk;

	എങ്കിൽ (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		എങ്കിൽ (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			തിരികെ BLOCK_ABORT;
		rec->bad_block_എണ്ണം++;
	} അല്ലെങ്കിൽ {
		/*
		 * An indirect block; fetch a block from the
		 * previously used indirect block list.  The block
		 * most be not marked as used; if so, get another one.
		 * If we run out of reserved indirect blocks, allocate
		 * a new one.
		 */
	retry:
		എങ്കിൽ (rec->ind_blocks_ptr < rec->ind_blocks_size) {
			blk = rec->ind_blocks[rec->ind_blocks_ptr++];
			എങ്കിൽ (ext2fs_test_block_bitmap2(fs->block_map, blk))
				പോകുക retry;
		} അല്ലെങ്കിൽ {
			retval = ext2fs_new_block(fs, 0, 0, &blk);
			എങ്കിൽ (retval) {
				rec->err = retval;
				തിരികെ BLOCK_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->block_buf);
		എങ്കിൽ (retval) {
			rec->err = retval;
			തിരികെ BLOCK_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_block_alloc_stats2(fs, blk, +1);

	*block_nr = blk;
	തിരികെ BLOCK_CHANGED;
}






