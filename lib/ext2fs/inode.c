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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

بنية ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_بایاں;
	blk_t			blocks_بایاں;
	dgrp_t			groups_بایاں;
	blk_t			inode_buffer_blocks;
	حرف *			inode_buffer;
	صحيح			inode_size;
	حرف *			ptr;
	صحيح			bytes_بایاں;
	حرف			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      فراغ * priv_data);
	فراغ *			done_group_data;
	صحيح			bad_block_ptr;
	صحيح			scan_flags;
	صحيح			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	إذا (!fs->icache)
		أرجع 0;

	لكل (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	أرجع 0;
}

/*
 * Free the inode cache structure
 */
فراغ ext2fs_free_inode_cache(بنية ext2_inode_cache *icache)
{
	unsigned i;

	إذا (--icache->refcount)
		أرجع;
	إذا (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	لكل (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	إذا (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned صحيح cache_size)
{
	unsigned	i;
	errcode_t	retval;

	إذا (fs->icache)
		أرجع 0;
	retval = ext2fs_get_mem(حجم(بنية ext2_inode_cache), &fs->icache);
	إذا (retval)
		أرجع retval;

	memset(fs->icache, 0, حجم(بنية ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	إذا (retval)
		اذهب errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  حجم(بنية ext2_inode_cache_ent),
				  &fs->icache->cache);
	إذا (retval)
		اذهب errout;

	لكل (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		إذا (retval)
			اذهب errout;
	}

	ext2fs_flush_icache(fs);
	أرجع 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	أرجع retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, صحيح buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	إذا (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		إذا (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_inode_scan), &scan);
	إذا (retval)
		أرجع retval;
	memset(scan, 0, حجم(بنية ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_بایاں = 0;
	scan->current_group = 0;
	scan->groups_بایاں = fs->group_desc_گنتی - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	إذا (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_گنتی(fs->super)))) {
		ext2fs_free_mem(&scan);
		أرجع EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_بایاں = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_بایاں = scan->fs->inode_blocks_per_group;
	إذا (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		إذا (scan->inodes_بایاں > unused)
			scan->inodes_بایاں -= unused;
		وإلا
			scan->inodes_بایاں = 0;
		scan->blocks_بایاں =
			(scan->inodes_بایاں +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	إذا (retval) {
		ext2fs_free_mem(&scan);
		أرجع retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	إذا (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		أرجع retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	إذا (scan->fs->badblocks && scan->fs->badblocks->عدد)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	إذا (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	أرجع 0;
}

فراغ ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	إذا (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		أرجع;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = عدم;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = عدم;
	ext2fs_free_mem(&scan);
	أرجع;
}

فراغ ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       فراغ * priv_data),
			       فراغ *done_group_data)
{
	إذا (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		أرجع;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

صحيح ext2fs_inode_scan_flags(ext2_inode_scan scan, صحيح set_flags,
			    صحيح clear_flags)
{
	صحيح	old_flags;

	إذا (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		أرجع 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	أرجع old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ساكن errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_بایاں--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_بایاں = 0;
	scan->inodes_بایاں = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_بایاں = fs->inode_blocks_per_group;
	إذا (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		إذا (scan->inodes_بایاں > unused)
			scan->inodes_بایاں -= unused;
		وإلا
			scan->inodes_بایاں = 0;
		scan->blocks_بایاں =
			(scan->inodes_بایاں +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	إذا (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_گنتی(fs->super))))
		أرجع EXT2_ET_GDESC_BAD_INODE_TABLE;
	أرجع 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    صحيح	group)
{
	scan->current_group = group - 1;
	scan->groups_بایاں = scan->fs->group_desc_گنتی - group;
	scan->bad_block_ptr = 0;
	أرجع get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ساكن errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *عدد_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	إذا (blk == 0)
		أرجع 0;

	/* Make sure bad_block_ptr is still valid */
	إذا (scan->bad_block_ptr >= bb->عدد) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	طالما (blk > bb->list[scan->bad_block_ptr]) {
		إذا (++scan->bad_block_ptr >= bb->عدد) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			أرجع 0;
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
	إذا (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*عدد_blocks = 1;
		إذا (++scan->bad_block_ptr >= bb->عدد)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	إذا ((blk + *عدد_blocks) > bb->list[scan->bad_block_ptr])
		*عدد_blocks = (صحيح) (bb->list[scan->bad_block_ptr] - blk);

	أرجع 0;
}

ساكن صحيح block_map_looks_insane(ext2_filsys fs,
				  بنية ext2_inode_large *inode)
{
	unsigned صحيح i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	إذا ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		أرجع 0;
	إذا (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		أرجع 0;
	إذا (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= حجم(inode->i_block))
		أرجع 0;

	/* Unused inodes probably aren't insane */
	إذا (inode->i_links_گنتی == 0)
		أرجع 0;

	/* See if more than half the block maps are insane */
	لكل (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		إذا (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_گنتی(fs->super)))
			bad++;
	أرجع bad > EXT2_N_BLOCKS / 2;
}

ساكن صحيح extent_head_looks_insane(بنية ext2_inode_large *inode)
{
	إذا (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					حجم(inode->i_block)) == 0)
		أرجع 0;
	أرجع 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ساكن فراغ check_inode_block_sanity(ext2_inode_scan scan, blk64_t عدد_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned صحيح	badness, checksum_failures;
	unsigned صحيح	inodes_in_buf, inodes_per_block;
	حرف		*p;
	بنية ext2_inode_large *inode;
	حرف		*block_status;
	unsigned صحيح	blk, bad_csum;

	إذا (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		أرجع;

	inodes_to_scan = scan->inodes_بایاں;
	inodes_in_buf = عدد_blocks * scan->fs->blocksize / scan->inode_size;
	إذا (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (حرف *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	إذا (inodes_per_block < 2)
		أرجع;

#ifdef الفاظ_BIGENDIAN
	إذا (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		أرجع;
#endif

	طالما (inodes_to_scan > 0) {
		blk = (p - (حرف *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(بنية ext2_inode_large *) p) == 0;

#ifdef الفاظ_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (بنية ext2_inode_large *) inode,
			       (بنية ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#وإلا
		inode = (بنية ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		إذا (bad_csum) {
			checksum_failures++;
			badness++;
		} وإلا إذا (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		إذا (badness > inodes_per_block / 2) {
			unsigned صحيح ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			إذا (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			استمر;
		}

		إذا ((ino % inodes_per_block) == 0) {
			إذا (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef الفاظ_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ساكن errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		عدد_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	عدد_blocks = scan->inode_buffer_blocks;
	إذا (عدد_blocks > scan->blocks_بایاں)
		عدد_blocks = scan->blocks_بایاں;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	إذا (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		إذا (scan->bytes_بایاں)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	إذا (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &عدد_blocks);
		إذا (retval)
			أرجع retval;
	}

	إذا ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) عدد_blocks * scan->fs->blocksize);
	} وإلا {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (صحيح) عدد_blocks,
					     scan->inode_buffer);
		إذا (retval)
			أرجع EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, عدد_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_بایاں = عدد_blocks * scan->fs->blocksize;

	scan->blocks_بایاں -= عدد_blocks;
	إذا (scan->current_block)
		scan->current_block += عدد_blocks;

	أرجع 0;
}

#إذا 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ساكن مضمن صحيح is_empty_scan(ext2_inode_scan scan)
{
	صحيح	i;

	إذا (scan->bytes_بایاں == 0)
		أرجع 0;

	لكل (i=0; i < scan->bytes_بایاں; i++)
		إذا (scan->ptr[i])
			أرجع 0;
	أرجع 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     بنية ext2_inode *inode, صحيح bufsize)
{
	errcode_t	retval;
	صحيح		extra_bytes = 0;
	صحيح		length;
	بنية ext2_inode_large	*iptr = (بنية ext2_inode_large *)inode;
	حرف		*iblock_status;
	unsigned صحيح	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	إذا (scan->inodes_بایاں <= 0) {
	force_new_group:
		إذا (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			إذا (retval)
				أرجع retval;
		}
		إذا (scan->groups_بایاں <= 0) {
			*ino = 0;
			أرجع 0;
		}
		retval = get_next_blockgroup(scan);
		إذا (retval)
			أرجع retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	إذا ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		اذهب force_new_group;
	إذا (scan->inodes_بایاں == 0)
		اذهب force_new_group;
	إذا (scan->current_block == 0) {
		إذا (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			اذهب force_new_group;
		} وإلا
			أرجع EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	إذا (scan->bytes_بایاں < scan->inode_size) {
		إذا (scan->bytes_بایاں)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_بایاں);
		extra_bytes = scan->bytes_بایاں;

		retval = get_next_blocks(scan);
		إذا (retval)
			أرجع retval;
#إذا 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		إذا (is_empty_scan(scan))
			اذهب force_new_group;
#endif
	}

	إذا (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		إذا (retval)
			أرجع retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	إذا (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_بایاں -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		إذا (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(بنية ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef الفاظ_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) scan->temp_buffer,
			       0, length);
#وإلا
		memcpy(iptr, scan->temp_buffer, length);
#endif
		إذا (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} وإلا {
		/* Verify the inode checksum. */
		إذا (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(بنية ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef الفاظ_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(بنية ext2_inode_large *) iptr,
				(بنية ext2_inode_large *) scan->ptr,
				0, length);
#وإلا
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_بایاں -= scan->inode_size;
		إذا (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	إذا ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_بایاں--;
	scan->current_inode++;
	*ino = scan->current_inode;
	إذا (iptr != (بنية ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	أرجع retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				بنية ext2_inode *inode)
{
	أرجع ext2fs_get_next_inode_full(scan, ino, inode,
						حجم(بنية ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode * inode, صحيح bufsize,
			     صحيح flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	حرف 		*ptr;
	errcode_t	retval;
	unsigned	i;
	صحيح		clen, inodes_per_block;
	io_channel	io;
	صحيح		length = EXT2_INODE_SIZE(fs->super);
	بنية ext2_inode_large	*iptr;
	صحيح		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	إذا (fs->read_inode &&
	    ((bufsize == حجم(بنية ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == حجم(بنية ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}
	إذا ((ino == 0) || (ino > fs->super->s_inodes_گنتی))
		أرجع EXT2_ET_BAD_INODE_عدد;
	/* Create inode cache if not present */
	إذا (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		إذا (retval)
			أرجع retval;
	}
	/* Check to see if it's in the inode cache */
	لكل (i = 0; i < fs->icache->cache_size; i++) {
		إذا (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			أرجع 0;
		}
	}
	إذا (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} وإلا {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		إذا (group > fs->group_desc_گنتی)
			أرجع EXT2_ET_BAD_INODE_عدد;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		إذا (!block_nr)
			أرجع EXT2_ET_MISSING_INODE_TABLE;
		إذا ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_گنتی(fs->super)))
			أرجع EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (بنية ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (حرف *) iptr;
	طالما (length) {
		clen = length;
		إذا ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		إذا (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			إذا (retval)
				أرجع retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((حرف *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef الفاظ_BIGENDIAN
	ext2fs_swap_inode_full(fs, (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	إذا (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		أرجع EXT2_ET_INODE_CSUM_INVALID;

	أرجع 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 بنية ext2_inode * inode, صحيح bufsize)
{
	أرجع ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    بنية ext2_inode * inode)
{
	أرجع ext2fs_read_inode2(fs, ino, inode,
				  حجم(بنية ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      بنية ext2_inode * inode, صحيح bufsize,
			      صحيح flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	بنية ext2_inode_large *w_inode;
	حرف *ptr;
	unsigned i;
	صحيح clen;
	صحيح length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	إذا (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}

	إذا ((ino == 0) || (ino > fs->super->s_inodes_گنتی))
		أرجع EXT2_ET_BAD_INODE_عدد;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	إذا (retval)
		أرجع retval;

	إذا (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (بنية ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		إذا (retval)
			اذهب errout;
	}

	/* Check to see if the inode cache needs to be updated */
	إذا (fs->icache) {
		لكل (i=0; i < fs->icache->cache_size; i++) {
			إذا (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				اكسر;
			}
		}
	} وإلا {
		retval = ext2fs_create_inode_cache(fs, 4);
		إذا (retval)
			اذهب errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		اذهب errout;
	}

#ifdef الفاظ_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	إذا ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		إذا (retval)
			اذهب errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	إذا (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		اذهب errout;
	}
	إذا ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_گنتی(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		اذهب errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (حرف *) w_inode;

	طالما (length) {
		clen = length;
		إذا ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		إذا (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			إذا (retval)
				اذهب errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((حرف *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		إذا (retval)
			اذهب errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	أرجع retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  بنية ext2_inode * inode, صحيح bufsize)
{
	أرجع ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode)
{
	أرجع ext2fs_write_inode2(fs, ino, inode,
				   حجم(بنية ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 بنية ext2_inode *inode)
{
	بنية ext2_inode	*buf;
	صحيح			size = EXT2_INODE_SIZE(fs->super);
	بنية ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	إذا (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	إذا (size == حجم(بنية ext2_inode))
		أرجع ext2fs_write_inode_full(fs, ino, inode,
					       حجم(بنية ext2_inode));

	buf = malloc(size);
	إذا (!buf)
		أرجع ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (بنية ext2_inode_large *) buf;
	large_inode->i_extra_isize = حجم(بنية ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	إذا (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	حرر(buf);
	أرجع retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	بنية ext2_inode	inode;
	صحيح			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ino > fs->super->s_inodes_گنتی)
		أرجع EXT2_ET_BAD_INODE_عدد;

	إذا (fs->get_blocks) {
		إذا (!(*fs->get_blocks)(fs, ino, blocks))
			أرجع 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	لكل (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	أرجع 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	بنية	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ino > fs->super->s_inodes_گنتی)
		أرجع EXT2_ET_BAD_INODE_عدد;

	إذا (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	إذا (!LINUX_S_ISDIR(inode.i_mode))
		أرجع EXT2_ET_NO_DIRECTORY;
	أرجع 0;
}

