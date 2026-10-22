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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"
#ଅନ୍ତର୍ଭାଵ "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

ସଂରଚନା ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_ବାମ;
	blk_t			blocks_ବାମ;
	dgrp_t			groups_ବାମ;
	blk_t			inode_buffer_blocks;
	ଅକ୍ଷର *			inode_buffer;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			inode_size;
	ଅକ୍ଷର *			ptr;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			bytes_ବାମ;
	ଅକ୍ଷର			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      ଶୂନ୍ୟ * priv_data);
	ଶୂନ୍ୟ *			done_group_data;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			bad_block_ptr;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			scan_flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	ଯଦି (!fs->icache)
		ଫେରନ୍ତୁ 0;

	ପାଇଁ (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	ଫେରନ୍ତୁ 0;
}

/*
 * Free the inode cache structure
 */
ଶୂନ୍ୟ ext2fs_free_inode_cache(ସଂରଚନା ext2_inode_cache *icache)
{
	unsigned i;

	ଯଦି (--icache->refcount)
		ଫେରନ୍ତୁ;
	ଯଦି (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	ପାଇଁ (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	ଯଦି (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cache_size)
{
	unsigned	i;
	errcode_t	retval;

	ଯଦି (fs->icache)
		ଫେରନ୍ତୁ 0;
	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_inode_cache), &fs->icache);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	memset(fs->icache, 0, ଆକାର(ସଂରଚନା ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	ଯଦି (retval)
		ଯାଅ errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  ଆକାର(ସଂରଚନା ext2_inode_cache_ent),
				  &fs->icache->cache);
	ଯଦି (retval)
		ଯାଅ errout;

	ପାଇଁ (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		ଯଦି (retval)
			ଯାଅ errout;
	}

	ext2fs_flush_icache(fs);
	ଫେରନ୍ତୁ 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (ext2fs_has_feature_journal_dev(fs->super))
		ଫେରନ୍ତୁ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ଯଦି (fs->blocksize < 1024)
		ଫେରନ୍ତୁ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	ଯଦି (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		ଯଦି (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2_struct_inode_scan), &scan);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	memset(scan, 0, ଆକାର(ସଂରଚନା ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_ବାମ = 0;
	scan->current_group = 0;
	scan->groups_ବାମ = fs->group_desc_ଗଣନା - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	ଯଦି (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_ଗଣନା(fs->super)))) {
		ext2fs_free_mem(&scan);
		ଫେରନ୍ତୁ EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_ବାମ = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_ବାମ = scan->fs->inode_blocks_per_group;
	ଯଦି (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		ଯଦି (scan->inodes_ବାମ > unused)
			scan->inodes_ବାମ -= unused;
		ଅନ୍ୟଥା
			scan->inodes_ବାମ = 0;
		scan->blocks_ବାମ =
			(scan->inodes_ବାମ +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	ଯଦି (retval) {
		ext2fs_free_mem(&scan);
		ଫେରନ୍ତୁ retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	ଯଦି (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		ଫେରନ୍ତୁ retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	ଯଦି (scan->fs->badblocks && scan->fs->badblocks->ଅଙ୍କ)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	ଯଦି (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	ଯଦି (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ଫେରନ୍ତୁ;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = ଖାଲି;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = ଖାଲି;
	ext2fs_free_mem(&scan);
	ଫେରନ୍ତୁ;
}

ଶୂନ୍ୟ ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       ଶୂନ୍ୟ * priv_data),
			       ଶୂନ୍ୟ *done_group_data)
{
	ଯଦି (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ଫେରନ୍ତୁ;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_inode_scan_flags(ext2_inode_scan scan, ପୂର୍ଣ୍ଣସଂଖ୍ୟା set_flags,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear_flags)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	old_flags;

	ଯଦି (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ଫେରନ୍ତୁ 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	ଫେରନ୍ତୁ old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ସ୍ଥିର errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_ବାମ--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_ବାମ = 0;
	scan->inodes_ବାମ = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_ବାମ = fs->inode_blocks_per_group;
	ଯଦି (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		ଯଦି (scan->inodes_ବାମ > unused)
			scan->inodes_ବାମ -= unused;
		ଅନ୍ୟଥା
			scan->inodes_ବାମ = 0;
		scan->blocks_ବାମ =
			(scan->inodes_ବାମ +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	ଯଦି (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_ଗଣନା(fs->super))))
		ଫେରନ୍ତୁ EXT2_ET_GDESC_BAD_INODE_TABLE;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    ପୂର୍ଣ୍ଣସଂଖ୍ୟା	group)
{
	scan->current_group = group - 1;
	scan->groups_ବାମ = scan->fs->group_desc_ଗଣନା - group;
	scan->bad_block_ptr = 0;
	ଫେରନ୍ତୁ get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ସ୍ଥିର errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *ଅଙ୍କ_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	ଯଦି (blk == 0)
		ଫେରନ୍ତୁ 0;

	/* Make sure bad_block_ptr is still valid */
	ଯଦି (scan->bad_block_ptr >= bb->ଅଙ୍କ) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ଫେରନ୍ତୁ 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	ଯେତେବେଳେ (blk > bb->list[scan->bad_block_ptr]) {
		ଯଦି (++scan->bad_block_ptr >= bb->ଅଙ୍କ) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			ଫେରନ୍ତୁ 0;
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
	ଯଦି (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*ଅଙ୍କ_blocks = 1;
		ଯଦି (++scan->bad_block_ptr >= bb->ଅଙ୍କ)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ଫେରନ୍ତୁ 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	ଯଦି ((blk + *ଅଙ୍କ_blocks) > bb->list[scan->bad_block_ptr])
		*ଅଙ୍କ_blocks = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (bb->list[scan->bad_block_ptr] - blk);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_map_looks_insane(ext2_filsys fs,
				  ସଂରଚନା ext2_inode_large *inode)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	ଯଦି ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		ଫେରନ୍ତୁ 0;
	ଯଦି (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		ଫେରନ୍ତୁ 0;
	ଯଦି (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= ଆକାର(inode->i_block))
		ଫେରନ୍ତୁ 0;

	/* Unused inodes probably aren't insane */
	ଯଦି (inode->i_links_ଗଣନା == 0)
		ଫେରନ୍ତୁ 0;

	/* See if more than half the block maps are insane */
	ପାଇଁ (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		ଯଦି (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_ଗଣନା(fs->super)))
			bad++;
	ଫେରନ୍ତୁ bad > EXT2_N_BLOCKS / 2;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା extent_head_looks_insane(ସଂରଚନା ext2_inode_large *inode)
{
	ଯଦି (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					ଆକାର(inode->i_block)) == 0)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ସ୍ଥିର ଶୂନ୍ୟ check_inode_block_sanity(ext2_inode_scan scan, blk64_t ଅଙ୍କ_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	badness, checksum_failures;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	inodes_in_buf, inodes_per_block;
	ଅକ୍ଷର		*p;
	ସଂରଚନା ext2_inode_large *inode;
	ଅକ୍ଷର		*block_status;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blk, bad_csum;

	ଯଦି (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		ଫେରନ୍ତୁ;

	inodes_to_scan = scan->inodes_ବାମ;
	inodes_in_buf = ଅଙ୍କ_blocks * scan->fs->blocksize / scan->inode_size;
	ଯଦି (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (ଅକ୍ଷର *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	ଯଦି (inodes_per_block < 2)
		ଫେରନ୍ତୁ;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ଯଦି (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		ଫେରନ୍ତୁ;
#endif

	ଯେତେବେଳେ (inodes_to_scan > 0) {
		blk = (p - (ଅକ୍ଷର *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(ସଂରଚନା ext2_inode_large *) p) == 0;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (ସଂରଚନା ext2_inode_large *) inode,
			       (ସଂରଚନା ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#ଅନ୍ୟଥା
		inode = (ସଂରଚନା ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		ଯଦି (bad_csum) {
			checksum_failures++;
			badness++;
		} ଅନ୍ୟଥା ଯଦି (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		ଯଦି (badness > inodes_per_block / 2) {
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			ଯଦି (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			ଜାରି;
		}

		ଯଦି ((ino % inodes_per_block) == 0) {
			ଯଦି (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ସ୍ଥିର errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		ଅଙ୍କ_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	ଅଙ୍କ_blocks = scan->inode_buffer_blocks;
	ଯଦି (ଅଙ୍କ_blocks > scan->blocks_ବାମ)
		ଅଙ୍କ_blocks = scan->blocks_ବାମ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	ଯଦି (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		ଯଦି (scan->bytes_ବାମ)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	ଯଦି (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &ଅଙ୍କ_blocks);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}

	ଯଦି ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) ଅଙ୍କ_blocks * scan->fs->blocksize);
	} ଅନ୍ୟଥା {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଅଙ୍କ_blocks,
					     scan->inode_buffer);
		ଯଦି (retval)
			ଫେରନ୍ତୁ EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, ଅଙ୍କ_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_ବାମ = ଅଙ୍କ_blocks * scan->fs->blocksize;

	scan->blocks_ବାମ -= ଅଙ୍କ_blocks;
	ଯଦି (scan->current_block)
		scan->current_block += ଅଙ୍କ_blocks;

	ଫେରନ୍ତୁ 0;
}

#ଯଦି 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_empty_scan(ext2_inode_scan scan)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i;

	ଯଦି (scan->bytes_ବାମ == 0)
		ଫେରନ୍ତୁ 0;

	ପାଇଁ (i=0; i < scan->bytes_ବାମ; i++)
		ଯଦି (scan->ptr[i])
			ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     ସଂରଚନା ext2_inode *inode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bufsize)
{
	errcode_t	retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		extra_bytes = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		length;
	ସଂରଚନା ext2_inode_large	*iptr = (ସଂରଚନା ext2_inode_large *)inode;
	ଅକ୍ଷର		*iblock_status;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	ଯଦି (scan->inodes_ବାମ <= 0) {
	force_new_group:
		ଯଦି (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
		}
		ଯଦି (scan->groups_ବାମ <= 0) {
			*ino = 0;
			ଫେରନ୍ତୁ 0;
		}
		retval = get_next_blockgroup(scan);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	ଯଦି ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		ଯାଅ force_new_group;
	ଯଦି (scan->inodes_ବାମ == 0)
		ଯାଅ force_new_group;
	ଯଦି (scan->current_block == 0) {
		ଯଦି (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			ଯାଅ force_new_group;
		} ଅନ୍ୟଥା
			ଫେରନ୍ତୁ EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	ଯଦି (scan->bytes_ବାମ < scan->inode_size) {
		ଯଦି (scan->bytes_ବାମ)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_ବାମ);
		extra_bytes = scan->bytes_ବାମ;

		retval = get_next_blocks(scan);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
#ଯଦି 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		ଯଦି (is_empty_scan(scan))
			ଯାଅ force_new_group;
#endif
	}

	ଯଦି (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	ଯଦି (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_ବାମ -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		ଯଦି (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(ସଂରଚନା ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (ସଂରଚନା ext2_inode_large *) iptr,
			       (ସଂରଚନା ext2_inode_large *) scan->temp_buffer,
			       0, length);
#ଅନ୍ୟଥା
		memcpy(iptr, scan->temp_buffer, length);
#endif
		ଯଦି (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} ଅନ୍ୟଥା {
		/* Verify the inode checksum. */
		ଯଦି (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(ସଂରଚନା ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(ସଂରଚନା ext2_inode_large *) iptr,
				(ସଂରଚନା ext2_inode_large *) scan->ptr,
				0, length);
#ଅନ୍ୟଥା
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_ବାମ -= scan->inode_size;
		ଯଦି (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	ଯଦି ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_ବାମ--;
	scan->current_inode++;
	*ino = scan->current_inode;
	ଯଦି (iptr != (ସଂରଚନା ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				ସଂରଚନା ext2_inode *inode)
{
	ଫେରନ୍ତୁ ext2fs_get_next_inode_full(scan, ino, inode,
						ଆକାର(ସଂରଚନା ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     ସଂରଚନା ext2_inode * inode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bufsize,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	ଅକ୍ଷର 		*ptr;
	errcode_t	retval;
	unsigned	i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		clen, inodes_per_block;
	io_channel	io;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		length = EXT2_INODE_SIZE(fs->super);
	ସଂରଚନା ext2_inode_large	*iptr;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (ext2fs_has_feature_journal_dev(fs->super))
		ଫେରନ୍ତୁ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ଯଦି (fs->blocksize < 1024)
		ଫେରନ୍ତୁ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	ଯଦି (fs->read_inode &&
	    ((bufsize == ଆକାର(ସଂରଚନା ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == ଆକାର(ସଂରଚନା ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		ଯଦି (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ଫେରନ୍ତୁ retval;
	}
	ଯଦି ((ino == 0) || (ino > fs->super->s_inodes_ଗଣନା))
		ଫେରନ୍ତୁ EXT2_ET_BAD_INODE_ଅଙ୍କ;
	/* Create inode cache if not present */
	ଯଦି (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	/* Check to see if it's in the inode cache */
	ପାଇଁ (i = 0; i < fs->icache->cache_size; i++) {
		ଯଦି (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			ଫେରନ୍ତୁ 0;
		}
	}
	ଯଦି (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} ଅନ୍ୟଥା {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		ଯଦି (group > fs->group_desc_ଗଣନା)
			ଫେରନ୍ତୁ EXT2_ET_BAD_INODE_ଅଙ୍କ;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		ଯଦି (!block_nr)
			ଫେରନ୍ତୁ EXT2_ET_MISSING_INODE_TABLE;
		ଯଦି ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_ଗଣନା(fs->super)))
			ଫେରନ୍ତୁ EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (ସଂରଚନା ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (ଅକ୍ଷର *) iptr;
	ଯେତେବେଳେ (length) {
		clen = length;
		ଯଦି ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		ଯଦି (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((ଅକ୍ଷର *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ext2fs_swap_inode_full(fs, (ସଂରଚନା ext2_inode_large *) iptr,
			       (ସଂରଚନା ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	ଯଦି (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	ଯଦି (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		ଫେରନ୍ତୁ EXT2_ET_INODE_CSUM_INVALID;

	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 ସଂରଚନା ext2_inode * inode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bufsize)
{
	ଫେରନ୍ତୁ ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ସଂରଚନା ext2_inode * inode)
{
	ଫେରନ୍ତୁ ext2fs_read_inode2(fs, ino, inode,
				  ଆକାର(ସଂରଚନା ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      ସଂରଚନା ext2_inode * inode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bufsize,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	ସଂରଚନା ext2_inode_large *w_inode;
	ଅକ୍ଷର *ptr;
	unsigned i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା clen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (ext2fs_has_feature_journal_dev(fs->super))
		ଫେରନ୍ତୁ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	ଯଦି (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		ଯଦି (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ଫେରନ୍ତୁ retval;
	}

	ଯଦି ((ino == 0) || (ino > fs->super->s_inodes_ଗଣନା))
		ଫେରନ୍ତୁ EXT2_ET_BAD_INODE_ଅଙ୍କ;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (ସଂରଚନା ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		ଯଦି (retval)
			ଯାଅ errout;
	}

	/* Check to see if the inode cache needs to be updated */
	ଯଦି (fs->icache) {
		ପାଇଁ (i=0; i < fs->icache->cache_size; i++) {
			ଯଦି (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				ଭାଙ୍ଗ;
			}
		}
	} ଅନ୍ୟଥା {
		retval = ext2fs_create_inode_cache(fs, 4);
		ଯଦି (retval)
			ଯାଅ errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	ଯଦି (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		ଯାଅ errout;
	}

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	ଯଦି ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		ଯଦି (retval)
			ଯାଅ errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	ଯଦି (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		ଯାଅ errout;
	}
	ଯଦି ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_ଗଣନା(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		ଯାଅ errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (ଅକ୍ଷର *) w_inode;

	ଯେତେବେଳେ (length) {
		clen = length;
		ଯଦି ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		ଯଦି (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			ଯଦି (retval)
				ଯାଅ errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((ଅକ୍ଷର *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		ଯଦି (retval)
			ଯାଅ errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  ସଂରଚନା ext2_inode * inode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bufsize)
{
	ଫେରନ୍ତୁ ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     ସଂରଚନା ext2_inode *inode)
{
	ଫେରନ୍ତୁ ext2fs_write_inode2(fs, ino, inode,
				   ଆକାର(ସଂରଚନା ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 ସଂରଚନା ext2_inode *inode)
{
	ସଂରଚନା ext2_inode	*buf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			size = EXT2_INODE_SIZE(fs->super);
	ସଂରଚନା ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	ଯଦି (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	ଯଦି (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	ଯଦି (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	ଯଦି (size == ଆକାର(ସଂରଚନା ext2_inode))
		ଫେରନ୍ତୁ ext2fs_write_inode_full(fs, ino, inode,
					       ଆକାର(ସଂରଚନା ext2_inode));

	buf = malloc(size);
	ଯଦି (!buf)
		ଫେରନ୍ତୁ ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (ସଂରଚନା ext2_inode_large *) buf;
	large_inode->i_extra_isize = ଆକାର(ସଂରଚନା ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	ଯଦି (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	ସଂରଚନା ext2_inode	inode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (ino > fs->super->s_inodes_ଗଣନା)
		ଫେରନ୍ତୁ EXT2_ET_BAD_INODE_ଅଙ୍କ;

	ଯଦି (fs->get_blocks) {
		ଯଦି (!(*fs->get_blocks)(fs, ino, blocks))
			ଫେରନ୍ତୁ 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ପାଇଁ (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	ସଂରଚନା	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (ino > fs->super->s_inodes_ଗଣନା)
		ଫେରନ୍ତୁ EXT2_ET_BAD_INODE_ଅଙ୍କ;

	ଯଦି (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		ଯଦି (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ଫେରନ୍ତୁ retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି (!LINUX_S_ISDIR(inode.i_mode))
		ଫେରନ୍ତୁ EXT2_ET_NO_DIRECTORY;
	ଫେରନ୍ତୁ 0;
}

