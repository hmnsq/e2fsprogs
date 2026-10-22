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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"
#অন্তর্ভাব "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->অস্থায়ী_বাফার + (scan)->inode_আকার)

কাঠামো ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_বাম;
	blk_t			blocks_বাম;
	dgrp_t			groups_বাম;
	blk_t			inode_বাফার_blocks;
	অক্ষর *			inode_বাফার;
	পূর্ণসংখ্যা			inode_আকার;
	অক্ষর *			ptr;
	পূর্ণসংখ্যা			bytes_বাম;
	অক্ষর			*অস্থায়ী_বাফার;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      শূন্য * priv_তথ্য);
	শূন্য *			done_group_তথ্য;
	পূর্ণসংখ্যা			bad_block_ptr;
	পূর্ণসংখ্যা			scan_flags;
	পূর্ণসংখ্যা			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	যদি (!fs->icache)
		ফেরত 0;

	জন্য (i=0; i < fs->icache->cache_আকার; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->বাফার_blk = 0;
	ফেরত 0;
}

/*
 * Free the inode cache structure
 */
শূন্য ext2fs_free_inode_cache(কাঠামো ext2_inode_cache *icache)
{
	unsigned i;

	যদি (--icache->refcount)
		ফেরত;
	যদি (icache->বাফার)
		ext2fs_free_mem(&icache->বাফার);
	জন্য (i = 0; i < icache->cache_আকার; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	যদি (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->বাফার_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned পূর্ণসংখ্যা cache_আকার)
{
	unsigned	i;
	errcode_t	retval;

	যদি (fs->icache)
		ফেরত 0;
	retval = ext2fs_get_mem(আকার(কাঠামো ext2_inode_cache), &fs->icache);
	যদি (retval)
		ফেরত retval;

	memset(fs->icache, 0, আকার(কাঠামো ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->বাফার);
	যদি (retval)
		যাও errout;

	fs->icache->বাফার_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_আকার = cache_আকার;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_আকার,
				  আকার(কাঠামো ext2_inode_cache_ent),
				  &fs->icache->cache);
	যদি (retval)
		যাও errout;

	জন্য (i = 0; i < fs->icache->cache_আকার; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_আকার(fs->super),
					&fs->icache->cache[i].inode);
		যদি (retval)
			যাও errout;
	}

	ext2fs_flush_icache(fs);
	ফেরত 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	ফেরত retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, পূর্ণসংখ্যা বাফার_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		ফেরত EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	যদি (fs->blocksize < 1024)
		ফেরত EXT2_FILSYS_CORRUPTED; /* Should never happen */

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

	retval = ext2fs_get_mem(আকার(কাঠামো ext2_struct_inode_scan), &scan);
	যদি (retval)
		ফেরত retval;
	memset(scan, 0, আকার(কাঠামো ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_আকার = EXT2_INODE_আকার(fs->super);
	scan->bytes_বাম = 0;
	scan->current_group = 0;
	scan->groups_বাম = fs->group_desc_গুনতি - 1;
	scan->inode_বাফার_blocks = বাফার_blocks ? বাফার_blocks :
				    EXT2_INODE_SCAN_DEFAULT_বাফার_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	যদি (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_তথ্য_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_গুনতি(fs->super)))) {
		ext2fs_free_mem(&scan);
		ফেরত EXT2_ET_GDESC_BAD_INODE_TABLE;
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
			 (fs->blocksize / scan->inode_আকার - 1)) *
			scan->inode_আকার / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_বাফার_blocks,
				      &scan->inode_বাফার);
	scan->done_group = 0;
	scan->done_group_তথ্য = 0;
	scan->bad_block_ptr = 0;
	যদি (retval) {
		ext2fs_free_mem(&scan);
		ফেরত retval;
	}
	retval = ext2fs_get_mem(scan->inode_আকার + scan->inode_বাফার_blocks,
				&scan->অস্থায়ী_বাফার);
	যদি (retval) {
		ext2fs_free_mem(&scan->inode_বাফার);
		ext2fs_free_mem(&scan);
		ফেরত retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_বাফার_blocks);
	যদি (scan->fs->badblocks && scan->fs->badblocks->অঙ্ক)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	যদি (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	ফেরত 0;
}

শূন্য ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	যদি (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ফেরত;

	ext2fs_free_mem(&scan->inode_বাফার);
	scan->inode_বাফার = ফাঁকা;
	ext2fs_free_mem(&scan->অস্থায়ী_বাফার);
	scan->অস্থায়ী_বাফার = ফাঁকা;
	ext2fs_free_mem(&scan);
	ফেরত;
}

শূন্য ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       শূন্য * priv_তথ্য),
			       শূন্য *done_group_তথ্য)
{
	যদি (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ফেরত;

	scan->done_group = done_group;
	scan->done_group_তথ্য = done_group_তথ্য;
}

পূর্ণসংখ্যা ext2fs_inode_scan_flags(ext2_inode_scan scan, পূর্ণসংখ্যা set_flags,
			    পূর্ণসংখ্যা clear_flags)
{
	পূর্ণসংখ্যা	old_flags;

	যদি (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ফেরত 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	ফেরত old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
স্থির errcode_t get_পরবর্তী_blockgroup(ext2_inode_scan scan)
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
			 (fs->blocksize / scan->inode_আকার - 1)) *
			scan->inode_আকার / fs->blocksize;
	}
	যদি (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_তথ্য_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_গুনতি(fs->super))))
		ফেরত EXT2_ET_GDESC_BAD_INODE_TABLE;
	ফেরত 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    পূর্ণসংখ্যা	group)
{
	scan->current_group = group - 1;
	scan->groups_বাম = scan->fs->group_desc_গুনতি - group;
	scan->bad_block_ptr = 0;
	ফেরত get_পরবর্তী_blockgroup(scan);
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
		ফেরত 0;

	/* Make sure bad_block_ptr is still valid */
	যদি (scan->bad_block_ptr >= bb->অঙ্ক) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ফেরত 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	যখন (blk > bb->list[scan->bad_block_ptr]) {
		যদি (++scan->bad_block_ptr >= bb->অঙ্ক) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			ফেরত 0;
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
		ফেরত 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	যদি ((blk + *অঙ্ক_blocks) > bb->list[scan->bad_block_ptr])
		*অঙ্ক_blocks = (পূর্ণসংখ্যা) (bb->list[scan->bad_block_ptr] - blk);

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা block_map_looks_insane(ext2_filsys fs,
				  কাঠামো ext2_inode_large *inode)
{
	unsigned পূর্ণসংখ্যা i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	যদি ((inode->i_flags & EXT4_INLINE_তথ্য_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		ফেরত 0;
	যদি (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		ফেরত 0;
	যদি (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_আকার(inode) <= আকার(inode->i_block))
		ফেরত 0;

	/* Unused inodes probably aren't insane */
	যদি (inode->i_links_গুনতি == 0)
		ফেরত 0;

	/* See if more than half the block maps are insane */
	জন্য (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		যদি (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_তথ্য_block ||
		     inode->i_block[i] >= ext2fs_blocks_গুনতি(fs->super)))
			bad++;
	ফেরত bad > EXT2_N_BLOCKS / 2;
}

স্থির পূর্ণসংখ্যা extent_শীর্ষ_looks_insane(কাঠামো ext2_inode_large *inode)
{
	যদি (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					আকার(inode->i_block)) == 0)
		ফেরত 0;
	ফেরত 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
স্থির শূন্য check_inode_block_sanity(ext2_inode_scan scan, blk64_t অঙ্ক_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned পূর্ণসংখ্যা	badness, checksum_failures;
	unsigned পূর্ণসংখ্যা	inodes_in_buf, inodes_per_block;
	অক্ষর		*p;
	কাঠামো ext2_inode_large *inode;
	অক্ষর		*block_status;
	unsigned পূর্ণসংখ্যা	blk, bad_csum;

	যদি (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		ফেরত;

	inodes_to_scan = scan->inodes_বাম;
	inodes_in_buf = অঙ্ক_blocks * scan->fs->blocksize / scan->inode_আকার;
	যদি (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (অক্ষর *) scan->inode_বাফার;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_বাফার_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	যদি (inodes_per_block < 2)
		ফেরত;

#ifdef শব্দগুলি_BIGENDIAN
	যদি (ext2fs_get_mem(EXT2_INODE_আকার(scan->fs->super), &inode))
		ফেরত;
#endif

	যখন (inodes_to_scan > 0) {
		blk = (p - (অক্ষর *)scan->inode_বাফার) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(কাঠামো ext2_inode_large *) p) == 0;

#ifdef শব্দগুলি_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (কাঠামো ext2_inode_large *) inode,
			       (কাঠামো ext2_inode_large *) p,
			       0, EXT2_INODE_আকার(scan->fs->super));
#অন্যথা
		inode = (কাঠামো ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		যদি (bad_csum) {
			checksum_failures++;
			badness++;
		} অন্যথা যদি (extent_শীর্ষ_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		যদি (badness > inodes_per_block / 2) {
			unsigned পূর্ণসংখ্যা ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			যদি (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_আকার * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			চালু;
		}

		যদি ((ino % inodes_per_block) == 0) {
			যদি (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_আকার;
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
স্থির errcode_t get_পরবর্তী_blocks(ext2_inode_scan scan)
{
	blk64_t		অঙ্ক_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	অঙ্ক_blocks = scan->inode_বাফার_blocks;
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
			ফেরত retval;
	}

	যদি ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_বাফার, 0,
		       (size_t) অঙ্ক_blocks * scan->fs->blocksize);
	} অন্যথা {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (পূর্ণসংখ্যা) অঙ্ক_blocks,
					     scan->inode_বাফার);
		যদি (retval)
			ফেরত EXT2_ET_পরবর্তী_INODE_READ;
	}
	check_inode_block_sanity(scan, অঙ্ক_blocks);

	scan->ptr = scan->inode_বাফার;
	scan->bytes_বাম = অঙ্ক_blocks * scan->fs->blocksize;

	scan->blocks_বাম -= অঙ্ক_blocks;
	যদি (scan->current_block)
		scan->current_block += অঙ্ক_blocks;

	ফেরত 0;
}

#যদি 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
স্থির অন্তঃরেখ পূর্ণসংখ্যা is_empty_scan(ext2_inode_scan scan)
{
	পূর্ণসংখ্যা	i;

	যদি (scan->bytes_বাম == 0)
		ফেরত 0;

	জন্য (i=0; i < scan->bytes_বাম; i++)
		যদি (scan->ptr[i])
			ফেরত 0;
	ফেরত 1;
}
#endif

errcode_t ext2fs_get_পরবর্তী_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     কাঠামো ext2_inode *inode, পূর্ণসংখ্যা bufsize)
{
	errcode_t	retval;
	পূর্ণসংখ্যা		extra_bytes = 0;
	পূর্ণসংখ্যা		দৈর্ঘ্য;
	কাঠামো ext2_inode_large	*iptr = (কাঠামো ext2_inode_large *)inode;
	অক্ষর		*iblock_status;
	unsigned পূর্ণসংখ্যা	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	দৈর্ঘ্য = EXT2_INODE_আকার(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	যদি (scan->inodes_বাম <= 0) {
	force_new_group:
		যদি (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_তথ্য);
			যদি (retval)
				ফেরত retval;
		}
		যদি (scan->groups_বাম <= 0) {
			*ino = 0;
			ফেরত 0;
		}
		retval = get_পরবর্তী_blockgroup(scan);
		যদি (retval)
			ফেরত retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	যদি ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		যাও force_new_group;
	যদি (scan->inodes_বাম == 0)
		যাও force_new_group;
	যদি (scan->current_block == 0) {
		যদি (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			যাও force_new_group;
		} অন্যথা
			ফেরত EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	যদি (scan->bytes_বাম < scan->inode_আকার) {
		যদি (scan->bytes_বাম)
			memcpy(scan->অস্থায়ী_বাফার, scan->ptr, scan->bytes_বাম);
		extra_bytes = scan->bytes_বাম;

		retval = get_পরবর্তী_blocks(scan);
		যদি (retval)
			ফেরত retval;
#যদি 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		যদি (is_empty_scan(scan))
			যাও force_new_group;
#endif
	}

	যদি (bufsize < দৈর্ঘ্য) {
		retval = ext2fs_get_mem(দৈর্ঘ্য, &iptr);
		যদি (retval)
			ফেরত retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_বাফার_blocks;
	যদি (extra_bytes) {
		memcpy(scan->অস্থায়ী_বাফার+extra_bytes, scan->ptr,
		       scan->inode_আকার - extra_bytes);
		scan->ptr += scan->inode_আকার - extra_bytes;
		scan->bytes_বাম -= scan->inode_আকার - extra_bytes;

		/* Verify the inode checksum. */
		যদি (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(কাঠামো ext2_inode_large *)scan->অস্থায়ী_বাফার))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef শব্দগুলি_BIGENDIAN
		memset(iptr, 0, দৈর্ঘ্য);
		ext2fs_swap_inode_full(scan->fs,
			       (কাঠামো ext2_inode_large *) iptr,
			       (কাঠামো ext2_inode_large *) scan->অস্থায়ী_বাফার,
			       0, দৈর্ঘ্য);
#অন্যথা
		memcpy(iptr, scan->অস্থায়ী_বাফার, দৈর্ঘ্য);
#endif
		যদি (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} অন্যথা {
		/* Verify the inode checksum. */
		যদি (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(কাঠামো ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef শব্দগুলি_BIGENDIAN
		memset(iptr, 0, দৈর্ঘ্য);
		ext2fs_swap_inode_full(scan->fs,
				(কাঠামো ext2_inode_large *) iptr,
				(কাঠামো ext2_inode_large *) scan->ptr,
				0, দৈর্ঘ্য);
#অন্যথা
		memcpy(iptr, scan->ptr, দৈর্ঘ্য);
#endif
		scan->ptr += scan->inode_আকার;
		scan->bytes_বাম -= scan->inode_আকার;
		যদি (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	যদি ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_বাম--;
	scan->current_inode++;
	*ino = scan->current_inode;
	যদি (iptr != (কাঠামো ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	ফেরত retval;
}

errcode_t ext2fs_get_পরবর্তী_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				কাঠামো ext2_inode *inode)
{
	ফেরত ext2fs_get_পরবর্তী_inode_full(scan, ino, inode,
						আকার(কাঠামো ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     কাঠামো ext2_inode * inode, পূর্ণসংখ্যা bufsize,
			     পূর্ণসংখ্যা flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	অক্ষর 		*ptr;
	errcode_t	retval;
	unsigned	i;
	পূর্ণসংখ্যা		clen, inodes_per_block;
	io_channel	io;
	পূর্ণসংখ্যা		দৈর্ঘ্য = EXT2_INODE_আকার(fs->super);
	কাঠামো ext2_inode_large	*iptr;
	পূর্ণসংখ্যা		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		ফেরত EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	যদি (fs->blocksize < 1024)
		ফেরত EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	যদি (fs->read_inode &&
	    ((bufsize == আকার(কাঠামো ext2_inode)) ||
	     (EXT2_INODE_আকার(fs->super) == আকার(কাঠামো ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		যদি (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ফেরত retval;
	}
	যদি ((ino == 0) || (ino > fs->super->s_inodes_গুনতি))
		ফেরত EXT2_ET_BAD_INODE_অঙ্ক;
	/* Create inode cache if not present */
	যদি (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		যদি (retval)
			ফেরত retval;
	}
	/* Check to see if it's in the inode cache */
	জন্য (i = 0; i < fs->icache->cache_আকার; i++) {
		যদি (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > দৈর্ঘ্য) ? দৈর্ঘ্য : bufsize);
			ফেরত 0;
		}
	}
	যদি (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_আকার(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_আকার(fs->super);
		io = fs->image_io;
	} অন্যথা {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		যদি (group > fs->group_desc_গুনতি)
			ফেরত EXT2_ET_BAD_INODE_অঙ্ক;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_আকার(fs->super);
		block = offset >> EXT2_BLOCK_আকার_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		যদি (!block_nr)
			ফেরত EXT2_ET_MISSING_INODE_TABLE;
		যদি ((block_nr < fs->super->s_first_তথ্য_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_গুনতি(fs->super)))
			ফেরত EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_আকার(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_আকার;
	iptr = (কাঠামো ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (অক্ষর *) iptr;
	যখন (দৈর্ঘ্য) {
		clen = দৈর্ঘ্য;
		যদি ((offset + দৈর্ঘ্য) > fs->blocksize)
			clen = fs->blocksize - offset;

		যদি (block_nr != fs->icache->বাফার_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->বাফার);
			যদি (retval)
				ফেরত retval;
			fs->icache->বাফার_blk = block_nr;
		}

		memcpy(ptr, ((অক্ষর *) fs->icache->বাফার) + (unsigned) offset,
		       clen);

		offset = 0;
		দৈর্ঘ্য -= clen;
		ptr += clen;
		block_nr++;
	}
	দৈর্ঘ্য = EXT2_INODE_আকার(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_swap_inode_full(fs, (কাঠামো ext2_inode_large *) iptr,
			       (কাঠামো ext2_inode_large *) iptr,
			       0, দৈর্ঘ্য);
#endif

	/* Update the inode cache bookkeeping */
	যদি (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > দৈর্ঘ্য) ? দৈর্ঘ্য : bufsize);

	যদি (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		ফেরত EXT2_ET_INODE_CSUM_INVALID;

	ফেরত 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 কাঠামো ext2_inode * inode, পূর্ণসংখ্যা bufsize)
{
	ফেরত ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    কাঠামো ext2_inode * inode)
{
	ফেরত ext2fs_read_inode2(fs, ino, inode,
				  আকার(কাঠামো ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      কাঠামো ext2_inode * inode, পূর্ণসংখ্যা bufsize,
			      পূর্ণসংখ্যা flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	কাঠামো ext2_inode_large *w_inode;
	অক্ষর *ptr;
	unsigned i;
	পূর্ণসংখ্যা clen;
	পূর্ণসংখ্যা দৈর্ঘ্য = EXT2_INODE_আকার(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		ফেরত EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	যদি (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		যদি (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ফেরত retval;
	}

	যদি ((ino == 0) || (ino > fs->super->s_inodes_গুনতি))
		ফেরত EXT2_ET_BAD_INODE_অঙ্ক;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(দৈর্ঘ্য, &w_inode);
	যদি (retval)
		ফেরত retval;

	যদি (bufsize < দৈর্ঘ্য) {
		retval = ext2fs_read_inode2(fs, ino,
					    (কাঠামো ext2_inode *)w_inode,
					    দৈর্ঘ্য, READ_INODE_NOCSUM);
		যদি (retval)
			যাও errout;
	}

	/* Check to see if the inode cache needs to be updated */
	যদি (fs->icache) {
		জন্য (i=0; i < fs->icache->cache_আকার; i++) {
			যদি (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > দৈর্ঘ্য) ? দৈর্ঘ্য : bufsize);
				বিরতি;
			}
		}
	} অন্যথা {
		retval = ext2fs_create_inode_cache(fs, 4);
		যদি (retval)
			যাও errout;
	}
	memcpy(w_inode, inode, (bufsize > দৈর্ঘ্য) ? দৈর্ঘ্য : bufsize);

	যদি (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		যাও errout;
	}

#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, দৈর্ঘ্য);
#endif

	যদি ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		যদি (retval)
			যাও errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_আকার(fs->super);
	block = offset >> EXT2_BLOCK_আকার_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	যদি (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		যাও errout;
	}
	যদি ((block_nr < fs->super->s_first_তথ্য_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_গুনতি(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		যাও errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_আকার(fs->super) - 1);

	ptr = (অক্ষর *) w_inode;

	যখন (দৈর্ঘ্য) {
		clen = দৈর্ঘ্য;
		যদি ((offset + দৈর্ঘ্য) > fs->blocksize)
			clen = fs->blocksize - offset;

		যদি (fs->icache->বাফার_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->বাফার);
			যদি (retval)
				যাও errout;
			fs->icache->বাফার_blk = block_nr;
		}


		memcpy((অক্ষর *) fs->icache->বাফার + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->বাফার);
		যদি (retval)
			যাও errout;

		offset = 0;
		ptr += clen;
		দৈর্ঘ্য -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	ফেরত retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  কাঠামো ext2_inode * inode, পূর্ণসংখ্যা bufsize)
{
	ফেরত ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     কাঠামো ext2_inode *inode)
{
	ফেরত ext2fs_write_inode2(fs, ino, inode,
				   আকার(কাঠামো ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 কাঠামো ext2_inode *inode)
{
	কাঠামো ext2_inode	*buf;
	পূর্ণসংখ্যা			আকার_2 = EXT2_INODE_আকার(fs->super);
	কাঠামো ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	যদি (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	যদি (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	যদি (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	যদি (আকার_2 == আকার(কাঠামো ext2_inode))
		ফেরত ext2fs_write_inode_full(fs, ino, inode,
					       আকার(কাঠামো ext2_inode));

	buf = malloc(আকার_2);
	যদি (!buf)
		ফেরত ENOMEM;

	memset(buf, 0, আকার_2);
	*buf = *inode;

	large_inode = (কাঠামো ext2_inode_large *) buf;
	large_inode->i_extra_isize = আকার(কাঠামো ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_আকার;
	যদি (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, আকার_2);
	মুক্ত(buf);
	ফেরত retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	কাঠামো ext2_inode	inode;
	পূর্ণসংখ্যা			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ino > fs->super->s_inodes_গুনতি)
		ফেরত EXT2_ET_BAD_INODE_অঙ্ক;

	যদি (fs->get_blocks) {
		যদি (!(*fs->get_blocks)(fs, ino, blocks))
			ফেরত 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ফেরত retval;
	জন্য (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	ফেরত 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	কাঠামো	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ino > fs->super->s_inodes_গুনতি)
		ফেরত EXT2_ET_BAD_INODE_অঙ্ক;

	যদি (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		যদি (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ফেরত retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ফেরত retval;
	যদি (!LINUX_S_ISDIR(inode.i_mode))
		ফেরত EXT2_ET_NO_DIRECTORY;
	ফেরত 0;
}

