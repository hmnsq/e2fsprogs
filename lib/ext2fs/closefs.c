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
#جيڪڏهن_پورو
#ضمن <time.h>
#ضمن <string.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

ساكن صحيح test_پاڙ(بنانشان صحيح a, بنانشان صحيح b)
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

صحيح ext2fs_bg_has_super(ext2_filsys fs, dgrp_t گروهه)
{
	إذا (گروهه == 0)
		أرجع 1;
	إذا (ext2fs_has_feature_sparse_super2(fs->super)) {
		إذا (گروهه == fs->super->s_backup_bgs[0] ||
		    گروهه == fs->super->s_backup_bgs[1])
			أرجع 1;
		أرجع 0;
	}
	إذا ((گروهه <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		أرجع 1;
	إذا (!(گروهه & 1))
		أرجع 0;
	إذا (test_پاڙ(گروهه, 3) || (test_پاڙ(گروهه, 5)) ||
	    test_پاڙ(گروهه, 7))
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
					   dgrp_t گروهه,
					   blk64_t *ret_super_blk,
					   blk64_t *ret_old_desc_blk,
					   blk64_t *ret_new_desc_blk,
					   blk_t *ret_استعمال_ٿيل_blks)
{
	blk64_t	گروهه_بلاڪ, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	بنانشان صحيح meta_bg, meta_bg_سائيز;
	blk_t	numblocks = 0;
	blk64_t old_desc_بلاڪون;
	صحيح	has_super;

	گروهه_بلاڪ = ext2fs_گروهه_پهريون_block2(fs, گروهه);
	إذا (گروهه_بلاڪ == 0 && fs->blocksize == 1024)
		گروهه_بلاڪ = 1; /* Deal with 1024 blocksize && bigalloc */

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_بلاڪون = fs->super->s_پهريون_meta_bg;
	وإلا
		old_desc_بلاڪون =
			fs->desc_بلاڪون + fs->super->s_reserved_gdt_بلاڪون;

	has_super = ext2fs_bg_has_super(fs, گروهه);

	إذا (has_super) {
		super_blk = گروهه_بلاڪ;
		numblocks++;
	}
	meta_bg_سائيز = EXT2_DESC_في_بلاڪ(fs->super);
	meta_bg = گروهه / meta_bg_سائيز;

	إذا (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_پهريون_meta_bg)) {
		إذا (has_super) {
			old_desc_blk = گروهه_بلاڪ + 1;
			numblocks += old_desc_بلاڪون;
		}
	} وإلا {
		إذا (((گروهه % meta_bg_سائيز) == 0) ||
		    ((گروهه % meta_bg_سائيز) == 1) ||
		    ((گروهه % meta_bg_سائيز) == (meta_bg_سائيز-1))) {
			إذا (has_super)
				has_super = 1;
			new_desc_blk = گروهه_بلاڪ + has_super;
			numblocks++;
		}
	}

	إذا (ret_super_blk)
		*ret_super_blk = super_blk;
	إذا (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	إذا (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	إذا (ret_استعمال_ٿيل_blks)
		*ret_استعمال_ٿيل_blks = numblocks;

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
			     dgrp_t گروهه,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     صحيح *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_استعمال_ٿيل_blks;
	blk_t numblocks;
	بنانشان صحيح meta_bg_سائيز;

	ext2fs_super_and_bgd_loc2(fs, گروهه, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_استعمال_ٿيل_blks);

	numblocks = ext2fs_گروهه_بلاڪون_ڳڻپ(fs, گروهه);

	إذا (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	إذا (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	إذا (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	إذا (ret_meta_bg) {
		meta_bg_سائيز = EXT2_DESC_في_بلاڪ(fs->super);
		*ret_meta_bg = گروهه / meta_bg_سائيز;
	}

	numblocks -= 2 + fs->inode_بلاڪون_في_گروهه + ret_استعمال_ٿيل_blks;

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
					  بنية ext2_super_بلاڪ *super)
{
	__u16		*old_super, *new_super;
	صحيح		check_idx, write_idx, سائيز;
	errcode_t	retval;

	إذا (!fs->io->manager->write_byte || !fs->orig_super) {
	fallback:
		io_channel_سيٽ_blksize(fs->io, SUPERBLOCK_وٿي);
		retval = io_channel_write_blk64(fs->io, 1, -SUPERBLOCK_سائيز,
					      super);
		io_channel_سيٽ_blksize(fs->io, fs->blocksize);
		أرجع retval;
	}

	old_super = (__u16 *) fs->orig_super;
	new_super = (__u16 *) super;

	لكل (check_idx = 0; check_idx < SUPERBLOCK_سائيز/2; check_idx++) {
		إذا (old_super[check_idx] == new_super[check_idx])
			استمر;
		write_idx = check_idx;
		لكل (check_idx++; check_idx < SUPERBLOCK_سائيز/2; check_idx++)
			إذا (old_super[check_idx] == new_super[check_idx])
				اكسر;
		سائيز = 2 * (check_idx - write_idx);
#إذا 0
		لکو("Writing %d bytes starting at %d\n",
		       سائيز, write_idx*2);
#جيڪڏهن_پورو
		retval = io_channel_write_byte(fs->io,
			       SUPERBLOCK_وٿي + (2 * write_idx), سائيز,
					       new_super + write_idx);
		إذا (retval == EXT2_ET_UNIMPLEMENTED)
			اذهب fallback;
		إذا (retval)
			أرجع retval;
	}
	يادگيري_نقل(fs->orig_super, super, SUPERBLOCK_سائيز);
	أرجع 0;
}


/*
 * Updates the revision to EXT2_DYNAMIC_REV
 */
فراغ ext2fs_update_dynamic_rev(ext2_filsys fs)
{
	بنية ext2_super_بلاڪ *sb = fs->super;

	إذا (sb->s_rev_level > EXT2_GOOD_OLD_REV)
		أرجع;

	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_پهريون_ino = EXT2_GOOD_OLD_پهريون_INO;
	sb->s_inode_سائيز = EXT2_GOOD_OLD_INODE_سائيز;
	/* s_uuid is handled by e2fsck already */
	/* other fields should be left alone */
}

ساكن errcode_t write_backup_super(ext2_filsys fs, dgrp_t گروهه,
				    blk64_t گروهه_بلاڪ,
				    بنية ext2_super_بلاڪ *super_shadow)
{
	errcode_t retval;
	dgrp_t	sgrp = گروهه;

	إذا (sgrp > ((1 << 16) - 1))
		sgrp = (1 << 16) - 1;

	super_shadow->s_بلاڪ_گروهه_nr = ext2fs_cpu_منزل_le16(sgrp);

	retval = ext2fs_superblock_csum_سيٽ(fs, super_shadow);
	إذا (retval)
		أرجع retval;

	أرجع io_channel_write_blk64(fs->io, گروهه_بلاڪ, -SUPERBLOCK_سائيز,
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
	بنانشان ڊگهو	fs_state;
	__u32		feature_incompat;
	بنية ext2_super_بلاڪ *super_shadow = 0;
	بنية opaque_ext2_گروهه_desc *گروهه_shadow = 0;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	بنية ext2_گروهه_desc *gdp;
	dgrp_t		j;
#جيڪڏهن_پورو
	حرف	*گروهه_ptr;
	blk64_t	old_desc_بلاڪون;
	بنية ext2fs_numeric_ترقي_struct ترقي;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا ((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0 &&
	    !ext2fs_has_feature_journal_dev(fs->super) &&
	    fs->گروهه_desc == عدم)
		أرجع EXT2_ET_NO_GDESC;

	fs_state = fs->super->s_state;
	feature_incompat = fs->super->s_feature_incompat;

	ext2fs_سيٽ_tstamp(fs->super, s_wtime, ext2fs_P_get_وقت(fs));
	fs->super->s_بلاڪ_گروهه_nr = 0;

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
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	retval = EXT2_ET_NO_يادگيري;
	retval = ext2fs_get_mem(SUPERBLOCK_سائيز, &super_shadow);
	إذا (retval)
		اذهب errout;
	يادگيري_نقل(super_shadow, fs->super, حجم(بنية ext2_super_بلاڪ));
	ext2fs_مٽا_سٽا_super(super_shadow);

	إذا (((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0) &&
	    !ext2fs_has_feature_journal_dev(fs->super)) {
		retval = ext2fs_get_array(fs->desc_بلاڪون, fs->blocksize,
					  &گروهه_shadow);
		إذا (retval)
			اذهب errout;
		يادگيري_نقل(گروهه_shadow, fs->گروهه_desc, (size_t) fs->blocksize *
		       fs->desc_بلاڪون);

		لكل (j = 0; j < fs->گروهه_desc_ڳڻپ; j++) {
			gdp = ext2fs_گروهه_desc(fs, گروهه_shadow, j);
			ext2fs_مٽا_سٽا_گروهه_desc2(fs, gdp);
		}
	}
#وإلا
	super_shadow = fs->super;
	گروهه_shadow = fs->گروهه_desc;
#جيڪڏهن_پورو

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
	گروهه_ptr = (حرف *) گروهه_shadow;
	إذا (ext2fs_has_feature_meta_bg(fs->super)) {
		old_desc_بلاڪون = fs->super->s_پهريون_meta_bg;
		إذا (old_desc_بلاڪون > fs->desc_بلاڪون)
			old_desc_بلاڪون = fs->desc_بلاڪون;
	} وإلا
		old_desc_بلاڪون = fs->desc_بلاڪون;

	إذا (fs->ترقي_ops && fs->ترقي_ops->init)
		(fs->ترقي_ops->init)(fs, &ترقي, عدم,
					 fs->گروهه_desc_ڳڻپ);


	لكل (i = 0; i < fs->گروهه_desc_ڳڻپ; i++) {
		blk64_t	super_blk, old_desc_blk, new_desc_blk;

		إذا (fs->ترقي_ops && fs->ترقي_ops->update)
			(fs->ترقي_ops->update)(fs, &ترقي, i);
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
			      old_desc_blk, old_desc_بلاڪون, گروهه_ptr);
			إذا (retval)
				اذهب errout;
		}
		إذا (new_desc_blk) {
			صحيح meta_bg = i / EXT2_DESC_في_بلاڪ(fs->super);

			retval = io_channel_write_blk64(fs->io, new_desc_blk,
				1, گروهه_ptr + (meta_bg*fs->blocksize));
			إذا (retval)
				اذهب errout;
		}
	}

	إذا (fs->ترقي_ops && fs->ترقي_ops->close)
		(fs->ترقي_ops->close)(fs, &ترقي, عدم);

write_primary_superblock_only:
	/*
	 * Write out master superblock.  This has to be done
	 * separately, since it is located at a fixed location
	 * (SUPERBLOCK_OFFSET).  We flush all other pending changes
	 * out to disk first, just to avoid a race condition with an
	 * insy-tinsy window....
	 */

	fs->super->s_بلاڪ_گروهه_nr = 0;
	fs->super->s_state = fs_state;
	fs->super->s_feature_incompat = feature_incompat;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	*super_shadow = *fs->super;
	ext2fs_مٽا_سٽا_super(super_shadow);
#جيڪڏهن_پورو

	retval = ext2fs_superblock_csum_سيٽ(fs, super_shadow);
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
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	إذا (super_shadow)
		ext2fs_free_mem(&super_shadow);
	إذا (گروهه_shadow)
		ext2fs_free_mem(&گروهه_shadow);
#جيڪڏهن_پورو
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
	إذا (stats && stats->بائيٽون_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->بائيٽون_written >> 10;
		meta_blks = fs->desc_بلاڪون + 1;
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

