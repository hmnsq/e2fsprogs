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

#include "config.h"
#include <stdio.h>
#include <string.h>
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#якщо HAVE_ERRNO_H
#include <errno.h>
#endif
#include <time.h>
#якщо HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#якщо HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

struct ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_left;
	blk_t			blocks_left;
	dgrp_t			groups_left;
	blk_t			inode_buffer_blocks;
	char *			inode_buffer;
	ціле			inode_size;
	char *			ptr;
	ціле			bytes_left;
	char			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      void * priv_data);
	void *			done_group_data;
	ціле			bad_block_ptr;
	ціле			scan_flags;
	ціле			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	якщо (!fs->icache)
		повернути 0;

	для (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	повернути 0;
}

/*
 * Free the inode cache structure
 */
void ext2fs_free_inode_cache(struct ext2_inode_cache *icache)
{
	unsigned i;

	якщо (--icache->refcount)
		повернути;
	якщо (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	для (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	якщо (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned ціле cache_size)
{
	unsigned	i;
	errcode_t	retval;

	якщо (fs->icache)
		повернути 0;
	retval = ext2fs_get_mem(sizeof(struct ext2_inode_cache), &fs->icache);
	якщо (retval)
		повернути retval;

	memset(fs->icache, 0, sizeof(struct ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	якщо (retval)
		goto errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  sizeof(struct ext2_inode_cache_ent),
				  &fs->icache->cache);
	якщо (retval)
		goto errout;

	для (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		якщо (retval)
			goto errout;
	}

	ext2fs_flush_icache(fs);
	повернути 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	повернути retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, ціле buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	якщо (ext2fs_has_feature_journal_dev(fs->super))
		повернути EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	якщо (fs->blocksize < 1024)
		повернути EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	якщо (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		якщо (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(sizeof(struct ext2_struct_inode_scan), &scan);
	якщо (retval)
		повернути retval;
	memset(scan, 0, sizeof(struct ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_left = 0;
	scan->current_group = 0;
	scan->groups_left = fs->group_desc_count - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	якщо (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_count(fs->super)))) {
		ext2fs_free_mem(&scan);
		повернути EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_left = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_left = scan->fs->inode_blocks_per_group;
	якщо (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		якщо (scan->inodes_left > unused)
			scan->inodes_left -= unused;
		інакше
			scan->inodes_left = 0;
		scan->blocks_left =
			(scan->inodes_left +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	якщо (retval) {
		ext2fs_free_mem(&scan);
		повернути retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	якщо (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		повернути retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	якщо (scan->fs->badblocks && scan->fs->badblocks->num)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	якщо (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	повернути 0;
}

void ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	якщо (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		повернути;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = NULL;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = NULL;
	ext2fs_free_mem(&scan);
	повернути;
}

void ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       void * priv_data),
			       void *done_group_data)
{
	якщо (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		повернути;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

ціле ext2fs_inode_scan_flags(ext2_inode_scan scan, ціле set_flags,
			    ціле clear_flags)
{
	ціле	old_flags;

	якщо (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		повернути 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	повернути old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
static errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_left--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_left = 0;
	scan->inodes_left = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_left = fs->inode_blocks_per_group;
	якщо (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		якщо (scan->inodes_left > unused)
			scan->inodes_left -= unused;
		інакше
			scan->inodes_left = 0;
		scan->blocks_left =
			(scan->inodes_left +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	якщо (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_count(fs->super))))
		повернути EXT2_ET_GDESC_BAD_INODE_TABLE;
	повернути 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    ціле	group)
{
	scan->current_group = group - 1;
	scan->groups_left = scan->fs->group_desc_count - group;
	scan->bad_block_ptr = 0;
	повернути get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
static errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *num_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	якщо (blk == 0)
		повернути 0;

	/* Make sure bad_block_ptr is still valid */
	якщо (scan->bad_block_ptr >= bb->num) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		повернути 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	поки (blk > bb->list[scan->bad_block_ptr]) {
		якщо (++scan->bad_block_ptr >= bb->num) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			повернути 0;
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
	якщо (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*num_blocks = 1;
		якщо (++scan->bad_block_ptr >= bb->num)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		повернути 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	якщо ((blk + *num_blocks) > bb->list[scan->bad_block_ptr])
		*num_blocks = (ціле) (bb->list[scan->bad_block_ptr] - blk);

	повернути 0;
}

static ціле block_map_looks_insane(ext2_filsys fs,
				  struct ext2_inode_large *inode)
{
	unsigned ціле i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	якщо ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		повернути 0;
	якщо (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		повернути 0;
	якщо (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= sizeof(inode->i_block))
		повернути 0;

	/* Unused inodes probably aren't insane */
	якщо (inode->i_links_count == 0)
		повернути 0;

	/* See if more than half the block maps are insane */
	для (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		якщо (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_count(fs->super)))
			bad++;
	повернути bad > EXT2_N_BLOCKS / 2;
}

static ціле extent_head_looks_insane(struct ext2_inode_large *inode)
{
	якщо (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					sizeof(inode->i_block)) == 0)
		повернути 0;
	повернути 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
static void check_inode_block_sanity(ext2_inode_scan scan, blk64_t num_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned ціле	badness, checksum_failures;
	unsigned ціле	inodes_in_buf, inodes_per_block;
	char		*p;
	struct ext2_inode_large *inode;
	char		*block_status;
	unsigned ціле	blk, bad_csum;

	якщо (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		повернути;

	inodes_to_scan = scan->inodes_left;
	inodes_in_buf = num_blocks * scan->fs->blocksize / scan->inode_size;
	якщо (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (char *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	якщо (inodes_per_block < 2)
		повернути;

#ifdef WORDS_BIGENDIAN
	якщо (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		повернути;
#endif

	поки (inodes_to_scan > 0) {
		blk = (p - (char *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(struct ext2_inode_large *) p) == 0;

#ifdef WORDS_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (struct ext2_inode_large *) inode,
			       (struct ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#інакше
		inode = (struct ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		якщо (bad_csum) {
			checksum_failures++;
			badness++;
		} інакше якщо (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		якщо (badness > inodes_per_block / 2) {
			unsigned ціле ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			якщо (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			continue;
		}

		якщо ((ino % inodes_per_block) == 0) {
			якщо (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef WORDS_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
static errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		num_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	num_blocks = scan->inode_buffer_blocks;
	якщо (num_blocks > scan->blocks_left)
		num_blocks = scan->blocks_left;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	якщо (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		якщо (scan->bytes_left)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	якщо (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &num_blocks);
		якщо (retval)
			повернути retval;
	}

	якщо ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) num_blocks * scan->fs->blocksize);
	} інакше {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (ціле) num_blocks,
					     scan->inode_buffer);
		якщо (retval)
			повернути EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, num_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_left = num_blocks * scan->fs->blocksize;

	scan->blocks_left -= num_blocks;
	якщо (scan->current_block)
		scan->current_block += num_blocks;

	повернути 0;
}

#якщо 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
static inline ціле is_empty_scan(ext2_inode_scan scan)
{
	ціле	i;

	якщо (scan->bytes_left == 0)
		повернути 0;

	для (i=0; i < scan->bytes_left; i++)
		якщо (scan->ptr[i])
			повернути 0;
	повернути 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     struct ext2_inode *inode, ціле bufsize)
{
	errcode_t	retval;
	ціле		extra_bytes = 0;
	ціле		length;
	struct ext2_inode_large	*iptr = (struct ext2_inode_large *)inode;
	char		*iblock_status;
	unsigned ціле	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	якщо (scan->inodes_left <= 0) {
	force_new_group:
		якщо (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			якщо (retval)
				повернути retval;
		}
		якщо (scan->groups_left <= 0) {
			*ino = 0;
			повернути 0;
		}
		retval = get_next_blockgroup(scan);
		якщо (retval)
			повернути retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	якщо ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		goto force_new_group;
	якщо (scan->inodes_left == 0)
		goto force_new_group;
	якщо (scan->current_block == 0) {
		якщо (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			goto force_new_group;
		} інакше
			повернути EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	якщо (scan->bytes_left < scan->inode_size) {
		якщо (scan->bytes_left)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_left);
		extra_bytes = scan->bytes_left;

		retval = get_next_blocks(scan);
		якщо (retval)
			повернути retval;
#якщо 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		якщо (is_empty_scan(scan))
			goto force_new_group;
#endif
	}

	якщо (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		якщо (retval)
			повернути retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	якщо (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_left -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		якщо (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(struct ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef WORDS_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (struct ext2_inode_large *) iptr,
			       (struct ext2_inode_large *) scan->temp_buffer,
			       0, length);
#інакше
		memcpy(iptr, scan->temp_buffer, length);
#endif
		якщо (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} інакше {
		/* Verify the inode checksum. */
		якщо (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(struct ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef WORDS_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(struct ext2_inode_large *) iptr,
				(struct ext2_inode_large *) scan->ptr,
				0, length);
#інакше
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_left -= scan->inode_size;
		якщо (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	якщо ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_left--;
	scan->current_inode++;
	*ino = scan->current_inode;
	якщо (iptr != (struct ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	повернути retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				struct ext2_inode *inode)
{
	повернути ext2fs_get_next_inode_full(scan, ino, inode,
						sizeof(struct ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     struct ext2_inode * inode, ціле bufsize,
			     ціле flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	char 		*ptr;
	errcode_t	retval;
	unsigned	i;
	ціле		clen, inodes_per_block;
	io_channel	io;
	ціле		length = EXT2_INODE_SIZE(fs->super);
	struct ext2_inode_large	*iptr;
	ціле		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	якщо (ext2fs_has_feature_journal_dev(fs->super))
		повернути EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	якщо (fs->blocksize < 1024)
		повернути EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	якщо (fs->read_inode &&
	    ((bufsize == sizeof(struct ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == sizeof(struct ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		якщо (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			повернути retval;
	}
	якщо ((ino == 0) || (ino > fs->super->s_inodes_count))
		повернути EXT2_ET_BAD_INODE_NUM;
	/* Create inode cache if not present */
	якщо (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		якщо (retval)
			повернути retval;
	}
	/* Check to see if it's in the inode cache */
	для (i = 0; i < fs->icache->cache_size; i++) {
		якщо (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			повернути 0;
		}
	}
	якщо (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} інакше {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		якщо (group > fs->group_desc_count)
			повернути EXT2_ET_BAD_INODE_NUM;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		якщо (!block_nr)
			повернути EXT2_ET_MISSING_INODE_TABLE;
		якщо ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_count(fs->super)))
			повернути EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (struct ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (char *) iptr;
	поки (length) {
		clen = length;
		якщо ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		якщо (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			якщо (retval)
				повернути retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((char *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_inode_full(fs, (struct ext2_inode_large *) iptr,
			       (struct ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	якщо (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	якщо (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		повернути EXT2_ET_INODE_CSUM_INVALID;

	повернути 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 struct ext2_inode * inode, ціле bufsize)
{
	повернути ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    struct ext2_inode * inode)
{
	повернути ext2fs_read_inode2(fs, ino, inode,
				  sizeof(struct ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      struct ext2_inode * inode, ціле bufsize,
			      ціле flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	struct ext2_inode_large *w_inode;
	char *ptr;
	unsigned i;
	ціле clen;
	ціле length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	якщо (ext2fs_has_feature_journal_dev(fs->super))
		повернути EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	якщо (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		якщо (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			повернути retval;
	}

	якщо ((ino == 0) || (ino > fs->super->s_inodes_count))
		повернути EXT2_ET_BAD_INODE_NUM;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	якщо (retval)
		повернути retval;

	якщо (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (struct ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		якщо (retval)
			goto errout;
	}

	/* Check to see if the inode cache needs to be updated */
	якщо (fs->icache) {
		для (i=0; i < fs->icache->cache_size; i++) {
			якщо (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				break;
			}
		}
	} інакше {
		retval = ext2fs_create_inode_cache(fs, 4);
		якщо (retval)
			goto errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	якщо (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		goto errout;
	}

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	якщо ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		якщо (retval)
			goto errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	якщо (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		goto errout;
	}
	якщо ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_count(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		goto errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (char *) w_inode;

	поки (length) {
		clen = length;
		якщо ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		якщо (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			якщо (retval)
				goto errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((char *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		якщо (retval)
			goto errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	повернути retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  struct ext2_inode * inode, ціле bufsize)
{
	повернути ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     struct ext2_inode *inode)
{
	повернути ext2fs_write_inode2(fs, ino, inode,
				   sizeof(struct ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 struct ext2_inode *inode)
{
	struct ext2_inode	*buf;
	ціле			size = EXT2_INODE_SIZE(fs->super);
	struct ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	якщо (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	якщо (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	якщо (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	якщо (size == sizeof(struct ext2_inode))
		повернути ext2fs_write_inode_full(fs, ino, inode,
					       sizeof(struct ext2_inode));

	buf = malloc(size);
	якщо (!buf)
		повернути ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (struct ext2_inode_large *) buf;
	large_inode->i_extra_isize = sizeof(struct ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	якщо (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	free(buf);
	повернути retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	struct ext2_inode	inode;
	ціле			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	якщо (ino > fs->super->s_inodes_count)
		повернути EXT2_ET_BAD_INODE_NUM;

	якщо (fs->get_blocks) {
		якщо (!(*fs->get_blocks)(fs, ino, blocks))
			повернути 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	якщо (retval)
		повернути retval;
	для (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	повернути 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	struct	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	якщо (ino > fs->super->s_inodes_count)
		повернути EXT2_ET_BAD_INODE_NUM;

	якщо (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		якщо (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			повернути retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	якщо (retval)
		повернути retval;
	якщо (!LINUX_S_ISDIR(inode.i_mode))
		повернути EXT2_ET_NO_DIRECTORY;
	повернути 0;
}

