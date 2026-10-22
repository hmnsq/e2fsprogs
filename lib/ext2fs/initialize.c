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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#جے_بنا_تعریف O_ثنائی
#تعریف_کرو O_ثنائی 0
#جے_انت

#إذا defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#تعریف_کرو CREATOR_OS EXT2_OS_LINUX
#وإلا
#إذا defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#تعریف_کرو CREATOR_OS EXT2_OS_HURD
#وإلا
#إذا defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#تعریف_کرو CREATOR_OS EXT2_OS_FREEBSD
#وإلا
#إذا defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#تعریف_کرو CREATOR_OS EXT2_OS_LITES
#وإلا
#تعریف_کرو CREATOR_OS EXT2_OS_LINUX /* by default */
#جے_انت /* defined(LITES) && defined(EXT2_OS_LITES) */
#جے_انت /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#جے_انت /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#جے_انت /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
ساكن بنانشانی صحيح calc_reserved_gdt_بلاکاں(ext2_filsys fs)
{
	بنية ext2_super_بلاک *sb = fs->super;
	بنانشانی لما bpg = sb->s_بلاکاں_فی_گروہ;
	بنانشانی صحيح gdpb = EXT2_DESC_فی_بلاک(sb);
	بنانشانی لما max_بلاکاں = 0xffffffff;
	بنانشانی لما rsv_groups;
	بنانشانی صحيح rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	إذا (ext2fs_بلاکاں_گنتی(sb) < max_بلاکاں / 1024)
		max_بلاکاں = ext2fs_بلاکاں_گنتی(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_بلاکاں - sb->s_پہلا_انکڑے_بلاک, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_بلاکاں;
	إذا (rsv_gdb > EXT2_ADDR_فی_بلاک(sb))
		rsv_gdb = EXT2_ADDR_فی_بلاک(sb);
#جے_تعریف_شدہ RES_GDT_DEBUG
	لکھو("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_بلاکاں, rsv_groups, rsv_gdb);
#جے_انت

	أرجع rsv_gdb;
}

errcode_t ext2fs_initialize(ثابت حرف *name, صحيح flags,
			    بنية ext2_super_بلاک *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	بنية ext2_super_بلاک *super;
	بنانشانی صحيح	rem;
	بنانشانی صحيح	overhead = 0;
	بنانشانی صحيح	ipg;
	dgrp_t		i;
	blk64_t		free_بلاکاں;
	blk_t		numblocks;
	صحيح		rsv_gdt;
	صحيح		csum_flag;
	صحيح		bigalloc_flag;
	صحيح		io_flags;
	صحيح		has_bg;
	بنانشانی	reserved_inos;
	حرف		*buf = 0;
	حرف		c;
	مزدوج		reserved_تناسب;
	حرف		*ویلا_env;

	إذا (!param || !ext2fs_بلاکاں_گنتی(param))
		أرجع EXT2_ET_INVALID_دلیل;

	retval = ext2fs_get_mem(حجم(بنية struct_ext2_filsys), &fs);
	إذا (retval)
		أرجع retval;

	memset(fs, 0, حجم(بنية struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	fs->flags |= EXT2_FLAG_وٹاندرا_بائٹاں;
#جے_انت

	ویلا_env = ext2fs_محفوظ_getenv("SOURCE_DATE_EPOCH");
	إذا (ویلا_env) {
		fs->now = strtoul(ویلا_env, عدم, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_ویلا;
	} وإلا {
		ویلا_env = ext2fs_محفوظ_getenv("E2FSPROGS_FAKE_TIME");
		إذا (ویلا_env)
			fs->now = strtoul(ویلا_env, عدم, 0);
	}

	io_flags = IO_FLAG_RW;
	إذا (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	إذا (flags & EXT2_FLAG_سدھا_IO)
		io_flags |= IO_FLAG_سدھا_IO;
	io_flags |= O_ثنائی;
	retval = manager->open(name, io_flags, &fs->io);
	إذا (retval)
		اذهب cleanup;
	fs->image_io = fs->io;
	fs->io->app_انکڑے = fs;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &fs->device_name);
	إذا (retval)
		اذهب cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_ناپ, &super);
	إذا (retval)
		اذهب cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_ناپ);

#تعریف_کرو سیٹ_میدان(میدان, افتراضي) (super->میدان = param->میدان ? \
				   param->میدان : (افتراضي))
#تعریف_کرو سونپو_میدان(میدان)	(super->میدان = param->میدان)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	سونپو_میدان(s_log_بلاک_ناپ);

	إذا (bigalloc_flag) {
		سیٹ_میدان(s_log_cluster_ناپ, super->s_log_بلاک_ناپ+4);
		إذا (super->s_log_بلاک_ناپ > super->s_log_cluster_ناپ) {
			retval = EXT2_ET_INVALID_دلیل;
			اذهب cleanup;
		}
	} وإلا
		super->s_log_cluster_ناپ = super->s_log_بلاک_ناپ;

	سیٹ_میدان(s_پہلا_انکڑے_بلاک, super->s_log_cluster_ناپ ? 0 : 1);
	سیٹ_میدان(s_max_mnt_گنتی, 0);
	سیٹ_میدان(s_errors, EXT2_ERRORS_DEFAULT);
	سیٹ_میدان(s_feature_compat, 0);
	سیٹ_میدان(s_feature_incompat, 0);
	سیٹ_میدان(s_feature_ro_compat, 0);
	سیٹ_میدان(s_default_mount_opts, 0);
	سیٹ_میدان(s_پہلا_meta_bg, 0);
	سیٹ_میدان(s_raid_stride, 0);		/* default stride size: 0 */
	سیٹ_میدان(s_raid_stripe_چوڑائی, 0);	/* default stripe width: 0 */
	سیٹ_میدان(s_log_groups_فی_flex, 0);
	سیٹ_میدان(s_flags, 0);
	سونپو_میدان(s_backup_bgs[0]);
	سونپو_میدان(s_backup_bgs[1]);

	سونپو_میدان(s_encoding);
	سونپو_میدان(s_encoding_flags);

	إذا (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_لدو_nls_جدول(param->s_encoding);

	إذا (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		اذهب cleanup;
	}
	إذا (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		اذهب cleanup;
	}

	سیٹ_میدان(s_rev_level, EXT2_GOOD_OLD_REV);
	إذا (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		سیٹ_میدان(s_پہلا_ino, EXT2_GOOD_OLD_پہلا_INO);
		سیٹ_میدان(s_inode_ناپ, EXT2_GOOD_OLD_INODE_ناپ);
		إذا (super->s_inode_ناپ >= حجم(بنية ext2_inode_large)) {
			صحيح extra_isize = حجم(بنية ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_ناپ;
			سیٹ_میدان(s_min_extra_isize, extra_isize);
			سیٹ_میدان(s_want_extra_isize, extra_isize);
		}
	} وإلا {
		super->s_پہلا_ino = EXT2_GOOD_OLD_پہلا_INO;
		super->s_inode_ناپ = EXT2_GOOD_OLD_INODE_ناپ;
	}

	سیٹ_میدان(s_checkinterval, 0);
	ext2fs_سیٹ_tstamp(super, s_mkfs_ویلا, ext2fs_P_get_ویلا(fs));
	ext2fs_سیٹ_tstamp(super, s_lastcheck, ext2fs_P_get_ویلا(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_بلاک_ناپ(super);
	fs->cluster_تناسب_بٹ = super->s_log_cluster_ناپ -
		super->s_log_بلاک_ناپ;

	إذا (bigalloc_flag) {
		بنانشانی لما لما bpg;

		إذا (param->s_بلاکاں_فی_گروہ &&
		    param->s_clusters_فی_گروہ &&
		    ((param->s_clusters_فی_گروہ * EXT2_FS_CLUSTER_تناسب(fs)) !=
		     param->s_بلاکاں_فی_گروہ)) {
			retval = EXT2_ET_INVALID_دلیل;
			اذهب cleanup;
		}
		إذا (param->s_clusters_فی_گروہ)
			سونپو_میدان(s_clusters_فی_گروہ);
		وإلا إذا (param->s_بلاکاں_فی_گروہ)
			super->s_clusters_فی_گروہ = 
				param->s_بلاکاں_فی_گروہ /
				EXT2_FS_CLUSTER_تناسب(fs);
		وإلا إذا (super->s_log_cluster_ناپ + 15 < 32)
			super->s_clusters_فی_گروہ = fs->blocksize * 8;
		وإلا
			super->s_clusters_فی_گروہ = (fs->blocksize - 1) * 8;
		إذا (super->s_clusters_فی_گروہ > EXT2_MAX_CLUSTERS_فی_گروہ(super))
			super->s_clusters_فی_گروہ = EXT2_MAX_CLUSTERS_فی_گروہ(super);
		bpg = EXT2FS_C2B(fs,
			(بنانشانی لما لما) super->s_clusters_فی_گروہ);
		إذا (bpg >= (((بنانشانی لما لما) 1) << 32)) {
			retval = EXT2_ET_INVALID_دلیل;
			اذهب cleanup;
		}
		super->s_بلاکاں_فی_گروہ = bpg;
	} وإلا {
		سیٹ_میدان(s_بلاکاں_فی_گروہ, fs->blocksize * 8);
		إذا (super->s_بلاکاں_فی_گروہ > EXT2_MAX_بلاکاں_فی_گروہ(super))
			super->s_بلاکاں_فی_گروہ = EXT2_MAX_بلاکاں_فی_گروہ(super);
		super->s_clusters_فی_گروہ = super->s_بلاکاں_فی_گروہ;
	}

	ext2fs_بلاکاں_گنتی_سیٹ(super, ext2fs_بلاکاں_گنتی(param) &
				~((blk64_t) EXT2_FS_CLUSTER_نقاب(fs)));
	ext2fs_r_بلاکاں_گنتی_سیٹ(super, ext2fs_r_بلاکاں_گنتی(param));
	إذا (ext2fs_r_بلاکاں_گنتی(super) >= ext2fs_بلاکاں_گنتی(param)) {
		retval = EXT2_ET_INVALID_دلیل;
		اذهب cleanup;
	}

	سیٹ_میدان(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	إذا (ext2fs_has_feature_journal_dev(super)) {
		fs->گروہ_desc_گنتی = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		أرجع 0;
	}

retry:
	fs->گروہ_desc_گنتی = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_بلاکاں_گنتی(super) - super->s_پہلا_انکڑے_بلاک,
		EXT2_بلاکاں_فی_گروہ(super));
	إذا (fs->گروہ_desc_گنتی == 0) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}

	سیٹ_میدان(s_desc_ناپ,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_ناپ_64_ثنائی_ہندسہ : 0);

	fs->desc_بلاکاں = ext2fs_div_ceil(fs->گروہ_desc_گنتی,
					  EXT2_DESC_فی_بلاک(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	إذا (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_بلاکاں_گنتی(super) / i) >= (1ULL << 32))
		سیٹ_میدان(s_inodes_گنتی, ~0U);
	وإلا
		سیٹ_میدان(s_inodes_گنتی, ext2fs_بلاکاں_گنتی(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_گنتی, fs->گروہ_desc_گنتی);
	إذا (ipg > fs->blocksize * 8) {
		إذا (!bigalloc_flag && super->s_بلاکاں_فی_گروہ >= 256) {
			/* Try again with slightly different parameters */
			super->s_بلاکاں_فی_گروہ -= 8;
			ext2fs_بلاکاں_گنتی_سیٹ(super,
						ext2fs_بلاکاں_گنتی(param));
			super->s_clusters_فی_گروہ = super->s_بلاکاں_فی_گروہ;
			اذهب retry;
		} وإلا {
			retval = EXT2_ET_TOO_MANY_INODES;
			اذهب cleanup;
		}
	}

	إذا (ipg > (بنانشانی) EXT2_MAX_INODES_فی_گروہ(super))
		ipg = EXT2_MAX_INODES_فی_گروہ(super);

ipg_retry:
	super->s_inodes_فی_گروہ = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_بلاکاں_فی_گروہ = (((super->s_inodes_فی_گروہ *
					EXT2_INODE_ناپ(super)) +
				       EXT2_بلاک_ناپ(super) - 1) /
				      EXT2_بلاک_ناپ(super));
	super->s_inodes_فی_گروہ = ((fs->inode_بلاکاں_فی_گروہ *
				      EXT2_بلاک_ناپ(super)) /
				     EXT2_INODE_ناپ(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	إذا (super->s_inodes_فی_گروہ < 8)
		super->s_inodes_فی_گروہ = 8;
	super->s_inodes_فی_گروہ &= ~7;
	fs->inode_بلاکاں_فی_گروہ = (((super->s_inodes_فی_گروہ *
					EXT2_INODE_ناپ(super)) +
				       EXT2_بلاک_ناپ(super) - 1) /
				      EXT2_بلاک_ناپ(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	إذا ((__u64)super->s_inodes_فی_گروہ * fs->گروہ_desc_گنتی > ~0U) {
		ipg--;
		اذهب ipg_retry;
	}
	super->s_inodes_گنتی = super->s_inodes_فی_گروہ *
		fs->گروہ_desc_گنتی;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	إذا (super->s_inodes_گنتی < EXT2_پہلا_INODE(super)+1) {
		ipg += 8;
		اذهب ipg_retry;
	}

	super->s_free_inodes_گنتی = super->s_inodes_گنتی;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	إذا (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_بلاکاں(fs);
	وإلا
		rsv_gdt = 0;
	سیٹ_میدان(s_reserved_gdt_بلاکاں, rsv_gdt);
	إذا (super->s_reserved_gdt_بلاکاں > EXT2_ADDR_فی_بلاک(super)) {
		retval = EXT2_ET_RES_GDT_بلاکاں;
		اذهب cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	إذا (super->s_reserved_gdt_بلاکاں + fs->desc_بلاکاں >
	    super->s_بلاکاں_فی_گروہ * 3 / 4) {
		ext2fs_سیٹ_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		سیٹ_میدان(s_reserved_gdt_بلاکاں, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (صحيح) (3 + fs->inode_بلاکاں_فی_گروہ +
			  super->s_reserved_gdt_بلاکاں);

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	وإلا
		overhead += fs->desc_بلاکاں;

	/* This can only happen if the user requested too many inodes */
	إذا (overhead > super->s_بلاکاں_فی_گروہ) {
		retval = EXT2_ET_TOO_MANY_INODES;
		اذهب cleanup;
	}

	/*
	 * See if the last group is big enough to support the
	 * necessary data structures.  If not, we need to get rid of
	 * it.  We need to recalculate the overhead for the last block
	 * group, since it might or might not have a superblock
	 * backup.
	 */
	overhead = (صحيح) (2 + fs->inode_بلاکاں_فی_گروہ);
	has_bg = 0;
	إذا (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		إذا (fs->گروہ_desc_گنتی == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		وإلا
			has_bg = param->s_backup_bgs[1] != 0;
	} وإلا
		has_bg = ext2fs_bg_has_super(fs, fs->گروہ_desc_گنتی - 1);
	إذا (has_bg)
		overhead += 1 + fs->desc_بلاکاں + super->s_reserved_gdt_بلاکاں;
	rem = ((ext2fs_بلاکاں_گنتی(super) - super->s_پہلا_انکڑے_بلاک) %
	       super->s_بلاکاں_فی_گروہ);
	إذا ((fs->گروہ_desc_گنتی == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}
	إذا (rem && (rem < overhead+50)) {
		ext2fs_بلاکاں_گنتی_سیٹ(super, ext2fs_بلاکاں_گنتی(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_تناسب = 100.0 * ext2fs_r_بلاکاں_گنتی(param) /
			ext2fs_بلاکاں_گنتی(param);
		ext2fs_r_بلاکاں_گنتی_سیٹ(super, reserved_تناسب *
			ext2fs_بلاکاں_گنتی(super) / 100.0);

		اذهب retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	إذا (ext2fs_has_feature_sparse_super2(super)) {
		إذا (super->s_backup_bgs[0] >= fs->گروہ_desc_گنتی)
			super->s_backup_bgs[0] = fs->گروہ_desc_گنتی - 1;
		إذا (super->s_backup_bgs[1] >= fs->گروہ_desc_گنتی)
			super->s_backup_bgs[1] = fs->گروہ_desc_گنتی - 1;
		إذا (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		إذا (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(لڑی_لمبائی(fs->device_name) + 80, &buf);
	إذا (retval)
		اذهب cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->بلاک_map);
	إذا (retval)
		اذهب cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	إذا (retval)
		اذهب cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_بلاکاں, fs->blocksize,
				&fs->گروہ_desc);
	إذا (retval)
		اذهب cleanup;

	memset(fs->گروہ_desc, 0, (size_t) fs->desc_بلاکاں * fs->blocksize);

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
	free_بلاکاں = 0;
	csum_flag = ext2fs_has_گروہ_desc_csum(fs);
	reserved_inos = super->s_پہلا_ino;
	لكل (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		blk_t grp_free_بلاکاں;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->بلاک_map,
						       &numblocks);
		إذا (retval)
			اذهب cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		إذا (csum_flag) {
			إذا (i != fs->گروہ_desc_گنتی - 1 && numblocks == 0)
				ext2fs_bg_flags_سیٹ(fs, i,
						    EXT2_BG_بلاک_UNINIT);
			ext2fs_bg_flags_سیٹ(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_فی_گروہ;
			إذا (reserved_inos) {
				إذا (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} وإلا {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_بن_ورتیا_سیٹ(fs, i, inodes);
		}

		إذا (!fs->super->s_log_groups_فی_flex)
			numblocks += 2 + fs->inode_بلاکاں_فی_گروہ;

		grp_free_بلاکاں = ext2fs_گروہ_بلاکاں_گنتی(fs, i) - numblocks;
		free_بلاکاں += grp_free_بلاکاں;
		ext2fs_bg_free_بلاکاں_گنتی_سیٹ(fs, i, grp_free_بلاکاں);
		ext2fs_bg_free_inodes_گنتی_سیٹ(fs, i, fs->super->s_inodes_فی_گروہ);
		ext2fs_bg_ورتیا_dirs_گنتی_سیٹ(fs, i, 0);
		ext2fs_گروہ_desc_csum_سیٹ(fs, i);
	}
	free_بلاکاں &= ~EXT2_FS_CLUSTER_نقاب(fs);
	ext2fs_free_بلاکاں_گنتی_سیٹ(super, free_بلاکاں);

	c = (حرف) 255;
	إذا (((صحيح) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} وإلا {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_سیٹ_blksize(fs->io, fs->blocksize);

	*ret_fs = fs;
	أرجع 0;
cleanup:
	حرر(buf);
	ext2fs_free(fs);
	أرجع retval;
}

errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, صحيح super_only)
{
	blk64_t		blk;
	ext2_ino_t	ino;
	بنانشانی صحيح	گروہ = 0;
	بنانشانی صحيح	گنتی = 0;
	صحيح		کل_free = 0;
	صحيح		گروہ_free = 0;
	صحيح		آخری_allocated = 0;
	صحيح		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	لكل (blk = fs->super->s_پہلا_انکڑے_بلاک;
	     blk < ext2fs_بلاکاں_گنتی(fs->super); blk++) {
		إذا (!ext2fs_fast_test_بلاک_bitmap2(fs->بلاک_map, blk)) {
			گروہ_free++;
			کل_free++;
		} وإلا {
			uninit = 0;
		}
		گنتی++;
		إذا ((گنتی == fs->super->s_بلاکاں_فی_گروہ) ||
		    (blk == ext2fs_بلاکاں_گنتی(fs->super)-1)) {
			ext2fs_bg_free_بلاکاں_گنتی_سیٹ(fs, گروہ,
							گروہ_free);
			إذا (!super_only) {
				إذا (uninit && blk !=
					ext2fs_بلاکاں_گنتی(fs->super) - 1)
					ext2fs_bg_flags_سیٹ(fs, گروہ,
							EXT2_BG_بلاک_UNINIT);
				وإلا
					ext2fs_bg_flags_clear(fs, گروہ,
							EXT2_BG_بلاک_UNINIT);
			}
			گنتی = 0;
			گروہ_free = 0;
			uninit = 1;
			گروہ++;
		}
	}
	کل_free = EXT2FS_C2B(fs, کل_free);
	ext2fs_free_بلاکاں_گنتی_سیٹ(fs->super, کل_free);

	/*
	 * Next, calculate the inode statistics
	 */
	گروہ_free = 0;
	کل_free = 0;
	آخری_allocated = 0;
	گنتی = 0;
	گروہ = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	لكل (ino = 1; ino <= fs->super->s_inodes_گنتی && ino > 0; ino++) {
		إذا (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			گروہ_free++;
			کل_free++;
		} وإلا {
			آخری_allocated = ino;
		}
		گنتی++;
		إذا ((گنتی == fs->super->s_inodes_فی_گروہ) ||
		    (ino == fs->super->s_inodes_گنتی)) {
			إذا (!super_only) {
				إذا (آخری_allocated) {
					ext2fs_bg_flags_clear(fs, گروہ,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_بن_ورتیا_سیٹ(fs, گروہ,
						fs->super->s_inodes_فی_گروہ -
						(آخری_allocated %
						fs->super->s_inodes_فی_گروہ));
				} وإلا {
					ext2fs_bg_flags_سیٹ(fs, گروہ,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_بن_ورتیا_سیٹ(fs, گروہ,
									0);
				}
				ext2fs_bg_free_inodes_گنتی_سیٹ(fs, گروہ,
								گروہ_free);
			}
			گروہ++;
			گنتی = 0;
			گروہ_free = 0;
			آخری_allocated = 0;
		}
	}
	fs->super->s_free_inodes_گنتی = کل_free;
	ext2fs_mark_super_dirty(fs);
	أرجع 0;
}
