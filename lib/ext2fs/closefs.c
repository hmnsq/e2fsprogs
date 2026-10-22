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

#ضمن "config.h"
#ضمن <stdio.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#ضمن <time.h>
#ضمن <string.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

ساكن صحيح test_جڑ(بنانشانی صحيح a, بنانشانی صحيح b)
{
	طالما (1) {
		إذا (a < b)
			أرجع 0;
		إذا (a == b)
			أرجع 1;
		إذا (a % b)
			أرجع 0;
		a = a / b;
	}
}

صحيح ext2fs_bg_has_super(ext2_filsys fs, dgrp_t گروہ)
{
	إذا (گروہ == 0)
		أرجع 1;
	إذا (ext2fs_has_feature_sparse_super2(fs->super)) {
		إذا (گروہ == fs->super->s_backup_bgs[0] ||
		    گروہ == fs->super->s_backup_bgs[1])
			أرجع 1;
		أرجع 0;
	}
	إذا ((گروہ <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		أرجع 1;
	إذا (!(گروہ & 1))
		أرجع 0;
	إذا (test_جڑ(گروہ, 3) || (test_جڑ(گروہ, 5)) ||
	    test_جڑ(گروہ, 7))
		أرجع 1;

	أرجع 0;
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
					   dgrp_t گروہ,
					   blk64_t *ret_super_blk,
					   blk64_t *ret_old_desc_blk,
					   blk64_t *ret_new_desc_blk,
					   blk_t *ret_ورتیا_blks)
{
	blk64_t	گروہ_بلاک, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	بنانشانی صحيح meta_bg, meta_bg_ناپ;
	blk_t	numblocks = 0;
	blk64_t old_desc_بلاکاں;
	صحيح	has_super;

	گروہ_بلاک = ext2fs_گروہ_پہلا_block2(fs, گروہ);
	إذا (گروہ_بلاک == 0 && fs->blocksize == 1024)
		گروہ_بلاک = 1; /* Deal with 1024 blocksize && bigalloc */

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_بلاکاں = fs->super->s_پہلا_meta_bg;
	وإلا
		old_desc_بلاکاں =
			fs->desc_بلاکاں + fs->super->s_reserved_gdt_بلاکاں;

	has_super = ext2fs_bg_has_super(fs, گروہ);

	إذا (has_super) {
		super_blk = گروہ_بلاک;
		numblocks++;
	}
	meta_bg_ناپ = EXT2_DESC_فی_بلاک(fs->super);
	meta_bg = گروہ / meta_bg_ناپ;

	إذا (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_پہلا_meta_bg)) {
		إذا (has_super) {
			old_desc_blk = گروہ_بلاک + 1;
			numblocks += old_desc_بلاکاں;
		}
	} وإلا {
		إذا (((گروہ % meta_bg_ناپ) == 0) ||
		    ((گروہ % meta_bg_ناپ) == 1) ||
		    ((گروہ % meta_bg_ناپ) == (meta_bg_ناپ-1))) {
			إذا (has_super)
				has_super = 1;
			new_desc_blk = گروہ_بلاک + has_super;
			numblocks++;
		}
	}

	إذا (ret_super_blk)
		*ret_super_blk = super_blk;
	إذا (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	إذا (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	إذا (ret_ورتیا_blks)
		*ret_ورتیا_blks = numblocks;

	أرجع 0;
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
صحيح ext2fs_super_and_bgd_loc(ext2_filsys fs,
			     dgrp_t گروہ,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     صحيح *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_ورتیا_blks;
	blk_t numblocks;
	بنانشانی صحيح meta_bg_ناپ;

	ext2fs_super_and_bgd_loc2(fs, گروہ, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_ورتیا_blks);

	numblocks = ext2fs_گروہ_بلاکاں_گنتی(fs, گروہ);

	إذا (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	إذا (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	إذا (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	إذا (ret_meta_bg) {
		meta_bg_ناپ = EXT2_DESC_فی_بلاک(fs->super);
		*ret_meta_bg = گروہ / meta_bg_ناپ;
	}

	numblocks -= 2 + fs->inode_بلاکاں_فی_گروہ + ret_ورتیا_blks;

	أرجع numblocks;
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
ساكن errcode_t write_primary_superblock(ext2_filsys fs,
					  بنية ext2_super_بلاک *super)
{
	__u16		*old_super, *new_super;
	صحيح		check_idx, write_idx, ناپ;
	errcode_t	retval;

	إذا (!fs->io->manager->write_byte || !fs->orig_super) {
	fallback:
		io_channel_سیٹ_blksize(fs->io, SUPERBLOCK_وتھ);
		retval = io_channel_write_blk64(fs->io, 1, -SUPERBLOCK_ناپ,
					      super);
		io_channel_سیٹ_blksize(fs->io, fs->blocksize);
		أرجع retval;
	}

	old_super = (__u16 *) fs->orig_super;
	new_super = (__u16 *) super;

	لكل (check_idx = 0; check_idx < SUPERBLOCK_ناپ/2; check_idx++) {
		إذا (old_super[check_idx] == new_super[check_idx])
			استمر;
		write_idx = check_idx;
		لكل (check_idx++; check_idx < SUPERBLOCK_ناپ/2; check_idx++)
			إذا (old_super[check_idx] == new_super[check_idx])
				اكسر;
		ناپ = 2 * (check_idx - write_idx);
#إذا 0
		لکھو("Writing %d bytes starting at %d\n",
		       ناپ, write_idx*2);
#جے_انت
		retval = io_channel_write_byte(fs->io,
			       SUPERBLOCK_وتھ + (2 * write_idx), ناپ,
					       new_super + write_idx);
		إذا (retval == EXT2_ET_UNIMPLEMENTED)
			اذهب fallback;
		إذا (retval)
			أرجع retval;
	}
	حافظہ_نقل(fs->orig_super, super, SUPERBLOCK_ناپ);
	أرجع 0;
}


/*
 * Updates the revision to EXT2_DYNAMIC_REV
 */
فراغ ext2fs_update_dynamic_rev(ext2_filsys fs)
{
	بنية ext2_super_بلاک *sb = fs->super;

	إذا (sb->s_rev_level > EXT2_GOOD_OLD_REV)
		أرجع;

	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_پہلا_ino = EXT2_GOOD_OLD_پہلا_INO;
	sb->s_inode_ناپ = EXT2_GOOD_OLD_INODE_ناپ;
	/* s_uuid is handled by e2fsck already */
	/* other fields should be left alone */
}

ساكن errcode_t write_backup_super(ext2_filsys fs, dgrp_t گروہ,
				    blk64_t گروہ_بلاک,
				    بنية ext2_super_بلاک *super_shadow)
{
	errcode_t retval;
	dgrp_t	sgrp = گروہ;

	إذا (sgrp > ((1 << 16) - 1))
		sgrp = (1 << 16) - 1;

	super_shadow->s_بلاک_گروہ_nr = ext2fs_cpu_منزل_le16(sgrp);

	retval = ext2fs_superblock_csum_سیٹ(fs, super_shadow);
	إذا (retval)
		أرجع retval;

	أرجع io_channel_write_blk64(fs->io, گروہ_بلاک, -SUPERBLOCK_ناپ,
				    super_shadow);
}

errcode_t ext2fs_flush(ext2_filsys fs)
{
	أرجع ext2fs_flush2(fs, 0);
}

errcode_t ext2fs_flush2(ext2_filsys fs, صحيح flags)
{
	dgrp_t		i;
	errcode_t	retval;
	بنانشانی لما	fs_state;
	__u32		feature_incompat;
	بنية ext2_super_بلاک *super_shadow = 0;
	بنية opaque_ext2_گروہ_desc *گروہ_shadow = 0;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	بنية ext2_گروہ_desc *gdp;
	dgrp_t		j;
#جے_انت
	حرف	*گروہ_ptr;
	blk64_t	old_desc_بلاکاں;
	بنية ext2fs_numeric_ترقی_struct ترقی;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا ((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0 &&
	    !ext2fs_has_feature_journal_dev(fs->super) &&
	    fs->گروہ_desc == عدم)
		أرجع EXT2_ET_NO_GDESC;

	fs_state = fs->super->s_state;
	feature_incompat = fs->super->s_feature_incompat;

	ext2fs_سیٹ_tstamp(fs->super, s_wtime, ext2fs_P_get_ویلا(fs));
	fs->super->s_بلاک_گروہ_nr = 0;

	/*
	 * If the write_bitmaps() function is present, call it to
	 * flush the bitmaps.  This is done this way so that a simple
	 * program that doesn't mess with the bitmaps doesn't need to
	 * drag in the bitmaps.c code.
	 *
	 * Bitmap checksums live in the group descriptor, so the
	 * bitmaps need to be written before the descriptors.
	 */
	إذا (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		إذا (retval)
			اذهب errout;
	}

	/*
	 * Set the state of the FS to be non-valid.  (The state has
	 * already been backed up earlier, and will be restored after
	 * we write out the backup superblocks.)
	 */
	fs->super->s_state &= ~EXT2_VALID_FS;
	ext2fs_clear_feature_journal_needs_recovery(fs->super);

	/* Byte swap the superblock and the group descriptors if necessary */
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	retval = EXT2_ET_NO_حافظہ;
	retval = ext2fs_get_mem(SUPERBLOCK_ناپ, &super_shadow);
	إذا (retval)
		اذهب errout;
	حافظہ_نقل(super_shadow, fs->super, حجم(بنية ext2_super_بلاک));
	ext2fs_وٹاندرا_super(super_shadow);

	إذا (((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0) &&
	    !ext2fs_has_feature_journal_dev(fs->super)) {
		retval = ext2fs_get_array(fs->desc_بلاکاں, fs->blocksize,
					  &گروہ_shadow);
		إذا (retval)
			اذهب errout;
		حافظہ_نقل(گروہ_shadow, fs->گروہ_desc, (size_t) fs->blocksize *
		       fs->desc_بلاکاں);

		لكل (j = 0; j < fs->گروہ_desc_گنتی; j++) {
			gdp = ext2fs_گروہ_desc(fs, گروہ_shadow, j);
			ext2fs_وٹاندرا_گروہ_desc2(fs, gdp);
		}
	}
#وإلا
	super_shadow = fs->super;
	گروہ_shadow = fs->گروہ_desc;
#جے_انت

	/*
	 * If this is an external journal device, don't write out the
	 * block group descriptors or any of the backup superblocks
	 */
	إذا (ext2fs_has_feature_journal_dev(fs->super))
		اذهب write_primary_superblock_only;

	/*
	 * Write out the master group descriptors, and the backup
	 * superblocks and group descriptors.
	 */
	گروہ_ptr = (حرف *) گروہ_shadow;
	إذا (ext2fs_has_feature_meta_bg(fs->super)) {
		old_desc_بلاکاں = fs->super->s_پہلا_meta_bg;
		إذا (old_desc_بلاکاں > fs->desc_بلاکاں)
			old_desc_بلاکاں = fs->desc_بلاکاں;
	} وإلا
		old_desc_بلاکاں = fs->desc_بلاکاں;

	إذا (fs->ترقی_ops && fs->ترقی_ops->init)
		(fs->ترقی_ops->init)(fs, &ترقی, عدم,
					 fs->گروہ_desc_گنتی);


	لكل (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		blk64_t	super_blk, old_desc_blk, new_desc_blk;

		إذا (fs->ترقی_ops && fs->ترقی_ops->update)
			(fs->ترقی_ops->update)(fs, &ترقی, i);
		ext2fs_super_and_bgd_loc2(fs, i, &super_blk, &old_desc_blk,
					 &new_desc_blk, 0);

		إذا (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) &&i && super_blk) {
			retval = write_backup_super(fs, i, super_blk,
						    super_shadow);
			إذا (retval)
				اذهب errout;
		}
		إذا (fs->flags & EXT2_FLAG_SUPER_ONLY)
			استمر;
		إذا ((old_desc_blk) &&
		    (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) || (i == 0))) {
			retval = io_channel_write_blk64(fs->io,
			      old_desc_blk, old_desc_بلاکاں, گروہ_ptr);
			إذا (retval)
				اذهب errout;
		}
		إذا (new_desc_blk) {
			صحيح meta_bg = i / EXT2_DESC_فی_بلاک(fs->super);

			retval = io_channel_write_blk64(fs->io, new_desc_blk,
				1, گروہ_ptr + (meta_bg*fs->blocksize));
			إذا (retval)
				اذهب errout;
		}
	}

	إذا (fs->ترقی_ops && fs->ترقی_ops->close)
		(fs->ترقی_ops->close)(fs, &ترقی, عدم);

write_primary_superblock_only:
	/*
	 * Write out master superblock.  This has to be done
	 * separately, since it is located at a fixed location
	 * (SUPERBLOCK_OFFSET).  We flush all other pending changes
	 * out to disk first, just to avoid a race condition with an
	 * insy-tinsy window....
	 */

	fs->super->s_بلاک_گروہ_nr = 0;
	fs->super->s_state = fs_state;
	fs->super->s_feature_incompat = feature_incompat;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	*super_shadow = *fs->super;
	ext2fs_وٹاندرا_super(super_shadow);
#جے_انت

	retval = ext2fs_superblock_csum_سیٹ(fs, super_shadow);
	إذا (retval)
		أرجع retval;

	إذا (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		إذا (retval)
			اذهب errout;
	}
	retval = write_primary_superblock(fs, super_shadow);
	إذا (retval)
		اذهب errout;

	fs->flags &= ~EXT2_FLAG_DIRTY;

	إذا (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		إذا (retval)
			اذهب errout;
	}
errout:
	fs->super->s_state = fs_state;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	إذا (super_shadow)
		ext2fs_free_mem(&super_shadow);
	إذا (گروہ_shadow)
		ext2fs_free_mem(&گروہ_shadow);
#جے_انت
	أرجع retval;
}

errcode_t ext2fs_close_free(ext2_filsys *fs_ptr)
{
	errcode_t ret;
	ext2_filsys fs = *fs_ptr;

	ret = ext2fs_close2(fs, 0);
	إذا (ret)
		ext2fs_free(fs);
	*fs_ptr = عدم;
	أرجع ret;
}

errcode_t ext2fs_close(ext2_filsys fs)
{
	أرجع ext2fs_close2(fs, 0);
}

errcode_t ext2fs_close2(ext2_filsys fs, صحيح flags)
{
	errcode_t	retval;
	صحيح		meta_blks;
	io_stats stats = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		إذا (retval)
			أرجع retval;
	}
	إذا (fs->super->s_kbytes_written &&
	    fs->io->manager->get_stats)
		fs->io->manager->get_stats(fs->io, &stats);
	إذا (stats && stats->بائٹاں_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->بائٹاں_written >> 10;
		meta_blks = fs->desc_بلاکاں + 1;
		إذا (!(fs->flags & EXT2_FLAG_SUPER_ONLY))
			fs->super->s_kbytes_written += meta_blks /
				(fs->blocksize / 1024);
		إذا ((fs->flags & EXT2_FLAG_DIRTY) == 0)
			fs->flags |= EXT2_FLAG_SUPER_ONLY | EXT2_FLAG_DIRTY;
	}
	إذا (fs->flags & EXT2_FLAG_DIRTY) {
		retval = ext2fs_flush2(fs, flags);
		إذا (retval)
			أرجع retval;
	}

	retval = ext2fs_mmp_stop(fs);
	إذا (retval)
		أرجع retval;

	ext2fs_free(fs);
	أرجع 0;
}

