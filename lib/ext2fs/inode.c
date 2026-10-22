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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"
#અન્તર્ભાવ "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

સંરચના ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_ડાબું;
	blk_t			blocks_ડાબું;
	dgrp_t			groups_ડાબું;
	blk_t			inode_buffer_blocks;
	અક્ષર *			inode_buffer;
	પૂર્ણાંક			inode_size;
	અક્ષર *			ptr;
	પૂર્ણાંક			bytes_ડાબું;
	અક્ષર			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      શૂન્ય * priv_data);
	શૂન્ય *			done_group_data;
	પૂર્ણાંક			bad_block_ptr;
	પૂર્ણાંક			scan_flags;
	પૂર્ણાંક			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	જો (!fs->icache)
		પાછા 0;

	માટે (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	પાછા 0;
}

/*
 * Free the inode cache structure
 */
શૂન્ય ext2fs_free_inode_cache(સંરચના ext2_inode_cache *icache)
{
	unsigned i;

	જો (--icache->refcount)
		પાછા;
	જો (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	માટે (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	જો (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned પૂર્ણાંક cache_size)
{
	unsigned	i;
	errcode_t	retval;

	જો (fs->icache)
		પાછા 0;
	retval = ext2fs_get_mem(કદ(સંરચના ext2_inode_cache), &fs->icache);
	જો (retval)
		પાછા retval;

	memset(fs->icache, 0, કદ(સંરચના ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	જો (retval)
		જાઓ errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  કદ(સંરચના ext2_inode_cache_ent),
				  &fs->icache->cache);
	જો (retval)
		જાઓ errout;

	માટે (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		જો (retval)
			જાઓ errout;
	}

	ext2fs_flush_icache(fs);
	પાછા 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	પાછા retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, પૂર્ણાંક buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (ext2fs_has_feature_journal_dev(fs->super))
		પાછા EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	જો (fs->blocksize < 1024)
		પાછા EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	જો (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		જો (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(કદ(સંરચના ext2_struct_inode_scan), &scan);
	જો (retval)
		પાછા retval;
	memset(scan, 0, કદ(સંરચના ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_ડાબું = 0;
	scan->current_group = 0;
	scan->groups_ડાબું = fs->group_desc_ગણતરી - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	જો (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_ગણતરી(fs->super)))) {
		ext2fs_free_mem(&scan);
		પાછા EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_ડાબું = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_ડાબું = scan->fs->inode_blocks_per_group;
	જો (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		જો (scan->inodes_ડાબું > unused)
			scan->inodes_ડાબું -= unused;
		નહીંતર
			scan->inodes_ડાબું = 0;
		scan->blocks_ડાબું =
			(scan->inodes_ડાબું +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	જો (retval) {
		ext2fs_free_mem(&scan);
		પાછા retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	જો (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		પાછા retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	જો (scan->fs->badblocks && scan->fs->badblocks->અંક)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	જો (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	પાછા 0;
}

શૂન્ય ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	જો (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		પાછા;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = ખાલી;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = ખાલી;
	ext2fs_free_mem(&scan);
	પાછા;
}

શૂન્ય ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       શૂન્ય * priv_data),
			       શૂન્ય *done_group_data)
{
	જો (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		પાછા;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

પૂર્ણાંક ext2fs_inode_scan_flags(ext2_inode_scan scan, પૂર્ણાંક set_flags,
			    પૂર્ણાંક clear_flags)
{
	પૂર્ણાંક	old_flags;

	જો (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		પાછા 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	પાછા old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
સ્થિર errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_ડાબું--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_ડાબું = 0;
	scan->inodes_ડાબું = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_ડાબું = fs->inode_blocks_per_group;
	જો (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		જો (scan->inodes_ડાબું > unused)
			scan->inodes_ડાબું -= unused;
		નહીંતર
			scan->inodes_ડાબું = 0;
		scan->blocks_ડાબું =
			(scan->inodes_ડાબું +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	જો (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_ગણતરી(fs->super))))
		પાછા EXT2_ET_GDESC_BAD_INODE_TABLE;
	પાછા 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    પૂર્ણાંક	group)
{
	scan->current_group = group - 1;
	scan->groups_ડાબું = scan->fs->group_desc_ગણતરી - group;
	scan->bad_block_ptr = 0;
	પાછા get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
સ્થિર errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *અંક_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	જો (blk == 0)
		પાછા 0;

	/* Make sure bad_block_ptr is still valid */
	જો (scan->bad_block_ptr >= bb->અંક) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		પાછા 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	જ્યારે (blk > bb->list[scan->bad_block_ptr]) {
		જો (++scan->bad_block_ptr >= bb->અંક) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			પાછા 0;
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
	જો (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*અંક_blocks = 1;
		જો (++scan->bad_block_ptr >= bb->અંક)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		પાછા 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	જો ((blk + *અંક_blocks) > bb->list[scan->bad_block_ptr])
		*અંક_blocks = (પૂર્ણાંક) (bb->list[scan->bad_block_ptr] - blk);

	પાછા 0;
}

સ્થિર પૂર્ણાંક block_map_looks_insane(ext2_filsys fs,
				  સંરચના ext2_inode_large *inode)
{
	unsigned પૂર્ણાંક i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	જો ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		પાછા 0;
	જો (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		પાછા 0;
	જો (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= કદ(inode->i_block))
		પાછા 0;

	/* Unused inodes probably aren't insane */
	જો (inode->i_links_ગણતરી == 0)
		પાછા 0;

	/* See if more than half the block maps are insane */
	માટે (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		જો (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_ગણતરી(fs->super)))
			bad++;
	પાછા bad > EXT2_N_BLOCKS / 2;
}

સ્થિર પૂર્ણાંક extent_head_looks_insane(સંરચના ext2_inode_large *inode)
{
	જો (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					કદ(inode->i_block)) == 0)
		પાછા 0;
	પાછા 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
સ્થિર શૂન્ય check_inode_block_sanity(ext2_inode_scan scan, blk64_t અંક_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned પૂર્ણાંક	badness, checksum_failures;
	unsigned પૂર્ણાંક	inodes_in_buf, inodes_per_block;
	અક્ષર		*p;
	સંરચના ext2_inode_large *inode;
	અક્ષર		*block_status;
	unsigned પૂર્ણાંક	blk, bad_csum;

	જો (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		પાછા;

	inodes_to_scan = scan->inodes_ડાબું;
	inodes_in_buf = અંક_blocks * scan->fs->blocksize / scan->inode_size;
	જો (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (અક્ષર *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	જો (inodes_per_block < 2)
		પાછા;

#ifdef શબ્દો_BIGENDIAN
	જો (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		પાછા;
#endif

	જ્યારે (inodes_to_scan > 0) {
		blk = (p - (અક્ષર *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(સંરચના ext2_inode_large *) p) == 0;

#ifdef શબ્દો_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (સંરચના ext2_inode_large *) inode,
			       (સંરચના ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#નહીંતર
		inode = (સંરચના ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		જો (bad_csum) {
			checksum_failures++;
			badness++;
		} નહીંતર જો (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		જો (badness > inodes_per_block / 2) {
			unsigned પૂર્ણાંક ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			જો (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			ચાલુ;
		}

		જો ((ino % inodes_per_block) == 0) {
			જો (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef શબ્દો_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
સ્થિર errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		અંક_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	અંક_blocks = scan->inode_buffer_blocks;
	જો (અંક_blocks > scan->blocks_ડાબું)
		અંક_blocks = scan->blocks_ડાબું;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	જો (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		જો (scan->bytes_ડાબું)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	જો (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &અંક_blocks);
		જો (retval)
			પાછા retval;
	}

	જો ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) અંક_blocks * scan->fs->blocksize);
	} નહીંતર {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (પૂર્ણાંક) અંક_blocks,
					     scan->inode_buffer);
		જો (retval)
			પાછા EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, અંક_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_ડાબું = અંક_blocks * scan->fs->blocksize;

	scan->blocks_ડાબું -= અંક_blocks;
	જો (scan->current_block)
		scan->current_block += અંક_blocks;

	પાછા 0;
}

#જો 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
સ્થિર અંતઃરેખ પૂર્ણાંક is_empty_scan(ext2_inode_scan scan)
{
	પૂર્ણાંક	i;

	જો (scan->bytes_ડાબું == 0)
		પાછા 0;

	માટે (i=0; i < scan->bytes_ડાબું; i++)
		જો (scan->ptr[i])
			પાછા 0;
	પાછા 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     સંરચના ext2_inode *inode, પૂર્ણાંક bufsize)
{
	errcode_t	retval;
	પૂર્ણાંક		extra_bytes = 0;
	પૂર્ણાંક		length;
	સંરચના ext2_inode_large	*iptr = (સંરચના ext2_inode_large *)inode;
	અક્ષર		*iblock_status;
	unsigned પૂર્ણાંક	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	જો (scan->inodes_ડાબું <= 0) {
	force_new_group:
		જો (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			જો (retval)
				પાછા retval;
		}
		જો (scan->groups_ડાબું <= 0) {
			*ino = 0;
			પાછા 0;
		}
		retval = get_next_blockgroup(scan);
		જો (retval)
			પાછા retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	જો ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		જાઓ force_new_group;
	જો (scan->inodes_ડાબું == 0)
		જાઓ force_new_group;
	જો (scan->current_block == 0) {
		જો (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			જાઓ force_new_group;
		} નહીંતર
			પાછા EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	જો (scan->bytes_ડાબું < scan->inode_size) {
		જો (scan->bytes_ડાબું)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_ડાબું);
		extra_bytes = scan->bytes_ડાબું;

		retval = get_next_blocks(scan);
		જો (retval)
			પાછા retval;
#જો 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		જો (is_empty_scan(scan))
			જાઓ force_new_group;
#endif
	}

	જો (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		જો (retval)
			પાછા retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	જો (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_ડાબું -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		જો (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(સંરચના ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef શબ્દો_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (સંરચના ext2_inode_large *) iptr,
			       (સંરચના ext2_inode_large *) scan->temp_buffer,
			       0, length);
#નહીંતર
		memcpy(iptr, scan->temp_buffer, length);
#endif
		જો (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} નહીંતર {
		/* Verify the inode checksum. */
		જો (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(સંરચના ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef શબ્દો_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(સંરચના ext2_inode_large *) iptr,
				(સંરચના ext2_inode_large *) scan->ptr,
				0, length);
#નહીંતર
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_ડાબું -= scan->inode_size;
		જો (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	જો ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_ડાબું--;
	scan->current_inode++;
	*ino = scan->current_inode;
	જો (iptr != (સંરચના ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	પાછા retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				સંરચના ext2_inode *inode)
{
	પાછા ext2fs_get_next_inode_full(scan, ino, inode,
						કદ(સંરચના ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     સંરચના ext2_inode * inode, પૂર્ણાંક bufsize,
			     પૂર્ણાંક flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	અક્ષર 		*ptr;
	errcode_t	retval;
	unsigned	i;
	પૂર્ણાંક		clen, inodes_per_block;
	io_channel	io;
	પૂર્ણાંક		length = EXT2_INODE_SIZE(fs->super);
	સંરચના ext2_inode_large	*iptr;
	પૂર્ણાંક		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (ext2fs_has_feature_journal_dev(fs->super))
		પાછા EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	જો (fs->blocksize < 1024)
		પાછા EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	જો (fs->read_inode &&
	    ((bufsize == કદ(સંરચના ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == કદ(સંરચના ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		જો (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			પાછા retval;
	}
	જો ((ino == 0) || (ino > fs->super->s_inodes_ગણતરી))
		પાછા EXT2_ET_BAD_INODE_અંક;
	/* Create inode cache if not present */
	જો (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		જો (retval)
			પાછા retval;
	}
	/* Check to see if it's in the inode cache */
	માટે (i = 0; i < fs->icache->cache_size; i++) {
		જો (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			પાછા 0;
		}
	}
	જો (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} નહીંતર {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		જો (group > fs->group_desc_ગણતરી)
			પાછા EXT2_ET_BAD_INODE_અંક;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		જો (!block_nr)
			પાછા EXT2_ET_MISSING_INODE_TABLE;
		જો ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_ગણતરી(fs->super)))
			પાછા EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (સંરચના ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (અક્ષર *) iptr;
	જ્યારે (length) {
		clen = length;
		જો ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		જો (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			જો (retval)
				પાછા retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((અક્ષર *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef શબ્દો_BIGENDIAN
	ext2fs_swap_inode_full(fs, (સંરચના ext2_inode_large *) iptr,
			       (સંરચના ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	જો (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	જો (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		પાછા EXT2_ET_INODE_CSUM_INVALID;

	પાછા 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 સંરચના ext2_inode * inode, પૂર્ણાંક bufsize)
{
	પાછા ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    સંરચના ext2_inode * inode)
{
	પાછા ext2fs_read_inode2(fs, ino, inode,
				  કદ(સંરચના ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      સંરચના ext2_inode * inode, પૂર્ણાંક bufsize,
			      પૂર્ણાંક flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	સંરચના ext2_inode_large *w_inode;
	અક્ષર *ptr;
	unsigned i;
	પૂર્ણાંક clen;
	પૂર્ણાંક length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (ext2fs_has_feature_journal_dev(fs->super))
		પાછા EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	જો (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		જો (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			પાછા retval;
	}

	જો ((ino == 0) || (ino > fs->super->s_inodes_ગણતરી))
		પાછા EXT2_ET_BAD_INODE_અંક;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	જો (retval)
		પાછા retval;

	જો (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (સંરચના ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		જો (retval)
			જાઓ errout;
	}

	/* Check to see if the inode cache needs to be updated */
	જો (fs->icache) {
		માટે (i=0; i < fs->icache->cache_size; i++) {
			જો (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				તોડો;
			}
		}
	} નહીંતર {
		retval = ext2fs_create_inode_cache(fs, 4);
		જો (retval)
			જાઓ errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	જો (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		જાઓ errout;
	}

#ifdef શબ્દો_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	જો ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		જો (retval)
			જાઓ errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	જો (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		જાઓ errout;
	}
	જો ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_ગણતરી(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		જાઓ errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (અક્ષર *) w_inode;

	જ્યારે (length) {
		clen = length;
		જો ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		જો (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			જો (retval)
				જાઓ errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((અક્ષર *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		જો (retval)
			જાઓ errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	પાછા retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  સંરચના ext2_inode * inode, પૂર્ણાંક bufsize)
{
	પાછા ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     સંરચના ext2_inode *inode)
{
	પાછા ext2fs_write_inode2(fs, ino, inode,
				   કદ(સંરચના ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 સંરચના ext2_inode *inode)
{
	સંરચના ext2_inode	*buf;
	પૂર્ણાંક			size = EXT2_INODE_SIZE(fs->super);
	સંરચના ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	જો (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	જો (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	જો (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	જો (size == કદ(સંરચના ext2_inode))
		પાછા ext2fs_write_inode_full(fs, ino, inode,
					       કદ(સંરચના ext2_inode));

	buf = malloc(size);
	જો (!buf)
		પાછા ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (સંરચના ext2_inode_large *) buf;
	large_inode->i_extra_isize = કદ(સંરચના ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	જો (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	મુક્ત(buf);
	પાછા retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	સંરચના ext2_inode	inode;
	પૂર્ણાંક			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (ino > fs->super->s_inodes_ગણતરી)
		પાછા EXT2_ET_BAD_INODE_અંક;

	જો (fs->get_blocks) {
		જો (!(*fs->get_blocks)(fs, ino, blocks))
			પાછા 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	જો (retval)
		પાછા retval;
	માટે (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	પાછા 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	સંરચના	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (ino > fs->super->s_inodes_ગણતરી)
		પાછા EXT2_ET_BAD_INODE_અંક;

	જો (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		જો (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			પાછા retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	જો (retval)
		પાછા retval;
	જો (!LINUX_S_ISDIR(inode.i_mode))
		પાછા EXT2_ET_NO_DIRECTORY;
	પાછા 0;
}

