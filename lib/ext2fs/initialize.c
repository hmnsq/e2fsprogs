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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#جيڪڏهن_وصف_نه_ٿيل O_ٻٽو
#وصف_ڪريو O_ٻٽو 0
#جيڪڏهن_پورو

#إذا defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#وصف_ڪريو CREATOR_OS EXT2_OS_LINUX
#وإلا
#إذا defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#وصف_ڪريو CREATOR_OS EXT2_OS_HURD
#وإلا
#إذا defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#وصف_ڪريو CREATOR_OS EXT2_OS_FREEBSD
#وإلا
#إذا defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#وصف_ڪريو CREATOR_OS EXT2_OS_LITES
#وإلا
#وصف_ڪريو CREATOR_OS EXT2_OS_LINUX /* by default */
#جيڪڏهن_پورو /* defined(LITES) && defined(EXT2_OS_LITES) */
#جيڪڏهن_پورو /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#جيڪڏهن_پورو /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#جيڪڏهن_پورو /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
ساكن بنانشان صحيح calc_reserved_gdt_بلاڪون(ext2_filsys fs)
{
	بنية ext2_super_بلاڪ *sb = fs->super;
	بنانشان ڊگهو bpg = sb->s_بلاڪون_في_گروهه;
	بنانشان صحيح gdpb = EXT2_DESC_في_بلاڪ(sb);
	بنانشان ڊگهو max_بلاڪون = 0xffffffff;
	بنانشان ڊگهو rsv_groups;
	بنانشان صحيح rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	إذا (ext2fs_بلاڪون_ڳڻپ(sb) < max_بلاڪون / 1024)
		max_بلاڪون = ext2fs_بلاڪون_ڳڻپ(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_بلاڪون - sb->s_پهريون_معلومات_بلاڪ, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_بلاڪون;
	إذا (rsv_gdb > EXT2_ADDR_في_بلاڪ(sb))
		rsv_gdb = EXT2_ADDR_في_بلاڪ(sb);
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
	لکو("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_بلاڪون, rsv_groups, rsv_gdb);
#جيڪڏهن_پورو

	أرجع rsv_gdb;
}

errcode_t ext2fs_initialize(ثابت حرف *name, صحيح flags,
			    بنية ext2_super_بلاڪ *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	بنية ext2_super_بلاڪ *super;
	بنانشان صحيح	rem;
	بنانشان صحيح	overhead = 0;
	بنانشان صحيح	ipg;
	dgrp_t		i;
	blk64_t		free_بلاڪون;
	blk_t		numblocks;
	صحيح		rsv_gdt;
	صحيح		csum_flag;
	صحيح		bigalloc_flag;
	صحيح		io_flags;
	صحيح		has_bg;
	بنانشان	reserved_inos;
	حرف		*buf = 0;
	حرف		c;
	مزدوج		reserved_تناسب;
	حرف		*وقت_env;

	إذا (!param || !ext2fs_بلاڪون_ڳڻپ(param))
		أرجع EXT2_ET_INVALID_دليل;

	retval = ext2fs_get_mem(حجم(بنية struct_ext2_filsys), &fs);
	إذا (retval)
		أرجع retval;

	memset(fs, 0, حجم(بنية struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	fs->flags |= EXT2_FLAG_مٽا_سٽا_بائيٽون;
#جيڪڏهن_پورو

	وقت_env = ext2fs_محفوظ_getenv("SOURCE_DATE_EPOCH");
	إذا (وقت_env) {
		fs->now = strtoul(وقت_env, عدم, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_وقت;
	} وإلا {
		وقت_env = ext2fs_محفوظ_getenv("E2FSPROGS_FAKE_TIME");
		إذا (وقت_env)
			fs->now = strtoul(وقت_env, عدم, 0);
	}

	io_flags = IO_FLAG_RW;
	إذا (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	إذا (flags & EXT2_FLAG_سڌو_IO)
		io_flags |= IO_FLAG_سڌو_IO;
	io_flags |= O_ٻٽو;
	retval = manager->open(name, io_flags, &fs->io);
	إذا (retval)
		اذهب cleanup;
	fs->image_io = fs->io;
	fs->io->app_معلومات = fs;
	retval = ext2fs_get_mem(لڙي_ڊيگهه(name)+1, &fs->device_name);
	إذا (retval)
		اذهب cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_سائيز, &super);
	إذا (retval)
		اذهب cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_سائيز);

#وصف_ڪريو سيٽ_ميدان(ميدان, افتراضي) (super->ميدان = param->ميدان ? \
				   param->ميدان : (افتراضي))
#وصف_ڪريو سونپيو_ميدان(ميدان)	(super->ميدان = param->ميدان)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	سونپيو_ميدان(s_log_بلاڪ_سائيز);

	إذا (bigalloc_flag) {
		سيٽ_ميدان(s_log_cluster_سائيز, super->s_log_بلاڪ_سائيز+4);
		إذا (super->s_log_بلاڪ_سائيز > super->s_log_cluster_سائيز) {
			retval = EXT2_ET_INVALID_دليل;
			اذهب cleanup;
		}
	} وإلا
		super->s_log_cluster_سائيز = super->s_log_بلاڪ_سائيز;

	سيٽ_ميدان(s_پهريون_معلومات_بلاڪ, super->s_log_cluster_سائيز ? 0 : 1);
	سيٽ_ميدان(s_max_mnt_ڳڻپ, 0);
	سيٽ_ميدان(s_errors, EXT2_ERRORS_DEFAULT);
	سيٽ_ميدان(s_feature_compat, 0);
	سيٽ_ميدان(s_feature_incompat, 0);
	سيٽ_ميدان(s_feature_ro_compat, 0);
	سيٽ_ميدان(s_default_mount_opts, 0);
	سيٽ_ميدان(s_پهريون_meta_bg, 0);
	سيٽ_ميدان(s_raid_stride, 0);		/* default stride size: 0 */
	سيٽ_ميدان(s_raid_stripe_ويڪر, 0);	/* default stripe width: 0 */
	سيٽ_ميدان(s_log_groups_في_flex, 0);
	سيٽ_ميدان(s_flags, 0);
	سونپيو_ميدان(s_backup_bgs[0]);
	سونپيو_ميدان(s_backup_bgs[1]);

	سونپيو_ميدان(s_encoding);
	سونپيو_ميدان(s_encoding_flags);

	إذا (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_ڀريو_nls_جدول(param->s_encoding);

	إذا (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		اذهب cleanup;
	}
	إذا (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		اذهب cleanup;
	}

	سيٽ_ميدان(s_rev_level, EXT2_GOOD_OLD_REV);
	إذا (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		سيٽ_ميدان(s_پهريون_ino, EXT2_GOOD_OLD_پهريون_INO);
		سيٽ_ميدان(s_inode_سائيز, EXT2_GOOD_OLD_INODE_سائيز);
		إذا (super->s_inode_سائيز >= حجم(بنية ext2_inode_large)) {
			صحيح extra_isize = حجم(بنية ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_سائيز;
			سيٽ_ميدان(s_min_extra_isize, extra_isize);
			سيٽ_ميدان(s_want_extra_isize, extra_isize);
		}
	} وإلا {
		super->s_پهريون_ino = EXT2_GOOD_OLD_پهريون_INO;
		super->s_inode_سائيز = EXT2_GOOD_OLD_INODE_سائيز;
	}

	سيٽ_ميدان(s_checkinterval, 0);
	ext2fs_سيٽ_tstamp(super, s_mkfs_وقت, ext2fs_P_get_وقت(fs));
	ext2fs_سيٽ_tstamp(super, s_lastcheck, ext2fs_P_get_وقت(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_بلاڪ_سائيز(super);
	fs->cluster_تناسب_بٽ = super->s_log_cluster_سائيز -
		super->s_log_بلاڪ_سائيز;

	إذا (bigalloc_flag) {
		بنانشان ڊگهو ڊگهو bpg;

		إذا (param->s_بلاڪون_في_گروهه &&
		    param->s_clusters_في_گروهه &&
		    ((param->s_clusters_في_گروهه * EXT2_FS_CLUSTER_تناسب(fs)) !=
		     param->s_بلاڪون_في_گروهه)) {
			retval = EXT2_ET_INVALID_دليل;
			اذهب cleanup;
		}
		إذا (param->s_clusters_في_گروهه)
			سونپيو_ميدان(s_clusters_في_گروهه);
		وإلا إذا (param->s_بلاڪون_في_گروهه)
			super->s_clusters_في_گروهه = 
				param->s_بلاڪون_في_گروهه /
				EXT2_FS_CLUSTER_تناسب(fs);
		وإلا إذا (super->s_log_cluster_سائيز + 15 < 32)
			super->s_clusters_في_گروهه = fs->blocksize * 8;
		وإلا
			super->s_clusters_في_گروهه = (fs->blocksize - 1) * 8;
		إذا (super->s_clusters_في_گروهه > EXT2_MAX_CLUSTERS_في_گروهه(super))
			super->s_clusters_في_گروهه = EXT2_MAX_CLUSTERS_في_گروهه(super);
		bpg = EXT2FS_C2B(fs,
			(بنانشان ڊگهو ڊگهو) super->s_clusters_في_گروهه);
		إذا (bpg >= (((بنانشان ڊگهو ڊگهو) 1) << 32)) {
			retval = EXT2_ET_INVALID_دليل;
			اذهب cleanup;
		}
		super->s_بلاڪون_في_گروهه = bpg;
	} وإلا {
		سيٽ_ميدان(s_بلاڪون_في_گروهه, fs->blocksize * 8);
		إذا (super->s_بلاڪون_في_گروهه > EXT2_MAX_بلاڪون_في_گروهه(super))
			super->s_بلاڪون_في_گروهه = EXT2_MAX_بلاڪون_في_گروهه(super);
		super->s_clusters_في_گروهه = super->s_بلاڪون_في_گروهه;
	}

	ext2fs_بلاڪون_ڳڻپ_سيٽ(super, ext2fs_بلاڪون_ڳڻپ(param) &
				~((blk64_t) EXT2_FS_CLUSTER_نقاب(fs)));
	ext2fs_r_بلاڪون_ڳڻپ_سيٽ(super, ext2fs_r_بلاڪون_ڳڻپ(param));
	إذا (ext2fs_r_بلاڪون_ڳڻپ(super) >= ext2fs_بلاڪون_ڳڻپ(param)) {
		retval = EXT2_ET_INVALID_دليل;
		اذهب cleanup;
	}

	سيٽ_ميدان(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	إذا (ext2fs_has_feature_journal_dev(super)) {
		fs->گروهه_desc_ڳڻپ = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		أرجع 0;
	}

retry:
	fs->گروهه_desc_ڳڻپ = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_بلاڪون_ڳڻپ(super) - super->s_پهريون_معلومات_بلاڪ,
		EXT2_بلاڪون_في_گروهه(super));
	إذا (fs->گروهه_desc_ڳڻپ == 0) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}

	سيٽ_ميدان(s_desc_سائيز,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_سائيز_64_ٻٽو_انگ : 0);

	fs->desc_بلاڪون = ext2fs_div_ceil(fs->گروهه_desc_ڳڻپ,
					  EXT2_DESC_في_بلاڪ(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	إذا (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_بلاڪون_ڳڻپ(super) / i) >= (1ULL << 32))
		سيٽ_ميدان(s_inodes_ڳڻپ, ~0U);
	وإلا
		سيٽ_ميدان(s_inodes_ڳڻپ, ext2fs_بلاڪون_ڳڻپ(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_ڳڻپ, fs->گروهه_desc_ڳڻپ);
	إذا (ipg > fs->blocksize * 8) {
		إذا (!bigalloc_flag && super->s_بلاڪون_في_گروهه >= 256) {
			/* Try again with slightly different parameters */
			super->s_بلاڪون_في_گروهه -= 8;
			ext2fs_بلاڪون_ڳڻپ_سيٽ(super,
						ext2fs_بلاڪون_ڳڻپ(param));
			super->s_clusters_في_گروهه = super->s_بلاڪون_في_گروهه;
			اذهب retry;
		} وإلا {
			retval = EXT2_ET_TOO_MANY_INODES;
			اذهب cleanup;
		}
	}

	إذا (ipg > (بنانشان) EXT2_MAX_INODES_في_گروهه(super))
		ipg = EXT2_MAX_INODES_في_گروهه(super);

ipg_retry:
	super->s_inodes_في_گروهه = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_بلاڪون_في_گروهه = (((super->s_inodes_في_گروهه *
					EXT2_INODE_سائيز(super)) +
				       EXT2_بلاڪ_سائيز(super) - 1) /
				      EXT2_بلاڪ_سائيز(super));
	super->s_inodes_في_گروهه = ((fs->inode_بلاڪون_في_گروهه *
				      EXT2_بلاڪ_سائيز(super)) /
				     EXT2_INODE_سائيز(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	إذا (super->s_inodes_في_گروهه < 8)
		super->s_inodes_في_گروهه = 8;
	super->s_inodes_في_گروهه &= ~7;
	fs->inode_بلاڪون_في_گروهه = (((super->s_inodes_في_گروهه *
					EXT2_INODE_سائيز(super)) +
				       EXT2_بلاڪ_سائيز(super) - 1) /
				      EXT2_بلاڪ_سائيز(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	إذا ((__u64)super->s_inodes_في_گروهه * fs->گروهه_desc_ڳڻپ > ~0U) {
		ipg--;
		اذهب ipg_retry;
	}
	super->s_inodes_ڳڻپ = super->s_inodes_في_گروهه *
		fs->گروهه_desc_ڳڻپ;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	إذا (super->s_inodes_ڳڻپ < EXT2_پهريون_INODE(super)+1) {
		ipg += 8;
		اذهب ipg_retry;
	}

	super->s_free_inodes_ڳڻپ = super->s_inodes_ڳڻپ;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	إذا (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_بلاڪون(fs);
	وإلا
		rsv_gdt = 0;
	سيٽ_ميدان(s_reserved_gdt_بلاڪون, rsv_gdt);
	إذا (super->s_reserved_gdt_بلاڪون > EXT2_ADDR_في_بلاڪ(super)) {
		retval = EXT2_ET_RES_GDT_بلاڪون;
		اذهب cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	إذا (super->s_reserved_gdt_بلاڪون + fs->desc_بلاڪون >
	    super->s_بلاڪون_في_گروهه * 3 / 4) {
		ext2fs_سيٽ_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		سيٽ_ميدان(s_reserved_gdt_بلاڪون, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (صحيح) (3 + fs->inode_بلاڪون_في_گروهه +
			  super->s_reserved_gdt_بلاڪون);

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	وإلا
		overhead += fs->desc_بلاڪون;

	/* This can only happen if the user requested too many inodes */
	إذا (overhead > super->s_بلاڪون_في_گروهه) {
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
	overhead = (صحيح) (2 + fs->inode_بلاڪون_في_گروهه);
	has_bg = 0;
	إذا (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		إذا (fs->گروهه_desc_ڳڻپ == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		وإلا
			has_bg = param->s_backup_bgs[1] != 0;
	} وإلا
		has_bg = ext2fs_bg_has_super(fs, fs->گروهه_desc_ڳڻپ - 1);
	إذا (has_bg)
		overhead += 1 + fs->desc_بلاڪون + super->s_reserved_gdt_بلاڪون;
	rem = ((ext2fs_بلاڪون_ڳڻپ(super) - super->s_پهريون_معلومات_بلاڪ) %
	       super->s_بلاڪون_في_گروهه);
	إذا ((fs->گروهه_desc_ڳڻپ == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}
	إذا (rem && (rem < overhead+50)) {
		ext2fs_بلاڪون_ڳڻپ_سيٽ(super, ext2fs_بلاڪون_ڳڻپ(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_تناسب = 100.0 * ext2fs_r_بلاڪون_ڳڻپ(param) /
			ext2fs_بلاڪون_ڳڻپ(param);
		ext2fs_r_بلاڪون_ڳڻپ_سيٽ(super, reserved_تناسب *
			ext2fs_بلاڪون_ڳڻپ(super) / 100.0);

		اذهب retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	إذا (ext2fs_has_feature_sparse_super2(super)) {
		إذا (super->s_backup_bgs[0] >= fs->گروهه_desc_ڳڻپ)
			super->s_backup_bgs[0] = fs->گروهه_desc_ڳڻپ - 1;
		إذا (super->s_backup_bgs[1] >= fs->گروهه_desc_ڳڻپ)
			super->s_backup_bgs[1] = fs->گروهه_desc_ڳڻپ - 1;
		إذا (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		إذا (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(لڙي_ڊيگهه(fs->device_name) + 80, &buf);
	إذا (retval)
		اذهب cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->بلاڪ_map);
	إذا (retval)
		اذهب cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	إذا (retval)
		اذهب cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_بلاڪون, fs->blocksize,
				&fs->گروهه_desc);
	إذا (retval)
		اذهب cleanup;

	memset(fs->گروهه_desc, 0, (size_t) fs->desc_بلاڪون * fs->blocksize);

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
	free_بلاڪون = 0;
	csum_flag = ext2fs_has_گروهه_desc_csum(fs);
	reserved_inos = super->s_پهريون_ino;
	لكل (i = 0; i < fs->گروهه_desc_ڳڻپ; i++) {
		blk_t grp_free_بلاڪون;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->بلاڪ_map,
						       &numblocks);
		إذا (retval)
			اذهب cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		إذا (csum_flag) {
			إذا (i != fs->گروهه_desc_ڳڻپ - 1 && numblocks == 0)
				ext2fs_bg_flags_سيٽ(fs, i,
						    EXT2_BG_بلاڪ_UNINIT);
			ext2fs_bg_flags_سيٽ(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_في_گروهه;
			إذا (reserved_inos) {
				إذا (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} وإلا {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_اڻ_استعمال_ٿيل_سيٽ(fs, i, inodes);
		}

		إذا (!fs->super->s_log_groups_في_flex)
			numblocks += 2 + fs->inode_بلاڪون_في_گروهه;

		grp_free_بلاڪون = ext2fs_گروهه_بلاڪون_ڳڻپ(fs, i) - numblocks;
		free_بلاڪون += grp_free_بلاڪون;
		ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(fs, i, grp_free_بلاڪون);
		ext2fs_bg_free_inodes_ڳڻپ_سيٽ(fs, i, fs->super->s_inodes_في_گروهه);
		ext2fs_bg_استعمال_ٿيل_dirs_ڳڻپ_سيٽ(fs, i, 0);
		ext2fs_گروهه_desc_csum_سيٽ(fs, i);
	}
	free_بلاڪون &= ~EXT2_FS_CLUSTER_نقاب(fs);
	ext2fs_free_بلاڪون_ڳڻپ_سيٽ(super, free_بلاڪون);

	c = (حرف) 255;
	إذا (((صحيح) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} وإلا {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_سيٽ_blksize(fs->io, fs->blocksize);

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
	بنانشان صحيح	گروهه = 0;
	بنانشان صحيح	ڳڻپ = 0;
	صحيح		ڪل_free = 0;
	صحيح		گروهه_free = 0;
	صحيح		آخري_allocated = 0;
	صحيح		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	لكل (blk = fs->super->s_پهريون_معلومات_بلاڪ;
	     blk < ext2fs_بلاڪون_ڳڻپ(fs->super); blk++) {
		إذا (!ext2fs_fast_test_بلاڪ_bitmap2(fs->بلاڪ_map, blk)) {
			گروهه_free++;
			ڪل_free++;
		} وإلا {
			uninit = 0;
		}
		ڳڻپ++;
		إذا ((ڳڻپ == fs->super->s_بلاڪون_في_گروهه) ||
		    (blk == ext2fs_بلاڪون_ڳڻپ(fs->super)-1)) {
			ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(fs, گروهه,
							گروهه_free);
			إذا (!super_only) {
				إذا (uninit && blk !=
					ext2fs_بلاڪون_ڳڻپ(fs->super) - 1)
					ext2fs_bg_flags_سيٽ(fs, گروهه,
							EXT2_BG_بلاڪ_UNINIT);
				وإلا
					ext2fs_bg_flags_clear(fs, گروهه,
							EXT2_BG_بلاڪ_UNINIT);
			}
			ڳڻپ = 0;
			گروهه_free = 0;
			uninit = 1;
			گروهه++;
		}
	}
	ڪل_free = EXT2FS_C2B(fs, ڪل_free);
	ext2fs_free_بلاڪون_ڳڻپ_سيٽ(fs->super, ڪل_free);

	/*
	 * Next, calculate the inode statistics
	 */
	گروهه_free = 0;
	ڪل_free = 0;
	آخري_allocated = 0;
	ڳڻپ = 0;
	گروهه = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	لكل (ino = 1; ino <= fs->super->s_inodes_ڳڻپ && ino > 0; ino++) {
		إذا (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			گروهه_free++;
			ڪل_free++;
		} وإلا {
			آخري_allocated = ino;
		}
		ڳڻپ++;
		إذا ((ڳڻپ == fs->super->s_inodes_في_گروهه) ||
		    (ino == fs->super->s_inodes_ڳڻپ)) {
			إذا (!super_only) {
				إذا (آخري_allocated) {
					ext2fs_bg_flags_clear(fs, گروهه,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_اڻ_استعمال_ٿيل_سيٽ(fs, گروهه,
						fs->super->s_inodes_في_گروهه -
						(آخري_allocated %
						fs->super->s_inodes_في_گروهه));
				} وإلا {
					ext2fs_bg_flags_سيٽ(fs, گروهه,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_اڻ_استعمال_ٿيل_سيٽ(fs, گروهه,
									0);
				}
				ext2fs_bg_free_inodes_ڳڻپ_سيٽ(fs, گروهه,
								گروهه_free);
			}
			گروهه++;
			ڳڻپ = 0;
			گروهه_free = 0;
			آخري_allocated = 0;
		}
	}
	fs->super->s_free_inodes_ڳڻپ = ڪل_free;
	ext2fs_mark_super_dirty(fs);
	أرجع 0;
}
