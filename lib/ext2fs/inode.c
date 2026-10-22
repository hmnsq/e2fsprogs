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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"
#అన్తర్భావ "e2image.h"

#నిర్వచించు IBLOCK_స్థితి_CSUMS_OK	1
#నిర్వచించు IBLOCK_స్థితి_INSANE	2
#నిర్వచించు పరిశీలన_ఖండం_స్థితి(పరిశీలన)	((పరిశీలన)->తాత్కాలిక_బఫరు + (పరిశీలన)->inode_కొలత)

నిర్మాణం ext2_struct_inode_పరిశీలన {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_ఖండం;
	dgrp_t			current_సమూహం;
	ext2_ino_t		inodes_ఎడమ;
	blk_t			ఖండాలు_ఎడమ;
	dgrp_t			groups_ఎడమ;
	blk_t			inode_బఫరు_ఖండాలు;
	అక్షరం *			inode_బఫరు;
	పూర్ణాంకం			inode_కొలత;
	అక్షరం *			ptr;
	పూర్ణాంకం			బైట్లు_ఎడమ;
	అక్షరం			*తాత్కాలిక_బఫరు;
	errcode_t		(*పూర్తి_సమూహం)(ext2_filsys fs,
					      ext2_inode_పరిశీలన పరిశీలన,
					      dgrp_t సమూహం,
					      శూన్య * priv_దత్తాంశం);
	శూన్య *			పూర్తి_సమూహం_దత్తాంశం;
	పూర్ణాంకం			bad_ఖండం_ptr;
	పూర్ణాంకం			పరిశీలన_flags;
	పూర్ణాంకం			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	గుర్తులేని	i;

	ఒకవేళ (!fs->icache)
		తిరిగి 0;

	కొరకు (i=0; i < fs->icache->cache_కొలత; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->బఫరు_blk = 0;
	తిరిగి 0;
}

/*
 * Free the inode cache structure
 */
శూన్య ext2fs_free_inode_cache(నిర్మాణం ext2_inode_cache *icache)
{
	గుర్తులేని i;

	ఒకవేళ (--icache->refcount)
		తిరిగి;
	ఒకవేళ (icache->బఫరు)
		ext2fs_free_mem(&icache->బఫరు);
	కొరకు (i = 0; i < icache->cache_కొలత; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	ఒకవేళ (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->బఫరు_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, గుర్తులేని పూర్ణాంకం cache_కొలత)
{
	గుర్తులేని	i;
	errcode_t	retval;

	ఒకవేళ (fs->icache)
		తిరిగి 0;
	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_inode_cache), &fs->icache);
	ఒకవేళ (retval)
		తిరిగి retval;

	memset(fs->icache, 0, పరిమాణం(నిర్మాణం ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->బఫరు);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	fs->icache->బఫరు_blk = 0;
	fs->icache->cache_చివరి = -1;
	fs->icache->cache_కొలత = cache_కొలత;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_కొలత,
				  పరిమాణం(నిర్మాణం ext2_inode_cache_ent),
				  &fs->icache->cache);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	కొరకు (i = 0; i < fs->icache->cache_కొలత; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_కొలత(fs->super),
					&fs->icache->cache[i].inode);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}

	ext2fs_flush_icache(fs);
	తిరిగి 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	తిరిగి retval;
}

errcode_t ext2fs_open_inode_పరిశీలన(ext2_filsys fs, పూర్ణాంకం బఫరు_ఖండాలు,
				 ext2_inode_పరిశీలన *ret_పరిశీలన)
{
	ext2_inode_పరిశీలన	పరిశీలన;
	errcode_t	retval;
	errcode_t (*save_get_ఖండాలు)(ext2_filsys f, ext2_ino_t ino, blk_t *ఖండాలు);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super))
		తిరిగి EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ఒకవేళ (fs->blocksize < 1024)
		తిరిగి EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	ఒకవేళ (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_ఖండాలు = fs->get_ఖండాలు;
		fs->get_ఖండాలు = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		ఒకవేళ (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_ఖండాలు = save_get_ఖండాలు;
	}

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_struct_inode_పరిశీలన), &పరిశీలన);
	ఒకవేళ (retval)
		తిరిగి retval;
	memset(పరిశీలన, 0, పరిమాణం(నిర్మాణం ext2_struct_inode_పరిశీలన));

	పరిశీలన->magic = EXT2_ET_MAGIC_INODE_పరిశీలన;
	పరిశీలన->fs = fs;
	పరిశీలన->inode_కొలత = EXT2_INODE_కొలత(fs->super);
	పరిశీలన->బైట్లు_ఎడమ = 0;
	పరిశీలన->current_సమూహం = 0;
	పరిశీలన->groups_ఎడమ = fs->సమూహం_desc_లెక్క - 1;
	పరిశీలన->inode_బఫరు_ఖండాలు = బఫరు_ఖండాలు ? బఫరు_ఖండాలు :
				    EXT2_INODE_పరిశీలన_DEFAULT_బఫరు_ఖండాలు;
	పరిశీలన->current_ఖండం = ext2fs_inode_పట్టిక_loc(పరిశీలన->fs,
						     పరిశీలన->current_సమూహం);
	ఒకవేళ (పరిశీలన->current_ఖండం &&
	    ((పరిశీలన->current_ఖండం < fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
	     (పరిశీలన->current_ఖండం + fs->inode_ఖండాలు_చొప్పున_సమూహం - 1 >=
	      ext2fs_ఖండాలు_లెక్క(fs->super)))) {
		ext2fs_free_mem(&పరిశీలన);
		తిరిగి EXT2_ET_GDESC_BAD_INODE_పట్టిక;
	}

	పరిశీలన->inodes_ఎడమ = EXT2_INODES_చొప్పున_సమూహం(పరిశీలన->fs->super);
	పరిశీలన->ఖండాలు_ఎడమ = పరిశీలన->fs->inode_ఖండాలు_చొప్పున_సమూహం;
	ఒకవేళ (ext2fs_has_సమూహం_desc_csum(fs)) {
		__u32 వాడని = ext2fs_bg_itable_వాడని(fs, పరిశీలన->current_సమూహం);
		ఒకవేళ (పరిశీలన->inodes_ఎడమ > వాడని)
			పరిశీలన->inodes_ఎడమ -= వాడని;
		లేకపోతే
			పరిశీలన->inodes_ఎడమ = 0;
		పరిశీలన->ఖండాలు_ఎడమ =
			(పరిశీలన->inodes_ఎడమ +
			 (fs->blocksize / పరిశీలన->inode_కొలత - 1)) *
			పరిశీలన->inode_కొలత / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, పరిశీలన->inode_బఫరు_ఖండాలు,
				      &పరిశీలన->inode_బఫరు);
	పరిశీలన->పూర్తి_సమూహం = 0;
	పరిశీలన->పూర్తి_సమూహం_దత్తాంశం = 0;
	పరిశీలన->bad_ఖండం_ptr = 0;
	ఒకవేళ (retval) {
		ext2fs_free_mem(&పరిశీలన);
		తిరిగి retval;
	}
	retval = ext2fs_get_mem(పరిశీలన->inode_కొలత + పరిశీలన->inode_బఫరు_ఖండాలు,
				&పరిశీలన->తాత్కాలిక_బఫరు);
	ఒకవేళ (retval) {
		ext2fs_free_mem(&పరిశీలన->inode_బఫరు);
		ext2fs_free_mem(&పరిశీలన);
		తిరిగి retval;
	}
	memset(పరిశీలన_ఖండం_స్థితి(పరిశీలన), 0, పరిశీలన->inode_బఫరు_ఖండాలు);
	ఒకవేళ (పరిశీలన->fs->badblocks && పరిశీలన->fs->badblocks->అంకె)
		పరిశీలన->పరిశీలన_flags |= EXT2_SF_CHK_BADBLOCKS;
	ఒకవేళ (ext2fs_has_సమూహం_desc_csum(fs))
		పరిశీలన->పరిశీలన_flags |= EXT2_SF_DO_LAZY;
	*ret_పరిశీలన = పరిశీలన;
	తిరిగి 0;
}

శూన్య ext2fs_close_inode_పరిశీలన(ext2_inode_పరిశీలన పరిశీలన)
{
	ఒకవేళ (!పరిశీలన || (పరిశీలన->magic != EXT2_ET_MAGIC_INODE_పరిశీలన))
		తిరిగి;

	ext2fs_free_mem(&పరిశీలన->inode_బఫరు);
	పరిశీలన->inode_బఫరు = ఖాళీ;
	ext2fs_free_mem(&పరిశీలన->తాత్కాలిక_బఫరు);
	పరిశీలన->తాత్కాలిక_బఫరు = ఖాళీ;
	ext2fs_free_mem(&పరిశీలన);
	తిరిగి;
}

శూన్య ext2fs_సమితి_inode_callback(ext2_inode_పరిశీలన పరిశీలన,
			       errcode_t (*పూర్తి_సమూహం)(ext2_filsys fs,
						       ext2_inode_పరిశీలన పరిశీలన,
						       dgrp_t సమూహం,
						       శూన్య * priv_దత్తాంశం),
			       శూన్య *పూర్తి_సమూహం_దత్తాంశం)
{
	ఒకవేళ (!పరిశీలన || (పరిశీలన->magic != EXT2_ET_MAGIC_INODE_పరిశీలన))
		తిరిగి;

	పరిశీలన->పూర్తి_సమూహం = పూర్తి_సమూహం;
	పరిశీలన->పూర్తి_సమూహం_దత్తాంశం = పూర్తి_సమూహం_దత్తాంశం;
}

పూర్ణాంకం ext2fs_inode_పరిశీలన_flags(ext2_inode_పరిశీలన పరిశీలన, పూర్ణాంకం సమితి_flags,
			    పూర్ణాంకం clear_flags)
{
	పూర్ణాంకం	old_flags;

	ఒకవేళ (!పరిశీలన || (పరిశీలన->magic != EXT2_ET_MAGIC_INODE_పరిశీలన))
		తిరిగి 0;

	old_flags = పరిశీలన->పరిశీలన_flags;
	పరిశీలన->పరిశీలన_flags &= ~clear_flags;
	పరిశీలన->పరిశీలన_flags |= సమితి_flags;
	తిరిగి old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
స్థిర errcode_t get_తదుపరి_blockgroup(ext2_inode_పరిశీలన పరిశీలన)
{
	ext2_filsys fs = పరిశీలన->fs;

	పరిశీలన->current_సమూహం++;
	పరిశీలన->groups_ఎడమ--;

	పరిశీలన->current_ఖండం = ext2fs_inode_పట్టిక_loc(పరిశీలన->fs,
						     పరిశీలన->current_సమూహం);
	పరిశీలన->current_inode = పరిశీలన->current_సమూహం *
		EXT2_INODES_చొప్పున_సమూహం(fs->super);

	పరిశీలన->బైట్లు_ఎడమ = 0;
	పరిశీలన->inodes_ఎడమ = EXT2_INODES_చొప్పున_సమూహం(fs->super);
	పరిశీలన->ఖండాలు_ఎడమ = fs->inode_ఖండాలు_చొప్పున_సమూహం;
	ఒకవేళ (ext2fs_has_సమూహం_desc_csum(fs)) {
		__u32 వాడని = ext2fs_bg_itable_వాడని(fs, పరిశీలన->current_సమూహం);
		ఒకవేళ (పరిశీలన->inodes_ఎడమ > వాడని)
			పరిశీలన->inodes_ఎడమ -= వాడని;
		లేకపోతే
			పరిశీలన->inodes_ఎడమ = 0;
		పరిశీలన->ఖండాలు_ఎడమ =
			(పరిశీలన->inodes_ఎడమ +
			 (fs->blocksize / పరిశీలన->inode_కొలత - 1)) *
			పరిశీలన->inode_కొలత / fs->blocksize;
	}
	ఒకవేళ (పరిశీలన->current_ఖండం &&
	    ((పరిశీలన->current_ఖండం < fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
	     (పరిశీలన->current_ఖండం + fs->inode_ఖండాలు_చొప్పున_సమూహం - 1 >=
	      ext2fs_ఖండాలు_లెక్క(fs->super))))
		తిరిగి EXT2_ET_GDESC_BAD_INODE_పట్టిక;
	తిరిగి 0;
}

errcode_t ext2fs_inode_పరిశీలన_goto_blockgroup(ext2_inode_పరిశీలన పరిశీలన,
					    పూర్ణాంకం	సమూహం)
{
	పరిశీలన->current_సమూహం = సమూహం - 1;
	పరిశీలన->groups_ఎడమ = పరిశీలన->fs->సమూహం_desc_లెక్క - సమూహం;
	పరిశీలన->bad_ఖండం_ptr = 0;
	తిరిగి get_తదుపరి_blockgroup(పరిశీలన);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
స్థిర errcode_t check_for_inode_bad_ఖండాలు(ext2_inode_పరిశీలన పరిశీలన,
					    blk64_t *అంకె_ఖండాలు)
{
	blk64_t	blk = పరిశీలన->current_ఖండం;
	badblocks_list	bb = పరిశీలన->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	ఒకవేళ (blk == 0)
		తిరిగి 0;

	/* Make sure bad_block_ptr is still valid */
	ఒకవేళ (పరిశీలన->bad_ఖండం_ptr >= bb->అంకె) {
		పరిశీలన->పరిశీలన_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		తిరిగి 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	అయితే (blk > bb->list[పరిశీలన->bad_ఖండం_ptr]) {
		ఒకవేళ (++పరిశీలన->bad_ఖండం_ptr >= bb->అంకె) {
			పరిశీలన->పరిశీలన_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			తిరిగి 0;
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
	ఒకవేళ (blk == bb->list[పరిశీలన->bad_ఖండం_ptr]) {
		పరిశీలన->పరిశీలన_flags |= EXT2_SF_BAD_INODE_BLK;
		*అంకె_ఖండాలు = 1;
		ఒకవేళ (++పరిశీలన->bad_ఖండం_ptr >= bb->అంకె)
			పరిశీలన->పరిశీలన_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		తిరిగి 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	ఒకవేళ ((blk + *అంకె_ఖండాలు) > bb->list[పరిశీలన->bad_ఖండం_ptr])
		*అంకె_ఖండాలు = (పూర్ణాంకం) (bb->list[పరిశీలన->bad_ఖండం_ptr] - blk);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం ఖండం_map_looks_insane(ext2_filsys fs,
				  నిర్మాణం ext2_inode_large *inode)
{
	గుర్తులేని పూర్ణాంకం i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	ఒకవేళ ((inode->i_flags & EXT4_INLINE_దత్తాంశం_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		తిరిగి 0;
	ఒకవేళ (!LINUX_S_ISREG(inode->i_బహుళకం) &&
	    !LINUX_S_ISLNK(inode->i_బహుళకం) &&
	    !LINUX_S_ISDIR(inode->i_బహుళకం))
		తిరిగి 0;
	ఒకవేళ (LINUX_S_ISLNK(inode->i_బహుళకం) &&
	    EXT2_I_కొలత(inode) <= పరిమాణం(inode->i_ఖండం))
		తిరిగి 0;

	/* Unused inodes probably aren't insane */
	ఒకవేళ (inode->i_links_లెక్క == 0)
		తిరిగి 0;

	/* See if more than half the block maps are insane */
	కొరకు (i = 0, bad = 0; i < EXT2_N_ఖండాలు; i++)
		ఒకవేళ (inode->i_ఖండం[i] != 0 &&
		    (inode->i_ఖండం[i] < fs->super->s_మొదటి_దత్తాంశం_ఖండం ||
		     inode->i_ఖండం[i] >= ext2fs_ఖండాలు_లెక్క(fs->super)))
			bad++;
	తిరిగి bad > EXT2_N_ఖండాలు / 2;
}

స్థిర పూర్ణాంకం extent_తల_looks_insane(నిర్మాణం ext2_inode_large *inode)
{
	ఒకవేళ (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_శీర్షిక_verify(inode->i_ఖండం,
					పరిమాణం(inode->i_ఖండం)) == 0)
		తిరిగి 0;
	తిరిగి 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
స్థిర శూన్య check_inode_ఖండం_sanity(ext2_inode_పరిశీలన పరిశీలన, blk64_t అంకె_ఖండాలు)
{
	ext2_ino_t	ino, inodes_గమ్యం_పరిశీలన;
	గుర్తులేని పూర్ణాంకం	badness, checksum_failures;
	గుర్తులేని పూర్ణాంకం	inodes_లోపల_buf, inodes_చొప్పున_ఖండం;
	అక్షరం		*p;
	నిర్మాణం ext2_inode_large *inode;
	అక్షరం		*ఖండం_స్థితి;
	గుర్తులేని పూర్ణాంకం	blk, bad_csum;

	ఒకవేళ (!(పరిశీలన->పరిశీలన_flags & EXT2_SF_WARN_GARBAGE_INODES))
		తిరిగి;

	inodes_గమ్యం_పరిశీలన = పరిశీలన->inodes_ఎడమ;
	inodes_లోపల_buf = అంకె_ఖండాలు * పరిశీలన->fs->blocksize / పరిశీలన->inode_కొలత;
	ఒకవేళ (inodes_గమ్యం_పరిశీలన > inodes_లోపల_buf)
		inodes_గమ్యం_పరిశీలన = inodes_లోపల_buf;

	p = (అక్షరం *) పరిశీలన->inode_బఫరు;
	ino = పరిశీలన->current_inode + 1;
	checksum_failures = badness = 0;
	ఖండం_స్థితి = పరిశీలన_ఖండం_స్థితి(పరిశీలన);
	memset(ఖండం_స్థితి, 0, పరిశీలన->inode_బఫరు_ఖండాలు);
	inodes_చొప్పున_ఖండం = EXT2_INODES_చొప్పున_ఖండం(పరిశీలన->fs->super);

	ఒకవేళ (inodes_చొప్పున_ఖండం < 2)
		తిరిగి;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	ఒకవేళ (ext2fs_get_mem(EXT2_INODE_కొలత(పరిశీలన->fs->super), &inode))
		తిరిగి;
#అయితే_ముగింపు

	అయితే (inodes_గమ్యం_పరిశీలన > 0) {
		blk = (p - (అక్షరం *)పరిశీలన->inode_బఫరు) / పరిశీలన->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(పరిశీలన->fs, ino,
				(నిర్మాణం ext2_inode_large *) p) == 0;

#నిర్వచితమైతే పదాలు_BIGENDIAN
		ext2fs_మార్పిడి_inode_నిండిన(పరిశీలన->fs,
			       (నిర్మాణం ext2_inode_large *) inode,
			       (నిర్మాణం ext2_inode_large *) p,
			       0, EXT2_INODE_కొలత(పరిశీలన->fs->super));
#లేకపోతే
		inode = (నిర్మాణం ext2_inode_large *) p;
#అయితే_ముగింపు

		/* Is this inode insane? */
		ఒకవేళ (bad_csum) {
			checksum_failures++;
			badness++;
		} లేకపోతే ఒకవేళ (extent_తల_looks_insane(inode) ||
			   ఖండం_map_looks_insane(పరిశీలన->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		ఒకవేళ (badness > inodes_చొప్పున_ఖండం / 2) {
			గుర్తులేని పూర్ణాంకం ino_adj;

			ఖండం_స్థితి[blk] |= IBLOCK_స్థితి_INSANE;
			ino_adj = inodes_చొప్పున_ఖండం -
						((ino - 1) % inodes_చొప్పున_ఖండం);
			ఒకవేళ (ino_adj > inodes_గమ్యం_పరిశీలన)
				ino_adj = inodes_గమ్యం_పరిశీలన;
			inodes_గమ్యం_పరిశీలన -= ino_adj;
			p += పరిశీలన->inode_కొలత * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			కొనసాగు;
		}

		ఒకవేళ ((ino % inodes_చొప్పున_ఖండం) == 0) {
			ఒకవేళ (checksum_failures == 0)
				ఖండం_స్థితి[blk] |= IBLOCK_స్థితి_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_గమ్యం_పరిశీలన--;
		p += పరిశీలన->inode_కొలత;
		ino++;
	};

#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_free_mem(&inode);
#అయితే_ముగింపు
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
స్థిర errcode_t get_తదుపరి_ఖండాలు(ext2_inode_పరిశీలన పరిశీలన)
{
	blk64_t		అంకె_ఖండాలు;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	అంకె_ఖండాలు = పరిశీలన->inode_బఫరు_ఖండాలు;
	ఒకవేళ (అంకె_ఖండాలు > పరిశీలన->ఖండాలు_ఎడమ)
		అంకె_ఖండాలు = పరిశీలన->ఖండాలు_ఎడమ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	ఒకవేళ (పరిశీలన->పరిశీలన_flags & EXT2_SF_BAD_INODE_BLK) {
		ఒకవేళ (పరిశీలన->బైట్లు_ఎడమ)
			పరిశీలన->పరిశీలన_flags |= EXT2_SF_BAD_EXTRA_బైట్లు;
		పరిశీలన->పరిశీలన_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	ఒకవేళ (పరిశీలన->పరిశీలన_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_ఖండాలు(పరిశీలన, &అంకె_ఖండాలు);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	ఒకవేళ ((పరిశీలన->పరిశీలన_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (పరిశీలన->current_ఖండం == 0)) {
		memset(పరిశీలన->inode_బఫరు, 0,
		       (size_t) అంకె_ఖండాలు * పరిశీలన->fs->blocksize);
	} లేకపోతే {
		retval = io_channel_read_blk64(పరిశీలన->fs->io,
					     పరిశీలన->current_ఖండం,
					     (పూర్ణాంకం) అంకె_ఖండాలు,
					     పరిశీలన->inode_బఫరు);
		ఒకవేళ (retval)
			తిరిగి EXT2_ET_తదుపరి_INODE_READ;
	}
	check_inode_ఖండం_sanity(పరిశీలన, అంకె_ఖండాలు);

	పరిశీలన->ptr = పరిశీలన->inode_బఫరు;
	పరిశీలన->బైట్లు_ఎడమ = అంకె_ఖండాలు * పరిశీలన->fs->blocksize;

	పరిశీలన->ఖండాలు_ఎడమ -= అంకె_ఖండాలు;
	ఒకవేళ (పరిశీలన->current_ఖండం)
		పరిశీలన->current_ఖండం += అంకె_ఖండాలు;

	తిరిగి 0;
}

#ఒకవేళ 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
స్థిర అంతఃవరుస పూర్ణాంకం is_ఖాళీలు_పరిశీలన(ext2_inode_పరిశీలన పరిశీలన)
{
	పూర్ణాంకం	i;

	ఒకవేళ (పరిశీలన->బైట్లు_ఎడమ == 0)
		తిరిగి 0;

	కొరకు (i=0; i < పరిశీలన->బైట్లు_ఎడమ; i++)
		ఒకవేళ (పరిశీలన->ptr[i])
			తిరిగి 0;
	తిరిగి 1;
}
#అయితే_ముగింపు

errcode_t ext2fs_get_తదుపరి_inode_నిండిన(ext2_inode_పరిశీలన పరిశీలన, ext2_ino_t *ino,
				     నిర్మాణం ext2_inode *inode, పూర్ణాంకం bufsize)
{
	errcode_t	retval;
	పూర్ణాంకం		extra_బైట్లు = 0;
	పూర్ణాంకం		నిడివి;
	నిర్మాణం ext2_inode_large	*iptr = (నిర్మాణం ext2_inode_large *)inode;
	అక్షరం		*iblock_స్థితి;
	గుర్తులేని పూర్ణాంకం	iblk;

	EXT2_CHECK_MAGIC(పరిశీలన, EXT2_ET_MAGIC_INODE_పరిశీలన);
	నిడివి = EXT2_INODE_కొలత(పరిశీలన->fs->super);
	iblock_స్థితి = పరిశీలన_ఖండం_స్థితి(పరిశీలన);

	/*
	 * Do we need to start reading a new block group?
	 */
	ఒకవేళ (పరిశీలన->inodes_ఎడమ <= 0) {
	force_new_సమూహం:
		ఒకవేళ (పరిశీలన->పూర్తి_సమూహం) {
			retval = (పరిశీలన->పూర్తి_సమూహం)
				(పరిశీలన->fs, పరిశీలన, పరిశీలన->current_సమూహం,
				 పరిశీలన->పూర్తి_సమూహం_దత్తాంశం);
			ఒకవేళ (retval)
				తిరిగి retval;
		}
		ఒకవేళ (పరిశీలన->groups_ఎడమ <= 0) {
			*ino = 0;
			తిరిగి 0;
		}
		retval = get_తదుపరి_blockgroup(పరిశీలన);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	ఒకవేళ ((పరిశీలన->పరిశీలన_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(పరిశీలన->fs, పరిశీలన->current_సమూహం, EXT2_BG_INODE_UNINIT)
	     ))
		వెళ్ళు force_new_సమూహం;
	ఒకవేళ (పరిశీలన->inodes_ఎడమ == 0)
		వెళ్ళు force_new_సమూహం;
	ఒకవేళ (పరిశీలన->current_ఖండం == 0) {
		ఒకవేళ (పరిశీలన->పరిశీలన_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			వెళ్ళు force_new_సమూహం;
		} లేకపోతే
			తిరిగి EXT2_ET_MISSING_INODE_పట్టిక;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	ఒకవేళ (పరిశీలన->బైట్లు_ఎడమ < పరిశీలన->inode_కొలత) {
		ఒకవేళ (పరిశీలన->బైట్లు_ఎడమ)
			జ్ఞాపకం_నకలు(పరిశీలన->తాత్కాలిక_బఫరు, పరిశీలన->ptr, పరిశీలన->బైట్లు_ఎడమ);
		extra_బైట్లు = పరిశీలన->బైట్లు_ఎడమ;

		retval = get_తదుపరి_ఖండాలు(పరిశీలన);
		ఒకవేళ (retval)
			తిరిగి retval;
#ఒకవేళ 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		ఒకవేళ (is_ఖాళీలు_పరిశీలన(పరిశీలన))
			వెళ్ళు force_new_సమూహం;
#అయితే_ముగింపు
	}

	ఒకవేళ (bufsize < నిడివి) {
		retval = ext2fs_get_mem(నిడివి, &iptr);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	retval = 0;
	iblk = పరిశీలన->current_inode % EXT2_INODES_చొప్పున_సమూహం(పరిశీలన->fs->super) /
				EXT2_INODES_చొప్పున_ఖండం(పరిశీలన->fs->super) %
				పరిశీలన->inode_బఫరు_ఖండాలు;
	ఒకవేళ (extra_బైట్లు) {
		జ్ఞాపకం_నకలు(పరిశీలన->తాత్కాలిక_బఫరు+extra_బైట్లు, పరిశీలన->ptr,
		       పరిశీలన->inode_కొలత - extra_బైట్లు);
		పరిశీలన->ptr += పరిశీలన->inode_కొలత - extra_బైట్లు;
		పరిశీలన->బైట్లు_ఎడమ -= పరిశీలన->inode_కొలత - extra_బైట్లు;

		/* Verify the inode checksum. */
		ఒకవేళ (!(iblock_స్థితి[iblk] & IBLOCK_స్థితి_CSUMS_OK) &&
		    !(పరిశీలన->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(పరిశీలన->fs, పరిశీలన->current_inode + 1,
				(నిర్మాణం ext2_inode_large *)పరిశీలన->తాత్కాలిక_బఫరు))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#నిర్వచితమైతే పదాలు_BIGENDIAN
		memset(iptr, 0, నిడివి);
		ext2fs_మార్పిడి_inode_నిండిన(పరిశీలన->fs,
			       (నిర్మాణం ext2_inode_large *) iptr,
			       (నిర్మాణం ext2_inode_large *) పరిశీలన->తాత్కాలిక_బఫరు,
			       0, నిడివి);
#లేకపోతే
		జ్ఞాపకం_నకలు(iptr, పరిశీలన->తాత్కాలిక_బఫరు, నిడివి);
#అయితే_ముగింపు
		ఒకవేళ (పరిశీలన->పరిశీలన_flags & EXT2_SF_BAD_EXTRA_బైట్లు)
			retval = EXT2_ET_BAD_ఖండం_లోపల_INODE_పట్టిక;
		పరిశీలన->పరిశీలన_flags &= ~EXT2_SF_BAD_EXTRA_బైట్లు;
	} లేకపోతే {
		/* Verify the inode checksum. */
		ఒకవేళ (!(iblock_స్థితి[iblk] & IBLOCK_స్థితి_CSUMS_OK) &&
		    !(పరిశీలన->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(పరిశీలన->fs, పరిశీలన->current_inode + 1,
				(నిర్మాణం ext2_inode_large *)పరిశీలన->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#నిర్వచితమైతే పదాలు_BIGENDIAN
		memset(iptr, 0, నిడివి);
		ext2fs_మార్పిడి_inode_నిండిన(పరిశీలన->fs,
				(నిర్మాణం ext2_inode_large *) iptr,
				(నిర్మాణం ext2_inode_large *) పరిశీలన->ptr,
				0, నిడివి);
#లేకపోతే
		జ్ఞాపకం_నకలు(iptr, పరిశీలన->ptr, నిడివి);
#అయితే_ముగింపు
		పరిశీలన->ptr += పరిశీలన->inode_కొలత;
		పరిశీలన->బైట్లు_ఎడమ -= పరిశీలన->inode_కొలత;
		ఒకవేళ (పరిశీలన->పరిశీలన_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_ఖండం_లోపల_INODE_పట్టిక;
	}
	ఒకవేళ ((iblock_స్థితి[iblk] & IBLOCK_స్థితి_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	పరిశీలన->inodes_ఎడమ--;
	పరిశీలన->current_inode++;
	*ino = పరిశీలన->current_inode;
	ఒకవేళ (iptr != (నిర్మాణం ext2_inode_large *)inode) {
		జ్ఞాపకం_నకలు(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	తిరిగి retval;
}

errcode_t ext2fs_get_తదుపరి_inode(ext2_inode_పరిశీలన పరిశీలన, ext2_ino_t *ino,
				నిర్మాణం ext2_inode *inode)
{
	తిరిగి ext2fs_get_తదుపరి_inode_నిండిన(పరిశీలన, ino, inode,
						పరిమాణం(నిర్మాణం ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     నిర్మాణం ext2_inode * inode, పూర్ణాంకం bufsize,
			     పూర్ణాంకం flags)
{
	blk64_t		ఖండం_nr;
	dgrp_t		సమూహం;
	గుర్తులేని దీర్ఘ 	ఖండం, స్థానభ్రంశం;
	అక్షరం 		*ptr;
	errcode_t	retval;
	గుర్తులేని	i;
	పూర్ణాంకం		clen, inodes_చొప్పున_ఖండం;
	io_channel	io;
	పూర్ణాంకం		నిడివి = EXT2_INODE_కొలత(fs->super);
	నిర్మాణం ext2_inode_large	*iptr;
	పూర్ణాంకం		cache_స్థానం, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super))
		తిరిగి EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ఒకవేళ (fs->blocksize < 1024)
		తిరిగి EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	ఒకవేళ (fs->read_inode &&
	    ((bufsize == పరిమాణం(నిర్మాణం ext2_inode)) ||
	     (EXT2_INODE_కొలత(fs->super) == పరిమాణం(నిర్మాణం ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		ఒకవేళ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			తిరిగి retval;
	}
	ఒకవేళ ((ino == 0) || (ino > fs->super->s_inodes_లెక్క))
		తిరిగి EXT2_ET_BAD_INODE_అంకె;
	/* Create inode cache if not present */
	ఒకవేళ (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	/* Check to see if it's in the inode cache */
	కొరకు (i = 0; i < fs->icache->cache_కొలత; i++) {
		ఒకవేళ (fs->icache->cache[i].ino == ino) {
			జ్ఞాపకం_నకలు(inode, fs->icache->cache[i].inode,
			       (bufsize > నిడివి) ? నిడివి : bufsize);
			తిరిగి 0;
		}
	}
	ఒకవేళ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_చొప్పున_ఖండం = fs->blocksize / EXT2_INODE_కొలత(fs->super);
		ఖండం_nr = ext2fs_le32_గమ్యం_cpu(fs->image_శీర్షిక->స్థానభ్రంశం_inode) / fs->blocksize;
		ఖండం_nr += (ino - 1) / inodes_చొప్పున_ఖండం;
		స్థానభ్రంశం = ((ino - 1) % inodes_చొప్పున_ఖండం) *
			EXT2_INODE_కొలత(fs->super);
		io = fs->image_io;
	} లేకపోతే {
		సమూహం = (ino - 1) / EXT2_INODES_చొప్పున_సమూహం(fs->super);
		ఒకవేళ (సమూహం > fs->సమూహం_desc_లెక్క)
			తిరిగి EXT2_ET_BAD_INODE_అంకె;
		స్థానభ్రంశం = ((ino - 1) % EXT2_INODES_చొప్పున_సమూహం(fs->super)) *
			EXT2_INODE_కొలత(fs->super);
		ఖండం = స్థానభ్రంశం >> EXT2_ఖండం_కొలత_బిట్లు(fs->super);
		ఖండం_nr = ext2fs_inode_పట్టిక_loc(fs, సమూహం);
		ఒకవేళ (!ఖండం_nr)
			తిరిగి EXT2_ET_MISSING_INODE_పట్టిక;
		ఒకవేళ ((ఖండం_nr < fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
		    (ఖండం_nr + fs->inode_ఖండాలు_చొప్పున_సమూహం - 1 >=
		     ext2fs_ఖండాలు_లెక్క(fs->super)))
			తిరిగి EXT2_ET_GDESC_BAD_INODE_పట్టిక;
		ఖండం_nr += ఖండం;
		io = fs->io;
	}
	స్థానభ్రంశం &= (EXT2_ఖండం_కొలత(fs->super) - 1);

	cache_స్థానం = (fs->icache->cache_చివరి + 1) % fs->icache->cache_కొలత;
	iptr = (నిర్మాణం ext2_inode_large *)fs->icache->cache[cache_స్థానం].inode;

	ptr = (అక్షరం *) iptr;
	అయితే (నిడివి) {
		clen = నిడివి;
		ఒకవేళ ((స్థానభ్రంశం + నిడివి) > fs->blocksize)
			clen = fs->blocksize - స్థానభ్రంశం;

		ఒకవేళ (ఖండం_nr != fs->icache->బఫరు_blk) {
			retval = io_channel_read_blk64(io, ఖండం_nr, 1,
						     fs->icache->బఫరు);
			ఒకవేళ (retval)
				తిరిగి retval;
			fs->icache->బఫరు_blk = ఖండం_nr;
		}

		జ్ఞాపకం_నకలు(ptr, ((అక్షరం *) fs->icache->బఫరు) + (గుర్తులేని) స్థానభ్రంశం,
		       clen);

		స్థానభ్రంశం = 0;
		నిడివి -= clen;
		ptr += clen;
		ఖండం_nr++;
	}
	నిడివి = EXT2_INODE_కొలత(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_మార్పిడి_inode_నిండిన(fs, (నిర్మాణం ext2_inode_large *) iptr,
			       (నిర్మాణం ext2_inode_large *) iptr,
			       0, నిడివి);
#అయితే_ముగింపు

	/* Update the inode cache bookkeeping */
	ఒకవేళ (!fail_csum) {
		fs->icache->cache_చివరి = cache_స్థానం;
		fs->icache->cache[cache_స్థానం].ino = ino;
	}
	జ్ఞాపకం_నకలు(inode, iptr, (bufsize > నిడివి) ? నిడివి : bufsize);

	ఒకవేళ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		తిరిగి EXT2_ET_INODE_CSUM_INVALID;

	తిరిగి 0;
}

errcode_t ext2fs_read_inode_నిండిన(ext2_filsys fs, ext2_ino_t ino,
				 నిర్మాణం ext2_inode * inode, పూర్ణాంకం bufsize)
{
	తిరిగి ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    నిర్మాణం ext2_inode * inode)
{
	తిరిగి ext2fs_read_inode2(fs, ino, inode,
				  పరిమాణం(నిర్మాణం ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      నిర్మాణం ext2_inode * inode, పూర్ణాంకం bufsize,
			      పూర్ణాంకం flags)
{
	blk64_t ఖండం_nr;
	dgrp_t సమూహం;
	గుర్తులేని దీర్ఘ ఖండం, స్థానభ్రంశం;
	errcode_t retval = 0;
	నిర్మాణం ext2_inode_large *w_inode;
	అక్షరం *ptr;
	గుర్తులేని i;
	పూర్ణాంకం clen;
	పూర్ణాంకం నిడివి = EXT2_INODE_కొలత(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super))
		తిరిగి EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	ఒకవేళ (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		ఒకవేళ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			తిరిగి retval;
	}

	ఒకవేళ ((ino == 0) || (ino > fs->super->s_inodes_లెక్క))
		తిరిగి EXT2_ET_BAD_INODE_అంకె;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(నిడివి, &w_inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (bufsize < నిడివి) {
		retval = ext2fs_read_inode2(fs, ino,
					    (నిర్మాణం ext2_inode *)w_inode,
					    నిడివి, READ_INODE_NOCSUM);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}

	/* Check to see if the inode cache needs to be updated */
	ఒకవేళ (fs->icache) {
		కొరకు (i=0; i < fs->icache->cache_కొలత; i++) {
			ఒకవేళ (fs->icache->cache[i].ino == ino) {
				జ్ఞాపకం_నకలు(fs->icache->cache[i].inode, inode,
				       (bufsize > నిడివి) ? నిడివి : bufsize);
				ఆపు;
			}
		}
	} లేకపోతే {
		retval = ext2fs_create_inode_cache(fs, 4);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}
	జ్ఞాపకం_నకలు(w_inode, inode, (bufsize > నిడివి) ? నిడివి : bufsize);

	ఒకవేళ (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		వెళ్ళు errout;
	}

#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_మార్పిడి_inode_నిండిన(fs, w_inode, w_inode, 1, నిడివి);
#అయితే_ముగింపు

	ఒకవేళ ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_సమితి(fs, ino, w_inode);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}

	సమూహం = (ino - 1) / EXT2_INODES_చొప్పున_సమూహం(fs->super);
	స్థానభ్రంశం = ((ino - 1) % EXT2_INODES_చొప్పున_సమూహం(fs->super)) *
		EXT2_INODE_కొలత(fs->super);
	ఖండం = స్థానభ్రంశం >> EXT2_ఖండం_కొలత_బిట్లు(fs->super);
	ఖండం_nr = ext2fs_inode_పట్టిక_loc(fs, (గుర్తులేని) సమూహం);
	ఒకవేళ (!ఖండం_nr) {
		retval = EXT2_ET_MISSING_INODE_పట్టిక;
		వెళ్ళు errout;
	}
	ఒకవేళ ((ఖండం_nr < fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
	    (ఖండం_nr + fs->inode_ఖండాలు_చొప్పున_సమూహం - 1 >=
	     ext2fs_ఖండాలు_లెక్క(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_పట్టిక;
		వెళ్ళు errout;
	}
	ఖండం_nr += ఖండం;

	స్థానభ్రంశం &= (EXT2_ఖండం_కొలత(fs->super) - 1);

	ptr = (అక్షరం *) w_inode;

	అయితే (నిడివి) {
		clen = నిడివి;
		ఒకవేళ ((స్థానభ్రంశం + నిడివి) > fs->blocksize)
			clen = fs->blocksize - స్థానభ్రంశం;

		ఒకవేళ (fs->icache->బఫరు_blk != ఖండం_nr) {
			retval = io_channel_read_blk64(fs->io, ఖండం_nr, 1,
						     fs->icache->బఫరు);
			ఒకవేళ (retval)
				వెళ్ళు errout;
			fs->icache->బఫరు_blk = ఖండం_nr;
		}


		జ్ఞాపకం_నకలు((అక్షరం *) fs->icache->బఫరు + (గుర్తులేని) స్థానభ్రంశం,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, ఖండం_nr, 1,
					      fs->icache->బఫరు);
		ఒకవేళ (retval)
			వెళ్ళు errout;

		స్థానభ్రంశం = 0;
		ptr += clen;
		నిడివి -= clen;
		ఖండం_nr++;
	}

	fs->flags |= EXT2_FLAG_మారింది;
errout:
	ext2fs_free_mem(&w_inode);
	తిరిగి retval;
}

errcode_t ext2fs_write_inode_నిండిన(ext2_filsys fs, ext2_ino_t ino,
				  నిర్మాణం ext2_inode * inode, పూర్ణాంకం bufsize)
{
	తిరిగి ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     నిర్మాణం ext2_inode *inode)
{
	తిరిగి ext2fs_write_inode2(fs, ino, inode,
				   పరిమాణం(నిర్మాణం ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 నిర్మాణం ext2_inode *inode)
{
	నిర్మాణం ext2_inode	*buf;
	పూర్ణాంకం			కొలత = EXT2_INODE_కొలత(fs->super);
	నిర్మాణం ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fs_P_get_సమయం(fs);

	ఒకవేళ (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_సమితి(inode, i_atime, t);
	ఒకవేళ (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_సమితి(inode, i_ctime, t);
	ఒకవేళ (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_సమితి(inode, i_mtime, t);

	ఒకవేళ (కొలత == పరిమాణం(నిర్మాణం ext2_inode))
		తిరిగి ext2fs_write_inode_నిండిన(fs, ino, inode,
					       పరిమాణం(నిర్మాణం ext2_inode));

	buf = జ్ఞాపకం_ఇవ్వు(కొలత);
	ఒకవేళ (!buf)
		తిరిగి ENOMEM;

	memset(buf, 0, కొలత);
	*buf = *inode;

	large_inode = (నిర్మాణం ext2_inode_large *) buf;
	large_inode->i_extra_isize = పరిమాణం(నిర్మాణం ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_కొలత;
	ఒకవేళ (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_నిండిన(fs, ino, buf, కొలత);
	విడుదల(buf);
	తిరిగి retval;
}


errcode_t ext2fs_get_ఖండాలు(ext2_filsys fs, ext2_ino_t ino, blk_t *ఖండాలు)
{
	నిర్మాణం ext2_inode	inode;
	పూర్ణాంకం			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (ino > fs->super->s_inodes_లెక్క)
		తిరిగి EXT2_ET_BAD_INODE_అంకె;

	ఒకవేళ (fs->get_ఖండాలు) {
		ఒకవేళ (!(*fs->get_ఖండాలు)(fs, ino, ఖండాలు))
			తిరిగి 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;
	కొరకు (i=0; i < EXT2_N_ఖండాలు; i++)
		ఖండాలు[i] = inode.i_ఖండం[i];
	తిరిగి 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	నిర్మాణం	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (ino > fs->super->s_inodes_లెక్క)
		తిరిగి EXT2_ET_BAD_INODE_అంకె;

	ఒకవేళ (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		ఒకవేళ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			తిరిగి retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ (!LINUX_S_ISDIR(inode.i_బహుళకం))
		తిరిగి EXT2_ET_NO_DIRECTORY;
	తిరిగి 0;
}

