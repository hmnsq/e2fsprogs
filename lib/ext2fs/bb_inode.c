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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

काष्ठा set_badblock_record {
	ext2_badblocks_iterate	bb_iter;
	पूर्णांक		bad_block_गिनती;
	blk_t		*ind_blocks;
	पूर्णांक		max_ind_blocks;
	पूर्णांक		ind_blocks_आकार;
	पूर्णांक		ind_blocks_ptr;
	अक्षर		*block_buf;
	errcode_t	err;
};

अटल पूर्णांक set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block, पूर्णांक ref_offset,
			      व्योम *priv_तथ्य);
अटल पूर्णांक clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block, पूर्णांक ref_offset,
				व्योम *priv_तथ्य);

/*
 * Given a bad blocks bitmap, update the bad blocks inode to reflect
 * the map.
 */
errcode_t ext2fs_update_bb_inode(ext2_filsys fs, ext2_badblocks_list bb_list)
{
	errcode_t			retval;
	काष्ठा set_badblock_record	rec;
	काष्ठा ext2_inode		inode;
	समय_प्रकार				now;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (!fs->block_map)
		वापस EXT2_ET_NO_BLOCK_BITMAP;

	स_रखो(&rec, 0, माप(rec));
	rec.max_ind_blocks = 10;
	retval = ext2fs_get_array(rec.max_ind_blocks, माप(blk_t),
				&rec.ind_blocks);
	अगर (retval)
		वापस retval;
	स_रखो(rec.ind_blocks, 0, rec.max_ind_blocks * माप(blk_t));
	retval = ext2fs_get_mem(fs->blocksize, &rec.block_buf);
	अगर (retval)
		जाओ cleanup;
	स_रखो(rec.block_buf, 0, fs->blocksize);
	rec.err = 0;

	/*
	 * First clear the old bad blocks (while saving the indirect blocks)
	 */
	retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
				       BLOCK_FLAG_DEPTH_TRAVERSE, 0,
				       clear_bad_block_proc, &rec);
	अगर (retval)
		जाओ cleanup;
	अगर (rec.err) {
		retval = rec.err;
		जाओ cleanup;
	}

	/*
	 * Now set the bad blocks!
	 *
	 * First, mark the bad blocks as used.  This prevents a bad
	 * block from being used as an indirect block for the bad
	 * block inode (!).
	 */
	अगर (bb_list) {
		retval = ext2fs_badblocks_list_iterate_begin(bb_list,
							     &rec.bb_iter);
		अगर (retval)
			जाओ cleanup;
		retval = ext2fs_block_iterate2(fs, EXT2_BAD_INO,
					       BLOCK_FLAG_APPEND, 0,
					       set_bad_block_proc, &rec);
		ext2fs_badblocks_list_iterate_end(rec.bb_iter);
		अगर (retval)
			जाओ cleanup;
		अगर (rec.err) {
			retval = rec.err;
			जाओ cleanup;
		}
	}

	/*
	 * Update the bad block inode's mod time and block count
	 * field.
	 */
	retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
	अगर (retval)
		जाओ cleanup;

	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_atime, now);
	अगर (!ext2fs_inode_xtime_get(&inode, i_ctime))
		ext2fs_inode_xtime_set(&inode, i_ctime, now);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_iblk_set(fs, &inode, rec.bad_block_गिनती);
	retval = ext2fs_inode_आकार_set(fs, &inode,
				       rec.bad_block_गिनती * fs->blocksize);
	अगर (retval)
		जाओ cleanup;

	retval = ext2fs_write_inode(fs, EXT2_BAD_INO, &inode);
	अगर (retval)
		जाओ cleanup;

cleanup:
	ext2fs_free_mem(&rec.ind_blocks);
	ext2fs_free_mem(&rec.block_buf);
	वापस retval;
}

/*
 * Helper function for update_bb_inode()
 *
 * Clear the bad blocks in the bad block inode, while saving the
 * indirect blocks.
 */
#अगर_घोषित __TURBOC__
 #आशय argsused
#पूर्ण_अगर
अटल पूर्णांक clear_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
				e2_blkcnt_t blockcnt,
				blk_t ref_block EXT2FS_ATTR((unused)),
				पूर्णांक ref_offset EXT2FS_ATTR((unused)),
				व्योम *priv_तथ्य)
{
	काष्ठा set_badblock_record *rec = (काष्ठा set_badblock_record *)
		priv_तथ्य;
	errcode_t	retval;
	अचिन्हित दीर्घ 	old_आकार;

	अगर (!*block_nr)
		वापस 0;

	/*
	 * If the block number is outrageous, clear it and ignore it.
	 */
	अगर (*block_nr >= ext2fs_blocks_गिनती(fs->super) ||
	    *block_nr < fs->super->s_first_तथ्य_block) {
		*block_nr = 0;
		वापस BLOCK_CHANGED;
	}

	अगर (blockcnt < 0) {
		अगर (rec->ind_blocks_आकार >= rec->max_ind_blocks) {
			old_आकार = rec->max_ind_blocks * माप(blk_t);
			rec->max_ind_blocks += 10;
			retval = ext2fs_resize_mem(old_आकार,
				   rec->max_ind_blocks * माप(blk_t),
				   &rec->ind_blocks);
			अगर (retval) {
				rec->max_ind_blocks -= 10;
				rec->err = retval;
				वापस BLOCK_ABORT;
			}
		}
		rec->ind_blocks[rec->ind_blocks_आकार++] = *block_nr;
	}

	/*
	 * Mark the block as unused, and update accounting information
	 */
	ext2fs_block_alloc_stats2(fs, *block_nr, -1);

	*block_nr = 0;
	वापस BLOCK_CHANGED;
}


/*
 * Helper function for update_bb_inode()
 *
 * Set the block list in the bad block inode, using the supplied bitmap.
 */
#अगर_घोषित __TURBOC__
 #आशय argsused
#पूर्ण_अगर
अटल पूर्णांक set_bad_block_proc(ext2_filsys fs, blk_t *block_nr,
			      e2_blkcnt_t blockcnt,
			      blk_t ref_block EXT2FS_ATTR((unused)),
			      पूर्णांक ref_offset EXT2FS_ATTR((unused)),
			      व्योम *priv_तथ्य)
{
	काष्ठा set_badblock_record *rec = (काष्ठा set_badblock_record *)
		priv_तथ्य;
	errcode_t	retval;
	blk_t		blk;

	अगर (blockcnt >= 0) {
		/*
		 * Get the next bad block.
		 */
		अगर (!ext2fs_badblocks_list_iterate(rec->bb_iter, &blk))
			वापस BLOCK_ABORT;
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
		अगर (rec->ind_blocks_ptr < rec->ind_blocks_आकार) {
			blk = rec->ind_blocks[rec->ind_blocks_ptr++];
			अगर (ext2fs_test_block_bitmap2(fs->block_map, blk))
				जाओ retry;
		} अन्यथा {
			retval = ext2fs_new_block(fs, 0, 0, &blk);
			अगर (retval) {
				rec->err = retval;
				वापस BLOCK_ABORT;
			}
		}
		retval = io_channel_write_blk64(fs->io, blk, 1, rec->block_buf);
		अगर (retval) {
			rec->err = retval;
			वापस BLOCK_ABORT;
		}
	}

	/*
	 * Update block counts
	 */
	ext2fs_block_alloc_stats2(fs, blk, +1);

	*block_nr = blk;
	वापस BLOCK_CHANGED;
}






