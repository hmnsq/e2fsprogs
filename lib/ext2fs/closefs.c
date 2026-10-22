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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਸਥਿਰ ਪੂਰਨ test_ਜੜ੍ਹ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b)
{
	ਜਦੋਂ (1) {
		ਜੇ (a < b)
			ਵਾਪਸ 0;
		ਜੇ (a == b)
			ਵਾਪਸ 1;
		ਜੇ (a % b)
			ਵਾਪਸ 0;
		a = a / b;
	}
}

ਪੂਰਨ ext2fs_bg_has_super(ext2_filsys fs, dgrp_t ਸਮੂਹ)
{
	ਜੇ (ਸਮੂਹ == 0)
		ਵਾਪਸ 1;
	ਜੇ (ext2fs_has_feature_sparse_super2(fs->super)) {
		ਜੇ (ਸਮੂਹ == fs->super->s_backup_bgs[0] ||
		    ਸਮੂਹ == fs->super->s_backup_bgs[1])
			ਵਾਪਸ 1;
		ਵਾਪਸ 0;
	}
	ਜੇ ((ਸਮੂਹ <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		ਵਾਪਸ 1;
	ਜੇ (!(ਸਮੂਹ & 1))
		ਵਾਪਸ 0;
	ਜੇ (test_ਜੜ੍ਹ(ਸਮੂਹ, 3) || (test_ਜੜ੍ਹ(ਸਮੂਹ, 5)) ||
	    test_ਜੜ੍ਹ(ਸਮੂਹ, 7))
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
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
					   dgrp_t ਸਮੂਹ,
					   blk64_t *ret_super_blk,
					   blk64_t *ret_old_desc_blk,
					   blk64_t *ret_new_desc_blk,
					   blk_t *ret_ਵਰਤਿਆ_blks)
{
	blk64_t	ਸਮੂਹ_ਖੰਡ, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ meta_bg, meta_bg_ਮਾਪ;
	blk_t	numblocks = 0;
	blk64_t old_desc_ਖੰਡਾਂ;
	ਪੂਰਨ	has_super;

	ਸਮੂਹ_ਖੰਡ = ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, ਸਮੂਹ);
	ਜੇ (ਸਮੂਹ_ਖੰਡ == 0 && fs->blocksize == 1024)
		ਸਮੂਹ_ਖੰਡ = 1; /* Deal with 1024 blocksize && bigalloc */

	ਜੇ (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_ਖੰਡਾਂ = fs->super->s_ਪਹਿਲਾ_meta_bg;
	ਨਹੀਂਤਾਂ
		old_desc_ਖੰਡਾਂ =
			fs->desc_ਖੰਡਾਂ + fs->super->s_reserved_gdt_ਖੰਡਾਂ;

	has_super = ext2fs_bg_has_super(fs, ਸਮੂਹ);

	ਜੇ (has_super) {
		super_blk = ਸਮੂਹ_ਖੰਡ;
		numblocks++;
	}
	meta_bg_ਮਾਪ = EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super);
	meta_bg = ਸਮੂਹ / meta_bg_ਮਾਪ;

	ਜੇ (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_ਪਹਿਲਾ_meta_bg)) {
		ਜੇ (has_super) {
			old_desc_blk = ਸਮੂਹ_ਖੰਡ + 1;
			numblocks += old_desc_ਖੰਡਾਂ;
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (((ਸਮੂਹ % meta_bg_ਮਾਪ) == 0) ||
		    ((ਸਮੂਹ % meta_bg_ਮਾਪ) == 1) ||
		    ((ਸਮੂਹ % meta_bg_ਮਾਪ) == (meta_bg_ਮਾਪ-1))) {
			ਜੇ (has_super)
				has_super = 1;
			new_desc_blk = ਸਮੂਹ_ਖੰਡ + has_super;
			numblocks++;
		}
	}

	ਜੇ (ret_super_blk)
		*ret_super_blk = super_blk;
	ਜੇ (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	ਜੇ (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	ਜੇ (ret_ਵਰਤਿਆ_blks)
		*ret_ਵਰਤਿਆ_blks = numblocks;

	ਵਾਪਸ 0;
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
ਪੂਰਨ ext2fs_super_and_bgd_loc(ext2_filsys fs,
			     dgrp_t ਸਮੂਹ,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     ਪੂਰਨ *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_ਵਰਤਿਆ_blks;
	blk_t numblocks;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ meta_bg_ਮਾਪ;

	ext2fs_super_and_bgd_loc2(fs, ਸਮੂਹ, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_ਵਰਤਿਆ_blks);

	numblocks = ext2fs_ਸਮੂਹ_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, ਸਮੂਹ);

	ਜੇ (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	ਜੇ (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	ਜੇ (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	ਜੇ (ret_meta_bg) {
		meta_bg_ਮਾਪ = EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super);
		*ret_meta_bg = ਸਮੂਹ / meta_bg_ਮਾਪ;
	}

	numblocks -= 2 + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ + ret_ਵਰਤਿਆ_blks;

	ਵਾਪਸ numblocks;
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
ਸਥਿਰ errcode_t write_primary_superblock(ext2_filsys fs,
					  ਬਣਤਰ ext2_super_ਖੰਡ *super)
{
	__u16		*old_super, *new_super;
	ਪੂਰਨ		check_idx, write_idx, ਮਾਪ;
	errcode_t	retval;

	ਜੇ (!fs->io->manager->write_byte || !fs->orig_super) {
	fallback:
		io_channel_ਸੈੱਟ_blksize(fs->io, SUPERBLOCK_ਵਿੱਥ);
		retval = io_channel_write_blk64(fs->io, 1, -SUPERBLOCK_ਮਾਪ,
					      super);
		io_channel_ਸੈੱਟ_blksize(fs->io, fs->blocksize);
		ਵਾਪਸ retval;
	}

	old_super = (__u16 *) fs->orig_super;
	new_super = (__u16 *) super;

	ਲਈ (check_idx = 0; check_idx < SUPERBLOCK_ਮਾਪ/2; check_idx++) {
		ਜੇ (old_super[check_idx] == new_super[check_idx])
			ਜਾਰੀ;
		write_idx = check_idx;
		ਲਈ (check_idx++; check_idx < SUPERBLOCK_ਮਾਪ/2; check_idx++)
			ਜੇ (old_super[check_idx] == new_super[check_idx])
				ਰੋਕੋ;
		ਮਾਪ = 2 * (check_idx - write_idx);
#ਜੇ 0
		ਲਿਖੋ("Writing %d bytes starting at %d\n",
		       ਮਾਪ, write_idx*2);
#ਜੇ_ਅੰਤ
		retval = io_channel_write_byte(fs->io,
			       SUPERBLOCK_ਵਿੱਥ + (2 * write_idx), ਮਾਪ,
					       new_super + write_idx);
		ਜੇ (retval == EXT2_ET_UNIMPLEMENTED)
			ਜਾਓ fallback;
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->orig_super, super, SUPERBLOCK_ਮਾਪ);
	ਵਾਪਸ 0;
}


/*
 * Updates the revision to EXT2_DYNAMIC_REV
 */
ਖਾਲੀ ext2fs_update_dynamic_rev(ext2_filsys fs)
{
	ਬਣਤਰ ext2_super_ਖੰਡ *sb = fs->super;

	ਜੇ (sb->s_rev_level > EXT2_GOOD_OLD_REV)
		ਵਾਪਸ;

	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_ਪਹਿਲਾ_ino = EXT2_GOOD_OLD_ਪਹਿਲਾ_INO;
	sb->s_inode_ਮਾਪ = EXT2_GOOD_OLD_INODE_ਮਾਪ;
	/* s_uuid is handled by e2fsck already */
	/* other fields should be left alone */
}

ਸਥਿਰ errcode_t write_backup_super(ext2_filsys fs, dgrp_t ਸਮੂਹ,
				    blk64_t ਸਮੂਹ_ਖੰਡ,
				    ਬਣਤਰ ext2_super_ਖੰਡ *super_shadow)
{
	errcode_t retval;
	dgrp_t	sgrp = ਸਮੂਹ;

	ਜੇ (sgrp > ((1 << 16) - 1))
		sgrp = (1 << 16) - 1;

	super_shadow->s_ਖੰਡ_ਸਮੂਹ_nr = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(sgrp);

	retval = ext2fs_superblock_csum_ਸੈੱਟ(fs, super_shadow);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਵਾਪਸ io_channel_write_blk64(fs->io, ਸਮੂਹ_ਖੰਡ, -SUPERBLOCK_ਮਾਪ,
				    super_shadow);
}

errcode_t ext2fs_flush(ext2_filsys fs)
{
	ਵਾਪਸ ext2fs_flush2(fs, 0);
}

errcode_t ext2fs_flush2(ext2_filsys fs, ਪੂਰਨ flags)
{
	dgrp_t		i;
	errcode_t	retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	fs_state;
	__u32		feature_incompat;
	ਬਣਤਰ ext2_super_ਖੰਡ *super_shadow = 0;
	ਬਣਤਰ opaque_ext2_ਸਮੂਹ_desc *ਸਮੂਹ_shadow = 0;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਬਣਤਰ ext2_ਸਮੂਹ_desc *gdp;
	dgrp_t		j;
#ਜੇ_ਅੰਤ
	ਅੱਖਰ	*ਸਮੂਹ_ptr;
	blk64_t	old_desc_ਖੰਡਾਂ;
	ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct ਤਰੱਕੀ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ ((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0 &&
	    !ext2fs_has_feature_journal_dev(fs->super) &&
	    fs->ਸਮੂਹ_desc == ਨਿਰਾ)
		ਵਾਪਸ EXT2_ET_NO_GDESC;

	fs_state = fs->super->s_state;
	feature_incompat = fs->super->s_feature_incompat;

	ext2fs_ਸੈੱਟ_tstamp(fs->super, s_wtime, ext2fs_P_get_ਸਮਾਂ(fs));
	fs->super->s_ਖੰਡ_ਸਮੂਹ_nr = 0;

	/*
	 * If the write_bitmaps() function is present, call it to
	 * flush the bitmaps.  This is done this way so that a simple
	 * program that doesn't mess with the bitmaps doesn't need to
	 * drag in the bitmaps.c code.
	 *
	 * Bitmap checksums live in the group descriptor, so the
	 * bitmaps need to be written before the descriptors.
	 */
	ਜੇ (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		ਜੇ (retval)
			ਜਾਓ errout;
	}

	/*
	 * Set the state of the FS to be non-valid.  (The state has
	 * already been backed up earlier, and will be restored after
	 * we write out the backup superblocks.)
	 */
	fs->super->s_state &= ~EXT2_VALID_FS;
	ext2fs_clear_feature_journal_needs_recovery(fs->super);

	/* Byte swap the superblock and the group descriptors if necessary */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	retval = EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
	retval = ext2fs_get_mem(SUPERBLOCK_ਮਾਪ, &super_shadow);
	ਜੇ (retval)
		ਜਾਓ errout;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(super_shadow, fs->super, ਆਕਾਰ(ਬਣਤਰ ext2_super_ਖੰਡ));
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_super(super_shadow);

	ਜੇ (((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0) &&
	    !ext2fs_has_feature_journal_dev(fs->super)) {
		retval = ext2fs_get_array(fs->desc_ਖੰਡਾਂ, fs->blocksize,
					  &ਸਮੂਹ_shadow);
		ਜੇ (retval)
			ਜਾਓ errout;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਸਮੂਹ_shadow, fs->ਸਮੂਹ_desc, (size_t) fs->blocksize *
		       fs->desc_ਖੰਡਾਂ);

		ਲਈ (j = 0; j < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; j++) {
			gdp = ext2fs_ਸਮੂਹ_desc(fs, ਸਮੂਹ_shadow, j);
			ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(fs, gdp);
		}
	}
#ਨਹੀਂਤਾਂ
	super_shadow = fs->super;
	ਸਮੂਹ_shadow = fs->ਸਮੂਹ_desc;
#ਜੇ_ਅੰਤ

	/*
	 * If this is an external journal device, don't write out the
	 * block group descriptors or any of the backup superblocks
	 */
	ਜੇ (ext2fs_has_feature_journal_dev(fs->super))
		ਜਾਓ write_primary_superblock_only;

	/*
	 * Write out the master group descriptors, and the backup
	 * superblocks and group descriptors.
	 */
	ਸਮੂਹ_ptr = (ਅੱਖਰ *) ਸਮੂਹ_shadow;
	ਜੇ (ext2fs_has_feature_meta_bg(fs->super)) {
		old_desc_ਖੰਡਾਂ = fs->super->s_ਪਹਿਲਾ_meta_bg;
		ਜੇ (old_desc_ਖੰਡਾਂ > fs->desc_ਖੰਡਾਂ)
			old_desc_ਖੰਡਾਂ = fs->desc_ਖੰਡਾਂ;
	} ਨਹੀਂਤਾਂ
		old_desc_ਖੰਡਾਂ = fs->desc_ਖੰਡਾਂ;

	ਜੇ (fs->ਤਰੱਕੀ_ops && fs->ਤਰੱਕੀ_ops->init)
		(fs->ਤਰੱਕੀ_ops->init)(fs, &ਤਰੱਕੀ, ਨਿਰਾ,
					 fs->ਸਮੂਹ_desc_ਗਿਣਤੀ);


	ਲਈ (i = 0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		blk64_t	super_blk, old_desc_blk, new_desc_blk;

		ਜੇ (fs->ਤਰੱਕੀ_ops && fs->ਤਰੱਕੀ_ops->update)
			(fs->ਤਰੱਕੀ_ops->update)(fs, &ਤਰੱਕੀ, i);
		ext2fs_super_and_bgd_loc2(fs, i, &super_blk, &old_desc_blk,
					 &new_desc_blk, 0);

		ਜੇ (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) &&i && super_blk) {
			retval = write_backup_super(fs, i, super_blk,
						    super_shadow);
			ਜੇ (retval)
				ਜਾਓ errout;
		}
		ਜੇ (fs->flags & EXT2_FLAG_SUPER_ONLY)
			ਜਾਰੀ;
		ਜੇ ((old_desc_blk) &&
		    (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) || (i == 0))) {
			retval = io_channel_write_blk64(fs->io,
			      old_desc_blk, old_desc_ਖੰਡਾਂ, ਸਮੂਹ_ptr);
			ਜੇ (retval)
				ਜਾਓ errout;
		}
		ਜੇ (new_desc_blk) {
			ਪੂਰਨ meta_bg = i / EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super);

			retval = io_channel_write_blk64(fs->io, new_desc_blk,
				1, ਸਮੂਹ_ptr + (meta_bg*fs->blocksize));
			ਜੇ (retval)
				ਜਾਓ errout;
		}
	}

	ਜੇ (fs->ਤਰੱਕੀ_ops && fs->ਤਰੱਕੀ_ops->close)
		(fs->ਤਰੱਕੀ_ops->close)(fs, &ਤਰੱਕੀ, ਨਿਰਾ);

write_primary_superblock_only:
	/*
	 * Write out master superblock.  This has to be done
	 * separately, since it is located at a fixed location
	 * (SUPERBLOCK_OFFSET).  We flush all other pending changes
	 * out to disk first, just to avoid a race condition with an
	 * insy-tinsy window....
	 */

	fs->super->s_ਖੰਡ_ਸਮੂਹ_nr = 0;
	fs->super->s_state = fs_state;
	fs->super->s_feature_incompat = feature_incompat;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	*super_shadow = *fs->super;
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_super(super_shadow);
#ਜੇ_ਅੰਤ

	retval = ext2fs_superblock_csum_ਸੈੱਟ(fs, super_shadow);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		ਜੇ (retval)
			ਜਾਓ errout;
	}
	retval = write_primary_superblock(fs, super_shadow);
	ਜੇ (retval)
		ਜਾਓ errout;

	fs->flags &= ~EXT2_FLAG_DIRTY;

	ਜੇ (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		ਜੇ (retval)
			ਜਾਓ errout;
	}
errout:
	fs->super->s_state = fs_state;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਜੇ (super_shadow)
		ext2fs_free_mem(&super_shadow);
	ਜੇ (ਸਮੂਹ_shadow)
		ext2fs_free_mem(&ਸਮੂਹ_shadow);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ retval;
}

errcode_t ext2fs_close_free(ext2_filsys *fs_ptr)
{
	errcode_t ret;
	ext2_filsys fs = *fs_ptr;

	ret = ext2fs_close2(fs, 0);
	ਜੇ (ret)
		ext2fs_free(fs);
	*fs_ptr = ਨਿਰਾ;
	ਵਾਪਸ ret;
}

errcode_t ext2fs_close(ext2_filsys fs)
{
	ਵਾਪਸ ext2fs_close2(fs, 0);
}

errcode_t ext2fs_close2(ext2_filsys fs, ਪੂਰਨ flags)
{
	errcode_t	retval;
	ਪੂਰਨ		meta_blks;
	io_stats stats = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ਜੇ (fs->super->s_kbytes_written &&
	    fs->io->manager->get_stats)
		fs->io->manager->get_stats(fs->io, &stats);
	ਜੇ (stats && stats->ਬਾਈਟਾਂ_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->ਬਾਈਟਾਂ_written >> 10;
		meta_blks = fs->desc_ਖੰਡਾਂ + 1;
		ਜੇ (!(fs->flags & EXT2_FLAG_SUPER_ONLY))
			fs->super->s_kbytes_written += meta_blks /
				(fs->blocksize / 1024);
		ਜੇ ((fs->flags & EXT2_FLAG_DIRTY) == 0)
			fs->flags |= EXT2_FLAG_SUPER_ONLY | EXT2_FLAG_DIRTY;
	}
	ਜੇ (fs->flags & EXT2_FLAG_DIRTY) {
		retval = ext2fs_flush2(fs, flags);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	retval = ext2fs_mmp_stop(fs);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ext2fs_free(fs);
	ਵਾਪਸ 0;
}

