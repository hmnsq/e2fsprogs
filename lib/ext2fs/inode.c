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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"
#அந்தர்பாவ "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

கட்டமைப்பு ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_இடது;
	blk_t			blocks_இடது;
	dgrp_t			groups_இடது;
	blk_t			inode_buffer_blocks;
	எழுத்து *			inode_buffer;
	முழு			inode_size;
	எழுத்து *			ptr;
	முழு			bytes_இடது;
	எழுத்து			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      வெறுமை * priv_data);
	வெறுமை *			done_group_data;
	முழு			bad_block_ptr;
	முழு			scan_flags;
	முழு			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	எனில் (!fs->icache)
		திரும்பு 0;

	க்கு (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	திரும்பு 0;
}

/*
 * Free the inode cache structure
 */
வெறுமை ext2fs_free_inode_cache(கட்டமைப்பு ext2_inode_cache *icache)
{
	unsigned i;

	எனில் (--icache->refcount)
		திரும்பு;
	எனில் (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	க்கு (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	எனில் (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned முழு cache_size)
{
	unsigned	i;
	errcode_t	retval;

	எனில் (fs->icache)
		திரும்பு 0;
	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_inode_cache), &fs->icache);
	எனில் (retval)
		திரும்பு retval;

	memset(fs->icache, 0, அளவு(கட்டமைப்பு ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	எனில் (retval)
		செல் errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  அளவு(கட்டமைப்பு ext2_inode_cache_ent),
				  &fs->icache->cache);
	எனில் (retval)
		செல் errout;

	க்கு (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		எனில் (retval)
			செல் errout;
	}

	ext2fs_flush_icache(fs);
	திரும்பு 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	திரும்பு retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, முழு buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (ext2fs_has_feature_journal_dev(fs->super))
		திரும்பு EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	எனில் (fs->blocksize < 1024)
		திரும்பு EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	எனில் (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		எனில் (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_struct_inode_scan), &scan);
	எனில் (retval)
		திரும்பு retval;
	memset(scan, 0, அளவு(கட்டமைப்பு ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_இடது = 0;
	scan->current_group = 0;
	scan->groups_இடது = fs->group_desc_மொத்தம் - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	எனில் (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_மொத்தம்(fs->super)))) {
		ext2fs_free_mem(&scan);
		திரும்பு EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_இடது = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_இடது = scan->fs->inode_blocks_per_group;
	எனில் (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		எனில் (scan->inodes_இடது > unused)
			scan->inodes_இடது -= unused;
		இல்லையெனில்
			scan->inodes_இடது = 0;
		scan->blocks_இடது =
			(scan->inodes_இடது +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	எனில் (retval) {
		ext2fs_free_mem(&scan);
		திரும்பு retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	எனில் (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		திரும்பு retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	எனில் (scan->fs->badblocks && scan->fs->badblocks->இலக்கம்)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	எனில் (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	திரும்பு 0;
}

வெறுமை ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	எனில் (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		திரும்பு;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = வெற்று;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = வெற்று;
	ext2fs_free_mem(&scan);
	திரும்பு;
}

வெறுமை ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       வெறுமை * priv_data),
			       வெறுமை *done_group_data)
{
	எனில் (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		திரும்பு;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

முழு ext2fs_inode_scan_flags(ext2_inode_scan scan, முழு set_flags,
			    முழு clear_flags)
{
	முழு	old_flags;

	எனில் (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		திரும்பு 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	திரும்பு old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
நிலையான errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_இடது--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_இடது = 0;
	scan->inodes_இடது = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_இடது = fs->inode_blocks_per_group;
	எனில் (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		எனில் (scan->inodes_இடது > unused)
			scan->inodes_இடது -= unused;
		இல்லையெனில்
			scan->inodes_இடது = 0;
		scan->blocks_இடது =
			(scan->inodes_இடது +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	எனில் (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_மொத்தம்(fs->super))))
		திரும்பு EXT2_ET_GDESC_BAD_INODE_TABLE;
	திரும்பு 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    முழு	group)
{
	scan->current_group = group - 1;
	scan->groups_இடது = scan->fs->group_desc_மொத்தம் - group;
	scan->bad_block_ptr = 0;
	திரும்பு get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
நிலையான errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *இலக்கம்_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	எனில் (blk == 0)
		திரும்பு 0;

	/* Make sure bad_block_ptr is still valid */
	எனில் (scan->bad_block_ptr >= bb->இலக்கம்) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		திரும்பு 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	போது (blk > bb->list[scan->bad_block_ptr]) {
		எனில் (++scan->bad_block_ptr >= bb->இலக்கம்) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			திரும்பு 0;
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
	எனில் (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*இலக்கம்_blocks = 1;
		எனில் (++scan->bad_block_ptr >= bb->இலக்கம்)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		திரும்பு 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	எனில் ((blk + *இலக்கம்_blocks) > bb->list[scan->bad_block_ptr])
		*இலக்கம்_blocks = (முழு) (bb->list[scan->bad_block_ptr] - blk);

	திரும்பு 0;
}

நிலையான முழு block_map_looks_insane(ext2_filsys fs,
				  கட்டமைப்பு ext2_inode_large *inode)
{
	unsigned முழு i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	எனில் ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		திரும்பு 0;
	எனில் (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		திரும்பு 0;
	எனில் (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= அளவு(inode->i_block))
		திரும்பு 0;

	/* Unused inodes probably aren't insane */
	எனில் (inode->i_links_மொத்தம் == 0)
		திரும்பு 0;

	/* See if more than half the block maps are insane */
	க்கு (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		எனில் (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_மொத்தம்(fs->super)))
			bad++;
	திரும்பு bad > EXT2_N_BLOCKS / 2;
}

நிலையான முழு extent_head_looks_insane(கட்டமைப்பு ext2_inode_large *inode)
{
	எனில் (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					அளவு(inode->i_block)) == 0)
		திரும்பு 0;
	திரும்பு 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
நிலையான வெறுமை check_inode_block_sanity(ext2_inode_scan scan, blk64_t இலக்கம்_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned முழு	badness, checksum_failures;
	unsigned முழு	inodes_in_buf, inodes_per_block;
	எழுத்து		*p;
	கட்டமைப்பு ext2_inode_large *inode;
	எழுத்து		*block_status;
	unsigned முழு	blk, bad_csum;

	எனில் (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		திரும்பு;

	inodes_to_scan = scan->inodes_இடது;
	inodes_in_buf = இலக்கம்_blocks * scan->fs->blocksize / scan->inode_size;
	எனில் (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (எழுத்து *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	எனில் (inodes_per_block < 2)
		திரும்பு;

#ifdef சொற்கள்_BIGENDIAN
	எனில் (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		திரும்பு;
#endif

	போது (inodes_to_scan > 0) {
		blk = (p - (எழுத்து *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(கட்டமைப்பு ext2_inode_large *) p) == 0;

#ifdef சொற்கள்_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (கட்டமைப்பு ext2_inode_large *) inode,
			       (கட்டமைப்பு ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#இல்லையெனில்
		inode = (கட்டமைப்பு ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		எனில் (bad_csum) {
			checksum_failures++;
			badness++;
		} இல்லையெனில் எனில் (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		எனில் (badness > inodes_per_block / 2) {
			unsigned முழு ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			எனில் (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			தொடர்;
		}

		எனில் ((ino % inodes_per_block) == 0) {
			எனில் (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef சொற்கள்_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
நிலையான errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		இலக்கம்_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	இலக்கம்_blocks = scan->inode_buffer_blocks;
	எனில் (இலக்கம்_blocks > scan->blocks_இடது)
		இலக்கம்_blocks = scan->blocks_இடது;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	எனில் (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		எனில் (scan->bytes_இடது)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	எனில் (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &இலக்கம்_blocks);
		எனில் (retval)
			திரும்பு retval;
	}

	எனில் ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) இலக்கம்_blocks * scan->fs->blocksize);
	} இல்லையெனில் {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (முழு) இலக்கம்_blocks,
					     scan->inode_buffer);
		எனில் (retval)
			திரும்பு EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, இலக்கம்_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_இடது = இலக்கம்_blocks * scan->fs->blocksize;

	scan->blocks_இடது -= இலக்கம்_blocks;
	எனில் (scan->current_block)
		scan->current_block += இலக்கம்_blocks;

	திரும்பு 0;
}

#எனில் 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
நிலையான உள்வரி முழு is_empty_scan(ext2_inode_scan scan)
{
	முழு	i;

	எனில் (scan->bytes_இடது == 0)
		திரும்பு 0;

	க்கு (i=0; i < scan->bytes_இடது; i++)
		எனில் (scan->ptr[i])
			திரும்பு 0;
	திரும்பு 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     கட்டமைப்பு ext2_inode *inode, முழு bufsize)
{
	errcode_t	retval;
	முழு		extra_bytes = 0;
	முழு		length;
	கட்டமைப்பு ext2_inode_large	*iptr = (கட்டமைப்பு ext2_inode_large *)inode;
	எழுத்து		*iblock_status;
	unsigned முழு	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	எனில் (scan->inodes_இடது <= 0) {
	force_new_group:
		எனில் (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			எனில் (retval)
				திரும்பு retval;
		}
		எனில் (scan->groups_இடது <= 0) {
			*ino = 0;
			திரும்பு 0;
		}
		retval = get_next_blockgroup(scan);
		எனில் (retval)
			திரும்பு retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	எனில் ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		செல் force_new_group;
	எனில் (scan->inodes_இடது == 0)
		செல் force_new_group;
	எனில் (scan->current_block == 0) {
		எனில் (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			செல் force_new_group;
		} இல்லையெனில்
			திரும்பு EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	எனில் (scan->bytes_இடது < scan->inode_size) {
		எனில் (scan->bytes_இடது)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_இடது);
		extra_bytes = scan->bytes_இடது;

		retval = get_next_blocks(scan);
		எனில் (retval)
			திரும்பு retval;
#எனில் 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		எனில் (is_empty_scan(scan))
			செல் force_new_group;
#endif
	}

	எனில் (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		எனில் (retval)
			திரும்பு retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	எனில் (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_இடது -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		எனில் (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(கட்டமைப்பு ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef சொற்கள்_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (கட்டமைப்பு ext2_inode_large *) iptr,
			       (கட்டமைப்பு ext2_inode_large *) scan->temp_buffer,
			       0, length);
#இல்லையெனில்
		memcpy(iptr, scan->temp_buffer, length);
#endif
		எனில் (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} இல்லையெனில் {
		/* Verify the inode checksum. */
		எனில் (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(கட்டமைப்பு ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef சொற்கள்_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(கட்டமைப்பு ext2_inode_large *) iptr,
				(கட்டமைப்பு ext2_inode_large *) scan->ptr,
				0, length);
#இல்லையெனில்
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_இடது -= scan->inode_size;
		எனில் (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	எனில் ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_இடது--;
	scan->current_inode++;
	*ino = scan->current_inode;
	எனில் (iptr != (கட்டமைப்பு ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	திரும்பு retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				கட்டமைப்பு ext2_inode *inode)
{
	திரும்பு ext2fs_get_next_inode_full(scan, ino, inode,
						அளவு(கட்டமைப்பு ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     கட்டமைப்பு ext2_inode * inode, முழு bufsize,
			     முழு flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	எழுத்து 		*ptr;
	errcode_t	retval;
	unsigned	i;
	முழு		clen, inodes_per_block;
	io_channel	io;
	முழு		length = EXT2_INODE_SIZE(fs->super);
	கட்டமைப்பு ext2_inode_large	*iptr;
	முழு		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (ext2fs_has_feature_journal_dev(fs->super))
		திரும்பு EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	எனில் (fs->blocksize < 1024)
		திரும்பு EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	எனில் (fs->read_inode &&
	    ((bufsize == அளவு(கட்டமைப்பு ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == அளவு(கட்டமைப்பு ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		எனில் (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			திரும்பு retval;
	}
	எனில் ((ino == 0) || (ino > fs->super->s_inodes_மொத்தம்))
		திரும்பு EXT2_ET_BAD_INODE_இலக்கம்;
	/* Create inode cache if not present */
	எனில் (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		எனில் (retval)
			திரும்பு retval;
	}
	/* Check to see if it's in the inode cache */
	க்கு (i = 0; i < fs->icache->cache_size; i++) {
		எனில் (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			திரும்பு 0;
		}
	}
	எனில் (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} இல்லையெனில் {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		எனில் (group > fs->group_desc_மொத்தம்)
			திரும்பு EXT2_ET_BAD_INODE_இலக்கம்;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		எனில் (!block_nr)
			திரும்பு EXT2_ET_MISSING_INODE_TABLE;
		எனில் ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_மொத்தம்(fs->super)))
			திரும்பு EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (கட்டமைப்பு ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (எழுத்து *) iptr;
	போது (length) {
		clen = length;
		எனில் ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		எனில் (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			எனில் (retval)
				திரும்பு retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((எழுத்து *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef சொற்கள்_BIGENDIAN
	ext2fs_swap_inode_full(fs, (கட்டமைப்பு ext2_inode_large *) iptr,
			       (கட்டமைப்பு ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	எனில் (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	எனில் (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		திரும்பு EXT2_ET_INODE_CSUM_INVALID;

	திரும்பு 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 கட்டமைப்பு ext2_inode * inode, முழு bufsize)
{
	திரும்பு ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    கட்டமைப்பு ext2_inode * inode)
{
	திரும்பு ext2fs_read_inode2(fs, ino, inode,
				  அளவு(கட்டமைப்பு ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      கட்டமைப்பு ext2_inode * inode, முழு bufsize,
			      முழு flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	கட்டமைப்பு ext2_inode_large *w_inode;
	எழுத்து *ptr;
	unsigned i;
	முழு clen;
	முழு length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (ext2fs_has_feature_journal_dev(fs->super))
		திரும்பு EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	எனில் (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		எனில் (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			திரும்பு retval;
	}

	எனில் ((ino == 0) || (ino > fs->super->s_inodes_மொத்தம்))
		திரும்பு EXT2_ET_BAD_INODE_இலக்கம்;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	எனில் (retval)
		திரும்பு retval;

	எனில் (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (கட்டமைப்பு ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		எனில் (retval)
			செல் errout;
	}

	/* Check to see if the inode cache needs to be updated */
	எனில் (fs->icache) {
		க்கு (i=0; i < fs->icache->cache_size; i++) {
			எனில் (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				நிறுத்து;
			}
		}
	} இல்லையெனில் {
		retval = ext2fs_create_inode_cache(fs, 4);
		எனில் (retval)
			செல் errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	எனில் (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		செல் errout;
	}

#ifdef சொற்கள்_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	எனில் ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		எனில் (retval)
			செல் errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	எனில் (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		செல் errout;
	}
	எனில் ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_மொத்தம்(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		செல் errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (எழுத்து *) w_inode;

	போது (length) {
		clen = length;
		எனில் ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		எனில் (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			எனில் (retval)
				செல் errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((எழுத்து *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		எனில் (retval)
			செல் errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	திரும்பு retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  கட்டமைப்பு ext2_inode * inode, முழு bufsize)
{
	திரும்பு ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     கட்டமைப்பு ext2_inode *inode)
{
	திரும்பு ext2fs_write_inode2(fs, ino, inode,
				   அளவு(கட்டமைப்பு ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 கட்டமைப்பு ext2_inode *inode)
{
	கட்டமைப்பு ext2_inode	*buf;
	முழு			size = EXT2_INODE_SIZE(fs->super);
	கட்டமைப்பு ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	எனில் (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	எனில் (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	எனில் (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	எனில் (size == அளவு(கட்டமைப்பு ext2_inode))
		திரும்பு ext2fs_write_inode_full(fs, ino, inode,
					       அளவு(கட்டமைப்பு ext2_inode));

	buf = malloc(size);
	எனில் (!buf)
		திரும்பு ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (கட்டமைப்பு ext2_inode_large *) buf;
	large_inode->i_extra_isize = அளவு(கட்டமைப்பு ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	எனில் (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	விடுவி(buf);
	திரும்பு retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	கட்டமைப்பு ext2_inode	inode;
	முழு			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (ino > fs->super->s_inodes_மொத்தம்)
		திரும்பு EXT2_ET_BAD_INODE_இலக்கம்;

	எனில் (fs->get_blocks) {
		எனில் (!(*fs->get_blocks)(fs, ino, blocks))
			திரும்பு 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	எனில் (retval)
		திரும்பு retval;
	க்கு (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	திரும்பு 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	கட்டமைப்பு	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (ino > fs->super->s_inodes_மொத்தம்)
		திரும்பு EXT2_ET_BAD_INODE_இலக்கம்;

	எனில் (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		எனில் (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			திரும்பு retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	எனில் (retval)
		திரும்பு retval;
	எனில் (!LINUX_S_ISDIR(inode.i_mode))
		திரும்பு EXT2_ET_NO_DIRECTORY;
	திரும்பு 0;
}

