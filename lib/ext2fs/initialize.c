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
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#اگر_تعریف_نشده O_دودویی
#تعریف_کن O_دودویی 0
#پایان_اگر

#إذا defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#تعریف_کن CREATOR_OS EXT2_OS_LINUX
#وإلا
#إذا defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#تعریف_کن CREATOR_OS EXT2_OS_HURD
#وإلا
#إذا defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#تعریف_کن CREATOR_OS EXT2_OS_FREEBSD
#وإلا
#إذا defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#تعریف_کن CREATOR_OS EXT2_OS_LITES
#وإلا
#تعریف_کن CREATOR_OS EXT2_OS_LINUX /* by default */
#پایان_اگر /* defined(LITES) && defined(EXT2_OS_LITES) */
#پایان_اگر /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#پایان_اگر /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#پایان_اگر /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
ساكن بیعلامت صحيح calc_reserved_gdt_بلوکها(ext2_filsys fs)
{
	بنية ext2_super_بلوک *sb = fs->super;
	بیعلامت دراز bpg = sb->s_بلوکها_در_هر_گروه;
	بیعلامت صحيح gdpb = EXT2_DESC_در_هر_بلوک(sb);
	بیعلامت دراز max_بلوکها = 0xffffffff;
	بیعلامت دراز rsv_groups;
	بیعلامت صحيح rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	إذا (ext2fs_بلوکها_شمار(sb) < max_بلوکها / 1024)
		max_بلوکها = ext2fs_بلوکها_شمار(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_بلوکها - sb->s_اولی_داده_بلوک, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_بلوکها;
	إذا (rsv_gdb > EXT2_ADDR_در_هر_بلوک(sb))
		rsv_gdb = EXT2_ADDR_در_هر_بلوک(sb);
#اگر_تعریف_شده RES_GDT_DEBUG
	چاپ_کن("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_بلوکها, rsv_groups, rsv_gdb);
#پایان_اگر

	أرجع rsv_gdb;
}

errcode_t ext2fs_initialize(ثابت حرف *name, صحيح flags,
			    بنية ext2_super_بلوک *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	بنية ext2_super_بلوک *super;
	بیعلامت صحيح	rem;
	بیعلامت صحيح	overhead = 0;
	بیعلامت صحيح	ipg;
	dgrp_t		i;
	blk64_t		free_بلوکها;
	blk_t		numblocks;
	صحيح		rsv_gdt;
	صحيح		csum_flag;
	صحيح		bigalloc_flag;
	صحيح		io_flags;
	صحيح		has_bg;
	بیعلامت	reserved_inos;
	حرف		*buf = 0;
	حرف		c;
	مزدوج		reserved_نسبت;
	حرف		*زمان_env;

	إذا (!param || !ext2fs_بلوکها_شمار(param))
		أرجع EXT2_ET_INVALID_آرگومان;

	retval = ext2fs_get_mem(حجم(بنية struct_ext2_filsys), &fs);
	إذا (retval)
		أرجع retval;

	memset(fs, 0, حجم(بنية struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#اگر_تعریف_شده واژگان_BIGENDIAN
	fs->flags |= EXT2_FLAG_جابجا_بایتها;
#پایان_اگر

	زمان_env = ext2fs_امن_getenv("SOURCE_DATE_EPOCH");
	إذا (زمان_env) {
		fs->now = strtoul(زمان_env, عدم, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_زمان;
	} وإلا {
		زمان_env = ext2fs_امن_getenv("E2FSPROGS_FAKE_TIME");
		إذا (زمان_env)
			fs->now = strtoul(زمان_env, عدم, 0);
	}

	io_flags = IO_FLAG_RW;
	إذا (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	إذا (flags & EXT2_FLAG_مستقیم_IO)
		io_flags |= IO_FLAG_مستقیم_IO;
	io_flags |= O_دودویی;
	retval = manager->open(name, io_flags, &fs->io);
	إذا (retval)
		اذهب cleanup;
	fs->image_io = fs->io;
	fs->io->app_داده = fs;
	retval = ext2fs_get_mem(طول_رشته(name)+1, &fs->device_name);
	إذا (retval)
		اذهب cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_اندازه, &super);
	إذا (retval)
		اذهب cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_اندازه);

#تعریف_کن مجموعه_فیلد(فیلد, افتراضي) (super->فیلد = param->فیلد ? \
				   param->فیلد : (افتراضي))
#تعریف_کن تخصیص_بده_فیلد(فیلد)	(super->فیلد = param->فیلد)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	تخصیص_بده_فیلد(s_log_بلوک_اندازه);

	إذا (bigalloc_flag) {
		مجموعه_فیلد(s_log_cluster_اندازه, super->s_log_بلوک_اندازه+4);
		إذا (super->s_log_بلوک_اندازه > super->s_log_cluster_اندازه) {
			retval = EXT2_ET_INVALID_آرگومان;
			اذهب cleanup;
		}
	} وإلا
		super->s_log_cluster_اندازه = super->s_log_بلوک_اندازه;

	مجموعه_فیلد(s_اولی_داده_بلوک, super->s_log_cluster_اندازه ? 0 : 1);
	مجموعه_فیلد(s_max_mnt_شمار, 0);
	مجموعه_فیلد(s_errors, EXT2_ERRORS_DEFAULT);
	مجموعه_فیلد(s_feature_compat, 0);
	مجموعه_فیلد(s_feature_incompat, 0);
	مجموعه_فیلد(s_feature_ro_compat, 0);
	مجموعه_فیلد(s_default_mount_opts, 0);
	مجموعه_فیلد(s_اولی_meta_bg, 0);
	مجموعه_فیلد(s_raid_stride, 0);		/* default stride size: 0 */
	مجموعه_فیلد(s_raid_stripe_عرض, 0);	/* default stripe width: 0 */
	مجموعه_فیلد(s_log_groups_در_هر_flex, 0);
	مجموعه_فیلد(s_flags, 0);
	تخصیص_بده_فیلد(s_backup_bgs[0]);
	تخصیص_بده_فیلد(s_backup_bgs[1]);

	تخصیص_بده_فیلد(s_encoding);
	تخصیص_بده_فیلد(s_encoding_flags);

	إذا (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_بارگیری_nls_جدول(param->s_encoding);

	إذا (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		اذهب cleanup;
	}
	إذا (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		اذهب cleanup;
	}

	مجموعه_فیلد(s_rev_level, EXT2_GOOD_OLD_REV);
	إذا (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		مجموعه_فیلد(s_اولی_ino, EXT2_GOOD_OLD_اولی_INO);
		مجموعه_فیلد(s_inode_اندازه, EXT2_GOOD_OLD_INODE_اندازه);
		إذا (super->s_inode_اندازه >= حجم(بنية ext2_inode_large)) {
			صحيح extra_isize = حجم(بنية ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_اندازه;
			مجموعه_فیلد(s_min_extra_isize, extra_isize);
			مجموعه_فیلد(s_want_extra_isize, extra_isize);
		}
	} وإلا {
		super->s_اولی_ino = EXT2_GOOD_OLD_اولی_INO;
		super->s_inode_اندازه = EXT2_GOOD_OLD_INODE_اندازه;
	}

	مجموعه_فیلد(s_checkinterval, 0);
	ext2fs_مجموعه_tstamp(super, s_mkfs_زمان, ext2fs_P_get_زمان(fs));
	ext2fs_مجموعه_tstamp(super, s_lastcheck, ext2fs_P_get_زمان(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_بلوک_اندازه(super);
	fs->cluster_نسبت_بیتها = super->s_log_cluster_اندازه -
		super->s_log_بلوک_اندازه;

	إذا (bigalloc_flag) {
		بیعلامت دراز دراز bpg;

		إذا (param->s_بلوکها_در_هر_گروه &&
		    param->s_clusters_در_هر_گروه &&
		    ((param->s_clusters_در_هر_گروه * EXT2_FS_CLUSTER_نسبت(fs)) !=
		     param->s_بلوکها_در_هر_گروه)) {
			retval = EXT2_ET_INVALID_آرگومان;
			اذهب cleanup;
		}
		إذا (param->s_clusters_در_هر_گروه)
			تخصیص_بده_فیلد(s_clusters_در_هر_گروه);
		وإلا إذا (param->s_بلوکها_در_هر_گروه)
			super->s_clusters_در_هر_گروه = 
				param->s_بلوکها_در_هر_گروه /
				EXT2_FS_CLUSTER_نسبت(fs);
		وإلا إذا (super->s_log_cluster_اندازه + 15 < 32)
			super->s_clusters_در_هر_گروه = fs->blocksize * 8;
		وإلا
			super->s_clusters_در_هر_گروه = (fs->blocksize - 1) * 8;
		إذا (super->s_clusters_در_هر_گروه > EXT2_MAX_CLUSTERS_در_هر_گروه(super))
			super->s_clusters_در_هر_گروه = EXT2_MAX_CLUSTERS_در_هر_گروه(super);
		bpg = EXT2FS_C2B(fs,
			(بیعلامت دراز دراز) super->s_clusters_در_هر_گروه);
		إذا (bpg >= (((بیعلامت دراز دراز) 1) << 32)) {
			retval = EXT2_ET_INVALID_آرگومان;
			اذهب cleanup;
		}
		super->s_بلوکها_در_هر_گروه = bpg;
	} وإلا {
		مجموعه_فیلد(s_بلوکها_در_هر_گروه, fs->blocksize * 8);
		إذا (super->s_بلوکها_در_هر_گروه > EXT2_MAX_بلوکها_در_هر_گروه(super))
			super->s_بلوکها_در_هر_گروه = EXT2_MAX_بلوکها_در_هر_گروه(super);
		super->s_clusters_در_هر_گروه = super->s_بلوکها_در_هر_گروه;
	}

	ext2fs_بلوکها_شمار_مجموعه(super, ext2fs_بلوکها_شمار(param) &
				~((blk64_t) EXT2_FS_CLUSTER_ماسک(fs)));
	ext2fs_r_بلوکها_شمار_مجموعه(super, ext2fs_r_بلوکها_شمار(param));
	إذا (ext2fs_r_بلوکها_شمار(super) >= ext2fs_بلوکها_شمار(param)) {
		retval = EXT2_ET_INVALID_آرگومان;
		اذهب cleanup;
	}

	مجموعه_فیلد(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	إذا (ext2fs_has_feature_journal_dev(super)) {
		fs->گروه_desc_شمار = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		أرجع 0;
	}

retry:
	fs->گروه_desc_شمار = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_بلوکها_شمار(super) - super->s_اولی_داده_بلوک,
		EXT2_بلوکها_در_هر_گروه(super));
	إذا (fs->گروه_desc_شمار == 0) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}

	مجموعه_فیلد(s_desc_اندازه,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_اندازه_64_بیت : 0);

	fs->desc_بلوکها = ext2fs_div_ceil(fs->گروه_desc_شمار,
					  EXT2_DESC_در_هر_بلوک(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	إذا (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_بلوکها_شمار(super) / i) >= (1ULL << 32))
		مجموعه_فیلد(s_inodes_شمار, ~0U);
	وإلا
		مجموعه_فیلد(s_inodes_شمار, ext2fs_بلوکها_شمار(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_شمار, fs->گروه_desc_شمار);
	إذا (ipg > fs->blocksize * 8) {
		إذا (!bigalloc_flag && super->s_بلوکها_در_هر_گروه >= 256) {
			/* Try again with slightly different parameters */
			super->s_بلوکها_در_هر_گروه -= 8;
			ext2fs_بلوکها_شمار_مجموعه(super,
						ext2fs_بلوکها_شمار(param));
			super->s_clusters_در_هر_گروه = super->s_بلوکها_در_هر_گروه;
			اذهب retry;
		} وإلا {
			retval = EXT2_ET_TOO_MANY_INODES;
			اذهب cleanup;
		}
	}

	إذا (ipg > (بیعلامت) EXT2_MAX_INODES_در_هر_گروه(super))
		ipg = EXT2_MAX_INODES_در_هر_گروه(super);

ipg_retry:
	super->s_inodes_در_هر_گروه = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_بلوکها_در_هر_گروه = (((super->s_inodes_در_هر_گروه *
					EXT2_INODE_اندازه(super)) +
				       EXT2_بلوک_اندازه(super) - 1) /
				      EXT2_بلوک_اندازه(super));
	super->s_inodes_در_هر_گروه = ((fs->inode_بلوکها_در_هر_گروه *
				      EXT2_بلوک_اندازه(super)) /
				     EXT2_INODE_اندازه(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	إذا (super->s_inodes_در_هر_گروه < 8)
		super->s_inodes_در_هر_گروه = 8;
	super->s_inodes_در_هر_گروه &= ~7;
	fs->inode_بلوکها_در_هر_گروه = (((super->s_inodes_در_هر_گروه *
					EXT2_INODE_اندازه(super)) +
				       EXT2_بلوک_اندازه(super) - 1) /
				      EXT2_بلوک_اندازه(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	إذا ((__u64)super->s_inodes_در_هر_گروه * fs->گروه_desc_شمار > ~0U) {
		ipg--;
		اذهب ipg_retry;
	}
	super->s_inodes_شمار = super->s_inodes_در_هر_گروه *
		fs->گروه_desc_شمار;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	إذا (super->s_inodes_شمار < EXT2_اولی_INODE(super)+1) {
		ipg += 8;
		اذهب ipg_retry;
	}

	super->s_free_inodes_شمار = super->s_inodes_شمار;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	إذا (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_بلوکها(fs);
	وإلا
		rsv_gdt = 0;
	مجموعه_فیلد(s_reserved_gdt_بلوکها, rsv_gdt);
	إذا (super->s_reserved_gdt_بلوکها > EXT2_ADDR_در_هر_بلوک(super)) {
		retval = EXT2_ET_RES_GDT_بلوکها;
		اذهب cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	إذا (super->s_reserved_gdt_بلوکها + fs->desc_بلوکها >
	    super->s_بلوکها_در_هر_گروه * 3 / 4) {
		ext2fs_مجموعه_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		مجموعه_فیلد(s_reserved_gdt_بلوکها, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (صحيح) (3 + fs->inode_بلوکها_در_هر_گروه +
			  super->s_reserved_gdt_بلوکها);

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	وإلا
		overhead += fs->desc_بلوکها;

	/* This can only happen if the user requested too many inodes */
	إذا (overhead > super->s_بلوکها_در_هر_گروه) {
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
	overhead = (صحيح) (2 + fs->inode_بلوکها_در_هر_گروه);
	has_bg = 0;
	إذا (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		إذا (fs->گروه_desc_شمار == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		وإلا
			has_bg = param->s_backup_bgs[1] != 0;
	} وإلا
		has_bg = ext2fs_bg_has_super(fs, fs->گروه_desc_شمار - 1);
	إذا (has_bg)
		overhead += 1 + fs->desc_بلوکها + super->s_reserved_gdt_بلوکها;
	rem = ((ext2fs_بلوکها_شمار(super) - super->s_اولی_داده_بلوک) %
	       super->s_بلوکها_در_هر_گروه);
	إذا ((fs->گروه_desc_شمار == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}
	إذا (rem && (rem < overhead+50)) {
		ext2fs_بلوکها_شمار_مجموعه(super, ext2fs_بلوکها_شمار(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_نسبت = 100.0 * ext2fs_r_بلوکها_شمار(param) /
			ext2fs_بلوکها_شمار(param);
		ext2fs_r_بلوکها_شمار_مجموعه(super, reserved_نسبت *
			ext2fs_بلوکها_شمار(super) / 100.0);

		اذهب retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	إذا (ext2fs_has_feature_sparse_super2(super)) {
		إذا (super->s_backup_bgs[0] >= fs->گروه_desc_شمار)
			super->s_backup_bgs[0] = fs->گروه_desc_شمار - 1;
		إذا (super->s_backup_bgs[1] >= fs->گروه_desc_شمار)
			super->s_backup_bgs[1] = fs->گروه_desc_شمار - 1;
		إذا (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		إذا (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(طول_رشته(fs->device_name) + 80, &buf);
	إذا (retval)
		اذهب cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->بلوک_map);
	إذا (retval)
		اذهب cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	إذا (retval)
		اذهب cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_بلوکها, fs->blocksize,
				&fs->گروه_desc);
	إذا (retval)
		اذهب cleanup;

	memset(fs->گروه_desc, 0, (size_t) fs->desc_بلوکها * fs->blocksize);

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
	free_بلوکها = 0;
	csum_flag = ext2fs_has_گروه_desc_csum(fs);
	reserved_inos = super->s_اولی_ino;
	لكل (i = 0; i < fs->گروه_desc_شمار; i++) {
		blk_t grp_free_بلوکها;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->بلوک_map,
						       &numblocks);
		إذا (retval)
			اذهب cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		إذا (csum_flag) {
			إذا (i != fs->گروه_desc_شمار - 1 && numblocks == 0)
				ext2fs_bg_flags_مجموعه(fs, i,
						    EXT2_BG_بلوک_UNINIT);
			ext2fs_bg_flags_مجموعه(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_در_هر_گروه;
			إذا (reserved_inos) {
				إذا (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} وإلا {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_استفادهنشده_مجموعه(fs, i, inodes);
		}

		إذا (!fs->super->s_log_groups_در_هر_flex)
			numblocks += 2 + fs->inode_بلوکها_در_هر_گروه;

		grp_free_بلوکها = ext2fs_گروه_بلوکها_شمار(fs, i) - numblocks;
		free_بلوکها += grp_free_بلوکها;
		ext2fs_bg_free_بلوکها_شمار_مجموعه(fs, i, grp_free_بلوکها);
		ext2fs_bg_free_inodes_شمار_مجموعه(fs, i, fs->super->s_inodes_در_هر_گروه);
		ext2fs_bg_استفاده_شده_dirs_شمار_مجموعه(fs, i, 0);
		ext2fs_گروه_desc_csum_مجموعه(fs, i);
	}
	free_بلوکها &= ~EXT2_FS_CLUSTER_ماسک(fs);
	ext2fs_free_بلوکها_شمار_مجموعه(super, free_بلوکها);

	c = (حرف) 255;
	إذا (((صحيح) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} وإلا {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_مجموعه_blksize(fs->io, fs->blocksize);

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
	بیعلامت صحيح	گروه = 0;
	بیعلامت صحيح	شمار = 0;
	صحيح		مجموع_free = 0;
	صحيح		گروه_free = 0;
	صحيح		آخرین_allocated = 0;
	صحيح		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	لكل (blk = fs->super->s_اولی_داده_بلوک;
	     blk < ext2fs_بلوکها_شمار(fs->super); blk++) {
		إذا (!ext2fs_fast_test_بلوک_bitmap2(fs->بلوک_map, blk)) {
			گروه_free++;
			مجموع_free++;
		} وإلا {
			uninit = 0;
		}
		شمار++;
		إذا ((شمار == fs->super->s_بلوکها_در_هر_گروه) ||
		    (blk == ext2fs_بلوکها_شمار(fs->super)-1)) {
			ext2fs_bg_free_بلوکها_شمار_مجموعه(fs, گروه,
							گروه_free);
			إذا (!super_only) {
				إذا (uninit && blk !=
					ext2fs_بلوکها_شمار(fs->super) - 1)
					ext2fs_bg_flags_مجموعه(fs, گروه,
							EXT2_BG_بلوک_UNINIT);
				وإلا
					ext2fs_bg_flags_clear(fs, گروه,
							EXT2_BG_بلوک_UNINIT);
			}
			شمار = 0;
			گروه_free = 0;
			uninit = 1;
			گروه++;
		}
	}
	مجموع_free = EXT2FS_C2B(fs, مجموع_free);
	ext2fs_free_بلوکها_شمار_مجموعه(fs->super, مجموع_free);

	/*
	 * Next, calculate the inode statistics
	 */
	گروه_free = 0;
	مجموع_free = 0;
	آخرین_allocated = 0;
	شمار = 0;
	گروه = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	لكل (ino = 1; ino <= fs->super->s_inodes_شمار && ino > 0; ino++) {
		إذا (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			گروه_free++;
			مجموع_free++;
		} وإلا {
			آخرین_allocated = ino;
		}
		شمار++;
		إذا ((شمار == fs->super->s_inodes_در_هر_گروه) ||
		    (ino == fs->super->s_inodes_شمار)) {
			إذا (!super_only) {
				إذا (آخرین_allocated) {
					ext2fs_bg_flags_clear(fs, گروه,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_استفادهنشده_مجموعه(fs, گروه,
						fs->super->s_inodes_در_هر_گروه -
						(آخرین_allocated %
						fs->super->s_inodes_در_هر_گروه));
				} وإلا {
					ext2fs_bg_flags_مجموعه(fs, گروه,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_استفادهنشده_مجموعه(fs, گروه,
									0);
				}
				ext2fs_bg_free_inodes_شمار_مجموعه(fs, گروه,
								گروه_free);
			}
			گروه++;
			شمار = 0;
			گروه_free = 0;
			آخرین_allocated = 0;
		}
	}
	fs->super->s_free_inodes_شمار = مجموع_free;
	ext2fs_mark_super_dirty(fs);
	أرجع 0;
}
