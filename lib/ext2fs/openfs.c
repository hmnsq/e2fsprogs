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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"

#समावेश "ext2fs.h"
#समावेश "e2image.h"

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
	अगर (i == 0 && fs->blocksize == 1024 && EXT2FS_CLUSTER_RATIO(fs) > 1)
		group_zero_adjust = 1;

	अगर (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (i < fs->super->s_first_meta_bg))
		वापस group_block + i + 1 + group_zero_adjust;

	bg = EXT2_DESC_PER_BLOCK(fs->super) * i;
	अगर (ext2fs_bg_has_super(fs, bg))
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
	अगर (group_block != fs->super->s_first_तथ्य_block &&
	    ((ret_blk + has_super + fs->super->s_blocks_per_group) <
	     ext2fs_blocks_गिनती(fs->super))) {
		ret_blk += fs->super->s_blocks_per_group;

		/*
		 * If we're going to jump forward a block group, make sure
		 * that we adjust has_super to account for the next group's
		 * backup superblock (or lack thereof).
		 */
		अगर (ext2fs_bg_has_super(fs, bg + 1))
			has_super = 1;
		अन्यथा
			has_super = 0;
	}
	वापस ret_blk + has_super + group_zero_adjust;
}

blk_t ext2fs_descriptor_block_loc(ext2_filsys fs, blk_t group_block, dgrp_t i)
{
	वापस ext2fs_descriptor_block_loc2(fs, group_block, i);
}

errcode_t ext2fs_open(स्थिर अक्षर *name, पूर्णांक flags, पूर्णांक superblock,
		      अचिन्हित पूर्णांक block_आकार, io_manager manager,
		      ext2_filsys *ret_fs)
{
	वापस ext2fs_open2(name, 0, flags, superblock, block_आकार,
			    manager, ret_fs);
}

अटल व्योम block_sha_map_free_entry(व्योम *तथ्य)
{
	मुक्त(तथ्य);
	वापस;
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
errcode_t ext2fs_open2(स्थिर अक्षर *name, स्थिर अक्षर *io_options,
		       पूर्णांक flags, पूर्णांक superblock,
		       अचिन्हित पूर्णांक block_आकार, io_manager manager,
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
	अचिन्हित पूर्णांक	inode_आकार;
	__u64		groups_cnt;
#अगर_घोषित शब्दगण_BIGENDIAN
	अचिन्हित पूर्णांक	groups_per_block;
	काष्ठा ext2_group_desc *gdp;
	पूर्णांक		j;
#पूर्ण_अगर
	अक्षर		*time_env;
	पूर्णांक		csum_retries = 0;

	EXT2_CHECK_MAGIC(manager, EXT2_ET_MAGIC_IO_MANAGER);

	retval = ext2fs_get_mem(माप(काष्ठा struct_ext2_filsys), &fs);
	अगर (retval)
		वापस retval;

	स_रखो(fs, 0, माप(काष्ठा struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->flags = flags;
	/* don't overwrite sb backups unless flag is explicitly cleared */
	fs->flags |= EXT2_FLAG_MASTER_SB_ONLY;
	fs->umask = 022;

	time_env = ext2fs_safe_getenv("SOURCE_DATE_EPOCH");
	अगर (time_env) {
		fs->now = म_से_अदीर्घ(time_env, शून्य, 0);
		fs->flags2 |= EXT2_FLAG2_USE_FAKE_TIME;
	} अन्यथा {
		time_env = ext2fs_safe_getenv("E2FSPROGS_FAKE_TIME");
		अगर (time_env)
			fs->now = म_से_अदीर्घ(time_env, शून्य, 0);
	}

	retval = ext2fs_get_mem(म_माप(name)+1, &fs->device_name);
	अगर (retval)
		जाओ cleanup;
	म_नकल(fs->device_name, name);
	cp = म_अक्षर(fs->device_name, '?');
	अगर (!io_options && cp) {
		*cp++ = 0;
		io_options = cp;
	}

	io_flags = 0;
	अगर (flags & EXT2_FLAG_RW)
		io_flags |= IO_FLAG_RW;
	अगर (flags & EXT2_FLAG_EXCLUSIVE)
		io_flags |= IO_FLAG_EXCLUSIVE;
	अगर (flags & EXT2_FLAG_DIRECT_IO)
		io_flags |= IO_FLAG_DIRECT_IO;
	अगर (flags & EXT2_FLAG_THREADS)
		io_flags |= IO_FLAG_THREADS;
	retval = manager->खोलो(fs->device_name, io_flags, &fs->io);
	अगर (retval)
		जाओ cleanup;
	अगर (io_options &&
	    (retval = io_channel_set_options(fs->io, io_options)))
		जाओ cleanup;
	fs->image_io = fs->io;
	fs->io->app_तथ्य = fs;
	retval = io_channel_alloc_buf(fs->io, -SUPERBLOCK_आकार, &fs->super);
	अगर (retval)
		जाओ cleanup;
	अगर (flags & EXT2_FLAG_IMAGE_FILE) {
		retval = ext2fs_get_mem(माप(काष्ठा ext2_image_hdr),
					&fs->image_header);
		अगर (retval)
			जाओ cleanup;
		retval = io_channel_read_blk(fs->io, 0,
					     -(पूर्णांक)माप(काष्ठा ext2_image_hdr),
					     fs->image_header);
		अगर (retval)
			जाओ cleanup;
		अगर (ext2fs_le32_to_cpu(fs->image_header->magic_संख्या) != EXT2_ET_MAGIC_E2IMAGE)
			वापस EXT2_ET_MAGIC_E2IMAGE;
		superblock = 1;
		block_आकार = ext2fs_le32_to_cpu(fs->image_header->fs_blocksize);
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
	अगर (superblock) {
		अगर (!block_आकार) {
			retval = EXT2_ET_INVALID_ARGUMENT;
			जाओ cleanup;
		}
		io_channel_set_blksize(fs->io, block_आकार);
		group_block = superblock;
		fs->orig_super = 0;
	} अन्यथा {
		io_channel_set_blksize(fs->io, SUPERBLOCK_OFFSET);
		superblock = 1;
		group_block = 0;
		retval = ext2fs_get_mem(SUPERBLOCK_आकार, &fs->orig_super);
		अगर (retval)
			जाओ cleanup;
	}
retry:
	retval = io_channel_read_blk(fs->io, superblock, -SUPERBLOCK_आकार,
				     fs->super);
	अगर (retval)
		जाओ cleanup;
	अगर (fs->orig_super)
		स_नकल(fs->orig_super, fs->super, SUPERBLOCK_आकार);

	अगर (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)) {
		retval = 0;
		अगर (!ext2fs_verify_csum_type(fs, fs->super))
			retval = EXT2_ET_UNKNOWN_CSUM;
		अगर (!ext2fs_superblock_csum_verify(fs, fs->super)) {
			अगर (csum_retries++ < 3)
				जाओ retry;
			retval = EXT2_ET_SB_CSUM_INVALID;
		}
	}

#अगर_घोषित शब्दगण_BIGENDIAN
	fs->flags |= EXT2_FLAG_SWAP_BYTES;
	ext2fs_swap_super(fs->super);
#अन्यथा
	अगर (fs->flags & EXT2_FLAG_SWAP_BYTES) {
		retval = EXT2_ET_UNIMPLEMENTED;
		जाओ cleanup;
	}
#पूर्ण_अगर

	अगर (fs->super->s_magic != EXT2_SUPER_MAGIC)
		retval = EXT2_ET_BAD_MAGIC;
	अगर (retval)
		जाओ cleanup;

	अगर (fs->super->s_rev_level > EXT2_LIB_CURRENT_REV) {
		retval = EXT2_ET_REV_TOO_ऊपरी;
		जाओ cleanup;
	}

	/*
	 * Check for feature set incompatibility
	 */
	अगर (!(flags & EXT2_FLAG_FORCE)) {
		features = fs->super->s_feature_incompat;
#अगर_घोषित EXT2_LIB_SOFTSUPP_INCOMPAT
		अगर (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_INCOMPAT;
#पूर्ण_अगर
		अगर (features & ~EXT2_LIB_FEATURE_INCOMPAT_SUPP) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			जाओ cleanup;
		}

		features = fs->super->s_feature_ro_compat;
#अगर_घोषित EXT2_LIB_SOFTSUPP_RO_COMPAT
		अगर (flags & EXT2_FLAG_SOFTSUPP_FEATURES)
			features &= ~EXT2_LIB_SOFTSUPP_RO_COMPAT;
#पूर्ण_अगर
		अगर ((flags & EXT2_FLAG_RW) &&
		    (features & ~EXT2_LIB_FEATURE_RO_COMPAT_SUPP)) {
			retval = EXT2_ET_RO_UNSUPP_FEATURE;
			जाओ cleanup;
		}

		अगर (!(flags & EXT2_FLAG_JOURNAL_DEV_OK) &&
		    ext2fs_has_feature_journal_dev(fs->super)) {
			retval = EXT2_ET_UNSUPP_FEATURE;
			जाओ cleanup;
		}
	}

	अगर ((fs->super->s_log_block_आकार >
	     (अचिन्हित) (EXT2_MAX_BLOCK_LOG_आकार - EXT2_MIN_BLOCK_LOG_आकार)) ||
	    (fs->super->s_log_cluster_आकार >
	     (अचिन्हित) (EXT2_MAX_CLUSTER_LOG_आकार - EXT2_MIN_CLUSTER_LOG_आकार)) ||
	    (fs->super->s_log_block_आकार > fs->super->s_log_cluster_आकार) ||
	    (fs->super->s_log_groups_per_flex > 31)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जाओ cleanup;
	}

	/*
	 * bigalloc requires cluster-aware bitfield operations, which at the
	 * moment means we need EXT2_FLAG_64BITS.
	 */
	अगर (ext2fs_has_feature_bigalloc(fs->super) &&
	    !(flags & EXT2_FLAG_64BITS)) {
		retval = EXT2_ET_CANT_USE_LEGACY_BITMAPS;
		जाओ cleanup;
	}

	अगर (!ext2fs_has_feature_bigalloc(fs->super) &&
	    (fs->super->s_log_block_आकार != fs->super->s_log_cluster_आकार)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जाओ cleanup;
	}
	fs->fragsize = fs->blocksize = EXT2_BLOCK_आकार(fs->super);
	inode_आकार = EXT2_INODE_आकार(fs->super);
	अगर ((inode_आकार < EXT2_GOOD_OLD_INODE_आकार) ||
	    (inode_आकार > fs->blocksize) ||
	    (inode_आकार & (inode_आकार - 1))) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जाओ cleanup;
	}

	/* Enforce the block group descriptor size */
	अगर (ext2fs_has_feature_64bit(fs->super)) {
		अचिन्हित desc_आकार = fs->super->s_desc_आकार;

		अगर (desc_आकार == 0 ||
		    (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
		     ((desc_आकार > EXT2_MAX_DESC_आकार) ||
		      (desc_आकार < EXT2_MIN_DESC_आकार_64_BIT) ||
		      (desc_आकार & (desc_आकार - 1)) != 0))) {
			retval = EXT2_ET_BAD_DESC_आकार;
			जाओ cleanup;
		}
	}

	fs->cluster_ratio_bits = fs->super->s_log_cluster_आकार -
		fs->super->s_log_block_आकार;
	अगर (EXT2_BLOCKS_PER_GROUP(fs->super) !=
	    EXT2_CLUSTERS_PER_GROUP(fs->super) << fs->cluster_ratio_bits) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जाओ cleanup;
	}
	fs->inode_blocks_per_group = ((EXT2_INODES_PER_GROUP(fs->super) *
				       EXT2_INODE_आकार(fs->super) +
				       EXT2_BLOCK_आकार(fs->super) - 1) /
				      EXT2_BLOCK_आकार(fs->super));
	अगर (block_आकार) {
		अगर (block_आकार != fs->blocksize) {
			retval = EXT2_ET_UNEXPECTED_BLOCK_आकार;
			जाओ cleanup;
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
	अगर (ext2fs_has_feature_journal_dev(fs->super)) {
		fs->group_desc_गिनती = 0;
		*ret_fs = fs;
		वापस 0;
	}

	अगर (EXT2_INODES_PER_GROUP(fs->super) == 0) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जाओ cleanup;
	}
	/* Precompute the FS UUID to seed other checksums */
	ext2fs_init_csum_seed(fs);

	/*
	 * Read group descriptors
	 */
	blocks_per_group = EXT2_BLOCKS_PER_GROUP(fs->super);
	अगर (blocks_per_group < 8 ||
	    blocks_per_group > EXT2_MAX_BLOCKS_PER_GROUP(fs->super) ||
	    fs->inode_blocks_per_group > EXT2_MAX_INODES_PER_GROUP(fs->super) ||
           EXT2_DESC_PER_BLOCK(fs->super) == 0 ||
           fs->super->s_first_तथ्य_block >= ext2fs_blocks_गिनती(fs->super)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जाओ cleanup;
	}
	groups_cnt = ext2fs_div64_ceil(ext2fs_blocks_गिनती(fs->super) -
				       fs->super->s_first_तथ्य_block,
				       blocks_per_group);
	अगर (groups_cnt >> 32) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जाओ cleanup;
	}
	fs->group_desc_गिनती = 	groups_cnt;
	अगर (!(flags & EXT2_FLAG_IGNORE_SB_ERRORS) &&
	    (__u64)fs->group_desc_गिनती * EXT2_INODES_PER_GROUP(fs->super) !=
	    fs->super->s_inodes_गिनती) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जाओ cleanup;
	}
	fs->desc_blocks = ext2fs_div_ceil(fs->group_desc_गिनती,
					  EXT2_DESC_PER_BLOCK(fs->super));
	अगर (ext2fs_has_feature_meta_bg(fs->super) &&
	    (fs->super->s_first_meta_bg > fs->desc_blocks) &&
	    !(flags & EXT2_FLAG_IGNORE_SB_ERRORS)) {
		retval = EXT2_ET_CORRUPT_SUPERBLOCK;
		जाओ cleanup;
	}
	अगर (flags & EXT2_FLAG_SUPER_ONLY)
		जाओ skip_read_bg;
	retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
				&fs->group_desc);
	अगर (retval)
		जाओ cleanup;
	अगर (!group_block)
		group_block = fs->super->s_first_तथ्य_block;
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
	अगर (group_block == 0 && fs->blocksize == 1024)
		group_zero_adjust = 1;
	dest = (अक्षर *) fs->group_desc;
#अगर_घोषित शब्दगण_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
#पूर्ण_अगर
	अगर (ext2fs_has_feature_meta_bg(fs->super) &&
	    !(flags & EXT2_FLAG_IMAGE_FILE)) {
		first_meta_bg = fs->super->s_first_meta_bg;
		अगर (first_meta_bg > fs->desc_blocks)
			first_meta_bg = fs->desc_blocks;
	} अन्यथा
		first_meta_bg = fs->desc_blocks;
	अगर (first_meta_bg) {
		retval = io_channel_read_blk(fs->io, group_block +
					     group_zero_adjust + 1,
					     first_meta_bg, dest);
		अगर (retval)
			जाओ cleanup;
#अगर_घोषित शब्दगण_BIGENDIAN
		gdp = (काष्ठा ext2_group_desc *) dest;
		क्रम (j=0; j < groups_per_block*first_meta_bg; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc, j);
			अगर (gdp)
				ext2fs_swap_group_desc2(fs, gdp);
		}
#पूर्ण_अगर
		dest += fs->blocksize*first_meta_bg;
	}

	क्रम (i = first_meta_bg ; i < fs->desc_blocks; i++) {
		blk = ext2fs_descriptor_block_loc2(fs, group_block, i);
		io_channel_cache_readahead(fs->io, blk, 1);
	}

	क्रम (i=first_meta_bg ; i < fs->desc_blocks; i++) {
		blk = ext2fs_descriptor_block_loc2(fs, group_block, i);
		retval = io_channel_read_blk64(fs->io, blk, 1, dest);
		अगर (retval)
			जाओ cleanup;
#अगर_घोषित शब्दगण_BIGENDIAN
		क्रम (j=0; j < groups_per_block; j++) {
			gdp = ext2fs_group_desc(fs, fs->group_desc,
						i * groups_per_block + j);
			अगर (gdp)
				ext2fs_swap_group_desc2(fs, gdp);
		}
#पूर्ण_अगर
		dest += fs->blocksize;
	}

	fs->stride = fs->super->s_raid_stride;

	/*
	 * If recovery is from backup superblock, Clear _UNININT flags &
	 * reset bg_itable_unused to zero
	 */
	अगर (superblock > 1 && ext2fs_has_group_desc_csum(fs)) {
		dgrp_t group;

		क्रम (group = 0; group < fs->group_desc_गिनती; group++) {
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_BLOCK_UNINIT);
			ext2fs_bg_flags_clear(fs, group, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, group, 0);
			/* The checksum will be reset later, but fix it here
			 * anyway to avoid printing a lot of spurious errors. */
			ext2fs_group_desc_csum_set(fs, group);
		}
		अगर (fs->flags & EXT2_FLAG_RW)
			ext2fs_mark_super_dirty(fs);
	}
skip_read_bg:
	अगर (ext2fs_has_feature_mmp(fs->super) &&
	    !(flags & EXT2_FLAG_SKIP_MMP) &&
	    (flags & (EXT2_FLAG_RW | EXT2_FLAG_EXCLUSIVE))) {
		retval = ext2fs_mmp_start(fs);
		अगर (retval) {
			fs->flags |= EXT2_FLAG_SKIP_MMP; /* just do cleanup */
			ext2fs_mmp_stop(fs);
			जाओ cleanup;
		}
	}

	अगर (fs->flags & EXT2_FLAG_SHARE_DUP) {
		fs->block_sha_map = ext2fs_hashmap_create(ext2fs_djb2_hash,
					block_sha_map_free_entry, 4096);
		अगर (!fs->block_sha_map) {
			retval = EXT2_ET_NO_MEMORY;
			जाओ cleanup;
		}
		ext2fs_set_feature_shared_blocks(fs->super);
	}

	अगर (ext2fs_has_feature_casefold(fs->super))
		fs->encoding = ext2fs_load_nls_table(fs->super->s_encoding);

	fs->flags &= ~EXT2_FLAG_NOFREE_ON_ERROR;
	*ret_fs = fs;

	वापस 0;
cleanup:
	अगर (!(flags & EXT2_FLAG_NOFREE_ON_ERROR)) {
		ext2fs_free(fs);
		fs = शून्य;
	}
	*ret_fs = fs;
	वापस retval;
}

/*
 * Set/get the filesystem data I/O channel.
 *
 * These functions are only valid if EXT2_FLAG_IMAGE_FILE is true.
 */
errcode_t ext2fs_get_तथ्य_io(ext2_filsys fs, io_channel *old_io)
{
	अगर ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		वापस EXT2_ET_NOT_IMAGE_FILE;
	अगर (old_io) {
		*old_io = (fs->image_io == fs->io) ? 0 : fs->io;
	}
	वापस 0;
}

errcode_t ext2fs_set_तथ्य_io(ext2_filsys fs, io_channel new_io)
{
	अगर ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		वापस EXT2_ET_NOT_IMAGE_FILE;
	fs->io = new_io ? new_io : fs->image_io;
	वापस 0;
}

errcode_t ext2fs_rewrite_to_io(ext2_filsys fs, io_channel new_io)
{
	errcode_t err;

	अगर ((fs->flags & EXT2_FLAG_IMAGE_FILE) == 0)
		वापस EXT2_ET_NOT_IMAGE_FILE;
	err = io_channel_set_blksize(new_io, fs->blocksize);
	अगर (err)
		वापस err;
	अगर ((new_io == fs->image_io) || (new_io == fs->io))
		वापस 0;
	अगर ((fs->image_io != fs->io) &&
	    fs->image_io)
		io_channel_close(fs->image_io);
	अगर (fs->io)
		io_channel_close(fs->io);
	fs->io = fs->image_io = new_io;
	fs->flags |= EXT2_FLAG_DIRTY | EXT2_FLAG_RW |
		EXT2_FLAG_BB_DIRTY | EXT2_FLAG_IB_DIRTY;
	fs->flags &= ~EXT2_FLAG_IMAGE_FILE;
	वापस 0;
}
