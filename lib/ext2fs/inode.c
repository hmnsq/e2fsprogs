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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

ಸಂರಚನಾ ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_ಎಡ;
	blk_t			blocks_ಎಡ;
	dgrp_t			groups_ಎಡ;
	blk_t			inode_buffer_blocks;
	ವರ್ಣ *			inode_buffer;
	ಪೂರ್ಣಾಙ್ಕ			inode_size;
	ವರ್ಣ *			ptr;
	ಪೂರ್ಣಾಙ್ಕ			bytes_ಎಡ;
	ವರ್ಣ			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      ಶೂನ್ಯ * priv_data);
	ಶೂನ್ಯ *			done_group_data;
	ಪೂರ್ಣಾಙ್ಕ			bad_block_ptr;
	ಪೂರ್ಣಾಙ್ಕ			scan_flags;
	ಪೂರ್ಣಾಙ್ಕ			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	ಯದಿ (!fs->icache)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತಿ (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Free the inode cache structure
 */
ಶೂನ್ಯ ext2fs_free_inode_cache(ಸಂರಚನಾ ext2_inode_cache *icache)
{
	unsigned i;

	ಯದಿ (--icache->refcount)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	ಪ್ರತಿ (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	ಯದಿ (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned ಪೂರ್ಣಾಙ್ಕ cache_size)
{
	unsigned	i;
	errcode_t	retval;

	ಯದಿ (fs->icache)
		ಪ್ರತ್ಯಾಗಮ 0;
	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_cache), &fs->icache);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	memset(fs->icache, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_cache_ent),
				  &fs->icache->cache);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;

	ಪ್ರತಿ (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
	}

	ext2fs_flush_icache(fs);
	ಪ್ರತ್ಯಾಗಮ 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (ext2fs_has_feature_journal_dev(fs->super))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ಯದಿ (fs->blocksize < 1024)
		ಪ್ರತ್ಯಾಗಮ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	ಯದಿ (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		ಯದಿ (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_struct_inode_scan), &scan);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	memset(scan, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_ಎಡ = 0;
	scan->current_group = 0;
	scan->groups_ಎಡ = fs->group_desc_ಎಣಿಕೆ - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	ಯದಿ (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_ಎಣಿಕೆ(fs->super)))) {
		ext2fs_free_mem(&scan);
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_ಎಡ = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_ಎಡ = scan->fs->inode_blocks_per_group;
	ಯದಿ (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		ಯದಿ (scan->inodes_ಎಡ > unused)
			scan->inodes_ಎಡ -= unused;
		ಅನ್ಯಥಾ
			scan->inodes_ಎಡ = 0;
		scan->blocks_ಎಡ =
			(scan->inodes_ಎಡ +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	ಯದಿ (retval) {
		ext2fs_free_mem(&scan);
		ಪ್ರತ್ಯಾಗಮ retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	ಯದಿ (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		ಪ್ರತ್ಯಾಗಮ retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	ಯದಿ (scan->fs->badblocks && scan->fs->badblocks->ಅಂಕೆ)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	ಯದಿ (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	ಯದಿ (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ಪ್ರತ್ಯಾಗಮ;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = ರಿಕ್ತ;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = ರಿಕ್ತ;
	ext2fs_free_mem(&scan);
	ಪ್ರತ್ಯಾಗಮ;
}

ಶೂನ್ಯ ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       ಶೂನ್ಯ * priv_data),
			       ಶೂನ್ಯ *done_group_data)
{
	ಯದಿ (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ಪ್ರತ್ಯಾಗಮ;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_inode_scan_flags(ext2_inode_scan scan, ಪೂರ್ಣಾಙ್ಕ set_flags,
			    ಪೂರ್ಣಾಙ್ಕ clear_flags)
{
	ಪೂರ್ಣಾಙ್ಕ	old_flags;

	ಯದಿ (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		ಪ್ರತ್ಯಾಗಮ 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	ಪ್ರತ್ಯಾಗಮ old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ಸ್ಥಿರ errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_ಎಡ--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_ಎಡ = 0;
	scan->inodes_ಎಡ = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_ಎಡ = fs->inode_blocks_per_group;
	ಯದಿ (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		ಯದಿ (scan->inodes_ಎಡ > unused)
			scan->inodes_ಎಡ -= unused;
		ಅನ್ಯಥಾ
			scan->inodes_ಎಡ = 0;
		scan->blocks_ಎಡ =
			(scan->inodes_ಎಡ +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	ಯದಿ (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_ಎಣಿಕೆ(fs->super))))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_GDESC_BAD_INODE_TABLE;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    ಪೂರ್ಣಾಙ್ಕ	group)
{
	scan->current_group = group - 1;
	scan->groups_ಎಡ = scan->fs->group_desc_ಎಣಿಕೆ - group;
	scan->bad_block_ptr = 0;
	ಪ್ರತ್ಯಾಗಮ get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ಸ್ಥಿರ errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *ಅಂಕೆ_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	ಯದಿ (blk == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Make sure bad_block_ptr is still valid */
	ಯದಿ (scan->bad_block_ptr >= bb->ಅಂಕೆ) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	ಯಾವತ್ (blk > bb->list[scan->bad_block_ptr]) {
		ಯದಿ (++scan->bad_block_ptr >= bb->ಅಂಕೆ) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			ಪ್ರತ್ಯಾಗಮ 0;
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
	ಯದಿ (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*ಅಂಕೆ_blocks = 1;
		ಯದಿ (++scan->bad_block_ptr >= bb->ಅಂಕೆ)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	ಯದಿ ((blk + *ಅಂಕೆ_blocks) > bb->list[scan->bad_block_ptr])
		*ಅಂಕೆ_blocks = (ಪೂರ್ಣಾಙ್ಕ) (bb->list[scan->bad_block_ptr] - blk);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ block_map_looks_insane(ext2_filsys fs,
				  ಸಂರಚನಾ ext2_inode_large *inode)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	ಯದಿ ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= ಪರಿಮಾಣ(inode->i_block))
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Unused inodes probably aren't insane */
	ಯದಿ (inode->i_links_ಎಣಿಕೆ == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* See if more than half the block maps are insane */
	ಪ್ರತಿ (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		ಯದಿ (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_ಎಣಿಕೆ(fs->super)))
			bad++;
	ಪ್ರತ್ಯಾಗಮ bad > EXT2_N_BLOCKS / 2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ extent_head_looks_insane(ಸಂರಚನಾ ext2_inode_large *inode)
{
	ಯದಿ (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					ಪರಿಮಾಣ(inode->i_block)) == 0)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ಸ್ಥಿರ ಶೂನ್ಯ check_inode_block_sanity(ext2_inode_scan scan, blk64_t ಅಂಕೆ_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned ಪೂರ್ಣಾಙ್ಕ	badness, checksum_failures;
	unsigned ಪೂರ್ಣಾಙ್ಕ	inodes_in_buf, inodes_per_block;
	ವರ್ಣ		*p;
	ಸಂರಚನಾ ext2_inode_large *inode;
	ವರ್ಣ		*block_status;
	unsigned ಪೂರ್ಣಾಙ್ಕ	blk, bad_csum;

	ಯದಿ (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		ಪ್ರತ್ಯಾಗಮ;

	inodes_to_scan = scan->inodes_ಎಡ;
	inodes_in_buf = ಅಂಕೆ_blocks * scan->fs->blocksize / scan->inode_size;
	ಯದಿ (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (ವರ್ಣ *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	ಯದಿ (inodes_per_block < 2)
		ಪ್ರತ್ಯಾಗಮ;

#ifdef ಪದಗಳು_BIGENDIAN
	ಯದಿ (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		ಪ್ರತ್ಯಾಗಮ;
#endif

	ಯಾವತ್ (inodes_to_scan > 0) {
		blk = (p - (ವರ್ಣ *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(ಸಂರಚನಾ ext2_inode_large *) p) == 0;

#ifdef ಪದಗಳು_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (ಸಂರಚನಾ ext2_inode_large *) inode,
			       (ಸಂರಚನಾ ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#ಅನ್ಯಥಾ
		inode = (ಸಂರಚನಾ ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		ಯದಿ (bad_csum) {
			checksum_failures++;
			badness++;
		} ಅನ್ಯಥಾ ಯದಿ (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		ಯದಿ (badness > inodes_per_block / 2) {
			unsigned ಪೂರ್ಣಾಙ್ಕ ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			ಯದಿ (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			ಅನುವರ್ತ;
		}

		ಯದಿ ((ino % inodes_per_block) == 0) {
			ಯದಿ (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef ಪದಗಳು_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ಸ್ಥಿರ errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		ಅಂಕೆ_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	ಅಂಕೆ_blocks = scan->inode_buffer_blocks;
	ಯದಿ (ಅಂಕೆ_blocks > scan->blocks_ಎಡ)
		ಅಂಕೆ_blocks = scan->blocks_ಎಡ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	ಯದಿ (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		ಯದಿ (scan->bytes_ಎಡ)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	ಯದಿ (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &ಅಂಕೆ_blocks);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	ಯದಿ ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) ಅಂಕೆ_blocks * scan->fs->blocksize);
	} ಅನ್ಯಥಾ {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (ಪೂರ್ಣಾಙ್ಕ) ಅಂಕೆ_blocks,
					     scan->inode_buffer);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, ಅಂಕೆ_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_ಎಡ = ಅಂಕೆ_blocks * scan->fs->blocksize;

	scan->blocks_ಎಡ -= ಅಂಕೆ_blocks;
	ಯದಿ (scan->current_block)
		scan->current_block += ಅಂಕೆ_blocks;

	ಪ್ರತ್ಯಾಗಮ 0;
}

#ಯದಿ 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ is_empty_scan(ext2_inode_scan scan)
{
	ಪೂರ್ಣಾಙ್ಕ	i;

	ಯದಿ (scan->bytes_ಎಡ == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತಿ (i=0; i < scan->bytes_ಎಡ; i++)
		ಯದಿ (scan->ptr[i])
			ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     ಸಂರಚನಾ ext2_inode *inode, ಪೂರ್ಣಾಙ್ಕ bufsize)
{
	errcode_t	retval;
	ಪೂರ್ಣಾಙ್ಕ		extra_bytes = 0;
	ಪೂರ್ಣಾಙ್ಕ		length;
	ಸಂರಚನಾ ext2_inode_large	*iptr = (ಸಂರಚನಾ ext2_inode_large *)inode;
	ವರ್ಣ		*iblock_status;
	unsigned ಪೂರ್ಣಾಙ್ಕ	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	ಯದಿ (scan->inodes_ಎಡ <= 0) {
	force_new_group:
		ಯದಿ (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
		}
		ಯದಿ (scan->groups_ಎಡ <= 0) {
			*ino = 0;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		retval = get_next_blockgroup(scan);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	ಯದಿ ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		ಗಚ್ಛ force_new_group;
	ಯದಿ (scan->inodes_ಎಡ == 0)
		ಗಚ್ಛ force_new_group;
	ಯದಿ (scan->current_block == 0) {
		ಯದಿ (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			ಗಚ್ಛ force_new_group;
		} ಅನ್ಯಥಾ
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	ಯದಿ (scan->bytes_ಎಡ < scan->inode_size) {
		ಯದಿ (scan->bytes_ಎಡ)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_ಎಡ);
		extra_bytes = scan->bytes_ಎಡ;

		retval = get_next_blocks(scan);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
#ಯದಿ 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		ಯದಿ (is_empty_scan(scan))
			ಗಚ್ಛ force_new_group;
#endif
	}

	ಯದಿ (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	ಯದಿ (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_ಎಡ -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		ಯದಿ (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(ಸಂರಚನಾ ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef ಪದಗಳು_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (ಸಂರಚನಾ ext2_inode_large *) iptr,
			       (ಸಂರಚನಾ ext2_inode_large *) scan->temp_buffer,
			       0, length);
#ಅನ್ಯಥಾ
		memcpy(iptr, scan->temp_buffer, length);
#endif
		ಯದಿ (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} ಅನ್ಯಥಾ {
		/* Verify the inode checksum. */
		ಯದಿ (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(ಸಂರಚನಾ ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef ಪದಗಳು_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(ಸಂರಚನಾ ext2_inode_large *) iptr,
				(ಸಂರಚನಾ ext2_inode_large *) scan->ptr,
				0, length);
#ಅನ್ಯಥಾ
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_ಎಡ -= scan->inode_size;
		ಯದಿ (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	ಯದಿ ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_ಎಡ--;
	scan->current_inode++;
	*ino = scan->current_inode;
	ಯದಿ (iptr != (ಸಂರಚನಾ ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				ಸಂರಚನಾ ext2_inode *inode)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_get_next_inode_full(scan, ino, inode,
						ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     ಸಂರಚನಾ ext2_inode * inode, ಪೂರ್ಣಾಙ್ಕ bufsize,
			     ಪೂರ್ಣಾಙ್ಕ flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	ವರ್ಣ 		*ptr;
	errcode_t	retval;
	unsigned	i;
	ಪೂರ್ಣಾಙ್ಕ		clen, inodes_per_block;
	io_channel	io;
	ಪೂರ್ಣಾಙ್ಕ		length = EXT2_INODE_SIZE(fs->super);
	ಸಂರಚನಾ ext2_inode_large	*iptr;
	ಪೂರ್ಣಾಙ್ಕ		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (ext2fs_has_feature_journal_dev(fs->super))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ಯದಿ (fs->blocksize < 1024)
		ಪ್ರತ್ಯಾಗಮ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	ಯದಿ (fs->read_inode &&
	    ((bufsize == ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		ಯದಿ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	ಯದಿ ((ino == 0) || (ino > fs->super->s_inodes_ಎಣಿಕೆ))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_INODE_ಅಂಕೆ;
	/* Create inode cache if not present */
	ಯದಿ (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	/* Check to see if it's in the inode cache */
	ಪ್ರತಿ (i = 0; i < fs->icache->cache_size; i++) {
		ಯದಿ (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}
	ಯದಿ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} ಅನ್ಯಥಾ {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		ಯದಿ (group > fs->group_desc_ಎಣಿಕೆ)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_INODE_ಅಂಕೆ;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		ಯದಿ (!block_nr)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_MISSING_INODE_TABLE;
		ಯದಿ ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_ಎಣಿಕೆ(fs->super)))
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (ಸಂರಚನಾ ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (ವರ್ಣ *) iptr;
	ಯಾವತ್ (length) {
		clen = length;
		ಯದಿ ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		ಯದಿ (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((ವರ್ಣ *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef ಪದಗಳು_BIGENDIAN
	ext2fs_swap_inode_full(fs, (ಸಂರಚನಾ ext2_inode_large *) iptr,
			       (ಸಂರಚನಾ ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	ಯದಿ (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	ಯದಿ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INODE_CSUM_INVALID;

	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 ಸಂರಚನಾ ext2_inode * inode, ಪೂರ್ಣಾಙ್ಕ bufsize)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ಸಂರಚನಾ ext2_inode * inode)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_read_inode2(fs, ino, inode,
				  ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      ಸಂರಚನಾ ext2_inode * inode, ಪೂರ್ಣಾಙ್ಕ bufsize,
			      ಪೂರ್ಣಾಙ್ಕ flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	ಸಂರಚನಾ ext2_inode_large *w_inode;
	ವರ್ಣ *ptr;
	unsigned i;
	ಪೂರ್ಣಾಙ್ಕ clen;
	ಪೂರ್ಣಾಙ್ಕ length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (ext2fs_has_feature_journal_dev(fs->super))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	ಯದಿ (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		ಯದಿ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	ಯದಿ ((ino == 0) || (ino > fs->super->s_inodes_ಎಣಿಕೆ))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_INODE_ಅಂಕೆ;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (ಸಂರಚನಾ ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
	}

	/* Check to see if the inode cache needs to be updated */
	ಯದಿ (fs->icache) {
		ಪ್ರತಿ (i=0; i < fs->icache->cache_size; i++) {
			ಯದಿ (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				ಭಙ್ಗ;
			}
		}
	} ಅನ್ಯಥಾ {
		retval = ext2fs_create_inode_cache(fs, 4);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	ಯದಿ (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		ಗಚ್ಛ errout;
	}

#ifdef ಪದಗಳು_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	ಯದಿ ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	ಯದಿ (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		ಗಚ್ಛ errout;
	}
	ಯದಿ ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_ಎಣಿಕೆ(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		ಗಚ್ಛ errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (ವರ್ಣ *) w_inode;

	ಯಾವತ್ (length) {
		clen = length;
		ಯದಿ ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		ಯದಿ (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((ವರ್ಣ *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		ಯದಿ (retval)
			ಗಚ್ಛ errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  ಸಂರಚನಾ ext2_inode * inode, ಪೂರ್ಣಾಙ್ಕ bufsize)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     ಸಂರಚನಾ ext2_inode *inode)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_write_inode2(fs, ino, inode,
				   ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 ಸಂರಚನಾ ext2_inode *inode)
{
	ಸಂರಚನಾ ext2_inode	*buf;
	ಪೂರ್ಣಾಙ್ಕ			size = EXT2_INODE_SIZE(fs->super);
	ಸಂರಚನಾ ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	ಯದಿ (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	ಯದಿ (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	ಯದಿ (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	ಯದಿ (size == ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode))
		ಪ್ರತ್ಯಾಗಮ ext2fs_write_inode_full(fs, ino, inode,
					       ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode));

	buf = malloc(size);
	ಯದಿ (!buf)
		ಪ್ರತ್ಯಾಗಮ ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (ಸಂರಚನಾ ext2_inode_large *) buf;
	large_inode->i_extra_isize = ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	ಯದಿ (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	ಸಂರಚನಾ ext2_inode	inode;
	ಪೂರ್ಣಾಙ್ಕ			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (ino > fs->super->s_inodes_ಎಣಿಕೆ)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_INODE_ಅಂಕೆ;

	ಯದಿ (fs->get_blocks) {
		ಯದಿ (!(*fs->get_blocks)(fs, ino, blocks))
			ಪ್ರತ್ಯಾಗಮ 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಪ್ರತಿ (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	ಸಂರಚನಾ	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (ino > fs->super->s_inodes_ಎಣಿಕೆ)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_INODE_ಅಂಕೆ;

	ಯದಿ (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		ಯದಿ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (!LINUX_S_ISDIR(inode.i_mode))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_DIRECTORY;
	ಪ್ರತ್ಯಾಗಮ 0;
}

