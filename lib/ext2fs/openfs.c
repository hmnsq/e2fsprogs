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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#ifdef HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif

#अन्तर्भाव "ext2_fs.h"

#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "e2image.h"

blk64_t ext2fs_descriptor_block_loc2(ext2_filsys fs, blk64_t group_block,
				     dgrp_t i)
{
	पूर्णांक	bg;
	पूर्णांक	has_super = 0, group_zero_adjust = 0;
	blk64_t	ret_blk;

	/*
	 * On a bigalloc FS with 1K blocks, block 0 is reserved for non-ext4
	 * stuff, so adjust for that if we're being asked for group 0.
	 */
	जर (i == 0 && fs->blocksize == 1024 && EXT2FS_CLUSTER_RATIO(fs) > 1)
		group_zero_adjust = 1;

	जर (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_first_meta_bg))
		परत group_block + i + 1 + group_zero_adjust;

	bg = EXT2_DESC_PER_BLOCK(fs->super) * i;
	जर (ext2fs_bg_has_super(fs, bg))
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
	जर (group_block != fs->super->s_first_data_block &&
	    ((ret_blk + has_super + fs->super->s_blocks_per_group) <
	     ext2fs_blocks_मोजणी(fs->super))) {
		ret_blk += fs->super->s_blocks_per_group;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		जर (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		अन्यथा
			has_super = 0;
	}
	परत ret_blk + has_super + group_zero_adjust;
}

blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block, dgrp_t i)
{
	परत ext2fs_descriptor_block_loc2(fs, group_block, i);
}

errcode_t ext2fs_open(स्थिरांक अक्षर *name, पूर्णांक flags, पूर्णांक superblock,
		      अचिन्हित पूर्णांक block_size, io_manager manager,
		      ext2_filsys *ret_fs)
{
	परत ext2fs_open2(name, 0, flags, superblock, block_size,
			    manager, ret_fs);
}

स्थिर शून्य block_sha_map_free_entry(शून्य *data)
{
	मुक्त(data);
	परत;
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
errcode_t ext2fs_open2(स्थिरांक अक्षर *name, स्थिरांक अक्षर *io_options,
		       पूर्णांक flags, पूर्णांक superblock,
		       अचिन्हित पूर्णांक block_size, io_manager manager,
		       ext2_filsys *ret_fs)
{
	ext2_filsys	fs;
	errcode_t	retval;
	अचिन्हित दीर्घ	i, first_meta_bg;
	__u32		features;
	अचिन्हित पूर्णांक	blocks_per_group, io_flags;
	blk64_t		group_block, blk;
	अक्षर		*dest, *cp;
	पूर्णांक		group_zero_adjust = 0;
	अचिन्हित पूर्णांक	inode_size;
	__u64		groups_cnt;
#ifdef शब्द_BIGENDIAN
	अचिन्हित पूर्णांक	groups_per_block;
	संरचना ext2_group_desc *gdp;
	पूर्णांक		j;
#endif
	अक्षर		*time_env;
	पूर्णांक		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(आकार(संरचना struct_ext2_filsys), &fs);
	जर (retval)
		परत retval;

	स_रखो(fs, 0, आकार(संरचना struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	जर (time_env) {
		fs->now = म_से_अदीर्घ(time_env, रिक्त, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} अन्यथा {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		जर (time_env)
			fs->now = म_से_अदीर्घ(time_env, रिक्त, 0);
	}

	retval = ext2fs_get_mem(म_माप(name)+1, &fs->device_name);
	जर (retval)
		जा cleanup;
	म_नकल(fs->device_name, name);
	cp = म_अक्षर(fs->device_name, '?');
	जर (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	जर (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	जर (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	जर (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	जर (flags & EXT2_FLAG_THREADS)
		io_flags |= IO_FLAG_THREADS;
	retval = manager->खोलो(fs->device_name, io_flags, &fs->io);
	जर (retval)
		जा cleanup;
	जर (io_options &&
	    (retval = io_channel_set_options(fs->io, io_options)))
		जा cleanup;
	fs->image_io = fs->io;
	fs->io->app_data = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_SIZE, &fs->super);
	जर (retval)
		जा cleanup;
	जर (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(आकार(संरचना ext2_image_hdr),
					&fs->image_header);
		जर (retval)
			जा cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(पूर्णांक)आकार(संरचना ext2_image_hdr),
					     fs->image_header);
		जर (retval)
			जा cleanup;
		जर (ext2fs_le32_to_cpu(fs->image_header->magic_संख्या) != EXT2_ET_MAGIC_E2IMAGE)
			परत EXT2_ET_MAGIC_E2IMAGE;
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
	जर (superblock) {
		जर (!block_size) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			जा cleanup;
		}
		io_channel_set_blksize(fs->io, block_size);
		group_block = superblock;
		fs->orig_super = 0;
	} अन्यथा {
		io_channel_set_blksize(fs->io, SUPERBLOCK_OFFSET);
		superblock = 1;
		group_block = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &fs->orig_super);
		जर (retval)
			जा cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_SIZE,
				     fs->super);
	जर (retval)
		जा cleanup;
	जर (fs->orig_super)
		स_नकल(fs->orig_super, fs->super, SUPERBLOCK_SIZE);

	जर (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		जर (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		जर (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			जर (csum_retries++ < 3)
				जा retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#ifdef शब्द_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
	ext2fs_swap_super(fs->super);
#अन्यथा
	जर (fs->flags & EXT2_FLAG_SWAP_BYTES) {
		retval = EXT2_ET_UNIMPLEMENTED;
		जा cleanup;
	}
#endif

	जर (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	जर (retval)
		जा cleanup;

	जर (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_वरचा;
		जा cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	जर (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#ifdef EXT2_LIB_SOFTSUPP_INCOMPAT
		जर (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#endif
		जर (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			जा cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#ifdef EXT2_LIB_SOFTSUPP_RO_COMPAT
		जर (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#endif
		जर ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			जा cleanup;
		}

		जर (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			जा cleanup;
		}
	}

	जर ((fs->super->s_log_block_size >
	     (अचिन्हित) (EXT2_MAX_BLOCK_LOG_SIZE - EXT2_MIN_BLOCK_LOG_SIZE)) ||
	    (fs->super->s_log_cluster_size >
	     (अचिन्हित) (EXT2_MAX_CLUSTER_LOG_SIZE - EXT2_MIN_CLUSTER_LOG_SIZE)) ||
	    (fs->super->s_log_block_size > fs->super->s_log_cluster_size) ||
	    (fs->super->s_log_groups_per_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जा cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	जर (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64BITS)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		जा cleanup;
	}

	जर (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_block_size != fs->super->s_log_cluster_size)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जा cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_BLOCK_SIZE(fs->super);
	inode_size = EXT2_INODE_SIZE(fs->super);
	जर ((inode_size < EXT2_GOOD_OLD_INODE_SIZE) ||
	    (inode_size > fs->blocksize) ||
	    (inode_size & (inode_size - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जा cleanup;
	}

	/* Enforce the block group descriptor size */
	जर (ext2fs_has_feature_64bit(fs->super)) {
		अचिन्हित desc_size = fs->super->s_desc_size;

		जर (desc_size == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_size > EXT2_MAX_DESC_SIZE) ||
		      (desc_size < EXT2_MIN_DESC_SIZE_64BIT) ||
		      (desc_size & (desc_size - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_SIZE;
			जा cleanup;
		}
	}

	fs->cluster_ratio_bits = fs->super->s_log_cluster_size -
		fs->super->s_log_block_size;
	जर (EXT2_BLOCKS_PER_GROUP(fs->super) !=
	    EXT2_CLUSTERS_PER_GROUP(fs->super) << fs->cluster_ratio_bits) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जा cleanup;
	}
	fs->inode_blocks_per_group = ((EXT2_INODES_PER_GROUP(fs->super) *
				       EXT2_INODE_SIZE(fs->super) +
				       EXT2_BLOCK_SIZE(fs->super) - 1) /
				      EXT2_BLOCK_SIZE(fs->super));
	जर (block_size) {
		जर (block_size != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_BLOCK_SIZE;
			जा cleanup;
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
	जर (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->group_desc_मोजणी = 0;
		*ret_fs = fs;
		परत 0;
	}

	जर (EXT2_INODES_PER_GROUP(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जा cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_seed(fs);

	/*
	 * Read group descriptors
	 */
	blocks_per_group = EXT2_BLOCKS_PER_GROUP(fs->super);
	जर (blocks_per_group < 8 ||
	    blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(fs->super) ||
	    fs->inode_blocks_per_group > EXT2_MAX_INODES_PER_GROUP(fs->super) ||
           EXT2_DESC_PER_BLOCK(fs->super) == 0 ||
           fs->super->s_first_data_block >= ext2fs_blocks_मोजणी(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जा cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_blocks_मोजणी(fs->super) -
				       fs->super->s_first_data_block,
				       blocks_per_group);
	जर (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जा cleanup;
	}
	fs->group_desc_मोजणी = 	groups_cnt;
	जर (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->group_desc_मोजणी * EXT2_INODES_PER_GROUP(fs->super) !=
	    fs->super->s_inodes_मोजणी) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जा cleanup;
	}
	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_मोजणी,
					  EXT2_DESC_PER_BLOCK(fs->super));
	जर (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_first_meta_bg > fs->desc_blocks) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जा cleanup;
	}
	जर (flags & EXT2_FLAG_SUPER_ONLY)
		जा skip_read_bg;
	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	जर (retval)
		जा cleanup;
	जर (!group_block)
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
	जर (group_block == 0 && fs->blocksize == 1024)
		group_zero_adjust = 1;
	dest = (अक्षर *) fs->group_desc;
#ifdef शब्द_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
#endif
	जर (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		first_meta_bg = fs->super->s_first_meta_bg;
		जर (first_meta_bg > fs->desc_blocks)
			first_meta_bg = fs->desc_blocks;
	} अन्यथा
		first_meta_bg = fs->desc_blocks;
	जर (first_meta_bg) {
		retval = io_channel_read_blk(fs->io, group_block +
					     group_zero_adjust + 1,
					     first_meta_bg, dest);
		जर (retval)
			जा cleanup;
#ifdef शब्द_BIGENDIAN
		gdp = (संरचना ext2_group_desc *) dest;
		साठी (j=0; j < groups_per_block*first_meta_bg; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc, j);
			जर (gdp)
				ext2fs_swap_group_desc2(fs, gdp);
		}
#endif
		dest += fs->blocksize*first_meta_bg;
	}

	साठी (i = first_meta_bg ; i < fs->desc_blocks; i++) {
		blk = ext2fs_descriptor_block_loc2(fs, group_block, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	साठी (i=first_meta_bg ; i < fs->desc_blocks; i++) {
		blk = ext2fs_descriptor_block_loc2(fs, group_block, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		जर (retval)
			जा cleanup;
#ifdef शब्द_BIGENDIAN
		साठी (j=0; j < groups_per_block; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc,
						i * groups_per_block + j);
			जर (gdp)
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
	जर (superblock > 1 && ext2fs_has_group_desc_csum(fs)) {
		dgrp_t group;

		साठी (group = 0; group < fs->group_desc_मोजणी; group++) {
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, group, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_group_desc_csum_set(fs, group);
		}
		जर (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	जर (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_start(fs);
		जर (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			जा cleanup;
		}
	}

	जर (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->block_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					block_sha_map_free_entry, 4096);
		जर (!fs->block_sha_map) {
			retval = EXT2_ET_NO_MEMORY;
			जा cleanup;
		}
		ext2fs_set_feature_shared_blocks(fs->super);
	}

	जर (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_load_nls_table(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	परत 0;
cleanup:
	जर (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = रिक्त;
	}
	*ret_fs = fs;
	परत retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_data_io(ext2_filsys fs, io_channel *old_io)
{
	जर ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		परत EXT2_ET_NOT_IMAGE_FILE;
	जर (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	परत 0;
}

errcode_t ext2fs_set_data_io(ext2_filsys fs, io_channel new_io)
{
	जर ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		परत EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	परत 0;
}

errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	जर ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		परत EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_set_blksize(new_io, fs->blocksize);
	जर (err)
		परत err;
	जर ((new_io == fs->image_io) || (new_io == fs->io))
		परत 0;
	जर ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	जर (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	परत 0;
}
