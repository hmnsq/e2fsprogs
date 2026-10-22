/*
 * openfs.c --- open an ext2 filesystem
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
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم

#شامل "ext2_fs.h"

#شامل "ext2fs.h"
#شامل "e2image.h"

blk64_t ext2fs_descriptor_بلاک_loc2(ext2_filsys fs, blk64_t گروہ_بلاک,
				     dgrp_t i)
{
	صحیح	bg;
	صحیح	has_super = 0, گروہ_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	اگر (i == 0 && fs->blocksize == 1024 && EXT2_FS_CLUSTER_تناسب(fs) > 1)
		گروہ_zero_adjust = 1;

	اگر (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_پہلا_meta_bg))
		واپس گروہ_بلاک + i + 1 + گروہ_zero_adjust;

	bg = EXT2_DESC_فی_بلاک(fs->super) * i;
	اگر (ext2fs_bg_has_super(fs, bg))
		has_super = 1;
	ret_blk = ext2fs_گروہ_پہلا_block2(fs, bg);
	/*
	 * If group_block is not the normal value, we're trying to use
	 * the backup group descriptors and superblock --- so use the
	 * alternate location of the second block group in the
	 * metablock group.  Ideally we should be testing each bg
	 * descriptor block individually for correctness, but we don't
	 * have the infrastructure in place to do that.
	 */
	اگر (گروہ_بلاک != fs->super->s_پہلا_ڈیٹا_بلاک &&
	    ((ret_blk + has_super + fs->super->s_بلاکیں_فی_گروہ) <
	     ext2fs_بلاکیں_گنتی(fs->super))) {
		ret_blk += fs->super->s_بلاکیں_فی_گروہ;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		اگر (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		ورنہ
			has_super = 0;
	}
	واپس ret_blk + has_super + گروہ_zero_adjust;
}

blk_t ext2fs_descriptor_بلاک_loc(ext2_filsys fs, blk_t گروہ_بلاک, dgrp_t i)
{
	واپس ext2fs_descriptor_بلاک_loc2(fs, گروہ_بلاک, i);
}

errcode_t ext2fs_open(ثابت حرف *name, صحیح flags, صحیح superblock,
		      غیرعلامتی صحیح بلاک_سائز, io_manager manager,
		      ext2_filsys *ret_fs)
{
	واپس ext2fs_open2(name, 0, flags, superblock, بلاک_سائز,
			    manager, ret_fs);
}

ساکن خالی بلاک_sha_map_free_اندراج(خالی *ڈیٹا)
{
	آزاد(ڈیٹا);
	واپس;
}

/*
 *  Note: if superblock is non-zero, block-size must also be non-zero.
 * 	Superblock and block_size can be zero to use the default size.
 *
 * Valid flags for ext2fs_open()
 *
 * 	EXT2_FLAG_RW	- Open the filesystem for read/write.
 * 	EXT2_FLAG_FORCE - Open the filesystem even if some of the
 *				features aren't supported.
 *	EXT2_FLAG_JOURNAL_DEV_OK - Open an ext3 journal device
 *	EXT2_FLAG_SKIP_MMP - Open without multi-mount protection check.
 *	EXT2_FLAG_64BITS - Allow 64-bit bitfields (needed for large
 *				filesystems)
 */
errcode_t ext2fs_open2(ثابت حرف *name, ثابت حرف *io_options,
		       صحیح flags, صحیح superblock,
		       غیرعلامتی صحیح بلاک_سائز, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	غیرعلامتی طویل	i, پہلا_meta_bg;
	__u32		features;
	غیرعلامتی صحیح	بلاکیں_فی_گروہ, io_flags;
	blk64_t		گروہ_بلاک, blk;
	حرف		*dest, *cp;
	صحیح		گروہ_zero_adjust = 0;
	غیرعلامتی صحیح	inode_سائز;
	__u64		groups_cnt;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	غیرعلامتی صحیح	groups_فی_بلاک;
	ساخت ext2_گروہ_desc *gdp;
	صحیح		j;
#اگر_ختم
	حرف		*وقت_env;
	صحیح		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(حجم(ساخت struct_ext2_filsys), &fs);
	اگر (retval)
		واپس retval;

	memset(fs, 0, حجم(ساخت struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	وقت_env = ext2fs_محفوظ_getenv("SOURCE_DATE_EPOCH");
	اگر (وقت_env) {
		fs->now = strtoul(وقت_env, تہی, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_وقت;
	} ورنہ {
		وقت_env = ext2fs_محفوظ_getenv("E2FSPROGS_FAKE_TIME");
		اگر (وقت_env)
			fs->now = strtoul(وقت_env, تہی, 0);
	}

	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &fs->device_name);
	اگر (retval)
		جاؤ cleanup;
	strcpy(fs->device_name, name);
	cp = strchr(fs->device_name, '?');
	اگر (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	اگر (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	اگر (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	اگر (flags & EXT2_FLAG_براہ_راست_IO)
		io_flags |= IO_FLAG_براہ_راست_IO;
	اگر (flags & EXT2_FLAG_دھاگے)
		io_flags |= IO_FLAG_دھاگے;
	retval = manager->open(fs->device_name, io_flags, &fs->io);
	اگر (retval)
		جاؤ cleanup;
	اگر (io_options &&
	    (retval = io_channel_سیٹ_options(fs->io, io_options)))
		جاؤ cleanup;
	fs->image_io = fs->io;
	fs->io->app_ڈیٹا = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_سائز, &fs->super);
	اگر (retval)
		جاؤ cleanup;
	اگر (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(حجم(ساخت ext2_image_hdr),
					&fs->image_سرخی);
		اگر (retval)
			جاؤ cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(صحیح)حجم(ساخت ext2_image_hdr),
					     fs->image_سرخی);
		اگر (retval)
			جاؤ cleanup;
		اگر (ext2fs_le32_مقصد_cpu(fs->image_سرخی->magic_نمبر) != EXT2_ET_MAGIC_E2IMAGE)
			واپس EXT2_ET_MAGIC_E2IMAGE;
		superblock = 1;
		بلاک_سائز = ext2fs_le32_مقصد_cpu(fs->image_سرخی->fs_blocksize);
	}

	/*
	 * If the user specifies a specific block # for the
	 * superblock, then he/she must also specify the block size!
	 * Otherwise, read the master superblock located at offset
	 * SUPERBLOCK_OFFSET from the start of the partition.
	 *
	 * Note: we only save a backup copy of the superblock if we
	 * are reading the superblock from the primary superblock location.
	 */
	اگر (superblock) {
		اگر (!بلاک_سائز) {
			retval = EXT2_ET_INVALID_دلیل;
			جاؤ cleanup;
		}
		io_channel_سیٹ_blksize(fs->io, بلاک_سائز);
		گروہ_بلاک = superblock;
		fs->orig_super = 0;
	} ورنہ {
		io_channel_سیٹ_blksize(fs->io, SUPERBLOCK_انحراف);
		superblock = 1;
		گروہ_بلاک = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_سائز, &fs->orig_super);
		اگر (retval)
			جاؤ cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_سائز,
				     fs->super);
	اگر (retval)
		جاؤ cleanup;
	اگر (fs->orig_super)
		حافظہ_نقل(fs->orig_super, fs->super, SUPERBLOCK_سائز);

	اگر (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		اگر (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		اگر (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			اگر (csum_retries++ < 3)
				جاؤ retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	fs->flags |= EXT2_FLAG_تبادلہ_بائٹیں;
	ext2fs_تبادلہ_super(fs->super);
#ورنہ
	اگر (fs->flags & EXT2_FLAG_تبادلہ_بائٹیں) {
		retval = EXT2_ET_UNIMPLEMENTED;
		جاؤ cleanup;
	}
#اگر_ختم

	اگر (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	اگر (retval)
		جاؤ cleanup;

	اگر (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_بلند;
		جاؤ cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	اگر (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#اگر_تعریف_شدہ EXT2_LIB_SOFTSUPP_INCOMPAT
		اگر (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#اگر_ختم
		اگر (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			جاؤ cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#اگر_تعریف_شدہ EXT2_LIB_SOFTSUPP_RO_COMPAT
		اگر (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#اگر_ختم
		اگر ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			جاؤ cleanup;
		}

		اگر (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			جاؤ cleanup;
		}
	}

	اگر ((fs->super->s_log_بلاک_سائز >
	     (غیرعلامتی) (EXT2_MAX_بلاک_LOG_سائز - EXT2_MIN_بلاک_LOG_سائز)) ||
	    (fs->super->s_log_cluster_سائز >
	     (غیرعلامتی) (EXT2_MAX_CLUSTER_LOG_سائز - EXT2_MIN_CLUSTER_LOG_سائز)) ||
	    (fs->super->s_log_بلاک_سائز > fs->super->s_log_cluster_سائز) ||
	    (fs->super->s_log_groups_فی_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		جاؤ cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	اگر (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64_بٹ)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		جاؤ cleanup;
	}

	اگر (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_بلاک_سائز != fs->super->s_log_cluster_سائز)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		جاؤ cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_بلاک_سائز(fs->super);
	inode_سائز = EXT2_INODE_سائز(fs->super);
	اگر ((inode_سائز < EXT2_GOOD_OLD_INODE_سائز) ||
	    (inode_سائز > fs->blocksize) ||
	    (inode_سائز & (inode_سائز - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		جاؤ cleanup;
	}

	/* Enforce the block group descriptor size */
	اگر (ext2fs_has_feature_64bit(fs->super)) {
		غیرعلامتی desc_سائز = fs->super->s_desc_سائز;

		اگر (desc_سائز == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_سائز > EXT2_MAX_DESC_سائز) ||
		      (desc_سائز < EXT2_MIN_DESC_سائز_64_ثنائی_ہندسہ) ||
		      (desc_سائز & (desc_سائز - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_سائز;
			جاؤ cleanup;
		}
	}

	fs->cluster_تناسب_بٹ = fs->super->s_log_cluster_سائز -
		fs->super->s_log_بلاک_سائز;
	اگر (EXT2_بلاکیں_فی_گروہ(fs->super) !=
	    EXT2_CLUSTERS_فی_گروہ(fs->super) << fs->cluster_تناسب_بٹ) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		جاؤ cleanup;
	}
	fs->inode_بلاکیں_فی_گروہ = ((EXT2_INODES_فی_گروہ(fs->super) *
				       EXT2_INODE_سائز(fs->super) +
				       EXT2_بلاک_سائز(fs->super) - 1) /
				      EXT2_بلاک_سائز(fs->super));
	اگر (بلاک_سائز) {
		اگر (بلاک_سائز != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_بلاک_سائز;
			جاؤ cleanup;
		}
	}
	/*
	 * Set the blocksize to the filesystem's blocksize.
	 */
	io_channel_سیٹ_blksize(fs->io, fs->blocksize);

	/*
	 * If this is an external journal device, don't try to read
	 * the group descriptors, because they're not there.
	 */
	اگر (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->گروہ_desc_گنتی = 0;
		*ret_fs = fs;
		واپس 0;
	}

	اگر (EXT2_INODES_فی_گروہ(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		جاؤ cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_بیج(fs);

	/*
	 * Read group descriptors
	 */
	بلاکیں_فی_گروہ = EXT2_بلاکیں_فی_گروہ(fs->super);
	اگر (بلاکیں_فی_گروہ < 8 ||
	    بلاکیں_فی_گروہ > EXT2_MAX_بلاکیں_فی_گروہ(fs->super) ||
	    fs->inode_بلاکیں_فی_گروہ > EXT2_MAX_INODES_فی_گروہ(fs->super) ||
           EXT2_DESC_فی_بلاک(fs->super) == 0 ||
           fs->super->s_پہلا_ڈیٹا_بلاک >= ext2fs_بلاکیں_گنتی(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		جاؤ cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_بلاکیں_گنتی(fs->super) -
				       fs->super->s_پہلا_ڈیٹا_بلاک,
				       بلاکیں_فی_گروہ);
	اگر (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		جاؤ cleanup;
	}
	fs->گروہ_desc_گنتی = 	groups_cnt;
	اگر (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->گروہ_desc_گنتی * EXT2_INODES_فی_گروہ(fs->super) !=
	    fs->super->s_inodes_گنتی) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		جاؤ cleanup;
	}
	fs->desc_بلاکیں = ext2fs_div_ceil(fs->گروہ_desc_گنتی,
					  EXT2_DESC_فی_بلاک(fs->super));
	اگر (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_پہلا_meta_bg > fs->desc_بلاکیں) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		جاؤ cleanup;
	}
	اگر (flags & EXT2_FLAG_SUPER_ONLY)
		جاؤ skip_read_bg;
	retval = ext2fs_get_array(fs->desc_بلاکیں, fs->blocksize,
				&fs->گروہ_desc);
	اگر (retval)
		جاؤ cleanup;
	اگر (!گروہ_بلاک)
		گروہ_بلاک = fs->super->s_پہلا_ڈیٹا_بلاک;
	/*
	 * On a FS with a 1K blocksize, block 0 is reserved for bootloaders
	 * so we must increment block numbers to any group 0 items.
	 *
	 * However, we cannot touch group_block directly because in the meta_bg
	 * case, the ext2fs_descriptor_block_loc2() function will interpret
	 * group_block != s_first_data_block to mean that we want to access the
	 * backup group descriptors.  This is not what we want if the caller
	 * set superblock == 0 (i.e. auto-detect the superblock), which is
	 * what's going on here.
	 */
	اگر (گروہ_بلاک == 0 && fs->blocksize == 1024)
		گروہ_zero_adjust = 1;
	dest = (حرف *) fs->گروہ_desc;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	groups_فی_بلاک = EXT2_DESC_فی_بلاک(fs->super);
#اگر_ختم
	اگر (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		پہلا_meta_bg = fs->super->s_پہلا_meta_bg;
		اگر (پہلا_meta_bg > fs->desc_بلاکیں)
			پہلا_meta_bg = fs->desc_بلاکیں;
	} ورنہ
		پہلا_meta_bg = fs->desc_بلاکیں;
	اگر (پہلا_meta_bg) {
		retval = io_channel_read_blk(fs->io, گروہ_بلاک +
					     گروہ_zero_adjust + 1,
					     پہلا_meta_bg, dest);
		اگر (retval)
			جاؤ cleanup;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		gdp = (ساخت ext2_گروہ_desc *) dest;
		برائے (j=0; j < groups_فی_بلاک*پہلا_meta_bg; j++) {
			gdp = ext2fs_گروہ_desc(fs, fs->گروہ_desc, j);
			اگر (gdp)
				ext2fs_تبادلہ_گروہ_desc2(fs, gdp);
		}
#اگر_ختم
		dest += fs->blocksize*پہلا_meta_bg;
	}

	برائے (i = پہلا_meta_bg ; i < fs->desc_بلاکیں; i++) {
		blk = ext2fs_descriptor_بلاک_loc2(fs, گروہ_بلاک, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	برائے (i=پہلا_meta_bg ; i < fs->desc_بلاکیں; i++) {
		blk = ext2fs_descriptor_بلاک_loc2(fs, گروہ_بلاک, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		اگر (retval)
			جاؤ cleanup;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		برائے (j=0; j < groups_فی_بلاک; j++) {
			gdp = ext2fs_گروہ_desc(fs, fs->گروہ_desc,
						i * groups_فی_بلاک + j);
			اگر (gdp)
				ext2fs_تبادلہ_گروہ_desc2(fs, gdp);
		}
#اگر_ختم
		dest += fs->blocksize;
	}

	fs->stride = fs->super->s_raid_stride;

	/*
	 * If recovery is from backup superblock, Clear _UNININT flags &
	 * reset bg_itable_unused to zero
	 */
	اگر (superblock > 1 && ext2fs_has_گروہ_desc_csum(fs)) {
		dgrp_t گروہ;

		برائے (گروہ = 0; گروہ < fs->گروہ_desc_گنتی; گروہ++) {
			ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_بلاک_UNINIT);
			ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_غیر_مستعمل_سیٹ(fs, گروہ, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);
		}
		اگر (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	اگر (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_آغاز(fs);
		اگر (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			جاؤ cleanup;
		}
	}

	اگر (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->بلاک_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					بلاک_sha_map_free_اندراج, 4096);
		اگر (!fs->بلاک_sha_map) {
			retval = EXT2_ET_NO_حافظہ;
			جاؤ cleanup;
		}
		ext2fs_سیٹ_feature_shared_بلاکیں(fs->super);
	}

	اگر (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_بھرو_nls_جدول(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	واپس 0;
cleanup:
	اگر (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = تہی;
	}
	*ret_fs = fs;
	واپس retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_ڈیٹا_io(ext2_filsys fs, io_channel *old_io)
{
	اگر ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		واپس EXT2_ET_NOT_IMAGE_FILE;
	اگر (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	واپس 0;
}

errcode_t ext2fs_سیٹ_ڈیٹا_io(ext2_filsys fs, io_channel new_io)
{
	اگر ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		واپس EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	واپس 0;
}

errcode_t ext2fs_rewrite_مقصد_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	اگر ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		واپس EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_سیٹ_blksize(new_io, fs->blocksize);
	اگر (err)
		واپس err;
	اگر ((new_io == fs->image_io) || (new_io == fs->io))
		واپس 0;
	اگر ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	اگر (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	واپس 0;
}
