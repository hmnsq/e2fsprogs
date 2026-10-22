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
#𐡄𐡍 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#𐡄𐡍 HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#𐡄𐡍 HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#𐡄𐡍 defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#define CREATOR_OS EXT2_OS_LINUX
#وإلا
#𐡄𐡍 defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#define CREATOR_OS EXT2_OS_HURD
#وإلا
#𐡄𐡍 defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#define CREATOR_OS EXT2_OS_FREEBSD
#وإلا
#𐡄𐡍 defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#define CREATOR_OS EXT2_OS_LITES
#وإلا
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
ساكن unsigned صحيح calc_reserved_gdt_blocks(ext2_filsys fs)
{
	بنية ext2_super_block *sb = fs->super;
	unsigned long bpg = sb->s_blocks_per_group;
	unsigned صحيح gdpb = EXT2_DESC_PER_BLOCK(sb);
	unsigned long max_blocks = 0xffffffff;
	unsigned long rsv_groups;
	unsigned صحيح rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	𐡄𐡍 (ext2fs_blocks_count(sb) < max_blocks / 1024)
		max_blocks = ext2fs_blocks_count(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_blocks - sb->s_first_data_block, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_blocks;
	𐡄𐡍 (rsv_gdb > EXT2_ADDR_PER_BLOCK(sb))
		rsv_gdb = EXT2_ADDR_PER_BLOCK(sb);
#ifdef RES_GDT_DEBUG
	printf("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_blocks, rsv_groups, rsv_gdb);
#endif

	أرجع rsv_gdb;
}

errcode_t ext2fs_initialize(ثابت حرف *name, صحيح flags,
			    بنية ext2_super_block *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	بنية ext2_super_block *super;
	unsigned صحيح	rem;
	unsigned صحيح	overhead = 0;
	unsigned صحيح	ipg;
	dgrp_t		i;
	blk64_t		free_blocks;
	blk_t		numblocks;
	صحيح		rsv_gdt;
	صحيح		csum_flag;
	صحيح		bigalloc_flag;
	صحيح		io_flags;
	صحيح		has_bg;
	unsigned	reserved_inos;
	حرف		*buf = 0;
	حرف		c;
	مزدوج		reserved_ratio;
	حرف		*time_env;

	𐡄𐡍 (!param || !ext2fs_blocks_count(param))
		أرجع EXT2_ET_INVALID_ARGUMENT;

	retval = ext2fs_get_mem(حجم(بنية struct_ext2_filsys), &fs);
	𐡄𐡍 (retval)
		أرجع retval;

	memset(fs, 0, حجم(بنية struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#ifdef WORDS_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
#endif

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	𐡄𐡍 (time_env) {
		fs->now = strtoul(time_env, عدم, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} وإلا {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		𐡄𐡍 (time_env)
			fs->now = strtoul(time_env, عدم, 0);
	}

	io_flags = IO_FLAG_RW;
	𐡄𐡍 (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	𐡄𐡍 (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	io_flags |= O_BINARY;
	retval = manager->open(name, io_flags, &fs->io);
	𐡄𐡍 (retval)
		اذهب cleanup;
	fs->image_io = fs->io;
	fs->io->app_data = fs;
	retval = ext2fs_get_mem(strlen(name)+1, &fs->device_name);
	𐡄𐡍 (retval)
		اذهب cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &super);
	𐡄𐡍 (retval)
		اذهب cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_SIZE);

#define set_field(field, افتراضي) (super->field = param->field ? \
				   param->field : (افتراضي))
#define assign_field(field)	(super->field = param->field)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	assign_field(s_log_block_size);

	𐡄𐡍 (bigalloc_flag) {
		set_field(s_log_cluster_size, super->s_log_block_size+4);
		𐡄𐡍 (super->s_log_block_size > super->s_log_cluster_size) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			اذهب cleanup;
		}
	} وإلا
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

	𐡄𐡍 (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_load_nls_table(param->s_encoding);

	𐡄𐡍 (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		اذهب cleanup;
	}
	𐡄𐡍 (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		اذهب cleanup;
	}

	set_field(s_rev_level, EXT2_GOOD_OLD_REV);
	𐡄𐡍 (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		set_field(s_first_ino, EXT2_GOOD_OLD_FIRST_INO);
		set_field(s_inode_size, EXT2_GOOD_OLD_INODE_SIZE);
		𐡄𐡍 (super->s_inode_size >= حجم(بنية ext2_inode_large)) {
			صحيح extra_isize = حجم(بنية ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_SIZE;
			set_field(s_min_extra_isize, extra_isize);
			set_field(s_want_extra_isize, extra_isize);
		}
	} وإلا {
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

	𐡄𐡍 (bigalloc_flag) {
		unsigned long long bpg;

		𐡄𐡍 (param->s_blocks_per_group &&
		    param->s_clusters_per_group &&
		    ((param->s_clusters_per_group * EXT2FS_CLUSTER_RATIO(fs)) !=
		     param->s_blocks_per_group)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			اذهب cleanup;
		}
		𐡄𐡍 (param->s_clusters_per_group)
			assign_field(s_clusters_per_group);
		وإلا 𐡄𐡍 (param->s_blocks_per_group)
			super->s_clusters_per_group = 
				param->s_blocks_per_group /
				EXT2FS_CLUSTER_RATIO(fs);
		وإلا 𐡄𐡍 (super->s_log_cluster_size + 15 < 32)
			super->s_clusters_per_group = fs->blocksize * 8;
		وإلا
			super->s_clusters_per_group = (fs->blocksize - 1) * 8;
		𐡄𐡍 (super->s_clusters_per_group > EXT2_MAX_CLUSTERS_PER_GROUP(super))
			super->s_clusters_per_group = EXT2_MAX_CLUSTERS_PER_GROUP(super);
		bpg = EXT2FS_C2B(fs,
			(unsigned long long) super->s_clusters_per_group);
		𐡄𐡍 (bpg >= (((unsigned long long) 1) << 32)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			اذهب cleanup;
		}
		super->s_blocks_per_group = bpg;
	} وإلا {
		set_field(s_blocks_per_group, fs->blocksize * 8);
		𐡄𐡍 (super->s_blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(super))
			super->s_blocks_per_group = EXT2_MAX_BLOCKS_PER_GROUP(super);
		super->s_clusters_per_group = super->s_blocks_per_group;
	}

	ext2fs_blocks_count_set(super, ext2fs_blocks_count(param) &
				~((blk64_t) EXT2FS_CLUSTER_MASK(fs)));
	ext2fs_r_blocks_count_set(super, ext2fs_r_blocks_count(param));
	𐡄𐡍 (ext2fs_r_blocks_count(super) >= ext2fs_blocks_count(param)) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		اذهب cleanup;
	}

	set_field(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	𐡄𐡍 (ext2fs_has_feature_journal_dev(super)) {
		fs->group_desc_count = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		أرجع 0;
	}

retry:
	fs->group_desc_count = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_blocks_count(super) - super->s_first_data_block,
		EXT2_BLOCKS_PER_GROUP(super));
	𐡄𐡍 (fs->group_desc_count == 0) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}

	set_field(s_desc_size,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_SIZE_64BIT : 0);

	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_count,
					  EXT2_DESC_PER_BLOCK(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	𐡄𐡍 (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_blocks_count(super) / i) >= (1ULL << 32))
		set_field(s_inodes_count, ~0U);
	وإلا
		set_field(s_inodes_count, ext2fs_blocks_count(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_count, fs->group_desc_count);
	𐡄𐡍 (ipg > fs->blocksize * 8) {
		𐡄𐡍 (!bigalloc_flag && super->s_blocks_per_group >= 256) {
			/* Try again with slightly different parameters */
			super->s_blocks_per_group -= 8;
			ext2fs_blocks_count_set(super,
						ext2fs_blocks_count(param));
			super->s_clusters_per_group = super->s_blocks_per_group;
			اذهب retry;
		} وإلا {
			retval = EXT2_ET_TOO_MANY_INODES;
			اذهب cleanup;
		}
	}

	𐡄𐡍 (ipg > (unsigned) EXT2_MAX_INODES_PER_GROUP(super))
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
	𐡄𐡍 (super->s_inodes_per_group < 8)
		super->s_inodes_per_group = 8;
	super->s_inodes_per_group &= ~7;
	fs->inode_blocks_per_group = (((super->s_inodes_per_group *
					EXT2_INODE_SIZE(super)) +
				       EXT2_BLOCK_SIZE(super) - 1) /
				      EXT2_BLOCK_SIZE(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	𐡄𐡍 ((__u64)super->s_inodes_per_group * fs->group_desc_count > ~0U) {
		ipg--;
		اذهب ipg_retry;
	}
	super->s_inodes_count = super->s_inodes_per_group *
		fs->group_desc_count;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	𐡄𐡍 (super->s_inodes_count < EXT2_FIRST_INODE(super)+1) {
		ipg += 8;
		اذهب ipg_retry;
	}

	super->s_free_inodes_count = super->s_inodes_count;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	𐡄𐡍 (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_blocks(fs);
	وإلا
		rsv_gdt = 0;
	set_field(s_reserved_gdt_blocks, rsv_gdt);
	𐡄𐡍 (super->s_reserved_gdt_blocks > EXT2_ADDR_PER_BLOCK(super)) {
		retval = EXT2_ET_RES_GDT_BLOCKS;
		اذهب cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	𐡄𐡍 (super->s_reserved_gdt_blocks + fs->desc_blocks >
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
	overhead = (صحيح) (3 + fs->inode_blocks_per_group +
			  super->s_reserved_gdt_blocks);

	𐡄𐡍 (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	وإلا
		overhead += fs->desc_blocks;

	/* This can only happen if the user requested too many inodes */
	𐡄𐡍 (overhead > super->s_blocks_per_group) {
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
	overhead = (صحيح) (2 + fs->inode_blocks_per_group);
	has_bg = 0;
	𐡄𐡍 (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		𐡄𐡍 (fs->group_desc_count == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		وإلا
			has_bg = param->s_backup_bgs[1] != 0;
	} وإلا
		has_bg = ext2fs_bg_has_super(fs, fs->group_desc_count - 1);
	𐡄𐡍 (has_bg)
		overhead += 1 + fs->desc_blocks + super->s_reserved_gdt_blocks;
	rem = ((ext2fs_blocks_count(super) - super->s_first_data_block) %
	       super->s_blocks_per_group);
	𐡄𐡍 ((fs->group_desc_count == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		اذهب cleanup;
	}
	𐡄𐡍 (rem && (rem < overhead+50)) {
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

		اذهب retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	𐡄𐡍 (ext2fs_has_feature_sparse_super2(super)) {
		𐡄𐡍 (super->s_backup_bgs[0] >= fs->group_desc_count)
			super->s_backup_bgs[0] = fs->group_desc_count - 1;
		𐡄𐡍 (super->s_backup_bgs[1] >= fs->group_desc_count)
			super->s_backup_bgs[1] = fs->group_desc_count - 1;
		𐡄𐡍 (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		𐡄𐡍 (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	𐡄𐡍 (retval)
		اذهب cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->block_map);
	𐡄𐡍 (retval)
		اذهب cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	𐡄𐡍 (retval)
		اذهب cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	𐡄𐡍 (retval)
		اذهب cleanup;

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
	لكل (i = 0; i < fs->group_desc_count; i++) {
		blk_t grp_free_blocks;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->block_map,
						       &numblocks);
		𐡄𐡍 (retval)
			اذهب cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		𐡄𐡍 (csum_flag) {
			𐡄𐡍 (i != fs->group_desc_count - 1 && numblocks == 0)
				ext2fs_bg_flags_set(fs, i,
						    EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_per_group;
			𐡄𐡍 (reserved_inos) {
				𐡄𐡍 (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} وإلا {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_unused_set(fs, i, inodes);
		}

		𐡄𐡍 (!fs->super->s_log_groups_per_flex)
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

	c = (حرف) 255;
	𐡄𐡍 (((صحيح) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} وإلا {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_set_blksize(fs->io, fs->blocksize);

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
	unsigned صحيح	group = 0;
	unsigned صحيح	count = 0;
	صحيح		total_free = 0;
	صحيح		group_free = 0;
	صحيح		last_allocated = 0;
	صحيح		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	لكل (blk = fs->super->s_first_data_block;
	     blk < ext2fs_blocks_count(fs->super); blk++) {
		𐡄𐡍 (!ext2fs_fast_test_block_bitmap2(fs->block_map, blk)) {
			group_free++;
			total_free++;
		} وإلا {
			uninit = 0;
		}
		count++;
		𐡄𐡍 ((count == fs->super->s_blocks_per_group) ||
		    (blk == ext2fs_blocks_count(fs->super)-1)) {
			ext2fs_bg_free_blocks_count_set(fs, group,
							group_free);
			𐡄𐡍 (!super_only) {
				𐡄𐡍 (uninit && blk !=
					ext2fs_blocks_count(fs->super) - 1)
					ext2fs_bg_flags_set(fs, group,
							EXT2_BG_BLOCK_UNINIT);
				وإلا
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
	لكل (ino = 1; ino <= fs->super->s_inodes_count && ino > 0; ino++) {
		𐡄𐡍 (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			group_free++;
			total_free++;
		} وإلا {
			last_allocated = ino;
		}
		count++;
		𐡄𐡍 ((count == fs->super->s_inodes_per_group) ||
		    (ino == fs->super->s_inodes_count)) {
			𐡄𐡍 (!super_only) {
				𐡄𐡍 (last_allocated) {
					ext2fs_bg_flags_clear(fs, group,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_unused_set(fs, group,
						fs->super->s_inodes_per_group -
						(last_allocated %
						fs->super->s_inodes_per_group));
				} وإلا {
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
	أرجع 0;
}
