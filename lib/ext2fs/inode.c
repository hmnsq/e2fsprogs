/*
 * inode.c --- utility routines to read and write inodes
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

संरचना ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_डावा;
	blk_t			blocks_डावा;
	dgrp_t			groups_डावा;
	blk_t			inode_buffer_blocks;
	अक्षर *			inode_buffer;
	पूर्णांक			inode_size;
	अक्षर *			ptr;
	पूर्णांक			bytes_डावा;
	अक्षर			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      शून्य * priv_data);
	शून्य *			done_group_data;
	पूर्णांक			bad_block_ptr;
	पूर्णांक			scan_flags;
	पूर्णांक			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	अचिन्हित	i;

	जर (!fs->icache)
		परत 0;

	साठी (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	परत 0;
}

/*
 * Free the inode cache structure
 */
शून्य ext2fs_free_inode_cache(संरचना ext2_inode_cache *icache)
{
	अचिन्हित i;

	जर (--icache->refcount)
		परत;
	जर (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	साठी (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	जर (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, अचिन्हित पूर्णांक cache_size)
{
	अचिन्हित	i;
	errcode_t	retval;

	जर (fs->icache)
		परत 0;
	retval = ext2fs_get_mem(आकार(संरचना ext2_inode_cache), &fs->icache);
	जर (retval)
		परत retval;

	स_रखो(fs->icache, 0, आकार(संरचना ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	जर (retval)
		जा errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  आकार(संरचना ext2_inode_cache_ent),
				  &fs->icache->cache);
	जर (retval)
		जा errout;

	साठी (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		जर (retval)
			जा errout;
	}

	ext2fs_flush_icache(fs);
	परत 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	परत retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, पूर्णांक buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर (ext2fs_has_feature_journal_dev(fs->super))
		परत EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	जर (fs->blocksize < 1024)
		परत EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	जर (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		जर (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(आकार(संरचना ext2_struct_inode_scan), &scan);
	जर (retval)
		परत retval;
	स_रखो(scan, 0, आकार(संरचना ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_डावा = 0;
	scan->current_group = 0;
	scan->groups_डावा = fs->group_desc_मोजणी - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	जर (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_मोजणी(fs->super)))) {
		ext2fs_free_mem(&scan);
		परत EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_डावा = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_डावा = scan->fs->inode_blocks_per_group;
	जर (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		जर (scan->inodes_डावा > unused)
			scan->inodes_डावा -= unused;
		अन्यथा
			scan->inodes_डावा = 0;
		scan->blocks_डावा =
			(scan->inodes_डावा +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	जर (retval) {
		ext2fs_free_mem(&scan);
		परत retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	जर (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		परत retval;
	}
	स_रखो(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	जर (scan->fs->badblocks && scan->fs->badblocks->अंक)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	जर (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	परत 0;
}

शून्य ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	जर (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		परत;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = रिक्त;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = रिक्त;
	ext2fs_free_mem(&scan);
	परत;
}

शून्य ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       शून्य * priv_data),
			       शून्य *done_group_data)
{
	जर (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		परत;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

पूर्णांक ext2fs_inode_scan_flags(ext2_inode_scan scan, पूर्णांक set_flags,
			    पूर्णांक clear_flags)
{
	पूर्णांक	old_flags;

	जर (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		परत 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	परत old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
स्थिर errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_डावा--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_डावा = 0;
	scan->inodes_डावा = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_डावा = fs->inode_blocks_per_group;
	जर (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		जर (scan->inodes_डावा > unused)
			scan->inodes_डावा -= unused;
		अन्यथा
			scan->inodes_डावा = 0;
		scan->blocks_डावा =
			(scan->inodes_डावा +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	जर (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_मोजणी(fs->super))))
		परत EXT2_ET_GDESC_BAD_INODE_TABLE;
	परत 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    पूर्णांक	group)
{
	scan->current_group = group - 1;
	scan->groups_डावा = scan->fs->group_desc_मोजणी - group;
	scan->bad_block_ptr = 0;
	परत get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
स्थिर errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *अंक_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	जर (blk == 0)
		परत 0;

	/* Make sure bad_block_ptr is still valid */
	जर (scan->bad_block_ptr >= bb->अंक) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		परत 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	पर्यंत (blk > bb->list[scan->bad_block_ptr]) {
		जर (++scan->bad_block_ptr >= bb->अंक) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			परत 0;
		}
	}

	/*
	 * If the current block is equal to the bad block listed in
	 * the bad block list, then handle that one block specially.
	 * (We could try to handle runs of bad blocks, but that
	 * only increases CPU efficiency by a small amount, at the
	 * expense of a huge expense of code complexity, and for an
	 * uncommon case at that.)
	 */
	जर (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*अंक_blocks = 1;
		जर (++scan->bad_block_ptr >= bb->अंक)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		परत 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	जर ((blk + *अंक_blocks) > bb->list[scan->bad_block_ptr])
		*अंक_blocks = (पूर्णांक) (bb->list[scan->bad_block_ptr] - blk);

	परत 0;
}

स्थिर पूर्णांक block_map_looks_insane(ext2_filsys fs,
				  संरचना ext2_inode_large *inode)
{
	अचिन्हित पूर्णांक i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	जर ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		परत 0;
	जर (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		परत 0;
	जर (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= आकार(inode->i_block))
		परत 0;

	/* Unused inodes probably aren't insane */
	जर (inode->i_links_मोजणी == 0)
		परत 0;

	/* See if more than half the block maps are insane */
	साठी (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		जर (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_मोजणी(fs->super)))
			bad++;
	परत bad > EXT2_N_BLOCKS / 2;
}

स्थिर पूर्णांक extent_head_looks_insane(संरचना ext2_inode_large *inode)
{
	जर (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					आकार(inode->i_block)) == 0)
		परत 0;
	परत 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
स्थिर शून्य check_inode_block_sanity(ext2_inode_scan scan, blk64_t अंक_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	अचिन्हित पूर्णांक	badness, checksum_failures;
	अचिन्हित पूर्णांक	inodes_in_buf, inodes_per_block;
	अक्षर		*p;
	संरचना ext2_inode_large *inode;
	अक्षर		*block_status;
	अचिन्हित पूर्णांक	blk, bad_csum;

	जर (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		परत;

	inodes_to_scan = scan->inodes_डावा;
	inodes_in_buf = अंक_blocks * scan->fs->blocksize / scan->inode_size;
	जर (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (अक्षर *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	स_रखो(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	जर (inodes_per_block < 2)
		परत;

#ifdef शब्द_BIGENDIAN
	जर (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		परत;
#endif

	पर्यंत (inodes_to_scan > 0) {
		blk = (p - (अक्षर *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(संरचना ext2_inode_large *) p) == 0;

#ifdef शब्द_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (संरचना ext2_inode_large *) inode,
			       (संरचना ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#अन्यथा
		inode = (संरचना ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		जर (bad_csum) {
			checksum_failures++;
			badness++;
		} अन्यथा जर (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		जर (badness > inodes_per_block / 2) {
			अचिन्हित पूर्णांक ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			जर (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			सुरूठेवा;
		}

		जर ((ino % inodes_per_block) == 0) {
			जर (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef शब्द_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
स्थिर errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		अंक_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	अंक_blocks = scan->inode_buffer_blocks;
	जर (अंक_blocks > scan->blocks_डावा)
		अंक_blocks = scan->blocks_डावा;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	जर (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		जर (scan->bytes_डावा)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	जर (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &अंक_blocks);
		जर (retval)
			परत retval;
	}

	जर ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		स_रखो(scan->inode_buffer, 0,
		       (माप_प्रकार) अंक_blocks * scan->fs->blocksize);
	} अन्यथा {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (पूर्णांक) अंक_blocks,
					     scan->inode_buffer);
		जर (retval)
			परत EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, अंक_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_डावा = अंक_blocks * scan->fs->blocksize;

	scan->blocks_डावा -= अंक_blocks;
	जर (scan->current_block)
		scan->current_block += अंक_blocks;

	परत 0;
}

#जर 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
स्थिर ओळीत पूर्णांक is_empty_scan(ext2_inode_scan scan)
{
	पूर्णांक	i;

	जर (scan->bytes_डावा == 0)
		परत 0;

	साठी (i=0; i < scan->bytes_डावा; i++)
		जर (scan->ptr[i])
			परत 0;
	परत 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     संरचना ext2_inode *inode, पूर्णांक bufsize)
{
	errcode_t	retval;
	पूर्णांक		extra_bytes = 0;
	पूर्णांक		length;
	संरचना ext2_inode_large	*iptr = (संरचना ext2_inode_large *)inode;
	अक्षर		*iblock_status;
	अचिन्हित पूर्णांक	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	जर (scan->inodes_डावा <= 0) {
	force_new_group:
		जर (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			जर (retval)
				परत retval;
		}
		जर (scan->groups_डावा <= 0) {
			*ino = 0;
			परत 0;
		}
		retval = get_next_blockgroup(scan);
		जर (retval)
			परत retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	जर ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		जा force_new_group;
	जर (scan->inodes_डावा == 0)
		जा force_new_group;
	जर (scan->current_block == 0) {
		जर (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			जा force_new_group;
		} अन्यथा
			परत EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	जर (scan->bytes_डावा < scan->inode_size) {
		जर (scan->bytes_डावा)
			स_नकल(scan->temp_buffer, scan->ptr, scan->bytes_डावा);
		extra_bytes = scan->bytes_डावा;

		retval = get_next_blocks(scan);
		जर (retval)
			परत retval;
#जर 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		जर (is_empty_scan(scan))
			जा force_new_group;
#endif
	}

	जर (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		जर (retval)
			परत retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	जर (extra_bytes) {
		स_नकल(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_डावा -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		जर (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(संरचना ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef शब्द_BIGENDIAN
		स_रखो(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (संरचना ext2_inode_large *) iptr,
			       (संरचना ext2_inode_large *) scan->temp_buffer,
			       0, length);
#अन्यथा
		स_नकल(iptr, scan->temp_buffer, length);
#endif
		जर (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} अन्यथा {
		/* Verify the inode checksum. */
		जर (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(संरचना ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef शब्द_BIGENDIAN
		स_रखो(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(संरचना ext2_inode_large *) iptr,
				(संरचना ext2_inode_large *) scan->ptr,
				0, length);
#अन्यथा
		स_नकल(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_डावा -= scan->inode_size;
		जर (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	जर ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_डावा--;
	scan->current_inode++;
	*ino = scan->current_inode;
	जर (iptr != (संरचना ext2_inode_large *)inode) {
		स_नकल(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	परत retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				संरचना ext2_inode *inode)
{
	परत ext2fs_get_next_inode_full(scan, ino, inode,
						आकार(संरचना ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode * inode, पूर्णांक bufsize,
			     पूर्णांक flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	अचिन्हित दीर्घ 	block, offset;
	अक्षर 		*ptr;
	errcode_t	retval;
	अचिन्हित	i;
	पूर्णांक		clen, inodes_per_block;
	io_channel	io;
	पूर्णांक		length = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large	*iptr;
	पूर्णांक		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर (ext2fs_has_feature_journal_dev(fs->super))
		परत EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	जर (fs->blocksize < 1024)
		परत EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	जर (fs->read_inode &&
	    ((bufsize == आकार(संरचना ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == आकार(संरचना ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		जर (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			परत retval;
	}
	जर ((ino == 0) || (ino > fs->super->s_inodes_मोजणी))
		परत EXT2_ET_BAD_INODE_अंक;
	/* Create inode cache if not present */
	जर (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		जर (retval)
			परत retval;
	}
	/* Check to see if it's in the inode cache */
	साठी (i = 0; i < fs->icache->cache_size; i++) {
		जर (fs->icache->cache[i].ino == ino) {
			स_नकल(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			परत 0;
		}
	}
	जर (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} अन्यथा {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		जर (group > fs->group_desc_मोजणी)
			परत EXT2_ET_BAD_INODE_अंक;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		जर (!block_nr)
			परत EXT2_ET_MISSING_INODE_TABLE;
		जर ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_मोजणी(fs->super)))
			परत EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (संरचना ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (अक्षर *) iptr;
	पर्यंत (length) {
		clen = length;
		जर ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		जर (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			जर (retval)
				परत retval;
			fs->icache->buffer_blk = block_nr;
		}

		स_नकल(ptr, ((अक्षर *) fs->icache->buffer) + (अचिन्हित) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef शब्द_BIGENDIAN
	ext2fs_swap_inode_full(fs, (संरचना ext2_inode_large *) iptr,
			       (संरचना ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	जर (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	स_नकल(inode, iptr, (bufsize > length) ? length : bufsize);

	जर (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		परत EXT2_ET_INODE_CSUM_INVALID;

	परत 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode * inode, पूर्णांक bufsize)
{
	परत ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode * inode)
{
	परत ext2fs_read_inode2(fs, ino, inode,
				  आकार(संरचना ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode * inode, पूर्णांक bufsize,
			      पूर्णांक flags)
{
	blk64_t block_nr;
	dgrp_t group;
	अचिन्हित दीर्घ block, offset;
	errcode_t retval = 0;
	संरचना ext2_inode_large *w_inode;
	अक्षर *ptr;
	अचिन्हित i;
	पूर्णांक clen;
	पूर्णांक length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर (ext2fs_has_feature_journal_dev(fs->super))
		परत EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	जर (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		जर (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			परत retval;
	}

	जर ((ino == 0) || (ino > fs->super->s_inodes_मोजणी))
		परत EXT2_ET_BAD_INODE_अंक;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	जर (retval)
		परत retval;

	जर (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (संरचना ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		जर (retval)
			जा errout;
	}

	/* Check to see if the inode cache needs to be updated */
	जर (fs->icache) {
		साठी (i=0; i < fs->icache->cache_size; i++) {
			जर (fs->icache->cache[i].ino == ino) {
				स_नकल(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				थांबा;
			}
		}
	} अन्यथा {
		retval = ext2fs_create_inode_cache(fs, 4);
		जर (retval)
			जा errout;
	}
	स_नकल(w_inode, inode, (bufsize > length) ? length : bufsize);

	जर (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		जा errout;
	}

#ifdef शब्द_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	जर ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		जर (retval)
			जा errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (अचिन्हित) group);
	जर (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		जा errout;
	}
	जर ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_मोजणी(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		जा errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (अक्षर *) w_inode;

	पर्यंत (length) {
		clen = length;
		जर ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		जर (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			जर (retval)
				जा errout;
			fs->icache->buffer_blk = block_nr;
		}


		स_नकल((अक्षर *) fs->icache->buffer + (अचिन्हित) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		जर (retval)
			जा errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	परत retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  संरचना ext2_inode * inode, पूर्णांक bufsize)
{
	परत ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode)
{
	परत ext2fs_write_inode2(fs, ino, inode,
				   आकार(संरचना ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode *inode)
{
	संरचना ext2_inode	*buf;
	पूर्णांक			size = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large	*large_inode;
	errcode_t		retval;
	समय_प्रकार			t = ext2fsP_get_time(fs);

	जर (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	जर (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	जर (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	जर (size == आकार(संरचना ext2_inode))
		परत ext2fs_write_inode_full(fs, ino, inode,
					       आकार(संरचना ext2_inode));

	buf = दो_स्मृति(size);
	जर (!buf)
		परत ENOMEM;

	स_रखो(buf, 0, size);
	*buf = *inode;

	large_inode = (संरचना ext2_inode_large *) buf;
	large_inode->i_extra_isize = आकार(संरचना ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	जर (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	मुक्त(buf);
	परत retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	संरचना ext2_inode	inode;
	पूर्णांक			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर (ino > fs->super->s_inodes_मोजणी)
		परत EXT2_ET_BAD_INODE_अंक;

	जर (fs->get_blocks) {
		जर (!(*fs->get_blocks)(fs, ino, blocks))
			परत 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	जर (retval)
		परत retval;
	साठी (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	परत 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	जर (ino > fs->super->s_inodes_मोजणी)
		परत EXT2_ET_BAD_INODE_अंक;

	जर (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		जर (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			परत retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	जर (retval)
		परत retval;
	जर (!LINUX_S_ISDIR(inode.i_mode))
		परत EXT2_ET_NO_DIRECTORY;
	परत 0;
}

