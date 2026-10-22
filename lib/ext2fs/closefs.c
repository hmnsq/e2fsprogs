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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ test_ಬೇರು(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ a, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ b)
{
	ಆಗ (1) {
		ಆದರೆ (a < b)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (a == b)
			ಹಿಂತಿರುಗಿ 1;
		ಆದರೆ (a % b)
			ಹಿಂತಿರುಗಿ 0;
		a = a / b;
	}
}

ಪೂರ್ಣಾಂಕ ext2fs_bg_has_super(ext2_filsys fs, dgrp_t ಗುಂಪು)
{
	ಆದರೆ (ಗುಂಪು == 0)
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (ext2fs_has_feature_sparse_super2(fs->super)) {
		ಆದರೆ (ಗುಂಪು == fs->super->s_backup_bgs[0] ||
		    ಗುಂಪು == fs->super->s_backup_bgs[1])
			ಹಿಂತಿರುಗಿ 1;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ ((ಗುಂಪು <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (!(ಗುಂಪು & 1))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (test_ಬೇರು(ಗುಂಪು, 3) || (test_ಬೇರು(ಗುಂಪು, 5)) ||
	    test_ಬೇರು(ಗುಂಪು, 7))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
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
					   dgrp_t ಗುಂಪು,
					   blk64_t *ret_super_blk,
					   blk64_t *ret_old_desc_blk,
					   blk64_t *ret_new_desc_blk,
					   blk_t *ret_ಬಳಸಿದ_blks)
{
	blk64_t	ಗುಂಪು_ಖಂಡ, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ meta_bg, meta_bg_ಗಾತ್ರ;
	blk_t	numblocks = 0;
	blk64_t old_desc_ಖಂಡಗಳು;
	ಪೂರ್ಣಾಂಕ	has_super;

	ಗುಂಪು_ಖಂಡ = ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, ಗುಂಪು);
	ಆದರೆ (ಗುಂಪು_ಖಂಡ == 0 && fs->blocksize == 1024)
		ಗುಂಪು_ಖಂಡ = 1; /* Deal with 1024 blocksize && bigalloc */

	ಆದರೆ (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_ಖಂಡಗಳು = fs->super->s_ಮೊದಲ_meta_bg;
	ಇಲ್ಲದಿದ್ದರೆ
		old_desc_ಖಂಡಗಳು =
			fs->desc_ಖಂಡಗಳು + fs->super->s_reserved_gdt_ಖಂಡಗಳು;

	has_super = ext2fs_bg_has_super(fs, ಗುಂಪು);

	ಆದರೆ (has_super) {
		super_blk = ಗುಂಪು_ಖಂಡ;
		numblocks++;
	}
	meta_bg_ಗಾತ್ರ = EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super);
	meta_bg = ಗುಂಪು / meta_bg_ಗಾತ್ರ;

	ಆದರೆ (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_ಮೊದಲ_meta_bg)) {
		ಆದರೆ (has_super) {
			old_desc_blk = ಗುಂಪು_ಖಂಡ + 1;
			numblocks += old_desc_ಖಂಡಗಳು;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (((ಗುಂಪು % meta_bg_ಗಾತ್ರ) == 0) ||
		    ((ಗುಂಪು % meta_bg_ಗಾತ್ರ) == 1) ||
		    ((ಗುಂಪು % meta_bg_ಗಾತ್ರ) == (meta_bg_ಗಾತ್ರ-1))) {
			ಆದರೆ (has_super)
				has_super = 1;
			new_desc_blk = ಗುಂಪು_ಖಂಡ + has_super;
			numblocks++;
		}
	}

	ಆದರೆ (ret_super_blk)
		*ret_super_blk = super_blk;
	ಆದರೆ (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	ಆದರೆ (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	ಆದರೆ (ret_ಬಳಸಿದ_blks)
		*ret_ಬಳಸಿದ_blks = numblocks;

	ಹಿಂತಿರುಗಿ 0;
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
ಪೂರ್ಣಾಂಕ ext2fs_super_and_bgd_loc(ext2_filsys fs,
			     dgrp_t ಗುಂಪು,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     ಪೂರ್ಣಾಂಕ *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_ಬಳಸಿದ_blks;
	blk_t numblocks;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ meta_bg_ಗಾತ್ರ;

	ext2fs_super_and_bgd_loc2(fs, ಗುಂಪು, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_ಬಳಸಿದ_blks);

	numblocks = ext2fs_ಗುಂಪು_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, ಗುಂಪು);

	ಆದರೆ (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	ಆದರೆ (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	ಆದರೆ (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	ಆದರೆ (ret_meta_bg) {
		meta_bg_ಗಾತ್ರ = EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super);
		*ret_meta_bg = ಗುಂಪು / meta_bg_ಗಾತ್ರ;
	}

	numblocks -= 2 + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು + ret_ಬಳಸಿದ_blks;

	ಹಿಂತಿರುಗಿ numblocks;
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
ಸ್ಥಿರ errcode_t write_primary_superblock(ext2_filsys fs,
					  ರಚನೆ ext2_super_ಖಂಡ *super)
{
	__u16		*old_super, *new_super;
	ಪೂರ್ಣಾಂಕ		check_idx, write_idx, ಗಾತ್ರ_2;
	errcode_t	retval;

	ಆದರೆ (!fs->io->manager->write_byte || !fs->orig_super) {
	fallback:
		io_channel_ಗಣ_blksize(fs->io, SUPERBLOCK_ಸ್ಥಾನಾಂತರ);
		retval = io_channel_write_blk64(fs->io, 1, -SUPERBLOCK_ಗಾತ್ರ,
					      super);
		io_channel_ಗಣ_blksize(fs->io, fs->blocksize);
		ಹಿಂತಿರುಗಿ retval;
	}

	old_super = (__u16 *) fs->orig_super;
	new_super = (__u16 *) super;

	ಗಾಗಿ (check_idx = 0; check_idx < SUPERBLOCK_ಗಾತ್ರ/2; check_idx++) {
		ಆದರೆ (old_super[check_idx] == new_super[check_idx])
			ಮುಂದುವರಿ;
		write_idx = check_idx;
		ಗಾಗಿ (check_idx++; check_idx < SUPERBLOCK_ಗಾತ್ರ/2; check_idx++)
			ಆದರೆ (old_super[check_idx] == new_super[check_idx])
				ನಿಲ್ಲಿಸು;
		ಗಾತ್ರ_2 = 2 * (check_idx - write_idx);
#ಆದರೆ 0
		ಬರೆ("Writing %d bytes starting at %d\n",
		       ಗಾತ್ರ_2, write_idx*2);
#ಆದರೆ_ಅಂತ್ಯ
		retval = io_channel_write_byte(fs->io,
			       SUPERBLOCK_ಸ್ಥಾನಾಂತರ + (2 * write_idx), ಗಾತ್ರ_2,
					       new_super + write_idx);
		ಆದರೆ (retval == EXT2_ET_UNIMPLEMENTED)
			ಹೋಗು fallback;
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ಸ್ಮರಣೆ_ನಕಲು(fs->orig_super, super, SUPERBLOCK_ಗಾತ್ರ);
	ಹಿಂತಿರುಗಿ 0;
}


/*
 * Updates the revision to EXT2_DYNAMIC_REV
 */
ಶೂನ್ಯ ext2fs_update_dynamic_rev(ext2_filsys fs)
{
	ರಚನೆ ext2_super_ಖಂಡ *sb = fs->super;

	ಆದರೆ (sb->s_rev_level > EXT2_GOOD_OLD_REV)
		ಹಿಂತಿರುಗಿ;

	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_ಮೊದಲ_ino = EXT2_GOOD_OLD_ಮೊದಲ_INO;
	sb->s_inode_ಗಾತ್ರ = EXT2_GOOD_OLD_INODE_ಗಾತ್ರ;
	/* s_uuid is handled by e2fsck already */
	/* other fields should be left alone */
}

ಸ್ಥಿರ errcode_t write_backup_super(ext2_filsys fs, dgrp_t ಗುಂಪು,
				    blk64_t ಗುಂಪು_ಖಂಡ,
				    ರಚನೆ ext2_super_ಖಂಡ *super_shadow)
{
	errcode_t retval;
	dgrp_t	sgrp = ಗುಂಪು;

	ಆದರೆ (sgrp > ((1 << 16) - 1))
		sgrp = (1 << 16) - 1;

	super_shadow->s_ಖಂಡ_ಗುಂಪು_nr = ext2fs_cpu_ಗಮ್ಯ_le16(sgrp);

	retval = ext2fs_superblock_csum_ಗಣ(fs, super_shadow);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಹಿಂತಿರುಗಿ io_channel_write_blk64(fs->io, ಗುಂಪು_ಖಂಡ, -SUPERBLOCK_ಗಾತ್ರ,
				    super_shadow);
}

errcode_t ext2fs_flush(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ ext2fs_flush2(fs, 0);
}

errcode_t ext2fs_flush2(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags)
{
	dgrp_t		i;
	errcode_t	retval;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	fs_state;
	__u32		feature_incompat;
	ರಚನೆ ext2_super_ಖಂಡ *super_shadow = 0;
	ರಚನೆ opaque_ext2_ಗುಂಪು_desc *ಗುಂಪು_shadow = 0;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ರಚನೆ ext2_ಗುಂಪು_desc *gdp;
	dgrp_t		j;
#ಆದರೆ_ಅಂತ್ಯ
	ಅಕ್ಷರ	*ಗುಂಪು_ptr;
	blk64_t	old_desc_ಖಂಡಗಳು;
	ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct ಪ್ರಗತಿ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ ((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0 &&
	    !ext2fs_has_feature_journal_dev(fs->super) &&
	    fs->ಗುಂಪು_desc == ಖಾಲಿ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_GDESC;

	fs_state = fs->super->s_state;
	feature_incompat = fs->super->s_feature_incompat;

	ext2fs_ಗಣ_tstamp(fs->super, s_wtime, ext2fs_P_get_ಸಮಯ(fs));
	fs->super->s_ಖಂಡ_ಗುಂಪು_nr = 0;

	/*
	 * If the write_bitmaps() function is present, call it to
	 * flush the bitmaps.  This is done this way so that a simple
	 * program that doesn't mess with the bitmaps doesn't need to
	 * drag in the bitmaps.c code.
	 *
	 * Bitmap checksums live in the group descriptor, so the
	 * bitmaps need to be written before the descriptors.
	 */
	ಆದರೆ (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}

	/*
	 * Set the state of the FS to be non-valid.  (The state has
	 * already been backed up earlier, and will be restored after
	 * we write out the backup superblocks.)
	 */
	fs->super->s_state &= ~EXT2_VALID_FS;
	ext2fs_clear_feature_journal_needs_recovery(fs->super);

	/* Byte swap the superblock and the group descriptors if necessary */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	retval = EXT2_ET_NO_ಸ್ಮರಣೆ;
	retval = ext2fs_get_mem(SUPERBLOCK_ಗಾತ್ರ, &super_shadow);
	ಆದರೆ (retval)
		ಹೋಗು errout;
	ಸ್ಮರಣೆ_ನಕಲು(super_shadow, fs->super, ಗಾತ್ರ(ರಚನೆ ext2_super_ಖಂಡ));
	ext2fs_ಅದಲುಬದಲು_super(super_shadow);

	ಆದರೆ (((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0) &&
	    !ext2fs_has_feature_journal_dev(fs->super)) {
		retval = ext2fs_get_array(fs->desc_ಖಂಡಗಳು, fs->blocksize,
					  &ಗುಂಪು_shadow);
		ಆದರೆ (retval)
			ಹೋಗು errout;
		ಸ್ಮರಣೆ_ನಕಲು(ಗುಂಪು_shadow, fs->ಗುಂಪು_desc, (size_t) fs->blocksize *
		       fs->desc_ಖಂಡಗಳು);

		ಗಾಗಿ (j = 0; j < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; j++) {
			gdp = ext2fs_ಗುಂಪು_desc(fs, ಗುಂಪು_shadow, j);
			ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(fs, gdp);
		}
	}
#ಇಲ್ಲದಿದ್ದರೆ
	super_shadow = fs->super;
	ಗುಂಪು_shadow = fs->ಗುಂಪು_desc;
#ಆದರೆ_ಅಂತ್ಯ

	/*
	 * If this is an external journal device, don't write out the
	 * block group descriptors or any of the backup superblocks
	 */
	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super))
		ಹೋಗು write_primary_superblock_only;

	/*
	 * Write out the master group descriptors, and the backup
	 * superblocks and group descriptors.
	 */
	ಗುಂಪು_ptr = (ಅಕ್ಷರ *) ಗುಂಪು_shadow;
	ಆದರೆ (ext2fs_has_feature_meta_bg(fs->super)) {
		old_desc_ಖಂಡಗಳು = fs->super->s_ಮೊದಲ_meta_bg;
		ಆದರೆ (old_desc_ಖಂಡಗಳು > fs->desc_ಖಂಡಗಳು)
			old_desc_ಖಂಡಗಳು = fs->desc_ಖಂಡಗಳು;
	} ಇಲ್ಲದಿದ್ದರೆ
		old_desc_ಖಂಡಗಳು = fs->desc_ಖಂಡಗಳು;

	ಆದರೆ (fs->ಪ್ರಗತಿ_ops && fs->ಪ್ರಗತಿ_ops->init)
		(fs->ಪ್ರಗತಿ_ops->init)(fs, &ಪ್ರಗತಿ, ಖಾಲಿ,
					 fs->ಗುಂಪು_desc_ಎಣಿಕೆ);


	ಗಾಗಿ (i = 0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		blk64_t	super_blk, old_desc_blk, new_desc_blk;

		ಆದರೆ (fs->ಪ್ರಗತಿ_ops && fs->ಪ್ರಗತಿ_ops->update)
			(fs->ಪ್ರಗತಿ_ops->update)(fs, &ಪ್ರಗತಿ, i);
		ext2fs_super_and_bgd_loc2(fs, i, &super_blk, &old_desc_blk,
					 &new_desc_blk, 0);

		ಆದರೆ (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) &&i && super_blk) {
			retval = write_backup_super(fs, i, super_blk,
						    super_shadow);
			ಆದರೆ (retval)
				ಹೋಗು errout;
		}
		ಆದರೆ (fs->flags & EXT2_FLAG_SUPER_ONLY)
			ಮುಂದುವರಿ;
		ಆದರೆ ((old_desc_blk) &&
		    (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) || (i == 0))) {
			retval = io_channel_write_blk64(fs->io,
			      old_desc_blk, old_desc_ಖಂಡಗಳು, ಗುಂಪು_ptr);
			ಆದರೆ (retval)
				ಹೋಗು errout;
		}
		ಆದರೆ (new_desc_blk) {
			ಪೂರ್ಣಾಂಕ meta_bg = i / EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super);

			retval = io_channel_write_blk64(fs->io, new_desc_blk,
				1, ಗುಂಪು_ptr + (meta_bg*fs->blocksize));
			ಆದರೆ (retval)
				ಹೋಗು errout;
		}
	}

	ಆದರೆ (fs->ಪ್ರಗತಿ_ops && fs->ಪ್ರಗತಿ_ops->close)
		(fs->ಪ್ರಗತಿ_ops->close)(fs, &ಪ್ರಗತಿ, ಖಾಲಿ);

write_primary_superblock_only:
	/*
	 * Write out master superblock.  This has to be done
	 * separately, since it is located at a fixed location
	 * (SUPERBLOCK_OFFSET).  We flush all other pending changes
	 * out to disk first, just to avoid a race condition with an
	 * insy-tinsy window....
	 */

	fs->super->s_ಖಂಡ_ಗುಂಪು_nr = 0;
	fs->super->s_state = fs_state;
	fs->super->s_feature_incompat = feature_incompat;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	*super_shadow = *fs->super;
	ext2fs_ಅದಲುಬದಲು_super(super_shadow);
#ಆದರೆ_ಅಂತ್ಯ

	retval = ext2fs_superblock_csum_ಗಣ(fs, super_shadow);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}
	retval = write_primary_superblock(fs, super_shadow);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	fs->flags &= ~EXT2_FLAG_DIRTY;

	ಆದರೆ (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}
errout:
	fs->super->s_state = fs_state;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ಆದರೆ (super_shadow)
		ext2fs_free_mem(&super_shadow);
	ಆದರೆ (ಗುಂಪು_shadow)
		ext2fs_free_mem(&ಗುಂಪು_shadow);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_close_free(ext2_filsys *fs_ptr)
{
	errcode_t ret;
	ext2_filsys fs = *fs_ptr;

	ret = ext2fs_close2(fs, 0);
	ಆದರೆ (ret)
		ext2fs_free(fs);
	*fs_ptr = ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ ret;
}

errcode_t ext2fs_close(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ ext2fs_close2(fs, 0);
}

errcode_t ext2fs_close2(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags)
{
	errcode_t	retval;
	ಪೂರ್ಣಾಂಕ		meta_blks;
	io_stats stats = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ಆದರೆ (fs->super->s_kbytes_written &&
	    fs->io->manager->get_stats)
		fs->io->manager->get_stats(fs->io, &stats);
	ಆದರೆ (stats && stats->ಬೈಟುಗಳು_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->ಬೈಟುಗಳು_written >> 10;
		meta_blks = fs->desc_ಖಂಡಗಳು + 1;
		ಆದರೆ (!(fs->flags & EXT2_FLAG_SUPER_ONLY))
			fs->super->s_kbytes_written += meta_blks /
				(fs->blocksize / 1024);
		ಆದರೆ ((fs->flags & EXT2_FLAG_DIRTY) == 0)
			fs->flags |= EXT2_FLAG_SUPER_ONLY | EXT2_FLAG_DIRTY;
	}
	ಆದರೆ (fs->flags & EXT2_FLAG_DIRTY) {
		retval = ext2fs_flush2(fs, flags);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	retval = ext2fs_mmp_stop(fs);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ext2fs_free(fs);
	ಹಿಂತಿರುಗಿ 0;
}

