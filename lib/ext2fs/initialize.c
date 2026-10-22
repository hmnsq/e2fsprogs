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

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

#నిర్వచితం_కాకపోతే O_ద్విమాన
#నిర్వచించు O_ద్విమాన 0
#అయితే_ముగింపు

#ఒకవేళ defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#నిర్వచించు CREATOR_OS EXT2_OS_LINUX
#లేకపోతే
#ఒకవేళ defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#నిర్వచించు CREATOR_OS EXT2_OS_HURD
#లేకపోతే
#ఒకవేళ defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#నిర్వచించు CREATOR_OS EXT2_OS_FREEBSD
#లేకపోతే
#ఒకవేళ defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#నిర్వచించు CREATOR_OS EXT2_OS_LITES
#లేకపోతే
#నిర్వచించు CREATOR_OS EXT2_OS_LINUX /* by default */
#అయితే_ముగింపు /* defined(LITES) && defined(EXT2_OS_LITES) */
#అయితే_ముగింపు /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#అయితే_ముగింపు /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#అయితే_ముగింపు /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
స్థిర గుర్తులేని పూర్ణాంకం calc_reserved_gdt_ఖండాలు(ext2_filsys fs)
{
	నిర్మాణం ext2_super_ఖండం *sb = fs->super;
	గుర్తులేని దీర్ఘ bpg = sb->s_ఖండాలు_చొప్పున_సమూహం;
	గుర్తులేని పూర్ణాంకం gdpb = EXT2_DESC_చొప్పున_ఖండం(sb);
	గుర్తులేని దీర్ఘ max_ఖండాలు = 0xffffffff;
	గుర్తులేని దీర్ఘ rsv_groups;
	గుర్తులేని పూర్ణాంకం rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	ఒకవేళ (ext2fs_ఖండాలు_లెక్క(sb) < max_ఖండాలు / 1024)
		max_ఖండాలు = ext2fs_ఖండాలు_లెక్క(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_ఖండాలు - sb->s_మొదటి_దత్తాంశం_ఖండం, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_ఖండాలు;
	ఒకవేళ (rsv_gdb > EXT2_ADDR_చొప్పున_ఖండం(sb))
		rsv_gdb = EXT2_ADDR_చొప్పున_ఖండం(sb);
#నిర్వచితమైతే RES_GDT_DEBUG
	రాయి("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_ఖండాలు, rsv_groups, rsv_gdb);
#అయితే_ముగింపు

	తిరిగి rsv_gdb;
}

errcode_t ext2fs_initialize(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags,
			    నిర్మాణం ext2_super_ఖండం *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	నిర్మాణం ext2_super_ఖండం *super;
	గుర్తులేని పూర్ణాంకం	rem;
	గుర్తులేని పూర్ణాంకం	overhead = 0;
	గుర్తులేని పూర్ణాంకం	ipg;
	dgrp_t		i;
	blk64_t		free_ఖండాలు;
	blk_t		numblocks;
	పూర్ణాంకం		rsv_gdt;
	పూర్ణాంకం		csum_flag;
	పూర్ణాంకం		bigalloc_flag;
	పూర్ణాంకం		io_flags;
	పూర్ణాంకం		has_bg;
	గుర్తులేని	reserved_inos;
	అక్షరం		*buf = 0;
	అక్షరం		c;
	ద్విగుణ		reserved_నిష్పత్తి;
	అక్షరం		*సమయం_env;

	ఒకవేళ (!param || !ext2fs_ఖండాలు_లెక్క(param))
		తిరిగి EXT2_ET_INVALID_వాదం;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం struct_ext2_filsys), &fs);
	ఒకవేళ (retval)
		తిరిగి retval;

	memset(fs, 0, పరిమాణం(నిర్మాణం struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	fs->flags |= EXT2_FLAG_మార్పిడి_బైట్లు;
#అయితే_ముగింపు

	సమయం_env = ext2fs_సురక్షిత_getenv("SOURCE_DATE_EPOCH");
	ఒకవేళ (సమయం_env) {
		fs->now = strtoul(సమయం_env, ఖాళీ, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_సమయం;
	} లేకపోతే {
		సమయం_env = ext2fs_సురక్షిత_getenv("E2FSPROGS_FAKE_TIME");
		ఒకవేళ (సమయం_env)
			fs->now = strtoul(సమయం_env, ఖాళీ, 0);
	}

	io_flags = IO_FLAG_RW;
	ఒకవేళ (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	ఒకవేళ (flags & EXT2_FLAG_ప్రత్యక్ష_IO)
		io_flags |= IO_FLAG_ప్రత్యక్ష_IO;
	io_flags |= O_ద్విమాన;
	retval = manager->open(name, io_flags, &fs->io);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	fs->image_io = fs->io;
	fs->io->app_దత్తాంశం = fs;
	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(name)+1, &fs->device_name);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_కొలత, &super);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_కొలత);

#నిర్వచించు సమితి_క్షేత్రం(క్షేత్రం, అప్రమేయ) (super->క్షేత్రం = param->క్షేత్రం ? \
				   param->క్షేత్రం : (అప్రమేయ))
#నిర్వచించు కేటాయించు_క్షేత్రం(క్షేత్రం)	(super->క్షేత్రం = param->క్షేత్రం)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	కేటాయించు_క్షేత్రం(s_log_ఖండం_కొలత);

	ఒకవేళ (bigalloc_flag) {
		సమితి_క్షేత్రం(s_log_cluster_కొలత, super->s_log_ఖండం_కొలత+4);
		ఒకవేళ (super->s_log_ఖండం_కొలత > super->s_log_cluster_కొలత) {
			retval = EXT2_ET_INVALID_వాదం;
			వెళ్ళు cleanup;
		}
	} లేకపోతే
		super->s_log_cluster_కొలత = super->s_log_ఖండం_కొలత;

	సమితి_క్షేత్రం(s_మొదటి_దత్తాంశం_ఖండం, super->s_log_cluster_కొలత ? 0 : 1);
	సమితి_క్షేత్రం(s_max_mnt_లెక్క, 0);
	సమితి_క్షేత్రం(s_errors, EXT2_ERRORS_DEFAULT);
	సమితి_క్షేత్రం(s_feature_compat, 0);
	సమితి_క్షేత్రం(s_feature_incompat, 0);
	సమితి_క్షేత్రం(s_feature_ro_compat, 0);
	సమితి_క్షేత్రం(s_default_mount_opts, 0);
	సమితి_క్షేత్రం(s_మొదటి_meta_bg, 0);
	సమితి_క్షేత్రం(s_raid_stride, 0);		/* default stride size: 0 */
	సమితి_క్షేత్రం(s_raid_stripe_వెడల్పు, 0);	/* default stripe width: 0 */
	సమితి_క్షేత్రం(s_log_groups_చొప్పున_flex, 0);
	సమితి_క్షేత్రం(s_flags, 0);
	కేటాయించు_క్షేత్రం(s_backup_bgs[0]);
	కేటాయించు_క్షేత్రం(s_backup_bgs[1]);

	కేటాయించు_క్షేత్రం(s_encoding);
	కేటాయించు_క్షేత్రం(s_encoding_flags);

	ఒకవేళ (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_ఎక్కించు_nls_పట్టిక(param->s_encoding);

	ఒకవేళ (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		వెళ్ళు cleanup;
	}
	ఒకవేళ (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		వెళ్ళు cleanup;
	}

	సమితి_క్షేత్రం(s_rev_level, EXT2_GOOD_OLD_REV);
	ఒకవేళ (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		సమితి_క్షేత్రం(s_మొదటి_ino, EXT2_GOOD_OLD_మొదటి_INO);
		సమితి_క్షేత్రం(s_inode_కొలత, EXT2_GOOD_OLD_INODE_కొలత);
		ఒకవేళ (super->s_inode_కొలత >= పరిమాణం(నిర్మాణం ext2_inode_large)) {
			పూర్ణాంకం extra_isize = పరిమాణం(నిర్మాణం ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_కొలత;
			సమితి_క్షేత్రం(s_min_extra_isize, extra_isize);
			సమితి_క్షేత్రం(s_want_extra_isize, extra_isize);
		}
	} లేకపోతే {
		super->s_మొదటి_ino = EXT2_GOOD_OLD_మొదటి_INO;
		super->s_inode_కొలత = EXT2_GOOD_OLD_INODE_కొలత;
	}

	సమితి_క్షేత్రం(s_checkinterval, 0);
	ext2fs_సమితి_tstamp(super, s_mkfs_సమయం, ext2fs_P_get_సమయం(fs));
	ext2fs_సమితి_tstamp(super, s_lastcheck, ext2fs_P_get_సమయం(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_ఖండం_కొలత(super);
	fs->cluster_నిష్పత్తి_బిట్లు = super->s_log_cluster_కొలత -
		super->s_log_ఖండం_కొలత;

	ఒకవేళ (bigalloc_flag) {
		గుర్తులేని దీర్ఘ దీర్ఘ bpg;

		ఒకవేళ (param->s_ఖండాలు_చొప్పున_సమూహం &&
		    param->s_clusters_చొప్పున_సమూహం &&
		    ((param->s_clusters_చొప్పున_సమూహం * EXT2_FS_CLUSTER_నిష్పత్తి(fs)) !=
		     param->s_ఖండాలు_చొప్పున_సమూహం)) {
			retval = EXT2_ET_INVALID_వాదం;
			వెళ్ళు cleanup;
		}
		ఒకవేళ (param->s_clusters_చొప్పున_సమూహం)
			కేటాయించు_క్షేత్రం(s_clusters_చొప్పున_సమూహం);
		లేకపోతే ఒకవేళ (param->s_ఖండాలు_చొప్పున_సమూహం)
			super->s_clusters_చొప్పున_సమూహం = 
				param->s_ఖండాలు_చొప్పున_సమూహం /
				EXT2_FS_CLUSTER_నిష్పత్తి(fs);
		లేకపోతే ఒకవేళ (super->s_log_cluster_కొలత + 15 < 32)
			super->s_clusters_చొప్పున_సమూహం = fs->blocksize * 8;
		లేకపోతే
			super->s_clusters_చొప్పున_సమూహం = (fs->blocksize - 1) * 8;
		ఒకవేళ (super->s_clusters_చొప్పున_సమూహం > EXT2_MAX_CLUSTERS_చొప్పున_సమూహం(super))
			super->s_clusters_చొప్పున_సమూహం = EXT2_MAX_CLUSTERS_చొప్పున_సమూహం(super);
		bpg = EXT2FS_C2B(fs,
			(గుర్తులేని దీర్ఘ దీర్ఘ) super->s_clusters_చొప్పున_సమూహం);
		ఒకవేళ (bpg >= (((గుర్తులేని దీర్ఘ దీర్ఘ) 1) << 32)) {
			retval = EXT2_ET_INVALID_వాదం;
			వెళ్ళు cleanup;
		}
		super->s_ఖండాలు_చొప్పున_సమూహం = bpg;
	} లేకపోతే {
		సమితి_క్షేత్రం(s_ఖండాలు_చొప్పున_సమూహం, fs->blocksize * 8);
		ఒకవేళ (super->s_ఖండాలు_చొప్పున_సమూహం > EXT2_MAX_ఖండాలు_చొప్పున_సమూహం(super))
			super->s_ఖండాలు_చొప్పున_సమూహం = EXT2_MAX_ఖండాలు_చొప్పున_సమూహం(super);
		super->s_clusters_చొప్పున_సమూహం = super->s_ఖండాలు_చొప్పున_సమూహం;
	}

	ext2fs_ఖండాలు_లెక్క_సమితి(super, ext2fs_ఖండాలు_లెక్క(param) &
				~((blk64_t) EXT2_FS_CLUSTER_ముసుగు(fs)));
	ext2fs_r_ఖండాలు_లెక్క_సమితి(super, ext2fs_r_ఖండాలు_లెక్క(param));
	ఒకవేళ (ext2fs_r_ఖండాలు_లెక్క(super) >= ext2fs_ఖండాలు_లెక్క(param)) {
		retval = EXT2_ET_INVALID_వాదం;
		వెళ్ళు cleanup;
	}

	సమితి_క్షేత్రం(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	ఒకవేళ (ext2fs_has_feature_journal_dev(super)) {
		fs->సమూహం_desc_లెక్క = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		తిరిగి 0;
	}

retry:
	fs->సమూహం_desc_లెక్క = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_ఖండాలు_లెక్క(super) - super->s_మొదటి_దత్తాంశం_ఖండం,
		EXT2_ఖండాలు_చొప్పున_సమూహం(super));
	ఒకవేళ (fs->సమూహం_desc_లెక్క == 0) {
		retval = EXT2_ET_TOOSMALL;
		వెళ్ళు cleanup;
	}

	సమితి_క్షేత్రం(s_desc_కొలత,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_కొలత_64_బిట్టు : 0);

	fs->desc_ఖండాలు = ext2fs_div_ceil(fs->సమూహం_desc_లెక్క,
					  EXT2_DESC_చొప్పున_ఖండం(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	ఒకవేళ (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_ఖండాలు_లెక్క(super) / i) >= (1ULL << 32))
		సమితి_క్షేత్రం(s_inodes_లెక్క, ~0U);
	లేకపోతే
		సమితి_క్షేత్రం(s_inodes_లెక్క, ext2fs_ఖండాలు_లెక్క(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_లెక్క, fs->సమూహం_desc_లెక్క);
	ఒకవేళ (ipg > fs->blocksize * 8) {
		ఒకవేళ (!bigalloc_flag && super->s_ఖండాలు_చొప్పున_సమూహం >= 256) {
			/* Try again with slightly different parameters */
			super->s_ఖండాలు_చొప్పున_సమూహం -= 8;
			ext2fs_ఖండాలు_లెక్క_సమితి(super,
						ext2fs_ఖండాలు_లెక్క(param));
			super->s_clusters_చొప్పున_సమూహం = super->s_ఖండాలు_చొప్పున_సమూహం;
			వెళ్ళు retry;
		} లేకపోతే {
			retval = EXT2_ET_TOO_MANY_INODES;
			వెళ్ళు cleanup;
		}
	}

	ఒకవేళ (ipg > (గుర్తులేని) EXT2_MAX_INODES_చొప్పున_సమూహం(super))
		ipg = EXT2_MAX_INODES_చొప్పున_సమూహం(super);

ipg_retry:
	super->s_inodes_చొప్పున_సమూహం = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_ఖండాలు_చొప్పున_సమూహం = (((super->s_inodes_చొప్పున_సమూహం *
					EXT2_INODE_కొలత(super)) +
				       EXT2_ఖండం_కొలత(super) - 1) /
				      EXT2_ఖండం_కొలత(super));
	super->s_inodes_చొప్పున_సమూహం = ((fs->inode_ఖండాలు_చొప్పున_సమూహం *
				      EXT2_ఖండం_కొలత(super)) /
				     EXT2_INODE_కొలత(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	ఒకవేళ (super->s_inodes_చొప్పున_సమూహం < 8)
		super->s_inodes_చొప్పున_సమూహం = 8;
	super->s_inodes_చొప్పున_సమూహం &= ~7;
	fs->inode_ఖండాలు_చొప్పున_సమూహం = (((super->s_inodes_చొప్పున_సమూహం *
					EXT2_INODE_కొలత(super)) +
				       EXT2_ఖండం_కొలత(super) - 1) /
				      EXT2_ఖండం_కొలత(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	ఒకవేళ ((__u64)super->s_inodes_చొప్పున_సమూహం * fs->సమూహం_desc_లెక్క > ~0U) {
		ipg--;
		వెళ్ళు ipg_retry;
	}
	super->s_inodes_లెక్క = super->s_inodes_చొప్పున_సమూహం *
		fs->సమూహం_desc_లెక్క;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	ఒకవేళ (super->s_inodes_లెక్క < EXT2_మొదటి_INODE(super)+1) {
		ipg += 8;
		వెళ్ళు ipg_retry;
	}

	super->s_free_inodes_లెక్క = super->s_inodes_లెక్క;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	ఒకవేళ (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_ఖండాలు(fs);
	లేకపోతే
		rsv_gdt = 0;
	సమితి_క్షేత్రం(s_reserved_gdt_ఖండాలు, rsv_gdt);
	ఒకవేళ (super->s_reserved_gdt_ఖండాలు > EXT2_ADDR_చొప్పున_ఖండం(super)) {
		retval = EXT2_ET_RES_GDT_ఖండాలు;
		వెళ్ళు cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	ఒకవేళ (super->s_reserved_gdt_ఖండాలు + fs->desc_ఖండాలు >
	    super->s_ఖండాలు_చొప్పున_సమూహం * 3 / 4) {
		ext2fs_సమితి_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		సమితి_క్షేత్రం(s_reserved_gdt_ఖండాలు, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (పూర్ణాంకం) (3 + fs->inode_ఖండాలు_చొప్పున_సమూహం +
			  super->s_reserved_gdt_ఖండాలు);

	ఒకవేళ (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	లేకపోతే
		overhead += fs->desc_ఖండాలు;

	/* This can only happen if the user requested too many inodes */
	ఒకవేళ (overhead > super->s_ఖండాలు_చొప్పున_సమూహం) {
		retval = EXT2_ET_TOO_MANY_INODES;
		వెళ్ళు cleanup;
	}

	/*
	 * See if the last group is big enough to support the
	 * necessary data structures.  If not, we need to get rid of
	 * it.  We need to recalculate the overhead for the last block
	 * group, since it might or might not have a superblock
	 * backup.
	 */
	overhead = (పూర్ణాంకం) (2 + fs->inode_ఖండాలు_చొప్పున_సమూహం);
	has_bg = 0;
	ఒకవేళ (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		ఒకవేళ (fs->సమూహం_desc_లెక్క == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		లేకపోతే
			has_bg = param->s_backup_bgs[1] != 0;
	} లేకపోతే
		has_bg = ext2fs_bg_has_super(fs, fs->సమూహం_desc_లెక్క - 1);
	ఒకవేళ (has_bg)
		overhead += 1 + fs->desc_ఖండాలు + super->s_reserved_gdt_ఖండాలు;
	rem = ((ext2fs_ఖండాలు_లెక్క(super) - super->s_మొదటి_దత్తాంశం_ఖండం) %
	       super->s_ఖండాలు_చొప్పున_సమూహం);
	ఒకవేళ ((fs->సమూహం_desc_లెక్క == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		వెళ్ళు cleanup;
	}
	ఒకవేళ (rem && (rem < overhead+50)) {
		ext2fs_ఖండాలు_లెక్క_సమితి(super, ext2fs_ఖండాలు_లెక్క(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_నిష్పత్తి = 100.0 * ext2fs_r_ఖండాలు_లెక్క(param) /
			ext2fs_ఖండాలు_లెక్క(param);
		ext2fs_r_ఖండాలు_లెక్క_సమితి(super, reserved_నిష్పత్తి *
			ext2fs_ఖండాలు_లెక్క(super) / 100.0);

		వెళ్ళు retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	ఒకవేళ (ext2fs_has_feature_sparse_super2(super)) {
		ఒకవేళ (super->s_backup_bgs[0] >= fs->సమూహం_desc_లెక్క)
			super->s_backup_bgs[0] = fs->సమూహం_desc_లెక్క - 1;
		ఒకవేళ (super->s_backup_bgs[1] >= fs->సమూహం_desc_లెక్క)
			super->s_backup_bgs[1] = fs->సమూహం_desc_లెక్క - 1;
		ఒకవేళ (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		ఒకవేళ (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(fs->device_name) + 80, &buf);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->ఖండం_map);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_ఖండాలు, fs->blocksize,
				&fs->సమూహం_desc);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	memset(fs->సమూహం_desc, 0, (size_t) fs->desc_ఖండాలు * fs->blocksize);

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
	free_ఖండాలు = 0;
	csum_flag = ext2fs_has_సమూహం_desc_csum(fs);
	reserved_inos = super->s_మొదటి_ino;
	కొరకు (i = 0; i < fs->సమూహం_desc_లెక్క; i++) {
		blk_t grp_free_ఖండాలు;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->ఖండం_map,
						       &numblocks);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		ఒకవేళ (csum_flag) {
			ఒకవేళ (i != fs->సమూహం_desc_లెక్క - 1 && numblocks == 0)
				ext2fs_bg_flags_సమితి(fs, i,
						    EXT2_BG_ఖండం_UNINIT);
			ext2fs_bg_flags_సమితి(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_చొప్పున_సమూహం;
			ఒకవేళ (reserved_inos) {
				ఒకవేళ (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} లేకపోతే {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_వాడని_సమితి(fs, i, inodes);
		}

		ఒకవేళ (!fs->super->s_log_groups_చొప్పున_flex)
			numblocks += 2 + fs->inode_ఖండాలు_చొప్పున_సమూహం;

		grp_free_ఖండాలు = ext2fs_సమూహం_ఖండాలు_లెక్క(fs, i) - numblocks;
		free_ఖండాలు += grp_free_ఖండాలు;
		ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(fs, i, grp_free_ఖండాలు);
		ext2fs_bg_free_inodes_లెక్క_సమితి(fs, i, fs->super->s_inodes_చొప్పున_సమూహం);
		ext2fs_bg_వాడిన_dirs_లెక్క_సమితి(fs, i, 0);
		ext2fs_సమూహం_desc_csum_సమితి(fs, i);
	}
	free_ఖండాలు &= ~EXT2_FS_CLUSTER_ముసుగు(fs);
	ext2fs_free_ఖండాలు_లెక్క_సమితి(super, free_ఖండాలు);

	c = (అక్షరం) 255;
	ఒకవేళ (((పూర్ణాంకం) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} లేకపోతే {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_సమితి_blksize(fs->io, fs->blocksize);

	*ret_fs = fs;
	తిరిగి 0;
cleanup:
	విడుదల(buf);
	ext2fs_free(fs);
	తిరిగి retval;
}

errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, పూర్ణాంకం super_only)
{
	blk64_t		blk;
	ext2_ino_t	ino;
	గుర్తులేని పూర్ణాంకం	సమూహం = 0;
	గుర్తులేని పూర్ణాంకం	లెక్క = 0;
	పూర్ణాంకం		మొత్తం_free = 0;
	పూర్ణాంకం		సమూహం_free = 0;
	పూర్ణాంకం		చివరి_allocated = 0;
	పూర్ణాంకం		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	కొరకు (blk = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
	     blk < ext2fs_ఖండాలు_లెక్క(fs->super); blk++) {
		ఒకవేళ (!ext2fs_fast_test_ఖండం_bitmap2(fs->ఖండం_map, blk)) {
			సమూహం_free++;
			మొత్తం_free++;
		} లేకపోతే {
			uninit = 0;
		}
		లెక్క++;
		ఒకవేళ ((లెక్క == fs->super->s_ఖండాలు_చొప్పున_సమూహం) ||
		    (blk == ext2fs_ఖండాలు_లెక్క(fs->super)-1)) {
			ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(fs, సమూహం,
							సమూహం_free);
			ఒకవేళ (!super_only) {
				ఒకవేళ (uninit && blk !=
					ext2fs_ఖండాలు_లెక్క(fs->super) - 1)
					ext2fs_bg_flags_సమితి(fs, సమూహం,
							EXT2_BG_ఖండం_UNINIT);
				లేకపోతే
					ext2fs_bg_flags_clear(fs, సమూహం,
							EXT2_BG_ఖండం_UNINIT);
			}
			లెక్క = 0;
			సమూహం_free = 0;
			uninit = 1;
			సమూహం++;
		}
	}
	మొత్తం_free = EXT2FS_C2B(fs, మొత్తం_free);
	ext2fs_free_ఖండాలు_లెక్క_సమితి(fs->super, మొత్తం_free);

	/*
	 * Next, calculate the inode statistics
	 */
	సమూహం_free = 0;
	మొత్తం_free = 0;
	చివరి_allocated = 0;
	లెక్క = 0;
	సమూహం = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	కొరకు (ino = 1; ino <= fs->super->s_inodes_లెక్క && ino > 0; ino++) {
		ఒకవేళ (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			సమూహం_free++;
			మొత్తం_free++;
		} లేకపోతే {
			చివరి_allocated = ino;
		}
		లెక్క++;
		ఒకవేళ ((లెక్క == fs->super->s_inodes_చొప్పున_సమూహం) ||
		    (ino == fs->super->s_inodes_లెక్క)) {
			ఒకవేళ (!super_only) {
				ఒకవేళ (చివరి_allocated) {
					ext2fs_bg_flags_clear(fs, సమూహం,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_వాడని_సమితి(fs, సమూహం,
						fs->super->s_inodes_చొప్పున_సమూహం -
						(చివరి_allocated %
						fs->super->s_inodes_చొప్పున_సమూహం));
				} లేకపోతే {
					ext2fs_bg_flags_సమితి(fs, సమూహం,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_వాడని_సమితి(fs, సమూహం,
									0);
				}
				ext2fs_bg_free_inodes_లెక్క_సమితి(fs, సమూహం,
								సమూహం_free);
			}
			సమూహం++;
			లెక్క = 0;
			సమూహం_free = 0;
			చివరి_allocated = 0;
		}
	}
	fs->super->s_free_inodes_లెక్క = మొత్తం_free;
	ext2fs_mark_super_dirty(fs);
	తిరిగి 0;
}
