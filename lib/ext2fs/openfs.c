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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"

#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "e2image.h"

blk64_t ext2fs_descriptor_ఖండం_loc2(ext2_filsys fs, blk64_t సమూహం_ఖండం,
				     dgrp_t i)
{
	పూర్ణాంకం	bg;
	పూర్ణాంకం	has_super = 0, సమూహం_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	ఒకవేళ (i == 0 && fs->blocksize == 1024 && EXT2_FS_CLUSTER_నిష్పత్తి(fs) > 1)
		సమూహం_zero_adjust = 1;

	ఒకవేళ (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_మొదటి_meta_bg))
		తిరిగి సమూహం_ఖండం + i + 1 + సమూహం_zero_adjust;

	bg = EXT2_DESC_చొప్పున_ఖండం(fs->super) * i;
	ఒకవేళ (ext2fs_bg_has_super(fs, bg))
		has_super = 1;
	ret_blk = ext2fs_సమూహం_మొదటి_block2(fs, bg);
	/*
	 * If group_block is not the normal value, we're trying to use
	 * the backup group descriptors and superblock --- so use the
	 * alternate location of the second block group in the
	 * metablock group.  Ideally we should be testing each bg
	 * descriptor block individually for correctness, but we don't
	 * have the infrastructure in place to do that.
	 */
	ఒకవేళ (సమూహం_ఖండం != fs->super->s_మొదటి_దత్తాంశం_ఖండం &&
	    ((ret_blk + has_super + fs->super->s_ఖండాలు_చొప్పున_సమూహం) <
	     ext2fs_ఖండాలు_లెక్క(fs->super))) {
		ret_blk += fs->super->s_ఖండాలు_చొప్పున_సమూహం;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		ఒకవేళ (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		లేకపోతే
			has_super = 0;
	}
	తిరిగి ret_blk + has_super + సమూహం_zero_adjust;
}

blk_t ext2fs_descriptor_ఖండం_loc(ext2_filsys fs, blk_t సమూహం_ఖండం, dgrp_t i)
{
	తిరిగి ext2fs_descriptor_ఖండం_loc2(fs, సమూహం_ఖండం, i);
}

errcode_t ext2fs_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, పూర్ణాంకం superblock,
		      గుర్తులేని పూర్ణాంకం ఖండం_కొలత, io_manager manager,
		      ext2_filsys *ret_fs)
{
	తిరిగి ext2fs_open2(name, 0, flags, superblock, ఖండం_కొలత,
			    manager, ret_fs);
}

స్థిర శూన్య ఖండం_sha_map_free_నమోదు(శూన్య *దత్తాంశం)
{
	విడుదల(దత్తాంశం);
	తిరిగి;
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
errcode_t ext2fs_open2(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *io_options,
		       పూర్ణాంకం flags, పూర్ణాంకం superblock,
		       గుర్తులేని పూర్ణాంకం ఖండం_కొలత, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	గుర్తులేని దీర్ఘ	i, మొదటి_meta_bg;
	__u32		features;
	గుర్తులేని పూర్ణాంకం	ఖండాలు_చొప్పున_సమూహం, io_flags;
	blk64_t		సమూహం_ఖండం, blk;
	అక్షరం		*dest, *cp;
	పూర్ణాంకం		సమూహం_zero_adjust = 0;
	గుర్తులేని పూర్ణాంకం	inode_కొలత;
	__u64		groups_cnt;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	గుర్తులేని పూర్ణాంకం	groups_చొప్పున_ఖండం;
	నిర్మాణం ext2_సమూహం_desc *gdp;
	పూర్ణాంకం		j;
#అయితే_ముగింపు
	అక్షరం		*సమయం_env;
	పూర్ణాంకం		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం struct_ext2_filsys), &fs);
	ఒకవేళ (retval)
		తిరిగి retval;

	memset(fs, 0, పరిమాణం(నిర్మాణం struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	సమయం_env = ext2fs_సురక్షిత_getenv("SOURCE_DATE_EPOCH");
	ఒకవేళ (సమయం_env) {
		fs->now = strtoul(సమయం_env, ఖాళీ, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_సమయం;
	} లేకపోతే {
		సమయం_env = ext2fs_సురక్షిత_getenv("E2FSPROGS_FAKE_TIME");
		ఒకవేళ (సమయం_env)
			fs->now = strtoul(సమయం_env, ఖాళీ, 0);
	}

	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(name)+1, &fs->device_name);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	strcpy(fs->device_name, name);
	cp = strchr(fs->device_name, '?');
	ఒకవేళ (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	ఒకవేళ (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	ఒకవేళ (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	ఒకవేళ (flags & EXT2_FLAG_ప్రత్యక్ష_IO)
		io_flags |= IO_FLAG_ప్రత్యక్ష_IO;
	ఒకవేళ (flags & EXT2_FLAG_దారాలు)
		io_flags |= IO_FLAG_దారాలు;
	retval = manager->open(fs->device_name, io_flags, &fs->io);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	ఒకవేళ (io_options &&
	    (retval = io_channel_సమితి_options(fs->io, io_options)))
		వెళ్ళు cleanup;
	fs->image_io = fs->io;
	fs->io->app_దత్తాంశం = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_కొలత, &fs->super);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	ఒకవేళ (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2_image_hdr),
					&fs->image_శీర్షిక);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(పూర్ణాంకం)పరిమాణం(నిర్మాణం ext2_image_hdr),
					     fs->image_శీర్షిక);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
		ఒకవేళ (ext2fs_le32_గమ్యం_cpu(fs->image_శీర్షిక->magic_సంఖ్య) != EXT2_ET_MAGIC_E2IMAGE)
			తిరిగి EXT2_ET_MAGIC_E2IMAGE;
		superblock = 1;
		ఖండం_కొలత = ext2fs_le32_గమ్యం_cpu(fs->image_శీర్షిక->fs_blocksize);
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
	ఒకవేళ (superblock) {
		ఒకవేళ (!ఖండం_కొలత) {
			retval = EXT2_ET_INVALID_వాదం;
			వెళ్ళు cleanup;
		}
		io_channel_సమితి_blksize(fs->io, ఖండం_కొలత);
		సమూహం_ఖండం = superblock;
		fs->orig_super = 0;
	} లేకపోతే {
		io_channel_సమితి_blksize(fs->io, SUPERBLOCK_స్థానభ్రంశం);
		superblock = 1;
		సమూహం_ఖండం = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_కొలత, &fs->orig_super);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_కొలత,
				     fs->super);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	ఒకవేళ (fs->orig_super)
		జ్ఞాపకం_నకలు(fs->orig_super, fs->super, SUPERBLOCK_కొలత);

	ఒకవేళ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		ఒకవేళ (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		ఒకవేళ (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			ఒకవేళ (csum_retries++ < 3)
				వెళ్ళు retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#నిర్వచితమైతే పదాలు_BIGENDIAN
	fs->flags |= EXT2_FLAG_మార్పిడి_బైట్లు;
	ext2fs_మార్పిడి_super(fs->super);
#లేకపోతే
	ఒకవేళ (fs->flags & EXT2_FLAG_మార్పిడి_బైట్లు) {
		retval = EXT2_ET_UNIMPLEMENTED;
		వెళ్ళు cleanup;
	}
#అయితే_ముగింపు

	ఒకవేళ (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	ఒకవేళ (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_ఎగువ;
		వెళ్ళు cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	ఒకవేళ (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#నిర్వచితమైతే EXT2_LIB_SOFTSUPP_INCOMPAT
		ఒకవేళ (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#అయితే_ముగింపు
		ఒకవేళ (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			వెళ్ళు cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#నిర్వచితమైతే EXT2_LIB_SOFTSUPP_RO_COMPAT
		ఒకవేళ (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#అయితే_ముగింపు
		ఒకవేళ ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			వెళ్ళు cleanup;
		}

		ఒకవేళ (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			వెళ్ళు cleanup;
		}
	}

	ఒకవేళ ((fs->super->s_log_ఖండం_కొలత >
	     (గుర్తులేని) (EXT2_MAX_ఖండం_LOG_కొలత - EXT2_MIN_ఖండం_LOG_కొలత)) ||
	    (fs->super->s_log_cluster_కొలత >
	     (గుర్తులేని) (EXT2_MAX_CLUSTER_LOG_కొలత - EXT2_MIN_CLUSTER_LOG_కొలత)) ||
	    (fs->super->s_log_ఖండం_కొలత > fs->super->s_log_cluster_కొలత) ||
	    (fs->super->s_log_groups_చొప్పున_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		వెళ్ళు cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	ఒకవేళ (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64_బిట్లు)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		వెళ్ళు cleanup;
	}

	ఒకవేళ (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_ఖండం_కొలత != fs->super->s_log_cluster_కొలత)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		వెళ్ళు cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_ఖండం_కొలత(fs->super);
	inode_కొలత = EXT2_INODE_కొలత(fs->super);
	ఒకవేళ ((inode_కొలత < EXT2_GOOD_OLD_INODE_కొలత) ||
	    (inode_కొలత > fs->blocksize) ||
	    (inode_కొలత & (inode_కొలత - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		వెళ్ళు cleanup;
	}

	/* Enforce the block group descriptor size */
	ఒకవేళ (ext2fs_has_feature_64bit(fs->super)) {
		గుర్తులేని desc_కొలత = fs->super->s_desc_కొలత;

		ఒకవేళ (desc_కొలత == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_కొలత > EXT2_MAX_DESC_కొలత) ||
		      (desc_కొలత < EXT2_MIN_DESC_కొలత_64_బిట్టు) ||
		      (desc_కొలత & (desc_కొలత - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_కొలత;
			వెళ్ళు cleanup;
		}
	}

	fs->cluster_నిష్పత్తి_బిట్లు = fs->super->s_log_cluster_కొలత -
		fs->super->s_log_ఖండం_కొలత;
	ఒకవేళ (EXT2_ఖండాలు_చొప్పున_సమూహం(fs->super) !=
	    EXT2_CLUSTERS_చొప్పున_సమూహం(fs->super) << fs->cluster_నిష్పత్తి_బిట్లు) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		వెళ్ళు cleanup;
	}
	fs->inode_ఖండాలు_చొప్పున_సమూహం = ((EXT2_INODES_చొప్పున_సమూహం(fs->super) *
				       EXT2_INODE_కొలత(fs->super) +
				       EXT2_ఖండం_కొలత(fs->super) - 1) /
				      EXT2_ఖండం_కొలత(fs->super));
	ఒకవేళ (ఖండం_కొలత) {
		ఒకవేళ (ఖండం_కొలత != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_ఖండం_కొలత;
			వెళ్ళు cleanup;
		}
	}
	/*
	 * Set the blocksize to the filesystem's blocksize.
	 */
	io_channel_సమితి_blksize(fs->io, fs->blocksize);

	/*
	 * If this is an external journal device, don't try to read
	 * the group descriptors, because they're not there.
	 */
	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->సమూహం_desc_లెక్క = 0;
		*ret_fs = fs;
		తిరిగి 0;
	}

	ఒకవేళ (EXT2_INODES_చొప్పున_సమూహం(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		వెళ్ళు cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_విత్తనం(fs);

	/*
	 * Read group descriptors
	 */
	ఖండాలు_చొప్పున_సమూహం = EXT2_ఖండాలు_చొప్పున_సమూహం(fs->super);
	ఒకవేళ (ఖండాలు_చొప్పున_సమూహం < 8 ||
	    ఖండాలు_చొప్పున_సమూహం > EXT2_MAX_ఖండాలు_చొప్పున_సమూహం(fs->super) ||
	    fs->inode_ఖండాలు_చొప్పున_సమూహం > EXT2_MAX_INODES_చొప్పున_సమూహం(fs->super) ||
           EXT2_DESC_చొప్పున_ఖండం(fs->super) == 0 ||
           fs->super->s_మొదటి_దత్తాంశం_ఖండం >= ext2fs_ఖండాలు_లెక్క(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		వెళ్ళు cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_ఖండాలు_లెక్క(fs->super) -
				       fs->super->s_మొదటి_దత్తాంశం_ఖండం,
				       ఖండాలు_చొప్పున_సమూహం);
	ఒకవేళ (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		వెళ్ళు cleanup;
	}
	fs->సమూహం_desc_లెక్క = 	groups_cnt;
	ఒకవేళ (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->సమూహం_desc_లెక్క * EXT2_INODES_చొప్పున_సమూహం(fs->super) !=
	    fs->super->s_inodes_లెక్క) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		వెళ్ళు cleanup;
	}
	fs->desc_ఖండాలు = ext2fs_div_ceil(fs->సమూహం_desc_లెక్క,
					  EXT2_DESC_చొప్పున_ఖండం(fs->super));
	ఒకవేళ (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_మొదటి_meta_bg > fs->desc_ఖండాలు) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		వెళ్ళు cleanup;
	}
	ఒకవేళ (flags & EXT2_FLAG_SUPER_ONLY)
		వెళ్ళు skip_read_bg;
	retval = ext2fs_get_array(fs->desc_ఖండాలు, fs->blocksize,
				&fs->సమూహం_desc);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	ఒకవేళ (!సమూహం_ఖండం)
		సమూహం_ఖండం = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
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
	ఒకవేళ (సమూహం_ఖండం == 0 && fs->blocksize == 1024)
		సమూహం_zero_adjust = 1;
	dest = (అక్షరం *) fs->సమూహం_desc;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	groups_చొప్పున_ఖండం = EXT2_DESC_చొప్పున_ఖండం(fs->super);
#అయితే_ముగింపు
	ఒకవేళ (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		మొదటి_meta_bg = fs->super->s_మొదటి_meta_bg;
		ఒకవేళ (మొదటి_meta_bg > fs->desc_ఖండాలు)
			మొదటి_meta_bg = fs->desc_ఖండాలు;
	} లేకపోతే
		మొదటి_meta_bg = fs->desc_ఖండాలు;
	ఒకవేళ (మొదటి_meta_bg) {
		retval = io_channel_read_blk(fs->io, సమూహం_ఖండం +
					     సమూహం_zero_adjust + 1,
					     మొదటి_meta_bg, dest);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
#నిర్వచితమైతే పదాలు_BIGENDIAN
		gdp = (నిర్మాణం ext2_సమూహం_desc *) dest;
		కొరకు (j=0; j < groups_చొప్పున_ఖండం*మొదటి_meta_bg; j++) {
			gdp = ext2fs_సమూహం_desc(fs, fs->సమూహం_desc, j);
			ఒకవేళ (gdp)
				ext2fs_మార్పిడి_సమూహం_desc2(fs, gdp);
		}
#అయితే_ముగింపు
		dest += fs->blocksize*మొదటి_meta_bg;
	}

	కొరకు (i = మొదటి_meta_bg ; i < fs->desc_ఖండాలు; i++) {
		blk = ext2fs_descriptor_ఖండం_loc2(fs, సమూహం_ఖండం, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	కొరకు (i=మొదటి_meta_bg ; i < fs->desc_ఖండాలు; i++) {
		blk = ext2fs_descriptor_ఖండం_loc2(fs, సమూహం_ఖండం, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
#నిర్వచితమైతే పదాలు_BIGENDIAN
		కొరకు (j=0; j < groups_చొప్పున_ఖండం; j++) {
			gdp = ext2fs_సమూహం_desc(fs, fs->సమూహం_desc,
						i * groups_చొప్పున_ఖండం + j);
			ఒకవేళ (gdp)
				ext2fs_మార్పిడి_సమూహం_desc2(fs, gdp);
		}
#అయితే_ముగింపు
		dest += fs->blocksize;
	}

	fs->stride = fs->super->s_raid_stride;

	/*
	 * If recovery is from backup superblock, Clear _UNININT flags &
	 * reset bg_itable_unused to zero
	 */
	ఒకవేళ (superblock > 1 && ext2fs_has_సమూహం_desc_csum(fs)) {
		dgrp_t సమూహం;

		కొరకు (సమూహం = 0; సమూహం < fs->సమూహం_desc_లెక్క; సమూహం++) {
			ext2fs_bg_flags_clear(fs, సమూహం, EXT2_BG_ఖండం_UNINIT);
			ext2fs_bg_flags_clear(fs, సమూహం, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_వాడని_సమితి(fs, సమూహం, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_సమూహం_desc_csum_సమితి(fs, సమూహం);
		}
		ఒకవేళ (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	ఒకవేళ (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_ప్రారంభం(fs);
		ఒకవేళ (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			వెళ్ళు cleanup;
		}
	}

	ఒకవేళ (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->ఖండం_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					ఖండం_sha_map_free_నమోదు, 4096);
		ఒకవేళ (!fs->ఖండం_sha_map) {
			retval = EXT2_ET_NO_జ్ఞాపకం;
			వెళ్ళు cleanup;
		}
		ext2fs_సమితి_feature_shared_ఖండాలు(fs->super);
	}

	ఒకవేళ (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_ఎక్కించు_nls_పట్టిక(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	తిరిగి 0;
cleanup:
	ఒకవేళ (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = ఖాళీ;
	}
	*ret_fs = fs;
	తిరిగి retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_దత్తాంశం_io(ext2_filsys fs, io_channel *old_io)
{
	ఒకవేళ ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		తిరిగి EXT2_ET_NOT_IMAGE_FILE;
	ఒకవేళ (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	తిరిగి 0;
}

errcode_t ext2fs_సమితి_దత్తాంశం_io(ext2_filsys fs, io_channel new_io)
{
	ఒకవేళ ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		తిరిగి EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	తిరిగి 0;
}

errcode_t ext2fs_rewrite_గమ్యం_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	ఒకవేళ ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		తిరిగి EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_సమితి_blksize(new_io, fs->blocksize);
	ఒకవేళ (err)
		తిరిగి err;
	ఒకవేళ ((new_io == fs->image_io) || (new_io == fs->io))
		తిరిగి 0;
	ఒకవేళ ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	ఒకవేళ (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	తిరిగి 0;
}
