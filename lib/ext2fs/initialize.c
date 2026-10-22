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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जइ HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जइ HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#ifndef O_द्विमान
#define O_द्विमान 0
#endif

#जइ defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#define CREATOR_OS EXT2_OS_LINUX
#अण्णहा
#जइ defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#define CREATOR_OS EXT2_OS_HURD
#अण्णहा
#जइ defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#define CREATOR_OS EXT2_OS_FREEBSD
#अण्णहा
#जइ defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#define CREATOR_OS EXT2_OS_LITES
#अण्णहा
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
स्थिर unsigned पूर्णाङ्क calc_reserved_gdt_blocks(ext2_filsys fs)
{
	संरचना ext2_super_block *sb = fs->super;
	unsigned long bpg = sb->s_blocks_per_group;
	unsigned पूर्णाङ्क gdpb = EXT2_DESC_PER_BLOCK(sb);
	unsigned long max_blocks = 0xffffffff;
	unsigned long rsv_groups;
	unsigned पूर्णाङ्क rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	जइ (ext2fs_blocks_गणनम्(sb) < max_blocks / 1024)
		max_blocks = ext2fs_blocks_गणनम्(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_blocks - sb->s_first_data_block, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_blocks;
	जइ (rsv_gdb > EXT2_ADDR_PER_BLOCK(sb))
		rsv_gdb = EXT2_ADDR_PER_BLOCK(sb);
#ifdef RES_GDT_DEBUG
	printf("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_blocks, rsv_groups, rsv_gdb);
#endif

	प्रत्यागम rsv_gdb;
}

errcode_t ext2fs_initialize(ध्रुव वर्ण *name, पूर्णाङ्क flags,
			    संरचना ext2_super_block *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	संरचना ext2_super_block *super;
	unsigned पूर्णाङ्क	rem;
	unsigned पूर्णाङ्क	overhead = 0;
	unsigned पूर्णाङ्क	ipg;
	dgrp_t		i;
	blk64_t		free_blocks;
	blk_t		numblocks;
	पूर्णाङ्क		rsv_gdt;
	पूर्णाङ्क		csum_flag;
	पूर्णाङ्क		bigalloc_flag;
	पूर्णाङ्क		io_flags;
	पूर्णाङ्क		has_bg;
	unsigned	reserved_inos;
	वर्ण		*buf = 0;
	वर्ण		c;
	द्विगुण		reserved_ratio;
	वर्ण		*time_env;

	जइ (!param || !ext2fs_blocks_गणनम्(param))
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	retval = ext2fs_get_mem(परिमाण(संरचना struct_ext2_filsys), &fs);
	जइ (retval)
		प्रत्यागम retval;

	memset(fs, 0, परिमाण(संरचना struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#ifdef शब्दाः_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
#endif

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	जइ (time_env) {
		fs->now = strtoul(time_env, रिक्त, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} अण्णहा {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		जइ (time_env)
			fs->now = strtoul(time_env, रिक्त, 0);
	}

	io_flags = IO_FLAG_RW;
	जइ (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	जइ (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	io_flags |= O_द्विमान;
	retval = manager->open(name, io_flags, &fs->io);
	जइ (retval)
		गच्छ cleanup;
	fs->image_io = fs->io;
	fs->io->app_data = fs;
	retval = ext2fs_get_mem(strlen(name)+1, &fs->device_name);
	जइ (retval)
		गच्छ cleanup;

	strcpy(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &super);
	जइ (retval)
		गच्छ cleanup;
	fs->super = super;

	memset(super, 0, SUPERBLOCK_SIZE);

#define set_field(field, स्वभाव) (super->field = param->field ? \
				   param->field : (स्वभाव))
#define assign_field(field)	(super->field = param->field)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	assign_field(s_log_block_size);

	जइ (bigalloc_flag) {
		set_field(s_log_cluster_size, super->s_log_block_size+4);
		जइ (super->s_log_block_size > super->s_log_cluster_size) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			गच्छ cleanup;
		}
	} अण्णहा
		super->s_log_cluster_size = super->s_log_block_size;

	set_field(s_first_data_block, super->s_log_cluster_size ? 0 : 1);
	set_field(s_max_mnt_गणनम्, 0);
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

	जइ (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_load_nls_table(param->s_encoding);

	जइ (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		गच्छ cleanup;
	}
	जइ (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		गच्छ cleanup;
	}

	set_field(s_rev_level, EXT2_GOOD_OLD_REV);
	जइ (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		set_field(s_first_ino, EXT2_GOOD_OLD_FIRST_INO);
		set_field(s_inode_size, EXT2_GOOD_OLD_INODE_SIZE);
		जइ (super->s_inode_size >= परिमाण(संरचना ext2_inode_large)) {
			पूर्णाङ्क extra_isize = परिमाण(संरचना ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_SIZE;
			set_field(s_min_extra_isize, extra_isize);
			set_field(s_want_extra_isize, extra_isize);
		}
	} अण्णहा {
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

	जइ (bigalloc_flag) {
		unsigned long long bpg;

		जइ (param->s_blocks_per_group &&
		    param->s_clusters_per_group &&
		    ((param->s_clusters_per_group * EXT2FS_CLUSTER_RATIO(fs)) !=
		     param->s_blocks_per_group)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			गच्छ cleanup;
		}
		जइ (param->s_clusters_per_group)
			assign_field(s_clusters_per_group);
		अण्णहा जइ (param->s_blocks_per_group)
			super->s_clusters_per_group = 
				param->s_blocks_per_group /
				EXT2FS_CLUSTER_RATIO(fs);
		अण्णहा जइ (super->s_log_cluster_size + 15 < 32)
			super->s_clusters_per_group = fs->blocksize * 8;
		अण्णहा
			super->s_clusters_per_group = (fs->blocksize - 1) * 8;
		जइ (super->s_clusters_per_group > EXT2_MAX_CLUSTERS_PER_GROUP(super))
			super->s_clusters_per_group = EXT2_MAX_CLUSTERS_PER_GROUP(super);
		bpg = EXT2FS_C2B(fs,
			(unsigned long long) super->s_clusters_per_group);
		जइ (bpg >= (((unsigned long long) 1) << 32)) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			गच्छ cleanup;
		}
		super->s_blocks_per_group = bpg;
	} अण्णहा {
		set_field(s_blocks_per_group, fs->blocksize * 8);
		जइ (super->s_blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(super))
			super->s_blocks_per_group = EXT2_MAX_BLOCKS_PER_GROUP(super);
		super->s_clusters_per_group = super->s_blocks_per_group;
	}

	ext2fs_blocks_गणनम्_set(super, ext2fs_blocks_गणनम्(param) &
				~((blk64_t) EXT2FS_CLUSTER_MASK(fs)));
	ext2fs_r_blocks_गणनम्_set(super, ext2fs_r_blocks_गणनम्(param));
	जइ (ext2fs_r_blocks_गणनम्(super) >= ext2fs_blocks_गणनम्(param)) {
		retval = EXT2_ET_INVALID_ARGUMENT;
		गच्छ cleanup;
	}

	set_field(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	जइ (ext2fs_has_feature_journal_dev(super)) {
		fs->group_desc_गणनम् = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		प्रत्यागम 0;
	}

retry:
	fs->group_desc_गणनम् = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_blocks_गणनम्(super) - super->s_first_data_block,
		EXT2_BLOCKS_PER_GROUP(super));
	जइ (fs->group_desc_गणनम् == 0) {
		retval = EXT2_ET_TOOSMALL;
		गच्छ cleanup;
	}

	set_field(s_desc_size,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_SIZE_64BIT : 0);

	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_गणनम्,
					  EXT2_DESC_PER_BLOCK(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	जइ (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_blocks_गणनम्(super) / i) >= (1ULL << 32))
		set_field(s_inodes_गणनम्, ~0U);
	अण्णहा
		set_field(s_inodes_गणनम्, ext2fs_blocks_गणनम्(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_गणनम्, fs->group_desc_गणनम्);
	जइ (ipg > fs->blocksize * 8) {
		जइ (!bigalloc_flag && super->s_blocks_per_group >= 256) {
			/* Try again with slightly different parameters */
			super->s_blocks_per_group -= 8;
			ext2fs_blocks_गणनम्_set(super,
						ext2fs_blocks_गणनम्(param));
			super->s_clusters_per_group = super->s_blocks_per_group;
			गच्छ retry;
		} अण्णहा {
			retval = EXT2_ET_TOO_MANY_INODES;
			गच्छ cleanup;
		}
	}

	जइ (ipg > (unsigned) EXT2_MAX_INODES_PER_GROUP(super))
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
	जइ (super->s_inodes_per_group < 8)
		super->s_inodes_per_group = 8;
	super->s_inodes_per_group &= ~7;
	fs->inode_blocks_per_group = (((super->s_inodes_per_group *
					EXT2_INODE_SIZE(super)) +
				       EXT2_BLOCK_SIZE(super) - 1) /
				      EXT2_BLOCK_SIZE(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	जइ ((__u64)super->s_inodes_per_group * fs->group_desc_गणनम् > ~0U) {
		ipg--;
		गच्छ ipg_retry;
	}
	super->s_inodes_गणनम् = super->s_inodes_per_group *
		fs->group_desc_गणनम्;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	जइ (super->s_inodes_गणनम् < EXT2_FIRST_INODE(super)+1) {
		ipg += 8;
		गच्छ ipg_retry;
	}

	super->s_free_inodes_गणनम् = super->s_inodes_गणनम्;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	जइ (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_blocks(fs);
	अण्णहा
		rsv_gdt = 0;
	set_field(s_reserved_gdt_blocks, rsv_gdt);
	जइ (super->s_reserved_gdt_blocks > EXT2_ADDR_PER_BLOCK(super)) {
		retval = EXT2_ET_RES_GDT_BLOCKS;
		गच्छ cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	जइ (super->s_reserved_gdt_blocks + fs->desc_blocks >
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
	overhead = (पूर्णाङ्क) (3 + fs->inode_blocks_per_group +
			  super->s_reserved_gdt_blocks);

	जइ (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	अण्णहा
		overhead += fs->desc_blocks;

	/* This can only happen if the user requested too many inodes */
	जइ (overhead > super->s_blocks_per_group) {
		retval = EXT2_ET_TOO_MANY_INODES;
		गच्छ cleanup;
	}

	/*
	 * See if the last group is big enough to support the
	 * necessary data structures.  If not, we need to get rid of
	 * it.  We need to recalculate the overhead for the last block
	 * group, since it might or might not have a superblock
	 * backup.
	 */
	overhead = (पूर्णाङ्क) (2 + fs->inode_blocks_per_group);
	has_bg = 0;
	जइ (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		जइ (fs->group_desc_गणनम् == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		अण्णहा
			has_bg = param->s_backup_bgs[1] != 0;
	} अण्णहा
		has_bg = ext2fs_bg_has_super(fs, fs->group_desc_गणनम् - 1);
	जइ (has_bg)
		overhead += 1 + fs->desc_blocks + super->s_reserved_gdt_blocks;
	rem = ((ext2fs_blocks_गणनम्(super) - super->s_first_data_block) %
	       super->s_blocks_per_group);
	जइ ((fs->group_desc_गणनम् == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		गच्छ cleanup;
	}
	जइ (rem && (rem < overhead+50)) {
		ext2fs_blocks_गणनम्_set(super, ext2fs_blocks_गणनम्(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_ratio = 100.0 * ext2fs_r_blocks_गणनम्(param) /
			ext2fs_blocks_गणनम्(param);
		ext2fs_r_blocks_गणनम्_set(super, reserved_ratio *
			ext2fs_blocks_गणनम्(super) / 100.0);

		गच्छ retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	जइ (ext2fs_has_feature_sparse_super2(super)) {
		जइ (super->s_backup_bgs[0] >= fs->group_desc_गणनम्)
			super->s_backup_bgs[0] = fs->group_desc_गणनम् - 1;
		जइ (super->s_backup_bgs[1] >= fs->group_desc_गणनम्)
			super->s_backup_bgs[1] = fs->group_desc_गणनम् - 1;
		जइ (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		जइ (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(strlen(fs->device_name) + 80, &buf);
	जइ (retval)
		गच्छ cleanup;

	strcpy(buf, "block bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->block_map);
	जइ (retval)
		गच्छ cleanup;

	strcpy(buf, "inode bitmap for ");
	strcat(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	जइ (retval)
		गच्छ cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	जइ (retval)
		गच्छ cleanup;

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
	प्रति (i = 0; i < fs->group_desc_गणनम्; i++) {
		blk_t grp_free_blocks;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->block_map,
						       &numblocks);
		जइ (retval)
			गच्छ cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		जइ (csum_flag) {
			जइ (i != fs->group_desc_गणनम् - 1 && numblocks == 0)
				ext2fs_bg_flags_set(fs, i,
						    EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_per_group;
			जइ (reserved_inos) {
				जइ (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} अण्णहा {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_unused_set(fs, i, inodes);
		}

		जइ (!fs->super->s_log_groups_per_flex)
			numblocks += 2 + fs->inode_blocks_per_group;

		grp_free_blocks = ext2fs_group_blocks_गणनम्(fs, i) - numblocks;
		free_blocks += grp_free_blocks;
		ext2fs_bg_free_blocks_गणनम्_set(fs, i, grp_free_blocks);
		ext2fs_bg_free_inodes_गणनम्_set(fs, i, fs->super->s_inodes_per_group);
		ext2fs_bg_used_dirs_गणनम्_set(fs, i, 0);
		ext2fs_group_desc_csum_set(fs, i);
	}
	free_blocks &= ~EXT2FS_CLUSTER_MASK(fs);
	ext2fs_free_blocks_गणनम्_set(super, free_blocks);

	c = (वर्ण) 255;
	जइ (((पूर्णाङ्क) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} अण्णहा {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_set_blksize(fs->io, fs->blocksize);

	*ret_fs = fs;
	प्रत्यागम 0;
cleanup:
	मुक्त(buf);
	ext2fs_free(fs);
	प्रत्यागम retval;
}

errcode_t ext2fs_calculate_summary_stats(ext2_filsys fs, पूर्णाङ्क super_only)
{
	blk64_t		blk;
	ext2_ino_t	ino;
	unsigned पूर्णाङ्क	group = 0;
	unsigned पूर्णाङ्क	गणनम् = 0;
	पूर्णाङ्क		total_free = 0;
	पूर्णाङ्क		group_free = 0;
	पूर्णाङ्क		last_allocated = 0;
	पूर्णाङ्क		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	प्रति (blk = fs->super->s_first_data_block;
	     blk < ext2fs_blocks_गणनम्(fs->super); blk++) {
		जइ (!ext2fs_fast_test_block_bitmap2(fs->block_map, blk)) {
			group_free++;
			total_free++;
		} अण्णहा {
			uninit = 0;
		}
		गणनम्++;
		जइ ((गणनम् == fs->super->s_blocks_per_group) ||
		    (blk == ext2fs_blocks_गणनम्(fs->super)-1)) {
			ext2fs_bg_free_blocks_गणनम्_set(fs, group,
							group_free);
			जइ (!super_only) {
				जइ (uninit && blk !=
					ext2fs_blocks_गणनम्(fs->super) - 1)
					ext2fs_bg_flags_set(fs, group,
							EXT2_BG_BLOCK_UNINIT);
				अण्णहा
					ext2fs_bg_flags_clear(fs, group,
							EXT2_BG_BLOCK_UNINIT);
			}
			गणनम् = 0;
			group_free = 0;
			uninit = 1;
			group++;
		}
	}
	total_free = EXT2FS_C2B(fs, total_free);
	ext2fs_free_blocks_गणनम्_set(fs->super, total_free);

	/*
	 * Next, calculate the inode statistics
	 */
	group_free = 0;
	total_free = 0;
	last_allocated = 0;
	गणनम् = 0;
	group = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	प्रति (ino = 1; ino <= fs->super->s_inodes_गणनम् && ino > 0; ino++) {
		जइ (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			group_free++;
			total_free++;
		} अण्णहा {
			last_allocated = ino;
		}
		गणनम्++;
		जइ ((गणनम् == fs->super->s_inodes_per_group) ||
		    (ino == fs->super->s_inodes_गणनम्)) {
			जइ (!super_only) {
				जइ (last_allocated) {
					ext2fs_bg_flags_clear(fs, group,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_unused_set(fs, group,
						fs->super->s_inodes_per_group -
						(last_allocated %
						fs->super->s_inodes_per_group));
				} अण्णहा {
					ext2fs_bg_flags_set(fs, group,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_unused_set(fs, group,
									0);
				}
				ext2fs_bg_free_inodes_गणनम्_set(fs, group,
								group_free);
			}
			group++;
			गणनम् = 0;
			group_free = 0;
			last_allocated = 0;
		}
	}
	fs->super->s_free_inodes_गणनम् = total_free;
	ext2fs_mark_super_dirty(fs);
	प्रत्यागम 0;
}
