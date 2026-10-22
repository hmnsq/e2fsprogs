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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "e2image.h"

blk64_t ext2fs_descriptor_ਖੰਡ_loc2(ext2_filsys fs, blk64_t ਸਮੂਹ_ਖੰਡ,
				     dgrp_t i)
{
	ਪੂਰਨ	bg;
	ਪੂਰਨ	has_super = 0, ਸਮੂਹ_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	ਜੇ (i == 0 && fs->blocksize == 1024 && EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs) > 1)
		ਸਮੂਹ_zero_adjust = 1;

	ਜੇ (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_ਪਹਿਲਾ_meta_bg))
		ਵਾਪਸ ਸਮੂਹ_ਖੰਡ + i + 1 + ਸਮੂਹ_zero_adjust;

	bg = EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super) * i;
	ਜੇ (ext2fs_bg_has_super(fs, bg))
		has_super = 1;
	ret_blk = ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, bg);
	/*
	 * If group_block is not the normal value, we're trying to use
	 * the backup group descriptors and superblock --- so use the
	 * alternate location of the second block group in the
	 * metablock group.  Ideally we should be testing each bg
	 * descriptor block individually for correctness, but we don't
	 * have the infrastructure in place to do that.
	 */
	ਜੇ (ਸਮੂਹ_ਖੰਡ != fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ &&
	    ((ret_blk + has_super + fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ) <
	     ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))) {
		ret_blk += fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		ਜੇ (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		ਨਹੀਂਤਾਂ
			has_super = 0;
	}
	ਵਾਪਸ ret_blk + has_super + ਸਮੂਹ_zero_adjust;
}

blk_t ext2fs_descriptor_ਖੰਡ_loc(ext2_filsys fs, blk_t ਸਮੂਹ_ਖੰਡ, dgrp_t i)
{
	ਵਾਪਸ ext2fs_descriptor_ਖੰਡ_loc2(fs, ਸਮੂਹ_ਖੰਡ, i);
}

errcode_t ext2fs_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, ਪੂਰਨ superblock,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ_ਮਾਪ, io_manager manager,
		      ext2_filsys *ret_fs)
{
	ਵਾਪਸ ext2fs_open2(name, 0, flags, superblock, ਖੰਡ_ਮਾਪ,
			    manager, ret_fs);
}

ਸਥਿਰ ਖਾਲੀ ਖੰਡ_sha_map_free_ਇੰਦਰਾਜ(ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਮੁਕਤ(ਅੰਕੜੇ);
	ਵਾਪਸ;
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
errcode_t ext2fs_open2(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *io_options,
		       ਪੂਰਨ flags, ਪੂਰਨ superblock,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ_ਮਾਪ, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	i, ਪਹਿਲਾ_meta_bg;
	__u32		features;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ, io_flags;
	blk64_t		ਸਮੂਹ_ਖੰਡ, blk;
	ਅੱਖਰ		*dest, *cp;
	ਪੂਰਨ		ਸਮੂਹ_zero_adjust = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	inode_ਮਾਪ;
	__u64		groups_cnt;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	groups_ਪ੍ਰਤੀ_ਖੰਡ;
	ਬਣਤਰ ext2_ਸਮੂਹ_desc *gdp;
	ਪੂਰਨ		j;
#ਜੇ_ਅੰਤ
	ਅੱਖਰ		*ਸਮਾਂ_env;
	ਪੂਰਨ		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ struct_ext2_filsys), &fs);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	memset(fs, 0, ਆਕਾਰ(ਬਣਤਰ struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	ਸਮਾਂ_env = ext2fs_ਸੁਰੱਖਿਅਤ_getenv("SOURCE_DATE_EPOCH");
	ਜੇ (ਸਮਾਂ_env) {
		fs->now = strtoul(ਸਮਾਂ_env, ਨਿਰਾ, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_ਸਮਾਂ;
	} ਨਹੀਂਤਾਂ {
		ਸਮਾਂ_env = ext2fs_ਸੁਰੱਖਿਅਤ_getenv("E2FSPROGS_FAKE_TIME");
		ਜੇ (ਸਮਾਂ_env)
			fs->now = strtoul(ਸਮਾਂ_env, ਨਿਰਾ, 0);
	}

	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(name)+1, &fs->device_name);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	strcpy(fs->device_name, name);
	cp = strchr(fs->device_name, '?');
	ਜੇ (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	ਜੇ (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	ਜੇ (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	ਜੇ (flags & EXT2_FLAG_ਸਿੱਧਾ_IO)
		io_flags |= IO_FLAG_ਸਿੱਧਾ_IO;
	ਜੇ (flags & EXT2_FLAG_ਧਾਗੇ)
		io_flags |= IO_FLAG_ਧਾਗੇ;
	retval = manager->open(fs->device_name, io_flags, &fs->io);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	ਜੇ (io_options &&
	    (retval = io_channel_ਸੈੱਟ_options(fs->io, io_options)))
		ਜਾਓ cleanup;
	fs->image_io = fs->io;
	fs->io->app_ਅੰਕੜੇ = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_ਮਾਪ, &fs->super);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	ਜੇ (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_image_hdr),
					&fs->image_ਸਿਰਲੇਖ);
		ਜੇ (retval)
			ਜਾਓ cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(ਪੂਰਨ)ਆਕਾਰ(ਬਣਤਰ ext2_image_hdr),
					     fs->image_ਸਿਰਲੇਖ);
		ਜੇ (retval)
			ਜਾਓ cleanup;
		ਜੇ (ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(fs->image_ਸਿਰਲੇਖ->magic_ਸੰਖਿਆ) != EXT2_ET_MAGIC_E2IMAGE)
			ਵਾਪਸ EXT2_ET_MAGIC_E2IMAGE;
		superblock = 1;
		ਖੰਡ_ਮਾਪ = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(fs->image_ਸਿਰਲੇਖ->fs_blocksize);
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
	ਜੇ (superblock) {
		ਜੇ (!ਖੰਡ_ਮਾਪ) {
			retval = EXT2_ET_INVALID_ਦਲੀਲ;
			ਜਾਓ cleanup;
		}
		io_channel_ਸੈੱਟ_blksize(fs->io, ਖੰਡ_ਮਾਪ);
		ਸਮੂਹ_ਖੰਡ = superblock;
		fs->orig_super = 0;
	} ਨਹੀਂਤਾਂ {
		io_channel_ਸੈੱਟ_blksize(fs->io, SUPERBLOCK_ਵਿੱਥ);
		superblock = 1;
		ਸਮੂਹ_ਖੰਡ = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_ਮਾਪ, &fs->orig_super);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_ਮਾਪ,
				     fs->super);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	ਜੇ (fs->orig_super)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->orig_super, fs->super, SUPERBLOCK_ਮਾਪ);

	ਜੇ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		ਜੇ (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		ਜੇ (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			ਜੇ (csum_retries++ < 3)
				ਜਾਓ retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	fs->flags |= EXT2_FLAG_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ;
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_super(fs->super);
#ਨਹੀਂਤਾਂ
	ਜੇ (fs->flags & EXT2_FLAG_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ) {
		retval = EXT2_ET_UNIMPLEMENTED;
		ਜਾਓ cleanup;
	}
#ਜੇ_ਅੰਤ

	ਜੇ (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	ਜੇ (retval)
		ਜਾਓ cleanup;

	ਜੇ (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_ਉੱਚਾ;
		ਜਾਓ cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	ਜੇ (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EXT2_LIB_SOFTSUPP_INCOMPAT
		ਜੇ (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#ਜੇ_ਅੰਤ
		ਜੇ (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			ਜਾਓ cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EXT2_LIB_SOFTSUPP_RO_COMPAT
		ਜੇ (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#ਜੇ_ਅੰਤ
		ਜੇ ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			ਜਾਓ cleanup;
		}

		ਜੇ (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			ਜਾਓ cleanup;
		}
	}

	ਜੇ ((fs->super->s_log_ਖੰਡ_ਮਾਪ >
	     (ਬਿਨਾਨਿਸ਼ਾਨੀ) (EXT2_MAX_ਖੰਡ_LOG_ਮਾਪ - EXT2_MIN_ਖੰਡ_LOG_ਮਾਪ)) ||
	    (fs->super->s_log_cluster_ਮਾਪ >
	     (ਬਿਨਾਨਿਸ਼ਾਨੀ) (EXT2_MAX_CLUSTER_LOG_ਮਾਪ - EXT2_MIN_CLUSTER_LOG_ਮਾਪ)) ||
	    (fs->super->s_log_ਖੰਡ_ਮਾਪ > fs->super->s_log_cluster_ਮਾਪ) ||
	    (fs->super->s_log_groups_ਪ੍ਰਤੀ_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ਜਾਓ cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	ਜੇ (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64_ਬਿੱਟ)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		ਜਾਓ cleanup;
	}

	ਜੇ (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_ਖੰਡ_ਮਾਪ != fs->super->s_log_cluster_ਮਾਪ)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ਜਾਓ cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_ਖੰਡ_ਮਾਪ(fs->super);
	inode_ਮਾਪ = EXT2_INODE_ਮਾਪ(fs->super);
	ਜੇ ((inode_ਮਾਪ < EXT2_GOOD_OLD_INODE_ਮਾਪ) ||
	    (inode_ਮਾਪ > fs->blocksize) ||
	    (inode_ਮਾਪ & (inode_ਮਾਪ - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ਜਾਓ cleanup;
	}

	/* Enforce the block group descriptor size */
	ਜੇ (ext2fs_has_feature_64bit(fs->super)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ desc_ਮਾਪ = fs->super->s_desc_ਮਾਪ;

		ਜੇ (desc_ਮਾਪ == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_ਮਾਪ > EXT2_MAX_DESC_ਮਾਪ) ||
		      (desc_ਮਾਪ < EXT2_MIN_DESC_ਮਾਪ_64_ਦੋਆਧਾਰੀ_ਅੰਕ) ||
		      (desc_ਮਾਪ & (desc_ਮਾਪ - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_ਮਾਪ;
			ਜਾਓ cleanup;
		}
	}

	fs->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ = fs->super->s_log_cluster_ਮਾਪ -
		fs->super->s_log_ਖੰਡ_ਮਾਪ;
	ਜੇ (EXT2_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) !=
	    EXT2_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) << fs->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ਜਾਓ cleanup;
	}
	fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ = ((EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) *
				       EXT2_INODE_ਮਾਪ(fs->super) +
				       EXT2_ਖੰਡ_ਮਾਪ(fs->super) - 1) /
				      EXT2_ਖੰਡ_ਮਾਪ(fs->super));
	ਜੇ (ਖੰਡ_ਮਾਪ) {
		ਜੇ (ਖੰਡ_ਮਾਪ != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_ਖੰਡ_ਮਾਪ;
			ਜਾਓ cleanup;
		}
	}
	/*
	 * Set the blocksize to the filesystem's blocksize.
	 */
	io_channel_ਸੈੱਟ_blksize(fs->io, fs->blocksize);

	/*
	 * If this is an external journal device, don't try to read
	 * the group descriptors, because they're not there.
	 */
	ਜੇ (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->ਸਮੂਹ_desc_ਗਿਣਤੀ = 0;
		*ret_fs = fs;
		ਵਾਪਸ 0;
	}

	ਜੇ (EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ਜਾਓ cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_ਬੀਜ(fs);

	/*
	 * Read group descriptors
	 */
	ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ = EXT2_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super);
	ਜੇ (ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ < 8 ||
	    ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ > EXT2_MAX_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) ||
	    fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ > EXT2_MAX_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) ||
           EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super) == 0 ||
           fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ਜਾਓ cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) -
				       fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ,
				       ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ);
	ਜੇ (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ਜਾਓ cleanup;
	}
	fs->ਸਮੂਹ_desc_ਗਿਣਤੀ = 	groups_cnt;
	ਜੇ (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->ਸਮੂਹ_desc_ਗਿਣਤੀ * EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) !=
	    fs->super->s_inodes_ਗਿਣਤੀ) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ਜਾਓ cleanup;
	}
	fs->desc_ਖੰਡਾਂ = ext2fs_div_ceil(fs->ਸਮੂਹ_desc_ਗਿਣਤੀ,
					  EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super));
	ਜੇ (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_ਪਹਿਲਾ_meta_bg > fs->desc_ਖੰਡਾਂ) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		ਜਾਓ cleanup;
	}
	ਜੇ (flags & EXT2_FLAG_SUPER_ONLY)
		ਜਾਓ skip_read_bg;
	retval = ext2fs_get_array(fs->desc_ਖੰਡਾਂ, fs->blocksize,
				&fs->ਸਮੂਹ_desc);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	ਜੇ (!ਸਮੂਹ_ਖੰਡ)
		ਸਮੂਹ_ਖੰਡ = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
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
	ਜੇ (ਸਮੂਹ_ਖੰਡ == 0 && fs->blocksize == 1024)
		ਸਮੂਹ_zero_adjust = 1;
	dest = (ਅੱਖਰ *) fs->ਸਮੂਹ_desc;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	groups_ਪ੍ਰਤੀ_ਖੰਡ = EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super);
#ਜੇ_ਅੰਤ
	ਜੇ (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		ਪਹਿਲਾ_meta_bg = fs->super->s_ਪਹਿਲਾ_meta_bg;
		ਜੇ (ਪਹਿਲਾ_meta_bg > fs->desc_ਖੰਡਾਂ)
			ਪਹਿਲਾ_meta_bg = fs->desc_ਖੰਡਾਂ;
	} ਨਹੀਂਤਾਂ
		ਪਹਿਲਾ_meta_bg = fs->desc_ਖੰਡਾਂ;
	ਜੇ (ਪਹਿਲਾ_meta_bg) {
		retval = io_channel_read_blk(fs->io, ਸਮੂਹ_ਖੰਡ +
					     ਸਮੂਹ_zero_adjust + 1,
					     ਪਹਿਲਾ_meta_bg, dest);
		ਜੇ (retval)
			ਜਾਓ cleanup;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		gdp = (ਬਣਤਰ ext2_ਸਮੂਹ_desc *) dest;
		ਲਈ (j=0; j < groups_ਪ੍ਰਤੀ_ਖੰਡ*ਪਹਿਲਾ_meta_bg; j++) {
			gdp = ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, j);
			ਜੇ (gdp)
				ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(fs, gdp);
		}
#ਜੇ_ਅੰਤ
		dest += fs->blocksize*ਪਹਿਲਾ_meta_bg;
	}

	ਲਈ (i = ਪਹਿਲਾ_meta_bg ; i < fs->desc_ਖੰਡਾਂ; i++) {
		blk = ext2fs_descriptor_ਖੰਡ_loc2(fs, ਸਮੂਹ_ਖੰਡ, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	ਲਈ (i=ਪਹਿਲਾ_meta_bg ; i < fs->desc_ਖੰਡਾਂ; i++) {
		blk = ext2fs_descriptor_ਖੰਡ_loc2(fs, ਸਮੂਹ_ਖੰਡ, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		ਜੇ (retval)
			ਜਾਓ cleanup;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		ਲਈ (j=0; j < groups_ਪ੍ਰਤੀ_ਖੰਡ; j++) {
			gdp = ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc,
						i * groups_ਪ੍ਰਤੀ_ਖੰਡ + j);
			ਜੇ (gdp)
				ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(fs, gdp);
		}
#ਜੇ_ਅੰਤ
		dest += fs->blocksize;
	}

	fs->stride = fs->super->s_raid_stride;

	/*
	 * If recovery is from backup superblock, Clear _UNININT flags &
	 * reset bg_itable_unused to zero
	 */
	ਜੇ (superblock > 1 && ext2fs_has_ਸਮੂਹ_desc_csum(fs)) {
		dgrp_t ਸਮੂਹ;

		ਲਈ (ਸਮੂਹ = 0; ਸਮੂਹ < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; ਸਮੂਹ++) {
			ext2fs_bg_flags_clear(fs, ਸਮੂਹ, EXT2_BG_ਖੰਡ_UNINIT);
			ext2fs_bg_flags_clear(fs, ਸਮੂਹ, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_ਅਣਵਰਤਿਆ_ਸੈੱਟ(fs, ਸਮੂਹ, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, ਸਮੂਹ);
		}
		ਜੇ (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	ਜੇ (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_ਸ਼ੁਰੂ(fs);
		ਜੇ (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			ਜਾਓ cleanup;
		}
	}

	ਜੇ (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->ਖੰਡ_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					ਖੰਡ_sha_map_free_ਇੰਦਰਾਜ, 4096);
		ਜੇ (!fs->ਖੰਡ_sha_map) {
			retval = EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
			ਜਾਓ cleanup;
		}
		ext2fs_ਸੈੱਟ_feature_shared_ਖੰਡਾਂ(fs->super);
	}

	ਜੇ (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_ਲੱਦੋ_nls_ਸਾਰਣੀ(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	ਵਾਪਸ 0;
cleanup:
	ਜੇ (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = ਨਿਰਾ;
	}
	*ret_fs = fs;
	ਵਾਪਸ retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_ਅੰਕੜੇ_io(ext2_filsys fs, io_channel *old_io)
{
	ਜੇ ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		ਵਾਪਸ EXT2_ET_NOT_IMAGE_FILE;
	ਜੇ (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	ਵਾਪਸ 0;
}

errcode_t ext2fs_ਸੈੱਟ_ਅੰਕੜੇ_io(ext2_filsys fs, io_channel new_io)
{
	ਜੇ ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		ਵਾਪਸ EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_rewrite_ਮੰਜ਼ਿਲ_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	ਜੇ ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		ਵਾਪਸ EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_ਸੈੱਟ_blksize(new_io, fs->blocksize);
	ਜੇ (err)
		ਵਾਪਸ err;
	ਜੇ ((new_io == fs->image_io) || (new_io == fs->io))
		ਵਾਪਸ 0;
	ਜੇ ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	ਜੇ (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	ਵਾਪਸ 0;
}
