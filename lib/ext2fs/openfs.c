/*
 * openfs.c --- open an ext2 filesystem
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
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
#यदि_परिभाषितम् HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"

#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "e2image.h"

blk64_t ext2fs_descriptor_खण्डम्_loc2(ext2_filsys fs, blk64_t गणः_खण्डम्,
				     dgrp_t i)
{
	पूर्णाङ्क	bg;
	पूर्णाङ्क	has_super = 0, गणः_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	यदि (i == 0 && fs->blocksize == 1024 && EXT2_FS_CLUSTER_अनुपातः(fs) > 1)
		गणः_zero_adjust = 1;

	यदि (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_first_meta_bg))
		प्रत्यागम गणः_खण्डम् + i + 1 + गणः_zero_adjust;

	bg = EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super) * i;
	यदि (ext2fs_bg_has_super(fs, bg))
		has_super = 1;
	ret_blk = ext2fs_गणः_first_block2(fs, bg);
	/*
	 * If group_block is not the normal value, we're trying to use
	 * the backup group descriptors and superblock --- so use the
	 * alternate location of the second block group in the
	 * metablock group.  Ideally we should be testing each bg
	 * descriptor block individually for correctness, but we don't
	 * have the infrastructure in place to do that.
	 */
	यदि (गणः_खण्डम् != fs->super->s_first_दत्त_खण्डम् &&
	    ((ret_blk + has_super + fs->super->s_खण्डानि_प्रत्येकम्_गणः) <
	     ext2fs_खण्डानि_गणनम्(fs->super))) {
		ret_blk += fs->super->s_खण्डानि_प्रत्येकम्_गणः;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		यदि (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		अन्यथा
			has_super = 0;
	}
	प्रत्यागम ret_blk + has_super + गणः_zero_adjust;
}

blk_t ext2fs_descriptor_खण्डम्_loc(ext2_filsys fs, blk_t गणः_खण्डम्, dgrp_t i)
{
	प्रत्यागम ext2fs_descriptor_खण्डम्_loc2(fs, गणः_खण्डम्, i);
}

errcode_t ext2fs_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, पूर्णाङ्क superblock,
		      अचिह्नित पूर्णाङ्क खण्डम्_आकार, io_manager manager,
		      ext2_filsys *ret_fs)
{
	प्रत्यागम ext2fs_open2(name, 0, flags, superblock, खण्डम्_आकार,
			    manager, ret_fs);
}

स्थिर शून्य खण्डम्_sha_map_free_प्रविष्टिः(शून्य *दत्त)
{
	मुक्त(दत्त);
	प्रत्यागम;
}

/*
 *  Note: if superblock is non-zero, block-size must also be non-zero.
 * 	Superblock and block_size can be zero to use the default size.
 *
 * Valid flags for ext2fs_open()
 *
 * 	EXT2_FLAG_RW	- Open the filesystem for read/write.
 * 	EXT2_FLAG_FORCE - Open the filesystem even if some of the
 *				features aren't supported.
 *	EXT2_FLAG_JOURNAL_DEV_OK - Open an ext3 journal device
 *	EXT2_FLAG_SKIP_MMP - Open without multi-mount protection check.
 *	EXT2_FLAG_64BITS - Allow 64-bit bitfields (needed for large
 *				filesystems)
 */
errcode_t ext2fs_open2(ध्रुव वर्ण *name, ध्रुव वर्ण *io_options,
		       पूर्णाङ्क flags, पूर्णाङ्क superblock,
		       अचिह्नित पूर्णाङ्क खण्डम्_आकार, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	अचिह्नित दीर्घ	i, first_meta_bg;
	__u32		features;
	अचिह्नित पूर्णाङ्क	खण्डानि_प्रत्येकम्_गणः, io_flags;
	blk64_t		गणः_खण्डम्, blk;
	वर्ण		*dest, *cp;
	पूर्णाङ्क		गणः_zero_adjust = 0;
	अचिह्नित पूर्णाङ्क	inode_आकार;
	__u64		groups_cnt;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	अचिह्नित पूर्णाङ्क	groups_प्रत्येकम्_खण्डम्;
	संरचना ext2_गणः_desc *gdp;
	पूर्णाङ्क		j;
#यदि_अन्तः
	वर्ण		*time_env;
	पूर्णाङ्क		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(परिमाण(संरचना struct_ext2_filsys), &fs);
	यदि (retval)
		प्रत्यागम retval;

	स्मृतिं_पूरय(fs, 0, परिमाण(संरचना struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	यदि (time_env) {
		fs->now = strtoul(time_env, रिक्त, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} अन्यथा {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		यदि (time_env)
			fs->now = strtoul(time_env, रिक्त, 0);
	}

	retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(name)+1, &fs->device_name);
	यदि (retval)
		गच्छ cleanup;
	वाक्यं_प्रतिलिख(fs->device_name, name);
	cp = strchr(fs->device_name, '?');
	यदि (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	यदि (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	यदि (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	यदि (flags & EXT2_FLAG_प्रत्यक्षम्_IO)
		io_flags |= IO_FLAG_प्रत्यक्षम्_IO;
	यदि (flags & EXT2_FLAG_तन्तवः)
		io_flags |= IO_FLAG_तन्तवः;
	retval = manager->उद्घाटय(fs->device_name, io_flags, &fs->io);
	यदि (retval)
		गच्छ cleanup;
	यदि (io_options &&
	    (retval = io_channel_समुच्चयः_options(fs->io, io_options)))
		गच्छ cleanup;
	fs->image_io = fs->io;
	fs->io->app_दत्त = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_आकार, &fs->super);
	यदि (retval)
		गच्छ cleanup;
	यदि (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(परिमाण(संरचना ext2_image_hdr),
					&fs->image_शीर्षकम्);
		यदि (retval)
			गच्छ cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(पूर्णाङ्क)परिमाण(संरचना ext2_image_hdr),
					     fs->image_शीर्षकम्);
		यदि (retval)
			गच्छ cleanup;
		यदि (ext2fs_le32_गन्तव्यम्_cpu(fs->image_शीर्षकम्->magic_सङ्ख्या) != EXT2_ET_MAGIC_E2IMAGE)
			प्रत्यागम EXT2_ET_MAGIC_E2IMAGE;
		superblock = 1;
		खण्डम्_आकार = ext2fs_le32_गन्तव्यम्_cpu(fs->image_शीर्षकम्->fs_blocksize);
	}

	/*
	 * If the user specifies a specific block # for the
	 * superblock, then he/she must also specify the block size!
	 * Otherwise, read the master superblock located at offset
	 * SUPERBLOCK_OFFSET from the start of the partition.
	 *
	 * Note: we only save a backup copy of the superblock if we
	 * are reading the superblock from the primary superblock location.
	 */
	यदि (superblock) {
		यदि (!खण्डम्_आकार) {
			retval = EXT2_ET_INVALID_तर्कः;
			गच्छ cleanup;
		}
		io_channel_समुच्चयः_blksize(fs->io, खण्डम्_आकार);
		गणः_खण्डम् = superblock;
		fs->orig_super = 0;
	} अन्यथा {
		io_channel_समुच्चयः_blksize(fs->io, SUPERBLOCK_विस्थापनम्);
		superblock = 1;
		गणः_खण्डम् = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_आकार, &fs->orig_super);
		यदि (retval)
			गच्छ cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_आकार,
				     fs->super);
	यदि (retval)
		गच्छ cleanup;
	यदि (fs->orig_super)
		स्मृतिं_प्रतिलिख(fs->orig_super, fs->super, SUPERBLOCK_आकार);

	यदि (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		यदि (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		यदि (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			यदि (csum_retries++ < 3)
				गच्छ retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_अष्टकानि;
	ext2fs_swap_super(fs->super);
#अन्यथा
	यदि (fs->flags & EXT2_FLAG_SWAP_अष्टकानि) {
		retval = EXT2_ET_UNIMPLEMENTED;
		गच्छ cleanup;
	}
#यदि_अन्तः

	यदि (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	यदि (retval)
		गच्छ cleanup;

	यदि (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_उच्चः;
		गच्छ cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	यदि (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#यदि_परिभाषितम् EXT2_LIB_SOFTSUPP_INCOMPAT
		यदि (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#यदि_अन्तः
		यदि (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			गच्छ cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#यदि_परिभाषितम् EXT2_LIB_SOFTSUPP_RO_COMPAT
		यदि (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#यदि_अन्तः
		यदि ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			गच्छ cleanup;
		}

		यदि (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			गच्छ cleanup;
		}
	}

	यदि ((fs->super->s_log_खण्डम्_आकार >
	     (अचिह्नित) (EXT2_MAX_खण्डम्_LOG_आकार - EXT2_MIN_खण्डम्_LOG_आकार)) ||
	    (fs->super->s_log_cluster_आकार >
	     (अचिह्नित) (EXT2_MAX_CLUSTER_LOG_आकार - EXT2_MIN_CLUSTER_LOG_आकार)) ||
	    (fs->super->s_log_खण्डम्_आकार > fs->super->s_log_cluster_आकार) ||
	    (fs->super->s_log_groups_प्रत्येकम्_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		गच्छ cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	यदि (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64_द्वयङ्काः)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		गच्छ cleanup;
	}

	यदि (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_खण्डम्_आकार != fs->super->s_log_cluster_आकार)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		गच्छ cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_खण्डम्_आकार(fs->super);
	inode_आकार = EXT2_INODE_आकार(fs->super);
	यदि ((inode_आकार < EXT2_GOOD_OLD_INODE_आकार) ||
	    (inode_आकार > fs->blocksize) ||
	    (inode_आकार & (inode_आकार - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		गच्छ cleanup;
	}

	/* Enforce the block group descriptor size */
	यदि (ext2fs_has_feature_64bit(fs->super)) {
		अचिह्नित desc_आकार = fs->super->s_desc_आकार;

		यदि (desc_आकार == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_आकार > EXT2_MAX_DESC_आकार) ||
		      (desc_आकार < EXT2_MIN_DESC_आकार_64_BIT) ||
		      (desc_आकार & (desc_आकार - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_आकार;
			गच्छ cleanup;
		}
	}

	fs->cluster_अनुपातः_द्वयङ्काः = fs->super->s_log_cluster_आकार -
		fs->super->s_log_खण्डम्_आकार;
	यदि (EXT2_खण्डानि_प्रत्येकम्_गणः(fs->super) !=
	    EXT2_CLUSTERS_प्रत्येकम्_गणः(fs->super) << fs->cluster_अनुपातः_द्वयङ्काः) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		गच्छ cleanup;
	}
	fs->inode_खण्डानि_प्रत्येकम्_गणः = ((EXT2_INODES_प्रत्येकम्_गणः(fs->super) *
				       EXT2_INODE_आकार(fs->super) +
				       EXT2_खण्डम्_आकार(fs->super) - 1) /
				      EXT2_खण्डम्_आकार(fs->super));
	यदि (खण्डम्_आकार) {
		यदि (खण्डम्_आकार != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_खण्डम्_आकार;
			गच्छ cleanup;
		}
	}
	/*
	 * Set the blocksize to the filesystem's blocksize.
	 */
	io_channel_समुच्चयः_blksize(fs->io, fs->blocksize);

	/*
	 * If this is an external journal device, don't try to read
	 * the group descriptors, because they're not there.
	 */
	यदि (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->गणः_desc_गणनम् = 0;
		*ret_fs = fs;
		प्रत्यागम 0;
	}

	यदि (EXT2_INODES_प्रत्येकम्_गणः(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		गच्छ cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_बीजम्(fs);

	/*
	 * Read group descriptors
	 */
	खण्डानि_प्रत्येकम्_गणः = EXT2_खण्डानि_प्रत्येकम्_गणः(fs->super);
	यदि (खण्डानि_प्रत्येकम्_गणः < 8 ||
	    खण्डानि_प्रत्येकम्_गणः > EXT2_MAX_खण्डानि_प्रत्येकम्_गणः(fs->super) ||
	    fs->inode_खण्डानि_प्रत्येकम्_गणः > EXT2_MAX_INODES_प्रत्येकम्_गणः(fs->super) ||
           EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super) == 0 ||
           fs->super->s_first_दत्त_खण्डम् >= ext2fs_खण्डानि_गणनम्(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		गच्छ cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_खण्डानि_गणनम्(fs->super) -
				       fs->super->s_first_दत्त_खण्डम्,
				       खण्डानि_प्रत्येकम्_गणः);
	यदि (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		गच्छ cleanup;
	}
	fs->गणः_desc_गणनम् = 	groups_cnt;
	यदि (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->गणः_desc_गणनम् * EXT2_INODES_प्रत्येकम्_गणः(fs->super) !=
	    fs->super->s_inodes_गणनम्) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		गच्छ cleanup;
	}
	fs->desc_खण्डानि = ext2fs_div_ceil(fs->गणः_desc_गणनम्,
					  EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super));
	यदि (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_first_meta_bg > fs->desc_खण्डानि) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		गच्छ cleanup;
	}
	यदि (flags & EXT2_FLAG_SUPER_ONLY)
		गच्छ skip_read_bg;
	retval = ext2fs_get_array(fs->desc_खण्डानि, fs->blocksize,
				&fs->गणः_desc);
	यदि (retval)
		गच्छ cleanup;
	यदि (!गणः_खण्डम्)
		गणः_खण्डम् = fs->super->s_first_दत्त_खण्डम्;
	/*
	 * On a FS with a 1K blocksize, block 0 is reserved for bootloaders
	 * so we must increment block numbers to any group 0 items.
	 *
	 * However, we cannot touch group_block directly because in the meta_bg
	 * case, the ext2fs_descriptor_block_loc2() function will interpret
	 * group_block != s_first_data_block to mean that we want to access the
	 * backup group descriptors.  This is not what we want if the caller
	 * set superblock == 0 (i.e. auto-detect the superblock), which is
	 * what's going on here.
	 */
	यदि (गणः_खण्डम् == 0 && fs->blocksize == 1024)
		गणः_zero_adjust = 1;
	dest = (वर्ण *) fs->गणः_desc;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	groups_प्रत्येकम्_खण्डम् = EXT2_DESC_प्रत्येकम्_खण्डम्(fs->super);
#यदि_अन्तः
	यदि (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		first_meta_bg = fs->super->s_first_meta_bg;
		यदि (first_meta_bg > fs->desc_खण्डानि)
			first_meta_bg = fs->desc_खण्डानि;
	} अन्यथा
		first_meta_bg = fs->desc_खण्डानि;
	यदि (first_meta_bg) {
		retval = io_channel_read_blk(fs->io, गणः_खण्डम् +
					     गणः_zero_adjust + 1,
					     first_meta_bg, dest);
		यदि (retval)
			गच्छ cleanup;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		gdp = (संरचना ext2_गणः_desc *) dest;
		प्रति (j=0; j < groups_प्रत्येकम्_खण्डम्*first_meta_bg; j++) {
			gdp = ext2fs_गणः_desc(fs, fs->गणः_desc, j);
			यदि (gdp)
				ext2fs_swap_गणः_desc2(fs, gdp);
		}
#यदि_अन्तः
		dest += fs->blocksize*first_meta_bg;
	}

	प्रति (i = first_meta_bg ; i < fs->desc_खण्डानि; i++) {
		blk = ext2fs_descriptor_खण्डम्_loc2(fs, गणः_खण्डम्, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	प्रति (i=first_meta_bg ; i < fs->desc_खण्डानि; i++) {
		blk = ext2fs_descriptor_खण्डम्_loc2(fs, गणः_खण्डम्, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		यदि (retval)
			गच्छ cleanup;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		प्रति (j=0; j < groups_प्रत्येकम्_खण्डम्; j++) {
			gdp = ext2fs_गणः_desc(fs, fs->गणः_desc,
						i * groups_प्रत्येकम्_खण्डम् + j);
			यदि (gdp)
				ext2fs_swap_गणः_desc2(fs, gdp);
		}
#यदि_अन्तः
		dest += fs->blocksize;
	}

	fs->stride = fs->super->s_raid_stride;

	/*
	 * If recovery is from backup superblock, Clear _UNININT flags &
	 * reset bg_itable_unused to zero
	 */
	यदि (superblock > 1 && ext2fs_has_गणः_desc_csum(fs)) {
		dgrp_t गणः;

		प्रति (गणः = 0; गणः < fs->गणः_desc_गणनम्; गणः++) {
			ext2fs_bg_flags_clear(fs, गणः, EXT2_BG_खण्डम्_UNINIT);
			ext2fs_bg_flags_clear(fs, गणः, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_अप्रयुक्तम्_समुच्चयः(fs, गणः, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_गणः_desc_csum_समुच्चयः(fs, गणः);
		}
		यदि (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	यदि (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_start(fs);
		यदि (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			गच्छ cleanup;
		}
	}

	यदि (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->खण्डम्_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					खण्डम्_sha_map_free_प्रविष्टिः, 4096);
		यदि (!fs->खण्डम्_sha_map) {
			retval = EXT2_ET_NO_स्मृतिः;
			गच्छ cleanup;
		}
		ext2fs_समुच्चयः_feature_shared_खण्डानि(fs->super);
	}

	यदि (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_आरोपय_nls_सारणी(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	प्रत्यागम 0;
cleanup:
	यदि (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = रिक्त;
	}
	*ret_fs = fs;
	प्रत्यागम retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_दत्त_io(ext2_filsys fs, io_channel *old_io)
{
	यदि ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		प्रत्यागम EXT2_ET_NOT_IMAGE_FILE;
	यदि (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	प्रत्यागम 0;
}

errcode_t ext2fs_समुच्चयः_दत्त_io(ext2_filsys fs, io_channel new_io)
{
	यदि ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		प्रत्यागम EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	प्रत्यागम 0;
}

errcode_t ext2fs_rewrite_गन्तव्यम्_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	यदि ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		प्रत्यागम EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_समुच्चयः_blksize(new_io, fs->blocksize);
	यदि (err)
		प्रत्यागम err;
	यदि ((new_io == fs->image_io) || (new_io == fs->io))
		प्रत्यागम 0;
	यदि ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	यदि (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	प्रत्यागम 0;
}
