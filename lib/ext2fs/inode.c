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
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "e2image.h"

#تعریف_کرو IBLOCK_حالت_CSUMS_OK	1
#تعریف_کرو IBLOCK_حالت_INSANE	2
#تعریف_کرو جانچ_بلاک_حالت(جانچ)	((جانچ)->عارضی_بفر + (جانچ)->inode_ناپ)

بنية ext2_struct_inode_جانچ {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_بلاک;
	dgrp_t			current_گروہ;
	ext2_ino_t		inodes_بایاں;
	blk_t			بلاکاں_بایاں;
	dgrp_t			groups_بایاں;
	blk_t			inode_بفر_بلاکاں;
	حرف *			inode_بفر;
	صحيح			inode_ناپ;
	حرف *			ptr;
	صحيح			بائٹاں_بایاں;
	حرف			*عارضی_بفر;
	errcode_t		(*پورا_گروہ)(ext2_filsys fs,
					      ext2_inode_جانچ جانچ,
					      dgrp_t گروہ,
					      فراغ * priv_انکڑے);
	فراغ *			پورا_گروہ_انکڑے;
	صحيح			bad_بلاک_ptr;
	صحيح			جانچ_flags;
	صحيح			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	بنانشانی	i;

	إذا (!fs->icache)
		أرجع 0;

	لكل (i=0; i < fs->icache->cache_ناپ; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->بفر_blk = 0;
	أرجع 0;
}

/*
 * Free the inode cache structure
 */
فراغ ext2fs_free_inode_cache(بنية ext2_inode_cache *icache)
{
	بنانشانی i;

	إذا (--icache->refcount)
		أرجع;
	إذا (icache->بفر)
		ext2fs_free_mem(&icache->بفر);
	لكل (i = 0; i < icache->cache_ناپ; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	إذا (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->بفر_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, بنانشانی صحيح cache_ناپ)
{
	بنانشانی	i;
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
	fs->icache->cache_آخری = -1;
	fs->icache->cache_ناپ = cache_ناپ;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_ناپ,
				  حجم(بنية ext2_inode_cache_ent),
				  &fs->icache->cache);
	إذا (retval)
		اذهب errout;

	لكل (i = 0; i < fs->icache->cache_ناپ; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_ناپ(fs->super),
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

errcode_t ext2fs_open_inode_جانچ(ext2_filsys fs, صحيح بفر_بلاکاں,
				 ext2_inode_جانچ *ret_جانچ)
{
	ext2_inode_جانچ	جانچ;
	errcode_t	retval;
	errcode_t (*save_get_بلاکاں)(ext2_filsys f, ext2_ino_t ino, blk_t *بلاکاں);

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
		save_get_بلاکاں = fs->get_بلاکاں;
		fs->get_بلاکاں = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		إذا (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_بلاکاں = save_get_بلاکاں;
	}

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_inode_جانچ), &جانچ);
	إذا (retval)
		أرجع retval;
	memset(جانچ, 0, حجم(بنية ext2_struct_inode_جانچ));

	جانچ->magic = EXT2_ET_MAGIC_INODE_جانچ;
	جانچ->fs = fs;
	جانچ->inode_ناپ = EXT2_INODE_ناپ(fs->super);
	جانچ->بائٹاں_بایاں = 0;
	جانچ->current_گروہ = 0;
	جانچ->groups_بایاں = fs->گروہ_desc_گنتی - 1;
	جانچ->inode_بفر_بلاکاں = بفر_بلاکاں ? بفر_بلاکاں :
				    EXT2_INODE_جانچ_DEFAULT_بفر_بلاکاں;
	جانچ->current_بلاک = ext2fs_inode_جدول_loc(جانچ->fs,
						     جانچ->current_گروہ);
	إذا (جانچ->current_بلاک &&
	    ((جانچ->current_بلاک < fs->super->s_پہلا_انکڑے_بلاک) ||
	     (جانچ->current_بلاک + fs->inode_بلاکاں_فی_گروہ - 1 >=
	      ext2fs_بلاکاں_گنتی(fs->super)))) {
		ext2fs_free_mem(&جانچ);
		أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
	}

	جانچ->inodes_بایاں = EXT2_INODES_فی_گروہ(جانچ->fs->super);
	جانچ->بلاکاں_بایاں = جانچ->fs->inode_بلاکاں_فی_گروہ;
	إذا (ext2fs_has_گروہ_desc_csum(fs)) {
		__u32 بن_ورتیا = ext2fs_bg_itable_بن_ورتیا(fs, جانچ->current_گروہ);
		إذا (جانچ->inodes_بایاں > بن_ورتیا)
			جانچ->inodes_بایاں -= بن_ورتیا;
		وإلا
			جانچ->inodes_بایاں = 0;
		جانچ->بلاکاں_بایاں =
			(جانچ->inodes_بایاں +
			 (fs->blocksize / جانچ->inode_ناپ - 1)) *
			جانچ->inode_ناپ / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, جانچ->inode_بفر_بلاکاں,
				      &جانچ->inode_بفر);
	جانچ->پورا_گروہ = 0;
	جانچ->پورا_گروہ_انکڑے = 0;
	جانچ->bad_بلاک_ptr = 0;
	إذا (retval) {
		ext2fs_free_mem(&جانچ);
		أرجع retval;
	}
	retval = ext2fs_get_mem(جانچ->inode_ناپ + جانچ->inode_بفر_بلاکاں,
				&جانچ->عارضی_بفر);
	إذا (retval) {
		ext2fs_free_mem(&جانچ->inode_بفر);
		ext2fs_free_mem(&جانچ);
		أرجع retval;
	}
	memset(جانچ_بلاک_حالت(جانچ), 0, جانچ->inode_بفر_بلاکاں);
	إذا (جانچ->fs->badblocks && جانچ->fs->badblocks->عدد)
		جانچ->جانچ_flags |= EXT2_SF_CHK_BADBLOCKS;
	إذا (ext2fs_has_گروہ_desc_csum(fs))
		جانچ->جانچ_flags |= EXT2_SF_DO_LAZY;
	*ret_جانچ = جانچ;
	أرجع 0;
}

فراغ ext2fs_close_inode_جانچ(ext2_inode_جانچ جانچ)
{
	إذا (!جانچ || (جانچ->magic != EXT2_ET_MAGIC_INODE_جانچ))
		أرجع;

	ext2fs_free_mem(&جانچ->inode_بفر);
	جانچ->inode_بفر = عدم;
	ext2fs_free_mem(&جانچ->عارضی_بفر);
	جانچ->عارضی_بفر = عدم;
	ext2fs_free_mem(&جانچ);
	أرجع;
}

فراغ ext2fs_سیٹ_inode_callback(ext2_inode_جانچ جانچ,
			       errcode_t (*پورا_گروہ)(ext2_filsys fs,
						       ext2_inode_جانچ جانچ,
						       dgrp_t گروہ,
						       فراغ * priv_انکڑے),
			       فراغ *پورا_گروہ_انکڑے)
{
	إذا (!جانچ || (جانچ->magic != EXT2_ET_MAGIC_INODE_جانچ))
		أرجع;

	جانچ->پورا_گروہ = پورا_گروہ;
	جانچ->پورا_گروہ_انکڑے = پورا_گروہ_انکڑے;
}

صحيح ext2fs_inode_جانچ_flags(ext2_inode_جانچ جانچ, صحيح سیٹ_flags,
			    صحيح clear_flags)
{
	صحيح	old_flags;

	إذا (!جانچ || (جانچ->magic != EXT2_ET_MAGIC_INODE_جانچ))
		أرجع 0;

	old_flags = جانچ->جانچ_flags;
	جانچ->جانچ_flags &= ~clear_flags;
	جانچ->جانچ_flags |= سیٹ_flags;
	أرجع old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ساكن errcode_t get_اگلا_blockgroup(ext2_inode_جانچ جانچ)
{
	ext2_filsys fs = جانچ->fs;

	جانچ->current_گروہ++;
	جانچ->groups_بایاں--;

	جانچ->current_بلاک = ext2fs_inode_جدول_loc(جانچ->fs,
						     جانچ->current_گروہ);
	جانچ->current_inode = جانچ->current_گروہ *
		EXT2_INODES_فی_گروہ(fs->super);

	جانچ->بائٹاں_بایاں = 0;
	جانچ->inodes_بایاں = EXT2_INODES_فی_گروہ(fs->super);
	جانچ->بلاکاں_بایاں = fs->inode_بلاکاں_فی_گروہ;
	إذا (ext2fs_has_گروہ_desc_csum(fs)) {
		__u32 بن_ورتیا = ext2fs_bg_itable_بن_ورتیا(fs, جانچ->current_گروہ);
		إذا (جانچ->inodes_بایاں > بن_ورتیا)
			جانچ->inodes_بایاں -= بن_ورتیا;
		وإلا
			جانچ->inodes_بایاں = 0;
		جانچ->بلاکاں_بایاں =
			(جانچ->inodes_بایاں +
			 (fs->blocksize / جانچ->inode_ناپ - 1)) *
			جانچ->inode_ناپ / fs->blocksize;
	}
	إذا (جانچ->current_بلاک &&
	    ((جانچ->current_بلاک < fs->super->s_پہلا_انکڑے_بلاک) ||
	     (جانچ->current_بلاک + fs->inode_بلاکاں_فی_گروہ - 1 >=
	      ext2fs_بلاکاں_گنتی(fs->super))))
		أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
	أرجع 0;
}

errcode_t ext2fs_inode_جانچ_goto_blockgroup(ext2_inode_جانچ جانچ,
					    صحيح	گروہ)
{
	جانچ->current_گروہ = گروہ - 1;
	جانچ->groups_بایاں = جانچ->fs->گروہ_desc_گنتی - گروہ;
	جانچ->bad_بلاک_ptr = 0;
	أرجع get_اگلا_blockgroup(جانچ);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ساكن errcode_t check_for_inode_bad_بلاکاں(ext2_inode_جانچ جانچ,
					    blk64_t *عدد_بلاکاں)
{
	blk64_t	blk = جانچ->current_بلاک;
	badblocks_list	bb = جانچ->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	إذا (blk == 0)
		أرجع 0;

	/* Make sure bad_block_ptr is still valid */
	إذا (جانچ->bad_بلاک_ptr >= bb->عدد) {
		جانچ->جانچ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	طالما (blk > bb->list[جانچ->bad_بلاک_ptr]) {
		إذا (++جانچ->bad_بلاک_ptr >= bb->عدد) {
			جانچ->جانچ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
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
	إذا (blk == bb->list[جانچ->bad_بلاک_ptr]) {
		جانچ->جانچ_flags |= EXT2_SF_BAD_INODE_BLK;
		*عدد_بلاکاں = 1;
		إذا (++جانچ->bad_بلاک_ptr >= bb->عدد)
			جانچ->جانچ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		أرجع 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	إذا ((blk + *عدد_بلاکاں) > bb->list[جانچ->bad_بلاک_ptr])
		*عدد_بلاکاں = (صحيح) (bb->list[جانچ->bad_بلاک_ptr] - blk);

	أرجع 0;
}

ساكن صحيح بلاک_map_looks_insane(ext2_filsys fs,
				  بنية ext2_inode_large *inode)
{
	بنانشانی صحيح i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	إذا ((inode->i_flags & EXT4_INLINE_انکڑے_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		أرجع 0;
	إذا (!LINUX_S_ISREG(inode->i_منوال) &&
	    !LINUX_S_ISLNK(inode->i_منوال) &&
	    !LINUX_S_ISDIR(inode->i_منوال))
		أرجع 0;
	إذا (LINUX_S_ISLNK(inode->i_منوال) &&
	    EXT2_I_ناپ(inode) <= حجم(inode->i_بلاک))
		أرجع 0;

	/* Unused inodes probably aren't insane */
	إذا (inode->i_links_گنتی == 0)
		أرجع 0;

	/* See if more than half the block maps are insane */
	لكل (i = 0, bad = 0; i < EXT2_N_بلاکاں; i++)
		إذا (inode->i_بلاک[i] != 0 &&
		    (inode->i_بلاک[i] < fs->super->s_پہلا_انکڑے_بلاک ||
		     inode->i_بلاک[i] >= ext2fs_بلاکاں_گنتی(fs->super)))
			bad++;
	أرجع bad > EXT2_N_بلاکاں / 2;
}

ساكن صحيح extent_سر_looks_insane(بنية ext2_inode_large *inode)
{
	إذا (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_سرخی_verify(inode->i_بلاک,
					حجم(inode->i_بلاک)) == 0)
		أرجع 0;
	أرجع 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ساكن فراغ check_inode_بلاک_sanity(ext2_inode_جانچ جانچ, blk64_t عدد_بلاکاں)
{
	ext2_ino_t	ino, inodes_منزل_جانچ;
	بنانشانی صحيح	badness, checksum_failures;
	بنانشانی صحيح	inodes_وچ_buf, inodes_فی_بلاک;
	حرف		*p;
	بنية ext2_inode_large *inode;
	حرف		*بلاک_حالت;
	بنانشانی صحيح	blk, bad_csum;

	إذا (!(جانچ->جانچ_flags & EXT2_SF_WARN_GARBAGE_INODES))
		أرجع;

	inodes_منزل_جانچ = جانچ->inodes_بایاں;
	inodes_وچ_buf = عدد_بلاکاں * جانچ->fs->blocksize / جانچ->inode_ناپ;
	إذا (inodes_منزل_جانچ > inodes_وچ_buf)
		inodes_منزل_جانچ = inodes_وچ_buf;

	p = (حرف *) جانچ->inode_بفر;
	ino = جانچ->current_inode + 1;
	checksum_failures = badness = 0;
	بلاک_حالت = جانچ_بلاک_حالت(جانچ);
	memset(بلاک_حالت, 0, جانچ->inode_بفر_بلاکاں);
	inodes_فی_بلاک = EXT2_INODES_فی_بلاک(جانچ->fs->super);

	إذا (inodes_فی_بلاک < 2)
		أرجع;

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	إذا (ext2fs_get_mem(EXT2_INODE_ناپ(جانچ->fs->super), &inode))
		أرجع;
#جے_انت

	طالما (inodes_منزل_جانچ > 0) {
		blk = (p - (حرف *)جانچ->inode_بفر) / جانچ->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(جانچ->fs, ino,
				(بنية ext2_inode_large *) p) == 0;

#جے_تعریف_شدہ الفاظ_BIGENDIAN
		ext2fs_وٹاندرا_inode_بھریا(جانچ->fs,
			       (بنية ext2_inode_large *) inode,
			       (بنية ext2_inode_large *) p,
			       0, EXT2_INODE_ناپ(جانچ->fs->super));
#وإلا
		inode = (بنية ext2_inode_large *) p;
#جے_انت

		/* Is this inode insane? */
		إذا (bad_csum) {
			checksum_failures++;
			badness++;
		} وإلا إذا (extent_سر_looks_insane(inode) ||
			   بلاک_map_looks_insane(جانچ->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		إذا (badness > inodes_فی_بلاک / 2) {
			بنانشانی صحيح ino_adj;

			بلاک_حالت[blk] |= IBLOCK_حالت_INSANE;
			ino_adj = inodes_فی_بلاک -
						((ino - 1) % inodes_فی_بلاک);
			إذا (ino_adj > inodes_منزل_جانچ)
				ino_adj = inodes_منزل_جانچ;
			inodes_منزل_جانچ -= ino_adj;
			p += جانچ->inode_ناپ * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			استمر;
		}

		إذا ((ino % inodes_فی_بلاک) == 0) {
			إذا (checksum_failures == 0)
				بلاک_حالت[blk] |= IBLOCK_حالت_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_منزل_جانچ--;
		p += جانچ->inode_ناپ;
		ino++;
	};

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_free_mem(&inode);
#جے_انت
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ساكن errcode_t get_اگلا_بلاکاں(ext2_inode_جانچ جانچ)
{
	blk64_t		عدد_بلاکاں;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	عدد_بلاکاں = جانچ->inode_بفر_بلاکاں;
	إذا (عدد_بلاکاں > جانچ->بلاکاں_بایاں)
		عدد_بلاکاں = جانچ->بلاکاں_بایاں;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	إذا (جانچ->جانچ_flags & EXT2_SF_BAD_INODE_BLK) {
		إذا (جانچ->بائٹاں_بایاں)
			جانچ->جانچ_flags |= EXT2_SF_BAD_EXTRA_بائٹاں;
		جانچ->جانچ_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	إذا (جانچ->جانچ_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_بلاکاں(جانچ, &عدد_بلاکاں);
		إذا (retval)
			أرجع retval;
	}

	إذا ((جانچ->جانچ_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (جانچ->current_بلاک == 0)) {
		memset(جانچ->inode_بفر, 0,
		       (size_t) عدد_بلاکاں * جانچ->fs->blocksize);
	} وإلا {
		retval = io_channel_read_blk64(جانچ->fs->io,
					     جانچ->current_بلاک,
					     (صحيح) عدد_بلاکاں,
					     جانچ->inode_بفر);
		إذا (retval)
			أرجع EXT2_ET_اگلا_INODE_READ;
	}
	check_inode_بلاک_sanity(جانچ, عدد_بلاکاں);

	جانچ->ptr = جانچ->inode_بفر;
	جانچ->بائٹاں_بایاں = عدد_بلاکاں * جانچ->fs->blocksize;

	جانچ->بلاکاں_بایاں -= عدد_بلاکاں;
	إذا (جانچ->current_بلاک)
		جانچ->current_بلاک += عدد_بلاکاں;

	أرجع 0;
}

#إذا 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ساكن مضمن صحيح is_خالی_تھاواں_جانچ(ext2_inode_جانچ جانچ)
{
	صحيح	i;

	إذا (جانچ->بائٹاں_بایاں == 0)
		أرجع 0;

	لكل (i=0; i < جانچ->بائٹاں_بایاں; i++)
		إذا (جانچ->ptr[i])
			أرجع 0;
	أرجع 1;
}
#جے_انت

errcode_t ext2fs_get_اگلا_inode_بھریا(ext2_inode_جانچ جانچ, ext2_ino_t *ino,
				     بنية ext2_inode *inode, صحيح bufsize)
{
	errcode_t	retval;
	صحيح		extra_بائٹاں = 0;
	صحيح		لمائی;
	بنية ext2_inode_large	*iptr = (بنية ext2_inode_large *)inode;
	حرف		*iblock_حالت;
	بنانشانی صحيح	iblk;

	EXT2_CHECK_MAGIC(جانچ, EXT2_ET_MAGIC_INODE_جانچ);
	لمائی = EXT2_INODE_ناپ(جانچ->fs->super);
	iblock_حالت = جانچ_بلاک_حالت(جانچ);

	/*
	 * Do we need to start reading a new block group?
	 */
	إذا (جانچ->inodes_بایاں <= 0) {
	force_new_گروہ:
		إذا (جانچ->پورا_گروہ) {
			retval = (جانچ->پورا_گروہ)
				(جانچ->fs, جانچ, جانچ->current_گروہ,
				 جانچ->پورا_گروہ_انکڑے);
			إذا (retval)
				أرجع retval;
		}
		إذا (جانچ->groups_بایاں <= 0) {
			*ino = 0;
			أرجع 0;
		}
		retval = get_اگلا_blockgroup(جانچ);
		إذا (retval)
			أرجع retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	إذا ((جانچ->جانچ_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(جانچ->fs, جانچ->current_گروہ, EXT2_BG_INODE_UNINIT)
	     ))
		اذهب force_new_گروہ;
	إذا (جانچ->inodes_بایاں == 0)
		اذهب force_new_گروہ;
	إذا (جانچ->current_بلاک == 0) {
		إذا (جانچ->جانچ_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			اذهب force_new_گروہ;
		} وإلا
			أرجع EXT2_ET_MISSING_INODE_جدول;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	إذا (جانچ->بائٹاں_بایاں < جانچ->inode_ناپ) {
		إذا (جانچ->بائٹاں_بایاں)
			حافظہ_نقل(جانچ->عارضی_بفر, جانچ->ptr, جانچ->بائٹاں_بایاں);
		extra_بائٹاں = جانچ->بائٹاں_بایاں;

		retval = get_اگلا_بلاکاں(جانچ);
		إذا (retval)
			أرجع retval;
#إذا 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		إذا (is_خالی_تھاواں_جانچ(جانچ))
			اذهب force_new_گروہ;
#جے_انت
	}

	إذا (bufsize < لمائی) {
		retval = ext2fs_get_mem(لمائی, &iptr);
		إذا (retval)
			أرجع retval;
	}

	retval = 0;
	iblk = جانچ->current_inode % EXT2_INODES_فی_گروہ(جانچ->fs->super) /
				EXT2_INODES_فی_بلاک(جانچ->fs->super) %
				جانچ->inode_بفر_بلاکاں;
	إذا (extra_بائٹاں) {
		حافظہ_نقل(جانچ->عارضی_بفر+extra_بائٹاں, جانچ->ptr,
		       جانچ->inode_ناپ - extra_بائٹاں);
		جانچ->ptr += جانچ->inode_ناپ - extra_بائٹاں;
		جانچ->بائٹاں_بایاں -= جانچ->inode_ناپ - extra_بائٹاں;

		/* Verify the inode checksum. */
		إذا (!(iblock_حالت[iblk] & IBLOCK_حالت_CSUMS_OK) &&
		    !(جانچ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(جانچ->fs, جانچ->current_inode + 1,
				(بنية ext2_inode_large *)جانچ->عارضی_بفر))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#جے_تعریف_شدہ الفاظ_BIGENDIAN
		memset(iptr, 0, لمائی);
		ext2fs_وٹاندرا_inode_بھریا(جانچ->fs,
			       (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) جانچ->عارضی_بفر,
			       0, لمائی);
#وإلا
		حافظہ_نقل(iptr, جانچ->عارضی_بفر, لمائی);
#جے_انت
		إذا (جانچ->جانچ_flags & EXT2_SF_BAD_EXTRA_بائٹاں)
			retval = EXT2_ET_BAD_بلاک_وچ_INODE_جدول;
		جانچ->جانچ_flags &= ~EXT2_SF_BAD_EXTRA_بائٹاں;
	} وإلا {
		/* Verify the inode checksum. */
		إذا (!(iblock_حالت[iblk] & IBLOCK_حالت_CSUMS_OK) &&
		    !(جانچ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(جانچ->fs, جانچ->current_inode + 1,
				(بنية ext2_inode_large *)جانچ->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#جے_تعریف_شدہ الفاظ_BIGENDIAN
		memset(iptr, 0, لمائی);
		ext2fs_وٹاندرا_inode_بھریا(جانچ->fs,
				(بنية ext2_inode_large *) iptr,
				(بنية ext2_inode_large *) جانچ->ptr,
				0, لمائی);
#وإلا
		حافظہ_نقل(iptr, جانچ->ptr, لمائی);
#جے_انت
		جانچ->ptr += جانچ->inode_ناپ;
		جانچ->بائٹاں_بایاں -= جانچ->inode_ناپ;
		إذا (جانچ->جانچ_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_بلاک_وچ_INODE_جدول;
	}
	إذا ((iblock_حالت[iblk] & IBLOCK_حالت_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	جانچ->inodes_بایاں--;
	جانچ->current_inode++;
	*ino = جانچ->current_inode;
	إذا (iptr != (بنية ext2_inode_large *)inode) {
		حافظہ_نقل(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	أرجع retval;
}

errcode_t ext2fs_get_اگلا_inode(ext2_inode_جانچ جانچ, ext2_ino_t *ino,
				بنية ext2_inode *inode)
{
	أرجع ext2fs_get_اگلا_inode_بھریا(جانچ, ino, inode,
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
	dgrp_t		گروہ;
	بنانشانی لما 	بلاک, وتھ;
	حرف 		*ptr;
	errcode_t	retval;
	بنانشانی	i;
	صحيح		clen, inodes_فی_بلاک;
	io_channel	io;
	صحيح		لمائی = EXT2_INODE_ناپ(fs->super);
	بنية ext2_inode_large	*iptr;
	صحيح		cache_تھاں, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	إذا (fs->blocksize < 1024)
		أرجع EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	إذا (fs->read_inode &&
	    ((bufsize == حجم(بنية ext2_inode)) ||
	     (EXT2_INODE_ناپ(fs->super) == حجم(بنية ext2_inode)))) {
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
	لكل (i = 0; i < fs->icache->cache_ناپ; i++) {
		إذا (fs->icache->cache[i].ino == ino) {
			حافظہ_نقل(inode, fs->icache->cache[i].inode,
			       (bufsize > لمائی) ? لمائی : bufsize);
			أرجع 0;
		}
	}
	إذا (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_فی_بلاک = fs->blocksize / EXT2_INODE_ناپ(fs->super);
		بلاک_nr = ext2fs_le32_منزل_cpu(fs->image_سرخی->وتھ_inode) / fs->blocksize;
		بلاک_nr += (ino - 1) / inodes_فی_بلاک;
		وتھ = ((ino - 1) % inodes_فی_بلاک) *
			EXT2_INODE_ناپ(fs->super);
		io = fs->image_io;
	} وإلا {
		گروہ = (ino - 1) / EXT2_INODES_فی_گروہ(fs->super);
		إذا (گروہ > fs->گروہ_desc_گنتی)
			أرجع EXT2_ET_BAD_INODE_عدد;
		وتھ = ((ino - 1) % EXT2_INODES_فی_گروہ(fs->super)) *
			EXT2_INODE_ناپ(fs->super);
		بلاک = وتھ >> EXT2_بلاک_ناپ_بٹ(fs->super);
		بلاک_nr = ext2fs_inode_جدول_loc(fs, گروہ);
		إذا (!بلاک_nr)
			أرجع EXT2_ET_MISSING_INODE_جدول;
		إذا ((بلاک_nr < fs->super->s_پہلا_انکڑے_بلاک) ||
		    (بلاک_nr + fs->inode_بلاکاں_فی_گروہ - 1 >=
		     ext2fs_بلاکاں_گنتی(fs->super)))
			أرجع EXT2_ET_GDESC_BAD_INODE_جدول;
		بلاک_nr += بلاک;
		io = fs->io;
	}
	وتھ &= (EXT2_بلاک_ناپ(fs->super) - 1);

	cache_تھاں = (fs->icache->cache_آخری + 1) % fs->icache->cache_ناپ;
	iptr = (بنية ext2_inode_large *)fs->icache->cache[cache_تھاں].inode;

	ptr = (حرف *) iptr;
	طالما (لمائی) {
		clen = لمائی;
		إذا ((وتھ + لمائی) > fs->blocksize)
			clen = fs->blocksize - وتھ;

		إذا (بلاک_nr != fs->icache->بفر_blk) {
			retval = io_channel_read_blk64(io, بلاک_nr, 1,
						     fs->icache->بفر);
			إذا (retval)
				أرجع retval;
			fs->icache->بفر_blk = بلاک_nr;
		}

		حافظہ_نقل(ptr, ((حرف *) fs->icache->بفر) + (بنانشانی) وتھ,
		       clen);

		وتھ = 0;
		لمائی -= clen;
		ptr += clen;
		بلاک_nr++;
	}
	لمائی = EXT2_INODE_ناپ(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_وٹاندرا_inode_بھریا(fs, (بنية ext2_inode_large *) iptr,
			       (بنية ext2_inode_large *) iptr,
			       0, لمائی);
#جے_انت

	/* Update the inode cache bookkeeping */
	إذا (!fail_csum) {
		fs->icache->cache_آخری = cache_تھاں;
		fs->icache->cache[cache_تھاں].ino = ino;
	}
	حافظہ_نقل(inode, iptr, (bufsize > لمائی) ? لمائی : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		أرجع EXT2_ET_INODE_CSUM_INVALID;

	أرجع 0;
}

errcode_t ext2fs_read_inode_بھریا(ext2_filsys fs, ext2_ino_t ino,
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
	dgrp_t گروہ;
	بنانشانی لما بلاک, وتھ;
	errcode_t retval = 0;
	بنية ext2_inode_large *w_inode;
	حرف *ptr;
	بنانشانی i;
	صحيح clen;
	صحيح لمائی = EXT2_INODE_ناپ(fs->super);

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
	retval = ext2fs_get_mem(لمائی, &w_inode);
	إذا (retval)
		أرجع retval;

	إذا (bufsize < لمائی) {
		retval = ext2fs_read_inode2(fs, ino,
					    (بنية ext2_inode *)w_inode,
					    لمائی, READ_INODE_NOCSUM);
		إذا (retval)
			اذهب errout;
	}

	/* Check to see if the inode cache needs to be updated */
	إذا (fs->icache) {
		لكل (i=0; i < fs->icache->cache_ناپ; i++) {
			إذا (fs->icache->cache[i].ino == ino) {
				حافظہ_نقل(fs->icache->cache[i].inode, inode,
				       (bufsize > لمائی) ? لمائی : bufsize);
				اكسر;
			}
		}
	} وإلا {
		retval = ext2fs_create_inode_cache(fs, 4);
		إذا (retval)
			اذهب errout;
	}
	حافظہ_نقل(w_inode, inode, (bufsize > لمائی) ? لمائی : bufsize);

	إذا (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		اذهب errout;
	}

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_وٹاندرا_inode_بھریا(fs, w_inode, w_inode, 1, لمائی);
#جے_انت

	إذا ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_سیٹ(fs, ino, w_inode);
		إذا (retval)
			اذهب errout;
	}

	گروہ = (ino - 1) / EXT2_INODES_فی_گروہ(fs->super);
	وتھ = ((ino - 1) % EXT2_INODES_فی_گروہ(fs->super)) *
		EXT2_INODE_ناپ(fs->super);
	بلاک = وتھ >> EXT2_بلاک_ناپ_بٹ(fs->super);
	بلاک_nr = ext2fs_inode_جدول_loc(fs, (بنانشانی) گروہ);
	إذا (!بلاک_nr) {
		retval = EXT2_ET_MISSING_INODE_جدول;
		اذهب errout;
	}
	إذا ((بلاک_nr < fs->super->s_پہلا_انکڑے_بلاک) ||
	    (بلاک_nr + fs->inode_بلاکاں_فی_گروہ - 1 >=
	     ext2fs_بلاکاں_گنتی(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_جدول;
		اذهب errout;
	}
	بلاک_nr += بلاک;

	وتھ &= (EXT2_بلاک_ناپ(fs->super) - 1);

	ptr = (حرف *) w_inode;

	طالما (لمائی) {
		clen = لمائی;
		إذا ((وتھ + لمائی) > fs->blocksize)
			clen = fs->blocksize - وتھ;

		إذا (fs->icache->بفر_blk != بلاک_nr) {
			retval = io_channel_read_blk64(fs->io, بلاک_nr, 1,
						     fs->icache->بفر);
			إذا (retval)
				اذهب errout;
			fs->icache->بفر_blk = بلاک_nr;
		}


		حافظہ_نقل((حرف *) fs->icache->بفر + (بنانشانی) وتھ,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, بلاک_nr, 1,
					      fs->icache->بفر);
		إذا (retval)
			اذهب errout;

		وتھ = 0;
		ptr += clen;
		لمائی -= clen;
		بلاک_nr++;
	}

	fs->flags |= EXT2_FLAG_بدلیا;
errout:
	ext2fs_free_mem(&w_inode);
	أرجع retval;
}

errcode_t ext2fs_write_inode_بھریا(ext2_filsys fs, ext2_ino_t ino,
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
	صحيح			ناپ = EXT2_INODE_ناپ(fs->super);
	بنية ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fs_P_get_ویلا(fs);

	إذا (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_سیٹ(inode, i_atime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_سیٹ(inode, i_ctime, t);
	إذا (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_سیٹ(inode, i_mtime, t);

	إذا (ناپ == حجم(بنية ext2_inode))
		أرجع ext2fs_write_inode_بھریا(fs, ino, inode,
					       حجم(بنية ext2_inode));

	buf = حافظہ_دیو(ناپ);
	إذا (!buf)
		أرجع ENOMEM;

	memset(buf, 0, ناپ);
	*buf = *inode;

	large_inode = (بنية ext2_inode_large *) buf;
	large_inode->i_extra_isize = حجم(بنية ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_ناپ;
	إذا (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_بھریا(fs, ino, buf, ناپ);
	حرر(buf);
	أرجع retval;
}


errcode_t ext2fs_get_بلاکاں(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاکاں)
{
	بنية ext2_inode	inode;
	صحيح			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ino > fs->super->s_inodes_گنتی)
		أرجع EXT2_ET_BAD_INODE_عدد;

	إذا (fs->get_بلاکاں) {
		إذا (!(*fs->get_بلاکاں)(fs, ino, بلاکاں))
			أرجع 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	إذا (retval)
		أرجع retval;
	لكل (i=0; i < EXT2_N_بلاکاں; i++)
		بلاکاں[i] = inode.i_بلاک[i];
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
	إذا (!LINUX_S_ISDIR(inode.i_منوال))
		أرجع EXT2_ET_NO_DIRECTORY;
	أرجع 0;
}

