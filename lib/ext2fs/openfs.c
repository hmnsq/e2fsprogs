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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"

#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "e2image.h"

blk64_t ext2fs_descriptor_ಖಂಡ_loc2(ext2_filsys fs, blk64_t ಗುಂಪು_ಖಂಡ,
				     dgrp_t i)
{
	ಪೂರ್ಣಾಂಕ	bg;
	ಪೂರ್ಣಾಂಕ	has_super = 0, ಗುಂಪು_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	ಆದರೆ (i == 0 && fs->blocksize == 1024 && EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) > 1)
		ಗುಂಪು_zero_adjust = 1;

	ಆದರೆ (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_ಮೊದಲ_meta_bg))
		ಹಿಂತಿರುಗಿ ಗುಂಪು_ಖಂಡ + i + 1 + ಗುಂಪು_zero_adjust;

	bg = EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super) * i;
	ಆದರೆ (ext2fs_bg_has_super(fs, bg))
		has_super = 1;
	ret_blk = ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, bg);
	/*
	 * If group_block is not the normal value, we're trying to use
	 * the backup group descriptors and superblock --- so use the
	 * alternate location of the second block group in the
	 * metablock group.  Ideally we should be testing each bg
	 * descriptor block individually for correctness, but we don't
	 * have the infrastructure in place to do that.
	 */
	ಆದರೆ (ಗುಂಪು_ಖಂಡ != fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ &&
	    ((ret_blk + has_super + fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು) <
	     ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))) {
		ret_blk += fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		ಆದರೆ (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		ಇಲ್ಲದಿದ್ದರೆ
			has_super = 0;
	}
	ಹಿಂತಿರುಗಿ ret_blk + has_super + ಗುಂಪು_zero_adjust;
}

blk_t ext2fs_descriptor_ಖಂಡ_loc(ext2_filsys fs, blk_t ಗುಂಪು_ಖಂಡ, dgrp_t i)
{
	ಹಿಂತಿರುಗಿ ext2fs_descriptor_ಖಂಡ_loc2(fs, ಗುಂಪು_ಖಂಡ, i);
}

errcode_t ext2fs_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ superblock,
		      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ, io_manager manager,
		      ext2_filsys *ret_fs)
{
	ಹಿಂತಿರುಗಿ ext2fs_open2(name, 0, flags, superblock, ಖಂಡ_ಗಾತ್ರ,
			    manager, ret_fs);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಖಂಡ_sha_map_free_ನಮೂದು(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
	ಹಿಂತಿರುಗಿ;
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
errcode_t ext2fs_open2(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *io_options,
		       ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ superblock,
		       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	i, ಮೊದಲ_meta_bg;
	__u32		features;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು, io_flags;
	blk64_t		ಗುಂಪು_ಖಂಡ, blk;
	ಅಕ್ಷರ		*dest, *cp;
	ಪೂರ್ಣಾಂಕ		ಗುಂಪು_zero_adjust = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	inode_ಗಾತ್ರ;
	__u64		groups_cnt;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	groups_ಪ್ರತಿ_ಖಂಡ;
	ರಚನೆ ext2_ಗುಂಪು_desc *gdp;
	ಪೂರ್ಣಾಂಕ		j;
#ಆದರೆ_ಅಂತ್ಯ
	ಅಕ್ಷರ		*ಸಮಯ_env;
	ಪೂರ್ಣಾಂಕ		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ struct_ext2_filsys), &fs);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	memset(fs, 0, ಗಾತ್ರ(ರಚನೆ struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	ಸಮಯ_env = ext2fs_ಸುರಕ್ಷಿತ_getenv("SOURCE_DATE_EPOCH");
	ಆದರೆ (ಸಮಯ_env) {
		fs->now = strtoul(ಸಮಯ_env, ಖಾಲಿ, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_ಸಮಯ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಸಮಯ_env = ext2fs_ಸುರಕ್ಷಿತ_getenv("E2FSPROGS_FAKE_TIME");
		ಆದರೆ (ಸಮಯ_env)
			fs->now = strtoul(ಸಮಯ_env, ಖಾಲಿ, 0);
	}

	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)+1, &fs->device_name);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	strcpy(fs->device_name, name);
	cp = strchr(fs->device_name, '?');
	ಆದರೆ (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	ಆದರೆ (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	ಆದರೆ (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	ಆದರೆ (flags & EXT2_FLAG_ನೇರ_IO)
		io_flags |= IO_FLAG_ನೇರ_IO;
	ಆದರೆ (flags & EXT2_FLAG_ಎಳೆಗಳು)
		io_flags |= IO_FLAG_ಎಳೆಗಳು;
	retval = manager->open(fs->device_name, io_flags, &fs->io);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	ಆದರೆ (io_options &&
	    (retval = io_channel_ಗಣ_options(fs->io, io_options)))
		ಹೋಗು cleanup;
	fs->image_io = fs->io;
	fs->io->app_ದತ್ತಾಂಶ = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_ಗಾತ್ರ, &fs->super);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	ಆದರೆ (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_image_hdr),
					&fs->image_ಶೀರ್ಷಿಕೆ);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ರಚನೆ ext2_image_hdr),
					     fs->image_ಶೀರ್ಷಿಕೆ);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
		ಆದರೆ (ext2fs_le32_ಗಮ್ಯ_cpu(fs->image_ಶೀರ್ಷಿಕೆ->magic_ಸಂಖ್ಯೆ) != EXT2_ET_MAGIC_E2IMAGE)
			ಹಿಂತಿರುಗಿ EXT2_ET_MAGIC_E2IMAGE;
		superblock = 1;
		ಖಂಡ_ಗಾತ್ರ = ext2fs_le32_ಗಮ್ಯ_cpu(fs->image_ಶೀರ್ಷಿಕೆ->fs_blocksize);
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
	ಆದರೆ (superblock) {
		ಆದರೆ (!ಖಂಡ_ಗಾತ್ರ) {
			retval = EXT2_ET_INVALID_ವಾದ;
			ಹೋಗು cleanup;
		}
		io_channel_ಗಣ_blksize(fs->io, ಖಂಡ_ಗಾತ್ರ);
		ಗುಂಪು_ಖಂಡ = superblock;
		fs->orig_super = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		io_channel_ಗಣ_blksize(fs->io, SUPERBLOCK_ಸ್ಥಾನಾಂತರ);
		superblock = 1;
		ಗುಂಪು_ಖಂಡ = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_ಗಾತ್ರ, &fs->orig_super);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_ಗಾತ್ರ,
				     fs->super);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	ಆದರೆ (fs->orig_super)
		ಸ್ಮರಣೆ_ನಕಲು(fs->orig_super, fs->super, SUPERBLOCK_ಗಾತ್ರ);

	ಆದರೆ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		ಆದರೆ (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		ಆದರೆ (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			ಆದರೆ (csum_retries++ < 3)
				ಹೋಗು retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	fs->flags |= EXT2_FLAG_ಅದಲುಬದಲು_ಬೈಟುಗಳು;
	ext2fs_ಅದಲುಬದಲು_super(fs->super);
#ಇಲ್ಲದಿದ್ದರೆ
	ಆದರೆ (fs->flags & EXT2_FLAG_ಅದಲುಬದಲು_ಬೈಟುಗಳು) {
		retval = EXT2_ET_UNIMPLEMENTED;
		ಹೋಗು cleanup;
	}
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	ಆದರೆ (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_ಮೇಲಿನ;
		ಹೋಗು cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	ಆದರೆ (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EXT2_LIB_SOFTSUPP_INCOMPAT
		ಆದರೆ (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			ಹೋಗು cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EXT2_LIB_SOFTSUPP_RO_COMPAT
		ಆದರೆ (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			ಹೋಗು cleanup;
		}

		ಆದರೆ (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			ಹೋಗು cleanup;
		}
	}

	ಆದರೆ ((fs->super->s_log_ಖಂಡ_ಗಾತ್ರ >
	     (ಚಿಹ್ನೆರಹಿತ) (EXT2_MAX_ಖಂಡ_LOG_ಗಾತ್ರ - EXT2_MIN_ಖಂಡ_LOG_ಗಾತ್ರ)) ||
	    (fs->super->s_log_cluster_ಗಾತ್ರ >
	     (ಚಿಹ್ನೆರಹಿತ) (EXT2_MAX_CLUSTER_LOG_ಗಾತ್ರ - EXT2_MIN_CLUSTER_LOG_ಗಾತ್ರ)) ||
	    (fs->super->s_log_ಖಂಡ_ಗಾತ್ರ > fs->super->s_log_cluster_ಗಾತ್ರ) ||
	    (fs->super->s_log_groups_ಪ್ರತಿ_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ಹೋಗು cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	ಆದರೆ (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		ಹೋಗು cleanup;
	}

	ಆದರೆ (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_ಖಂಡ_ಗಾತ್ರ != fs->super->s_log_cluster_ಗಾತ್ರ)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ಹೋಗು cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_ಖಂಡ_ಗಾತ್ರ(fs->super);
	inode_ಗಾತ್ರ = EXT2_INODE_ಗಾತ್ರ(fs->super);
	ಆದರೆ ((inode_ಗಾತ್ರ < EXT2_GOOD_OLD_INODE_ಗಾತ್ರ) ||
	    (inode_ಗಾತ್ರ > fs->blocksize) ||
	    (inode_ಗಾತ್ರ & (inode_ಗಾತ್ರ - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ಹೋಗು cleanup;
	}

	/* Enforce the block group descriptor size */
	ಆದರೆ (ext2fs_has_feature_64bit(fs->super)) {
		ಚಿಹ್ನೆರಹಿತ desc_ಗಾತ್ರ = fs->super->s_desc_ಗಾತ್ರ;

		ಆದರೆ (desc_ಗಾತ್ರ == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_ಗಾತ್ರ > EXT2_MAX_DESC_ಗಾತ್ರ) ||
		      (desc_ಗಾತ್ರ < EXT2_MIN_DESC_ಗಾತ್ರ_64_ಬಿಟ್) ||
		      (desc_ಗಾತ್ರ & (desc_ಗಾತ್ರ - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_ಗಾತ್ರ;
			ಹೋಗು cleanup;
		}
	}

	fs->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು = fs->super->s_log_cluster_ಗಾತ್ರ -
		fs->super->s_log_ಖಂಡ_ಗಾತ್ರ;
	ಆದರೆ (EXT2_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(fs->super) !=
	    EXT2_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(fs->super) << fs->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ಹೋಗು cleanup;
	}
	fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು = ((EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) *
				       EXT2_INODE_ಗಾತ್ರ(fs->super) +
				       EXT2_ಖಂಡ_ಗಾತ್ರ(fs->super) - 1) /
				      EXT2_ಖಂಡ_ಗಾತ್ರ(fs->super));
	ಆದರೆ (ಖಂಡ_ಗಾತ್ರ) {
		ಆದರೆ (ಖಂಡ_ಗಾತ್ರ != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_ಖಂಡ_ಗಾತ್ರ;
			ಹೋಗು cleanup;
		}
	}
	/*
	 * Set the blocksize to the filesystem's blocksize.
	 */
	io_channel_ಗಣ_blksize(fs->io, fs->blocksize);

	/*
	 * If this is an external journal device, don't try to read
	 * the group descriptors, because they're not there.
	 */
	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->ಗುಂಪು_desc_ಎಣಿಕೆ = 0;
		*ret_fs = fs;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ಹೋಗು cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_ಬೀಜ(fs);

	/*
	 * Read group descriptors
	 */
	ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು = EXT2_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(fs->super);
	ಆದರೆ (ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು < 8 ||
	    ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು > EXT2_MAX_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(fs->super) ||
	    fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು > EXT2_MAX_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) ||
           EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super) == 0 ||
           fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ಹೋಗು cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) -
				       fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ,
				       ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು);
	ಆದರೆ (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ಹೋಗು cleanup;
	}
	fs->ಗುಂಪು_desc_ಎಣಿಕೆ = 	groups_cnt;
	ಆದರೆ (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->ಗುಂಪು_desc_ಎಣಿಕೆ * EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) !=
	    fs->super->s_inodes_ಎಣಿಕೆ) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ಹೋಗು cleanup;
	}
	fs->desc_ಖಂಡಗಳು = ext2fs_div_ceil(fs->ಗುಂಪು_desc_ಎಣಿಕೆ,
					  EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super));
	ಆದರೆ (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_ಮೊದಲ_meta_bg > fs->desc_ಖಂಡಗಳು) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ಹೋಗು cleanup;
	}
	ಆದರೆ (flags & EXT2_FLAG_SUPER_ONLY)
		ಹೋಗು skip_read_bg;
	retval = ext2fs_get_array(fs->desc_ಖಂಡಗಳು, fs->blocksize,
				&fs->ಗುಂಪು_desc);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	ಆದರೆ (!ಗುಂಪು_ಖಂಡ)
		ಗುಂಪು_ಖಂಡ = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
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
	ಆದರೆ (ಗುಂಪು_ಖಂಡ == 0 && fs->blocksize == 1024)
		ಗುಂಪು_zero_adjust = 1;
	dest = (ಅಕ್ಷರ *) fs->ಗುಂಪು_desc;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	groups_ಪ್ರತಿ_ಖಂಡ = EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super);
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		ಮೊದಲ_meta_bg = fs->super->s_ಮೊದಲ_meta_bg;
		ಆದರೆ (ಮೊದಲ_meta_bg > fs->desc_ಖಂಡಗಳು)
			ಮೊದಲ_meta_bg = fs->desc_ಖಂಡಗಳು;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಮೊದಲ_meta_bg = fs->desc_ಖಂಡಗಳು;
	ಆದರೆ (ಮೊದಲ_meta_bg) {
		retval = io_channel_read_blk(fs->io, ಗುಂಪು_ಖಂಡ +
					     ಗುಂಪು_zero_adjust + 1,
					     ಮೊದಲ_meta_bg, dest);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		gdp = (ರಚನೆ ext2_ಗುಂಪು_desc *) dest;
		ಗಾಗಿ (j=0; j < groups_ಪ್ರತಿ_ಖಂಡ*ಮೊದಲ_meta_bg; j++) {
			gdp = ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, j);
			ಆದರೆ (gdp)
				ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(fs, gdp);
		}
#ಆದರೆ_ಅಂತ್ಯ
		dest += fs->blocksize*ಮೊದಲ_meta_bg;
	}

	ಗಾಗಿ (i = ಮೊದಲ_meta_bg ; i < fs->desc_ಖಂಡಗಳು; i++) {
		blk = ext2fs_descriptor_ಖಂಡ_loc2(fs, ಗುಂಪು_ಖಂಡ, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	ಗಾಗಿ (i=ಮೊದಲ_meta_bg ; i < fs->desc_ಖಂಡಗಳು; i++) {
		blk = ext2fs_descriptor_ಖಂಡ_loc2(fs, ಗುಂಪು_ಖಂಡ, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		ಗಾಗಿ (j=0; j < groups_ಪ್ರತಿ_ಖಂಡ; j++) {
			gdp = ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc,
						i * groups_ಪ್ರತಿ_ಖಂಡ + j);
			ಆದರೆ (gdp)
				ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(fs, gdp);
		}
#ಆದರೆ_ಅಂತ್ಯ
		dest += fs->blocksize;
	}

	fs->stride = fs->super->s_raid_stride;

	/*
	 * If recovery is from backup superblock, Clear _UNININT flags &
	 * reset bg_itable_unused to zero
	 */
	ಆದರೆ (superblock > 1 && ext2fs_has_ಗುಂಪು_desc_csum(fs)) {
		dgrp_t ಗುಂಪು;

		ಗಾಗಿ (ಗುಂಪು = 0; ಗುಂಪು < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; ಗುಂಪು++) {
			ext2fs_bg_flags_clear(fs, ಗುಂಪು, EXT2_BG_ಖಂಡ_UNINIT);
			ext2fs_bg_flags_clear(fs, ಗುಂಪು, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_ಬಳಸದ_ಗಣ(fs, ಗುಂಪು, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, ಗುಂಪು);
		}
		ಆದರೆ (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	ಆದರೆ (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_ಆರಂಭ(fs);
		ಆದರೆ (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			ಹೋಗು cleanup;
		}
	}

	ಆದರೆ (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->ಖಂಡ_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					ಖಂಡ_sha_map_free_ನಮೂದು, 4096);
		ಆದರೆ (!fs->ಖಂಡ_sha_map) {
			retval = EXT2_ET_NO_ಸ್ಮರಣೆ;
			ಹೋಗು cleanup;
		}
		ext2fs_ಗಣ_feature_shared_ಖಂಡಗಳು(fs->super);
	}

	ಆದರೆ (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_ತುಂಬು_nls_ಕೋಷ್ಟಕ(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	ಹಿಂತಿರುಗಿ 0;
cleanup:
	ಆದರೆ (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = ಖಾಲಿ;
	}
	*ret_fs = fs;
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_ದತ್ತಾಂಶ_io(ext2_filsys fs, io_channel *old_io)
{
	ಆದರೆ ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_NOT_IMAGE_FILE;
	ಆದರೆ (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_ಗಣ_ದತ್ತಾಂಶ_io(ext2_filsys fs, io_channel new_io)
{
	ಆದರೆ ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_rewrite_ಗಮ್ಯ_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	ಆದರೆ ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_ಗಣ_blksize(new_io, fs->blocksize);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;
	ಆದರೆ ((new_io == fs->image_io) || (new_io == fs->io))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	ಆದರೆ (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	ಹಿಂತಿರುಗಿ 0;
}
