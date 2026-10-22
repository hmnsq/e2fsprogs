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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "e2image.h"

#परिभाषय IBLOCK_स्थितिः_CSUMS_OK	1
#परिभाषय IBLOCK_स्थितिः_INSANE	2
#परिभाषय अवलोकनम्_खण्डम्_स्थितिः(अवलोकनम्)	((अवलोकनम्)->अस्थायी_बफर + (अवलोकनम्)->inode_आकार)

संरचना ext2_struct_inode_अवलोकनम् {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_खण्डम्;
	dgrp_t			current_गणः;
	ext2_ino_t		inodes_वामः;
	blk_t			खण्डानि_वामः;
	dgrp_t			groups_वामः;
	blk_t			inode_बफर_खण्डानि;
	वर्ण *			inode_बफर;
	पूर्णाङ्क			inode_आकार;
	वर्ण *			ptr;
	पूर्णाङ्क			अष्टकानि_वामः;
	वर्ण			*अस्थायी_बफर;
	errcode_t		(*समाप्तम्_गणः)(ext2_filsys fs,
					      ext2_inode_अवलोकनम् अवलोकनम्,
					      dgrp_t गणः,
					      शून्य * priv_दत्त);
	शून्य *			समाप्तम्_गणः_दत्त;
	पूर्णाङ्क			bad_खण्डम्_ptr;
	पूर्णाङ्क			अवलोकनम्_flags;
	पूर्णाङ्क			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	अचिह्नित	i;

	यदि (!fs->icache)
		प्रत्यागम 0;

	प्रति (i=0; i < fs->icache->cache_आकार; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->बफर_blk = 0;
	प्रत्यागम 0;
}

/*
 * Free the inode cache structure
 */
शून्य ext2fs_free_inode_cache(संरचना ext2_inode_cache *icache)
{
	अचिह्नित i;

	यदि (--icache->refcount)
		प्रत्यागम;
	यदि (icache->बफर)
		ext2fs_free_mem(&icache->बफर);
	प्रति (i = 0; i < icache->cache_आकार; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	यदि (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->बफर_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, अचिह्नित पूर्णाङ्क cache_आकार)
{
	अचिह्नित	i;
	errcode_t	retval;

	यदि (fs->icache)
		प्रत्यागम 0;
	retval = ext2fs_get_mem(परिमाण(संरचना ext2_inode_cache), &fs->icache);
	यदि (retval)
		प्रत्यागम retval;

	स्मृतिं_पूरय(fs->icache, 0, परिमाण(संरचना ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->बफर);
	यदि (retval)
		गच्छ errout;

	fs->icache->बफर_blk = 0;
	fs->icache->cache_चरमः = -1;
	fs->icache->cache_आकार = cache_आकार;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_आकार,
				  परिमाण(संरचना ext2_inode_cache_ent),
				  &fs->icache->cache);
	यदि (retval)
		गच्छ errout;

	प्रति (i = 0; i < fs->icache->cache_आकार; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_आकार(fs->super),
					&fs->icache->cache[i].inode);
		यदि (retval)
			गच्छ errout;
	}

	ext2fs_flush_icache(fs);
	प्रत्यागम 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	प्रत्यागम retval;
}

errcode_t ext2fs_open_inode_अवलोकनम्(ext2_filsys fs, पूर्णाङ्क बफर_खण्डानि,
				 ext2_inode_अवलोकनम् *ret_अवलोकनम्)
{
	ext2_inode_अवलोकनम्	अवलोकनम्;
	errcode_t	retval;
	errcode_t (*save_get_खण्डानि)(ext2_filsys f, ext2_ino_t ino, blk_t *खण्डानि);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	यदि (fs->blocksize < 1024)
		प्रत्यागम EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	यदि (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_खण्डानि = fs->get_खण्डानि;
		fs->get_खण्डानि = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		यदि (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_खण्डानि = save_get_खण्डानि;
	}

	retval = ext2fs_get_mem(परिमाण(संरचना ext2_struct_inode_अवलोकनम्), &अवलोकनम्);
	यदि (retval)
		प्रत्यागम retval;
	स्मृतिं_पूरय(अवलोकनम्, 0, परिमाण(संरचना ext2_struct_inode_अवलोकनम्));

	अवलोकनम्->magic = EXT2_ET_MAGIC_INODE_अवलोकनम्;
	अवलोकनम्->fs = fs;
	अवलोकनम्->inode_आकार = EXT2_INODE_आकार(fs->super);
	अवलोकनम्->अष्टकानि_वामः = 0;
	अवलोकनम्->current_गणः = 0;
	अवलोकनम्->groups_वामः = fs->गणः_desc_गणनम् - 1;
	अवलोकनम्->inode_बफर_खण्डानि = बफर_खण्डानि ? बफर_खण्डानि :
				    EXT2_INODE_अवलोकनम्_DEFAULT_बफर_खण्डानि;
	अवलोकनम्->current_खण्डम् = ext2fs_inode_सारणी_loc(अवलोकनम्->fs,
						     अवलोकनम्->current_गणः);
	यदि (अवलोकनम्->current_खण्डम् &&
	    ((अवलोकनम्->current_खण्डम् < fs->super->s_first_दत्त_खण्डम्) ||
	     (अवलोकनम्->current_खण्डम् + fs->inode_खण्डानि_प्रत्येकम्_गणः - 1 >=
	      ext2fs_खण्डानि_गणनम्(fs->super)))) {
		ext2fs_free_mem(&अवलोकनम्);
		प्रत्यागम EXT2_ET_GDESC_BAD_INODE_सारणी;
	}

	अवलोकनम्->inodes_वामः = EXT2_INODES_प्रत्येकम्_गणः(अवलोकनम्->fs->super);
	अवलोकनम्->खण्डानि_वामः = अवलोकनम्->fs->inode_खण्डानि_प्रत्येकम्_गणः;
	यदि (ext2fs_has_गणः_desc_csum(fs)) {
		__u32 अप्रयुक्तम् = ext2fs_bg_itable_अप्रयुक्तम्(fs, अवलोकनम्->current_गणः);
		यदि (अवलोकनम्->inodes_वामः > अप्रयुक्तम्)
			अवलोकनम्->inodes_वामः -= अप्रयुक्तम्;
		अन्यथा
			अवलोकनम्->inodes_वामः = 0;
		अवलोकनम्->खण्डानि_वामः =
			(अवलोकनम्->inodes_वामः +
			 (fs->blocksize / अवलोकनम्->inode_आकार - 1)) *
			अवलोकनम्->inode_आकार / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, अवलोकनम्->inode_बफर_खण्डानि,
				      &अवलोकनम्->inode_बफर);
	अवलोकनम्->समाप्तम्_गणः = 0;
	अवलोकनम्->समाप्तम्_गणः_दत्त = 0;
	अवलोकनम्->bad_खण्डम्_ptr = 0;
	यदि (retval) {
		ext2fs_free_mem(&अवलोकनम्);
		प्रत्यागम retval;
	}
	retval = ext2fs_get_mem(अवलोकनम्->inode_आकार + अवलोकनम्->inode_बफर_खण्डानि,
				&अवलोकनम्->अस्थायी_बफर);
	यदि (retval) {
		ext2fs_free_mem(&अवलोकनम्->inode_बफर);
		ext2fs_free_mem(&अवलोकनम्);
		प्रत्यागम retval;
	}
	स्मृतिं_पूरय(अवलोकनम्_खण्डम्_स्थितिः(अवलोकनम्), 0, अवलोकनम्->inode_बफर_खण्डानि);
	यदि (अवलोकनम्->fs->badblocks && अवलोकनम्->fs->badblocks->अङ्कः)
		अवलोकनम्->अवलोकनम्_flags |= EXT2_SF_CHK_BADBLOCKS;
	यदि (ext2fs_has_गणः_desc_csum(fs))
		अवलोकनम्->अवलोकनम्_flags |= EXT2_SF_DO_LAZY;
	*ret_अवलोकनम् = अवलोकनम्;
	प्रत्यागम 0;
}

शून्य ext2fs_close_inode_अवलोकनम्(ext2_inode_अवलोकनम् अवलोकनम्)
{
	यदि (!अवलोकनम् || (अवलोकनम्->magic != EXT2_ET_MAGIC_INODE_अवलोकनम्))
		प्रत्यागम;

	ext2fs_free_mem(&अवलोकनम्->inode_बफर);
	अवलोकनम्->inode_बफर = रिक्त;
	ext2fs_free_mem(&अवलोकनम्->अस्थायी_बफर);
	अवलोकनम्->अस्थायी_बफर = रिक्त;
	ext2fs_free_mem(&अवलोकनम्);
	प्रत्यागम;
}

शून्य ext2fs_समुच्चयः_inode_callback(ext2_inode_अवलोकनम् अवलोकनम्,
			       errcode_t (*समाप्तम्_गणः)(ext2_filsys fs,
						       ext2_inode_अवलोकनम् अवलोकनम्,
						       dgrp_t गणः,
						       शून्य * priv_दत्त),
			       शून्य *समाप्तम्_गणः_दत्त)
{
	यदि (!अवलोकनम् || (अवलोकनम्->magic != EXT2_ET_MAGIC_INODE_अवलोकनम्))
		प्रत्यागम;

	अवलोकनम्->समाप्तम्_गणः = समाप्तम्_गणः;
	अवलोकनम्->समाप्तम्_गणः_दत्त = समाप्तम्_गणः_दत्त;
}

पूर्णाङ्क ext2fs_inode_अवलोकनम्_flags(ext2_inode_अवलोकनम् अवलोकनम्, पूर्णाङ्क समुच्चयः_flags,
			    पूर्णाङ्क clear_flags)
{
	पूर्णाङ्क	old_flags;

	यदि (!अवलोकनम् || (अवलोकनम्->magic != EXT2_ET_MAGIC_INODE_अवलोकनम्))
		प्रत्यागम 0;

	old_flags = अवलोकनम्->अवलोकनम्_flags;
	अवलोकनम्->अवलोकनम्_flags &= ~clear_flags;
	अवलोकनम्->अवलोकनम्_flags |= समुच्चयः_flags;
	प्रत्यागम old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
स्थिर errcode_t get_अग्र_blockgroup(ext2_inode_अवलोकनम् अवलोकनम्)
{
	ext2_filsys fs = अवलोकनम्->fs;

	अवलोकनम्->current_गणः++;
	अवलोकनम्->groups_वामः--;

	अवलोकनम्->current_खण्डम् = ext2fs_inode_सारणी_loc(अवलोकनम्->fs,
						     अवलोकनम्->current_गणः);
	अवलोकनम्->current_inode = अवलोकनम्->current_गणः *
		EXT2_INODES_प्रत्येकम्_गणः(fs->super);

	अवलोकनम्->अष्टकानि_वामः = 0;
	अवलोकनम्->inodes_वामः = EXT2_INODES_प्रत्येकम्_गणः(fs->super);
	अवलोकनम्->खण्डानि_वामः = fs->inode_खण्डानि_प्रत्येकम्_गणः;
	यदि (ext2fs_has_गणः_desc_csum(fs)) {
		__u32 अप्रयुक्तम् = ext2fs_bg_itable_अप्रयुक्तम्(fs, अवलोकनम्->current_गणः);
		यदि (अवलोकनम्->inodes_वामः > अप्रयुक्तम्)
			अवलोकनम्->inodes_वामः -= अप्रयुक्तम्;
		अन्यथा
			अवलोकनम्->inodes_वामः = 0;
		अवलोकनम्->खण्डानि_वामः =
			(अवलोकनम्->inodes_वामः +
			 (fs->blocksize / अवलोकनम्->inode_आकार - 1)) *
			अवलोकनम्->inode_आकार / fs->blocksize;
	}
	यदि (अवलोकनम्->current_खण्डम् &&
	    ((अवलोकनम्->current_खण्डम् < fs->super->s_first_दत्त_खण्डम्) ||
	     (अवलोकनम्->current_खण्डम् + fs->inode_खण्डानि_प्रत्येकम्_गणः - 1 >=
	      ext2fs_खण्डानि_गणनम्(fs->super))))
		प्रत्यागम EXT2_ET_GDESC_BAD_INODE_सारणी;
	प्रत्यागम 0;
}

errcode_t ext2fs_inode_अवलोकनम्_goto_blockgroup(ext2_inode_अवलोकनम् अवलोकनम्,
					    पूर्णाङ्क	गणः)
{
	अवलोकनम्->current_गणः = गणः - 1;
	अवलोकनम्->groups_वामः = अवलोकनम्->fs->गणः_desc_गणनम् - गणः;
	अवलोकनम्->bad_खण्डम्_ptr = 0;
	प्रत्यागम get_अग्र_blockgroup(अवलोकनम्);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
स्थिर errcode_t check_for_inode_bad_खण्डानि(ext2_inode_अवलोकनम् अवलोकनम्,
					    blk64_t *अङ्कः_खण्डानि)
{
	blk64_t	blk = अवलोकनम्->current_खण्डम्;
	badblocks_list	bb = अवलोकनम्->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	यदि (blk == 0)
		प्रत्यागम 0;

	/* Make sure bad_block_ptr is still valid */
	यदि (अवलोकनम्->bad_खण्डम्_ptr >= bb->अङ्कः) {
		अवलोकनम्->अवलोकनम्_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		प्रत्यागम 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	यावत् (blk > bb->list[अवलोकनम्->bad_खण्डम्_ptr]) {
		यदि (++अवलोकनम्->bad_खण्डम्_ptr >= bb->अङ्कः) {
			अवलोकनम्->अवलोकनम्_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			प्रत्यागम 0;
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
	यदि (blk == bb->list[अवलोकनम्->bad_खण्डम्_ptr]) {
		अवलोकनम्->अवलोकनम्_flags |= EXT2_SF_BAD_INODE_BLK;
		*अङ्कः_खण्डानि = 1;
		यदि (++अवलोकनम्->bad_खण्डम्_ptr >= bb->अङ्कः)
			अवलोकनम्->अवलोकनम्_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		प्रत्यागम 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	यदि ((blk + *अङ्कः_खण्डानि) > bb->list[अवलोकनम्->bad_खण्डम्_ptr])
		*अङ्कः_खण्डानि = (पूर्णाङ्क) (bb->list[अवलोकनम्->bad_खण्डम्_ptr] - blk);

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क खण्डम्_map_looks_insane(ext2_filsys fs,
				  संरचना ext2_inode_large *inode)
{
	अचिह्नित पूर्णाङ्क i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	यदि ((inode->i_flags & EXT4_INLINE_दत्त_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		प्रत्यागम 0;
	यदि (!LINUX_S_ISREG(inode->i_बहुलकम्) &&
	    !LINUX_S_ISLNK(inode->i_बहुलकम्) &&
	    !LINUX_S_ISDIR(inode->i_बहुलकम्))
		प्रत्यागम 0;
	यदि (LINUX_S_ISLNK(inode->i_बहुलकम्) &&
	    EXT2_I_आकार(inode) <= परिमाण(inode->i_खण्डम्))
		प्रत्यागम 0;

	/* Unused inodes probably aren't insane */
	यदि (inode->i_links_गणनम् == 0)
		प्रत्यागम 0;

	/* See if more than half the block maps are insane */
	प्रति (i = 0, bad = 0; i < EXT2_N_खण्डानि; i++)
		यदि (inode->i_खण्डम्[i] != 0 &&
		    (inode->i_खण्डम्[i] < fs->super->s_first_दत्त_खण्डम् ||
		     inode->i_खण्डम्[i] >= ext2fs_खण्डानि_गणनम्(fs->super)))
			bad++;
	प्रत्यागम bad > EXT2_N_खण्डानि / 2;
}

स्थिर पूर्णाङ्क extent_शीर्ष_looks_insane(संरचना ext2_inode_large *inode)
{
	यदि (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_शीर्षकम्_verify(inode->i_खण्डम्,
					परिमाण(inode->i_खण्डम्)) == 0)
		प्रत्यागम 0;
	प्रत्यागम 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
स्थिर शून्य check_inode_खण्डम्_sanity(ext2_inode_अवलोकनम् अवलोकनम्, blk64_t अङ्कः_खण्डानि)
{
	ext2_ino_t	ino, inodes_गन्तव्यम्_अवलोकनम्;
	अचिह्नित पूर्णाङ्क	badness, checksum_failures;
	अचिह्नित पूर्णाङ्क	inodes_अन्तर्_buf, inodes_प्रत्येकम्_खण्डम्;
	वर्ण		*p;
	संरचना ext2_inode_large *inode;
	वर्ण		*खण्डम्_स्थितिः;
	अचिह्नित पूर्णाङ्क	blk, bad_csum;

	यदि (!(अवलोकनम्->अवलोकनम्_flags & EXT2_SF_WARN_GARBAGE_INODES))
		प्रत्यागम;

	inodes_गन्तव्यम्_अवलोकनम् = अवलोकनम्->inodes_वामः;
	inodes_अन्तर्_buf = अङ्कः_खण्डानि * अवलोकनम्->fs->blocksize / अवलोकनम्->inode_आकार;
	यदि (inodes_गन्तव्यम्_अवलोकनम् > inodes_अन्तर्_buf)
		inodes_गन्तव्यम्_अवलोकनम् = inodes_अन्तर्_buf;

	p = (वर्ण *) अवलोकनम्->inode_बफर;
	ino = अवलोकनम्->current_inode + 1;
	checksum_failures = badness = 0;
	खण्डम्_स्थितिः = अवलोकनम्_खण्डम्_स्थितिः(अवलोकनम्);
	स्मृतिं_पूरय(खण्डम्_स्थितिः, 0, अवलोकनम्->inode_बफर_खण्डानि);
	inodes_प्रत्येकम्_खण्डम् = EXT2_INODES_प्रत्येकम्_खण्डम्(अवलोकनम्->fs->super);

	यदि (inodes_प्रत्येकम्_खण्डम् < 2)
		प्रत्यागम;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	यदि (ext2fs_get_mem(EXT2_INODE_आकार(अवलोकनम्->fs->super), &inode))
		प्रत्यागम;
#यदि_अन्तः

	यावत् (inodes_गन्तव्यम्_अवलोकनम् > 0) {
		blk = (p - (वर्ण *)अवलोकनम्->inode_बफर) / अवलोकनम्->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(अवलोकनम्->fs, ino,
				(संरचना ext2_inode_large *) p) == 0;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		ext2fs_swap_inode_पूर्णम्(अवलोकनम्->fs,
			       (संरचना ext2_inode_large *) inode,
			       (संरचना ext2_inode_large *) p,
			       0, EXT2_INODE_आकार(अवलोकनम्->fs->super));
#अन्यथा
		inode = (संरचना ext2_inode_large *) p;
#यदि_अन्तः

		/* Is this inode insane? */
		यदि (bad_csum) {
			checksum_failures++;
			badness++;
		} अन्यथा यदि (extent_शीर्ष_looks_insane(inode) ||
			   खण्डम्_map_looks_insane(अवलोकनम्->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		यदि (badness > inodes_प्रत्येकम्_खण्डम् / 2) {
			अचिह्नित पूर्णाङ्क ino_adj;

			खण्डम्_स्थितिः[blk] |= IBLOCK_स्थितिः_INSANE;
			ino_adj = inodes_प्रत्येकम्_खण्डम् -
						((ino - 1) % inodes_प्रत्येकम्_खण्डम्);
			यदि (ino_adj > inodes_गन्तव्यम्_अवलोकनम्)
				ino_adj = inodes_गन्तव्यम्_अवलोकनम्;
			inodes_गन्तव्यम्_अवलोकनम् -= ino_adj;
			p += अवलोकनम्->inode_आकार * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			अनुवर्त;
		}

		यदि ((ino % inodes_प्रत्येकम्_खण्डम्) == 0) {
			यदि (checksum_failures == 0)
				खण्डम्_स्थितिः[blk] |= IBLOCK_स्थितिः_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_गन्तव्यम्_अवलोकनम्--;
		p += अवलोकनम्->inode_आकार;
		ino++;
	};

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	ext2fs_free_mem(&inode);
#यदि_अन्तः
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
स्थिर errcode_t get_अग्र_खण्डानि(ext2_inode_अवलोकनम् अवलोकनम्)
{
	blk64_t		अङ्कः_खण्डानि;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	अङ्कः_खण्डानि = अवलोकनम्->inode_बफर_खण्डानि;
	यदि (अङ्कः_खण्डानि > अवलोकनम्->खण्डानि_वामः)
		अङ्कः_खण्डानि = अवलोकनम्->खण्डानि_वामः;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	यदि (अवलोकनम्->अवलोकनम्_flags & EXT2_SF_BAD_INODE_BLK) {
		यदि (अवलोकनम्->अष्टकानि_वामः)
			अवलोकनम्->अवलोकनम्_flags |= EXT2_SF_BAD_EXTRA_अष्टकानि;
		अवलोकनम्->अवलोकनम्_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	यदि (अवलोकनम्->अवलोकनम्_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_खण्डानि(अवलोकनम्, &अङ्कः_खण्डानि);
		यदि (retval)
			प्रत्यागम retval;
	}

	यदि ((अवलोकनम्->अवलोकनम्_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (अवलोकनम्->current_खण्डम् == 0)) {
		स्मृतिं_पूरय(अवलोकनम्->inode_बफर, 0,
		       (size_t) अङ्कः_खण्डानि * अवलोकनम्->fs->blocksize);
	} अन्यथा {
		retval = io_channel_read_blk64(अवलोकनम्->fs->io,
					     अवलोकनम्->current_खण्डम्,
					     (पूर्णाङ्क) अङ्कः_खण्डानि,
					     अवलोकनम्->inode_बफर);
		यदि (retval)
			प्रत्यागम EXT2_ET_अग्र_INODE_READ;
	}
	check_inode_खण्डम्_sanity(अवलोकनम्, अङ्कः_खण्डानि);

	अवलोकनम्->ptr = अवलोकनम्->inode_बफर;
	अवलोकनम्->अष्टकानि_वामः = अङ्कः_खण्डानि * अवलोकनम्->fs->blocksize;

	अवलोकनम्->खण्डानि_वामः -= अङ्कः_खण्डानि;
	यदि (अवलोकनम्->current_खण्डम्)
		अवलोकनम्->current_खण्डम् += अङ्कः_खण्डानि;

	प्रत्यागम 0;
}

#यदि 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
स्थिर अन्तःस्थ पूर्णाङ्क is_रिक्तानि_अवलोकनम्(ext2_inode_अवलोकनम् अवलोकनम्)
{
	पूर्णाङ्क	i;

	यदि (अवलोकनम्->अष्टकानि_वामः == 0)
		प्रत्यागम 0;

	प्रति (i=0; i < अवलोकनम्->अष्टकानि_वामः; i++)
		यदि (अवलोकनम्->ptr[i])
			प्रत्यागम 0;
	प्रत्यागम 1;
}
#यदि_अन्तः

errcode_t ext2fs_get_अग्र_inode_पूर्णम्(ext2_inode_अवलोकनम् अवलोकनम्, ext2_ino_t *ino,
				     संरचना ext2_inode *inode, पूर्णाङ्क bufsize)
{
	errcode_t	retval;
	पूर्णाङ्क		extra_अष्टकानि = 0;
	पूर्णाङ्क		दीर्घता;
	संरचना ext2_inode_large	*iptr = (संरचना ext2_inode_large *)inode;
	वर्ण		*iblock_स्थितिः;
	अचिह्नित पूर्णाङ्क	iblk;

	EXT2_CHECK_MAGIC(अवलोकनम्, EXT2_ET_MAGIC_INODE_अवलोकनम्);
	दीर्घता = EXT2_INODE_आकार(अवलोकनम्->fs->super);
	iblock_स्थितिः = अवलोकनम्_खण्डम्_स्थितिः(अवलोकनम्);

	/*
	 * Do we need to start reading a new block group?
	 */
	यदि (अवलोकनम्->inodes_वामः <= 0) {
	force_new_गणः:
		यदि (अवलोकनम्->समाप्तम्_गणः) {
			retval = (अवलोकनम्->समाप्तम्_गणः)
				(अवलोकनम्->fs, अवलोकनम्, अवलोकनम्->current_गणः,
				 अवलोकनम्->समाप्तम्_गणः_दत्त);
			यदि (retval)
				प्रत्यागम retval;
		}
		यदि (अवलोकनम्->groups_वामः <= 0) {
			*ino = 0;
			प्रत्यागम 0;
		}
		retval = get_अग्र_blockgroup(अवलोकनम्);
		यदि (retval)
			प्रत्यागम retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	यदि ((अवलोकनम्->अवलोकनम्_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(अवलोकनम्->fs, अवलोकनम्->current_गणः, EXT2_BG_INODE_UNINIT)
	     ))
		गच्छ force_new_गणः;
	यदि (अवलोकनम्->inodes_वामः == 0)
		गच्छ force_new_गणः;
	यदि (अवलोकनम्->current_खण्डम् == 0) {
		यदि (अवलोकनम्->अवलोकनम्_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			गच्छ force_new_गणः;
		} अन्यथा
			प्रत्यागम EXT2_ET_MISSING_INODE_सारणी;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	यदि (अवलोकनम्->अष्टकानि_वामः < अवलोकनम्->inode_आकार) {
		यदि (अवलोकनम्->अष्टकानि_वामः)
			स्मृतिं_प्रतिलिख(अवलोकनम्->अस्थायी_बफर, अवलोकनम्->ptr, अवलोकनम्->अष्टकानि_वामः);
		extra_अष्टकानि = अवलोकनम्->अष्टकानि_वामः;

		retval = get_अग्र_खण्डानि(अवलोकनम्);
		यदि (retval)
			प्रत्यागम retval;
#यदि 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		यदि (is_रिक्तानि_अवलोकनम्(अवलोकनम्))
			गच्छ force_new_गणः;
#यदि_अन्तः
	}

	यदि (bufsize < दीर्घता) {
		retval = ext2fs_get_mem(दीर्घता, &iptr);
		यदि (retval)
			प्रत्यागम retval;
	}

	retval = 0;
	iblk = अवलोकनम्->current_inode % EXT2_INODES_प्रत्येकम्_गणः(अवलोकनम्->fs->super) /
				EXT2_INODES_प्रत्येकम्_खण्डम्(अवलोकनम्->fs->super) %
				अवलोकनम्->inode_बफर_खण्डानि;
	यदि (extra_अष्टकानि) {
		स्मृतिं_प्रतिलिख(अवलोकनम्->अस्थायी_बफर+extra_अष्टकानि, अवलोकनम्->ptr,
		       अवलोकनम्->inode_आकार - extra_अष्टकानि);
		अवलोकनम्->ptr += अवलोकनम्->inode_आकार - extra_अष्टकानि;
		अवलोकनम्->अष्टकानि_वामः -= अवलोकनम्->inode_आकार - extra_अष्टकानि;

		/* Verify the inode checksum. */
		यदि (!(iblock_स्थितिः[iblk] & IBLOCK_स्थितिः_CSUMS_OK) &&
		    !(अवलोकनम्->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(अवलोकनम्->fs, अवलोकनम्->current_inode + 1,
				(संरचना ext2_inode_large *)अवलोकनम्->अस्थायी_बफर))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		स्मृतिं_पूरय(iptr, 0, दीर्घता);
		ext2fs_swap_inode_पूर्णम्(अवलोकनम्->fs,
			       (संरचना ext2_inode_large *) iptr,
			       (संरचना ext2_inode_large *) अवलोकनम्->अस्थायी_बफर,
			       0, दीर्घता);
#अन्यथा
		स्मृतिं_प्रतिलिख(iptr, अवलोकनम्->अस्थायी_बफर, दीर्घता);
#यदि_अन्तः
		यदि (अवलोकनम्->अवलोकनम्_flags & EXT2_SF_BAD_EXTRA_अष्टकानि)
			retval = EXT2_ET_BAD_खण्डम्_अन्तर्_INODE_सारणी;
		अवलोकनम्->अवलोकनम्_flags &= ~EXT2_SF_BAD_EXTRA_अष्टकानि;
	} अन्यथा {
		/* Verify the inode checksum. */
		यदि (!(iblock_स्थितिः[iblk] & IBLOCK_स्थितिः_CSUMS_OK) &&
		    !(अवलोकनम्->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(अवलोकनम्->fs, अवलोकनम्->current_inode + 1,
				(संरचना ext2_inode_large *)अवलोकनम्->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		स्मृतिं_पूरय(iptr, 0, दीर्घता);
		ext2fs_swap_inode_पूर्णम्(अवलोकनम्->fs,
				(संरचना ext2_inode_large *) iptr,
				(संरचना ext2_inode_large *) अवलोकनम्->ptr,
				0, दीर्घता);
#अन्यथा
		स्मृतिं_प्रतिलिख(iptr, अवलोकनम्->ptr, दीर्घता);
#यदि_अन्तः
		अवलोकनम्->ptr += अवलोकनम्->inode_आकार;
		अवलोकनम्->अष्टकानि_वामः -= अवलोकनम्->inode_आकार;
		यदि (अवलोकनम्->अवलोकनम्_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_खण्डम्_अन्तर्_INODE_सारणी;
	}
	यदि ((iblock_स्थितिः[iblk] & IBLOCK_स्थितिः_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	अवलोकनम्->inodes_वामः--;
	अवलोकनम्->current_inode++;
	*ino = अवलोकनम्->current_inode;
	यदि (iptr != (संरचना ext2_inode_large *)inode) {
		स्मृतिं_प्रतिलिख(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	प्रत्यागम retval;
}

errcode_t ext2fs_get_अग्र_inode(ext2_inode_अवलोकनम् अवलोकनम्, ext2_ino_t *ino,
				संरचना ext2_inode *inode)
{
	प्रत्यागम ext2fs_get_अग्र_inode_पूर्णम्(अवलोकनम्, ino, inode,
						परिमाण(संरचना ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode * inode, पूर्णाङ्क bufsize,
			     पूर्णाङ्क flags)
{
	blk64_t		खण्डम्_nr;
	dgrp_t		गणः;
	अचिह्नित दीर्घ 	खण्डम्, विस्थापनम्;
	वर्ण 		*ptr;
	errcode_t	retval;
	अचिह्नित	i;
	पूर्णाङ्क		clen, inodes_प्रत्येकम्_खण्डम्;
	io_channel	io;
	पूर्णाङ्क		दीर्घता = EXT2_INODE_आकार(fs->super);
	संरचना ext2_inode_large	*iptr;
	पूर्णाङ्क		cache_slot, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	यदि (fs->blocksize < 1024)
		प्रत्यागम EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	यदि (fs->read_inode &&
	    ((bufsize == परिमाण(संरचना ext2_inode)) ||
	     (EXT2_INODE_आकार(fs->super) == परिमाण(संरचना ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		यदि (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			प्रत्यागम retval;
	}
	यदि ((ino == 0) || (ino > fs->super->s_inodes_गणनम्))
		प्रत्यागम EXT2_ET_BAD_INODE_अङ्कः;
	/* Create inode cache if not present */
	यदि (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		यदि (retval)
			प्रत्यागम retval;
	}
	/* Check to see if it's in the inode cache */
	प्रति (i = 0; i < fs->icache->cache_आकार; i++) {
		यदि (fs->icache->cache[i].ino == ino) {
			स्मृतिं_प्रतिलिख(inode, fs->icache->cache[i].inode,
			       (bufsize > दीर्घता) ? दीर्घता : bufsize);
			प्रत्यागम 0;
		}
	}
	यदि (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_प्रत्येकम्_खण्डम् = fs->blocksize / EXT2_INODE_आकार(fs->super);
		खण्डम्_nr = ext2fs_le32_गन्तव्यम्_cpu(fs->image_शीर्षकम्->विस्थापनम्_inode) / fs->blocksize;
		खण्डम्_nr += (ino - 1) / inodes_प्रत्येकम्_खण्डम्;
		विस्थापनम् = ((ino - 1) % inodes_प्रत्येकम्_खण्डम्) *
			EXT2_INODE_आकार(fs->super);
		io = fs->image_io;
	} अन्यथा {
		गणः = (ino - 1) / EXT2_INODES_प्रत्येकम्_गणः(fs->super);
		यदि (गणः > fs->गणः_desc_गणनम्)
			प्रत्यागम EXT2_ET_BAD_INODE_अङ्कः;
		विस्थापनम् = ((ino - 1) % EXT2_INODES_प्रत्येकम्_गणः(fs->super)) *
			EXT2_INODE_आकार(fs->super);
		खण्डम् = विस्थापनम् >> EXT2_खण्डम्_आकार_द्वयङ्काः(fs->super);
		खण्डम्_nr = ext2fs_inode_सारणी_loc(fs, गणः);
		यदि (!खण्डम्_nr)
			प्रत्यागम EXT2_ET_MISSING_INODE_सारणी;
		यदि ((खण्डम्_nr < fs->super->s_first_दत्त_खण्डम्) ||
		    (खण्डम्_nr + fs->inode_खण्डानि_प्रत्येकम्_गणः - 1 >=
		     ext2fs_खण्डानि_गणनम्(fs->super)))
			प्रत्यागम EXT2_ET_GDESC_BAD_INODE_सारणी;
		खण्डम्_nr += खण्डम्;
		io = fs->io;
	}
	विस्थापनम् &= (EXT2_खण्डम्_आकार(fs->super) - 1);

	cache_slot = (fs->icache->cache_चरमः + 1) % fs->icache->cache_आकार;
	iptr = (संरचना ext2_inode_large *)fs->icache->cache[cache_slot].inode;

	ptr = (वर्ण *) iptr;
	यावत् (दीर्घता) {
		clen = दीर्घता;
		यदि ((विस्थापनम् + दीर्घता) > fs->blocksize)
			clen = fs->blocksize - विस्थापनम्;

		यदि (खण्डम्_nr != fs->icache->बफर_blk) {
			retval = io_channel_read_blk64(io, खण्डम्_nr, 1,
						     fs->icache->बफर);
			यदि (retval)
				प्रत्यागम retval;
			fs->icache->बफर_blk = खण्डम्_nr;
		}

		स्मृतिं_प्रतिलिख(ptr, ((वर्ण *) fs->icache->बफर) + (अचिह्नित) विस्थापनम्,
		       clen);

		विस्थापनम् = 0;
		दीर्घता -= clen;
		ptr += clen;
		खण्डम्_nr++;
	}
	दीर्घता = EXT2_INODE_आकार(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	ext2fs_swap_inode_पूर्णम्(fs, (संरचना ext2_inode_large *) iptr,
			       (संरचना ext2_inode_large *) iptr,
			       0, दीर्घता);
#यदि_अन्तः

	/* Update the inode cache bookkeeping */
	यदि (!fail_csum) {
		fs->icache->cache_चरमः = cache_slot;
		fs->icache->cache[cache_slot].ino = ino;
	}
	स्मृतिं_प्रतिलिख(inode, iptr, (bufsize > दीर्घता) ? दीर्घता : bufsize);

	यदि (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		प्रत्यागम EXT2_ET_INODE_CSUM_INVALID;

	प्रत्यागम 0;
}

errcode_t ext2fs_read_inode_पूर्णम्(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode * inode, पूर्णाङ्क bufsize)
{
	प्रत्यागम ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    संरचना ext2_inode * inode)
{
	प्रत्यागम ext2fs_read_inode2(fs, ino, inode,
				  परिमाण(संरचना ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      संरचना ext2_inode * inode, पूर्णाङ्क bufsize,
			      पूर्णाङ्क flags)
{
	blk64_t खण्डम्_nr;
	dgrp_t गणः;
	अचिह्नित दीर्घ खण्डम्, विस्थापनम्;
	errcode_t retval = 0;
	संरचना ext2_inode_large *w_inode;
	वर्ण *ptr;
	अचिह्नित i;
	पूर्णाङ्क clen;
	पूर्णाङ्क दीर्घता = EXT2_INODE_आकार(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	यदि (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		यदि (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			प्रत्यागम retval;
	}

	यदि ((ino == 0) || (ino > fs->super->s_inodes_गणनम्))
		प्रत्यागम EXT2_ET_BAD_INODE_अङ्कः;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(दीर्घता, &w_inode);
	यदि (retval)
		प्रत्यागम retval;

	यदि (bufsize < दीर्घता) {
		retval = ext2fs_read_inode2(fs, ino,
					    (संरचना ext2_inode *)w_inode,
					    दीर्घता, READ_INODE_NOCSUM);
		यदि (retval)
			गच्छ errout;
	}

	/* Check to see if the inode cache needs to be updated */
	यदि (fs->icache) {
		प्रति (i=0; i < fs->icache->cache_आकार; i++) {
			यदि (fs->icache->cache[i].ino == ino) {
				स्मृतिं_प्रतिलिख(fs->icache->cache[i].inode, inode,
				       (bufsize > दीर्घता) ? दीर्घता : bufsize);
				भङ्ग;
			}
		}
	} अन्यथा {
		retval = ext2fs_create_inode_cache(fs, 4);
		यदि (retval)
			गच्छ errout;
	}
	स्मृतिं_प्रतिलिख(w_inode, inode, (bufsize > दीर्घता) ? दीर्घता : bufsize);

	यदि (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		गच्छ errout;
	}

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	ext2fs_swap_inode_पूर्णम्(fs, w_inode, w_inode, 1, दीर्घता);
#यदि_अन्तः

	यदि ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_समुच्चयः(fs, ino, w_inode);
		यदि (retval)
			गच्छ errout;
	}

	गणः = (ino - 1) / EXT2_INODES_प्रत्येकम्_गणः(fs->super);
	विस्थापनम् = ((ino - 1) % EXT2_INODES_प्रत्येकम्_गणः(fs->super)) *
		EXT2_INODE_आकार(fs->super);
	खण्डम् = विस्थापनम् >> EXT2_खण्डम्_आकार_द्वयङ्काः(fs->super);
	खण्डम्_nr = ext2fs_inode_सारणी_loc(fs, (अचिह्नित) गणः);
	यदि (!खण्डम्_nr) {
		retval = EXT2_ET_MISSING_INODE_सारणी;
		गच्छ errout;
	}
	यदि ((खण्डम्_nr < fs->super->s_first_दत्त_खण्डम्) ||
	    (खण्डम्_nr + fs->inode_खण्डानि_प्रत्येकम्_गणः - 1 >=
	     ext2fs_खण्डानि_गणनम्(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_सारणी;
		गच्छ errout;
	}
	खण्डम्_nr += खण्डम्;

	विस्थापनम् &= (EXT2_खण्डम्_आकार(fs->super) - 1);

	ptr = (वर्ण *) w_inode;

	यावत् (दीर्घता) {
		clen = दीर्घता;
		यदि ((विस्थापनम् + दीर्घता) > fs->blocksize)
			clen = fs->blocksize - विस्थापनम्;

		यदि (fs->icache->बफर_blk != खण्डम्_nr) {
			retval = io_channel_read_blk64(fs->io, खण्डम्_nr, 1,
						     fs->icache->बफर);
			यदि (retval)
				गच्छ errout;
			fs->icache->बफर_blk = खण्डम्_nr;
		}


		स्मृतिं_प्रतिलिख((वर्ण *) fs->icache->बफर + (अचिह्नित) विस्थापनम्,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, खण्डम्_nr, 1,
					      fs->icache->बफर);
		यदि (retval)
			गच्छ errout;

		विस्थापनम् = 0;
		ptr += clen;
		दीर्घता -= clen;
		खण्डम्_nr++;
	}

	fs->flags |= EXT2_FLAG_परिवर्तितम्;
errout:
	ext2fs_free_mem(&w_inode);
	प्रत्यागम retval;
}

errcode_t ext2fs_write_inode_पूर्णम्(ext2_filsys fs, ext2_ino_t ino,
				  संरचना ext2_inode * inode, पूर्णाङ्क bufsize)
{
	प्रत्यागम ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode)
{
	प्रत्यागम ext2fs_write_inode2(fs, ino, inode,
				   परिमाण(संरचना ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 संरचना ext2_inode *inode)
{
	संरचना ext2_inode	*buf;
	पूर्णाङ्क			आकार = EXT2_INODE_आकार(fs->super);
	संरचना ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fsP_get_time(fs);

	यदि (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_समुच्चयः(inode, i_atime, t);
	यदि (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_समुच्चयः(inode, i_ctime, t);
	यदि (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_समुच्चयः(inode, i_mtime, t);

	यदि (आकार == परिमाण(संरचना ext2_inode))
		प्रत्यागम ext2fs_write_inode_पूर्णम्(fs, ino, inode,
					       परिमाण(संरचना ext2_inode));

	buf = स्मृतिं_देहि(आकार);
	यदि (!buf)
		प्रत्यागम ENOMEM;

	स्मृतिं_पूरय(buf, 0, आकार);
	*buf = *inode;

	large_inode = (संरचना ext2_inode_large *) buf;
	large_inode->i_extra_isize = परिमाण(संरचना ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_आकार;
	यदि (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_पूर्णम्(fs, ino, buf, आकार);
	मुक्त(buf);
	प्रत्यागम retval;
}


errcode_t ext2fs_get_खण्डानि(ext2_filsys fs, ext2_ino_t ino, blk_t *खण्डानि)
{
	संरचना ext2_inode	inode;
	पूर्णाङ्क			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ino > fs->super->s_inodes_गणनम्)
		प्रत्यागम EXT2_ET_BAD_INODE_अङ्कः;

	यदि (fs->get_खण्डानि) {
		यदि (!(*fs->get_खण्डानि)(fs, ino, खण्डानि))
			प्रत्यागम 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	यदि (retval)
		प्रत्यागम retval;
	प्रति (i=0; i < EXT2_N_खण्डानि; i++)
		खण्डानि[i] = inode.i_खण्डम्[i];
	प्रत्यागम 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ino > fs->super->s_inodes_गणनम्)
		प्रत्यागम EXT2_ET_BAD_INODE_अङ्कः;

	यदि (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		यदि (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			प्रत्यागम retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	यदि (retval)
		प्रत्यागम retval;
	यदि (!LINUX_S_ISDIR(inode.i_बहुलकम्))
		प्रत्यागम EXT2_ET_NO_DIRECTORY;
	प्रत्यागम 0;
}

