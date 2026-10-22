/*
 * closefs.c --- close an ext2 filesystem
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <time.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

स्थिर पूर्णाङ्क test_मूलम्(अचिह्नित पूर्णाङ्क a, अचिह्नित पूर्णाङ्क b)
{
	यावत् (1) {
		यदि (a < b)
			प्रत्यागम 0;
		यदि (a == b)
			प्रत्यागम 1;
		यदि (a % b)
			प्रत्यागम 0;
		a = a / b;
	}
}

पूर्णाङ्क ext2fs_bg_has_super(ext2_filsys fs, dgrp_t गणः)
{
	यदि (गणः == 0)
		प्रत्यागम 1;
	यदि (ext2fs_has_feature_sparse_super2(fs->super)) {
		यदि (गणः == fs->super->s_backup_bgs[0] ||
		    गणः == fs->super->s_backup_bgs[1])
			प्रत्यागम 1;
		प्रत्यागम 0;
	}
	यदि ((गणः <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		प्रत्यागम 1;
	यदि (!(गणः & 1))
		प्रत्यागम 0;
	यदि (test_मूलम्(गणः, 3) || (test_मूलम्(गणः, 5)) ||
	    test_मूलम्(गणः, 7))
		प्रत्यागम 1;

	प्रत्यागम 0;
}

/*
 * ext2fs_super_and_bgd_loc2()
 * @fs:			ext2 fs pointer
 * @group		given block group
 * @ret_super_blk:	if !NULL, returns super block location
 * @ret_old_desc_blk:	if !NULL, returns location of the old block
 *			group descriptor
 * @ret_new_desc_blk:	if !NULL, returns location of meta_bg block
 *			group descriptor
 * @ret_used_blks:	if !NULL, returns number of blocks used by
 *			super block and group_descriptors.
 *
 * Returns errcode_t of 0
 */
errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
					   dgrp_t गणः,
					   blk64_t *ret_super_blk,
					   blk64_t *ret_old_desc_blk,
					   blk64_t *ret_new_desc_blk,
					   blk_t *ret_प्रयुक्तम्_blks)
{
	blk64_t	गणः_खण्डम्, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	अचिह्नित पूर्णाङ्क meta_bg, meta_bg_आकार;
	blk_t	numblocks = 0;
	blk64_t old_desc_खण्डानि;
	पूर्णाङ्क	has_super;

	गणः_खण्डम् = ext2fs_गणः_first_block2(fs, गणः);
	यदि (गणः_खण्डम् == 0 && fs->blocksize == 1024)
		गणः_खण्डम् = 1; /* Deal with 1024 blocksize && bigalloc */

	यदि (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_खण्डानि = fs->super->s_first_meta_bg;
	अन्यथा
		old_desc_खण्डानि =
			fs->desc_खण्डानि + fs->super->s_reserved_gdt_खण्डानि;

	has_super = ext2fs_bg_has_super(fs, गणः);

	यदि (has_super) {
		super_blk = गणः_खण्डम्;
		numblocks++;
	}
	meta_bg_आकार = EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super);
	meta_bg = गणः / meta_bg_आकार;

	यदि (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_first_meta_bg)) {
		यदि (has_super) {
			old_desc_blk = गणः_खण्डम् + 1;
			numblocks += old_desc_खण्डानि;
		}
	} अन्यथा {
		यदि (((गणः % meta_bg_आकार) == 0) ||
		    ((गणः % meta_bg_आकार) == 1) ||
		    ((गणः % meta_bg_आकार) == (meta_bg_आकार-1))) {
			यदि (has_super)
				has_super = 1;
			new_desc_blk = गणः_खण्डम् + has_super;
			numblocks++;
		}
	}

	यदि (ret_super_blk)
		*ret_super_blk = super_blk;
	यदि (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	यदि (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	यदि (ret_प्रयुक्तम्_blks)
		*ret_प्रयुक्तम्_blks = numblocks;

	प्रत्यागम 0;
}

/*
 * This function returns the location of the superblock, block group
 * descriptors for a given block group.  It currently returns the
 * number of free blocks assuming that inode table and allocation
 * bitmaps will be in the group.  This is not necessarily the case
 * when the flex_bg feature is enabled, so callers should take care!
 * It was only really intended for use by mke2fs, and even there it's
 * not that useful.
 *
 * The ext2fs_super_and_bgd_loc2() function is 64-bit block number
 * capable and returns the number of blocks used by super block and
 * group descriptors.
 */
पूर्णाङ्क ext2fs_super_and_bgd_loc(ext2_filsys fs,
			     dgrp_t गणः,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     पूर्णाङ्क *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_प्रयुक्तम्_blks;
	blk_t numblocks;
	अचिह्नित पूर्णाङ्क meta_bg_आकार;

	ext2fs_super_and_bgd_loc2(fs, गणः, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_प्रयुक्तम्_blks);

	numblocks = ext2fs_गणः_खण्डानि_गणनम्(fs, गणः);

	यदि (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	यदि (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	यदि (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	यदि (ret_meta_bg) {
		meta_bg_आकार = EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super);
		*ret_meta_bg = गणः / meta_bg_आकार;
	}

	numblocks -= 2 + fs->inode_खण्डानि_प्रत्येकम्_गणः + ret_प्रयुक्तम्_blks;

	प्रत्यागम numblocks;
}

/*
 * This function forces out the primary superblock.  We need to only
 * write out those fields which we have changed, since if the
 * filesystem is mounted, it may have changed some of the other
 * fields.
 *
 * It takes as input a superblock which has already been byte swapped
 * (if necessary).
 *
 */
स्थिर errcode_t write_primary_superblock(ext2_filsys fs,
					  संरचना ext2_super_खण्डम् *super)
{
	__u16		*old_super, *new_super;
	पूर्णाङ्क		check_idx, write_idx, आकार;
	errcode_t	retval;

	यदि (!fs->io->manager->write_byte || !fs->orig_super) {
	fallback:
		io_channel_समुच्चयः_blksize(fs->io, SUPERBLOCK_विस्थापनम्);
		retval = io_channel_write_blk64(fs->io, 1, -SUPERBLOCK_आकार,
					      super);
		io_channel_समुच्चयः_blksize(fs->io, fs->blocksize);
		प्रत्यागम retval;
	}

	old_super = (__u16 *) fs->orig_super;
	new_super = (__u16 *) super;

	प्रति (check_idx = 0; check_idx < SUPERBLOCK_आकार/2; check_idx++) {
		यदि (old_super[check_idx] == new_super[check_idx])
			अनुवर्त;
		write_idx = check_idx;
		प्रति (check_idx++; check_idx < SUPERBLOCK_आकार/2; check_idx++)
			यदि (old_super[check_idx] == new_super[check_idx])
				भङ्ग;
		आकार = 2 * (check_idx - write_idx);
#यदि 0
		प्रारूप_लिख("Writing %d bytes starting at %d\n",
		       आकार, write_idx*2);
#यदि_अन्तः
		retval = io_channel_write_byte(fs->io,
			       SUPERBLOCK_विस्थापनम् + (2 * write_idx), आकार,
					       new_super + write_idx);
		यदि (retval == EXT2_ET_UNIMPLEMENTED)
			गच्छ fallback;
		यदि (retval)
			प्रत्यागम retval;
	}
	स्मृतिं_प्रतिलिख(fs->orig_super, super, SUPERBLOCK_आकार);
	प्रत्यागम 0;
}


/*
 * Updates the revision to EXT2_DYNAMIC_REV
 */
शून्य ext2fs_update_dynamic_rev(ext2_filsys fs)
{
	संरचना ext2_super_खण्डम् *sb = fs->super;

	यदि (sb->s_rev_level > EXT2_GOOD_OLD_REV)
		प्रत्यागम;

	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_first_ino = EXT2_GOOD_OLD_FIRST_INO;
	sb->s_inode_आकार = EXT2_GOOD_OLD_INODE_आकार;
	/* s_uuid is handled by e2fsck already */
	/* other fields should be left alone */
}

स्थिर errcode_t write_backup_super(ext2_filsys fs, dgrp_t गणः,
				    blk64_t गणः_खण्डम्,
				    संरचना ext2_super_खण्डम् *super_shadow)
{
	errcode_t retval;
	dgrp_t	sgrp = गणः;

	यदि (sgrp > ((1 << 16) - 1))
		sgrp = (1 << 16) - 1;

	super_shadow->s_खण्डम्_गणः_nr = ext2fs_cpu_गन्तव्यम्_le16(sgrp);

	retval = ext2fs_superblock_csum_समुच्चयः(fs, super_shadow);
	यदि (retval)
		प्रत्यागम retval;

	प्रत्यागम io_channel_write_blk64(fs->io, गणः_खण्डम्, -SUPERBLOCK_आकार,
				    super_shadow);
}

errcode_t ext2fs_flush(ext2_filsys fs)
{
	प्रत्यागम ext2fs_flush2(fs, 0);
}

errcode_t ext2fs_flush2(ext2_filsys fs, पूर्णाङ्क flags)
{
	dgrp_t		i;
	errcode_t	retval;
	अचिह्नित दीर्घ	fs_state;
	__u32		feature_incompat;
	संरचना ext2_super_खण्डम् *super_shadow = 0;
	संरचना opaque_ext2_गणः_desc *गणः_shadow = 0;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	संरचना ext2_गणः_desc *gdp;
	dgrp_t		j;
#यदि_अन्तः
	वर्ण	*गणः_ptr;
	blk64_t	old_desc_खण्डानि;
	संरचना ext2fs_numeric_प्रगतिः_struct प्रगतिः;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि ((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0 &&
	    !ext2fs_has_feature_journal_dev(fs->super) &&
	    fs->गणः_desc == रिक्त)
		प्रत्यागम EXT2_ET_NO_GDESC;

	fs_state = fs->super->s_state;
	feature_incompat = fs->super->s_feature_incompat;

	ext2fs_समुच्चयः_tstamp(fs->super, s_wtime, ext2fsP_get_time(fs));
	fs->super->s_खण्डम्_गणः_nr = 0;

	/*
	 * If the write_bitmaps() function is present, call it to
	 * flush the bitmaps.  This is done this way so that a simple
	 * program that doesn't mess with the bitmaps doesn't need to
	 * drag in the bitmaps.c code.
	 *
	 * Bitmap checksums live in the group descriptor, so the
	 * bitmaps need to be written before the descriptors.
	 */
	यदि (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		यदि (retval)
			गच्छ errout;
	}

	/*
	 * Set the state of the FS to be non-valid.  (The state has
	 * already been backed up earlier, and will be restored after
	 * we write out the backup superblocks.)
	 */
	fs->super->s_state &= ~EXT2_VALID_FS;
	ext2fs_clear_feature_journal_needs_recovery(fs->super);

	/* Byte swap the superblock and the group descriptors if necessary */
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	retval = EXT2_ET_NO_स्मृतिः;
	retval = ext2fs_get_mem(SUPERBLOCK_आकार, &super_shadow);
	यदि (retval)
		गच्छ errout;
	स्मृतिं_प्रतिलिख(super_shadow, fs->super, परिमाण(संरचना ext2_super_खण्डम्));
	ext2fs_swap_super(super_shadow);

	यदि (((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0) &&
	    !ext2fs_has_feature_journal_dev(fs->super)) {
		retval = ext2fs_get_array(fs->desc_खण्डानि, fs->blocksize,
					  &गणः_shadow);
		यदि (retval)
			गच्छ errout;
		स्मृतिं_प्रतिलिख(गणः_shadow, fs->गणः_desc, (size_t) fs->blocksize *
		       fs->desc_खण्डानि);

		प्रति (j = 0; j < fs->गणः_desc_गणनम्; j++) {
			gdp = ext2fs_गणः_desc(fs, गणः_shadow, j);
			ext2fs_swap_गणः_desc2(fs, gdp);
		}
	}
#अन्यथा
	super_shadow = fs->super;
	गणः_shadow = fs->गणः_desc;
#यदि_अन्तः

	/*
	 * If this is an external journal device, don't write out the
	 * block group descriptors or any of the backup superblocks
	 */
	यदि (ext2fs_has_feature_journal_dev(fs->super))
		गच्छ write_primary_superblock_only;

	/*
	 * Write out the master group descriptors, and the backup
	 * superblocks and group descriptors.
	 */
	गणः_ptr = (वर्ण *) गणः_shadow;
	यदि (ext2fs_has_feature_meta_bg(fs->super)) {
		old_desc_खण्डानि = fs->super->s_first_meta_bg;
		यदि (old_desc_खण्डानि > fs->desc_खण्डानि)
			old_desc_खण्डानि = fs->desc_खण्डानि;
	} अन्यथा
		old_desc_खण्डानि = fs->desc_खण्डानि;

	यदि (fs->प्रगतिः_ops && fs->प्रगतिः_ops->init)
		(fs->प्रगतिः_ops->init)(fs, &प्रगतिः, रिक्त,
					 fs->गणः_desc_गणनम्);


	प्रति (i = 0; i < fs->गणः_desc_गणनम्; i++) {
		blk64_t	super_blk, old_desc_blk, new_desc_blk;

		यदि (fs->प्रगतिः_ops && fs->प्रगतिः_ops->update)
			(fs->प्रगतिः_ops->update)(fs, &प्रगतिः, i);
		ext2fs_super_and_bgd_loc2(fs, i, &super_blk, &old_desc_blk,
					 &new_desc_blk, 0);

		यदि (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) &&i && super_blk) {
			retval = write_backup_super(fs, i, super_blk,
						    super_shadow);
			यदि (retval)
				गच्छ errout;
		}
		यदि (fs->flags & EXT2_FLAG_SUPER_ONLY)
			अनुवर्त;
		यदि ((old_desc_blk) &&
		    (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) || (i == 0))) {
			retval = io_channel_write_blk64(fs->io,
			      old_desc_blk, old_desc_खण्डानि, गणः_ptr);
			यदि (retval)
				गच्छ errout;
		}
		यदि (new_desc_blk) {
			पूर्णाङ्क meta_bg = i / EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super);

			retval = io_channel_write_blk64(fs->io, new_desc_blk,
				1, गणः_ptr + (meta_bg*fs->blocksize));
			यदि (retval)
				गच्छ errout;
		}
	}

	यदि (fs->प्रगतिः_ops && fs->प्रगतिः_ops->close)
		(fs->प्रगतिः_ops->close)(fs, &प्रगतिः, रिक्त);

write_primary_superblock_only:
	/*
	 * Write out master superblock.  This has to be done
	 * separately, since it is located at a fixed location
	 * (SUPERBLOCK_OFFSET).  We flush all other pending changes
	 * out to disk first, just to avoid a race condition with an
	 * insy-tinsy window....
	 */

	fs->super->s_खण्डम्_गणः_nr = 0;
	fs->super->s_state = fs_state;
	fs->super->s_feature_incompat = feature_incompat;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	*super_shadow = *fs->super;
	ext2fs_swap_super(super_shadow);
#यदि_अन्तः

	retval = ext2fs_superblock_csum_समुच्चयः(fs, super_shadow);
	यदि (retval)
		प्रत्यागम retval;

	यदि (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		यदि (retval)
			गच्छ errout;
	}
	retval = write_primary_superblock(fs, super_shadow);
	यदि (retval)
		गच्छ errout;

	fs->flags &= ~EXT2_FLAG_DIRTY;

	यदि (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		यदि (retval)
			गच्छ errout;
	}
errout:
	fs->super->s_state = fs_state;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	यदि (super_shadow)
		ext2fs_free_mem(&super_shadow);
	यदि (गणः_shadow)
		ext2fs_free_mem(&गणः_shadow);
#यदि_अन्तः
	प्रत्यागम retval;
}

errcode_t ext2fs_close_free(ext2_filsys *fs_ptr)
{
	errcode_t ret;
	ext2_filsys fs = *fs_ptr;

	ret = ext2fs_close2(fs, 0);
	यदि (ret)
		ext2fs_free(fs);
	*fs_ptr = रिक्त;
	प्रत्यागम ret;
}

errcode_t ext2fs_close(ext2_filsys fs)
{
	प्रत्यागम ext2fs_close2(fs, 0);
}

errcode_t ext2fs_close2(ext2_filsys fs, पूर्णाङ्क flags)
{
	errcode_t	retval;
	पूर्णाङ्क		meta_blks;
	io_stats stats = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		यदि (retval)
			प्रत्यागम retval;
	}
	यदि (fs->super->s_kbytes_written &&
	    fs->io->manager->get_stats)
		fs->io->manager->get_stats(fs->io, &stats);
	यदि (stats && stats->अष्टकानि_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->अष्टकानि_written >> 10;
		meta_blks = fs->desc_खण्डानि + 1;
		यदि (!(fs->flags & EXT2_FLAG_SUPER_ONLY))
			fs->super->s_kbytes_written += meta_blks /
				(fs->blocksize / 1024);
		यदि ((fs->flags & EXT2_FLAG_DIRTY) == 0)
			fs->flags |= EXT2_FLAG_SUPER_ONLY | EXT2_FLAG_DIRTY;
	}
	यदि (fs->flags & EXT2_FLAG_DIRTY) {
		retval = ext2fs_flush2(fs, flags);
		यदि (retval)
			प्रत्यागम retval;
	}

	retval = ext2fs_mmp_stop(fs);
	यदि (retval)
		प्रत्यागम retval;

	ext2fs_free(fs);
	प्रत्यागम 0;
}

