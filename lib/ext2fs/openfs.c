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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"

#ضمن "ext2fs.h"
#ضمن "e2image.h"

blk64_t ext2fs_descriptor_بلاڪ_loc2(ext2_filsys fs, blk64_t گروهه_بلاڪ,
				     dgrp_t i)
{
	صحيح	bg;
	صحيح	has_super = 0, گروهه_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	إذا (i == 0 && fs->blocksize == 1024 && EXT2_FS_CLUSTER_تناسب(fs) > 1)
		گروهه_zero_adjust = 1;

	إذا (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_پهريون_meta_bg))
		أرجع گروهه_بلاڪ + i + 1 + گروهه_zero_adjust;

	bg = EXT2_DESC_في_بلاڪ(fs->super) * i;
	إذا (ext2fs_bg_has_super(fs, bg))
		has_super = 1;
	ret_blk = ext2fs_گروهه_پهريون_block2(fs, bg);
	/*
	 * If group_block is not the normal value, we're trying to use
	 * the backup group descriptors and superblock --- so use the
	 * alternate location of the second block group in the
	 * metablock group.  Ideally we should be testing each bg
	 * descriptor block individually for correctness, but we don't
	 * have the infrastructure in place to do that.
	 */
	إذا (گروهه_بلاڪ != fs->super->s_پهريون_معلومات_بلاڪ &&
	    ((ret_blk + has_super + fs->super->s_بلاڪون_في_گروهه) <
	     ext2fs_بلاڪون_ڳڻپ(fs->super))) {
		ret_blk += fs->super->s_بلاڪون_في_گروهه;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		إذا (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		وإلا
			has_super = 0;
	}
	أرجع ret_blk + has_super + گروهه_zero_adjust;
}

blk_t ext2fs_descriptor_بلاڪ_loc(ext2_filsys fs, blk_t گروهه_بلاڪ, dgrp_t i)
{
	أرجع ext2fs_descriptor_بلاڪ_loc2(fs, گروهه_بلاڪ, i);
}

errcode_t ext2fs_open(ثابت حرف *name, صحيح flags, صحيح superblock,
		      بنانشان صحيح بلاڪ_سائيز, io_manager manager,
		      ext2_filsys *ret_fs)
{
	أرجع ext2fs_open2(name, 0, flags, superblock, بلاڪ_سائيز,
			    manager, ret_fs);
}

ساكن فراغ بلاڪ_sha_map_free_داخلا(فراغ *معلومات)
{
	حرر(معلومات);
	أرجع;
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
		       صحيح flags, صحيح superblock,
		       بنانشان صحيح بلاڪ_سائيز, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	بنانشان ڊگهو	i, پهريون_meta_bg;
	__u32		features;
	بنانشان صحيح	بلاڪون_في_گروهه, io_flags;
	blk64_t		گروهه_بلاڪ, blk;
	حرف		*dest, *cp;
	صحيح		گروهه_zero_adjust = 0;
	بنانشان صحيح	inode_سائيز;
	__u64		groups_cnt;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	بنانشان صحيح	groups_في_بلاڪ;
	بنية ext2_گروهه_desc *gdp;
	صحيح		j;
#جيڪڏهن_پورو
	حرف		*وقت_env;
	صحيح		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(حجم(بنية struct_ext2_filsys), &fs);
	إذا (retval)
		أرجع retval;

	memset(fs, 0, حجم(بنية struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	وقت_env = ext2fs_محفوظ_getenv("SOURCE_DATE_EPOCH");
	إذا (وقت_env) {
		fs->now = strtoul(وقت_env, عدم, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_وقت;
	} وإلا {
		وقت_env = ext2fs_محفوظ_getenv("E2FSPROGS_FAKE_TIME");
		إذا (وقت_env)
			fs->now = strtoul(وقت_env, عدم, 0);
	}

	retval = ext2fs_get_mem(لڙي_ڊيگهه(name)+1, &fs->device_name);
	إذا (retval)
		اذهب cleanup;
	strcpy(fs->device_name, name);
	cp = strchr(fs->device_name, '?');
	إذا (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	إذا (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	إذا (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	إذا (flags & EXT2_FLAG_سڌو_IO)
		io_flags |= IO_FLAG_سڌو_IO;
	إذا (flags & EXT2_FLAG_ڌاڳا)
		io_flags |= IO_FLAG_ڌاڳا;
	retval = manager->open(fs->device_name, io_flags, &fs->io);
	إذا (retval)
		اذهب cleanup;
	إذا (io_options &&
	    (retval = io_channel_سيٽ_options(fs->io, io_options)))
		اذهب cleanup;
	fs->image_io = fs->io;
	fs->io->app_معلومات = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_سائيز, &fs->super);
	إذا (retval)
		اذهب cleanup;
	إذا (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(حجم(بنية ext2_image_hdr),
					&fs->image_سرو);
		إذا (retval)
			اذهب cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(صحيح)حجم(بنية ext2_image_hdr),
					     fs->image_سرو);
		إذا (retval)
			اذهب cleanup;
		إذا (ext2fs_le32_منزل_cpu(fs->image_سرو->magic_عدد) != EXT2_ET_MAGIC_E2IMAGE)
			أرجع EXT2_ET_MAGIC_E2IMAGE;
		superblock = 1;
		بلاڪ_سائيز = ext2fs_le32_منزل_cpu(fs->image_سرو->fs_blocksize);
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
	إذا (superblock) {
		إذا (!بلاڪ_سائيز) {
			retval = EXT2_ET_INVALID_دليل;
			اذهب cleanup;
		}
		io_channel_سيٽ_blksize(fs->io, بلاڪ_سائيز);
		گروهه_بلاڪ = superblock;
		fs->orig_super = 0;
	} وإلا {
		io_channel_سيٽ_blksize(fs->io, SUPERBLOCK_وٿي);
		superblock = 1;
		گروهه_بلاڪ = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_سائيز, &fs->orig_super);
		إذا (retval)
			اذهب cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_سائيز,
				     fs->super);
	إذا (retval)
		اذهب cleanup;
	إذا (fs->orig_super)
		يادگيري_نقل(fs->orig_super, fs->super, SUPERBLOCK_سائيز);

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		إذا (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		إذا (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			إذا (csum_retries++ < 3)
				اذهب retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	fs->flags |= EXT2_FLAG_مٽا_سٽا_بائيٽون;
	ext2fs_مٽا_سٽا_super(fs->super);
#وإلا
	إذا (fs->flags & EXT2_FLAG_مٽا_سٽا_بائيٽون) {
		retval = EXT2_ET_UNIMPLEMENTED;
		اذهب cleanup;
	}
#جيڪڏهن_پورو

	إذا (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	إذا (retval)
		اذهب cleanup;

	إذا (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_مٿاهون;
		اذهب cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	إذا (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#جيڪڏهن_وصف_ٿيل EXT2_LIB_SOFTSUPP_INCOMPAT
		إذا (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#جيڪڏهن_پورو
		إذا (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			اذهب cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#جيڪڏهن_وصف_ٿيل EXT2_LIB_SOFTSUPP_RO_COMPAT
		إذا (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#جيڪڏهن_پورو
		إذا ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			اذهب cleanup;
		}

		إذا (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			اذهب cleanup;
		}
	}

	إذا ((fs->super->s_log_بلاڪ_سائيز >
	     (بنانشان) (EXT2_MAX_بلاڪ_LOG_سائيز - EXT2_MIN_بلاڪ_LOG_سائيز)) ||
	    (fs->super->s_log_cluster_سائيز >
	     (بنانشان) (EXT2_MAX_CLUSTER_LOG_سائيز - EXT2_MIN_CLUSTER_LOG_سائيز)) ||
	    (fs->super->s_log_بلاڪ_سائيز > fs->super->s_log_cluster_سائيز) ||
	    (fs->super->s_log_groups_في_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		اذهب cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	إذا (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64_بٽ)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		اذهب cleanup;
	}

	إذا (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_بلاڪ_سائيز != fs->super->s_log_cluster_سائيز)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		اذهب cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_بلاڪ_سائيز(fs->super);
	inode_سائيز = EXT2_INODE_سائيز(fs->super);
	إذا ((inode_سائيز < EXT2_GOOD_OLD_INODE_سائيز) ||
	    (inode_سائيز > fs->blocksize) ||
	    (inode_سائيز & (inode_سائيز - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		اذهب cleanup;
	}

	/* Enforce the block group descriptor size */
	إذا (ext2fs_has_feature_64bit(fs->super)) {
		بنانشان desc_سائيز = fs->super->s_desc_سائيز;

		إذا (desc_سائيز == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_سائيز > EXT2_MAX_DESC_سائيز) ||
		      (desc_سائيز < EXT2_MIN_DESC_سائيز_64_ٻٽو_انگ) ||
		      (desc_سائيز & (desc_سائيز - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_سائيز;
			اذهب cleanup;
		}
	}

	fs->cluster_تناسب_بٽ = fs->super->s_log_cluster_سائيز -
		fs->super->s_log_بلاڪ_سائيز;
	إذا (EXT2_بلاڪون_في_گروهه(fs->super) !=
	    EXT2_CLUSTERS_في_گروهه(fs->super) << fs->cluster_تناسب_بٽ) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		اذهب cleanup;
	}
	fs->inode_بلاڪون_في_گروهه = ((EXT2_INODES_في_گروهه(fs->super) *
				       EXT2_INODE_سائيز(fs->super) +
				       EXT2_بلاڪ_سائيز(fs->super) - 1) /
				      EXT2_بلاڪ_سائيز(fs->super));
	إذا (بلاڪ_سائيز) {
		إذا (بلاڪ_سائيز != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_بلاڪ_سائيز;
			اذهب cleanup;
		}
	}
	/*
	 * Set the blocksize to the filesystem's blocksize.
	 */
	io_channel_سيٽ_blksize(fs->io, fs->blocksize);

	/*
	 * If this is an external journal device, don't try to read
	 * the group descriptors, because they're not there.
	 */
	إذا (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->گروهه_desc_ڳڻپ = 0;
		*ret_fs = fs;
		أرجع 0;
	}

	إذا (EXT2_INODES_في_گروهه(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		اذهب cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_ٻج(fs);

	/*
	 * Read group descriptors
	 */
	بلاڪون_في_گروهه = EXT2_بلاڪون_في_گروهه(fs->super);
	إذا (بلاڪون_في_گروهه < 8 ||
	    بلاڪون_في_گروهه > EXT2_MAX_بلاڪون_في_گروهه(fs->super) ||
	    fs->inode_بلاڪون_في_گروهه > EXT2_MAX_INODES_في_گروهه(fs->super) ||
           EXT2_DESC_في_بلاڪ(fs->super) == 0 ||
           fs->super->s_پهريون_معلومات_بلاڪ >= ext2fs_بلاڪون_ڳڻپ(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		اذهب cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_بلاڪون_ڳڻپ(fs->super) -
				       fs->super->s_پهريون_معلومات_بلاڪ,
				       بلاڪون_في_گروهه);
	إذا (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		اذهب cleanup;
	}
	fs->گروهه_desc_ڳڻپ = 	groups_cnt;
	إذا (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->گروهه_desc_ڳڻپ * EXT2_INODES_في_گروهه(fs->super) !=
	    fs->super->s_inodes_ڳڻپ) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		اذهب cleanup;
	}
	fs->desc_بلاڪون = ext2fs_div_ceil(fs->گروهه_desc_ڳڻپ,
					  EXT2_DESC_في_بلاڪ(fs->super));
	إذا (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_پهريون_meta_bg > fs->desc_بلاڪون) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		اذهب cleanup;
	}
	إذا (flags & EXT2_FLAG_SUPER_ONLY)
		اذهب skip_read_bg;
	retval = ext2fs_get_array(fs->desc_بلاڪون, fs->blocksize,
				&fs->گروهه_desc);
	إذا (retval)
		اذهب cleanup;
	إذا (!گروهه_بلاڪ)
		گروهه_بلاڪ = fs->super->s_پهريون_معلومات_بلاڪ;
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
	إذا (گروهه_بلاڪ == 0 && fs->blocksize == 1024)
		گروهه_zero_adjust = 1;
	dest = (حرف *) fs->گروهه_desc;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	groups_في_بلاڪ = EXT2_DESC_في_بلاڪ(fs->super);
#جيڪڏهن_پورو
	إذا (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		پهريون_meta_bg = fs->super->s_پهريون_meta_bg;
		إذا (پهريون_meta_bg > fs->desc_بلاڪون)
			پهريون_meta_bg = fs->desc_بلاڪون;
	} وإلا
		پهريون_meta_bg = fs->desc_بلاڪون;
	إذا (پهريون_meta_bg) {
		retval = io_channel_read_blk(fs->io, گروهه_بلاڪ +
					     گروهه_zero_adjust + 1,
					     پهريون_meta_bg, dest);
		إذا (retval)
			اذهب cleanup;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		gdp = (بنية ext2_گروهه_desc *) dest;
		لكل (j=0; j < groups_في_بلاڪ*پهريون_meta_bg; j++) {
			gdp = ext2fs_گروهه_desc(fs, fs->گروهه_desc, j);
			إذا (gdp)
				ext2fs_مٽا_سٽا_گروهه_desc2(fs, gdp);
		}
#جيڪڏهن_پورو
		dest += fs->blocksize*پهريون_meta_bg;
	}

	لكل (i = پهريون_meta_bg ; i < fs->desc_بلاڪون; i++) {
		blk = ext2fs_descriptor_بلاڪ_loc2(fs, گروهه_بلاڪ, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	لكل (i=پهريون_meta_bg ; i < fs->desc_بلاڪون; i++) {
		blk = ext2fs_descriptor_بلاڪ_loc2(fs, گروهه_بلاڪ, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		إذا (retval)
			اذهب cleanup;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		لكل (j=0; j < groups_في_بلاڪ; j++) {
			gdp = ext2fs_گروهه_desc(fs, fs->گروهه_desc,
						i * groups_في_بلاڪ + j);
			إذا (gdp)
				ext2fs_مٽا_سٽا_گروهه_desc2(fs, gdp);
		}
#جيڪڏهن_پورو
		dest += fs->blocksize;
	}

	fs->stride = fs->super->s_raid_stride;

	/*
	 * If recovery is from backup superblock, Clear _UNININT flags &
	 * reset bg_itable_unused to zero
	 */
	إذا (superblock > 1 && ext2fs_has_گروهه_desc_csum(fs)) {
		dgrp_t گروهه;

		لكل (گروهه = 0; گروهه < fs->گروهه_desc_ڳڻپ; گروهه++) {
			ext2fs_bg_flags_clear(fs, گروهه, EXT2_BG_بلاڪ_UNINIT);
			ext2fs_bg_flags_clear(fs, گروهه, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_اڻ_استعمال_ٿيل_سيٽ(fs, گروهه, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_گروهه_desc_csum_سيٽ(fs, گروهه);
		}
		إذا (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	إذا (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_شروعات(fs);
		إذا (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			اذهب cleanup;
		}
	}

	إذا (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->بلاڪ_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					بلاڪ_sha_map_free_داخلا, 4096);
		إذا (!fs->بلاڪ_sha_map) {
			retval = EXT2_ET_NO_يادگيري;
			اذهب cleanup;
		}
		ext2fs_سيٽ_feature_shared_بلاڪون(fs->super);
	}

	إذا (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_ڀريو_nls_جدول(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	أرجع 0;
cleanup:
	إذا (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = عدم;
	}
	*ret_fs = fs;
	أرجع retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_معلومات_io(ext2_filsys fs, io_channel *old_io)
{
	إذا ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		أرجع EXT2_ET_NOT_IMAGE_FILE;
	إذا (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	أرجع 0;
}

errcode_t ext2fs_سيٽ_معلومات_io(ext2_filsys fs, io_channel new_io)
{
	إذا ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		أرجع EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	أرجع 0;
}

errcode_t ext2fs_rewrite_منزل_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	إذا ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		أرجع EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_سيٽ_blksize(new_io, fs->blocksize);
	إذا (err)
		أرجع err;
	إذا ((new_io == fs->image_io) || (new_io == fs->io))
		أرجع 0;
	إذا ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	إذا (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	أرجع 0;
}
