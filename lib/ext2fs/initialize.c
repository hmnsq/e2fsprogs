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

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ O_ਦੁਆਧਾਰੀ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ O_ਦੁਆਧਾਰੀ 0
#ਜੇ_ਅੰਤ

#ਜੇ defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CREATOR_OS EXT2_OS_LINUX
#ਨਹੀਂਤਾਂ
#ਜੇ defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CREATOR_OS EXT2_OS_HURD
#ਨਹੀਂਤਾਂ
#ਜੇ defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CREATOR_OS EXT2_OS_FREEBSD
#ਨਹੀਂਤਾਂ
#ਜੇ defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CREATOR_OS EXT2_OS_LITES
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CREATOR_OS EXT2_OS_LINUX /* by default */
#ਜੇ_ਅੰਤ /* defined(LITES) && defined(EXT2_OS_LITES) */
#ਜੇ_ਅੰਤ /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#ਜੇ_ਅੰਤ /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#ਜੇ_ਅੰਤ /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ calc_reserved_gdt_ਖੰਡਾਂ(ext2_filsys fs)
{
	ਬਣਤਰ ext2_super_ਖੰਡ *sb = fs->super;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ bpg = sb->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ gdpb = EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(sb);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_ਖੰਡਾਂ = 0xffffffff;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ rsv_groups;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	ਜੇ (ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(sb) < max_ਖੰਡਾਂ / 1024)
		max_ਖੰਡਾਂ = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_ਖੰਡਾਂ - sb->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_ਖੰਡਾਂ;
	ਜੇ (rsv_gdb > EXT2_ADDR_ਪ੍ਰਤੀ_ਖੰਡ(sb))
		rsv_gdb = EXT2_ADDR_ਪ੍ਰਤੀ_ਖੰਡ(sb);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
	ਲਿਖੋ("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_ਖੰਡਾਂ, rsv_groups, rsv_gdb);
#ਜੇ_ਅੰਤ

	ਵਾਪਸ rsv_gdb;
}

errcode_t ext2fs_initialize(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags,
			    ਬਣਤਰ ext2_super_ਖੰਡ *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	ਬਣਤਰ ext2_super_ਖੰਡ *super;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	rem;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	overhead = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ipg;
	dgrp_t		i;
	blk64_t		free_ਖੰਡਾਂ;
	blk_t		numblocks;
	ਪੂਰਨ		rsv_gdt;
	ਪੂਰਨ		csum_flag;
	ਪੂਰਨ		bigalloc_flag;
	ਪੂਰਨ		io_flags;
	ਪੂਰਨ		has_bg;
	ਬਿਨਾਨਿਸ਼ਾਨੀ	reserved_inos;
	ਅੱਖਰ		*buf = 0;
	ਅੱਖਰ		c;
	ਦੁੱਗਣਾ		reserved_ਅਨੁਪਾਤ;
	ਅੱਖਰ		*ਸਮਾਂ_env;

	ਜੇ (!param || !ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(param))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ struct_ext2_filsys), &fs);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	memset(fs, 0, ਆਕਾਰ(ਬਣਤਰ struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	fs->flags |= EXT2_FLAG_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ;
#ਜੇ_ਅੰਤ

	ਸਮਾਂ_env = ext2fs_ਸੁਰੱਖਿਅਤ_getenv("SOURCE_DATE_EPOCH");
	ਜੇ (ਸਮਾਂ_env) {
		fs->now = strtoul(ਸਮਾਂ_env, ਨਿਰਾ, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_ਸਮਾਂ;
	} ਨਹੀਂਤਾਂ {
		ਸਮਾਂ_env = ext2fs_ਸੁਰੱਖਿਅਤ_getenv("E2FSPROGS_FAKE_TIME");
		ਜੇ (ਸਮਾਂ_env)
			fs->now = strtoul(ਸਮਾਂ_env, ਨਿਰਾ, 0);
	}

	io_flags = IO_FLAG_RW;
	ਜੇ (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	ਜੇ (flags & EXT2_FLAG_ਸਿੱਧਾ_IO)
		io_flags |= IO_FLAG_ਸਿੱਧਾ_IO;
	io_flags |= O_ਦੁਆਧਾਰੀ;
	retval = manager->open(name, io_flags, &fs->io);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	fs->image_io = fs->io;
	fs->io->app_ਅੰਕੜੇ = fs;
	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(name)+1, &fs->device_name);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_ਮਾਪ, &super);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_ਮਾਪ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_ਖੇਤਰ(ਖੇਤਰ, ਮੂਲ) (super->ਖੇਤਰ = param->ਖੇਤਰ ? \
				   param->ਖੇਤਰ : (ਮੂਲ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੌਂਪੋ_ਖੇਤਰ(ਖੇਤਰ)	(super->ਖੇਤਰ = param->ਖੇਤਰ)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	ਸੌਂਪੋ_ਖੇਤਰ(s_log_ਖੰਡ_ਮਾਪ);

	ਜੇ (bigalloc_flag) {
		ਸੈੱਟ_ਖੇਤਰ(s_log_cluster_ਮਾਪ, super->s_log_ਖੰਡ_ਮਾਪ+4);
		ਜੇ (super->s_log_ਖੰਡ_ਮਾਪ > super->s_log_cluster_ਮਾਪ) {
			retval = EXT2_ET_INVALID_ਦਲੀਲ;
			ਜਾਓ cleanup;
		}
	} ਨਹੀਂਤਾਂ
		super->s_log_cluster_ਮਾਪ = super->s_log_ਖੰਡ_ਮਾਪ;

	ਸੈੱਟ_ਖੇਤਰ(s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ, super->s_log_cluster_ਮਾਪ ? 0 : 1);
	ਸੈੱਟ_ਖੇਤਰ(s_max_mnt_ਗਿਣਤੀ, 0);
	ਸੈੱਟ_ਖੇਤਰ(s_errors, EXT2_ERRORS_DEFAULT);
	ਸੈੱਟ_ਖੇਤਰ(s_feature_compat, 0);
	ਸੈੱਟ_ਖੇਤਰ(s_feature_incompat, 0);
	ਸੈੱਟ_ਖੇਤਰ(s_feature_ro_compat, 0);
	ਸੈੱਟ_ਖੇਤਰ(s_default_mount_opts, 0);
	ਸੈੱਟ_ਖੇਤਰ(s_ਪਹਿਲਾ_meta_bg, 0);
	ਸੈੱਟ_ਖੇਤਰ(s_raid_stride, 0);		/* default stride size: 0 */
	ਸੈੱਟ_ਖੇਤਰ(s_raid_stripe_ਚੌੜਾਈ, 0);	/* default stripe width: 0 */
	ਸੈੱਟ_ਖੇਤਰ(s_log_groups_ਪ੍ਰਤੀ_flex, 0);
	ਸੈੱਟ_ਖੇਤਰ(s_flags, 0);
	ਸੌਂਪੋ_ਖੇਤਰ(s_backup_bgs[0]);
	ਸੌਂਪੋ_ਖੇਤਰ(s_backup_bgs[1]);

	ਸੌਂਪੋ_ਖੇਤਰ(s_encoding);
	ਸੌਂਪੋ_ਖੇਤਰ(s_encoding_flags);

	ਜੇ (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_ਲੱਦੋ_nls_ਸਾਰਣੀ(param->s_encoding);

	ਜੇ (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		ਜਾਓ cleanup;
	}
	ਜੇ (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		ਜਾਓ cleanup;
	}

	ਸੈੱਟ_ਖੇਤਰ(s_rev_level, EXT2_GOOD_OLD_REV);
	ਜੇ (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		ਸੈੱਟ_ਖੇਤਰ(s_ਪਹਿਲਾ_ino, EXT2_GOOD_OLD_ਪਹਿਲਾ_INO);
		ਸੈੱਟ_ਖੇਤਰ(s_inode_ਮਾਪ, EXT2_GOOD_OLD_INODE_ਮਾਪ);
		ਜੇ (super->s_inode_ਮਾਪ >= ਆਕਾਰ(ਬਣਤਰ ext2_inode_large)) {
			ਪੂਰਨ extra_isize = ਆਕਾਰ(ਬਣਤਰ ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_ਮਾਪ;
			ਸੈੱਟ_ਖੇਤਰ(s_min_extra_isize, extra_isize);
			ਸੈੱਟ_ਖੇਤਰ(s_want_extra_isize, extra_isize);
		}
	} ਨਹੀਂਤਾਂ {
		super->s_ਪਹਿਲਾ_ino = EXT2_GOOD_OLD_ਪਹਿਲਾ_INO;
		super->s_inode_ਮਾਪ = EXT2_GOOD_OLD_INODE_ਮਾਪ;
	}

	ਸੈੱਟ_ਖੇਤਰ(s_checkinterval, 0);
	ext2fs_ਸੈੱਟ_tstamp(super, s_mkfs_ਸਮਾਂ, ext2fs_P_get_ਸਮਾਂ(fs));
	ext2fs_ਸੈੱਟ_tstamp(super, s_lastcheck, ext2fs_P_get_ਸਮਾਂ(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_ਖੰਡ_ਮਾਪ(super);
	fs->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ = super->s_log_cluster_ਮਾਪ -
		super->s_log_ਖੰਡ_ਮਾਪ;

	ਜੇ (bigalloc_flag) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ bpg;

		ਜੇ (param->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ &&
		    param->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ &&
		    ((param->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ * EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs)) !=
		     param->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ)) {
			retval = EXT2_ET_INVALID_ਦਲੀਲ;
			ਜਾਓ cleanup;
		}
		ਜੇ (param->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ)
			ਸੌਂਪੋ_ਖੇਤਰ(s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ);
		ਨਹੀਂਤਾਂ ਜੇ (param->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ)
			super->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ = 
				param->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ /
				EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs);
		ਨਹੀਂਤਾਂ ਜੇ (super->s_log_cluster_ਮਾਪ + 15 < 32)
			super->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ = fs->blocksize * 8;
		ਨਹੀਂਤਾਂ
			super->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ = (fs->blocksize - 1) * 8;
		ਜੇ (super->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ > EXT2_MAX_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(super))
			super->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ = EXT2_MAX_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(super);
		bpg = EXT2FS_C2B(fs,
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) super->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ);
		ਜੇ (bpg >= (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) 1) << 32)) {
			retval = EXT2_ET_INVALID_ਦਲੀਲ;
			ਜਾਓ cleanup;
		}
		super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ = bpg;
	} ਨਹੀਂਤਾਂ {
		ਸੈੱਟ_ਖੇਤਰ(s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ, fs->blocksize * 8);
		ਜੇ (super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ > EXT2_MAX_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(super))
			super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ = EXT2_MAX_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(super);
		super->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ = super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
	}

	ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(super, ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(param) &
				~((blk64_t) EXT2_FS_CLUSTER_ਨਕਾਬ(fs)));
	ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(super, ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ(param));
	ਜੇ (ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ(super) >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(param)) {
		retval = EXT2_ET_INVALID_ਦਲੀਲ;
		ਜਾਓ cleanup;
	}

	ਸੈੱਟ_ਖੇਤਰ(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	ਜੇ (ext2fs_has_feature_journal_dev(super)) {
		fs->ਸਮੂਹ_desc_ਗਿਣਤੀ = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		ਵਾਪਸ 0;
	}

retry:
	fs->ਸਮੂਹ_desc_ਗਿਣਤੀ = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(super) - super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ,
		EXT2_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(super));
	ਜੇ (fs->ਸਮੂਹ_desc_ਗਿਣਤੀ == 0) {
		retval = EXT2_ET_TOOSMALL;
		ਜਾਓ cleanup;
	}

	ਸੈੱਟ_ਖੇਤਰ(s_desc_ਮਾਪ,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_ਮਾਪ_64_ਦੋਆਧਾਰੀ_ਅੰਕ : 0);

	fs->desc_ਖੰਡਾਂ = ext2fs_div_ceil(fs->ਸਮੂਹ_desc_ਗਿਣਤੀ,
					  EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	ਜੇ (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(super) / i) >= (1ULL << 32))
		ਸੈੱਟ_ਖੇਤਰ(s_inodes_ਗਿਣਤੀ, ~0U);
	ਨਹੀਂਤਾਂ
		ਸੈੱਟ_ਖੇਤਰ(s_inodes_ਗਿਣਤੀ, ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_ਗਿਣਤੀ, fs->ਸਮੂਹ_desc_ਗਿਣਤੀ);
	ਜੇ (ipg > fs->blocksize * 8) {
		ਜੇ (!bigalloc_flag && super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ >= 256) {
			/* Try again with slightly different parameters */
			super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ -= 8;
			ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(super,
						ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(param));
			super->s_clusters_ਪ੍ਰਤੀ_ਸਮੂਹ = super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
			ਜਾਓ retry;
		} ਨਹੀਂਤਾਂ {
			retval = EXT2_ET_TOO_MANY_INODES;
			ਜਾਓ cleanup;
		}
	}

	ਜੇ (ipg > (ਬਿਨਾਨਿਸ਼ਾਨੀ) EXT2_MAX_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(super))
		ipg = EXT2_MAX_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(super);

ipg_retry:
	super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ = (((super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ *
					EXT2_INODE_ਮਾਪ(super)) +
				       EXT2_ਖੰਡ_ਮਾਪ(super) - 1) /
				      EXT2_ਖੰਡ_ਮਾਪ(super));
	super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ = ((fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ *
				      EXT2_ਖੰਡ_ਮਾਪ(super)) /
				     EXT2_INODE_ਮਾਪ(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	ਜੇ (super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ < 8)
		super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ = 8;
	super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ &= ~7;
	fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ = (((super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ *
					EXT2_INODE_ਮਾਪ(super)) +
				       EXT2_ਖੰਡ_ਮਾਪ(super) - 1) /
				      EXT2_ਖੰਡ_ਮਾਪ(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	ਜੇ ((__u64)super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ * fs->ਸਮੂਹ_desc_ਗਿਣਤੀ > ~0U) {
		ipg--;
		ਜਾਓ ipg_retry;
	}
	super->s_inodes_ਗਿਣਤੀ = super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ *
		fs->ਸਮੂਹ_desc_ਗਿਣਤੀ;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	ਜੇ (super->s_inodes_ਗਿਣਤੀ < EXT2_ਪਹਿਲਾ_INODE(super)+1) {
		ipg += 8;
		ਜਾਓ ipg_retry;
	}

	super->s_free_inodes_ਗਿਣਤੀ = super->s_inodes_ਗਿਣਤੀ;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	ਜੇ (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_ਖੰਡਾਂ(fs);
	ਨਹੀਂਤਾਂ
		rsv_gdt = 0;
	ਸੈੱਟ_ਖੇਤਰ(s_reserved_gdt_ਖੰਡਾਂ, rsv_gdt);
	ਜੇ (super->s_reserved_gdt_ਖੰਡਾਂ > EXT2_ADDR_ਪ੍ਰਤੀ_ਖੰਡ(super)) {
		retval = EXT2_ET_RES_GDT_ਖੰਡਾਂ;
		ਜਾਓ cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	ਜੇ (super->s_reserved_gdt_ਖੰਡਾਂ + fs->desc_ਖੰਡਾਂ >
	    super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ * 3 / 4) {
		ext2fs_ਸੈੱਟ_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		ਸੈੱਟ_ਖੇਤਰ(s_reserved_gdt_ਖੰਡਾਂ, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (ਪੂਰਨ) (3 + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ +
			  super->s_reserved_gdt_ਖੰਡਾਂ);

	ਜੇ (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	ਨਹੀਂਤਾਂ
		overhead += fs->desc_ਖੰਡਾਂ;

	/* This can only happen if the user requested too many inodes */
	ਜੇ (overhead > super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ) {
		retval = EXT2_ET_TOO_MANY_INODES;
		ਜਾਓ cleanup;
	}

	/*
	 * See if the last group is big enough to support the
	 * necessary data structures.  If not, we need to get rid of
	 * it.  We need to recalculate the overhead for the last block
	 * group, since it might or might not have a superblock
	 * backup.
	 */
	overhead = (ਪੂਰਨ) (2 + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ);
	has_bg = 0;
	ਜੇ (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		ਜੇ (fs->ਸਮੂਹ_desc_ਗਿਣਤੀ == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		ਨਹੀਂਤਾਂ
			has_bg = param->s_backup_bgs[1] != 0;
	} ਨਹੀਂਤਾਂ
		has_bg = ext2fs_bg_has_super(fs, fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1);
	ਜੇ (has_bg)
		overhead += 1 + fs->desc_ਖੰਡਾਂ + super->s_reserved_gdt_ਖੰਡਾਂ;
	rem = ((ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(super) - super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) %
	       super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ);
	ਜੇ ((fs->ਸਮੂਹ_desc_ਗਿਣਤੀ == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		ਜਾਓ cleanup;
	}
	ਜੇ (rem && (rem < overhead+50)) {
		ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(super, ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_ਅਨੁਪਾਤ = 100.0 * ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ(param) /
			ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(param);
		ext2fs_r_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(super, reserved_ਅਨੁਪਾਤ *
			ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(super) / 100.0);

		ਜਾਓ retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	ਜੇ (ext2fs_has_feature_sparse_super2(super)) {
		ਜੇ (super->s_backup_bgs[0] >= fs->ਸਮੂਹ_desc_ਗਿਣਤੀ)
			super->s_backup_bgs[0] = fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1;
		ਜੇ (super->s_backup_bgs[1] >= fs->ਸਮੂਹ_desc_ਗਿਣਤੀ)
			super->s_backup_bgs[1] = fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1;
		ਜੇ (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		ਜੇ (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(fs->device_name) + 80, &buf);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->ਖੰਡ_map);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_ਖੰਡਾਂ, fs->blocksize,
				&fs->ਸਮੂਹ_desc);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	memset(fs->ਸਮੂਹ_desc, 0, (size_t) fs->desc_ਖੰਡਾਂ * fs->blocksize);

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
	free_ਖੰਡਾਂ = 0;
	csum_flag = ext2fs_has_ਸਮੂਹ_desc_csum(fs);
	reserved_inos = super->s_ਪਹਿਲਾ_ino;
	ਲਈ (i = 0; i < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		blk_t grp_free_ਖੰਡਾਂ;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->ਖੰਡ_map,
						       &numblocks);
		ਜੇ (retval)
			ਜਾਓ cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		ਜੇ (csum_flag) {
			ਜੇ (i != fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1 && numblocks == 0)
				ext2fs_bg_flags_ਸੈੱਟ(fs, i,
						    EXT2_BG_ਖੰਡ_UNINIT);
			ext2fs_bg_flags_ਸੈੱਟ(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ;
			ਜੇ (reserved_inos) {
				ਜੇ (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} ਨਹੀਂਤਾਂ {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_ਅਣਵਰਤਿਆ_ਸੈੱਟ(fs, i, inodes);
		}

		ਜੇ (!fs->super->s_log_groups_ਪ੍ਰਤੀ_flex)
			numblocks += 2 + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;

		grp_free_ਖੰਡਾਂ = ext2fs_ਸਮੂਹ_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, i) - numblocks;
		free_ਖੰਡਾਂ += grp_free_ਖੰਡਾਂ;
		ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs, i, grp_free_ਖੰਡਾਂ);
		ext2fs_bg_free_inodes_ਗਿਣਤੀ_ਸੈੱਟ(fs, i, fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ);
		ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_ਸੈੱਟ(fs, i, 0);
		ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, i);
	}
	free_ਖੰਡਾਂ &= ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
	ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(super, free_ਖੰਡਾਂ);

	c = (ਅੱਖਰ) 255;
	ਜੇ (((ਪੂਰਨ) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} ਨਹੀਂਤਾਂ {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_ਸੈੱਟ_blksize(fs->io, fs->blocksize);

	*ret_fs = fs;
	ਵਾਪਸ 0;
cleanup:
	ਮੁਕਤ(buf);
	ext2fs_free(fs);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, ਪੂਰਨ super_only)
{
	blk64_t		blk;
	ext2_ino_t	ino;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਸਮੂਹ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਗਿਣਤੀ_2 = 0;
	ਪੂਰਨ		ਕੁੱਲ_free = 0;
	ਪੂਰਨ		ਸਮੂਹ_free = 0;
	ਪੂਰਨ		ਆਖਰੀ_allocated = 0;
	ਪੂਰਨ		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	ਲਈ (blk = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
	     blk < ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super); blk++) {
		ਜੇ (!ext2fs_fast_test_ਖੰਡ_bitmap2(fs->ਖੰਡ_map, blk)) {
			ਸਮੂਹ_free++;
			ਕੁੱਲ_free++;
		} ਨਹੀਂਤਾਂ {
			uninit = 0;
		}
		ਗਿਣਤੀ_2++;
		ਜੇ ((ਗਿਣਤੀ_2 == fs->super->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ) ||
		    (blk == ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)-1)) {
			ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs, ਸਮੂਹ,
							ਸਮੂਹ_free);
			ਜੇ (!super_only) {
				ਜੇ (uninit && blk !=
					ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) - 1)
					ext2fs_bg_flags_ਸੈੱਟ(fs, ਸਮੂਹ,
							EXT2_BG_ਖੰਡ_UNINIT);
				ਨਹੀਂਤਾਂ
					ext2fs_bg_flags_clear(fs, ਸਮੂਹ,
							EXT2_BG_ਖੰਡ_UNINIT);
			}
			ਗਿਣਤੀ_2 = 0;
			ਸਮੂਹ_free = 0;
			uninit = 1;
			ਸਮੂਹ++;
		}
	}
	ਕੁੱਲ_free = EXT2FS_C2B(fs, ਕੁੱਲ_free);
	ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs->super, ਕੁੱਲ_free);

	/*
	 * Next, calculate the inode statistics
	 */
	ਸਮੂਹ_free = 0;
	ਕੁੱਲ_free = 0;
	ਆਖਰੀ_allocated = 0;
	ਗਿਣਤੀ_2 = 0;
	ਸਮੂਹ = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	ਲਈ (ino = 1; ino <= fs->super->s_inodes_ਗਿਣਤੀ && ino > 0; ino++) {
		ਜੇ (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			ਸਮੂਹ_free++;
			ਕੁੱਲ_free++;
		} ਨਹੀਂਤਾਂ {
			ਆਖਰੀ_allocated = ino;
		}
		ਗਿਣਤੀ_2++;
		ਜੇ ((ਗਿਣਤੀ_2 == fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ) ||
		    (ino == fs->super->s_inodes_ਗਿਣਤੀ)) {
			ਜੇ (!super_only) {
				ਜੇ (ਆਖਰੀ_allocated) {
					ext2fs_bg_flags_clear(fs, ਸਮੂਹ,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_ਅਣਵਰਤਿਆ_ਸੈੱਟ(fs, ਸਮੂਹ,
						fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ -
						(ਆਖਰੀ_allocated %
						fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ));
				} ਨਹੀਂਤਾਂ {
					ext2fs_bg_flags_ਸੈੱਟ(fs, ਸਮੂਹ,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_ਅਣਵਰਤਿਆ_ਸੈੱਟ(fs, ਸਮੂਹ,
									0);
				}
				ext2fs_bg_free_inodes_ਗਿਣਤੀ_ਸੈੱਟ(fs, ਸਮੂਹ,
								ਸਮੂਹ_free);
			}
			ਸਮੂਹ++;
			ਗਿਣਤੀ_2 = 0;
			ਸਮੂਹ_free = 0;
			ਆਖਰੀ_allocated = 0;
		}
	}
	fs->super->s_free_inodes_ਗਿਣਤੀ = ਕੁੱਲ_free;
	ext2fs_mark_super_dirty(fs);
	ਵਾਪਸ 0;
}
