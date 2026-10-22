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
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#وصف_ڪريو IBLOCK_حالت_CSUMS_OK	1
#وصف_ڪريو IBLOCK_حالت_INSANE	2
#وصف_ڪريو جاچ_بلاڪ_حالت(جاچ)	((جاچ)->عارضي_بفر + (جاچ)->inode_سائيز)

بنية ext2_struct_inode_جاچ {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_بلاڪ;
	dgrp_t			current_گروهه;
	ext2_ino_t		inodes_کاٻو;
	blk_t			بلاڪون_کاٻو;
	dgrp_t			groups_کاٻو;
	blk_t			inode_بفر_بلاڪون;
	حرف *			inode_بفر;
	صحيح			inode_سائيز;
	حرف *			ptr;
	صحيح			بائيٽون_کاٻو;
	حرف			*عارضي_بفر;
	errcode_t		(*پورو_گروهه)(ext2_filsys fs,
					      ext2_inode_جاچ جاچ,
					      dgrp_t گروهه,
					      فراغ * priv_معلومات);
	فراغ *			پورو_گروهه_معلومات;
	صحيح			bad_بلاڪ_ptr;
	صحيح			جاچ_flags;
	صحيح			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	بنانشان	i;

	إذا (!fs->icache)
		أرجع 0;

	لكل (i=0; i < fs->icache->cache_سائيز; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->بفر_blk = 0;
	أرجع 0;
}

/*
 * Free the inode cache structure
 */
فراغ ext2fs_free_inode_cache(بنية ext2_inode_cache *icache)
{
	بنانشان i;

	إذا (--icache->refcount)
		أرجع;
	إذا (icache->بفر)
		ext2fs_free_mem(&icache->بفر);
	لكل (i = 0; i < icache->cache_سائيز; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	إذا (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->بفر_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, بنانشان صحيح cache_سائيز)
{
	بنانشان	i;
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
	fs->icache->cache_آخري = -1;
	fs->icache->cache_سائيز = cache_سائيز;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_سائيز,
				  حجم(بنية ext2_inode_cache_ent),
				  &fs->icache->cache);
	إذا (retval)
		اذهب errout;

	لكل (i = 0; i < fs->icache->cache_سائيز; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_سائيز(fs->super),
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

errcode_t ext2fs_open_inode_جاچ(ext2_filsys fs, صحيح بفر_بلاڪون,
				 ext2_inode_جاچ *ret_جاچ)
{
	ext2_inode_جاچ	جاچ;
	errcode_t	retval;
	errcode_t (*save_get_بلاڪون)(ext2_filsys f, ext2_ino_t ino, blk_t *بلاڪون);

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
		save_get_بلاڪون = fs->get_بلاڪون;
		fs->get_بلاڪون = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		إذا (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_بلاڪون = save_get_بلاڪون;
	}

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_inode_جاچ), &جاچ);
	إذا (retval)
		أرجع retval;
	memset(جاچ, 0, حجم(بنية ext2_struct_inode_جاچ));

	جاچ->magic = EXT2_ET_MAGIC_INODE_جاچ;
	جاچ->fs = fs;
	جاچ->inode_سائيز = EXT2_INODE_سائيز(fs->super);
	جاچ->بائيٽون_کاٻو = 0;
	جاچ->current_گروهه = 0;
	جاچ->groups_کاٻو = fs->گروهه_desc_ڳڻپ - 1;
	جاچ->inode_بفر_بلاڪون = بفر_بلاڪون ? بفر_بلاڪون :
				    EXT2_INODE_جاچ_DEFAULT_بفر_بلاڪون;
	جاچ->current_بلاڪ = ext2fs_inode_جدول_loc(جاچ->fs,
						     جاچ->current_گروهه);
	إذا (جاچ->current_بلاڪ &&
	    ((جاچ->current_بلاڪ < fs->super->s_پهريون_معلومات_بلاڪ) ||
	     (جاچ->current_بلاڪ + fs->inode_بلاڪون_في_گروهه - 1 >=
	      ext2fs_بلاڪون_ڳڻپ(fs->super)))) {
		ext2fs_free_mem(&جاچ);
		أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
	}

	جاچ->inodes_کاٻو = EXT2_INODES_في_گروهه(جاچ->fs->super);
	جاچ->بلاڪون_کاٻو = جاچ->fs->inode_بلاڪون_في_گروهه;
	إذا (ext2fs_has_گروهه_desc_csum(fs)) {
		__u32 اڻ_استعمال_ٿيل = ext2fs_bg_itable_اڻ_استعمال_ٿيل(fs, جاچ->current_گروهه);
		إذا (جاچ->inodes_کاٻو > اڻ_استعمال_ٿيل)
			جاچ->inodes_کاٻو -= اڻ_استعمال_ٿيل;
		وإلا
			جاچ->inodes_کاٻو = 0;
		جاچ->بلاڪون_کاٻو =
			(جاچ->inodes_کاٻو +
			 (fs->blocksize / جاچ->inode_سائيز - 1)) *
			جاچ->inode_سائيز / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, جاچ->inode_بفر_بلاڪون,
				      &جاچ->inode_بفر);
	جاچ->پورو_گروهه = 0;
	جاچ->پورو_گروهه_معلومات = 0;
	جاچ->bad_بلاڪ_ptr = 0;
	إذا (retval) {
		ext2fs_free_mem(&جاچ);
		أرجع retval;
	}
	retval = ext2fs_get_mem(جاچ->inode_سائيز + جاچ->inode_بفر_بلاڪون,
				&جاچ->عارضي_بفر);
	إذا (retval) {
		ext2fs_free_mem(&جاچ->inode_بفر);
		ext2fs_free_mem(&جاچ);
		أرجع retval;
	}
	memset(جاچ_بلاڪ_حالت(جاچ), 0, جاچ->inode_بفر_بلاڪون);
	إذا (جاچ->fs->badblocks && جاچ->fs->badblocks->انگ)
		جاچ->جاچ_flags |= EXT2_SF_CHK_BADBLOCKS;
	إذا (ext2fs_has_گروهه_desc_csum(fs))
		جاچ->جاچ_flags |= EXT2_SF_DO_LAZY;
	*ret_جاچ = جاچ;
	أرجع 0;
}

فراغ ext2fs_close_inode_جاچ(ext2_inode_جاچ جاچ)
{
	إذا (!جاچ || (جاچ->magic != EXT2_ET_MAGIC_INODE_جاچ))
		أرجع;

	ext2fs_free_mem(&جاچ->inode_بفر);
	جاچ->inode_بفر = عدم;
	ext2fs_free_mem(&جاچ->عارضي_بفر);
	جاچ->عارضي_بفر = عدم;
	ext2fs_free_mem(&جاچ);
	أرجع;
}

فراغ ext2fs_سيٽ_inode_callback(ext2_inode_جاچ جاچ,
			       errcode_t (*پورو_گروهه)(ext2_filsys fs,
						       ext2_inode_جاچ جاچ,
						       dgrp_t گروهه,
						       فراغ * priv_معلومات),
			       فراغ *پورو_گروهه_معلومات)
{
	إذا (!جاچ || (جاچ->magic != EXT2_ET_MAGIC_INODE_جاچ))
		أرجع;

	جاچ->پورو_گروهه = پورو_گروهه;
	جاچ->پورو_گروهه_معلومات = پورو_گروهه_معلومات;
}

صحيح ext2fs_inode_جاچ_flags(ext2_inode_جاچ جاچ, صحيح سيٽ_flags,
			    صحيح clear_flags)
{
	صحيح	old_flags;

	إذا (!جاچ || (جاچ->magic != EXT2_ET_MAGIC_INODE_جاچ))
		أرجع 0;

	old_flags = جاچ->جاچ_flags;
	جاچ->جاچ_flags &= ~clear_flags;
	جاچ->جاچ_flags |= سيٽ_flags;
	أرجع old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ساكن errcode_t get_ايندڙ_blockgroup(ext2_inode_جاچ جاچ)
{
	ext2_filsys fs = جاچ->fs;

	جاچ->current_گروهه++;
	جاچ->groups_کاٻو--;

	جاچ->current_بلاڪ = ext2fs_inode_جدول_loc(جاچ->fs,
						     جاچ->current_گروهه);
	جاچ->current_inode = جاچ->current_گروهه *
		EXT2_INODES_في_گروهه(fs->super);

	جاچ->بائيٽون_کاٻو = 0;
	جاچ->inodes_کاٻو = EXT2_INODES_في_گروهه(fs->super);
	جاچ->بلاڪون_کاٻو = fs->inode_بلاڪون_في_گروهه;
	إذا (ext2fs_has_گروهه_desc_csum(fs)) {
		__u32 اڻ_استعمال_ٿيل = ext2fs_bg_itable_اڻ_استعمال_ٿيل(fs, جاچ->current_گروهه);
		إذا (جاچ->inodes_کاٻو > اڻ_استعمال_ٿيل)
			جاچ->inodes_کاٻو -= اڻ_استعمال_ٿيل;
		وإلا
			جاچ->inodes_کاٻو = 0;
		جاچ->بلاڪون_کاٻو =
			(جاچ->inodes_کاٻو +
			 (fs->blocksize / جاچ->inode_سائيز - 1)) *
			جاچ->inode_سائيز / fs->blocksize;
	}
	إذا (جاچ->current_بلاڪ &&
	    ((جاچ->current_بلاڪ < fs->super->s_پهريون_معلومات_بلاڪ) ||
	     (جاچ->current_بلاڪ + fs->inode_بلاڪون_في_گروهه - 1 >=
	      ext2fs_بلاڪون_ڳڻپ(fs->super))))
		أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
	أرجع 0;
}

errcode_t ext2fs_inode_جاچ_goto_blockgroup(ext2_inode_جاچ جاچ,
					    صحيح	گروهه)
{
	جاچ->current_گروهه = گروهه - 1;
	جاچ->groups_کاٻو = جاچ->fs->گروهه_desc_ڳڻپ - گروهه;
	جاچ->bad_بلاڪ_ptr = 0;
	أرجع get_ايندڙ_blockgroup(جاچ);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ساكن errcode_t check_for_inode_bad_بلاڪون(ext2_inode_جاچ جاچ,
					    blk64_t *انگ_بلاڪون)
{
	blk64_t	blk = جاچ->current_بلاڪ;
	badblocks_list	bb = جاچ->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	إذا (blk == 0)
		أرجع 0;

	/* Make sure bad_block_ptr is still valid */
	إذا (جاچ->bad_بلاڪ_ptr >= bb->انگ) {
		جاچ->جاچ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	طالما (blk > bb->list[جاچ->bad_بلاڪ_ptr]) {
		إذا (++جاچ->bad_بلاڪ_ptr >= bb->انگ) {
			جاچ->جاچ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
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
	إذا (blk == bb->list[جاچ->bad_بلاڪ_ptr]) {
		جاچ->جاچ_flags |= EXT2_SF_BAD_INODE_BLK;
		*انگ_بلاڪون = 1;
		إذا (++جاچ->bad_بلاڪ_ptr >= bb->انگ)
			جاچ->جاچ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	إذا ((blk + *انگ_بلاڪون) > bb->list[جاچ->bad_بلاڪ_ptr])
		*انگ_بلاڪون = (صحيح) (bb->list[جاچ->bad_بلاڪ_ptr] - blk);

	أرجع 0;
}

ساكن صحيح بلاڪ_map_looks_insane(ext2_filsys fs,
				  بنية ext2_inode_large *inode)
{
	بنانشان صحيح i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	إذا ((inode->i_flags & EXT4_INLINE_معلومات_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		أرجع 0;
	إذا (!LINUX_S_ISREG(inode->i_گهڻائي) &&
	    !LINUX_S_ISLNK(inode->i_گهڻائي) &&
	    !LINUX_S_ISDIR(inode->i_گهڻائي))
		أرجع 0;
	إذا (LINUX_S_ISLNK(inode->i_گهڻائي) &&
	    EXT2_I_سائيز(inode) <= حجم(inode->i_بلاڪ))
		أرجع 0;

	/* Unused inodes probably aren't insane */
	إذا (inode->i_links_ڳڻپ == 0)
		أرجع 0;

	/* See if more than half the block maps are insane */
	لكل (i = 0, bad = 0; i < EXT2_N_بلاڪون; i++)
		إذا (inode->i_بلاڪ[i] != 0 &&
		    (inode->i_بلاڪ[i] < fs->super->s_پهريون_معلومات_بلاڪ ||
		     inode->i_بلاڪ[i] >= ext2fs_بلاڪون_ڳڻپ(fs->super)))
			bad++;
	أرجع bad > EXT2_N_بلاڪون / 2;
}

ساكن صحيح extent_سر_looks_insane(بنية ext2_inode_large *inode)
{
	إذا (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_سرو_verify(inode->i_بلاڪ,
					حجم(inode->i_بلاڪ)) == 0)
		أرجع 0;
	أرجع 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ساكن فراغ check_inode_بلاڪ_sanity(ext2_inode_جاچ جاچ, blk64_t انگ_بلاڪون)
{
	ext2_ino_t	ino, inodes_منزل_جاچ;
	بنانشان صحيح	badness, checksum_failures;
	بنانشان صحيح	inodes_اندريون_buf, inodes_في_بلاڪ;
	حرف		*p;
	بنية ext2_inode_large *inode;
	حرف		*بلاڪ_حالت;
	بنانشان صحيح	blk, bad_csum;

	إذا (!(جاچ->جاچ_flags & EXT2_SF_WARN_GARBAGE_INODES))
		أرجع;

	inodes_منزل_جاچ = جاچ->inodes_کاٻو;
	inodes_اندريون_buf = انگ_بلاڪون * جاچ->fs->blocksize / جاچ->inode_سائيز;
	إذا (inodes_منزل_جاچ > inodes_اندريون_buf)
		inodes_منزل_جاچ = inodes_اندريون_buf;

	p = (حرف *) جاچ->inode_بفر;
	ino = جاچ->current_inode + 1;
	checksum_failures = badness = 0;
	بلاڪ_حالت = جاچ_بلاڪ_حالت(جاچ);
	memset(بلاڪ_حالت, 0, جاچ->inode_بفر_بلاڪون);
	inodes_في_بلاڪ = EXT2_INODES_في_بلاڪ(جاچ->fs->super);

	إذا (inodes_في_بلاڪ < 2)
		أرجع;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	إذا (ext2fs_get_mem(EXT2_INODE_سائيز(جاچ->fs->super), &inode))
		أرجع;
#جيڪڏهن_پورو

	طالما (inodes_منزل_جاچ > 0) {
		blk = (p - (حرف *)جاچ->inode_بفر) / جاچ->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(جاچ->fs, ino,
				(بنية ext2_inode_large *) p) == 0;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		ext2fs_مٽا_سٽا_inode_ڀريل(جاچ->fs,
			       (بنية ext2_inode_large *) inode,
			       (بنية ext2_inode_large *) p,
			       0, EXT2_INODE_سائيز(جاچ->fs->super));
#وإلا
		inode = (بنية ext2_inode_large *) p;
#جيڪڏهن_پورو

		/* Is this inode insane? */
		إذا (bad_csum) {
			checksum_failures++;
			badness++;
		} وإلا إذا (extent_سر_looks_insane(inode) ||
			   بلاڪ_map_looks_insane(جاچ->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		إذا (badness > inodes_في_بلاڪ / 2) {
			بنانشان صحيح ino_adj;

			بلاڪ_حالت[blk] |= IBLOCK_حالت_INSANE;
			ino_adj = inodes_في_بلاڪ -
						((ino - 1) % inodes_في_بلاڪ);
			إذا (ino_adj > inodes_منزل_جاچ)
				ino_adj = inodes_منزل_جاچ;
			inodes_منزل_جاچ -= ino_adj;
			p += جاچ->inode_سائيز * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			استمر;
		}

		إذا ((ino % inodes_في_بلاڪ) == 0) {
			إذا (checksum_failures == 0)
				بلاڪ_حالت[blk] |= IBLOCK_حالت_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_منزل_جاچ--;
		p += جاچ->inode_سائيز;
		ino++;
	};

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	ext2fs_free_mem(&inode);
#جيڪڏهن_پورو
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ساكن errcode_t get_ايندڙ_بلاڪون(ext2_inode_جاچ جاچ)
{
	blk64_t		انگ_بلاڪون;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	انگ_بلاڪون = جاچ->inode_بفر_بلاڪون;
	إذا (انگ_بلاڪون > جاچ->بلاڪون_کاٻو)
		انگ_بلاڪون = جاچ->بلاڪون_کاٻو;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	إذا (جاچ->جاچ_flags & EXT2_SF_BAD_INODE_BLK) {
		إذا (جاچ->بائيٽون_کاٻو)
			جاچ->جاچ_flags |= EXT2_SF_BAD_EXTRA_بائيٽون;
		جاچ->جاچ_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	إذا (جاچ->جاچ_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_بلاڪون(جاچ, &انگ_بلاڪون);
		إذا (retval)
			أرجع retval;
	}

	إذا ((جاچ->جاچ_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (جاچ->current_بلاڪ == 0)) {
		memset(جاچ->inode_بفر, 0,
		       (size_t) انگ_بلاڪون * جاچ->fs->blocksize);
	} وإلا {
		retval = io_channel_read_blk64(جاچ->fs->io,
					     جاچ->current_بلاڪ,
					     (صحيح) انگ_بلاڪون,
					     جاچ->inode_بفر);
		إذا (retval)
			أرجع EXT2_ET_ايندڙ_INODE_READ;
	}
	check_inode_بلاڪ_sanity(جاچ, انگ_بلاڪون);

	جاچ->ptr = جاچ->inode_بفر;
	جاچ->بائيٽون_کاٻو = انگ_بلاڪون * جاچ->fs->blocksize;

	جاچ->بلاڪون_کاٻو -= انگ_بلاڪون;
	إذا (جاچ->current_بلاڪ)
		جاچ->current_بلاڪ += انگ_بلاڪون;

	أرجع 0;
}

#إذا 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ساكن مضمن صحيح is_خالي_جايون_جاچ(ext2_inode_جاچ جاچ)
{
	صحيح	i;

	إذا (جاچ->بائيٽون_کاٻو == 0)
		أرجع 0;

	لكل (i=0; i < جاچ->بائيٽون_کاٻو; i++)
		إذا (جاچ->ptr[i])
			أرجع 0;
	أرجع 1;
}
#جيڪڏهن_پورو

errcode_t ext2fs_get_ايندڙ_inode_ڀريل(ext2_inode_جاچ جاچ, ext2_ino_t *ino,
				     بنية ext2_inode *inode, صحيح bufsize)
{
	errcode_t	retval;
	صحيح		extra_بائيٽون = 0;
	صحيح		ڊيگهائي;
	بنية ext2_inode_large	*iptr = (بنية ext2_inode_large *)inode;
	حرف		*iblock_حالت;
	بنانشان صحيح	iblk;

	EXT2_CHECK_MAGIC(جاچ, EXT2_ET_MAGIC_INODE_جاچ);
	ڊيگهائي = EXT2_INODE_سائيز(جاچ->fs->super);
	iblock_حالت = جاچ_بلاڪ_حالت(جاچ);

	/*
	 * Do we need to start reading a new block group?
	 */
	إذا (جاچ->inodes_کاٻو <= 0) {
	force_new_گروهه:
		إذا (جاچ->پورو_گروهه) {
			retval = (جاچ->پورو_گروهه)
				(جاچ->fs, جاچ, جاچ->current_گروهه,
				 جاچ->پورو_گروهه_معلومات);
			إذا (retval)
				أرجع retval;
		}
		إذا (جاچ->groups_کاٻو <= 0) {
			*ino = 0;
			أرجع 0;
		}
		retval = get_ايندڙ_blockgroup(جاچ);
		إذا (retval)
			أرجع retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	إذا ((جاچ->جاچ_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(جاچ->fs, جاچ->current_گروهه, EXT2_BG_INODE_UNINIT)
	     ))
		اذهب force_new_گروهه;
	إذا (جاچ->inodes_کاٻو == 0)
		اذهب force_new_گروهه;
	إذا (جاچ->current_بلاڪ == 0) {
		إذا (جاچ->جاچ_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			اذهب force_new_گروهه;
		} وإلا
			أرجع EXT2_ET_MISSING_INODE_جدول;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	إذا (جاچ->بائيٽون_کاٻو < جاچ->inode_سائيز) {
		إذا (جاچ->بائيٽون_کاٻو)
			يادگيري_نقل(جاچ->عارضي_بفر, جاچ->ptr, جاچ->بائيٽون_کاٻو);
		extra_بائيٽون = جاچ->بائيٽون_کاٻو;

		retval = get_ايندڙ_بلاڪون(جاچ);
		إذا (retval)
			أرجع retval;
#إذا 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		إذا (is_خالي_جايون_جاچ(جاچ))
			اذهب force_new_گروهه;
#جيڪڏهن_پورو
	}

	إذا (bufsize < ڊيگهائي) {
		retval = ext2fs_get_mem(ڊيگهائي, &iptr);
		إذا (retval)
			أرجع retval;
	}

	retval = 0;
	iblk = جاچ->current_inode % EXT2_INODES_في_گروهه(جاچ->fs->super) /
				EXT2_INODES_في_بلاڪ(جاچ->fs->super) %
				جاچ->inode_بفر_بلاڪون;
	إذا (extra_بائيٽون) {
		يادگيري_نقل(جاچ->عارضي_بفر+extra_بائيٽون, جاچ->ptr,
		       جاچ->inode_سائيز - extra_بائيٽون);
		جاچ->ptr += جاچ->inode_سائيز - extra_بائيٽون;
		جاچ->بائيٽون_کاٻو -= جاچ->inode_سائيز - extra_بائيٽون;

		/* Verify the inode checksum. */
		إذا (!(iblock_حالت[iblk] & IBLOCK_حالت_CSUMS_OK) &&
		    !(جاچ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(جاچ->fs, جاچ->current_inode + 1,
				(بنية ext2_inode_large *)جاچ->عارضي_بفر))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		memset(iptr, 0, ڊيگهائي);
		ext2fs_مٽا_سٽا_inode_ڀريل(جاچ->fs,
			       (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) جاچ->عارضي_بفر,
			       0, ڊيگهائي);
#وإلا
		يادگيري_نقل(iptr, جاچ->عارضي_بفر, ڊيگهائي);
#جيڪڏهن_پورو
		إذا (جاچ->جاچ_flags & EXT2_SF_BAD_EXTRA_بائيٽون)
			retval = EXT2_ET_BAD_بلاڪ_اندريون_INODE_جدول;
		جاچ->جاچ_flags &= ~EXT2_SF_BAD_EXTRA_بائيٽون;
	} وإلا {
		/* Verify the inode checksum. */
		إذا (!(iblock_حالت[iblk] & IBLOCK_حالت_CSUMS_OK) &&
		    !(جاچ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(جاچ->fs, جاچ->current_inode + 1,
				(بنية ext2_inode_large *)جاچ->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		memset(iptr, 0, ڊيگهائي);
		ext2fs_مٽا_سٽا_inode_ڀريل(جاچ->fs,
				(بنية ext2_inode_large *) iptr,
				(بنية ext2_inode_large *) جاچ->ptr,
				0, ڊيگهائي);
#وإلا
		يادگيري_نقل(iptr, جاچ->ptr, ڊيگهائي);
#جيڪڏهن_پورو
		جاچ->ptr += جاچ->inode_سائيز;
		جاچ->بائيٽون_کاٻو -= جاچ->inode_سائيز;
		إذا (جاچ->جاچ_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_بلاڪ_اندريون_INODE_جدول;
	}
	إذا ((iblock_حالت[iblk] & IBLOCK_حالت_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	جاچ->inodes_کاٻو--;
	جاچ->current_inode++;
	*ino = جاچ->current_inode;
	إذا (iptr != (بنية ext2_inode_large *)inode) {
		يادگيري_نقل(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	أرجع retval;
}

errcode_t ext2fs_get_ايندڙ_inode(ext2_inode_جاچ جاچ, ext2_ino_t *ino,
				بنية ext2_inode *inode)
{
	أرجع ext2fs_get_ايندڙ_inode_ڀريل(جاچ, ino, inode,
						حجم(بنية ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode * inode, صحيح bufsize,
			     صحيح flags)
{
	blk64_t		بلاڪ_nr;
	dgrp_t		گروهه;
	بنانشان ڊگهو 	بلاڪ, وٿي;
	حرف 		*ptr;
	errcode_t	retval;
	بنانشان	i;
	صحيح		clen, inodes_في_بلاڪ;
	io_channel	io;
	صحيح		ڊيگهائي = EXT2_INODE_سائيز(fs->super);
	بنية ext2_inode_large	*iptr;
	صحيح		cache_جڳهه, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	إذا (fs->read_inode &&
	    ((bufsize == حجم(بنية ext2_inode)) ||
	     (EXT2_INODE_سائيز(fs->super) == حجم(بنية ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}
	إذا ((ino == 0) || (ino > fs->super->s_inodes_ڳڻپ))
		أرجع EXT2_ET_BAD_INODE_انگ;
	/* Create inode cache if not present */
	إذا (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		إذا (retval)
			أرجع retval;
	}
	/* Check to see if it's in the inode cache */
	لكل (i = 0; i < fs->icache->cache_سائيز; i++) {
		إذا (fs->icache->cache[i].ino == ino) {
			يادگيري_نقل(inode, fs->icache->cache[i].inode,
			       (bufsize > ڊيگهائي) ? ڊيگهائي : bufsize);
			أرجع 0;
		}
	}
	إذا (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_في_بلاڪ = fs->blocksize / EXT2_INODE_سائيز(fs->super);
		بلاڪ_nr = ext2fs_le32_منزل_cpu(fs->image_سرو->وٿي_inode) / fs->blocksize;
		بلاڪ_nr += (ino - 1) / inodes_في_بلاڪ;
		وٿي = ((ino - 1) % inodes_في_بلاڪ) *
			EXT2_INODE_سائيز(fs->super);
		io = fs->image_io;
	} وإلا {
		گروهه = (ino - 1) / EXT2_INODES_في_گروهه(fs->super);
		إذا (گروهه > fs->گروهه_desc_ڳڻپ)
			أرجع EXT2_ET_BAD_INODE_انگ;
		وٿي = ((ino - 1) % EXT2_INODES_في_گروهه(fs->super)) *
			EXT2_INODE_سائيز(fs->super);
		بلاڪ = وٿي >> EXT2_بلاڪ_سائيز_بٽ(fs->super);
		بلاڪ_nr = ext2fs_inode_جدول_loc(fs, گروهه);
		إذا (!بلاڪ_nr)
			أرجع EXT2_ET_MISSING_INODE_جدول;
		إذا ((بلاڪ_nr < fs->super->s_پهريون_معلومات_بلاڪ) ||
		    (بلاڪ_nr + fs->inode_بلاڪون_في_گروهه - 1 >=
		     ext2fs_بلاڪون_ڳڻپ(fs->super)))
			أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
		بلاڪ_nr += بلاڪ;
		io = fs->io;
	}
	وٿي &= (EXT2_بلاڪ_سائيز(fs->super) - 1);

	cache_جڳهه = (fs->icache->cache_آخري + 1) % fs->icache->cache_سائيز;
	iptr = (بنية ext2_inode_large *)fs->icache->cache[cache_جڳهه].inode;

	ptr = (حرف *) iptr;
	طالما (ڊيگهائي) {
		clen = ڊيگهائي;
		إذا ((وٿي + ڊيگهائي) > fs->blocksize)
			clen = fs->blocksize - وٿي;

		إذا (بلاڪ_nr != fs->icache->بفر_blk) {
			retval = io_channel_read_blk64(io, بلاڪ_nr, 1,
						     fs->icache->بفر);
			إذا (retval)
				أرجع retval;
			fs->icache->بفر_blk = بلاڪ_nr;
		}

		يادگيري_نقل(ptr, ((حرف *) fs->icache->بفر) + (بنانشان) وٿي,
		       clen);

		وٿي = 0;
		ڊيگهائي -= clen;
		ptr += clen;
		بلاڪ_nr++;
	}
	ڊيگهائي = EXT2_INODE_سائيز(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	ext2fs_مٽا_سٽا_inode_ڀريل(fs, (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) iptr,
			       0, ڊيگهائي);
#جيڪڏهن_پورو

	/* Update the inode cache bookkeeping */
	إذا (!fail_csum) {
		fs->icache->cache_آخري = cache_جڳهه;
		fs->icache->cache[cache_جڳهه].ino = ino;
	}
	يادگيري_نقل(inode, iptr, (bufsize > ڊيگهائي) ? ڊيگهائي : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		أرجع EXT2_ET_INODE_CSUM_INVALID;

	أرجع 0;
}

errcode_t ext2fs_read_inode_ڀريل(ext2_filsys fs, ext2_ino_t ino,
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
	blk64_t بلاڪ_nr;
	dgrp_t گروهه;
	بنانشان ڊگهو بلاڪ, وٿي;
	errcode_t retval = 0;
	بنية ext2_inode_large *w_inode;
	حرف *ptr;
	بنانشان i;
	صحيح clen;
	صحيح ڊيگهائي = EXT2_INODE_سائيز(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	إذا (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}

	إذا ((ino == 0) || (ino > fs->super->s_inodes_ڳڻپ))
		أرجع EXT2_ET_BAD_INODE_انگ;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(ڊيگهائي, &w_inode);
	إذا (retval)
		أرجع retval;

	إذا (bufsize < ڊيگهائي) {
		retval = ext2fs_read_inode2(fs, ino,
					    (بنية ext2_inode *)w_inode,
					    ڊيگهائي, READ_INODE_NOCSUM);
		إذا (retval)
			اذهب errout;
	}

	/* Check to see if the inode cache needs to be updated */
	إذا (fs->icache) {
		لكل (i=0; i < fs->icache->cache_سائيز; i++) {
			إذا (fs->icache->cache[i].ino == ino) {
				يادگيري_نقل(fs->icache->cache[i].inode, inode,
				       (bufsize > ڊيگهائي) ? ڊيگهائي : bufsize);
				اكسر;
			}
		}
	} وإلا {
		retval = ext2fs_create_inode_cache(fs, 4);
		إذا (retval)
			اذهب errout;
	}
	يادگيري_نقل(w_inode, inode, (bufsize > ڊيگهائي) ? ڊيگهائي : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		اذهب errout;
	}

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	ext2fs_مٽا_سٽا_inode_ڀريل(fs, w_inode, w_inode, 1, ڊيگهائي);
#جيڪڏهن_پورو

	إذا ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_سيٽ(fs, ino, w_inode);
		إذا (retval)
			اذهب errout;
	}

	گروهه = (ino - 1) / EXT2_INODES_في_گروهه(fs->super);
	وٿي = ((ino - 1) % EXT2_INODES_في_گروهه(fs->super)) *
		EXT2_INODE_سائيز(fs->super);
	بلاڪ = وٿي >> EXT2_بلاڪ_سائيز_بٽ(fs->super);
	بلاڪ_nr = ext2fs_inode_جدول_loc(fs, (بنانشان) گروهه);
	إذا (!بلاڪ_nr) {
		retval = EXT2_ET_MISSING_INODE_جدول;
		اذهب errout;
	}
	إذا ((بلاڪ_nr < fs->super->s_پهريون_معلومات_بلاڪ) ||
	    (بلاڪ_nr + fs->inode_بلاڪون_في_گروهه - 1 >=
	     ext2fs_بلاڪون_ڳڻپ(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_جدول;
		اذهب errout;
	}
	بلاڪ_nr += بلاڪ;

	وٿي &= (EXT2_بلاڪ_سائيز(fs->super) - 1);

	ptr = (حرف *) w_inode;

	طالما (ڊيگهائي) {
		clen = ڊيگهائي;
		إذا ((وٿي + ڊيگهائي) > fs->blocksize)
			clen = fs->blocksize - وٿي;

		إذا (fs->icache->بفر_blk != بلاڪ_nr) {
			retval = io_channel_read_blk64(fs->io, بلاڪ_nr, 1,
						     fs->icache->بفر);
			إذا (retval)
				اذهب errout;
			fs->icache->بفر_blk = بلاڪ_nr;
		}


		يادگيري_نقل((حرف *) fs->icache->بفر + (بنانشان) وٿي,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, بلاڪ_nr, 1,
					      fs->icache->بفر);
		إذا (retval)
			اذهب errout;

		وٿي = 0;
		ptr += clen;
		ڊيگهائي -= clen;
		بلاڪ_nr++;
	}

	fs->flags |= EXT2_FLAG_بدليو;
errout:
	ext2fs_free_mem(&w_inode);
	أرجع retval;
}

errcode_t ext2fs_write_inode_ڀريل(ext2_filsys fs, ext2_ino_t ino,
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
	صحيح			سائيز = EXT2_INODE_سائيز(fs->super);
	بنية ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fs_P_get_وقت(fs);

	إذا (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_سيٽ(inode, i_atime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_سيٽ(inode, i_ctime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_سيٽ(inode, i_mtime, t);

	إذا (سائيز == حجم(بنية ext2_inode))
		أرجع ext2fs_write_inode_ڀريل(fs, ino, inode,
					       حجم(بنية ext2_inode));

	buf = يادگيري_ڏيو(سائيز);
	إذا (!buf)
		أرجع ENOMEM;

	memset(buf, 0, سائيز);
	*buf = *inode;

	large_inode = (بنية ext2_inode_large *) buf;
	large_inode->i_extra_isize = حجم(بنية ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_سائيز;
	إذا (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_ڀريل(fs, ino, buf, سائيز);
	حرر(buf);
	أرجع retval;
}


errcode_t ext2fs_get_بلاڪون(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاڪون)
{
	بنية ext2_inode	inode;
	صحيح			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ino > fs->super->s_inodes_ڳڻپ)
		أرجع EXT2_ET_BAD_INODE_انگ;

	إذا (fs->get_بلاڪون) {
		إذا (!(*fs->get_بلاڪون)(fs, ino, بلاڪون))
			أرجع 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	لكل (i=0; i < EXT2_N_بلاڪون; i++)
		بلاڪون[i] = inode.i_بلاڪ[i];
	أرجع 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	بنية	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ino > fs->super->s_inodes_ڳڻپ)
		أرجع EXT2_ET_BAD_INODE_انگ;

	إذا (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		إذا (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			أرجع retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	إذا (!LINUX_S_ISDIR(inode.i_گهڻائي))
		أرجع EXT2_ET_NO_DIRECTORY;
	أرجع 0;
}

