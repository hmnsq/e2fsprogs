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
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#که_نه_تعریف_شوی O_دوګونی
#تعریف_کړه O_دوګونی 0
#د_که_پای

#إذا defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#تعریف_کړه CREATOR_OS EXT2_OS_LINUX
#وإلا
#إذا defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#تعریف_کړه CREATOR_OS EXT2_OS_HURD
#وإلا
#إذا defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#تعریف_کړه CREATOR_OS EXT2_OS_FREEBSD
#وإلا
#إذا defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#تعریف_کړه CREATOR_OS EXT2_OS_LITES
#وإلا
#تعریف_کړه CREATOR_OS EXT2_OS_LINUX /* by default */
#د_که_پای /* defined(LITES) && defined(EXT2_OS_LITES) */
#د_که_پای /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#د_که_پای /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#د_که_پای /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
ساكن بینښه صحيح calc_reserved_gdt_بلاکونه(ext2_filsys fs)
{
	بنية ext2_super_بلاک *sb = fs->super;
	بینښه اوږد bpg = sb->s_بلاکونه_فی_ډله;
	بینښه صحيح gdpb = EXT2_DESC_فی_بلاک(sb);
	بینښه اوږد max_بلاکونه = 0xffffffff;
	بینښه اوږد rsv_groups;
	بینښه صحيح rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	إذا (ext2fs_بلاکونه_شمېر(sb) < max_بلاکونه / 1024)
		max_بلاکونه = ext2fs_بلاکونه_شمېر(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_بلاکونه - sb->s_لومړی_معلومات_بلاک, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_بلاکونه;
	إذا (rsv_gdb > EXT2_ADDR_فی_بلاک(sb))
		rsv_gdb = EXT2_ADDR_فی_بلاک(sb);
#که_تعریف_شوی RES_GDT_DEBUG
	چاپ_کړه("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_بلاکونه, rsv_groups, rsv_gdb);
#د_که_پای

	أرجع rsv_gdb;
}

errcode_t ext2fs_initialize(ثابت حرف *name, صحيح flags,
			    بنية ext2_super_بلاک *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	بنية ext2_super_بلاک *super;
	بینښه صحيح	rem;
	بینښه صحيح	overhead = 0;
	بینښه صحيح	ipg;
	dgrp_t		i;
	blk64_t		free_بلاکونه;
	blk_t		numblocks;
	صحيح		rsv_gdt;
	صحيح		csum_flag;
	صحيح		bigalloc_flag;
	صحيح		io_flags;
	صحيح		has_bg;
	بینښه	reserved_inos;
	حرف		*buf = 0;
	حرف		c;
	مزدوج		reserved_نسبت;
	حرف		*وخت_env;

	إذا (!param || !ext2fs_بلاکونه_شمېر(param))
		أرجع EXT2_ET_INVALID_دلیل;

	retval = ext2fs_get_mem(حجم(بنية struct_ext2_filsys), &fs);
	إذا (retval)
		أرجع retval;

	memset(fs, 0, حجم(بنية struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#که_تعریف_شوی ټکي_BIGENDIAN
	fs->flags |= EXT2_FLAG_بدلون_بایټونه;
#د_که_پای

	وخت_env = ext2fs_خوندي_getenv("SOURCE_DATE_EPOCH");
	إذا (وخت_env) {
		fs->now = strtoul(وخت_env, عدم, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_وخت;
	} وإلا {
		وخت_env = ext2fs_خوندي_getenv("E2FSPROGS_FAKE_TIME");
		إذا (وخت_env)
			fs->now = strtoul(وخت_env, عدم, 0);
	}

	io_flags = IO_FLAG_RW;
	إذا (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	إذا (flags & EXT2_FLAG_مستقیم_IO)
		io_flags |= IO_FLAG_مستقیم_IO;
	io_flags |= O_دوګونی;
	retval = manager->open(name, io_flags, &fs->io);
	إذا (retval)
		اذهب cleanup;
	fs->image_io = fs->io;
	fs->io->app_معلومات = fs;
	retval = ext2fs_get_mem(مزي_اوږدوالی(name)+1, &fs->device_name);
	إذا (retval)
		اذهب cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_اندازه, &super);
	إذا (retval)
		اذهب cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_اندازه);

#تعریف_کړه سېټ_ډګر(ډګر, افتراضي) (super->ډګر = param->ډګر ? \
				   param->ډګر : (افتراضي))
#تعریف_کړه ټاکل_ډګر(ډګر)	(super->ډګر = param->ډګر)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	ټاکل_ډګر(s_log_بلاک_اندازه);

	إذا (bigalloc_flag) {
		سېټ_ډګر(s_log_cluster_اندازه, super->s_log_بلاک_اندازه+4);
		إذا (super->s_log_بلاک_اندازه > super->s_log_cluster_اندازه) {
			retval = EXT2_ET_INVALID_دلیل;
			اذهب cleanup;
		}
	} وإلا
		super->s_log_cluster_اندازه = super->s_log_بلاک_اندازه;

	سېټ_ډګر(s_لومړی_معلومات_بلاک, super->s_log_cluster_اندازه ? 0 : 1);
	سېټ_ډګر(s_max_mnt_شمېر, 0);
	سېټ_ډګر(s_errors, EXT2_ERRORS_DEFAULT);
	سېټ_ډګر(s_feature_compat, 0);
	سېټ_ډګر(s_feature_incompat, 0);
	سېټ_ډګر(s_feature_ro_compat, 0);
	سېټ_ډګر(s_default_mount_opts, 0);
	سېټ_ډګر(s_لومړی_meta_bg, 0);
	سېټ_ډګر(s_raid_stride, 0);		/* default stride size: 0 */
	سېټ_ډګر(s_raid_stripe_پلنوالی, 0);	/* default stripe width: 0 */
	سېټ_ډګر(s_log_groups_فی_flex, 0);
	سېټ_ډګر(s_flags, 0);
	ټاکل_ډګر(s_backup_bgs[0]);
	ټاکل_ډګر(s_backup_bgs[1]);

	ټاکل_ډګر(s_encoding);
	ټاکل_ډګر(s_encoding_flags);

	إذا (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_بارول_nls_جدول(param->s_encoding);

	إذا (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		اذهب cleanup;
	}
	إذا (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		اذهب cleanup;
	}

	سېټ_ډګر(s_rev_level, EXT2_GOOD_OLD_REV);
	إذا (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		سېټ_ډګر(s_لومړی_ino, EXT2_GOOD_OLD_لومړی_INO);
		سېټ_ډګر(s_inode_اندازه, EXT2_GOOD_OLD_INODE_اندازه);
		إذا (super->s_inode_اندازه >= حجم(بنية ext2_inode_large)) {
			صحيح extra_isize = حجم(بنية ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_اندازه;
			سېټ_ډګر(s_min_extra_isize, extra_isize);
			سېټ_ډګر(s_want_extra_isize, extra_isize);
		}
	} وإلا {
		super->s_لومړی_ino = EXT2_GOOD_OLD_لومړی_INO;
		super->s_inode_اندازه = EXT2_GOOD_OLD_INODE_اندازه;
	}

	سېټ_ډګر(s_checkinterval, 0);
	ext2fs_سېټ_tstamp(super, s_mkfs_وخت, ext2fs_P_get_وخت(fs));
	ext2fs_سېټ_tstamp(super, s_lastcheck, ext2fs_P_get_وخت(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_بلاک_اندازه(super);
	fs->cluster_نسبت_بټونه = super->s_log_cluster_اندازه -
		super->s_log_بلاک_اندازه;

	إذا (bigalloc_flag) {
		بینښه اوږد اوږد bpg;

		إذا (param->s_بلاکونه_فی_ډله &&
		    param->s_clusters_فی_ډله &&
		    ((param->s_clusters_فی_ډله * EXT2_FS_CLUSTER_نسبت(fs)) !=
		     param->s_بلاکونه_فی_ډله)) {
			retval = EXT2_ET_INVALID_دلیل;
			اذهب cleanup;
		}
		إذا (param->s_clusters_فی_ډله)
			ټاکل_ډګر(s_clusters_فی_ډله);
		وإلا إذا (param->s_بلاکونه_فی_ډله)
			super->s_clusters_فی_ډله = 
				param->s_بلاکونه_فی_ډله /
				EXT2_FS_CLUSTER_نسبت(fs);
		وإلا إذا (super->s_log_cluster_اندازه + 15 < 32)
			super->s_clusters_فی_ډله = fs->blocksize * 8;
		وإلا
			super->s_clusters_فی_ډله = (fs->blocksize - 1) * 8;
		إذا (super->s_clusters_فی_ډله > EXT2_MAX_CLUSTERS_فی_ډله(super))
			super->s_clusters_فی_ډله = EXT2_MAX_CLUSTERS_فی_ډله(super);
		bpg = EXT2FS_C2B(fs,
			(بینښه اوږد اوږد) super->s_clusters_فی_ډله);
		إذا (bpg >= (((بینښه اوږد اوږد) 1) << 32)) {
			retval = EXT2_ET_INVALID_دلیل;
			اذهب cleanup;
		}
		super->s_بلاکونه_فی_ډله = bpg;
	} وإلا {
		سېټ_ډګر(s_بلاکونه_فی_ډله, fs->blocksize * 8);
		إذا (super->s_بلاکونه_فی_ډله > EXT2_MAX_بلاکونه_فی_ډله(super))
			super->s_بلاکونه_فی_ډله = EXT2_MAX_بلاکونه_فی_ډله(super);
		super->s_clusters_فی_ډله = super->s_بلاکونه_فی_ډله;
	}

	ext2fs_بلاکونه_شمېر_سېټ(super, ext2fs_بلاکونه_شمېر(param) &
				~((blk64_t) EXT2_FS_CLUSTER_ماسک(fs)));
	ext2fs_r_بلاکونه_شمېر_سېټ(super, ext2fs_r_بلاکونه_شمېر(param));
	إذا (ext2fs_r_بلاکونه_شمېر(super) >= ext2fs_بلاکونه_شمېر(param)) {
		retval = EXT2_ET_INVALID_دلیل;
		اذهب cleanup;
	}

	سېټ_ډګر(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	إذا (ext2fs_has_feature_journal_dev(super)) {
		fs->ډله_desc_شمېر = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		أرجع 0;
	}

retry:
	fs->ډله_desc_شمېر = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_بلاکونه_شمېر(super) - super->s_لومړی_معلومات_بلاک,
		EXT2_بلاکونه_فی_ډله(super));
	إذا (fs->ډله_desc_شمېر == 0) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}

	سېټ_ډګر(s_desc_اندازه,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_اندازه_64_بټ : 0);

	fs->desc_بلاکونه = ext2fs_div_ceil(fs->ډله_desc_شمېر,
					  EXT2_DESC_فی_بلاک(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	إذا (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_بلاکونه_شمېر(super) / i) >= (1ULL << 32))
		سېټ_ډګر(s_inodes_شمېر, ~0U);
	وإلا
		سېټ_ډګر(s_inodes_شمېر, ext2fs_بلاکونه_شمېر(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_شمېر, fs->ډله_desc_شمېر);
	إذا (ipg > fs->blocksize * 8) {
		إذا (!bigalloc_flag && super->s_بلاکونه_فی_ډله >= 256) {
			/* Try again with slightly different parameters */
			super->s_بلاکونه_فی_ډله -= 8;
			ext2fs_بلاکونه_شمېر_سېټ(super,
						ext2fs_بلاکونه_شمېر(param));
			super->s_clusters_فی_ډله = super->s_بلاکونه_فی_ډله;
			اذهب retry;
		} وإلا {
			retval = EXT2_ET_TOO_MANY_INODES;
			اذهب cleanup;
		}
	}

	إذا (ipg > (بینښه) EXT2_MAX_INODES_فی_ډله(super))
		ipg = EXT2_MAX_INODES_فی_ډله(super);

ipg_retry:
	super->s_inodes_فی_ډله = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_بلاکونه_فی_ډله = (((super->s_inodes_فی_ډله *
					EXT2_INODE_اندازه(super)) +
				       EXT2_بلاک_اندازه(super) - 1) /
				      EXT2_بلاک_اندازه(super));
	super->s_inodes_فی_ډله = ((fs->inode_بلاکونه_فی_ډله *
				      EXT2_بلاک_اندازه(super)) /
				     EXT2_INODE_اندازه(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	إذا (super->s_inodes_فی_ډله < 8)
		super->s_inodes_فی_ډله = 8;
	super->s_inodes_فی_ډله &= ~7;
	fs->inode_بلاکونه_فی_ډله = (((super->s_inodes_فی_ډله *
					EXT2_INODE_اندازه(super)) +
				       EXT2_بلاک_اندازه(super) - 1) /
				      EXT2_بلاک_اندازه(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	إذا ((__u64)super->s_inodes_فی_ډله * fs->ډله_desc_شمېر > ~0U) {
		ipg--;
		اذهب ipg_retry;
	}
	super->s_inodes_شمېر = super->s_inodes_فی_ډله *
		fs->ډله_desc_شمېر;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	إذا (super->s_inodes_شمېر < EXT2_لومړی_INODE(super)+1) {
		ipg += 8;
		اذهب ipg_retry;
	}

	super->s_free_inodes_شمېر = super->s_inodes_شمېر;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	إذا (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_بلاکونه(fs);
	وإلا
		rsv_gdt = 0;
	سېټ_ډګر(s_reserved_gdt_بلاکونه, rsv_gdt);
	إذا (super->s_reserved_gdt_بلاکونه > EXT2_ADDR_فی_بلاک(super)) {
		retval = EXT2_ET_RES_GDT_بلاکونه;
		اذهب cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	إذا (super->s_reserved_gdt_بلاکونه + fs->desc_بلاکونه >
	    super->s_بلاکونه_فی_ډله * 3 / 4) {
		ext2fs_سېټ_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		سېټ_ډګر(s_reserved_gdt_بلاکونه, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (صحيح) (3 + fs->inode_بلاکونه_فی_ډله +
			  super->s_reserved_gdt_بلاکونه);

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	وإلا
		overhead += fs->desc_بلاکونه;

	/* This can only happen if the user requested too many inodes */
	إذا (overhead > super->s_بلاکونه_فی_ډله) {
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
	overhead = (صحيح) (2 + fs->inode_بلاکونه_فی_ډله);
	has_bg = 0;
	إذا (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		إذا (fs->ډله_desc_شمېر == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		وإلا
			has_bg = param->s_backup_bgs[1] != 0;
	} وإلا
		has_bg = ext2fs_bg_has_super(fs, fs->ډله_desc_شمېر - 1);
	إذا (has_bg)
		overhead += 1 + fs->desc_بلاکونه + super->s_reserved_gdt_بلاکونه;
	rem = ((ext2fs_بلاکونه_شمېر(super) - super->s_لومړی_معلومات_بلاک) %
	       super->s_بلاکونه_فی_ډله);
	إذا ((fs->ډله_desc_شمېر == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}
	إذا (rem && (rem < overhead+50)) {
		ext2fs_بلاکونه_شمېر_سېټ(super, ext2fs_بلاکونه_شمېر(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_نسبت = 100.0 * ext2fs_r_بلاکونه_شمېر(param) /
			ext2fs_بلاکونه_شمېر(param);
		ext2fs_r_بلاکونه_شمېر_سېټ(super, reserved_نسبت *
			ext2fs_بلاکونه_شمېر(super) / 100.0);

		اذهب retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	إذا (ext2fs_has_feature_sparse_super2(super)) {
		إذا (super->s_backup_bgs[0] >= fs->ډله_desc_شمېر)
			super->s_backup_bgs[0] = fs->ډله_desc_شمېر - 1;
		إذا (super->s_backup_bgs[1] >= fs->ډله_desc_شمېر)
			super->s_backup_bgs[1] = fs->ډله_desc_شمېر - 1;
		إذا (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		إذا (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(مزي_اوږدوالی(fs->device_name) + 80, &buf);
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

	retval = ext2fs_get_array(fs->desc_بلاکونه, fs->blocksize,
				&fs->ډله_desc);
	إذا (retval)
		اذهب cleanup;

	memset(fs->ډله_desc, 0, (size_t) fs->desc_بلاکونه * fs->blocksize);

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
	free_بلاکونه = 0;
	csum_flag = ext2fs_has_ډله_desc_csum(fs);
	reserved_inos = super->s_لومړی_ino;
	لكل (i = 0; i < fs->ډله_desc_شمېر; i++) {
		blk_t grp_free_بلاکونه;
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
			إذا (i != fs->ډله_desc_شمېر - 1 && numblocks == 0)
				ext2fs_bg_flags_سېټ(fs, i,
						    EXT2_BG_بلاک_UNINIT);
			ext2fs_bg_flags_سېټ(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_فی_ډله;
			إذا (reserved_inos) {
				إذا (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} وإلا {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_ناکارول_شوی_سېټ(fs, i, inodes);
		}

		إذا (!fs->super->s_log_groups_فی_flex)
			numblocks += 2 + fs->inode_بلاکونه_فی_ډله;

		grp_free_بلاکونه = ext2fs_ډله_بلاکونه_شمېر(fs, i) - numblocks;
		free_بلاکونه += grp_free_بلاکونه;
		ext2fs_bg_free_بلاکونه_شمېر_سېټ(fs, i, grp_free_بلاکونه);
		ext2fs_bg_free_inodes_شمېر_سېټ(fs, i, fs->super->s_inodes_فی_ډله);
		ext2fs_bg_کارول_شوی_dirs_شمېر_سېټ(fs, i, 0);
		ext2fs_ډله_desc_csum_سېټ(fs, i);
	}
	free_بلاکونه &= ~EXT2_FS_CLUSTER_ماسک(fs);
	ext2fs_free_بلاکونه_شمېر_سېټ(super, free_بلاکونه);

	c = (حرف) 255;
	إذا (((صحيح) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} وإلا {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_سېټ_blksize(fs->io, fs->blocksize);

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
	بینښه صحيح	ډله = 0;
	بینښه صحيح	شمېر = 0;
	صحيح		ټول_free = 0;
	صحيح		ډله_free = 0;
	صحيح		وروستی_allocated = 0;
	صحيح		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	لكل (blk = fs->super->s_لومړی_معلومات_بلاک;
	     blk < ext2fs_بلاکونه_شمېر(fs->super); blk++) {
		إذا (!ext2fs_fast_test_بلاک_bitmap2(fs->بلاک_map, blk)) {
			ډله_free++;
			ټول_free++;
		} وإلا {
			uninit = 0;
		}
		شمېر++;
		إذا ((شمېر == fs->super->s_بلاکونه_فی_ډله) ||
		    (blk == ext2fs_بلاکونه_شمېر(fs->super)-1)) {
			ext2fs_bg_free_بلاکونه_شمېر_سېټ(fs, ډله,
							ډله_free);
			إذا (!super_only) {
				إذا (uninit && blk !=
					ext2fs_بلاکونه_شمېر(fs->super) - 1)
					ext2fs_bg_flags_سېټ(fs, ډله,
							EXT2_BG_بلاک_UNINIT);
				وإلا
					ext2fs_bg_flags_clear(fs, ډله,
							EXT2_BG_بلاک_UNINIT);
			}
			شمېر = 0;
			ډله_free = 0;
			uninit = 1;
			ډله++;
		}
	}
	ټول_free = EXT2FS_C2B(fs, ټول_free);
	ext2fs_free_بلاکونه_شمېر_سېټ(fs->super, ټول_free);

	/*
	 * Next, calculate the inode statistics
	 */
	ډله_free = 0;
	ټول_free = 0;
	وروستی_allocated = 0;
	شمېر = 0;
	ډله = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	لكل (ino = 1; ino <= fs->super->s_inodes_شمېر && ino > 0; ino++) {
		إذا (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			ډله_free++;
			ټول_free++;
		} وإلا {
			وروستی_allocated = ino;
		}
		شمېر++;
		إذا ((شمېر == fs->super->s_inodes_فی_ډله) ||
		    (ino == fs->super->s_inodes_شمېر)) {
			إذا (!super_only) {
				إذا (وروستی_allocated) {
					ext2fs_bg_flags_clear(fs, ډله,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_ناکارول_شوی_سېټ(fs, ډله,
						fs->super->s_inodes_فی_ډله -
						(وروستی_allocated %
						fs->super->s_inodes_فی_ډله));
				} وإلا {
					ext2fs_bg_flags_سېټ(fs, ډله,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_ناکارول_شوی_سېټ(fs, ډله,
									0);
				}
				ext2fs_bg_free_inodes_شمېر_سېټ(fs, ډله,
								ډله_free);
			}
			ډله++;
			شمېر = 0;
			ډله_free = 0;
			وروستی_allocated = 0;
		}
	}
	fs->super->s_free_inodes_شمېر = ټول_free;
	ext2fs_mark_super_dirty(fs);
	أرجع 0;
}
