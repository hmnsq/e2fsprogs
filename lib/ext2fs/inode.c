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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "e2image.h"

#تعریف_کرو IBLOCK_حالت_CSUMS_OK	1
#تعریف_کرو IBLOCK_حالت_INSANE	2
#تعریف_کرو جائزہ_بلاک_حالت(جائزہ)	((جائزہ)->عارضی_بفر + (جائزہ)->inode_سائز)

ساخت ext2_struct_inode_جائزہ {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_بلاک;
	dgrp_t			current_گروہ;
	ext2_ino_t		inodes_بایاں;
	blk_t			بلاکیں_بایاں;
	dgrp_t			groups_بایاں;
	blk_t			inode_بفر_بلاکیں;
	حرف *			inode_بفر;
	صحیح			inode_سائز;
	حرف *			ptr;
	صحیح			بائٹیں_بایاں;
	حرف			*عارضی_بفر;
	errcode_t		(*مکمل_گروہ)(ext2_filsys fs,
					      ext2_inode_جائزہ جائزہ,
					      dgrp_t گروہ,
					      خالی * priv_ڈیٹا);
	خالی *			مکمل_گروہ_ڈیٹا;
	صحیح			bad_بلاک_ptr;
	صحیح			جائزہ_flags;
	صحیح			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	غیرعلامتی	i;

	اگر (!fs->icache)
		واپس 0;

	برائے (i=0; i < fs->icache->cache_سائز; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->بفر_blk = 0;
	واپس 0;
}

/*
 * Free the inode cache structure
 */
خالی ext2fs_free_inode_cache(ساخت ext2_inode_cache *icache)
{
	غیرعلامتی i;

	اگر (--icache->refcount)
		واپس;
	اگر (icache->بفر)
		ext2fs_free_mem(&icache->بفر);
	برائے (i = 0; i < icache->cache_سائز; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	اگر (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->بفر_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, غیرعلامتی صحیح cache_سائز)
{
	غیرعلامتی	i;
	errcode_t	retval;

	اگر (fs->icache)
		واپس 0;
	retval = ext2fs_get_mem(حجم(ساخت ext2_inode_cache), &fs->icache);
	اگر (retval)
		واپس retval;

	memset(fs->icache, 0, حجم(ساخت ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->بفر);
	اگر (retval)
		جاؤ errout;

	fs->icache->بفر_blk = 0;
	fs->icache->cache_آخری = -1;
	fs->icache->cache_سائز = cache_سائز;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_سائز,
				  حجم(ساخت ext2_inode_cache_ent),
				  &fs->icache->cache);
	اگر (retval)
		جاؤ errout;

	برائے (i = 0; i < fs->icache->cache_سائز; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_سائز(fs->super),
					&fs->icache->cache[i].inode);
		اگر (retval)
			جاؤ errout;
	}

	ext2fs_flush_icache(fs);
	واپس 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	واپس retval;
}

errcode_t ext2fs_open_inode_جائزہ(ext2_filsys fs, صحیح بفر_بلاکیں,
				 ext2_inode_جائزہ *ret_جائزہ)
{
	ext2_inode_جائزہ	جائزہ;
	errcode_t	retval;
	errcode_t (*save_get_بلاکیں)(ext2_filsys f, ext2_ino_t ino, blk_t *بلاکیں);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		واپس EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	اگر (fs->blocksize < 1024)
		واپس EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	اگر (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_بلاکیں = fs->get_بلاکیں;
		fs->get_بلاکیں = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		اگر (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_بلاکیں = save_get_بلاکیں;
	}

	retval = ext2fs_get_mem(حجم(ساخت ext2_struct_inode_جائزہ), &جائزہ);
	اگر (retval)
		واپس retval;
	memset(جائزہ, 0, حجم(ساخت ext2_struct_inode_جائزہ));

	جائزہ->magic = EXT2_ET_MAGIC_INODE_جائزہ;
	جائزہ->fs = fs;
	جائزہ->inode_سائز = EXT2_INODE_سائز(fs->super);
	جائزہ->بائٹیں_بایاں = 0;
	جائزہ->current_گروہ = 0;
	جائزہ->groups_بایاں = fs->گروہ_desc_گنتی - 1;
	جائزہ->inode_بفر_بلاکیں = بفر_بلاکیں ? بفر_بلاکیں :
				    EXT2_INODE_جائزہ_DEFAULT_بفر_بلاکیں;
	جائزہ->current_بلاک = ext2fs_inode_جدول_loc(جائزہ->fs,
						     جائزہ->current_گروہ);
	اگر (جائزہ->current_بلاک &&
	    ((جائزہ->current_بلاک < fs->super->s_پہلا_ڈیٹا_بلاک) ||
	     (جائزہ->current_بلاک + fs->inode_بلاکیں_فی_گروہ - 1 >=
	      ext2fs_بلاکیں_گنتی(fs->super)))) {
		ext2fs_free_mem(&جائزہ);
		واپس EXT2_ET_GDESC_BAD_INODE_جدول;
	}

	جائزہ->inodes_بایاں = EXT2_INODES_فی_گروہ(جائزہ->fs->super);
	جائزہ->بلاکیں_بایاں = جائزہ->fs->inode_بلاکیں_فی_گروہ;
	اگر (ext2fs_has_گروہ_desc_csum(fs)) {
		__u32 غیر_مستعمل = ext2fs_bg_itable_غیر_مستعمل(fs, جائزہ->current_گروہ);
		اگر (جائزہ->inodes_بایاں > غیر_مستعمل)
			جائزہ->inodes_بایاں -= غیر_مستعمل;
		ورنہ
			جائزہ->inodes_بایاں = 0;
		جائزہ->بلاکیں_بایاں =
			(جائزہ->inodes_بایاں +
			 (fs->blocksize / جائزہ->inode_سائز - 1)) *
			جائزہ->inode_سائز / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, جائزہ->inode_بفر_بلاکیں,
				      &جائزہ->inode_بفر);
	جائزہ->مکمل_گروہ = 0;
	جائزہ->مکمل_گروہ_ڈیٹا = 0;
	جائزہ->bad_بلاک_ptr = 0;
	اگر (retval) {
		ext2fs_free_mem(&جائزہ);
		واپس retval;
	}
	retval = ext2fs_get_mem(جائزہ->inode_سائز + جائزہ->inode_بفر_بلاکیں,
				&جائزہ->عارضی_بفر);
	اگر (retval) {
		ext2fs_free_mem(&جائزہ->inode_بفر);
		ext2fs_free_mem(&جائزہ);
		واپس retval;
	}
	memset(جائزہ_بلاک_حالت(جائزہ), 0, جائزہ->inode_بفر_بلاکیں);
	اگر (جائزہ->fs->badblocks && جائزہ->fs->badblocks->عدد)
		جائزہ->جائزہ_flags |= EXT2_SF_CHK_BADBLOCKS;
	اگر (ext2fs_has_گروہ_desc_csum(fs))
		جائزہ->جائزہ_flags |= EXT2_SF_DO_LAZY;
	*ret_جائزہ = جائزہ;
	واپس 0;
}

خالی ext2fs_close_inode_جائزہ(ext2_inode_جائزہ جائزہ)
{
	اگر (!جائزہ || (جائزہ->magic != EXT2_ET_MAGIC_INODE_جائزہ))
		واپس;

	ext2fs_free_mem(&جائزہ->inode_بفر);
	جائزہ->inode_بفر = تہی;
	ext2fs_free_mem(&جائزہ->عارضی_بفر);
	جائزہ->عارضی_بفر = تہی;
	ext2fs_free_mem(&جائزہ);
	واپس;
}

خالی ext2fs_سیٹ_inode_callback(ext2_inode_جائزہ جائزہ,
			       errcode_t (*مکمل_گروہ)(ext2_filsys fs,
						       ext2_inode_جائزہ جائزہ,
						       dgrp_t گروہ,
						       خالی * priv_ڈیٹا),
			       خالی *مکمل_گروہ_ڈیٹا)
{
	اگر (!جائزہ || (جائزہ->magic != EXT2_ET_MAGIC_INODE_جائزہ))
		واپس;

	جائزہ->مکمل_گروہ = مکمل_گروہ;
	جائزہ->مکمل_گروہ_ڈیٹا = مکمل_گروہ_ڈیٹا;
}

صحیح ext2fs_inode_جائزہ_flags(ext2_inode_جائزہ جائزہ, صحیح سیٹ_flags,
			    صحیح clear_flags)
{
	صحیح	old_flags;

	اگر (!جائزہ || (جائزہ->magic != EXT2_ET_MAGIC_INODE_جائزہ))
		واپس 0;

	old_flags = جائزہ->جائزہ_flags;
	جائزہ->جائزہ_flags &= ~clear_flags;
	جائزہ->جائزہ_flags |= سیٹ_flags;
	واپس old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ساکن errcode_t get_اگلا_blockgroup(ext2_inode_جائزہ جائزہ)
{
	ext2_filsys fs = جائزہ->fs;

	جائزہ->current_گروہ++;
	جائزہ->groups_بایاں--;

	جائزہ->current_بلاک = ext2fs_inode_جدول_loc(جائزہ->fs,
						     جائزہ->current_گروہ);
	جائزہ->current_inode = جائزہ->current_گروہ *
		EXT2_INODES_فی_گروہ(fs->super);

	جائزہ->بائٹیں_بایاں = 0;
	جائزہ->inodes_بایاں = EXT2_INODES_فی_گروہ(fs->super);
	جائزہ->بلاکیں_بایاں = fs->inode_بلاکیں_فی_گروہ;
	اگر (ext2fs_has_گروہ_desc_csum(fs)) {
		__u32 غیر_مستعمل = ext2fs_bg_itable_غیر_مستعمل(fs, جائزہ->current_گروہ);
		اگر (جائزہ->inodes_بایاں > غیر_مستعمل)
			جائزہ->inodes_بایاں -= غیر_مستعمل;
		ورنہ
			جائزہ->inodes_بایاں = 0;
		جائزہ->بلاکیں_بایاں =
			(جائزہ->inodes_بایاں +
			 (fs->blocksize / جائزہ->inode_سائز - 1)) *
			جائزہ->inode_سائز / fs->blocksize;
	}
	اگر (جائزہ->current_بلاک &&
	    ((جائزہ->current_بلاک < fs->super->s_پہلا_ڈیٹا_بلاک) ||
	     (جائزہ->current_بلاک + fs->inode_بلاکیں_فی_گروہ - 1 >=
	      ext2fs_بلاکیں_گنتی(fs->super))))
		واپس EXT2_ET_GDESC_BAD_INODE_جدول;
	واپس 0;
}

errcode_t ext2fs_inode_جائزہ_goto_blockgroup(ext2_inode_جائزہ جائزہ,
					    صحیح	گروہ)
{
	جائزہ->current_گروہ = گروہ - 1;
	جائزہ->groups_بایاں = جائزہ->fs->گروہ_desc_گنتی - گروہ;
	جائزہ->bad_بلاک_ptr = 0;
	واپس get_اگلا_blockgroup(جائزہ);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ساکن errcode_t check_for_inode_bad_بلاکیں(ext2_inode_جائزہ جائزہ,
					    blk64_t *عدد_بلاکیں)
{
	blk64_t	blk = جائزہ->current_بلاک;
	badblocks_list	bb = جائزہ->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	اگر (blk == 0)
		واپس 0;

	/* Make sure bad_block_ptr is still valid */
	اگر (جائزہ->bad_بلاک_ptr >= bb->عدد) {
		جائزہ->جائزہ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		واپس 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	جب (blk > bb->list[جائزہ->bad_بلاک_ptr]) {
		اگر (++جائزہ->bad_بلاک_ptr >= bb->عدد) {
			جائزہ->جائزہ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			واپس 0;
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
	اگر (blk == bb->list[جائزہ->bad_بلاک_ptr]) {
		جائزہ->جائزہ_flags |= EXT2_SF_BAD_INODE_BLK;
		*عدد_بلاکیں = 1;
		اگر (++جائزہ->bad_بلاک_ptr >= bb->عدد)
			جائزہ->جائزہ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		واپس 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	اگر ((blk + *عدد_بلاکیں) > bb->list[جائزہ->bad_بلاک_ptr])
		*عدد_بلاکیں = (صحیح) (bb->list[جائزہ->bad_بلاک_ptr] - blk);

	واپس 0;
}

ساکن صحیح بلاک_map_looks_insane(ext2_filsys fs,
				  ساخت ext2_inode_large *inode)
{
	غیرعلامتی صحیح i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	اگر ((inode->i_flags & EXT4_INLINE_ڈیٹا_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		واپس 0;
	اگر (!LINUX_S_ISREG(inode->i_منوال) &&
	    !LINUX_S_ISLNK(inode->i_منوال) &&
	    !LINUX_S_ISDIR(inode->i_منوال))
		واپس 0;
	اگر (LINUX_S_ISLNK(inode->i_منوال) &&
	    EXT2_I_سائز(inode) <= حجم(inode->i_بلاک))
		واپس 0;

	/* Unused inodes probably aren't insane */
	اگر (inode->i_links_گنتی == 0)
		واپس 0;

	/* See if more than half the block maps are insane */
	برائے (i = 0, bad = 0; i < EXT2_N_بلاکیں; i++)
		اگر (inode->i_بلاک[i] != 0 &&
		    (inode->i_بلاک[i] < fs->super->s_پہلا_ڈیٹا_بلاک ||
		     inode->i_بلاک[i] >= ext2fs_بلاکیں_گنتی(fs->super)))
			bad++;
	واپس bad > EXT2_N_بلاکیں / 2;
}

ساکن صحیح extent_سر_looks_insane(ساخت ext2_inode_large *inode)
{
	اگر (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_سرخی_verify(inode->i_بلاک,
					حجم(inode->i_بلاک)) == 0)
		واپس 0;
	واپس 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ساکن خالی check_inode_بلاک_sanity(ext2_inode_جائزہ جائزہ, blk64_t عدد_بلاکیں)
{
	ext2_ino_t	ino, inodes_مقصد_جائزہ;
	غیرعلامتی صحیح	badness, checksum_failures;
	غیرعلامتی صحیح	inodes_اندر_buf, inodes_فی_بلاک;
	حرف		*p;
	ساخت ext2_inode_large *inode;
	حرف		*بلاک_حالت;
	غیرعلامتی صحیح	blk, bad_csum;

	اگر (!(جائزہ->جائزہ_flags & EXT2_SF_WARN_GARBAGE_INODES))
		واپس;

	inodes_مقصد_جائزہ = جائزہ->inodes_بایاں;
	inodes_اندر_buf = عدد_بلاکیں * جائزہ->fs->blocksize / جائزہ->inode_سائز;
	اگر (inodes_مقصد_جائزہ > inodes_اندر_buf)
		inodes_مقصد_جائزہ = inodes_اندر_buf;

	p = (حرف *) جائزہ->inode_بفر;
	ino = جائزہ->current_inode + 1;
	checksum_failures = badness = 0;
	بلاک_حالت = جائزہ_بلاک_حالت(جائزہ);
	memset(بلاک_حالت, 0, جائزہ->inode_بفر_بلاکیں);
	inodes_فی_بلاک = EXT2_INODES_فی_بلاک(جائزہ->fs->super);

	اگر (inodes_فی_بلاک < 2)
		واپس;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	اگر (ext2fs_get_mem(EXT2_INODE_سائز(جائزہ->fs->super), &inode))
		واپس;
#اگر_ختم

	جب (inodes_مقصد_جائزہ > 0) {
		blk = (p - (حرف *)جائزہ->inode_بفر) / جائزہ->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(جائزہ->fs, ino,
				(ساخت ext2_inode_large *) p) == 0;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		ext2fs_تبادلہ_inode_بھرا(جائزہ->fs,
			       (ساخت ext2_inode_large *) inode,
			       (ساخت ext2_inode_large *) p,
			       0, EXT2_INODE_سائز(جائزہ->fs->super));
#ورنہ
		inode = (ساخت ext2_inode_large *) p;
#اگر_ختم

		/* Is this inode insane? */
		اگر (bad_csum) {
			checksum_failures++;
			badness++;
		} ورنہ اگر (extent_سر_looks_insane(inode) ||
			   بلاک_map_looks_insane(جائزہ->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		اگر (badness > inodes_فی_بلاک / 2) {
			غیرعلامتی صحیح ino_adj;

			بلاک_حالت[blk] |= IBLOCK_حالت_INSANE;
			ino_adj = inodes_فی_بلاک -
						((ino - 1) % inodes_فی_بلاک);
			اگر (ino_adj > inodes_مقصد_جائزہ)
				ino_adj = inodes_مقصد_جائزہ;
			inodes_مقصد_جائزہ -= ino_adj;
			p += جائزہ->inode_سائز * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			جاری;
		}

		اگر ((ino % inodes_فی_بلاک) == 0) {
			اگر (checksum_failures == 0)
				بلاک_حالت[blk] |= IBLOCK_حالت_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_مقصد_جائزہ--;
		p += جائزہ->inode_سائز;
		ino++;
	};

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_free_mem(&inode);
#اگر_ختم
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ساکن errcode_t get_اگلا_بلاکیں(ext2_inode_جائزہ جائزہ)
{
	blk64_t		عدد_بلاکیں;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	عدد_بلاکیں = جائزہ->inode_بفر_بلاکیں;
	اگر (عدد_بلاکیں > جائزہ->بلاکیں_بایاں)
		عدد_بلاکیں = جائزہ->بلاکیں_بایاں;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	اگر (جائزہ->جائزہ_flags & EXT2_SF_BAD_INODE_BLK) {
		اگر (جائزہ->بائٹیں_بایاں)
			جائزہ->جائزہ_flags |= EXT2_SF_BAD_EXTRA_بائٹیں;
		جائزہ->جائزہ_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	اگر (جائزہ->جائزہ_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_بلاکیں(جائزہ, &عدد_بلاکیں);
		اگر (retval)
			واپس retval;
	}

	اگر ((جائزہ->جائزہ_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (جائزہ->current_بلاک == 0)) {
		memset(جائزہ->inode_بفر, 0,
		       (size_t) عدد_بلاکیں * جائزہ->fs->blocksize);
	} ورنہ {
		retval = io_channel_read_blk64(جائزہ->fs->io,
					     جائزہ->current_بلاک,
					     (صحیح) عدد_بلاکیں,
					     جائزہ->inode_بفر);
		اگر (retval)
			واپس EXT2_ET_اگلا_INODE_READ;
	}
	check_inode_بلاک_sanity(جائزہ, عدد_بلاکیں);

	جائزہ->ptr = جائزہ->inode_بفر;
	جائزہ->بائٹیں_بایاں = عدد_بلاکیں * جائزہ->fs->blocksize;

	جائزہ->بلاکیں_بایاں -= عدد_بلاکیں;
	اگر (جائزہ->current_بلاک)
		جائزہ->current_بلاک += عدد_بلاکیں;

	واپس 0;
}

#اگر 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ساکن اندرونی صحیح is_خالی_خانے_جائزہ(ext2_inode_جائزہ جائزہ)
{
	صحیح	i;

	اگر (جائزہ->بائٹیں_بایاں == 0)
		واپس 0;

	برائے (i=0; i < جائزہ->بائٹیں_بایاں; i++)
		اگر (جائزہ->ptr[i])
			واپس 0;
	واپس 1;
}
#اگر_ختم

errcode_t ext2fs_get_اگلا_inode_بھرا(ext2_inode_جائزہ جائزہ, ext2_ino_t *ino,
				     ساخت ext2_inode *inode, صحیح bufsize)
{
	errcode_t	retval;
	صحیح		extra_بائٹیں = 0;
	صحیح		لمبائی;
	ساخت ext2_inode_large	*iptr = (ساخت ext2_inode_large *)inode;
	حرف		*iblock_حالت;
	غیرعلامتی صحیح	iblk;

	EXT2_CHECK_MAGIC(جائزہ, EXT2_ET_MAGIC_INODE_جائزہ);
	لمبائی = EXT2_INODE_سائز(جائزہ->fs->super);
	iblock_حالت = جائزہ_بلاک_حالت(جائزہ);

	/*
	 * Do we need to start reading a new block group?
	 */
	اگر (جائزہ->inodes_بایاں <= 0) {
	force_new_گروہ:
		اگر (جائزہ->مکمل_گروہ) {
			retval = (جائزہ->مکمل_گروہ)
				(جائزہ->fs, جائزہ, جائزہ->current_گروہ,
				 جائزہ->مکمل_گروہ_ڈیٹا);
			اگر (retval)
				واپس retval;
		}
		اگر (جائزہ->groups_بایاں <= 0) {
			*ino = 0;
			واپس 0;
		}
		retval = get_اگلا_blockgroup(جائزہ);
		اگر (retval)
			واپس retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	اگر ((جائزہ->جائزہ_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(جائزہ->fs, جائزہ->current_گروہ, EXT2_BG_INODE_UNINIT)
	     ))
		جاؤ force_new_گروہ;
	اگر (جائزہ->inodes_بایاں == 0)
		جاؤ force_new_گروہ;
	اگر (جائزہ->current_بلاک == 0) {
		اگر (جائزہ->جائزہ_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			جاؤ force_new_گروہ;
		} ورنہ
			واپس EXT2_ET_MISSING_INODE_جدول;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	اگر (جائزہ->بائٹیں_بایاں < جائزہ->inode_سائز) {
		اگر (جائزہ->بائٹیں_بایاں)
			حافظہ_نقل(جائزہ->عارضی_بفر, جائزہ->ptr, جائزہ->بائٹیں_بایاں);
		extra_بائٹیں = جائزہ->بائٹیں_بایاں;

		retval = get_اگلا_بلاکیں(جائزہ);
		اگر (retval)
			واپس retval;
#اگر 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		اگر (is_خالی_خانے_جائزہ(جائزہ))
			جاؤ force_new_گروہ;
#اگر_ختم
	}

	اگر (bufsize < لمبائی) {
		retval = ext2fs_get_mem(لمبائی, &iptr);
		اگر (retval)
			واپس retval;
	}

	retval = 0;
	iblk = جائزہ->current_inode % EXT2_INODES_فی_گروہ(جائزہ->fs->super) /
				EXT2_INODES_فی_بلاک(جائزہ->fs->super) %
				جائزہ->inode_بفر_بلاکیں;
	اگر (extra_بائٹیں) {
		حافظہ_نقل(جائزہ->عارضی_بفر+extra_بائٹیں, جائزہ->ptr,
		       جائزہ->inode_سائز - extra_بائٹیں);
		جائزہ->ptr += جائزہ->inode_سائز - extra_بائٹیں;
		جائزہ->بائٹیں_بایاں -= جائزہ->inode_سائز - extra_بائٹیں;

		/* Verify the inode checksum. */
		اگر (!(iblock_حالت[iblk] & IBLOCK_حالت_CSUMS_OK) &&
		    !(جائزہ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(جائزہ->fs, جائزہ->current_inode + 1,
				(ساخت ext2_inode_large *)جائزہ->عارضی_بفر))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		memset(iptr, 0, لمبائی);
		ext2fs_تبادلہ_inode_بھرا(جائزہ->fs,
			       (ساخت ext2_inode_large *) iptr,
			       (ساخت ext2_inode_large *) جائزہ->عارضی_بفر,
			       0, لمبائی);
#ورنہ
		حافظہ_نقل(iptr, جائزہ->عارضی_بفر, لمبائی);
#اگر_ختم
		اگر (جائزہ->جائزہ_flags & EXT2_SF_BAD_EXTRA_بائٹیں)
			retval = EXT2_ET_BAD_بلاک_اندر_INODE_جدول;
		جائزہ->جائزہ_flags &= ~EXT2_SF_BAD_EXTRA_بائٹیں;
	} ورنہ {
		/* Verify the inode checksum. */
		اگر (!(iblock_حالت[iblk] & IBLOCK_حالت_CSUMS_OK) &&
		    !(جائزہ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(جائزہ->fs, جائزہ->current_inode + 1,
				(ساخت ext2_inode_large *)جائزہ->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		memset(iptr, 0, لمبائی);
		ext2fs_تبادلہ_inode_بھرا(جائزہ->fs,
				(ساخت ext2_inode_large *) iptr,
				(ساخت ext2_inode_large *) جائزہ->ptr,
				0, لمبائی);
#ورنہ
		حافظہ_نقل(iptr, جائزہ->ptr, لمبائی);
#اگر_ختم
		جائزہ->ptr += جائزہ->inode_سائز;
		جائزہ->بائٹیں_بایاں -= جائزہ->inode_سائز;
		اگر (جائزہ->جائزہ_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_بلاک_اندر_INODE_جدول;
	}
	اگر ((iblock_حالت[iblk] & IBLOCK_حالت_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	جائزہ->inodes_بایاں--;
	جائزہ->current_inode++;
	*ino = جائزہ->current_inode;
	اگر (iptr != (ساخت ext2_inode_large *)inode) {
		حافظہ_نقل(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	واپس retval;
}

errcode_t ext2fs_get_اگلا_inode(ext2_inode_جائزہ جائزہ, ext2_ino_t *ino,
				ساخت ext2_inode *inode)
{
	واپس ext2fs_get_اگلا_inode_بھرا(جائزہ, ino, inode,
						حجم(ساخت ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     ساخت ext2_inode * inode, صحیح bufsize,
			     صحیح flags)
{
	blk64_t		بلاک_nr;
	dgrp_t		گروہ;
	غیرعلامتی طویل 	بلاک, انحراف;
	حرف 		*ptr;
	errcode_t	retval;
	غیرعلامتی	i;
	صحیح		clen, inodes_فی_بلاک;
	io_channel	io;
	صحیح		لمبائی = EXT2_INODE_سائز(fs->super);
	ساخت ext2_inode_large	*iptr;
	صحیح		cache_خانہ, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		واپس EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	اگر (fs->blocksize < 1024)
		واپس EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	اگر (fs->read_inode &&
	    ((bufsize == حجم(ساخت ext2_inode)) ||
	     (EXT2_INODE_سائز(fs->super) == حجم(ساخت ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		اگر (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			واپس retval;
	}
	اگر ((ino == 0) || (ino > fs->super->s_inodes_گنتی))
		واپس EXT2_ET_BAD_INODE_عدد;
	/* Create inode cache if not present */
	اگر (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		اگر (retval)
			واپس retval;
	}
	/* Check to see if it's in the inode cache */
	برائے (i = 0; i < fs->icache->cache_سائز; i++) {
		اگر (fs->icache->cache[i].ino == ino) {
			حافظہ_نقل(inode, fs->icache->cache[i].inode,
			       (bufsize > لمبائی) ? لمبائی : bufsize);
			واپس 0;
		}
	}
	اگر (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_فی_بلاک = fs->blocksize / EXT2_INODE_سائز(fs->super);
		بلاک_nr = ext2fs_le32_مقصد_cpu(fs->image_سرخی->انحراف_inode) / fs->blocksize;
		بلاک_nr += (ino - 1) / inodes_فی_بلاک;
		انحراف = ((ino - 1) % inodes_فی_بلاک) *
			EXT2_INODE_سائز(fs->super);
		io = fs->image_io;
	} ورنہ {
		گروہ = (ino - 1) / EXT2_INODES_فی_گروہ(fs->super);
		اگر (گروہ > fs->گروہ_desc_گنتی)
			واپس EXT2_ET_BAD_INODE_عدد;
		انحراف = ((ino - 1) % EXT2_INODES_فی_گروہ(fs->super)) *
			EXT2_INODE_سائز(fs->super);
		بلاک = انحراف >> EXT2_بلاک_سائز_بٹ(fs->super);
		بلاک_nr = ext2fs_inode_جدول_loc(fs, گروہ);
		اگر (!بلاک_nr)
			واپس EXT2_ET_MISSING_INODE_جدول;
		اگر ((بلاک_nr < fs->super->s_پہلا_ڈیٹا_بلاک) ||
		    (بلاک_nr + fs->inode_بلاکیں_فی_گروہ - 1 >=
		     ext2fs_بلاکیں_گنتی(fs->super)))
			واپس EXT2_ET_GDESC_BAD_INODE_جدول;
		بلاک_nr += بلاک;
		io = fs->io;
	}
	انحراف &= (EXT2_بلاک_سائز(fs->super) - 1);

	cache_خانہ = (fs->icache->cache_آخری + 1) % fs->icache->cache_سائز;
	iptr = (ساخت ext2_inode_large *)fs->icache->cache[cache_خانہ].inode;

	ptr = (حرف *) iptr;
	جب (لمبائی) {
		clen = لمبائی;
		اگر ((انحراف + لمبائی) > fs->blocksize)
			clen = fs->blocksize - انحراف;

		اگر (بلاک_nr != fs->icache->بفر_blk) {
			retval = io_channel_read_blk64(io, بلاک_nr, 1,
						     fs->icache->بفر);
			اگر (retval)
				واپس retval;
			fs->icache->بفر_blk = بلاک_nr;
		}

		حافظہ_نقل(ptr, ((حرف *) fs->icache->بفر) + (غیرعلامتی) انحراف,
		       clen);

		انحراف = 0;
		لمبائی -= clen;
		ptr += clen;
		بلاک_nr++;
	}
	لمبائی = EXT2_INODE_سائز(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_تبادلہ_inode_بھرا(fs, (ساخت ext2_inode_large *) iptr,
			       (ساخت ext2_inode_large *) iptr,
			       0, لمبائی);
#اگر_ختم

	/* Update the inode cache bookkeeping */
	اگر (!fail_csum) {
		fs->icache->cache_آخری = cache_خانہ;
		fs->icache->cache[cache_خانہ].ino = ino;
	}
	حافظہ_نقل(inode, iptr, (bufsize > لمبائی) ? لمبائی : bufsize);

	اگر (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		واپس EXT2_ET_INODE_CSUM_INVALID;

	واپس 0;
}

errcode_t ext2fs_read_inode_بھرا(ext2_filsys fs, ext2_ino_t ino,
				 ساخت ext2_inode * inode, صحیح bufsize)
{
	واپس ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ساخت ext2_inode * inode)
{
	واپس ext2fs_read_inode2(fs, ino, inode,
				  حجم(ساخت ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      ساخت ext2_inode * inode, صحیح bufsize,
			      صحیح flags)
{
	blk64_t بلاک_nr;
	dgrp_t گروہ;
	غیرعلامتی طویل بلاک, انحراف;
	errcode_t retval = 0;
	ساخت ext2_inode_large *w_inode;
	حرف *ptr;
	غیرعلامتی i;
	صحیح clen;
	صحیح لمبائی = EXT2_INODE_سائز(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		واپس EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	اگر (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		اگر (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			واپس retval;
	}

	اگر ((ino == 0) || (ino > fs->super->s_inodes_گنتی))
		واپس EXT2_ET_BAD_INODE_عدد;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(لمبائی, &w_inode);
	اگر (retval)
		واپس retval;

	اگر (bufsize < لمبائی) {
		retval = ext2fs_read_inode2(fs, ino,
					    (ساخت ext2_inode *)w_inode,
					    لمبائی, READ_INODE_NOCSUM);
		اگر (retval)
			جاؤ errout;
	}

	/* Check to see if the inode cache needs to be updated */
	اگر (fs->icache) {
		برائے (i=0; i < fs->icache->cache_سائز; i++) {
			اگر (fs->icache->cache[i].ino == ino) {
				حافظہ_نقل(fs->icache->cache[i].inode, inode,
				       (bufsize > لمبائی) ? لمبائی : bufsize);
				توڑو;
			}
		}
	} ورنہ {
		retval = ext2fs_create_inode_cache(fs, 4);
		اگر (retval)
			جاؤ errout;
	}
	حافظہ_نقل(w_inode, inode, (bufsize > لمبائی) ? لمبائی : bufsize);

	اگر (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		جاؤ errout;
	}

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_تبادلہ_inode_بھرا(fs, w_inode, w_inode, 1, لمبائی);
#اگر_ختم

	اگر ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_سیٹ(fs, ino, w_inode);
		اگر (retval)
			جاؤ errout;
	}

	گروہ = (ino - 1) / EXT2_INODES_فی_گروہ(fs->super);
	انحراف = ((ino - 1) % EXT2_INODES_فی_گروہ(fs->super)) *
		EXT2_INODE_سائز(fs->super);
	بلاک = انحراف >> EXT2_بلاک_سائز_بٹ(fs->super);
	بلاک_nr = ext2fs_inode_جدول_loc(fs, (غیرعلامتی) گروہ);
	اگر (!بلاک_nr) {
		retval = EXT2_ET_MISSING_INODE_جدول;
		جاؤ errout;
	}
	اگر ((بلاک_nr < fs->super->s_پہلا_ڈیٹا_بلاک) ||
	    (بلاک_nr + fs->inode_بلاکیں_فی_گروہ - 1 >=
	     ext2fs_بلاکیں_گنتی(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_جدول;
		جاؤ errout;
	}
	بلاک_nr += بلاک;

	انحراف &= (EXT2_بلاک_سائز(fs->super) - 1);

	ptr = (حرف *) w_inode;

	جب (لمبائی) {
		clen = لمبائی;
		اگر ((انحراف + لمبائی) > fs->blocksize)
			clen = fs->blocksize - انحراف;

		اگر (fs->icache->بفر_blk != بلاک_nr) {
			retval = io_channel_read_blk64(fs->io, بلاک_nr, 1,
						     fs->icache->بفر);
			اگر (retval)
				جاؤ errout;
			fs->icache->بفر_blk = بلاک_nr;
		}


		حافظہ_نقل((حرف *) fs->icache->بفر + (غیرعلامتی) انحراف,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, بلاک_nr, 1,
					      fs->icache->بفر);
		اگر (retval)
			جاؤ errout;

		انحراف = 0;
		ptr += clen;
		لمبائی -= clen;
		بلاک_nr++;
	}

	fs->flags |= EXT2_FLAG_بدلا;
errout:
	ext2fs_free_mem(&w_inode);
	واپس retval;
}

errcode_t ext2fs_write_inode_بھرا(ext2_filsys fs, ext2_ino_t ino,
				  ساخت ext2_inode * inode, صحیح bufsize)
{
	واپس ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     ساخت ext2_inode *inode)
{
	واپس ext2fs_write_inode2(fs, ino, inode,
				   حجم(ساخت ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 ساخت ext2_inode *inode)
{
	ساخت ext2_inode	*buf;
	صحیح			سائز = EXT2_INODE_سائز(fs->super);
	ساخت ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fs_P_get_وقت(fs);

	اگر (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_سیٹ(inode, i_atime, t);
	اگر (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_سیٹ(inode, i_ctime, t);
	اگر (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_سیٹ(inode, i_mtime, t);

	اگر (سائز == حجم(ساخت ext2_inode))
		واپس ext2fs_write_inode_بھرا(fs, ino, inode,
					       حجم(ساخت ext2_inode));

	buf = حافظہ_دو(سائز);
	اگر (!buf)
		واپس ENOMEM;

	memset(buf, 0, سائز);
	*buf = *inode;

	large_inode = (ساخت ext2_inode_large *) buf;
	large_inode->i_extra_isize = حجم(ساخت ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_سائز;
	اگر (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_بھرا(fs, ino, buf, سائز);
	آزاد(buf);
	واپس retval;
}


errcode_t ext2fs_get_بلاکیں(ext2_filsys fs, ext2_ino_t ino, blk_t *بلاکیں)
{
	ساخت ext2_inode	inode;
	صحیح			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ino > fs->super->s_inodes_گنتی)
		واپس EXT2_ET_BAD_INODE_عدد;

	اگر (fs->get_بلاکیں) {
		اگر (!(*fs->get_بلاکیں)(fs, ino, بلاکیں))
			واپس 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		واپس retval;
	برائے (i=0; i < EXT2_N_بلاکیں; i++)
		بلاکیں[i] = inode.i_بلاک[i];
	واپس 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	ساخت	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ino > fs->super->s_inodes_گنتی)
		واپس EXT2_ET_BAD_INODE_عدد;

	اگر (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		اگر (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			واپس retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		واپس retval;
	اگر (!LINUX_S_ISDIR(inode.i_منوال))
		واپس EXT2_ET_NO_DIRECTORY;
	واپس 0;
}

