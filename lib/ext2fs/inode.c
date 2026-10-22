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
#د_که_پای
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#تعریف_کړه IBLOCK_حالت_CSUMS_OK	1
#تعریف_کړه IBLOCK_حالت_INSANE	2
#تعریف_کړه لټون_بلاک_حالت(لټون)	((لټون)->لنډمهاله_بفر + (لټون)->inode_اندازه)

بنية ext2_struct_inode_لټون {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_بلاک;
	dgrp_t			current_ډله;
	ext2_ino_t		inodes_کیڼ;
	blk_t			بلاکونه_کیڼ;
	dgrp_t			groups_کیڼ;
	blk_t			inode_بفر_بلاکونه;
	حرف *			inode_بفر;
	صحيح			inode_اندازه;
	حرف *			ptr;
	صحيح			بایټونه_کیڼ;
	حرف			*لنډمهاله_بفر;
	errcode_t		(*بشپړ_ډله)(ext2_filsys fs,
					      ext2_inode_لټون لټون,
					      dgrp_t ډله,
					      فراغ * priv_معلومات);
	فراغ *			بشپړ_ډله_معلومات;
	صحيح			bad_بلاک_ptr;
	صحيح			لټون_flags;
	صحيح			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	بینښه	i;

	إذا (!fs->icache)
		أرجع 0;

	لكل (i=0; i < fs->icache->cache_اندازه; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->بفر_blk = 0;
	أرجع 0;
}

/*
 * Free the inode cache structure
 */
فراغ ext2fs_free_inode_cache(بنية ext2_inode_cache *icache)
{
	بینښه i;

	إذا (--icache->refcount)
		أرجع;
	إذا (icache->بفر)
		ext2fs_free_mem(&icache->بفر);
	لكل (i = 0; i < icache->cache_اندازه; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	إذا (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->بفر_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, بینښه صحيح cache_اندازه)
{
	بینښه	i;
	errcode_t	retval;

	إذا (fs->icache)
		أرجع 0;
	retval = ext2fs_get_mem(حجم(بنية ext2_inode_cache), &fs->icache);
	إذا (retval)
		أرجع retval;

	memset(fs->icache, 0, حجم(بنية ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->بفر);
	إذا (retval)
		اذهب errout;

	fs->icache->بفر_blk = 0;
	fs->icache->cache_وروستی = -1;
	fs->icache->cache_اندازه = cache_اندازه;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_اندازه,
				  حجم(بنية ext2_inode_cache_ent),
				  &fs->icache->cache);
	إذا (retval)
		اذهب errout;

	لكل (i = 0; i < fs->icache->cache_اندازه; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_اندازه(fs->super),
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

errcode_t ext2fs_open_inode_لټون(ext2_filsys fs, صحيح بفر_بلاکونه,
				 ext2_inode_لټون *ret_لټون)
{
	ext2_inode_لټون	لټون;
	errcode_t	retval;
	errcode_t (*save_get_بلاکونه)(ext2_filsys f, ext2_ino_t ino, blk_t *بلاکونه);

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
		save_get_بلاکونه = fs->get_بلاکونه;
		fs->get_بلاکونه = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		إذا (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_بلاکونه = save_get_بلاکونه;
	}

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_inode_لټون), &لټون);
	إذا (retval)
		أرجع retval;
	memset(لټون, 0, حجم(بنية ext2_struct_inode_لټون));

	لټون->magic = EXT2_ET_MAGIC_INODE_لټون;
	لټون->fs = fs;
	لټون->inode_اندازه = EXT2_INODE_اندازه(fs->super);
	لټون->بایټونه_کیڼ = 0;
	لټون->current_ډله = 0;
	لټون->groups_کیڼ = fs->ډله_desc_شمېر - 1;
	لټون->inode_بفر_بلاکونه = بفر_بلاکونه ? بفر_بلاکونه :
				    EXT2_INODE_لټون_DEFAULT_بفر_بلاکونه;
	لټون->current_بلاک = ext2fs_inode_جدول_loc(لټون->fs,
						     لټون->current_ډله);
	إذا (لټون->current_بلاک &&
	    ((لټون->current_بلاک < fs->super->s_لومړی_معلومات_بلاک) ||
	     (لټون->current_بلاک + fs->inode_بلاکونه_فی_ډله - 1 >=
	      ext2fs_بلاکونه_شمېر(fs->super)))) {
		ext2fs_free_mem(&لټون);
		أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
	}

	لټون->inodes_کیڼ = EXT2_INODES_فی_ډله(لټون->fs->super);
	لټون->بلاکونه_کیڼ = لټون->fs->inode_بلاکونه_فی_ډله;
	إذا (ext2fs_has_ډله_desc_csum(fs)) {
		__u32 ناکارول_شوی = ext2fs_bg_itable_ناکارول_شوی(fs, لټون->current_ډله);
		إذا (لټون->inodes_کیڼ > ناکارول_شوی)
			لټون->inodes_کیڼ -= ناکارول_شوی;
		وإلا
			لټون->inodes_کیڼ = 0;
		لټون->بلاکونه_کیڼ =
			(لټون->inodes_کیڼ +
			 (fs->blocksize / لټون->inode_اندازه - 1)) *
			لټون->inode_اندازه / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, لټون->inode_بفر_بلاکونه,
				      &لټون->inode_بفر);
	لټون->بشپړ_ډله = 0;
	لټون->بشپړ_ډله_معلومات = 0;
	لټون->bad_بلاک_ptr = 0;
	إذا (retval) {
		ext2fs_free_mem(&لټون);
		أرجع retval;
	}
	retval = ext2fs_get_mem(لټون->inode_اندازه + لټون->inode_بفر_بلاکونه,
				&لټون->لنډمهاله_بفر);
	إذا (retval) {
		ext2fs_free_mem(&لټون->inode_بفر);
		ext2fs_free_mem(&لټون);
		أرجع retval;
	}
	memset(لټون_بلاک_حالت(لټون), 0, لټون->inode_بفر_بلاکونه);
	إذا (لټون->fs->badblocks && لټون->fs->badblocks->عدد)
		لټون->لټون_flags |= EXT2_SF_CHK_BADBLOCKS;
	إذا (ext2fs_has_ډله_desc_csum(fs))
		لټون->لټون_flags |= EXT2_SF_DO_LAZY;
	*ret_لټون = لټون;
	أرجع 0;
}

فراغ ext2fs_close_inode_لټون(ext2_inode_لټون لټون)
{
	إذا (!لټون || (لټون->magic != EXT2_ET_MAGIC_INODE_لټون))
		أرجع;

	ext2fs_free_mem(&لټون->inode_بفر);
	لټون->inode_بفر = عدم;
	ext2fs_free_mem(&لټون->لنډمهاله_بفر);
	لټون->لنډمهاله_بفر = عدم;
	ext2fs_free_mem(&لټون);
	أرجع;
}

فراغ ext2fs_سېټ_inode_callback(ext2_inode_لټون لټون,
			       errcode_t (*بشپړ_ډله)(ext2_filsys fs,
						       ext2_inode_لټون لټون,
						       dgrp_t ډله,
						       فراغ * priv_معلومات),
			       فراغ *بشپړ_ډله_معلومات)
{
	إذا (!لټون || (لټون->magic != EXT2_ET_MAGIC_INODE_لټون))
		أرجع;

	لټون->بشپړ_ډله = بشپړ_ډله;
	لټون->بشپړ_ډله_معلومات = بشپړ_ډله_معلومات;
}

صحيح ext2fs_inode_لټون_flags(ext2_inode_لټون لټون, صحيح سېټ_flags,
			    صحيح clear_flags)
{
	صحيح	old_flags;

	إذا (!لټون || (لټون->magic != EXT2_ET_MAGIC_INODE_لټون))
		أرجع 0;

	old_flags = لټون->لټون_flags;
	لټون->لټون_flags &= ~clear_flags;
	لټون->لټون_flags |= سېټ_flags;
	أرجع old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ساكن errcode_t get_بل_blockgroup(ext2_inode_لټون لټون)
{
	ext2_filsys fs = لټون->fs;

	لټون->current_ډله++;
	لټون->groups_کیڼ--;

	لټون->current_بلاک = ext2fs_inode_جدول_loc(لټون->fs,
						     لټون->current_ډله);
	لټون->current_inode = لټون->current_ډله *
		EXT2_INODES_فی_ډله(fs->super);

	لټون->بایټونه_کیڼ = 0;
	لټون->inodes_کیڼ = EXT2_INODES_فی_ډله(fs->super);
	لټون->بلاکونه_کیڼ = fs->inode_بلاکونه_فی_ډله;
	إذا (ext2fs_has_ډله_desc_csum(fs)) {
		__u32 ناکارول_شوی = ext2fs_bg_itable_ناکارول_شوی(fs, لټون->current_ډله);
		إذا (لټون->inodes_کیڼ > ناکارول_شوی)
			لټون->inodes_کیڼ -= ناکارول_شوی;
		وإلا
			لټون->inodes_کیڼ = 0;
		لټون->بلاکونه_کیڼ =
			(لټون->inodes_کیڼ +
			 (fs->blocksize / لټون->inode_اندازه - 1)) *
			لټون->inode_اندازه / fs->blocksize;
	}
	إذا (لټون->current_بلاک &&
	    ((لټون->current_بلاک < fs->super->s_لومړی_معلومات_بلاک) ||
	     (لټون->current_بلاک + fs->inode_بلاکونه_فی_ډله - 1 >=
	      ext2fs_بلاکونه_شمېر(fs->super))))
		أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
	أرجع 0;
}

errcode_t ext2fs_inode_لټون_goto_blockgroup(ext2_inode_لټون لټون,
					    صحيح	ډله)
{
	لټون->current_ډله = ډله - 1;
	لټون->groups_کیڼ = لټون->fs->ډله_desc_شمېر - ډله;
	لټون->bad_بلاک_ptr = 0;
	أرجع get_بل_blockgroup(لټون);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ساكن errcode_t check_for_inode_bad_بلاکونه(ext2_inode_لټون لټون,
					    blk64_t *عدد_بلاکونه)
{
	blk64_t	blk = لټون->current_بلاک;
	badblocks_list	bb = لټون->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	إذا (blk == 0)
		أرجع 0;

	/* Make sure bad_block_ptr is still valid */
	إذا (لټون->bad_بلاک_ptr >= bb->عدد) {
		لټون->لټون_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	طالما (blk > bb->list[لټون->bad_بلاک_ptr]) {
		إذا (++لټون->bad_بلاک_ptr >= bb->عدد) {
			لټون->لټون_flags &= ~EXT2_SF_CHK_BADBLOCKS;
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
	إذا (blk == bb->list[لټون->bad_بلاک_ptr]) {
		لټون->لټون_flags |= EXT2_SF_BAD_INODE_BLK;
		*عدد_بلاکونه = 1;
		إذا (++لټون->bad_بلاک_ptr >= bb->عدد)
			لټون->لټون_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	إذا ((blk + *عدد_بلاکونه) > bb->list[لټون->bad_بلاک_ptr])
		*عدد_بلاکونه = (صحيح) (bb->list[لټون->bad_بلاک_ptr] - blk);

	أرجع 0;
}

ساكن صحيح بلاک_map_looks_insane(ext2_filsys fs,
				  بنية ext2_inode_large *inode)
{
	بینښه صحيح i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	إذا ((inode->i_flags & EXT4_INLINE_معلومات_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		أرجع 0;
	إذا (!LINUX_S_ISREG(inode->i_ډېرپېښ) &&
	    !LINUX_S_ISLNK(inode->i_ډېرپېښ) &&
	    !LINUX_S_ISDIR(inode->i_ډېرپېښ))
		أرجع 0;
	إذا (LINUX_S_ISLNK(inode->i_ډېرپېښ) &&
	    EXT2_I_اندازه(inode) <= حجم(inode->i_بلاک))
		أرجع 0;

	/* Unused inodes probably aren't insane */
	إذا (inode->i_links_شمېر == 0)
		أرجع 0;

	/* See if more than half the block maps are insane */
	لكل (i = 0, bad = 0; i < EXT2_N_بلاکونه; i++)
		إذا (inode->i_بلاک[i] != 0 &&
		    (inode->i_بلاک[i] < fs->super->s_لومړی_معلومات_بلاک ||
		     inode->i_بلاک[i] >= ext2fs_بلاکونه_شمېر(fs->super)))
			bad++;
	أرجع bad > EXT2_N_بلاکونه / 2;
}

ساكن صحيح extent_سر_looks_insane(بنية ext2_inode_large *inode)
{
	إذا (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_سرلیک_verify(inode->i_بلاک,
					حجم(inode->i_بلاک)) == 0)
		أرجع 0;
	أرجع 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ساكن فراغ check_inode_بلاک_sanity(ext2_inode_لټون لټون, blk64_t عدد_بلاکونه)
{
	ext2_ino_t	ino, inodes_منزل_لټون;
	بینښه صحيح	badness, checksum_failures;
	بینښه صحيح	inodes_دننه_buf, inodes_فی_بلاک;
	حرف		*p;
	بنية ext2_inode_large *inode;
	حرف		*بلاک_حالت;
	بینښه صحيح	blk, bad_csum;

	إذا (!(لټون->لټون_flags & EXT2_SF_WARN_GARBAGE_INODES))
		أرجع;

	inodes_منزل_لټون = لټون->inodes_کیڼ;
	inodes_دننه_buf = عدد_بلاکونه * لټون->fs->blocksize / لټون->inode_اندازه;
	إذا (inodes_منزل_لټون > inodes_دننه_buf)
		inodes_منزل_لټون = inodes_دننه_buf;

	p = (حرف *) لټون->inode_بفر;
	ino = لټون->current_inode + 1;
	checksum_failures = badness = 0;
	بلاک_حالت = لټون_بلاک_حالت(لټون);
	memset(بلاک_حالت, 0, لټون->inode_بفر_بلاکونه);
	inodes_فی_بلاک = EXT2_INODES_فی_بلاک(لټون->fs->super);

	إذا (inodes_فی_بلاک < 2)
		أرجع;

#که_تعریف_شوی ټکي_BIGENDIAN
	إذا (ext2fs_get_mem(EXT2_INODE_اندازه(لټون->fs->super), &inode))
		أرجع;
#د_که_پای

	طالما (inodes_منزل_لټون > 0) {
		blk = (p - (حرف *)لټون->inode_بفر) / لټون->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(لټون->fs, ino,
				(بنية ext2_inode_large *) p) == 0;

#که_تعریف_شوی ټکي_BIGENDIAN
		ext2fs_بدلون_inode_ډک(لټون->fs,
			       (بنية ext2_inode_large *) inode,
			       (بنية ext2_inode_large *) p,
			       0, EXT2_INODE_اندازه(لټون->fs->super));
#وإلا
		inode = (بنية ext2_inode_large *) p;
#د_که_پای

		/* Is this inode insane? */
		إذا (bad_csum) {
			checksum_failures++;
			badness++;
		} وإلا إذا (extent_سر_looks_insane(inode) ||
			   بلاک_map_looks_insane(لټون->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		إذا (badness > inodes_فی_بلاک / 2) {
			بینښه صحيح ino_adj;

			بلاک_حالت[blk] |= IBLOCK_حالت_INSANE;
			ino_adj = inodes_فی_بلاک -
						((ino - 1) % inodes_فی_بلاک);
			إذا (ino_adj > inodes_منزل_لټون)
				ino_adj = inodes_منزل_لټون;
			inodes_منزل_لټون -= ino_adj;
			p += لټون->inode_اندازه * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			استمر;
		}

		إذا ((ino % inodes_فی_بلاک) == 0) {
			إذا (checksum_failures == 0)
				بلاک_حالت[blk] |= IBLOCK_حالت_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_منزل_لټون--;
		p += لټون->inode_اندازه;
		ino++;
	};

#که_تعریف_شوی ټکي_BIGENDIAN
	ext2fs_free_mem(&inode);
#د_که_پای
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ساكن errcode_t get_بل_بلاکونه(ext2_inode_لټون لټون)
{
	blk64_t		عدد_بلاکونه;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	عدد_بلاکونه = لټون->inode_بفر_بلاکونه;
	إذا (عدد_بلاکونه > لټون->بلاکونه_کیڼ)
		عدد_بلاکونه = لټون->بلاکونه_کیڼ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	إذا (لټون->لټون_flags & EXT2_SF_BAD_INODE_BLK) {
		إذا (لټون->بایټونه_کیڼ)
			لټون->لټون_flags |= EXT2_SF_BAD_EXTRA_بایټونه;
		لټون->لټون_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	إذا (لټون->لټون_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_بلاکونه(لټون, &عدد_بلاکونه);
		إذا (retval)
			أرجع retval;
	}

	إذا ((لټون->لټون_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (لټون->current_بلاک == 0)) {
		memset(لټون->inode_بفر, 0,
		       (size_t) عدد_بلاکونه * لټون->fs->blocksize);
	} وإلا {
		retval = io_channel_read_blk64(لټون->fs->io,
					     لټون->current_بلاک,
					     (صحيح) عدد_بلاکونه,
					     لټون->inode_بفر);
		إذا (retval)
			أرجع EXT2_ET_بل_INODE_READ;
	}
	check_inode_بلاک_sanity(لټون, عدد_بلاکونه);

	لټون->ptr = لټون->inode_بفر;
	لټون->بایټونه_کیڼ = عدد_بلاکونه * لټون->fs->blocksize;

	لټون->بلاکونه_کیڼ -= عدد_بلاکونه;
	إذا (لټون->current_بلاک)
		لټون->current_بلاک += عدد_بلاکونه;

	أرجع 0;
}

#إذا 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ساكن مضمن صحيح is_تشه_لټون(ext2_inode_لټون لټون)
{
	صحيح	i;

	إذا (لټون->بایټونه_کیڼ == 0)
		أرجع 0;

	لكل (i=0; i < لټون->بایټونه_کیڼ; i++)
		إذا (لټون->ptr[i])
			أرجع 0;
	أرجع 1;
}
#د_که_پای

errcode_t ext2fs_get_بل_inode_ډک(ext2_inode_لټون لټون, ext2_ino_t *ino,
				     بنية ext2_inode *inode, صحيح bufsize)
{
	errcode_t	retval;
	صحيح		extra_بایټونه = 0;
	صحيح		اوږدتیا;
	بنية ext2_inode_large	*iptr = (بنية ext2_inode_large *)inode;
	حرف		*iblock_حالت;
	بینښه صحيح	iblk;

	EXT2_CHECK_MAGIC(لټون, EXT2_ET_MAGIC_INODE_لټون);
	اوږدتیا = EXT2_INODE_اندازه(لټون->fs->super);
	iblock_حالت = لټون_بلاک_حالت(لټون);

	/*
	 * Do we need to start reading a new block group?
	 */
	إذا (لټون->inodes_کیڼ <= 0) {
	force_new_ډله:
		إذا (لټون->بشپړ_ډله) {
			retval = (لټون->بشپړ_ډله)
				(لټون->fs, لټون, لټون->current_ډله,
				 لټون->بشپړ_ډله_معلومات);
			إذا (retval)
				أرجع retval;
		}
		إذا (لټون->groups_کیڼ <= 0) {
			*ino = 0;
			أرجع 0;
		}
		retval = get_بل_blockgroup(لټون);
		إذا (retval)
			أرجع retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	إذا ((لټون->لټون_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(لټون->fs, لټون->current_ډله, EXT2_BG_INODE_UNINIT)
	     ))
		اذهب force_new_ډله;
	إذا (لټون->inodes_کیڼ == 0)
		اذهب force_new_ډله;
	إذا (لټون->current_بلاک == 0) {
		إذا (لټون->لټون_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			اذهب force_new_ډله;
		} وإلا
			أرجع EXT2_ET_MISSING_INODE_جدول;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	إذا (لټون->بایټونه_کیڼ < لټون->inode_اندازه) {
		إذا (لټون->بایټونه_کیڼ)
			حافظه_نقل(لټون->لنډمهاله_بفر, لټون->ptr, لټون->بایټونه_کیڼ);
		extra_بایټونه = لټون->بایټونه_کیڼ;

		retval = get_بل_بلاکونه(لټون);
		إذا (retval)
			أرجع retval;
#إذا 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		إذا (is_تشه_لټون(لټون))
			اذهب force_new_ډله;
#د_که_پای
	}

	إذا (bufsize < اوږدتیا) {
		retval = ext2fs_get_mem(اوږدتیا, &iptr);
		إذا (retval)
			أرجع retval;
	}

	retval = 0;
	iblk = لټون->current_inode % EXT2_INODES_فی_ډله(لټون->fs->super) /
				EXT2_INODES_فی_بلاک(لټون->fs->super) %
				لټون->inode_بفر_بلاکونه;
	إذا (extra_بایټونه) {
		حافظه_نقل(لټون->لنډمهاله_بفر+extra_بایټونه, لټون->ptr,
		       لټون->inode_اندازه - extra_بایټونه);
		لټون->ptr += لټون->inode_اندازه - extra_بایټونه;
		لټون->بایټونه_کیڼ -= لټون->inode_اندازه - extra_بایټونه;

		/* Verify the inode checksum. */
		إذا (!(iblock_حالت[iblk] & IBLOCK_حالت_CSUMS_OK) &&
		    !(لټون->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(لټون->fs, لټون->current_inode + 1,
				(بنية ext2_inode_large *)لټون->لنډمهاله_بفر))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#که_تعریف_شوی ټکي_BIGENDIAN
		memset(iptr, 0, اوږدتیا);
		ext2fs_بدلون_inode_ډک(لټون->fs,
			       (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) لټون->لنډمهاله_بفر,
			       0, اوږدتیا);
#وإلا
		حافظه_نقل(iptr, لټون->لنډمهاله_بفر, اوږدتیا);
#د_که_پای
		إذا (لټون->لټون_flags & EXT2_SF_BAD_EXTRA_بایټونه)
			retval = EXT2_ET_BAD_بلاک_دننه_INODE_جدول;
		لټون->لټون_flags &= ~EXT2_SF_BAD_EXTRA_بایټونه;
	} وإلا {
		/* Verify the inode checksum. */
		إذا (!(iblock_حالت[iblk] & IBLOCK_حالت_CSUMS_OK) &&
		    !(لټون->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(لټون->fs, لټون->current_inode + 1,
				(بنية ext2_inode_large *)لټون->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#که_تعریف_شوی ټکي_BIGENDIAN
		memset(iptr, 0, اوږدتیا);
		ext2fs_بدلون_inode_ډک(لټون->fs,
				(بنية ext2_inode_large *) iptr,
				(بنية ext2_inode_large *) لټون->ptr,
				0, اوږدتیا);
#وإلا
		حافظه_نقل(iptr, لټون->ptr, اوږدتیا);
#د_که_پای
		لټون->ptr += لټون->inode_اندازه;
		لټون->بایټونه_کیڼ -= لټون->inode_اندازه;
		إذا (لټون->لټون_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_بلاک_دننه_INODE_جدول;
	}
	إذا ((iblock_حالت[iblk] & IBLOCK_حالت_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	لټون->inodes_کیڼ--;
	لټون->current_inode++;
	*ino = لټون->current_inode;
	إذا (iptr != (بنية ext2_inode_large *)inode) {
		حافظه_نقل(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	أرجع retval;
}

errcode_t ext2fs_get_بل_inode(ext2_inode_لټون لټون, ext2_ino_t *ino,
				بنية ext2_inode *inode)
{
	أرجع ext2fs_get_بل_inode_ډک(لټون, ino, inode,
						حجم(بنية ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode * inode, صحيح bufsize,
			     صحيح flags)
{
	blk64_t		بلاک_nr;
	dgrp_t		ډله;
	بینښه اوږد 	بلاک, دننه_واټن;
	حرف 		*ptr;
	errcode_t	retval;
	بینښه	i;
	صحيح		clen, inodes_فی_بلاک;
	io_channel	io;
	صحيح		اوږدتیا = EXT2_INODE_اندازه(fs->super);
	بنية ext2_inode_large	*iptr;
	صحيح		cache_خونه, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	إذا (fs->read_inode &&
	    ((bufsize == حجم(بنية ext2_inode)) ||
	     (EXT2_INODE_اندازه(fs->super) == حجم(بنية ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}
	إذا ((ino == 0) || (ino > fs->super->s_inodes_شمېر))
		أرجع EXT2_ET_BAD_INODE_عدد;
	/* Create inode cache if not present */
	إذا (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		إذا (retval)
			أرجع retval;
	}
	/* Check to see if it's in the inode cache */
	لكل (i = 0; i < fs->icache->cache_اندازه; i++) {
		إذا (fs->icache->cache[i].ino == ino) {
			حافظه_نقل(inode, fs->icache->cache[i].inode,
			       (bufsize > اوږدتیا) ? اوږدتیا : bufsize);
			أرجع 0;
		}
	}
	إذا (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_فی_بلاک = fs->blocksize / EXT2_INODE_اندازه(fs->super);
		بلاک_nr = ext2fs_le32_منزل_cpu(fs->image_سرلیک->دننه_واټن_inode) / fs->blocksize;
		بلاک_nr += (ino - 1) / inodes_فی_بلاک;
		دننه_واټن = ((ino - 1) % inodes_فی_بلاک) *
			EXT2_INODE_اندازه(fs->super);
		io = fs->image_io;
	} وإلا {
		ډله = (ino - 1) / EXT2_INODES_فی_ډله(fs->super);
		إذا (ډله > fs->ډله_desc_شمېر)
			أرجع EXT2_ET_BAD_INODE_عدد;
		دننه_واټن = ((ino - 1) % EXT2_INODES_فی_ډله(fs->super)) *
			EXT2_INODE_اندازه(fs->super);
		بلاک = دننه_واټن >> EXT2_بلاک_اندازه_بټونه(fs->super);
		بلاک_nr = ext2fs_inode_جدول_loc(fs, ډله);
		إذا (!بلاک_nr)
			أرجع EXT2_ET_MISSING_INODE_جدول;
		إذا ((بلاک_nr < fs->super->s_لومړی_معلومات_بلاک) ||
		    (بلاک_nr + fs->inode_بلاکونه_فی_ډله - 1 >=
		     ext2fs_بلاکونه_شمېر(fs->super)))
			أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
		بلاک_nr += بلاک;
		io = fs->io;
	}
	دننه_واټن &= (EXT2_بلاک_اندازه(fs->super) - 1);

	cache_خونه = (fs->icache->cache_وروستی + 1) % fs->icache->cache_اندازه;
	iptr = (بنية ext2_inode_large *)fs->icache->cache[cache_خونه].inode;

	ptr = (حرف *) iptr;
	طالما (اوږدتیا) {
		clen = اوږدتیا;
		إذا ((دننه_واټن + اوږدتیا) > fs->blocksize)
			clen = fs->blocksize - دننه_واټن;

		إذا (بلاک_nr != fs->icache->بفر_blk) {
			retval = io_channel_read_blk64(io, بلاک_nr, 1,
						     fs->icache->بفر);
			إذا (retval)
				أرجع retval;
			fs->icache->بفر_blk = بلاک_nr;
		}

		حافظه_نقل(ptr, ((حرف *) fs->icache->بفر) + (بینښه) دننه_واټن,
		       clen);

		دننه_واټن = 0;
		اوږدتیا -= clen;
		ptr += clen;
		بلاک_nr++;
	}
	اوږدتیا = EXT2_INODE_اندازه(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#که_تعریف_شوی ټکي_BIGENDIAN
	ext2fs_بدلون_inode_ډک(fs, (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) iptr,
			       0, اوږدتیا);
#د_که_پای

	/* Update the inode cache bookkeeping */
	إذا (!fail_csum) {
		fs->icache->cache_وروستی = cache_خونه;
		fs->icache->cache[cache_خونه].ino = ino;
	}
	حافظه_نقل(inode, iptr, (bufsize > اوږدتیا) ? اوږدتیا : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		أرجع EXT2_ET_INODE_CSUM_INVALID;

	أرجع 0;
}

errcode_t ext2fs_read_inode_ډک(ext2_filsys fs, ext2_ino_t ino,
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
	blk64_t بلاک_nr;
	dgrp_t ډله;
	بینښه اوږد بلاک, دننه_واټن;
	errcode_t retval = 0;
	بنية ext2_inode_large *w_inode;
	حرف *ptr;
	بینښه i;
	صحيح clen;
	صحيح اوږدتیا = EXT2_INODE_اندازه(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	إذا (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}

	إذا ((ino == 0) || (ino > fs->super->s_inodes_شمېر))
		أرجع EXT2_ET_BAD_INODE_عدد;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(اوږدتیا, &w_inode);
	إذا (retval)
		أرجع retval;

	إذا (bufsize < اوږدتیا) {
		retval = ext2fs_read_inode2(fs, ino,
					    (بنية ext2_inode *)w_inode,
					    اوږدتیا, READ_INODE_NOCSUM);
		إذا (retval)
			اذهب errout;
	}

	/* Check to see if the inode cache needs to be updated */
	إذا (fs->icache) {
		لكل (i=0; i < fs->icache->cache_اندازه; i++) {
			إذا (fs->icache->cache[i].ino == ino) {
				حافظه_نقل(fs->icache->cache[i].inode, inode,
				       (bufsize > اوږدتیا) ? اوږدتیا : bufsize);
				اكسر;
			}
		}
	} وإلا {
		retval = ext2fs_create_inode_cache(fs, 4);
		إذا (retval)
			اذهب errout;
	}
	حافظه_نقل(w_inode, inode, (bufsize > اوږدتیا) ? اوږدتیا : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		اذهب errout;
	}

#که_تعریف_شوی ټکي_BIGENDIAN
	ext2fs_بدلون_inode_ډک(fs, w_inode, w_inode, 1, اوږدتیا);
#د_که_پای

	إذا ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_سېټ(fs, ino, w_inode);
		إذا (retval)
			اذهب errout;
	}

	ډله = (ino - 1) / EXT2_INODES_فی_ډله(fs->super);
	دننه_واټن = ((ino - 1) % EXT2_INODES_فی_ډله(fs->super)) *
		EXT2_INODE_اندازه(fs->super);
	بلاک = دننه_واټن >> EXT2_بلاک_اندازه_بټونه(fs->super);
	بلاک_nr = ext2fs_inode_جدول_loc(fs, (بینښه) ډله);
	إذا (!بلاک_nr) {
		retval = EXT2_ET_MISSING_INODE_جدول;
		اذهب errout;
	}
	إذا ((بلاک_nr < fs->super->s_لومړی_معلومات_بلاک) ||
	    (بلاک_nr + fs->inode_بلاکونه_فی_ډله - 1 >=
	     ext2fs_بلاکونه_شمېر(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_جدول;
		اذهب errout;
	}
	بلاک_nr += بلاک;

	دننه_واټن &= (EXT2_بلاک_اندازه(fs->super) - 1);

	ptr = (حرف *) w_inode;

	طالما (اوږدتیا) {
		clen = اوږدتیا;
		إذا ((دننه_واټن + اوږدتیا) > fs->blocksize)
			clen = fs->blocksize - دننه_واټن;

		إذا (fs->icache->بفر_blk != بلاک_nr) {
			retval = io_channel_read_blk64(fs->io, بلاک_nr, 1,
						     fs->icache->بفر);
			إذا (retval)
				اذهب errout;
			fs->icache->بفر_blk = بلاک_nr;
		}


		حافظه_نقل((حرف *) fs->icache->بفر + (بینښه) دننه_واټن,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, بلاک_nr, 1,
					      fs->icache->بفر);
		إذا (retval)
			اذهب errout;

		دننه_واټن = 0;
		ptr += clen;
		اوږدتیا -= clen;
		بلاک_nr++;
	}

	fs->flags |= EXT2_FLAG_بدل_شوی;
errout:
	ext2fs_free_mem(&w_inode);
	أرجع retval;
}

errcode_t ext2fs_write_inode_ډک(ext2_filsys fs, ext2_ino_t ino,
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
	صحيح			اندازه = EXT2_INODE_اندازه(fs->super);
	بنية ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fs_P_get_وخت(fs);

	إذا (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_سېټ(inode, i_atime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_سېټ(inode, i_ctime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_سېټ(inode, i_mtime, t);

	إذا (اندازه == حجم(بنية ext2_inode))
		أرجع ext2fs_write_inode_ډک(fs, ino, inode,
					       حجم(بنية ext2_inode));

	buf = حافظه_ورکړه(اندازه);
	إذا (!buf)
		أرجع ENOMEM;

	memset(buf, 0, اندازه);
	*buf = *inode;

	large_inode = (بنية ext2_inode_large *) buf;
	large_inode->i_extra_isize = حجم(بنية ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_اندازه;
	إذا (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_ډک(fs, ino, buf, اندازه);
	حرر(buf);
	أرجع retval;
}


errcode_t ext2fs_get_بلاکونه(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاکونه)
{
	بنية ext2_inode	inode;
	صحيح			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ino > fs->super->s_inodes_شمېر)
		أرجع EXT2_ET_BAD_INODE_عدد;

	إذا (fs->get_بلاکونه) {
		إذا (!(*fs->get_بلاکونه)(fs, ino, بلاکونه))
			أرجع 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	لكل (i=0; i < EXT2_N_بلاکونه; i++)
		بلاکونه[i] = inode.i_بلاک[i];
	أرجع 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	بنية	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ino > fs->super->s_inodes_شمېر)
		أرجع EXT2_ET_BAD_INODE_عدد;

	إذا (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	إذا (!LINUX_S_ISDIR(inode.i_ډېرپېښ))
		أرجع EXT2_ET_NO_DIRECTORY;
	أرجع 0;
}

