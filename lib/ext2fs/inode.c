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
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#تعریف_کن IBLOCK_وضعیت_CSUMS_OK	1
#تعریف_کن IBLOCK_وضعیت_INSANE	2
#تعریف_کن پیمایش_بلوک_وضعیت(پیمایش)	((پیمایش)->موقت_میانگیر + (پیمایش)->inode_اندازه)

بنية ext2_struct_inode_پیمایش {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_بلوک;
	dgrp_t			current_گروه;
	ext2_ino_t		inodes_چپ;
	blk_t			بلوکها_چپ;
	dgrp_t			groups_چپ;
	blk_t			inode_میانگیر_بلوکها;
	حرف *			inode_میانگیر;
	صحيح			inode_اندازه;
	حرف *			ptr;
	صحيح			بایتها_چپ;
	حرف			*موقت_میانگیر;
	errcode_t		(*انجامشده_گروه)(ext2_filsys fs,
					      ext2_inode_پیمایش پیمایش,
					      dgrp_t گروه,
					      فراغ * priv_داده);
	فراغ *			انجامشده_گروه_داده;
	صحيح			bad_بلوک_ptr;
	صحيح			پیمایش_flags;
	صحيح			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	بیعلامت	i;

	إذا (!fs->icache)
		أرجع 0;

	لكل (i=0; i < fs->icache->cache_اندازه; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->میانگیر_blk = 0;
	أرجع 0;
}

/*
 * Free the inode cache structure
 */
فراغ ext2fs_free_inode_cache(بنية ext2_inode_cache *icache)
{
	بیعلامت i;

	إذا (--icache->refcount)
		أرجع;
	إذا (icache->میانگیر)
		ext2fs_free_mem(&icache->میانگیر);
	لكل (i = 0; i < icache->cache_اندازه; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	إذا (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->میانگیر_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, بیعلامت صحيح cache_اندازه)
{
	بیعلامت	i;
	errcode_t	retval;

	إذا (fs->icache)
		أرجع 0;
	retval = ext2fs_get_mem(حجم(بنية ext2_inode_cache), &fs->icache);
	إذا (retval)
		أرجع retval;

	memset(fs->icache, 0, حجم(بنية ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->میانگیر);
	إذا (retval)
		اذهب errout;

	fs->icache->میانگیر_blk = 0;
	fs->icache->cache_آخرین = -1;
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

errcode_t ext2fs_open_inode_پیمایش(ext2_filsys fs, صحيح میانگیر_بلوکها,
				 ext2_inode_پیمایش *ret_پیمایش)
{
	ext2_inode_پیمایش	پیمایش;
	errcode_t	retval;
	errcode_t (*save_get_بلوکها)(ext2_filsys f, ext2_ino_t ino, blk_t *بلوکها);

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
		save_get_بلوکها = fs->get_بلوکها;
		fs->get_بلوکها = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		إذا (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_بلوکها = save_get_بلوکها;
	}

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_inode_پیمایش), &پیمایش);
	إذا (retval)
		أرجع retval;
	memset(پیمایش, 0, حجم(بنية ext2_struct_inode_پیمایش));

	پیمایش->magic = EXT2_ET_MAGIC_INODE_پیمایش;
	پیمایش->fs = fs;
	پیمایش->inode_اندازه = EXT2_INODE_اندازه(fs->super);
	پیمایش->بایتها_چپ = 0;
	پیمایش->current_گروه = 0;
	پیمایش->groups_چپ = fs->گروه_desc_شمار - 1;
	پیمایش->inode_میانگیر_بلوکها = میانگیر_بلوکها ? میانگیر_بلوکها :
				    EXT2_INODE_پیمایش_DEFAULT_میانگیر_بلوکها;
	پیمایش->current_بلوک = ext2fs_inode_جدول_loc(پیمایش->fs,
						     پیمایش->current_گروه);
	إذا (پیمایش->current_بلوک &&
	    ((پیمایش->current_بلوک < fs->super->s_اولی_داده_بلوک) ||
	     (پیمایش->current_بلوک + fs->inode_بلوکها_در_هر_گروه - 1 >=
	      ext2fs_بلوکها_شمار(fs->super)))) {
		ext2fs_free_mem(&پیمایش);
		أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
	}

	پیمایش->inodes_چپ = EXT2_INODES_در_هر_گروه(پیمایش->fs->super);
	پیمایش->بلوکها_چپ = پیمایش->fs->inode_بلوکها_در_هر_گروه;
	إذا (ext2fs_has_گروه_desc_csum(fs)) {
		__u32 استفادهنشده = ext2fs_bg_itable_استفادهنشده(fs, پیمایش->current_گروه);
		إذا (پیمایش->inodes_چپ > استفادهنشده)
			پیمایش->inodes_چپ -= استفادهنشده;
		وإلا
			پیمایش->inodes_چپ = 0;
		پیمایش->بلوکها_چپ =
			(پیمایش->inodes_چپ +
			 (fs->blocksize / پیمایش->inode_اندازه - 1)) *
			پیمایش->inode_اندازه / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, پیمایش->inode_میانگیر_بلوکها,
				      &پیمایش->inode_میانگیر);
	پیمایش->انجامشده_گروه = 0;
	پیمایش->انجامشده_گروه_داده = 0;
	پیمایش->bad_بلوک_ptr = 0;
	إذا (retval) {
		ext2fs_free_mem(&پیمایش);
		أرجع retval;
	}
	retval = ext2fs_get_mem(پیمایش->inode_اندازه + پیمایش->inode_میانگیر_بلوکها,
				&پیمایش->موقت_میانگیر);
	إذا (retval) {
		ext2fs_free_mem(&پیمایش->inode_میانگیر);
		ext2fs_free_mem(&پیمایش);
		أرجع retval;
	}
	memset(پیمایش_بلوک_وضعیت(پیمایش), 0, پیمایش->inode_میانگیر_بلوکها);
	إذا (پیمایش->fs->badblocks && پیمایش->fs->badblocks->رقم)
		پیمایش->پیمایش_flags |= EXT2_SF_CHK_BADBLOCKS;
	إذا (ext2fs_has_گروه_desc_csum(fs))
		پیمایش->پیمایش_flags |= EXT2_SF_DO_LAZY;
	*ret_پیمایش = پیمایش;
	أرجع 0;
}

فراغ ext2fs_close_inode_پیمایش(ext2_inode_پیمایش پیمایش)
{
	إذا (!پیمایش || (پیمایش->magic != EXT2_ET_MAGIC_INODE_پیمایش))
		أرجع;

	ext2fs_free_mem(&پیمایش->inode_میانگیر);
	پیمایش->inode_میانگیر = عدم;
	ext2fs_free_mem(&پیمایش->موقت_میانگیر);
	پیمایش->موقت_میانگیر = عدم;
	ext2fs_free_mem(&پیمایش);
	أرجع;
}

فراغ ext2fs_مجموعه_inode_callback(ext2_inode_پیمایش پیمایش,
			       errcode_t (*انجامشده_گروه)(ext2_filsys fs,
						       ext2_inode_پیمایش پیمایش,
						       dgrp_t گروه,
						       فراغ * priv_داده),
			       فراغ *انجامشده_گروه_داده)
{
	إذا (!پیمایش || (پیمایش->magic != EXT2_ET_MAGIC_INODE_پیمایش))
		أرجع;

	پیمایش->انجامشده_گروه = انجامشده_گروه;
	پیمایش->انجامشده_گروه_داده = انجامشده_گروه_داده;
}

صحيح ext2fs_inode_پیمایش_flags(ext2_inode_پیمایش پیمایش, صحيح مجموعه_flags,
			    صحيح clear_flags)
{
	صحيح	old_flags;

	إذا (!پیمایش || (پیمایش->magic != EXT2_ET_MAGIC_INODE_پیمایش))
		أرجع 0;

	old_flags = پیمایش->پیمایش_flags;
	پیمایش->پیمایش_flags &= ~clear_flags;
	پیمایش->پیمایش_flags |= مجموعه_flags;
	أرجع old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ساكن errcode_t get_بعدی_blockgroup(ext2_inode_پیمایش پیمایش)
{
	ext2_filsys fs = پیمایش->fs;

	پیمایش->current_گروه++;
	پیمایش->groups_چپ--;

	پیمایش->current_بلوک = ext2fs_inode_جدول_loc(پیمایش->fs,
						     پیمایش->current_گروه);
	پیمایش->current_inode = پیمایش->current_گروه *
		EXT2_INODES_در_هر_گروه(fs->super);

	پیمایش->بایتها_چپ = 0;
	پیمایش->inodes_چپ = EXT2_INODES_در_هر_گروه(fs->super);
	پیمایش->بلوکها_چپ = fs->inode_بلوکها_در_هر_گروه;
	إذا (ext2fs_has_گروه_desc_csum(fs)) {
		__u32 استفادهنشده = ext2fs_bg_itable_استفادهنشده(fs, پیمایش->current_گروه);
		إذا (پیمایش->inodes_چپ > استفادهنشده)
			پیمایش->inodes_چپ -= استفادهنشده;
		وإلا
			پیمایش->inodes_چپ = 0;
		پیمایش->بلوکها_چپ =
			(پیمایش->inodes_چپ +
			 (fs->blocksize / پیمایش->inode_اندازه - 1)) *
			پیمایش->inode_اندازه / fs->blocksize;
	}
	إذا (پیمایش->current_بلوک &&
	    ((پیمایش->current_بلوک < fs->super->s_اولی_داده_بلوک) ||
	     (پیمایش->current_بلوک + fs->inode_بلوکها_در_هر_گروه - 1 >=
	      ext2fs_بلوکها_شمار(fs->super))))
		أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
	أرجع 0;
}

errcode_t ext2fs_inode_پیمایش_goto_blockgroup(ext2_inode_پیمایش پیمایش,
					    صحيح	گروه)
{
	پیمایش->current_گروه = گروه - 1;
	پیمایش->groups_چپ = پیمایش->fs->گروه_desc_شمار - گروه;
	پیمایش->bad_بلوک_ptr = 0;
	أرجع get_بعدی_blockgroup(پیمایش);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ساكن errcode_t check_for_inode_bad_بلوکها(ext2_inode_پیمایش پیمایش,
					    blk64_t *رقم_بلوکها)
{
	blk64_t	blk = پیمایش->current_بلوک;
	badblocks_list	bb = پیمایش->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	إذا (blk == 0)
		أرجع 0;

	/* Make sure bad_block_ptr is still valid */
	إذا (پیمایش->bad_بلوک_ptr >= bb->رقم) {
		پیمایش->پیمایش_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	طالما (blk > bb->list[پیمایش->bad_بلوک_ptr]) {
		إذا (++پیمایش->bad_بلوک_ptr >= bb->رقم) {
			پیمایش->پیمایش_flags &= ~EXT2_SF_CHK_BADBLOCKS;
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
	إذا (blk == bb->list[پیمایش->bad_بلوک_ptr]) {
		پیمایش->پیمایش_flags |= EXT2_SF_BAD_INODE_BLK;
		*رقم_بلوکها = 1;
		إذا (++پیمایش->bad_بلوک_ptr >= bb->رقم)
			پیمایش->پیمایش_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	إذا ((blk + *رقم_بلوکها) > bb->list[پیمایش->bad_بلوک_ptr])
		*رقم_بلوکها = (صحيح) (bb->list[پیمایش->bad_بلوک_ptr] - blk);

	أرجع 0;
}

ساكن صحيح بلوک_map_looks_insane(ext2_filsys fs,
				  بنية ext2_inode_large *inode)
{
	بیعلامت صحيح i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	إذا ((inode->i_flags & EXT4_INLINE_داده_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		أرجع 0;
	إذا (!LINUX_S_ISREG(inode->i_نما) &&
	    !LINUX_S_ISLNK(inode->i_نما) &&
	    !LINUX_S_ISDIR(inode->i_نما))
		أرجع 0;
	إذا (LINUX_S_ISLNK(inode->i_نما) &&
	    EXT2_I_اندازه(inode) <= حجم(inode->i_بلوک))
		أرجع 0;

	/* Unused inodes probably aren't insane */
	إذا (inode->i_links_شمار == 0)
		أرجع 0;

	/* See if more than half the block maps are insane */
	لكل (i = 0, bad = 0; i < EXT2_N_بلوکها; i++)
		إذا (inode->i_بلوک[i] != 0 &&
		    (inode->i_بلوک[i] < fs->super->s_اولی_داده_بلوک ||
		     inode->i_بلوک[i] >= ext2fs_بلوکها_شمار(fs->super)))
			bad++;
	أرجع bad > EXT2_N_بلوکها / 2;
}

ساكن صحيح extent_سر_looks_insane(بنية ext2_inode_large *inode)
{
	إذا (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_سرآیند_verify(inode->i_بلوک,
					حجم(inode->i_بلوک)) == 0)
		أرجع 0;
	أرجع 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ساكن فراغ check_inode_بلوک_sanity(ext2_inode_پیمایش پیمایش, blk64_t رقم_بلوکها)
{
	ext2_ino_t	ino, inodes_مقصد_پیمایش;
	بیعلامت صحيح	badness, checksum_failures;
	بیعلامت صحيح	inodes_درون_buf, inodes_در_هر_بلوک;
	حرف		*p;
	بنية ext2_inode_large *inode;
	حرف		*بلوک_وضعیت;
	بیعلامت صحيح	blk, bad_csum;

	إذا (!(پیمایش->پیمایش_flags & EXT2_SF_WARN_GARBAGE_INODES))
		أرجع;

	inodes_مقصد_پیمایش = پیمایش->inodes_چپ;
	inodes_درون_buf = رقم_بلوکها * پیمایش->fs->blocksize / پیمایش->inode_اندازه;
	إذا (inodes_مقصد_پیمایش > inodes_درون_buf)
		inodes_مقصد_پیمایش = inodes_درون_buf;

	p = (حرف *) پیمایش->inode_میانگیر;
	ino = پیمایش->current_inode + 1;
	checksum_failures = badness = 0;
	بلوک_وضعیت = پیمایش_بلوک_وضعیت(پیمایش);
	memset(بلوک_وضعیت, 0, پیمایش->inode_میانگیر_بلوکها);
	inodes_در_هر_بلوک = EXT2_INODES_در_هر_بلوک(پیمایش->fs->super);

	إذا (inodes_در_هر_بلوک < 2)
		أرجع;

#اگر_تعریف_شده واژگان_BIGENDIAN
	إذا (ext2fs_get_mem(EXT2_INODE_اندازه(پیمایش->fs->super), &inode))
		أرجع;
#پایان_اگر

	طالما (inodes_مقصد_پیمایش > 0) {
		blk = (p - (حرف *)پیمایش->inode_میانگیر) / پیمایش->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(پیمایش->fs, ino,
				(بنية ext2_inode_large *) p) == 0;

#اگر_تعریف_شده واژگان_BIGENDIAN
		ext2fs_جابجا_inode_پر(پیمایش->fs,
			       (بنية ext2_inode_large *) inode,
			       (بنية ext2_inode_large *) p,
			       0, EXT2_INODE_اندازه(پیمایش->fs->super));
#وإلا
		inode = (بنية ext2_inode_large *) p;
#پایان_اگر

		/* Is this inode insane? */
		إذا (bad_csum) {
			checksum_failures++;
			badness++;
		} وإلا إذا (extent_سر_looks_insane(inode) ||
			   بلوک_map_looks_insane(پیمایش->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		إذا (badness > inodes_در_هر_بلوک / 2) {
			بیعلامت صحيح ino_adj;

			بلوک_وضعیت[blk] |= IBLOCK_وضعیت_INSANE;
			ino_adj = inodes_در_هر_بلوک -
						((ino - 1) % inodes_در_هر_بلوک);
			إذا (ino_adj > inodes_مقصد_پیمایش)
				ino_adj = inodes_مقصد_پیمایش;
			inodes_مقصد_پیمایش -= ino_adj;
			p += پیمایش->inode_اندازه * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			استمر;
		}

		إذا ((ino % inodes_در_هر_بلوک) == 0) {
			إذا (checksum_failures == 0)
				بلوک_وضعیت[blk] |= IBLOCK_وضعیت_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_مقصد_پیمایش--;
		p += پیمایش->inode_اندازه;
		ino++;
	};

#اگر_تعریف_شده واژگان_BIGENDIAN
	ext2fs_free_mem(&inode);
#پایان_اگر
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ساكن errcode_t get_بعدی_بلوکها(ext2_inode_پیمایش پیمایش)
{
	blk64_t		رقم_بلوکها;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	رقم_بلوکها = پیمایش->inode_میانگیر_بلوکها;
	إذا (رقم_بلوکها > پیمایش->بلوکها_چپ)
		رقم_بلوکها = پیمایش->بلوکها_چپ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	إذا (پیمایش->پیمایش_flags & EXT2_SF_BAD_INODE_BLK) {
		إذا (پیمایش->بایتها_چپ)
			پیمایش->پیمایش_flags |= EXT2_SF_BAD_EXTRA_بایتها;
		پیمایش->پیمایش_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	إذا (پیمایش->پیمایش_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_بلوکها(پیمایش, &رقم_بلوکها);
		إذا (retval)
			أرجع retval;
	}

	إذا ((پیمایش->پیمایش_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (پیمایش->current_بلوک == 0)) {
		memset(پیمایش->inode_میانگیر, 0,
		       (size_t) رقم_بلوکها * پیمایش->fs->blocksize);
	} وإلا {
		retval = io_channel_read_blk64(پیمایش->fs->io,
					     پیمایش->current_بلوک,
					     (صحيح) رقم_بلوکها,
					     پیمایش->inode_میانگیر);
		إذا (retval)
			أرجع EXT2_ET_بعدی_INODE_READ;
	}
	check_inode_بلوک_sanity(پیمایش, رقم_بلوکها);

	پیمایش->ptr = پیمایش->inode_میانگیر;
	پیمایش->بایتها_چپ = رقم_بلوکها * پیمایش->fs->blocksize;

	پیمایش->بلوکها_چپ -= رقم_بلوکها;
	إذا (پیمایش->current_بلوک)
		پیمایش->current_بلوک += رقم_بلوکها;

	أرجع 0;
}

#إذا 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ساكن مضمن صحيح is_خالی_پیمایش(ext2_inode_پیمایش پیمایش)
{
	صحيح	i;

	إذا (پیمایش->بایتها_چپ == 0)
		أرجع 0;

	لكل (i=0; i < پیمایش->بایتها_چپ; i++)
		إذا (پیمایش->ptr[i])
			أرجع 0;
	أرجع 1;
}
#پایان_اگر

errcode_t ext2fs_get_بعدی_inode_پر(ext2_inode_پیمایش پیمایش, ext2_ino_t *ino,
				     بنية ext2_inode *inode, صحيح bufsize)
{
	errcode_t	retval;
	صحيح		extra_بایتها = 0;
	صحيح		درازی;
	بنية ext2_inode_large	*iptr = (بنية ext2_inode_large *)inode;
	حرف		*iblock_وضعیت;
	بیعلامت صحيح	iblk;

	EXT2_CHECK_MAGIC(پیمایش, EXT2_ET_MAGIC_INODE_پیمایش);
	درازی = EXT2_INODE_اندازه(پیمایش->fs->super);
	iblock_وضعیت = پیمایش_بلوک_وضعیت(پیمایش);

	/*
	 * Do we need to start reading a new block group?
	 */
	إذا (پیمایش->inodes_چپ <= 0) {
	force_new_گروه:
		إذا (پیمایش->انجامشده_گروه) {
			retval = (پیمایش->انجامشده_گروه)
				(پیمایش->fs, پیمایش, پیمایش->current_گروه,
				 پیمایش->انجامشده_گروه_داده);
			إذا (retval)
				أرجع retval;
		}
		إذا (پیمایش->groups_چپ <= 0) {
			*ino = 0;
			أرجع 0;
		}
		retval = get_بعدی_blockgroup(پیمایش);
		إذا (retval)
			أرجع retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	إذا ((پیمایش->پیمایش_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(پیمایش->fs, پیمایش->current_گروه, EXT2_BG_INODE_UNINIT)
	     ))
		اذهب force_new_گروه;
	إذا (پیمایش->inodes_چپ == 0)
		اذهب force_new_گروه;
	إذا (پیمایش->current_بلوک == 0) {
		إذا (پیمایش->پیمایش_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			اذهب force_new_گروه;
		} وإلا
			أرجع EXT2_ET_MISSING_INODE_جدول;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	إذا (پیمایش->بایتها_چپ < پیمایش->inode_اندازه) {
		إذا (پیمایش->بایتها_چپ)
			کپی_حافظه(پیمایش->موقت_میانگیر, پیمایش->ptr, پیمایش->بایتها_چپ);
		extra_بایتها = پیمایش->بایتها_چپ;

		retval = get_بعدی_بلوکها(پیمایش);
		إذا (retval)
			أرجع retval;
#إذا 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		إذا (is_خالی_پیمایش(پیمایش))
			اذهب force_new_گروه;
#پایان_اگر
	}

	إذا (bufsize < درازی) {
		retval = ext2fs_get_mem(درازی, &iptr);
		إذا (retval)
			أرجع retval;
	}

	retval = 0;
	iblk = پیمایش->current_inode % EXT2_INODES_در_هر_گروه(پیمایش->fs->super) /
				EXT2_INODES_در_هر_بلوک(پیمایش->fs->super) %
				پیمایش->inode_میانگیر_بلوکها;
	إذا (extra_بایتها) {
		کپی_حافظه(پیمایش->موقت_میانگیر+extra_بایتها, پیمایش->ptr,
		       پیمایش->inode_اندازه - extra_بایتها);
		پیمایش->ptr += پیمایش->inode_اندازه - extra_بایتها;
		پیمایش->بایتها_چپ -= پیمایش->inode_اندازه - extra_بایتها;

		/* Verify the inode checksum. */
		إذا (!(iblock_وضعیت[iblk] & IBLOCK_وضعیت_CSUMS_OK) &&
		    !(پیمایش->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(پیمایش->fs, پیمایش->current_inode + 1,
				(بنية ext2_inode_large *)پیمایش->موقت_میانگیر))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#اگر_تعریف_شده واژگان_BIGENDIAN
		memset(iptr, 0, درازی);
		ext2fs_جابجا_inode_پر(پیمایش->fs,
			       (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) پیمایش->موقت_میانگیر,
			       0, درازی);
#وإلا
		کپی_حافظه(iptr, پیمایش->موقت_میانگیر, درازی);
#پایان_اگر
		إذا (پیمایش->پیمایش_flags & EXT2_SF_BAD_EXTRA_بایتها)
			retval = EXT2_ET_BAD_بلوک_درون_INODE_جدول;
		پیمایش->پیمایش_flags &= ~EXT2_SF_BAD_EXTRA_بایتها;
	} وإلا {
		/* Verify the inode checksum. */
		إذا (!(iblock_وضعیت[iblk] & IBLOCK_وضعیت_CSUMS_OK) &&
		    !(پیمایش->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(پیمایش->fs, پیمایش->current_inode + 1,
				(بنية ext2_inode_large *)پیمایش->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#اگر_تعریف_شده واژگان_BIGENDIAN
		memset(iptr, 0, درازی);
		ext2fs_جابجا_inode_پر(پیمایش->fs,
				(بنية ext2_inode_large *) iptr,
				(بنية ext2_inode_large *) پیمایش->ptr,
				0, درازی);
#وإلا
		کپی_حافظه(iptr, پیمایش->ptr, درازی);
#پایان_اگر
		پیمایش->ptr += پیمایش->inode_اندازه;
		پیمایش->بایتها_چپ -= پیمایش->inode_اندازه;
		إذا (پیمایش->پیمایش_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_بلوک_درون_INODE_جدول;
	}
	إذا ((iblock_وضعیت[iblk] & IBLOCK_وضعیت_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	پیمایش->inodes_چپ--;
	پیمایش->current_inode++;
	*ino = پیمایش->current_inode;
	إذا (iptr != (بنية ext2_inode_large *)inode) {
		کپی_حافظه(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	أرجع retval;
}

errcode_t ext2fs_get_بعدی_inode(ext2_inode_پیمایش پیمایش, ext2_ino_t *ino,
				بنية ext2_inode *inode)
{
	أرجع ext2fs_get_بعدی_inode_پر(پیمایش, ino, inode,
						حجم(بنية ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode * inode, صحيح bufsize,
			     صحيح flags)
{
	blk64_t		بلوک_nr;
	dgrp_t		گروه;
	بیعلامت دراز 	بلوک, جابه_جایی;
	حرف 		*ptr;
	errcode_t	retval;
	بیعلامت	i;
	صحيح		clen, inodes_در_هر_بلوک;
	io_channel	io;
	صحيح		درازی = EXT2_INODE_اندازه(fs->super);
	بنية ext2_inode_large	*iptr;
	صحيح		cache_جایگاه, fail_csum;

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
	إذا ((ino == 0) || (ino > fs->super->s_inodes_شمار))
		أرجع EXT2_ET_BAD_INODE_رقم;
	/* Create inode cache if not present */
	إذا (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		إذا (retval)
			أرجع retval;
	}
	/* Check to see if it's in the inode cache */
	لكل (i = 0; i < fs->icache->cache_اندازه; i++) {
		إذا (fs->icache->cache[i].ino == ino) {
			کپی_حافظه(inode, fs->icache->cache[i].inode,
			       (bufsize > درازی) ? درازی : bufsize);
			أرجع 0;
		}
	}
	إذا (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_در_هر_بلوک = fs->blocksize / EXT2_INODE_اندازه(fs->super);
		بلوک_nr = ext2fs_le32_مقصد_cpu(fs->image_سرآیند->جابه_جایی_inode) / fs->blocksize;
		بلوک_nr += (ino - 1) / inodes_در_هر_بلوک;
		جابه_جایی = ((ino - 1) % inodes_در_هر_بلوک) *
			EXT2_INODE_اندازه(fs->super);
		io = fs->image_io;
	} وإلا {
		گروه = (ino - 1) / EXT2_INODES_در_هر_گروه(fs->super);
		إذا (گروه > fs->گروه_desc_شمار)
			أرجع EXT2_ET_BAD_INODE_رقم;
		جابه_جایی = ((ino - 1) % EXT2_INODES_در_هر_گروه(fs->super)) *
			EXT2_INODE_اندازه(fs->super);
		بلوک = جابه_جایی >> EXT2_بلوک_اندازه_بیتها(fs->super);
		بلوک_nr = ext2fs_inode_جدول_loc(fs, گروه);
		إذا (!بلوک_nr)
			أرجع EXT2_ET_MISSING_INODE_جدول;
		إذا ((بلوک_nr < fs->super->s_اولی_داده_بلوک) ||
		    (بلوک_nr + fs->inode_بلوکها_در_هر_گروه - 1 >=
		     ext2fs_بلوکها_شمار(fs->super)))
			أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
		بلوک_nr += بلوک;
		io = fs->io;
	}
	جابه_جایی &= (EXT2_بلوک_اندازه(fs->super) - 1);

	cache_جایگاه = (fs->icache->cache_آخرین + 1) % fs->icache->cache_اندازه;
	iptr = (بنية ext2_inode_large *)fs->icache->cache[cache_جایگاه].inode;

	ptr = (حرف *) iptr;
	طالما (درازی) {
		clen = درازی;
		إذا ((جابه_جایی + درازی) > fs->blocksize)
			clen = fs->blocksize - جابه_جایی;

		إذا (بلوک_nr != fs->icache->میانگیر_blk) {
			retval = io_channel_read_blk64(io, بلوک_nr, 1,
						     fs->icache->میانگیر);
			إذا (retval)
				أرجع retval;
			fs->icache->میانگیر_blk = بلوک_nr;
		}

		کپی_حافظه(ptr, ((حرف *) fs->icache->میانگیر) + (بیعلامت) جابه_جایی,
		       clen);

		جابه_جایی = 0;
		درازی -= clen;
		ptr += clen;
		بلوک_nr++;
	}
	درازی = EXT2_INODE_اندازه(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#اگر_تعریف_شده واژگان_BIGENDIAN
	ext2fs_جابجا_inode_پر(fs, (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) iptr,
			       0, درازی);
#پایان_اگر

	/* Update the inode cache bookkeeping */
	إذا (!fail_csum) {
		fs->icache->cache_آخرین = cache_جایگاه;
		fs->icache->cache[cache_جایگاه].ino = ino;
	}
	کپی_حافظه(inode, iptr, (bufsize > درازی) ? درازی : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		أرجع EXT2_ET_INODE_CSUM_INVALID;

	أرجع 0;
}

errcode_t ext2fs_read_inode_پر(ext2_filsys fs, ext2_ino_t ino,
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
	blk64_t بلوک_nr;
	dgrp_t گروه;
	بیعلامت دراز بلوک, جابه_جایی;
	errcode_t retval = 0;
	بنية ext2_inode_large *w_inode;
	حرف *ptr;
	بیعلامت i;
	صحيح clen;
	صحيح درازی = EXT2_INODE_اندازه(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	إذا (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}

	إذا ((ino == 0) || (ino > fs->super->s_inodes_شمار))
		أرجع EXT2_ET_BAD_INODE_رقم;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(درازی, &w_inode);
	إذا (retval)
		أرجع retval;

	إذا (bufsize < درازی) {
		retval = ext2fs_read_inode2(fs, ino,
					    (بنية ext2_inode *)w_inode,
					    درازی, READ_INODE_NOCSUM);
		إذا (retval)
			اذهب errout;
	}

	/* Check to see if the inode cache needs to be updated */
	إذا (fs->icache) {
		لكل (i=0; i < fs->icache->cache_اندازه; i++) {
			إذا (fs->icache->cache[i].ino == ino) {
				کپی_حافظه(fs->icache->cache[i].inode, inode,
				       (bufsize > درازی) ? درازی : bufsize);
				اكسر;
			}
		}
	} وإلا {
		retval = ext2fs_create_inode_cache(fs, 4);
		إذا (retval)
			اذهب errout;
	}
	کپی_حافظه(w_inode, inode, (bufsize > درازی) ? درازی : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		اذهب errout;
	}

#اگر_تعریف_شده واژگان_BIGENDIAN
	ext2fs_جابجا_inode_پر(fs, w_inode, w_inode, 1, درازی);
#پایان_اگر

	إذا ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_مجموعه(fs, ino, w_inode);
		إذا (retval)
			اذهب errout;
	}

	گروه = (ino - 1) / EXT2_INODES_در_هر_گروه(fs->super);
	جابه_جایی = ((ino - 1) % EXT2_INODES_در_هر_گروه(fs->super)) *
		EXT2_INODE_اندازه(fs->super);
	بلوک = جابه_جایی >> EXT2_بلوک_اندازه_بیتها(fs->super);
	بلوک_nr = ext2fs_inode_جدول_loc(fs, (بیعلامت) گروه);
	إذا (!بلوک_nr) {
		retval = EXT2_ET_MISSING_INODE_جدول;
		اذهب errout;
	}
	إذا ((بلوک_nr < fs->super->s_اولی_داده_بلوک) ||
	    (بلوک_nr + fs->inode_بلوکها_در_هر_گروه - 1 >=
	     ext2fs_بلوکها_شمار(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_جدول;
		اذهب errout;
	}
	بلوک_nr += بلوک;

	جابه_جایی &= (EXT2_بلوک_اندازه(fs->super) - 1);

	ptr = (حرف *) w_inode;

	طالما (درازی) {
		clen = درازی;
		إذا ((جابه_جایی + درازی) > fs->blocksize)
			clen = fs->blocksize - جابه_جایی;

		إذا (fs->icache->میانگیر_blk != بلوک_nr) {
			retval = io_channel_read_blk64(fs->io, بلوک_nr, 1,
						     fs->icache->میانگیر);
			إذا (retval)
				اذهب errout;
			fs->icache->میانگیر_blk = بلوک_nr;
		}


		کپی_حافظه((حرف *) fs->icache->میانگیر + (بیعلامت) جابه_جایی,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, بلوک_nr, 1,
					      fs->icache->میانگیر);
		إذا (retval)
			اذهب errout;

		جابه_جایی = 0;
		ptr += clen;
		درازی -= clen;
		بلوک_nr++;
	}

	fs->flags |= EXT2_FLAG_تغییریافته;
errout:
	ext2fs_free_mem(&w_inode);
	أرجع retval;
}

errcode_t ext2fs_write_inode_پر(ext2_filsys fs, ext2_ino_t ino,
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
	time_t			t = ext2fs_P_get_زمان(fs);

	إذا (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_مجموعه(inode, i_atime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_مجموعه(inode, i_ctime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_مجموعه(inode, i_mtime, t);

	إذا (اندازه == حجم(بنية ext2_inode))
		أرجع ext2fs_write_inode_پر(fs, ino, inode,
					       حجم(بنية ext2_inode));

	buf = حافظه_بده(اندازه);
	إذا (!buf)
		أرجع ENOMEM;

	memset(buf, 0, اندازه);
	*buf = *inode;

	large_inode = (بنية ext2_inode_large *) buf;
	large_inode->i_extra_isize = حجم(بنية ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_اندازه;
	إذا (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_پر(fs, ino, buf, اندازه);
	حرر(buf);
	أرجع retval;
}


errcode_t ext2fs_get_بلوکها(ext2_filsys fs, ext2_ino_t ino, blk_t *بلوکها)
{
	بنية ext2_inode	inode;
	صحيح			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ino > fs->super->s_inodes_شمار)
		أرجع EXT2_ET_BAD_INODE_رقم;

	إذا (fs->get_بلوکها) {
		إذا (!(*fs->get_بلوکها)(fs, ino, بلوکها))
			أرجع 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	لكل (i=0; i < EXT2_N_بلوکها; i++)
		بلوکها[i] = inode.i_بلوک[i];
	أرجع 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	بنية	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ino > fs->super->s_inodes_شمار)
		أرجع EXT2_ET_BAD_INODE_رقم;

	إذا (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	إذا (!LINUX_S_ISDIR(inode.i_نما))
		أرجع EXT2_ET_NO_DIRECTORY;
	أرجع 0;
}

