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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif
#ifdef HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif

#অন্তর্ভাব "ext2_fs.h"

#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "e2image.h"

blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs, blk64_t group_block,
				     dgrp_t i)
{
	পূর্ণসংখ্যা	bg;
	পূর্ণসংখ্যা	has_super = 0, group_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	যদি (i == 0 && fs->blocksize == 1024 && EXT2FS_CLUSTER_RATIO(fs) > 1)
		group_zero_adjust = 1;

	যদি (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_first_meta_bg))
		ফেরত group_block + i + 1 + group_zero_adjust;

	bg = EXT2_DESC_PER_BLOCK(fs->super) * i;
	যদি (ext2fs_bg_has_super(fs, bg))
		has_super = 1;
	ret_blk = ext2fs_group_first_block2(fs, bg);
	/*
	 * If group_block is not the normal value, we're trying to use
	 * the backup group descriptors and superblock --- so use the
	 * alternate location of the second block group in the
	 * metablock group.  Ideally we should be testing each bg
	 * descriptor block individually for correctness, but we don't
	 * have the infrastructure in place to do that.
	 */
	যদি (group_block != fs->super->s_first_তথ্য_block &&
	    ((ret_blk + has_super + fs->super->s_blocks_per_group) <
	     ext2fs_blocks_গুনতি(fs->super))) {
		ret_blk += fs->super->s_blocks_per_group;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		যদি (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		অন্যথা
			has_super = 0;
	}
	ফেরত ret_blk + has_super + group_zero_adjust;
}

blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block, dgrp_t i)
{
	ফেরত ext2fs_descriptor_block_loc2(fs, group_block, i);
}

errcode_t ext2fs_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা superblock,
		      unsigned পূর্ণসংখ্যা block_আকার, io_manager manager,
		      ext2_filsys *ret_fs)
{
	ফেরত ext2fs_open2(name, 0, flags, superblock, block_আকার,
			    manager, ret_fs);
}

স্থির শূন্য block_sha_map_free_entry(শূন্য *তথ্য)
{
	মুক্ত(তথ্য);
	ফেরত;
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
errcode_t ext2fs_open2(ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *io_options,
		       পূর্ণসংখ্যা flags, পূর্ণসংখ্যা superblock,
		       unsigned পূর্ণসংখ্যা block_আকার, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	unsigned long	i, first_meta_bg;
	__u32		features;
	unsigned পূর্ণসংখ্যা	blocks_per_group, io_flags;
	blk64_t		group_block, blk;
	অক্ষর		*dest, *cp;
	পূর্ণসংখ্যা		group_zero_adjust = 0;
	unsigned পূর্ণসংখ্যা	inode_আকার;
	__u64		groups_cnt;
#ifdef শব্দগুলি_BIGENDIAN
	unsigned পূর্ণসংখ্যা	groups_per_block;
	কাঠামো ext2_group_desc *gdp;
	পূর্ণসংখ্যা		j;
#endif
	অক্ষর		*time_env;
	পূর্ণসংখ্যা		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(আকার(কাঠামো struct_ext2_filsys), &fs);
	যদি (retval)
		ফেরত retval;

	memset(fs, 0, আকার(কাঠামো struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	যদি (time_env) {
		fs->now = strtoul(time_env, ফাঁকা, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} অন্যথা {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		যদি (time_env)
			fs->now = strtoul(time_env, ফাঁকা, 0);
	}

	retval = ext2fs_get_mem(strlen(name)+1, &fs->device_name);
	যদি (retval)
		যাও cleanup;
	strcpy(fs->device_name, name);
	cp = strchr(fs->device_name, '?');
	যদি (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	যদি (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	যদি (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	যদি (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	যদি (flags & EXT2_FLAG_THREADS)
		io_flags |= IO_FLAG_THREADS;
	retval = manager->open(fs->device_name, io_flags, &fs->io);
	যদি (retval)
		যাও cleanup;
	যদি (io_options &&
	    (retval = io_channel_set_options(fs->io, io_options)))
		যাও cleanup;
	fs->image_io = fs->io;
	fs->io->app_তথ্য = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_আকার, &fs->super);
	যদি (retval)
		যাও cleanup;
	যদি (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(আকার(কাঠামো ext2_image_hdr),
					&fs->image_header);
		যদি (retval)
			যাও cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(পূর্ণসংখ্যা)আকার(কাঠামো ext2_image_hdr),
					     fs->image_header);
		যদি (retval)
			যাও cleanup;
		যদি (ext2fs_le32_to_cpu(fs->image_header->magic_সংখ্যা) != EXT2_ET_MAGIC_E2IMAGE)
			ফেরত EXT2_ET_MAGIC_E2IMAGE;
		superblock = 1;
		block_আকার = ext2fs_le32_to_cpu(fs->image_header->fs_blocksize);
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
	যদি (superblock) {
		যদি (!block_আকার) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			যাও cleanup;
		}
		io_channel_set_blksize(fs->io, block_আকার);
		group_block = superblock;
		fs->orig_super = 0;
	} অন্যথা {
		io_channel_set_blksize(fs->io, SUPERBLOCK_OFFSET);
		superblock = 1;
		group_block = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_আকার, &fs->orig_super);
		যদি (retval)
			যাও cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_আকার,
				     fs->super);
	যদি (retval)
		যাও cleanup;
	যদি (fs->orig_super)
		memcpy(fs->orig_super, fs->super, SUPERBLOCK_আকার);

	যদি (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		যদি (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		যদি (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			যদি (csum_retries++ < 3)
				যাও retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#ifdef শব্দগুলি_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
	ext2fs_swap_super(fs->super);
#অন্যথা
	যদি (fs->flags & EXT2_FLAG_SWAP_BYTES) {
		retval = EXT2_ET_UNIMPLEMENTED;
		যাও cleanup;
	}
#endif

	যদি (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	যদি (retval)
		যাও cleanup;

	যদি (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_উপরের;
		যাও cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	যদি (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#ifdef EXT2_LIB_SOFTSUPP_INCOMPAT
		যদি (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#endif
		যদি (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			যাও cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#ifdef EXT2_LIB_SOFTSUPP_RO_COMPAT
		যদি (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#endif
		যদি ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			যাও cleanup;
		}

		যদি (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			যাও cleanup;
		}
	}

	যদি ((fs->super->s_log_block_আকার >
	     (unsigned) (EXT2_MAX_BLOCK_LOG_আকার - EXT2_MIN_BLOCK_LOG_আকার)) ||
	    (fs->super->s_log_cluster_আকার >
	     (unsigned) (EXT2_MAX_CLUSTER_LOG_আকার - EXT2_MIN_CLUSTER_LOG_আকার)) ||
	    (fs->super->s_log_block_আকার > fs->super->s_log_cluster_আকার) ||
	    (fs->super->s_log_groups_per_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		যাও cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	যদি (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64BITS)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		যাও cleanup;
	}

	যদি (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_block_আকার != fs->super->s_log_cluster_আকার)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		যাও cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_BLOCK_আকার(fs->super);
	inode_আকার = EXT2_INODE_আকার(fs->super);
	যদি ((inode_আকার < EXT2_GOOD_OLD_INODE_আকার) ||
	    (inode_আকার > fs->blocksize) ||
	    (inode_আকার & (inode_আকার - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		যাও cleanup;
	}

	/* Enforce the block group descriptor size */
	যদি (ext2fs_has_feature_64bit(fs->super)) {
		unsigned desc_আকার = fs->super->s_desc_আকার;

		যদি (desc_আকার == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_আকার > EXT2_MAX_DESC_আকার) ||
		      (desc_আকার < EXT2_MIN_DESC_আকার_64_BIT) ||
		      (desc_আকার & (desc_আকার - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_আকার;
			যাও cleanup;
		}
	}

	fs->cluster_ratio_bits = fs->super->s_log_cluster_আকার -
		fs->super->s_log_block_আকার;
	যদি (EXT2_BLOCKS_PER_GROUP(fs->super) !=
	    EXT2_CLUSTERS_PER_GROUP(fs->super) << fs->cluster_ratio_bits) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		যাও cleanup;
	}
	fs->inode_blocks_per_group = ((EXT2_INODES_PER_GROUP(fs->super) *
				       EXT2_INODE_আকার(fs->super) +
				       EXT2_BLOCK_আকার(fs->super) - 1) /
				      EXT2_BLOCK_আকার(fs->super));
	যদি (block_আকার) {
		যদি (block_আকার != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_BLOCK_আকার;
			যাও cleanup;
		}
	}
	/*
	 * Set the blocksize to the filesystem's blocksize.
	 */
	io_channel_set_blksize(fs->io, fs->blocksize);

	/*
	 * If this is an external journal device, don't try to read
	 * the group descriptors, because they're not there.
	 */
	যদি (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->group_desc_গুনতি = 0;
		*ret_fs = fs;
		ফেরত 0;
	}

	যদি (EXT2_INODES_PER_GROUP(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		যাও cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_seed(fs);

	/*
	 * Read group descriptors
	 */
	blocks_per_group = EXT2_BLOCKS_PER_GROUP(fs->super);
	যদি (blocks_per_group < 8 ||
	    blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(fs->super) ||
	    fs->inode_blocks_per_group > EXT2_MAX_INODES_PER_GROUP(fs->super) ||
           EXT2_DESC_PER_BLOCK(fs->super) == 0 ||
           fs->super->s_first_তথ্য_block >= ext2fs_blocks_গুনতি(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		যাও cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_blocks_গুনতি(fs->super) -
				       fs->super->s_first_তথ্য_block,
				       blocks_per_group);
	যদি (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		যাও cleanup;
	}
	fs->group_desc_গুনতি = 	groups_cnt;
	যদি (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->group_desc_গুনতি * EXT2_INODES_PER_GROUP(fs->super) !=
	    fs->super->s_inodes_গুনতি) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		যাও cleanup;
	}
	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_গুনতি,
					  EXT2_DESC_PER_BLOCK(fs->super));
	যদি (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_first_meta_bg > fs->desc_blocks) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		যাও cleanup;
	}
	যদি (flags & EXT2_FLAG_SUPER_ONLY)
		যাও skip_read_bg;
	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	যদি (retval)
		যাও cleanup;
	যদি (!group_block)
		group_block = fs->super->s_first_তথ্য_block;
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
	যদি (group_block == 0 && fs->blocksize == 1024)
		group_zero_adjust = 1;
	dest = (অক্ষর *) fs->group_desc;
#ifdef শব্দগুলি_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
#endif
	যদি (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		first_meta_bg = fs->super->s_first_meta_bg;
		যদি (first_meta_bg > fs->desc_blocks)
			first_meta_bg = fs->desc_blocks;
	} অন্যথা
		first_meta_bg = fs->desc_blocks;
	যদি (first_meta_bg) {
		retval = io_channel_read_blk(fs->io, group_block +
					     group_zero_adjust + 1,
					     first_meta_bg, dest);
		যদি (retval)
			যাও cleanup;
#ifdef শব্দগুলি_BIGENDIAN
		gdp = (কাঠামো ext2_group_desc *) dest;
		জন্য (j=0; j < groups_per_block*first_meta_bg; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc, j);
			যদি (gdp)
				ext2fs_swap_group_desc2(fs, gdp);
		}
#endif
		dest += fs->blocksize*first_meta_bg;
	}

	জন্য (i = first_meta_bg ; i < fs->desc_blocks; i++) {
		blk = ext2fs_descriptor_block_loc2(fs, group_block, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	জন্য (i=first_meta_bg ; i < fs->desc_blocks; i++) {
		blk = ext2fs_descriptor_block_loc2(fs, group_block, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		যদি (retval)
			যাও cleanup;
#ifdef শব্দগুলি_BIGENDIAN
		জন্য (j=0; j < groups_per_block; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc,
						i * groups_per_block + j);
			যদি (gdp)
				ext2fs_swap_group_desc2(fs, gdp);
		}
#endif
		dest += fs->blocksize;
	}

	fs->stride = fs->super->s_raid_stride;

	/*
	 * If recovery is from backup superblock, Clear _UNININT flags &
	 * reset bg_itable_unused to zero
	 */
	যদি (superblock > 1 && ext2fs_has_group_desc_csum(fs)) {
		dgrp_t group;

		জন্য (group = 0; group < fs->group_desc_গুনতি; group++) {
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, group, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_group_desc_csum_set(fs, group);
		}
		যদি (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	যদি (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_start(fs);
		যদি (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			যাও cleanup;
		}
	}

	যদি (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->block_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					block_sha_map_free_entry, 4096);
		যদি (!fs->block_sha_map) {
			retval = EXT2_ET_NO_MEMORY;
			যাও cleanup;
		}
		ext2fs_set_feature_shared_blocks(fs->super);
	}

	যদি (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_load_nls_table(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	ফেরত 0;
cleanup:
	যদি (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = ফাঁকা;
	}
	*ret_fs = fs;
	ফেরত retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_তথ্য_io(ext2_filsys fs, io_channel *old_io)
{
	যদি ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		ফেরত EXT2_ET_NOT_IMAGE_FILE;
	যদি (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	ফেরত 0;
}

errcode_t ext2fs_set_তথ্য_io(ext2_filsys fs, io_channel new_io)
{
	যদি ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		ফেরত EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	ফেরত 0;
}

errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	যদি ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		ফেরত EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_set_blksize(new_io, fs->blocksize);
	যদি (err)
		ফেরত err;
	যদি ((new_io == fs->image_io) || (new_io == fs->io))
		ফেরত 0;
	যদি ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	যদি (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	ফেরত 0;
}
