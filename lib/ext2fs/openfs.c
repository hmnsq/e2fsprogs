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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#ifdef HAVE_ERRNO_H
#شامل <errno.h>
#endif

#شامل "ext2_fs.h"

#شامل "ext2fs.h"
#شامل "e2image.h"

blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs, blk64_t group_block,
				     dgrp_t i)
{
	صحیح	bg;
	صحیح	has_super = 0, group_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	اگر (i == 0 && fs->blocksize == 1024 && EXT2FS_CLUSTER_RATIO(fs) > 1)
		group_zero_adjust = 1;

	اگر (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_first_meta_bg))
		بازگشت group_block + i + 1 + group_zero_adjust;

	bg = EXT2_DESC_PER_BLOCK(fs->super) * i;
	اگر (ext2fs_bg_has_super(fs, bg))
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
	اگر (group_block != fs->super->s_first_data_block &&
	    ((ret_blk + has_super + fs->super->s_blocks_per_group) <
	     ext2fs_blocks_شمار(fs->super))) {
		ret_blk += fs->super->s_blocks_per_group;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		اگر (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		وگرنه
			has_super = 0;
	}
	بازگشت ret_blk + has_super + group_zero_adjust;
}

blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block, dgrp_t i)
{
	بازگشت ext2fs_descriptor_block_loc2(fs, group_block, i);
}

errcode_t ext2fs_open(ثابت نویسه *name, صحیح flags, صحیح superblock,
		      unsigned صحیح block_size, io_manager manager,
		      ext2_filsys *ret_fs)
{
	بازگشت ext2fs_open2(name, 0, flags, superblock, block_size,
			    manager, ret_fs);
}

ایستا تهی block_sha_map_free_entry(تهی *data)
{
	آزاد(data);
	بازگشت;
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
errcode_t ext2fs_open2(ثابت نویسه *name, ثابت نویسه *io_options,
		       صحیح flags, صحیح superblock,
		       unsigned صحیح block_size, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	unsigned long	i, first_meta_bg;
	__u32		features;
	unsigned صحیح	blocks_per_group, io_flags;
	blk64_t		group_block, blk;
	نویسه		*dest, *cp;
	صحیح		group_zero_adjust = 0;
	unsigned صحیح	inode_size;
	__u64		groups_cnt;
#ifdef واژگان_BIGENDIAN
	unsigned صحیح	groups_per_block;
	ساختار ext2_group_desc *gdp;
	صحیح		j;
#endif
	نویسه		*time_env;
	صحیح		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(اندازه(ساختار struct_ext2_filsys), &fs);
	اگر (retval)
		بازگشت retval;

	memset(fs, 0, اندازه(ساختار struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	اگر (time_env) {
		fs->now = strtoul(time_env, پوچ, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} وگرنه {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		اگر (time_env)
			fs->now = strtoul(time_env, پوچ, 0);
	}

	retval = ext2fs_get_mem(strlen(name)+1, &fs->device_name);
	اگر (retval)
		برو cleanup;
	strcpy(fs->device_name, name);
	cp = strchr(fs->device_name, '?');
	اگر (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	اگر (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	اگر (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	اگر (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	اگر (flags & EXT2_FLAG_THREADS)
		io_flags |= IO_FLAG_THREADS;
	retval = manager->open(fs->device_name, io_flags, &fs->io);
	اگر (retval)
		برو cleanup;
	اگر (io_options &&
	    (retval = io_channel_set_options(fs->io, io_options)))
		برو cleanup;
	fs->image_io = fs->io;
	fs->io->app_data = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_SIZE, &fs->super);
	اگر (retval)
		برو cleanup;
	اگر (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(اندازه(ساختار ext2_image_hdr),
					&fs->image_header);
		اگر (retval)
			برو cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(صحیح)اندازه(ساختار ext2_image_hdr),
					     fs->image_header);
		اگر (retval)
			برو cleanup;
		اگر (ext2fs_le32_to_cpu(fs->image_header->magic_عدد) != EXT2_ET_MAGIC_E2IMAGE)
			بازگشت EXT2_ET_MAGIC_E2IMAGE;
		superblock = 1;
		block_size = ext2fs_le32_to_cpu(fs->image_header->fs_blocksize);
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
	اگر (superblock) {
		اگر (!block_size) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			برو cleanup;
		}
		io_channel_set_blksize(fs->io, block_size);
		group_block = superblock;
		fs->orig_super = 0;
	} وگرنه {
		io_channel_set_blksize(fs->io, SUPERBLOCK_OFFSET);
		superblock = 1;
		group_block = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
		اگر (retval)
			برو cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_SIZE,
				     fs->super);
	اگر (retval)
		برو cleanup;
	اگر (fs->orig_super)
		memcpy(fs->orig_super, fs->super, SUPERBLOCK_SIZE);

	اگر (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		اگر (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		اگر (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			اگر (csum_retries++ < 3)
				برو retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#ifdef واژگان_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
	ext2fs_swap_super(fs->super);
#وگرنه
	اگر (fs->flags & EXT2_FLAG_SWAP_BYTES) {
		retval = EXT2_ET_UNIMPLEMENTED;
		برو cleanup;
	}
#endif

	اگر (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	اگر (retval)
		برو cleanup;

	اگر (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_بالا;
		برو cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	اگر (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#ifdef EXT2_LIB_SOFTSUPP_INCOMPAT
		اگر (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#endif
		اگر (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			برو cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#ifdef EXT2_LIB_SOFTSUPP_RO_COMPAT
		اگر (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#endif
		اگر ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			برو cleanup;
		}

		اگر (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			برو cleanup;
		}
	}

	اگر ((fs->super->s_log_block_size >
	     (unsigned) (EXT2_MAX_BLOCK_LOG_SIZE - EXT2_MIN_BLOCK_LOG_SIZE)) ||
	    (fs->super->s_log_cluster_size >
	     (unsigned) (EXT2_MAX_CLUSTER_LOG_SIZE - EXT2_MIN_CLUSTER_LOG_SIZE)) ||
	    (fs->super->s_log_block_size > fs->super->s_log_cluster_size) ||
	    (fs->super->s_log_groups_per_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		برو cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	اگر (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64BITS)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		برو cleanup;
	}

	اگر (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_block_size != fs->super->s_log_cluster_size)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		برو cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_BLOCK_SIZE(fs->super);
	inode_size = EXT2_INODE_SIZE(fs->super);
	اگر ((inode_size < EXT2_GOOD_OLD_INODE_SIZE) ||
	    (inode_size > fs->blocksize) ||
	    (inode_size & (inode_size - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		برو cleanup;
	}

	/* Enforce the block group descriptor size */
	اگر (ext2fs_has_feature_64bit(fs->super)) {
		unsigned desc_size = fs->super->s_desc_size;

		اگر (desc_size == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_size > EXT2_MAX_DESC_SIZE) ||
		      (desc_size < EXT2_MIN_DESC_SIZE_64BIT) ||
		      (desc_size & (desc_size - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_SIZE;
			برو cleanup;
		}
	}

	fs->cluster_ratio_bits = fs->super->s_log_cluster_size -
		fs->super->s_log_block_size;
	اگر (EXT2_BLOCKS_PER_GROUP(fs->super) !=
	    EXT2_CLUSTERS_PER_GROUP(fs->super) << fs->cluster_ratio_bits) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		برو cleanup;
	}
	fs->inode_blocks_per_group = ((EXT2_INODES_PER_GROUP(fs->super) *
				       EXT2_INODE_SIZE(fs->super) +
				       EXT2_BLOCK_SIZE(fs->super) - 1) /
				      EXT2_BLOCK_SIZE(fs->super));
	اگر (block_size) {
		اگر (block_size != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_BLOCK_SIZE;
			برو cleanup;
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
	اگر (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->group_desc_شمار = 0;
		*ret_fs = fs;
		بازگشت 0;
	}

	اگر (EXT2_INODES_PER_GROUP(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		برو cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_seed(fs);

	/*
	 * Read group descriptors
	 */
	blocks_per_group = EXT2_BLOCKS_PER_GROUP(fs->super);
	اگر (blocks_per_group < 8 ||
	    blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(fs->super) ||
	    fs->inode_blocks_per_group > EXT2_MAX_INODES_PER_GROUP(fs->super) ||
           EXT2_DESC_PER_BLOCK(fs->super) == 0 ||
           fs->super->s_first_data_block >= ext2fs_blocks_شمار(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		برو cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_blocks_شمار(fs->super) -
				       fs->super->s_first_data_block,
				       blocks_per_group);
	اگر (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		برو cleanup;
	}
	fs->group_desc_شمار = 	groups_cnt;
	اگر (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->group_desc_شمار * EXT2_INODES_PER_GROUP(fs->super) !=
	    fs->super->s_inodes_شمار) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		برو cleanup;
	}
	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_شمار,
					  EXT2_DESC_PER_BLOCK(fs->super));
	اگر (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_first_meta_bg > fs->desc_blocks) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		برو cleanup;
	}
	اگر (flags & EXT2_FLAG_SUPER_ONLY)
		برو skip_read_bg;
	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	اگر (retval)
		برو cleanup;
	اگر (!group_block)
		group_block = fs->super->s_first_data_block;
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
	اگر (group_block == 0 && fs->blocksize == 1024)
		group_zero_adjust = 1;
	dest = (نویسه *) fs->group_desc;
#ifdef واژگان_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
#endif
	اگر (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		first_meta_bg = fs->super->s_first_meta_bg;
		اگر (first_meta_bg > fs->desc_blocks)
			first_meta_bg = fs->desc_blocks;
	} وگرنه
		first_meta_bg = fs->desc_blocks;
	اگر (first_meta_bg) {
		retval = io_channel_read_blk(fs->io, group_block +
					     group_zero_adjust + 1,
					     first_meta_bg, dest);
		اگر (retval)
			برو cleanup;
#ifdef واژگان_BIGENDIAN
		gdp = (ساختار ext2_group_desc *) dest;
		برای (j=0; j < groups_per_block*first_meta_bg; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc, j);
			اگر (gdp)
				ext2fs_swap_group_desc2(fs, gdp);
		}
#endif
		dest += fs->blocksize*first_meta_bg;
	}

	برای (i = first_meta_bg ; i < fs->desc_blocks; i++) {
		blk = ext2fs_descriptor_block_loc2(fs, group_block, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	برای (i=first_meta_bg ; i < fs->desc_blocks; i++) {
		blk = ext2fs_descriptor_block_loc2(fs, group_block, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		اگر (retval)
			برو cleanup;
#ifdef واژگان_BIGENDIAN
		برای (j=0; j < groups_per_block; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc,
						i * groups_per_block + j);
			اگر (gdp)
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
	اگر (superblock > 1 && ext2fs_has_group_desc_csum(fs)) {
		dgrp_t group;

		برای (group = 0; group < fs->group_desc_شمار; group++) {
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, group, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_group_desc_csum_set(fs, group);
		}
		اگر (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	اگر (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_start(fs);
		اگر (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			برو cleanup;
		}
	}

	اگر (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->block_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					block_sha_map_free_entry, 4096);
		اگر (!fs->block_sha_map) {
			retval = EXT2_ET_NO_MEMORY;
			برو cleanup;
		}
		ext2fs_set_feature_shared_blocks(fs->super);
	}

	اگر (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_load_nls_table(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	بازگشت 0;
cleanup:
	اگر (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = پوچ;
	}
	*ret_fs = fs;
	بازگشت retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_data_io(ext2_filsys fs, io_channel *old_io)
{
	اگر ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		بازگشت EXT2_ET_NOT_IMAGE_FILE;
	اگر (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	بازگشت 0;
}

errcode_t ext2fs_set_data_io(ext2_filsys fs, io_channel new_io)
{
	اگر ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		بازگشت EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	بازگشت 0;
}

errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	اگر ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		بازگشت EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_set_blksize(new_io, fs->blocksize);
	اگر (err)
		بازگشت err;
	اگر ((new_io == fs->image_io) || (new_io == fs->io))
		بازگشت 0;
	اگر ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	اگر (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	بازگشت 0;
}
