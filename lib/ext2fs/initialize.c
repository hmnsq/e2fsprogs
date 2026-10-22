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

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

#اگر_غیر_تعریف_شدہ O_ثنائی
#تعریف_کرو O_ثنائی 0
#اگر_ختم

#اگر defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#تعریف_کرو CREATOR_OS EXT2_OS_LINUX
#ورنہ
#اگر defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#تعریف_کرو CREATOR_OS EXT2_OS_HURD
#ورنہ
#اگر defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#تعریف_کرو CREATOR_OS EXT2_OS_FREEBSD
#ورنہ
#اگر defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#تعریف_کرو CREATOR_OS EXT2_OS_LITES
#ورنہ
#تعریف_کرو CREATOR_OS EXT2_OS_LINUX /* by default */
#اگر_ختم /* defined(LITES) && defined(EXT2_OS_LITES) */
#اگر_ختم /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#اگر_ختم /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#اگر_ختم /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
ساکن غیرعلامتی صحیح calc_reserved_gdt_بلاکیں(ext2_filsys fs)
{
	ساخت ext2_super_بلاک *sb = fs->super;
	غیرعلامتی طویل bpg = sb->s_بلاکیں_فی_گروہ;
	غیرعلامتی صحیح gdpb = EXT2_DESC_فی_بلاک(sb);
	غیرعلامتی طویل max_بلاکیں = 0xffffffff;
	غیرعلامتی طویل rsv_groups;
	غیرعلامتی صحیح rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	اگر (ext2fs_بلاکیں_گنتی(sb) < max_بلاکیں / 1024)
		max_بلاکیں = ext2fs_بلاکیں_گنتی(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_بلاکیں - sb->s_پہلا_ڈیٹا_بلاک, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_بلاکیں;
	اگر (rsv_gdb > EXT2_ADDR_فی_بلاک(sb))
		rsv_gdb = EXT2_ADDR_فی_بلاک(sb);
#اگر_تعریف_شدہ RES_GDT_DEBUG
	لکھو("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_بلاکیں, rsv_groups, rsv_gdb);
#اگر_ختم

	واپس rsv_gdb;
}

errcode_t ext2fs_initialize(ثابت حرف *name, صحیح flags,
			    ساخت ext2_super_بلاک *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	ساخت ext2_super_بلاک *super;
	غیرعلامتی صحیح	rem;
	غیرعلامتی صحیح	overhead = 0;
	غیرعلامتی صحیح	ipg;
	dgrp_t		i;
	blk64_t		free_بلاکیں;
	blk_t		numblocks;
	صحیح		rsv_gdt;
	صحیح		csum_flag;
	صحیح		bigalloc_flag;
	صحیح		io_flags;
	صحیح		has_bg;
	غیرعلامتی	reserved_inos;
	حرف		*buf = 0;
	حرف		c;
	دوہرا		reserved_تناسب;
	حرف		*وقت_env;

	اگر (!param || !ext2fs_بلاکیں_گنتی(param))
		واپس EXT2_ET_INVALID_دلیل;

	retval = ext2fs_get_mem(حجم(ساخت struct_ext2_filsys), &fs);
	اگر (retval)
		واپس retval;

	memset(fs, 0, حجم(ساخت struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	fs->flags |= EXT2_FLAG_تبادلہ_بائٹیں;
#اگر_ختم

	وقت_env = ext2fs_محفوظ_getenv("SOURCE_DATE_EPOCH");
	اگر (وقت_env) {
		fs->now = strtoul(وقت_env, تہی, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_وقت;
	} ورنہ {
		وقت_env = ext2fs_محفوظ_getenv("E2FSPROGS_FAKE_TIME");
		اگر (وقت_env)
			fs->now = strtoul(وقت_env, تہی, 0);
	}

	io_flags = IO_FLAG_RW;
	اگر (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	اگر (flags & EXT2_FLAG_براہ_راست_IO)
		io_flags |= IO_FLAG_براہ_راست_IO;
	io_flags |= O_ثنائی;
	retval = manager->open(name, io_flags, &fs->io);
	اگر (retval)
		جاؤ cleanup;
	fs->image_io = fs->io;
	fs->io->app_ڈیٹا = fs;
	retval = ext2fs_get_mem(لڑی_لمبائی(name)+1, &fs->device_name);
	اگر (retval)
		جاؤ cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_سائز, &super);
	اگر (retval)
		جاؤ cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_سائز);

#تعریف_کرو سیٹ_میدان(میدان, طےشدہ) (super->میدان = param->میدان ? \
				   param->میدان : (طےشدہ))
#تعریف_کرو تفویض_میدان(میدان)	(super->میدان = param->میدان)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	تفویض_میدان(s_log_بلاک_سائز);

	اگر (bigalloc_flag) {
		سیٹ_میدان(s_log_cluster_سائز, super->s_log_بلاک_سائز+4);
		اگر (super->s_log_بلاک_سائز > super->s_log_cluster_سائز) {
			retval = EXT2_ET_INVALID_دلیل;
			جاؤ cleanup;
		}
	} ورنہ
		super->s_log_cluster_سائز = super->s_log_بلاک_سائز;

	سیٹ_میدان(s_پہلا_ڈیٹا_بلاک, super->s_log_cluster_سائز ? 0 : 1);
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
	تفویض_میدان(s_backup_bgs[0]);
	تفویض_میدان(s_backup_bgs[1]);

	تفویض_میدان(s_encoding);
	تفویض_میدان(s_encoding_flags);

	اگر (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_بھرو_nls_جدول(param->s_encoding);

	اگر (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		جاؤ cleanup;
	}
	اگر (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		جاؤ cleanup;
	}

	سیٹ_میدان(s_rev_level, EXT2_GOOD_OLD_REV);
	اگر (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		سیٹ_میدان(s_پہلا_ino, EXT2_GOOD_OLD_پہلا_INO);
		سیٹ_میدان(s_inode_سائز, EXT2_GOOD_OLD_INODE_سائز);
		اگر (super->s_inode_سائز >= حجم(ساخت ext2_inode_large)) {
			صحیح extra_isize = حجم(ساخت ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_سائز;
			سیٹ_میدان(s_min_extra_isize, extra_isize);
			سیٹ_میدان(s_want_extra_isize, extra_isize);
		}
	} ورنہ {
		super->s_پہلا_ino = EXT2_GOOD_OLD_پہلا_INO;
		super->s_inode_سائز = EXT2_GOOD_OLD_INODE_سائز;
	}

	سیٹ_میدان(s_checkinterval, 0);
	ext2fs_سیٹ_tstamp(super, s_mkfs_وقت, ext2fs_P_get_وقت(fs));
	ext2fs_سیٹ_tstamp(super, s_lastcheck, ext2fs_P_get_وقت(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_بلاک_سائز(super);
	fs->cluster_تناسب_بٹ = super->s_log_cluster_سائز -
		super->s_log_بلاک_سائز;

	اگر (bigalloc_flag) {
		غیرعلامتی طویل طویل bpg;

		اگر (param->s_بلاکیں_فی_گروہ &&
		    param->s_clusters_فی_گروہ &&
		    ((param->s_clusters_فی_گروہ * EXT2_FS_CLUSTER_تناسب(fs)) !=
		     param->s_بلاکیں_فی_گروہ)) {
			retval = EXT2_ET_INVALID_دلیل;
			جاؤ cleanup;
		}
		اگر (param->s_clusters_فی_گروہ)
			تفویض_میدان(s_clusters_فی_گروہ);
		ورنہ اگر (param->s_بلاکیں_فی_گروہ)
			super->s_clusters_فی_گروہ = 
				param->s_بلاکیں_فی_گروہ /
				EXT2_FS_CLUSTER_تناسب(fs);
		ورنہ اگر (super->s_log_cluster_سائز + 15 < 32)
			super->s_clusters_فی_گروہ = fs->blocksize * 8;
		ورنہ
			super->s_clusters_فی_گروہ = (fs->blocksize - 1) * 8;
		اگر (super->s_clusters_فی_گروہ > EXT2_MAX_CLUSTERS_فی_گروہ(super))
			super->s_clusters_فی_گروہ = EXT2_MAX_CLUSTERS_فی_گروہ(super);
		bpg = EXT2FS_C2B(fs,
			(غیرعلامتی طویل طویل) super->s_clusters_فی_گروہ);
		اگر (bpg >= (((غیرعلامتی طویل طویل) 1) << 32)) {
			retval = EXT2_ET_INVALID_دلیل;
			جاؤ cleanup;
		}
		super->s_بلاکیں_فی_گروہ = bpg;
	} ورنہ {
		سیٹ_میدان(s_بلاکیں_فی_گروہ, fs->blocksize * 8);
		اگر (super->s_بلاکیں_فی_گروہ > EXT2_MAX_بلاکیں_فی_گروہ(super))
			super->s_بلاکیں_فی_گروہ = EXT2_MAX_بلاکیں_فی_گروہ(super);
		super->s_clusters_فی_گروہ = super->s_بلاکیں_فی_گروہ;
	}

	ext2fs_بلاکیں_گنتی_سیٹ(super, ext2fs_بلاکیں_گنتی(param) &
				~((blk64_t) EXT2_FS_CLUSTER_نقاب(fs)));
	ext2fs_r_بلاکیں_گنتی_سیٹ(super, ext2fs_r_بلاکیں_گنتی(param));
	اگر (ext2fs_r_بلاکیں_گنتی(super) >= ext2fs_بلاکیں_گنتی(param)) {
		retval = EXT2_ET_INVALID_دلیل;
		جاؤ cleanup;
	}

	سیٹ_میدان(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	اگر (ext2fs_has_feature_journal_dev(super)) {
		fs->گروہ_desc_گنتی = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		واپس 0;
	}

retry:
	fs->گروہ_desc_گنتی = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_بلاکیں_گنتی(super) - super->s_پہلا_ڈیٹا_بلاک,
		EXT2_بلاکیں_فی_گروہ(super));
	اگر (fs->گروہ_desc_گنتی == 0) {
		retval = EXT2_ET_TOOSMALL;
		جاؤ cleanup;
	}

	سیٹ_میدان(s_desc_سائز,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_سائز_64_ثنائی_ہندسہ : 0);

	fs->desc_بلاکیں = ext2fs_div_ceil(fs->گروہ_desc_گنتی,
					  EXT2_DESC_فی_بلاک(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	اگر (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_بلاکیں_گنتی(super) / i) >= (1ULL << 32))
		سیٹ_میدان(s_inodes_گنتی, ~0U);
	ورنہ
		سیٹ_میدان(s_inodes_گنتی, ext2fs_بلاکیں_گنتی(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_گنتی, fs->گروہ_desc_گنتی);
	اگر (ipg > fs->blocksize * 8) {
		اگر (!bigalloc_flag && super->s_بلاکیں_فی_گروہ >= 256) {
			/* Try again with slightly different parameters */
			super->s_بلاکیں_فی_گروہ -= 8;
			ext2fs_بلاکیں_گنتی_سیٹ(super,
						ext2fs_بلاکیں_گنتی(param));
			super->s_clusters_فی_گروہ = super->s_بلاکیں_فی_گروہ;
			جاؤ retry;
		} ورنہ {
			retval = EXT2_ET_TOO_MANY_INODES;
			جاؤ cleanup;
		}
	}

	اگر (ipg > (غیرعلامتی) EXT2_MAX_INODES_فی_گروہ(super))
		ipg = EXT2_MAX_INODES_فی_گروہ(super);

ipg_retry:
	super->s_inodes_فی_گروہ = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_بلاکیں_فی_گروہ = (((super->s_inodes_فی_گروہ *
					EXT2_INODE_سائز(super)) +
				       EXT2_بلاک_سائز(super) - 1) /
				      EXT2_بلاک_سائز(super));
	super->s_inodes_فی_گروہ = ((fs->inode_بلاکیں_فی_گروہ *
				      EXT2_بلاک_سائز(super)) /
				     EXT2_INODE_سائز(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	اگر (super->s_inodes_فی_گروہ < 8)
		super->s_inodes_فی_گروہ = 8;
	super->s_inodes_فی_گروہ &= ~7;
	fs->inode_بلاکیں_فی_گروہ = (((super->s_inodes_فی_گروہ *
					EXT2_INODE_سائز(super)) +
				       EXT2_بلاک_سائز(super) - 1) /
				      EXT2_بلاک_سائز(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	اگر ((__u64)super->s_inodes_فی_گروہ * fs->گروہ_desc_گنتی > ~0U) {
		ipg--;
		جاؤ ipg_retry;
	}
	super->s_inodes_گنتی = super->s_inodes_فی_گروہ *
		fs->گروہ_desc_گنتی;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	اگر (super->s_inodes_گنتی < EXT2_پہلا_INODE(super)+1) {
		ipg += 8;
		جاؤ ipg_retry;
	}

	super->s_free_inodes_گنتی = super->s_inodes_گنتی;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	اگر (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_بلاکیں(fs);
	ورنہ
		rsv_gdt = 0;
	سیٹ_میدان(s_reserved_gdt_بلاکیں, rsv_gdt);
	اگر (super->s_reserved_gdt_بلاکیں > EXT2_ADDR_فی_بلاک(super)) {
		retval = EXT2_ET_RES_GDT_بلاکیں;
		جاؤ cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	اگر (super->s_reserved_gdt_بلاکیں + fs->desc_بلاکیں >
	    super->s_بلاکیں_فی_گروہ * 3 / 4) {
		ext2fs_سیٹ_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		سیٹ_میدان(s_reserved_gdt_بلاکیں, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (صحیح) (3 + fs->inode_بلاکیں_فی_گروہ +
			  super->s_reserved_gdt_بلاکیں);

	اگر (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	ورنہ
		overhead += fs->desc_بلاکیں;

	/* This can only happen if the user requested too many inodes */
	اگر (overhead > super->s_بلاکیں_فی_گروہ) {
		retval = EXT2_ET_TOO_MANY_INODES;
		جاؤ cleanup;
	}

	/*
	 * See if the last group is big enough to support the
	 * necessary data structures.  If not, we need to get rid of
	 * it.  We need to recalculate the overhead for the last block
	 * group, since it might or might not have a superblock
	 * backup.
	 */
	overhead = (صحیح) (2 + fs->inode_بلاکیں_فی_گروہ);
	has_bg = 0;
	اگر (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		اگر (fs->گروہ_desc_گنتی == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		ورنہ
			has_bg = param->s_backup_bgs[1] != 0;
	} ورنہ
		has_bg = ext2fs_bg_has_super(fs, fs->گروہ_desc_گنتی - 1);
	اگر (has_bg)
		overhead += 1 + fs->desc_بلاکیں + super->s_reserved_gdt_بلاکیں;
	rem = ((ext2fs_بلاکیں_گنتی(super) - super->s_پہلا_ڈیٹا_بلاک) %
	       super->s_بلاکیں_فی_گروہ);
	اگر ((fs->گروہ_desc_گنتی == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		جاؤ cleanup;
	}
	اگر (rem && (rem < overhead+50)) {
		ext2fs_بلاکیں_گنتی_سیٹ(super, ext2fs_بلاکیں_گنتی(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_تناسب = 100.0 * ext2fs_r_بلاکیں_گنتی(param) /
			ext2fs_بلاکیں_گنتی(param);
		ext2fs_r_بلاکیں_گنتی_سیٹ(super, reserved_تناسب *
			ext2fs_بلاکیں_گنتی(super) / 100.0);

		جاؤ retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	اگر (ext2fs_has_feature_sparse_super2(super)) {
		اگر (super->s_backup_bgs[0] >= fs->گروہ_desc_گنتی)
			super->s_backup_bgs[0] = fs->گروہ_desc_گنتی - 1;
		اگر (super->s_backup_bgs[1] >= fs->گروہ_desc_گنتی)
			super->s_backup_bgs[1] = fs->گروہ_desc_گنتی - 1;
		اگر (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		اگر (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(لڑی_لمبائی(fs->device_name) + 80, &buf);
	اگر (retval)
		جاؤ cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->بلاک_map);
	اگر (retval)
		جاؤ cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	اگر (retval)
		جاؤ cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_بلاکیں, fs->blocksize,
				&fs->گروہ_desc);
	اگر (retval)
		جاؤ cleanup;

	memset(fs->گروہ_desc, 0, (size_t) fs->desc_بلاکیں * fs->blocksize);

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
	free_بلاکیں = 0;
	csum_flag = ext2fs_has_گروہ_desc_csum(fs);
	reserved_inos = super->s_پہلا_ino;
	برائے (i = 0; i < fs->گروہ_desc_گنتی; i++) {
		blk_t grp_free_بلاکیں;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->بلاک_map,
						       &numblocks);
		اگر (retval)
			جاؤ cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		اگر (csum_flag) {
			اگر (i != fs->گروہ_desc_گنتی - 1 && numblocks == 0)
				ext2fs_bg_flags_سیٹ(fs, i,
						    EXT2_BG_بلاک_UNINIT);
			ext2fs_bg_flags_سیٹ(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_فی_گروہ;
			اگر (reserved_inos) {
				اگر (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} ورنہ {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_غیر_مستعمل_سیٹ(fs, i, inodes);
		}

		اگر (!fs->super->s_log_groups_فی_flex)
			numblocks += 2 + fs->inode_بلاکیں_فی_گروہ;

		grp_free_بلاکیں = ext2fs_گروہ_بلاکیں_گنتی(fs, i) - numblocks;
		free_بلاکیں += grp_free_بلاکیں;
		ext2fs_bg_free_بلاکیں_گنتی_سیٹ(fs, i, grp_free_بلاکیں);
		ext2fs_bg_free_inodes_گنتی_سیٹ(fs, i, fs->super->s_inodes_فی_گروہ);
		ext2fs_bg_مستعمل_dirs_گنتی_سیٹ(fs, i, 0);
		ext2fs_گروہ_desc_csum_سیٹ(fs, i);
	}
	free_بلاکیں &= ~EXT2_FS_CLUSTER_نقاب(fs);
	ext2fs_free_بلاکیں_گنتی_سیٹ(super, free_بلاکیں);

	c = (حرف) 255;
	اگر (((صحیح) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} ورنہ {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_سیٹ_blksize(fs->io, fs->blocksize);

	*ret_fs = fs;
	واپس 0;
cleanup:
	آزاد(buf);
	ext2fs_free(fs);
	واپس retval;
}

errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, صحیح super_only)
{
	blk64_t		blk;
	ext2_ino_t	ino;
	غیرعلامتی صحیح	گروہ = 0;
	غیرعلامتی صحیح	گنتی = 0;
	صحیح		کل_free = 0;
	صحیح		گروہ_free = 0;
	صحیح		آخری_allocated = 0;
	صحیح		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	برائے (blk = fs->super->s_پہلا_ڈیٹا_بلاک;
	     blk < ext2fs_بلاکیں_گنتی(fs->super); blk++) {
		اگر (!ext2fs_fast_test_بلاک_bitmap2(fs->بلاک_map, blk)) {
			گروہ_free++;
			کل_free++;
		} ورنہ {
			uninit = 0;
		}
		گنتی++;
		اگر ((گنتی == fs->super->s_بلاکیں_فی_گروہ) ||
		    (blk == ext2fs_بلاکیں_گنتی(fs->super)-1)) {
			ext2fs_bg_free_بلاکیں_گنتی_سیٹ(fs, گروہ,
							گروہ_free);
			اگر (!super_only) {
				اگر (uninit && blk !=
					ext2fs_بلاکیں_گنتی(fs->super) - 1)
					ext2fs_bg_flags_سیٹ(fs, گروہ,
							EXT2_BG_بلاک_UNINIT);
				ورنہ
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
	ext2fs_free_بلاکیں_گنتی_سیٹ(fs->super, کل_free);

	/*
	 * Next, calculate the inode statistics
	 */
	گروہ_free = 0;
	کل_free = 0;
	آخری_allocated = 0;
	گنتی = 0;
	گروہ = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	برائے (ino = 1; ino <= fs->super->s_inodes_گنتی && ino > 0; ino++) {
		اگر (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			گروہ_free++;
			کل_free++;
		} ورنہ {
			آخری_allocated = ino;
		}
		گنتی++;
		اگر ((گنتی == fs->super->s_inodes_فی_گروہ) ||
		    (ino == fs->super->s_inodes_گنتی)) {
			اگر (!super_only) {
				اگر (آخری_allocated) {
					ext2fs_bg_flags_clear(fs, گروہ,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_غیر_مستعمل_سیٹ(fs, گروہ,
						fs->super->s_inodes_فی_گروہ -
						(آخری_allocated %
						fs->super->s_inodes_فی_گروہ));
				} ورنہ {
					ext2fs_bg_flags_سیٹ(fs, گروہ,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_غیر_مستعمل_سیٹ(fs, گروہ,
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
	واپس 0;
}
