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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <time.h>
#شامل <string.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

ساکن صحیح test_جڑ(غیرعلامتی صحیح a, غیرعلامتی صحیح b)
{
	جب (1) {
		اگر (a < b)
			واپس 0;
		اگر (a == b)
			واپس 1;
		اگر (a % b)
			واپس 0;
		a = a / b;
	}
}

صحیح ext2fs_bg_has_super(ext2_filsys fs, dgrp_t گروہ)
{
	اگر (گروہ == 0)
		واپس 1;
	اگر (ext2fs_has_feature_sparse_super2(fs->super)) {
		اگر (گروہ == fs->super->s_backup_bgs[0] ||
		    گروہ == fs->super->s_backup_bgs[1])
			واپس 1;
		واپس 0;
	}
	اگر ((گروہ <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		واپس 1;
	اگر (!(گروہ & 1))
		واپس 0;
	اگر (test_جڑ(گروہ, 3) || (test_جڑ(گروہ, 5)) ||
	    test_جڑ(گروہ, 7))
		واپس 1;

	واپس 0;
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
					   blk_t *ret_مستعمل_blks)
{
	blk64_t	گروہ_بلاک, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	غیرعلامتی صحیح meta_bg, meta_bg_سائز;
	blk_t	numblocks = 0;
	blk64_t old_desc_بلاکیں;
	صحیح	has_super;

	گروہ_بلاک = ext2fs_گروہ_پہلا_block2(fs, گروہ);
	اگر (گروہ_بلاک == 0 && fs->blocksize == 1024)
		گروہ_بلاک = 1; /* Deal with 1024 blocksize && bigalloc */

	اگر (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_بلاکیں = fs->super->s_پہلا_meta_bg;
	ورنہ
		old_desc_بلاکیں =
			fs->desc_بلاکیں + fs->super->s_reserved_gdt_بلاکیں;

	has_super = ext2fs_bg_has_super(fs, گروہ);

	اگر (has_super) {
		super_blk = گروہ_بلاک;
		numblocks++;
	}
	meta_bg_سائز = EXT2_DESC_فی_بلاک(fs->super);
	meta_bg = گروہ / meta_bg_سائز;

	اگر (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_پہلا_meta_bg)) {
		اگر (has_super) {
			old_desc_blk = گروہ_بلاک + 1;
			numblocks += old_desc_بلاکیں;
		}
	} ورنہ {
		اگر (((گروہ % meta_bg_سائز) == 0) ||
		    ((گروہ % meta_bg_سائز) == 1) ||
		    ((گروہ % meta_bg_سائز) == (meta_bg_سائز-1))) {
			اگر (has_super)
				has_super = 1;
			new_desc_blk = گروہ_بلاک + has_super;
			numblocks++;
		}
	}

	اگر (ret_super_blk)
		*ret_super_blk = super_blk;
	اگر (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	اگر (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	اگر (ret_مستعمل_blks)
		*ret_مستعمل_blks = numblocks;

	واپس 0;
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
صحیح ext2fs_super_and_bgd_loc(ext2_filsys fs,
			     dgrp_t گروہ,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     صحیح *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_مستعمل_blks;
	blk_t numblocks;
	غیرعلامتی صحیح meta_bg_سائز;

	ext2fs_super_and_bgd_loc2(fs, گروہ, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_مستعمل_blks);

	numblocks = ext2fs_گروہ_بلاکیں_گنتی(fs, گروہ);

	اگر (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	اگر (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	اگر (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	اگر (ret_meta_bg) {
		meta_bg_سائز = EXT2_DESC_فی_بلاک(fs->super);
		*ret_meta_bg = گروہ / meta_bg_سائز;
	}

	numblocks -= 2 + fs->inode_بلاکیں_فی_گروہ + ret_مستعمل_blks;

	واپس numblocks;
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
ساکن errcode_t write_primary_superblock(ext2_filsys fs,
					  ساخت ext2_super_بلاک *super)
{
	__u16		*old_super, *new_super;
	صحیح		check_idx, write_idx, سائز;
	errcode_t	retval;

	اگر (!fs->io->manager->write_byte || !fs->orig_super) {
	fallback:
		io_channel_سیٹ_blksize(fs->io, SUPERBLOCK_انحراف);
		retval = io_channel_write_blk64(fs->io, 1, -SUPERBLOCK_سائز,
					      super);
		io_channel_سیٹ_blksize(fs->io, fs->blocksize);
		واپس retval;
	}

	old_super = (__u16 *) fs->orig_super;
	new_super = (__u16 *) super;

	برائے (check_idx = 0; check_idx < SUPERBLOCK_سائز/2; check_idx++) {
		اگر (old_super[check_idx] == new_super[check_idx])
			جاری;
		write_idx = check_idx;
		برائے (check_idx++; check_idx < SUPERBLOCK_سائز/2; check_idx++)
			اگر (old_super[check_idx] == new_super[check_idx])
				توڑو;
		سائز = 2 * (check_idx - write_idx);
#اگر 0
		لکھو("Writing %d bytes starting at %d\n",
		       سائز, write_idx*2);
#اگر_ختم
		retval = io_channel_write_byte(fs->io,
			       SUPERBLOCK_انحراف + (2 * write_idx), سائز,
					       new_super + write_idx);
		اگر (retval == EXT2_ET_UNIMPLEMENTED)
			جاؤ fallback;
		اگر (retval)
			واپس retval;
	}
	حافظہ_نقل(fs->orig_super, super, SUPERBLOCK_سائز);
	واپس 0;
}


/*
 * Updates the revision to EXT2_DYNAMIC_REV
 */
خالی ext2fs_update_dynamic_rev(ext2_filsys fs)
{
	ساخت ext2_super_بلاک *sb = fs->super;

	اگر (sb->s_rev_level > EXT2_GOOD_OLD_REV)
		واپس;

	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_پہلا_ino = EXT2_GOOD_OLD_پہلا_INO;
	sb->s_inode_سائز = EXT2_GOOD_OLD_INODE_سائز;
	/* s_uuid is handled by e2fsck already */
	/* other fields should be left alone */
}

ساکن errcode_t write_backup_super(ext2_filsys fs, dgrp_t گروہ,
				    blk64_t گروہ_بلاک,
				    ساخت ext2_super_بلاک *super_shadow)
{
	errcode_t retval;
	dgrp_t	sgrp = گروہ;

	اگر (sgrp > ((1 << 16) - 1))
		sgrp = (1 << 16) - 1;

	super_shadow->s_بلاک_گروہ_nr = ext2fs_cpu_مقصد_le16(sgrp);

	retval = ext2fs_superblock_csum_سیٹ(fs, super_shadow);
	اگر (retval)
		واپس retval;

	واپس io_channel_write_blk64(fs->io, گروہ_بلاک, -SUPERBLOCK_سائز,
				    super_shadow);
}

errcode_t ext2fs_flush(ext2_filsys fs)
{
	واپس ext2fs_flush2(fs, 0);
}

errcode_t ext2fs_flush2(ext2_filsys fs, صحیح flags)
{
	dgrp_t		i;
	errcode_t	retval;
	غیرعلامتی طویل	fs_state;
	__u32		feature_incompat;
	ساخت ext2_super_بلاک *super_shadow = 0;
	ساخت opaque_ext2_گروہ_desc *گروہ_shadow = 0;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ساخت ext2_گروہ_desc *gdp;
	dgrp_t		j;
#اگر_ختم
	حرف	*گروہ_ptr;
	blk64_t	old_desc_بلاکیں;
	ساخت ext2fs_numeric_پیش_رفت_struct پیش_رفت;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر ((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0 &&
	    !ext2fs_has_feature_journal_dev(fs->super) &&
	    fs->گروہ_desc == تہی)
		واپس EXT2_ET_NO_GDESC;

	fs_state = fs->super->s_state;
	feature_incompat = fs->super->s_feature_incompat;

	ext2fs_سیٹ_tstamp(fs->super, s_wtime, ext2fs_P_get_وقت(fs));
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
	اگر (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		اگر (retval)
			جاؤ errout;
	}

	/*
	 * Set the state of the FS to be non-valid.  (The state has
	 * already been backed up earlier, and will be restored after
	 * we write out the backup superblocks.)
	 */
	fs->super->s_state &= ~EXT2_VALID_FS;
	ext2fs_clear_feature_journal_needs_recovery(fs->super);

	/* Byte swap the superblock and the group descriptors if necessary */
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	retval = EXT2_ET_NO_حافظہ;
	retval = ext2fs_get_mem(SUPERBLOCK_سائز, &super_shadow);
	اگر (retval)
		جاؤ errout;
	حافظہ_نقل(super_shadow, fs->super, حجم(ساخت ext2_super_بلاک));
	ext2fs_تبادلہ_super(super_shadow);

	اگر (((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0) &&
	    !ext2fs_has_feature_journal_dev(fs->super)) {
		retval = ext2fs_get_array(fs->desc_بلاکیں, fs->blocksize,
					  &گروہ_shadow);
		اگر (retval)
			جاؤ errout;
		حافظہ_نقل(گروہ_shadow, fs->گروہ_desc, (size_t) fs->blocksize *
		       fs->desc_بلاکیں);

		برائے (j = 0; j < fs->گروہ_desc_گنتی; j++) {
			gdp = ext2fs_گروہ_desc(fs, گروہ_shadow, j);
			ext2fs_تبادلہ_گروہ_desc2(fs, gdp);
		}
	}
#ورنہ
	super_shadow = fs->super;
	گروہ_shadow = fs->گروہ_desc;
#اگر_ختم

	/*
	 * If this is an external journal device, don't write out the
	 * block group descriptors or any of the backup superblocks
	 */
	اگر (ext2fs_has_feature_journal_dev(fs->super))
		جاؤ write_primary_superblock_only;

	/*
	 * Write out the master group descriptors, and the backup
	 * superblocks and group descriptors.
	 */
	گروہ_ptr = (حرف *) گروہ_shadow;
	اگر (ext2fs_has_feature_meta_bg(fs->super)) {
		old_desc_بلاکیں = fs->super->s_پہلا_meta_bg;
		اگر (old_desc_بلاکیں > fs->desc_بلاکیں)
			old_desc_بلاکیں = fs->desc_بلاکیں;
	} ورنہ
		old_desc_بلاکیں = fs->desc_بلاکیں;

	اگر (fs->پیش_رفت_ops && fs->پیش_رفت_ops->init)
		(fs->پیش_رفت_ops->init)(fs, &پیش_رفت, تہی,
					 fs->گروہ_desc_گنتی);


	برائے (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		blk64_t	super_blk, old_desc_blk, new_desc_blk;

		اگر (fs->پیش_رفت_ops && fs->پیش_رفت_ops->update)
			(fs->پیش_رفت_ops->update)(fs, &پیش_رفت, i);
		ext2fs_super_and_bgd_loc2(fs, i, &super_blk, &old_desc_blk,
					 &new_desc_blk, 0);

		اگر (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) &&i && super_blk) {
			retval = write_backup_super(fs, i, super_blk,
						    super_shadow);
			اگر (retval)
				جاؤ errout;
		}
		اگر (fs->flags & EXT2_FLAG_SUPER_ONLY)
			جاری;
		اگر ((old_desc_blk) &&
		    (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) || (i == 0))) {
			retval = io_channel_write_blk64(fs->io,
			      old_desc_blk, old_desc_بلاکیں, گروہ_ptr);
			اگر (retval)
				جاؤ errout;
		}
		اگر (new_desc_blk) {
			صحیح meta_bg = i / EXT2_DESC_فی_بلاک(fs->super);

			retval = io_channel_write_blk64(fs->io, new_desc_blk,
				1, گروہ_ptr + (meta_bg*fs->blocksize));
			اگر (retval)
				جاؤ errout;
		}
	}

	اگر (fs->پیش_رفت_ops && fs->پیش_رفت_ops->close)
		(fs->پیش_رفت_ops->close)(fs, &پیش_رفت, تہی);

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
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	*super_shadow = *fs->super;
	ext2fs_تبادلہ_super(super_shadow);
#اگر_ختم

	retval = ext2fs_superblock_csum_سیٹ(fs, super_shadow);
	اگر (retval)
		واپس retval;

	اگر (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		اگر (retval)
			جاؤ errout;
	}
	retval = write_primary_superblock(fs, super_shadow);
	اگر (retval)
		جاؤ errout;

	fs->flags &= ~EXT2_FLAG_DIRTY;

	اگر (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		اگر (retval)
			جاؤ errout;
	}
errout:
	fs->super->s_state = fs_state;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	اگر (super_shadow)
		ext2fs_free_mem(&super_shadow);
	اگر (گروہ_shadow)
		ext2fs_free_mem(&گروہ_shadow);
#اگر_ختم
	واپس retval;
}

errcode_t ext2fs_close_free(ext2_filsys *fs_ptr)
{
	errcode_t ret;
	ext2_filsys fs = *fs_ptr;

	ret = ext2fs_close2(fs, 0);
	اگر (ret)
		ext2fs_free(fs);
	*fs_ptr = تہی;
	واپس ret;
}

errcode_t ext2fs_close(ext2_filsys fs)
{
	واپس ext2fs_close2(fs, 0);
}

errcode_t ext2fs_close2(ext2_filsys fs, صحیح flags)
{
	errcode_t	retval;
	صحیح		meta_blks;
	io_stats stats = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		اگر (retval)
			واپس retval;
	}
	اگر (fs->super->s_kbytes_written &&
	    fs->io->manager->get_stats)
		fs->io->manager->get_stats(fs->io, &stats);
	اگر (stats && stats->بائٹیں_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->بائٹیں_written >> 10;
		meta_blks = fs->desc_بلاکیں + 1;
		اگر (!(fs->flags & EXT2_FLAG_SUPER_ONLY))
			fs->super->s_kbytes_written += meta_blks /
				(fs->blocksize / 1024);
		اگر ((fs->flags & EXT2_FLAG_DIRTY) == 0)
			fs->flags |= EXT2_FLAG_SUPER_ONLY | EXT2_FLAG_DIRTY;
	}
	اگر (fs->flags & EXT2_FLAG_DIRTY) {
		retval = ext2fs_flush2(fs, flags);
		اگر (retval)
			واپس retval;
	}

	retval = ext2fs_mmp_stop(fs);
	اگر (retval)
		واپس retval;

	ext2fs_free(fs);
	واپس 0;
}

