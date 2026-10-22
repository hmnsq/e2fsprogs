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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#यदि_अपरिभाषितम् O_द्विमान
#परिभाषय O_द्विमान 0
#यदि_अन्तः

#यदि defined(__linux__)    &&	defined(EXT2_OS_LINUX)
#परिभाषय CREATOR_OS EXT2_OS_LINUX
#अन्यथा
#यदि defined(__GNU__)     &&	defined(EXT2_OS_HURD)
#परिभाषय CREATOR_OS EXT2_OS_HURD
#अन्यथा
#यदि defined(__FreeBSD__) &&	defined(EXT2_OS_FREEBSD)
#परिभाषय CREATOR_OS EXT2_OS_FREEBSD
#अन्यथा
#यदि defined(LITES) 	   &&	defined(EXT2_OS_LITES)
#परिभाषय CREATOR_OS EXT2_OS_LITES
#अन्यथा
#परिभाषय CREATOR_OS EXT2_OS_LINUX /* by default */
#यदि_अन्तः /* defined(LITES) && defined(EXT2_OS_LITES) */
#यदि_अन्तः /* defined(__FreeBSD__) && defined(EXT2_OS_FREEBSD) */
#यदि_अन्तः /* defined(__GNU__)     && defined(EXT2_OS_HURD) */
#यदि_अन्तः /* defined(__linux__)   && defined(EXT2_OS_LINUX) */

/*
 * Calculate the number of GDT blocks to reserve for online filesystem growth.
 * The absolute maximum number of GDT blocks we can reserve is determined by
 * the number of block pointers that can fit into a single block.
 */
स्थिर अचिह्नित पूर्णाङ्क calc_reserved_gdt_खण्डानि(ext2_filsys fs)
{
	संरचना ext2_super_खण्डम् *sb = fs->super;
	अचिह्नित दीर्घ bpg = sb->s_खण्डानि_प्रत्येकम्_गणः;
	अचिह्नित पूर्णाङ्क gdpb = EXT2_DESC_प्रत्येकम्_खण्डम्(sb);
	अचिह्नित दीर्घ max_खण्डानि = 0xffffffff;
	अचिह्नित दीर्घ rsv_groups;
	अचिह्नित पूर्णाङ्क rsv_gdb;

	/* We set it at 1024x the current filesystem size, or
	 * the upper block count limit (2^32), whichever is lower.
	 */
	यदि (ext2fs_खण्डानि_गणनम्(sb) < max_खण्डानि / 1024)
		max_खण्डानि = ext2fs_खण्डानि_गणनम्(sb) * 1024;
	/*
	 * ext2fs_div64_ceil() is unnecessary because max_blocks is
	 * max _GDT_ blocks, which is limited to 32 bits.
	 */
	rsv_groups = ext2fs_div_ceil(max_खण्डानि - sb->s_first_दत्त_खण्डम्, bpg);
	rsv_gdb = ext2fs_div_ceil(rsv_groups, gdpb) - fs->desc_खण्डानि;
	यदि (rsv_gdb > EXT2_ADDR_प्रत्येकम्_खण्डम्(sb))
		rsv_gdb = EXT2_ADDR_प्रत्येकम्_खण्डम्(sb);
#यदि_परिभाषितम् RES_GDT_DEBUG
	प्रारूप_लिख("max_blocks %lu, rsv_groups = %lu, rsv_gdb = %u\n",
	       max_खण्डानि, rsv_groups, rsv_gdb);
#यदि_अन्तः

	प्रत्यागम rsv_gdb;
}

errcode_t ext2fs_initialize(ध्रुव वर्ण *name, पूर्णाङ्क flags,
			    संरचना ext2_super_खण्डम् *param,
			    io_manager manager, ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	संरचना ext2_super_खण्डम् *super;
	अचिह्नित पूर्णाङ्क	rem;
	अचिह्नित पूर्णाङ्क	overhead = 0;
	अचिह्नित पूर्णाङ्क	ipg;
	dgrp_t		i;
	blk64_t		free_खण्डानि;
	blk_t		numblocks;
	पूर्णाङ्क		rsv_gdt;
	पूर्णाङ्क		csum_flag;
	पूर्णाङ्क		bigalloc_flag;
	पूर्णाङ्क		io_flags;
	पूर्णाङ्क		has_bg;
	अचिह्नित	reserved_inos;
	वर्ण		*buf = 0;
	वर्ण		c;
	द्विगुण		reserved_अनुपातः;
	वर्ण		*time_env;

	यदि (!param || !ext2fs_खण्डानि_गणनम्(param))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	retval = ext2fs_get_mem(परिमाण(संरचना struct_ext2_filsys), &fs);
	यदि (retval)
		प्रत्यागम retval;

	स्मृतिं_पूरय(fs, 0, परिमाण(संरचना struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags | EXT2_FLAG_RW;
	fs->umask = 022;
	fs->default_bitmap_type = EXT2FS_BMAP64_RBTREE;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_अष्टकानि;
#यदि_अन्तः

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	यदि (time_env) {
		fs->now = strtoul(time_env, रिक्त, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} अन्यथा {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		यदि (time_env)
			fs->now = strtoul(time_env, रिक्त, 0);
	}

	io_flags = IO_FLAG_RW;
	यदि (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	यदि (flags & EXT2_FLAG_प्रत्यक्षम्_IO)
		io_flags |= IO_FLAG_प्रत्यक्षम्_IO;
	io_flags |= O_द्विमान;
	retval = manager->उद्घाटय(name, io_flags, &fs->io);
	यदि (retval)
		गच्छ cleanup;
	fs->image_io = fs->io;
	fs->io->app_दत्त = fs;
	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(name)+1, &fs->device_name);
	यदि (retval)
		गच्छ cleanup;

	वाक्यं_प्रतिलिख(fs->device_name, name);
	retval = ext2fs_get_mem(SUPERBLOCK_आकार, &super);
	यदि (retval)
		गच्छ cleanup;
	fs->super = super;

	स्मृतिं_पूरय(super, 0, SUPERBLOCK_आकार);

#परिभाषय समुच्चयः_क्षेत्रम्(क्षेत्रम्, स्वभाव) (super->क्षेत्रम् = param->क्षेत्रम् ? \
				   param->क्षेत्रम् : (स्वभाव))
#परिभाषय नियोजय_क्षेत्रम्(क्षेत्रम्)	(super->क्षेत्रम् = param->क्षेत्रम्)

	super->s_magic = EXT2_SUPER_MAGIC;
	super->s_state = EXT2_VALID_FS;

	bigalloc_flag = ext2fs_has_feature_bigalloc(param);

	नियोजय_क्षेत्रम्(s_log_खण्डम्_आकार);

	यदि (bigalloc_flag) {
		समुच्चयः_क्षेत्रम्(s_log_cluster_आकार, super->s_log_खण्डम्_आकार+4);
		यदि (super->s_log_खण्डम्_आकार > super->s_log_cluster_आकार) {
			retval = EXT2_ET_INVALID_तर्कः;
			गच्छ cleanup;
		}
	} अन्यथा
		super->s_log_cluster_आकार = super->s_log_खण्डम्_आकार;

	समुच्चयः_क्षेत्रम्(s_first_दत्त_खण्डम्, super->s_log_cluster_आकार ? 0 : 1);
	समुच्चयः_क्षेत्रम्(s_max_mnt_गणनम्, 0);
	समुच्चयः_क्षेत्रम्(s_errors, EXT2_ERRORS_DEFAULT);
	समुच्चयः_क्षेत्रम्(s_feature_compat, 0);
	समुच्चयः_क्षेत्रम्(s_feature_incompat, 0);
	समुच्चयः_क्षेत्रम्(s_feature_ro_compat, 0);
	समुच्चयः_क्षेत्रम्(s_default_mount_opts, 0);
	समुच्चयः_क्षेत्रम्(s_first_meta_bg, 0);
	समुच्चयः_क्षेत्रम्(s_raid_stride, 0);		/* default stride size: 0 */
	समुच्चयः_क्षेत्रम्(s_raid_stripe_विस्तारः, 0);	/* default stripe width: 0 */
	समुच्चयः_क्षेत्रम्(s_log_groups_प्रत्येकम्_flex, 0);
	समुच्चयः_क्षेत्रम्(s_flags, 0);
	नियोजय_क्षेत्रम्(s_backup_bgs[0]);
	नियोजय_क्षेत्रम्(s_backup_bgs[1]);

	नियोजय_क्षेत्रम्(s_encoding);
	नियोजय_क्षेत्रम्(s_encoding_flags);

	यदि (ext2fs_has_feature_casefold(param))
		fs->encoding = ext2fs_आरोपय_nls_सारणी(param->s_encoding);

	यदि (super->s_feature_incompat & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
		retval = EXT2_ET_UNSUPP_FEATURE;
		गच्छ cleanup;
	}
	यदि (super->s_feature_ro_compat & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP) {
		retval = EXT2_ET_RO_UNSUPP_FEATURE;
		गच्छ cleanup;
	}

	समुच्चयः_क्षेत्रम्(s_rev_level, EXT2_GOOD_OLD_REV);
	यदि (super->s_rev_level >= EXT2_DYNAMIC_REV) {
		समुच्चयः_क्षेत्रम्(s_first_ino, EXT2_GOOD_OLD_FIRST_INO);
		समुच्चयः_क्षेत्रम्(s_inode_आकार, EXT2_GOOD_OLD_INODE_आकार);
		यदि (super->s_inode_आकार >= परिमाण(संरचना ext2_inode_large)) {
			पूर्णाङ्क extra_isize = परिमाण(संरचना ext2_inode_large) -
				EXT2_GOOD_OLD_INODE_आकार;
			समुच्चयः_क्षेत्रम्(s_min_extra_isize, extra_isize);
			समुच्चयः_क्षेत्रम्(s_want_extra_isize, extra_isize);
		}
	} अन्यथा {
		super->s_first_ino = EXT2_GOOD_OLD_FIRST_INO;
		super->s_inode_आकार = EXT2_GOOD_OLD_INODE_आकार;
	}

	समुच्चयः_क्षेत्रम्(s_checkinterval, 0);
	ext2fs_समुच्चयः_tstamp(super, s_mkfs_time, ext2fsP_get_time(fs));
	ext2fs_समुच्चयः_tstamp(super, s_lastcheck, ext2fsP_get_time(fs));

	super->s_creator_os = CREATOR_OS;

	fs->fragsize = fs->blocksize = EXT2_खण्डम्_आकार(super);
	fs->cluster_अनुपातः_द्वयङ्काः = super->s_log_cluster_आकार -
		super->s_log_खण्डम्_आकार;

	यदि (bigalloc_flag) {
		अचिह्नित दीर्घ दीर्घ bpg;

		यदि (param->s_खण्डानि_प्रत्येकम्_गणः &&
		    param->s_clusters_प्रत्येकम्_गणः &&
		    ((param->s_clusters_प्रत्येकम्_गणः * EXT2_FS_CLUSTER_अनुपातः(fs)) !=
		     param->s_खण्डानि_प्रत्येकम्_गणः)) {
			retval = EXT2_ET_INVALID_तर्कः;
			गच्छ cleanup;
		}
		यदि (param->s_clusters_प्रत्येकम्_गणः)
			नियोजय_क्षेत्रम्(s_clusters_प्रत्येकम्_गणः);
		अन्यथा यदि (param->s_खण्डानि_प्रत्येकम्_गणः)
			super->s_clusters_प्रत्येकम्_गणः = 
				param->s_खण्डानि_प्रत्येकम्_गणः /
				EXT2_FS_CLUSTER_अनुपातः(fs);
		अन्यथा यदि (super->s_log_cluster_आकार + 15 < 32)
			super->s_clusters_प्रत्येकम्_गणः = fs->blocksize * 8;
		अन्यथा
			super->s_clusters_प्रत्येकम्_गणः = (fs->blocksize - 1) * 8;
		यदि (super->s_clusters_प्रत्येकम्_गणः > EXT2_MAX_CLUSTERS_प्रत्येकम्_गणः(super))
			super->s_clusters_प्रत्येकम्_गणः = EXT2_MAX_CLUSTERS_प्रत्येकम्_गणः(super);
		bpg = EXT2FS_C2B(fs,
			(अचिह्नित दीर्घ दीर्घ) super->s_clusters_प्रत्येकम्_गणः);
		यदि (bpg >= (((अचिह्नित दीर्घ दीर्घ) 1) << 32)) {
			retval = EXT2_ET_INVALID_तर्कः;
			गच्छ cleanup;
		}
		super->s_खण्डानि_प्रत्येकम्_गणः = bpg;
	} अन्यथा {
		समुच्चयः_क्षेत्रम्(s_खण्डानि_प्रत्येकम्_गणः, fs->blocksize * 8);
		यदि (super->s_खण्डानि_प्रत्येकम्_गणः > EXT2_MAX_खण्डानि_प्रत्येकम्_गणः(super))
			super->s_खण्डानि_प्रत्येकम्_गणः = EXT2_MAX_खण्डानि_प्रत्येकम्_गणः(super);
		super->s_clusters_प्रत्येकम्_गणः = super->s_खण्डानि_प्रत्येकम्_गणः;
	}

	ext2fs_खण्डानि_गणनम्_समुच्चयः(super, ext2fs_खण्डानि_गणनम्(param) &
				~((blk64_t) EXT2_FS_CLUSTER_आवरणम्(fs)));
	ext2fs_r_खण्डानि_गणनम्_समुच्चयः(super, ext2fs_r_खण्डानि_गणनम्(param));
	यदि (ext2fs_r_खण्डानि_गणनम्(super) >= ext2fs_खण्डानि_गणनम्(param)) {
		retval = EXT2_ET_INVALID_तर्कः;
		गच्छ cleanup;
	}

	समुच्चयः_क्षेत्रम्(s_mmp_update_interval, 0);

	/*
	 * If we're creating an external journal device, we don't need
	 * to bother with the rest.
	 */
	यदि (ext2fs_has_feature_journal_dev(super)) {
		fs->गणः_desc_गणनम् = 0;
		ext2fs_mark_super_dirty(fs);
		*ret_fs = fs;
		प्रत्यागम 0;
	}

retry:
	fs->गणः_desc_गणनम् = (dgrp_t) ext2fs_div64_ceil(
		ext2fs_खण्डानि_गणनम्(super) - super->s_first_दत्त_खण्डम्,
		EXT2_खण्डानि_प्रत्येकम्_गणः(super));
	यदि (fs->गणः_desc_गणनम् == 0) {
		retval = EXT2_ET_TOOSMALL;
		गच्छ cleanup;
	}

	समुच्चयः_क्षेत्रम्(s_desc_आकार,
		  ext2fs_has_feature_64bit(super) ?
		  EXT2_MIN_DESC_आकार_64_BIT : 0);

	fs->desc_खण्डानि = ext2fs_div_ceil(fs->गणः_desc_गणनम्,
					  EXT2_DESC_प्रत्येकम्_खण्डम्(super));

	i = fs->blocksize >= 4096 ? 1 : 4096 / fs->blocksize;

	यदि (ext2fs_has_feature_64bit(super) &&
	    (ext2fs_खण्डानि_गणनम्(super) / i) >= (1ULL << 32))
		समुच्चयः_क्षेत्रम्(s_inodes_गणनम्, ~0U);
	अन्यथा
		समुच्चयः_क्षेत्रम्(s_inodes_गणनम्, ext2fs_खण्डानि_गणनम्(super) / i);

	/*
	 * There should be at least as many inodes as the user
	 * requested.  Figure out how many inodes per group that
	 * should be.  But make sure that we don't allocate more than
	 * one bitmap's worth of inodes each group.
	 */
	ipg = ext2fs_div_ceil(super->s_inodes_गणनम्, fs->गणः_desc_गणनम्);
	यदि (ipg > fs->blocksize * 8) {
		यदि (!bigalloc_flag && super->s_खण्डानि_प्रत्येकम्_गणः >= 256) {
			/* Try again with slightly different parameters */
			super->s_खण्डानि_प्रत्येकम्_गणः -= 8;
			ext2fs_खण्डानि_गणनम्_समुच्चयः(super,
						ext2fs_खण्डानि_गणनम्(param));
			super->s_clusters_प्रत्येकम्_गणः = super->s_खण्डानि_प्रत्येकम्_गणः;
			गच्छ retry;
		} अन्यथा {
			retval = EXT2_ET_TOO_MANY_INODES;
			गच्छ cleanup;
		}
	}

	यदि (ipg > (अचिह्नित) EXT2_MAX_INODES_प्रत्येकम्_गणः(super))
		ipg = EXT2_MAX_INODES_प्रत्येकम्_गणः(super);

ipg_retry:
	super->s_inodes_प्रत्येकम्_गणः = ipg;

	/*
	 * Make sure the number of inodes per group completely fills
	 * the inode table blocks in the descriptor.  If not, add some
	 * additional inodes/group.  Waste not, want not...
	 */
	fs->inode_खण्डानि_प्रत्येकम्_गणः = (((super->s_inodes_प्रत्येकम्_गणः *
					EXT2_INODE_आकार(super)) +
				       EXT2_खण्डम्_आकार(super) - 1) /
				      EXT2_खण्डम्_आकार(super));
	super->s_inodes_प्रत्येकम्_गणः = ((fs->inode_खण्डानि_प्रत्येकम्_गणः *
				      EXT2_खण्डम्_आकार(super)) /
				     EXT2_INODE_आकार(super));
	/*
	 * Finally, make sure the number of inodes per group is a
	 * multiple of 8.  This is needed to simplify the bitmap
	 * splicing code.
	 */
	यदि (super->s_inodes_प्रत्येकम्_गणः < 8)
		super->s_inodes_प्रत्येकम्_गणः = 8;
	super->s_inodes_प्रत्येकम्_गणः &= ~7;
	fs->inode_खण्डानि_प्रत्येकम्_गणः = (((super->s_inodes_प्रत्येकम्_गणः *
					EXT2_INODE_आकार(super)) +
				       EXT2_खण्डम्_आकार(super) - 1) /
				      EXT2_खण्डम्_आकार(super));

	/*
	 * adjust inode count to reflect the adjusted inodes_per_group
	 */
	यदि ((__u64)super->s_inodes_प्रत्येकम्_गणः * fs->गणः_desc_गणनम् > ~0U) {
		ipg--;
		गच्छ ipg_retry;
	}
	super->s_inodes_गणनम् = super->s_inodes_प्रत्येकम्_गणः *
		fs->गणः_desc_गणनम्;
	/*
	 * Make sure we have at least EXT2_FIRST_INO + 1 inodes, so
	 * that we have enough inodes for the filesystem(!)
	 */
	यदि (super->s_inodes_गणनम् < EXT2_FIRST_INODE(super)+1) {
		ipg += 8;
		गच्छ ipg_retry;
	}

	super->s_free_inodes_गणनम् = super->s_inodes_गणनम्;

	/*
	 * check the number of reserved group descriptor table blocks
	 */
	यदि (ext2fs_has_feature_resize_inode(super))
		rsv_gdt = calc_reserved_gdt_खण्डानि(fs);
	अन्यथा
		rsv_gdt = 0;
	समुच्चयः_क्षेत्रम्(s_reserved_gdt_खण्डानि, rsv_gdt);
	यदि (super->s_reserved_gdt_खण्डानि > EXT2_ADDR_प्रत्येकम्_खण्डम्(super)) {
		retval = EXT2_ET_RES_GDT_खण्डानि;
		गच्छ cleanup;
	}
	/* Enable meta_bg if we'd lose more than 3/4 of a BG to GDT blocks. */
	यदि (super->s_reserved_gdt_खण्डानि + fs->desc_खण्डानि >
	    super->s_खण्डानि_प्रत्येकम्_गणः * 3 / 4) {
		ext2fs_समुच्चयः_feature_meta_bg(fs->super);
		ext2fs_clear_feature_resize_inode(fs->super);
		समुच्चयः_क्षेत्रम्(s_reserved_gdt_खण्डानि, 0);
	}

	/*
	 * Calculate the maximum number of bookkeeping blocks per
	 * group.  It includes the superblock, the block group
	 * descriptors, the block bitmap, the inode bitmap, the inode
	 * table, and the reserved gdt blocks.
	 */
	overhead = (पूर्णाङ्क) (3 + fs->inode_खण्डानि_प्रत्येकम्_गणः +
			  super->s_reserved_gdt_खण्डानि);

	यदि (ext2fs_has_feature_meta_bg(fs->super))
		overhead++;
	अन्यथा
		overhead += fs->desc_खण्डानि;

	/* This can only happen if the user requested too many inodes */
	यदि (overhead > super->s_खण्डानि_प्रत्येकम्_गणः) {
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
	overhead = (पूर्णाङ्क) (2 + fs->inode_खण्डानि_प्रत्येकम्_गणः);
	has_bg = 0;
	यदि (ext2fs_has_feature_sparse_super2(super)) {
		/*
		 * We have to do this manually since
		 * super->s_backup_bgs hasn't been set up yet.
		 */
		यदि (fs->गणः_desc_गणनम् == 2)
			has_bg = param->s_backup_bgs[0] != 0;
		अन्यथा
			has_bg = param->s_backup_bgs[1] != 0;
	} अन्यथा
		has_bg = ext2fs_bg_has_super(fs, fs->गणः_desc_गणनम् - 1);
	यदि (has_bg)
		overhead += 1 + fs->desc_खण्डानि + super->s_reserved_gdt_खण्डानि;
	rem = ((ext2fs_खण्डानि_गणनम्(super) - super->s_first_दत्त_खण्डम्) %
	       super->s_खण्डानि_प्रत्येकम्_गणः);
	यदि ((fs->गणः_desc_गणनम् == 1) && rem && (rem < overhead)) {
		retval = EXT2_ET_TOOSMALL;
		गच्छ cleanup;
	}
	यदि (rem && (rem < overhead+50)) {
		ext2fs_खण्डानि_गणनम्_समुच्चयः(super, ext2fs_खण्डानि_गणनम्(super) -
					rem);
		/*
		 * If blocks count is changed, we need to recalculate
		 * reserved blocks count not to exceed 50%.
		 */
		reserved_अनुपातः = 100.0 * ext2fs_r_खण्डानि_गणनम्(param) /
			ext2fs_खण्डानि_गणनम्(param);
		ext2fs_r_खण्डानि_गणनम्_समुच्चयः(super, reserved_अनुपातः *
			ext2fs_खण्डानि_गणनम्(super) / 100.0);

		गच्छ retry;
	}

	/*
	 * At this point we know how big the filesystem will be.  So
	 * we can do any and all allocations that depend on the block
	 * count.
	 */

	/* Set up the locations of the backup superblocks */
	यदि (ext2fs_has_feature_sparse_super2(super)) {
		यदि (super->s_backup_bgs[0] >= fs->गणः_desc_गणनम्)
			super->s_backup_bgs[0] = fs->गणः_desc_गणनम् - 1;
		यदि (super->s_backup_bgs[1] >= fs->गणः_desc_गणनम्)
			super->s_backup_bgs[1] = fs->गणः_desc_गणनम् - 1;
		यदि (super->s_backup_bgs[0] == super->s_backup_bgs[1])
			super->s_backup_bgs[1] = 0;
		यदि (super->s_backup_bgs[0] > super->s_backup_bgs[1]) {
			__u32 t = super->s_backup_bgs[0];
			super->s_backup_bgs[0] = super->s_backup_bgs[1];
			super->s_backup_bgs[1] = t;
		}
	}

	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(fs->device_name) + 80, &buf);
	यदि (retval)
		गच्छ cleanup;

	वाक्यं_प्रतिलिख(buf, "block bitmap for ");
	वाक्यं_योजय(buf, fs->device_name);
	retval = ext2fs_allocate_subcluster_bitmap(fs, buf, &fs->खण्डम्_map);
	यदि (retval)
		गच्छ cleanup;

	वाक्यं_प्रतिलिख(buf, "inode bitmap for ");
	वाक्यं_योजय(buf, fs->device_name);
	retval = ext2fs_allocate_inode_bitmap(fs, buf, &fs->inode_map);
	यदि (retval)
		गच्छ cleanup;

	ext2fs_free_mem(&buf);

	retval = ext2fs_get_array(fs->desc_खण्डानि, fs->blocksize,
				&fs->गणः_desc);
	यदि (retval)
		गच्छ cleanup;

	स्मृतिं_पूरय(fs->गणः_desc, 0, (size_t) fs->desc_खण्डानि * fs->blocksize);

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
	free_खण्डानि = 0;
	csum_flag = ext2fs_has_गणः_desc_csum(fs);
	reserved_inos = super->s_first_ino;
	प्रति (i = 0; i < fs->गणः_desc_गणनम्; i++) {
		blk_t grp_free_खण्डानि;
		ext2_ino_t inodes;

		retval = ext2fs_reserve_super_and_bgd2(fs, i,
						       fs->खण्डम्_map,
						       &numblocks);
		यदि (retval)
			गच्छ cleanup;

		/*
		 * Don't set the BLOCK_UNINIT group for the last group
		 * because the block bitmap needs to be padded.
		 */
		यदि (csum_flag) {
			यदि (i != fs->गणः_desc_गणनम् - 1 && numblocks == 0)
				ext2fs_bg_flags_समुच्चयः(fs, i,
						    EXT2_BG_खण्डम्_UNINIT);
			ext2fs_bg_flags_समुच्चयः(fs, i, EXT2_BG_INODE_UNINIT);
			inodes = super->s_inodes_प्रत्येकम्_गणः;
			यदि (reserved_inos) {
				यदि (inodes > reserved_inos) {
					inodes -= reserved_inos;
					reserved_inos = 0;
				} अन्यथा {
					reserved_inos -= inodes;
					inodes = 0;
				}
			}
			ext2fs_bg_itable_अप्रयुक्तम्_समुच्चयः(fs, i, inodes);
		}

		यदि (!fs->super->s_log_groups_प्रत्येकम्_flex)
			numblocks += 2 + fs->inode_खण्डानि_प्रत्येकम्_गणः;

		grp_free_खण्डानि = ext2fs_गणः_खण्डानि_गणनम्(fs, i) - numblocks;
		free_खण्डानि += grp_free_खण्डानि;
		ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(fs, i, grp_free_खण्डानि);
		ext2fs_bg_free_inodes_गणनम्_समुच्चयः(fs, i, fs->super->s_inodes_प्रत्येकम्_गणः);
		ext2fs_bg_प्रयुक्तम्_dirs_गणनम्_समुच्चयः(fs, i, 0);
		ext2fs_गणः_desc_csum_समुच्चयः(fs, i);
	}
	free_खण्डानि &= ~EXT2_FS_CLUSTER_आवरणम्(fs);
	ext2fs_free_खण्डानि_गणनम्_समुच्चयः(super, free_खण्डानि);

	c = (वर्ण) 255;
	यदि (((पूर्णाङ्क) c) == -1) {
		super->s_flags |= EXT2_FLAGS_SIGNED_HASH;
	} अन्यथा {
		super->s_flags |= EXT2_FLAGS_UNSIGNED_HASH;
	}

	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ext2fs_mark_ib_dirty(fs);

	io_channel_समुच्चयः_blksize(fs->io, fs->blocksize);

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
	अचिह्नित पूर्णाङ्क	गणः = 0;
	अचिह्नित पूर्णाङ्क	गणनम् = 0;
	पूर्णाङ्क		योग_free = 0;
	पूर्णाङ्क		गणः_free = 0;
	पूर्णाङ्क		चरमः_allocated = 0;
	पूर्णाङ्क		uninit;

	/*
	 * First calculate the block statistics
	 */
	uninit = 1;
	प्रति (blk = fs->super->s_first_दत्त_खण्डम्;
	     blk < ext2fs_खण्डानि_गणनम्(fs->super); blk++) {
		यदि (!ext2fs_fast_test_खण्डम्_bitmap2(fs->खण्डम्_map, blk)) {
			गणः_free++;
			योग_free++;
		} अन्यथा {
			uninit = 0;
		}
		गणनम्++;
		यदि ((गणनम् == fs->super->s_खण्डानि_प्रत्येकम्_गणः) ||
		    (blk == ext2fs_खण्डानि_गणनम्(fs->super)-1)) {
			ext2fs_bg_free_खण्डानि_गणनम्_समुच्चयः(fs, गणः,
							गणः_free);
			यदि (!super_only) {
				यदि (uninit && blk !=
					ext2fs_खण्डानि_गणनम्(fs->super) - 1)
					ext2fs_bg_flags_समुच्चयः(fs, गणः,
							EXT2_BG_खण्डम्_UNINIT);
				अन्यथा
					ext2fs_bg_flags_clear(fs, गणः,
							EXT2_BG_खण्डम्_UNINIT);
			}
			गणनम् = 0;
			गणः_free = 0;
			uninit = 1;
			गणः++;
		}
	}
	योग_free = EXT2FS_C2B(fs, योग_free);
	ext2fs_free_खण्डानि_गणनम्_समुच्चयः(fs->super, योग_free);

	/*
	 * Next, calculate the inode statistics
	 */
	गणः_free = 0;
	योग_free = 0;
	चरमः_allocated = 0;
	गणनम् = 0;
	गणः = 0;

	/* Protect loop from wrap-around if s_inodes_count maxed */
	प्रति (ino = 1; ino <= fs->super->s_inodes_गणनम् && ino > 0; ino++) {
		यदि (!ext2fs_test_inode_bitmap2(fs->inode_map, ino)) {
			गणः_free++;
			योग_free++;
		} अन्यथा {
			चरमः_allocated = ino;
		}
		गणनम्++;
		यदि ((गणनम् == fs->super->s_inodes_प्रत्येकम्_गणः) ||
		    (ino == fs->super->s_inodes_गणनम्)) {
			यदि (!super_only) {
				यदि (चरमः_allocated) {
					ext2fs_bg_flags_clear(fs, गणः,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_अप्रयुक्तम्_समुच्चयः(fs, गणः,
						fs->super->s_inodes_प्रत्येकम्_गणः -
						(चरमः_allocated %
						fs->super->s_inodes_प्रत्येकम्_गणः));
				} अन्यथा {
					ext2fs_bg_flags_समुच्चयः(fs, गणः,
						EXT2_BG_INODE_UNINIT);
					ext2fs_bg_itable_अप्रयुक्तम्_समुच्चयः(fs, गणः,
									0);
				}
				ext2fs_bg_free_inodes_गणनम्_समुच्चयः(fs, गणः,
								गणः_free);
			}
			गणः++;
			गणनम् = 0;
			गणः_free = 0;
			चरमः_allocated = 0;
		}
	}
	fs->super->s_free_inodes_गणनम् = योग_free;
	ext2fs_mark_super_dirty(fs);
	प्रत्यागम 0;
}
