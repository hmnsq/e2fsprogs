/*
 * alloc.c --- allocate new inodes, blocks for ext2fs
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <time.h>
#अन्तर्भाव <string.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

#परिभाषय min(a, b) ((a) < (b) ? (a) : (b))

#अपरिभाषय DEBUG

#यदि_परिभाषितम् DEBUG
# define dbg_printf(f, a...)  कुरु {प्रारूप_लिख(f, ## a); fflush(stdout); } यावत् (0)
#अन्यथा
# define dbg_printf(f, a...)
#यदि_अन्तः

/*
 * Clear the uninit block bitmap flag if necessary
 */
शून्य ext2fs_clear_खण्डम्_uninit(ext2_filsys fs, dgrp_t गणः)
{
	यदि (गणः >= fs->गणः_desc_गणनम् ||
	    !ext2fs_has_गणः_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, गणः, EXT2_BG_खण्डम्_UNINIT)))
		प्रत्यागम;

	/* uninit block bitmaps are now initialized in read_bitmaps() */

	ext2fs_bg_flags_clear(fs, गणः, EXT2_BG_खण्डम्_UNINIT);
	ext2fs_गणः_desc_csum_समुच्चयः(fs, गणः);
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
}

/*
 * Check for uninit inode bitmaps and deal with them appropriately
 */
स्थिर शून्य check_inode_uninit(ext2_filsys fs, ext2fs_inode_bitmap map,
			  dgrp_t गणः)
{
	ext2_ino_t	i, ino;

	यदि (गणः >= fs->गणः_desc_गणनम् ||
	    !ext2fs_has_गणः_desc_csum(fs) ||
	    !(ext2fs_bg_flags_test(fs, गणः, EXT2_BG_INODE_UNINIT)))
		प्रत्यागम;

	ino = (गणः * fs->super->s_inodes_प्रत्येकम्_गणः) + 1;
	प्रति (i=0; i < fs->super->s_inodes_प्रत्येकम्_गणः; i++, ino++)
		ext2fs_fast_unmark_inode_bitmap2(map, ino);

	ext2fs_bg_flags_clear(fs, गणः, EXT2_BG_INODE_UNINIT);
	/* Mimics what the kernel does */
	ext2fs_bg_flags_clear(fs, गणः, EXT2_BG_खण्डम्_UNINIT);
	ext2fs_गणः_desc_csum_समुच्चयः(fs, गणः);
	ext2fs_mark_ib_dirty(fs);
	ext2fs_mark_super_dirty(fs);
}

/*
 * Right now, just search forward from the parent directory's block
 * group to find the next free inode.
 *
 * Should have a special policy for directories.
 */
errcode_t ext2fs_new_inode(ext2_filsys fs, ext2_ino_t dir,
			   पूर्णाङ्क बहुलकम् EXT2FS_ATTR((अप्रयुक्तम्)),
			   ext2fs_inode_bitmap map, ext2_ino_t *ret)
{
	ext2_ino_t	start_inode = 0;
	ext2_ino_t	i, ino_अन्तर्_गणः, upto, first_zero;
	errcode_t	retval;
	dgrp_t		गणः;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!map)
		map = fs->inode_map;
	यदि (!map)
		प्रत्यागम EXT2_ET_NO_INODE_BITMAP;

	यदि (dir > 0) {
		गणः = (dir - 1) / EXT2_INODES_प्रत्येकम्_गणः(fs->super);
		start_inode = (गणः * EXT2_INODES_प्रत्येकम्_गणः(fs->super)) + 1;
	}
	यदि (start_inode < EXT2_FIRST_INODE(fs->super))
		start_inode = EXT2_FIRST_INODE(fs->super);
	यदि (start_inode > fs->super->s_inodes_गणनम्)
		प्रत्यागम EXT2_ET_INODE_ALLOC_FAIL;
	i = start_inode;
	कुरु {
		ino_अन्तर्_गणः = (i - 1) % EXT2_INODES_प्रत्येकम्_गणः(fs->super);
		गणः = (i - 1) / EXT2_INODES_प्रत्येकम्_गणः(fs->super);

		check_inode_uninit(fs, map, गणः);
		upto = i + (EXT2_INODES_प्रत्येकम्_गणः(fs->super) - ino_अन्तर्_गणः);
		यदि (i < start_inode && upto >= start_inode)
			upto = start_inode - 1;
		यदि (upto > fs->super->s_inodes_गणनम्)
			upto = fs->super->s_inodes_गणनम्;

		retval = ext2fs_find_first_zero_inode_bitmap2(map, i, upto,
							      &first_zero);
		यदि (retval == 0) {
			i = first_zero;
			भङ्ग;
		}
		यदि (retval != ENOENT)
			प्रत्यागम EXT2_ET_INODE_ALLOC_FAIL;
		i = upto + 1;
		यदि (i > fs->super->s_inodes_गणनम्)
			i = EXT2_FIRST_INODE(fs->super);
	} यावत् (i != start_inode);

	यदि (ext2fs_test_inode_bitmap2(map, i))
		प्रत्यागम EXT2_ET_INODE_ALLOC_FAIL;
	*ret = i;
	प्रत्यागम 0;
}

/*
 * Stupid algorithm --- we now just search forward starting from the
 * goal.  Should put in a smarter one someday....
 */
errcode_t ext2fs_new_block3(ext2_filsys fs, blk64_t goal,
			    ext2fs_खण्डम्_bitmap map, blk64_t *ret,
			    संरचना blk_alloc_ctx *ctx)
{
	errcode_t retval;
	blk64_t	b = 0;
	errcode_t (*gab)(ext2_filsys fs, blk64_t goal, blk64_t *ret);
	errcode_t (*gab2)(ext2_filsys, blk64_t, blk64_t *,
			  संरचना blk_alloc_ctx *);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!map) {
		/*
		 * In case there are clients out there whose get_alloc_block
		 * handlers call ext2fs_new_block2 with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		यदि (fs->get_alloc_block2) {
			gab2 = fs->get_alloc_block2;
			fs->get_alloc_block2 = रिक्त;
			retval = gab2(fs, goal, &b, ctx);
			fs->get_alloc_block2 = gab2;
			गच्छ allocated;
		} अन्यथा यदि (fs->get_alloc_खण्डम्) {
			gab = fs->get_alloc_खण्डम्;
			fs->get_alloc_खण्डम् = रिक्त;
			retval = gab(fs, goal, &b);
			fs->get_alloc_खण्डम् = gab;
			गच्छ allocated;
		}
	}
	यदि (!map)
		map = fs->खण्डम्_map;
	यदि (!map)
		प्रत्यागम EXT2_ET_NO_खण्डम्_BITMAP;
	यदि (!goal || (goal >= ext2fs_खण्डानि_गणनम्(fs->super)))
		goal = fs->super->s_first_दत्त_खण्डम्;
	goal &= ~EXT2_FS_CLUSTER_आवरणम्(fs);

	retval = ext2fs_find_first_zero_खण्डम्_bitmap2(map,
			goal, ext2fs_खण्डानि_गणनम्(fs->super) - 1, &b);
	यदि ((retval == ENOENT) && (goal != fs->super->s_first_दत्त_खण्डम्))
		retval = ext2fs_find_first_zero_खण्डम्_bitmap2(map,
			fs->super->s_first_दत्त_खण्डम्, goal - 1, &b);
allocated:
	यदि (retval == ENOENT)
		प्रत्यागम EXT2_ET_खण्डम्_ALLOC_FAIL;
	यदि (retval)
		प्रत्यागम retval;

	ext2fs_clear_खण्डम्_uninit(fs, ext2fs_गणः_सम्बन्धि_blk2(fs, b));
	*ret = b;
	प्रत्यागम 0;
}

errcode_t ext2fs_new_block2(ext2_filsys fs, blk64_t goal,
			   ext2fs_खण्डम्_bitmap map, blk64_t *ret)
{
	प्रत्यागम ext2fs_new_block3(fs, goal, map, ret, रिक्त);
}

errcode_t ext2fs_new_खण्डम्(ext2_filsys fs, blk_t goal,
			   ext2fs_खण्डम्_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_new_block2(fs, goal, map, &val);
	यदि (!retval)
		*ret = (blk_t) val;
	प्रत्यागम retval;
}

/*
 * This function zeros out the allocated block, and updates all of the
 * appropriate filesystem records.
 */
errcode_t ext2fs_alloc_block3(ext2_filsys fs, blk64_t goal, वर्ण *खण्डम्_buf,
			      blk64_t *ret, संरचना blk_alloc_ctx *ctx)
{
	errcode_t	retval;
	blk64_t		खण्डम्;

	यदि (fs->get_alloc_block2) {
		retval = (fs->get_alloc_block2)(fs, goal, &खण्डम्, ctx);
		यदि (retval)
			गच्छ fail;
	} अन्यथा यदि (fs->get_alloc_खण्डम्) {
		retval = (fs->get_alloc_खण्डम्)(fs, goal, &खण्डम्);
		यदि (retval)
			गच्छ fail;
	} अन्यथा {
		यदि (!fs->खण्डम्_map) {
			retval = ext2fs_read_खण्डम्_bitmap(fs);
			यदि (retval)
				गच्छ fail;
		}

		retval = ext2fs_new_block3(fs, goal, 0, &खण्डम्, ctx);
		यदि (retval)
			गच्छ fail;
	}

	यदि (खण्डम्_buf) {
		स्मृतिं_पूरय(खण्डम्_buf, 0, fs->blocksize);
		retval = io_channel_write_blk64(fs->io, खण्डम्, 1, खण्डम्_buf);
	} अन्यथा
		retval = ext2fs_zero_blocks2(fs, खण्डम्, 1, रिक्त, रिक्त);
	यदि (retval)
		गच्छ fail;

	ext2fs_खण्डम्_alloc_stats2(fs, खण्डम्, +1);
	*ret = खण्डम्;

fail:
	प्रत्यागम retval;
}

errcode_t ext2fs_alloc_block2(ext2_filsys fs, blk64_t goal,
			     वर्ण *खण्डम्_buf, blk64_t *ret)
{
	प्रत्यागम ext2fs_alloc_block3(fs, goal, खण्डम्_buf, ret, रिक्त);
}

errcode_t ext2fs_alloc_खण्डम्(ext2_filsys fs, blk_t goal,
			     वर्ण *खण्डम्_buf, blk_t *ret)
{
	errcode_t retval;
	blk64_t ret64, goal64 = goal;
	retval = ext2fs_alloc_block3(fs, goal64, खण्डम्_buf, &ret64, रिक्त);
	यदि (!retval)
		*ret = (blk_t)ret64;
        प्रत्यागम retval;
}

errcode_t ext2fs_get_free_blocks2(ext2_filsys fs, blk64_t start, blk64_t finish,
				 पूर्णाङ्क अङ्कः, ext2fs_खण्डम्_bitmap map, blk64_t *ret)
{
	blk64_t	b = start;
	पूर्णाङ्क	c_अनुपातः;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!map)
		map = fs->खण्डम्_map;
	यदि (!map)
		प्रत्यागम EXT2_ET_NO_खण्डम्_BITMAP;
	यदि (!b)
		b = fs->super->s_first_दत्त_खण्डम्;
	यदि (!finish)
		finish = start;
	यदि (!अङ्कः)
		अङ्कः = 1;
	c_अनुपातः = 1 << ext2fs_get_bitmap_granularity(map);
	b &= ~(c_अनुपातः - 1);
	finish &= ~(c_अनुपातः -1);
	कुरु {
		यदि (b + अङ्कः - 1 >= ext2fs_खण्डानि_गणनम्(fs->super)) {
			यदि (finish > start)
				प्रत्यागम EXT2_ET_खण्डम्_ALLOC_FAIL;
			b = fs->super->s_first_दत्त_खण्डम्;
		}
		यदि (ext2fs_fast_test_खण्डम्_bitmap_range2(map, b, अङ्कः)) {
			*ret = b;
			प्रत्यागम 0;
		}
		b += c_अनुपातः;
	} यावत् (b != finish);
	प्रत्यागम EXT2_ET_खण्डम्_ALLOC_FAIL;
}

errcode_t ext2fs_get_free_खण्डानि(ext2_filsys fs, blk_t start, blk_t finish,
				 पूर्णाङ्क अङ्कः, ext2fs_खण्डम्_bitmap map, blk_t *ret)
{
	errcode_t retval;
	blk64_t val;
	retval = ext2fs_get_free_blocks2(fs, start, finish, अङ्कः, map, &val);
	यदि(!retval)
		*ret = (blk_t) val;
	प्रत्यागम retval;
}

शून्य ext2fs_समुच्चयः_alloc_खण्डम्_callback(ext2_filsys fs,
				     errcode_t (*func)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret),
				     errcode_t (**old)(ext2_filsys fs,
						       blk64_t goal,
						       blk64_t *ret))
{
	यदि (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		प्रत्यागम;

	यदि (old)
		*old = fs->get_alloc_खण्डम्;

	fs->get_alloc_खण्डम् = func;
}

blk64_t ext2fs_find_inode_goal(ext2_filsys fs, ext2_ino_t ino,
			       संरचना ext2_inode *inode, blk64_t lblk)
{
	dgrp_t			गणः;
	__u8			log_flex;
	संरचना ext2fs_extent	extent;
	ext2_extent_handle_t	handle = रिक्त;
	errcode_t		err;

	/* Make sure data stored in inode->i_block is neither fast symlink nor
	 * inline data.
	 */
	यदि (inode == रिक्त || ext2fs_is_fast_symlink(inode) ||
	    inode->i_flags & EXT4_INLINE_दत्त_FL)
		गच्छ no_खण्डानि;

	यदि (inode->i_flags & EXT4_EXTENTS_FL) {
		err = ext2fs_extent_open2(fs, ino, inode, &handle);
		यदि (err)
			गच्छ no_खण्डानि;
		err = ext2fs_extent_goto2(handle, 0, lblk);
		यदि (err)
			गच्छ no_खण्डानि;
		err = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
		यदि (err)
			गच्छ no_खण्डानि;
		ext2fs_extent_free(handle);
		प्रत्यागम extent.e_pblk + (lblk - extent.e_lblk);
	}

	/* block mapped file; see if block zero is mapped? */
	यदि (inode->i_खण्डम्[0])
		प्रत्यागम inode->i_खण्डम्[0];

no_खण्डानि:
	ext2fs_extent_free(handle);
	log_flex = fs->super->s_log_groups_प्रत्येकम्_flex;
	गणः = ext2fs_गणः_सम्बन्धि_ino(fs, ino);
	यदि (log_flex)
		गणः = गणः & ~((1 << (log_flex)) - 1);
	प्रत्यागम ext2fs_गणः_first_block2(fs, गणः);
}

/*
 * Starting at _goal_, scan around the filesystem to find a run of free blocks
 * that's at least _len_ blocks long.  Possible flags:
 * - EXT2_NEWRANGE_EXACT_GOAL: The range of blocks must start at _goal_.
 * - EXT2_NEWRANGE_MIN_LENGTH: do not return a allocation shorter than _len_.
 * - EXT2_NEWRANGE_ZERO_BLOCKS: Zero blocks pblk to pblk+plen before returning.
 *
 * The starting block is returned in _pblk_ and the length is returned via
 * _plen_.  The blocks are not marked in the bitmap; the caller must mark
 * however much of the returned run they actually use, hopefully via
 * ext2fs_block_alloc_stats_range().
 *
 * This function can return a range that is longer than what was requested.
 */
errcode_t ext2fs_new_range(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			   blk64_t len, ext2fs_खण्डम्_bitmap map, blk64_t *pblk,
			   blk64_t *plen)
{
	errcode_t retval;
	blk64_t start, अन्तः, b;
	पूर्णाङ्क looped = 0;
	blk64_t max_खण्डानि = ext2fs_खण्डानि_गणनम्(fs->super);
	errcode_t (*nrf)(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			 blk64_t len, blk64_t *pblk, blk64_t *plen);

	dbg_printf("%s: flags=0x%x goal=%llu len=%llu\n", __func__, flags,
		   goal, len);
	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	यदि (len == 0 || (flags & ~EXT2_NEWRANGE_ALL_FLAGS))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	यदि (!map && fs->new_range) {
		/*
		 * In case there are clients out there whose new_range
		 * handlers call ext2fs_new_range with a NULL block map,
		 * temporarily swap out the function pointer so that we don't
		 * end up in an infinite loop.
		 */
		nrf = fs->new_range;
		fs->new_range = रिक्त;
		retval = nrf(fs, flags, goal, len, pblk, plen);
		fs->new_range = nrf;
		यदि (retval)
			प्रत्यागम retval;
		start = *pblk;
		अन्तः = *pblk + *plen;
		गच्छ allocated;
	}
	यदि (!map)
		map = fs->खण्डम्_map;
	यदि (!map)
		प्रत्यागम EXT2_ET_NO_खण्डम्_BITMAP;
	यदि (!goal || goal >= ext2fs_खण्डानि_गणनम्(fs->super))
		goal = fs->super->s_first_दत्त_खण्डम्;

	start = goal;
	यावत् (!looped || start <= goal) {
		retval = ext2fs_find_first_zero_खण्डम्_bitmap2(map, start,
							      max_खण्डानि - 1,
							      &start);
		यदि (retval == ENOENT) {
			/*
			 * If there are no free blocks beyond the starting
			 * point, try scanning the whole filesystem, unless the
			 * user told us only to allocate from _goal_, or if
			 * we're already scanning the whole filesystem.
			 */
			यदि (flags & EXT2_NEWRANGE_FIXED_GOAL ||
			    start == fs->super->s_first_दत्त_खण्डम्)
				गच्छ fail;
			start = fs->super->s_first_दत्त_खण्डम्;
			अनुवर्त;
		} अन्यथा यदि (retval)
			गच्छ errout;

		यदि (flags & EXT2_NEWRANGE_FIXED_GOAL && start != goal)
			गच्छ fail;

		b = min(start + len - 1, max_खण्डानि - 1);
		retval =  ext2fs_find_first_समुच्चयः_खण्डम्_bitmap2(map, start, b,
							      &अन्तः);
		यदि (retval == ENOENT)
			अन्तः = b + 1;
		अन्यथा यदि (retval)
			गच्छ errout;

		यदि (!(flags & EXT2_NEWRANGE_MIN_दीर्घता) ||
		    (अन्तः - start) >= len) {
			/* Success! */
			*pblk = start;
			*plen = अन्तः - start;
			dbg_printf("%s: new_range goal=%llu--%llu "
				   "blk=%llu--%llu %llu\n",
				   __func__, goal, goal + len - 1,
				   *pblk, *pblk + *plen - 1, *plen);
allocated:
			प्रति (b = start; b < अन्तः;
			     b += fs->super->s_खण्डानि_प्रत्येकम्_गणः)
				ext2fs_clear_खण्डम्_uninit(fs,
						ext2fs_गणः_सम्बन्धि_blk2(fs, b));
			प्रत्यागम 0;
		}

		यदि (flags & EXT2_NEWRANGE_FIXED_GOAL)
			गच्छ fail;
		start = अन्तः;
		यदि (start >= max_खण्डानि) {
			यदि (looped)
				गच्छ fail;
			looped = 1;
			start = fs->super->s_first_दत्त_खण्डम्;
		}
	}

fail:
	retval = EXT2_ET_खण्डम्_ALLOC_FAIL;
errout:
	प्रत्यागम retval;
}

शून्य ext2fs_समुच्चयः_new_range_callback(ext2_filsys fs,
	errcode_t (*func)(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen),
	errcode_t (**old)(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			       blk64_t len, blk64_t *pblk, blk64_t *plen))
{
	यदि (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		प्रत्यागम;

	यदि (old)
		*old = fs->new_range;

	fs->new_range = func;
}

errcode_t ext2fs_alloc_range(ext2_filsys fs, पूर्णाङ्क flags, blk64_t goal,
			     blk_t len, blk64_t *ret)
{
	पूर्णाङ्क newr_flags = EXT2_NEWRANGE_MIN_दीर्घता;
	errcode_t retval;
	blk64_t plen;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);
	यदि (len == 0 || (flags & ~EXT2_ALLOCRANGE_ALL_FLAGS))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	यदि (flags & EXT2_ALLOCRANGE_FIXED_GOAL)
		newr_flags |= EXT2_NEWRANGE_FIXED_GOAL;

	retval = ext2fs_new_range(fs, newr_flags, goal, len, रिक्त, ret, &plen);
	यदि (retval)
		प्रत्यागम retval;

	यदि (plen < len)
		प्रत्यागम EXT2_ET_खण्डम्_ALLOC_FAIL;

	यदि (flags & EXT2_ALLOCRANGE_ZERO_खण्डानि) {
		retval = ext2fs_zero_blocks2(fs, *ret, len, रिक्त, रिक्त);
		यदि (retval)
			प्रत्यागम retval;
	}

	ext2fs_खण्डम्_alloc_stats_range(fs, *ret, len, +1);
	प्रत्यागम retval;
}
