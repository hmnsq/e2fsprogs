/*
 * initialize.c --- initialize a filesystem handle given superblock
 * 	parameters.  Used by mke2fs when initializing a filesystem.
 *
 * Copyright (C) 1994, 1995, 1996 Theodore Ts'o.
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

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ O_ದ್ವಿಮಾನ
#ವ್ಯಾಖ್ಯಾನಿಸು O_ದ್ವಿಮಾನ 0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#ವ್ಯಾಖ್ಯಾನಿಸು CREATOR_OS EXT2_OS_LINUX
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#ವ್ಯಾಖ್ಯಾನಿಸು CREATOR_OS EXT2_OS_HURD
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#ವ್ಯಾಖ್ಯಾನಿಸು CREATOR_OS EXT2_OS_FREEBSD
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#ವ್ಯಾಖ್ಯಾನಿಸು CREATOR_OS EXT2_OS_LITES
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CREATOR_OS EXT2_OS_LINUX /* by default */
#ಆದರೆ_ಅಂತ್ಯ /* defined(LITES) && defined(EXT2_OS_LITES) */
#ಆದರೆ_ಅಂತ್ಯ /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#ಆದರೆ_ಅಂತ್ಯ /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#ಆದರೆ_ಅಂತ್ಯ /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ calc_reserved_gdt_ಖಂಡಗಳು(ext2_filsys fs)
{
	ರಚನೆ ext2_super_ಖಂಡ *sb = fs->super;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ bpg = sb->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ gdpb = EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(sb);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_ಖಂಡಗಳು = 0xffffffff;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ rsv_groups;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	ಆದರೆ (ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(sb) < max_ಖಂಡಗಳು / 1024)
		max_ಖಂಡಗಳು = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_ಖಂಡಗಳು - sb->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_ಖಂಡಗಳು;
	ಆದರೆ (rsv_gdb > EXT2_ADDR_ಪ್ರತಿ_ಖಂಡ(sb))
		rsv_gdb = EXT2_ADDR_ಪ್ರತಿ_ಖಂಡ(sb);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
	ಬರೆ("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_ಖಂಡಗಳು, rsv_groups, rsv_gdb);
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ rsv_gdb;
}

errcode_t ext2fs_initialize(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags,
			    ರಚನೆ ext2_super_ಖಂಡ *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	ರಚನೆ ext2_super_ಖಂಡ *super;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	rem;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	overhead = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ipg;
	dgrp_t		i;
	blk64_t		free_ಖಂಡಗಳು;
	blk_t		numblocks;
	ಪೂರ್ಣಾಂಕ		rsv_gdt;
	ಪೂರ್ಣಾಂಕ		csum_flag;
	ಪೂರ್ಣಾಂಕ		bigalloc_flag;
	ಪೂರ್ಣಾಂಕ		io_flags;
	ಪೂರ್ಣಾಂಕ		has_bg;
	ಚಿಹ್ನೆರಹಿತ	reserved_inos;
	ಅಕ್ಷರ		*buf = 0;
	ಅಕ್ಷರ		c;
	ದ್ವಿಗುಣ		reserved_ಅನುಪಾತ;
	ಅಕ್ಷರ		*ಸಮಯ_env;

	ಆದರೆ (!param || !ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(param))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ struct_ext2_filsys), &fs);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	memset(fs, 0, ಗಾತ್ರ(ರಚನೆ struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	fs->flags |= EXT2_FLAG_ಅದಲುಬದಲು_ಬೈಟುಗಳು;
#ಆದರೆ_ಅಂತ್ಯ

	ಸಮಯ_env = ext2fs_ಸುರಕ್ಷಿತ_getenv("SOURCE_DATE_EPOCH");
	ಆದರೆ (ಸಮಯ_env) {
		fs->now = strtoul(ಸಮಯ_env, ಖಾಲಿ, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_ಸಮಯ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಸಮಯ_env = ext2fs_ಸುರಕ್ಷಿತ_getenv("E2FSPROGS_FAKE_TIME");
		ಆದರೆ (ಸಮಯ_env)
			fs->now = strtoul(ಸಮಯ_env, ಖಾಲಿ, 0);
	}

	io_flags = IO_FLAG_RW;
	ಆದರೆ (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	ಆದರೆ (flags & EXT2_FLAG_ನೇರ_IO)
		io_flags |= IO_FLAG_ನೇರ_IO;
	io_flags |= O_ದ್ವಿಮಾನ;
	retval = manager->open(name, io_flags, &fs->io);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	fs->image_io = fs->io;
	fs->io->app_ದತ್ತಾಂಶ = fs;
	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name)+1, &fs->device_name);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_ಗಾತ್ರ, &super);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_ಗಾತ್ರ);

#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_ಕ್ಷೇತ್ರ(ಕ್ಷೇತ್ರ, ಪೂರ್ವನಿಯೋಜಿತ) (super->ಕ್ಷೇತ್ರ = param->ಕ್ಷೇತ್ರ ? \
				   param->ಕ್ಷೇತ್ರ : (ಪೂರ್ವನಿಯೋಜಿತ))
#ವ್ಯಾಖ್ಯಾನಿಸು ನಿಯೋಜಿಸು_ಕ್ಷೇತ್ರ(ಕ್ಷೇತ್ರ)	(super->ಕ್ಷೇತ್ರ = param->ಕ್ಷೇತ್ರ)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	ನಿಯೋಜಿಸು_ಕ್ಷೇತ್ರ(s_log_ಖಂಡ_ಗಾತ್ರ);

	ಆದರೆ (bigalloc_flag) {
		ಗಣ_ಕ್ಷೇತ್ರ(s_log_cluster_ಗಾತ್ರ, super->s_log_ಖಂಡ_ಗಾತ್ರ+4);
		ಆದರೆ (super->s_log_ಖಂಡ_ಗಾತ್ರ > super->s_log_cluster_ಗಾತ್ರ) {
			retval = EXT2_ET_INVALID_ವಾದ;
			ಹೋಗು cleanup;
		}
	} ಇಲ್ಲದಿದ್ದರೆ
		super->s_log_cluster_ಗಾತ್ರ = super->s_log_ಖಂಡ_ಗಾತ್ರ;

	ಗಣ_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ, super->s_log_cluster_ಗಾತ್ರ ? 0 : 1);
	ಗಣ_ಕ್ಷೇತ್ರ(s_max_mnt_ಎಣಿಕೆ, 0);
	ಗಣ_ಕ್ಷೇತ್ರ(s_errors, EXT2_ERRORS_DEFAULT);
	ಗಣ_ಕ್ಷೇತ್ರ(s_feature_compat, 0);
	ಗಣ_ಕ್ಷೇತ್ರ(s_feature_incompat, 0);
	ಗಣ_ಕ್ಷೇತ್ರ(s_feature_ro_compat, 0);
	ಗಣ_ಕ್ಷೇತ್ರ(s_default_mount_opts, 0);
	ಗಣ_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_meta_bg, 0);
	ಗಣ_ಕ್ಷೇತ್ರ(s_raid_stride, 0);		/* default stride size: 0 */
	ಗಣ_ಕ್ಷೇತ್ರ(s_raid_stripe_ಅಗಲ, 0);	/* default stripe width: 0 */
	ಗಣ_ಕ್ಷೇತ್ರ(s_log_groups_ಪ್ರತಿ_flex, 0);
	ಗಣ_ಕ್ಷೇತ್ರ(s_flags, 0);
	ನಿಯೋಜಿಸು_ಕ್ಷೇತ್ರ(s_backup_bgs[0]);
	ನಿಯೋಜಿಸು_ಕ್ಷೇತ್ರ(s_backup_bgs[1]);

	ನಿಯೋಜಿಸು_ಕ್ಷೇತ್ರ(s_encoding);
	ನಿಯೋಜಿಸು_ಕ್ಷೇತ್ರ(s_encoding_flags);

	ಆದರೆ (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_ತುಂಬು_nls_ಕೋಷ್ಟಕ(param->s_encoding);

	ಆದರೆ (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		ಹೋಗು cleanup;
	}
	ಆದರೆ (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		ಹೋಗು cleanup;
	}

	ಗಣ_ಕ್ಷೇತ್ರ(s_rev_level, EXT2_GOOD_OLD_REV);
	ಆದರೆ (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		ಗಣ_ಕ್ಷೇತ್ರ(s_ಮೊದಲ_ino, EXT2_GOOD_OLD_ಮೊದಲ_INO);
		ಗಣ_ಕ್ಷೇತ್ರ(s_inode_ಗಾತ್ರ, EXT2_GOOD_OLD_INODE_ಗಾತ್ರ);
		ಆದರೆ (super->s_inode_ಗಾತ್ರ >= ಗಾತ್ರ(ರಚನೆ ext2_inode_large)) {
			ಪೂರ್ಣಾಂಕ extra_isize = ಗಾತ್ರ(ರಚನೆ ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_ಗಾತ್ರ;
			ಗಣ_ಕ್ಷೇತ್ರ(s_min_extra_isize, extra_isize);
			ಗಣ_ಕ್ಷೇತ್ರ(s_want_extra_isize, extra_isize);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		super->s_ಮೊದಲ_ino = EXT2_GOOD_OLD_ಮೊದಲ_INO;
		super->s_inode_ಗಾತ್ರ = EXT2_GOOD_OLD_INODE_ಗಾತ್ರ;
	}

	ಗಣ_ಕ್ಷೇತ್ರ(s_checkinterval, 0);
	ext2fs_ಗಣ_tstamp(super, s_mkfs_ಸಮಯ, ext2fs_P_get_ಸಮಯ(fs));
	ext2fs_ಗಣ_tstamp(super, s_lastcheck, ext2fs_P_get_ಸಮಯ(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_ಖಂಡ_ಗಾತ್ರ(super);
	fs->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು = super->s_log_cluster_ಗಾತ್ರ -
		super->s_log_ಖಂಡ_ಗಾತ್ರ;

	ಆದರೆ (bigalloc_flag) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ bpg;

		ಆದರೆ (param->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು &&
		    param->s_clusters_ಪ್ರತಿ_ಗುಂಪು &&
		    ((param->s_clusters_ಪ್ರತಿ_ಗುಂಪು * EXT2_FS_CLUSTER_ಅನುಪಾತ(fs)) !=
		     param->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು)) {
			retval = EXT2_ET_INVALID_ವಾದ;
			ಹೋಗು cleanup;
		}
		ಆದರೆ (param->s_clusters_ಪ್ರತಿ_ಗುಂಪು)
			ನಿಯೋಜಿಸು_ಕ್ಷೇತ್ರ(s_clusters_ಪ್ರತಿ_ಗುಂಪು);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (param->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು)
			super->s_clusters_ಪ್ರತಿ_ಗುಂಪು = 
				param->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು /
				EXT2_FS_CLUSTER_ಅನುಪಾತ(fs);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (super->s_log_cluster_ಗಾತ್ರ + 15 < 32)
			super->s_clusters_ಪ್ರತಿ_ಗುಂಪು = fs->blocksize * 8;
		ಇಲ್ಲದಿದ್ದರೆ
			super->s_clusters_ಪ್ರತಿ_ಗುಂಪು = (fs->blocksize - 1) * 8;
		ಆದರೆ (super->s_clusters_ಪ್ರತಿ_ಗುಂಪು > EXT2_MAX_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(super))
			super->s_clusters_ಪ್ರತಿ_ಗುಂಪು = EXT2_MAX_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(super);
		bpg = EXT2FS_C2B(fs,
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) super->s_clusters_ಪ್ರತಿ_ಗುಂಪು);
		ಆದರೆ (bpg >= (((ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) 1) << 32)) {
			retval = EXT2_ET_INVALID_ವಾದ;
			ಹೋಗು cleanup;
		}
		super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು = bpg;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗಣ_ಕ್ಷೇತ್ರ(s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು, fs->blocksize * 8);
		ಆದರೆ (super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು > EXT2_MAX_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(super))
			super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು = EXT2_MAX_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(super);
		super->s_clusters_ಪ್ರತಿ_ಗುಂಪು = super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
	}

	ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(super, ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(param) &
				~((blk64_t) EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs)));
	ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(super, ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ(param));
	ಆದರೆ (ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ(super) >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(param)) {
		retval = EXT2_ET_INVALID_ವಾದ;
		ಹೋಗು cleanup;
	}

	ಗಣ_ಕ್ಷೇತ್ರ(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	ಆದರೆ (ext2fs_has_feature_journal_dev(super)) {
		fs->ಗುಂಪು_desc_ಎಣಿಕೆ = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		ಹಿಂತಿರುಗಿ 0;
	}

retry:
	fs->ಗುಂಪು_desc_ಎಣಿಕೆ = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(super) - super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ,
		EXT2_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(super));
	ಆದರೆ (fs->ಗುಂಪು_desc_ಎಣಿಕೆ == 0) {
		retval = EXT2_ET_TOOSMALL;
		ಹೋಗು cleanup;
	}

	ಗಣ_ಕ್ಷೇತ್ರ(s_desc_ಗಾತ್ರ,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_ಗಾತ್ರ_64_ಬಿಟ್ : 0);

	fs->desc_ಖಂಡಗಳು = ext2fs_div_ceil(fs->ಗುಂಪು_desc_ಎಣಿಕೆ,
					  EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	ಆದರೆ (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(super) / i) >= (1ULL << 32))
		ಗಣ_ಕ್ಷೇತ್ರ(s_inodes_ಎಣಿಕೆ, ~0U);
	ಇಲ್ಲದಿದ್ದರೆ
		ಗಣ_ಕ್ಷೇತ್ರ(s_inodes_ಎಣಿಕೆ, ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_ಎಣಿಕೆ, fs->ಗುಂಪು_desc_ಎಣಿಕೆ);
	ಆದರೆ (ipg > fs->blocksize * 8) {
		ಆದರೆ (!bigalloc_flag && super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು >= 256) {
			/* Try again with slightly different parameters */
			super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು -= 8;
			ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(super,
						ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(param));
			super->s_clusters_ಪ್ರತಿ_ಗುಂಪು = super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
			ಹೋಗು retry;
		} ಇಲ್ಲದಿದ್ದರೆ {
			retval = EXT2_ET_TOO_MANY_INODES;
			ಹೋಗು cleanup;
		}
	}

	ಆದರೆ (ipg > (ಚಿಹ್ನೆರಹಿತ) EXT2_MAX_INODES_ಪ್ರತಿ_ಗುಂಪು(super))
		ipg = EXT2_MAX_INODES_ಪ್ರತಿ_ಗುಂಪು(super);

ipg_retry:
	super->s_inodes_ಪ್ರತಿ_ಗುಂಪು = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು = (((super->s_inodes_ಪ್ರತಿ_ಗುಂಪು *
					EXT2_INODE_ಗಾತ್ರ(super)) +
				       EXT2_ಖಂಡ_ಗಾತ್ರ(super) - 1) /
				      EXT2_ಖಂಡ_ಗಾತ್ರ(super));
	super->s_inodes_ಪ್ರತಿ_ಗುಂಪು = ((fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು *
				      EXT2_ಖಂಡ_ಗಾತ್ರ(super)) /
				     EXT2_INODE_ಗಾತ್ರ(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	ಆದರೆ (super->s_inodes_ಪ್ರತಿ_ಗುಂಪು < 8)
		super->s_inodes_ಪ್ರತಿ_ಗುಂಪು = 8;
	super->s_inodes_ಪ್ರತಿ_ಗುಂಪು &= ~7;
	fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು = (((super->s_inodes_ಪ್ರತಿ_ಗುಂಪು *
					EXT2_INODE_ಗಾತ್ರ(super)) +
				       EXT2_ಖಂಡ_ಗಾತ್ರ(super) - 1) /
				      EXT2_ಖಂಡ_ಗಾತ್ರ(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	ಆದರೆ ((__u64)super->s_inodes_ಪ್ರತಿ_ಗುಂಪು * fs->ಗುಂಪು_desc_ಎಣಿಕೆ > ~0U) {
		ipg--;
		ಹೋಗು ipg_retry;
	}
	super->s_inodes_ಎಣಿಕೆ = super->s_inodes_ಪ್ರತಿ_ಗುಂಪು *
		fs->ಗುಂಪು_desc_ಎಣಿಕೆ;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	ಆದರೆ (super->s_inodes_ಎಣಿಕೆ < EXT2_ಮೊದಲ_INODE(super)+1) {
		ipg += 8;
		ಹೋಗು ipg_retry;
	}

	super->s_free_inodes_ಎಣಿಕೆ = super->s_inodes_ಎಣಿಕೆ;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	ಆದರೆ (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_ಖಂಡಗಳು(fs);
	ಇಲ್ಲದಿದ್ದರೆ
		rsv_gdt = 0;
	ಗಣ_ಕ್ಷೇತ್ರ(s_reserved_gdt_ಖಂಡಗಳು, rsv_gdt);
	ಆದರೆ (super->s_reserved_gdt_ಖಂಡಗಳು > EXT2_ADDR_ಪ್ರತಿ_ಖಂಡ(super)) {
		retval = EXT2_ET_RES_GDT_ಖಂಡಗಳು;
		ಹೋಗು cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	ಆದರೆ (super->s_reserved_gdt_ಖಂಡಗಳು + fs->desc_ಖಂಡಗಳು >
	    super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು * 3 / 4) {
		ext2fs_ಗಣ_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		ಗಣ_ಕ್ಷೇತ್ರ(s_reserved_gdt_ಖಂಡಗಳು, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (ಪೂರ್ಣಾಂಕ) (3 + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು +
			  super->s_reserved_gdt_ಖಂಡಗಳು);

	ಆದರೆ (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	ಇಲ್ಲದಿದ್ದರೆ
		overhead += fs->desc_ಖಂಡಗಳು;

	/* This can only happen if the user requested too many inodes */
	ಆದರೆ (overhead > super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು) {
		retval = EXT2_ET_TOO_MANY_INODES;
		ಹೋಗು cleanup;
	}

	/*
	 * See if the last group is big enough to support the
	 * necessary data structures.  If not, we need to get rid of
	 * it.  We need to recalculate the overhead for the last block
	 * group, since it might or might not have a superblock
	 * backup.
	 */
	overhead = (ಪೂರ್ಣಾಂಕ) (2 + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು);
	has_bg = 0;
	ಆದರೆ (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		ಆದರೆ (fs->ಗುಂಪು_desc_ಎಣಿಕೆ == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		ಇಲ್ಲದಿದ್ದರೆ
			has_bg = param->s_backup_bgs[1] != 0;
	} ಇಲ್ಲದಿದ್ದರೆ
		has_bg = ext2fs_bg_has_super(fs, fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1);
	ಆದರೆ (has_bg)
		overhead += 1 + fs->desc_ಖಂಡಗಳು + super->s_reserved_gdt_ಖಂಡಗಳು;
	rem = ((ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(super) - super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) %
	       super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು);
	ಆದರೆ ((fs->ಗುಂಪು_desc_ಎಣಿಕೆ == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		ಹೋಗು cleanup;
	}
	ಆದರೆ (rem && (rem < overhead+50)) {
		ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(super, ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_ಅನುಪಾತ = 100.0 * ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ(param) /
			ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(param);
		ext2fs_r_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(super, reserved_ಅನುಪಾತ *
			ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(super) / 100.0);

		ಹೋಗು retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	ಆದರೆ (ext2fs_has_feature_sparse_super2(super)) {
		ಆದರೆ (super->s_backup_bgs[0] >= fs->ಗುಂಪು_desc_ಎಣಿಕೆ)
			super->s_backup_bgs[0] = fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1;
		ಆದರೆ (super->s_backup_bgs[1] >= fs->ಗುಂಪು_desc_ಎಣಿಕೆ)
			super->s_backup_bgs[1] = fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1;
		ಆದರೆ (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		ಆದರೆ (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(fs->device_name) + 80, &buf);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->ಖಂಡ_map);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_ಖಂಡಗಳು, fs->blocksize,
				&fs->ಗುಂಪು_desc);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	memset(fs->ಗುಂಪು_desc, 0, (size_t) fs->desc_ಖಂಡಗಳು * fs->blocksize);

	/*
	 * Reserve the superblock and group descriptors for each
	 * group, and fill in the correct group statistics for group.
	 * Note that although the block bitmap, inode bitmap, and
	 * inode table have not been allocated (and in fact won't be
	 * by this routine), they are accounted for nevertheless.
	 *
	 * If FLEX_BG meta-data grouping is used, only account for the
	 * superblock and group descriptors (the inode tables and
	 * bitmaps will be accounted for when allocated).
	 */
	free_ಖಂಡಗಳು = 0;
	csum_flag = ext2fs_has_ಗುಂಪು_desc_csum(fs);
	reserved_inos = super->s_ಮೊದಲ_ino;
	ಗಾಗಿ (i = 0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		blk_t grp_free_ಖಂಡಗಳು;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->ಖಂಡ_map,
						       &numblocks);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		ಆದರೆ (csum_flag) {
			ಆದರೆ (i != fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1 && numblocks == 0)
				ext2fs_bg_flags_ಗಣ(fs, i,
						    EXT2_BG_ಖಂಡ_UNINIT);
			ext2fs_bg_flags_ಗಣ(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_ಪ್ರತಿ_ಗುಂಪು;
			ಆದರೆ (reserved_inos) {
				ಆದರೆ (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} ಇಲ್ಲದಿದ್ದರೆ {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_ಬಳಸದ_ಗಣ(fs, i, inodes);
		}

		ಆದರೆ (!fs->super->s_log_groups_ಪ್ರತಿ_flex)
			numblocks += 2 + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;

		grp_free_ಖಂಡಗಳು = ext2fs_ಗುಂಪು_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, i) - numblocks;
		free_ಖಂಡಗಳು += grp_free_ಖಂಡಗಳು;
		ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs, i, grp_free_ಖಂಡಗಳು);
		ext2fs_bg_free_inodes_ಎಣಿಕೆ_ಗಣ(fs, i, fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು);
		ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_ಗಣ(fs, i, 0);
		ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, i);
	}
	free_ಖಂಡಗಳು &= ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
	ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(super, free_ಖಂಡಗಳು);

	c = (ಅಕ್ಷರ) 255;
	ಆದರೆ (((ಪೂರ್ಣಾಂಕ) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} ಇಲ್ಲದಿದ್ದರೆ {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_ಗಣ_blksize(fs->io, fs->blocksize);

	*ret_fs = fs;
	ಹಿಂತಿರುಗಿ 0;
cleanup:
	ಬಿಡುಗಡೆ(buf);
	ext2fs_free(fs);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, ಪೂರ್ಣಾಂಕ super_only)
{
	blk64_t		blk;
	ext2_ino_t	ino;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ಗುಂಪು = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ಎಣಿಕೆ_2 = 0;
	ಪೂರ್ಣಾಂಕ		ಒಟ್ಟು_free = 0;
	ಪೂರ್ಣಾಂಕ		ಗುಂಪು_free = 0;
	ಪೂರ್ಣಾಂಕ		ಕೊನೆಯ_allocated = 0;
	ಪೂರ್ಣಾಂಕ		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	ಗಾಗಿ (blk = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
	     blk < ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super); blk++) {
		ಆದರೆ (!ext2fs_fast_test_ಖಂಡ_bitmap2(fs->ಖಂಡ_map, blk)) {
			ಗುಂಪು_free++;
			ಒಟ್ಟು_free++;
		} ಇಲ್ಲದಿದ್ದರೆ {
			uninit = 0;
		}
		ಎಣಿಕೆ_2++;
		ಆದರೆ ((ಎಣಿಕೆ_2 == fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು) ||
		    (blk == ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)-1)) {
			ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs, ಗುಂಪು,
							ಗುಂಪು_free);
			ಆದರೆ (!super_only) {
				ಆದರೆ (uninit && blk !=
					ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) - 1)
					ext2fs_bg_flags_ಗಣ(fs, ಗುಂಪು,
							EXT2_BG_ಖಂಡ_UNINIT);
				ಇಲ್ಲದಿದ್ದರೆ
					ext2fs_bg_flags_clear(fs, ಗುಂಪು,
							EXT2_BG_ಖಂಡ_UNINIT);
			}
			ಎಣಿಕೆ_2 = 0;
			ಗುಂಪು_free = 0;
			uninit = 1;
			ಗುಂಪು++;
		}
	}
	ಒಟ್ಟು_free = EXT2FS_C2B(fs, ಒಟ್ಟು_free);
	ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs->super, ಒಟ್ಟು_free);

	/*
	 * Next, calculate the inode statistics
	 */
	ಗುಂಪು_free = 0;
	ಒಟ್ಟು_free = 0;
	ಕೊನೆಯ_allocated = 0;
	ಎಣಿಕೆ_2 = 0;
	ಗುಂಪು = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	ಗಾಗಿ (ino = 1; ino <= fs->super->s_inodes_ಎಣಿಕೆ && ino > 0; ino++) {
		ಆದರೆ (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			ಗುಂಪು_free++;
			ಒಟ್ಟು_free++;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಕೊನೆಯ_allocated = ino;
		}
		ಎಣಿಕೆ_2++;
		ಆದರೆ ((ಎಣಿಕೆ_2 == fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು) ||
		    (ino == fs->super->s_inodes_ಎಣಿಕೆ)) {
			ಆದರೆ (!super_only) {
				ಆದರೆ (ಕೊನೆಯ_allocated) {
					ext2fs_bg_flags_clear(fs, ಗುಂಪು,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_ಬಳಸದ_ಗಣ(fs, ಗುಂಪು,
						fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು -
						(ಕೊನೆಯ_allocated %
						fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು));
				} ಇಲ್ಲದಿದ್ದರೆ {
					ext2fs_bg_flags_ಗಣ(fs, ಗುಂಪು,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_ಬಳಸದ_ಗಣ(fs, ಗುಂಪು,
									0);
				}
				ext2fs_bg_free_inodes_ಎಣಿಕೆ_ಗಣ(fs, ಗುಂಪು,
								ಗುಂಪು_free);
			}
			ಗುಂಪು++;
			ಎಣಿಕೆ_2 = 0;
			ಗುಂಪು_free = 0;
			ಕೊನೆಯ_allocated = 0;
		}
	}
	fs->super->s_free_inodes_ಎಣಿಕೆ = ಒಟ್ಟು_free;
	ext2fs_mark_super_dirty(fs);
	ಹಿಂತಿರುಗಿ 0;
}
