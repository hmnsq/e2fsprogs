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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"
#समावेश "e2image.h"

#घोषणा IBLOCK_STATUS_CSUMS_OK	1
#घोषणा IBLOCK_STATUS_INSANE	2
#घोषणा SCAN_BLOCK_STATUS(scan)	((scan)->अस्थायी_बफर + (scan)->inode_आकार)

काष्ठा ext2_struct_inode_scan {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_block;
	dgrp_t			current_group;
	ext2_ino_t		inodes_बायाँ;
	blk_t			blocks_बायाँ;
	dgrp_t			groups_बायाँ;
	blk_t			inode_बफर_blocks;
	अक्षर *			inode_बफर;
	पूर्णांक			inode_आकार;
	अक्षर *			ptr;
	पूर्णांक			bytes_बायाँ;
	अक्षर			*अस्थायी_बफर;
	errcode_t		(*done_group)(ext2_filsys fs,
					      ext2_inode_scan scan,
					      dgrp_t group,
					      व्योम * priv_तथ्य);
	व्योम *			done_group_तथ्य;
	पूर्णांक			bad_block_ptr;
	पूर्णांक			scan_flags;
	पूर्णांक			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	अचिन्हित	i;

	अगर (!fs->icache)
		वापस 0;

	क्रम (i=0; i < fs->icache->cache_आकार; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->बफर_blk = 0;
	वापस 0;
}

/*
 * Free the inode cache structure
 */
व्योम ext2fs_free_inode_cache(काष्ठा ext2_inode_cache *icache)
{
	अचिन्हित i;

	अगर (--icache->refcount)
		वापस;
	अगर (icache->बफर)
		ext2fs_free_mem(&icache->बफर);
	क्रम (i = 0; i < icache->cache_आकार; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	अगर (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->बफर_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, अचिन्हित पूर्णांक cache_आकार)
{
	अचिन्हित	i;
	errcode_t	retval;

	अगर (fs->icache)
		वापस 0;
	retval = ext2fs_get_mem(माप(काष्ठा ext2_inode_cache), &fs->icache);
	अगर (retval)
		वापस retval;

	स_रखो(fs->icache, 0, माप(काष्ठा ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->बफर);
	अगर (retval)
		जाओ errout;

	fs->icache->बफर_blk = 0;
	fs->icache->cache_last = -1;
	fs->icache->cache_आकार = cache_आकार;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_आकार,
				  माप(काष्ठा ext2_inode_cache_ent),
				  &fs->icache->cache);
	अगर (retval)
		जाओ errout;

	क्रम (i = 0; i < fs->icache->cache_आकार; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_आकार(fs->super),
					&fs->icache->cache[i].inode);
		अगर (retval)
			जाओ errout;
	}

	ext2fs_flush_icache(fs);
	वापस 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	वापस retval;
}

errcode_t ext2fs_open_inode_scan(ext2_filsys fs, पूर्णांक बफर_blocks,
				 ext2_inode_scan *ret_scan)
{
	ext2_inode_scan	scan;
	errcode_t	retval;
	errcode_t (*save_get_blocks)(ext2_filsys f, ext2_ino_t ino, blk_t *blocks);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (ext2fs_has_feature_journal_dev(fs->super))
		वापस EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	अगर (fs->blocksize < 1024)
		वापस EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	अगर (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_blocks = fs->get_blocks;
		fs->get_blocks = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		अगर (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_blocks = save_get_blocks;
	}

	retval = ext2fs_get_mem(माप(काष्ठा ext2_struct_inode_scan), &scan);
	अगर (retval)
		वापस retval;
	स_रखो(scan, 0, माप(काष्ठा ext2_struct_inode_scan));

	scan->magic = EXT2_ET_MAGIC_INODE_SCAN;
	scan->fs = fs;
	scan->inode_आकार = EXT2_INODE_आकार(fs->super);
	scan->bytes_बायाँ = 0;
	scan->current_group = 0;
	scan->groups_बायाँ = fs->group_desc_गिनती - 1;
	scan->inode_बफर_blocks = बफर_blocks ? बफर_blocks :
				    EXT2_INODE_SCAN_DEFAULT_बफर_BLOCKS;
	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	अगर (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_तथ्य_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_गिनती(fs->super)))) {
		ext2fs_free_mem(&scan);
		वापस EXT2_ET_GDESC_BAD_INODE_TABLE;
	}

	scan->inodes_बायाँ = EXT2_INODES_PER_GROUP(scan->fs->super);
	scan->blocks_बायाँ = scan->fs->inode_blocks_per_group;
	अगर (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		अगर (scan->inodes_बायाँ > unused)
			scan->inodes_बायाँ -= unused;
		अन्यथा
			scan->inodes_बायाँ = 0;
		scan->blocks_बायाँ =
			(scan->inodes_बायाँ +
			 (fs->blocksize / scan->inode_आकार - 1)) *
			scan->inode_आकार / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, scan->inode_बफर_blocks,
				      &scan->inode_बफर);
	scan->done_group = 0;
	scan->done_group_तथ्य = 0;
	scan->bad_block_ptr = 0;
	अगर (retval) {
		ext2fs_free_mem(&scan);
		वापस retval;
	}
	retval = ext2fs_get_mem(scan->inode_आकार + scan->inode_बफर_blocks,
				&scan->अस्थायी_बफर);
	अगर (retval) {
		ext2fs_free_mem(&scan->inode_बफर);
		ext2fs_free_mem(&scan);
		वापस retval;
	}
	स_रखो(SCAN_BLOCK_STATUS(scan), 0, scan->inode_बफर_blocks);
	अगर (scan->fs->badblocks && scan->fs->badblocks->अंक)
		scan->scan_flags |= EXT2_SF_CHK_BADBLOCKS;
	अगर (ext2fs_has_group_desc_csum(fs))
		scan->scan_flags |= EXT2_SF_DO_LAZY;
	*ret_scan = scan;
	वापस 0;
}

व्योम ext2fs_close_inode_scan(ext2_inode_scan scan)
{
	अगर (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		वापस;

	ext2fs_free_mem(&scan->inode_बफर);
	scan->inode_बफर = शून्य;
	ext2fs_free_mem(&scan->अस्थायी_बफर);
	scan->अस्थायी_बफर = शून्य;
	ext2fs_free_mem(&scan);
	वापस;
}

व्योम ext2fs_set_inode_callback(ext2_inode_scan scan,
			       errcode_t (*done_group)(ext2_filsys fs,
						       ext2_inode_scan scan,
						       dgrp_t group,
						       व्योम * priv_तथ्य),
			       व्योम *done_group_तथ्य)
{
	अगर (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		वापस;

	scan->done_group = done_group;
	scan->done_group_तथ्य = done_group_तथ्य;
}

पूर्णांक ext2fs_inode_scan_flags(ext2_inode_scan scan, पूर्णांक set_flags,
			    पूर्णांक clear_flags)
{
	पूर्णांक	old_flags;

	अगर (!scan || (scan->magic != EXT2_ET_MAGIC_INODE_SCAN))
		वापस 0;

	old_flags = scan->scan_flags;
	scan->scan_flags &= ~clear_flags;
	scan->scan_flags |= set_flags;
	वापस old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
अटल errcode_t get_अग्र_blockgroup(ext2_inode_scan scan)
{
	ext2_filsys fs = scan->fs;

	scan->current_group++;
	scan->groups_बायाँ--;

	scan->current_block = ext2fs_inode_table_loc(scan->fs,
						     scan->current_group);
	scan->current_inode = scan->current_group *
		EXT2_INODES_PER_GROUP(fs->super);

	scan->bytes_बायाँ = 0;
	scan->inodes_बायाँ = EXT2_INODES_PER_GROUP(fs->super);
	scan->blocks_बायाँ = fs->inode_blocks_per_group;
	अगर (ext2fs_has_group_desc_csum(fs)) {
		__u32 unused = ext2fs_bg_itable_unused(fs, scan->current_group);
		अगर (scan->inodes_बायाँ > unused)
			scan->inodes_बायाँ -= unused;
		अन्यथा
			scan->inodes_बायाँ = 0;
		scan->blocks_बायाँ =
			(scan->inodes_बायाँ +
			 (fs->blocksize / scan->inode_आकार - 1)) *
			scan->inode_आकार / fs->blocksize;
	}
	अगर (scan->current_block &&
	    ((scan->current_block < fs->super->s_first_तथ्य_block) ||
	     (scan->current_block + fs->inode_blocks_per_group - 1 >=
	      ext2fs_blocks_गिनती(fs->super))))
		वापस EXT2_ET_GDESC_BAD_INODE_TABLE;
	वापस 0;
}

errcode_t ext2fs_inode_scan_goto_blockgroup(ext2_inode_scan scan,
					    पूर्णांक	group)
{
	scan->current_group = group - 1;
	scan->groups_बायाँ = scan->fs->group_desc_गिनती - group;
	scan->bad_block_ptr = 0;
	वापस get_अग्र_blockgroup(scan);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
अटल errcode_t check_for_inode_bad_blocks(ext2_inode_scan scan,
					    blk64_t *अंक_blocks)
{
	blk64_t	blk = scan->current_block;
	badblocks_list	bb = scan->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	अगर (blk == 0)
		वापस 0;

	/* Make sure bad_block_ptr is still valid */
	अगर (scan->bad_block_ptr >= bb->अंक) {
		scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		वापस 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	जबतक (blk > bb->list[scan->bad_block_ptr]) {
		अगर (++scan->bad_block_ptr >= bb->अंक) {
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			वापस 0;
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
	अगर (blk == bb->list[scan->bad_block_ptr]) {
		scan->scan_flags |= EXT2_SF_BAD_INODE_BLK;
		*अंक_blocks = 1;
		अगर (++scan->bad_block_ptr >= bb->अंक)
			scan->scan_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		वापस 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	अगर ((blk + *अंक_blocks) > bb->list[scan->bad_block_ptr])
		*अंक_blocks = (पूर्णांक) (bb->list[scan->bad_block_ptr] - blk);

	वापस 0;
}

अटल पूर्णांक block_map_looks_insane(ext2_filsys fs,
				  काष्ठा ext2_inode_large *inode)
{
	अचिन्हित पूर्णांक i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	अगर ((inode->i_flags & EXT4_INLINE_तथ्य_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		वापस 0;
	अगर (!LINUX_S_ISREG(inode->i_mode) &&
	    !LINUX_S_ISLNK(inode->i_mode) &&
	    !LINUX_S_ISDIR(inode->i_mode))
		वापस 0;
	अगर (LINUX_S_ISLNK(inode->i_mode) &&
	    EXT2_I_आकार(inode) <= माप(inode->i_block))
		वापस 0;

	/* Unused inodes probably aren't insane */
	अगर (inode->i_links_गिनती == 0)
		वापस 0;

	/* See if more than half the block maps are insane */
	क्रम (i = 0, bad = 0; i < EXT2_N_BLOCKS; i++)
		अगर (inode->i_block[i] != 0 &&
		    (inode->i_block[i] < fs->super->s_first_तथ्य_block ||
		     inode->i_block[i] >= ext2fs_blocks_गिनती(fs->super)))
			bad++;
	वापस bad > EXT2_N_BLOCKS / 2;
}

अटल पूर्णांक extent_शीर्ष_looks_insane(काष्ठा ext2_inode_large *inode)
{
	अगर (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_header_verify(inode->i_block,
					माप(inode->i_block)) == 0)
		वापस 0;
	वापस 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
अटल व्योम check_inode_block_sanity(ext2_inode_scan scan, blk64_t अंक_blocks)
{
	ext2_ino_t	ino, inodes_to_scan;
	अचिन्हित पूर्णांक	badness, checksum_failures;
	अचिन्हित पूर्णांक	inodes_in_buf, inodes_per_block;
	अक्षर		*p;
	काष्ठा ext2_inode_large *inode;
	अक्षर		*block_status;
	अचिन्हित पूर्णांक	blk, bad_csum;

	अगर (!(scan->scan_flags & EXT2_SF_WARN_GARBAGE_INODES))
		वापस;

	inodes_to_scan = scan->inodes_बायाँ;
	inodes_in_buf = अंक_blocks * scan->fs->blocksize / scan->inode_आकार;
	अगर (inodes_to_scan > inodes_in_buf)
		inodes_to_scan = inodes_in_buf;

	p = (अक्षर *) scan->inode_बफर;
	ino = scan->current_inode + 1;
	checksum_failures = badness = 0;
	block_status = SCAN_BLOCK_STATUS(scan);
	स_रखो(block_status, 0, scan->inode_बफर_blocks);
	inodes_per_block = EXT2_INODES_PER_BLOCK(scan->fs->super);

	अगर (inodes_per_block < 2)
		वापस;

#अगर_घोषित शब्दगण_BIGENDIAN
	अगर (ext2fs_get_mem(EXT2_INODE_आकार(scan->fs->super), &inode))
		वापस;
#पूर्ण_अगर

	जबतक (inodes_to_scan > 0) {
		blk = (p - (अक्षर *)scan->inode_बफर) / scan->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(scan->fs, ino,
				(काष्ठा ext2_inode_large *) p) == 0;

#अगर_घोषित शब्दगण_BIGENDIAN
		ext2fs_swap_inode_full(scan->fs,
			       (काष्ठा ext2_inode_large *) inode,
			       (काष्ठा ext2_inode_large *) p,
			       0, EXT2_INODE_आकार(scan->fs->super));
#अन्यथा
		inode = (काष्ठा ext2_inode_large *) p;
#पूर्ण_अगर

		/* Is this inode insane? */
		अगर (bad_csum) {
			checksum_failures++;
			badness++;
		} अन्यथा अगर (extent_शीर्ष_looks_insane(inode) ||
			   block_map_looks_insane(scan->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		अगर (badness > inodes_per_block / 2) {
			अचिन्हित पूर्णांक ino_adj;

			block_status[blk] |= IBLOCK_STATUS_INSANE;
			ino_adj = inodes_per_block -
						((ino - 1) % inodes_per_block);
			अगर (ino_adj > inodes_to_scan)
				ino_adj = inodes_to_scan;
			inodes_to_scan -= ino_adj;
			p += scan->inode_आकार * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			जारी;
		}

		अगर ((ino % inodes_per_block) == 0) {
			अगर (checksum_failures == 0)
				block_status[blk] |= IBLOCK_STATUS_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_to_scan--;
		p += scan->inode_आकार;
		ino++;
	};

#अगर_घोषित शब्दगण_BIGENDIAN
	ext2fs_free_mem(&inode);
#पूर्ण_अगर
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
अटल errcode_t get_अग्र_blocks(ext2_inode_scan scan)
{
	blk64_t		अंक_blocks;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	अंक_blocks = scan->inode_बफर_blocks;
	अगर (अंक_blocks > scan->blocks_बायाँ)
		अंक_blocks = scan->blocks_बायाँ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	अगर (scan->scan_flags & EXT2_SF_BAD_INODE_BLK) {
		अगर (scan->bytes_बायाँ)
			scan->scan_flags |= EXT2_SF_BAD_EXTRA_BYTES;
		scan->scan_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	अगर (scan->scan_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_blocks(scan, &अंक_blocks);
		अगर (retval)
			वापस retval;
	}

	अगर ((scan->scan_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (scan->current_block == 0)) {
		स_रखो(scan->inode_बफर, 0,
		       (माप_प्रकार) अंक_blocks * scan->fs->blocksize);
	} अन्यथा {
		retval = io_channel_read_blk64(scan->fs->io,
					     scan->current_block,
					     (पूर्णांक) अंक_blocks,
					     scan->inode_बफर);
		अगर (retval)
			वापस EXT2_ET_अग्र_INODE_READ;
	}
	check_inode_block_sanity(scan, अंक_blocks);

	scan->ptr = scan->inode_बफर;
	scan->bytes_बायाँ = अंक_blocks * scan->fs->blocksize;

	scan->blocks_बायाँ -= अंक_blocks;
	अगर (scan->current_block)
		scan->current_block += अंक_blocks;

	वापस 0;
}

#अगर 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
अटल अंतरभूत पूर्णांक is_empty_scan(ext2_inode_scan scan)
{
	पूर्णांक	i;

	अगर (scan->bytes_बायाँ == 0)
		वापस 0;

	क्रम (i=0; i < scan->bytes_बायाँ; i++)
		अगर (scan->ptr[i])
			वापस 0;
	वापस 1;
}
#पूर्ण_अगर

errcode_t ext2fs_get_अग्र_inode_full(ext2_inode_scan scan, ext2_ino_t *ino,
				     काष्ठा ext2_inode *inode, पूर्णांक bufsize)
{
	errcode_t	retval;
	पूर्णांक		extra_bytes = 0;
	पूर्णांक		लंबाई;
	काष्ठा ext2_inode_large	*iptr = (काष्ठा ext2_inode_large *)inode;
	अक्षर		*iblock_status;
	अचिन्हित पूर्णांक	iblk;

	EXT2_CHECK_MAGIC(scan, EXT2_ET_MAGIC_INODE_SCAN);
	लंबाई = EXT2_INODE_आकार(scan->fs->super);
	iblock_status = SCAN_BLOCK_STATUS(scan);

	/*
	 * Do we need to start reading a new block group?
	 */
	अगर (scan->inodes_बायाँ <= 0) {
	force_new_group:
		अगर (scan->done_group) {
			retval = (scan->done_group)
				(scan->fs, scan, scan->current_group,
				 scan->done_group_तथ्य);
			अगर (retval)
				वापस retval;
		}
		अगर (scan->groups_बायाँ <= 0) {
			*ino = 0;
			वापस 0;
		}
		retval = get_अग्र_blockgroup(scan);
		अगर (retval)
			वापस retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	अगर ((scan->scan_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(scan->fs, scan->current_group, EXT2_BG_INODE_UNINIT)
	     ))
		जाओ force_new_group;
	अगर (scan->inodes_बायाँ == 0)
		जाओ force_new_group;
	अगर (scan->current_block == 0) {
		अगर (scan->scan_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			जाओ force_new_group;
		} अन्यथा
			वापस EXT2_ET_MISSING_INODE_TABLE;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	अगर (scan->bytes_बायाँ < scan->inode_आकार) {
		अगर (scan->bytes_बायाँ)
			स_नकल(scan->अस्थायी_बफर, scan->ptr, scan->bytes_बायाँ);
		extra_bytes = scan->bytes_बायाँ;

		retval = get_अग्र_blocks(scan);
		अगर (retval)
			वापस retval;
#अगर 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		अगर (is_empty_scan(scan))
			जाओ force_new_group;
#पूर्ण_अगर
	}

	अगर (bufsize < लंबाई) {
		retval = ext2fs_get_mem(लंबाई, &iptr);
		अगर (retval)
			वापस retval;
	}

	retval = 0;
	iblk = scan->current_inode % EXT2_INODES_PER_GROUP(scan->fs->super) /
				EXT2_INODES_PER_BLOCK(scan->fs->super) %
				scan->inode_बफर_blocks;
	अगर (extra_bytes) {
		स_नकल(scan->अस्थायी_बफर+extra_bytes, scan->ptr,
		       scan->inode_आकार - extra_bytes);
		scan->ptr += scan->inode_आकार - extra_bytes;
		scan->bytes_बायाँ -= scan->inode_आकार - extra_bytes;

		/* Verify the inode checksum. */
		अगर (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(काष्ठा ext2_inode_large *)scan->अस्थायी_बफर))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#अगर_घोषित शब्दगण_BIGENDIAN
		स_रखो(iptr, 0, लंबाई);
		ext2fs_swap_inode_full(scan->fs,
			       (काष्ठा ext2_inode_large *) iptr,
			       (काष्ठा ext2_inode_large *) scan->अस्थायी_बफर,
			       0, लंबाई);
#अन्यथा
		स_नकल(iptr, scan->अस्थायी_बफर, लंबाई);
#पूर्ण_अगर
		अगर (scan->scan_flags & EXT2_SF_BAD_EXTRA_BYTES)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
		scan->scan_flags &= ~EXT2_SF_BAD_EXTRA_BYTES;
	} अन्यथा {
		/* Verify the inode checksum. */
		अगर (!(iblock_status[iblk] & IBLOCK_STATUS_CSUMS_OK) &&
		    !(scan->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(scan->fs, scan->current_inode + 1,
				(काष्ठा ext2_inode_large *)scan->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#अगर_घोषित शब्दगण_BIGENDIAN
		स_रखो(iptr, 0, लंबाई);
		ext2fs_swap_inode_full(scan->fs,
				(काष्ठा ext2_inode_large *) iptr,
				(काष्ठा ext2_inode_large *) scan->ptr,
				0, लंबाई);
#अन्यथा
		स_नकल(iptr, scan->ptr, लंबाई);
#पूर्ण_अगर
		scan->ptr += scan->inode_आकार;
		scan->bytes_बायाँ -= scan->inode_आकार;
		अगर (scan->scan_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_BLOCK_IN_INODE_TABLE;
	}
	अगर ((iblock_status[iblk] & IBLOCK_STATUS_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	scan->inodes_बायाँ--;
	scan->current_inode++;
	*ino = scan->current_inode;
	अगर (iptr != (काष्ठा ext2_inode_large *)inode) {
		स_नकल(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	वापस retval;
}

errcode_t ext2fs_get_अग्र_inode(ext2_inode_scan scan, ext2_ino_t *ino,
				काष्ठा ext2_inode *inode)
{
	वापस ext2fs_get_अग्र_inode_full(scan, ino, inode,
						माप(काष्ठा ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     काष्ठा ext2_inode * inode, पूर्णांक bufsize,
			     पूर्णांक flags)
{
	blk64_t		block_nr;
	dgrp_t		group;
	अचिन्हित दीर्घ 	block, offset;
	अक्षर 		*ptr;
	errcode_t	retval;
	अचिन्हित	i;
	पूर्णांक		clen, inodes_per_block;
	io_channel	io;
	पूर्णांक		लंबाई = EXT2_INODE_आकार(fs->super);
	काष्ठा ext2_inode_large	*iptr;
	पूर्णांक		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (ext2fs_has_feature_journal_dev(fs->super))
		वापस EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	अगर (fs->blocksize < 1024)
		वापस EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	अगर (fs->read_inode &&
	    ((bufsize == माप(काष्ठा ext2_inode)) ||
	     (EXT2_INODE_आकार(fs->super) == माप(काष्ठा ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		अगर (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			वापस retval;
	}
	अगर ((ino == 0) || (ino > fs->super->s_inodes_गिनती))
		वापस EXT2_ET_BAD_INODE_अंक;
	/* Create inode cache if not present */
	अगर (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		अगर (retval)
			वापस retval;
	}
	/* Check to see if it's in the inode cache */
	क्रम (i = 0; i < fs->icache->cache_आकार; i++) {
		अगर (fs->icache->cache[i].ino == ino) {
			स_नकल(inode, fs->icache->cache[i].inode,
			       (bufsize > लंबाई) ? लंबाई : bufsize);
			वापस 0;
		}
	}
	अगर (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_per_block = fs->blocksize / EXT2_INODE_आकार(fs->super);
		block_nr = ext2fs_le32_to_cpu(fs->image_header->offset_inode) / fs->blocksize;
		block_nr += (ino - 1) / inodes_per_block;
		offset = ((ino - 1) % inodes_per_block) *
			EXT2_INODE_आकार(fs->super);
		io = fs->image_io;
	} अन्यथा {
		group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
		अगर (group > fs->group_desc_गिनती)
			वापस EXT2_ET_BAD_INODE_अंक;
		offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
			EXT2_INODE_आकार(fs->super);
		block = offset >> EXT2_BLOCK_आकार_BITS(fs->super);
		block_nr = ext2fs_inode_table_loc(fs, group);
		अगर (!block_nr)
			वापस EXT2_ET_MISSING_INODE_TABLE;
		अगर ((block_nr < fs->super->s_first_तथ्य_block) ||
		    (block_nr + fs->inode_blocks_per_group - 1 >=
		     ext2fs_blocks_गिनती(fs->super)))
			वापस EXT2_ET_GDESC_BAD_INODE_TABLE;
		block_nr += block;
		io = fs->io;
	}
	offset &= (EXT2_BLOCK_आकार(fs->super) - 1);

	cache_slot = (fs->icache->cache_last + 1) % fs->icache->cache_आकार;
	iptr = (काष्ठा ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (अक्षर *) iptr;
	जबतक (लंबाई) {
		clen = लंबाई;
		अगर ((offset + लंबाई) > fs->blocksize)
			clen = fs->blocksize - offset;

		अगर (block_nr != fs->icache->बफर_blk) {
			retval = io_channel_read_blk64(io, block_nr, 1,
						     fs->icache->बफर);
			अगर (retval)
				वापस retval;
			fs->icache->बफर_blk = block_nr;
		}

		स_नकल(ptr, ((अक्षर *) fs->icache->बफर) + (अचिन्हित) offset,
		       clen);

		offset = 0;
		लंबाई -= clen;
		ptr += clen;
		block_nr++;
	}
	लंबाई = EXT2_INODE_आकार(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#अगर_घोषित शब्दगण_BIGENDIAN
	ext2fs_swap_inode_full(fs, (काष्ठा ext2_inode_large *) iptr,
			       (काष्ठा ext2_inode_large *) iptr,
			       0, लंबाई);
#पूर्ण_अगर

	/* Update the inode cache bookkeeping */
	अगर (!fail_csum) {
		fs->icache->cache_last = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	स_नकल(inode, iptr, (bufsize > लंबाई) ? लंबाई : bufsize);

	अगर (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		वापस EXT2_ET_INODE_CSUM_INVALID;

	वापस 0;
}

errcode_t ext2fs_read_inode_full(ext2_filsys fs, ext2_ino_t ino,
				 काष्ठा ext2_inode * inode, पूर्णांक bufsize)
{
	वापस ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    काष्ठा ext2_inode * inode)
{
	वापस ext2fs_read_inode2(fs, ino, inode,
				  माप(काष्ठा ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      काष्ठा ext2_inode * inode, पूर्णांक bufsize,
			      पूर्णांक flags)
{
	blk64_t block_nr;
	dgrp_t group;
	अचिन्हित दीर्घ block, offset;
	errcode_t retval = 0;
	काष्ठा ext2_inode_large *w_inode;
	अक्षर *ptr;
	अचिन्हित i;
	पूर्णांक clen;
	पूर्णांक लंबाई = EXT2_INODE_आकार(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (ext2fs_has_feature_journal_dev(fs->super))
		वापस EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	अगर (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		अगर (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			वापस retval;
	}

	अगर ((ino == 0) || (ino > fs->super->s_inodes_गिनती))
		वापस EXT2_ET_BAD_INODE_अंक;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(लंबाई, &w_inode);
	अगर (retval)
		वापस retval;

	अगर (bufsize < लंबाई) {
		retval = ext2fs_read_inode2(fs, ino,
					    (काष्ठा ext2_inode *)w_inode,
					    लंबाई, READ_INODE_NOCSUM);
		अगर (retval)
			जाओ errout;
	}

	/* Check to see if the inode cache needs to be updated */
	अगर (fs->icache) {
		क्रम (i=0; i < fs->icache->cache_आकार; i++) {
			अगर (fs->icache->cache[i].ino == ino) {
				स_नकल(fs->icache->cache[i].inode, inode,
				       (bufsize > लंबाई) ? लंबाई : bufsize);
				अवरोध;
			}
		}
	} अन्यथा {
		retval = ext2fs_create_inode_cache(fs, 4);
		अगर (retval)
			जाओ errout;
	}
	स_नकल(w_inode, inode, (bufsize > लंबाई) ? लंबाई : bufsize);

	अगर (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		जाओ errout;
	}

#अगर_घोषित शब्दगण_BIGENDIAN
	ext2fs_swap_inode_full(fs, w_inode, w_inode, 1, लंबाई);
#पूर्ण_अगर

	अगर ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_set(fs, ino, w_inode);
		अगर (retval)
			जाओ errout;
	}

	group = (ino - 1) / EXT2_INODES_PER_GROUP(fs->super);
	offset = ((ino - 1) % EXT2_INODES_PER_GROUP(fs->super)) *
		EXT2_INODE_आकार(fs->super);
	block = offset >> EXT2_BLOCK_आकार_BITS(fs->super);
	block_nr = ext2fs_inode_table_loc(fs, (अचिन्हित) group);
	अगर (!block_nr) {
		retval = EXT2_ET_MISSING_INODE_TABLE;
		जाओ errout;
	}
	अगर ((block_nr < fs->super->s_first_तथ्य_block) ||
	    (block_nr + fs->inode_blocks_per_group - 1 >=
	     ext2fs_blocks_गिनती(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
		जाओ errout;
	}
	block_nr += block;

	offset &= (EXT2_BLOCK_आकार(fs->super) - 1);

	ptr = (अक्षर *) w_inode;

	जबतक (लंबाई) {
		clen = लंबाई;
		अगर ((offset + लंबाई) > fs->blocksize)
			clen = fs->blocksize - offset;

		अगर (fs->icache->बफर_blk != block_nr) {
			retval = io_channel_read_blk64(fs->io, block_nr, 1,
						     fs->icache->बफर);
			अगर (retval)
				जाओ errout;
			fs->icache->बफर_blk = block_nr;
		}


		स_नकल((अक्षर *) fs->icache->बफर + (अचिन्हित) offset,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, block_nr, 1,
					      fs->icache->बफर);
		अगर (retval)
			जाओ errout;

		offset = 0;
		ptr += clen;
		लंबाई -= clen;
		block_nr++;
	}

	fs->flags |= EXT2_FLAG_CHANGED;
errout:
	ext2fs_free_mem(&w_inode);
	वापस retval;
}

errcode_t ext2fs_write_inode_full(ext2_filsys fs, ext2_ino_t ino,
				  काष्ठा ext2_inode * inode, पूर्णांक bufsize)
{
	वापस ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     काष्ठा ext2_inode *inode)
{
	वापस ext2fs_write_inode2(fs, ino, inode,
				   माप(काष्ठा ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 काष्ठा ext2_inode *inode)
{
	काष्ठा ext2_inode	*buf;
	पूर्णांक			आकार = EXT2_INODE_आकार(fs->super);
	काष्ठा ext2_inode_large	*large_inode;
	errcode_t		retval;
	समय_प्रकार			t = ext2fsP_get_time(fs);

	अगर (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_set(inode, i_atime, t);
	अगर (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_set(inode, i_ctime, t);
	अगर (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_set(inode, i_mtime, t);

	अगर (आकार == माप(काष्ठा ext2_inode))
		वापस ext2fs_write_inode_full(fs, ino, inode,
					       माप(काष्ठा ext2_inode));

	buf = दो_स्मृति(आकार);
	अगर (!buf)
		वापस ENOMEM;

	स_रखो(buf, 0, आकार);
	*buf = *inode;

	large_inode = (काष्ठा ext2_inode_large *) buf;
	large_inode->i_extra_isize = माप(काष्ठा ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_आकार;
	अगर (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_full(fs, ino, buf, आकार);
	मुक्त(buf);
	वापस retval;
}


errcode_t ext2fs_get_blocks(ext2_filsys fs, ext2_ino_t ino, blk_t *blocks)
{
	काष्ठा ext2_inode	inode;
	पूर्णांक			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (ino > fs->super->s_inodes_गिनती)
		वापस EXT2_ET_BAD_INODE_अंक;

	अगर (fs->get_blocks) {
		अगर (!(*fs->get_blocks)(fs, ino, blocks))
			वापस 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	अगर (retval)
		वापस retval;
	क्रम (i=0; i < EXT2_N_BLOCKS; i++)
		blocks[i] = inode.i_block[i];
	वापस 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	काष्ठा	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (ino > fs->super->s_inodes_गिनती)
		वापस EXT2_ET_BAD_INODE_अंक;

	अगर (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		अगर (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			वापस retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	अगर (retval)
		वापस retval;
	अगर (!LINUX_S_ISDIR(inode.i_mode))
		वापस EXT2_ET_NO_DIRECTORY;
	वापस 0;
}

