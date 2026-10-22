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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

#ifndef O_இருமம்
#define O_இருமம் 0
#endif

#எனில் defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#define CREATOR_OS EXT2_OS_LINUX
#இல்லையெனில்
#எனில் defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#define CREATOR_OS EXT2_OS_HURD
#இல்லையெனில்
#எனில் defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#define CREATOR_OS EXT2_OS_FREEBSD
#இல்லையெனில்
#எனில் defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#define CREATOR_OS EXT2_OS_LITES
#இல்லையெனில்
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
நிலையான unsigned முழு calc_reserved_gdt_blocks(ext2_filsys fs)
{
	கட்டமைப்பு ext2_super_block *sb = fs->super;
	unsigned long bpg = sb->s_blocks_per_group;
	unsigned முழு gdpb = EXT2_DESC_PER_BLOCK(sb);
	unsigned long max_blocks = 0xffffffff;
	unsigned long rsv_groups;
	unsigned முழு rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	எனில் (ext2fs_blocks_மொத்தம்(sb) < max_blocks / 1024)
		max_blocks = ext2fs_blocks_மொத்தம்(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_blocks - sb->s_first_data_block, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_blocks;
	எனில் (rsv_gdb > EXT2_ADDR_PER_BLOCK(sb))
		rsv_gdb = EXT2_ADDR_PER_BLOCK(sb);
#ifdef RES_GDT_DEBUG
	printf("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_blocks, rsv_groups, rsv_gdb);
#endif

	திரும்பு rsv_gdb;
}

errcode_t ext2fs_initialize(மாறிலி எழுத்து *name, முழு flags,
			    கட்டமைப்பு ext2_super_block *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	கட்டமைப்பு ext2_super_block *super;
	unsigned முழு	rem;
	unsigned முழு	overhead = 0;
	unsigned முழு	ipg;
	dgrp_t		i;
	blk64_t		free_blocks;
	blk_t		numblocks;
	முழு		rsv_gdt;
	முழு		csum_flag;
	முழு		bigalloc_flag;
	முழு		io_flags;
	முழு		has_bg;
	unsigned	reserved_inos;
	எழுத்து		*buf = 0;
	எழுத்து		c;
	இரட்டை		reserved_ratio;
	எழுத்து		*time_env;

	எனில் (!param || !ext2fs_blocks_மொத்தம்(param))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு struct_ext2_filsys), &fs);
	எனில் (retval)
		திரும்பு retval;

	memset(fs, 0, அளவு(கட்டமைப்பு struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#ifdef சொற்கள்_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
#endif

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	எனில் (time_env) {
		fs->now = strtoul(time_env, வெற்று, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} இல்லையெனில் {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		எனில் (time_env)
			fs->now = strtoul(time_env, வெற்று, 0);
	}

	io_flags = IO_FLAG_RW;
	எனில் (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	எனில் (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	io_flags |= O_இருமம்;
	retval = manager->open(name, io_flags, &fs->io);
	எனில் (retval)
		செல் cleanup;
	fs->image_io = fs->io;
	fs->io->app_data = fs;
	retval = ext2fs_get_mem(strlen(name)+1, &fs->device_name);
	எனில் (retval)
		செல் cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &super);
	எனில் (retval)
		செல் cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_SIZE);

#define set_field(field, இயல்பு) (super->field = param->field ? \
				   param->field : (இயல்பு))
#define assign_field(field)	(super->field = param->field)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	assign_field(s_log_block_size);

	எனில் (bigalloc_flag) {
		set_field(s_log_cluster_size, super->s_log_block_size+4);
		எனில் (super->s_log_block_size > super->s_log_cluster_size) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			செல் cleanup;
		}
	} இல்லையெனில்
		super->s_log_cluster_size = super->s_log_block_size;

	set_field(s_first_data_block, super->s_log_cluster_size ? 0 : 1);
	set_field(s_max_mnt_மொத்தம், 0);
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

	எனில் (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_load_nls_table(param->s_encoding);

	எனில் (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		செல் cleanup;
	}
	எனில் (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		செல் cleanup;
	}

	set_field(s_rev_level, EXT2_GOOD_OLD_REV);
	எனில் (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		set_field(s_first_ino, EXT2_GOOD_OLD_FIRST_INO);
		set_field(s_inode_size, EXT2_GOOD_OLD_INODE_SIZE);
		எனில் (super->s_inode_size >= அளவு(கட்டமைப்பு ext2_inode_large)) {
			முழு extra_isize = அளவு(கட்டமைப்பு ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_SIZE;
			set_field(s_min_extra_isize, extra_isize);
			set_field(s_want_extra_isize, extra_isize);
		}
	} இல்லையெனில் {
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

	எனில் (bigalloc_flag) {
		unsigned long long bpg;

		எனில் (param->s_blocks_per_group &&
		    param->s_clusters_per_group &&
		    ((param->s_clusters_per_group * EXT2FS_CLUSTER_RATIO(fs)) !=
		     param->s_blocks_per_group)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			செல் cleanup;
		}
		எனில் (param->s_clusters_per_group)
			assign_field(s_clusters_per_group);
		இல்லையெனில் எனில் (param->s_blocks_per_group)
			super->s_clusters_per_group = 
				param->s_blocks_per_group /
				EXT2FS_CLUSTER_RATIO(fs);
		இல்லையெனில் எனில் (super->s_log_cluster_size + 15 < 32)
			super->s_clusters_per_group = fs->blocksize * 8;
		இல்லையெனில்
			super->s_clusters_per_group = (fs->blocksize - 1) * 8;
		எனில் (super->s_clusters_per_group > EXT2_MAX_CLUSTERS_PER_GROUP(super))
			super->s_clusters_per_group = EXT2_MAX_CLUSTERS_PER_GROUP(super);
		bpg = EXT2FS_C2B(fs,
			(unsigned long long) super->s_clusters_per_group);
		எனில் (bpg >= (((unsigned long long) 1) << 32)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			செல் cleanup;
		}
		super->s_blocks_per_group = bpg;
	} இல்லையெனில் {
		set_field(s_blocks_per_group, fs->blocksize * 8);
		எனில் (super->s_blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(super))
			super->s_blocks_per_group = EXT2_MAX_BLOCKS_PER_GROUP(super);
		super->s_clusters_per_group = super->s_blocks_per_group;
	}

	ext2fs_blocks_மொத்தம்_set(super, ext2fs_blocks_மொத்தம்(param) &
				~((blk64_t) EXT2FS_CLUSTER_MASK(fs)));
	ext2fs_r_blocks_மொத்தம்_set(super, ext2fs_r_blocks_மொத்தம்(param));
	எனில் (ext2fs_r_blocks_மொத்தம்(super) >= ext2fs_blocks_மொத்தம்(param)) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		செல் cleanup;
	}

	set_field(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	எனில் (ext2fs_has_feature_journal_dev(super)) {
		fs->group_desc_மொத்தம் = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		திரும்பு 0;
	}

retry:
	fs->group_desc_மொத்தம் = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_blocks_மொத்தம்(super) - super->s_first_data_block,
		EXT2_BLOCKS_PER_GROUP(super));
	எனில் (fs->group_desc_மொத்தம் == 0) {
		retval = EXT2_ET_TOOSMALL;
		செல் cleanup;
	}

	set_field(s_desc_size,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_SIZE_64BIT : 0);

	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_மொத்தம்,
					  EXT2_DESC_PER_BLOCK(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	எனில் (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_blocks_மொத்தம்(super) / i) >= (1ULL << 32))
		set_field(s_inodes_மொத்தம், ~0U);
	இல்லையெனில்
		set_field(s_inodes_மொத்தம், ext2fs_blocks_மொத்தம்(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_மொத்தம், fs->group_desc_மொத்தம்);
	எனில் (ipg > fs->blocksize * 8) {
		எனில் (!bigalloc_flag && super->s_blocks_per_group >= 256) {
			/* Try again with slightly different parameters */
			super->s_blocks_per_group -= 8;
			ext2fs_blocks_மொத்தம்_set(super,
						ext2fs_blocks_மொத்தம்(param));
			super->s_clusters_per_group = super->s_blocks_per_group;
			செல் retry;
		} இல்லையெனில் {
			retval = EXT2_ET_TOO_MANY_INODES;
			செல் cleanup;
		}
	}

	எனில் (ipg > (unsigned) EXT2_MAX_INODES_PER_GROUP(super))
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
	எனில் (super->s_inodes_per_group < 8)
		super->s_inodes_per_group = 8;
	super->s_inodes_per_group &= ~7;
	fs->inode_blocks_per_group = (((super->s_inodes_per_group *
					EXT2_INODE_SIZE(super)) +
				       EXT2_BLOCK_SIZE(super) - 1) /
				      EXT2_BLOCK_SIZE(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	எனில் ((__u64)super->s_inodes_per_group * fs->group_desc_மொத்தம் > ~0U) {
		ipg--;
		செல் ipg_retry;
	}
	super->s_inodes_மொத்தம் = super->s_inodes_per_group *
		fs->group_desc_மொத்தம்;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	எனில் (super->s_inodes_மொத்தம் < EXT2_FIRST_INODE(super)+1) {
		ipg += 8;
		செல் ipg_retry;
	}

	super->s_free_inodes_மொத்தம் = super->s_inodes_மொத்தம்;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	எனில் (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_blocks(fs);
	இல்லையெனில்
		rsv_gdt = 0;
	set_field(s_reserved_gdt_blocks, rsv_gdt);
	எனில் (super->s_reserved_gdt_blocks > EXT2_ADDR_PER_BLOCK(super)) {
		retval = EXT2_ET_RES_GDT_BLOCKS;
		செல் cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	எனில் (super->s_reserved_gdt_blocks + fs->desc_blocks >
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
	overhead = (முழு) (3 + fs->inode_blocks_per_group +
			  super->s_reserved_gdt_blocks);

	எனில் (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	இல்லையெனில்
		overhead += fs->desc_blocks;

	/* This can only happen if the user requested too many inodes */
	எனில் (overhead > super->s_blocks_per_group) {
		retval = EXT2_ET_TOO_MANY_INODES;
		செல் cleanup;
	}

	/*
	 * See if the last group is big enough to support the
	 * necessary data structures.  If not, we need to get rid of
	 * it.  We need to recalculate the overhead for the last block
	 * group, since it might or might not have a superblock
	 * backup.
	 */
	overhead = (முழு) (2 + fs->inode_blocks_per_group);
	has_bg = 0;
	எனில் (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		எனில் (fs->group_desc_மொத்தம் == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		இல்லையெனில்
			has_bg = param->s_backup_bgs[1] != 0;
	} இல்லையெனில்
		has_bg = ext2fs_bg_has_super(fs, fs->group_desc_மொத்தம் - 1);
	எனில் (has_bg)
		overhead += 1 + fs->desc_blocks + super->s_reserved_gdt_blocks;
	rem = ((ext2fs_blocks_மொத்தம்(super) - super->s_first_data_block) %
	       super->s_blocks_per_group);
	எனில் ((fs->group_desc_மொத்தம் == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		செல் cleanup;
	}
	எனில் (rem && (rem < overhead+50)) {
		ext2fs_blocks_மொத்தம்_set(super, ext2fs_blocks_மொத்தம்(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_ratio = 100.0 * ext2fs_r_blocks_மொத்தம்(param) /
			ext2fs_blocks_மொத்தம்(param);
		ext2fs_r_blocks_மொத்தம்_set(super, reserved_ratio *
			ext2fs_blocks_மொத்தம்(super) / 100.0);

		செல் retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	எனில் (ext2fs_has_feature_sparse_super2(super)) {
		எனில் (super->s_backup_bgs[0] >= fs->group_desc_மொத்தம்)
			super->s_backup_bgs[0] = fs->group_desc_மொத்தம் - 1;
		எனில் (super->s_backup_bgs[1] >= fs->group_desc_மொத்தம்)
			super->s_backup_bgs[1] = fs->group_desc_மொத்தம் - 1;
		எனில் (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		எனில் (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	எனில் (retval)
		செல் cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->block_map);
	எனில் (retval)
		செல் cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	எனில் (retval)
		செல் cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	எனில் (retval)
		செல் cleanup;

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
	க்கு (i = 0; i < fs->group_desc_மொத்தம்; i++) {
		blk_t grp_free_blocks;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->block_map,
						       &numblocks);
		எனில் (retval)
			செல் cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		எனில் (csum_flag) {
			எனில் (i != fs->group_desc_மொத்தம் - 1 && numblocks == 0)
				ext2fs_bg_flags_set(fs, i,
						    EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_per_group;
			எனில் (reserved_inos) {
				எனில் (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} இல்லையெனில் {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_unused_set(fs, i, inodes);
		}

		எனில் (!fs->super->s_log_groups_per_flex)
			numblocks += 2 + fs->inode_blocks_per_group;

		grp_free_blocks = ext2fs_group_blocks_மொத்தம்(fs, i) - numblocks;
		free_blocks += grp_free_blocks;
		ext2fs_bg_free_blocks_மொத்தம்_set(fs, i, grp_free_blocks);
		ext2fs_bg_free_inodes_மொத்தம்_set(fs, i, fs->super->s_inodes_per_group);
		ext2fs_bg_used_dirs_மொத்தம்_set(fs, i, 0);
		ext2fs_group_desc_csum_set(fs, i);
	}
	free_blocks &= ~EXT2FS_CLUSTER_MASK(fs);
	ext2fs_free_blocks_மொத்தம்_set(super, free_blocks);

	c = (எழுத்து) 255;
	எனில் (((முழு) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} இல்லையெனில் {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_set_blksize(fs->io, fs->blocksize);

	*ret_fs = fs;
	திரும்பு 0;
cleanup:
	விடுவி(buf);
	ext2fs_free(fs);
	திரும்பு retval;
}

errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, முழு super_only)
{
	blk64_t		blk;
	ext2_ino_t	ino;
	unsigned முழு	group = 0;
	unsigned முழு	மொத்தம் = 0;
	முழு		total_free = 0;
	முழு		group_free = 0;
	முழு		last_allocated = 0;
	முழு		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	க்கு (blk = fs->super->s_first_data_block;
	     blk < ext2fs_blocks_மொத்தம்(fs->super); blk++) {
		எனில் (!ext2fs_fast_test_block_bitmap2(fs->block_map, blk)) {
			group_free++;
			total_free++;
		} இல்லையெனில் {
			uninit = 0;
		}
		மொத்தம்++;
		எனில் ((மொத்தம் == fs->super->s_blocks_per_group) ||
		    (blk == ext2fs_blocks_மொத்தம்(fs->super)-1)) {
			ext2fs_bg_free_blocks_மொத்தம்_set(fs, group,
							group_free);
			எனில் (!super_only) {
				எனில் (uninit && blk !=
					ext2fs_blocks_மொத்தம்(fs->super) - 1)
					ext2fs_bg_flags_set(fs, group,
							EXT2_BG_BLOCK_UNINIT);
				இல்லையெனில்
					ext2fs_bg_flags_clear(fs, group,
							EXT2_BG_BLOCK_UNINIT);
			}
			மொத்தம் = 0;
			group_free = 0;
			uninit = 1;
			group++;
		}
	}
	total_free = EXT2FS_C2B(fs, total_free);
	ext2fs_free_blocks_மொத்தம்_set(fs->super, total_free);

	/*
	 * Next, calculate the inode statistics
	 */
	group_free = 0;
	total_free = 0;
	last_allocated = 0;
	மொத்தம் = 0;
	group = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	க்கு (ino = 1; ino <= fs->super->s_inodes_மொத்தம் && ino > 0; ino++) {
		எனில் (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			group_free++;
			total_free++;
		} இல்லையெனில் {
			last_allocated = ino;
		}
		மொத்தம்++;
		எனில் ((மொத்தம் == fs->super->s_inodes_per_group) ||
		    (ino == fs->super->s_inodes_மொத்தம்)) {
			எனில் (!super_only) {
				எனில் (last_allocated) {
					ext2fs_bg_flags_clear(fs, group,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_unused_set(fs, group,
						fs->super->s_inodes_per_group -
						(last_allocated %
						fs->super->s_inodes_per_group));
				} இல்லையெனில் {
					ext2fs_bg_flags_set(fs, group,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_unused_set(fs, group,
									0);
				}
				ext2fs_bg_free_inodes_மொத்தம்_set(fs, group,
								group_free);
			}
			group++;
			மொத்தம் = 0;
			group_free = 0;
			last_allocated = 0;
		}
	}
	fs->super->s_free_inodes_மொத்தம் = total_free;
	ext2fs_mark_super_dirty(fs);
	திரும்பு 0;
}
