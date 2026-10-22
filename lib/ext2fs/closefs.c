/*
 * closefs.c --- close an ext2 filesystem
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#include "config.h"
#include <stdio.h>
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <time.h>
#include <string.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

static ціле test_root(unsigned ціле a, unsigned ціле b)
{
	поки (1) {
		якщо (a < b)
			повернути 0;
		якщо (a == b)
			повернути 1;
		якщо (a % b)
			повернути 0;
		a = a / b;
	}
}

ціле ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group)
{
	якщо (group == 0)
		повернути 1;
	якщо (ext2fs_has_feature_sparse_super2(fs->super)) {
		якщо (group == fs->super->s_backup_bgs[0] ||
		    group == fs->super->s_backup_bgs[1])
			повернути 1;
		повернути 0;
	}
	якщо ((group <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		повернути 1;
	якщо (!(group & 1))
		повернути 0;
	якщо (test_root(group, 3) || (test_root(group, 5)) ||
	    test_root(group, 7))
		повернути 1;

	повернути 0;
}

/*
 * ext2fs_super_and_bgd_loc2()
 * @fs:			ext2 fs pointer
 * @group		given block group
 * @ret_super_blk:	if !NULL, returns super block location
 * @ret_old_desc_blk:	if !NULL, returns location of the old block
 *			group descriptor
 * @ret_new_desc_blk:	if !NULL, returns location of meta_bg block
 *			group descriptor
 * @ret_used_blks:	if !NULL, returns number of blocks used by
 *			super block and group_descriptors.
 *
 * Returns errcode_t of 0
 */
errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
					   dgrp_t group,
					   blk64_t *ret_super_blk,
					   blk64_t *ret_old_desc_blk,
					   blk64_t *ret_new_desc_blk,
					   blk_t *ret_used_blks)
{
	blk64_t	group_block, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	unsigned ціле meta_bg, meta_bg_size;
	blk_t	numblocks = 0;
	blk64_t old_desc_blocks;
	ціле	has_super;

	group_block = ext2fs_group_first_block2(fs, group);
	якщо (group_block == 0 && fs->blocksize == 1024)
		group_block = 1; /* Deal with 1024 blocksize && bigalloc */

	якщо (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_blocks = fs->super->s_first_meta_bg;
	інакше
		old_desc_blocks =
			fs->desc_blocks + fs->super->s_reserved_gdt_blocks;

	has_super = ext2fs_bg_has_super(fs, group);

	якщо (has_super) {
		super_blk = group_block;
		numblocks++;
	}
	meta_bg_size = EXT2_DESC_PER_BLOCK(fs->super);
	meta_bg = group / meta_bg_size;

	якщо (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_first_meta_bg)) {
		якщо (has_super) {
			old_desc_blk = group_block + 1;
			numblocks += old_desc_blocks;
		}
	} інакше {
		якщо (((group % meta_bg_size) == 0) ||
		    ((group % meta_bg_size) == 1) ||
		    ((group % meta_bg_size) == (meta_bg_size-1))) {
			якщо (has_super)
				has_super = 1;
			new_desc_blk = group_block + has_super;
			numblocks++;
		}
	}

	якщо (ret_super_blk)
		*ret_super_blk = super_blk;
	якщо (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	якщо (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	якщо (ret_used_blks)
		*ret_used_blks = numblocks;

	повернути 0;
}

/*
 * This function returns the location of the superblock, block group
 * descriptors for a given block group.  It currently returns the
 * number of free blocks assuming that inode table and allocation
 * bitmaps will be in the group.  This is not necessarily the case
 * when the flex_bg feature is enabled, so callers should take care!
 * It was only really intended for use by mke2fs, and even there it's
 * not that useful.
 *
 * The ext2fs_super_and_bgd_loc2() function is 64-bit block number
 * capable and returns the number of blocks used by super block and
 * group descriptors.
 */
ціле ext2fs_super_and_bgd_loc(ext2_filsys fs,
			     dgrp_t group,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     ціле *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_used_blks;
	blk_t numblocks;
	unsigned ціле meta_bg_size;

	ext2fs_super_and_bgd_loc2(fs, group, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_used_blks);

	numblocks = ext2fs_group_blocks_count(fs, group);

	якщо (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	якщо (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	якщо (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	якщо (ret_meta_bg) {
		meta_bg_size = EXT2_DESC_PER_BLOCK(fs->super);
		*ret_meta_bg = group / meta_bg_size;
	}

	numblocks -= 2 + fs->inode_blocks_per_group + ret_used_blks;

	повернути numblocks;
}

/*
 * This function forces out the primary superblock.  We need to only
 * write out those fields which we have changed, since if the
 * filesystem is mounted, it may have changed some of the other
 * fields.
 *
 * It takes as input a superblock which has already been byte swapped
 * (if necessary).
 *
 */
static errcode_t write_primary_superblock(ext2_filsys fs,
					  struct ext2_super_block *super)
{
	__u16		*old_super, *new_super;
	ціле		check_idx, write_idx, size;
	errcode_t	retval;

	якщо (!fs->io->manager->write_byte || !fs->orig_super) {
	fallback:
		io_channel_set_blksize(fs->io, SUPERBLOCK_OFFSET);
		retval = io_channel_write_blk64(fs->io, 1, -SUPERBLOCK_SIZE,
					      super);
		io_channel_set_blksize(fs->io, fs->blocksize);
		повернути retval;
	}

	old_super = (__u16 *) fs->orig_super;
	new_super = (__u16 *) super;

	для (check_idx = 0; check_idx < SUPERBLOCK_SIZE/2; check_idx++) {
		якщо (old_super[check_idx] == new_super[check_idx])
			continue;
		write_idx = check_idx;
		для (check_idx++; check_idx < SUPERBLOCK_SIZE/2; check_idx++)
			якщо (old_super[check_idx] == new_super[check_idx])
				break;
		size = 2 * (check_idx - write_idx);
#якщо 0
		printf("Writing %d bytes starting at %d\n",
		       size, write_idx*2);
#endif
		retval = io_channel_write_byte(fs->io,
			       SUPERBLOCK_OFFSET + (2 * write_idx), size,
					       new_super + write_idx);
		якщо (retval == EXT2_ET_UNIMPLEMENTED)
			goto fallback;
		якщо (retval)
			повернути retval;
	}
	memcpy(fs->orig_super, super, SUPERBLOCK_SIZE);
	повернути 0;
}


/*
 * Updates the revision to EXT2_DYNAMIC_REV
 */
void ext2fs_update_dynamic_rev(ext2_filsys fs)
{
	struct ext2_super_block *sb = fs->super;

	якщо (sb->s_rev_level > EXT2_GOOD_OLD_REV)
		повернути;

	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_first_ino = EXT2_GOOD_OLD_FIRST_INO;
	sb->s_inode_size = EXT2_GOOD_OLD_INODE_SIZE;
	/* s_uuid is handled by e2fsck already */
	/* other fields should be left alone */
}

static errcode_t write_backup_super(ext2_filsys fs, dgrp_t group,
				    blk64_t group_block,
				    struct ext2_super_block *super_shadow)
{
	errcode_t retval;
	dgrp_t	sgrp = group;

	якщо (sgrp > ((1 << 16) - 1))
		sgrp = (1 << 16) - 1;

	super_shadow->s_block_group_nr = ext2fs_cpu_to_le16(sgrp);

	retval = ext2fs_superblock_csum_set(fs, super_shadow);
	якщо (retval)
		повернути retval;

	повернути io_channel_write_blk64(fs->io, group_block, -SUPERBLOCK_SIZE,
				    super_shadow);
}

errcode_t ext2fs_flush(ext2_filsys fs)
{
	повернути ext2fs_flush2(fs, 0);
}

errcode_t ext2fs_flush2(ext2_filsys fs, ціле flags)
{
	dgrp_t		i;
	errcode_t	retval;
	unsigned long	fs_state;
	__u32		feature_incompat;
	struct ext2_super_block *super_shadow = 0;
	struct opaque_ext2_group_desc *group_shadow = 0;
#ifdef WORDS_BIGENDIAN
	struct ext2_group_desc *gdp;
	dgrp_t		j;
#endif
	char	*group_ptr;
	blk64_t	old_desc_blocks;
	struct ext2fs_numeric_progress_struct progress;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	якщо ((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0 &&
	    !ext2fs_has_feature_journal_dev(fs->super) &&
	    fs->group_desc == NULL)
		повернути EXT2_ET_NO_GDESC;

	fs_state = fs->super->s_state;
	feature_incompat = fs->super->s_feature_incompat;

	ext2fs_set_tstamp(fs->super, s_wtime, ext2fsP_get_time(fs));
	fs->super->s_block_group_nr = 0;

	/*
	 * If the write_bitmaps() function is present, call it to
	 * flush the bitmaps.  This is done this way so that a simple
	 * program that doesn't mess with the bitmaps doesn't need to
	 * drag in the bitmaps.c code.
	 *
	 * Bitmap checksums live in the group descriptor, so the
	 * bitmaps need to be written before the descriptors.
	 */
	якщо (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		якщо (retval)
			goto errout;
	}

	/*
	 * Set the state of the FS to be non-valid.  (The state has
	 * already been backed up earlier, and will be restored after
	 * we write out the backup superblocks.)
	 */
	fs->super->s_state &= ~EXT2_VALID_FS;
	ext2fs_clear_feature_journal_needs_recovery(fs->super);

	/* Byte swap the superblock and the group descriptors if necessary */
#ifdef WORDS_BIGENDIAN
	retval = EXT2_ET_NO_MEMORY;
	retval = ext2fs_get_mem(SUPERBLOCK_SIZE, &super_shadow);
	якщо (retval)
		goto errout;
	memcpy(super_shadow, fs->super, sizeof(struct ext2_super_block));
	ext2fs_swap_super(super_shadow);

	якщо (((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0) &&
	    !ext2fs_has_feature_journal_dev(fs->super)) {
		retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
					  &group_shadow);
		якщо (retval)
			goto errout;
		memcpy(group_shadow, fs->group_desc, (size_t) fs->blocksize *
		       fs->desc_blocks);

		для (j = 0; j < fs->group_desc_count; j++) {
			gdp = ext2fs_group_desc(fs, group_shadow, j);
			ext2fs_swap_group_desc2(fs, gdp);
		}
	}
#інакше
	super_shadow = fs->super;
	group_shadow = fs->group_desc;
#endif

	/*
	 * If this is an external journal device, don't write out the
	 * block group descriptors or any of the backup superblocks
	 */
	якщо (ext2fs_has_feature_journal_dev(fs->super))
		goto write_primary_superblock_only;

	/*
	 * Write out the master group descriptors, and the backup
	 * superblocks and group descriptors.
	 */
	group_ptr = (char *) group_shadow;
	якщо (ext2fs_has_feature_meta_bg(fs->super)) {
		old_desc_blocks = fs->super->s_first_meta_bg;
		якщо (old_desc_blocks > fs->desc_blocks)
			old_desc_blocks = fs->desc_blocks;
	} інакше
		old_desc_blocks = fs->desc_blocks;

	якщо (fs->progress_ops && fs->progress_ops->init)
		(fs->progress_ops->init)(fs, &progress, NULL,
					 fs->group_desc_count);


	для (i = 0; i < fs->group_desc_count; i++) {
		blk64_t	super_blk, old_desc_blk, new_desc_blk;

		якщо (fs->progress_ops && fs->progress_ops->update)
			(fs->progress_ops->update)(fs, &progress, i);
		ext2fs_super_and_bgd_loc2(fs, i, &super_blk, &old_desc_blk,
					 &new_desc_blk, 0);

		якщо (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) &&i && super_blk) {
			retval = write_backup_super(fs, i, super_blk,
						    super_shadow);
			якщо (retval)
				goto errout;
		}
		якщо (fs->flags & EXT2_FLAG_SUPER_ONLY)
			continue;
		якщо ((old_desc_blk) &&
		    (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) || (i == 0))) {
			retval = io_channel_write_blk64(fs->io,
			      old_desc_blk, old_desc_blocks, group_ptr);
			якщо (retval)
				goto errout;
		}
		якщо (new_desc_blk) {
			ціле meta_bg = i / EXT2_DESC_PER_BLOCK(fs->super);

			retval = io_channel_write_blk64(fs->io, new_desc_blk,
				1, group_ptr + (meta_bg*fs->blocksize));
			якщо (retval)
				goto errout;
		}
	}

	якщо (fs->progress_ops && fs->progress_ops->close)
		(fs->progress_ops->close)(fs, &progress, NULL);

write_primary_superblock_only:
	/*
	 * Write out master superblock.  This has to be done
	 * separately, since it is located at a fixed location
	 * (SUPERBLOCK_OFFSET).  We flush all other pending changes
	 * out to disk first, just to avoid a race condition with an
	 * insy-tinsy window....
	 */

	fs->super->s_block_group_nr = 0;
	fs->super->s_state = fs_state;
	fs->super->s_feature_incompat = feature_incompat;
#ifdef WORDS_BIGENDIAN
	*super_shadow = *fs->super;
	ext2fs_swap_super(super_shadow);
#endif

	retval = ext2fs_superblock_csum_set(fs, super_shadow);
	якщо (retval)
		повернути retval;

	якщо (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		якщо (retval)
			goto errout;
	}
	retval = write_primary_superblock(fs, super_shadow);
	якщо (retval)
		goto errout;

	fs->flags &= ~EXT2_FLAG_DIRTY;

	якщо (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		якщо (retval)
			goto errout;
	}
errout:
	fs->super->s_state = fs_state;
#ifdef WORDS_BIGENDIAN
	якщо (super_shadow)
		ext2fs_free_mem(&super_shadow);
	якщо (group_shadow)
		ext2fs_free_mem(&group_shadow);
#endif
	повернути retval;
}

errcode_t ext2fs_close_free(ext2_filsys *fs_ptr)
{
	errcode_t ret;
	ext2_filsys fs = *fs_ptr;

	ret = ext2fs_close2(fs, 0);
	якщо (ret)
		ext2fs_free(fs);
	*fs_ptr = NULL;
	повернути ret;
}

errcode_t ext2fs_close(ext2_filsys fs)
{
	повернути ext2fs_close2(fs, 0);
}

errcode_t ext2fs_close2(ext2_filsys fs, ціле flags)
{
	errcode_t	retval;
	ціле		meta_blks;
	io_stats stats = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	якщо (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		якщо (retval)
			повернути retval;
	}
	якщо (fs->super->s_kbytes_written &&
	    fs->io->manager->get_stats)
		fs->io->manager->get_stats(fs->io, &stats);
	якщо (stats && stats->bytes_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->bytes_written >> 10;
		meta_blks = fs->desc_blocks + 1;
		якщо (!(fs->flags & EXT2_FLAG_SUPER_ONLY))
			fs->super->s_kbytes_written += meta_blks /
				(fs->blocksize / 1024);
		якщо ((fs->flags & EXT2_FLAG_DIRTY) == 0)
			fs->flags |= EXT2_FLAG_SUPER_ONLY | EXT2_FLAG_DIRTY;
	}
	якщо (fs->flags & EXT2_FLAG_DIRTY) {
		retval = ext2fs_flush2(fs, flags);
		якщо (retval)
			повернути retval;
	}

	retval = ext2fs_mmp_stop(fs);
	якщо (retval)
		повернути retval;

	ext2fs_free(fs);
	повернути 0;
}

