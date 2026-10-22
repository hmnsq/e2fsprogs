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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

#ifndef O_ദ്വിമാന
#define O_ദ്വിമാന 0
#endif

#എങ്കിൽ defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#define CREATOR_OS EXT2_OS_LINUX
#അല്ലെങ്കിൽ
#എങ്കിൽ defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#define CREATOR_OS EXT2_OS_HURD
#അല്ലെങ്കിൽ
#എങ്കിൽ defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#define CREATOR_OS EXT2_OS_FREEBSD
#അല്ലെങ്കിൽ
#എങ്കിൽ defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#define CREATOR_OS EXT2_OS_LITES
#അല്ലെങ്കിൽ
#define CREATOR_OS EXT2_OS_LINUX /* by default */
#endif /* defined(LITES) && defined(EXT2_OS_LITES) */
#endif /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#endif /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#endif /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ calc_reserved_gdt_blocks(ext2_filsys fs)
{
	ഘടന ext2_super_block *sb = fs->super;
	unsigned long bpg = sb->s_blocks_per_group;
	unsigned പൂർണ്ണസംഖ്യ gdpb = EXT2_DESC_PER_BLOCK(sb);
	unsigned long max_blocks = 0xffffffff;
	unsigned long rsv_groups;
	unsigned പൂർണ്ണസംഖ്യ rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	എങ്കിൽ (ext2fs_blocks_എണ്ണം(sb) < max_blocks / 1024)
		max_blocks = ext2fs_blocks_എണ്ണം(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_blocks - sb->s_first_data_block, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_blocks;
	എങ്കിൽ (rsv_gdb > EXT2_ADDR_PER_BLOCK(sb))
		rsv_gdb = EXT2_ADDR_PER_BLOCK(sb);
#ifdef RES_GDT_DEBUG
	printf("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_blocks, rsv_groups, rsv_gdb);
#endif

	തിരികെ rsv_gdb;
}

errcode_t ext2fs_initialize(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags,
			    ഘടന ext2_super_block *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	ഘടന ext2_super_block *super;
	unsigned പൂർണ്ണസംഖ്യ	rem;
	unsigned പൂർണ്ണസംഖ്യ	overhead = 0;
	unsigned പൂർണ്ണസംഖ്യ	ipg;
	dgrp_t		i;
	blk64_t		free_blocks;
	blk_t		numblocks;
	പൂർണ്ണസംഖ്യ		rsv_gdt;
	പൂർണ്ണസംഖ്യ		csum_flag;
	പൂർണ്ണസംഖ്യ		bigalloc_flag;
	പൂർണ്ണസംഖ്യ		io_flags;
	പൂർണ്ണസംഖ്യ		has_bg;
	unsigned	reserved_inos;
	അക്ഷരം		*buf = 0;
	അക്ഷരം		c;
	ഇരട്ട		reserved_ratio;
	അക്ഷരം		*time_env;

	എങ്കിൽ (!param || !ext2fs_blocks_എണ്ണം(param))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന struct_ext2_filsys), &fs);
	എങ്കിൽ (retval)
		തിരികെ retval;

	memset(fs, 0, വലുപ്പം(ഘടന struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#ifdef വാക്കുകൾ_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
#endif

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	എങ്കിൽ (time_env) {
		fs->now = strtoul(time_env, ശൂന്യ, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} അല്ലെങ്കിൽ {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		എങ്കിൽ (time_env)
			fs->now = strtoul(time_env, ശൂന്യ, 0);
	}

	io_flags = IO_FLAG_RW;
	എങ്കിൽ (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	എങ്കിൽ (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	io_flags |= O_ദ്വിമാന;
	retval = manager->open(name, io_flags, &fs->io);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	fs->image_io = fs->io;
	fs->io->app_data = fs;
	retval = ext2fs_get_mem(strlen(name)+1, &fs->device_name);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &super);
	എങ്കിൽ (retval)
		പോകുക cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_SIZE);

#define set_field(field, സ്ഥിരസ്ഥിതി) (super->field = param->field ? \
				   param->field : (സ്ഥിരസ്ഥിതി))
#define assign_field(field)	(super->field = param->field)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	assign_field(s_log_block_size);

	എങ്കിൽ (bigalloc_flag) {
		set_field(s_log_cluster_size, super->s_log_block_size+4);
		എങ്കിൽ (super->s_log_block_size > super->s_log_cluster_size) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			പോകുക cleanup;
		}
	} അല്ലെങ്കിൽ
		super->s_log_cluster_size = super->s_log_block_size;

	set_field(s_first_data_block, super->s_log_cluster_size ? 0 : 1);
	set_field(s_max_mnt_എണ്ണം, 0);
	set_field(s_errors, EXT2_ERRORS_DEFAULT);
	set_field(s_feature_compat, 0);
	set_field(s_feature_incompat, 0);
	set_field(s_feature_ro_compat, 0);
	set_field(s_default_mount_opts, 0);
	set_field(s_first_meta_bg, 0);
	set_field(s_raid_stride, 0);		/* default stride size: 0 */
	set_field(s_raid_stripe_width, 0);	/* default stripe width: 0 */
	set_field(s_log_groups_per_flex, 0);
	set_field(s_flags, 0);
	assign_field(s_backup_bgs[0]);
	assign_field(s_backup_bgs[1]);

	assign_field(s_encoding);
	assign_field(s_encoding_flags);

	എങ്കിൽ (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_load_nls_table(param->s_encoding);

	എങ്കിൽ (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		പോകുക cleanup;
	}
	എങ്കിൽ (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		പോകുക cleanup;
	}

	set_field(s_rev_level, EXT2_GOOD_OLD_REV);
	എങ്കിൽ (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		set_field(s_first_ino, EXT2_GOOD_OLD_FIRST_INO);
		set_field(s_inode_size, EXT2_GOOD_OLD_INODE_SIZE);
		എങ്കിൽ (super->s_inode_size >= വലുപ്പം(ഘടന ext2_inode_large)) {
			പൂർണ്ണസംഖ്യ extra_isize = വലുപ്പം(ഘടന ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_SIZE;
			set_field(s_min_extra_isize, extra_isize);
			set_field(s_want_extra_isize, extra_isize);
		}
	} അല്ലെങ്കിൽ {
		super->s_first_ino = EXT2_GOOD_OLD_FIRST_INO;
		super->s_inode_size = EXT2_GOOD_OLD_INODE_SIZE;
	}

	set_field(s_checkinterval, 0);
	ext2fs_set_tstamp(super, s_mkfs_time, ext2fsP_get_time(fs));
	ext2fs_set_tstamp(super, s_lastcheck, ext2fsP_get_time(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_BLOCK_SIZE(super);
	fs->cluster_ratio_bits = super->s_log_cluster_size -
		super->s_log_block_size;

	എങ്കിൽ (bigalloc_flag) {
		unsigned long long bpg;

		എങ്കിൽ (param->s_blocks_per_group &&
		    param->s_clusters_per_group &&
		    ((param->s_clusters_per_group * EXT2FS_CLUSTER_RATIO(fs)) !=
		     param->s_blocks_per_group)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			പോകുക cleanup;
		}
		എങ്കിൽ (param->s_clusters_per_group)
			assign_field(s_clusters_per_group);
		അല്ലെങ്കിൽ എങ്കിൽ (param->s_blocks_per_group)
			super->s_clusters_per_group = 
				param->s_blocks_per_group /
				EXT2FS_CLUSTER_RATIO(fs);
		അല്ലെങ്കിൽ എങ്കിൽ (super->s_log_cluster_size + 15 < 32)
			super->s_clusters_per_group = fs->blocksize * 8;
		അല്ലെങ്കിൽ
			super->s_clusters_per_group = (fs->blocksize - 1) * 8;
		എങ്കിൽ (super->s_clusters_per_group > EXT2_MAX_CLUSTERS_PER_GROUP(super))
			super->s_clusters_per_group = EXT2_MAX_CLUSTERS_PER_GROUP(super);
		bpg = EXT2FS_C2B(fs,
			(unsigned long long) super->s_clusters_per_group);
		എങ്കിൽ (bpg >= (((unsigned long long) 1) << 32)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			പോകുക cleanup;
		}
		super->s_blocks_per_group = bpg;
	} അല്ലെങ്കിൽ {
		set_field(s_blocks_per_group, fs->blocksize * 8);
		എങ്കിൽ (super->s_blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(super))
			super->s_blocks_per_group = EXT2_MAX_BLOCKS_PER_GROUP(super);
		super->s_clusters_per_group = super->s_blocks_per_group;
	}

	ext2fs_blocks_എണ്ണം_set(super, ext2fs_blocks_എണ്ണം(param) &
				~((blk64_t) EXT2FS_CLUSTER_MASK(fs)));
	ext2fs_r_blocks_എണ്ണം_set(super, ext2fs_r_blocks_എണ്ണം(param));
	എങ്കിൽ (ext2fs_r_blocks_എണ്ണം(super) >= ext2fs_blocks_എണ്ണം(param)) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		പോകുക cleanup;
	}

	set_field(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	എങ്കിൽ (ext2fs_has_feature_journal_dev(super)) {
		fs->group_desc_എണ്ണം = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		തിരികെ 0;
	}

retry:
	fs->group_desc_എണ്ണം = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_blocks_എണ്ണം(super) - super->s_first_data_block,
		EXT2_BLOCKS_PER_GROUP(super));
	എങ്കിൽ (fs->group_desc_എണ്ണം == 0) {
		retval = EXT2_ET_TOOSMALL;
		പോകുക cleanup;
	}

	set_field(s_desc_size,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_SIZE_64BIT : 0);

	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_എണ്ണം,
					  EXT2_DESC_PER_BLOCK(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	എങ്കിൽ (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_blocks_എണ്ണം(super) / i) >= (1ULL << 32))
		set_field(s_inodes_എണ്ണം, ~0U);
	അല്ലെങ്കിൽ
		set_field(s_inodes_എണ്ണം, ext2fs_blocks_എണ്ണം(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_എണ്ണം, fs->group_desc_എണ്ണം);
	എങ്കിൽ (ipg > fs->blocksize * 8) {
		എങ്കിൽ (!bigalloc_flag && super->s_blocks_per_group >= 256) {
			/* Try again with slightly different parameters */
			super->s_blocks_per_group -= 8;
			ext2fs_blocks_എണ്ണം_set(super,
						ext2fs_blocks_എണ്ണം(param));
			super->s_clusters_per_group = super->s_blocks_per_group;
			പോകുക retry;
		} അല്ലെങ്കിൽ {
			retval = EXT2_ET_TOO_MANY_INODES;
			പോകുക cleanup;
		}
	}

	എങ്കിൽ (ipg > (unsigned) EXT2_MAX_INODES_PER_GROUP(super))
		ipg = EXT2_MAX_INODES_PER_GROUP(super);

ipg_retry:
	super->s_inodes_per_group = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_blocks_per_group = (((super->s_inodes_per_group *
					EXT2_INODE_SIZE(super)) +
				       EXT2_BLOCK_SIZE(super) - 1) /
				      EXT2_BLOCK_SIZE(super));
	super->s_inodes_per_group = ((fs->inode_blocks_per_group *
				      EXT2_BLOCK_SIZE(super)) /
				     EXT2_INODE_SIZE(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	എങ്കിൽ (super->s_inodes_per_group < 8)
		super->s_inodes_per_group = 8;
	super->s_inodes_per_group &= ~7;
	fs->inode_blocks_per_group = (((super->s_inodes_per_group *
					EXT2_INODE_SIZE(super)) +
				       EXT2_BLOCK_SIZE(super) - 1) /
				      EXT2_BLOCK_SIZE(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	എങ്കിൽ ((__u64)super->s_inodes_per_group * fs->group_desc_എണ്ണം > ~0U) {
		ipg--;
		പോകുക ipg_retry;
	}
	super->s_inodes_എണ്ണം = super->s_inodes_per_group *
		fs->group_desc_എണ്ണം;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	എങ്കിൽ (super->s_inodes_എണ്ണം < EXT2_FIRST_INODE(super)+1) {
		ipg += 8;
		പോകുക ipg_retry;
	}

	super->s_free_inodes_എണ്ണം = super->s_inodes_എണ്ണം;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	എങ്കിൽ (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_blocks(fs);
	അല്ലെങ്കിൽ
		rsv_gdt = 0;
	set_field(s_reserved_gdt_blocks, rsv_gdt);
	എങ്കിൽ (super->s_reserved_gdt_blocks > EXT2_ADDR_PER_BLOCK(super)) {
		retval = EXT2_ET_RES_GDT_BLOCKS;
		പോകുക cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	എങ്കിൽ (super->s_reserved_gdt_blocks + fs->desc_blocks >
	    super->s_blocks_per_group * 3 / 4) {
		ext2fs_set_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		set_field(s_reserved_gdt_blocks, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (പൂർണ്ണസംഖ്യ) (3 + fs->inode_blocks_per_group +
			  super->s_reserved_gdt_blocks);

	എങ്കിൽ (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	അല്ലെങ്കിൽ
		overhead += fs->desc_blocks;

	/* This can only happen if the user requested too many inodes */
	എങ്കിൽ (overhead > super->s_blocks_per_group) {
		retval = EXT2_ET_TOO_MANY_INODES;
		പോകുക cleanup;
	}

	/*
	 * See if the last group is big enough to support the
	 * necessary data structures.  If not, we need to get rid of
	 * it.  We need to recalculate the overhead for the last block
	 * group, since it might or might not have a superblock
	 * backup.
	 */
	overhead = (പൂർണ്ണസംഖ്യ) (2 + fs->inode_blocks_per_group);
	has_bg = 0;
	എങ്കിൽ (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		എങ്കിൽ (fs->group_desc_എണ്ണം == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		അല്ലെങ്കിൽ
			has_bg = param->s_backup_bgs[1] != 0;
	} അല്ലെങ്കിൽ
		has_bg = ext2fs_bg_has_super(fs, fs->group_desc_എണ്ണം - 1);
	എങ്കിൽ (has_bg)
		overhead += 1 + fs->desc_blocks + super->s_reserved_gdt_blocks;
	rem = ((ext2fs_blocks_എണ്ണം(super) - super->s_first_data_block) %
	       super->s_blocks_per_group);
	എങ്കിൽ ((fs->group_desc_എണ്ണം == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		പോകുക cleanup;
	}
	എങ്കിൽ (rem && (rem < overhead+50)) {
		ext2fs_blocks_എണ്ണം_set(super, ext2fs_blocks_എണ്ണം(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_ratio = 100.0 * ext2fs_r_blocks_എണ്ണം(param) /
			ext2fs_blocks_എണ്ണം(param);
		ext2fs_r_blocks_എണ്ണം_set(super, reserved_ratio *
			ext2fs_blocks_എണ്ണം(super) / 100.0);

		പോകുക retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	എങ്കിൽ (ext2fs_has_feature_sparse_super2(super)) {
		എങ്കിൽ (super->s_backup_bgs[0] >= fs->group_desc_എണ്ണം)
			super->s_backup_bgs[0] = fs->group_desc_എണ്ണം - 1;
		എങ്കിൽ (super->s_backup_bgs[1] >= fs->group_desc_എണ്ണം)
			super->s_backup_bgs[1] = fs->group_desc_എണ്ണം - 1;
		എങ്കിൽ (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		എങ്കിൽ (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->block_map);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	എങ്കിൽ (retval)
		പോകുക cleanup;

	memset(fs->group_desc, 0, (size_t) fs->desc_blocks * fs->blocksize);

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
	free_blocks = 0;
	csum_flag = ext2fs_has_group_desc_csum(fs);
	reserved_inos = super->s_first_ino;
	വേണ്ടി (i = 0; i < fs->group_desc_എണ്ണം; i++) {
		blk_t grp_free_blocks;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->block_map,
						       &numblocks);
		എങ്കിൽ (retval)
			പോകുക cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		എങ്കിൽ (csum_flag) {
			എങ്കിൽ (i != fs->group_desc_എണ്ണം - 1 && numblocks == 0)
				ext2fs_bg_flags_set(fs, i,
						    EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_per_group;
			എങ്കിൽ (reserved_inos) {
				എങ്കിൽ (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} അല്ലെങ്കിൽ {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_unused_set(fs, i, inodes);
		}

		എങ്കിൽ (!fs->super->s_log_groups_per_flex)
			numblocks += 2 + fs->inode_blocks_per_group;

		grp_free_blocks = ext2fs_group_blocks_എണ്ണം(fs, i) - numblocks;
		free_blocks += grp_free_blocks;
		ext2fs_bg_free_blocks_എണ്ണം_set(fs, i, grp_free_blocks);
		ext2fs_bg_free_inodes_എണ്ണം_set(fs, i, fs->super->s_inodes_per_group);
		ext2fs_bg_used_dirs_എണ്ണം_set(fs, i, 0);
		ext2fs_group_desc_csum_set(fs, i);
	}
	free_blocks &= ~EXT2FS_CLUSTER_MASK(fs);
	ext2fs_free_blocks_എണ്ണം_set(super, free_blocks);

	c = (അക്ഷരം) 255;
	എങ്കിൽ (((പൂർണ്ണസംഖ്യ) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} അല്ലെങ്കിൽ {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_set_blksize(fs->io, fs->blocksize);

	*ret_fs = fs;
	തിരികെ 0;
cleanup:
	സ്വതന്ത്രം(buf);
	ext2fs_free(fs);
	തിരികെ retval;
}

errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, പൂർണ്ണസംഖ്യ super_only)
{
	blk64_t		blk;
	ext2_ino_t	ino;
	unsigned പൂർണ്ണസംഖ്യ	group = 0;
	unsigned പൂർണ്ണസംഖ്യ	എണ്ണം = 0;
	പൂർണ്ണസംഖ്യ		total_free = 0;
	പൂർണ്ണസംഖ്യ		group_free = 0;
	പൂർണ്ണസംഖ്യ		last_allocated = 0;
	പൂർണ്ണസംഖ്യ		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	വേണ്ടി (blk = fs->super->s_first_data_block;
	     blk < ext2fs_blocks_എണ്ണം(fs->super); blk++) {
		എങ്കിൽ (!ext2fs_fast_test_block_bitmap2(fs->block_map, blk)) {
			group_free++;
			total_free++;
		} അല്ലെങ്കിൽ {
			uninit = 0;
		}
		എണ്ണം++;
		എങ്കിൽ ((എണ്ണം == fs->super->s_blocks_per_group) ||
		    (blk == ext2fs_blocks_എണ്ണം(fs->super)-1)) {
			ext2fs_bg_free_blocks_എണ്ണം_set(fs, group,
							group_free);
			എങ്കിൽ (!super_only) {
				എങ്കിൽ (uninit && blk !=
					ext2fs_blocks_എണ്ണം(fs->super) - 1)
					ext2fs_bg_flags_set(fs, group,
							EXT2_BG_BLOCK_UNINIT);
				അല്ലെങ്കിൽ
					ext2fs_bg_flags_clear(fs, group,
							EXT2_BG_BLOCK_UNINIT);
			}
			എണ്ണം = 0;
			group_free = 0;
			uninit = 1;
			group++;
		}
	}
	total_free = EXT2FS_C2B(fs, total_free);
	ext2fs_free_blocks_എണ്ണം_set(fs->super, total_free);

	/*
	 * Next, calculate the inode statistics
	 */
	group_free = 0;
	total_free = 0;
	last_allocated = 0;
	എണ്ണം = 0;
	group = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	വേണ്ടി (ino = 1; ino <= fs->super->s_inodes_എണ്ണം && ino > 0; ino++) {
		എങ്കിൽ (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			group_free++;
			total_free++;
		} അല്ലെങ്കിൽ {
			last_allocated = ino;
		}
		എണ്ണം++;
		എങ്കിൽ ((എണ്ണം == fs->super->s_inodes_per_group) ||
		    (ino == fs->super->s_inodes_എണ്ണം)) {
			എങ്കിൽ (!super_only) {
				എങ്കിൽ (last_allocated) {
					ext2fs_bg_flags_clear(fs, group,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_unused_set(fs, group,
						fs->super->s_inodes_per_group -
						(last_allocated %
						fs->super->s_inodes_per_group));
				} അല്ലെങ്കിൽ {
					ext2fs_bg_flags_set(fs, group,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_unused_set(fs, group,
									0);
				}
				ext2fs_bg_free_inodes_എണ്ണം_set(fs, group,
								group_free);
			}
			group++;
			എണ്ണം = 0;
			group_free = 0;
			last_allocated = 0;
		}
	}
	fs->super->s_free_inodes_എണ്ണം = total_free;
	ext2fs_mark_super_dirty(fs);
	തിരികെ 0;
}
