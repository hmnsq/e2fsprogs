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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
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
	ext2_ino_t		inodes_बायाँ;
	blk_t			blocks_बायाँ;
	dgrp_t			groups_बायाँ;
	blk_t			inode_buffer_blocks;
	वर्ण *			inode_buffer;
	पूर्णाङ्क			inode_size;
	वर्ण *			ptr;
	पूर्णाङ्क			bytes_बायाँ;
	वर्ण			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      शून्य * priv_data);
	शून्य *			done_group_data;
	पूर्णाङ्क			bad_block_ptr;
	पूर्णाङ्क			scan_flags;
	पूर्णाङ्क			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	यदि (!fs->icache)
		प्रत्यागम 0;

	प्रति (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	प्रत्यागम 0;
}

/*
 * Free the inode cache structure
 */
शून्य ext2fs_free_inode_cache(संरचना ext2_inode_cache *icache)
{
	unsigned i;

	यदि (--icache->refcount)
		प्रत्यागम;
	यदि (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	प्रति (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	यदि (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned पूर्णाङ्क cache_size)
{
	unsigned	i;
	errcode_t	retval;

	यदि (fs->icache)
		प्रत्यागम 0;
	retval = ext2fs_get_mem(परिमाण(संरचना ext2_inode_cache), &fs->icache);
	यदि (retval)
		प्रत्यागम retval;

	memset(fs->icache, 0, परिमाण(संरचना ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	यदि (retval)
		गच्छ errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  परिमाण(संरचना ext2_inode_cache_ent),
				  &fs->icache->cache);
	यदि (retval)
		गच्छ errout;

	प्रति (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		यदि (retval)
			गच्छ errout;
	}

	ext2fs_flush_icache(fs);
	प्रत्यागम 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	प्रत्यागम retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, पूर्णाङ्क buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	यदि (fs->blocksize < 1024)
		प्रत्यागम EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	यदि (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		यदि (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_struct_inode_scan), &scan);
	यदि (retval)
		प्रत्यागम retval;
	memset(scan, 0, परिमाण(संरचना ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_बायाँ = 0;
	scan->current_group = 0;
	scan->groups_बायाँ = fs->group_desc_गिनती - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	यदि (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_गिनती(fs->super)))) {
		ext2fs_free_mem(&scan);
		प्रत्यागम EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_बायाँ = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_बायाँ = scan->fs->inode_blocks_per_group;
	यदि (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		यदि (scan->inodes_बायाँ > unused)
			scan->inodes_बायाँ -= unused;
		अन्यथा
			scan->inodes_बायाँ = 0;
		scan->blocks_बायाँ =
			(scan->inodes_बायाँ +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	यदि (retval) {
		ext2fs_free_mem(&scan);
		प्रत्यागम retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	यदि (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		प्रत्यागम retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	यदि (scan->fs->badblocks && scan->fs->badblocks->अंक)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	यदि (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	प्रत्यागम 0;
}

शून्य ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	यदि (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		प्रत्यागम;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = रिक्त;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = रिक्त;
	ext2fs_free_mem(&scan);
	प्रत्यागम;
}

शून्य ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       शून्य * priv_data),
			       शून्य *done_group_data)
{
	यदि (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		प्रत्यागम;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

पूर्णाङ्क ext2fs_inode_scan_flags(ext2_inode_scan scan, पूर्णाङ्क set_flags,
			    पूर्णाङ्क clear_flags)
{
	पूर्णाङ्क	old_flags;

	यदि (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		प्रत्यागम 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	प्रत्यागम old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
स्थिर errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_बायाँ--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_बायाँ = 0;
	scan->inodes_बायाँ = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_बायाँ = fs->inode_blocks_per_group;
	यदि (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		यदि (scan->inodes_बायाँ > unused)
			scan->inodes_बायाँ -= unused;
		अन्यथा
			scan->inodes_बायाँ = 0;
		scan->blocks_बायाँ =
			(scan->inodes_बायाँ +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	यदि (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_गिनती(fs->super))))
		प्रत्यागम EXT2_ET_GDESC_BAD_INODE_TABLE;
	प्रत्यागम 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    पूर्णाङ्क	group)
{
	scan->current_group = group - 1;
	scan->groups_बायाँ = scan->fs->group_desc_गिनती - group;
	scan->bad_block_ptr = 0;
	प्रत्यागम get_next_blockgroup(scan);
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
	यदि (blk == 0)
		प्रत्यागम 0;

	/* Make sure bad_block_ptr is still valid */
	यदि (scan->bad_block_ptr >= bb->अंक) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		प्रत्यागम 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	यावत् (blk > bb->list[scan->bad_block_ptr]) {
		यदि (++scan->bad_block_ptr >= bb->अंक) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			प्रत्यागम 0;
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
	यदि (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*अंक_blocks = 1;
		यदि (++scan->bad_block_ptr >= bb->अंक)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		प्रत्यागम 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	यदि ((blk + *अंक_blocks) > bb->list[scan->bad_block_ptr])
		*अंक_blocks = (पूर्णाङ्क) (bb->list[scan->bad_block_ptr] - blk);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क block_map_looks_insane(ext2_filsys fs,
				  संरचना ext2_inode_large *inode)
{
	unsigned पूर्णाङ्क i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	यदि ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		प्रत्यागम 0;
	यदि (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		प्रत्यागम 0;
	यदि (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= परिमाण(inode->i_block))
		प्रत्यागम 0;

	/* Unused inodes probably aren't insane */
	यदि (inode->i_links_गिनती == 0)
		प्रत्यागम 0;

	/* See if more than half the block maps are insane */
	प्रति (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		यदि (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_गिनती(fs->super)))
			bad++;
	प्रत्यागम bad > EXT2_N_BLOCKS / 2;
}

स्थिर पूर्णाङ्क extent_head_looks_insane(संरचना ext2_inode_large *inode)
{
	यदि (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					परिमाण(inode->i_block)) == 0)
		प्रत्यागम 0;
	प्रत्यागम 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
स्थिर शून्य check_inode_block_sanity(ext2_inode_scan scan, blk64_t अंक_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned पूर्णाङ्क	badness, checksum_failures;
	unsigned पूर्णाङ्क	inodes_in_buf, inodes_per_block;
	वर्ण		*p;
	संरचना ext2_inode_large *inode;
	वर्ण		*block_status;
	unsigned पूर्णाङ्क	blk, bad_csum;

	यदि (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		प्रत्यागम;

	inodes_to_scan = scan->inodes_बायाँ;
	inodes_in_buf = अंक_blocks * scan->fs->blocksize / scan->inode_size;
	यदि (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (वर्ण *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	यदि (inodes_per_block < 2)
		प्रत्यागम;

#ifdef शब्दगण_BIGENDIAN
	यदि (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		प्रत्यागम;
#endif

	यावत् (inodes_to_scan > 0) {
		blk = (p - (वर्ण *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(संरचना ext2_inode_large *) p) == 0;

#ifdef शब्दगण_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (संरचना ext2_inode_large *) inode,
			       (संरचना ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#अन्यथा
		inode = (संरचना ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		यदि (bad_csum) {
			checksum_failures++;
			badness++;
		} अन्यथा यदि (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		यदि (badness > inodes_per_block / 2) {
			unsigned पूर्णाङ्क ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			यदि (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			अनुवर्त;
		}

		यदि ((ino % inodes_per_block) == 0) {
			यदि (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef शब्दगण_BIGENDIAN
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
	यदि (अंक_blocks > scan->blocks_बायाँ)
		अंक_blocks = scan->blocks_बायाँ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	यदि (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		यदि (scan->bytes_बायाँ)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	यदि (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &अंक_blocks);
		यदि (retval)
			प्रत्यागम retval;
	}

	यदि ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) अंक_blocks * scan->fs->blocksize);
	} अन्यथा {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (पूर्णाङ्क) अंक_blocks,
					     scan->inode_buffer);
		यदि (retval)
			प्रत्यागम EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, अंक_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_बायाँ = अंक_blocks * scan->fs->blocksize;

	scan->blocks_बायाँ -= अंक_blocks;
	यदि (scan->current_block)
		scan->current_block += अंक_blocks;

	प्रत्यागम 0;
}

#यदि 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
स्थिर अन्तःस्थ पूर्णाङ्क is_empty_scan(ext2_inode_scan scan)
{
	पूर्णाङ्क	i;

	यदि (scan->bytes_बायाँ == 0)
		प्रत्यागम 0;

	प्रति (i=0; i < scan->bytes_बायाँ; i++)
		यदि (scan->ptr[i])
			प्रत्यागम 0;
	प्रत्यागम 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     संरचना ext2_inode *inode, पूर्णाङ्क bufsize)
{
	errcode_t	retval;
	पूर्णाङ्क		extra_bytes = 0;
	पूर्णाङ्क		length;
	संरचना ext2_inode_large	*iptr = (संरचना ext2_inode_large *)inode;
	वर्ण		*iblock_status;
	unsigned पूर्णाङ्क	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	यदि (scan->inodes_बायाँ <= 0) {
	force_new_group:
		यदि (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			यदि (retval)
				प्रत्यागम retval;
		}
		यदि (scan->groups_बायाँ <= 0) {
			*ino = 0;
			प्रत्यागम 0;
		}
		retval = get_next_blockgroup(scan);
		यदि (retval)
			प्रत्यागम retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	यदि ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		गच्छ force_new_group;
	यदि (scan->inodes_बायाँ == 0)
		गच्छ force_new_group;
	यदि (scan->current_block == 0) {
		यदि (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			गच्छ force_new_group;
		} अन्यथा
			प्रत्यागम EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	यदि (scan->bytes_बायाँ < scan->inode_size) {
		यदि (scan->bytes_बायाँ)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_बायाँ);
		extra_bytes = scan->bytes_बायाँ;

		retval = get_next_blocks(scan);
		यदि (retval)
			प्रत्यागम retval;
#यदि 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		यदि (is_empty_scan(scan))
			गच्छ force_new_group;
#endif
	}

	यदि (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		यदि (retval)
			प्रत्यागम retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	यदि (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_बायाँ -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		यदि (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(संरचना ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef शब्दगण_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (संरचना ext2_inode_large *) iptr,
			       (संरचना ext2_inode_large *) scan->temp_buffer,
			       0, length);
#अन्यथा
		memcpy(iptr, scan->temp_buffer, length);
#endif
		यदि (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} अन्यथा {
		/* Verify the inode checksum. */
		यदि (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(संरचना ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef शब्दगण_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(संरचना ext2_inode_large *) iptr,
				(संरचना ext2_inode_large *) scan->ptr,
				0, length);
#अन्यथा
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_बायाँ -= scan->inode_size;
		यदि (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	यदि ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_बायाँ--;
	scan->current_inode++;
	*ino = scan->current_inode;
	यदि (iptr != (संरचना ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	प्रत्यागम retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				संरचना ext2_inode *inode)
{
	प्रत्यागम ext2fs_get_next_inode_full(scan, ino, inode,
						परिमाण(संरचना ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode * inode, पूर्णाङ्क bufsize,
			     पूर्णाङ्क flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	वर्ण 		*ptr;
	errcode_t	retval;
	unsigned	i;
	पूर्णाङ्क		clen, inodes_per_block;
	io_channel	io;
	पूर्णाङ्क		length = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large	*iptr;
	पूर्णाङ्क		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	यदि (fs->blocksize < 1024)
		प्रत्यागम EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	यदि (fs->read_inode &&
	    ((bufsize == परिमाण(संरचना ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == परिमाण(संरचना ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		यदि (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			प्रत्यागम retval;
	}
	यदि ((ino == 0) || (ino > fs->super->s_inodes_गिनती))
		प्रत्यागम EXT2_ET_BAD_INODE_अंक;
	/* Create inode cache if not present */
	यदि (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		यदि (retval)
			प्रत्यागम retval;
	}
	/* Check to see if it's in the inode cache */
	प्रति (i = 0; i < fs->icache->cache_size; i++) {
		यदि (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			प्रत्यागम 0;
		}
	}
	यदि (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} अन्यथा {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		यदि (group > fs->group_desc_गिनती)
			प्रत्यागम EXT2_ET_BAD_INODE_अंक;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		यदि (!block_nr)
			प्रत्यागम EXT2_ET_MISSING_INODE_TABLE;
		यदि ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_गिनती(fs->super)))
			प्रत्यागम EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (संरचना ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (वर्ण *) iptr;
	यावत् (length) {
		clen = length;
		यदि ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		यदि (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			यदि (retval)
				प्रत्यागम retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((वर्ण *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef शब्दगण_BIGENDIAN
	ext2fs_swap_inode_full(fs, (संरचना ext2_inode_large *) iptr,
			       (संरचना ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	यदि (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	यदि (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		प्रत्यागम EXT2_ET_INODE_CSUM_INVALID;

	प्रत्यागम 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode * inode, पूर्णाङ्क bufsize)
{
	प्रत्यागम ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode * inode)
{
	प्रत्यागम ext2fs_read_inode2(fs, ino, inode,
				  परिमाण(संरचना ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode * inode, पूर्णाङ्क bufsize,
			      पूर्णाङ्क flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	संरचना ext2_inode_large *w_inode;
	वर्ण *ptr;
	unsigned i;
	पूर्णाङ्क clen;
	पूर्णाङ्क length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	यदि (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		यदि (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			प्रत्यागम retval;
	}

	यदि ((ino == 0) || (ino > fs->super->s_inodes_गिनती))
		प्रत्यागम EXT2_ET_BAD_INODE_अंक;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	यदि (retval)
		प्रत्यागम retval;

	यदि (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (संरचना ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		यदि (retval)
			गच्छ errout;
	}

	/* Check to see if the inode cache needs to be updated */
	यदि (fs->icache) {
		प्रति (i=0; i < fs->icache->cache_size; i++) {
			यदि (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				भङ्ग;
			}
		}
	} अन्यथा {
		retval = ext2fs_create_inode_cache(fs, 4);
		यदि (retval)
			गच्छ errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	यदि (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		गच्छ errout;
	}

#ifdef शब्दगण_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	यदि ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		यदि (retval)
			गच्छ errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	यदि (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		गच्छ errout;
	}
	यदि ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_गिनती(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		गच्छ errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (वर्ण *) w_inode;

	यावत् (length) {
		clen = length;
		यदि ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		यदि (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			यदि (retval)
				गच्छ errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((वर्ण *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		यदि (retval)
			गच्छ errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	प्रत्यागम retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  संरचना ext2_inode * inode, पूर्णाङ्क bufsize)
{
	प्रत्यागम ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode)
{
	प्रत्यागम ext2fs_write_inode2(fs, ino, inode,
				   परिमाण(संरचना ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode *inode)
{
	संरचना ext2_inode	*buf;
	पूर्णाङ्क			size = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	यदि (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	यदि (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	यदि (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	यदि (size == परिमाण(संरचना ext2_inode))
		प्रत्यागम ext2fs_write_inode_full(fs, ino, inode,
					       परिमाण(संरचना ext2_inode));

	buf = malloc(size);
	यदि (!buf)
		प्रत्यागम ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (संरचना ext2_inode_large *) buf;
	large_inode->i_extra_isize = परिमाण(संरचना ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	यदि (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	मुक्त(buf);
	प्रत्यागम retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	संरचना ext2_inode	inode;
	पूर्णाङ्क			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ino > fs->super->s_inodes_गिनती)
		प्रत्यागम EXT2_ET_BAD_INODE_अंक;

	यदि (fs->get_blocks) {
		यदि (!(*fs->get_blocks)(fs, ino, blocks))
			प्रत्यागम 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	यदि (retval)
		प्रत्यागम retval;
	प्रति (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	प्रत्यागम 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ino > fs->super->s_inodes_गिनती)
		प्रत्यागम EXT2_ET_BAD_INODE_अंक;

	यदि (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		यदि (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			प्रत्यागम retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	यदि (retval)
		प्रत्यागम retval;
	यदि (!LINUX_S_ISDIR(inode.i_mode))
		प्रत्यागम EXT2_ET_NO_DIRECTORY;
	प्रत्यागम 0;
}

