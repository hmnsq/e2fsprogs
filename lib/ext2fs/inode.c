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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

ساختار ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_چپ;
	blk_t			blocks_چپ;
	dgrp_t			groups_چپ;
	blk_t			inode_buffer_blocks;
	نویسه *			inode_buffer;
	صحیح			inode_size;
	نویسه *			ptr;
	صحیح			bytes_چپ;
	نویسه			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      تهی * priv_data);
	تهی *			done_group_data;
	صحیح			bad_block_ptr;
	صحیح			scan_flags;
	صحیح			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	اگر (!fs->icache)
		بازگشت 0;

	برای (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	بازگشت 0;
}

/*
 * Free the inode cache structure
 */
تهی ext2fs_free_inode_cache(ساختار ext2_inode_cache *icache)
{
	unsigned i;

	اگر (--icache->refcount)
		بازگشت;
	اگر (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	برای (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	اگر (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned صحیح cache_size)
{
	unsigned	i;
	errcode_t	retval;

	اگر (fs->icache)
		بازگشت 0;
	retval = ext2fs_get_mem(اندازه(ساختار ext2_inode_cache), &fs->icache);
	اگر (retval)
		بازگشت retval;

	memset(fs->icache, 0, اندازه(ساختار ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	اگر (retval)
		برو errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  اندازه(ساختار ext2_inode_cache_ent),
				  &fs->icache->cache);
	اگر (retval)
		برو errout;

	برای (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		اگر (retval)
			برو errout;
	}

	ext2fs_flush_icache(fs);
	بازگشت 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	بازگشت retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, صحیح buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		بازگشت EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	اگر (fs->blocksize < 1024)
		بازگشت EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	اگر (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		اگر (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(اندازه(ساختار ext2_struct_inode_scan), &scan);
	اگر (retval)
		بازگشت retval;
	memset(scan, 0, اندازه(ساختار ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_چپ = 0;
	scan->current_group = 0;
	scan->groups_چپ = fs->group_desc_شمار - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	اگر (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_شمار(fs->super)))) {
		ext2fs_free_mem(&scan);
		بازگشت EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_چپ = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_چپ = scan->fs->inode_blocks_per_group;
	اگر (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		اگر (scan->inodes_چپ > unused)
			scan->inodes_چپ -= unused;
		وگرنه
			scan->inodes_چپ = 0;
		scan->blocks_چپ =
			(scan->inodes_چپ +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	اگر (retval) {
		ext2fs_free_mem(&scan);
		بازگشت retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	اگر (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		بازگشت retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	اگر (scan->fs->badblocks && scan->fs->badblocks->رقم)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	اگر (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	بازگشت 0;
}

تهی ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	اگر (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		بازگشت;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = پوچ;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = پوچ;
	ext2fs_free_mem(&scan);
	بازگشت;
}

تهی ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       تهی * priv_data),
			       تهی *done_group_data)
{
	اگر (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		بازگشت;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

صحیح ext2fs_inode_scan_flags(ext2_inode_scan scan, صحیح set_flags,
			    صحیح clear_flags)
{
	صحیح	old_flags;

	اگر (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		بازگشت 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	بازگشت old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ایستا errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_چپ--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_چپ = 0;
	scan->inodes_چپ = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_چپ = fs->inode_blocks_per_group;
	اگر (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		اگر (scan->inodes_چپ > unused)
			scan->inodes_چپ -= unused;
		وگرنه
			scan->inodes_چپ = 0;
		scan->blocks_چپ =
			(scan->inodes_چپ +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	اگر (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_شمار(fs->super))))
		بازگشت EXT2_ET_GDESC_BAD_INODE_TABLE;
	بازگشت 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    صحیح	group)
{
	scan->current_group = group - 1;
	scan->groups_چپ = scan->fs->group_desc_شمار - group;
	scan->bad_block_ptr = 0;
	بازگشت get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ایستا errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *رقم_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	اگر (blk == 0)
		بازگشت 0;

	/* Make sure bad_block_ptr is still valid */
	اگر (scan->bad_block_ptr >= bb->رقم) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		بازگشت 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	وقتی (blk > bb->list[scan->bad_block_ptr]) {
		اگر (++scan->bad_block_ptr >= bb->رقم) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			بازگشت 0;
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
	اگر (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*رقم_blocks = 1;
		اگر (++scan->bad_block_ptr >= bb->رقم)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		بازگشت 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	اگر ((blk + *رقم_blocks) > bb->list[scan->bad_block_ptr])
		*رقم_blocks = (صحیح) (bb->list[scan->bad_block_ptr] - blk);

	بازگشت 0;
}

ایستا صحیح block_map_looks_insane(ext2_filsys fs,
				  ساختار ext2_inode_large *inode)
{
	unsigned صحیح i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	اگر ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		بازگشت 0;
	اگر (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		بازگشت 0;
	اگر (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= اندازه(inode->i_block))
		بازگشت 0;

	/* Unused inodes probably aren't insane */
	اگر (inode->i_links_شمار == 0)
		بازگشت 0;

	/* See if more than half the block maps are insane */
	برای (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		اگر (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_شمار(fs->super)))
			bad++;
	بازگشت bad > EXT2_N_BLOCKS / 2;
}

ایستا صحیح extent_head_looks_insane(ساختار ext2_inode_large *inode)
{
	اگر (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					اندازه(inode->i_block)) == 0)
		بازگشت 0;
	بازگشت 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ایستا تهی check_inode_block_sanity(ext2_inode_scan scan, blk64_t رقم_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned صحیح	badness, checksum_failures;
	unsigned صحیح	inodes_in_buf, inodes_per_block;
	نویسه		*p;
	ساختار ext2_inode_large *inode;
	نویسه		*block_status;
	unsigned صحیح	blk, bad_csum;

	اگر (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		بازگشت;

	inodes_to_scan = scan->inodes_چپ;
	inodes_in_buf = رقم_blocks * scan->fs->blocksize / scan->inode_size;
	اگر (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (نویسه *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	اگر (inodes_per_block < 2)
		بازگشت;

#ifdef واژگان_BIGENDIAN
	اگر (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		بازگشت;
#endif

	وقتی (inodes_to_scan > 0) {
		blk = (p - (نویسه *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(ساختار ext2_inode_large *) p) == 0;

#ifdef واژگان_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (ساختار ext2_inode_large *) inode,
			       (ساختار ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#وگرنه
		inode = (ساختار ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		اگر (bad_csum) {
			checksum_failures++;
			badness++;
		} وگرنه اگر (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		اگر (badness > inodes_per_block / 2) {
			unsigned صحیح ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			اگر (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			ادامه;
		}

		اگر ((ino % inodes_per_block) == 0) {
			اگر (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef واژگان_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ایستا errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		رقم_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	رقم_blocks = scan->inode_buffer_blocks;
	اگر (رقم_blocks > scan->blocks_چپ)
		رقم_blocks = scan->blocks_چپ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	اگر (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		اگر (scan->bytes_چپ)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	اگر (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &رقم_blocks);
		اگر (retval)
			بازگشت retval;
	}

	اگر ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) رقم_blocks * scan->fs->blocksize);
	} وگرنه {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (صحیح) رقم_blocks,
					     scan->inode_buffer);
		اگر (retval)
			بازگشت EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, رقم_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_چپ = رقم_blocks * scan->fs->blocksize;

	scan->blocks_چپ -= رقم_blocks;
	اگر (scan->current_block)
		scan->current_block += رقم_blocks;

	بازگشت 0;
}

#اگر 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ایستا درونخطی صحیح is_empty_scan(ext2_inode_scan scan)
{
	صحیح	i;

	اگر (scan->bytes_چپ == 0)
		بازگشت 0;

	برای (i=0; i < scan->bytes_چپ; i++)
		اگر (scan->ptr[i])
			بازگشت 0;
	بازگشت 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     ساختار ext2_inode *inode, صحیح bufsize)
{
	errcode_t	retval;
	صحیح		extra_bytes = 0;
	صحیح		length;
	ساختار ext2_inode_large	*iptr = (ساختار ext2_inode_large *)inode;
	نویسه		*iblock_status;
	unsigned صحیح	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	اگر (scan->inodes_چپ <= 0) {
	force_new_group:
		اگر (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			اگر (retval)
				بازگشت retval;
		}
		اگر (scan->groups_چپ <= 0) {
			*ino = 0;
			بازگشت 0;
		}
		retval = get_next_blockgroup(scan);
		اگر (retval)
			بازگشت retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	اگر ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		برو force_new_group;
	اگر (scan->inodes_چپ == 0)
		برو force_new_group;
	اگر (scan->current_block == 0) {
		اگر (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			برو force_new_group;
		} وگرنه
			بازگشت EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	اگر (scan->bytes_چپ < scan->inode_size) {
		اگر (scan->bytes_چپ)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_چپ);
		extra_bytes = scan->bytes_چپ;

		retval = get_next_blocks(scan);
		اگر (retval)
			بازگشت retval;
#اگر 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		اگر (is_empty_scan(scan))
			برو force_new_group;
#endif
	}

	اگر (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		اگر (retval)
			بازگشت retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	اگر (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_چپ -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		اگر (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(ساختار ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef واژگان_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (ساختار ext2_inode_large *) iptr,
			       (ساختار ext2_inode_large *) scan->temp_buffer,
			       0, length);
#وگرنه
		memcpy(iptr, scan->temp_buffer, length);
#endif
		اگر (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} وگرنه {
		/* Verify the inode checksum. */
		اگر (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(ساختار ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef واژگان_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(ساختار ext2_inode_large *) iptr,
				(ساختار ext2_inode_large *) scan->ptr,
				0, length);
#وگرنه
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_چپ -= scan->inode_size;
		اگر (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	اگر ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_چپ--;
	scan->current_inode++;
	*ino = scan->current_inode;
	اگر (iptr != (ساختار ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	بازگشت retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				ساختار ext2_inode *inode)
{
	بازگشت ext2fs_get_next_inode_full(scan, ino, inode,
						اندازه(ساختار ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     ساختار ext2_inode * inode, صحیح bufsize,
			     صحیح flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	نویسه 		*ptr;
	errcode_t	retval;
	unsigned	i;
	صحیح		clen, inodes_per_block;
	io_channel	io;
	صحیح		length = EXT2_INODE_SIZE(fs->super);
	ساختار ext2_inode_large	*iptr;
	صحیح		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		بازگشت EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	اگر (fs->blocksize < 1024)
		بازگشت EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	اگر (fs->read_inode &&
	    ((bufsize == اندازه(ساختار ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == اندازه(ساختار ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		اگر (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			بازگشت retval;
	}
	اگر ((ino == 0) || (ino > fs->super->s_inodes_شمار))
		بازگشت EXT2_ET_BAD_INODE_رقم;
	/* Create inode cache if not present */
	اگر (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		اگر (retval)
			بازگشت retval;
	}
	/* Check to see if it's in the inode cache */
	برای (i = 0; i < fs->icache->cache_size; i++) {
		اگر (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			بازگشت 0;
		}
	}
	اگر (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} وگرنه {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		اگر (group > fs->group_desc_شمار)
			بازگشت EXT2_ET_BAD_INODE_رقم;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		اگر (!block_nr)
			بازگشت EXT2_ET_MISSING_INODE_TABLE;
		اگر ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_شمار(fs->super)))
			بازگشت EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (ساختار ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (نویسه *) iptr;
	وقتی (length) {
		clen = length;
		اگر ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		اگر (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			اگر (retval)
				بازگشت retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((نویسه *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef واژگان_BIGENDIAN
	ext2fs_swap_inode_full(fs, (ساختار ext2_inode_large *) iptr,
			       (ساختار ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	اگر (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	اگر (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		بازگشت EXT2_ET_INODE_CSUM_INVALID;

	بازگشت 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 ساختار ext2_inode * inode, صحیح bufsize)
{
	بازگشت ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ساختار ext2_inode * inode)
{
	بازگشت ext2fs_read_inode2(fs, ino, inode,
				  اندازه(ساختار ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      ساختار ext2_inode * inode, صحیح bufsize,
			      صحیح flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	ساختار ext2_inode_large *w_inode;
	نویسه *ptr;
	unsigned i;
	صحیح clen;
	صحیح length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		بازگشت EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	اگر (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		اگر (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			بازگشت retval;
	}

	اگر ((ino == 0) || (ino > fs->super->s_inodes_شمار))
		بازگشت EXT2_ET_BAD_INODE_رقم;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	اگر (retval)
		بازگشت retval;

	اگر (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (ساختار ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		اگر (retval)
			برو errout;
	}

	/* Check to see if the inode cache needs to be updated */
	اگر (fs->icache) {
		برای (i=0; i < fs->icache->cache_size; i++) {
			اگر (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				بشکن;
			}
		}
	} وگرنه {
		retval = ext2fs_create_inode_cache(fs, 4);
		اگر (retval)
			برو errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	اگر (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		برو errout;
	}

#ifdef واژگان_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	اگر ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		اگر (retval)
			برو errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	اگر (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		برو errout;
	}
	اگر ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_شمار(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		برو errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (نویسه *) w_inode;

	وقتی (length) {
		clen = length;
		اگر ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		اگر (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			اگر (retval)
				برو errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((نویسه *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		اگر (retval)
			برو errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	بازگشت retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  ساختار ext2_inode * inode, صحیح bufsize)
{
	بازگشت ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     ساختار ext2_inode *inode)
{
	بازگشت ext2fs_write_inode2(fs, ino, inode,
				   اندازه(ساختار ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 ساختار ext2_inode *inode)
{
	ساختار ext2_inode	*buf;
	صحیح			size = EXT2_INODE_SIZE(fs->super);
	ساختار ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	اگر (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	اگر (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	اگر (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	اگر (size == اندازه(ساختار ext2_inode))
		بازگشت ext2fs_write_inode_full(fs, ino, inode,
					       اندازه(ساختار ext2_inode));

	buf = malloc(size);
	اگر (!buf)
		بازگشت ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (ساختار ext2_inode_large *) buf;
	large_inode->i_extra_isize = اندازه(ساختار ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	اگر (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	آزاد(buf);
	بازگشت retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	ساختار ext2_inode	inode;
	صحیح			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ino > fs->super->s_inodes_شمار)
		بازگشت EXT2_ET_BAD_INODE_رقم;

	اگر (fs->get_blocks) {
		اگر (!(*fs->get_blocks)(fs, ino, blocks))
			بازگشت 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		بازگشت retval;
	برای (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	بازگشت 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	ساختار	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ino > fs->super->s_inodes_شمار)
		بازگشت EXT2_ET_BAD_INODE_رقم;

	اگر (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		اگر (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			بازگشت retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		بازگشت retval;
	اگر (!LINUX_S_ISDIR(inode.i_mode))
		بازگشت EXT2_ET_NO_DIRECTORY;
	بازگشت 0;
}

