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

#include "config.h"
#include <stdio.h>
#include <string.h>
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#အကယ်၍ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#အကယ်၍ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#အကယ်၍ defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#define CREATOR_OS EXT2_OS_LINUX
#မဟုတ်ပါက
#အကယ်၍ defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#define CREATOR_OS EXT2_OS_HURD
#မဟုတ်ပါက
#အကယ်၍ defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#define CREATOR_OS EXT2_OS_FREEBSD
#မဟုတ်ပါက
#အကယ်၍ defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#define CREATOR_OS EXT2_OS_LITES
#မဟုတ်ပါက
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
static unsigned ကိန်းပြည့် calc_reserved_gdt_blocks(ext2_filsys fs)
{
	sa_nracanaa ext2_super_block *sb = fs->super;
	unsigned long bpg = sb->s_blocks_per_group;
	unsigned ကိန်းပြည့် gdpb = EXT2_DESC_PER_BLOCK(sb);
	unsigned long max_blocks = 0xffffffff;
	unsigned long rsv_groups;
	unsigned ကိန်းပြည့် rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	အကယ်၍ (ext2fs_blocks_count(sb) < max_blocks / 1024)
		max_blocks = ext2fs_blocks_count(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_blocks - sb->s_first_data_block, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_blocks;
	အကယ်၍ (rsv_gdb > EXT2_ADDR_PER_BLOCK(sb))
		rsv_gdb = EXT2_ADDR_PER_BLOCK(sb);
#ifdef RES_GDT_DEBUG
	printf("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_blocks, rsv_groups, rsv_gdb);
#endif

	ပြန်ပေး rsv_gdb;
}

errcode_t ext2fs_initialize(const var_na *name, ကိန်းပြည့် flags,
			    sa_nracanaa ext2_super_block *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	sa_nracanaa ext2_super_block *super;
	unsigned ကိန်းပြည့်	rem;
	unsigned ကိန်းပြည့်	overhead = 0;
	unsigned ကိန်းပြည့်	ipg;
	dgrp_t		i;
	blk64_t		free_blocks;
	blk_t		numblocks;
	ကိန်းပြည့်		rsv_gdt;
	ကိန်းပြည့်		csum_flag;
	ကိန်းပြည့်		bigalloc_flag;
	ကိန်းပြည့်		io_flags;
	ကိန်းပြည့်		has_bg;
	unsigned	reserved_inos;
	var_na		*buf = 0;
	var_na		c;
	dviguu_na		reserved_ratio;
	var_na		*time_env;

	အကယ်၍ (!param || !ext2fs_blocks_count(param))
		ပြန်ပေး EXT2_ET_INVALID_ARGUMENT;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa struct_ext2_filsys), &fs);
	အကယ်၍ (retval)
		ပြန်ပေး retval;

	memset(fs, 0, parimaa_na(sa_nracanaa struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#ifdef WORDS_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
#endif

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	အကယ်၍ (time_env) {
		fs->now = strtoul(time_env, NULL, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} မဟုတ်ပါက {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		အကယ်၍ (time_env)
			fs->now = strtoul(time_env, NULL, 0);
	}

	io_flags = IO_FLAG_RW;
	အကယ်၍ (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	အကယ်၍ (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	io_flags |= O_BINARY;
	retval = manager->open(name, io_flags, &fs->io);
	အကယ်၍ (retval)
		goto cleanup;
	fs->image_io = fs->io;
	fs->io->app_data = fs;
	retval = ext2fs_get_mem(strlen(name)+1, &fs->device_name);
	အကယ်၍ (retval)
		goto cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &super);
	အကယ်၍ (retval)
		goto cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_SIZE);

#define set_field(field, default) (super->field = param->field ? \
				   param->field : (default))
#define assign_field(field)	(super->field = param->field)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	assign_field(s_log_block_size);

	အကယ်၍ (bigalloc_flag) {
		set_field(s_log_cluster_size, super->s_log_block_size+4);
		အကယ်၍ (super->s_log_block_size > super->s_log_cluster_size) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			goto cleanup;
		}
	} မဟုတ်ပါက
		super->s_log_cluster_size = super->s_log_block_size;

	set_field(s_first_data_block, super->s_log_cluster_size ? 0 : 1);
	set_field(s_max_mnt_count, 0);
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

	အကယ်၍ (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_load_nls_table(param->s_encoding);

	အကယ်၍ (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		goto cleanup;
	}
	အကယ်၍ (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		goto cleanup;
	}

	set_field(s_rev_level, EXT2_GOOD_OLD_REV);
	အကယ်၍ (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		set_field(s_first_ino, EXT2_GOOD_OLD_FIRST_INO);
		set_field(s_inode_size, EXT2_GOOD_OLD_INODE_SIZE);
		အကယ်၍ (super->s_inode_size >= parimaa_na(sa_nracanaa ext2_inode_large)) {
			ကိန်းပြည့် extra_isize = parimaa_na(sa_nracanaa ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_SIZE;
			set_field(s_min_extra_isize, extra_isize);
			set_field(s_want_extra_isize, extra_isize);
		}
	} မဟုတ်ပါက {
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

	အကယ်၍ (bigalloc_flag) {
		unsigned long long bpg;

		အကယ်၍ (param->s_blocks_per_group &&
		    param->s_clusters_per_group &&
		    ((param->s_clusters_per_group * EXT2FS_CLUSTER_RATIO(fs)) !=
		     param->s_blocks_per_group)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			goto cleanup;
		}
		အကယ်၍ (param->s_clusters_per_group)
			assign_field(s_clusters_per_group);
		မဟုတ်ပါက အကယ်၍ (param->s_blocks_per_group)
			super->s_clusters_per_group = 
				param->s_blocks_per_group /
				EXT2FS_CLUSTER_RATIO(fs);
		မဟုတ်ပါက အကယ်၍ (super->s_log_cluster_size + 15 < 32)
			super->s_clusters_per_group = fs->blocksize * 8;
		မဟုတ်ပါက
			super->s_clusters_per_group = (fs->blocksize - 1) * 8;
		အကယ်၍ (super->s_clusters_per_group > EXT2_MAX_CLUSTERS_PER_GROUP(super))
			super->s_clusters_per_group = EXT2_MAX_CLUSTERS_PER_GROUP(super);
		bpg = EXT2FS_C2B(fs,
			(unsigned long long) super->s_clusters_per_group);
		အကယ်၍ (bpg >= (((unsigned long long) 1) << 32)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			goto cleanup;
		}
		super->s_blocks_per_group = bpg;
	} မဟုတ်ပါက {
		set_field(s_blocks_per_group, fs->blocksize * 8);
		အကယ်၍ (super->s_blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(super))
			super->s_blocks_per_group = EXT2_MAX_BLOCKS_PER_GROUP(super);
		super->s_clusters_per_group = super->s_blocks_per_group;
	}

	ext2fs_blocks_count_set(super, ext2fs_blocks_count(param) &
				~((blk64_t) EXT2FS_CLUSTER_MASK(fs)));
	ext2fs_r_blocks_count_set(super, ext2fs_r_blocks_count(param));
	အကယ်၍ (ext2fs_r_blocks_count(super) >= ext2fs_blocks_count(param)) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		goto cleanup;
	}

	set_field(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	အကယ်၍ (ext2fs_has_feature_journal_dev(super)) {
		fs->group_desc_count = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		ပြန်ပေး 0;
	}

retry:
	fs->group_desc_count = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_blocks_count(super) - super->s_first_data_block,
		EXT2_BLOCKS_PER_GROUP(super));
	အကယ်၍ (fs->group_desc_count == 0) {
		retval = EXT2_ET_TOOSMALL;
		goto cleanup;
	}

	set_field(s_desc_size,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_SIZE_64BIT : 0);

	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_count,
					  EXT2_DESC_PER_BLOCK(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	အကယ်၍ (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_blocks_count(super) / i) >= (1ULL << 32))
		set_field(s_inodes_count, ~0U);
	မဟုတ်ပါက
		set_field(s_inodes_count, ext2fs_blocks_count(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_count, fs->group_desc_count);
	အကယ်၍ (ipg > fs->blocksize * 8) {
		အကယ်၍ (!bigalloc_flag && super->s_blocks_per_group >= 256) {
			/* Try again with slightly different parameters */
			super->s_blocks_per_group -= 8;
			ext2fs_blocks_count_set(super,
						ext2fs_blocks_count(param));
			super->s_clusters_per_group = super->s_blocks_per_group;
			goto retry;
		} မဟုတ်ပါက {
			retval = EXT2_ET_TOO_MANY_INODES;
			goto cleanup;
		}
	}

	အကယ်၍ (ipg > (unsigned) EXT2_MAX_INODES_PER_GROUP(super))
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
	အကယ်၍ (super->s_inodes_per_group < 8)
		super->s_inodes_per_group = 8;
	super->s_inodes_per_group &= ~7;
	fs->inode_blocks_per_group = (((super->s_inodes_per_group *
					EXT2_INODE_SIZE(super)) +
				       EXT2_BLOCK_SIZE(super) - 1) /
				      EXT2_BLOCK_SIZE(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	အကယ်၍ ((__u64)super->s_inodes_per_group * fs->group_desc_count > ~0U) {
		ipg--;
		goto ipg_retry;
	}
	super->s_inodes_count = super->s_inodes_per_group *
		fs->group_desc_count;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	အကယ်၍ (super->s_inodes_count < EXT2_FIRST_INODE(super)+1) {
		ipg += 8;
		goto ipg_retry;
	}

	super->s_free_inodes_count = super->s_inodes_count;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	အကယ်၍ (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_blocks(fs);
	မဟုတ်ပါက
		rsv_gdt = 0;
	set_field(s_reserved_gdt_blocks, rsv_gdt);
	အကယ်၍ (super->s_reserved_gdt_blocks > EXT2_ADDR_PER_BLOCK(super)) {
		retval = EXT2_ET_RES_GDT_BLOCKS;
		goto cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	အကယ်၍ (super->s_reserved_gdt_blocks + fs->desc_blocks >
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
	overhead = (ကိန်းပြည့်) (3 + fs->inode_blocks_per_group +
			  super->s_reserved_gdt_blocks);

	အကယ်၍ (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	မဟုတ်ပါက
		overhead += fs->desc_blocks;

	/* This can only happen if the user requested too many inodes */
	အကယ်၍ (overhead > super->s_blocks_per_group) {
		retval = EXT2_ET_TOO_MANY_INODES;
		goto cleanup;
	}

	/*
	 * See if the last group is big enough to support the
	 * necessary data structures.  If not, we need to get rid of
	 * it.  We need to recalculate the overhead for the last block
	 * group, since it might or might not have a superblock
	 * backup.
	 */
	overhead = (ကိန်းပြည့်) (2 + fs->inode_blocks_per_group);
	has_bg = 0;
	အကယ်၍ (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		အကယ်၍ (fs->group_desc_count == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		မဟုတ်ပါက
			has_bg = param->s_backup_bgs[1] != 0;
	} မဟုတ်ပါက
		has_bg = ext2fs_bg_has_super(fs, fs->group_desc_count - 1);
	အကယ်၍ (has_bg)
		overhead += 1 + fs->desc_blocks + super->s_reserved_gdt_blocks;
	rem = ((ext2fs_blocks_count(super) - super->s_first_data_block) %
	       super->s_blocks_per_group);
	အကယ်၍ ((fs->group_desc_count == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		goto cleanup;
	}
	အကယ်၍ (rem && (rem < overhead+50)) {
		ext2fs_blocks_count_set(super, ext2fs_blocks_count(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_ratio = 100.0 * ext2fs_r_blocks_count(param) /
			ext2fs_blocks_count(param);
		ext2fs_r_blocks_count_set(super, reserved_ratio *
			ext2fs_blocks_count(super) / 100.0);

		goto retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	အကယ်၍ (ext2fs_has_feature_sparse_super2(super)) {
		အကယ်၍ (super->s_backup_bgs[0] >= fs->group_desc_count)
			super->s_backup_bgs[0] = fs->group_desc_count - 1;
		အကယ်၍ (super->s_backup_bgs[1] >= fs->group_desc_count)
			super->s_backup_bgs[1] = fs->group_desc_count - 1;
		အကယ်၍ (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		အကယ်၍ (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	အကယ်၍ (retval)
		goto cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->block_map);
	အကယ်၍ (retval)
		goto cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	အကယ်၍ (retval)
		goto cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	အကယ်၍ (retval)
		goto cleanup;

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
	အတွက် (i = 0; i < fs->group_desc_count; i++) {
		blk_t grp_free_blocks;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->block_map,
						       &numblocks);
		အကယ်၍ (retval)
			goto cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		အကယ်၍ (csum_flag) {
			အကယ်၍ (i != fs->group_desc_count - 1 && numblocks == 0)
				ext2fs_bg_flags_set(fs, i,
						    EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_per_group;
			အကယ်၍ (reserved_inos) {
				အကယ်၍ (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} မဟုတ်ပါက {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_unused_set(fs, i, inodes);
		}

		အကယ်၍ (!fs->super->s_log_groups_per_flex)
			numblocks += 2 + fs->inode_blocks_per_group;

		grp_free_blocks = ext2fs_group_blocks_count(fs, i) - numblocks;
		free_blocks += grp_free_blocks;
		ext2fs_bg_free_blocks_count_set(fs, i, grp_free_blocks);
		ext2fs_bg_free_inodes_count_set(fs, i, fs->super->s_inodes_per_group);
		ext2fs_bg_used_dirs_count_set(fs, i, 0);
		ext2fs_group_desc_csum_set(fs, i);
	}
	free_blocks &= ~EXT2FS_CLUSTER_MASK(fs);
	ext2fs_free_blocks_count_set(super, free_blocks);

	c = (var_na) 255;
	အကယ်၍ (((ကိန်းပြည့်) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} မဟုတ်ပါက {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_set_blksize(fs->io, fs->blocksize);

	*ret_fs = fs;
	ပြန်ပေး 0;
cleanup:
	free(buf);
	ext2fs_free(fs);
	ပြန်ပေး retval;
}

errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, ကိန်းပြည့် super_only)
{
	blk64_t		blk;
	ext2_ino_t	ino;
	unsigned ကိန်းပြည့်	group = 0;
	unsigned ကိန်းပြည့်	count = 0;
	ကိန်းပြည့်		total_free = 0;
	ကိန်းပြည့်		group_free = 0;
	ကိန်းပြည့်		last_allocated = 0;
	ကိန်းပြည့်		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	အတွက် (blk = fs->super->s_first_data_block;
	     blk < ext2fs_blocks_count(fs->super); blk++) {
		အကယ်၍ (!ext2fs_fast_test_block_bitmap2(fs->block_map, blk)) {
			group_free++;
			total_free++;
		} မဟုတ်ပါက {
			uninit = 0;
		}
		count++;
		အကယ်၍ ((count == fs->super->s_blocks_per_group) ||
		    (blk == ext2fs_blocks_count(fs->super)-1)) {
			ext2fs_bg_free_blocks_count_set(fs, group,
							group_free);
			အကယ်၍ (!super_only) {
				အကယ်၍ (uninit && blk !=
					ext2fs_blocks_count(fs->super) - 1)
					ext2fs_bg_flags_set(fs, group,
							EXT2_BG_BLOCK_UNINIT);
				မဟုတ်ပါက
					ext2fs_bg_flags_clear(fs, group,
							EXT2_BG_BLOCK_UNINIT);
			}
			count = 0;
			group_free = 0;
			uninit = 1;
			group++;
		}
	}
	total_free = EXT2FS_C2B(fs, total_free);
	ext2fs_free_blocks_count_set(fs->super, total_free);

	/*
	 * Next, calculate the inode statistics
	 */
	group_free = 0;
	total_free = 0;
	last_allocated = 0;
	count = 0;
	group = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	အတွက် (ino = 1; ino <= fs->super->s_inodes_count && ino > 0; ino++) {
		အကယ်၍ (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			group_free++;
			total_free++;
		} မဟုတ်ပါက {
			last_allocated = ino;
		}
		count++;
		အကယ်၍ ((count == fs->super->s_inodes_per_group) ||
		    (ino == fs->super->s_inodes_count)) {
			အကယ်၍ (!super_only) {
				အကယ်၍ (last_allocated) {
					ext2fs_bg_flags_clear(fs, group,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_unused_set(fs, group,
						fs->super->s_inodes_per_group -
						(last_allocated %
						fs->super->s_inodes_per_group));
				} မဟုတ်ပါက {
					ext2fs_bg_flags_set(fs, group,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_unused_set(fs, group,
									0);
				}
				ext2fs_bg_free_inodes_count_set(fs, group,
								group_free);
			}
			group++;
			count = 0;
			group_free = 0;
			last_allocated = 0;
		}
	}
	fs->super->s_free_inodes_count = total_free;
	ext2fs_mark_super_dirty(fs);
	ပြန်ပေး 0;
}
