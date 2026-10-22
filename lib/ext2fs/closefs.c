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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <time.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

స్థిర పూర్ణాంకం test_మూలం(గుర్తులేని పూర్ణాంకం a, గుర్తులేని పూర్ణాంకం b)
{
	అయితే (1) {
		ఒకవేళ (a < b)
			తిరిగి 0;
		ఒకవేళ (a == b)
			తిరిగి 1;
		ఒకవేళ (a % b)
			తిరిగి 0;
		a = a / b;
	}
}

పూర్ణాంకం ext2fs_bg_has_super(ext2_filsys fs, dgrp_t సమూహం)
{
	ఒకవేళ (సమూహం == 0)
		తిరిగి 1;
	ఒకవేళ (ext2fs_has_feature_sparse_super2(fs->super)) {
		ఒకవేళ (సమూహం == fs->super->s_backup_bgs[0] ||
		    సమూహం == fs->super->s_backup_bgs[1])
			తిరిగి 1;
		తిరిగి 0;
	}
	ఒకవేళ ((సమూహం <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		తిరిగి 1;
	ఒకవేళ (!(సమూహం & 1))
		తిరిగి 0;
	ఒకవేళ (test_మూలం(సమూహం, 3) || (test_మూలం(సమూహం, 5)) ||
	    test_మూలం(సమూహం, 7))
		తిరిగి 1;

	తిరిగి 0;
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
					   dgrp_t సమూహం,
					   blk64_t *ret_super_blk,
					   blk64_t *ret_old_desc_blk,
					   blk64_t *ret_new_desc_blk,
					   blk_t *ret_వాడిన_blks)
{
	blk64_t	సమూహం_ఖండం, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	గుర్తులేని పూర్ణాంకం meta_bg, meta_bg_కొలత;
	blk_t	numblocks = 0;
	blk64_t old_desc_ఖండాలు;
	పూర్ణాంకం	has_super;

	సమూహం_ఖండం = ext2fs_సమూహం_మొదటి_block2(fs, సమూహం);
	ఒకవేళ (సమూహం_ఖండం == 0 && fs->blocksize == 1024)
		సమూహం_ఖండం = 1; /* Deal with 1024 blocksize && bigalloc */

	ఒకవేళ (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_ఖండాలు = fs->super->s_మొదటి_meta_bg;
	లేకపోతే
		old_desc_ఖండాలు =
			fs->desc_ఖండాలు + fs->super->s_reserved_gdt_ఖండాలు;

	has_super = ext2fs_bg_has_super(fs, సమూహం);

	ఒకవేళ (has_super) {
		super_blk = సమూహం_ఖండం;
		numblocks++;
	}
	meta_bg_కొలత = EXT2_DESC_చొప్పున_ఖండం(fs->super);
	meta_bg = సమూహం / meta_bg_కొలత;

	ఒకవేళ (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_మొదటి_meta_bg)) {
		ఒకవేళ (has_super) {
			old_desc_blk = సమూహం_ఖండం + 1;
			numblocks += old_desc_ఖండాలు;
		}
	} లేకపోతే {
		ఒకవేళ (((సమూహం % meta_bg_కొలత) == 0) ||
		    ((సమూహం % meta_bg_కొలత) == 1) ||
		    ((సమూహం % meta_bg_కొలత) == (meta_bg_కొలత-1))) {
			ఒకవేళ (has_super)
				has_super = 1;
			new_desc_blk = సమూహం_ఖండం + has_super;
			numblocks++;
		}
	}

	ఒకవేళ (ret_super_blk)
		*ret_super_blk = super_blk;
	ఒకవేళ (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	ఒకవేళ (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	ఒకవేళ (ret_వాడిన_blks)
		*ret_వాడిన_blks = numblocks;

	తిరిగి 0;
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
పూర్ణాంకం ext2fs_super_and_bgd_loc(ext2_filsys fs,
			     dgrp_t సమూహం,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     పూర్ణాంకం *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_వాడిన_blks;
	blk_t numblocks;
	గుర్తులేని పూర్ణాంకం meta_bg_కొలత;

	ext2fs_super_and_bgd_loc2(fs, సమూహం, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_వాడిన_blks);

	numblocks = ext2fs_సమూహం_ఖండాలు_లెక్క(fs, సమూహం);

	ఒకవేళ (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	ఒకవేళ (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	ఒకవేళ (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	ఒకవేళ (ret_meta_bg) {
		meta_bg_కొలత = EXT2_DESC_చొప్పున_ఖండం(fs->super);
		*ret_meta_bg = సమూహం / meta_bg_కొలత;
	}

	numblocks -= 2 + fs->inode_ఖండాలు_చొప్పున_సమూహం + ret_వాడిన_blks;

	తిరిగి numblocks;
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
స్థిర errcode_t write_primary_superblock(ext2_filsys fs,
					  నిర్మాణం ext2_super_ఖండం *super)
{
	__u16		*old_super, *new_super;
	పూర్ణాంకం		check_idx, write_idx, కొలత;
	errcode_t	retval;

	ఒకవేళ (!fs->io->manager->write_byte || !fs->orig_super) {
	fallback:
		io_channel_సమితి_blksize(fs->io, SUPERBLOCK_స్థానభ్రంశం);
		retval = io_channel_write_blk64(fs->io, 1, -SUPERBLOCK_కొలత,
					      super);
		io_channel_సమితి_blksize(fs->io, fs->blocksize);
		తిరిగి retval;
	}

	old_super = (__u16 *) fs->orig_super;
	new_super = (__u16 *) super;

	కొరకు (check_idx = 0; check_idx < SUPERBLOCK_కొలత/2; check_idx++) {
		ఒకవేళ (old_super[check_idx] == new_super[check_idx])
			కొనసాగు;
		write_idx = check_idx;
		కొరకు (check_idx++; check_idx < SUPERBLOCK_కొలత/2; check_idx++)
			ఒకవేళ (old_super[check_idx] == new_super[check_idx])
				ఆపు;
		కొలత = 2 * (check_idx - write_idx);
#ఒకవేళ 0
		రాయి("Writing %d bytes starting at %d\n",
		       కొలత, write_idx*2);
#అయితే_ముగింపు
		retval = io_channel_write_byte(fs->io,
			       SUPERBLOCK_స్థానభ్రంశం + (2 * write_idx), కొలత,
					       new_super + write_idx);
		ఒకవేళ (retval == EXT2_ET_UNIMPLEMENTED)
			వెళ్ళు fallback;
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	జ్ఞాపకం_నకలు(fs->orig_super, super, SUPERBLOCK_కొలత);
	తిరిగి 0;
}


/*
 * Updates the revision to EXT2_DYNAMIC_REV
 */
శూన్య ext2fs_update_dynamic_rev(ext2_filsys fs)
{
	నిర్మాణం ext2_super_ఖండం *sb = fs->super;

	ఒకవేళ (sb->s_rev_level > EXT2_GOOD_OLD_REV)
		తిరిగి;

	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_మొదటి_ino = EXT2_GOOD_OLD_మొదటి_INO;
	sb->s_inode_కొలత = EXT2_GOOD_OLD_INODE_కొలత;
	/* s_uuid is handled by e2fsck already */
	/* other fields should be left alone */
}

స్థిర errcode_t write_backup_super(ext2_filsys fs, dgrp_t సమూహం,
				    blk64_t సమూహం_ఖండం,
				    నిర్మాణం ext2_super_ఖండం *super_shadow)
{
	errcode_t retval;
	dgrp_t	sgrp = సమూహం;

	ఒకవేళ (sgrp > ((1 << 16) - 1))
		sgrp = (1 << 16) - 1;

	super_shadow->s_ఖండం_సమూహం_nr = ext2fs_cpu_గమ్యం_le16(sgrp);

	retval = ext2fs_superblock_csum_సమితి(fs, super_shadow);
	ఒకవేళ (retval)
		తిరిగి retval;

	తిరిగి io_channel_write_blk64(fs->io, సమూహం_ఖండం, -SUPERBLOCK_కొలత,
				    super_shadow);
}

errcode_t ext2fs_flush(ext2_filsys fs)
{
	తిరిగి ext2fs_flush2(fs, 0);
}

errcode_t ext2fs_flush2(ext2_filsys fs, పూర్ణాంకం flags)
{
	dgrp_t		i;
	errcode_t	retval;
	గుర్తులేని దీర్ఘ	fs_state;
	__u32		feature_incompat;
	నిర్మాణం ext2_super_ఖండం *super_shadow = 0;
	నిర్మాణం opaque_ext2_సమూహం_desc *సమూహం_shadow = 0;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	నిర్మాణం ext2_సమూహం_desc *gdp;
	dgrp_t		j;
#అయితే_ముగింపు
	అక్షరం	*సమూహం_ptr;
	blk64_t	old_desc_ఖండాలు;
	నిర్మాణం ext2fs_numeric_పురోగతి_struct పురోగతి;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ ((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0 &&
	    !ext2fs_has_feature_journal_dev(fs->super) &&
	    fs->సమూహం_desc == ఖాళీ)
		తిరిగి EXT2_ET_NO_GDESC;

	fs_state = fs->super->s_state;
	feature_incompat = fs->super->s_feature_incompat;

	ext2fs_సమితి_tstamp(fs->super, s_wtime, ext2fs_P_get_సమయం(fs));
	fs->super->s_ఖండం_సమూహం_nr = 0;

	/*
	 * If the write_bitmaps() function is present, call it to
	 * flush the bitmaps.  This is done this way so that a simple
	 * program that doesn't mess with the bitmaps doesn't need to
	 * drag in the bitmaps.c code.
	 *
	 * Bitmap checksums live in the group descriptor, so the
	 * bitmaps need to be written before the descriptors.
	 */
	ఒకవేళ (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}

	/*
	 * Set the state of the FS to be non-valid.  (The state has
	 * already been backed up earlier, and will be restored after
	 * we write out the backup superblocks.)
	 */
	fs->super->s_state &= ~EXT2_VALID_FS;
	ext2fs_clear_feature_journal_needs_recovery(fs->super);

	/* Byte swap the superblock and the group descriptors if necessary */
#నిర్వచితమైతే పదాలు_BIGENDIAN
	retval = EXT2_ET_NO_జ్ఞాపకం;
	retval = ext2fs_get_mem(SUPERBLOCK_కొలత, &super_shadow);
	ఒకవేళ (retval)
		వెళ్ళు errout;
	జ్ఞాపకం_నకలు(super_shadow, fs->super, పరిమాణం(నిర్మాణం ext2_super_ఖండం));
	ext2fs_మార్పిడి_super(super_shadow);

	ఒకవేళ (((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0) &&
	    !ext2fs_has_feature_journal_dev(fs->super)) {
		retval = ext2fs_get_array(fs->desc_ఖండాలు, fs->blocksize,
					  &సమూహం_shadow);
		ఒకవేళ (retval)
			వెళ్ళు errout;
		జ్ఞాపకం_నకలు(సమూహం_shadow, fs->సమూహం_desc, (size_t) fs->blocksize *
		       fs->desc_ఖండాలు);

		కొరకు (j = 0; j < fs->సమూహం_desc_లెక్క; j++) {
			gdp = ext2fs_సమూహం_desc(fs, సమూహం_shadow, j);
			ext2fs_మార్పిడి_సమూహం_desc2(fs, gdp);
		}
	}
#లేకపోతే
	super_shadow = fs->super;
	సమూహం_shadow = fs->సమూహం_desc;
#అయితే_ముగింపు

	/*
	 * If this is an external journal device, don't write out the
	 * block group descriptors or any of the backup superblocks
	 */
	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super))
		వెళ్ళు write_primary_superblock_only;

	/*
	 * Write out the master group descriptors, and the backup
	 * superblocks and group descriptors.
	 */
	సమూహం_ptr = (అక్షరం *) సమూహం_shadow;
	ఒకవేళ (ext2fs_has_feature_meta_bg(fs->super)) {
		old_desc_ఖండాలు = fs->super->s_మొదటి_meta_bg;
		ఒకవేళ (old_desc_ఖండాలు > fs->desc_ఖండాలు)
			old_desc_ఖండాలు = fs->desc_ఖండాలు;
	} లేకపోతే
		old_desc_ఖండాలు = fs->desc_ఖండాలు;

	ఒకవేళ (fs->పురోగతి_ops && fs->పురోగతి_ops->init)
		(fs->పురోగతి_ops->init)(fs, &పురోగతి, ఖాళీ,
					 fs->సమూహం_desc_లెక్క);


	కొరకు (i = 0; i < fs->సమూహం_desc_లెక్క; i++) {
		blk64_t	super_blk, old_desc_blk, new_desc_blk;

		ఒకవేళ (fs->పురోగతి_ops && fs->పురోగతి_ops->update)
			(fs->పురోగతి_ops->update)(fs, &పురోగతి, i);
		ext2fs_super_and_bgd_loc2(fs, i, &super_blk, &old_desc_blk,
					 &new_desc_blk, 0);

		ఒకవేళ (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) &&i && super_blk) {
			retval = write_backup_super(fs, i, super_blk,
						    super_shadow);
			ఒకవేళ (retval)
				వెళ్ళు errout;
		}
		ఒకవేళ (fs->flags & EXT2_FLAG_SUPER_ONLY)
			కొనసాగు;
		ఒకవేళ ((old_desc_blk) &&
		    (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) || (i == 0))) {
			retval = io_channel_write_blk64(fs->io,
			      old_desc_blk, old_desc_ఖండాలు, సమూహం_ptr);
			ఒకవేళ (retval)
				వెళ్ళు errout;
		}
		ఒకవేళ (new_desc_blk) {
			పూర్ణాంకం meta_bg = i / EXT2_DESC_చొప్పున_ఖండం(fs->super);

			retval = io_channel_write_blk64(fs->io, new_desc_blk,
				1, సమూహం_ptr + (meta_bg*fs->blocksize));
			ఒకవేళ (retval)
				వెళ్ళు errout;
		}
	}

	ఒకవేళ (fs->పురోగతి_ops && fs->పురోగతి_ops->close)
		(fs->పురోగతి_ops->close)(fs, &పురోగతి, ఖాళీ);

write_primary_superblock_only:
	/*
	 * Write out master superblock.  This has to be done
	 * separately, since it is located at a fixed location
	 * (SUPERBLOCK_OFFSET).  We flush all other pending changes
	 * out to disk first, just to avoid a race condition with an
	 * insy-tinsy window....
	 */

	fs->super->s_ఖండం_సమూహం_nr = 0;
	fs->super->s_state = fs_state;
	fs->super->s_feature_incompat = feature_incompat;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	*super_shadow = *fs->super;
	ext2fs_మార్పిడి_super(super_shadow);
#అయితే_ముగింపు

	retval = ext2fs_superblock_csum_సమితి(fs, super_shadow);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}
	retval = write_primary_superblock(fs, super_shadow);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	fs->flags &= ~EXT2_FLAG_DIRTY;

	ఒకవేళ (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		ఒకవేళ (retval)
			వెళ్ళు errout;
	}
errout:
	fs->super->s_state = fs_state;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	ఒకవేళ (super_shadow)
		ext2fs_free_mem(&super_shadow);
	ఒకవేళ (సమూహం_shadow)
		ext2fs_free_mem(&సమూహం_shadow);
#అయితే_ముగింపు
	తిరిగి retval;
}

errcode_t ext2fs_close_free(ext2_filsys *fs_ptr)
{
	errcode_t ret;
	ext2_filsys fs = *fs_ptr;

	ret = ext2fs_close2(fs, 0);
	ఒకవేళ (ret)
		ext2fs_free(fs);
	*fs_ptr = ఖాళీ;
	తిరిగి ret;
}

errcode_t ext2fs_close(ext2_filsys fs)
{
	తిరిగి ext2fs_close2(fs, 0);
}

errcode_t ext2fs_close2(ext2_filsys fs, పూర్ణాంకం flags)
{
	errcode_t	retval;
	పూర్ణాంకం		meta_blks;
	io_stats stats = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	ఒకవేళ (fs->super->s_kbytes_written &&
	    fs->io->manager->get_stats)
		fs->io->manager->get_stats(fs->io, &stats);
	ఒకవేళ (stats && stats->బైట్లు_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->బైట్లు_written >> 10;
		meta_blks = fs->desc_ఖండాలు + 1;
		ఒకవేళ (!(fs->flags & EXT2_FLAG_SUPER_ONLY))
			fs->super->s_kbytes_written += meta_blks /
				(fs->blocksize / 1024);
		ఒకవేళ ((fs->flags & EXT2_FLAG_DIRTY) == 0)
			fs->flags |= EXT2_FLAG_SUPER_ONLY | EXT2_FLAG_DIRTY;
	}
	ఒకవేళ (fs->flags & EXT2_FLAG_DIRTY) {
		retval = ext2fs_flush2(fs, flags);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	retval = ext2fs_mmp_stop(fs);
	ఒకవేళ (retval)
		తిరిగి retval;

	ext2fs_free(fs);
	తిరిగి 0;
}

