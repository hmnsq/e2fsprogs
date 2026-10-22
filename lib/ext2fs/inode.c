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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"
#അന്തര്ഭാവ "e2image.h"

#define IBLOCK_STATUS_CSUMS_OK	1
#define IBLOCK_STATUS_INSANE	2
#define SCAN_BLOCK_STATUS(scan)	((scan)->temp_buffer + (scan)->inode_size)

ഘടന ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_ഇടത്;
	blk_t			blocks_ഇടത്;
	dgrp_t			groups_ഇടത്;
	blk_t			inode_buffer_blocks;
	അക്ഷരം *			inode_buffer;
	പൂർണ്ണസംഖ്യ			inode_size;
	അക്ഷരം *			ptr;
	പൂർണ്ണസംഖ്യ			bytes_ഇടത്;
	അക്ഷരം			*temp_buffer;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      ശൂന്യം * priv_data);
	ശൂന്യം *			done_group_data;
	പൂർണ്ണസംഖ്യ			bad_block_ptr;
	പൂർണ്ണസംഖ്യ			scan_flags;
	പൂർണ്ണസംഖ്യ			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	unsigned	i;

	എങ്കിൽ (!fs->icache)
		തിരികെ 0;

	വേണ്ടി (i=0; i < fs->icache->cache_size; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->buffer_blk = 0;
	തിരികെ 0;
}

/*
 * Free the inode cache structure
 */
ശൂന്യം ext2fs_free_inode_cache(ഘടന ext2_inode_cache *icache)
{
	unsigned i;

	എങ്കിൽ (--icache->refcount)
		തിരികെ;
	എങ്കിൽ (icache->buffer)
		ext2fs_free_mem(&icache->buffer);
	വേണ്ടി (i = 0; i < icache->cache_size; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	എങ്കിൽ (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->buffer_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, unsigned പൂർണ്ണസംഖ്യ cache_size)
{
	unsigned	i;
	errcode_t	retval;

	എങ്കിൽ (fs->icache)
		തിരികെ 0;
	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_inode_cache), &fs->icache);
	എങ്കിൽ (retval)
		തിരികെ retval;

	memset(fs->icache, 0, വലുപ്പം(ഘടന ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->buffer);
	എങ്കിൽ (retval)
		പോകുക errout;

	fs->icache->buffer_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_size = cache_size;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_size,
				  വലുപ്പം(ഘടന ext2_inode_cache_ent),
				  &fs->icache->cache);
	എങ്കിൽ (retval)
		പോകുക errout;

	വേണ്ടി (i = 0; i < fs->icache->cache_size; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_SIZE(fs->super),
					&fs->icache->cache[i].inode);
		എങ്കിൽ (retval)
			പോകുക errout;
	}

	ext2fs_flush_icache(fs);
	തിരികെ 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	തിരികെ retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, പൂർണ്ണസംഖ്യ buffer_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (ext2fs_has_feature_journal_dev(fs->super))
		തിരികെ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	എങ്കിൽ (fs->blocksize < 1024)
		തിരികെ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	എങ്കിൽ (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		എങ്കിൽ (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2_struct_inode_scan), &scan);
	എങ്കിൽ (retval)
		തിരികെ retval;
	memset(scan, 0, വലുപ്പം(ഘടന ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_size = EXT2_INODE_SIZE(fs->super);
	scan->bytes_ഇടത് = 0;
	scan->current_group = 0;
	scan->groups_ഇടത് = fs->group_desc_എണ്ണം - 1;
	scan->inode_buffer_blocks = buffer_blocks ? buffer_blocks :
				    EXT2_INODE_SCAN_DEFAULT_BUFFER_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	എങ്കിൽ (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_എണ്ണം(fs->super)))) {
		ext2fs_free_mem(&scan);
		തിരികെ EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_ഇടത് = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_ഇടത് = scan->fs->inode_blocks_per_group;
	എങ്കിൽ (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		എങ്കിൽ (scan->inodes_ഇടത് > unused)
			scan->inodes_ഇടത് -= unused;
		അല്ലെങ്കിൽ
			scan->inodes_ഇടത് = 0;
		scan->blocks_ഇടത് =
			(scan->inodes_ഇടത് +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_buffer_blocks,
				      &scan->inode_buffer);
	scan->done_group = 0;
	scan->done_group_data = 0;
	scan->bad_block_ptr = 0;
	എങ്കിൽ (retval) {
		ext2fs_free_mem(&scan);
		തിരികെ retval;
	}
	retval = ext2fs_get_mem(scan->inode_size + scan->inode_buffer_blocks,
				&scan->temp_buffer);
	എങ്കിൽ (retval) {
		ext2fs_free_mem(&scan->inode_buffer);
		ext2fs_free_mem(&scan);
		തിരികെ retval;
	}
	memset(SCAN_BLOCK_STATUS(scan), 0, scan->inode_buffer_blocks);
	എങ്കിൽ (scan->fs->badblocks && scan->fs->badblocks->അക്കം)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	എങ്കിൽ (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	തിരികെ 0;
}

ശൂന്യം ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	എങ്കിൽ (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		തിരികെ;

	ext2fs_free_mem(&scan->inode_buffer);
	scan->inode_buffer = ശൂന്യ;
	ext2fs_free_mem(&scan->temp_buffer);
	scan->temp_buffer = ശൂന്യ;
	ext2fs_free_mem(&scan);
	തിരികെ;
}

ശൂന്യം ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       ശൂന്യം * priv_data),
			       ശൂന്യം *done_group_data)
{
	എങ്കിൽ (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		തിരികെ;

	scan->done_group = done_group;
	scan->done_group_data = done_group_data;
}

പൂർണ്ണസംഖ്യ ext2fs_inode_scan_flags(ext2_inode_scan scan, പൂർണ്ണസംഖ്യ set_flags,
			    പൂർണ്ണസംഖ്യ clear_flags)
{
	പൂർണ്ണസംഖ്യ	old_flags;

	എങ്കിൽ (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		തിരികെ 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	തിരികെ old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
സ്ഥിതം errcode_t get_next_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_ഇടത്--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_ഇടത് = 0;
	scan->inodes_ഇടത് = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_ഇടത് = fs->inode_blocks_per_group;
	എങ്കിൽ (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		എങ്കിൽ (scan->inodes_ഇടത് > unused)
			scan->inodes_ഇടത് -= unused;
		അല്ലെങ്കിൽ
			scan->inodes_ഇടത് = 0;
		scan->blocks_ഇടത് =
			(scan->inodes_ഇടത് +
			 (fs->blocksize / scan->inode_size - 1)) *
			scan->inode_size / fs->blocksize;
	}
	എങ്കിൽ (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_data_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_എണ്ണം(fs->super))))
		തിരികെ EXT2_ET_GDESC_BAD_INODE_TABLE;
	തിരികെ 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    പൂർണ്ണസംഖ്യ	group)
{
	scan->current_group = group - 1;
	scan->groups_ഇടത് = scan->fs->group_desc_എണ്ണം - group;
	scan->bad_block_ptr = 0;
	തിരികെ get_next_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
സ്ഥിതം errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *അക്കം_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	എങ്കിൽ (blk == 0)
		തിരികെ 0;

	/* Make sure bad_block_ptr is still valid */
	എങ്കിൽ (scan->bad_block_ptr >= bb->അക്കം) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		തിരികെ 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	ആകുമ്പോൾ (blk > bb->list[scan->bad_block_ptr]) {
		എങ്കിൽ (++scan->bad_block_ptr >= bb->അക്കം) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			തിരികെ 0;
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
	എങ്കിൽ (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*അക്കം_blocks = 1;
		എങ്കിൽ (++scan->bad_block_ptr >= bb->അക്കം)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		തിരികെ 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	എങ്കിൽ ((blk + *അക്കം_blocks) > bb->list[scan->bad_block_ptr])
		*അക്കം_blocks = (പൂർണ്ണസംഖ്യ) (bb->list[scan->bad_block_ptr] - blk);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ block_map_looks_insane(ext2_filsys fs,
				  ഘടന ext2_inode_large *inode)
{
	unsigned പൂർണ്ണസംഖ്യ i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	എങ്കിൽ ((inode->i_flags & EXT4_INLINE_DATA_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		തിരികെ 0;
	എങ്കിൽ (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		തിരികെ 0;
	എങ്കിൽ (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_SIZE(inode) <= വലുപ്പം(inode->i_block))
		തിരികെ 0;

	/* Unused inodes probably aren't insane */
	എങ്കിൽ (inode->i_links_എണ്ണം == 0)
		തിരികെ 0;

	/* See if more than half the block maps are insane */
	വേണ്ടി (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		എങ്കിൽ (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_data_block ||
		     inode->i_block[i] >= ext2fs_blocks_എണ്ണം(fs->super)))
			bad++;
	തിരികെ bad > EXT2_N_BLOCKS / 2;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ extent_head_looks_insane(ഘടന ext2_inode_large *inode)
{
	എങ്കിൽ (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					വലുപ്പം(inode->i_block)) == 0)
		തിരികെ 0;
	തിരികെ 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
സ്ഥിതം ശൂന്യം check_inode_block_sanity(ext2_inode_scan scan, blk64_t അക്കം_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	unsigned പൂർണ്ണസംഖ്യ	badness, checksum_failures;
	unsigned പൂർണ്ണസംഖ്യ	inodes_in_buf, inodes_per_block;
	അക്ഷരം		*p;
	ഘടന ext2_inode_large *inode;
	അക്ഷരം		*block_status;
	unsigned പൂർണ്ണസംഖ്യ	blk, bad_csum;

	എങ്കിൽ (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		തിരികെ;

	inodes_to_scan = scan->inodes_ഇടത്;
	inodes_in_buf = അക്കം_blocks * scan->fs->blocksize / scan->inode_size;
	എങ്കിൽ (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (അക്ഷരം *) scan->inode_buffer;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	memset(block_status, 0, scan->inode_buffer_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	എങ്കിൽ (inodes_per_block < 2)
		തിരികെ;

#ifdef വാക്കുകൾ_BIGENDIAN
	എങ്കിൽ (ext2fs_get_mem(EXT2_INODE_SIZE(scan->fs->super), &inode))
		തിരികെ;
#endif

	ആകുമ്പോൾ (inodes_to_scan > 0) {
		blk = (p - (അക്ഷരം *)scan->inode_buffer) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(ഘടന ext2_inode_large *) p) == 0;

#ifdef വാക്കുകൾ_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (ഘടന ext2_inode_large *) inode,
			       (ഘടന ext2_inode_large *) p,
			       0, EXT2_INODE_SIZE(scan->fs->super));
#അല്ലെങ്കിൽ
		inode = (ഘടന ext2_inode_large *) p;
#endif

		/* Is this inode insane? */
		എങ്കിൽ (bad_csum) {
			checksum_failures++;
			badness++;
		} അല്ലെങ്കിൽ എങ്കിൽ (extent_head_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		എങ്കിൽ (badness > inodes_per_block / 2) {
			unsigned പൂർണ്ണസംഖ്യ ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			എങ്കിൽ (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_size * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			തുടരുക;
		}

		എങ്കിൽ ((ino % inodes_per_block) == 0) {
			എങ്കിൽ (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_size;
		ino++;
	};

#ifdef വാക്കുകൾ_BIGENDIAN
	ext2fs_free_mem(&inode);
#endif
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
സ്ഥിതം errcode_t get_next_blocks(ext2_inode_scan scan)
{
	blk64_t		അക്കം_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	അക്കം_blocks = scan->inode_buffer_blocks;
	എങ്കിൽ (അക്കം_blocks > scan->blocks_ഇടത്)
		അക്കം_blocks = scan->blocks_ഇടത്;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	എങ്കിൽ (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		എങ്കിൽ (scan->bytes_ഇടത്)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	എങ്കിൽ (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &അക്കം_blocks);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}

	എങ്കിൽ ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		memset(scan->inode_buffer, 0,
		       (size_t) അക്കം_blocks * scan->fs->blocksize);
	} അല്ലെങ്കിൽ {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (പൂർണ്ണസംഖ്യ) അക്കം_blocks,
					     scan->inode_buffer);
		എങ്കിൽ (retval)
			തിരികെ EXT2_ET_NEXT_INODE_READ;
	}
	check_inode_block_sanity(scan, അക്കം_blocks);

	scan->ptr = scan->inode_buffer;
	scan->bytes_ഇടത് = അക്കം_blocks * scan->fs->blocksize;

	scan->blocks_ഇടത് -= അക്കം_blocks;
	എങ്കിൽ (scan->current_block)
		scan->current_block += അക്കം_blocks;

	തിരികെ 0;
}

#എങ്കിൽ 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ is_empty_scan(ext2_inode_scan scan)
{
	പൂർണ്ണസംഖ്യ	i;

	എങ്കിൽ (scan->bytes_ഇടത് == 0)
		തിരികെ 0;

	വേണ്ടി (i=0; i < scan->bytes_ഇടത്; i++)
		എങ്കിൽ (scan->ptr[i])
			തിരികെ 0;
	തിരികെ 1;
}
#endif

errcode_t ext2fs_get_next_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     ഘടന ext2_inode *inode, പൂർണ്ണസംഖ്യ bufsize)
{
	errcode_t	retval;
	പൂർണ്ണസംഖ്യ		extra_bytes = 0;
	പൂർണ്ണസംഖ്യ		length;
	ഘടന ext2_inode_large	*iptr = (ഘടന ext2_inode_large *)inode;
	അക്ഷരം		*iblock_status;
	unsigned പൂർണ്ണസംഖ്യ	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	length = EXT2_INODE_SIZE(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	എങ്കിൽ (scan->inodes_ഇടത് <= 0) {
	force_new_group:
		എങ്കിൽ (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_data);
			എങ്കിൽ (retval)
				തിരികെ retval;
		}
		എങ്കിൽ (scan->groups_ഇടത് <= 0) {
			*ino = 0;
			തിരികെ 0;
		}
		retval = get_next_blockgroup(scan);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	എങ്കിൽ ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		പോകുക force_new_group;
	എങ്കിൽ (scan->inodes_ഇടത് == 0)
		പോകുക force_new_group;
	എങ്കിൽ (scan->current_block == 0) {
		എങ്കിൽ (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			പോകുക force_new_group;
		} അല്ലെങ്കിൽ
			തിരികെ EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	എങ്കിൽ (scan->bytes_ഇടത് < scan->inode_size) {
		എങ്കിൽ (scan->bytes_ഇടത്)
			memcpy(scan->temp_buffer, scan->ptr, scan->bytes_ഇടത്);
		extra_bytes = scan->bytes_ഇടത്;

		retval = get_next_blocks(scan);
		എങ്കിൽ (retval)
			തിരികെ retval;
#എങ്കിൽ 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		എങ്കിൽ (is_empty_scan(scan))
			പോകുക force_new_group;
#endif
	}

	എങ്കിൽ (bufsize < length) {
		retval = ext2fs_get_mem(length, &iptr);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_buffer_blocks;
	എങ്കിൽ (extra_bytes) {
		memcpy(scan->temp_buffer+extra_bytes, scan->ptr,
		       scan->inode_size - extra_bytes);
		scan->ptr += scan->inode_size - extra_bytes;
		scan->bytes_ഇടത് -= scan->inode_size - extra_bytes;

		/* Verify the inode checksum. */
		എങ്കിൽ (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(ഘടന ext2_inode_large *)scan->temp_buffer))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef വാക്കുകൾ_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
			       (ഘടന ext2_inode_large *) iptr,
			       (ഘടന ext2_inode_large *) scan->temp_buffer,
			       0, length);
#അല്ലെങ്കിൽ
		memcpy(iptr, scan->temp_buffer, length);
#endif
		എങ്കിൽ (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} അല്ലെങ്കിൽ {
		/* Verify the inode checksum. */
		എങ്കിൽ (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(ഘടന ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ifdef വാക്കുകൾ_BIGENDIAN
		memset(iptr, 0, length);
		ext2fs_swap_inode_full(scan->fs,
				(ഘടന ext2_inode_large *) iptr,
				(ഘടന ext2_inode_large *) scan->ptr,
				0, length);
#അല്ലെങ്കിൽ
		memcpy(iptr, scan->ptr, length);
#endif
		scan->ptr += scan->inode_size;
		scan->bytes_ഇടത് -= scan->inode_size;
		എങ്കിൽ (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	എങ്കിൽ ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_ഇടത്--;
	scan->current_inode++;
	*ino = scan->current_inode;
	എങ്കിൽ (iptr != (ഘടന ext2_inode_large *)inode) {
		memcpy(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	തിരികെ retval;
}

errcode_t ext2fs_get_next_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				ഘടന ext2_inode *inode)
{
	തിരികെ ext2fs_get_next_inode_full(scan, ino, inode,
						വലുപ്പം(ഘടന ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     ഘടന ext2_inode * inode, പൂർണ്ണസംഖ്യ bufsize,
			     പൂർണ്ണസംഖ്യ flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	unsigned long 	block, offset;
	അക്ഷരം 		*ptr;
	errcode_t	retval;
	unsigned	i;
	പൂർണ്ണസംഖ്യ		clen, inodes_per_block;
	io_channel	io;
	പൂർണ്ണസംഖ്യ		length = EXT2_INODE_SIZE(fs->super);
	ഘടന ext2_inode_large	*iptr;
	പൂർണ്ണസംഖ്യ		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (ext2fs_has_feature_journal_dev(fs->super))
		തിരികെ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	എങ്കിൽ (fs->blocksize < 1024)
		തിരികെ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	എങ്കിൽ (fs->read_inode &&
	    ((bufsize == വലുപ്പം(ഘടന ext2_inode)) ||
	     (EXT2_INODE_SIZE(fs->super) == വലുപ്പം(ഘടന ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		എങ്കിൽ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			തിരികെ retval;
	}
	എങ്കിൽ ((ino == 0) || (ino > fs->super->s_inodes_എണ്ണം))
		തിരികെ EXT2_ET_BAD_INODE_അക്കം;
	/* Create inode cache if not present */
	എങ്കിൽ (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	/* Check to see if it's in the inode cache */
	വേണ്ടി (i = 0; i < fs->icache->cache_size; i++) {
		എങ്കിൽ (fs->icache->cache[i].ino == ino) {
			memcpy(inode, fs->icache->cache[i].inode,
			       (bufsize > length) ? length : bufsize);
			തിരികെ 0;
		}
	}
	എങ്കിൽ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_SIZE(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_SIZE(fs->super);
		io = fs->image_io;
	} അല്ലെങ്കിൽ {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		എങ്കിൽ (group > fs->group_desc_എണ്ണം)
			തിരികെ EXT2_ET_BAD_INODE_അക്കം;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_SIZE(fs->super);
		block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		എങ്കിൽ (!block_nr)
			തിരികെ EXT2_ET_MISSING_INODE_TABLE;
		എങ്കിൽ ((block_nr < fs->super->s_first_data_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_എണ്ണം(fs->super)))
			തിരികെ EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_size;
	iptr = (ഘടന ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (അക്ഷരം *) iptr;
	ആകുമ്പോൾ (length) {
		clen = length;
		എങ്കിൽ ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		എങ്കിൽ (block_nr != fs->icache->buffer_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->buffer);
			എങ്കിൽ (retval)
				തിരികെ retval;
			fs->icache->buffer_blk = block_nr;
		}

		memcpy(ptr, ((അക്ഷരം *) fs->icache->buffer) + (unsigned) offset,
		       clen);

		offset = 0;
		length -= clen;
		ptr += clen;
		block_nr++;
	}
	length = EXT2_INODE_SIZE(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ifdef വാക്കുകൾ_BIGENDIAN
	ext2fs_swap_inode_full(fs, (ഘടന ext2_inode_large *) iptr,
			       (ഘടന ext2_inode_large *) iptr,
			       0, length);
#endif

	/* Update the inode cache bookkeeping */
	എങ്കിൽ (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	memcpy(inode, iptr, (bufsize > length) ? length : bufsize);

	എങ്കിൽ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		തിരികെ EXT2_ET_INODE_CSUM_INVALID;

	തിരികെ 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 ഘടന ext2_inode * inode, പൂർണ്ണസംഖ്യ bufsize)
{
	തിരികെ ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ഘടന ext2_inode * inode)
{
	തിരികെ ext2fs_read_inode2(fs, ino, inode,
				  വലുപ്പം(ഘടന ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      ഘടന ext2_inode * inode, പൂർണ്ണസംഖ്യ bufsize,
			      പൂർണ്ണസംഖ്യ flags)
{
	blk64_t block_nr;
	dgrp_t group;
	unsigned long block, offset;
	errcode_t retval = 0;
	ഘടന ext2_inode_large *w_inode;
	അക്ഷരം *ptr;
	unsigned i;
	പൂർണ്ണസംഖ്യ clen;
	പൂർണ്ണസംഖ്യ length = EXT2_INODE_SIZE(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (ext2fs_has_feature_journal_dev(fs->super))
		തിരികെ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	എങ്കിൽ (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		എങ്കിൽ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			തിരികെ retval;
	}

	എങ്കിൽ ((ino == 0) || (ino > fs->super->s_inodes_എണ്ണം))
		തിരികെ EXT2_ET_BAD_INODE_അക്കം;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(length, &w_inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (bufsize < length) {
		retval = ext2fs_read_inode2(fs, ino,
					    (ഘടന ext2_inode *)w_inode,
					    length, READ_INODE_NOCSUM);
		എങ്കിൽ (retval)
			പോകുക errout;
	}

	/* Check to see if the inode cache needs to be updated */
	എങ്കിൽ (fs->icache) {
		വേണ്ടി (i=0; i < fs->icache->cache_size; i++) {
			എങ്കിൽ (fs->icache->cache[i].ino == ino) {
				memcpy(fs->icache->cache[i].inode, inode,
				       (bufsize > length) ? length : bufsize);
				നിർത്തുക;
			}
		}
	} അല്ലെങ്കിൽ {
		retval = ext2fs_create_inode_cache(fs, 4);
		എങ്കിൽ (retval)
			പോകുക errout;
	}
	memcpy(w_inode, inode, (bufsize > length) ? length : bufsize);

	എങ്കിൽ (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		പോകുക errout;
	}

#ifdef വാക്കുകൾ_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, length);
#endif

	എങ്കിൽ ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		എങ്കിൽ (retval)
			പോകുക errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_SIZE(fs->super);
	block = offset >> EXT2_BLOCK_SIZE_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (unsigned) group);
	എങ്കിൽ (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		പോകുക errout;
	}
	എങ്കിൽ ((block_nr < fs->super->s_first_data_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_എണ്ണം(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		പോകുക errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_SIZE(fs->super) - 1);

	ptr = (അക്ഷരം *) w_inode;

	ആകുമ്പോൾ (length) {
		clen = length;
		എങ്കിൽ ((offset + length) > fs->blocksize)
			clen = fs->blocksize - offset;

		എങ്കിൽ (fs->icache->buffer_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->buffer);
			എങ്കിൽ (retval)
				പോകുക errout;
			fs->icache->buffer_blk = block_nr;
		}


		memcpy((അക്ഷരം *) fs->icache->buffer + (unsigned) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->buffer);
		എങ്കിൽ (retval)
			പോകുക errout;

		offset = 0;
		ptr += clen;
		length -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	തിരികെ retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  ഘടന ext2_inode * inode, പൂർണ്ണസംഖ്യ bufsize)
{
	തിരികെ ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     ഘടന ext2_inode *inode)
{
	തിരികെ ext2fs_write_inode2(fs, ino, inode,
				   വലുപ്പം(ഘടന ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 ഘടന ext2_inode *inode)
{
	ഘടന ext2_inode	*buf;
	പൂർണ്ണസംഖ്യ			size = EXT2_INODE_SIZE(fs->super);
	ഘടന ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	എങ്കിൽ (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	എങ്കിൽ (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	എങ്കിൽ (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	എങ്കിൽ (size == വലുപ്പം(ഘടന ext2_inode))
		തിരികെ ext2fs_write_inode_full(fs, ino, inode,
					       വലുപ്പം(ഘടന ext2_inode));

	buf = malloc(size);
	എങ്കിൽ (!buf)
		തിരികെ ENOMEM;

	memset(buf, 0, size);
	*buf = *inode;

	large_inode = (ഘടന ext2_inode_large *) buf;
	large_inode->i_extra_isize = വലുപ്പം(ഘടന ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_SIZE;
	എങ്കിൽ (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, size);
	സ്വതന്ത്രം(buf);
	തിരികെ retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	ഘടന ext2_inode	inode;
	പൂർണ്ണസംഖ്യ			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (ino > fs->super->s_inodes_എണ്ണം)
		തിരികെ EXT2_ET_BAD_INODE_അക്കം;

	എങ്കിൽ (fs->get_blocks) {
		എങ്കിൽ (!(*fs->get_blocks)(fs, ino, blocks))
			തിരികെ 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;
	വേണ്ടി (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	തിരികെ 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	ഘടന	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (ino > fs->super->s_inodes_എണ്ണം)
		തിരികെ EXT2_ET_BAD_INODE_അക്കം;

	എങ്കിൽ (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		എങ്കിൽ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			തിരികെ retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ (!LINUX_S_ISDIR(inode.i_mode))
		തിരികെ EXT2_ET_NO_DIRECTORY;
	തിരികെ 0;
}

