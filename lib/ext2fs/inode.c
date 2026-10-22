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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"
#অন্তর্ভাৱ "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

সংরচনা ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_বাম;
	blk_t			blocks_বাম;
	dgrp_t			groups_বাম;
	blk_t			inode_buffer_blocks;
	ৱর্ণ *			inode_buffer;
	পূর্ণাঙ্ক			inode_size;
	ৱর্ণ *			ptr;
	পূর্ণাঙ্ক			bytes_বাম;
	ৱর্ণ			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      শূন্য * priv_data);
	শূন্য *			done_group_data;
	পূর্ণাঙ্ক			bad_block_ptr;
	পূর্ণাঙ্ক			scan_flags;
	পূর্ণাঙ্ক			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	যদি (!fs->icache)
		ঘূৰাই 0;

	বাবে (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	ঘূৰাই 0;
}

/*
 * Free the inode cache structure
 */
শূন্য ext2fs_free_inode_cache(সংরচনা ext2_inode_cache *icache)
{
	unsigned i;

	যদি (--icache->refcount)
		ঘূৰাই;
	যদি (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	বাবে (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	যদি (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned পূর্ণাঙ্ক cache_size)
{
	unsigned	i;
	errcode_t	retval;

	যদি (fs->icache)
		ঘূৰাই 0;
	retval = ext2fs_get_mem(পরিমাণ(সংরচনা ext2_inode_cache), &fs->icache);
	যদি (retval)
		ঘূৰাই retval;

	memset(fs->icache, 0, পরিমাণ(সংরচনা ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	যদি (retval)
		গচ্ছ errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  পরিমাণ(সংরচনা ext2_inode_cache_ent),
				  &fs->icache->cache);
	যদি (retval)
		গচ্ছ errout;

	বাবে (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		যদি (retval)
			গচ্ছ errout;
	}

	ext2fs_flush_icache(fs);
	ঘূৰাই 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	ঘূৰাই retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, পূর্ণাঙ্ক buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		ঘূৰাই EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	যদি (fs->blocksize < 1024)
		ঘূৰাই EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	যদি (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		যদি (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা ext2_struct_inode_scan), &scan);
	যদি (retval)
		ঘূৰাই retval;
	memset(scan, 0, পরিমাণ(সংরচনা ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_বাম = 0;
	scan->current_group = 0;
	scan->groups_বাম = fs->group_desc_গুনতি - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	যদি (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_গুনতি(fs->super)))) {
		ext2fs_free_mem(&scan);
		ঘূৰাই EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_বাম = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_বাম = scan->fs->inode_blocks_per_group;
	যদি (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		যদি (scan->inodes_বাম > unused)
			scan->inodes_বাম -= unused;
		অন্যথা
			scan->inodes_বাম = 0;
		scan->blocks_বাম =
			(scan->inodes_বাম +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	যদি (retval) {
		ext2fs_free_mem(&scan);
		ঘূৰাই retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	যদি (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		ঘূৰাই retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	যদি (scan->fs->badblocks && scan->fs->badblocks->অঙ্ক)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	যদি (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	ঘূৰাই 0;
}

শূন্য ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	যদি (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ঘূৰাই;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = রিক্ত;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = রিক্ত;
	ext2fs_free_mem(&scan);
	ঘূৰাই;
}

শূন্য ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       শূন্য * priv_data),
			       শূন্য *done_group_data)
{
	যদি (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ঘূৰাই;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

পূর্ণাঙ্ক ext2fs_inode_scan_flags(ext2_inode_scan scan, পূর্ণাঙ্ক set_flags,
			    পূর্ণাঙ্ক clear_flags)
{
	পূর্ণাঙ্ক	old_flags;

	যদি (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ঘূৰাই 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	ঘূৰাই old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
স্থির errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_বাম--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_বাম = 0;
	scan->inodes_বাম = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_বাম = fs->inode_blocks_per_group;
	যদি (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		যদি (scan->inodes_বাম > unused)
			scan->inodes_বাম -= unused;
		অন্যথা
			scan->inodes_বাম = 0;
		scan->blocks_বাম =
			(scan->inodes_বাম +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	যদি (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_গুনতি(fs->super))))
		ঘূৰাই EXT2_ET_GDESC_BAD_INODE_TABLE;
	ঘূৰাই 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    পূর্ণাঙ্ক	group)
{
	scan->current_group = group - 1;
	scan->groups_বাম = scan->fs->group_desc_গুনতি - group;
	scan->bad_block_ptr = 0;
	ঘূৰাই get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
স্থির errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *অঙ্ক_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	যদি (blk == 0)
		ঘূৰাই 0;

	/* Make sure bad_block_ptr is still valid */
	যদি (scan->bad_block_ptr >= bb->অঙ্ক) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ঘূৰাই 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	যেতিয়ালৈ (blk > bb->list[scan->bad_block_ptr]) {
		যদি (++scan->bad_block_ptr >= bb->অঙ্ক) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			ঘূৰাই 0;
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
	যদি (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*অঙ্ক_blocks = 1;
		যদি (++scan->bad_block_ptr >= bb->অঙ্ক)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ঘূৰাই 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	যদি ((blk + *অঙ্ক_blocks) > bb->list[scan->bad_block_ptr])
		*অঙ্ক_blocks = (পূর্ণাঙ্ক) (bb->list[scan->bad_block_ptr] - blk);

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক block_map_looks_insane(ext2_filsys fs,
				  সংরচনা ext2_inode_large *inode)
{
	unsigned পূর্ণাঙ্ক i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	যদি ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		ঘূৰাই 0;
	যদি (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		ঘূৰাই 0;
	যদি (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= পরিমাণ(inode->i_block))
		ঘূৰাই 0;

	/* Unused inodes probably aren't insane */
	যদি (inode->i_links_গুনতি == 0)
		ঘূৰাই 0;

	/* See if more than half the block maps are insane */
	বাবে (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		যদি (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_গুনতি(fs->super)))
			bad++;
	ঘূৰাই bad > EXT2_N_BLOCKS / 2;
}

স্থির পূর্ণাঙ্ক extent_head_looks_insane(সংরচনা ext2_inode_large *inode)
{
	যদি (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					পরিমাণ(inode->i_block)) == 0)
		ঘূৰাই 0;
	ঘূৰাই 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
স্থির শূন্য check_inode_block_sanity(ext2_inode_scan scan, blk64_t অঙ্ক_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned পূর্ণাঙ্ক	badness, checksum_failures;
	unsigned পূর্ণাঙ্ক	inodes_in_buf, inodes_per_block;
	ৱর্ণ		*p;
	সংরচনা ext2_inode_large *inode;
	ৱর্ণ		*block_status;
	unsigned পূর্ণাঙ্ক	blk, bad_csum;

	যদি (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		ঘূৰাই;

	inodes_to_scan = scan->inodes_বাম;
	inodes_in_buf = অঙ্ক_blocks * scan->fs->blocksize / scan->inode_size;
	যদি (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (ৱর্ণ *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	যদি (inodes_per_block < 2)
		ঘূৰাই;

#ifdef শব্দগুলি_BIGENDIAN
	যদি (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		ঘূৰাই;
#endif

	যেতিয়ালৈ (inodes_to_scan > 0) {
		blk = (p - (ৱর্ণ *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(সংরচনা ext2_inode_large *) p) == 0;

#ifdef শব্দগুলি_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (সংরচনা ext2_inode_large *) inode,
			       (সংরচনা ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#অন্যথা
		inode = (সংরচনা ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		যদি (bad_csum) {
			checksum_failures++;
			badness++;
		} অন্যথা যদি (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		যদি (badness > inodes_per_block / 2) {
			unsigned পূর্ণাঙ্ক ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			যদি (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			অনুৱর্ত;
		}

		যদি ((ino % inodes_per_block) == 0) {
			যদি (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
স্থির errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		অঙ্ক_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	অঙ্ক_blocks = scan->inode_buffer_blocks;
	যদি (অঙ্ক_blocks > scan->blocks_বাম)
		অঙ্ক_blocks = scan->blocks_বাম;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	যদি (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		যদি (scan->bytes_বাম)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	যদি (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &অঙ্ক_blocks);
		যদি (retval)
			ঘূৰাই retval;
	}

	যদি ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) অঙ্ক_blocks * scan->fs->blocksize);
	} অন্যথা {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (পূর্ণাঙ্ক) অঙ্ক_blocks,
					     scan->inode_buffer);
		যদি (retval)
			ঘূৰাই EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, অঙ্ক_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_বাম = অঙ্ক_blocks * scan->fs->blocksize;

	scan->blocks_বাম -= অঙ্ক_blocks;
	যদি (scan->current_block)
		scan->current_block += অঙ্ক_blocks;

	ঘূৰাই 0;
}

#যদি 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
স্থির অন্তঃস্থ পূর্ণাঙ্ক is_empty_scan(ext2_inode_scan scan)
{
	পূর্ণাঙ্ক	i;

	যদি (scan->bytes_বাম == 0)
		ঘূৰাই 0;

	বাবে (i=0; i < scan->bytes_বাম; i++)
		যদি (scan->ptr[i])
			ঘূৰাই 0;
	ঘূৰাই 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     সংরচনা ext2_inode *inode, পূর্ণাঙ্ক bufsize)
{
	errcode_t	retval;
	পূর্ণাঙ্ক		extra_bytes = 0;
	পূর্ণাঙ্ক		length;
	সংরচনা ext2_inode_large	*iptr = (সংরচনা ext2_inode_large *)inode;
	ৱর্ণ		*iblock_status;
	unsigned পূর্ণাঙ্ক	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	যদি (scan->inodes_বাম <= 0) {
	force_new_group:
		যদি (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			যদি (retval)
				ঘূৰাই retval;
		}
		যদি (scan->groups_বাম <= 0) {
			*ino = 0;
			ঘূৰাই 0;
		}
		retval = get_next_blockgroup(scan);
		যদি (retval)
			ঘূৰাই retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	যদি ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		গচ্ছ force_new_group;
	যদি (scan->inodes_বাম == 0)
		গচ্ছ force_new_group;
	যদি (scan->current_block == 0) {
		যদি (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			গচ্ছ force_new_group;
		} অন্যথা
			ঘূৰাই EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	যদি (scan->bytes_বাম < scan->inode_size) {
		যদি (scan->bytes_বাম)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_বাম);
		extra_bytes = scan->bytes_বাম;

		retval = get_next_blocks(scan);
		যদি (retval)
			ঘূৰাই retval;
#যদি 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		যদি (is_empty_scan(scan))
			গচ্ছ force_new_group;
#endif
	}

	যদি (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		যদি (retval)
			ঘূৰাই retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	যদি (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_বাম -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		যদি (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(সংরচনা ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef শব্দগুলি_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (সংরচনা ext2_inode_large *) iptr,
			       (সংরচনা ext2_inode_large *) scan->temp_buffer,
			       0, length);
#অন্যথা
		memcpy(iptr, scan->temp_buffer, length);
#endif
		যদি (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} অন্যথা {
		/* Verify the inode checksum. */
		যদি (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(সংরচনা ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef শব্দগুলি_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(সংরচনা ext2_inode_large *) iptr,
				(সংরচনা ext2_inode_large *) scan->ptr,
				0, length);
#অন্যথা
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_বাম -= scan->inode_size;
		যদি (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	যদি ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_বাম--;
	scan->current_inode++;
	*ino = scan->current_inode;
	যদি (iptr != (সংরচনা ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	ঘূৰাই retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				সংরচনা ext2_inode *inode)
{
	ঘূৰাই ext2fs_get_next_inode_full(scan, ino, inode,
						পরিমাণ(সংরচনা ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     সংরচনা ext2_inode * inode, পূর্ণাঙ্ক bufsize,
			     পূর্ণাঙ্ক flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	ৱর্ণ 		*ptr;
	errcode_t	retval;
	unsigned	i;
	পূর্ণাঙ্ক		clen, inodes_per_block;
	io_channel	io;
	পূর্ণাঙ্ক		length = EXT2_INODE_SIZE(fs->super);
	সংরচনা ext2_inode_large	*iptr;
	পূর্ণাঙ্ক		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		ঘূৰাই EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	যদি (fs->blocksize < 1024)
		ঘূৰাই EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	যদি (fs->read_inode &&
	    ((bufsize == পরিমাণ(সংরচনা ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == পরিমাণ(সংরচনা ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		যদি (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ঘূৰাই retval;
	}
	যদি ((ino == 0) || (ino > fs->super->s_inodes_গুনতি))
		ঘূৰাই EXT2_ET_BAD_INODE_অঙ্ক;
	/* Create inode cache if not present */
	যদি (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		যদি (retval)
			ঘূৰাই retval;
	}
	/* Check to see if it's in the inode cache */
	বাবে (i = 0; i < fs->icache->cache_size; i++) {
		যদি (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			ঘূৰাই 0;
		}
	}
	যদি (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} অন্যথা {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		যদি (group > fs->group_desc_গুনতি)
			ঘূৰাই EXT2_ET_BAD_INODE_অঙ্ক;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		যদি (!block_nr)
			ঘূৰাই EXT2_ET_MISSING_INODE_TABLE;
		যদি ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_গুনতি(fs->super)))
			ঘূৰাই EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (সংরচনা ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (ৱর্ণ *) iptr;
	যেতিয়ালৈ (length) {
		clen = length;
		যদি ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		যদি (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			যদি (retval)
				ঘূৰাই retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((ৱর্ণ *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_swap_inode_full(fs, (সংরচনা ext2_inode_large *) iptr,
			       (সংরচনা ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	যদি (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	যদি (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		ঘূৰাই EXT2_ET_INODE_CSUM_INVALID;

	ঘূৰাই 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 সংরচনা ext2_inode * inode, পূর্ণাঙ্ক bufsize)
{
	ঘূৰাই ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    সংরচনা ext2_inode * inode)
{
	ঘূৰাই ext2fs_read_inode2(fs, ino, inode,
				  পরিমাণ(সংরচনা ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      সংরচনা ext2_inode * inode, পূর্ণাঙ্ক bufsize,
			      পূর্ণাঙ্ক flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	সংরচনা ext2_inode_large *w_inode;
	ৱর্ণ *ptr;
	unsigned i;
	পূর্ণাঙ্ক clen;
	পূর্ণাঙ্ক length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		ঘূৰাই EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	যদি (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		যদি (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ঘূৰাই retval;
	}

	যদি ((ino == 0) || (ino > fs->super->s_inodes_গুনতি))
		ঘূৰাই EXT2_ET_BAD_INODE_অঙ্ক;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	যদি (retval)
		ঘূৰাই retval;

	যদি (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (সংরচনা ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		যদি (retval)
			গচ্ছ errout;
	}

	/* Check to see if the inode cache needs to be updated */
	যদি (fs->icache) {
		বাবে (i=0; i < fs->icache->cache_size; i++) {
			যদি (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				ভঙ্গ;
			}
		}
	} অন্যথা {
		retval = ext2fs_create_inode_cache(fs, 4);
		যদি (retval)
			গচ্ছ errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	যদি (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		গচ্ছ errout;
	}

#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	যদি ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		যদি (retval)
			গচ্ছ errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	যদি (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		গচ্ছ errout;
	}
	যদি ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_গুনতি(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		গচ্ছ errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (ৱর্ণ *) w_inode;

	যেতিয়ালৈ (length) {
		clen = length;
		যদি ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		যদি (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			যদি (retval)
				গচ্ছ errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((ৱর্ণ *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		যদি (retval)
			গচ্ছ errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	ঘূৰাই retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  সংরচনা ext2_inode * inode, পূর্ণাঙ্ক bufsize)
{
	ঘূৰাই ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     সংরচনা ext2_inode *inode)
{
	ঘূৰাই ext2fs_write_inode2(fs, ino, inode,
				   পরিমাণ(সংরচনা ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 সংরচনা ext2_inode *inode)
{
	সংরচনা ext2_inode	*buf;
	পূর্ণাঙ্ক			size = EXT2_INODE_SIZE(fs->super);
	সংরচনা ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	যদি (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	যদি (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	যদি (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	যদি (size == পরিমাণ(সংরচনা ext2_inode))
		ঘূৰাই ext2fs_write_inode_full(fs, ino, inode,
					       পরিমাণ(সংরচনা ext2_inode));

	buf = malloc(size);
	যদি (!buf)
		ঘূৰাই ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (সংরচনা ext2_inode_large *) buf;
	large_inode->i_extra_isize = পরিমাণ(সংরচনা ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	যদি (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	মুক্ত(buf);
	ঘূৰাই retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	সংরচনা ext2_inode	inode;
	পূর্ণাঙ্ক			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ino > fs->super->s_inodes_গুনতি)
		ঘূৰাই EXT2_ET_BAD_INODE_অঙ্ক;

	যদি (fs->get_blocks) {
		যদি (!(*fs->get_blocks)(fs, ino, blocks))
			ঘূৰাই 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ঘূৰাই retval;
	বাবে (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	ঘূৰাই 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	সংরচনা	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ino > fs->super->s_inodes_গুনতি)
		ঘূৰাই EXT2_ET_BAD_INODE_অঙ্ক;

	যদি (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		যদি (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ঘূৰাই retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ঘূৰাই retval;
	যদি (!LINUX_S_ISDIR(inode.i_mode))
		ঘূৰাই EXT2_ET_NO_DIRECTORY;
	ঘূৰাই 0;
}

